#include "UserInfoUpdate.h"

void UserInfoUpdatePacket::Unpack()
{
	ServerPacket::Unpack();

	uint8 type,count;
	*this>>stEvent.unSrcUIN>>count;	
	while(count-->0)
	{
		*this>>type;
		if(type == core::INFOTYPE_NAME || type == core::INFOTYPE_HOME_PROVINCE || 
			type == core::INFOTYPE_HOME_CITY || type == core::USERINFO_ROOM_JIAZU)
		{
			std::wstring string;
			*this>>string;
			stEvent.map_key2string[(core::ENM_InfoType)type] = string;
		}
		else if(type == core::INFOTYPE_179ID || type == core::INFOTYPE_SELF_SETTING)
		{
			uint32 id;
			*this>>id;
			stEvent.map_key2uin32[(core::ENM_InfoType)type] = id;
		}
		else if(type == core::INFOTYPE_ONLINE_TIME || type == core::INFOTYPE_LEFT_TIME || 
			type == core::INFOTYPE_BIRTHDAY_YEAR || type == core::USERINFO_ROOM_TITLELEVEL ||
			type == core::USERINFO_ROOM_CLIENTINFO || type == core::USERINFO_ROOM_STATUS)
		{
			uint16 id;
			*this>>id;
			stEvent.map_key2uin16[(core::ENM_InfoType)type] = id;
		}
		else if(type == core::INFOTYPE_GENDER || type == core::INFOTYPE_VIP_LEVEL || 
			type == core::INFOTYPE_USR_LEVEL || type == core::INFOTYPE_BIRTHDAY_MONTH ||
			type == core::INFOTYPE_BIRTHDAY_DAY || type == core::INFOTYPE_RICH_LEVEL)
		{
			uint8 id;
			*this>>id;
			stEvent.map_key2uin8[(core::ENM_InfoType)type] = id;
		}
		else
		{
			assert(0);
		}
	}
}