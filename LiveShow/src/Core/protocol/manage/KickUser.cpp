#include "KickUser.h"

void KickUserPacket::Pack()
{
	HeadBusiness();
	*this<<(uint8)stEvent.bKickAllVisitor;
	if(!stEvent.bKickAllVisitor)
	{
		*this<<stEvent.unDstUIN<<stEvent.strReason<<stEvent.unTime;
	}
	byte* p = new byte[8];
	*(uint32*)p = stEvent.bKickAllVisitor;
	*(uint32*)(p+4) = stEvent.unDstUIN;
	ClientPacket::SetAttachData((uint32)p);
	TailBusiness();
}

//////////////////////////////////////////////////////////////////////////

void KickUserRspPacket::Unpack()
{
	ServerPacket::Unpack();
	uint32 result=0;
	*this>>result;
	stEvent.enmKickResult = (core::ENM_KickResult)result;
	byte* p = (byte*)ClientPacket::GetAttachData(seq);
	if(p != NULL)
	{
		stEvent.bKickAllVisitor = (*(uint32*)p);
		stEvent.unDstUIN = (*(uint32*)(p+4));
		delete[] p;
	}
	else
	{
		assert(0);
		stEvent.unDstUIN = 0;
	}
	
}

//////////////////////////////////////////////////////////////////////////

void KickUserNotifyPacket::Unpack()
{
	ServerPacket::Unpack();

	uint8 kick_type=0;
	*this>>stEvent.unSrcUIN>>kick_type;
	stEvent.enmType = (core::KickUserNotifyEvent::ENM_KickType)kick_type;
	if(stEvent.enmType == core::KickUserNotifyEvent::KICKTYPE_ONE)
	{
		*this>>stEvent.unDstUIN;
		*this>>stEvent.strReason>>stEvent.unTime;
	}
}

//////////////////////////////////////////////////////////////////////////
