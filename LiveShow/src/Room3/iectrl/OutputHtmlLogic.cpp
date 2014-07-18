#include "stdafx.h"
#include "OutputHtmlLogic.h"
#include "..\RoomParentDlg.h"
#include "..\userlist\UserListMenu.h"
#include "..\..\CommonLibrary\include\utility\SystemHelper.h"
#include "..\GlobalRoomDefine.h"

#define CHARGE_IFRAME_CODE L"<iframe src=\"http://client.179.com/room/roomLinkQQ?roomid=%d\"  width=\"116\" height=\"114\"  scrolling=\"no\"	frameborder=\"no\" border=\"0\" marginwidth=\"0\" allowtransparency=\"true\" marginheight=\"0\" style=\"background:transparent\" ></iframe>"

COutputHtmlLogic::COutputHtmlLogic( CRoomParentDlg* pParentDlg , ENM_HTML_TYPE nHtmlType )
:HtmlEditLogic(EDITABLE_OUT)
,m_pParentDlg(pParentDlg)
,m_nHtmlType(nHtmlType)
{
	m_Setting.m_bEnableContextMenu = true;
}

COutputHtmlLogic::~COutputHtmlLogic(void)
{

}


void COutputHtmlLogic::TranslateMsgForInput(CString& strText)
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

BOOL COutputHtmlLogic::OnClick(HtmlEditImpl* pHtmlEditImp, IHTMLEventObj *pEvtObj)
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
	if(tag == _T("A") || tag == _T("a"))
	{
		if ( m_pParentDlg != NULL )
		{
			CString strName = GetValue( pElement , _T("name") ) ;
			CString strUUID = GetValue( pElement , _T("uuid") ) ;
			CString strType = GetValue( pElement , _T("type") ) ;
			CString str179ID = GetValue( pElement , L"179id" ) ;
			CString strIndex = GetValue( pElement , _T("uindex") ) ;
			if ( !strName.IsEmpty() && !strUUID.IsEmpty() && !strType.IsEmpty() )
			{
				int nType = _wtoi( strType ) ;
				m_pParentDlg->OnClickHerfLink( strName , strUUID ,str179ID ,nType  , strIndex ) ;				
				if ( nType == CLICK_TYPE_P2P_ACCEPT || nType == CLICK_TYPE_P2P_REFUSE || nType == CLICK_TYPE_PUBLIC_ACCEPT || nType == CLICK_TYPE_PUBLIC_REFUSE )
				{										
					//删除当前行
					CComQIPtr<IHTMLDOMNode> pbParentNode(pElement);
					HRESULT hr  ;
					CComBSTR node_name ;
					do 
					{
						if ( pbParentNode->get_nodeName( &node_name) != S_OK )
						{
							break;
						}
						if ( node_name == L"DIV" || node_name == L"div")
						{
							pbParentNode->removeNode( VARIANT_TRUE , NULL ) ;
							pHtmlEditImp->UpdateMy3() ;
							break;
						}
						CComQIPtr<IHTMLDOMNode> pbTempNode ;
						hr = pbParentNode->get_parentNode( &pbTempNode ) ;
						pbParentNode = pbTempNode ;
					} while ( hr ==S_OK );
					
				}	
				else if ( nType == CLICK_TYPE_NO_REMIND || nType == CLICK_TYPE_TEMP_NOTICE )
				{
					CComQIPtr<IHTMLDOMNode> pbNode(pElement) ;
					CComPtr<IHTMLDocument2> pDoc2 = pHtmlEditImp->GetHtmlDocument2() ;
					if ( pDoc2 )
					{
						CComQIPtr<IHTMLElement> pParentElement ;
						pElement->get_parentElement( &pParentElement ) ;
						if ( pParentElement )
						{
							CComQIPtr<IHTMLDOMNode> pParentNode(pParentElement);
							CString strTemp ;
							if ( nType == CLICK_TYPE_TEMP_NOTICE )
							{
								strTemp.Format(_T("<SPAN style=FONT-SIZE:%dpx;color:%s>%s</SPAN>"),12 ,L"#8B8B8B",L"转发到临时公告") ;
							}
							else
							{
								strTemp.Format(_T("<SPAN style=FONT-SIZE:%dpx;color:%s>%s</SPAN>"),12 ,L"#8B8B8B",L"不再提醒") ;
							}							
							pParentNode->removeChild( pbNode , NULL ) ;
							pParentElement->insertAdjacentHTML(CComBSTR(_T("beforeEnd")),CComBSTR(strTemp));
						}
					}
				}
				return FALSE ;
			}
		}
		CComVariant url;
		CComBSTR url2;

		pElement->getAttribute(CComBSTR(_T("href")), 0, &url);
		url.CopyTo(&url2);

		CString strURL = url2;
		strURL.Replace( L"res://HtmlEditd.dll/out.html" ,L"") ;
		strURL.Replace( L"res://HtmlEditd.dll/" ,L"") ;
		if ( !strURL.IsEmpty() )
		{
			common::utility::systemhelper::OpenUrl( strURL.GetBuffer() ) ;
		}
	}
	else if ( tag == _T("img") || tag == _T("IMG") )
	{
		CComQIPtr<IHTMLElement> pParentElement ;
		if ( pElement->get_parentElement( &pParentElement ) == S_OK && pParentElement )
		{
			CComQIPtr<IHTMLDOMNode> pParentNode(pParentElement);
			CComBSTR node_name ;
			if ( pParentNode->get_nodeName( &node_name) == S_OK && (node_name == L"A" || node_name == L"a") )
			{
				CComVariant url;
				CComBSTR url2;

				pParentElement->getAttribute(CComBSTR(_T("href")), 0, &url);
				url.CopyTo(&url2);

				CString strURL = url2;
				strURL.Replace( L"res://HtmlEditd.dll/out.html" ,L"") ;
				strURL.Replace( L"res://HtmlEditd.dll/" ,L"") ;
				if ( !strURL.IsEmpty() )
				{
					common::utility::systemhelper::OpenUrl( strURL.GetBuffer() ) ;
				}
			}
		}
	}


	return FALSE;
}

BOOL COutputHtmlLogic::OnMouseOut(HtmlEditImpl* pHtmlEditImp, IHTMLEventObj *pEvtObj , CRect& CWindowRect ) 
{
	if ( m_pParentDlg )
	{
		m_pParentDlg->OnOutPutHtmlMouseOut( CWindowRect , m_nHtmlType ) ;
	}
	return TRUE ;
}

BOOL COutputHtmlLogic::OnMouseMove(HtmlEditImpl* pHtmlEditImp, IHTMLEventObj *pEvtObj , CRect& CWindowRect )
{
	if ( m_pParentDlg )
	{
		m_pParentDlg->OnOutPutHtmlMouseOver( CWindowRect , m_nHtmlType ) ;
		//pHtmlEditImp->SetFocus() ;
	}
	return TRUE ;
}

void COutputHtmlLogic::OnTrackHrefLinkMenu( uint32 unDstUin ) 
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


CString COutputHtmlLogic::GetValue(CComPtr<IHTMLElement> & p,CString key) 
{
	if(p)
	{
		CComVariant valuetemp;
		CComBSTR valuestr;
		HRESULT HR = p->getAttribute(CComBSTR(key),0,&valuetemp) ;
		if ( HR == S_OK && valuetemp.vt != VT_NULL )
		{
			valuetemp.CopyTo(&valuestr);
			valuetemp.Clear() ;
			CString  re(valuestr);
			return re;
		}
		valuetemp.Clear() ;
	}
	return L"";
}

void COutputHtmlLogic::OnOutputReady( HtmlEditImpl* pHtmlEditImp )
{
	pHtmlEditImp->InsertText( m_strInitText ) ;
	//pHtmlEditImp->SetBgColor( HTML_BACKGROUND_COLOR ) ;

	if(m_nHtmlType == HTML_TYPE_INPUT)
	{
		//CString str;
		//str.Format(CHARGE_IFRAME_CODE, m_pParentDlg->GetRoomID());
		//pHtmlEditImp->InsertDivText( str, L"chargeHelper" ) ;
		pHtmlEditImp->SetBgColor(HTML_BACKGROUND_COLOR);
	}else
	{
		pHtmlEditImp->SetBgColor(HTML_NEW_BACKGROUND_COLOR);
	}
}

void COutputHtmlLogic::SetInitText( CString& strText )
{
	m_strInitText = strText ;
}