#pragma once

#include "common.h"
#include "netstream.h"

namespace FreezeTimePro {

	enum class PackageType {
		Register = 'IGER',
		ChooseCamera = 'MCHC',
		Ping = 'GNIP',
		PingReply = 'PRGP',
		Snapshot = 'PANS',
		SnapshotReply = 'PRPS',
		AutoFocus = 'SFTA',
		Update = 'TDPU',
		CheckUpdate = 'DUKC'
	};

}