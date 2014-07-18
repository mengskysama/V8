#include "StdAfx.h"
#include "SofaItem.h"
#include "HallDefine.h"
#include "..\Room.h"
#include "..\userlist\ListIconManager.h"
#include "..\richlevel\RichLevelManager.h"

#define SPLIT_1	L"&spor;"
#define SPLIT_2	L"&spco;"

CSofaItem::CSofaItem(uint8 unIndex)
{
	m_SofaInfo.unIndex = unIndex;
}

CSofaItem::~CSofaItem(void)
{
}

void CSofaItem::SetSofaInfo( uint8 unIndex, core::GetSofaListRspEvent::SofaInfo sofainfo )
{
	//m_SofaInfo.unIndex = unIndex;
	m_SofaInfo.unPrice = sofainfo.price;
	m_SofaInfo.unUIN = sofainfo.uin;
	m_SofaInfo.un179ID = sofainfo.un179ID;
	m_SofaInfo.strUserName = sofainfo.nickname;
	m_SofaInfo.bMan = sofainfo.bman;
	m_SofaInfo.unVipLevel = sofainfo.vipLevel;
	m_SofaInfo.unRichLevel = sofainfo.richLevel;

	Hall::HeadImage headImage;
	headImage.m_bMan = sofainfo.bman;
	headImage.m_isGuest = false;
	headImage.m_unUIN = m_SofaInfo.unUIN;
	g_RoomModule->m_pModuleManager->CallService(Hall::SERVICE_VALUE_HALL_UPDATE_HEAD_IMAGE_PATH, (param)&headImage);
	m_SofaInfo.strHeadPath = headImage.m_HeadImagePath;
}

void CSofaItem::UpdateHead()
{
	Hall::HeadImage headImage ;
	headImage.m_bMan = m_SofaInfo.bMan;
	headImage.m_unUIN = m_SofaInfo.unUIN;
	headImage.m_isGuest = false;
	g_RoomModule->m_pModuleManager->CallService(Hall::SERVICE_VALUE_HALL_GET_HEAD_IMAGE_PATH, (param)&headImage);
	m_SofaInfo.strHeadPath = headImage.m_HeadImagePath;
}

void CSofaItem::UpdateVipLevel( uint8 unVipLevel )
{
	m_SofaInfo.unVipLevel = unVipLevel;
}

void CSofaItem::UpdateRichLevel( uint8 unRichLevel )
{
	m_SofaInfo.unRichLevel = unRichLevel;
}

//void CSofaItem::UpdateSofaInfo( core::GetSofaListRspEvent::SofaInfo sofainfo )
//{
//	if (m_SofaInfo.unUIN == sofainfo.uin)
//	{
//		m_SofaInfo.enmType = ONSOFATYPE_SELF;
//	}
//	else
//	{
//		m_SofaInfo.enmType = ONSOFATYPE_KICK;
//	}
//	m_SofaInfo.unPrice = sofainfo.price;
//	m_SofaInfo.unLastUIN = m_SofaInfo.unUIN;
//	m_SofaInfo.unUIN = sofainfo.uin;
//	m_SofaInfo.un179ID = sofainfo.un179ID;
//	m_SofaInfo.strUserName = sofainfo.nickname;
//	m_SofaInfo.bMan = sofainfo.bman;
//	m_SofaInfo.unVipLevel = sofainfo.vipLevel;
//	m_SofaInfo.unRichLevel = sofainfo.richLevel;
//
//	Hall::HeadImage headImage;
//	headImage.m_bMan = sofainfo.bman;
//	headImage.m_isGuest = false;
//	headImage.m_unUIN = m_SofaInfo.unUIN;
//	g_RoomModule->m_pModuleManager->CallService(Hall::SERVICE_VALUE_HALL_UPDATE_HEAD_IMAGE_PATH, (param)&headImage);
//	m_SofaInfo.strHeadPath = headImage.m_HeadImagePath;
//}

void CSofaItem::UpdateSofaInfo(uint32 unPrice, IUserItem * pUserItem )
{
	if (m_SofaInfo.unUIN == 0)
	{
		m_SofaInfo.enmType = ONSOFATYPE_EMPTY;
	}
	else
	{
		if (m_SofaInfo.unUIN == pUserItem->GetUserUIN())
		{
			m_SofaInfo.enmType = ONSOFATYPE_SELF;
		}
		else
		{
			m_SofaInfo.enmType = ONSOFATYPE_KICK;
		}
	}

	m_SofaInfo.unLastUIN = m_SofaInfo.unUIN;
	m_SofaInfo.unLast179ID = m_SofaInfo.un179ID;
	m_SofaInfo.strLastUserName = m_SofaInfo.strUserName;
	m_SofaInfo.unLastVipLevel = m_SofaInfo.unVipLevel;
	m_SofaInfo.unLastRichLevel = m_SofaInfo.unRichLevel;

	m_SofaInfo.unPrice = unPrice;
	m_SofaInfo.unUIN = pUserItem->GetUserUIN();
	m_SofaInfo.un179ID = pUserItem->GetUser179ID();
	m_SofaInfo.strUserName = pUserItem->GetUserName();
	m_SofaInfo.bMan = pUserItem->IsMale();
	m_SofaInfo.unVipLevel = pUserItem->GetUserVipLevel();
	m_SofaInfo.unRichLevel = pUserItem->GetUserRichLevel();

	Hall::HeadImage headImage;
	headImage.m_bMan = pUserItem->IsMale();
	headImage.m_isGuest = false;
	headImage.m_unUIN = pUserItem->GetUserUIN();
	g_RoomModule->m_pModuleManager->CallService(Hall::SERVICE_VALUE_HALL_UPDATE_HEAD_IMAGE_PATH, (param)&headImage);
	m_SofaInfo.strHeadPath = headImage.m_HeadImagePath;
}

void CSofaItem::UpdateSofaInfo( IUserItem * pUserItem )
{
	m_SofaInfo.enmType = ONSOFATYPE_UPDATE;
	m_SofaInfo.unUIN = pUserItem->GetUserUIN();
	m_SofaInfo.un179ID = pUserItem->GetUser179ID();
	m_SofaInfo.strUserName = pUserItem->GetUserName();
	m_SofaInfo.bMan = pUserItem->IsMale();
	m_SofaInfo.unVipLevel = pUserItem->GetUserVipLevel();
	m_SofaInfo.unRichLevel = pUserItem->GetUserRichLevel();

	Hall::HeadImage headImage;
	headImage.m_bMan = pUserItem->IsMale();
	headImage.m_isGuest = false;
	headImage.m_unUIN = pUserItem->GetUserUIN();
	g_RoomModule->m_pModuleManager->CallService(Hall::SERVICE_VALUE_HALL_UPDATE_HEAD_IMAGE_PATH, (param)&headImage);
	m_SofaInfo.strHeadPath = headImage.m_HeadImagePath;
}

ENM_OnSofaType CSofaItem::GetOnSofaType()
{
	return m_SofaInfo.enmType;
}

CString CSofaItem::GetStringParam( bool bJustUpdate/* = false*/ )
{
	CString strTemp;

	// 座位号,类型,价格,名字,179ID,VIP,富豪,头像
	CString strName = m_SofaInfo.strUserName.c_str();
	strName.Replace(L"|", SPLIT_1);
	strName.Replace(L",", SPLIT_2);
	strTemp.Format(L"%d,%d,%d,%s,%d,%s,%s,%s", m_SofaInfo.unIndex, bJustUpdate ? ONSOFATYPE_UPDATE : m_SofaInfo.enmType, m_SofaInfo.unPrice, strName, m_SofaInfo.un179ID, ListIconManager::GetIconPathByVipLevel(m_SofaInfo.unVipLevel).c_str(), (m_SofaInfo.unRichLevel > 0 ? RichLevelManager::GetRichLevelImgPathByLevel(m_SofaInfo.unRichLevel).c_str() : L""), m_SofaInfo.strHeadPath.c_str());

	return strTemp;
}

uint32 CSofaItem::GetUserUIN()
{
	return m_SofaInfo.unUIN;
}

uint8 CSofaItem::GetIndex()
{
	return m_SofaInfo.unIndex;
}

uint32 CSofaItem::GetLastUserUIN()
{
	return m_SofaInfo.unLastUIN;
}

uint32 CSofaItem::GetPrice()
{
	return m_SofaInfo.unPrice;
}

uint8 CSofaItem::GetLastVipLevel()
{
	return m_SofaInfo.unLastVipLevel;
}

uint8 CSofaItem::GetLastRichLevel()
{
	return m_SofaInfo.unLastRichLevel;
}

uint32 CSofaItem::GetLast179ID()
{
	return m_SofaInfo.unLast179ID;
}

std::wstring CSofaItem::GetLastUserName()
{
	return m_SofaInfo.strLastUserName;
}