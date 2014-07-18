#include "SendGift.h"
#include "utility\SystemHelper.h"

void SendGiftPacket::Pack()
{
	HeadBusiness();
	*this<<(uint8)stEvent.enmType<<(uint8)stEvent.enmDstType<<stEvent.unDstUIN<<stEvent.unGiftID<<stEvent.unCount;
	*this<<stEvent.strAttachMessage;
	byte* p = new byte[14];
	*(uint32*)p = stEvent.enmType;
	*(uint32*)(p+4) = stEvent.enmDstType;
	*(uint32*)(p+8) = stEvent.unDstUIN;
	*(uint16*)(p+12) = stEvent.unGiftID;
	ClientPacket::SetAttachData((uint32)p);
	TailBusiness();
}

//////////////////////////////////////////////////////////////////////////

void SendGiftRspPacket::Unpack()
{
	ServerPacket::Unpack();

	uint32 result=0;
	*this>>result;
	stEvent.enmResult = (core::ENM_SendGiftResult)result;
	if(stEvent.enmResult == core::SENDGIFTRESULT_OK)
	{
		uint8 unLuckyCount=0;
		uint32 unTemp=0;
		*this>>stEvent.unTime>>stEvent.unLeftMoney>>stEvent.unSentCount>>stEvent.strAttachMessage>>unTemp>>unTemp>>stEvent.unRichUpdateMoney>>unLuckyCount;
		while(unLuckyCount-- > 0)
		{
			core::SendGiftLuckyInfo info;
			*this>>info.unLuckyIndex>>info.unLuckyTimes>>info.unLuckyMoney;
			stEvent.vecLuckyInfo.push_back(info);
		}
	}
	else
	{
		*this>>stEvent.strFailReason;
	}
	byte* p = (byte*)ClientPacket::GetAttachData(seq);
	if(p != NULL)
	{
		stEvent.enmType = (core::ENM_GiftType)(*(uint32*)p);
		stEvent.enmDstType = (core::ENM_GiftDstType)(*(uint32*)(p+4));
		stEvent.unDstUIN = (*(uint32*)(p+8));
		stEvent.unGiftID = (*(uint16*)(p+12));
		delete[] p;
	}
	else
	{
		assert(0);
		stEvent.unDstUIN = 0;
	}
	
}

//////////////////////////////////////////////////////////////////////////

void SendGiftNotifyPacket::Unpack()
{
	ServerPacket::Unpack();

	uint8 unGiftType,unDstType;
	*this>>stEvent.unSrcUIN>>stEvent.unTime>>stEvent.unSrc179ID>>stEvent.strSrcName>>unGiftType>>unDstType;
	common::utility::systemhelper::FilterNickName(stEvent.strSrcName);
	if(stEvent.strSrcName.empty())
	{
		wchar_t sz[100];
		wsprintf(sz, L"%d", stEvent.unSrc179ID);
		stEvent.strSrcName = sz;
	}
	stEvent.enmType = (core::ENM_GiftType)unGiftType;
	stEvent.enmDstType = (core::ENM_GiftDstType)unDstType;
	if(stEvent.enmDstType == core::GIFTDSTTYPE_SINGLE)
	{
		*this>>stEvent.unDstUIN>>stEvent.unDst179ID>>stEvent.strDstName;
		common::utility::systemhelper::FilterNickName(stEvent.strDstName);
		if(stEvent.strDstName.empty())
		{
			wchar_t sz[100];
			wsprintf(sz, L"%d", stEvent.unDst179ID);
			stEvent.strDstName = sz;
		}
	}

	uint8 unLuckyCount=0;
	uint8 unAttachEffect=0;
	*this>>stEvent.unGiftID>>stEvent.unSentCount>>stEvent.strAttachMessage>>unAttachEffect;

	stEvent.bAttachFlyEffect = unAttachEffect;
			
	*this>>unLuckyCount;

	while(unLuckyCount-- > 0)
	{
		core::SendGiftLuckyInfo info;
		*this>>info.unLuckyIndex>>info.unLuckyTimes>>info.unLuckyMoney;
		stEvent.vecLuckyInfo.push_back(info);
	}
}