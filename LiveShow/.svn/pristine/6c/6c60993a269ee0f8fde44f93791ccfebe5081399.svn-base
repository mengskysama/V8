#include "LockIP.h"

void LockIPPacket::Pack()
{
	HeadBusiness();
	*this<<stEvent.unDstUIN<<stEvent.strReason<<(uint16)(stEvent.unTime*60);
	ClientPacket::SetAttachData(stEvent.unDstUIN);
	TailBusiness();
}

//////////////////////////////////////////////////////////////////////////

void LockIPNotifyPacket::Unpack()
{
	ServerPacket::Unpack();
	uint16 unTime;
	*this>>stEvent.unSrcUIN>>stEvent.unDstUIN>>stEvent.strReason>>unTime;
	stEvent.unTime = unTime/60;
}

//////////////////////////////////////////////////////////////////////////
