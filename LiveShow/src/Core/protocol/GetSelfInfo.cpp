#include "GetSelfInfo.h"

void GetSelfInfoRspPacket::Unpack()
{
	ServerPacket::Unpack();

	uint8 unType=0;
	*this>>unType;
	if(unType == 0)
	{
		*this>>bithday_year>>bithday_month>>bithday_day>>home_province>>home_city;
	}
	else
	{
		assert(0);
	}
}