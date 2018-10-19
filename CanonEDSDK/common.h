#pragma once

#include <string>
#include <Windows.h>

namespace EDSDK {
	class Utility {
	public:

		static std::string PathCombine(const std::string &p1, const std::string &p2) {
			if (p1.empty()) {
				return p2;
			}
			char c = p1.at(p1.length() - 1);
			if ((c == '\\') || (c == '/')) {
				return p1 + p2;
			} else {
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

		static std::string GetSystemFolder() {
			char buf[MAX_PATH + 1];
			GetCurrentDirectory(MAX_PATH, buf);
			return buf;
		}
	};
}