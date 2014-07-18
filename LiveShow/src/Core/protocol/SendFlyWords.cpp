#include "SendFlyWords.h"

void SendFlyWordsPacket::Pack()
{
	HeadBusiness();
	*this<<stEvent.unGiftID<<stEvent.strMessage;
	wchar_t* p = new wchar_t[stEvent.strMessage.size()+2];
	memcpy(p, stEvent.strMessage.c_str(), stEvent.strMessage.size()*sizeof(wchar_t));
	p[stEvent.strMessage.size()]=0;
	ClientPacket::SetAttachData((uint32)p);
	TailBusiness();
}

//////////////////////////////////////////////////////////////////////////

void SendFlyWordsRspPacket::Unpack()
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
		delete[] p;
	}
	else
	{
		assert(0);
	}	
}

//////////////////////////////////////////////////////////////////////////

void SendFlyWordsNotifyPacket::Unpack()
{
	ServerPacket::Unpack();

	*this>>stEvent.unGiftID>>stEvent.unSrcUIN>>stEvent.strName>>stEvent.un179id>>stEvent.strMessage;
}