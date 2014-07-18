#include "Login.h"
#include "..\TokenConnection.h"
#include "version.h"
#include "..\CoreModule.h"
#include "utility\SystemHelper.h"
#include "GGHelper.h"
#include "utility\func4Stat.h"

uint32 LoginPacket::GetChannelID()
{
	std::wstring strConfig = common::utility::systemhelper::Get179AppPath();
	strConfig += L"config\\Channel.ini";
	wchar_t sz[20]={0};
	GetPrivateProfileString(L"Channel",L"ChannelNumber",_T("0"),sz,19,strConfig.c_str());
	return _ttoi(sz);
}

uint32 LoginPacket::GetOsIEInfo()
{
	OSVERSIONINFO st;
	common::utility::systemhelper::GetOsVersion(st);

	uint32 nSysVersion = 100000*(st.dwPlatformId%10)+1000*(st.dwMajorVersion%100)+st.dwMinorVersion%1000;

	std::wstring strIEVer = common::utility::systemhelper::GetIEVersion();
	std::vector<std::wstring> vecDot = common::utility::stringhelper::Split_String_by_Delimiter(strIEVer, L'.');
	if(vecDot.size() < 2)
		return nSysVersion*1000;
	
	//"9.0.8112.16421" -> 90
	int nIEVersion = (_wtoi(vecDot[0].c_str()) % 100);
	nIEVersion *= 10;
	nIEVersion += (_wtoi(vecDot[1].c_str()) % 10);

	return (nSysVersion*1000+nIEVersion);
}

void LoginPacket::Pack()
{
	HeadBusiness();

	*this<<(uint8)CoreModule::GetCurInfo().enmAccountType<<(uint16)(TokenConnection::GetToken().first);

	WriteData(TokenConnection::GetToken().second, TokenConnection::GetToken().first);

	int nSizeBegin = Size();

	*this<<CoreModule::GetCurInfo().unUIN<<(uint8)0<<(uint32)CLIENT_VERSION;

	std::wstring strMacAddr;

	if(!GetRealMacAddress(strMacAddr))
	{
		get_hardware_info(strMacAddr);
	}	

	*this<<GetOsIEInfo()<<strMacAddr<<GetChannelID();

	uint16 nSizeEnd = Size();

	ByteBuf buf(nSizeEnd-nSizeBegin, data+nSizeBegin);

	Encode(buf);

	MoveCur(nSizeBegin+buf.first, Packet::from_begin);
}

//////////////////////////////////////////////////////////////////////////

void LoginRspPacket::Unpack()
{
	ServerPacket::Unpack();
	
	uint32 result;
	*this>>result;
	if(result == core::LOGIN_ERROR_SUCCESS)
	{
		*this>>stEvent.latestversion>>stEvent.lastlogin_time_high>>stEvent.lastlogin_time_low>>system_time_high>>system_time_low;
		lastlogin_time_high=stEvent.lastlogin_time_high;
		lastlogin_time_low=stEvent.lastlogin_time_low;
		uint8 unMan,unVipLevel,unDianxin;
		*this>>unMan>>_179name>>_179id>>unVipLevel>>usr_level>>online_time>>left_time>>leftMoney>>unDianxin>>unSelfSetting;
		bMan = unMan;
		vip_level = (core::ENM_VipLevel)unVipLevel;
		stEvent.enmLoginResult = core::LOGIN_ERROR_SUCCESS;
		bDianxin = (unDianxin==0);
		uint8 temp;
		*this>>temp;
		bRmbUser = (temp==1);
		*this>>temp;
		bPasswordSafe = (temp==1);
		*this>>rich_level>>rich_update_money>>unBusiness;

		common::utility::systemhelper::FilterNickName(_179name);
	}
	else
	{
		result += core::LOGIN_ERROR_179_ERROR_BEGIN;
		stEvent.enmLoginResult = (core::ENM_LoginErrorCode)result;
		*this>>stEvent.strFailReason;

		if(result == LOGIN_ERROR_179_ACCOUNT_LOCK ||
			result == LOGIN_ERROR_179_IP_LOCK ||
			result == LOGIN_ERROR_179_MAC_LOCK)
		{
			*this>>stEvent.lockDays>>stEvent.strUnlockDate;
		}
	}
}

//////////////////////////////////////////////////////////////////////////