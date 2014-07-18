#include "StdAfx.h"
#include "RoomOption.h"
#include "coredefine.h"
#include "..\Room.h"
#include "utility/StringHelper.h"
#include "utility/SystemHelper.h"
#include "utility/expat++/expat_document.h"

#define NOTICE_DELIM L"^*(nd)*^"

RoomOption::RoomOption(uint32 unRoomID)
:	m_uRoomID(unRoomID)
{
	common::utility::systemhelper::GetDefaultFontName(m_strDefaulFontName);

	m_stWelcomeFont.font_type = m_strDefaulFontName;
	m_stWelcomeFont.font_size = 12;
	m_stWelcomeFont.font_style = 0;
	m_stWelcomeFont.font_color = RGB(0,0,0);
	m_unOptionCode = 0;
	m_unPreSetFreeShowTime = 0;
	m_unPreSetOptionCode = 0;
	m_bPublicChatChange = false;
	m_bColorTextChange = false;
	m_bFreeShowChange = false;
	m_bSignatureChange = false;
	m_bNameChange = false;
	m_bInited = false;
	m_unMicFirstStatus = m_unMicSecondStatus = m_unMicThirdStatus = 0 ;	
}

RoomOption::~RoomOption(void)
{
}

void RoomOption::LoadRoomOption( std::map<core::ENM_RoomInfoType, std::wstring> & mapKey2String, std::map<core::ENM_RoomInfoType, uint8> & mapKey2Uint8, std::map<core::ENM_RoomInfoType, uint32> & mapKey2Uint32 )
{
	std::wstring strTempSignature = m_strSignature;
	std::wstring strTempName = m_strName;
	for (std::map<core::ENM_RoomInfoType, std::wstring>::iterator it = mapKey2String.begin(); it != mapKey2String.end(); ++it)
	{
		switch (it->first)
		{
		case core::ROOMINFOTYPE_NAME:
			{
				m_strName = it->second;
			}
			break;
		case core::ROOMINFOTYPE_SIGNATURE:
			{
				m_strSignature = it->second;
			}
			break;
		case core::ROOMINFOTYPE_PASSWORD:
			{
				m_strPassword = it->second;
			}
			break;
		case core::ROOMINFOTYPE_WELCOME_WORDS:
			{
				m_strWelcomeWords = it->second;
			}
			break;
		case core::ROOMINFOTYPE_NOTICE:
			{
				DecodeRoomNotice(it->second);
			}
			break;
		default:
			break;
		}
	}

	if (strTempName != m_strName)
	{
		//m_strName = strTempName;
		m_bNameChange = true;
	}
	else
	{
		m_bNameChange = false;
	}

	if (strTempSignature != m_strSignature)
	{
		//m_strSignature = strTempSignature;
		m_bSignatureChange = true;
	}
	else
	{
		m_bSignatureChange = false;
	}

	if (m_strWelcomeWords == L"")
	{
		m_strWelcomeWords = DEFAULT_WELCOME_WORDS;
	}
	else
	{
		if (m_strWelcomeWords != DEFAULT_WELCOME_WORDS)
		{
			DecodeWelcomeWords(m_strWelcomeWords, m_stWelcomeFont);
		}
	}

	for (std::map<core::ENM_RoomInfoType, uint32>::iterator it = mapKey2Uint32.begin(); it != mapKey2Uint32.end(); ++it)
	{
		switch (it->first)
		{
		case core::ROOMINFOTYPE_OPTION:
			{
				m_unOptionCode = it->second;
				DecodeRoomOption(it->second);
			}
			break;
		default:
			break;
		}
	}

	if (mapKey2Uint32.size() == 0)
	{
		m_bPublicChatChange = false;
		m_bColorTextChange = false;
		m_bFreeShowChange = false;
	}

	for (std::map<core::ENM_RoomInfoType, uint8>::iterator it = mapKey2Uint8.begin(); it != mapKey2Uint8.end(); ++it)
	{
		switch (it->first)
		{
		case core::ROOMINFOTYPE_FREESHOW_TIME:
			{
				m_unFreeShowTime = it->second;
			}
			break;
		case core::ROOMINFOTYPE_MIC_FIRST_STATUS:
			{
				m_unMicFirstStatus = it->second ;
			}
			break;
		case core::ROOMINFOTYPE_MIC_SECOND_STATUS:
			{
				m_unMicSecondStatus = it->second ;
			}
			break;
		case core::ROOMINFOTYPE_MIC_THIRD_STATUS:
			{
				m_unMicThirdStatus = it->second ;
			}
			break;
		default:
			break;
		}
	}

	if (!m_bInited)
	{
		m_strPreSetName = m_strName;
		m_strPreSetSignature = m_strSignature;
		m_strPreSetPassword = m_strPassword;
		m_unPreSetFreeShowTime = m_unFreeShowTime;
		m_unPreSetOptionCode = m_unOptionCode;
		m_bInited = true;
	}
}

void RoomOption::OnConfirmRoomSetting( std::wstring strName, std::wstring strSignature, bool bOpen, std::wstring strPassword, ENM_RoomPublicChat enmPublicType, bool bColorText, bool bUserInOut)
{
	std::map<core::ENM_RoomInfoType, std::wstring> mapKey2String;
	std::map<core::ENM_RoomInfoType, uint32> mapKey2Uint32;

	if (m_strName != strName)
	{
		m_strPreSetName = strName;
		mapKey2String[core::ROOMINFOTYPE_NAME] = strName;
	}

	if (m_strSignature != strSignature)
	{
		m_strPreSetSignature = strSignature;
		mapKey2String[core::ROOMINFOTYPE_SIGNATURE] = strSignature;
	}

	if (m_strPassword != strPassword)
	{
		m_strPreSetPassword = strPassword;
		mapKey2String[core::ROOMINFOTYPE_PASSWORD] = strPassword;
	}

	RoomOptionCode stRoomOption;
	SetDefaultOption(stRoomOption);
	stRoomOption.bOpen = bOpen;
	stRoomOption.enmPublicChat = enmPublicType;
	stRoomOption.bColorText = bColorText;
	stRoomOption.bUserInOut = bUserInOut;
	uint32 unTemp = EncodeRoomOption(stRoomOption);
	if (unTemp != m_unOptionCode)
	{
		mapKey2Uint32[core::ROOMINFOTYPE_OPTION] = unTemp;
	}

	if (mapKey2String.size() != 0 || mapKey2Uint32.size() != 0)
	{
		core::SetRoomInfoEvent * pEvent = new core::SetRoomInfoEvent();
		pEvent->unRoomID = m_uRoomID;
		pEvent->map_key2string = mapKey2String;
		pEvent->map_key2uin32 = mapKey2Uint32;

		g_RoomModule->m_pModuleManager->PushEvent(MakeEvent<MODULE_ID_ROOM>()(core::EVENT_VALUE_CORE_SET_ROOMINFO, MODULE_ID_CORE, 0, 0, 0, pEvent));
	}
}

void RoomOption::OnConfirmMicSetting( uint8 unFreeShowTime, bool bFreeShowStatus, bool bFreeShowForManager, bool bFreeShowEditTime, bool bPrivateShowStatus )
{
	std::map<core::ENM_RoomInfoType, uint32> mapKey2Uint32;
	std::map<core::ENM_RoomInfoType, uint8> mapKey2Uint8;

	RoomOptionCode stRoomOption;
	SetDefaultOption(stRoomOption);
	stRoomOption.bFreeShowStatus = bFreeShowStatus;
	stRoomOption.bFreeShowForManager = bFreeShowForManager;
	stRoomOption.bFreeShowEditTime = bFreeShowEditTime;
	stRoomOption.bPrivateShowStatus = bPrivateShowStatus;
	uint32 unTemp = EncodeRoomOption(stRoomOption);
	if (unTemp != m_unOptionCode)
	{
		mapKey2Uint32[core::ROOMINFOTYPE_OPTION] = unTemp;
	}

	if (unFreeShowTime != m_unFreeShowTime)
	{
		m_unPreSetFreeShowTime = unFreeShowTime;
		mapKey2Uint8[core::ROOMINFOTYPE_FREESHOW_TIME] = unFreeShowTime;
	}

	if (mapKey2Uint32.size() != 0 || mapKey2Uint8.size() != 0)
	{
		core::SetRoomInfoEvent * pEvent = new core::SetRoomInfoEvent();
		pEvent->unRoomID = m_uRoomID;
		pEvent->map_key2uin32 = mapKey2Uint32;
		pEvent->map_key2uin8 = mapKey2Uint8;

		g_RoomModule->m_pModuleManager->PushEvent(MakeEvent<MODULE_ID_ROOM>()(core::EVENT_VALUE_CORE_SET_ROOMINFO, MODULE_ID_CORE, 0, 0, 0, pEvent));
	}
}

void RoomOption::OnConfirmUserManager( /*bool bAutoWelcome, */std::wstring strWelcomeWords, core::MessageFont stFont, bool bDefaultWelcome )
{
	std::map<core::ENM_RoomInfoType, std::wstring> mapKey2String;
	std::map<core::ENM_RoomInfoType, uint32> mapKey2Uint32;

	//uint32 unTemp = EncodeRoomOption();
	//if (unTemp != m_unOptionCode)
	//{
	//	mapKey2Uint32[core::ROOMINFOTYPE_OPTION] = unTemp;
	//}

	if (m_strWelcomeWords != strWelcomeWords || !CompareFont(m_stWelcomeFont, stFont))
	{
		m_strWelcomeWords = strWelcomeWords;
		m_stWelcomeFont = stFont;
		//common::utility::systemhelper::FilterWords(strWelcomeWords);
		if (!bDefaultWelcome)
		{
			EncodeWelcomeWords(strWelcomeWords, stFont);
		}
		mapKey2String[core::ROOMINFOTYPE_WELCOME_WORDS] = strWelcomeWords;
	}

	if (mapKey2String.size() != 0 || mapKey2Uint32.size() != 0)
	{
		core::SetRoomInfoEvent * pEvent = new core::SetRoomInfoEvent();
		pEvent->unRoomID = m_uRoomID;
		pEvent->map_key2string = mapKey2String;
		pEvent->map_key2uin32 = mapKey2Uint32;

		g_RoomModule->m_pModuleManager->PushEvent(MakeEvent<MODULE_ID_ROOM>()(core::EVENT_VALUE_CORE_SET_ROOMINFO, MODULE_ID_CORE, 0, 0, 0, pEvent));
	}
}

void RoomOption::EncodeRoomNotice( std::wstring & strNotice )
{
	for (std::vector<std::wstring>::iterator it = m_vecRoomNocice.begin(); it != m_vecRoomNocice.end(); ++it)
	{
		strNotice += (*it + NOTICE_DELIM);
	}
}

void RoomOption::DecodeRoomNotice( std::wstring strNotice )
{
	m_vecRoomNocice.clear();
	CString strTemp = strNotice.c_str();
	while (strTemp.Find(NOTICE_DELIM) > 0)
	{
		int nPos = strTemp.Find(NOTICE_DELIM);
		CString strSub = strTemp.Left(nPos);
		m_vecRoomNocice.push_back(strSub.GetBuffer(0));
		strTemp = strTemp.Right(strTemp.GetLength() - nPos - 8);
	}
}

void RoomOption::OnSaveRoomNotice( std::vector<std::wstring> & vecNoticeList )
{
	m_vecRoomNocice.clear();
	m_vecRoomNocice = vecNoticeList;
	std::wstring strBuf;
	EncodeRoomNotice(strBuf);
	//common::utility::systemhelper::FilterWords(strBuf);

	core::SetRoomInfoEvent *pEvent = new core::SetRoomInfoEvent();
	pEvent->unRoomID = m_uRoomID;
	pEvent->map_key2string[core::ROOMINFOTYPE_NOTICE] = strBuf;

	g_RoomModule->m_pModuleManager->PushEvent(MakeEvent<MODULE_ID_ROOM>()(core::EVENT_VALUE_CORE_SET_ROOMINFO, MODULE_ID_CORE, 0, 0, 0, pEvent));
}

void RoomOption::SetBaseRoomInfo( BaseRoomInfo & stRoomInfo )
{
	m_strName = stRoomInfo.strRoomName;
	m_strSignature = stRoomInfo.strRoomTheme;
	m_strOwnerName = stRoomInfo.strOwnerName;
	m_unOwner179ID = stRoomInfo.unOwner179ID;
	m_bOwnerMale = stRoomInfo.bOwnerMan;

	//std::wstring strNotice = stRoomInfo.strRoomNotice;
	//DecodeRoomNotice(strNotice);

	//std::wstring strWelcome = stRoomInfo.strRoomWelcome;
	//if (strWelcome != DEFAULT_WELCOME_WORDS)
	//{
	//	DecodeWelcomeWords(strWelcome, m_stWelcomeFont);
	//}
}

void RoomOption::SetPublicMicStatus( uint16 nIndex , uint8 unStatus ) 
{
	if ( nIndex == 1 )
	{
		m_unMicFirstStatus = unStatus ;
	}
	else if ( nIndex == 2 )
	{
		m_unMicSecondStatus = unStatus ;
	}
	else if ( nIndex == 3 )
	{
		m_unMicThirdStatus = unStatus ;
	}
}

uint8 RoomOption::GetPublicMicStatus( uint16 nIndex ) // 0 open
{
	if ( nIndex == 1 )
	{
		return m_unMicFirstStatus ;
	}
	else if ( nIndex == 2 )
	{
		return m_unMicSecondStatus ;
	}
	else if ( nIndex == 3 )
	{
		return m_unMicThirdStatus ;
	}
	else
	{
		return 0 ;
	}
}

bool RoomOption::EncodeWelcomeWords( std::wstring & strWelcomeWords, core::MessageFont stFont )
{
	strWelcomeWords = common::utility::stringhelper::XMLEncode(strWelcomeWords);

	CString strContent;
	strContent.Format(L"<?xml version=\"1.0\" encoding=\"utf-8\" ?><WELCOMEWORDS><Content FontStyle=\"%d\" FontSize=\"%d\" FontType=\"%s\" FontColor=\"%d\" >%s</Content></WELCOMEWORDS>", 
		stFont.font_style, stFont.font_size, stFont.font_type.c_str(), stFont.font_color, strWelcomeWords.c_str());

	strWelcomeWords = strContent.GetBuffer(0);

	return true;
}

bool RoomOption::DecodeWelcomeWords( std::wstring & strWelcomeWords, core::MessageFont & stFont )
{
	common::utility::expat_document xmlDoc;

	std::string strUtf8 = common::utility::stringhelper::UnicodeToUTF8(strWelcomeWords);
	if (!xmlDoc.load_from_buffer(strUtf8.c_str(), strUtf8.size()))
	{
		return false;
	}

	common::utility::expat_element * pRoot = xmlDoc.get_root();
	if (!pRoot)
	{
		return false;
	}

	common::utility::expat_element * pElement = pRoot->get_first_child_element();
	if (!pElement)
	{
		return false;
	}

	strWelcomeWords = pElement->get_element_contents();
	stFont.font_type = pElement->get_attribute_by_name(L"FontType")->get_attribute_value();
	stFont.font_color = common::utility::stringhelper::StringToInt(pElement->get_attribute_by_name(L"FontColor")->get_attribute_value());
	stFont.font_size = common::utility::stringhelper::StringToInt(pElement->get_attribute_by_name(L"FontSize")->get_attribute_value());
	stFont.font_style = common::utility::stringhelper::StringToInt(pElement->get_attribute_by_name(L"FontStyle")->get_attribute_value());

	return true;
}

void RoomOption::DecodeRoomOption( uint32 unKey )
{
	m_bOpen = ((unKey & core::ROOMOPTIONTYPE_STATUS) == 0);
	m_bUserInOut = ((unKey & core::ROOMOPTIONTYPE_USER_INOUT) != 0);
	m_bFreeShowForManager = ((unKey & core::ROOMOPTIONTYPE_FREESHOW_FOR_MANAGER) == 0);
	m_bFreeShowEditTime = ((unKey & core::ROOMOPTIONTYPE_FREESHOW_EDIT_TIME) == 0);
	m_bPrivateShowEnable = ((unKey & core::ROOMOPTIONTYPE_PRIVATESHOW_ENABLE) != 0);
	m_bPrivateShowStatus = ((unKey & core::ROOMOPTIONTYPE_PRIVATESHOW_STATUS) == 0);
	m_bAutoWelcome = ((unKey & core::ROOMOPTIONTYPE_AUTO_WELCOME) == 0);

	if (m_bInited)
	{
		//bool bTemp = ((unKey & core::ROOMOPTIONTYPE_PUBLICCHAT) == 0);
		ENM_RoomPublicChat enmPublicType = ((unKey & core::ROOMOPTIONTYPE_PUBLICCHAT) == 0 ? PUBLICCHAT_ABOVE_REGISTER : PUBLICCHAT_ABOVE_VIP);
		if (enmPublicType != m_enmPublicChat)
		{
			m_bPublicChatChange = true;
			m_enmPublicChat = enmPublicType;
		}
		else
		{
			m_bPublicChatChange = false;
		}

		bool bTemp = ((unKey & core::ROOMOPTIONTYPE_COLORTEXT) == 0);
		if (bTemp != m_bColorText)
		{
			m_bColorTextChange = true;
			m_bColorText = bTemp;
		}
		else
		{
			m_bColorTextChange = false;
		}

		bTemp = ((unKey & core::ROOMOPTIONTYPE_FREESHOW_STATUS) != 0);
		if (bTemp != m_bFreeShowStatus)
		{
			m_bFreeShowChange = true;
			m_bFreeShowStatus = bTemp;
		}
		else
		{
			m_bFreeShowChange = false;
		}
	}
	else
	{
		//m_bPublicChat = ((unKey & core::ROOMOPTIONTYPE_PUBLICCHAT) == 0);
		m_enmPublicChat = ((unKey & core::ROOMOPTIONTYPE_PUBLICCHAT) == 0 ? PUBLICCHAT_ABOVE_REGISTER : PUBLICCHAT_ABOVE_VIP);
		m_bColorText = ((unKey & core::ROOMOPTIONTYPE_COLORTEXT) == 0);
		m_bFreeShowStatus = ((unKey & core::ROOMOPTIONTYPE_FREESHOW_STATUS) != 0);
	}
}

uint32 RoomOption::EncodeRoomOption(RoomOptionCode stRoomOption)
{
	uint32 unTemp = 0x0000;
	if (!stRoomOption.bOpen)
	{
		unTemp |= core::ROOMOPTIONTYPE_STATUS;
	}
	if (stRoomOption.enmPublicChat == PUBLICCHAT_ABOVE_VIP)
	{
		unTemp |= core::ROOMOPTIONTYPE_PUBLICCHAT;
	}
	if (!stRoomOption.bColorText)
	{
		unTemp |= core::ROOMOPTIONTYPE_COLORTEXT;
	}
	if (stRoomOption.bUserInOut)
	{
		unTemp |= core::ROOMOPTIONTYPE_USER_INOUT;
	}
	if (stRoomOption.bFreeShowStatus)
	{
		unTemp |= core::ROOMOPTIONTYPE_FREESHOW_STATUS;
	}
	if (!stRoomOption.bFreeShowForManager)
	{
		unTemp |= core::ROOMOPTIONTYPE_FREESHOW_FOR_MANAGER;
	}
	if (!stRoomOption.bFreeShowEditTime)
	{
		unTemp |= core::ROOMOPTIONTYPE_FREESHOW_EDIT_TIME;
	}
	if (stRoomOption.bPrivateShowEnable)
	{
		unTemp |= core::ROOMOPTIONTYPE_PRIVATESHOW_ENABLE;
	}
	if (!stRoomOption.bPrivateShowStatus)
	{
		unTemp |= core::ROOMOPTIONTYPE_PRIVATESHOW_STATUS;
	}
	if (!stRoomOption.bAutoWelcome)
	{
		unTemp |= core::ROOMOPTIONTYPE_AUTO_WELCOME;
	}

	m_unPreSetOptionCode = unTemp;
	return unTemp;
}

void RoomOption::OnSetInfoOK()
{
	if (m_strName != m_strPreSetName)
	{
		m_strName = m_strPreSetName;
		m_bNameChange = true;
	}
	else
	{
		 m_bNameChange = false;
	}

	if (m_strSignature != m_strPreSetSignature)
	{
		m_strSignature = m_strPreSetSignature;
		m_bSignatureChange = true;
	}
	else
	{
		m_bSignatureChange = false;
	}
	m_strPassword = m_strPreSetPassword;
	m_unFreeShowTime = m_unPreSetFreeShowTime;
	m_unOptionCode = m_unPreSetOptionCode;
	DecodeRoomOption(m_unOptionCode);
}

bool RoomOption::CompareFont( core::MessageFont stFont1, core::MessageFont stFont2 )
{
	if (stFont1.font_color != stFont2.font_color || stFont1.font_size != stFont2.font_size || stFont1.font_style != stFont2.font_style || stFont1.font_type != stFont2.font_type)
	{
		return false;
	}
	
	return true;
}

void RoomOption::SetDefaultOption( RoomOptionCode & stRoomOption )
{
	stRoomOption.bOpen = m_bOpen;
	stRoomOption.enmPublicChat = m_enmPublicChat;
	stRoomOption.bColorText = m_bColorText;
	stRoomOption.bUserInOut = m_bUserInOut;
	stRoomOption.bFreeShowStatus = m_bFreeShowStatus;
	stRoomOption.bFreeShowForManager = m_bFreeShowForManager;
	stRoomOption.bFreeShowEditTime = m_bFreeShowEditTime;
	stRoomOption.bPrivateShowEnable = m_bPrivateShowEnable;
	stRoomOption.bPrivateShowStatus = m_bPrivateShowStatus;
	stRoomOption.bAutoWelcome = m_bAutoWelcome;
}