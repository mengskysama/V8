#include "QueryClientInfo.h"
#include "ReportClientInfo.h"

void QueryClientInfoPacket::Pack()
{
	HeadBusiness();

	*this<<stEvent.unDstUIN;
	uint8 count=0;
	if(stEvent.bVideoInfo) count++;
	if(stEvent.bNetInfo) count++;
	if(stEvent.bHardwareInfo) count++;
	*this<<count;
	if(stEvent.bVideoInfo)
	{
		*this<<(uint8)ReportClientInfoPacket::REPORTTYPE_VIDEO;
	}
	if(stEvent.bNetInfo)
	{
		*this<<(uint8)ReportClientInfoPacket::REPORTTYPE_NETSPEED;
	}
	if(stEvent.bHardwareInfo)
	{
		*this<<(uint8)ReportClientInfoPacket::REPORTTYPE_HARDWARE;
	}

	TailBusiness();
}

//////////////////////////////////////////////////////////////////////////

void QueryClientInfoRspPacket::Unpack()
{
	ServerPacket::Unpack();
	uint8 count=0;
	*this>>count;
	stEvent.bVideoInfo=false;
	stEvent.bNetInfo=false;
	stEvent.bHardwareInfo=false;
	while(count-- > 0)
	{
		uint8 type=0;
		*this>>type;
		if(type == ReportClientInfoPacket::REPORTTYPE_VIDEO)
		{
			stEvent.bVideoInfo=true;
			uint8 unVedio=0;
			*this>>unVedio;
			stEvent.bHaveVideo = unVedio!=0;
		}
		else if(type == ReportClientInfoPacket::REPORTTYPE_NETSPEED)
		{
			stEvent.bNetInfo=true;			
			*this>>stEvent.unPacketLost>>stEvent.unTimeout;
		}
		else if(type == ReportClientInfoPacket::REPORTTYPE_HARDWARE)
		{
			stEvent.bHardwareInfo=true;			
			*this>>stEvent.strCPUType>>stEvent.unCPUUse>>stEvent.unMemSize>>stEvent.unMemUse;
		}
	}
}

//////////////////////////////////////////////////////////////////////////
