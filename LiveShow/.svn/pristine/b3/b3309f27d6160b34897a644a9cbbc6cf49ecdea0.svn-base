#include "SetSelfInfo.h"

void SetSelfInfoPacket::Pack()
{
	HeadBusiness();
	*this<<(uint8)(stEvent.uint8Map.size()+stEvent.uint16Map.size()+stEvent.uint32Map.size()+stEvent.stringMap.size());
	for(std::map<core::ENM_InfoType, uint8>::iterator it = stEvent.uint8Map.begin(); it != stEvent.uint8Map.end(); ++it)
	{
		*this<<(uint8)it->first<<it->second;
	}
	for(std::map<core::ENM_InfoType, uint16>::iterator it = stEvent.uint16Map.begin(); it != stEvent.uint16Map.end(); ++it)
	{
		*this<<(uint8)it->first<<it->second;
	}
	for(std::map<core::ENM_InfoType, uint32>::iterator it = stEvent.uint32Map.begin(); it != stEvent.uint32Map.end(); ++it)
	{
		*this<<(uint8)it->first<<it->second;
	}
	for(std::map<core::ENM_InfoType, std::wstring>::iterator it = stEvent.stringMap.begin(); it != stEvent.stringMap.end(); ++it)
	{
		*this<<(uint8)it->first<<it->second;
	}
	TailBusiness();
}

void SetSelfInfoNotifyPacket::Unpack()
{
	ServerPacket::Unpack();

	uint8 count=0;
	*this>>count;
	uint8 type;
	while(count-->0)
	{
		*this>>type;
		if(type == core::INFOTYPE_NAME || type == core::INFOTYPE_HOME_PROVINCE || 
			type == core::INFOTYPE_HOME_CITY)
		{
			std::wstring string;
			*this>>string;
			stEvent.stringMap[(core::ENM_InfoType)type] = string;
		}
		else if(type == core::INFOTYPE_179ID || type == core::INFOTYPE_SELF_SETTING)
		{
			uint32 id;
			*this>>id;
			stEvent.uint32Map[(core::ENM_InfoType)type] = id;
		}
		else if(type == core::INFOTYPE_ONLINE_TIME || type == core::INFOTYPE_LEFT_TIME || 
			type == core::INFOTYPE_BIRTHDAY_YEAR)
		{
			uint16 id;
			*this>>id;
			stEvent.uint16Map[(core::ENM_InfoType)type] = id;
		}
		else if(type == core::INFOTYPE_GENDER || type == core::INFOTYPE_VIP_LEVEL || 
			type == core::INFOTYPE_USR_LEVEL || type == core::INFOTYPE_BIRTHDAY_MONTH ||
			type == core::INFOTYPE_BIRTHDAY_DAY || type == core::INFOTYPE_RICH_LEVEL)
		{
			uint8 id;
			*this>>id;
			stEvent.uint8Map[(core::ENM_InfoType)type] = id;
		}
		else
		{
			assert(0);
		}
	}
}