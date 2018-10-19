#pragma once

#include "form.h"
#include "server.h"

namespace FreezeTimePro {

	typedef class ServerFormItem : public FormContainer {
	protected:
		ServerThreadPtr ServerThread_;

		int SlotID_;
		FormLabelPtr SlotLabel_;
		FormLabelPtr StateLabel_;
		FormLabelPtr MessageLabel_;

	public:
		ServerFormItem(ServerThreadPtr thread) : FormContainer(), ServerThread_(thread), SlotID_(-1) {
			SlotLabel_ = new FormLabel();
			SlotLabel_->SetForeColor(RGB(255, 255, 255));
			SlotLabel_->SetBackColor(RGB(64, 64, 64));
			SlotLabel_->SetAlignment(TextAlignment::MiddleCenter);
			this->AddElement(SlotLabel_);

			StateLabel_ = new FormLabel();
			StateLabel_->SetAlignment(TextAlignment::MiddleCenter);
			this->AddElement(StateLabel_);

			MessageLabel_ = new FormLabel();
			MessageLabel_->SetForeColor(RGB(230, 230, 230));
			MessageLabel_->SetBackColor(-1);
			MessageLabel_->SetAlignment(TextAlignment::MiddleLeft);
			this->AddElement(MessageLabel_);
		}

		virtual ~ServerFormItem() {
		}

	public:
		inline ServerThreadPtr GetThread() {
			return this->ServerThread_;
		}

		inline int GetSlot() const {
			return this->SlotID_;
		}
		
		inline void SetSlot(int value) {
			SlotID_ = value;
			this->SlotLabel_->SetText(Utility::ToString(value));
		}

		inline void SetState(State state, const std::string &msg) {
			FormUtility::UpdateLabelState(this->StateLabel_, state);
			this->StateLabel_->SetText(msg);
		}

		inline void SetMessage(const std::string &msg) {
			this->MessageLabel_->SetText(msg);
		}

		void Update() {
			this->SetSlot(this->ServerThread_->GetSlotID());
			this->SetState(this->ServerThread_->GetState(), this->ServerThread_->GetStateMessage());
			auto m = this->ServerThread_->GetMachineName();
			if (m.length() == 0) {
				this->SetMessage(this->ServerThread_->GetInfoMessage());
			} else {
				this->SetMessage(std::string("[") + m + "]   " + this->ServerThread_->GetInfoMessage());
			}
		}

	public:
		virtual void OnResize() {
			int inner_height = this->Height_ - 4;
			int width = inner_height;
			int left = 2;
			SlotLabel_->SetBounds(left, 2, inner_height, width);
			left += width + 2;
			width = 100;
			StateLabel_->SetBounds(left, 2, width, inner_height);
			left += width + 2;
			width = this->Width_ - 2 - left;
			MessageLabel_->SetBounds(left, 2, width, inner_height);
		}

	} *ServerFormItemPtr;

	typedef class ServerFormList : public FormContainer {
	protected:
		int SelectedItem_;
		std::vector<ServerFormItemPtr> Items_;
		std::unordered_map<ServerThreadPtr, ServerFormItemPtr> ThreadMapping_;
		std::recursive_mutex Mutex_;
	
	public:
		ServerFormItemPtr GetItem(ServerThreadPtr thread) {
			Mutex_.lock();
			auto e = ThreadMapping_.find(thread);
			if (e == ThreadMapping_.end()) {
				auto item = new ServerFormItem(thread);
				this->AddElement(item);
				Items_.push_back(item);
				ThreadMapping_.insert_or_assign(thread, item);
				this->Sort();
				Mutex_.unlock();
				return item;
			} else {
				Mutex_.unlock();
				return e->second;
			}
		}

		inline ServerFormItemPtr GetSelectedItem() {
			if ((this->SelectedItem_ >= 0) && (this->SelectedItem_ < (int)(this->Items_.size()))) {
				return this->Items_.at(this->SelectedItem_);
			} else {
				return nullptr;
			}
		}

		inline ServerThreadPtr GetSelectedThread() {
			auto item = GetSelectedItem();
			if (item == nullptr) {
				return nullptr;
			} else {
				return item->GetThread();
			}
		}

		inline bool HasSelectedItem() {
			return ((this->SelectedItem_ >= 0) && (this->SelectedItem_ < (int)(this->Items_.size())));
		}

		int InBound(int id) {
			int count = (int)Items_.size();
			if (id < 0) {
				id = 0;
			}
			if (id >= count) {
				id = count - 1;
			}
			return id;
		}

	public:
		ServerFormList() : 
			FormContainer(), Items_(), ThreadMapping_(), SelectedItem_(-1), Mutex_() {
		}

		virtual ~ServerFormList() {
		}

		void Deselect() {
			int count = (int)Items_.size();
			if ((SelectedItem_ >= 0) && (SelectedItem_ < count)) {
				this->Items_[SelectedItem_]->SetBackColor(RGB(48, 48, 48));
				SelectedItem_ = -1;
			}
		}

		void SelectItem(int id) {
			int count = (int)Items_.size();
			Deselect();
			if ((id >= 0) && (id < count)) {
				this->Items_[id]->SetBackColor(RGB(32, 64, 200));
				SelectedItem_ = id;
			}
		}

		virtual bool OnKeyDown(int keycode, int shift) {
			switch (keycode) {
			case VK_UP:
				SelectItem(InBound(SelectedItem_ - 1));
				this->Invalidate();
				return true;
			case VK_DOWN:
				SelectItem(InBound(SelectedItem_ + 1));
				this->Invalidate();
				return true;
			case VK_LEFT:
				if (auto thread = GetSelectedThread()) {
					thread->SetSlotID(thread->GetSlotID() - 1);
					GetSelectedItem()->Update();
					this->Invalidate();
					return false; // parent also handle
				} else {
					return true;
				}
			case VK_RIGHT:
				if (auto thread = GetSelectedThread()) {
					thread->SetSlotID(thread->GetSlotID() + 1);
					GetSelectedItem()->Update();
					this->Invalidate();
					return false; // parent also handle
				} else {
					return true;
				}
			case VK_RETURN:
				this->Sort();
				this->Invalidate();
				return false; // parent also handle
			case VK_ESCAPE:
				this->Deselect();
				this->Invalidate();
				return false; // parent also handle
			default:
				return false; // parent also handle
			}
		}

		virtual void OnResize() {
			int top = 0;
			int height = 18;
			for (auto e : Items_) {
				e->SetBounds(0, top, this->Width_, height);
				top += height;
			}
		}

		inline void Update(ServerThreadPtr thread) {
			Mutex_.lock();
			if (thread->IsRunning()) {
				this->GetItem(thread)->Update();
			} else {
				auto el = this->GetItem(thread);
				if (this->GetSelectedItem() == el) {
					Deselect();
				}
				auto found = std::find(this->Items_.begin(), this->Items_.end(), el);
				if (found != this->Items_.end()) {
					this->Items_.erase(found);
					this->RemoveElement(el);
					delete el;
					this->ThreadMapping_.erase(thread);
					this->OnResize();
				}
			}
			Mutex_.unlock();
		}

		void Sort() {
			Mutex_.lock();
			Deselect();
			for (int b = (int)this->Items_.size(); b > 0; b--) {
				for (int i = 1; i < b; i++) {
					if (Items_[i]->GetSlot() < Items_[i - 1]->GetSlot()) {
						auto t = Items_[i];
						Items_[i] = Items_[i - 1];
						Items_[i - 1] = t;
					}
				}
			}
			OnResize();
			Mutex_.unlock();
		}

	} *ServerFormListPtr;

	class ServerForm : public Form, public IServerNoticeReceiver {
	private:
		ServerPtr Server_;

		#pragma region Form Components

		FormLabelPtr StatusLabel_;
		ServerFormListPtr ThreadList_;
		FormLabelPtr CommandLabel_;

		#pragma endregion

	public:
		ServerForm(HINSTANCE instance = NULL) :
			Form(instance) {
			auto port = CommandLine.GetPort();
			if (port < 0) {
				port = SERVICE_PORT;
			}
			Server_ = new Server(port, this);
			UpdateCaption();
		}

		virtual ~ServerForm() {
			delete Server_;
		}

	public:
		virtual void OnInitializeComponents() {
			Form::OnInitializeComponents();

			this->SetBackColor(RGB(32, 32, 32));

			this->StatusLabel_ = new FormLabel();
			this->StatusLabel_->SetBackColor(RGB(255, 255, 255));
			this->StatusLabel_->SetForeColor(RGB(0, 0, 0));
			this->StatusLabel_->SetText("正在加载...");
			this->StatusLabel_->SetAlignment(TextAlignment::MiddleCenter);
			this->AddElement(this->StatusLabel_);

			this->ThreadList_ = new ServerFormList();
			this->ThreadList_->SetBackColor(RGB(48, 48, 48));
			this->AddElement(this->ThreadList_);

			this->CommandLabel_ = new FormLabel();
			this->CommandLabel_->SetBackColor(RGB(48, 48, 48));
			this->CommandLabel_->SetForeColor(RGB(255, 255, 255));
			this->CommandLabel_->SetText("  [F1] 测试链接    [F4] 自动对焦    [F5] 获取照片    [6] 自动配准");
			this->CommandLabel_->SetAlignment(TextAlignment::MiddleLeft);
			this->AddElement(this->CommandLabel_);
		}

		virtual bool OnCommand(int command) {
			return Form::OnCommand(command);
		}

		virtual bool OnDestroy() {
			Server_->Stop();
			return true;
		}

		virtual void OnResize() {
			this->StatusLabel_->SetBounds(8, 8, this->Width_ - 16, 32);
			this->ThreadList_->SetBounds(8, 8 + 32 + 8, this->Width_ - 16, this->Height_ - 24 - 32 - 24 - 8);
			this->CommandLabel_->SetBounds(8, this->Height_ - 24 - 8, this->Width_ - 16, 24);
		}

		virtual void OnPaint(HDC hdc) {
			Form::OnPaint(hdc);
		}

		virtual void OnLoad() {
			Server_->Start();
		}

		virtual bool OnKeyUp(int keycode, int shift) {
			this->CommandLabel_->SetBackColor(RGB(48, 48, 48));
			this->CommandLabel_->Invalidate();
			return true;
		}

		virtual bool OnKeyDown(int keycode, int shift) {
			this->CommandLabel_->SetBackColor(RGB(48, 200, 48));
			this->CommandLabel_->Invalidate();
			if (this->ThreadList_->OnKeyDown(keycode, shift)) {
				return true;
			}
			switch (keycode) {
			case VK_F1:
				if (auto thread = this->ThreadList_->GetSelectedThread()) {
					thread->SendPing();
				} else {
					Server_->SendPing();
				}
				break;
			case VK_F3:
				Server_->OpenCurrentFolder();
				break;
			case VK_F4:
				if (auto thread = this->ThreadList_->GetSelectedThread()) {
					thread->SendAutoFocus();
				} else {
					Server_->SendAutoFocus();
				}
				break;
			case VK_F5:
				if (auto thread = this->ThreadList_->GetSelectedThread()) {
					Server_->GenerateMovie(false);
					this->SendSnapshot(thread, nullptr);
				} else {
					Server_->GenerateMovie(true);
					Server_->SendSnapshot();
				}
				break;
			case VK_F6:
				Server_->GenerateMovie(false);
				if (auto thread = this->ThreadList_->GetSelectedThread()) {
					this->SendSnapshot(thread, &ServerThread::Calibrate);
				} else {
					Server_->SendSnapshot(&ServerThread::CalibrateSilently);
				}
				break;
			case VK_RETURN:
				Server_->GetConfiguration()->Save();
				UpdateCaption();
				break;
			case VK_LEFT:
			case VK_RIGHT:
				UpdateCaption();
				break;
			}
			return true;
		}

		virtual LRESULT WndProc(UINT message, WPARAM wParam, LPARAM lParam) {
			switch (message) {
			case WM_FREEZETIME_UPDATE_UI:
				this->ThreadList_->Update((ServerThreadPtr)wParam);
				this->Invalidate();
				break;
			default:
				return Form::WndProc(message, wParam, lParam);
			}
			return 0;
		}

		void UpdateCaption() {
			auto config = this->Server_->GetConfiguration();
			std::string caption = "FreezeTime Pro Server";
			if (config->IsDirty()) {
				caption += " [配置已修改 - 回车键保存]";
			}
			this->SetCaption(caption);
		}

	protected:
		bool SendSnapshot(ServerThreadPtr thread, bool (ServerThread::*callback)(cv::Mat) = nullptr) {
			return thread->SendSnapshot(Server_->EnsureWorkspace(),
				Server_->GetConfiguration()->Av,
				Server_->GetConfiguration()->Tv,
				Server_->GetConfiguration()->ISOSpeed,
				callback);
		}

	public:
		virtual void OnNetworkStateChanged(const std::string &info, State state) {
			this->StatusLabel_->SetText(info);
			FormUtility::UpdateLabelState(this->StatusLabel_, state);
			Invalidate();
		}

		virtual void OnClientStateChanged(ServerThreadPtr thread) {
			this->SendInternalMessage(WM_FREEZETIME_UPDATE_UI, (WPARAM)thread, 0);
		}

		virtual void OnClientListChanged() {
			this->ThreadList_->Sort();
			this->ThreadList_->Invalidate();
			Invalidate();
		}

		virtual LRESULT SendInternalMessage(UINT msg, WPARAM wparam, LPARAM lparam) {
			return SendMessage(this->Handle_, msg, wparam, lparam);
		}

	};

}