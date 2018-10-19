#pragma once

#include "common.h"
#include "netstream.h"
#include "package.h"
#include "image_algorithms.h"
#include "server_configuration.h"

namespace FreezeTimePro {

	class ServerThread;

	typedef interface IServer {
		virtual bool AssignPort(ServerThread *thread, const std::string machine, const std::string &port) = 0;
		virtual void OnClientClose(ServerThread *thread) = 0;
		virtual void OnClientStateChanged(ServerThread *thread) = 0;
		virtual void OnClientListChanged() = 0;
		virtual ServerConfigurationPtr GetConfiguration() = 0;
		virtual void OnSnapshotComplete(ServerThread *thread, State state) = 0;
		virtual bool CheckUpdate(NetStream &stream) = 0;
	} *IServerPtr;

	typedef class ServerThread {
	private:
		IServerPtr Server_;
		SOCKET Socket_;
		HANDLE Thread_;
		NetStream Stream_;
		bool Running_;
		bool Capturing_;
		std::string CurrentPath_;
		bool (ServerThread::*CaptureCallback_)(cv::Mat frame);

		std::string MachineName_;
		std::string MachinePortName_;
		
		int SlotID_;
		ImageTransform Transform_;

		State State_;
		std::string StateMessage_;
		std::string Message_;
		int64 LastPingTick_;

	public:
		ServerThread(IServerPtr server, SOCKET socket) :
			Server_(server), Socket_(socket), Stream_(socket), Thread_(NULL), Running_(false),
			MachineName_(), MachinePortName_(), SlotID_(-1), LastPingTick_(0), CurrentPath_(""),
			Transform_(), Capturing_(false), CaptureCallback_(nullptr) {
		}

		~ServerThread() {
		}

	private:
		static DWORD WINAPI ServiceProc(LPVOID lpThreadParameter) {
			ServerThread &me = *(static_cast<ServerThread *>(lpThreadParameter));
			me.UpdateState(State::Succeed, "已经连接", "已经连接客户端，正在等待注册信息...");
			while (me.Running_) {
				PackageType type;
				if (!me.Stream_.Read(type)) {
					break; // error
				}
				if (!me.ProcessPackage(type, me.Stream_)) {
					break;
				}
			}
			me.UpdateState(State::Failure, "断开连接", "已经断开连接");
			closesocket(me.Socket_);
			CloseHandle(me.Thread_);
			me.Thread_ = NULL;
			me.Server_->OnClientClose(&me);
			return 0;
		}

		bool ProcessPackage(PackageType type, NetStream &stream) {
			switch (type) {
			case PackageType::Register:
				return ReceiveRegister();
			case PackageType::PingReply:
				return ReceivePingReply();
			case PackageType::SnapshotReply:
				{
					auto retval = ReceiveSnapshotReply();
					this->Capturing_ = false;
					Server_->OnSnapshotComplete(this, this->State_);
					return retval;
				}
			case PackageType::CheckUpdate:
				return ReceiveCheckUpdate();
			default:
				return false;
			}
		}

		inline void UpdateState() {
			this->Server_->OnClientStateChanged(this);
		}

		inline void UpdateState(const std::string &msg) {
			this->Message_ = msg;
			UpdateState();
		}
		
		inline void UpdateState(State state, const std::string &state_msg, const std::string &msg) {
			this->State_ = state;
			this->StateMessage_ = state_msg;
			this->Message_ = msg;
			UpdateState();
		}

	public:
		inline bool IsCapturing() const {
			return this->Capturing_;
		}

		inline void ResetCapturing() {
			this->Capturing_ = false;
		}

		inline ImageTransform GetTransform() {
			return this->Transform_;
		}

		inline bool IsAssigned() const {
			return (this->MachineName_.length() > 0) &&
				(this->MachinePortName_.length() > 0);
		}

		inline std::string GetMachineName() const {
			return this->MachineName_;
		}

		inline std::string GetPortName() const {
			return this->MachinePortName_;
		}

		inline std::string GetFullName() const {
			return this->MachineName_ + " " + this->MachinePortName_;
		}

		inline int GetSlotID() const {
			return this->SlotID_;
		}

		inline void SetSlotID(int id) {
			if (id < 0) {
				id = -1;
			}
			if (id != this->SlotID_) {
				this->SlotID_ = id;
				auto config = this->Server_->GetConfiguration();
				config->Slots.insert_or_assign(this->GetFullName(), id);
				config->SetDirty();
			}
		}

		inline State GetState() const {
			return this->State_;
		}

		inline std::string GetStateMessage() const {
			return this->StateMessage_;
		}

		inline std::string GetInfoMessage() const {
			return this->Message_;
		}

		inline void SetPortName(const std::string &port) {
			this->MachinePortName_ = port;
		}

		inline bool IsMachine(const std::string &machine, const std::string &port) {
			return (this->MachineName_ == machine) &&
				(this->MachinePortName_ == port);
		}

		void RestoreSlotID() {
			auto config = this->Server_->GetConfiguration();
			auto slot_el = config->Slots.find(this->GetFullName());
			if (slot_el != config->Slots.end()) {
				this->SlotID_ = slot_el->second;
			} else {
				this->SlotID_ = -1;
			}
		}

	public:
		bool SendChooseCamera(const std::string &port) {
			BOOL_CHECK(Stream_.Write((int)PackageType::ChooseCamera));
			BOOL_CHECK(Stream_.Write(port));
			return true;
		}

		bool SendPing() {
			LastPingTick_ = cv::getTickCount();
			BOOL_CHECK(Stream_.Write((int)PackageType::Ping));
			return true;
		}

		bool SendSnapshot(const std::string &path, int av, int tv, int iso, bool (ServerThread::*callback)(cv::Mat frame) = nullptr) {
			if (this->Capturing_) {
				return false;
			}
			this->CurrentPath_ = path;
			this->Capturing_ = true;
			this->CaptureCallback_ = callback;
			BOOL_CHECK(Stream_.Write((int)PackageType::Snapshot));
			BOOL_CHECK(Stream_.Write(av));
			BOOL_CHECK(Stream_.Write(tv));
			BOOL_CHECK(Stream_.Write(iso));
			this->UpdateState(State::Processing, "正在照相", "正在照相并获取数据中...");
			return true;
		}

		bool SendAutoFocus() {
			BOOL_CHECK(Stream_.Write((int)PackageType::AutoFocus));
			return true;
		}

		bool Calibrate(cv::Mat frame) {
			return Calibrate(frame, false);
		}

		bool CalibrateSilently(cv::Mat frame) {
			return Calibrate(frame, true);
		}

		bool Calibrate(cv::Mat frame, bool silently) {
			static std::recursive_mutex mutex;
			mutex.lock();
			auto transform = ImageAlgorithms::Calibrate(frame, silently);
			if (transform.IsAvailable) {
				this->Transform_ = transform;
				State state = State::Succeed;
				if (this->Transform_.FillRate < 1) {
					state = State::Warning;
				}
				this->UpdateState(state, "更新配准",
					Format("平移：%.2lf, %.2lf； 旋转：%.2lf； 放缩：%.2lf%%； 填充率：%.2lf%%",
						this->Transform_.TranslateX,
						this->Transform_.TranslateY,
						this->Transform_.Rotation * 57.2957795,
						this->Transform_.Scaling * 100,
						this->Transform_.FillRate * 100));
				mutex.unlock();
				return true;
			} else {
				this->UpdateState(State::Failure, "配准无效", "配准失效或用户取消配准操作");
			}
			mutex.unlock();
			return true;
		}

	protected:
		bool ReceiveRegister() {
			int port_count = 0;
			std::string chosen;
			BOOL_CHECK(Stream_.ReadString(MachineName_));
			BOOL_CHECK(Stream_.Read(port_count));
			for (int i = 0; i < port_count; i++) {
				std::string port_name;
				BOOL_CHECK(Stream_.ReadString(port_name));
				if (chosen.length() == 0) {
					if (Server_->AssignPort(this, MachineName_, port_name)) {
						chosen = port_name;
					}
				}
			}
			if (chosen.length() == 0) {
				UpdateState(State::Failure, "相机无效", Format("在提交的 %d 个相机中，未发现未经使用的相机", port_count));
			} else {
				UpdateState(State::Succeed, "选中相机", Format("在提交的 %d 个相机中，选中端口为 %s 的相机", port_count, chosen.c_str()));
			}
			this->Server_->OnClientListChanged();
			BOOL_CHECK(SendChooseCamera(chosen));
			return true;
		}

		bool ReceivePingReply() {
			double t = (double)(cv::getTickCount() - this->LastPingTick_) / (double)cv::getTickFrequency();
			if (t > 0.030) {
				UpdateState(State::Failure, "通信障碍", Format("应答时间 %.2lf ms", t * 1000));
			} else if (t > 0.003) {
				UpdateState(State::Warning, "通信延迟", Format("应答时间 %.2lf ms", t * 1000));
			} else {
				UpdateState(State::Succeed, "应答成功", Format("应答时间 %.2lf ms", t * 1000));
			}
			return true;
		}

		bool ReceiveSnapshotReply() {
			int av, tv, iso;
			int length;
			BOOL_CHECK(Stream_.Read(av));
			BOOL_CHECK(Stream_.Read(tv));
			BOOL_CHECK(Stream_.Read(iso));
			BOOL_CHECK(Stream_.Read(length));
			if (length > 0) {
				auto buffer = new char[length];
				BOOL_CHECK(Stream_.Read(buffer, length));
				if (this->SlotID_ >= 0) {
					auto filename = Format("%d.jpg", this->SlotID_);
					filename = Utility::PathCombine(this->CurrentPath_, filename);
					std::fstream f(filename, std::ios::out | std::ios::binary);
					if (f.is_open()) {
						f.write(buffer, length);
						f.close();
						auto outmsg = Format("光圈：%s; 曝光时间：%s; %s; 数据文件大小：%d 字节",
							EDSDK::CameraValues::AV(av).c_str(),
							EDSDK::CameraValues::TV(tv).c_str(),
							EDSDK::CameraValues::ISO(iso).c_str(),
							length);
						UpdateState(State::Succeed, "获得照片", outmsg);

						if (this->CaptureCallback_ != nullptr) {
							// comprehensive operation
							cv::Mat frame = cv::imread(filename);
							if (!frame.empty()) {
								(this->*(this->CaptureCallback_))(frame);
							}
						} else {
							// just snapshot
							if (this->Transform_.IsAvailable) {
								cv::Mat frame = cv::imread(filename);
								cv::Mat target;
								auto transform_m = this->Transform_.GetAffineMatrix();
								cv::warpAffine(frame, target, transform_m, frame.size());
								auto refined_filename = Format("refined_%d.jpg", this->SlotID_);
								refined_filename = Utility::PathCombine(this->CurrentPath_, refined_filename);
								cv::imwrite(refined_filename, target);
								UpdateState(State::Succeed, "获得照片", outmsg + " [已根据配准进行变换]");
							} else {
								UpdateState(State::Warning, "获得照片", outmsg + " [尚未配准]");
							}
						}
						return true;
					}
				} else {
					UpdateState(State::Failure, "获得照片", "已经获得数据，但由于未分配帧槽位，未能够保存");
				}
			}
			UpdateState(State::Failure, "获得照片", "获得照片数据失败");
			return true;
		}

		bool ReceiveCheckUpdate() {
			return this->Server_->CheckUpdate(this->Stream_);
		}

	public:
		void Start() {
			this->Running_ = true;
			this->Thread_ = CreateThread(NULL, 0, ServiceProc, this, 0, NULL);
			SetThreadPriority(this->Thread_, THREAD_PRIORITY_TIME_CRITICAL);
		}

		void Join() {
			if (this->Thread_ != NULL) {
				WaitForSingleObject(this->Thread_, INFINITE);
			}
		}

		void Stop() {
			this->Running_ = false;
			closesocket(this->Socket_);
			Join();
		}

		inline bool IsRunning() {
			return this->Thread_ != NULL;
		}

	} *ServerThreadPtr;

}