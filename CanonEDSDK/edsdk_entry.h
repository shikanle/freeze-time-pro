#pragma once

#include <mutex>
#include <Windows.h>
#include "edsdk_api.h"

namespace EDSDK
{
	class LibraryModuleType {
	public:
		static HMODULE Module;
		bool Initialized;

	public:
		LibraryModuleType();
		~LibraryModuleType(); 
	};

	HMODULE LibraryModuleType::Module = LoadLibrary("EDSDK.dll");
	std::recursive_mutex GlobalMutex;

	#define EDSDK_LOADPROC(x) x##Pointer x = (x##Pointer)GetProcAddress(LibraryModuleType::Module, #x)
	EDSDK_LOADPROC(EdsInitializeSDK);
	EDSDK_LOADPROC(EdsTerminateSDK);
	EDSDK_LOADPROC(EdsRetain);
	EDSDK_LOADPROC(EdsRelease);
	EDSDK_LOADPROC(EdsGetChildCount);
	EDSDK_LOADPROC(EdsGetChildAtIndex);
	EDSDK_LOADPROC(EdsGetParent);
	EDSDK_LOADPROC(EdsGetPropertySize);
	EDSDK_LOADPROC(EdsGetPropertyData);
	EDSDK_LOADPROC(EdsSetPropertyData);
	EDSDK_LOADPROC(EdsGetPropertyDesc);
	EDSDK_LOADPROC(EdsGetCameraList);
	EDSDK_LOADPROC(EdsGetDeviceInfo);
	EDSDK_LOADPROC(EdsOpenSession);
	EDSDK_LOADPROC(EdsCloseSession);
	EDSDK_LOADPROC(EdsSendCommand);
	EDSDK_LOADPROC(EdsSendStatusCommand);
	EDSDK_LOADPROC(EdsSetCapacity);
	EDSDK_LOADPROC(EdsGetVolumeInfo);
	EDSDK_LOADPROC(EdsFormatVolume);
	EDSDK_LOADPROC(EdsGetDirectoryItemInfo);
	EDSDK_LOADPROC(EdsDeleteDirectoryItem);
	EDSDK_LOADPROC(EdsDownload);
	EDSDK_LOADPROC(EdsDownloadCancel);
	EDSDK_LOADPROC(EdsDownloadComplete);
	EDSDK_LOADPROC(EdsDownloadThumbnail);
	EDSDK_LOADPROC(EdsGetAttribute);
	EDSDK_LOADPROC(EdsSetAttribute);
	EDSDK_LOADPROC(EdsCreateFileStream);
	EDSDK_LOADPROC(EdsCreateMemoryStream);
	EDSDK_LOADPROC(EdsCreateStreamEx);
	EDSDK_LOADPROC(EdsCreateMemoryStreamFromPointer);
	EDSDK_LOADPROC(EdsGetPointer);
	EDSDK_LOADPROC(EdsRead);
	EDSDK_LOADPROC(EdsWrite);
	EDSDK_LOADPROC(EdsSeek);
	EDSDK_LOADPROC(EdsGetPosition);
	EDSDK_LOADPROC(EdsGetLength);
	EDSDK_LOADPROC(EdsCopyData);
	EDSDK_LOADPROC(EdsSetProgressCallback);
	EDSDK_LOADPROC(EdsCreateImageRef);
	EDSDK_LOADPROC(EdsGetImageInfo);
	EDSDK_LOADPROC(EdsGetImage);
	EDSDK_LOADPROC(EdsSaveImage);
	EDSDK_LOADPROC(EdsCacheImage);
	EDSDK_LOADPROC(EdsReflectImageProperty);
	EDSDK_LOADPROC(EdsSetCameraAddedHandler);
	EDSDK_LOADPROC(EdsSetPropertyEventHandler);
	EDSDK_LOADPROC(EdsSetObjectEventHandler);
	EDSDK_LOADPROC(EdsSetCameraStateEventHandler);
	EDSDK_LOADPROC(EdsDownloadEvfImage);
	EDSDK_LOADPROC(EdsGetEvent);
	#undef EDSDK_LOADPROC

	LibraryModuleType LibraryModule;

	LibraryModuleType::LibraryModuleType() {
		this->Initialized = false;
		if (LibraryModuleType::Module != NULL) {
			if (EdsInitializeSDK() == EDS_ERR_OK) {
				this->Initialized = true;
			}
		}
	}
	LibraryModuleType::~LibraryModuleType() {
		EdsTerminateSDK();
	}

}