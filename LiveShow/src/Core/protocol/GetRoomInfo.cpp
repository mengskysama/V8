#include "GetRoomInfo.h"

void GetRoomInfoRspPacket::Unpack()
{
	ServerPacket::Unpack();
	uint8 count=0;
	*this>>count;
	while(count-- > 0)
	{
		uint8 type=0;
		*this>>type;
		if(type == core::ROOMINFOTYPE_NAME || type == core::ROOMINFOTYPE_SIGNATURE ||
				type == core::ROOMINFOTYPE_PASSWORD || type == core::ROOMINFOTYPE_WELCOME_WORDS ||
				type == core::ROOMINFOTYPE_NOTICE)
		{
			std::wstring value;
			*this>>value;
			stEvent.map_key2string[(core::ENM_RoomInfoType)type] = value;
		}
		else if(type == core::ROOMINFOTYPE_FREESHOW_TIME || type == core::ROOMINFOTYPE_MIC_FIRST_STATUS
			|| type == core::ROOMINFOTYPE_MIC_SECOND_STATUS || type == core::ROOMINFOTYPE_MIC_THIRD_STATUS)
		{
			uint8 value;
			*this>>value;
			stEvent.map_key2uin8[(core::ENM_RoomInfoType)type] = value;
		}
		else if(type == core::ROOMINFOTYPE_OPTION)
		{
			uint32 value;
			*this>>value;
			stEvent.map_key2uin32[(core::ENM_RoomInfoType)type] = value;
		}
		else
		{
			assert(0);
		}
	}
}

//////////////////////////////////////////////////////////////////////////
