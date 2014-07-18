#include "StdAfx.h"
#include "SpeakerMsg.h"
#include "utility/StringHelper.h"
#include "utility/expat++/expat_document.h"
#include "..\..\HTMLEdit\FilterHtmlEdit.h"
#include "..\userlist\ListIconManager.h"

CSpeakerMsg::CSpeakerMsg(void)
{
}

CSpeakerMsg::~CSpeakerMsg(void)
{
}

bool CSpeakerMsg::EncodeMessage( std::wstring & strMessage, core::MessageFont stFont, IUserItem * pUserItem, 
								std::wstring& strRoomName , uint32 unRoomID , bool bWish/* = false*/ )
{
	CString strTemp = strMessage.c_str();
	CString strRoomShortCut ;
	//简化超级链接
	strRoomShortCut.Format( L"<Aid=%d name=%s >"  , unRoomID , strRoomName.c_str() );
	strTemp.Replace( GetRoomShortCutFromHtml( unRoomID , strRoomName ) , strRoomShortCut ) ;
	strTemp.Replace(_T("&nbsp;"),_T(" "));
	strMessage = strTemp.GetBuffer();
	strMessage = common::utility::stringhelper::XMLEncode(strMessage);
	CString strContent ;
	strContent.Format( L"<Content Style=\"%d\" Size=\"%d\" FType=\"%s\" Color=\"%d\" Name=\"%s\" ID=\"%u\" Stamp=\"%d\" Vip=\"%d\" Type=\"%d\">%s</Content>" ,
		stFont.font_style, stFont.font_size, stFont.font_type.c_str(), stFont.font_color, 
		pUserItem->GetDecodeUserName().c_str(), pUserItem->GetUser179ID(), pUserItem->GetUserStampID(),
		pUserItem->GetUserVipLevel(), bWish ? 1 : 0 , strMessage.c_str() );

	strMessage = strContent.GetBuffer();

	return true;
}

bool CSpeakerMsg::DecodeMessage( std::wstring & strMessage, core::MessageFont & stFont, SpeakerInfo & stInfo )
{
	CString strText ;
	strText.Format( L"<?xml version=\"1.0\" encoding=\"utf-8\" ?><WELCOMEWORDS>%s</WELCOMEWORDS>" , strMessage.c_str() ) ;

	common::utility::expat_document xmlDoc;

	std::string strUtf8 = common::utility::stringhelper::UnicodeToUTF8( strText.GetBuffer() ) ;
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

	strMessage = pElement->get_element_contents();
	strText = strMessage.c_str();


	//对字符串分割
	vector<CString> vStringList ;
	while( !strText.IsEmpty() )
	{
		int nBeginPos = strText.Find( L"<Aid" , 0 ) ;
		if ( nBeginPos < 0 )
		{
			vStringList.push_back( strText ) ;
			strText.Delete( 0 , strText.GetLength() ) ;			
			break;
		}
		else
		{
			CString strTemp = strText.Mid( 0 , nBeginPos ) ;
			vStringList.push_back( strTemp ) ;
			strText.Delete( 0 , nBeginPos ) ;
		}

		int nPos = strText.Find( L">" , 0 ) ;
		if ( nPos < 0 )
		{
			break;
		}
		else
		{
			CString strTemp = strText.Mid( 0 , nPos+1 ) ;
			vStringList.push_back( strTemp ) ;
			strText.Delete( 0 , nPos+1 ) ;
		}

	}
	
	//替换空格后再合并字符串
	vector<CString>::iterator iter ;
	for ( iter = vStringList.begin() ; iter != vStringList.end() ; ++iter )
	{
		CString strTemp = *iter ;
		if ( strTemp.Find( L"Aid") < 0 )
		{
			strTemp.Replace(_T(" "),_T("&nbsp;"));
		}
		strText += strTemp ;
	}

	//还原超级链接
	int nStartPos = 0 ;
	while( true )
	{
		nStartPos = strText.Find( L"<Aid" , nStartPos ) ;
		if ( nStartPos < 0 )
		{
			break;
		}
		int nEndPos = strText.Find( L">" , nStartPos+1 ) ;
		if ( nEndPos < 0 )
		{
			break; 
		}
		CString strCode = strText.Mid( nStartPos , nEndPos - nStartPos+1 ) ;
		TCHAR szRoomName[MAX_PATH] ;
		uint32 unRoomID ;
		swscanf( strCode.GetBuffer() , L"<Aid=%d name=%s >"  , &unRoomID ,szRoomName  ) ;
		CString strTemp = GetRoomShortCut( unRoomID , szRoomName ) ;
		strText.Replace( strCode , strTemp ) ;
		nStartPos = nStartPos +1 ;
	}

	strMessage = strText.GetBuffer();
	stFont.font_type = pElement->get_attribute_by_name(L"FType")->get_attribute_value();
	stFont.font_color = common::utility::stringhelper::StringToInt(pElement->get_attribute_by_name(L"Color")->get_attribute_value());
	stFont.font_size = common::utility::stringhelper::StringToInt(pElement->get_attribute_by_name(L"Size")->get_attribute_value());
	stFont.font_style = common::utility::stringhelper::StringToInt(pElement->get_attribute_by_name(L"Style")->get_attribute_value());

	stInfo.strName = pElement->get_attribute_by_name(L"Name")->get_attribute_value();
	stInfo.un179ID = common::utility::stringhelper::StringToInt( pElement->get_attribute_by_name(L"ID")->get_attribute_value());
	stInfo.unStampID = common::utility::stringhelper::StringToInt( pElement->get_attribute_by_name(L"Stamp")->get_attribute_value() );
	stInfo.unVipLevel = common::utility::stringhelper::StringToInt(pElement->get_attribute_by_name(L"Vip")->get_attribute_value() );
	stInfo.enmType = (SpeakerInfo::ENM_SpeakerType)common::utility::stringhelper::StringToInt( pElement->get_attribute_by_name(L"Type")->get_attribute_value());
	return true;
}

CString CSpeakerMsg::GetRoomShortCut( uint32 unRoomID, std::wstring strRoomName )
{
	CString strTemp;
	strTemp.Format(L"<a href=1111 type=%s name=%s roomid=%d>%s(%d)</a>", L"roomshortcut", strRoomName.c_str(), unRoomID, strRoomName.c_str(), unRoomID);
	return strTemp;
}

CString CSpeakerMsg::GetRoomShortCutFromHtml( uint32 unRoomID, std::wstring strRoomName ) 
{
	CString strTemp;
#ifdef _DEBUG
	strTemp.Format(L"<A href=\"res://HtmlEditd.dll/1111\" type=%s name=%s roomid=\"%d\">%s(%d)</A>", /*L"roomshortcut",*/ L"roomshortcut", strRoomName.c_str(), unRoomID, strRoomName.c_str(), unRoomID);
#else	
	strTemp.Format(L"<A href=\"res://HtmlEdit.dll/1111\" type=%s name=%s roomid=\"%d\">%s(%d)</A>", /*L"roomshortcut",*/ L"roomshortcut", strRoomName.c_str(), unRoomID, strRoomName.c_str(), unRoomID);
#endif
	return strTemp;
}

CString CSpeakerMsg::GetSpeakerShortCut( core::ENM_LoudSpeakerType enmType, SpeakerInfo stInfo )
{
	CString strTemp;
	//if (stInfo.enmType == SpeakerInfo::SPEAKERTYPE_WISH)
	//{
	//	strTemp.Format(L"<a href=\"%s\" type=\"%s\" name=\"%s(%d)\"><img src=\"%s\"   border=\"0\" title=\"点击回复TA的喇叭\"></a>", L"wish", L"wish", stInfo.strName.c_str(), stInfo.un179ID, ListIconManager::GetIconPathByIndex(USERLIST_ICON_WISH).c_str());
	//}
	//else
	//{
	if (core::LOUDSPEAKERTYPE_SMALL == enmType)
	{
		strTemp.Format(L"<a href=\"%s\" type=\"%s\" name=\"%s(%d)\"><img src=\"%s\"  border=\"0\" title=\"点击回复TA的喇叭\"></a>", L"smallspeaker", L"smallspeaker", stInfo.strName.c_str(), stInfo.un179ID, ListIconManager::GetIconPathByIndex(USERLIST_ICON_SMALLSPEAKER).c_str());
	}
	else
	{
		strTemp.Format(L"<a href=\"%s\" type=\"%s\" name=\"%s(%d)\"><img src=\"%s\"  border=\"0\" title=\"点击回复TA的喇叭\"></a>", L"bigspeaker", L"bigspeaker", stInfo.strName.c_str(), stInfo.un179ID, ListIconManager::GetIconPathByIndex(USERLIST_ICON_BIGSPEAKER).c_str());
	}
	 //}

	return strTemp;
}

void CSpeakerMsg::DisableRoomShortcut( CString & strMessage )
{
	CHtmlFilter::DelLink(strMessage);
}