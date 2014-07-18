#include "StdAfx.h"
#include "SpeakerOutputLogic.h"
#include "..\RoomParentDlg.h"
#include "utility\StringHelper.h"

CSpeakerOutputLogic::CSpeakerOutputLogic( CRoomParentDlg* pParentDlg, bool bScrollBtn )
:HtmlEditLogic(EDITABLE_OUT),m_pParentDlg(pParentDlg),m_bScrollBtn(bScrollBtn)
{
	m_Setting.m_bEnableContextMenu = true;
}

CSpeakerOutputLogic::~CSpeakerOutputLogic(void)
{
}

BOOL CSpeakerOutputLogic::OnClick( HtmlEditImpl* pHtmlEditImp, IHTMLEventObj *pEvtObj )
{
	if(pEvtObj == NULL)
	{
		assert(false);
		return FALSE;
	}

	CComPtr<IHTMLElement> pElement;
	pEvtObj->get_srcElement(&pElement);

	if(pElement == NULL)
	{
		assert(false);
		return FALSE;
	}	

	CComBSTR tag;
	pElement->get_tagName(&tag);
	if (tag == _T("img") || tag == _T("IMG"))
	{
		CComPtr<IHTMLElement> pParentElement;
		pElement->get_parentElement(&pParentElement);
		pParentElement->get_tagName((&tag));

		if(tag == _T("A") || tag == _T("a"))
		{
			CString strType = GetValue(pParentElement, _T("type"));
			CString strName = GetValue(pParentElement, _T("name"));
			if (strType == L"smallspeaker")
			{
				if (m_pParentDlg != NULL)
				{
					m_pParentDlg->OnClickSpeakerShortCut(strName.GetBuffer(0));
				}
			}
			else if (strType == L"bigspeaker")
			{
				if (m_pParentDlg != NULL)
				{
					m_pParentDlg->OnClickSpeakerShortCut(strName.GetBuffer(0), true);
				}
			}
			else if (strType == L"wish")
			{
				if (m_pParentDlg != NULL)
				{
					m_pParentDlg->OnClickWishShortCut(strName.GetBuffer(0));
				}
			}
		}

		return FALSE;
	}
	if(tag == _T("A") || tag == _T("a"))
	{
		CString strType = GetValue(pElement, _T("type"));
		if (strType == L"roomshortcut")
		{
			CString strRoomID = GetValue(pElement, _T("roomid"));
			uint32 unRoomID = common::utility::stringhelper::StringToInt(strRoomID.GetBuffer(0));
			m_pParentDlg->OnClickRoomShortCut(unRoomID);
		}
		else if (strType == L"gamereward")
		{
			CString strUrl = GetValue(pElement, _T("href"));
			m_pParentDlg->OnClickGameReward(strUrl.GetBuffer(0));
		}
	}

	return FALSE;
}

BOOL CSpeakerOutputLogic::OnMouseOut(HtmlEditImpl* pHtmlEditImp, IHTMLEventObj *pEvtObj , CRect& CWindowRect ) 
{
	if ( m_pParentDlg )
	{
		m_pParentDlg->OnOutPutHtmlMouseOut( CWindowRect , HTML_TYPE_SPEAK ) ;
	}
	return TRUE ;
}

BOOL CSpeakerOutputLogic::OnMouseMove(HtmlEditImpl* pHtmlEditImp, IHTMLEventObj *pEvtObj , CRect& CWindowRect )
{
	if ( m_pParentDlg && m_bScrollBtn )
	{
		m_pParentDlg->OnOutPutHtmlMouseOver( CWindowRect , HTML_TYPE_SPEAK ) ;
		//pHtmlEditImp->SetFocus() ;
	}
	return TRUE ;
}

CString CSpeakerOutputLogic::GetValue( CComPtr<IHTMLElement> & p,CString key )
{
	if(p)
	{
		CComVariant value;
		CComBSTR valuestr;
		HRESULT re = p->getAttribute(CComBSTR(key),0,&value);
		CString  restr;
		if( re == S_OK && value.vt != VT_NULL)
		{
			value.CopyTo(&valuestr);
			restr = valuestr;
		}
		return restr;
	}
	return NULL;
}

void CSpeakerOutputLogic::OnOutputReady( HtmlEditImpl* pHtmlEditImp )
{
	pHtmlEditImp->SetBgColor( L"ECF0F6" ) ;
}