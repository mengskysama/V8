#include "StdAfx.h"
#include "InputHtmlLogic.h"
#include "..\RoomParentDlg.h"
#include "..\..\CxImage\include\ximage.h"
#include "..\..\CommonLibrary\include\utility\URLEncode.h"
#include "..\..\CommonLibrary\include\utility\StringHelper.h"
#include "..\..\CommonLibrary\include\utility\Md5.h"
#include "..\..\CommonLibrary\include\utility\SystemHelper.h"
#include "..\userlist\UserListMenu.h"

CInputHtmlLogic::CInputHtmlLogic( CRoomParentDlg* pParentDlg , ENM_HTML_TYPE nHtmlType )
:HtmlEditLogic(EDITABLE_INPUT)
,m_pParentDlg(pParentDlg)
,m_nHtmlType(nHtmlType)
{
	m_Setting.m_bEnableContextMenu = true;
}

CInputHtmlLogic::~CInputHtmlLogic(void)
{
}

//输入字符时的字符个数判断
bool CInputHtmlLogic::OnInput(HtmlEditImpl* pHtmlEditImp)
{
	return true ;
}

void CInputHtmlLogic::OnEditCopy(HtmlEditImpl* pHtmlEditImp)
{
	CString strText, strHtmlText;

	m_pHtmlEditImp->OnEditCopy(strHtmlText,strText);

	CopyDataToClipbord(strHtmlText,strText);
}


//按 CTRL+ENTER 键发送消息
void CInputHtmlLogic::OnSendMessage(HtmlEditImpl* pHtmlEditImp)
{
	if ( m_pParentDlg )
	{
		m_pParentDlg->OnSendMsg() ;
	}
}


void CInputHtmlLogic::GetOutputString(CString& strText, CString& strImgText)
{
	m_pHtmlEditImp->GetContent( strText ) ;

	if(strText == _T("<P>&nbsp;</P>"))
	{
		strText = L"";
	}
	ReplaceImgForInput(strText);


	strImgText = strText;
}

void CInputHtmlLogic::ReplaceImgForInput(CString& strText)
{
	strText.Replace(_T("</P>\r\n<P>"), _T("<BR><BR>"));
	strText.Replace( _T("<BR>") , _T("") ) ;   ///
	strText.Replace( _T("</P>") ,_T("") ) ;	   ///
	strText.Replace( _T("<P>") , _T("") ) ;    ///
}

void CInputHtmlLogic::MsgFormatInput(CString& strText)
{
	strText.Replace(L" tag=\"c_i\"", L"");
	strText.Replace(L" tag=\"j_i\"", L"");


	static std::wstring format1( L"[I:0:$2]" );
	static std::wstring format2( L"[I:1:$2.$3]");
	static std::wstring format3( L"[I:2:$2.$3]");
	static std::wstring format4( L"$2" );
	static std::wstring format5( L"[A:0:$1]" );

	wstring strMsg = strText.GetBuffer();

	strMsg = regex_replace(strMsg, m_regex[0], format1);
	strMsg = regex_replace(strMsg, m_regex[1], format2);
	strMsg = regex_replace(strMsg, m_regex[2], format3);
	strMsg = regex_replace(strMsg, m_regex[3], format4);
	strMsg = regex_replace(strMsg, m_regex[4], format5);

	strText = strMsg.c_str();
}


//清空输入框的内容
void CInputHtmlLogic::ClearInput() 
{
	m_pHtmlEditImp->ClearMsg() ;
}

uint32 CInputHtmlLogic::GetPictureNum(CString& strText)
{
	uint32 nCount = 0;

	int nPos = 0;
	CString strTemp = strText.MakeUpper();

	while(1)
	{
		int nTemp = strTemp.Find(L"<IMG", nPos);
		if (nTemp < 0)
		{
			break;
		}

		++nCount;
		nPos = nTemp + 1;
	}

	return nCount;
}

uint32 CInputHtmlLogic::GetPictureNum() 
{
	CString strText ;
	m_pHtmlEditImp->GetContent( strText ) ;

	if(strText == _T("<P>&nbsp;</P>"))
	{
		strText = L"";
	}
	ReplaceImgForInput(strText);
	return GetPictureNum( strText ) ;
}

uint32 CInputHtmlLogic::GetContentLen() 
{
	CString strText = m_pHtmlEditImp->GetContentText() ;
	//这个接口，中文算2个字符，字母算1个
	uint32 con = common::utility::stringhelper::CalcStringAsciiLength(std::wstring(strText.GetBuffer(0)));
	
	//这个会把中文当一个字符算
	//return strText.GetLength() ;
	
	return con;
}

void CInputHtmlLogic::TranslateMsgForLimit(CString& strText)
{
	CComPtr<IHTMLElement> pTranslateElement;
	m_pHtmlEditImp->GetHtmlElement(_T("fortranslate"), &pTranslateElement);

	if(pTranslateElement != NULL)
	{
		pTranslateElement->put_innerHTML(CComBSTR(strText));
		CComBSTR strTemp;
		pTranslateElement->get_innerText(&strTemp);
		pTranslateElement->put_innerHTML(CComBSTR(_T("")));
		strText = strTemp;
	}
}

void CInputHtmlLogic::OnOutputReady( HtmlEditImpl* pHtmlEditImp )
{
	pHtmlEditImp->SetBgColor( HTML_BACKGROUND_COLOR ) ;
}


void CInputHtmlLogic::OnTrackHrefLinkMenu( uint32 unDstUin ) 
{
	IUserItem* pUserItem = m_pParentDlg->GetRoomBusiness()->GetUserManager()->SearchUserByUIN( unDstUin ) ;
	if ( pUserItem == NULL )
	{
		return ;
	}
	UserListMenu * pMenu = new UserListMenu(m_pParentDlg);
	if ( pMenu != NULL )
	{
		m_pParentDlg->m_pSelectUserItem = pUserItem ;
		IUserItem * pSelfItem = m_pParentDlg->GetRoomBusiness()->GetUserManager()->GetSelfUserItem();
		if (pSelfItem != NULL)
		{
			pMenu->PopupMenu( pSelfItem, pUserItem ) ;
		}
		delete pMenu;
		pMenu = NULL;
	}
}


void CInputHtmlLogic::SetFontSize(UINT fontsize ) 
{
	m_Setting.m_stChatFont.font_size = fontsize ;
}


void CInputHtmlLogic::SetFontBold( int checked ) 
{
	if ( checked == 1 )
	{
		m_Setting.m_stChatFont.font_style |= FONT_TYPE_BOLD ;
	}
	else
	{
		m_Setting.m_stChatFont.font_style &= (~FONT_TYPE_BOLD) ;
	}
}

void CInputHtmlLogic::SetFontItalic( int checked ) 
{
	if ( checked == 1 )
	{
		m_Setting.m_stChatFont.font_style |= FONT_TYPE_ITALIC ;
	}
	else
	{
		m_Setting.m_stChatFont.font_style &= (~FONT_TYPE_ITALIC) ;
	}
}

void CInputHtmlLogic::SetFontUnderLine( int checked ) 
{
	if ( checked == 1 )
	{
		m_Setting.m_stChatFont.font_style |= FONT_TYPE_UNDERLINE ;
	}
	else
	{
		m_Setting.m_stChatFont.font_style &= (~FONT_TYPE_UNDERLINE) ;
	}
}

void CInputHtmlLogic::SetFontName(CString& strFontName ) 
{
	m_Setting.m_stChatFont.font_type = strFontName ;
}

void CInputHtmlLogic::SetFontColor(COLORREF cf ) 
{
	m_Setting.m_stChatFont.font_color = cf ;
}

//得到字体信息(大小、颜色、字体名)
MessageFont CInputHtmlLogic::GetFontInfo() 
{
	return m_Setting.m_stChatFont ;
}

void CInputHtmlLogic::GetOutputStringWithoutPic( CString & strText )
{
	m_pHtmlEditImp->GetContent( strText ) ;

	if(strText == _T("<P>&nbsp;</P>"))
	{
		strText = L"";
	}
	ReplaceImgForInput(strText);
	strText.Replace(_T("&nbsp;"),_T(" "));
	//strText.TrimRight();

	int nPos = 0;
	CString strTemp = strText.MakeUpper();

	while (1)
	{
		int nBegin = strTemp.Find(L"<", 0);
		if (nBegin < 0)
		{
			break;
		}

		int nEnd = strTemp.Find(L">", nBegin);
		ASSERT(nEnd >= 0);
		strTemp = strTemp.Left(nBegin) + strTemp.Right(strTemp.GetLength() - nEnd - 1);
	}

	strTemp.Replace(_T("&LT;"),_T("<"));
	strTemp.Replace(_T("&GT;"),_T(">"));

	strText = strTemp;
}