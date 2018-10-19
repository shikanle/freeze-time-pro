#pragma once

#pragma once

#include "common.h"

namespace FreezeTimePro {

	struct UpdateFile {
		enum UpdateMethod {
			Normal = 0,
			Add = 1,
			Update = 2,
			Delete = 3
		} Method;
		std::string FileName;
		std::string AlternativeFileName;
		unsigned char MD5Code[16];
		unsigned int FileSize;
		char *FileContent;
	};

	class UpdatePackage : public std::vector<UpdateFile> {
	protected:
		bool IsWeak_;
	public:
		UpdatePackage operator-(UpdatePackage &p) {
			UpdatePackage retval(true);
			for (UpdateFile &f : *this) {
				UpdateFile *fp = p.FindFile(f.AlternativeFileName);
				if (fp == nullptr) {
					// new file
					UpdateFile newf = f;
					newf.Method = UpdateFile::Add;
					retval.push_back(newf);
				}
				else {
					// file found, test md5
					bool same_md5 = true;
					for (int i = 0; i < 16; i++) {
						if (f.MD5Code[i] != fp->MD5Code[i]) {
							same_md5 = false;
							break;
						}
					}
					if (same_md5 && (f.FileSize == fp->FileSize)) {
						// complete same file
						// do nothing
					}
					else {
						// update
						UpdateFile newf = f;
						newf.Method = UpdateFile::Update;
						retval.push_back(newf);
					}
				}
			}
			for (UpdateFile &f : p) {
				if (!this->FindFile(f.AlternativeFileName)) {
					UpdateFile newf = f;
					newf.Method = UpdateFile::Delete;
					retval.push_back(newf);
				}
			}
			return retval;
		}

		UpdateFile *FindFile(std::string alternative_filename) {
			for (UpdateFile &f : *this) {
				if (f.AlternativeFileName == alternative_filename) {
					return &f;
				}
			}
			return nullptr;
		}

		UpdatePackage(bool weak = false)
			: std::vector<UpdateFile>(), IsWeak_(weak) { }

		~UpdatePackage() {
			if (!IsWeak_) {
				for (UpdateFile &f : *this) {
					if (f.FileContent != nullptr) {
						delete[] f.FileContent;
					}
				}
			}
		}
	};

	class Updater {
	protected:
		std::vector<std::string> Extensions_;
		UpdatePackage LocalPackage_;

	protected:
		bool TryAddFile(std::string &base_path, WIN32_FIND_DATA &p, bool has_content) {
			if ((p.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0) {
				return false;
			}
			std::string filename = p.cFileName;
			std::transform(filename.begin(), filename.end(), filename.begin(), ::tolower);
			for (std::string &ext : Extensions_) {
				int offset = (int)filename.length() - (int)ext.length();
				if (offset > 0) {
					if (filename.substr(offset, ext.length()) == ext) {
						UpdateFile rf;
						rf.Method = UpdateFile::Normal;
						rf.FileName = p.cFileName;
						rf.AlternativeFileName = rf.FileName;
						std::transform(rf.AlternativeFileName.begin(), rf.AlternativeFileName.end(),
							rf.AlternativeFileName.begin(), ::tolower);
						rf.FileSize = p.nFileSizeLow;
						rf.FileContent = new char[p.nFileSizeLow];
						std::ifstream f(base_path + p.cFileName, std::ios::in | std::ios::binary);
						f.read(rf.FileContent, p.nFileSizeLow);
						f.close();
						MD5_CTX md5;
						MD5Init(&md5);
						MD5Update(&md5, (unsigned char *)rf.FileContent, p.nFileSizeLow);
						MD5Final(rf.MD5Code, &md5);
						if (!has_content) {
							delete[] rf.FileContent;
							rf.FileContent = nullptr;
						}
						LocalPackage_.push_back(rf);
						return true;
					}
				}
			}
			return false;
		}

	public:
		UpdatePackage &GetLocalPackage() {
			return this->LocalPackage_;
		}

		Updater(bool has_content) : Extensions_() {
			Extensions_.push_back(".exe");
			Extensions_.push_back(".dll");

			WIN32_FIND_DATA p;
			auto base_path = Utility::GetApplicationPath();
			HANDLE h = FindFirstFile((base_path + "*.*").data(), &p);
			if (h != INVALID_HANDLE_VALUE) {
				TryAddFile(base_path, p, has_content);
				while (FindNextFile(h, &p)) {
					TryAddFile(base_path, p, has_content);
				}
				FindClose(h);
			}
		}

	};

}