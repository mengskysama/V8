#include "StdAfx.h"
#include "OutputHtmlEditImpl.h"

COutputHtmlEditImpl::COutputHtmlEditImpl(uint32 nHoldLine , ENM_HTML_TYPE nHtmlType )
:m_nHoldLine(nHoldLine),
m_nHtmlType( nHtmlType ) ,
m_nLine(0),
m_pDisplay(NULL)
{
}

COutputHtmlEditImpl::~COutputHtmlEditImpl(void)
{
}

void COutputHtmlEditImpl::DeleteDom(CString& strToDel)
{
	if(m_pDisplay == NULL)
	{
		return;
	}

	CComQIPtr<IHTMLDocument3> doc3(m_pDoc2);
	if (!doc3)
		return;

	CComQIPtr<IHTMLDOMNode> elementNode(m_pDisplay);

	CComBSTR delId(strToDel.GetBuffer());
	BOOL bNeedUpdate = FALSE;

	while(TRUE)
	{
		CComPtr<IHTMLElement> eleChild;
		if(S_OK == doc3->getElementById(delId,&eleChild))
		{					
			if (eleChild)
			{
				CComQIPtr<IHTMLDOMNode> pbRemoveNode(eleChild);
				if(S_OK == elementNode->removeChild(pbRemoveNode, NULL))
				{
					bNeedUpdate = TRUE;
				}
				break;
			}
			break;
		}
		else
		{
			break;
		}		
	}
	if(bNeedUpdate)
	{
		UpdateMy3();
	}
}

void COutputHtmlEditImpl::InsertText(CString& s)
{
	if(s.IsEmpty())
	{
		assert(0);
		return;
	}

	if(m_pDoc2 != NULL)
	{
		CString strTemp ;
		if ( m_nHtmlType == HTML_TYPE_PRIVATE )
		{
			strTemp.Format( L"<div id=%d>%s</div>" ,m_nLine , s ) ;			
			if ( m_nLine >= m_nHoldLine )
			{				
				long lScrollPos = 0 ;
				CComPtr<IHTMLTextContainer> pTextContainer ;

				CComPtr<IHTMLElement> pBody;
				m_pDoc2->get_body(&pBody);
				pBody->QueryInterface( IID_IHTMLTextContainer ,(void**)&pTextContainer ) ;
				pTextContainer->get_scrollTop( &lScrollPos ) ;

				CString strTodel ;
				strTodel.Format( L"%d" , m_nLine-m_nHoldLine ) ;
				DeleteDom( strTodel ) ;

				pTextContainer->put_scrollTop( lScrollPos ) ;
			}
			++m_nLine ;
		}
		else if ( m_nHtmlType == HTML_TYPE_SPEAK )
		{
			strTemp.Format( L"<div id=%d>%s</div>" ,m_nLine , s ) ;			
			if ( m_nLine >= m_nHoldLine )
			{				
				long lScrollPos = 0 ;
				CComPtr<IHTMLTextContainer> pTextContainer ;

				CComPtr<IHTMLElement> pBody;
				m_pDoc2->get_body(&pBody);
				pBody->QueryInterface( IID_IHTMLTextContainer ,(void**)&pTextContainer ) ;
				pTextContainer->get_scrollTop( &lScrollPos ) ;

				CString strTodel ;
				strTodel.Format( L"%d" , m_nLine-m_nHoldLine ) ;
				DeleteDom( strTodel ) ;

				pTextContainer->put_scrollTop( lScrollPos ) ;
			}
			++m_nLine ;
		}
		else
		{
			if ( m_nLine  >= m_nHoldLine )
			{
				ClearMsg();
			}
			++m_nLine ;

			strTemp.Format( L"<div>%s</div>" , s ) ;
		}		

		if(m_pDisplay == NULL)
			GetHtmlElement(_T("display"), &m_pDisplay);

		if(m_pDisplay != NULL)
		{
			m_pDisplay->insertAdjacentHTML(CComBSTR(_T("beforeEnd")),CComBSTR(strTemp));
		}
		if ( m_bIsNeedGotoEnd )
		{
			GotoEnd() ;
		}
	}
}

void COutputHtmlEditImpl::ClearMsg()
{
	HtmlEditImpl::ClearMsg() ;
	if ( m_nHtmlType != HTML_TYPE_PRIVATE )
		m_nLine= 0 ;
}

BOOL COutputHtmlEditImpl::OnHtmlFocusIn(IHTMLEventObj *pEvtObj)
{
	CWnd* pParent = GetParent();
	if (pParent == NULL)
	{
		return FALSE;
	}
	::PostMessage(pParent->GetSafeHwnd(), WM_OUTPUTHTMLEDITIMPLM_FOCUS_IN, (WPARAM)this, 0);
	return TRUE;
}

BOOL COutputHtmlEditImpl::OnHtmlFocusOut(IHTMLEventObj *pEvtObj)
{
	CWnd* pParent = GetParent();
	if (pParent == NULL)
	{
		return FALSE;
	}
	::PostMessage(pParent->GetSafeHwnd(), WM_OUTPUTHTMLEDITIMPLM_FOCUS_OUT, (WPARAM)this, 0);
	return TRUE;
}