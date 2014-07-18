#include "RequestShow.h"

void RequestShowPacket::Pack()
{
	HeadBusiness();
	*this<<(uint8)enmShowType;
	TailBusiness();
}

//////////////////////////////////////////////////////////////////////////

void RequestShowRspPacket::Unpack()
{
	ServerPacket::Unpack();

	uint32 result=0;
	*this>>result;
	stEvent.enmResult = (core::ENM_RequestShowResult)result;
	if(stEvent.enmResult == core::REQUESTSHOWRESULT_PUBLIC)
	{
		*this>>stEvent.unIndex>>stEvent.unTime;
	}
	else if(stEvent.enmResult == core::REQUESTSHOWRESULT_WAITTINGLIST)
	{
		uint8 nFreeShowOpened=0;
		*this>>nFreeShowOpened;
		stEvent.bFreeShowOpened = nFreeShowOpened != 0;
	}
}

//////////////////////////////////////////////////////////////////////////

void RequestShowNotifyPacket::Unpack()
{
	ServerPacket::Unpack();

	uint32 result=0;
	*this>>result>>stEvent.unSrcUIN;
	stEvent.enmResult = (core::ENM_RequestShowResult)result;
	if(stEvent.enmResult == core::REQUESTSHOWRESULT_PUBLIC)
	{
		*this>>stEvent.unIndex>>stEvent.unTime;
	}
}