#include "stdafx.h"
#include "OutInputHtmlEditImpl.h"

COutInputHtmlEditImpl::COutInputHtmlEditImpl() 
{

}

COutInputHtmlEditImpl::~COutInputHtmlEditImpl() 
{

}

void COutInputHtmlEditImpl::InsertText( CString s, bool bText/* = false*/ )
{
	if(m_pDoc2 != NULL && !s.IsEmpty() )
	{
		if ( bText )
		{
			CComPtr<IHTMLElement> p;
			GetHtmlElement(_T("display"), &p);

			if(p != NULL)
			{
				p->insertAdjacentHTML(CComBSTR(_T("beforeEnd")),CComBSTR(s));
			}
			else
			{
				CComPtr<IHTMLElement> pBody;
				m_pDoc2->get_body(&pBody);
				if (pBody)
				{
					if (bText)
					{
						pBody->insertAdjacentText(CComBSTR(_T("beforeEnd")),CComBSTR(s));
					}
					else
					{
						pBody->insertAdjacentHTML(CComBSTR(_T("beforeEnd")),CComBSTR(s));
					}
				}

			}
		}
		else
		{
			InsertAtSelectionArea( s ) ;
		}
		
		if ( m_bIsNeedGotoEnd )
		{
			GotoEnd() ;
		}
	}
}

void COutInputHtmlEditImpl::GetContent( CString & content )
{
	if (m_pDoc2)
	{
		CComPtr<IHTMLElement> p;
		GetHtmlElement(_T("display"), &p);

		CComBSTR con;
		if (p != NULL)
		{
			p->get_innerHTML(&con);
			//p->get_innerText(&con);
		}
		else
		{
			CComPtr<IHTMLElement> pBody;
			m_pDoc2->get_body(&pBody);

			pBody->get_innerHTML(&con);
			//pBody->get_innerText(&con);

		}		
		content = con;
		content.Replace( L"<P>&nbsp;</P>" , L"" ) ;
		content.Replace( L"<P>" , L"" ) ;
		content.Replace(L"</P>" ,L"" ) ;
	}
}