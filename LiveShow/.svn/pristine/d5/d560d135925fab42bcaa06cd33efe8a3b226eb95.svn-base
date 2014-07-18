#include "StdAfx.h"
#include "UserItem.h"
#include "..\gift/GiftManager.h"
#include "..\room.h"
#include "..\GlobalManager.h"
#include "..\..\CommonLibrary\include\utility\StringHelper.h"
IUserItem::IUserItem(void)
:	m_bWaiting(false),
	m_unBlinkEnterID(0),
	m_unStampID(0),
	m_bBlockChatToMe(false),
	m_unChangeVoiceID(0),
	m_unWealthyIndex(0)
{
	memset(&m_unListIconIndex, 0, sizeof(m_unListIconIndex));
}

IUserItem::~IUserItem(void)
{
}

void IUserItem::SetUserInfo( core::room_user_info & userinfo )
{
	m_UserInfo.unUIN = userinfo.unUIN;
	m_UserInfo.un179id = userinfo.un179id;
	m_UserInfo.strName = userinfo.strName;
	m_strDecodeName = common::utility::stringhelper::XMLEncode( m_UserInfo.strName ) ;
	m_UserInfo.unVipLevel = userinfo.unVipLevel;
	m_UserInfo.unUsrLevel = userinfo.unUsrLevel;
	m_UserInfo.unTitleLevel = userinfo.unTitleLevel;
	m_UserInfo.unRichLevel = userinfo.unRichLevel;
	//m_UserInfo.unRichLevel = rand() % 25;			// test code
	//m_UserInfo.strJiaZu = userinfo.strJiaZu;
	m_UserInfo.stClientInfo = userinfo.stClientInfo;
	m_UserInfo.unStatus = userinfo.unStatus;
	//m_UserInfo.unStampID = userinfo.unStampID;
	//m_UserInfo.bShowJiaZu = userinfo.bShowJiaZu;
	m_UserInfo.vecItemID = userinfo.vecItemID;
	m_UserInfo.unBusiness = userinfo.unBusiness;
	m_UserInfo.bMan = userinfo.bMan;
	m_UserInfo.stClientInfo = userinfo.stClientInfo;
	for (std::vector<uint16>::iterator it = userinfo.vecItemID.begin(); it != userinfo.vecItemID.end(); ++it)
	{
		IGiftItem * pGiftItem = GiftManager::GetInstance()->SearchGiftByID(*it);
		if (pGiftItem != NULL)
		{
			if (pGiftItem->GetGiftType() == core::GIFTTYPE_STAMP)
			{
				m_unStampID = pGiftItem->GetID();
			}

			if (pGiftItem->GetGiftType() == core::GIFTTYPE_CHANGEVOICE)
			{
				m_unChangeVoiceID = pGiftItem->GetID();
			}

			if (pGiftItem->GetStrType() == L"blink enter")
			{
				m_unBlinkEnterID = pGiftItem->GetID();
			}
		}
	}

	m_unWealthyIndex = WealthyMgr->GetEffectIndex(userinfo.un179id);
}

void IUserItem::AddGiftItemID( uint16 unGiftID )
{
	m_UserInfo.vecItemID.push_back(unGiftID);
	IGiftItem * pGiftItem = GiftManager::GetInstance()->SearchGiftByID(unGiftID);
	if (pGiftItem != NULL)
	{
		if (pGiftItem->GetGiftType() == core::GIFTTYPE_STAMP)
		{
			m_unStampID = unGiftID;
		}

		if (pGiftItem->GetGiftType() == core::GIFTTYPE_CHANGEVOICE)
		{
			m_unChangeVoiceID = unGiftID;
		}
	}
}

void IUserItem::DelGiftItemID( uint16 unGiftID )
{
	//for (std::vector<uint16>::iterator it = m_UserInfo.vecItemID.begin(); it != m_UserInfo.vecItemID.end();)
	//{
	//	if (*it == unGiftID)
	//	{
	//		m_UserInfo.vecItemID.erase(it);
	//		break;
	//	}
	//}

	if (m_unStampID == unGiftID)
	{
		m_unStampID = 0;
	}

	if (m_unChangeVoiceID == unGiftID)
	{
		m_unChangeVoiceID = 0;
	}
}

bool IUserItem::IsActor()  //是否是艺人
{
	return ( (m_UserInfo.unBusiness&core::BUSINESSTYPE_SONGER) 
		|| (m_UserInfo.unBusiness&core::BUSINESSTYPE_DANCER) 
		|| (m_UserInfo.unBusiness&core::BUSINESSTYPE_HOST) ) ;
}

int IUserItem::GetPublicMicIndex()
{
	int nIndex = 0 ;
	if ( m_UserInfo.unStatus & core::USERSTATUSTYPE_PUBLIC_1 )
	{
		nIndex = 1 ;
	}
	else if ( m_UserInfo.unStatus & core::USERSTATUSTYPE_PUBLIC_2 )
	{
		nIndex = 2 ;
	}
	else if ( m_UserInfo.unStatus & core::USERSTATUSTYPE_PUBLIC_3 )
	{
		nIndex = 3 ;
	}
	return nIndex ;
}

std::vector<core::GiftStarInfo> IUserItem::GetGiftStarInfo()
{
	core::GetGiftStarInfo st;
	st.unUIN = GetUserUIN();
	g_RoomModule->m_pModuleManager->CallService(core::SERVICE_VALUE_CORE_GET_GIFT_STAR_INFO, (param)&st);

	return st.vecInfo;
}

bool IUserItem::IsOfficial()
{
	return ( (m_UserInfo.unBusiness&core::BUSINESSTYPE_AGENT) || (m_UserInfo.unBusiness&core::BUSINESSTYPE_OPERATOR) 
		|| (m_UserInfo.unBusiness&core::BUSINESSTYPE_AD_CLEANER) || (m_UserInfo.unBusiness&core::BUSINESSTYPE_PATROLMAN) ) ;
}