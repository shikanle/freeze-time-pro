#pragma once

#include "form.h"
#include "client.h"

namespace FreezeTimePro {

	class ClientForm : public Form, public IClientNoticeReceiver {
	private:
		ClientPtr Client_;

		#pragma region Form Components

		FormLabelPtr StatusLabel_;

		#pragma endregion

	public:
		ClientForm(HINSTANCE instance = NULL) : 
			Form(instance) {
			this->SetCaption("FreezeTime Pro");

			auto ip = CommandLine.GetIP();
			if (ip.length() == 0) {
				ip = "127.0.0.1";
			}
			auto port = CommandLine.GetPort();
			if (port < 0) {
				port = SERVICE_PORT;
			}
			Client_ = new Client(ip, port, this);
		}

		virtual ~ClientForm() {
			delete Client_;
		}

	public:
		virtual void OnInitializeComponents() {
			Form::OnInitializeComponents();

			this->SetBackColor(RGB(32, 32, 32));

			this->StatusLabel_ = new FormLabel();
			this->StatusLabel_->SetBackColor(RGB(255, 255, 255));
			this->StatusLabel_->SetForeColor(RGB(0, 0, 0));
			this->StatusLabel_->SetText("Hello world!");
			this->StatusLabel_->SetAlignment(TextAlignment::MiddleCenter);
			this->AddElement(this->StatusLabel_);
		}

		virtual bool OnCommand(int command) {
			return Form::OnCommand(command);
		}

		virtual bool OnDestroy() {
			Client_->Stop();
			return true;
		}

		virtual void OnResize() {
			this->StatusLabel_->SetBounds(8, 8, this->Width_ - 16, 32);
		}

		virtual void OnPaint(HDC hdc) {
			Form::OnPaint(hdc);
		}

		virtual void OnLoad() {
			Client_->Start();
		}

		virtual LRESULT WndProc(UINT message, WPARAM wParam, LPARAM lParam) {
			switch (message) {
			case WM_FREEZETIME_REGISTER:
				this->Client_->SendRegister();
				break;
			case WM_FREEZETIME_PROCESS_PACKAGE:
				return this->Client_->ProcessPackage((PackageType)wParam) ? TRUE : FALSE;
			default:
				return Form::WndProc(message, wParam, lParam);
			}
			return 0;
		}

	public:

		virtual void OnNetworkStateChanged(const std::string &info, State state) {
			this->StatusLabel_->SetText(info);
			FormUtility::UpdateLabelState(this->StatusLabel_, state);
			Invalidate();
		}

		virtual LRESULT SendInternalMessage(UINT msg, WPARAM wparam, LPARAM lparam) {
			return SendMessage(this->Handle_, msg, wparam, lparam);
		}

	};

}