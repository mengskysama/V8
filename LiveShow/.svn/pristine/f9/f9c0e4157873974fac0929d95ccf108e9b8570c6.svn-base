#include "SetRoomInfo.h"

void SetRoomInfoPacket::Pack()
{
	HeadBusiness();

	*this<<(uint8)(stEvent.map_key2string.size()+stEvent.map_key2uin8.size()+stEvent.map_key2uin32.size());
	for(std::map<core::ENM_RoomInfoType, std::wstring>::iterator it = stEvent.map_key2string.begin();
		it != stEvent.map_key2string.end(); ++it)
	{
		*this<<(uint8)it->first<<it->second;
	}
	for(std::map<core::ENM_RoomInfoType, uint8>::iterator it = stEvent.map_key2uin8.begin();
		it != stEvent.map_key2uin8.end(); ++it)
	{
		*this<<(uint8)it->first<<it->second;
	}
	for(std::map<core::ENM_RoomInfoType, uint32>::iterator it = stEvent.map_key2uin32.begin();
		it != stEvent.map_key2uin32.end(); ++it)
	{
		*this<<(uint8)it->first<<it->second;
	}

	TailBusiness();
}

//////////////////////////////////////////////////////////////////////////

void SetRoomInfoNotifyPacket::Unpack()
{
	ServerPacket::Unpack();
	uint8 count=0;
	*this>>stEvent.unSrcUIN>>count;
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
		else if(type == core::ROOMINFOTYPE_FREESHOW_TIME)
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
