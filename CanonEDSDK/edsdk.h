#pragma once

#include <list>
#include <string>
#include <unordered_map>
#include <iostream>
#include "common.h"
#include "edsdk_entry.h"
#include "edsdk_values.h"

namespace EDSDK
{
	struct WhiteBalanceShift {
		int A;
		int B;
	};

	enum class SaveTo {
		Camera = 1,
		Host = 2,
		Both = 3,
	};

	enum class Result {
		OK = 0x00000000,
		UNIMPLEMENTED = 0x00000001,
		INTERNAL_ERROR = 0x00000002,
		MEM_ALLOC_FAILED = 0x00000003,
		MEM_FREE_FAILED = 0x00000004,
		OPERATION_CANCELLED = 0x00000005,
		INCOMPATIBLE_VERSION = 0x00000006,
		NOT_SUPPORTED = 0x00000007,
		UNEXPECTED_EXCEPTION = 0x00000008,
		PROTECTION_VIOLATION = 0x00000009,
		MISSING_SUBCOMPONENT = 0x0000000A,
		SELECTION_UNAVAILABLE = 0x0000000B,
		FILE_IO_ERROR = 0x00000020,
		FILE_TOO_MANY_OPEN = 0x00000021,
		FILE_NOT_FOUND = 0x00000022,
		FILE_OPEN_ERROR = 0x00000023,
		FILE_CLOSE_ERROR = 0x00000024,
		FILE_SEEK_ERROR = 0x00000025,
		FILE_TELL_ERROR = 0x00000026,
		FILE_READ_ERROR = 0x00000027,
		FILE_WRITE_ERROR = 0x00000028,
		FILE_PERMISSION_ERROR = 0x00000029,
		FILE_DISK_FULL_ERROR = 0x0000002A,
		FILE_ALREADY_EXISTS = 0x0000002B,
		FILE_FORMAT_UNRECOGNIZED = 0x0000002C,
		FILE_DATA_CORRUPT = 0x0000002D,
		FILE_NAMING_NA = 0x0000002E,
		DIR_NOT_FOUND = 0x00000040,
		DIR_IO_ERROR = 0x00000041,
		DIR_ENTRY_NOT_FOUND = 0x00000042,
		DIR_ENTRY_EXISTS = 0x00000043,
		DIR_NOT_EMPTY = 0x00000044,
		PROPERTIES_UNAVAILABLE = 0x00000050,
		PROPERTIES_MISMATCH = 0x00000051,
		PROPERTIES_NOT_LOADED = 0x00000053,
		INVALID_PARAMETER = 0x00000060,
		INVALID_HANDLE = 0x00000061,
		INVALID_POINTER = 0x00000062,
		INVALID_INDEX = 0x00000063,
		INVALID_LENGTH = 0x00000064,
		INVALID_FN_POINTER = 0x00000065,
		INVALID_SORT_FN = 0x00000066,
		DEVICE_NOT_FOUND = 0x00000080,
		DEVICE_BUSY = 0x00000081,
		DEVICE_INVALID = 0x00000082,
		DEVICE_EMERGENCY = 0x00000083,
		DEVICE_MEMORY_FULL = 0x00000084,
		DEVICE_INTERNAL_ERROR = 0x00000085,
		DEVICE_INVALID_PARAMETER = 0x00000086,
		DEVICE_NO_DISK = 0x00000087,
		DEVICE_DISK_ERROR = 0x00000088,
		DEVICE_CF_GATE_CHANGED = 0x00000089,
		DEVICE_DIAL_CHANGED = 0x0000008A,
		DEVICE_NOT_INSTALLED = 0x0000008B,
		DEVICE_STAY_AWAKE = 0x0000008C,
		DEVICE_NOT_RELEASED = 0x0000008D,
		STREAM_IO_ERROR = 0x000000A0,
		STREAM_NOT_OPEN = 0x000000A1,
		STREAM_ALREADY_OPEN = 0x000000A2,
		STREAM_OPEN_ERROR = 0x000000A3,
		STREAM_CLOSE_ERROR = 0x000000A4,
		STREAM_SEEK_ERROR = 0x000000A5,
		STREAM_TELL_ERROR = 0x000000A6,
		STREAM_READ_ERROR = 0x000000A7,
		STREAM_WRITE_ERROR = 0x000000A8,
		STREAM_PERMISSION_ERROR = 0x000000A9,
		STREAM_COULDNT_BEGIN_THREAD = 0x000000AA,
		STREAM_BAD_OPTIONS = 0x000000AB,
		STREAM_END_OF_STREAM = 0x000000AC,
		COMM_PORT_IS_IN_USE = 0x000000C0,
		COMM_DISCONNECTED = 0x000000C1,
		COMM_DEVICE_INCOMPATIBLE = 0x000000C2,
		COMM_BUFFER_FULL = 0x000000C3,
		COMM_USB_BUS_ERR = 0x000000C4,
		USB_DEVICE_LOCK_ERROR = 0x000000D0,
		USB_DEVICE_UNLOCK_ERROR = 0x000000D1,
		STI_UNKNOWN_ERROR = 0x000000E0,
		STI_INTERNAL_ERROR = 0x000000E1,
		STI_DEVICE_CREATE_ERROR = 0x000000E2,
		STI_DEVICE_RELEASE_ERROR = 0x000000E3,
		DEVICE_NOT_LAUNCHED = 0x000000E4,
		ENUM_NA = 0x000000F0,
		INVALID_FN_CALL = 0x000000F1,
		HANDLE_NOT_FOUND = 0x000000F2,
		INVALID_ID = 0x000000F3,
		WAIT_TIMEOUT_ERROR = 0x000000F4,
		SESSION_NOT_OPEN = 0x00002003,
		INVALID_TRANSACTIONID = 0x00002004,
		INCOMPLETE_TRANSFER = 0x00002007,
		INVALID_STRAGEID = 0x00002008,
		DEVICEPROP_NOT_SUPPORTED = 0x0000200A,
		INVALID_OBJECTFORMATCODE = 0x0000200B,
		SELF_TEST_FAILED = 0x00002011,
		PARTIAL_DELETION = 0x00002012,
		SPECIFICATION_BY_FORMAT_UNSUPPORTED = 0x00002014,
		NO_VALID_OBJECTINFO = 0x00002015,
		INVALID_CODE_FORMAT = 0x00002016,
		UNKNOWN_VENDER_CODE = 0x00002017,
		CAPTURE_ALREADY_TERMINATED = 0x00002018,
		INVALID_PARENTOBJECT = 0x0000201A,
		INVALID_DEVICEPROP_FORMAT = 0x0000201B,
		INVALID_DEVICEPROP_VALUE = 0x0000201C,
		SESSION_ALREADY_OPEN = 0x0000201E,
		TRANSACTION_CANCELLED = 0x0000201F,
		SPECIFICATION_OF_DESTINATION_UNSUPPORTED = 0x00002020,
		UNKNOWN_COMMAND = 0x0000A001,
		OPERATION_REFUSED = 0x0000A005,
		LENS_COVER_CLOSE = 0x0000A006,
		LOW_BATTERY = 0x0000A101,
		OBJECT_NOTREADY = 0x0000A102,
		TAKE_PICTURE_AF_NG = 0x00008D01,
		TAKE_PICTURE_RESERVED = 0x00008D02,
		TAKE_PICTURE_MIRROR_UP_NG = 0x00008D03,
		TAKE_PICTURE_SENSOR_CLEANING_NG = 0x00008D04,
		TAKE_PICTURE_SILENCE_NG = 0x00008D05,
		TAKE_PICTURE_NO_CARD_NG = 0x00008D06,
		TAKE_PICTURE_CARD_NG = 0x00008D07,
		TAKE_PICTURE_CARD_PROTECT_NG = 0x00008D08,
		LAST_GENERIC_ERROR_PLUS_ONE = 0x000000F5,
	};

	enum class FileCreateDisposition {
		CreateNew = 0,
		CreateAlways = 1,
		OpenExisting = 2,
		OpenAlways = 3,
		TruncateExsisting = 4
	};

	enum class SeekOrigin {
		Cur = 0,
		Begin = 1,
		End = 2
	};
	
	enum class Access {
		Read = 0,
		Write = 1,
		ReadWrite = 2,
		Error = -1
	};

	enum class ProgressOption {
		NoReport = 0,
		Done = 1,
		Periodically = 2
	};

	enum class AEMode {
		Program = 0,
		Tv = 1,
		Av = 2,
		Manual = 3,
		Bulb = 4,
		A_DEP = 5,
		DEP = 6,
		Custom = 7,
		Lock = 8,
		Green = 9,
		NigntPortrait = 10,
		Sports = 11,
		Portrait = 12,
		Landscape = 13,
		Closeup = 14,
		FlashOff = 15,
		CreativeAuto = 19,
		Movie = 20,
		PhotoInMovie = 21,
		SceneIntelligentAuto = 22,
		SCN = 25,
		Unknown = -1
	};

	enum class EvfAFMode {
		Quick = 0,
		Live = 1,
		LiveFace = 2,
		LiveMulti = 3,
	};

	enum class StroboMode {
		Internal = 0,
		ExternalETTL = 1,
		ExternalATTL = 2,
		ExternalTTL = 3,
		ExternalAuto = 4,
		ExternalManual = 5,
		Manual = 6,
	};

	enum class ETTL2Mode {
		Evaluative = 0,
		Average = 1,
	};

	enum class ImageQuality {
		/* Jpeg Only */
		LJ = 0x0010ff0f,    /* Jpeg Large */
		M1J = 0x0510ff0f,	/* Jpeg Middle1 */
		M2J = 0x0610ff0f,   /* Jpeg Middle2 */
		SJ = 0x0210ff0f,	/* Jpeg Small */
		LJF = 0x0013ff0f,	/* Jpeg Large Fine */
		LJN = 0x0012ff0f,	/* Jpeg Large Normal */
		MJF = 0x0113ff0f,	/* Jpeg Middle Fine */
		MJN = 0x0112ff0f,	/* Jpeg Middle Normal */
		SJF = 0x0213ff0f,	/* Jpeg Small Fine */
		SJN = 0x0212ff0f,	/* Jpeg Small Normal */
		S1JF = 0x0E13ff0f,	/* Jpeg Small1 Fine */
		S1JN = 0x0E12ff0f,  /* Jpeg Small1 Normal */
		S2JF = 0x0F13ff0f,	/* Jpeg Small2 */
		S3JF = 0x1013ff0f,	/* Jpeg Small3 */

		/* RAW + Jpeg */
		LR = 0x0064ff0f,    /* RAW */
		LRLJF = 0x00640013,	/* RAW + Jpeg Large Fine */
		LRLJN = 0x00640012,	/* RAW + Jpeg Large Normal */
		LRMJF = 0x00640113,	/* RAW + Jpeg Middle Fine */
		LRMJN = 0x00640112,	/* RAW + Jpeg Middle Normal */
		LRSJF = 0x00640213,	/* RAW + Jpeg Small Fine */
		LRSJN = 0x00640212,	/* RAW + Jpeg Small Normal */
		LRS1JF = 0x00640E13,	/* RAW + Jpeg Small1 Fine */
		LRS1JN = 0x00640E12,	/* RAW + Jpeg Small1 Normal */
		LRS2JF = 0x00640F13,	/* RAW + Jpeg Small2 */
		LRS3JF = 0x00641013,	/* RAW + Jpeg Small3 */

		LRLJ = 0x00640010,	/* RAW + Jpeg Large */
		LRM1J = 0x00640510,	/* RAW + Jpeg Middle1 */
		LRM2J = 0x00640610,	/* RAW + Jpeg Middle2 */
		LRSJ = 0x00640210,	/* RAW + Jpeg Small */

		/* MRAW(SRAW1) + Jpeg */
		MR = 0x0164ff0f,	/* MRAW(SRAW1) */
		MRLJF = 0x01640013,	/* MRAW(SRAW1) + Jpeg Large Fine */
		MRLJN = 0x01640012,	/* MRAW(SRAW1) + Jpeg Large Normal */
		MRMJF = 0x01640113,	/* MRAW(SRAW1) + Jpeg Middle Fine */
		MRMJN = 0x01640112,	/* MRAW(SRAW1) + Jpeg Middle Normal */
		MRSJF = 0x01640213,	/* MRAW(SRAW1) + Jpeg Small Fine */
		MRSJN = 0x01640212,	/* MRAW(SRAW1) + Jpeg Small Normal */
		MRS1JF = 0x01640E13,	/* MRAW(SRAW1) + Jpeg Small1 Fine */
		MRS1JN = 0x01640E12,	/* MRAW(SRAW1) + Jpeg Small1 Normal */
		MRS2JF = 0x01640F13,	/* MRAW(SRAW1) + Jpeg Small2 */
		MRS3JF = 0x01641013,	/* MRAW(SRAW1) + Jpeg Small3 */

		MRLJ = 0x01640010,	/* MRAW(SRAW1) + Jpeg Large */
		MRM1J = 0x01640510,	/* MRAW(SRAW1) + Jpeg Middle1 */
		MRM2J = 0x01640610,	/* MRAW(SRAW1) + Jpeg Middle2 */
		MRSJ = 0x01640210,	/* MRAW(SRAW1) + Jpeg Small */

		/* SRAW(SRAW2) + Jpeg */
		SR = 0x0264ff0f,	/* SRAW(SRAW2) */
		SRLJF = 0x02640013,	/* SRAW(SRAW2) + Jpeg Large Fine */
		SRLJN = 0x02640012,	/* SRAW(SRAW2) + Jpeg Large Normal */
		SRMJF = 0x02640113,	/* SRAW(SRAW2) + Jpeg Middle Fine */
		SRMJN = 0x02640112,	/* SRAW(SRAW2) + Jpeg Middle Normal */
		SRSJF = 0x02640213,	/* SRAW(SRAW2) + Jpeg Small Fine */
		SRSJN = 0x02640212,	/* SRAW(SRAW2) + Jpeg Small Normal */
		SRS1JF = 0x02640E13,	/* SRAW(SRAW2) + Jpeg Small1 Fine */
		SRS1JN = 0x02640E12,	/* SRAW(SRAW2) + Jpeg Small1 Normal */
		SRS2JF = 0x02640F13,	/* SRAW(SRAW2) + Jpeg Small2 */
		SRS3JF = 0x02641013,	/* SRAW(SRAW2) + Jpeg Small3 */

		SRLJ = 0x02640010,	/* SRAW(SRAW2) + Jpeg Large */
		SRM1J = 0x02640510,	/* SRAW(SRAW2) + Jpeg Middle1 */
		SRM2J = 0x02640610,	/* SRAW(SRAW2) + Jpeg Middle2 */
		SRSJ = 0x02640210,	/* SRAW(SRAW2) + Jpeg Small */

		Unknown = -1
	};

	enum class ImageQualityForLegacy
	{
		LJ = 0x001f000f,	/* Jpeg Large */
		M1J = 0x051f000f,	/* Jpeg Middle1 */
		M2J = 0x061f000f,	/* Jpeg Middle2 */
		SJ = 0x021f000f,	/* Jpeg Small */
		LJF = 0x00130000,	/* Jpeg Large Fine */
		LJN = 0x00120000,	/* Jpeg Large Normal */
		MJF = 0x01130000,	/* Jpeg Middle Fine */
		MJN = 0x01120000,	/* Jpeg Middle Normal */
		SJF = 0x02130000,	/* Jpeg Small Fine */
		SJN = 0x02120000,	/* Jpeg Small Normal */

		LR = 0x00240000,	/* RAW */
		LRLJF = 0x00240013,	/* RAW + Jpeg Large Fine */
		LRLJN = 0x00240012,	/* RAW + Jpeg Large Normal */
		LRMJF = 0x00240113,	/* RAW + Jpeg Middle Fine */
		LRMJN = 0x00240112,	/* RAW + Jpeg Middle Normal */
		LRSJF = 0x00240213,	/* RAW + Jpeg Small Fine */
		LRSJN = 0x00240212,	/* RAW + Jpeg Small Normal */

		LR2 = 0x002f000f,	/* RAW */
		LR2LJ = 0x002f001f,	/* RAW + Jpeg Large */
		LR2M1J = 0x002f051f,	/* RAW + Jpeg Middle1 */
		LR2M2J = 0x002f061f,	/* RAW + Jpeg Middle2 */
		LR2SJ = 0x002f021f,	/* RAW + Jpeg Small */

		Unknown = -1
	};

	enum class WhiteBalance {
		Click = -1,
		Auto = 0,
		Daylight = 1,
		Cloudy = 2,
		Tangsten = 3,
		Fluorescent = 4,
		Strobe = 5,
		WhitePaper = 6,
		Shade = 8,
		ColorTemp = 9,
		PCSet1 = 10,
		PCSet2 = 11,
		PCSet3 = 12
	};

	enum class PictureSytle {
		Standard = 0x0081,
		Portrait = 0x0082,
		Landscape = 0x0083,
		Neutral = 0x0084,
		Faithful = 0x0085,
		Monochrome = 0x0086,
		User1 = 0x0021,
		User2 = 0x0022,
		User3 = 0x0023,
		PC1 = 0x0041,
		PC2 = 0x0042,
		PC3 = 0x0043
	};

	enum class ShutterButton {
		Off = 0x00000000,
		Halfway = 0x00000001,
		Completely = 0x00000003,
		Halfway_NonAF = 0x00010001,
		Completely_NonAF = 0x00010003
	};

	#define EDS_CHECK(v) \
		retval = (Result)(v);	\
		if (retval != Result::OK) { \
			GlobalMutex.unlock(); \
			return retval; \
		}

	#define EDS_CHECK_RET(v) \
		{ Result retval = (Result)(v);	\
		if (retval != Result::OK) { \
			return retval; \
		} }

	#define EDS_LOCK_AND_RET(methods, before_ret) \
		Result retval;	\
		GlobalMutex.lock();	\
		{ methods; } \
		GlobalMutex.unlock();	\
		{ before_ret; } \
		return retval;

	#define EDS_SESSION_LOCK_AND_RET(methods, before_ret) \
		if (!SessionOpen_) { \
			return Result::SESSION_NOT_OPEN; \
		} \
		EDS_LOCK_AND_RET(methods, before_ret)

	typedef class ICameraEventHandler {
	public:
		virtual void OnDownloadComplete(const std::string &filename) = 0;
	} *ICameraEventHandlerPtr;

	typedef class Camera {
	private:
		LPVOID Ref_;
		EdsDeviceInfo Info_;
		DWORD Error_;
		bool SessionOpen_;
		std::string Path_;
		static std::unordered_map<LPVOID, Camera *> RefMapping_;
		ICameraEventHandlerPtr EventHandler_;
	
	public:
		LPVOID GetRef() const { return Ref_; }
		const EdsDeviceInfo &GetInfo() const { return Info_; }
		DWORD GetError() const { return Error_; }
		std::string GetPath() const { return Path_; }

		void SetPath(const std::string &path) {
			this->Path_ = path;
			if (!Utility::DirectoryExists(path)) {
				CreateDirectory(path.c_str(), NULL);
			}
		}

	public:
		Result TakePicture() {
			EDS_LOCK_AND_RET(
				EDS_CHECK(EdsSendCommand(this->Ref_, CameraCommand_TakePicture, 0)), { });
		}

		Result PressShutterButton(ShutterButton behavior) {
			EDS_LOCK_AND_RET(
				EDS_CHECK(EdsSendCommand(this->Ref_, CameraCommand_PressShutterButton, (DWORD)behavior)), {});
		}

		Result OpenSession() {
			if (SessionOpen_) {
				return Result::OK;
			}
			EDS_LOCK_AND_RET({
				EDS_CHECK(EdsOpenSession(this->Ref_));
				EDS_CHECK(EdsSetCameraStateEventHandler(this->Ref_, StateEvent_All, Camera::StateEvent, this));
				EDS_CHECK(EdsSetObjectEventHandler(this->Ref_, ObjectEvent_All, Camera::ObjectEvent, this));
				EDS_CHECK(EdsSetPropertyEventHandler(this->Ref_, PropertyEvent_All, Camera::PropertyEvent, this));
			}, {
				SessionOpen_ = (retval == Result::OK);
			});
		}

		Result CloseSession() {
			if (!SessionOpen_) {
				return Result::OK;
			}
			EDS_LOCK_AND_RET({
				EDS_CHECK(EdsSetCameraStateEventHandler(this->Ref_, StateEvent_All, nullptr, this));
				EDS_CHECK(EdsSetObjectEventHandler(this->Ref_, ObjectEvent_All, nullptr, this));
				EDS_CHECK(EdsSetPropertyEventHandler(this->Ref_, PropertyEvent_All, nullptr, this));
				EDS_CHECK(EdsCloseSession(this->Ref_))
			}, {
				SessionOpen_ = (retval != Result::OK);
			});
		}

		template <typename T>
		Result SetSetting(DWORD PropID, T Value) {
			DWORD proptype;
			INT propsize;
			EDS_SESSION_LOCK_AND_RET({
				EDS_CHECK(EdsGetPropertySize(this->Ref_, PropID, 0, &proptype, &propsize));
				if (propsize != sizeof(T)) {
					GlobalMutex.unlock();
					return Result::FILE_DATA_CORRUPT;
				}
				EDS_CHECK(EdsSetPropertyData(this->Ref_, PropID, 0, propsize, &Value));
			}, {});
		}

		template <typename T = DWORD>
		T GetSetting(DWORD PropID) const {
			T retval;
			INT propsize;
			DWORD proptype;
			if (this->SessionOpen_) {
				GlobalMutex.lock();
				EdsGetPropertySize(this->Ref_, PropID, 0, &proptype, &propsize);
				if (propsize == sizeof(T)) {
					EdsGetPropertyData(this->Ref_, PropID, 0, propsize, &retval);
				}
				GlobalMutex.unlock();
			}
			return retval;
		}

		std::vector<int> GetSettingInt32Array(DWORD PropID) const {
			std::vector<int> retval;
			INT propsize;
			DWORD proptype;
			if (this->SessionOpen_) {
				GlobalMutex.lock();
				EdsGetPropertySize(this->Ref_, PropID, 0, &proptype, &propsize);
				if ((proptype == (DWORD)EdsDataType::UInt32_Array) ||
					(proptype == (DWORD)EdsDataType::Int32_Array)) {
					int count = propsize / sizeof(int);
					int *buffer = new int[count];
					EdsGetPropertyData(this->Ref_, PropID, 0, propsize, buffer);
					for (int i = 0; i < count; i++) {
						retval.push_back(buffer[i]);
					}
					delete[] buffer;
				}
				GlobalMutex.unlock();
			}
			return retval;
		}

		Result SetCapacity() {
			EdsCapacity capacity;
			capacity.Reset = 1;
			capacity.BytesPerSector = 0x1000;
			capacity.NumberOfFreeClusters = 0x7FFFFFFF;
			EDS_SESSION_LOCK_AND_RET(
				EDS_CHECK(EdsSetCapacity(this->Ref_, capacity)), { });
		}

		std::vector<DWORD> GetSettingsList(DWORD PropID) const {
			std::vector<DWORD> retval;
			if (!this->SessionOpen_) {
				return retval;
			}
			if (PropID == PropID_AEModeSelect || 
				PropID == PropID_ISOSpeed || 
				PropID == PropID_Av || 
				PropID == PropID_Tv || 
				PropID == PropID_MeteringMode || 
				PropID == PropID_ExposureCompensation) {
				EdsPropertyDesc des;
				GlobalMutex.lock();
				auto result = EdsGetPropertyDesc(this->Ref_, PropID, &des);
				GlobalMutex.unlock();
				if (result == EDS_ERR_OK) {
					for (int i = 0; i < des.NumElements; i++) {
						retval.push_back(des.PropDesc[i]);
					}
				}
			}
			return retval;
		}

	private:
		Result DownloadData(LPVOID ObjectPointer, LPVOID stream) {
			EdsDirectoryItemInfo dirInfo;
			EdsGetDirectoryItemInfo(ObjectPointer, &dirInfo);
			EDS_SESSION_LOCK_AND_RET({
				try {
					EDS_CHECK(EdsSetProgressCallback(stream, this->ProgressCallbackEvent,
						(DWORD)ProgressOption::Periodically, this));
					EDS_CHECK(EdsDownload(ObjectPointer, dirInfo.Size, stream));
				}
				catch (...) {
				}
				EDS_CHECK(EdsDownloadComplete(ObjectPointer));
				EDS_CHECK(EdsRelease(ObjectPointer));
			}, {
			});
		}

		Result DownloadImage(LPVOID inRef) {
			EdsDirectoryItemInfo dirInfo;
			LPVOID streamRef;
			EDS_SESSION_LOCK_AND_RET({
				EDS_CHECK(EdsGetDirectoryItemInfo(inRef, &dirInfo));
				std::string f = Utility::PathCombine(this->Path_, dirInfo.szFileName);
				EDS_CHECK(EdsCreateFileStream((LPSTR)f.c_str(),
					(DWORD)FileCreateDisposition::CreateAlways, 
					(DWORD)Access::ReadWrite, &streamRef));
				this->DownloadData(inRef, streamRef);
				EDS_CHECK(EdsRelease(streamRef));
				if (this->EventHandler_ != nullptr) {
					this->EventHandler_->OnDownloadComplete(f);
				}
			}, {
			});
		}

	private:

		void ProgressChanged(int Progress) {
		}

		static DWORD WINAPI ProgressCallbackEvent(DWORD inPercent, LPVOID inContext, bool &outCancel) {
			((Camera *)inContext)->ProgressChanged((int)inPercent);
			return (DWORD)Result::OK;
		}

		static DWORD WINAPI ObjectEvent(DWORD inEvent, LPVOID inRef, LPVOID inContext) {
			switch (inEvent)
			{
			case ObjectEvent_All:
				break;
			case ObjectEvent_DirItemCancelTransferDT:
				break;
			case ObjectEvent_DirItemContentChanged:
				break;
			case ObjectEvent_DirItemCreated:
				// if (DownloadVideo) { DownloadImage(inRef, ImageSaveDirectory); DownloadVideo = false; }
				break;
			case ObjectEvent_DirItemInfoChanged:
				break;
			case ObjectEvent_DirItemRemoved:
				break;
			case ObjectEvent_DirItemRequestTransfer:
				{
					auto me = (Camera *)inContext;
					if (me->DownloadImage(inRef) != Result::OK) {
						if (me->EventHandler_ != nullptr) {
							me->EventHandler_->OnDownloadComplete("");
						}
					}
					break;
				}
			case ObjectEvent_DirItemRequestTransferDT:
				break;
			case ObjectEvent_FolderUpdateItems:
				break;
			case ObjectEvent_VolumeAdded:
				break;
			case ObjectEvent_VolumeInfoChanged:
				break;
			case ObjectEvent_VolumeRemoved:
				break;
			case ObjectEvent_VolumeUpdateItems:
				break;
			}
			return EDS_ERR_OK;
		}

		static DWORD WINAPI PropertyEvent(DWORD inEvent, DWORD inPropertyID, DWORD inParameter, LPVOID inContext)
		{
			switch (inEvent)
			{
			case PropertyEvent_All:
				break;
			case PropertyEvent_PropertyChanged:
				break;
			case PropertyEvent_PropertyDescChanged:
				break;
			}

			switch (inPropertyID)
			{
			case PropID_AEBracket:
				break;
			case PropID_AEMode:
				break;
			case PropID_AEModeSelect:
				break;
			case PropID_AFMode:
				break;
			case PropID_Artist:
				break;
			case PropID_AtCapture_Flag:
				break;
			case PropID_Av:
				break;
			case PropID_AvailableShots:
				break;
			case PropID_BatteryLevel:
				break;
			case PropID_BatteryQuality:
				break;
			case PropID_BodyIDEx:
				break;
			case PropID_Bracket:
				break;
			case PropID_CFn:
				break;
			case PropID_ClickWBPoint:
				break;
			case PropID_ColorMatrix:
				break;
			case PropID_ColorSaturation:
				break;
			case PropID_ColorSpace:
				break;
			case PropID_ColorTemperature:
				break;
			case PropID_ColorTone:
				break;
			case PropID_Contrast:
				break;
			case PropID_Copyright:
				break;
			case PropID_DateTime:
				break;
			case PropID_DepthOfField:
				break;
			case PropID_DigitalExposure:
				break;
			case PropID_DriveMode:
				break;
			case PropID_EFCompensation:
				break;
			case PropID_Evf_AFMode:
				break;
			case PropID_Evf_ColorTemperature:
				break;
			case PropID_Evf_DepthOfFieldPreview:
				break;
			case PropID_Evf_FocusAid:
				break;
			case PropID_Evf_Histogram:
				break;
			case PropID_Evf_HistogramStatus:
				break;
			case PropID_Evf_ImagePosition:
				break;
			case PropID_Evf_Mode:
				break;
			case PropID_Evf_OutputDevice:
				//if (IsLiveViewOn == true) DownloadEvf();
				break;
			case PropID_Evf_WhiteBalance:
				break;
			case PropID_Evf_Zoom:
				break;
			case PropID_Evf_ZoomPosition:
				break;
			case PropID_ExposureCompensation:
				break;
			case PropID_FEBracket:
				break;
			case PropID_FilterEffect:
				break;
			case PropID_FirmwareVersion:
				break;
			case PropID_FlashCompensation:
				break;
			case PropID_FlashMode:
				break;
			case PropID_FlashOn:
				break;
			case PropID_FocalLength:
				break;
			case PropID_FocusInfo:
				break;
			case PropID_GPSAltitude:
				break;
			case PropID_GPSAltitudeRef:
				break;
			case PropID_GPSDateStamp:
				break;
			case PropID_GPSLatitude:
				break;
			case PropID_GPSLatitudeRef:
				break;
			case PropID_GPSLongitude:
				break;
			case PropID_GPSLongitudeRef:
				break;
			case PropID_GPSMapDatum:
				break;
			case PropID_GPSSatellites:
				break;
			case PropID_GPSStatus:
				break;
			case PropID_GPSTimeStamp:
				break;
			case PropID_GPSVersionID:
				break;
			case PropID_HDDirectoryStructure:
				break;
			case PropID_ICCProfile:
				break;
			case PropID_ImageQuality:
				break;
			case PropID_ISOBracket:
				break;
			case PropID_ISOSpeed:
				break;
			case PropID_JpegQuality:
				break;
			case PropID_LensName:
				break;
			case PropID_LensStatus:
				break;
			case PropID_Linear:
				break;
			case PropID_MakerName:
				break;
			case PropID_MeteringMode:
				break;
			case PropID_NoiseReduction:
				break;
			case PropID_Orientation:
				break;
			case PropID_OwnerName:
				break;
			case PropID_ParameterSet:
				break;
			case PropID_PhotoEffect:
				break;
			case PropID_PictureStyle:
				break;
			case PropID_PictureStyleCaption:
				break;
			case PropID_PictureStyleDesc:
				break;
			case PropID_ProductName:
				break;
			case PropID_Record:
				break;
			case PropID_RedEye:
				break;
			case PropID_SaveTo:
				break;
			case PropID_Sharpness:
				break;
			case PropID_ToneCurve:
				break;
			case PropID_ToningEffect:
				break;
			case PropID_Tv:
				break;
			case PropID_Unknown:
				break;
			case PropID_WBCoeffs:
				break;
			case PropID_WhiteBalance:
				break;
			case PropID_WhiteBalanceBracket:
				break;
			case PropID_WhiteBalanceShift:
				break;
			}
			return EDS_ERR_OK;
		}

		static DWORD WINAPI StateEvent(DWORD inEvent, DWORD inParameter, LPVOID inContext)
		{
			switch (inEvent)
			{
			case StateEvent_All:
				break;
			case StateEvent_AfResult:
				break;
			case StateEvent_BulbExposureTime:
				break;
			case StateEvent_CaptureError:
				break;
			case StateEvent_InternalError:
				break;
			case StateEvent_JobStatusChanged:
				break;
			case StateEvent_Shutdown:
				((Camera *)inContext)->SessionOpen_ = false;
				break;
			case StateEvent_ShutDownTimerUpdate:
				break;
			case StateEvent_WillSoonShutDown:
				break;
			}
			return EDS_ERR_OK;
		}

	public:
		void SetSaveTo(SaveTo save_to) {
			this->SetSetting(PropID_SaveTo, (DWORD)save_to);
		}

		void SetSaveTo(const std::string &path = Utility::GetSystemFolder()) {
			this->SetSaveTo(SaveTo::Host);
			this->SetCapacity();
			this->SetPath(path);
		}

		std::vector<DWORD> GetAvailableAEModes() const { return GetSettingsList(PropID_AEModeSelect); }
		std::vector<DWORD> GetAvailableISOSpeeds() const { return GetSettingsList(PropID_ISOSpeed); }
		std::vector<DWORD> GetAvailableAvs() const { return GetSettingsList(PropID_Av); }
		std::vector<DWORD> GetAvailableTvs() const { return GetSettingsList(PropID_Tv); }
		std::vector<DWORD> GetAvailableMeteringMode() const { return GetSettingsList(PropID_MeteringMode); }
		std::vector<DWORD> GetAvailableExposureCompensation() const { return GetSettingsList(PropID_ExposureCompensation); }

		DWORD GetAv() const { return this->GetSetting(PropID_Av); }
		void SetAv(DWORD value) { this->SetSetting(PropID_Av, value); }

		DWORD GetISOSpeed() const { return this->GetSetting(PropID_ISOSpeed); }
		void SetISOSpeed(DWORD value) { this->SetSetting(PropID_ISOSpeed, value); }

		DWORD GetTv() const { return this->GetSetting(PropID_Tv); }
		void SetTv(DWORD value) { this->SetSetting(PropID_Tv, value); }

		AEMode GetAEMode() const { return (AEMode)this->GetSetting(PropID_AEMode); }

		ImageQuality GetImageQuality() const { return (ImageQuality)this->GetSetting(PropID_ImageQuality); }
		void SetImageQuality(ImageQuality value) { this->SetSetting(PropID_ImageQuality, (DWORD)value); }

		WhiteBalance GetWhiteBalance() const { return (WhiteBalance)this->GetSetting(PropID_WhiteBalance); }
		void SetWhiteBalance(WhiteBalance value) { this->SetSetting(PropID_WhiteBalance, (DWORD)value); }

		WhiteBalanceShift GetWhiteBalanceShift() const { return this->GetSetting<WhiteBalanceShift>(PropID_WhiteBalanceShift); }
		void SetWhiteBalanceShift(WhiteBalanceShift value) { this->SetSetting(PropID_WhiteBalanceShift, value); }

		DWORD GetColorTemperature() const { return this->GetSetting(PropID_ColorTemperature); }
		void SetColorTemperature(DWORD value) { this->SetSetting(PropID_ColorTemperature, value); }
		
		DWORD GetPictureStyle() const { return this->GetSetting(PropID_PictureStyle); }
		void SetPictureStyle(PictureSytle value) { this->SetSetting(PropID_PictureStyle, (DWORD)value); }

	private:
		HANDLE AsyncThread_;
		typedef void(Camera::*AsyncCallbackPtr)();
		AsyncCallbackPtr AsyncCallback_;

		static DWORD WINAPI AsyncEvent(LPVOID lpThreadParameter) {
			Camera *me = (Camera *)lpThreadParameter;
			if (me->AsyncCallback_ != nullptr) {
				(me->*(me->AsyncCallback_))();
			}
			CloseHandle(me->AsyncThread_);
			return 0;
		}

	public:
		void AutoFocus() {
			PressShutterButton(ShutterButton::Halfway);
			PressShutterButton(ShutterButton::Off);
		}

		void PressShutterWithoutAF() {
			PressShutterButton(ShutterButton::Completely_NonAF);
			PressShutterButton(ShutterButton::Off);
		}

		void Async(AsyncCallbackPtr func) {
			if (AsyncThread_ != NULL) {
				WaitForSingleObject(AsyncThread_, INFINITE);
			};
			AsyncCallback_ = func;
			AsyncThread_ = CreateThread(NULL, 0, AsyncEvent, this, 0, NULL);
		}

		void BindEventHandler(ICameraEventHandlerPtr handler) {
			this->EventHandler_ = handler;
		}

	public:
		std::string ToString() const {
			return std::string(Info_.szPortName) + " " + Info_.szDeviceDescription;
		}

		Camera(LPVOID ref) : 
			SessionOpen_(false), AsyncThread_(NULL), 
			AsyncCallback_(nullptr), EventHandler_(nullptr) {
			if (ref == nullptr) {
				throw "Camera pointer is zero";
			}
			this->Ref_ = ref;
			this->Error_ = EdsGetDeviceInfo(ref, &Info_);
			if (this->Error_ != EDS_ERR_OK) {
				throw "Cannot get camera info";
			}
			this->Path_ = Utility::GetSystemFolder();
			GlobalMutex.lock();
			Camera::RefMapping_.insert_or_assign(this->Ref_, this);
			GlobalMutex.unlock();
		}

		~Camera() {
			GlobalMutex.lock();
			Camera::RefMapping_.erase(this->Ref_);
			GlobalMutex.unlock();
		}

		static Camera *Retrieve(LPVOID ref) {
			auto v = Camera::RefMapping_.find(ref);
			if (v == Camera::RefMapping_.end()) {
				return new Camera(ref);
			} else {
				return v->second;
			}
		}

	} *CameraPtr;

	std::unordered_map<LPVOID, Camera *> Camera::RefMapping_;

	std::vector<CameraPtr> GetCameraList() {
		std::vector<CameraPtr> retval;
		LPVOID camlist;
		if (!LibraryModule.Initialized) {
			return retval;
		}
		if (EdsGetCameraList(&camlist) != EDS_ERR_OK) {
			return retval;
		}
		INT c;
		if (EdsGetChildCount(camlist, &c) != EDS_ERR_OK) {
			return retval;
		}
		for (int i = 0; i < c; i++) {
			LPVOID cptr;
			if (EdsGetChildAtIndex(camlist, i, &cptr) != EDS_ERR_OK) {
				return retval;
			}
			retval.push_back(Camera::Retrieve(cptr));
		}
		return retval;
	}
}