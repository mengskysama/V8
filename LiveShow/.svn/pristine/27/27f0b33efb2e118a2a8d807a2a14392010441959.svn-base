/*
@file:		HtmlEditCore2.cpp
@purpose:	ie编辑器
@feature:	
@version:	1.0 2008.01.17 created by ken
*/
#pragma once
#include "stdafx.h"
#include "HtmlEditCore2.h"
#include "mshtmcid.h"
#include "afxole.h"
#include "mshtmlc.h"
#include <afxctl.h>
#include <ExDispid.h>
#include "CMyIEDropTarget.h"  
#include <string>
#include <vector>
#include "shlwapi.h"
#include "mshtmlc.h"
#include "windef.h"
#include "afxadv.h"
#include <stack>
#include "winnt.h"
#include "..\CommonLibrary\thirdlibrary\CxImage\include\ximage.h"
#include "FilterHtmlEdit.h"
#include "LiteHTMLReader.h"
#include "NoActionDispatch.h"
#include "resource.h"
#include "HtmlControlSite.h"
#include "globaluidefine.h"
#include "../CommonLibrary/include/utility/URLEncode.h"
#include "../CommonLibrary/include/utility/systemhelper.h"
#include <map>
using namespace common;

BEGIN_DISPATCH_MAP(HtmlEditImpl, CHtmlEditView)
	DISP_FUNCTION_ID(HtmlEditImpl, "onkeydown",        DISPID_HTMLDOCUMENTEVENTS2_ONKEYDOWN,       OnKeyDown,       VT_BOOL, VTS_DISPATCH)	
	DISP_FUNCTION_ID(HtmlEditImpl, "onclick",          DISPID_HTMLDOCUMENTEVENTS2_ONCLICK,         onClick,         VT_BOOL, VTS_DISPATCH)
	DISP_FUNCTION_ID(HtmlEditImpl, "onmouseout" ,	   DISPID_HTMLDOCUMENTEVENTS2_ONMOUSEOUT,	   OnMouseOut,		VT_BOOL, VTS_DISPATCH)
	DISP_FUNCTION_ID(HtmlEditImpl, "onmousemove",	   DISPID_HTMLDOCUMENTEVENTS2_ONMOUSEMOVE,	   OnMouseMove,		VT_BOOL, VTS_DISPATCH)
	DISP_FUNCTION_ID(HtmlEditImpl, "oncontrolselect",  DISPID_HTMLDOCUMENTEVENTS2_ONCONTROLSELECT, OnControlSelect, VT_BOOL, VTS_DISPATCH)
	DISP_FUNCTION_ID(HtmlEditImpl, "oncontrolselect",  DISPID_HTMLDOCUMENTEVENTS2_ONFOCUSIN, OnHtmlFocusIn, VT_BOOL, VTS_DISPATCH)
	DISP_FUNCTION_ID(HtmlEditImpl, "oncontrolselect",  DISPID_HTMLDOCUMENTEVENTS2_ONFOCUSOUT, OnHtmlFocusOut, VT_BOOL, VTS_DISPATCH)
END_DISPATCH_MAP()

BEGIN_EVENTSINK_MAP(HtmlEditImpl, CHtmlEditView)
	ON_EVENT_REFLECT(HtmlEditImpl, DISPID_NAVIGATECOMPLETE2,  OnNavigateComplete2, VTS_DISPATCH VTS_PVARIANT)
	ON_EVENT_REFLECT(HtmlEditImpl, DISPID_PROGRESSCHANGE,     OnProgressChange,    VTS_I4 VTS_I4)
    ON_EVENT_REFLECT(HtmlEditImpl, DISPID_DOWNLOADCOMPLETE,   OnHtmlDownloadComplete,    VTS_NONE)
END_EVENTSINK_MAP()

IMPLEMENT_DYNAMIC(HtmlEditImpl, CHtmlEditView)

BEGIN_MESSAGE_MAP(HtmlEditImpl, CHtmlEditView)
	ON_WM_DESTROY()
	ON_WM_CREATE()
	ON_WM_TIMER()
	ON_COMMAND(HTMLEDITIMPL_EDIT_COPY_WORD,          OnEditCopyWord)
	ON_COMMAND(HTMLEDITIMPL_EDIT_CLEAN_SCREEN,		 OnEditCleanScreen)
	ON_COMMAND(HTMLEDITIMPL_EDIT_PASTE,				 OnEditPasteWrapper)	
	ON_MESSAGE(HTMLEDITIMPL_EDIT_GET_FOCUS,          OnGetFocus)
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////

HtmlEditImpl::HtmlEditImpl()
{
	m_bReady = false;
	m_bIsNeedGotoEnd = true;
	EnableAutomation();
	m_pOrgIEProc = NULL ;
}

HtmlEditImpl::~HtmlEditImpl(void)
{
	if(m_pDoc2 != NULL)
	{
		AfxConnectionUnadvise(m_pDoc2, DIID_HTMLDocumentEvents2 , 
			GetIDispatch(FALSE),  FALSE, m_dwCookie);
	}
	std::map<HWND,CWnd*>::iterator iter ;
	for ( iter = m_staTable.begin() ; iter != m_staTable.end() ; ++iter )
	{
		if ( iter->second == this )
		{
			m_staTable.erase( iter ) ;
			break;
		}
	}
}

void HtmlEditImpl::SetHtmlEditLogic(HtmlEditLogic * pLogic )
{
	m_pLogic = pLogic;	

	if(pLogic != NULL)
	{
		pLogic->SetHtmlEditImpl(this);
	}
}

HtmlEditLogic* HtmlEditImpl::GetHtmlEditLogic()
{
	return m_pLogic;
}

void HtmlEditImpl::ClearMsg()
{
	if ( m_pLogic->m_Setting.m_nEditable == EDITABLE_INPUT)
	{
		PutBodyInnerHTML(_T(""));
	}
	else
	{
		CComPtr<IHTMLElement> pEle;
		GetHtmlElement(_T("display"),&pEle);
		if ( pEle )
		{
			pEle->put_innerHTML(_T("") ) ;
		}
		else
		{
			PutBodyInnerHTML(_T(""));
		}
	}
}

unsigned long HtmlEditImpl::CountInnterTextLen(CString & const input)
{
	if(input.IsEmpty())
	{
		return 0;
	}

	CComPtr<IHTMLElement> pEle;
	GetHtmlElement(_T("fortranslate"),&pEle);
	if(pEle)
	{
		pEle->put_innerHTML(CComBSTR(input));
		CComBSTR ss;
		pEle->get_innerText(&ss);
		CString content (ss);
		long len = content.GetLength();
		return len;
	}

	return 0;
}

void HtmlEditImpl::ClearDiv(std::wstring  const & const id)
{
	CComPtr<IHTMLElement> p;
	if (m_pDoc2)
	{
		CString id2 = id.c_str();
		GetHtmlElement(id2,&p);
		if (p)
		{
			CComBSTR strTemp;
			p->get_innerHTML(&strTemp);
			if(strTemp.Length() > 0)// 当不加此判断时，ie会put一串伟大的"&nbsp;"进去 2009-3-11
			{
				p->put_innerHTML(CComBSTR(""));
			}
		}
	}
}

void HtmlEditImpl::InsertNewLine()
{
	CComPtr<IHTMLSelectionObject> pSelection;
	CComPtr<IHTMLTxtRange> pDisp;
	if (m_pDoc2)
	{
		m_pDoc2->get_selection(&pSelection);
		pSelection->createRange((IDispatch**)&pDisp);
		pDisp->pasteHTML(CComBSTR(_T("<BR>")));

		//2009-4-8 光标移动到下一行 plushu找了好久啊，nnd
		pDisp->moveStart(CComBSTR(L"character"), 0, 0);
		pDisp->collapse(TRUE);
		pDisp->select();
	}
}

void HtmlEditImpl::PutBodyInnerHTML(CString s)
{
	if(m_pDoc2)
	{
		CComPtr<IHTMLElement> pBody;
		m_pDoc2->get_body(&pBody);
		if(pBody)
		{
			if(s.IsEmpty())
			{
				CComBSTR strTemp;
				pBody->get_innerHTML(&strTemp);
				if(strTemp.Length() == 0)// 当不加此判断时，ie会put一串伟大的"&nbsp;"进去 2009-3-11
				{
					return;
				}
			}			

			pBody->put_innerHTML(CComBSTR(s));
		}
	}
}

BOOL HtmlEditImpl::Create(CWnd* pParent,UINT nID,CRect& rcIE, DWORD style)
{
	BOOL bRet =  CHtmlEditView::Create(NULL, // class name
		NULL,     // title
		style,    // style
		rcIE,     // rectangle
		pParent,  // parent
		nID,      // control ID
		NULL);   // frame/doc context not used
	if ( bRet )
	{
		//设置是否可编辑
		if ( m_pLogic->m_Setting.m_nEditable == EDITABLE_OUT )
		{
			SetUnEditAble() ;			
		}
		else
		{
			SetEditAble() ;
		}
	}	
	return bRet;
}

void HtmlEditImpl::GotoEnd()
{
	InnerGotoEnd();
}

void HtmlEditImpl::SetGotoEndAble()
{
	m_bIsNeedGotoEnd = true ;
	GotoEnd();
}

void HtmlEditImpl::SetUnGotoEnd() 
{
	m_bIsNeedGotoEnd = false ;
}

void HtmlEditImpl::InnerGotoEnd()
{
	if(IsReady())
	{
		_variant_t pResult;
		CStringArray paramArray;
		CallJScript(_T("scroll2Bottom"),paramArray,&pResult);
	}	
}

long HtmlEditImpl::GetScrollPos()
{
	long  pos = 0;
	CComPtr <IHTMLElement>  pElembody;

	if (m_pDoc2 != NULL)
	{
		m_pDoc2->get_body (&pElembody); 

		CComPtr <IHTMLTextContainer>  pCont;  
		if (pElembody) 
		{ 
			pElembody->QueryInterface(IID_IHTMLTextContainer, (LPVOID *)(&pCont));  
			if (pCont) 
			{ 
				pCont->get_scrollTop(&pos); 
			} 
		}  
	}
	
	return pos;
}


long HtmlEditImpl::GetScrollHeight()
{
	long  height = 0;
	CComPtr <IHTMLElement>  pElembody;

	if (m_pDoc2 != NULL)
	{
		m_pDoc2->get_body (&pElembody); 

		CComPtr <IHTMLTextContainer>  pCont;  
		if (pElembody) 
		{ 
			pElembody->QueryInterface(IID_IHTMLTextContainer, (LPVOID *)(&pCont));  
			if (pCont) 
			{ 
				pCont->get_scrollHeight(&height); 
			} 
		} 
	}
	
	return height;
}

long  HtmlEditImpl::GetClientHeight()
{
	//long  height = 0;
	//CComPtr <IHTMLElement>  pElembody;

	//m_pDoc2->get_body(&pElembody);

	CRect rect;
	GetWindowRect(&rect);

	return rect.Height() + 60;
}

void HtmlEditImpl::GetContent(CString & content)
{
	CComPtr<IHTMLElement> pInsertPlace;

	if (m_pDoc2)
	{
		m_pDoc2->get_body(&pInsertPlace);
		if(pInsertPlace != NULL)
		{
			CComBSTR con;
			pInsertPlace->get_innerHTML(&con);

			/*plushu test

			CComBSTR innerText;	
			pInsertPlace->get_innerText(&innerText);

			CComBSTR outerText;
			pInsertPlace->get_outerText(&outerText);

			CComBSTR outerHtml;
			pInsertPlace->get_outerHTML(&outerHtml);*/

			content = con;
		}
	}
}

CString HtmlEditImpl::GetContentText( ) 
{
	CString strText ;
	CComPtr<IHTMLElement> pInsertPlace;

	if (m_pDoc2)
	{
		m_pDoc2->get_body(&pInsertPlace);
		if(pInsertPlace != NULL)
		{
			CComBSTR con;
			pInsertPlace->get_innerText( &con) ;

			strText = con;
		}
	}
	return strText ;
}

void HtmlEditImpl::OutputDebugInfo()
{
	CComPtr<IHTMLElement> pInsertPlace;
	CComBSTR con;

	CString info;

	if (m_pDoc2)
	{
		m_pDoc2->get_body(&pInsertPlace);
		if(pInsertPlace != NULL)
		{
			pInsertPlace->get_innerHTML(&con);

			CString abc(con);
			abc.Replace(L"\r\n", L"");
			info.Format(L"plushu new innerHTML %s\r\n", abc.GetBuffer());
			//OutputDebugString(info.GetBuffer());

			CComBSTR innerText;
			pInsertPlace->get_innerText(&innerText);

			CString ddd(innerText);
			ddd.Replace(L"\r\n", L"");
			info.Format(L"plushu new innerText %s\r\n", ddd.GetBuffer());
			//OutputDebugString(info.GetBuffer());

			CComBSTR outerText;
			pInsertPlace->get_outerText(&outerText);

			CString eee(outerText);
			eee.Replace(L"\r\n", L"");
			info.Format(L"plushu new outerText %s\r\n", eee.GetBuffer());
			//OutputDebugString(info.GetBuffer());

			CComBSTR outerHtml;
			pInsertPlace->get_outerHTML(&outerHtml);

			CString fff(outerHtml);
			fff.Replace(L"\r\n", L"");
			info.Format(L"plushu new outerHTML %s\r\n", fff.GetBuffer());
			//OutputDebugString(info.GetBuffer());
		}
	}
}

long HtmlEditImpl::GetContentLen()
{
	CString content;

	GetContent(content);

	if(content == _T("<P>&nbsp;</P>"))
	{
		return 0;
	}
	else
	{
		return content.GetLength();
	}
}

bool HtmlEditImpl::HasContent()
{
	CString strContent;
	GetContent(strContent);

	strContent.Replace(_T("<P>&nbsp;</P>"),_T(""));

	return strContent.GetLength() > 0;
}

void HtmlEditImpl::SetBgColor(CString color)
{
	if(m_pDoc2)
	{
		m_pDoc2->put_bgColor(CComVariant(color));
	}
}

void HtmlEditImpl::InsertAtSelectionArea(CString & content)
{
	if(m_pDoc2)
	{
		CComPtr<IHTMLSelectionObject> pSelection;
		CComPtr<IHTMLTxtRange> pDisp;
		m_pDoc2->get_selection(&pSelection);
		pSelection->createRange((IDispatch**)&pDisp);
		pDisp->pasteHTML(CComBSTR(content));
	}
}

void  HtmlEditImpl::UpdateMy3()
{
	if(m_pLogic->m_Setting.m_nEditable == EDITABLE_INPUT || m_pLogic->m_Setting.m_nEditable ==EDITABLE_OUT_INPUT )
	{
		BuildEnviromentInput();
	}
	else
	{
		BuildEnviromentOutput();
	}

	//_SetFocus();
}

void  HtmlEditImpl::UpdateMy4()
{
	if(m_pLogic->m_Setting.m_nEditable == EDITABLE_INPUT || m_pLogic->m_Setting.m_nEditable == EDITABLE_OUT_INPUT )
	{
		BuildEnviromentInput();
	}
	else
	{
		BuildEnviromentOutput();
	}
}

void  HtmlEditImpl::_SetFocus()
{
	CComPtr<IHTMLDocument4> m_pCaret;
	if(m_pDoc2)
	{
		m_pDoc2->QueryInterface(IID_IHTMLDocument4, (void**) &m_pCaret);
		if ( (m_pLogic->m_Setting.m_nEditable == EDITABLE_INPUT ||m_pLogic->m_Setting.m_nEditable==EDITABLE_OUT_INPUT) && m_pCaret)
		{
			m_pCaret->focus();
			m_pCaret->focus();
		}
	}
}

void HtmlEditImpl::OnTrackMenuResult( int nreturn , CString& strName , CString& strUUID ) 
{	
	switch ( nreturn )
	{
	case HTMLEDITIMPL_EDIT_COPY_WORD:
		{
			OnEditCopyWord() ;//复制文字
		}
		break;
	case HTMLEDITIMPL_EDIT_CLEAN_SCREEN:
		{
			OnEditCleanScreen(); //清屏幕
		}
		break;
	case HTMLEDITIMPL_EDIT_PASTE:
		{
			OnEditPasteWrapper() ; //粘贴
		}
		break;
	default: 
		break ;
	}
}

//复制文字
void HtmlEditImpl::OnEditCopyWord( CString& strText )
{
	CComBSTR strCopyInfo ;
	if (m_pDoc2)
	{
		long status = this->QueryStatus(IDM_COPY) ;
		if ( status & OLECMDF_ENABLED )
		{
			CComPtr<IHTMLElement> pBody ;
			m_pDoc2->get_body( &pBody ) ;
			if ( pBody )
			{
				CComPtr<IHTMLSelectionObject> pSelect ;
				m_pDoc2->get_selection( &pSelect ) ;
				if ( pSelect )
				{
					CComPtr<IHTMLTxtRange> pRange ;
					CComPtr<IDispatch> pDispatch ;
					HRESULT hr = pSelect->createRange( (IDispatch**)&pDispatch ) ;
					if ( hr == S_OK && pDispatch )
					{
						pDispatch->QueryInterface( IID_IHTMLTxtRange , (void**)&pRange ) ;
						if ( pRange )
						{
							pRange->get_text( &strCopyInfo ) ;
							strText = strCopyInfo ;
						}
					}

				}
			}
		}
	}
}

//复制HTML代码
void HtmlEditImpl::OnEditCopyHtml( CString& strHtmlText ) 
{
	CComBSTR strCopyInfo ;
	if (m_pDoc2)
	{
		long status = this->QueryStatus(IDM_COPY) ;
		if ( status & OLECMDF_ENABLED )
		{
			CComPtr<IHTMLElement> pBody ;
			m_pDoc2->get_body( &pBody ) ;
			if ( pBody )
			{
				CComPtr<IHTMLSelectionObject> pSelect ;
				m_pDoc2->get_selection( &pSelect ) ;
				if ( pSelect )
				{
					CComPtr<IHTMLTxtRange> pRange ;
					CComPtr<IDispatch> pDispatch ;
					HRESULT hr = pSelect->createRange( (IDispatch**)&pDispatch ) ;
					if ( hr == S_OK && pDispatch )
					{
						pDispatch->QueryInterface( IID_IHTMLTxtRange , (void**)&pRange ) ;
						if ( pRange )
						{
							pRange->get_htmlText( &strCopyInfo ) ;
							strHtmlText = strCopyInfo ;
						}
					}
				}
			}
		}
	}
}

void HtmlEditImpl::OnEditPasteWrapper()
{
	m_pLogic->OnEditPaste(this);
	_SetFocus();
}

void HtmlEditImpl::OnEditCopy(CString & strHtmlText,CString & strText)
{
	CComBSTR strCopyInfo;
	CComBSTR strCopyInfo2;
	if (m_pDoc2)
	{
		long status;
		
		status = this->QueryStatus(IDM_COPY);
		
		if (status & OLECMDF_ENABLED)
		{		
			CComPtr<IHTMLElement> pBody;

			m_pDoc2->get_body(&pBody);
			if (pBody)
			{
				CComPtr<IHTMLSelectionObject> pSelect;
				m_pDoc2->get_selection(&pSelect);
				if (pSelect)
				{
					CComPtr<IHTMLElement> pElement;
					CComPtr<IHTMLTxtRange> pRange;
					CComPtr<IDispatch> pDispatch ;
					pSelect->createRange((IDispatch**)&pDispatch);
					if (pDispatch)
					{
						pDispatch->QueryInterface( IID_IHTMLTxtRange , (void**)&pRange ) ;
						if ( pRange )
						{
							pRange->get_htmlText(&strCopyInfo);
							pRange->get_text(&strCopyInfo2);

							strHtmlText = strCopyInfo;
							strHtmlText.Replace(_T("\r\n"),_T(""));
							strText = strCopyInfo2;
						}
					}
				}
			}
		}
	}
}

bool HtmlEditImpl::HasContentSelect()
{
	CComPtr<IHTMLElement> pBody;

	m_pDoc2->get_body(&pBody);
	if (pBody)
	{
		CComPtr<IHTMLSelectionObject> pSelect;
		m_pDoc2->get_selection(&pSelect);
		if (pSelect)
		{
			CComPtr<IHTMLElement> pElement;
			CComPtr<IHTMLTxtRange> pRange;
			CComPtr<IDispatch> pDispatch ;
			pSelect->createRange((IDispatch**)&pDispatch);
			if (pDispatch)
			{
				pDispatch->QueryInterface( IID_IHTMLTxtRange ,(void**)&pRange ) ;
				if ( pRange )
				{
					CComBSTR strCopyInfo;
					pRange->get_text(&strCopyInfo);
					CString strHtmlText = strCopyInfo;
					return !strHtmlText.IsEmpty();				
				}
			}
		}
	}

	return false;
}


void HtmlEditImpl::OnEditCut(CString & strHtmlText,CString & strText)
{
	if (m_pDoc2)
	{
		CComPtr<IHTMLElement> pBody;
		CComBSTR strCopyInfo;
		CComBSTR strCopyInfo2;
		m_pDoc2->get_body(&pBody);
		if (pBody)
		{
			long status;
			status = this->QueryStatus(IDM_CUT);
			if (status & OLECMDF_ENABLED)
			{
				CComPtr<IHTMLSelectionObject> pSelect;
				m_pDoc2->get_selection(&pSelect);
				if (pSelect)
				{
					CComPtr<IHTMLTxtRange> pRange;
					pSelect->createRange((IDispatch**)&pRange);
					if (pRange)
					{
						CComBSTR ss1,ss2;
						pRange->get_htmlText(&ss1);
						pRange->get_text(&ss2);
						strHtmlText = ss1;
						strText = ss2;

						strHtmlText.Replace(_T("\r\n"),_T(""));

						ExecCommand(IDM_CUT,OLECMDEXECOPT_PROMPTUSER,NULL,NULL);
					}
				}
			}
		}
	}
}

int  HtmlEditImpl::OnEditPaste(CString & strHtmlText,long type)
{
	if (m_pLogic->m_Setting.m_nEditable == EDITABLE_OUT)
	{
		return 0;
	}
	if (strHtmlText==L"")
	{
		return 0;
	}

	//修改剪切板。
	if (m_pDoc2)
	{
		CComPtr<IHTMLSelectionObject> pSelect;
		m_pDoc2->get_selection(&pSelect);
		if (pSelect)
		{
			CComPtr<IHTMLTxtRange> pRange;
			pSelect->createRange((IDispatch**)&pRange);

			if(pRange)
			{
				if(type == CF_TEXT)
				{
					pRange->put_text(CComBSTR(strHtmlText));
				}
				else
				{
					pRange->pasteHTML(CComBSTR(strHtmlText));
				}

				pRange->scrollIntoView();
				pRange->scrollIntoView();
			}
		}
	}

	return 0;
}

void HtmlEditImpl::OnProgressChange(long lProgress, long lProgressMax)
{
	UNUSED_ALWAYS(lProgress);
	UNUSED_ALWAYS(lProgressMax);
	if(lProgress == -1)
	{
		CComPtr<IHTMLDocument4> m_pCaret;
		if(m_pDoc2 != NULL)
		{
#ifdef DEBUG
			CComPtr<IHTMLElement> debug_body;
			CComBSTR ss;
			m_pDoc2->get_body(&debug_body);
			if(debug_body)
			{
				debug_body->get_innerHTML(&ss);
			}
#endif
			m_pDoc2->QueryInterface(IID_IHTMLDocument4, (void**) &m_pCaret);
			if(m_bReady == FALSE)
			{
				m_bReady = TRUE;
				UpdateMy3();

				OnOutputReady();

				if (m_pLogic->m_Setting.m_nEditable == EDITABLE_INPUT || m_pLogic->m_Setting.m_nEditable == EDITABLE_OUT_INPUT )
				{
					
					if(m_pCaret != NULL)
					{
						m_pCaret->focus();
						m_pCaret->focus();
					}

				}
				else
				{
					
				}
			}
			if (m_pLogic->m_Setting.m_nEditable == EDITABLE_INPUT || m_pLogic->m_Setting.m_nEditable == EDITABLE_OUT_INPUT )
			{
				if(m_pCaret != NULL)
				{
					m_pCaret->focus();
					m_pCaret->focus();
				}
			}
		}
	}
}

std::map<HWND,CWnd*> HtmlEditImpl::m_staTable ;

LRESULT CALLBACK IESubClassProc( HWND hwnd , UINT uMsg , WPARAM wParam , LPARAM lParam )
{
	std::map<HWND,CWnd*>::iterator iter = HtmlEditImpl::m_staTable.find( hwnd ) ;
	if ( iter != HtmlEditImpl::m_staTable.end() )
	{
		HtmlEditImpl* pHtmlEdltImpl =(HtmlEditImpl*) iter->second ;
		if ( uMsg == WM_ERASEBKGND )
		{
			return TRUE ;
		}
		WNDPROC oldProc = pHtmlEdltImpl->m_pOrgIEProc;
		return CallWindowProc( oldProc , hwnd ,uMsg , wParam , lParam ) ;
	}
	return TRUE ;	
}

void HtmlEditImpl::OnNavigateComplete2(LPDISPATCH pDisp, VARIANT FAR* URL)
{
	if (m_bReady == false)
	{
		bool editable;
		if(m_pLogic->m_Setting.m_nEditable == EDITABLE_INPUT || m_pLogic->m_Setting.m_nEditable == EDITABLE_OUT_INPUT )
		{
			editable = true;
		}
		else
		{
			editable = false;
		}

		this->SetDesignMode(editable);
	
		this->GetDHtmlDocument(&m_pDoc2);
		if(m_pDoc2!=NULL)
		{
			AfxConnectionAdvise( 
				m_pDoc2,  
				DIID_HTMLDocumentEvents2,
				GetIDispatch(FALSE),
				FALSE,  
				&m_dwCookie);
		}
	}

	ExecHelperSetVal(IDM_AUTOURLDETECT_MODE, false, 0);
	if(m_pOrgIEProc ==NULL)
	{
		TCHAR class_name[MAX_PATH+1];
		bool found;
		found=false;
		CWnd * child;
		for(child=GetWindow(GW_CHILD);child!=NULL;child=child->GetWindow(GW_HWNDNEXT))
		{
			CWnd * grand_child;
			for(grand_child=child->GetWindow(GW_CHILD);grand_child!=NULL;grand_child=grand_child->GetWindow(GW_HWNDNEXT))
			{
				CWnd * great_grand_child;
				for(great_grand_child=grand_child->GetWindow(GW_CHILD);great_grand_child!=NULL;great_grand_child=great_grand_child->GetWindow(GW_HWNDNEXT))
				{
					GetClassName(great_grand_child->GetSafeHwnd(),class_name,MAX_PATH);
					if(class_name==CString(L"Internet Explorer_Server"))
					{
						m_staTable[great_grand_child->GetSafeHwnd()] = this ;
						found=true;
						m_pOrgIEProc = (WNDPROC)SetWindowLong(great_grand_child->GetSafeHwnd(),GWL_WNDPROC,(LONG)IESubClassProc);
						break;
					}
				}
				if(found)
					break;
			}
			if(found)
				break;
		}
	}
	return;
}

void HtmlEditImpl::OnHtmlDownloadComplete()
{
	if (m_bIsNeedGotoEnd)
	{
		GotoEnd();
	}
}

LPCTSTR HtmlEditImpl::GetStartDocument()
{
	if(m_pLogic->m_Setting.m_nEditable == EDITABLE_INPUT)
	{
		m_pLogic->m_Setting.m_strDocument = GetURLFromResource(_T("in.html"));
	}
	else if ( m_pLogic->m_Setting.m_nEditable == EDITABLE_OUT_INPUT)
	{
		m_pLogic->m_Setting.m_strDocument = GetURLFromResource(_T("out.html"));
	}
	else if(m_pLogic->m_Setting.m_nEditable == EDITABLE_OUT)
	{
		m_pLogic->m_Setting.m_strDocument = GetURLFromResource(_T("out.html"));
	}
	
	return m_pLogic->m_Setting.m_strDocument;
}

BOOL HtmlEditImpl::OnKeyDown(IHTMLEventObj *pEvtObj)
{
    if(pEvtObj)
	{
		long key=0;
		pEvtObj->get_keyCode(&key);
		return m_pLogic->OnKeyDown(this, key);
	}
	return TRUE;		
}

BOOL HtmlEditImpl::onClick(IHTMLEventObj *pEvtObj)
{
	return m_pLogic->OnClick(this, pEvtObj);
}

BOOL HtmlEditImpl::OnMouseOut(IHTMLEventObj *pEvtObj) 
{
	CRect CWindowRect ;
	GetClientRect( &CWindowRect ) ;
	ClientToScreen( &CWindowRect ) ;
	return m_pLogic->OnMouseOut( this , pEvtObj ,CWindowRect ) ;
}

BOOL HtmlEditImpl::OnMouseMove(IHTMLEventObj *pEvtObj)
{
	CRect CWindowRect ;
	GetClientRect( &CWindowRect ) ;
	ClientToScreen( &CWindowRect ) ;
	return m_pLogic->OnMouseMove( this , pEvtObj , CWindowRect ) ;
}

BOOL HtmlEditImpl::OnControlSelect(IHTMLEventObj *pEvtObj)
{
	CComPtr<IHTMLElement> pElem;

	pEvtObj->get_srcElement(&pElem);
	if(pElem)
	{
		CComBSTR tag;
		pElem->get_tagName(&tag);
		if(tag == _T("IMG") || tag == _T("img"))
		{

			CComPtr<IHTMLSelectionObject> pSelect;
			m_pDoc2->get_selection(&pSelect);
			if (pSelect)
			{
				CComPtr<IHTMLElement> pElement;
				CComPtr<IHTMLTxtRange> pRange;
				pSelect->createRange((IDispatch**)&pRange);
				if (pRange)
				{
					pRange->moveToElementText(pElem);
					pRange->select();
				}
			}
		}
	}
	return FALSE ;
}

BOOL HtmlEditImpl::OnHtmlFocusIn(IHTMLEventObj *pEvtObj)
{
	//让子类处理
	return TRUE;
}

BOOL HtmlEditImpl::OnHtmlFocusOut(IHTMLEventObj *pEvtObj)
{
	//让子类处理
	return TRUE;
}


void  HtmlEditImpl::UpdateMy2(CMapStringToString & font )
{
	CComPtr<IHTMLElement> pBody;
	if (m_pDoc2)
	{
		if(FAILED(m_pDoc2->get_body(&pBody)))
			return;

		CComBSTR test;
		CComBSTR test2;
		CComBSTR test3;
		pBody->get_outerHTML(&test);
		pBody->get_innerHTML(&test2);
		pBody->get_innerText(&test3);

		CComPtr<IHTMLElement> pfont;
		GetHtmlElement(_T("fonttag"),&pfont);
		if (pfont != NULL)
		{
			CComVariant va(font[_T("face")]);
			CComVariant vb(font[_T("size")]);
			CComVariant vc(font[_T("color")]);
			pfont->setAttribute(CComBSTR(_T("face")),va,1);
			pfont->setAttribute(CComBSTR(_T("size")),vb,1);
			pfont->setAttribute(CComBSTR(_T("color")),vc,1);
		}
		pBody->get_outerHTML(&test);
		pBody->get_innerHTML(&test2);
		pBody->get_innerText(&test3);
		return ;
	}
}

void HtmlEditImpl::GetHtmlElement(CString id,IHTMLElement ** pElem2)
{
#ifdef DEBUG
	CComPtr<IHTMLElement> debug_body;
	CComBSTR ss;
	if(m_pDoc2)
	{
		m_pDoc2->get_body(&debug_body);
	}

	if(debug_body)
	{
		debug_body->get_innerHTML(&ss);
	}
#endif
	HRESULT hr = S_OK;
	CComPtr<IHTMLElementCollection> pElemColl;
	if (m_pDoc2 == NULL)
	{
		*pElem2 = NULL;
		return ;
	}
	hr =m_pDoc2->get_all(&pElemColl);
	CComPtr<IHTMLElement> pElem;
	CComVariant varID(id);
	CComVariant varIdx(0);
	CComPtr<IDispatch> pElemDisp;

	//如果document中有对应的item,取IDispatch接口的指针,赋值到pElemDisp.
	if (pElemColl)
	{
		hr = pElemColl->item(varID, varIdx, &pElemDisp);
		if(pElemDisp != NULL)
		{
			hr = pElemDisp->QueryInterface(IID_IHTMLElement, (void**)&pElem);
			if (pElem)
			{
				*pElem2 = pElem;
				(*pElem2)->AddRef();
			}
			else
			{
				*pElem2 = NULL;
			}
		}
	}
}

int HtmlEditImpl::OnCreate(LPCREATESTRUCT p)
{
	CHtmlEditView::OnCreate(p);
	m_DropTarget.Register(this);
	SetClassLong(m_hWnd, GCL_STYLE, GetClassLong(m_hWnd, GCL_STYLE) & ~CS_HREDRAW & ~CS_VREDRAW);	
	ATLASSERT(m_hWnd != NULL);

	return 0;
}

void HtmlEditImpl::InsertText(CString text)
{
	InsertDivText(text, _T("display"));
}

void HtmlEditImpl::InsertDivText(CString text, CString div)
{
	if(m_pDoc2 != NULL)
	{
		CComPtr<IHTMLElement> p;
		GetHtmlElement(div, &p);

		if(p != NULL)
		{
			p->insertAdjacentHTML(CComBSTR(_T("beforeEnd")),CComBSTR(text));
		}
		if ( m_bIsNeedGotoEnd )
		{
			GotoEnd() ;
		}
	}
}

void HtmlEditImpl::InsertTextDiv(CString s)
{
	CComPtr<IHTMLElement> p;
	if (m_pDoc2)
	{
		GetHtmlElement(_T("THEONE"),&p);
		if (p)
		{
			CComBSTR strTemp;
			p->get_innerHTML(&strTemp);
			if(strTemp.Length() > 0)// 当不加此判断时，ie会put一串伟大的"&nbsp;"进去 2009-3-11
			{
				p->put_innerHTML(CComBSTR(s));
			}
		}
		else
		{
			CComPtr<IHTMLElement> pBody;
			m_pDoc2->get_body(&pBody);
			if (pBody)
			{
				pBody->insertAdjacentHTML(CComBSTR(_T("beforeEnd")),CComBSTR(_T("<div id = \"THEONE\" ></div>")));
				InsertTextDiv(s);
			}
		}
	}
}

void HtmlEditImpl::OnDestroy()
{
	if (m_pBrowserApp) {
		m_pBrowserApp = NULL;
	}
	CWnd::OnDestroy(); 
}

BOOL HtmlEditImpl::IsClipbordAvailable()
{
	COleDataObject obj;

	if (obj.AttachClipboard()) 
	{
		if(obj.IsDataAvailable(CF_UNICODETEXT))
		{
			return TRUE;
		}
		else if(obj.IsDataAvailable(CF_TEXT))
		{
			return TRUE;
		}		
		else
		{
			return obj.IsDataAvailable(CF_DIB);
		}
	}

	return FALSE;
}

HRESULT HtmlEditImpl::OnShowContextMenu(DWORD dwID, LPPOINT ppt, LPUNKNOWN pcmdtReserved,	LPDISPATCH pdispReserved)
{
	if(!m_pLogic->m_Setting.m_bEnableContextMenu)
	{
		return S_OK;
	}

	CString strName ;
	CString strUUID ;
	CMenu* pMenu = NULL ;

	CComPtr<IHTMLElement> pElem;
	pdispReserved->QueryInterface(IID_IHTMLElement, (void**)&pElem);

	if(HasContentSelect())
	{
		//有文字被选中
		pMenu = &m_pLogic->m_Setting.m_menuText ;
		if ( m_pLogic->m_Setting.m_nEditable == EDITABLE_OUT )
		{
			//输出框,禁止粘贴
			pMenu->EnableMenuItem( HTMLEDITIMPL_EDIT_PASTE , MF_GRAYED ) ;
		}
	}
	else
	{
		pMenu = &m_pLogic->m_Setting.m_menuText ;
		if ( m_pLogic->m_Setting.m_nEditable == EDITABLE_OUT )
		{			
			//输出框
			if ( pElem)
			{
				CComBSTR tag ;
				pElem->get_tagName(&tag) ;

				if ( tag==_T("A") || tag==_T("a") )
				{
					strName = GetValue( pElem , _T("name") ) ;
					strUUID = GetValue( pElem , _T("uuid") ) ;
					if ( !strName.IsEmpty() && !strName.IsEmpty() && strName != _T("您") && strName != _T("所有人") )
					{
						m_pLogic->OnTrackHrefLinkMenu( _wtol( strUUID ) ) ;
						return S_OK ;
					}
				}
			}			
		}
	}

	if ( pMenu != NULL )
	{
		int nreturn = pMenu->TrackPopupMenu( TPM_LEFTALIGN | TPM_RIGHTBUTTON |TPM_RETURNCMD, ppt->x, 
			ppt->y, this) ;
		OnTrackMenuResult( nreturn , strName , strUUID ) ;
	}

	return S_OK ;
}

int HtmlEditImpl::GetFontColor()
{
	return 65536*GetRValue(m_pLogic->m_Setting.m_stChatFont.font_color) + 
		256*GetGValue(m_pLogic->m_Setting.m_stChatFont.font_color) + GetBValue(m_pLogic->m_Setting.m_stChatFont.font_color);
}

void HtmlEditImpl::ConstructScrollColor()
{

	//m_pLogic->m_Setting.m_strVcolorScroll       = _T("#7DA6D7");
	//m_pLogic->m_Setting.m_strVcolorScrollBorder = _T("#3465A4");
	//m_pLogic->m_Setting.m_strVcolorScrollArrow  = _T("#0141C7");

	m_pLogic->m_Setting.m_strVcolorScrollTrackColor = _T("E6E6E6");
	m_pLogic->m_Setting.m_strVcolorScroll3dLightColor = _T("4A85BD");
	m_pLogic->m_Setting.m_strVcolorScrollArrowColor = _T("3465A4");
	m_pLogic->m_Setting.m_strVcolorScrollShadowColor = _T("E7EFF7");
	m_pLogic->m_Setting.m_strVcolorScrollFaceColor = _T("7DA6D7");
	m_pLogic->m_Setting.m_strVcolorScrollHighlightColor = _T("E7EFF7");
	m_pLogic->m_Setting.m_strVcolorScrollDarkShadowColor = _T("3465A4");
}

void HtmlEditImpl::BuildEnviromentInput()
{
	USES_CONVERSION;

	ConstructScrollColor();

	CComPtr<IHTMLElement> pInsertPlace;
	CComPtr<IHTMLElement> pfont;
	CComPtr<IHTMLElement> pBody,pBody2;
	CComBSTR test;
	CComBSTR test2;
	CComBSTR test3;

	if(m_pDoc2 == NULL) return ;

	m_pDoc2->get_body(&pBody2);

	CString strFontColor;
	strFontColor.Format(_T("%06x"), GetFontColor());

	CComVariant vcolorlink(strFontColor);
	m_pDoc2->put_alinkColor(vcolorlink);
	m_pDoc2->put_linkColor(vcolorlink);
	m_pDoc2->put_vlinkColor(vcolorlink);

	if (pBody2 != NULL)
	{
		/*CComVariant vcolorScroll(m_pLogic->m_Setting.m_strVcolorScroll);
		CComVariant vcolorScrollBorder(m_pLogic->m_Setting.m_strVcolorScrollBorder);
		CComVariant vcolorScrollArrow(m_pLogic->m_Setting.m_strVcolorScrollArrow);*/

		CComVariant VcolorScrollTrackColor(m_pLogic->m_Setting.m_strVcolorScrollTrackColor);
		CComVariant VcolorScroll3dLightColor(m_pLogic->m_Setting.m_strVcolorScroll3dLightColor);
		CComVariant VcolorScrollArrowColor(m_pLogic->m_Setting.m_strVcolorScrollArrowColor);
		CComVariant VcolorScrollShadowColor(m_pLogic->m_Setting.m_strVcolorScrollShadowColor);
		CComVariant VcolorScrollFaceColor(m_pLogic->m_Setting.m_strVcolorScrollFaceColor);
		CComVariant VcolorScrollHighlightColor(m_pLogic->m_Setting.m_strVcolorScrollHighlightColor);
		CComVariant VcolorScrollDarkShadowColor(m_pLogic->m_Setting.m_strVcolorScrollDarkShadowColor);
	
		CComPtr<IHTMLStyle3> pBodyStyle3;
		CComPtr<IHTMLStyle> pBodyStyle;

		pBody2->get_style(&pBodyStyle);
		pBodyStyle->QueryInterface(IID_IHTMLStyle3,(void**)&pBodyStyle3);

		if(pBodyStyle3)
		{
		/*	rex |=pBodyStyle3->put_scrollbarHighlightColor(VcolorScrollHighlightColor);
			rex |=pBodyStyle3->put_scrollbarShadowColor(VcolorScrollShadowColor);
			rex |=pBodyStyle3->put_scrollbarFaceColor(VcolorScrollFaceColor);
			rex |= pBodyStyle3->put_scrollbarDarkShadowColor(VcolorScrollDarkShadowColor);
			rex |= pBodyStyle3->put_scrollbar3dLightColor(VcolorScroll3dLightColor);
			rex |=pBodyStyle3->put_scrollbarArrowColor(VcolorScrollArrowColor);
			rex |=pBodyStyle3->put_scrollbarTrackColor(VcolorScrollTrackColor);*/
		}

		pInsertPlace = pBody2;
		CComVariant vdis;
		CComPtr<IHTMLStyle> pStyle;
		CString  temp;
		HRESULT re = pBody2->getAttribute(CComBSTR(_T("style")),0,&vdis);
		re = vdis.pdispVal->QueryInterface(IID_IHTMLStyle,(void**)&pStyle);

		pStyle->put_fontFamily(CComBSTR(m_pLogic->m_Setting.m_stChatFont.font_type.c_str()));

		CString strFontSize;
		strFontSize.Format(_T("%02dpx"), m_pLogic->m_Setting.m_stChatFont.font_size); 

		CComVariant vsize(strFontSize);

		pStyle->put_fontSize(vsize);

		CString strFontColor;
		strFontColor.Format(_T("%06x"), GetFontColor());

		CComVariant vcolor(strFontColor);
		pStyle->put_color(vcolor);

		unsigned   char style = m_pLogic->m_Setting.m_stChatFont.font_style;

		if(style & FONT_TYPE_BOLD)
		{
			pStyle->put_fontWeight(CComBSTR(_T("bold")));
		}
		else
		{
			pStyle->put_fontWeight(CComBSTR(_T("normal")));
		}
		if(style & FONT_TYPE_ITALIC)
		{
			pStyle->put_fontStyle(CComBSTR(_T("italic")));
		}
		else
		{
			pStyle->put_fontStyle(CComBSTR(_T("normal")));
		}
		if(style & FONT_TYPE_UNDERLINE)
		{
			pStyle->put_textDecorationUnderline(VARIANT_TRUE);
		}
		else
		{
			pStyle->put_textDecorationUnderline(VARIANT_FALSE);
		}
		if(style & FONT_TYPE_STRIKEOUT)
		{
			pStyle->put_textDecorationLineThrough(VARIANT_TRUE);
		}
		else
		{
			pStyle->put_textDecorationLineThrough(VARIANT_FALSE);
		}
	}

	if(m_pLogic->m_Setting.m_nEditable == EDITABLE_OUT_INPUT )
	{
		CComPtr<IHTMLCaret> pCaret;
		CComPtr<IDisplayServices> spDS;
		if(m_pDoc2)
		{
			m_pDoc2->QueryInterface(IID_IDisplayServices, (void**)&spDS);
			if(spDS)
			{
				spDS->GetCaret(&pCaret);
				if(pCaret)
				{
					pCaret->Hide();
				}
			}
		}

		if(m_pDoc2)
		{
			m_pDoc2->put_bgColor(CComVariant(CString(_T("#c6d6ef"))));
		}
	}
}

void HtmlEditImpl::BuildEnviromentOutput()
{
	ConstructScrollColor();

	CComPtr<IHTMLElement> pInsertPlace;
	CComPtr<IHTMLElement> pfont;
	CComPtr<IHTMLElement> pBody,pBody2;

	if(m_pDoc2 == NULL) return ;
	
	m_pDoc2->get_body(&pBody2);

	if(pBody2 != NULL)
	{
		CComVariant VcolorScrollTrackColor(m_pLogic->m_Setting.m_strVcolorScrollTrackColor);
		CComVariant VcolorScroll3dLightColor(m_pLogic->m_Setting.m_strVcolorScroll3dLightColor);
		CComVariant VcolorScrollArrowColor(m_pLogic->m_Setting.m_strVcolorScrollArrowColor);
		CComVariant VcolorScrollShadowColor(m_pLogic->m_Setting.m_strVcolorScrollShadowColor);
		CComVariant VcolorScrollFaceColor(m_pLogic->m_Setting.m_strVcolorScrollFaceColor);
		CComVariant VcolorScrollHighlightColor(m_pLogic->m_Setting.m_strVcolorScrollHighlightColor);
		CComVariant VcolorScrollDarkShadowColor(m_pLogic->m_Setting.m_strVcolorScrollDarkShadowColor);
		CComPtr<IHTMLStyle3> pBodyStyle3;
		CComPtr<IHTMLStyle> pBodyStyle;

		pBody2->get_style(&pBodyStyle);
		pBodyStyle->QueryInterface(IID_IHTMLStyle3,(void**)&pBodyStyle3);

		if(pBodyStyle3 && !m_pLogic->m_Setting.m_bDefaultScrollStyle)
		{
		/*	rex |=pBodyStyle3->put_scrollbarHighlightColor(VcolorScrollHighlightColor);
			rex |=pBodyStyle3->put_scrollbarShadowColor(VcolorScrollShadowColor);
			rex |=pBodyStyle3->put_scrollbarFaceColor(VcolorScrollFaceColor);
			rex |= pBodyStyle3->put_scrollbarDarkShadowColor(VcolorScrollDarkShadowColor);
			rex |= pBodyStyle3->put_scrollbar3dLightColor(VcolorScroll3dLightColor);
			rex |=pBodyStyle3->put_scrollbarArrowColor(VcolorScrollArrowColor);
			rex |=pBodyStyle3->put_scrollbarTrackColor(VcolorScrollTrackColor);*/
		}
	}
}

//复制文字
void HtmlEditImpl::OnEditCopyWord() 
{
	m_pLogic->OnEditCopyWord( this ) ;
}

//清屏
void HtmlEditImpl::OnEditCleanScreen() 
{
	ClearMsg() ;
}

LRESULT  HtmlEditImpl::OnGetFocus(WPARAM wParam, LPARAM lParam)
{
	CComPtr<IHTMLSelectionObject> pSelection;
	CComPtr<IHTMLTxtRange> pDisp;
	_SetFocus();
	CString ss;
	ss.Format(_T("%c"),wParam);
	m_pDoc2->get_selection(&pSelection);
	pSelection->createRange((IDispatch**)&pDisp);
	pDisp->pasteHTML(CComBSTR(ss));

	return 0;
}

CString HtmlEditImpl::GetValue(CComPtr<IHTMLElement> & p,CString key)
{
	if(p)
	{
		CComVariant value;
		CComBSTR valuestr;
		if ( p->getAttribute(CComBSTR(key),0,&value)== S_OK && value.vt != VT_NULL )
		{
			value.CopyTo(&valuestr);
			CString  re(valuestr);
			return re;
		}
	}

	return CString(L"");
}

BOOL HtmlEditImpl::IsReady()
{
	return m_bReady;
}


CString HtmlEditImpl::GetURLFromResource(CString nRes)
{
	HINSTANCE hInstance = AfxGetResourceHandle();
	ASSERT(hInstance != NULL);

	CString strResourceURL;	

#if 0
	CString szModule = GetModulePath().c_str();
	szModule.Replace(_T('/'),_T('\\'));
#else
	///这里采用相对路径不采用绝对路径以避免路径中出现%等时IE无法显示的问题
	CString szModule;
#endif

	common::CURLEncode urlEncode;
	szModule = (urlEncode.URLEncodeLocalPath(szModule.GetBuffer())).c_str();

#ifdef _DEBUG
	TCHAR dllname[]=_T("HtmlEditd.dll");
#else
	TCHAR dllname[]=_T("HtmlEdit.dll");
#endif

	strResourceURL.Format(_T("res://%s/%s"), dllname,nRes);

	return strResourceURL;
}

void HtmlEditImpl::OnOutputReady()
{
	m_pLogic->OnOutputReady(this);
}

void HtmlEditImpl::SetUnEditAble()
{
	m_pLogic->m_Setting.m_nEditable = EDITABLE_OUT;
	
	SetDesignMode(false);

	if(m_pDoc2)
	{
		m_pDoc2->put_bgColor(CComVariant(CString(_T("#F4F4F4"))));
	}
}

void HtmlEditImpl::SetEditAble()
{
	m_pLogic->m_Setting.m_nEditable = EDITABLE_INPUT;

	SetDesignMode(true);

	if(m_pDoc2)
	{
		m_pDoc2->put_bgColor(CComVariant(CString(_T("#FFFFFF"))));
	}
}

void HtmlEditImpl::DeleteDom(CString strParent, CString strToDel)
{
	CComQIPtr<IHTMLDocument3> doc3(m_pDoc2);
	if (!doc3)
		return;

	CComPtr<IHTMLElement> element;
	CComBSTR id(strParent.GetBuffer());
	doc3->getElementById(id, &element);	
	
	CComBSTR delId(strToDel.GetBuffer());
	BOOL bNeedUpdate = FALSE;
	while(TRUE)
	{
		CComPtr<IHTMLElement> eleChild;
		if(S_OK == doc3->getElementById(delId,&eleChild))
		{
			CComQIPtr<IHTMLDOMNode> elementNode(element);
			CComQIPtr<IHTMLDOMNode> pbRemoveNode(eleChild);

			if (eleChild)
			{
				if(S_OK == elementNode->removeChild(pbRemoveNode, NULL))
				{
					bNeedUpdate = TRUE;
				}
				else
				{
					break;
				}
			}
			else
			{
				break;
			}
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

BOOL HtmlEditImpl::CallJScript(const CString strFunc, const CStringArray& paramArray,_variant_t* pVarResult)
{
	CComPtr<IDispatch> spScript;
	HRESULT hr = m_pDoc2->get_Script(&spScript);
	ATLASSERT(SUCCEEDED(hr));

	CComBSTR bstrFunc(strFunc);
	DISPID dispid = NULL;
	hr = spScript->GetIDsOfNames(IID_NULL,&bstrFunc,1,
		LOCALE_SYSTEM_DEFAULT,&dispid);
	if(FAILED(hr))
	{
		return FALSE;
	}

	INT_PTR arraySize = paramArray.GetSize();

	DISPPARAMS dispparams;
	memset(&dispparams, 0, sizeof dispparams);
	dispparams.cArgs = (UINT)arraySize;
	dispparams.rgvarg = new VARIANT[dispparams.cArgs];

	for( int i = 0; i < arraySize; i++)
	{
		CComBSTR bstr = paramArray.GetAt(arraySize - 1 - i); // back reading
		bstr.CopyTo(&dispparams.rgvarg[i].bstrVal);
		dispparams.rgvarg[i].vt = VT_BSTR;
	}
	dispparams.cNamedArgs = 0;

	EXCEPINFO excepInfo;
	memset(&excepInfo, 0, sizeof excepInfo);
	_variant_t vaResult;
	UINT nArgErr = (UINT)-1;  // initialize to invalid arg

	hr = spScript->Invoke(dispid,IID_NULL,0,
		DISPATCH_METHOD,&dispparams,&vaResult,&excepInfo,&nArgErr);

	for( int j = 0; j < arraySize; j++)
		::SysFreeString(dispparams.rgvarg[j].bstrVal);
	delete [] dispparams.rgvarg;
	if(FAILED(hr))
	{
		return FALSE;
	}

	if(pVarResult)
	{
		*pVarResult = vaResult;
	}
	return TRUE;
}


HRESULT HtmlEditImpl::OnGetDropTarget(LPDROPTARGET pDropTarget, LPDROPTARGET* ppDropTarget )
{
	LPDROPTARGET pMyDropTarget;
	pMyDropTarget = (LPDROPTARGET)m_DropTarget.GetInterface(&IID_IDropTarget);
	if(pMyDropTarget)
	{
		*ppDropTarget = pMyDropTarget;
		pMyDropTarget->AddRef();
		return S_OK;
	}

	return S_FALSE;
}

DROPEFFECT HtmlEditImpl::OnDragEnter(COleDataObject* pDataObject, DWORD dwKeyState, CPoint point)
{
	return DROPEFFECT_NONE ;
}

DROPEFFECT HtmlEditImpl::OnDragOver(COleDataObject* pDataObject, DWORD dwKeyState, CPoint point)
{
	return DROPEFFECT_NONE ;
}


BOOL HtmlEditImpl::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	return TRUE ;
	return CHtmlEditView::OnEraseBkgnd(pDC);
}

BOOL HtmlEditImpl::PreTranslateMessage( MSG* pMsg ) 
{
	if ( pMsg->message == WM_KEYDOWN)
	{
		switch(pMsg->wParam)
		{
		case VK_F5:  //屏蔽F5 键
			return TRUE ;
		case 0x46:	//CTRL + F
		case 0x4E:  //CTRL + N
		case 0x4F:  //CTRL + O
		case 0x4C:	//CTRL + L
			if ( GetKeyState(VK_CONTROL)&0x8000 ) 
			{
				return TRUE ;
			}
			break;
		default: break;
		}	

	}
	else if ( pMsg->message == WM_SYSKEYDOWN )
	{
		if ( pMsg->wParam == VK_BACK &&  ( GetKeyState(VK_MENU)&0x8000 ) ) //屏蔽ALT BACKSPACE
		{
			return TRUE ;
		}
	}
	return CHtmlEditView::PreTranslateMessage(pMsg);
}
