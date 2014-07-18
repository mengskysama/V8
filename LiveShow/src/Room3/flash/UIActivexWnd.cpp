/********************************************************************
	created:	
	created:
	file base:
	file ext:	cpp
	author:		Garret Lian
	
	purpose:	
*********************************************************************/
#include "StdAfx.h"
#include "UIActiveXWnd.h"
#include "UIActiveXBase.h"
#include "UIActiveX.h"


//////////////////////////////////////////////////////////////////////////
//CActiveXWnd
//////////////////////////////////////////////////////////////////////////
HWND CActiveXWnd::Init(CActiveXCtrl* pOwner, HWND hWndParent, int width, int height)
{
   m_pOwner = pOwner;

   CRect rc(0,0,width,height);
  
   Create(AfxRegisterWndClass(0), _T("UIActiveX"), WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN | WS_POPUP,//| WS_CLIPSIBLINGS,
	   rc, CWnd::FromHandle(hWndParent), 0);

   SetWindowLong(m_hWnd, GWL_EXSTYLE, (GetWindowLong(m_hWnd, GWL_EXSTYLE)|WS_EX_LAYERED)|WS_EX_TOOLWINDOW);
   
   return m_hWnd;
}

LPCTSTR CActiveXWnd::GetWindowClassName() const
{
   return _T("ActiveXWnd");
}

void CActiveXWnd::OnFinalMessage(HWND hWnd)
{	
	if(m_pOwner)
		m_pOwner->SetActiveXWnd(NULL);
	delete this;
}

void CActiveXWnd::DoVerb(LONG iVerb)
{
   if( m_pOwner == NULL ) 
	   return;

   IOleObject* pUnk = NULL;
   m_pOwner->GetControl(IID_IOleObject, (LPVOID*) &pUnk);
   
   if( pUnk == NULL ) 
	   return;

   CUIAutoPtr<IOleObject> RefOleObject = pUnk;
   IOleClientSite* pOleClientSite = NULL;

   m_pOwner->QueryInterface(IID_IOleClientSite, (LPVOID*) &pOleClientSite);
   
   CUIAutoPtr<IOleClientSite> RefOleClientSite = pOleClientSite;
   
   pUnk->DoVerb(iVerb, NULL, pOleClientSite, 0, m_hWnd, &m_pOwner->GetPos());
}

LRESULT CActiveXWnd::HandleMessage( UINT uMsg, WPARAM wParam, LPARAM lParam, bool &bHandled )
{
   LRESULT lRes = 0;
   bHandled = true;

   switch( uMsg )
   {
   case WM_PAINT:         
	   lRes = OnPaint(uMsg, wParam, lParam, bHandled); 
	   break;
   case WM_SETFOCUS:      
	   lRes = OnSetFocus(uMsg, wParam, lParam, bHandled); 
	   break;
   case WM_KILLFOCUS:     
	   lRes = OnKillFocus(uMsg, wParam, lParam, bHandled);
	   break;
   case WM_ERASEBKGND:    
	   lRes = OnEraseBkgnd(uMsg, wParam, lParam, bHandled); 
	   break;
   case WM_MOUSEACTIVATE: 
	   lRes = OnMouseActivate(uMsg, wParam, lParam, bHandled);
	   break;
   default:
	  bHandled = FALSE;
   }
  
   return lRes;
}

LRESULT CActiveXWnd::OnEraseBkgnd(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled)
{
    if( !m_pOwner->IsWindowless() ) 
 	   bHandled = FALSE;
	return 1;
}

LRESULT CActiveXWnd::OnMouseActivate(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled)
{
   IOleObject* pUnk = NULL;
   m_pOwner->GetControl(IID_IOleObject, (LPVOID*) &pUnk);

   if( pUnk == NULL ) 
	   return 0;

   CUIAutoPtr<IOleObject> RefOleObject = pUnk;
   DWORD dwMiscStatus = 0;

   pUnk->GetMiscStatus(DVASPECT_CONTENT, &dwMiscStatus);

   if( (dwMiscStatus & OLEMISC_NOUIACTIVATE) != 0 ) 
	   return 0;

   if( !m_pOwner->IsInplaceActivated() ) 
	   DoVerb(OLEIVERB_INPLACEACTIVATE);

   bHandled = FALSE;

   return 0;
}

LRESULT CActiveXWnd::OnSetFocus(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled)
{
   bHandled = FALSE;

   if( !m_pOwner->IsUIActivated() )
	   DoVerb(OLEIVERB_UIACTIVATE);
   return 0;
}

LRESULT CActiveXWnd::OnKillFocus(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled)
{
   bHandled = FALSE;
   m_pOwner->SetFocus(FALSE);
   return 0;
}

LRESULT CActiveXWnd::OnPaint(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled)
{
   PAINTSTRUCT ps = { 0 };
   ::BeginPaint(m_hWnd, &ps);
   ::EndPaint(m_hWnd, &ps);
   return 1;
}

