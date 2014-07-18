#include "EnterRoom.h"
#include "utility\SystemHelper.h"

void EnterRoomRspPacket::Unpack()
{
	ServerPacket::Unpack();

	uint32 result;
	*this>>result;
	stEvent.enmResult = (core::EnterRoomRspEvent::ENM_EnterRoomResult)result;

	if(stEvent.enmResult == core::EnterRoomRspEvent::ENM_EnterRoomResult::ENTERROOM_ERROR_OK)
	{
		*this>>stEvent.strRoomName>>stEvent.strSignature>>stEvent.relayIP_dianxin>>stEvent.relayPort_dianxin>>stEvent.relayIP_wangtong>>stEvent.relayPort_wangtong;
		uint8 unMan;
		*this>>stEvent.strOwnerName>>stEvent.unOwner179ID>>unMan>>stEvent.unAVKey;

		stEvent.bOwnerMan = unMan;
		common::utility::systemhelper::FilterNickName(stEvent.strOwnerName);

		if(stEvent.strOwnerName.empty())
		{
			wchar_t sz[100];
			wsprintf(sz, L"%d", stEvent.unOwner179ID);
			stEvent.strOwnerName = sz;
		}

		//读取公麦用户信息

		uint8 unCount=0;
		*this>>unCount;
		core::room_user_info userinfo;
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
				wsprintf(sz, L"游客%04d", userinfo.un179id%10000);
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
			stEvent.vecPubMic.push_back(userinfo);			
		}

		uint8 collected=0;
		*this>>collected;
		stEvent.bCollected = collected;

		return;
	}

	*this>>stEvent.strFailReason;
	
	if(stEvent.enmResult == core::EnterRoomRspEvent::ENM_EnterRoomResult::ENTERROOM_ERROR_KICKED ||
		stEvent.enmResult == core::EnterRoomRspEvent::ENM_EnterRoomResult::ENTERROOM_ERROR_IP_LOCKED)
	{
		uint16 unTimeLeftSec=0;
		*this>>unTimeLeftSec;
		stEvent.unKickTimeLeft = (unTimeLeftSec+59)/60;
	}
	else if(stEvent.enmResult == core::EnterRoomRspEvent::ENM_EnterRoomResult::ENTERROOM_ERROR_ROOM_LOCKED)
	{
		*this>>stEvent.unLockDaysLeft;
	}
	else if(stEvent.enmResult == core::EnterRoomRspEvent::ENM_EnterRoomResult::ENTERROOM_ERROR_MAX_ROOM_NUM)
	{
		*this>>stEvent.unMaxRoomNum;
	}
}

//////////////////////////////////////////////////////////////////////////

void EnterRoomNotifyPacket::Unpack()
{
	ServerPacket::Unpack();

	uint8 unMan;
	*this>>stEvent.stUserInfo.unUIN>>unMan>>stEvent.stUserInfo.un179id>>stEvent.stUserInfo.strName;
	stEvent.stUserInfo.bMan = unMan;
	*this>>stEvent.stUserInfo.unVipLevel>>stEvent.stUserInfo.unUsrLevel;
	uint16 unClientInfo=0;
	*this>>stEvent.stUserInfo.unTitleLevel>>stEvent.stUserInfo.unBusiness>>unClientInfo;
	uint16 unItemCount=0,unItemID=0;
	*this>>stEvent.stUserInfo.unStatus>>unItemCount;
	while(unItemCount-- > 0)
	{
		*this>>unItemID;
		stEvent.stUserInfo.vecItemID.push_back(unItemID);
	}

	*this>>stEvent.stUserInfo.unRichLevel;
	
	if(stEvent.stUserInfo.unVipLevel == core::VIPLEVEL_NONE)
	{
		wchar_t sz[100];
		wsprintf(sz, L"游客%04d", stEvent.stUserInfo.un179id%10000);
		stEvent.stUserInfo.strName = sz;
	}
	else
	{
		common::utility::systemhelper::FilterNickName(stEvent.stUserInfo.strName);
	}

	if(stEvent.stUserInfo.strName.empty())
	{
		wchar_t sz[100];
		wsprintf(sz, L"%d", stEvent.stUserInfo.un179id);
		stEvent.stUserInfo.strName = sz;
	}
	
	stEvent.stUserInfo.stClientInfo.setData(unClientInfo);
}