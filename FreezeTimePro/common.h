#pragma once

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <unordered_map>
#include <mutex>
#include "../ThirdParty/cmdline/cmdline.h"

#include <SDKDDKVer.h>
#include <WinSock2.h>
#include <cstdlib>
#include <Windows.h>
#include <WS2tcpip.h>
#include <mstcpip.h>

#include "../CanonEDSDK/edsdk.h"

#include <opencv2/core/core.hpp>
#include <opencv/cvaux.hpp>
#include <opencv2/opencv.hpp>
#include "../ThirdParty/Eigen 3.3.4/Eigen/Dense"
#include "../ThirdParty/Eigen 3.3.4/Eigen/Core"
#include <opencv2/core/eigen.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include "../ThirdParty/jsoncpp/include/json/json.h"

#define PROTOTYPES 1
extern "C" {
#include "../ThirdParty/md5/global.h"
#include "../ThirdParty/md5/md5.h"
}

#include "Resource.h"

#pragma comment(lib, "ws2_32.lib")
#if defined(NDEBUG)
#pragma comment(lib, "opencv_core340.lib")
#pragma comment(lib, "opencv_imgproc340.lib")
#pragma comment(lib, "opencv_imgcodecs340.lib")
#pragma comment(lib, "opencv_highgui340.lib")
#else
#pragma comment(lib, "opencv_core340d.lib")
#pragma comment(lib, "opencv_imgproc340d.lib")
#pragma comment(lib, "opencv_imgcodecs340d.lib")
#pragma comment(lib, "opencv_highgui340d.lib")
#endif

//#define FAKE_CAMERAS	1	
#define SERVICE_PORT	6608
#define BOOL_CHECK(x)	if (!(x)) { return false; }

#define PROJECT_VERSION					"0.1.1000"
#define SERVER_CONFIG_FILENAME			"FreezeTimePro.Server.json"
//#define CAMERA_FRAME_WIDTH				4752
//#define CAMERA_FRAME_HEIGHT				3168
//#define CALIBRATE_DISTANCE_IN_PIXELS    (CAMERA_FRAME_HEIGHT * 1055 / 1250)

#define WM_FREEZETIME_REGISTER			(WM_USER + 1)
#define	WM_FREEZETIME_PROCESS_PACKAGE	(WM_USER + 2)
#define	WM_FREEZETIME_UPDATE_UI			(WM_USER + 3)

namespace FreezeTimePro {

	typedef __int64 int64;

	class Utility {
	public:

		static std::string ToString(int value) {
			char buf[16];
			_itoa_s(value, buf, 15, 10);
			return buf;
		}

		static std::string GetApplicationPath()
		{
			TCHAR exeFullPath[MAX_PATH];
			GetModuleFileName(NULL, exeFullPath, MAX_PATH);
			std::string strPath = exeFullPath;
			auto pos = strPath.find_last_of('\\', strPath.length());
			return strPath.substr(0, pos + 1);
		}

		static std::string GetMachineName() {
			char buffer[256];
			DWORD size = 255;
			GetComputerName(buffer, &size);
			return buffer;
		}

		static std::string PathCombine(const std::string &p1, const std::string &p2) {
			if (p1.empty()) {
				return p2;
			}
			char c = p1.at(p1.length() - 1);
			if ((c == '\\') || (c == '/')) {
				return p1 + p2;
			}
			else {
				return p1 + "\\" + p2;
			}
		}

		static bool DirectoryExists(const std::string &p) {
			auto attr = GetFileAttributes(p.c_str());
			if (attr == INVALID_FILE_ATTRIBUTES) {
				return false;
			}
			return ((attr & FILE_ATTRIBUTE_DIRECTORY) != 0);
		}

		static bool FileExists(const std::string &p) {
			auto attr = GetFileAttributes(p.c_str());
			if (attr == INVALID_FILE_ATTRIBUTES) {
				return false;
			}
			return ((attr & FILE_ATTRIBUTE_DIRECTORY) == 0);
		}

		static int GetTimeUniqueID() {
			time_t lt = time(NULL);
			tm tm_struct;
			localtime_s(&tm_struct, &lt);
			int retval = tm_struct.tm_year % 100;
			retval = retval * 1000 + tm_struct.tm_yday;
			retval = retval * 10000 + (tm_struct.tm_sec / 10) + tm_struct.tm_min * 10 + tm_struct.tm_hour * 10 * 60;
			return retval;
		}
		
		static std::string GetLegalFileName(const std::string &s) {
			std::stringstream ss;
			for (char c : s) {
				if (((c >= 'a') && (c <= 'z')) ||
					((c >= 'A') && (c <= 'Z')) ||
					((c >= '0') && (c <= '9')) ||
					(c == ' ') || (c == '.')) {
					ss << c;
				} else {
					ss << '_';
				}
			}
			return ss.str();
		}
	};

	template <typename T1>
	std::string Format(const std::string &format, T1 v1) {
		char buf[1024];
		sprintf_s(buf, 1023, format.data(), v1);
		return buf;
	}

	template <typename T1, typename T2>
	std::string Format(const std::string &format, T1 v1, T2 v2) {
		char buf[1024];
		sprintf_s(buf, 1023, format.data(), v1, v2);
		return buf;
	}

	template <typename T1, typename T2, typename T3>
	std::string Format(const std::string &format, T1 v1, T2 v2, T3 v3) {
		char buf[1024];
		sprintf_s(buf, 1023, format.data(), v1, v2, v3);
		return buf;
	}

	template <typename T1, typename T2, typename T3, typename T4>
	std::string Format(const std::string &format, T1 v1, T2 v2, T3 v3, T4 v4) {
		char buf[1024];
		sprintf_s(buf, 1023, format.data(), v1, v2, v3, v4);
		return buf;
	}

	template <typename T1, typename T2, typename T3, typename T4, typename T5>
	std::string Format(const std::string &format, T1 v1, T2 v2, T3 v3, T4 v4, T5 v5) {
		char buf[1024];
		sprintf_s(buf, 1023, format.data(), v1, v2, v3, v4, v5);
		return buf;
	}

	template <typename T1, typename T2, typename T3, typename T4, typename T5, typename T6>
	std::string Format(const std::string &format, T1 v1, T2 v2, T3 v3, T4 v4, T5 v5, T6 v6) {
		char buf[1024];
		sprintf_s(buf, 1023, format.data(), v1, v2, v3, v4, v5, v6);
		return buf;
	}

	template <typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7>
	std::string Format(const std::string &format, T1 v1, T2 v2, T3 v3, T4 v4, T5 v5, T6 v6, T7 v7) {
		char buf[1024];
		sprintf_s(buf, 1023, format.data(), v1, v2, v3, v4, v5, v6, v7);
		return buf;
	}

	template <typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8>
	std::string Format(const std::string &format, T1 v1, T2 v2, T3 v3, T4 v4, T5 v5, T6 v6, T7 v7, T8 v8) {
		char buf[1024];
		sprintf_s(buf, 1023, format.data(), v1, v2, v3, v4, v5, v6, v7, v8);
		return buf;
	}

	class CommandLineParser {
	protected:
		cmdline::parser Parser_;
		std::string CommandLine_;

	public:
		std::string GetMode() { return Parser_.get<std::string>("mode"); }
		int GetPort() { return Parser_.get<int>("port"); }
		std::string GetIP() { return Parser_.get<std::string>("ip"); }
		int GetSlot() { return Parser_.get<int>("slot"); }
		bool GetNoCalibration() { return Parser_.exist("no-calibration"); }
		std::string GetCommand() { return this->CommandLine_; }
		std::string GetDeviceName() { return Parser_.get<std::string>("name"); }

		CommandLineParser() {
			Parser_.set_program_name("FreezeTime Pro");
			Parser_.add<std::string>("mode", 'm', "service mode", false, "", cmdline::oneof<std::string>("server", "client", ""));
			Parser_.add<int>("port", 'p', "USB port of camera", false, -1, cmdline::range(-1, 15));
			Parser_.add<std::string>("ip", 'i', "server IP", false, "");
			Parser_.add<int>("slot", 's', "slot id", false, -1, cmdline::range(-1, 65535));
			Parser_.add<std::string>("name", 'n', "device name", false, "default");
		}
		void Parse(int argc, char **argv) {
			Parser_.parse_check(argc, argv);
			CommandLine_ = "";
			for (int i = 0; i < argc; i++) {
				if (i > 0) {
					CommandLine_ += " ";
				}
				CommandLine_ += argv[i];
			}
		}
	} CommandLine;
}