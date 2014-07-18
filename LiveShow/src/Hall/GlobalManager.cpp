#include "stdafx.h"
#include "GlobalManager.h"
#include "../../include/utility/FileHelper.h"
#include "../../include/utility/SystemHelper.h"
#include "../../CommonLibrary/include/utility/StringHelper.h"
#include "../../include/utility/expat++/expat_document.h"
#include "utility/URLEncode.h"



DeferPicPathManager* DeferPicPathManager::m_pDeferPicPathManager = NULL ;
DeferPicPathManager::DeferPicPathManager() 
{

}

DeferPicPathManager::~DeferPicPathManager() 
{

}

DeferPicPathManager* DeferPicPathManager::GetInstance() 
{
	if ( m_pDeferPicPathManager == NULL )
	{
		m_pDeferPicPathManager = new DeferPicPathManager ;
	}
	return m_pDeferPicPathManager ;
}

bool DeferPicPathManager::LoadDeferPicPathList() 
{
	common::utility::expat_document xmlDoc;
	std::wstring strModulePath = common::utility::systemhelper::Get179AppPath();
	common::CURLEncode urlEncode;
	strModulePath = urlEncode.URLEncodeLocalPath(strModulePath);
	std::wstring strXmlPath = strModulePath + DEFER_PIC_CONFIG_PATH;

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
		if ( pElement->get_element_name() == L"default" )
		{
			common::utility::expat_element * pChildElement = pElement->get_first_child_element();
			while( pChildElement )
			{
				std::wstring strDeferPic = pChildElement->get_attribute_value( L"file") ; ;
				std::wstring strtemp = strModulePath + DEFER_PIC_DIR + strDeferPic ;
				m_vDeferPicPathList.push_back( strtemp.c_str() ) ;
				pChildElement = pChildElement->get_sibling_element() ;
			}
		}
		pElement = pElement->get_sibling_element();
	}
	return true ;
}

void DeferPicPathManager::Release() 
{
	if ( m_pDeferPicPathManager != NULL )
	{
		delete m_pDeferPicPathManager ;
		m_pDeferPicPathManager = NULL ;
	}
}

CString DeferPicPathManager::GetDefaultPicPathRandom( ) 
{
	if ( !m_vDeferPicPathList.empty() )
	{
		srand( (unsigned)time( NULL ) );
		int nIndex = rand()%m_vDeferPicPathList.size() ;
		return m_vDeferPicPathList[nIndex] ;
	}	
	else
	{
		return L"" ;
	}
}