#pragma once

#include "common.h"
#include "form_element.h"

namespace FreezeTimePro {

	class Form : public FormContainer {
	private:
		HINSTANCE Instance_;
		std::string Caption_;
		std::string ClassName_;
		FormGraphics Graphics_;

	protected:
		HWND Handle_;
		bool Resized_;

	public:
		Form(HINSTANCE instance = NULL)
			: Instance_(instance), Handle_(NULL), Resized_(false), FormContainer() {
			static int serial_no = 0;
			Caption_ = "Form";
			ClassName_ = "__FormClass_" + Utility::ToString(++serial_no);
			RegisterClass(instance);
		}

		virtual ~Form() {
		}

	private:

		ATOM RegisterClass(HINSTANCE hInstance)
		{
			WNDCLASSEX wcex;
			wcex.cbSize = sizeof(WNDCLASSEX);
			wcex.style = CS_HREDRAW | CS_VREDRAW;
			wcex.lpfnWndProc = WndProc;
			wcex.cbClsExtra = 0;
			wcex.cbWndExtra = 0;
			wcex.hInstance = hInstance;
			wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_FREEZETIMEPRO));
			wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
			wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
			wcex.lpszMenuName = NULL; // MAKEINTRESOURCE(IDC_FREEZETIMEPRO);
			wcex.lpszClassName = ClassName_.c_str();
			wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));
			return RegisterClassEx(&wcex);
		}
		
		BOOL InitializeWindow()
		{
			OnInitializeComponents();
			Handle_ = CreateWindow(ClassName_.c_str(), this->Caption_.c_str(), WS_OVERLAPPEDWINDOW,
				CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, this->Instance_, nullptr);
			SetWindowLongPtr(Handle_, GWLP_USERDATA, (LONG)this);
			return (Handle_ != NULL);
		}

		static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
			#define me ((Form *)GetWindowLongPtr(hWnd, GWLP_USERDATA))
			switch (message) {
			case WM_COMMAND:
				if (!me->OnCommand(LOWORD(wParam))) {
					return DefWindowProc(hWnd, message, wParam, lParam);
				}
				break;
			case WM_PAINT:
				{
					PAINTSTRUCT ps;
					HDC hdc = BeginPaint(hWnd, &ps);
					me->OnPaint(hdc);
					EndPaint(hWnd, &ps);
				}
				break;
			case WM_DESTROY:
				if (me->OnDestroy()) {
					PostQuitMessage(0);
				}
				break;
			case WM_SIZE:
				me->Resized_ = true;
				break;
			case WM_KEYDOWN:
				me->OnKeyDown(wParam, lParam);
				break;
			case WM_KEYUP:
				me->OnKeyUp(wParam, lParam);
				break;
			default:
				if (me == nullptr) {
					return DefWindowProc(hWnd, message, wParam, lParam);
				} else {
					return me->WndProc(message, wParam, lParam);
				}
			}
			#undef me
			return 0;
		}

		int MessageLoop() {
			MSG msg;
			HACCEL hAccelTable = LoadAccelerators(this->Instance_, MAKEINTRESOURCE(IDC_FREEZETIMEPRO));
			while (GetMessage(&msg, nullptr, 0, 0)) {
				if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg)) {
					TranslateMessage(&msg);
					DispatchMessage(&msg);
				}
			}
			return (int)msg.wParam;
		}

	protected:
		virtual LRESULT WndProc(UINT message, WPARAM wParam, LPARAM lParam) {
			return DefWindowProc(this->Handle_, message, wParam, lParam);
		}

	public:

		virtual void OnInitializeComponents() { }

		virtual void OnLoad() { }
		
		virtual bool OnDestroy() { return true; }

		virtual bool OnCommand(int command) { return false; }

		virtual bool OnKeyDown(int keycode, int shift) { return true; }
		
		virtual bool OnKeyUp(int keycode, int shift) { return true; }

		virtual void OnPaint(HDC hdc) {
			Graphics_.Update(hdc);
			if (this->Resized_) {
				RECT r;
				GetClientRect(this->Handle_, &r);
				this->Graphics_.InverseTransform(r);
				this->SetBounds(r.left, r.top, r.right - r.left, r.bottom - r.top);
			}
			this->OnDraw(Graphics_, 0, 0);
		}

	public:
		std::string GetCaption() const {
			return this->Caption_;
		}

		void SetCaption(const std::string &value) {
			if (this->Caption_ != value) {
				this->Caption_ = value;
				if (this->Handle_ != NULL) {
					SetWindowText(this->Handle_, value.c_str());
				}
			}
		}

	public:

		int ShowModal() {
			if (Handle_ == NULL) {
				InitializeWindow();
			}
			if (Handle_ != NULL) {
				ShowWindow(Handle_, SW_SHOW);
				UpdateWindow(Handle_);
			};
			this->OnLoad();
			return MessageLoop();
		}

		virtual void Invalidate() {
			InvalidateRect(this->Handle_, NULL, FALSE);
		}
	};

	enum class State {
		Ready,
		Processing,
		Succeed,
		Warning,
		Failure
	};

	class FormUtility {
	public:
		static void UpdateLabelState(FormLabelPtr label, State state) {
			switch (state) {
			case State::Ready:
				label->SetBackColor(RGB(64, 64, 64));
				label->SetForeColor(RGB(230, 230, 230));
				break;
			case State::Processing:
				label->SetBackColor(RGB(255, 255, 100));
				label->SetForeColor(RGB(0, 0, 0));
				break;
			case State::Failure:
				label->SetBackColor(RGB(160, 0, 0));
				label->SetForeColor(RGB(255, 255, 255));
				break;
			case State::Succeed:
				label->SetBackColor(RGB(0, 140, 0));
				label->SetForeColor(RGB(255, 255, 255));
				break;
			case State::Warning:
				label->SetBackColor(RGB(255, 150, 0));
				label->SetForeColor(RGB(255, 255, 255));
				break;
			}
		}

	};

}