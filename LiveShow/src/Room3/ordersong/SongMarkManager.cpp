#include "StdAfx.h"
#include "SongMarkManager.h"
#include "utility\expat++\expat_document.h"
#include "utility\SystemHelper.h"
#include "utility\StringHelper.h"
#include "utility\URLEncode.h"
#include "..\GlobalRoomDefine.h"

std::map<ENM_Marker_Type, std::map<ENM_Mark_Step, marker_info>> CSongMarkManager::m_mapMarkerInfo;
uint8 CSongMarkManager::m_unUnitTicket = 1;

CSongMarkManager::CSongMarkManager(void)
: m_unSongOrderID(0), m_unCurSongerID(0), m_unCurOrdererID(0), m_strSongerName(L""), m_strCurSong(L""), m_unLeftTicket(0), m_bLastGetTicket(false), m_bGetTicketFail(false), m_curMarkerType(MARKER_TYPE_NO_TICKET)
{
}

CSongMarkManager::~CSongMarkManager(void)
{
}

bool CSongMarkManager::LoadMarkerConfig()
{
	common::utility::expat_document xmlDoc;

	std::wstring strModulePath = common::utility::systemhelper::Get179AppPath();

	common::CURLEncode urlEncode;
	strModulePath = urlEncode.URLEncodeLocalPath(strModulePath);

	std::wstring strXmlPath = strModulePath + MARKER_CONFIG_PATH;

	if (!xmlDoc.load_from_file(strXmlPath.c_str()))
	{
		return false;
	}

	common::utility::expat_element * pRoot = xmlDoc.get_root();
	if (!pRoot)
	{
		return false;
	}

	common::utility::expat_element * pElement = pRoot->get_first_child_element();
	m_unUnitTicket = common::utility::stringhelper::StringToInt(pElement->get_attribute_value(L"count"));

	pElement = pElement->get_sibling_element();

	while (pElement)
	{
		common::utility::expat_element * pChildElement = pElement->get_first_child_element();
		ENM_Marker_Type enmType = (ENM_Marker_Type)common::utility::stringhelper::StringToInt(pElement->get_attribute_value(L"type"));
		std::map<ENM_Mark_Step, marker_info> mapMarkerInfo;
		while (pChildElement)
		{
			marker_info info;
			info.enmType = enmType;
			info.enmStep = (ENM_Mark_Step)common::utility::stringhelper::StringToInt(pChildElement->get_attribute_value(L"id"));
			info.str1 = pChildElement->get_attribute_value(L"str1");
			info.str2 = pChildElement->get_attribute_value(L"str2");
			info.str3 = pChildElement->get_attribute_value(L"str3");

			mapMarkerInfo[info.enmStep] = info;

			pChildElement = pChildElement->get_sibling_element();
		}

		m_mapMarkerInfo[enmType] = mapMarkerInfo;
		pElement = pElement->get_sibling_element();
	}

	return true;

}

void CSongMarkManager::SetCurMarkInfo( uint32 unOrderID, uint32 unSongerID, std::wstring strSongerName, uint32 unOrdererID, std::wstring strSong )
{
	m_unSongOrderID = unOrdererID;
	m_unCurSongerID = unSongerID;
	m_unCurOrdererID = unOrdererID;
	m_strSongerName = strSongerName;
	m_strCurSong = strSong;
}

void CSongMarkManager::GetMarkerInfo( ENM_Marker_Type enmType, ENM_Mark_Step enmStep, marker_info & stMarkerInfo )
{
	if (m_mapMarkerInfo.find(enmType) != m_mapMarkerInfo.end())
	{
		if (m_mapMarkerInfo[enmType].find(enmStep) != m_mapMarkerInfo[enmType].end())
		{
			stMarkerInfo = m_mapMarkerInfo[enmType][enmStep];
		}
	}
}

//void CSongMarkManager::SetOwnerInfo( uint32 unOwnerID )
//{
//	m_unRoomOwnerID = unOwnerID;
//}

ENM_Marker_Type CSongMarkManager::CheckMarkerType( IUserItem * pUserItem, bool bHaveTicket )
{
	ENM_Marker_Type enmType = MARKER_TYPE_NO_TICKET;
	if (pUserItem->GetUserUIN() == m_unCurSongerID)
	{
		enmType = MARKER_TYPE_ACTOR;
	}
	else if (pUserItem->GetUserTitleLevel() == core::TITLELEVEL_OWNER)
	{
		enmType = MARKER_TYPE_OWNER;
	}
	else if (pUserItem->GetUserUIN() == m_unCurOrdererID)
	{
		enmType = MARKER_TYPE_ORDER;
	}
	else if (bHaveTicket)
	{
		enmType = MARKER_TYPE_HAVE_TICKET;
	}
	else
	{
		enmType = MARKER_TYPE_NO_TICKET;
	}

	m_curMarkerType = enmType;
	return enmType;
}

uint8 CSongMarkManager::GetUnitTicket()
{
	return m_unUnitTicket;
}

void CSongMarkManager::Release()
{
	for (std::map<ENM_Marker_Type, std::map<ENM_Mark_Step, marker_info>>::iterator it = m_mapMarkerInfo.begin(); it != m_mapMarkerInfo.end(); ++it)
	{
		it->second.clear();
	}

	m_mapMarkerInfo.clear();
}

void CSongMarkManager::Reset()
{
	m_unLeftTicket = 0;
	m_bLastGetTicket = false;
	m_bGetTicketFail = false;
}