#include "StdAfx.h"
#include "TempNoticeManager.h"
#include "utility/expat++/expat_document.h"
#include "..\..\CommonLibrary/include\utility\FileHelper.h"
#include "..\..\CommonLibrary\include\utility\StringHelper.h"

CTempNoticeManager::CTempNoticeManager(void)
{
}

CTempNoticeManager::~CTempNoticeManager(void)
{
}

bool CTempNoticeManager::SaveXML( std::wstring strPath, std::wstring strFile, VecNoticeInfo & noticelist )
{
	if (!PathFileExists(strPath.c_str()))
	{
		common::utility::filehelper::CreateAllDirectory(strPath);
	}

	std::wstring strFullFilePath = strPath + L"\\" + strFile;
	CFile xmlFile;
	if (!xmlFile.Open(strFullFilePath.c_str(), CFile::modeCreate|CFile::modeWrite))
	{
		return false;
	}

	char chHead[] = {0xEF, 0xBB, 0xBF};
	xmlFile.Write(chHead, 3);

	std::wstring strHead = L"<?xml version=\"1.0\" encoding=\"utf-8\" ?>\r\n<TEMPNOTICE>";
	std::wstring strEnd = L"</TEMPNOTICE>";

	std::string strTemp = common::utility::stringhelper::UnicodeToUTF8(strHead);
	xmlFile.Write(strTemp.c_str(), strTemp.length());

	for (VecNoticeInfo::iterator it = noticelist.begin(); it != noticelist.end(); ++it)
	{
		NoticeInfo * pNoticInfo = *it;
		CString strNotice;
		strNotice.Format(L"\r\n<Item><Content>%s</Content><FontStyle>%d</FontStyle><FontSize>%d</FontSize><FontType>%s</FontType><FontColor>%d</FontColor></Item>", pNoticInfo->strContent.c_str(), pNoticInfo->stFont.font_style, pNoticInfo->stFont.font_size, pNoticInfo->stFont.font_type.c_str(), pNoticInfo->stFont.font_color);
		strTemp = common::utility::stringhelper::UnicodeToUTF8(strNotice.GetBuffer(0));
		xmlFile.Write(strTemp.c_str(), strTemp.length());
	}

	strTemp = common::utility::stringhelper::UnicodeToUTF8(strEnd);
	xmlFile.Write(strTemp.c_str(), strTemp.length());

	xmlFile.Close();

	return true;
}

bool CTempNoticeManager::LoadXML( std::wstring strFullPath, VecNoticeInfo & noticelist )
{
	common::utility::expat_document xmlDoc;
	if (!xmlDoc.load_from_file(strFullPath.c_str()))
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
		//NoticeInfo stNoticeInfo;
		NoticeInfo * pNoticeInfo = new NoticeInfo();
		pNoticeInfo->strContent = pElement->get_element_by_name(L"Content")->get_element_contents();
		pNoticeInfo->stFont.font_type = pElement->get_element_by_name(L"FontType")->get_element_contents();
		pNoticeInfo->stFont.font_color = common::utility::stringhelper::StringToInt(pElement->get_element_by_name(L"FontColor")->get_element_contents());
		pNoticeInfo->stFont.font_style = common::utility::stringhelper::StringToInt(pElement->get_element_by_name(L"FontStyle")->get_element_contents());
		pNoticeInfo->stFont.font_size = common::utility::stringhelper::StringToInt(pElement->get_element_by_name(L"FontSize")->get_element_contents());

		noticelist.push_back(pNoticeInfo);

		pElement = pElement->get_sibling_element();
	}
	return true;
}