#include "StdAfx.h"
#include "GiftManager.h"
#include "utility/expat++/expat_document.h"
#include "utility/StringHelper.h"
#include "utility/SystemHelper.h"
#include "utility/URLEncode.h"
#include "..\GlobalRoomDefine.h"
#include "ChangeVoiceItem.h"
#include <algorithm>
#include <functional>


GiftManager * GiftManager::s_pGiftManager = NULL;

GiftManager::GiftManager(void)
{
	m_pWishSongItem = NULL;
	m_pFlyingWordsItem = NULL;
}

GiftManager::~GiftManager(void)
{
	Release();
}

bool GiftManager::LoadGiftList()
{
	common::utility::expat_document xmlDoc;

	std::wstring strModulePath = common::utility::systemhelper::Get179AppPath();

	common::CURLEncode urlEncode;
	strModulePath = urlEncode.URLEncodeLocalPath(strModulePath);

	std::wstring strXmlPath = strModulePath + GIFT_CONFIG_PATH;

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
	std::set<std::wstring> setGiftCatalogList,setStampCataLogList;
	std::vector<std::wstring> vecStampLabel;
	uint16 nIndex = 0;
	while (pElement)
	{
		//GiftItem * pGiftItem = new GiftItem();
		room::gift_info giftinfo;
		giftinfo.unIndex = nIndex;
		giftinfo.unID = common::utility::stringhelper::StringToInt(pElement->get_attribute_value(L"id"),16);
		giftinfo.strName = pElement->get_attribute_value(L"name");
		giftinfo.strRichName = pElement->get_attribute_value(L"richname");
		if (giftinfo.strRichName == L"")
		{
			giftinfo.strRichName = giftinfo.strName;
		}
		giftinfo.strDesc = pElement->get_child_element_attribute_value_by_name(L"desc", L"content");
		giftinfo.strSmallImagePath = strModulePath + GIFT_DIR + pElement->get_attribute_value(L"icon");
		giftinfo.strLargeImagePath = strModulePath + GIFT_DIR + pElement->get_attribute_value(L"image");
		giftinfo.strUnitName = pElement->get_attribute_value(L"unit");
		giftinfo.bTempNotice = (pElement->get_attribute_value(L"tempnotice") == L"true");
		std::wstring strCatalog = pElement->get_attribute_value(L"catalog");
		giftinfo.strCatalog = pElement->get_attribute_value(L"catalog");
		std::wstring strBeginTime = pElement->get_attribute_value(L"begin_time");
		if (strBeginTime != L"")
		{
			int nYear, nMonth, nDay, nHour, nMinute, nSec;
			nYear = nMonth = nDay = nHour = nMinute = nSec = 0 ;
			swscanf(strBeginTime.c_str(), L"%d-%d-%d %d:%d:%d", &nYear, &nMonth, &nDay, &nHour, &nMinute, &nSec);
			giftinfo.tmBeginTime = CTime(nYear, nMonth, nDay, nHour, nMinute, nSec);
		}
		std::wstring strEndTime = pElement->get_attribute_value(L"end_time");
		if (strEndTime != L"")
		{			
			int nYear, nMonth, nDay, nHour, nMinute, nSec;
			nYear = nMonth = nDay = nHour = nMinute = nSec = 0 ;
			swscanf(strEndTime.c_str(), L"%d-%d-%d %d:%d:%d", &nYear, &nMonth, &nDay, &nHour, &nMinute, &nSec);
			giftinfo.tmEndTime = CTime(nYear, nMonth, nDay, nHour, nMinute, nSec);
		}
		if (pElement->get_attribute_value(L"status") == L"2")
		{
			giftinfo.bInUse = false;
		}
		if (pElement->get_child_element_attribute_value_by_name(L"senddesc", L"content") != L"")
		{
			giftinfo.strSendDesc = pElement->get_child_element_attribute_value_by_name(L"senddesc", L"content");
		}
		if (pElement->get_child_element_attribute_value_by_name(L"tipbtn", L"content") != L"")
		{
			giftinfo.strSendBtn = pElement->get_child_element_attribute_value_by_name(L"tipbtn", L"content");
		}
		if (pElement->get_child_element_attribute_value_by_name(L"prescription", L"content") != L"")
		{
			giftinfo.strPrescription = pElement->get_child_element_attribute_value_by_name(L"prescription", L"content");
		}
		if (pElement->get_child_element_attribute_value_by_name(L"giftwords", L"content") != L"")
		{
			giftinfo.strGiftWords = pElement->get_child_element_attribute_value_by_name(L"giftwords", L"content");
			giftinfo.bGiftWords = true;
		}

		common::utility::expat_element * pChildElement = pElement->get_element_by_name(L"param");
		if (pChildElement)
		{
			giftinfo.unPrice = common::utility::stringhelper::StringToInt(pChildElement->get_child_element_attribute_value_by_name(L"common", L"price"));
			giftinfo.unDstEarn = common::utility::stringhelper::StringToInt(pChildElement->get_child_element_attribute_value_by_name(L"donee", L"money"));
		}

		std::wstring strType = pElement->get_attribute_value(L"type");
		giftinfo.strType = strType;
		if (strType == L"stamp")
		{
			giftinfo.unGiftType = core::GIFTTYPE_STAMP;
			std::wstring strLabel = pElement->get_child_element_attribute_value_by_name(L"label", L"name");
			giftinfo.strCatalog = strLabel;
			if (std::find(vecStampLabel.begin(), vecStampLabel.end(), strLabel) == vecStampLabel.end())
			{
				vecStampLabel.push_back(strLabel);
			}
			giftinfo.strSmallImagePath = strModulePath + STAMP_DIR + pElement->get_attribute_value(L"icon");
			giftinfo.strLargeImagePath = strModulePath + STAMP_DIR + pElement->get_attribute_value(L"image");
		}
		else if (strType == L"luxury")
		{
			giftinfo.unGiftType = core::GIFTTYPE_SHECHI;
		}
		else
		{
			giftinfo.unGiftType = core::GIFTTYPE_NORMAL;
		}

		if (strCatalog != L"印章" && strCatalog != L"变声卡" && std::find(m_vecGiftCataList.begin(), m_vecGiftCataList.end(), strCatalog) == m_vecGiftCataList.end())
		{
			m_vecGiftCataList.push_back(strCatalog);
		}

		std::wstring strTarget = pElement->get_attribute_value(L"target");
		std::wstring strRange = pElement->get_attribute_value(L"range");
		if (strTarget == L"self")
		{
			giftinfo.unSendType = room::SENDTYPE_SELF;
		}
		else if (strTarget == L"everyone_except_self" && strRange == L"one")
		{
			giftinfo.unSendType = room::SENDTYPE_OTHERSINGLE;
		}
		else
		{
			giftinfo.unSendType = room::SENDTYPE_OTHERALL;
		}

		if (strRange == L"one")
		{
			giftinfo.unDstType = core::GIFTDSTTYPE_SINGLE;
		}
		else if (strRange == L"management in room")
		{
			giftinfo.unDstType = core::GIFTDSTTYPE_MANAGER;
		}
		else
		{
			giftinfo.unDstType = core::GIFTDSTTYPE_ALL;
		}

		if (strType == L"change voice")
		{
			CChangeVoiceItem * pGiftItem = new CChangeVoiceItem();
			giftinfo.unGiftType = core::GIFTTYPE_CHANGEVOICE;
			pGiftItem->SetGiftInfo(giftinfo);
			//std::wstring strVoice = pElement->get_child_element_attribute_value_by_name(L"voice", L"content");
			pChildElement = pElement->get_element_by_name(L"voice");
			if (pChildElement)
			{
				common::utility::expat_element * pVoiceElement = pChildElement->get_first_child_element();
				while (pVoiceElement)
				{
					std::wstring strVoice = pVoiceElement->get_attribute_value(L"content");
					pGiftItem->AddVoiceWords(strVoice);

					pVoiceElement = pVoiceElement->get_sibling_element();
				}
			}
			m_mapAllGiftItem[giftinfo.unID] = pGiftItem;
			m_mapChangeVoiceItem[giftinfo.unID] = pGiftItem;
		}
		else
		{
			GiftItem * pGiftItem = new GiftItem;
			pGiftItem->SetGiftInfo(giftinfo);
			m_mapAllGiftItem[giftinfo.unID] = pGiftItem;

			if (strType == L"stamp")
			{
				m_mapStampItem[pGiftItem->GetID()] = pGiftItem;
			}

			if (strType == L"blink enter")
			{
				m_mapBlinkEnterItem[pGiftItem->GetID()] = pGiftItem;
			}

			if (strType == L"fly words")
			{
				m_pFlyingWordsItem = pGiftItem;
			}

			if (strType == L"loudspeaker")
			{
				//common::utility::expat_element * pChildElement = pElement->get_element_by_name(L"param");
				//pGiftItem->m_GiftInfo.unPrice = common::utility::stringhelper::StringToInt(pChildElement->get_child_element_attribute_value_by_name(L"common", L"price"));
				//m_mapSpeakerItem[pGiftItem->GetID()] = pGiftItem;
				IGiftItem * pIGiftItem = pGiftItem;
				m_vecSpeakerItem.push_back(pIGiftItem);
			}

			if (strType == L"romantic choose song")
			{
				m_pWishSongItem = pGiftItem;
			}

		}

		pElement = pElement->get_sibling_element();
		nIndex++;
	}

	int nCount = 0;
	for (std::vector<std::wstring>::iterator it = vecStampLabel.begin(); it != vecStampLabel.end(); ++it)
	{
		m_mapStampTypeList[nCount++] = *it;
	}

	if (m_mapStampTypeList.size() != 0)
	{
		room::gift_info giftinfo;
		giftinfo.unID = 0;
		giftinfo.strName = L"印章";
		giftinfo.strRichName = L"印章";
		giftinfo.strDesc = L"给好友盖个章";
		giftinfo.unPrice = 2000;
		giftinfo.unDstEarn = 0;
		giftinfo.strSmallImagePath = strModulePath + GIFT_DIR + L"印章.png";
		giftinfo.strLargeImagePath = strModulePath + GIFT_DIR + L"印章.gif";
		giftinfo.strCatalog = L"道具";
		giftinfo.strSendBtn = L"点击选择印章";
		giftinfo.unSendType = room::SENDTYPE_NONE;
		giftinfo.strPrescription = L"10分钟";
		IGiftItem * pGiftItem = new GiftItem();
		pGiftItem->SetGiftInfo(giftinfo);
		m_vecPropsList.push_back(pGiftItem);
	}

	if (m_vecSpeakerItem.size() != 0)
	{
		room::gift_info giftinfo;
		giftinfo.unID = 0;
		giftinfo.strName = L"喇叭";
		giftinfo.strRichName = L"喇叭";
		giftinfo.strDesc = L"让全区、全平台收听您的声音";
		giftinfo.unPrice = 60000;
		giftinfo.unDstEarn = 0;
		giftinfo.strSmallImagePath = m_vecSpeakerItem[0]->GetSmallImagePath();
		giftinfo.strLargeImagePath = m_vecSpeakerItem[0]->GetLargeImagePath();
		giftinfo.strCatalog = L"道具";
		giftinfo.strSendBtn = L"点此发布";
		giftinfo.unSendType = room::SENDTYPE_NONE;
		IGiftItem * pGiftItem = new GiftItem();
		pGiftItem->SetGiftInfo(giftinfo);
		m_vecPropsList.push_back(pGiftItem);
	}

	if (m_mapChangeVoiceItem.size() != 0)
	{
		room::gift_info giftinfo;
		giftinfo.unID = 0;
		giftinfo.strName = L"变声卡";
		giftinfo.strRichName = L"变声卡";
		giftinfo.strDesc = L"使用“变声之术”将TA变成小动物";
		giftinfo.unPrice = 5000;
		giftinfo.unDstEarn = 0;
		giftinfo.strSmallImagePath = strModulePath + GIFT_DIR + L"变声卡.png";
		giftinfo.strLargeImagePath = strModulePath + GIFT_DIR + L"变声卡.gif";
		giftinfo.strCatalog = L"道具";
		giftinfo.strSendBtn = L"点此使用";
		giftinfo.unSendType = room::SENDTYPE_NONE;
		giftinfo.strPrescription = L"5分钟";
		IGiftItem * pGiftItem = new GiftItem();
		pGiftItem->SetGiftInfo(giftinfo);
		m_vecPropsList.push_back(pGiftItem);
	}

	if (m_pWishSongItem != NULL)
	{
		m_vecPropsList.push_back(m_pWishSongItem);
	}

	//if (m_pBlinkEnterItem != NULL)
	//{
	//	m_vecPropsList.push_back(m_pBlinkEnterItem);
	//}
	for (MapGiftItem::iterator it = m_mapBlinkEnterItem.begin(); it != m_mapBlinkEnterItem.end(); ++it)
	{
		m_vecPropsList.push_back(it->second);
	}

	//if (m_pFlyingWordsItem != NULL)
	//{
	//	m_vecPropsList.push_back(m_pFlyingWordsItem);
	//}

	return true;
}

IGiftItem * GiftManager::SearchGiftByID( uint16 unGiftID )
{
	if (m_mapAllGiftItem.find(unGiftID) != m_mapAllGiftItem.end())
	{
		return m_mapAllGiftItem[unGiftID];
	}

	return NULL;
}

IGiftItem * GiftManager::SearchStampByID( uint16 unGiftID )
{
	if (m_mapStampItem.find(unGiftID) != m_mapStampItem.end())
	{
		return m_mapStampItem[unGiftID];
	}

	return NULL;
}

std::map<int ,std::wstring>& GiftManager::GetStampTypeList() 
{
	return m_mapStampTypeList ;
}

bool GiftManager::IsStamp( uint16 unGiftID )
{
	if (m_mapStampItem.find(unGiftID) != m_mapStampItem.end())
	{
		return true;
	}

	return false;
}

bool Greater(IGiftItem * pGiftItem1, IGiftItem * pGiftItem2)
{
	return pGiftItem1->GetIndex() < pGiftItem2->GetIndex();
}

void GiftManager::GetGiftListByCatalog( std::wstring strCatalog, std::vector<IGiftItem*> & vecGiftList )
{
	if (strCatalog == L"道具")
	{
		vecGiftList = m_vecPropsList;
		return;
	}
	CTime curTime = CTime::GetCurrentTime(); 
	for (MapGiftItem::iterator it = m_mapAllGiftItem.begin(); it != m_mapAllGiftItem.end(); ++it)
	{
		IGiftItem * pGiftItem = it->second;
		if (pGiftItem != NULL && pGiftItem->GetCatalogName() == strCatalog)
		{
			if (!pGiftItem->IsInUse())
			{
				continue;
			}
			if (curTime < pGiftItem->GetBeginTime() || curTime > pGiftItem->GetEndTime())
			{
				continue;
			}
			vecGiftList.push_back(pGiftItem);
		}
	}

	std::sort(vecGiftList.begin(), vecGiftList.end(), Greater);
}

void GiftManager::GetStampListByCatalog( std::wstring strCatalog, std::vector<IGiftItem*> & vecStampList )
{
	CTime curTime = CTime::GetCurrentTime(); 
	for (MapGiftItem::iterator it = m_mapStampItem.begin(); it != m_mapStampItem.end(); ++it)
	{
		IGiftItem * pGiftItem = it->second;
		if (pGiftItem != NULL && pGiftItem->GetCatalogName() == strCatalog)
		{
			if (!pGiftItem->IsInUse())
			{
				continue;
			}
			if (curTime < pGiftItem->GetBeginTime() || curTime > pGiftItem->GetEndTime())
			{
				continue;
			}
			vecStampList.push_back(pGiftItem);
		}
	}

	std::sort(vecStampList.begin(), vecStampList.end(), Greater);
}

IGiftItem * GiftManager::GetGiftItemByName( std::wstring strName )
{
	for (MapGiftItem::iterator it = m_mapAllGiftItem.begin(); it != m_mapAllGiftItem.end(); ++it)
	{
		IGiftItem * pGiftItem = it->second;
		if (pGiftItem->GetName() == strName)
		{
			return pGiftItem;
		}
	}

	return NULL;
}

bool GiftManager::IsFirework( uint16 unGiftID )
{
	if (m_mapAllGiftItem.find(unGiftID) != m_mapAllGiftItem.end())
	{
		return m_mapAllGiftItem[unGiftID]->GetName() == SPECIAL_GIFTNAME_FIREWORK;
	}

	return false;
}

bool GiftManager::IsSalute( uint16 unGiftID )
{
	if (m_mapAllGiftItem.find(unGiftID) != m_mapAllGiftItem.end())
	{
		return m_mapAllGiftItem[unGiftID]->GetName() == SPECIAL_GIFTNAME_SALUTE;
	}

	return false;
}

GiftManager * GiftManager::GetInstance()
{
	if (s_pGiftManager == NULL)
	{
		s_pGiftManager = new GiftManager();
	}

	return s_pGiftManager;
}

void GiftManager::DeleteInstance()
{
	if (s_pGiftManager)
	{
		delete s_pGiftManager;
		s_pGiftManager = NULL;
	}
}

std::vector<std::wstring>& GiftManager::GetGiftCatalogList()
{
	return m_vecGiftCataList;
}

std::vector<IGiftItem*>& GiftManager::GetPropsList()
{
	return m_vecPropsList;
}

VecGiftItem& GiftManager::GetSpeakerList()
{
	return m_vecSpeakerItem;
}

IGiftItem * GiftManager::GetWishSongItem()
{
	return m_pWishSongItem;
}

void GiftManager::GetChangeVoiceList( VecGiftItem & vecChangeVoiceList )
{
	for (MapGiftItem::iterator it = m_mapAllGiftItem.begin(); it != m_mapAllGiftItem.end(); ++it)
	{
		IGiftItem * pGiftItem = it->second;
		if (pGiftItem != NULL && pGiftItem->GetStrType() == L"change voice")
		{
			vecChangeVoiceList.push_back(pGiftItem);
		}
	}
}

bool GiftManager::IsChangeVoice(uint16 unGiftID)
{
	if (m_mapChangeVoiceItem.find(unGiftID) != m_mapChangeVoiceItem.end())
	{
		return true;
	}

	return false;
}

IGiftItem * GiftManager::GetFlyingWordsItem()
{
	return m_pFlyingWordsItem;
}

IGiftItem * GiftManager::SearchBlinkEnterByID( uint16 unGiftID )
{
	if (m_mapBlinkEnterItem.find(unGiftID) != m_mapBlinkEnterItem.end())
	{
		return m_mapBlinkEnterItem[unGiftID];
	}

	return NULL;
}

void GiftManager::Release()
{
	for (VecGiftItem::iterator it = m_vecPropsList.begin(); it != m_vecPropsList.end(); ++it)
	{
		if (*it != NULL)
		{
			if ((*it)->GetStrType() == L"blink enter" || *it == m_pWishSongItem || *it == m_pFlyingWordsItem)
			{
				continue;
			}
			delete *it;
			*it = NULL;
		}
	}
	for (MapGiftItem::iterator it = m_mapAllGiftItem.begin(); it != m_mapAllGiftItem.end(); ++it)
	{
		GiftItem * pGiftItem = it->second;
		if (it->second != NULL)
		{
			delete it->second;
			it->second = NULL;
		}
	}

	m_mapAllGiftItem.clear();
	m_mapStampItem.clear();
	m_mapBlinkEnterItem.clear();
	m_vecSpeakerItem.clear();
	m_mapStampTypeList.clear();
	m_vecGiftCataList.clear();
	m_vecPropsList.clear();
	m_mapChangeVoiceItem.clear();
}

bool GiftManager::ReloadGiftList()
{
	Release();
	return LoadGiftList();
}