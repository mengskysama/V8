#include "SendMessage.h"

void SendMessagePacket::Pack()
{
	HeadBusiness();
	*this<<(uint8)stEvent.enmType<<stEvent.dstuin<<stEvent.message;
	*this<<stEvent.stFont.font_size<<stEvent.stFont.font_color<<stEvent.stFont.font_style<<stEvent.stFont.font_type;
	*this<<(uint8)stEvent.bIsAutoMsg<<(uint8)stEvent.bRejectAutoMsg;
	TailBusiness();
}

//////////////////////////////////////////////////////////////////////////

void SendMessageRspPacket::Unpack()
{
	ServerPacket::Unpack();

	uint32 result=0;
	*this>>result;

	stEvent.enmResult = (core::ENM_SendMessageResult)result;
	if(stEvent.enmResult != core::SENDMESSAGERESULT_OK)
	{
		*this>>stEvent.strFailReason;
		if ( stEvent.enmResult == core::SENDMESSAGERESULT_USER_SHUTUP )
		{
			*this>>stEvent.unLeftTime ;
		}
	}
}

//////////////////////////////////////////////////////////////////////////

void SendMessageNotifyPacket::Unpack()
{
	ServerPacket::Unpack();

	uint8 untype;
	*this>>untype;

	stEvent.enmType = (core::ENM_SendMessageType)untype;
	*this>>stEvent.srcuin>>stEvent.dstuin>>stEvent.message;
	*this>>stEvent.stFont.font_size>>stEvent.stFont.font_color>>stEvent.stFont.font_style>>stEvent.stFont.font_type;
	uint8 temp1,temp2;
	*this>>temp1>>temp2;
	stEvent.bIsAutoMsg = temp1;
	stEvent.bRejectAutoMsg = temp2;
}