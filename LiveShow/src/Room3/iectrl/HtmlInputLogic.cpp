#include "StdAfx.h"
#include "HtmlInputLogic.h"
//#include "SpeakerDlg.h"
#include "..\..\CxImage\include\ximage.h"
#include "..\..\CommonLibrary\include\utility\URLEncode.h"
#include "..\..\CommonLibrary\include\utility\StringHelper.h"
#include "..\..\CommonLibrary\include\utility\Md5.h"
#include "..\..\CommonLibrary\include\utility\SystemHelper.h"
#include "OutInputHtmlEditImpl.h"
CHtmlInputLogic::CHtmlInputLogic(int iEditable)
:HtmlEditLogic(iEditable)
{
	m_Setting ;		//设置属性
}

CHtmlInputLogic::~CHtmlInputLogic(void)
{
}

//输入字符时的字符个数判断
bool CHtmlInputLogic::OnInput(HtmlEditImpl* pHtmlEditImp)
{
	CString strText = m_pHtmlEditImp->GetContentText();
	int nLen = common::utility::stringhelper::CalcStringAsciiLength(strText.GetBuffer(0));
	//if ( m_pSpeakerDlg )
	//{
	//	return m_pSpeakerDlg->OnInput( nLen ) ;
	//}
	return true ;
}

void CHtmlInputLogic::OnEditCopy(HtmlEditImpl* pHtmlEditImp)
{
	CString strText, strHtmlText;

	m_pHtmlEditImp->OnEditCopy(strHtmlText,strText);

	CopyDataToClipbord(strHtmlText,strText);
}


//按 CTRL+ENTER 键发送消息
void CHtmlInputLogic::OnSendMessage(HtmlEditImpl* pHtmlEditImp)
{
	//if ( m_pRoomDlg )
	//{
	//	m_pRoomDlg->SendMsg() ;
	//}
}


void CHtmlInputLogic::GetOutputString(CString& strText, CString& strImgText)
{
	m_pHtmlEditImp->GetContent( strText ) ;

	if(strText == _T("<P>&nbsp;</P>"))
	{
		strText = L"";
	}
	ReplaceImgForInput(strText);


	strImgText = strText;
}

void CHtmlInputLogic::ReplaceImgForInput(CString& strText)
{
	strText.Replace(_T("</P>\r\n<P>"), _T("<BR><BR>"));
}

void CHtmlInputLogic::MsgFormatInput(CString& strText)
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
void CHtmlInputLogic::ClearInput() 
{
	m_pHtmlEditImp->ClearMsg() ;
}

uint32 CHtmlInputLogic::GetPictureNum(CString& strText)
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

uint32 CHtmlInputLogic::GetPictureNum() 
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

void CHtmlInputLogic::OnOutputReady(HtmlEditImpl* pHtmlEditImp)
{
	COutInputHtmlEditImpl * pHtml = dynamic_cast<COutInputHtmlEditImpl*>(pHtmlEditImp);
	if (pHtml != NULL)
	{
		pHtml->InsertText(m_strInitText);
	}
	//pHtmlEditImp->InsertText( m_strInitText ) ;
}

void CHtmlInputLogic::TranslateMsgForLimit(CString& strText)
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

void CHtmlInputLogic::SetInitText( CString& strText ) 
{
	m_strInitText = strText ;
}

void CHtmlInputLogic::SetFontSize(UINT fontsize ) 
{
	m_Setting.m_stChatFont.font_size = fontsize ;
}


void CHtmlInputLogic::SetFontBold( int checked ) 
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

void CHtmlInputLogic::SetFontItalic( int checked ) 
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

void CHtmlInputLogic::SetFontUnderLine( int checked ) 
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

void CHtmlInputLogic::SetFontName(CString& strFontName ) 
{
	m_Setting.m_stChatFont.font_type = strFontName ;
}

void CHtmlInputLogic::SetFontColor(COLORREF cf ) 
{
	m_Setting.m_stChatFont.font_color = cf ;
}

//得到字体信息(大小、颜色、字体名)
MessageFont CHtmlInputLogic::GetFontInfo() 
{
	return m_Setting.m_stChatFont ;
}
