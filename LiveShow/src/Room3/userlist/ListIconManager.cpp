#include "StdAfx.h"
#include "ListIconManager.h"
#include "utility/expat++/expat_document.h"
#include "utility/StringHelper.h"
#include "utility/SystemHelper.h"
#include "utility/URLEncode.h"
#include "..\GlobalRoomDefine.h"

MapIconList ListIconManager::g_mapIconList;

ListIconManager::ListIconManager(void)
{
}

ListIconManager::~ListIconManager(void)
{
}

bool ListIconManager::LoadListIcon()
{
	common::utility::expat_document xmlDoc;

	std::wstring strModulePath = common::utility::systemhelper::Get179AppPath();

	common::CURLEncode urlEncode;
	strModulePath = urlEncode.URLEncodeLocalPath(strModulePath);

	std::wstring strXmlPath = strModulePath + LISTICON_CONFIG_PATH;

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
	while (pElement)
	{
		ListIconInfo * pIconInfo = new ListIconInfo;
		uint32 unIndex = common::utility::stringhelper::StringToInt(pElement->get_attribute_value(L"Index"));
		pIconInfo->enmIconIndex = (ENM_UserListIcon)unIndex;
		pIconInfo->strPath = strModulePath + LISTICON_DIR + pElement->get_attribute_value(L"Path");
		pIconInfo->strTip = pElement->get_attribute_value(L"Tip");

		g_mapIconList[unIndex] = pIconInfo;

		pElement = pElement->get_sibling_element();
	}
	return true;
}

std::wstring ListIconManager::GetVIPIcon(IUserItem* pUserItem) 
{
	switch (pUserItem->GetUserVipLevel())
	{
	case core::VIPLEVEL_NONE:
		{
			return L"";
		}
		break;
	case core::VIPLEVEL_REGISTER:
		{
			return L"";
		}
		break;
	case core::VIPLEVEL_VIP:
		{
			return g_mapIconList[USERLIST_ICON_VIP_RED]->strPath;
		}
		break;
	case core::VIPLEVEL_HONOR:
		{
			return g_mapIconList[USERLIST_ICON_VIP_PURPLE]->strPath;
		}
		break;
	case core::VIPLEVEL_SILVER:
		{
			return g_mapIconList[USERLIST_ICON_VIP_REDKING]->strPath;
		}
		break;
	case core::VIPLEVEL_GOLDEN:
		{
			return g_mapIconList[USERLIST_ICON_VIP_PURPLEKING]->strPath;
		}
		break;
	case core::VIPLEVEL_CRYSTAL:
		{
			return g_mapIconList[USERLIST_ICON_VIP_SUPER]->strPath;
		}
		break;
	case core::VIPLEVEL_DIAMOND:
		{
			return g_mapIconList[USERLIST_ICON_VIP_DIAMOND]->strPath;
		}
		break;
	case core::VIPLEVEL_BLACKDIAMOND:
		{
			return g_mapIconList[USERLIST_ICON_VIP_BLACKDIAMOND]->strPath;
		}
		break;
	case core::VIPLEVEL_TOP:
		{
			return g_mapIconList[USERLIST_ICON_VIP_TOP]->strPath;
		}
		break;
	default:
		break;
	}

	return L"";
}

std::wstring ListIconManager::GetFirstColumnIcon( IUserItem * pUserItem )
{
	if (pUserItem->IsForbidden())
	{
		return g_mapIconList[USERLIST_ICON_SHUTUP]->strPath;
	}

	switch (pUserItem->GetUserVipLevel())
	{
	case core::VIPLEVEL_NONE:
		{
			return L"";
		}
		break;
	case core::VIPLEVEL_REGISTER:
		{
			return L"";
		}
		break;
	case core::VIPLEVEL_VIP:
		{
			return g_mapIconList[USERLIST_ICON_VIP_RED]->strPath;
		}
		break;
	case core::VIPLEVEL_HONOR:
		{
			return g_mapIconList[USERLIST_ICON_VIP_PURPLE]->strPath;
		}
		break;
	case core::VIPLEVEL_SILVER:
		{
			return g_mapIconList[USERLIST_ICON_VIP_REDKING]->strPath;
		}
		break;
	case core::VIPLEVEL_GOLDEN:
		{
			return g_mapIconList[USERLIST_ICON_VIP_PURPLEKING]->strPath;
		}
		break;
	case core::VIPLEVEL_CRYSTAL:
		{
			return g_mapIconList[USERLIST_ICON_VIP_SUPER]->strPath;
		}
		break;
	case core::VIPLEVEL_DIAMOND:
		{
			return g_mapIconList[USERLIST_ICON_VIP_DIAMOND]->strPath;
		}
		break;
	case core::VIPLEVEL_BLACKDIAMOND:
		{
			return g_mapIconList[USERLIST_ICON_VIP_BLACKDIAMOND]->strPath;
		}
		break;
	case core::VIPLEVEL_TOP:
		{
			return g_mapIconList[USERLIST_ICON_VIP_TOP]->strPath;
		}
		break;
	default:
		break;
	}

	return L"";
}

std::wstring ListIconManager::GetSecondColumnIcon( IUserItem * pUserItem )
{
	if (pUserItem->IsPublicOne() || pUserItem->IsPublicTwo() || pUserItem->IsPublicThree())
	{
		return g_mapIconList[USERLIST_ICON_PUB_MIC]->strPath;
	}

	if (pUserItem->IsPrivate())
	{
		return g_mapIconList[USERLIST_ICON_PRI_MIC]->strPath;
	}

	if (pUserItem->IsP2P())
	{
		return g_mapIconList[USERLIST_ICON_P2P_MIC]->strPath;
	}

	//ÓÐÉãÏñÍ·
	if (pUserItem->IsHaveCam())
	{
		return g_mapIconList[USERLIST_ICON_NORMAL_MIC]->strPath;
	}
	
	return L"";
}

std::wstring ListIconManager::GetThirdColumnIcon( IUserItem * pUserItem )
{
	if (pUserItem->IsPatrolMan())
	{
		return g_mapIconList[USERLIST_ICON_PATROLMAN]->strPath;
	}

	if (pUserItem->IsOperator())
	{
		return g_mapIconList[USERLIST_ICON_OPERATOR]->strPath;
	}

	if (pUserItem->GetUserTitleLevel() == core::TITLELEVEL_OWNER)
	{
		if (pUserItem->IsMale())
		{
			return g_mapIconList[USERLIST_ICON_MALE_OWNER]->strPath;
		}
		else
		{
			return g_mapIconList[USERLIST_ICON_FEMALE_OWNER]->strPath;
		}
	}

	if (pUserItem->IsSinger())
	{
		return g_mapIconList[USERLIST_ICON_SINGER]->strPath;
	}

	if (pUserItem->IsDancer())
	{
		return g_mapIconList[USERLIST_ICON_DANCER]->strPath;
	}

	if (pUserItem->IsHost())
	{
		return g_mapIconList[USERLIST_ICON_HOST]->strPath;
	}

	if (pUserItem->GetUserTitleLevel() == core::TITLELEVEL_SECOND_OWNER)
	{
		if (pUserItem->IsMale())
		{
			return g_mapIconList[USERLIST_ICON_MALE_VICEOWNER]->strPath;
		}
		else
		{
			return g_mapIconList[USERLIST_ICON_FEMALE_VICEOWNER]->strPath;
		}
	}

	if (pUserItem->GetUserTitleLevel() == core::TITLELEVEL_MANAGER)
	{
		if (pUserItem->IsMale())
		{
			return g_mapIconList[USERLIST_ICON_MALE_MANAGER]->strPath;
		}
		else
		{
			return g_mapIconList[USERLIST_ICON_FEMALE_MANAGER]->strPath;
		}
	}

	if (pUserItem->GetUserTitleLevel() == core::TITLELEVEL_TEMP_MANAGER)
	{
		if (pUserItem->IsMale())
		{
			return g_mapIconList[USERLIST_ICON_MALE_TEMPMANAGER]->strPath;
		}
		else
		{
			return g_mapIconList[USERLIST_ICON_FEMALE_TEMPMANAGER]->strPath;
		}
	}

	//ÌøÎè±¦±´

	if (pUserItem->GetUserVipLevel() == core::VIPLEVEL_NONE)
	{
		return g_mapIconList[USERLIST_ICON_GUEST]->strPath;
	}
	else
	{
		if (pUserItem->IsMale())
		{
			return g_mapIconList[USERLIST_ICON_USERLEVEL_MALE]->strPath;
		}
		else
		{
			return g_mapIconList[USERLIST_ICON_USERLEVEL_FEMALE]->strPath;
		}
	}

	//uint8 unUserLevel = pUserItem->GetUserUsrLevel();
	//if (unUserLevel <= 10)
	//{
	//	return g_mapIconList[USERLIST_ICON_USERLEVEL_ONE]->strPath;
	//}
	//else if (unUserLevel <= 30)
	//{
	//	return g_mapIconList[USERLIST_ICON_USERLEVEL_TWO]->strPath;
	//}
	//else if (unUserLevel <= 50)
	//{
	//	return g_mapIconList[USERLIST_ICON_USERLEVEL_THREE]->strPath;
	//}
	//else if (unUserLevel <= 70)
	//{
	//	return g_mapIconList[USERLIST_ICON_USERLEVEL_FOUR]->strPath;
	//}
	//else if (unUserLevel <= 90)
	//{
	//	return g_mapIconList[USERLIST_ICON_USERLEVEL_FIVE]->strPath;
	//}
	//else if (unUserLevel <= 100)
	//{
	//	return g_mapIconList[USERLIST_ICON_USERLEVEL_SIX]->strPath;
	//}

	return L"";
}

std::wstring ListIconManager::GetFourthColumnIcon( IUserItem * pUserItem )
{
	//ÌøÎè±¦±´
	return L"";
}

std::wstring ListIconManager::GetIconPathByIndex( uint32 unIndex )
{
	if (g_mapIconList.find(unIndex) != g_mapIconList.end())
	{
		return g_mapIconList[unIndex]->strPath;
	}

	return L"";
}

std::wstring ListIconManager::GetIconTipByIndex( uint32 unIndex )
{
	if (g_mapIconList.find(unIndex) != g_mapIconList.end())
	{
		return g_mapIconList[unIndex]->strTip;
	}

	return L"";
}

uint32 ListIconManager::GetFirstColumnIconIndex( IUserItem * pUserItem )
{
	if (pUserItem->IsForbidden())
	{
		return USERLIST_ICON_SHUTUP;
	}

	if (pUserItem->IsAgent())
	{
		return USERLIST_ICON_AGENT;
	}

	switch (pUserItem->GetUserVipLevel())
	{
	case core::VIPLEVEL_NONE:
		{
			return USERLIST_ICON_NONE;
		}
		break;
	case core::VIPLEVEL_REGISTER:
		{
			return USERLIST_ICON_NONE;
		}
		break;
	case core::VIPLEVEL_VIP:
		{
			return USERLIST_ICON_VIP_RED;
		}
		break;
	case core::VIPLEVEL_HONOR:
		{
			return USERLIST_ICON_VIP_PURPLE;
		}
		break;
	case core::VIPLEVEL_SILVER:
		{
			return USERLIST_ICON_VIP_REDKING;
		}
		break;
	case core::VIPLEVEL_GOLDEN:
		{
			return USERLIST_ICON_VIP_PURPLEKING;
		}
		break;
	case core::VIPLEVEL_CRYSTAL:
		{
			return USERLIST_ICON_VIP_SUPER;
		}
		break;
	case core::VIPLEVEL_DIAMOND:
		{
			return USERLIST_ICON_VIP_DIAMOND;
		}
		break;
	case core::VIPLEVEL_BLACKDIAMOND:
		{
			return USERLIST_ICON_VIP_BLACKDIAMOND;
		}
		break;
	case core::VIPLEVEL_TOP:
		{
			return USERLIST_ICON_VIP_TOP;
		}
		break;
	default:
		break;
	}

	return USERLIST_ICON_NONE;
}

uint32 ListIconManager::GetSecondColumnIconIndex( IUserItem * pUserItem )
{
	if (pUserItem->IsPublicOne() || pUserItem->IsPublicTwo() || pUserItem->IsPublicThree())
	{
		return USERLIST_ICON_PUB_MIC;
	}

	if (pUserItem->IsPrivate())
	{
		return USERLIST_ICON_PRI_MIC;
	}

	if (pUserItem->IsP2P())
	{
		return USERLIST_ICON_P2P_MIC;
	}

	//ÓÐÉãÏñÍ·
	if (pUserItem->IsHaveCam())
	{
		return USERLIST_ICON_NORMAL_MIC;
	}

	return USERLIST_ICON_NONE;
}

uint32 ListIconManager::GetThirdColumnIconIndex( IUserItem * pUserItem )
{
	if (pUserItem->IsPatrolMan())
	{
		return USERLIST_ICON_PATROLMAN;
	}

	if (pUserItem->IsOperator())
	{
		return USERLIST_ICON_OPERATOR;
	}

	if (pUserItem->GetUserTitleLevel() == core::TITLELEVEL_OWNER)
	{
		if (pUserItem->IsMale())
		{
			return USERLIST_ICON_MALE_OWNER;
		}
		else
		{
			return USERLIST_ICON_FEMALE_OWNER;
		}
	}

	if (pUserItem->IsSinger())
	{
		return USERLIST_ICON_SINGER;
	}

	if (pUserItem->IsDancer())
	{
		return USERLIST_ICON_DANCER;
	}

	if (pUserItem->IsHost())
	{
		return USERLIST_ICON_HOST;
	}

	if (pUserItem->GetUserTitleLevel() == core::TITLELEVEL_SECOND_OWNER)
	{
		if (pUserItem->IsMale())
		{
			return USERLIST_ICON_MALE_VICEOWNER;
		}
		else
		{
			return USERLIST_ICON_FEMALE_VICEOWNER;
		}
	}

	if (pUserItem->GetUserTitleLevel() == core::TITLELEVEL_MANAGER)
	{
		if (pUserItem->IsMale())
		{
			return USERLIST_ICON_MALE_MANAGER;
		}
		else
		{
			return USERLIST_ICON_FEMALE_MANAGER;
		}
	}

	if (pUserItem->GetUserTitleLevel() == core::TITLELEVEL_TEMP_MANAGER)
	{
		if (pUserItem->IsMale())
		{
			return USERLIST_ICON_MALE_TEMPMANAGER;
		}
		else
		{
			return USERLIST_ICON_FEMALE_TEMPMANAGER;
		}
	}

	if (pUserItem->GetUserVipLevel() == core::VIPLEVEL_NONE)
	{
		return USERLIST_ICON_GUEST;
	}
	else
	{
		if (pUserItem->IsMale())
		{
			return USERLIST_ICON_USERLEVEL_MALE;
		}
		else
		{
			return USERLIST_ICON_USERLEVEL_FEMALE;
		}
	}

	//uint8 unUserLevel = pUserItem->GetUserUsrLevel();
	//if (unUserLevel <= 10)
	//{
	//	return USERLIST_ICON_USERLEVEL_ONE;
	//}
	//else if (unUserLevel <= 30)
	//{
	//	return USERLIST_ICON_USERLEVEL_TWO;
	//}
	//else if (unUserLevel <= 50)
	//{
	//	return USERLIST_ICON_USERLEVEL_THREE;
	//}
	//else if (unUserLevel <= 70)
	//{
	//	return USERLIST_ICON_USERLEVEL_FOUR;
	//}
	//else if (unUserLevel <= 90)
	//{
	//	return USERLIST_ICON_USERLEVEL_FIVE;
	//}
	//else if (unUserLevel <= 100)
	//{
	//	return USERLIST_ICON_USERLEVEL_SIX;
	//}

	return USERLIST_ICON_NONE;
}

uint32 ListIconManager::GetFourthColumnIconIndex( IUserItem * pUserItem )
{
	return USERLIST_ICON_NONE;
}

uint32 ListIconManager::GetIconIndexByColumn( IUserItem * pUserItem, uint32 unColumn )
{
	switch (unColumn)
	{
	case 0:
		{
			return GetFirstColumnIconIndex(pUserItem);
		}
		break;
	case 1:
		{
			return USERLIST_ICON_NONE;
		}
		break;
	case 2:
		{
			return GetSecondColumnIconIndex(pUserItem);
		}
		break;
	case 3:
		{
			return GetThirdColumnIconIndex(pUserItem);
		}
		break;
	case 4:
		{
			return GetFourthColumnIconIndex(pUserItem);
		}
		break;
	case 5:
		{
			return USERLIST_ICON_NONE;
		}
		break;
	default:
		break;
	}

	return USERLIST_ICON_NONE;
}

void ListIconManager::Release()
{
	for (MapIconList::iterator it = g_mapIconList.begin(); it != g_mapIconList.end(); ++it)
	{
		if (it->second != NULL)
		{
			delete it->second;
		}
	}
	g_mapIconList.clear();
}

std::wstring ListIconManager::GetIconPathByVipLevel( uint8 unVipLevel )
{
	switch (unVipLevel)
	{
	case core::VIPLEVEL_VIP:
		{
			return g_mapIconList[USERLIST_ICON_VIP_RED]->strPath;
		}
		break;
	case core::VIPLEVEL_HONOR:
		{
			return g_mapIconList[USERLIST_ICON_VIP_PURPLE]->strPath;
		}
		break;
	case core::VIPLEVEL_SILVER:
		{
			return g_mapIconList[USERLIST_ICON_VIP_REDKING]->strPath;
		}
		break;
	case core::VIPLEVEL_GOLDEN:
		{
			return g_mapIconList[USERLIST_ICON_VIP_PURPLEKING]->strPath;
		}
		break;
	case core::VIPLEVEL_CRYSTAL:
		{
			return g_mapIconList[USERLIST_ICON_VIP_SUPER]->strPath;
		}
		break;
	case core::VIPLEVEL_DIAMOND:
		{
			return g_mapIconList[USERLIST_ICON_VIP_DIAMOND]->strPath;
		}
		break;
	case core::VIPLEVEL_BLACKDIAMOND:
		{
			return g_mapIconList[USERLIST_ICON_VIP_BLACKDIAMOND]->strPath;
		}
		break;
	case core::VIPLEVEL_TOP:
		{
			return g_mapIconList[USERLIST_ICON_VIP_TOP]->strPath;
		}
		break;
	default:
		break;
	}

	return L"";
}

void ListIconManager::GetAllIcon( std::map<int, std::wstring> & mapIconPath )
{
	for (MapIconList::iterator it = g_mapIconList.begin(); it != g_mapIconList.end(); ++it)
	{
		mapIconPath[it->first] = it->second->strPath;
	}
}

std::wstring ListIconManager::GetOutputIcon( IUserItem * pUserItem ) 
{
	if (pUserItem->IsOperator())
	{
		return g_mapIconList[USERLIST_ICON_OPERATOR]->strPath;
	}

	if (pUserItem->IsPatrolMan())
	{
		return g_mapIconList[USERLIST_ICON_PATROLMAN]->strPath;
	}

	if (pUserItem->IsAdCleaner())
	{
		return g_mapIconList[USERLIST_ICON_ADCLEANER]->strPath;
	}

	if (pUserItem->IsAgent())
	{
		return g_mapIconList[USERLIST_ICON_AGENT]->strPath;
	}

	if (pUserItem->IsSinger())
	{
		return g_mapIconList[USERLIST_ICON_SINGER]->strPath;
	}

	if (pUserItem->IsDancer())
	{
		return g_mapIconList[USERLIST_ICON_DANCER]->strPath;
	}

	if (pUserItem->IsHost())
	{
		return g_mapIconList[USERLIST_ICON_HOST]->strPath;
	}

	switch (pUserItem->GetUserVipLevel())
	{
	case core::VIPLEVEL_VIP:	//vip
		{
			return g_mapIconList[USERLIST_ICON_VIP_RED]->strPath;
		}
		break;
	case core::VIPLEVEL_HONOR: //ÈÙÒ«vip
		{
			return g_mapIconList[USERLIST_ICON_VIP_PURPLE]->strPath;
		}
		break;
	case core::VIPLEVEL_SILVER:
		{
			return g_mapIconList[USERLIST_ICON_VIP_REDKING]->strPath;
		}
		break;
	case core::VIPLEVEL_GOLDEN:
		{
			return g_mapIconList[USERLIST_ICON_VIP_PURPLEKING]->strPath;
		}
		break;
	case core::VIPLEVEL_CRYSTAL:
		{
			return g_mapIconList[USERLIST_ICON_VIP_SUPER]->strPath;
		}
		break;
	case core::VIPLEVEL_DIAMOND:
		{
			return g_mapIconList[USERLIST_ICON_VIP_DIAMOND]->strPath;
		}
		break;
	case core::VIPLEVEL_BLACKDIAMOND:
		{
			return g_mapIconList[USERLIST_ICON_VIP_BLACKDIAMOND]->strPath;
		}
		break;
	case core::VIPLEVEL_TOP:
		{
			return g_mapIconList[USERLIST_ICON_VIP_TOP]->strPath;
		}
		break;
	default:
		break;
	}
	return L"" ;
}

std::wstring ListIconManager::GetVIPIconStatic( IUserItem * pUserItem )
{
	switch (pUserItem->GetUserVipLevel())
	{
	case core::VIPLEVEL_NONE:
		{
			return g_mapIconList[USERLIST_ICON_VIP_NONE]->strPath;
		}
		break;
	case core::VIPLEVEL_REGISTER:
		{
			return g_mapIconList[USERLIST_ICON_VIP_NONE]->strPath;
		}
		break;
	case core::VIPLEVEL_VIP:
		{
			return g_mapIconList[USERLIST_ICON_VIP_RED_STATIC]->strPath;
		}
		break;
	case core::VIPLEVEL_HONOR:
		{
			return g_mapIconList[USERLIST_ICON_VIP_PURPLE_STATIC]->strPath;
		}
		break;
	case core::VIPLEVEL_SILVER:
		{
			return g_mapIconList[USERLIST_ICON_VIP_REDKING_STATIC]->strPath;
		}
		break;
	case core::VIPLEVEL_GOLDEN:
		{
			return g_mapIconList[USERLIST_ICON_VIP_PURPLEKING_STATIC]->strPath;
		}
		break;
	case core::VIPLEVEL_CRYSTAL:
		{
			return g_mapIconList[USERLIST_ICON_VIP_SUPER_STATIC]->strPath;
		}
		break;
	case core::VIPLEVEL_DIAMOND:
		{
			return g_mapIconList[USERLIST_ICON_VIP_DIAMOND_STATIC]->strPath;
		}
		break;
	case core::VIPLEVEL_BLACKDIAMOND:
		{
			return g_mapIconList[USERLIST_ICON_VIP_BLACKDIAMOND_STATIC]->strPath;
		}
		break;
	case core::VIPLEVEL_TOP:
		{
			return g_mapIconList[USERLIST_ICON_VIP_TOP_STATIC]->strPath;
		}
		break;
	default:
		break;
	}

	return L"";
}

std::wstring ListIconManager::GetIconTipByVipLevel( uint8 unVipLevel )
{
	switch (unVipLevel)
	{
	case core::VIPLEVEL_NONE:
		{
			return L"";
		}
		break;
	case core::VIPLEVEL_REGISTER:
		{
			return L"";
		}
		break;
	case core::VIPLEVEL_VIP:
		{
			return g_mapIconList[USERLIST_ICON_VIP_RED]->strTip;
		}
		break;
	case core::VIPLEVEL_HONOR:
		{
			return g_mapIconList[USERLIST_ICON_VIP_PURPLE]->strTip;
		}
		break;
	case core::VIPLEVEL_SILVER:
		{
			return g_mapIconList[USERLIST_ICON_VIP_REDKING]->strTip;
		}
		break;
	case core::VIPLEVEL_GOLDEN:
		{
			return g_mapIconList[USERLIST_ICON_VIP_PURPLEKING]->strTip;
		}
		break;
	case core::VIPLEVEL_CRYSTAL:
		{
			return g_mapIconList[USERLIST_ICON_VIP_SUPER]->strTip;
		}
		break;
	case core::VIPLEVEL_DIAMOND:
		{
			return g_mapIconList[USERLIST_ICON_VIP_DIAMOND]->strTip;
		}
		break;
	case core::VIPLEVEL_BLACKDIAMOND:
		{
			return g_mapIconList[USERLIST_ICON_VIP_BLACKDIAMOND]->strTip;
		}
		break;
	case core::VIPLEVEL_TOP:
		{
			return g_mapIconList[USERLIST_ICON_VIP_TOP]->strTip;
		}
		break;
	default:
		break;
	}

	return L"";
}

std::wstring ListIconManager::GetIconTipByUser( IUserItem * pUserItem )
{
	if (pUserItem->IsPatrolMan())
	{
		return g_mapIconList[USERLIST_ICON_PATROLMAN]->strTip;
	}

	if (pUserItem->IsOperator())
	{
		return g_mapIconList[USERLIST_ICON_OPERATOR]->strTip;
	}

	if (pUserItem->GetUserTitleLevel() == core::TITLELEVEL_OWNER)
	{
		if (pUserItem->IsMale())
		{
			return g_mapIconList[USERLIST_ICON_MALE_OWNER]->strTip;
		}
		else
		{
			return g_mapIconList[USERLIST_ICON_FEMALE_OWNER]->strTip;
		}
	}

	if (pUserItem->IsSinger())
	{
		return g_mapIconList[USERLIST_ICON_SINGER]->strTip;
	}

	if (pUserItem->IsDancer())
	{
		return g_mapIconList[USERLIST_ICON_DANCER]->strTip;
	}

	if (pUserItem->IsHost())
	{
		return g_mapIconList[USERLIST_ICON_HOST]->strTip;
	}

	if (pUserItem->GetUserTitleLevel() == core::TITLELEVEL_SECOND_OWNER)
	{
		if (pUserItem->IsMale())
		{
			return g_mapIconList[USERLIST_ICON_MALE_VICEOWNER]->strTip;
		}
		else
		{
			return g_mapIconList[USERLIST_ICON_FEMALE_VICEOWNER]->strTip;
		}
	}

	if (pUserItem->GetUserTitleLevel() == core::TITLELEVEL_MANAGER)
	{
		if (pUserItem->IsMale())
		{
			return g_mapIconList[USERLIST_ICON_MALE_MANAGER]->strTip;
		}
		else
		{
			return g_mapIconList[USERLIST_ICON_FEMALE_MANAGER]->strTip;
		}
	}

	if (pUserItem->GetUserTitleLevel() == core::TITLELEVEL_TEMP_MANAGER)
	{
		if (pUserItem->IsMale())
		{
			return g_mapIconList[USERLIST_ICON_MALE_TEMPMANAGER]->strTip;
		}
		else
		{
			return g_mapIconList[USERLIST_ICON_FEMALE_TEMPMANAGER]->strTip;
		}
	}

	//ÌøÎè±¦±´

	if (pUserItem->GetUserVipLevel() == core::VIPLEVEL_NONE)
	{
		return g_mapIconList[USERLIST_ICON_GUEST]->strTip;
	}
	else
	{
		if (pUserItem->IsMale())
		{
			return L"ÆÕÍ¨ÓÃ»§";
			//return g_mapIconList[USERLIST_ICON_USERLEVEL_MALE]->strTip;
		}
		else
		{
			return L"ÆÕÍ¨ÓÃ»§";
			//return g_mapIconList[USERLIST_ICON_USERLEVEL_FEMALE]->strTip;
		}
	}

	return L"";
}