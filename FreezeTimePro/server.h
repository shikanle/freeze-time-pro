#pragma once

#include "common.h"
#include "server_configuration.h"
#include "server_thread.h"
#include "updater.h"

namespace FreezeTimePro {

	class IServerNoticeReceiver {
	public:
		virtual void OnNetworkStateChanged(const std::string &info, State state) = 0;
		virtual void OnClientStateChanged(ServerThreadPtr thread) = 0;
		virtual void OnClientListChanged() = 0;
	};

	typedef class Server : public IServer {
	private:
		IServerNoticeReceiver *UI_;

		int Port_;
		bool Running_;
		char *ErrorMessage_;
		std::vector<ServerThreadPtr> WorkingThreads_;
		HANDLE LoopThread_;
		SOCKET *Socket_;
		Updater Updater_;
		std::recursive_mutex Mutex_;
		ServerConfiguration Config_;
		std::string CurrentPath_;
		bool GenerateMovieWhenComplete_;

	public:
		Server(int port = SERVICE_PORT, IServerNoticeReceiver *ui = nullptr) :
			UI_(ui), Port_(port), Running_(false), ErrorMessage_(nullptr), Updater_(true),
			LoopThread_(NULL), Socket_(nullptr), Mutex_(), Config_(), CurrentPath_(),
			GenerateMovieWhenComplete_(false) {
		}

	private:
		inline void UpdateState(ServerThreadPtr update_thread = nullptr) {
			Mutex_.lock();
			UI_->OnNetworkStateChanged(Format("正在工作连接 %d 个，其中 %d 进入槽位，正在等待其他连接...",
				(int)WorkingThreads_.size(), 0), State::Succeed);
			if (update_thread != nullptr) {
				UI_->OnClientStateChanged(update_thread);
			}
			Mutex_.unlock();
		}

		static DWORD WINAPI ServiceProc(LPVOID lpThreadParameter) {
			Server &me = *(static_cast<Server *>(lpThreadParameter));

			WSADATA wsaData;
			if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
				me.ErrorMessage_ = "套接字初始化失败";
				me.UI_->OnNetworkStateChanged(me.ErrorMessage_, State::Failure);
				return 0;
			}

			SOCKET socket_server = socket(AF_INET, SOCK_STREAM, 0);
			me.Socket_ = &socket_server;

			SOCKADDR_IN addrSrv;
			addrSrv.sin_family = AF_INET;
			addrSrv.sin_port = htons(me.Port_);
			addrSrv.sin_addr.S_un.S_addr = htonl(INADDR_ANY);

			int retVal = bind(socket_server, (LPSOCKADDR)&addrSrv, sizeof(SOCKADDR_IN));
			if (retVal == SOCKET_ERROR) {
				me.ErrorMessage_ = "端口打开失败，端口是否被占用？";
				me.UI_->OnNetworkStateChanged(me.ErrorMessage_, State::Failure);
				return 0;
			}

			if (listen(socket_server, 10) == SOCKET_ERROR) {
				me.ErrorMessage_ = "无法监听端口，是否有程序阻碍网络连接？";
				me.UI_->OnNetworkStateChanged(me.ErrorMessage_, State::Failure);
				return 0;
			}

			SOCKADDR_IN addrClient;
			int len = sizeof(SOCKADDR);

			while (me.Running_) {
				me.UpdateState();
				SOCKET sockConn = accept(socket_server, (SOCKADDR *)&addrClient, &len); // block here
																						// set alive
				DWORD dwError = 0L;
				DWORD dwBytes = 0L;
				tcp_keepalive sKA_Settings = { 0 }, sReturned = { 0 };
				sKA_Settings.onoff = 1;
				sKA_Settings.keepalivetime = 5000;
				sKA_Settings.keepaliveinterval = 3000;
				BOOL bSet = TRUE;
				setsockopt(sockConn, SOL_SOCKET, SO_KEEPALIVE, (const char*)&bSet, sizeof(BOOL));
				WSAIoctl(sockConn, SIO_KEEPALIVE_VALS, &sKA_Settings,
					sizeof(sKA_Settings), &sReturned, sizeof(sReturned), &dwBytes,
					NULL, NULL);

				if (sockConn == SOCKET_ERROR) {
					me.ErrorMessage_ = "无法等待到访的连接";
					me.UI_->OnNetworkStateChanged(me.ErrorMessage_, State::Failure);
					break;
				}

				me.Mutex_.lock();
				ServerThread *p = new ServerThread(&me, sockConn);
				me.WorkingThreads_.push_back(p);
				p->Start();
				me.Mutex_.unlock();
			}

			while (true) {
				me.Mutex_.lock();
				ServerThreadPtr thread = nullptr;
				if (me.WorkingThreads_.size() > 0) {
					thread = *(me.WorkingThreads_.begin());
				}
				me.Mutex_.unlock();

				if (thread != nullptr) {
					thread->Stop();
				} else {
					break;
				}
			}

			me.UI_->OnNetworkStateChanged("已经结束服务", State::Succeed);

			closesocket(socket_server);
			me.Socket_ = nullptr;
			WSACleanup();
			CloseHandle(me.LoopThread_);
			me.LoopThread_ = NULL;
			return 0;
		}

		// return true if assigned
		virtual bool AssignPort(ServerThreadPtr thread, const std::string machine, const std::string &port) {
			Mutex_.lock();
			for (auto e = WorkingThreads_.begin(); e != WorkingThreads_.end(); e++) {
				if ((*e)->IsMachine(machine, port)) {
					Mutex_.unlock();
					return false;
				}
			}
			thread->SetPortName(port);
			thread->RestoreSlotID();
			Mutex_.unlock();
			return true;
		}

		virtual void OnClientClose(ServerThread *thread) {
			Mutex_.lock();
			for (auto e = WorkingThreads_.begin(); e != WorkingThreads_.end(); e++) {
				if (*e == thread) {
					WorkingThreads_.erase(e);
					break;
				}
			}
			Mutex_.unlock();
			UpdateState(thread);
			delete thread;
		}

		virtual void OnClientStateChanged(ServerThreadPtr thread) {
			this->UI_->OnClientStateChanged(thread);
		}

		virtual void OnClientListChanged() {
			this->UI_->OnClientListChanged();
		}

		virtual void OnSnapshotComplete(ServerThread *thread, State state) {
			if (this->GenerateMovieWhenComplete_) {
				bool all_ok = true;
				for (auto e = WorkingThreads_.begin(); e != WorkingThreads_.end(); e++) {
					if ((*e)->IsCapturing() || ((*e)->GetState() == State::Failure)) {
						all_ok = false;
						break;
					}
				}
				if (all_ok) {
					this->GenerateMovie();
				}
			}
		}


	public:
		virtual ServerConfigurationPtr GetConfiguration() {
			return &(this->Config_);
		}

	public:
		void Start() {
			if (this->Running_) {
				return;
			}
			this->Running_ = true;
			if (this->LoopThread_ == NULL) {
				this->LoopThread_ = CreateThread(NULL, 0, ServiceProc, this, 0, NULL);
				SetThreadPriority(this->LoopThread_, THREAD_PRIORITY_TIME_CRITICAL);
			}
		}

		void Join() {
			if (this->LoopThread_ != NULL) {
				WaitForSingleObject(this->LoopThread_, INFINITE);
			}
		}

		void Stop() {
			this->Running_ = false;
			closesocket(*(this->Socket_));
			Join();
		}

		void SendPing() {
			for (auto e = WorkingThreads_.begin(); e != WorkingThreads_.end(); e++) {
				(*e)->SendPing();
			}
		}

		void SendSnapshot(bool (ServerThread::*callback)(cv::Mat frame) = nullptr) {
			std::string path = this->CreateWorkspace();
			for (auto e = WorkingThreads_.begin(); e != WorkingThreads_.end(); e++) {
				(*e)->SendSnapshot(path, this->Config_.Av, this->Config_.Tv, this->Config_.ISOSpeed, callback);
			}
		}

		void SendAutoFocus() {
			for (auto e = WorkingThreads_.begin(); e != WorkingThreads_.end(); e++) {
				(*e)->SendAutoFocus();
			}
		}

		void OpenCurrentFolder() {
			if (CurrentPath_.size() != 0) {
				ShellExecute(NULL, "open", this->CurrentPath_.c_str(), NULL, NULL, SW_SHOW);
			}
		}

		std::string CreateWorkspace() {
			auto folder = Format("%d", Utility::GetTimeUniqueID());
			this->CurrentPath_ = Utility::PathCombine(Config_.GetWorkspacePath(), folder);
			CreateDirectory(this->CurrentPath_.c_str(), NULL);
			return this->CurrentPath_;
		}

		std::string EnsureWorkspace() {
			if (this->CurrentPath_.size() == 0) {
				return CreateWorkspace();
			} else {
				return this->CurrentPath_;
			}
		}

		void GenerateMovie(bool value) {
			this->GenerateMovieWhenComplete_ = value;
		}

		bool GenerateMovie() {
			if (this->CurrentPath_.empty()) {
				MessageBeep(MB_ICONEXCLAMATION);
				return false;
			}

			/*
			// get min fillrate
			double min_fillrate = 1.0;
			for (auto e = WorkingThreads_.begin(); e != WorkingThreads_.end(); e++) {
				auto t = (*e)->GetTransform();
				if (t.IsAvailable) {
					if (t.FillRate < min_fillrate) {
						min_fillrate = t.FillRate;
					}
				} else {
					MessageBeep(MB_ICONEXCLAMATION);
					return false;
				}
			}

			// transform
			for (auto e = WorkingThreads_.begin(); e != WorkingThreads_.end(); e++) {
				auto src = Utility::PathCombine(this->CurrentPath_, Format("%d.jpg", (*e)->GetSlotID()));
				if (!Utility::FileExists(src)) {
					continue;
				}
				cv::Mat frame = cv::imread(src);
				if (frame.empty()) {
					continue;
				}
				auto transform = (*e)->GetTransform();
				transform.Scaling /= min_fillrate;
				auto m = transform.GetAffineMatrix();
				cv::Mat target;
				cv::warpAffine(frame, target, m, frame.size());

				auto dest = Utility::PathCombine(this->CurrentPath_, Format("target_%d.jpg", (*e)->GetSlotID()));
				cv::imwrite(dest, target);
			}
			*/
			
			// generate movie
			DWORD cpu_info_size = 32;
			SYSTEM_LOGICAL_PROCESSOR_INFORMATION cpu_info[32];
			GetLogicalProcessorInformation(cpu_info, &cpu_info_size);

			PROCESS_INFORMATION process_info;
			STARTUPINFO startup_info;
			memset(&startup_info, 0, sizeof(startup_info));
			startup_info.cb = sizeof(STARTUPINFO);
			startup_info.lpDesktop = NULL;
			startup_info.lpTitle = NULL;
			startup_info.dwFlags = STARTF_USESHOWWINDOW;
			startup_info.wShowWindow = FALSE;
			startup_info.cbReserved2 = NULL;
			startup_info.lpReserved2 = NULL;
			std::string workpath = this->CurrentPath_;
			std::string params = Format(" -threads %d -y -r 12 -i \"%s\\%%d.jpg\" -absf aac_adtstoasc \"%s\\output.mp4\"",
				cpu_info_size, workpath.data(), workpath.data());

			if (CreateProcess(
				this->Config_.FFMpegPath.c_str(),
				(char *)params.c_str(), NULL, NULL, FALSE, CREATE_NEW_CONSOLE, NULL, (char *)workpath.c_str(),
				&startup_info, &process_info)) {

				WaitForSingleObject(process_info.hThread, 20000);
				WaitForSingleObject(process_info.hProcess, 20000);
				CloseHandle(process_info.hThread);
				CloseHandle(process_info.hProcess);
				
				MessageBeep(MB_OK);

				MoveFile(Utility::PathCombine(this->CurrentPath_, "output.mp4").c_str(),
					Utility::PathCombine(this->CurrentPath_, "output.rmvb").c_str());

				ShellExecute(NULL, "open", Utility::PathCombine(this->CurrentPath_, 
					"output.rmvb").c_str(), NULL, this->CurrentPath_.c_str(), SW_SHOWMAXIMIZED);
				return true;
			} else {
				MessageBeep(MB_ICONEXCLAMATION);
				return false;
			}
		}

		bool CheckUpdate(NetStream &stream) {
			this->Mutex_.lock();
			UpdatePackage &local = this->Updater_.GetLocalPackage();
			UpdatePackage remote;
			int count;
			if (!stream.Read(count)) {
				this->Mutex_.unlock();
				return false;
			}
			for (int i = 0; i < count; i++) {
				UpdateFile uf;
				if (!stream.ReadString(uf.FileName)) {
					this->Mutex_.unlock();
					return false;
				}
				uf.AlternativeFileName = uf.FileName;
				std::transform(uf.AlternativeFileName.begin(), uf.AlternativeFileName.end(),
					uf.AlternativeFileName.begin(), std::tolower);
				if (!stream.Read(uf.FileSize)) {
					this->Mutex_.unlock();
					return false;
				}
				if (!stream.Read((char *)uf.MD5Code, 16)) {
					this->Mutex_.unlock();
					return false;
				}
				uf.FileContent = nullptr;
				uf.Method = UpdateFile::Normal;
				remote.push_back(uf);
			}
			UpdatePackage update = local - remote;
			if (update.size() > 0) {
				int size = 4;
				for (UpdateFile &f : update) {
					size += 4 + 4 + (int)f.FileName.length() + 4;
					if ((f.Method == UpdateFile::Add) || (f.Method == UpdateFile::Update)) {
						size += f.FileSize;
					}
				}
				stream.Write((int)PackageType::Update);
				stream.Write((int)update.size());
				size -= 4;
				for (UpdateFile &f : update) {
					stream.Write((int)f.Method); // 4
					stream.Write(f.FileName);	 // 4 + length
					stream.Write(f.FileSize);	 // 4
					size -= 12 + (int)f.FileName.length();
					if ((f.Method == UpdateFile::Add) || (f.Method == UpdateFile::Update)) {
						stream.Write(f.FileContent, f.FileSize);
						size -= f.FileSize;
					}
				}
			}
			this->Mutex_.unlock();
			return true;
		}

	} *ServerPtr;

}