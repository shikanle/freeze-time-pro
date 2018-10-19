#pragma once

#include "common.h"

namespace FreezeTimePro {

	typedef struct SnapshotData {
		DWORD Av;
		DWORD Tv;
		DWORD ISOSpeed;
		std::string FileName;
	} *SnapshotDataPtr;

	typedef class ICameraEventHandler {
	public:
		virtual void OnSnapshotComplete(const SnapshotData &data) = 0;
	} *ICameraEventHandlerPtr;

	typedef class Camera : public EDSDK::ICameraEventHandler {
	protected:
		EDSDK::CameraPtr Camera_;
		ICameraEventHandlerPtr EventHandler_;
		std::string CurrentPath_;

	public:
		Camera(EDSDK::CameraPtr camera) : Camera_(camera), EventHandler_(nullptr), CurrentPath_("") {
		}

		~Camera() {
			#ifndef FAKE_CAMERAS
			Camera_->CloseSession();
			#endif
		}

	protected:
		virtual void OnDownloadComplete(const std::string &filename) {
			if (this->EventHandler_ != nullptr) {
				SnapshotData data;
				data.Av = this->Camera_->GetAv();
				data.Tv = this->Camera_->GetTv();
				data.ISOSpeed = this->Camera_->GetISOSpeed();
				data.FileName = filename;
				this->EventHandler_->OnSnapshotComplete(data);
			}
		}

	protected:
		std::string EnsureCurrentPath() {
			if (this->CurrentPath_.size() != 0) {
				return this->CurrentPath_;
			}
			auto dir = Utility::PathCombine(Utility::GetApplicationPath(), "Workspace");
			if (!Utility::DirectoryExists(dir)) {
				CreateDirectory(dir.c_str(), NULL);
			}
			dir = Utility::PathCombine(dir, Utility::GetLegalFileName(Camera_->ToString()));
			if (!Utility::DirectoryExists(dir)) {
				CreateDirectory(dir.c_str(), NULL);
			}
			this->CurrentPath_ = dir;
			return this->CurrentPath_;
		}

	public:
		void OpenSession() {
			#ifndef FAKE_CAMERAS
			this->Camera_->OpenSession();
			#endif
		}

		void CloseSession() {
			#ifndef FAKE_CAMERAS
			this->Camera_->CloseSession();
			#endif
		}

		void Snapshot(ICameraEventHandlerPtr handler = nullptr, DWORD av = 0x30, DWORD tv = 0x78, DWORD iso = 0x00000068) {
			#ifndef FAKE_CAMERAS
			this->EventHandler_ = handler;
			this->Camera_->SetSaveTo(this->EnsureCurrentPath());
			this->Camera_->SetAv(av); // 5.6
			this->Camera_->SetTv(tv); // 1/250
			this->Camera_->SetISOSpeed(iso); // 1600
			this->Camera_->SetWhiteBalance(EDSDK::WhiteBalance::Fluorescent);
			this->Camera_->SetImageQuality(EDSDK::ImageQuality::LJ);
			this->Camera_->SetPictureStyle(EDSDK::PictureSytle::Standard);
			this->Camera_->BindEventHandler(this);
			this->Camera_->Async(&(EDSDK::Camera::PressShutterWithoutAF));
			#endif
		}

		void AutoFocus() {
			#ifndef FAKE_CAMERAS
			this->Camera_->PressShutterButton(EDSDK::ShutterButton::Halfway);
			this->Camera_->PressShutterButton(EDSDK::ShutterButton::Off);
			#endif
		}

	public:
		static std::vector<std::string> GetAvailablePorts() {
			std::vector<std::string> retval;
#ifdef FAKE_CAMERAS
			retval.push_back("usb0");
			retval.push_back("usb1");
#else
			for (auto c : EDSDK::GetCameraList()) {
				retval.push_back(c->ToString());
			}
#endif
			return retval;
		}

		static Camera *Create(const std::string &port) {
#ifdef FAKE_CAMERAS
			return new Camera(nullptr);
#else
			for (auto c : EDSDK::GetCameraList()) {
				if (port == c->ToString()) {
					return new Camera(c);
				}
			}
			return nullptr;
#endif
		}

	} *CameraPtr;


}