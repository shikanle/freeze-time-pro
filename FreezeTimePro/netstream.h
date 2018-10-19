#pragma once

#include "common.h"

namespace FreezeTimePro {

	class NetStream {
	private:
		SOCKET Socket_;

	public:
		bool Read(char *buffer, int length) {
			while (length > 0) {
				// recv return: 
				//    0 - the connection was gracefully closed
				//   -1 - connection failure (error) = SOCKET_ERROR
				int readlen = recv(this->Socket_, (char *)buffer, length, 0);
				if (readlen <= 0) {
					return false;
				}
				else {
					length -= readlen;
					buffer += readlen;
				}
			}
			return (length == 0);
		}

		bool ReadString(std::string &s) {
			int length;
			if (!Read(length)) {
				return false;
			}
			char *buf = new char[length + 1];
			if (!Read(buf, length)) {
				delete[] buf;
				return false;
			}
			buf[length] = 0;
			s = buf;
			return true;
		}

		bool Write(const char *buffer, int length) {
			return send(this->Socket_, buffer, length, 0) == length;
		}

		template <typename T>
		bool Read(T &buf) {
			return this->Read((char *)&buf, sizeof(T));
		}

		template <typename T>
		bool Write(const T &data) {
			return this->Write((char *)&data, sizeof(T));
		}

		bool Write(int data) {
			return this->Write((char *)&data, sizeof(int));
		}

		bool Write(int64 data) {
			return this->Write((char *)&data, sizeof(int64));
		}

		bool Write(const std::string &s) {
			int length = (int)s.length();
			if (!this->Write(length)) {
				return false;
			}
			if (!this->Write(s.data(), length)) {
				return false;
			}
			return true;
		}

		NetStream(SOCKET socket)
			: Socket_(socket) {
		}

		~NetStream() {
		}
	};

}
