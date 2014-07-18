#include "StdAfx.h"
#include "RichLevelManager.h"
#include "utility\expat++\expat_document.h"
#include "utility\StringHelper.h"
#include "utility\SystemHelper.h"
#include "utility\URLEncode.h"
#include "..\GlobalRoomDefine.h"

MapRichInfoList RichLevelManager::g_mapRichInfoList;

RichLevelManager::RichLevelManager(void)
{
}

RichLevelManager::~RichLevelManager(void)
{
}

bool RichLevelManager::LoadRichInfo()
{
	common::utility::expat_document xmlDoc;

	std::wstring strModulePath = common::utility::systemhelper::Get179AppPath();

	common::CURLEncode urlEncode;
	strModulePath = urlEncode.URLEncodeLocalPath(strModulePath);

	std::wstring strXmlPath = strModulePath + RICHLEVEL_CONFIG_PATH;

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
		RichLevelInfo info;
		info.unRichLevel = common::utility::stringhelper::StringToInt(pElement->get_attribute_value(L"Index"));
		info.strImgPath = strModulePath + RICHLEVEL_DIR + pElement->get_attribute_value(L"Path");
		info.strLevelName = pElement->get_attribute_value(L"Tip");

		g_mapRichInfoList[info.unRichLevel] = info;

		pElement = pElement->get_sibling_element();
	}
	return true;
}

void RichLevelManager::Release()
{
	g_mapRichInfoList.clear();
}

std::wstring RichLevelManager::GetRichLevelName( IUserItem * pUserItem )
{
	if (g_mapRichInfoList.find(pUserItem->GetUserRichLevel()) != g_mapRichInfoList.end())
	{
		return g_mapRichInfoList[pUserItem->GetUserRichLevel()].strLevelName;
	}

	return L"";
}

std::wstring RichLevelManager::GetRichLevelImgPath( IUserItem * pUserItem )
{
	if (g_mapRichInfoList.find(pUserItem->GetUserRichLevel()) != g_mapRichInfoList.end())
	{
		return g_mapRichInfoList[pUserItem->GetUserRichLevel()].strImgPath;
	}

	return L"";
}

void RichLevelManager::GetAllRichImg( std::map<uint8, std::wstring> & mapRichImgPath )
{
	for (MapRichInfoList::iterator it = g_mapRichInfoList.begin(); it != g_mapRichInfoList.end(); ++it)
	{
		mapRichImgPath[it->first] = it->second.strImgPath;
	}
}

std::wstring RichLevelManager::GetNextRichLevelName( IUserItem * pUserItem )
{
	if (g_mapRichInfoList.find(pUserItem->GetUserRichLevel() + 1) != g_mapRichInfoList.end())
	{
		return g_mapRichInfoList[pUserItem->GetUserRichLevel() + 1].strLevelName;
	}

	return L"";
}

std::wstring RichLevelManager::GetRichLevelNameByLevel( uint8 unRichLevel )
{
	if (g_mapRichInfoList.find(unRichLevel) != g_mapRichInfoList.end())
	{
		return g_mapRichInfoList[unRichLevel].strLevelName;
	}

	return L"";
}

std::wstring RichLevelManager::GetRichLevelImgPathByLevel( uint8 unRichLevel )
{
	if (g_mapRichInfoList.find(unRichLevel) != g_mapRichInfoList.end())
	{
		return g_mapRichInfoList[unRichLevel].strImgPath;
	}

	return L"";
}