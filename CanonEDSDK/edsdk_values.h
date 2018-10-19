#pragma once

#include <string>
#include <Windows.h>

namespace EDSDK {
	class CameraValues {
	public:
		static std::string AV(DWORD v)
		{
			switch (v)
			{
			case 0x00:
				return "Auto";
			case 0x08:
				return "1";
			case 0x40:
				return "11";
			case 0x0B:
				return "1.1";
			case 0x43:
				return "13 (1/3)";
			case 0x0C:
				return "1.2";
			case 0x44:
				return "13";
			case 0x0D:
				return "1.2 (1/3)";
			case 0x45:
				return "14";
			case 0x10:
				return "1.4";
			case 0x48:
				return "16";
			case 0x13:
				return "1.6";
			case 0x4B:
				return "18";
			case 0x14:
				return "1.8";
			case 0x4C:
				return "19";
			case 0x15:
				return "1.8 (1/3)";
			case 0x4D:
				return "20";
			case 0x18:
				return "2";
			case 0x50:
				return "22";
			case 0x1B:
				return "2.2";
			case 0x53:
				return "25";
			case 0x1C:
				return "2.5";
			case 0x54:
				return "27";
			case 0x1D:
				return "2.5 (1/3)";
			case 0x55:
				return "29";
			case 0x20:
				return "2.8";
			case 0x58:
				return "32";
			case 0x23:
				return "3.2";
			case 0x5B:
				return "36";
			case 0x24:
				return "3.5";
			case 0x5C:
				return "38";
			case 0x25:
				return "3.5 (1/3)";
			case 0x5D:
				return "40";
			case 0x28:
				return "4";
			case 0x60:
				return "45";
			case 0x2B:
				return "4.5";
			case 0x63:
				return "51";
			case 0x2C:
				return "4.5 (1/3)";
			case 0x64:
				return "54";
			case 0x2D:
				return "5.0";
			case 0x65:
				return "57";
			case 0x30:
				return "5.6";
			case 0x68:
				return "64";
			case 0x33:
				return "6.3";
			case 0x6B:
				return "72";
			case 0x34:
				return "6.7";
			case 0x6C:
				return "76";
			case 0x35:
				return "7.1";
			case 0x6D:
				return "80";
			case 0x38:
				return "8";
			case 0x70:
				return "91";
			case 0x3B:
				return "9";
			case 0x3C:
				return "9.5";
			case 0x3D:
				return "10";

			case 0xffffffff:
			default:
				return "N/A";
			}
		}

		/// <summary>
		/// Gets the ISO std::string value from an ISO ID
		/// </summary>
		/// <param name="v">The ISO ID</param>
		/// <returns>the ISO std::string</returns>
		static std::string ISO(DWORD v)
		{
			switch (v)
			{
			case 0x00000000:
				return "Auto ISO";
			case 0x00000028:
				return "ISO 6";
			case 0x00000030:
				return "ISO 12";
			case 0x00000038:
				return "ISO 25";
			case 0x00000040:
				return "ISO 50";
			case 0x00000048:
				return "ISO 100";
			case 0x0000004b:
				return "ISO 125";
			case 0x0000004d:
				return "ISO 160";
			case 0x00000050:
				return "ISO 200";
			case 0x00000053:
				return "ISO 250";
			case 0x00000055:
				return "ISO 320";
			case 0x00000058:
				return "ISO 400";
			case 0x0000005b:
				return "ISO 500";
			case 0x0000005d:
				return "ISO 640";
			case 0x00000060:
				return "ISO 800";
			case 0x00000063:
				return "ISO 1000";
			case 0x00000065:
				return "ISO 1250";
			case 0x00000068:
				return "ISO 1600";
			case 0x00000070:
				return "ISO 3200";
			case 0x00000078:
				return "ISO 6400";
			case 0x00000080:
				return "ISO 12800";
			case 0x00000088:
				return "ISO 25600";
			case 0x00000090:
				return "ISO 51200";
			case 0x00000098:
				return "ISO 102400";
			case 0xffffffff:
			default:
				return "N/A";
			}
		}

		/// <summary>
		/// Gets the Tv std::string value from an Tv ID
		/// </summary>
		/// <param name="v">The Tv ID</param>
		/// <returns>the Tv std::string</returns>
		static std::string TV(DWORD v)
		{
			switch (v)
			{
			case 0x00:
				return "Auto";
			case 0x0C:
				return "Bulb";
			case 0x5D:
				return "1/25";
			case 0x10:
				return "30\"";
			case 0x60:
				return "1/30";
			case 0x13:
				return "25\"";
			case 0x63:
				return "1/40";
			case 0x14:
				return "20\"";
			case 0x64:
				return "1/45";
			case 0x15:
				return "20\" (1/3)";
			case 0x65:
				return "1/50";
			case 0x18:
				return "15\"";
			case 0x68:
				return "1/60";
			case 0x1B:
				return "13\"";
			case 0x6B:
				return "1/80";
			case 0x1C:
				return "10\"";
			case 0x6C:
				return "1/90";
			case 0x1D:
				return "10\" (1/3)";
			case 0x6D:
				return "1/100";
			case 0x20:
				return "8\"";
			case 0x70:
				return "1/125";
			case 0x23:
				return "6\" (1/3)";
			case 0x73:
				return "1/160";
			case 0x24:
				return "6\"";
			case 0x74:
				return "1/180";
			case 0x25:
				return "5\"";
			case 0x75:
				return "1/200";
			case 0x28:
				return "4\"";
			case 0x78:
				return "1/250";
			case 0x2B:
				return "3\"2";
			case 0x7B:
				return "1/320";
			case 0x2C:
				return "3\"";
			case 0x7C:
				return "1/350";
			case 0x2D:
				return "2\"5";
			case 0x7D:
				return "1/400";
			case 0x30:
				return "2\"";
			case 0x80:
				return "1/500";
			case 0x33:
				return "1\"6";
			case 0x83:
				return "1/640";
			case 0x34:
				return "1\"5";
			case 0x84:
				return "1/750";
			case 0x35:
				return "1\"3";
			case 0x85:
				return "1/800";
			case 0x38:
				return "1\"";
			case 0x88:
				return "1/1000";
			case 0x3B:
				return "0\"8";
			case 0x8B:
				return "1/1250";
			case 0x3C:
				return "0\"7";
			case 0x8C:
				return "1/1500";
			case 0x3D:
				return "0\"6";
			case 0x8D:
				return "1/1600";
			case 0x40:
				return "0\"5";
			case 0x90:
				return "1/2000";
			case 0x43:
				return "0\"4";
			case 0x93:
				return "1/2500";
			case 0x44:
				return "0\"3";
			case 0x94:
				return "1/3000";
			case 0x45:
				return "0\"3 (1/3)";
			case 0x95:
				return "1/3200";
			case 0x48:
				return "1/4";
			case 0x98:
				return "1/4000";
			case 0x4B:
				return "1/5";
			case 0x9B:
				return "1/5000";
			case 0x4C:
				return "1/6";
			case 0x9C:
				return "1/6000";
			case 0x4D:
				return "1/6 (1/3)";
			case 0x9D:
				return "1/6400";
			case 0x50:
				return "1/8";
			case 0xA0:
				return "1/8000";
			case 0x53:
				return "1/10 (1/3)";
			case 0x54:
				return "1/10";
			case 0x55:
				return "1/13";
			case 0x58:
				return "1/15";
			case 0x5B:
				return "1/20 (1/3)";
			case 0x5C:
				return "1/20";
			case 0xffffffff:
			default:
				return "N/A";
			}
		}

		///////////////////////////////////////

#define EDSDK_VALUES_INVERSE_MAP(x,y) if (v == y) return x;

		static DWORD AV(const std::string &v)
		{
			EDSDK_VALUES_INVERSE_MAP(0x00,
				"Auto")
			EDSDK_VALUES_INVERSE_MAP(0x08,
				"1")
			EDSDK_VALUES_INVERSE_MAP(0x40,
				"11")
			EDSDK_VALUES_INVERSE_MAP(0x0B,
				"1.1")
			EDSDK_VALUES_INVERSE_MAP(0x43,
			"13 (1/3)")
			EDSDK_VALUES_INVERSE_MAP(0x0C,
				"1.2")
			EDSDK_VALUES_INVERSE_MAP(0x44,
				"13")
			EDSDK_VALUES_INVERSE_MAP(0x0D,
				"1.2 (1/3)")
			EDSDK_VALUES_INVERSE_MAP(0x45,
				"14")
			EDSDK_VALUES_INVERSE_MAP(0x10,
				"1.4")
			EDSDK_VALUES_INVERSE_MAP(0x48,
				"16")
			EDSDK_VALUES_INVERSE_MAP(0x13,
				"1.6")
			EDSDK_VALUES_INVERSE_MAP(0x4B,
				"18")
			EDSDK_VALUES_INVERSE_MAP(0x14,
				"1.8")
			EDSDK_VALUES_INVERSE_MAP(0x4C,
				"19")
			EDSDK_VALUES_INVERSE_MAP(0x15,
				"1.8 (1/3)")
			EDSDK_VALUES_INVERSE_MAP(0x4D,
				"20")
			EDSDK_VALUES_INVERSE_MAP(0x18,
				"2")
			EDSDK_VALUES_INVERSE_MAP(0x50,
				"22")
			EDSDK_VALUES_INVERSE_MAP(0x1B,
				"2.2")
			EDSDK_VALUES_INVERSE_MAP(0x53,
				"25")
			EDSDK_VALUES_INVERSE_MAP(0x1C,
				"2.5")
			EDSDK_VALUES_INVERSE_MAP(0x54,
				"27")
			EDSDK_VALUES_INVERSE_MAP(0x1D,
				"2.5 (1/3)")
			EDSDK_VALUES_INVERSE_MAP(0x55,
				"29")
			EDSDK_VALUES_INVERSE_MAP(0x20,
				"2.8")
			EDSDK_VALUES_INVERSE_MAP(0x58,
				"32")
			EDSDK_VALUES_INVERSE_MAP(0x23,
				"3.2")
			EDSDK_VALUES_INVERSE_MAP(0x5B,
				"36")
			EDSDK_VALUES_INVERSE_MAP(0x24,
				"3.5")
			EDSDK_VALUES_INVERSE_MAP(0x5C,
				"38")
			EDSDK_VALUES_INVERSE_MAP(0x25,
				"3.5 (1/3)")
			EDSDK_VALUES_INVERSE_MAP(0x5D,
				"40")
			EDSDK_VALUES_INVERSE_MAP(0x28,
				"4")
			EDSDK_VALUES_INVERSE_MAP(0x60,
				"45")
			EDSDK_VALUES_INVERSE_MAP(0x2B,
				"4.5")
			EDSDK_VALUES_INVERSE_MAP(0x63,
				"51")
			EDSDK_VALUES_INVERSE_MAP(0x2C,
				"4.5 (1/3)")
			EDSDK_VALUES_INVERSE_MAP(0x64,
				"54")
			EDSDK_VALUES_INVERSE_MAP(0x2D,
				"5.0")
			EDSDK_VALUES_INVERSE_MAP(0x65,
				"57")
			EDSDK_VALUES_INVERSE_MAP(0x30,
				"5.6")
			EDSDK_VALUES_INVERSE_MAP(0x68,
				"64")
			EDSDK_VALUES_INVERSE_MAP(0x33,
				"6.3")
			EDSDK_VALUES_INVERSE_MAP(0x6B,
				"72")
			EDSDK_VALUES_INVERSE_MAP(0x34,
				"6.7")
			EDSDK_VALUES_INVERSE_MAP(0x6C,
				"76")
			EDSDK_VALUES_INVERSE_MAP(0x35,
				"7.1")
			EDSDK_VALUES_INVERSE_MAP(0x6D,
				"80")
			EDSDK_VALUES_INVERSE_MAP(0x38,
				"8")
			EDSDK_VALUES_INVERSE_MAP(0x70,
				"91")
			EDSDK_VALUES_INVERSE_MAP(0x3B,
				"9")
			EDSDK_VALUES_INVERSE_MAP(0x3C,
				"9.5")
			EDSDK_VALUES_INVERSE_MAP(0x3D,
				"10")
			return -1;
		}

		/// <summary>
		/// Gets the ISO std,,string value from an ISO ID
		/// </summary>
		/// <param name="v">The ISO ID</param>
		/// <returns>the ISO std,,string</returns>
		static DWORD ISO(const std::string &v)
		{
			EDSDK_VALUES_INVERSE_MAP(0x00000000,
				"Auto ISO")
			EDSDK_VALUES_INVERSE_MAP(0x00000028,
				"ISO 6")
			EDSDK_VALUES_INVERSE_MAP(0x00000030,
				"ISO 12")
			EDSDK_VALUES_INVERSE_MAP(0x00000038,
				"ISO 25")
			EDSDK_VALUES_INVERSE_MAP(0x00000040,
				"ISO 50")
			EDSDK_VALUES_INVERSE_MAP(0x00000048,
				"ISO 100")
			EDSDK_VALUES_INVERSE_MAP(0x0000004b,
				"ISO 125")
			EDSDK_VALUES_INVERSE_MAP(0x0000004d,
				"ISO 160")
			EDSDK_VALUES_INVERSE_MAP(0x00000050,
				"ISO 200")
			EDSDK_VALUES_INVERSE_MAP(0x00000053,
				"ISO 250")
			EDSDK_VALUES_INVERSE_MAP(0x00000055,
				"ISO 320")
			EDSDK_VALUES_INVERSE_MAP(0x00000058,
				"ISO 400")
			EDSDK_VALUES_INVERSE_MAP(0x0000005b,
				"ISO 500")
			EDSDK_VALUES_INVERSE_MAP(0x0000005d,
				"ISO 640")
			EDSDK_VALUES_INVERSE_MAP(0x00000060,
				"ISO 800")
			EDSDK_VALUES_INVERSE_MAP(0x00000063,
				"ISO 1000")
			EDSDK_VALUES_INVERSE_MAP(0x00000065,
				"ISO 1250")
			EDSDK_VALUES_INVERSE_MAP(0x00000068,
				"ISO 1600")
			EDSDK_VALUES_INVERSE_MAP(0x00000070,
				"ISO 3200")
			EDSDK_VALUES_INVERSE_MAP(0x00000078,
				"ISO 6400")
			EDSDK_VALUES_INVERSE_MAP(0x00000080,
				"ISO 12800")
			EDSDK_VALUES_INVERSE_MAP(0x00000088,
				"ISO 25600")
			EDSDK_VALUES_INVERSE_MAP(0x00000090,
				"ISO 51200")
			EDSDK_VALUES_INVERSE_MAP(0x00000098,
				"ISO 102400")
			return -1;
		}

		/// <summary>
		/// Gets the Tv std,,string value from an Tv ID
		/// </summary>
		/// <param name="v">The Tv ID</param>
		/// <returns>the Tv std,,string</returns>
		static DWORD TV(const std::string &v)
		{
			EDSDK_VALUES_INVERSE_MAP(0x00,
				"Auto")
			EDSDK_VALUES_INVERSE_MAP(0x0C,
				"Bulb")
			EDSDK_VALUES_INVERSE_MAP(0x5D,
				"1/25")
			EDSDK_VALUES_INVERSE_MAP(0x10,
				"30\"")
			EDSDK_VALUES_INVERSE_MAP(0x60,
				"1/30")
			EDSDK_VALUES_INVERSE_MAP(0x13,
				"25\"")
			EDSDK_VALUES_INVERSE_MAP(0x63,
				"1/40")
			EDSDK_VALUES_INVERSE_MAP(0x14,
				"20\"")
			EDSDK_VALUES_INVERSE_MAP(0x64,
				"1/45")
			EDSDK_VALUES_INVERSE_MAP(0x15,
				"20\" (1/3)")
			EDSDK_VALUES_INVERSE_MAP(0x65,
				"1/50")
			EDSDK_VALUES_INVERSE_MAP(0x18,
				"15\"")
			EDSDK_VALUES_INVERSE_MAP(0x68,
				"1/60")
			EDSDK_VALUES_INVERSE_MAP(0x1B,
				"13\"")
			EDSDK_VALUES_INVERSE_MAP(0x6B,
				"1/80")
			EDSDK_VALUES_INVERSE_MAP(0x1C,
				"10\"")
			EDSDK_VALUES_INVERSE_MAP(0x6C,
				"1/90")
			EDSDK_VALUES_INVERSE_MAP(0x1D,
				"10\" (1/3)")
			EDSDK_VALUES_INVERSE_MAP(0x6D,
				"1/100")
			EDSDK_VALUES_INVERSE_MAP(0x20,
				"8\"")
			EDSDK_VALUES_INVERSE_MAP(0x70,
				"1/125")
			EDSDK_VALUES_INVERSE_MAP(0x23,
				"6\" (1/3)")
			EDSDK_VALUES_INVERSE_MAP(0x73,
				"1/160")
			EDSDK_VALUES_INVERSE_MAP(0x24,
				"6\"")
			EDSDK_VALUES_INVERSE_MAP(0x74,
				"1/180")
			EDSDK_VALUES_INVERSE_MAP(0x25,
				"5\"")
			EDSDK_VALUES_INVERSE_MAP(0x75,
				"1/200")
			EDSDK_VALUES_INVERSE_MAP(0x28,
				"4\"")
			EDSDK_VALUES_INVERSE_MAP(0x78,
				"1/250")
			EDSDK_VALUES_INVERSE_MAP(0x2B,
				"3\"2")
			EDSDK_VALUES_INVERSE_MAP(0x7B,
				"1/320")
			EDSDK_VALUES_INVERSE_MAP(0x2C,
				"3\"")
			EDSDK_VALUES_INVERSE_MAP(0x7C,
				"1/350")
			EDSDK_VALUES_INVERSE_MAP(0x2D,
				"2\"5")
			EDSDK_VALUES_INVERSE_MAP(0x7D,
				"1/400")
			EDSDK_VALUES_INVERSE_MAP(0x30,
				"2\"")
			EDSDK_VALUES_INVERSE_MAP(0x80,
				"1/500")
			EDSDK_VALUES_INVERSE_MAP(0x33,
				"1\"6")
			EDSDK_VALUES_INVERSE_MAP(0x83,
				"1/640")
			EDSDK_VALUES_INVERSE_MAP(0x34,
				"1\"5")
			EDSDK_VALUES_INVERSE_MAP(0x84,
				"1/750")
			EDSDK_VALUES_INVERSE_MAP(0x35,
				"1\"3")
			EDSDK_VALUES_INVERSE_MAP(0x85,
				"1/800")
			EDSDK_VALUES_INVERSE_MAP(0x38,
				"1\"")
			EDSDK_VALUES_INVERSE_MAP(0x88,
				"1/1000")
			EDSDK_VALUES_INVERSE_MAP(0x3B,
				"0\"8")
			EDSDK_VALUES_INVERSE_MAP(0x8B,
				"1/1250")
			EDSDK_VALUES_INVERSE_MAP(0x3C,
				"0\"7")
			EDSDK_VALUES_INVERSE_MAP(0x8C,
				"1/1500")
			EDSDK_VALUES_INVERSE_MAP(0x3D,
				"0\"6")
			EDSDK_VALUES_INVERSE_MAP(0x8D,
				"1/1600")
			EDSDK_VALUES_INVERSE_MAP(0x40,
				"0\"5")
			EDSDK_VALUES_INVERSE_MAP(0x90,
				"1/2000")
			EDSDK_VALUES_INVERSE_MAP(0x43,
				"0\"4")
			EDSDK_VALUES_INVERSE_MAP(0x93,
				"1/2500")
			EDSDK_VALUES_INVERSE_MAP(0x44,
				"0\"3")
			EDSDK_VALUES_INVERSE_MAP(0x94,
				"1/3000")
			EDSDK_VALUES_INVERSE_MAP(0x45,
				"0\"3 (1/3)")
			EDSDK_VALUES_INVERSE_MAP(0x95,
				"1/3200")
			EDSDK_VALUES_INVERSE_MAP(0x48,
				"1/4")
			EDSDK_VALUES_INVERSE_MAP(0x98,
				"1/4000")
			EDSDK_VALUES_INVERSE_MAP(0x4B,
				"1/5")
			EDSDK_VALUES_INVERSE_MAP(0x9B,
				"1/5000")
			EDSDK_VALUES_INVERSE_MAP(0x4C,
				"1/6")
			EDSDK_VALUES_INVERSE_MAP(0x9C,
				"1/6000")
			EDSDK_VALUES_INVERSE_MAP(0x4D,
				"1/6 (1/3)")
			EDSDK_VALUES_INVERSE_MAP(0x9D,
				"1/6400")
			EDSDK_VALUES_INVERSE_MAP(0x50,
				"1/8")
			EDSDK_VALUES_INVERSE_MAP(0xA0,
				"1/8000")
			EDSDK_VALUES_INVERSE_MAP(0x53,
				"1/10 (1/3)")
			EDSDK_VALUES_INVERSE_MAP(0x54,
				"1/10")
			EDSDK_VALUES_INVERSE_MAP(0x55,
				"1/13")
			EDSDK_VALUES_INVERSE_MAP(0x58,
				"1/15")
			EDSDK_VALUES_INVERSE_MAP(0x5B,
				"1/20 (1/3)")
			EDSDK_VALUES_INVERSE_MAP(0x5C,
				"1/20")
			return -1;
		}
	};
}