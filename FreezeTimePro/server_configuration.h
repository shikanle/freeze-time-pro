#pragma once

#include "common.h"

namespace FreezeTimePro {

	typedef class ServerConfiguration {
	public:
		std::unordered_map<std::string, int> Slots;
		int Av;
		int Tv;
		int ISOSpeed;
		std::string FFMpegPath;

	protected:
		bool IsDirty_;

	protected:
		void Parse(std::fstream &f) {
			Json::Value root;
			f >> root;
			if (root["slots"].isArray()) {
				for (Json::ArrayIndex index = 0; index < root["slots"].size(); index++) {
					this->Slots.insert_or_assign(
						root["slots"][index]["name"].asString(),
						root["slots"][index]["slot"].asInt()
						);
				}
			}
			if (root["av"].isString()) {
				this->Av = EDSDK::CameraValues::AV(root["av"].asString());
			} else {
				this->Av = root.get("av", 0x30).asInt();
			}

			if (root["tv"].isString()) {
				this->Tv = EDSDK::CameraValues::TV(root["tv"].asString());
			} else {
				this->Tv = root.get("tv", 0x78).asInt();
			}

			if (root["iso"].isString()) {
				this->ISOSpeed = EDSDK::CameraValues::ISO(root["iso"].asString());
			} else {
				this->ISOSpeed = root.get("iso", 0x68).asInt();
			}

			this->FFMpegPath = root.get("ffmpeg", "ffmpeg.exe").asString();
		}

	public:
		std::string GetWorkspacePath() {
			auto p = Utility::PathCombine(Utility::GetApplicationPath(), "Workspace");
			if (!Utility::DirectoryExists(p)) {
				CreateDirectory(p.c_str(), NULL);
			}
			return p;
		}

		inline void SetDirty() {
			IsDirty_ = true;
		}

		inline bool IsDirty() const {
			return this->IsDirty_;
		}

		void Save() {
			Json::Value json_root;
			json_root["version"] = PROJECT_VERSION;
			int slot_id = 0;
			for (auto el : this->Slots) {
				json_root["slots"][slot_id]["name"] = el.first;
				json_root["slots"][slot_id]["slot"] = el.second;
				slot_id++;
			}
			if (slot_id == 0) {
				json_root["slots"].resize(0);
			}
			json_root["av"] = EDSDK::CameraValues::AV(this->Av);
			json_root["tv"] = EDSDK::CameraValues::TV(this->Tv);
			json_root["iso"] = EDSDK::CameraValues::ISO(this->ISOSpeed);
			json_root["ffmpeg"] = this->FFMpegPath;

			std::fstream f;
			f.open(SERVER_CONFIG_FILENAME, std::ios::out);
			f << json_root;
			f.close();
			IsDirty_ = false;
		}

		ServerConfiguration() {
			std::fstream f;
			f.open(SERVER_CONFIG_FILENAME, std::ios::in);
			if (!f) {
				Save();
			} else {
				this->Parse(f);
			}
			f.close();
			IsDirty_ = false;
		}
		
		~ServerConfiguration() {
		}
	} *ServerConfigurationPtr;
}
