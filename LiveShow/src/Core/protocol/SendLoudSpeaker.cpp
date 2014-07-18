#include "SendLoudSpeaker.h"

void SendLoudSpeakerPacket::Pack()
{
	HeadBusiness();
	*this<<(uint8)stEvent.enmType<<stEvent.unGiftID<<stEvent.strMessage;
	wchar_t* p = new wchar_t[stEvent.strMessage.size()+2];
	memcpy(p, stEvent.strMessage.c_str(), stEvent.strMessage.size()*sizeof(wchar_t));
	p[stEvent.strMessage.size()]=0;
	p[stEvent.strMessage.size()+1]=stEvent.enmType;
	ClientPacket::SetAttachData((uint32)p);
	TailBusiness();
}

//////////////////////////////////////////////////////////////////////////

void SendLoudSpeakerRspPacket::Unpack()
{
	ServerPacket::Unpack();

	uint32 result=0;
	*this>>result;	
	stEvent.enmResult = (core::ENM_SendGiftResult)result;
	if(stEvent.enmResult == core::SENDGIFTRESULT_OK)
	{
		*this>>stEvent.unLeftMoney;
		uint32 unTemp=0;
		*this>>unTemp>>unTemp>>stEvent.unRichUpdateMoney;
	}
	else
	{
		*this>>stEvent.strFailReason;
	}
	wchar_t* p = (wchar_t*)ClientPacket::GetAttachData(seq);
	if(p != NULL)
	{
		stEvent.strMessage = p;
		stEvent.enmType = (core::ENM_LoudSpeakerType)p[stEvent.strMessage.size()+1];
		delete[] p;
	}
	else
	{
		assert(0);
	}
	
}

//////////////////////////////////////////////////////////////////////////

void SendLoudSpeakerNotifyPacket::Unpack()
{
	ServerPacket::Unpack();

	uint8 untype;
	*this>>untype;
	stEvent.enmType = (core::ENM_LoudSpeakerType)untype;
	*this>>stEvent.unGiftID>>stEvent.unSrcUIN>>stEvent.strName>>stEvent.un179id>>stEvent.unVipLevel;
	uint16 count=0, itemid;
	*this>>count;
	while(count-- > 0)
	{
		*this>>itemid;
		stEvent.vecGiftID.push_back(itemid);
	}

	*this>>stEvent.unRichLevel;
	uint8 unMsgType=0;
	*this>>unMsgType>>stEvent.strMessage>>stEvent.unCache;

	stEvent.enmMessageType = (core::SendLoudSpeakerNotifyEvent::SpeakerMessageType)unMsgType;
}