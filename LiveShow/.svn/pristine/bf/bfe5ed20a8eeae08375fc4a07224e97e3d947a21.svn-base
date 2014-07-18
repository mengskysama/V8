#include "GetUserList.h"
#include "utility\SystemHelper.h"

void GetUserListRspPacket::Unpack()
{
	ServerPacket::Unpack();

	uint32 unResult=-1;	
	*this>>unResult;

	if(unResult != 0)
	{
		std::wstring strReason;
		*this>>strReason;
		stEvent.bcomplete = true;
		return;
	}

	uint8 unEndFlag=0;
	*this>>unEndFlag;
	stEvent.bcomplete = unEndFlag != 0;

	uint8 unCount=0;
	*this>>unCount;
	core::room_user_info userinfo;
	uint8 unMan;
	while(unCount-- > 0)
	{
		userinfo.vecItemID.clear();

		*this>>userinfo.unUIN>>unMan>>userinfo.un179id>>userinfo.strName>>userinfo.unVipLevel>>userinfo.unUsrLevel;
		userinfo.bMan = unMan;
		uint16 unClientInfo=0;
		uint16 unItemCount=0,unItemID=0;
		*this>>userinfo.unTitleLevel>>userinfo.unBusiness>>unClientInfo>>userinfo.unStatus>>unItemCount;
		while(unItemCount-- > 0)
		{
			*this>>unItemID;
			userinfo.vecItemID.push_back(unItemID);
		}

		*this>>userinfo.unRichLevel;
		
		if(userinfo.unVipLevel == core::VIPLEVEL_NONE)
		{
			wchar_t sz[100];
			wsprintf(sz, L"сн©м%04d", userinfo.un179id%10000);
			userinfo.strName = sz;
		}
		else
		{
			common::utility::systemhelper::FilterNickName(userinfo.strName);
		}

		if(userinfo.strName.empty())
		{
			wchar_t sz[100];
			wsprintf(sz, L"%d", userinfo.un179id);
			userinfo.strName = sz;
		}
		
		userinfo.stClientInfo.setData(unClientInfo);
		stEvent.vecUsrList.push_back(userinfo);
	}
}