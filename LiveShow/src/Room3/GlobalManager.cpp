#include "stdafx.h"
#include "GlobalManager.h"
#include "..\..\include\utility\FileHelper.h"
#include "..\..\include\utility\SystemHelper.h"
#include "..\..\CommonLibrary\include\utility\StringHelper.h"
#include "..\..\include\utility\expat++\expat_document.h"
#include "utility\URLEncode.h"
#include "GlobalRoomDefine.h"
#include "RoomModule.h"
#include <algorithm>

EmotionItem::EmotionItem()
{

}

EmotionItem::~EmotionItem()
{

}


wstring EmotionItem::GetBigEmotionPath()
{
	std::wstring strModulePath = common::utility::systemhelper::Get179AppPath();
	common::CURLEncode urlEncode;
	strModulePath = urlEncode.URLEncodeLocalPath(strModulePath);
	return strModulePath + EMOTION_DIR + m_strBigFile ;
}

wstring EmotionItem::GetCode() 
{
	CString strCode;
	strCode.Format(L"/%s/", m_strEmotionName.c_str());
	return strCode.GetBuffer();
}

EmotionManager* EmotionManager::m_pEmotionManager = NULL ;
EmotionManager::EmotionManager()
{
}

EmotionManager::~EmotionManager()
{
}

EmotionManager* EmotionManager::GetInstance() 
{
	if ( m_pEmotionManager == NULL )
	{
		m_pEmotionManager = new EmotionManager() ;
	}
	return m_pEmotionManager ;
}

EmotionItem* EmotionManager::SearchEmotionByCode( wstring strCode ) 
{
	EmotionItem* pEmotionItem = NULL ;
	vector<EmotionItem*>::iterator iter ;
	for ( iter = m_vEmotionList.begin() ; iter != m_vEmotionList.end() ; ++iter )
	{
		EmotionItem* pItem = *iter ;
		if ( pItem->GetCode() == strCode )
		{
			pEmotionItem = pItem ;
			break;
		}
	}
	return pEmotionItem ;
}

EmotionItem* EmotionManager::SearchEmotionByBigFilePath( wstring strFileName ) 
{
	EmotionItem* pEmotionItem = NULL ;
	vector<EmotionItem*>::iterator iter ;
	for( iter = m_vEmotionList.begin() ; iter != m_vEmotionList.end() ; ++iter )
	{
		EmotionItem* pItem = *iter ;
		if ( strFileName.find( pItem->m_strBigFile ) != wstring::npos )
		{
			pEmotionItem = pItem ;
			break;
		}
	}
	return pEmotionItem ;
}

bool EmotionManager::LoadEmotionList() 
{
	common::utility::expat_document xmlDoc;

	std::wstring strModulePath = common::utility::systemhelper::Get179AppPath();
	common::CURLEncode urlEncode;
	strModulePath = urlEncode.URLEncodeLocalPath(strModulePath);
	std::wstring strXmlPath = strModulePath + EMOTION_CONFIG_PATH;

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
	int tabIndex=0;
	common::utility::expat_element * pLastTabElement = NULL;
	while( pElement )
	{
		if ( pElement->get_element_name() == L"emotion")
		{
			EmotionItem *pFaceItem = new EmotionItem ;
			pFaceItem->m_nType = tabIndex-1;
			pFaceItem->m_strBigFile = pElement->get_attribute_value( L"path" ) ;
			pFaceItem->m_strEmotionName = pElement->get_attribute_value( L"name" );
			m_vEmotionList.push_back( pFaceItem ) ;
			pElement = pElement->get_sibling_element();
			if(pElement == NULL)
			{
				pElement = pLastTabElement->get_sibling_element();
			}
		}
		else if ( pElement->get_element_name() == L"tab")
		{
			wstring strTypeName = pElement->get_attribute_value( L"name" );
			m_mTypeList.insert( make_pair( tabIndex++ , strTypeName ) ) ;
			pLastTabElement = pElement;
			pElement = pElement->get_next_element(true);
		}
		else
		{
			break;
		}
	}
	return true ;
}

void EmotionManager::Release()
{
	vector<EmotionItem*>::iterator iter ;
	for ( iter = m_vEmotionList.begin() ; iter != m_vEmotionList.end() ; ++iter )
	{
		EmotionItem *pEmotionItem = *iter ;
		delete pEmotionItem ;
	}
	m_vEmotionList.clear() ;
	if ( m_pEmotionManager != NULL )
	{
		delete m_pEmotionManager ;
	}
}

vector<EmotionItem*> EmotionManager::GetEmotionListByType( int nType ) 
{
	vector<EmotionItem*> vEmotionList ;
	vector<EmotionItem*>::iterator iter ;
	for ( iter = m_vEmotionList.begin() ; iter != m_vEmotionList.end() ; ++iter )
	{
		EmotionItem* pEmotionItem = *iter ;
		if ( pEmotionItem->m_nType == nType )
		{
			vEmotionList.push_back( pEmotionItem ) ;
		}
	}
	return vEmotionList ;
}

ShortCutMsgManager* ShortCutMsgManager::m_pShortCutMsgManager = NULL ;
ShortCutMsgManager::ShortCutMsgManager()
{
}

ShortCutMsgManager::~ShortCutMsgManager()
{
}


ShortCutMsgManager* ShortCutMsgManager::GetInstance() 
{
	if ( m_pShortCutMsgManager == NULL )
	{
		m_pShortCutMsgManager = new ShortCutMsgManager ;
	}
	return m_pShortCutMsgManager ;
}

bool ShortCutMsgManager::LoadShortCutMsgList() 
{
	common::utility::expat_document xmlDoc;

	std::wstring strModulePath = common::utility::systemhelper::Get179AppPath();
	common::CURLEncode urlEncode;
	strModulePath = urlEncode.URLEncodeLocalPath(strModulePath);
	std::wstring strXmlPath = strModulePath + SHORTCUTMSG_CONFIG_PATH;

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
	while( pElement )
	{
		if ( pElement->get_element_name() == L"ShortCutMsg")
		{
			wstring strShortCutMsg = pElement->get_attribute_value( L"Msg" ) ;
			m_vShortCutList.push_back( strShortCutMsg ) ;
		}
		pElement = pElement->get_sibling_element();
	}
	return true ;
}

void ShortCutMsgManager::SaveShortCutMsgList() 
{
	std::wstring strModulePath = common::utility::systemhelper::Get179AppPath();
	common::CURLEncode urlEncode;
	strModulePath = urlEncode.URLEncodeLocalPath(strModulePath);
	std::wstring strXmlPath = strModulePath + SHORTCUTMSG_CONFIG_PATH;

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
	vector<wstring>::iterator iter ;
	for ( iter = m_vShortCutList.begin() ; iter != m_vShortCutList.end() ; ++iter )
	{
		CString strShortCut ;
		strShortCut.Format( L"\r\n<ShortCutMsg Msg=\"%s\" />" , iter->c_str() ) ;
		strTemp = common::utility::stringhelper::UnicodeToUTF8(strShortCut.GetBuffer(0));
		xmlFile.Write(strTemp.c_str(), strTemp.length());
	}

	strTemp = common::utility::stringhelper::UnicodeToUTF8(strEnd);
	xmlFile.Write(strTemp.c_str(), strTemp.length());

	xmlFile.Close();
}

wstring ShortCutMsgManager::GetShortCutMsgByIndex( int nIndex ) 
{
	if ( nIndex > m_vShortCutList.size() )
	{
		return L"" ;
	}
	else
	{
		return m_vShortCutList[nIndex] ;
	}
}

vector<wstring>& ShortCutMsgManager::GetShortCutMsgList() 
{
	return m_vShortCutList ;
}

void ShortCutMsgManager::UpdateShortCutMsgList(vector<wstring>& vShortCutList ) 
{
	m_vShortCutList = vShortCutList ;
	SaveShortCutMsgList() ;
}
void ShortCutMsgManager::Release() 
{
	SaveShortCutMsgList() ;
	if ( m_pShortCutMsgManager != NULL )
	{
		delete m_pShortCutMsgManager ;
		m_pShortCutMsgManager = NULL ;
	}
}

CaiZiItem::CaiZiItem() 
{

}

CaiZiItem::~CaiZiItem() 
{

}

int CaiZiItem::GetCaiZiIndex() 
{
	return m_nIndex ;
}

wstring CaiZiItem::GetCaiZiHtml() 
{
	return m_strHtml ;
}

wstring CaiZiItem::GetCaiZiName() 
{
	return m_strName ;
}

CaiZiManager* CaiZiManager::m_pCaiZiManager = NULL ;
CaiZiManager::CaiZiManager()
{

}

CaiZiManager::~CaiZiManager() 
{

}

CaiZiManager* CaiZiManager::GetInstance() 
{
	if ( m_pCaiZiManager == NULL )
	{
		m_pCaiZiManager = new CaiZiManager ;
	}
	return m_pCaiZiManager ;
}

bool CaiZiManager::LoadCaiZiList()
{
	common::utility::expat_document xmlDoc;
	std::wstring strModulePath = common::utility::systemhelper::Get179AppPath();
	common::CURLEncode urlEncode;
	strModulePath = urlEncode.URLEncodeLocalPath(strModulePath);
	std::wstring strXmlPath = strModulePath + CAIZI_CONFIG_PATH;

	if (!xmlDoc.load_from_file(strXmlPath.c_str()))
	{
		return false;
	} 

	common::utility::expat_element * pRoot = xmlDoc.get_root();
	if (!pRoot)
	{
		return false;
	}
	std::wstring strCaiZiPath = strModulePath + CAIZI_DIR ;
	common::utility::expat_element * pElement = pRoot->get_first_child_element();
	while( pElement )
	{
		if ( pElement->get_element_name() == L"CaiZi")
		{
			CaiZiItem *pCaiZiItem = new CaiZiItem ;
			pCaiZiItem->m_nIndex = _wtoi( pElement->get_child_element_contents_by_name( L"Index" ).c_str() );
			pCaiZiItem->m_strName = pElement->get_child_element_contents_by_name( L"Name") ;
			CString strTemp = pElement->get_child_element_contents_by_name( L"Html").c_str() ;
			strTemp.Replace( L"images/" , strCaiZiPath.c_str() ) ;
			pCaiZiItem->m_strHtml =  strTemp.GetBuffer();
			m_vCaiZiList.push_back( pCaiZiItem ) ;
		}
		pElement = pElement->get_sibling_element();
	}
	return true ;
}

void CaiZiManager::Release() 
{
	vector<CaiZiItem* >::iterator iter  ;
	for ( iter = m_vCaiZiList.begin() ; iter != m_vCaiZiList.end() ; ++iter )
	{
		CaiZiItem* pCaiZiItem = *iter;
		delete pCaiZiItem ;
	}
	m_vCaiZiList.clear() ;
	if ( m_pCaiZiManager != NULL )
	{
		delete m_pCaiZiManager ;
		m_pCaiZiManager = NULL ;
	}
}

CaiZiItem* CaiZiManager::GetCaiZiItemByIndex( int nIndex ) 
{
	CaiZiItem *pReturn = NULL ;
	vector<CaiZiItem* >::iterator iter  ;
	for ( iter = m_vCaiZiList.begin() ; iter != m_vCaiZiList.end() ; ++iter )
	{
		CaiZiItem *pCaiZiItem = *iter ;
		if ( pCaiZiItem->m_nIndex == nIndex )
		{
			pReturn = pCaiZiItem ;
			break;
		}
	}
	return  pReturn ;
}

vector<CaiZiItem*>& CaiZiManager::GetCaiZiList() 
{
	return m_vCaiZiList ;
}

EnterManager* EnterManager::m_pEnterManager = NULL ;
EnterManager::EnterManager()
{

}

EnterManager::~EnterManager() 
{

}

EnterManager* EnterManager::GetInstance() 
{
	if ( m_pEnterManager == NULL )
	{
		m_pEnterManager = new EnterManager ;
	}
	return m_pEnterManager ;
}

bool EnterManager::LoadEnterList()
{
	common::utility::expat_document xmlDoc;
	std::wstring strModulePath = common::utility::systemhelper::Get179AppPath();
	common::CURLEncode urlEncode;
	strModulePath = urlEncode.URLEncodeLocalPath(strModulePath);
	std::wstring strXmlPath = strModulePath + ENTER_CONFIG_PATH;

	if (!xmlDoc.load_from_file(strXmlPath.c_str()))
	{
		return false;
	} 

	common::utility::expat_element * pRoot = xmlDoc.get_root();
	if (!pRoot)
	{
		return false;
	}
	std::wstring strEnterPath = strModulePath + ENTER_DIR ;
	common::utility::expat_element * pElement = pRoot->get_first_child_element();
	while( pElement )
	{
		if ( pElement->get_element_name() == L"Enter")
		{
			int nIndex = _wtoi( pElement->get_child_element_contents_by_name( L"VipLevel" ).c_str() );
			CString strTemp = pElement->get_child_element_contents_by_name( L"Html").c_str() ;
			strTemp.Replace( L"images/" , strEnterPath.c_str() ) ;
			m_mEnterList.insert( make_pair( nIndex , strTemp ) ) ;
		}
		pElement = pElement->get_sibling_element();
	}
	return true ;
}

void EnterManager::Release() 
{
	if ( m_pEnterManager != NULL )
	{
		delete m_pEnterManager ;
		m_pEnterManager = NULL ;
	}
}
CString EnterManager::GetEnterHtmlByVipLevel( int nVipLevel )
{
	CString strHtml ;
	map< UINT32 , CString >::iterator iter ;
	iter = m_mEnterList.find( nVipLevel ) ;
	if ( iter != m_mEnterList.end() )
	{
		return iter->second ;
	}
	return L"" ;
}

WelcomeManager* WelcomeManager::m_pWelcomeManager = NULL ;
WelcomeManager::WelcomeManager() 
{

}

WelcomeManager::~WelcomeManager() 
{

}

WelcomeManager* WelcomeManager::GetInstance() 
{
	if ( m_pWelcomeManager == NULL )
	{
		m_pWelcomeManager = new WelcomeManager ;
	}
	return m_pWelcomeManager ;
}

bool WelcomeManager::LoadWelcomeList() 
{
	common::utility::expat_document xmlDoc;
	std::wstring strModulePath = common::utility::systemhelper::Get179AppPath();
	common::CURLEncode urlEncode;
	strModulePath = urlEncode.URLEncodeLocalPath(strModulePath);
	std::wstring strXmlPath = strModulePath + WELCOME_CONFIG_PATH;

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
	while( pElement )
	{
		if ( pElement->get_element_name() == L"Welcome")
		{
			m_vWelcomeList.push_back( pElement->get_element_contents().c_str() ) ;
		}else if (pElement->get_element_name() == L"ActorSpeak")
		{
			m_vActorSpeak.push_back( pElement->get_element_contents().c_str());
		}else if (pElement->get_element_name() == L"AgentSpeak")
		{
			m_vAgentSpeak.push_back( pElement->get_element_contents().c_str());
		}
		pElement = pElement->get_sibling_element();
	}
	return true ;
}

void WelcomeManager::Release() 
{
	if ( m_pWelcomeManager != NULL )
	{
		delete m_pWelcomeManager ;
		m_pWelcomeManager = NULL ;
	}
}

CString WelcomeManager::GetWelcomeRandom( ) 
{//随机不包括最后5条
	if ( m_vWelcomeList.size() > 5 )
	{
		int nIndex = rand()%(m_vWelcomeList.size() - 5) ;
		return m_vWelcomeList[nIndex] ;
	}	
	else
	{
		return L"" ;
	}
}

CString WelcomeManager::GetFirstLoginUserWelcomeRandom()
{//对于显示给第一次登录用户的欢迎词在最后5条，详看配置文件
	if ( m_vWelcomeList.size() >= 5 )
	{
		int nIndex = m_vWelcomeList.size() - 5 + rand()%5;
		return m_vWelcomeList[nIndex] ;
	}	
	else
	{
		return L"" ;
	}
}

CString WelcomeManager::GetActorSpeakRandom()
{
	if ( m_vActorSpeak.size() > 0 )
	{
		int nIndex = rand()%(m_vActorSpeak.size()) ;
		return m_vActorSpeak[nIndex] ;
	}	
	else
	{
		return L"" ;
	}
}

CString WelcomeManager::GetAgentSpeakRandom()
{
	if ( m_vAgentSpeak.size() > 0 )
	{
		int nIndex = rand()%(m_vAgentSpeak.size()) ;
		return m_vAgentSpeak[nIndex] ;
	}	
	else
	{
		return L"" ;
	}
}

VisitorManager* VisitorManager::m_pVisitorManager = NULL ;
VisitorManager::VisitorManager() 
{

}

VisitorManager::~VisitorManager() 
{

}

VisitorManager* VisitorManager::GetInstance() 
{
	if ( m_pVisitorManager == NULL )
	{
		m_pVisitorManager = new VisitorManager ;
	}
	return m_pVisitorManager ;
}

bool VisitorManager::LoadVisitorList() 
{
	common::utility::expat_document xmlDoc;
	std::wstring strModulePath = common::utility::systemhelper::Get179AppPath();
	common::CURLEncode urlEncode;
	strModulePath = urlEncode.URLEncodeLocalPath(strModulePath);
	std::wstring strXmlPath = strModulePath + VISITOR_CONFIG_PATH;

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
	while( pElement )
	{
		if ( pElement->get_element_name() == L"Visitor")
		{
			m_vVisitorList.push_back( pElement->get_element_contents().c_str() ) ;
		}
		pElement = pElement->get_sibling_element();
	}
	return true ;
}

void VisitorManager::Release() 
{
	if ( m_pVisitorManager != NULL )
	{
		delete m_pVisitorManager ;
		m_pVisitorManager = NULL ;
	}
}

CString VisitorManager::GetVisitorRandom() 
{
	if ( !m_vVisitorList.empty() )
	{
		int nIndex = rand()%m_vVisitorList.size() ;
		return m_vVisitorList[nIndex] ;
	}	
	else
	{
		return L"" ;
	}
}

WealthyManager* WealthyManager::m_pWealthyManager = NULL ;
WealthyManager::WealthyManager() 
{

}

WealthyManager::~WealthyManager() 
{
	m_mapListEffect.clear();
}

WealthyManager* WealthyManager::GetInstance() 
{
	if ( m_pWealthyManager == NULL )
	{
		m_pWealthyManager = new WealthyManager ;
	}
	return m_pWealthyManager ;
}

bool WealthyManager::LoadWealthyList() 
{
	common::utility::expat_document xmlDoc;

	std::wstring strModulePath = common::utility::systemhelper::Get179AppPath();
	common::CURLEncode urlEncode;
	strModulePath = urlEncode.URLEncodeLocalPath(strModulePath);
	std::wstring strXmlPath = strModulePath + WEALTHY_CONFIG_PATH;

	if (!xmlDoc.load_from_file(strXmlPath.c_str()))
	{
		return false;
	} 

	common::utility::expat_element * pRoot = xmlDoc.get_root();
	if (!pRoot)
	{
		return false;
	}

	std::wstring strConfigPath = strModulePath + WEALTHY_CONFIG_DIR;
	common::utility::expat_element * pElement = pRoot->get_first_child_element();
	while( pElement )
	{
		list_effect_info info;
		uint8 unIndex = _wtoi( pElement->get_attribute_value( L"Index" ).c_str() );
		info.unIndex = unIndex;

		std::wstring strTemp = pElement->get_child_element_attribute_value_by_name(L"Image", L"path");
		info.strImagePath = strConfigPath + strTemp;

		std::wstring strTime = pElement->get_child_element_attribute_value_by_name(L"Time", L"value") ;
		int nYear ,nMonth ,nDay ,nHour ,nMinute , nSec ;
		nYear = nMonth = nDay = nHour = nMinute = nSec = 0 ;
		swscanf( strTime.c_str() , L"%d:%d:%d:%d:%d:%d" ,&nYear , &nMonth ,&nDay , &nHour , &nMinute , &nSec ) ;
		info.tmInvalid = CTime( nYear ,nMonth , nDay ,nHour , nMinute , nSec ) ;

		common::utility::expat_element * pChildElement = pElement->get_element_by_name(L"User");
		if (pChildElement)
		{
			pChildElement = pChildElement->get_first_child_element();
			while (pChildElement)
			{
				uint32 un179ID = _wtoi( pChildElement->get_attribute_value( L"ID" ).c_str() );
				info.vec179ID.push_back(un179ID);
				pChildElement = pChildElement->get_sibling_element();
			}
		}
		m_mapListEffect[unIndex] = info;
		pElement = pElement->get_sibling_element();
	}
	return true ;
}

void WealthyManager::Release() 
{
	if ( m_pWealthyManager != NULL )
	{
		delete m_pWealthyManager ;
		m_pWealthyManager = NULL ;
	}
}

std::wstring WealthyManager::GetWealthyImagePath( uint8 unIndex )
{
	if (m_mapListEffect.find(unIndex) != m_mapListEffect.end())
	{
		return m_mapListEffect[unIndex].strImagePath;
	}

	return L"";
}

uint8 WealthyManager::GetEffectIndex( uint32 un179ID )
{
	uint32 unTime = GetCurInfo().unSystemTime;
	CTime currenttime = unTime;
	for (std::map<uint8, list_effect_info>::iterator it = m_mapListEffect.begin(); it != m_mapListEffect.end(); ++it)
	{
		list_effect_info info = it->second;
		if (std::find(info.vec179ID.begin(), info.vec179ID.end(), un179ID) != info.vec179ID.end())
		{
			CTimeSpan spantime = info.tmInvalid - currenttime ;
			if ( spantime.GetTotalSeconds() > 0 )
			{
				return info.unIndex;
			}
		}
	}

	return 0;
}

void WealthyManager::GetAllWealthyImg( std::map<uint8, std::wstring> & mapWealthyImgPath )
{
	for (std::map<uint8, list_effect_info>::iterator it = m_mapListEffect.begin(); it != m_mapListEffect.end(); ++it)
	{
		list_effect_info info = it->second;
		mapWealthyImgPath[it->first] = info.strImagePath;
	}
}

ImageManager* ImageManager::s_pImageManager = NULL;

ImageManager::ImageManager()
{
}

ImageManager::~ImageManager()
{
	for (std::map<int, common::ui::ImageEx*>::iterator it = m_IconList.begin(); it != m_IconList.end(); ++it)
	{
		delete it->second;
	}
	m_IconList.clear();

	for (std::map<int, common::ui::ImageEx*>::iterator it = m_StampList.begin(); it != m_StampList.end(); ++it)
	{
		delete it->second;
	}
	m_StampList.clear();

	for (std::map<uint8, common::ui::ImageEx*>::iterator it = m_RichImgList.begin(); it != m_RichImgList.end(); ++it)
	{
		delete it->second;
	}
	m_RichImgList.clear();

	for (std::map<uint8, common::ui::ImageEx*>::iterator it = m_WealthyImgList.begin(); it != m_WealthyImgList.end(); ++it)
	{
		delete it->second;
	}
	m_WealthyImgList.clear();
}

common::ui::ImageEx* ImageManager::GetIcon(int iconIndex)
{
	std::map<int, common::ui::ImageEx*>::iterator it = m_IconList.find(iconIndex);
	if (it != m_IconList.end())
	{
		return it->second;
	}
	return NULL;
}

void ImageManager::DrawAnimation()
{
	for (std::map<int, common::ui::ImageEx*>::iterator it = m_IconList.begin(); it != m_IconList.end(); ++it)
	{
		common::ui::ImageEx* pImage = it->second;
		if (pImage->IsAnimatedGIF())
		{
			pImage->DrawFrameGIF();
		}
	}

	for (std::map<uint8, common::ui::ImageEx*>::iterator it = m_RichImgList.begin(); it != m_RichImgList.end(); ++it)
	{
		common::ui::ImageEx* pImage = it->second;
		if (pImage->IsAnimatedGIF())
		{
			pImage->DrawFrameGIF();
		}
	}
}

void ImageManager::AddAllIcon( std::map<int, std::wstring>& mapIconPath )
{
	for (std::map<int, std::wstring>::iterator it = mapIconPath.begin(); it != mapIconPath.end(); ++it)
	{
		common::ui::ImageEx* p = new common::ui::ImageEx(it->second.c_str());
		m_IconList[it->first] = p;
	}
}

common::ui::ImageEx* ImageManager::GetStamp( int stampIndex )
{
	std::map<int, common::ui::ImageEx*>::iterator it = m_StampList.find(stampIndex);
	if (it != m_StampList.end())
	{
		return it->second;
	}
	return NULL;
}

void ImageManager::AddStamp( int stampIndex, std::wstring strPath )
{
	std::map<int, common::ui::ImageEx*>::iterator it = m_StampList.find(stampIndex);
	if (it != m_StampList.end())
	{
		return;
	}

	common::ui::ImageEx* p = new common::ui::ImageEx(strPath.c_str());
	m_StampList[stampIndex] = p;
}

void ImageManager::AddAllRichImg( std::map<uint8, std::wstring>& mapRichImgPath )
{
	for (std::map<uint8, std::wstring>::iterator it = mapRichImgPath.begin(); it != mapRichImgPath.end(); ++it)
	{
		common::ui::ImageEx* p = new common::ui::ImageEx(it->second.c_str());
		m_RichImgList[it->first] = p;
	}
}

common::ui::ImageEx* ImageManager::GetRichImg( uint8 unRichLevel )
{
	std::map<uint8, common::ui::ImageEx*>::iterator it = m_RichImgList.find(unRichLevel);
	if (it != m_RichImgList.end())
	{
		return it->second;
	}
	return NULL;
}

void ImageManager::AddAllWealthyImg( std::map<uint8, std::wstring>& mapWealthyImgPath )
{
	for (std::map<uint8, std::wstring>::iterator it = mapWealthyImgPath.begin(); it != mapWealthyImgPath.end(); ++it)
	{
		common::ui::ImageEx* p = new common::ui::ImageEx(it->second.c_str());
		m_WealthyImgList[it->first] = p;
	}
}

common::ui::ImageEx* ImageManager::GetWealthyImg( uint8 unWealthyIndex )
{
	std::map<uint8, common::ui::ImageEx*>::iterator it = m_WealthyImgList.find(unWealthyIndex);
	if (it != m_WealthyImgList.end())
	{
		return it->second;
	}
	return NULL;
}

EventEnterManager* EventEnterManager::m_pEventEnterManager = NULL ;
EventEnterManager::EventEnterManager()
{

}

EventEnterManager::~EventEnterManager() 
{
	m_mapEnterList.clear();
}

EventEnterManager* EventEnterManager::GetInstance() 
{
	if ( m_pEventEnterManager == NULL )
	{
		m_pEventEnterManager = new EventEnterManager ;
	}
	return m_pEventEnterManager ;
}

bool EventEnterManager::LoadEventEnterList()
{
	common::utility::expat_document xmlDoc;
	std::wstring strModulePath = common::utility::systemhelper::Get179AppPath();
	common::CURLEncode urlEncode;
	strModulePath = urlEncode.URLEncodeLocalPath(strModulePath);
	std::wstring strXmlPath = strModulePath + EVENTENTER_CONFIG_PATH;

	if (!xmlDoc.load_from_file(strXmlPath.c_str()))
	{
		return false;
	} 

	common::utility::expat_element * pRoot = xmlDoc.get_root();
	if (!pRoot)
	{
		return false;
	}
	std::wstring strEnterPath = strModulePath + ENTER_DIR ;
	common::utility::expat_element * pElement = pRoot->get_first_child_element();
	while( pElement )
	{
		if ( pElement->get_element_name() == L"Enter")
		{
			event_enter_info info;
			uint8 unIndex = _wtoi( pElement->get_attribute_value( L"Index" ).c_str() );
			info.unIndex = unIndex;

			CString strTemp = pElement->get_child_element_contents_by_name( L"Html").c_str() ;
			strTemp.Replace( L"images/" , strEnterPath.c_str() ) ;
			info.strEnter = strTemp;

			common::utility::expat_element * pChildElement = pElement->get_element_by_name(L"User");
			if (pChildElement)
			{
				pChildElement = pChildElement->get_first_child_element();
				while (pChildElement)
				{
					uint32 un179ID = _wtoi( pChildElement->get_attribute_value( L"ID" ).c_str() );
					info.vec179ID.push_back(un179ID);
					pChildElement = pChildElement->get_sibling_element();
				}
			}
			m_mapEnterList[unIndex] = info;
		}
		pElement = pElement->get_sibling_element();
	}
	return true;
}

void EventEnterManager::Release() 
{
	if ( m_pEventEnterManager != NULL )
	{
		delete m_pEventEnterManager ;
		m_pEventEnterManager = NULL ;
	}
}

uint8 EventEnterManager::GetEventIndex( uint32 un179ID )
{
	for (std::map<uint8, event_enter_info>::iterator it = m_mapEnterList.begin(); it != m_mapEnterList.end(); ++it)
	{
		event_enter_info info = it->second;
		if (std::find(info.vec179ID.begin(), info.vec179ID.end(), un179ID) != info.vec179ID.end())
		{
			return info.unIndex;
		}
	}

	return 0;
}

CString EventEnterManager::GetEnterHtmlByIndex( uint8 unIndex )
{
	if (m_mapEnterList.find(unIndex) != m_mapEnterList.end())
	{
		return m_mapEnterList[unIndex].strEnter;
	}

	return L"";
}