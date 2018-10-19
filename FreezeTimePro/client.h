#pragma once

#include "common.h"
#include "package.h"
#include "netstream.h"
#include "camera.h"
#include "updater.h"

namespace FreezeTimePro {

	class IClientNoticeReceiver {
	public:
		virtual LRESULT SendInternalMessage(UINT msg, WPARAM wparam, LPARAM lparam) = 0;

		virtual void OnNetworkStateChanged(const std::string &info, State state) = 0;
	};

	typedef class Client : public ICameraEventHandler {
	private:
		IClientNoticeReceiver *UI_;
		bool Running_;
		std::string ServerIP_;
		int ServerPort_;
		SOCKADDR_IN ServerAddress_;
		HANDLE Thread_;
		SOCKET *Socket_;
		NetStream *Stream_;

		bool Restart_;
		std::string RestartCommand_;
		std::string RestartDirectory_;
		Updater Updater_;

		CameraPtr Camera_;
		char *PackageErrorMessage_;
		int PackageErrorSleep_;

	public:
		Client(std::string ip = "", int port = SERVICE_PORT, IClientNoticeReceiver *ui = nullptr)
			: Running_(false), UI_(ui), Socket_(nullptr), Camera_(nullptr),
			  ServerIP_(ip), ServerPort_(port), Stream_(nullptr), Updater_(false),
			  Restart_(false), RestartCommand_(), RestartDirectory_(),
			  PackageErrorMessage_(nullptr), PackageErrorSleep_(0) {

			this->ServerIP_ = ip;
			this->ServerAddress_.sin_family = AF_INET;
			this->ServerAddress_.sin_port = htons(port);
			inet_pton(AF_INET, ip.data(), &this->ServerAddress_.sin_addr);
		}

		~Client() {
			if (Camera_ != nullptr) {
				delete Camera_;
			}
		}

	private:
		void CheckUpdate(NetStream &stream) {
			UpdatePackage &local = this->Updater_.GetLocalPackage();
			stream.Write((int)(PackageType::CheckUpdate));
			stream.Write((int)local.size());
			for (UpdateFile &f : local) {
				stream.Write(f.FileName); // 4 + length
				stream.Write(f.FileSize); // 4
				stream.Write((const char *)f.MD5Code, 16); // 16
			}
		}

		static DWORD WINAPI ServiceProc(LPVOID lpParameter) {

			DWORD dwError = 0L;
			DWORD dwBytes = 0L;
			tcp_keepalive sKA_Settings = { 0 }, sReturned = { 0 };
			Client &me = *((Client *)lpParameter);

			WSADATA wsaData;
			if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
				me.UI_->OnNetworkStateChanged("套接字服务程序初始化失败 (" + me.ServerIP_ + ")", State::Failure);
				goto final_exit;
			}

		reconnect:

			me.UI_->OnNetworkStateChanged("正在连接服务器 (" + me.ServerIP_ + ")...", State::Processing);

			sKA_Settings.onoff = 1;
			sKA_Settings.keepalivetime = 5000;
			sKA_Settings.keepaliveinterval = 3000;

			SOCKET sockClient = socket(AF_INET, SOCK_STREAM, 0);
			me.Socket_ = &sockClient;
			if (SOCKET_ERROR == sockClient) {
				me.UI_->OnNetworkStateChanged("套接字初始化失败 (" + me.ServerIP_ + ")", State::Failure);
				goto final_exit;
			}

			BOOL bSet = TRUE;
			setsockopt(sockClient, SOL_SOCKET, SO_KEEPALIVE, (const char*)&bSet, sizeof(BOOL));
			if (WSAIoctl(sockClient, SIO_KEEPALIVE_VALS, &sKA_Settings,
				sizeof(sKA_Settings), &sReturned, sizeof(sReturned), &dwBytes,
				NULL, NULL) != 0) {
				me.UI_->OnNetworkStateChanged("套接字参数初始化失败 (" + me.ServerIP_ + ")", State::Failure);
				goto final_exit;
			}


			int rep_con = 0;
			while (me.Running_) {
				if (connect(sockClient, (struct sockaddr *)&me.ServerAddress_, sizeof(me.ServerAddress_)) == INVALID_SOCKET) {
					me.UI_->OnNetworkStateChanged(Format("未能成功连接到指定的服务器 (%s)，尝试连接中 (%d)...", me.ServerIP_.data(), ++rep_con), State::Failure);
					if (rep_con < 4 * 10) {
						Sleep(250);
					}
					else if (rep_con < 4 * 10 + 60) {
						Sleep(1000);
					}
					else {
						Sleep(3000);
					}
				} else {
					break;
				}
			}

			if (!me.Running_) {
				goto final_exit;
			}

			me.UI_->OnNetworkStateChanged("已连接到服务器 (" + me.ServerIP_ + ")，等待服务器控制中", State::Succeed);

			{
				NetStream stream(sockClient);
				me.Stream_ = &stream;
				me.CheckUpdate(stream);
				me.UI_->SendInternalMessage(WM_FREEZETIME_REGISTER, 0, 0);
				int rec_pk_id = 0;
				while (me.Running_) {
					PackageType type;
					if (!stream.Read(type)) {
						me.UI_->OnNetworkStateChanged("通信中断或协议不一致 (" + me.ServerIP_ + ")...", State::Failure);
						Sleep(5000);
						break; // error
					}

					rec_pk_id++;
					char rec_pk_id_buf[16];
					_itoa_s(rec_pk_id, rec_pk_id_buf, 15, 10);
					me.UI_->OnNetworkStateChanged("已经连接到服务器 (" + me.ServerIP_ + ")，正在处理指令 (" + rec_pk_id_buf + ")...", State::Processing);

					auto package_result = me.UI_->SendInternalMessage(
						WM_FREEZETIME_PROCESS_PACKAGE, (WPARAM)type, (LPARAM)(&stream));
					if (package_result != TRUE) {
						Sleep(2000);
						break;
					}

					me.UI_->OnNetworkStateChanged("已经连接到服务器 (" + me.ServerIP_ + ")，等待服务器控制中 (" + rec_pk_id_buf + ")", State::Succeed);
				}

				me.UI_->OnNetworkStateChanged("与服务器的连接断开 (" + me.ServerIP_ + ")", State::Failure);
				me.Stream_ = nullptr;
				closesocket(sockClient);
				me.Socket_ = nullptr;

				if (me.PackageErrorMessage_ != nullptr) {
					me.UI_->OnNetworkStateChanged(me.PackageErrorMessage_, State::Failure);
					Sleep(me.PackageErrorSleep_);
					me.PackageErrorMessage_ = nullptr;
					me.PackageErrorSleep_ = 0;
				}

				if (me.Restart_) {
					goto final_exit;
				} else if (me.Running_) {
					goto reconnect;
				} else {
					goto final_exit;
				}
			}

		final_exit:
			WSACleanup();

			if (me.Restart_) {
				ShellExecute(NULL, "open", "cmd.exe",
					me.RestartCommand_.data(), me.RestartDirectory_.data(), SW_SHOWNORMAL);

				ExitProcess(0);
			}

			CloseHandle(me.Thread_);
			me.Thread_ = NULL;
			return 0;
		}

	public:

		bool ProcessPackage(PackageType type) {
			this->PackageErrorMessage_ = nullptr;
			this->PackageErrorSleep_ = 0;
			switch (type) {
			case PackageType::ChooseCamera:
				return ReceiveChooseCamera();
			case PackageType::Ping:
				return ReceivePing();
			case PackageType::Snapshot:
				return ReceiveSnapshot();
			case PackageType::AutoFocus:
				return ReceiveAutoFocus();
			case PackageType::Update:
				return ReceiveUpdate();
			default:
				return true;
			}
		}

	protected:
		bool ReceiveChooseCamera() {
			std::string port;
			BOOL_CHECK(Stream_->ReadString(port));
			if (port.length() == 0) {
				// non-camera chosen
				this->PackageErrorMessage_ = "未找到合适的照相机连接，等待新的照相机接入...";
				this->PackageErrorSleep_ = 5000;
				return false; // break session
			}
			Camera_ = Camera::Create(port);
			Camera_->OpenSession();
			return true;
		}

		bool ReceivePing() {
			SendPingReply();
			return true;
		}

		bool ReceiveSnapshot() {
			int av, tv, iso;
			BOOL_CHECK(Stream_->Read(av));
			BOOL_CHECK(Stream_->Read(tv));
			BOOL_CHECK(Stream_->Read(iso));
			Camera_->Snapshot(this, av, tv, iso);
			return true;
		}

		bool ReceiveAutoFocus() {
			Camera_->AutoFocus();
			return true;
		}

		bool ReceiveUpdate() {
			int count;
			if (!Stream_->Read(count)) {
				return false;
			}
			std::string app_path = Utility::GetApplicationPath();
			std::string path = app_path + Format("Update_%d", Utility::GetTimeUniqueID());
			CreateDirectory(path.data(), NULL);
			path += "\\";

			std::string cmd;
			cmd += "@echo off\r\n";
			cmd += "echo 等待服务程序关闭...\r\n";
			cmd += "choice /t 10 /d y /n > nul\r\n";
			for (int i = 0; i < count; i++) {
				int method;
				if (!Stream_->Read(method)) {
					return false;
				}
				std::string filename;
				if (!Stream_->ReadString(filename)) {
					return false;
				}
				int filesize;
				if (!Stream_->Read(filesize)) {
					return false;
				}
				UpdateFile::UpdateMethod m = (UpdateFile::UpdateMethod)method;
				if ((m == UpdateFile::Add) || (m == UpdateFile::Update)) {
					char *buf = new char[filesize];
					if (!Stream_->Read(buf, filesize)) {
						delete[] buf;
						return false;
					}
					std::ofstream f(path + filename, std::ios::binary | std::ios::out | std::ios::trunc);
					f.write(buf, filesize);
					f.close();
					delete[] buf;
					if (m == UpdateFile::Add) {
						cmd += "echo 添加 " + filename + "...\r\n";
					}
					else {
						cmd += "echo 更新 " + filename + "...\r\n";
					}
					cmd += "copy \"" + path + filename + "\" \"" + app_path + filename + "\" /y\r\n";
					cmd += "choice /t 1 /d y /n > nul\r\n";
				}
				else if (m == UpdateFile::Delete) {
					cmd += "echo 删除 " + filename + "...\r\n";
					cmd += "del \"" + app_path + filename + "\" /q\r\n";
					cmd += "choice /t 1 /d y /n > nul\r\n";
				}
			}
			cmd += "cls\r\n";
			cmd += GetCommandLine();
			cmd += "\r\n";
			std::string cmd_filename = path.substr(0, path.size() - 1) + ".cmd";
			std::ofstream fx(cmd_filename, std::ios::out | std::ios::trunc);
			fx << cmd << std::endl;
			fx.close();

			this->Running_ = false;
			this->Restart_ = true;
			this->RestartCommand_ = (" /c \"" + cmd_filename + "\"");
			this->RestartDirectory_ = app_path;
			return true;
		}

	protected:
		virtual void OnSnapshotComplete(const SnapshotData &data) {
			SendSnapshotReply(data);
		}

	public:
		bool SendRegister() {
			auto ports = Camera::GetAvailablePorts();
			BOOL_CHECK(Stream_->Write((int)PackageType::Register));
			BOOL_CHECK(Stream_->Write(Utility::GetMachineName()));
			BOOL_CHECK(Stream_->Write((int)ports.size()));
			for (auto &s : ports) {
				BOOL_CHECK(Stream_->Write(s));
			}
			return true;
		}

	protected:
		bool SendPingReply() {
			BOOL_CHECK(Stream_->Write((int)PackageType::PingReply));
			return true;
		}

		bool SendSnapshotReply(const SnapshotData &data) {
			BOOL_CHECK(Stream_->Write((int)PackageType::SnapshotReply));
			BOOL_CHECK(Stream_->Write((int)data.Av));
			BOOL_CHECK(Stream_->Write((int)data.Tv));
			BOOL_CHECK(Stream_->Write((int)data.ISOSpeed));
			std::fstream f(data.FileName, std::ios::in | std::ios::binary);
			if (f.is_open()) {
				f.seekg(0, std::ios::end);
				int length = f.tellg();
				f.seekg(0, std::ios::beg);
				auto buffer = new char[length];
				f.read(buffer, length);
				f.close();
				BOOL_CHECK(Stream_->Write(length));
				BOOL_CHECK(Stream_->Write(buffer, length));
			} else {
				BOOL_CHECK(Stream_->Write((int)0));
			}
			return true;
		}

	public:

		void Start() {
			this->Running_ = true;
			Thread_ = CreateThread(NULL, 0, ServiceProc, this, 0, NULL);
			SetThreadPriority(Thread_, THREAD_PRIORITY_TIME_CRITICAL);
		}

		void Stop() {
			this->Running_ = false;
			if (this->Socket_ != nullptr) {
				closesocket(*(this->Socket_));
			}
			WaitForSingleObject(this->Thread_, INFINITE);
		}

	} *ClientPtr;

}
