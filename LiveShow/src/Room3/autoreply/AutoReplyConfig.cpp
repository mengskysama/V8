#include "StdAfx.h"
#include "..\Room.h"
#include "AutoReplyConfig.h"
#include "..\..\CommonLibrary\include\utility\StringHelper.h"
#include "..\..\include\utility\expat++\expat_document.h"
#include "..\..\include\utility\SystemHelper.h"
#include "..\..\CommonLibrary/include\utility\FileHelper.h"
#include "utility/URLEncode.h"


AutoReplyConfig* AutoReplyConfig::m_pAutoReplyManager = NULL;
AutoReplyConfig::AutoReplyConfig()
{
	m_bAutoReply = false;
}

AutoReplyConfig::~AutoReplyConfig()
{
	m_vAutoReplyList.clear();
	m_mapReject.clear();
}

AutoReplyConfig* AutoReplyConfig::GetInstance()
{
	if (m_pAutoReplyManager == NULL)
	{
		m_pAutoReplyManager = new AutoReplyConfig();
	}
	return m_pAutoReplyManager;
}

bool AutoReplyConfig::LoadAutoReplyList()
{
	m_vAutoReplyList.clear();
	m_mapReject.clear();

	m_bAutoReply = false;
	common::utility::expat_document xmlDoc;

	std::wstring strModulePath = common::utility::systemhelper::Get179AppPath();
	common::CURLEncode urlEncode;
	strModulePath = urlEncode.URLEncodeLocalPath(strModulePath);
	CString strFilePath;
	strFilePath.Format(L"config\\%d\\autoreplycfg.xml",GetCurInfo().unUIN);
	std::wstring strXmlPath = strModulePath + strFilePath.GetBuffer(0);

	if (!xmlDoc.load_from_file(strXmlPath.c_str()))//不存在，则加载默认自动回复
	{
		LoadDefaultAutoReplyList();
		return true;
	} 

	common::utility::expat_element * pRoot = xmlDoc.get_root();
	if (!pRoot)
	{
		return false;
	}
	common::utility::expat_element * pElement = pRoot->get_first_child_element();
	while(pElement)
	{
		if (pElement->get_element_name() == L"SetAutoReply")
		{
			std::wstring strSetAutoReply = pElement->get_attribute_value(L"AutoReply");
			if (strSetAutoReply == L"true")
			{
				m_bAutoReply = true;
			}else
			{
				m_bAutoReply = false;
			}
		}

		if (pElement->get_element_name() == L"AutoReplyMsg")
		{
			std::wstring strAutoReplyMsg = pElement->get_attribute_value(L"Msg") ;
			std::wstring strChecked = pElement->get_attribute_value(L"Checked");
			stAutoReplyMenuItem stItem;
			stItem.strMenuItem = strAutoReplyMsg;
			if (strChecked == L"true")
			{
				stItem.bChecked = true;
			}else
			{
				stItem.bChecked = false;
			}
			m_vAutoReplyList.push_back(stItem);
		}
		pElement = pElement->get_sibling_element();
	}
	return true ;
}

bool AutoReplyConfig::LoadDefaultAutoReplyList()
{
	//m_vAutoReplyList.clear();
	stAutoReplyMenuItem stItem;
	stItem.strMenuItem = DEFAULT_AUTOREPLY_1;
	stItem.bChecked = false;
	m_vAutoReplyList.push_back(stItem);
	stItem.strMenuItem = DEFAULT_AUTOREPLY_2;
	stItem.bChecked = false;
	m_vAutoReplyList.push_back(stItem);
	stItem.strMenuItem = DEFAULT_AUTOREPLY_3;
	stItem.bChecked = false;
	m_vAutoReplyList.push_back(stItem);

	SaveAutoReplyList();
	return true;
}

void AutoReplyConfig::SaveAutoReplyList()
{
	if ( GetCurInfo().enmVIPLevel == core::VIPLEVEL_NONE )//游客
	{
		return;
	}

	std::wstring strModulePath = common::utility::systemhelper::Get179AppPath();
	common::CURLEncode urlEncode;
	strModulePath = urlEncode.URLEncodeLocalPath(strModulePath);
	CString strFilePath;
	strFilePath.Format(L"config\\%d",GetCurInfo().unUIN);
	//strFilePath.Format(L"config\\autoreplycfg.xml");
	std::wstring strXmlPath = strModulePath + strFilePath.GetBuffer(0);

	if (!PathFileExists(strXmlPath.c_str()))
	{
		common::utility::filehelper::CreateAllDirectory(strXmlPath);
	}
	strXmlPath += L"\\autoreplycfg.xml";
	CFile xmlFile;
	if (!xmlFile.Open(strXmlPath.c_str(), CFile::modeCreate|CFile::modeWrite))
	{
		return;
	}

	char chHead[] = {0xEF, 0xBB, 0xBF};
	xmlFile.Write(chHead, 3);

	std::wstring strHead = L"<?xml version=\"1.0\" encoding=\"utf-8\" ?>\r\n<config>";
	std::wstring strEnd = L"\r\n</config>";

	std::string strTemp = common::utility::stringhelper::UnicodeToUTF8(strHead);
	xmlFile.Write(strTemp.c_str(), strTemp.length());
	//先设置是否设置了自动回复
	CString boolAutoReply;
	if (m_bAutoReply)
	{
		boolAutoReply = L"true";
	}
	else
	{
		boolAutoReply = L"false";
	}
	CString strInsertAutoReply;
	strInsertAutoReply.Format( L"\r\n<SetAutoReply AutoReply=\"%s\" />", boolAutoReply) ;
	strTemp = common::utility::stringhelper::UnicodeToUTF8(strInsertAutoReply.GetBuffer(0));
	xmlFile.Write(strTemp.c_str(), strTemp.length());
	//再设置自动回复的内容
	/*if (m_vAutoReplyList.size() <= 0)
	{
		LoadDefaultAutoReplyList();
	}*/

	std::vector<stAutoReplyMenuItem>::iterator iter ;
	for ( iter = m_vAutoReplyList.begin() ; iter != m_vAutoReplyList.end() ; ++iter )
	{
		CString strAutoReply;
		CString strChecked;
		stAutoReplyMenuItem stItem = *iter;
		strAutoReply = stItem.strMenuItem.c_str();
		if (stItem.bChecked)
		{
			strChecked = L"true";
		}
		else
		{
			strChecked = L"false";
		}
		CString strInsert;
		strInsert.Format( L"\r\n<AutoReplyMsg Msg=\"%s\" Checked=\"%s\" />", strAutoReply, strChecked) ;
		strTemp = common::utility::stringhelper::UnicodeToUTF8(strInsert.GetBuffer(0));
		xmlFile.Write(strTemp.c_str(), strTemp.length());
	}

	strTemp = common::utility::stringhelper::UnicodeToUTF8(strEnd);
	xmlFile.Write(strTemp.c_str(), strTemp.length());

	xmlFile.Close();
}

bool AutoReplyConfig::GetAutoReplyByIndex(int nIndex, stAutoReplyMenuItem& stMenuItem)
{
	if ( nIndex > m_vAutoReplyList.size() )
	{
		return false;
	}
	else
	{
		stMenuItem = m_vAutoReplyList[nIndex];
		return  true;
	}
}

std::wstring AutoReplyConfig::GetCheckedAutoReply()
{
	std::vector<stAutoReplyMenuItem>::iterator iter;
	std::wstring strTemp = L"";
	for ( iter = m_vAutoReplyList.begin(); iter != m_vAutoReplyList.end(); ++iter)
	{
		stAutoReplyMenuItem stItem = *iter;
		if (stItem.bChecked)
		{
			strTemp = stItem.strMenuItem;
		}
	}

    if (strTemp == L"")//没找到，就默认用第一个。
    {
		strTemp = DEFAULT_AUTOREPLY_1;
		SetCheckedByIndex(0);//设置第一项选中
    }

	return strTemp;
}

void AutoReplyConfig::SetCheckedByIndex(int nIndex)
{
	if (nIndex > m_vAutoReplyList.size())
	{
		return;
	}
	else
	{
		m_bAutoReply = TRUE;
		for (int i = 0; i < m_vAutoReplyList.size(); ++i)
		{
			if (i == nIndex)
				m_vAutoReplyList[i].bChecked = true;
			else
				m_vAutoReplyList[i].bChecked = false;
		}
		SaveAutoReplyList();

	}
}

std::vector<stAutoReplyMenuItem>& AutoReplyConfig::GetAutoReplyList()
{
	return m_vAutoReplyList;
}

void AutoReplyConfig::SetAutoReply(bool bAutoReply)
{
	m_bAutoReply = bAutoReply;

	if (!m_bAutoReply)
	{
		for (int i = 0; i < m_vAutoReplyList.size(); ++i)
		{
			m_vAutoReplyList[i].bChecked = false;
		}
	}
	
	SaveAutoReplyList();
}

void AutoReplyConfig::UpdateAutoReplyList(std::vector<stAutoReplyMenuItem> &vAutoReplyList)
{
	m_vAutoReplyList = vAutoReplyList;
	SaveAutoReplyList();
}

void AutoReplyConfig::Release()
{
	//SaveAutoReplyList();
	if (NULL != m_pAutoReplyManager)
	{
		delete m_pAutoReplyManager;
		m_pAutoReplyManager = NULL;
	}
}

void AutoReplyConfig::SetRejectAutoMsg(uint32 unWho, bool bReject)
{
	std::map<uint32,bool>::iterator iter = m_mapReject.find(unWho);
	if (iter != m_mapReject.end())
	{
		iter->second = bReject;
	}
	else
	{
		m_mapReject.insert(std::pair<uint32,bool>(unWho, bReject));
	}

}

bool AutoReplyConfig::GetRejectAutoMsg(uint32 unWho)
{
	std::map<uint32,bool>::iterator iter = m_mapReject.find(unWho);
	if (iter != m_mapReject.end())
	{
		return iter->second;
	}
	else
	{
		return false;
	}
}