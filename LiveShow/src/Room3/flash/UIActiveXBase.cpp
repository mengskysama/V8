/********************************************************************
created:	2006/04/12
created:	12:4:2006   16:41
file base:	UIString
file ext:	cpp
author:		Garret Lian

  purpose:	
*********************************************************************/
#include "stdafx.h"
#include <atlbase.h>
#include <atlwin.h>
#include "UIActiveXBase.h"
#include "UIActiveXWnd.h"
#include "UIEnumUnknown.h"
#include "UIActiveXFrameWnd.h"
#include "UIApi.h"
#include "assert.h"

#define UITRACE(...)

//////////////////////////////////////////////////////////////////////////
CActiveXCtrl::CActiveXCtrl() : 
m_dwRef(1), 
m_pObject(NULL),
m_hWndHost(NULL),
m_pOwner(NULL), 
m_pWindow(NULL),
m_pUnkSite(NULL), 
m_pViewObject(NULL),
m_pInPlaceObject(NULL),
m_bLocked(false), 
m_bFocused(false),
m_bCaptured(false),
m_bWindowless(true),
m_bUIActivated(false),
m_bInPlaceActive(false)
{
	ZeroMemory(&m_rcPos, sizeof(m_rcPos));
}

CActiveXCtrl::~CActiveXCtrl()
{
	if( m_pWindow != NULL ) {
		::DestroyWindow(*m_pWindow);
		m_pWindow = NULL;
	}

	MyRelease( m_pUnkSite );
	m_pUnkSite = NULL;
	MyRelease( m_pViewObject );
	m_pViewObject = NULL;
	MyRelease( m_pInPlaceObject );
	m_pInPlaceObject = NULL;
	MyRelease( m_pObject );
	m_pObject = NULL;
}

void CActiveXCtrl::MyRelease(IUnknown* pObject)
{
	if(pObject)
		pObject->Release();
}

//////////////////////////////////////////////////////////////////////////
STDMETHODIMP_(ULONG) CActiveXCtrl::AddRef() 
{
	ULONG temp = InterlockedIncrement(&m_dwRef);
	UITRACE("AX: CActiveXCtrl::AddRef %d", temp);	
	return temp;
}

STDMETHODIMP_(ULONG) CActiveXCtrl::Release()
{
	ULONG temp = InterlockedDecrement(&m_dwRef);	
	if( temp == 0 ) 
		delete this;	
	UITRACE("AX: CActiveXCtrl::Release %d", temp);	
	return temp;
}

STDMETHODIMP CActiveXCtrl::QueryInterface(REFIID riid, LPVOID *ppvObject)
{
	UITRACE("AX: CActiveXCtrl::QueryInterface %s", DirectUI::GetIIDString(riid));

	*ppvObject = NULL;
		
	if( riid == IID_IUnknown )
		*ppvObject = static_cast<IOleControlSite*>(this);//IIUnknown
	else if( riid == IID_IObjectWithSite ) 
		*ppvObject = static_cast<IObjectWithSite*>(this);//public IObjectWithSite
	else if( riid == IID_IOleClientSite )
		*ppvObject = static_cast<IOleClientSite*>(this);//public IOleClientSite
	else if( riid == IID_IOleControlSite )
		*ppvObject = static_cast<IOleControlSite*>(this);//public IOleControlSite
	else if( riid == IID_IOleInPlaceSiteWindowless )
		*ppvObject = static_cast<IOleInPlaceSiteWindowless*>(this);//public IOleInPlaceSiteWindowless
	else if( riid == IID_IOleInPlaceSiteEx )
		*ppvObject = static_cast<IOleInPlaceSiteEx*>(this);
	else if( riid == IID_IOleInPlaceSite )
		*ppvObject = static_cast<IOleInPlaceSite*>(this);
	else if( riid == IID_IOleWindow ) 
		*ppvObject = static_cast<IOleInPlaceSiteWindowless*>(this);
	else if( riid == IID_IOleContainer )
		*ppvObject = static_cast<IOleContainer*>(this);//public IOleContainer
	else if( riid == IID_IParseDisplayName )
		*ppvObject = static_cast<IParseDisplayName*>(this);
	else if( riid == IID_IBindHost )
		*ppvObject = static_cast<IBindHost*>(this);//public IBindHost

	if( *ppvObject != NULL ) 
		AddRef();

	return *ppvObject == NULL ? E_NOINTERFACE : S_OK;
}


//////////////////////////////////////////////////////////////////////////
//IObjectWithSite
STDMETHODIMP CActiveXCtrl::SetSite(IUnknown *pUnkSite)
{
	UITRACE("AX: CActiveXCtrl::SetSite");	
	MyRelease( m_pUnkSite );	
	if( pUnkSite != NULL ) {
		m_pUnkSite = pUnkSite;
		m_pUnkSite->AddRef();
	}	
	return S_OK;
}

STDMETHODIMP CActiveXCtrl::GetSite(REFIID riid, LPVOID* ppvSite)
{
	UITRACE("AX: CActiveXCtrl::GetSite");
	if( ppvSite == NULL ) 
		return E_POINTER;	
	*ppvSite = NULL;	
	if( m_pUnkSite == NULL )
		return E_FAIL;	
	return m_pUnkSite->QueryInterface(riid, ppvSite);
}


//////////////////////////////////////////////////////////////////////////
//IOleClientSite
STDMETHODIMP CActiveXCtrl::SaveObject(void)
{
	UITRACE("AX: CActiveXCtrl::SaveObject");
	return E_NOTIMPL;
}

STDMETHODIMP CActiveXCtrl::GetMoniker(DWORD dwAssign, DWORD dwWhichMoniker, IMoniker** ppmk)
{
	UITRACE("AX: CActiveXCtrl::GetMoniker");	
	if( ppmk != NULL )
		*ppmk = NULL;	
	return E_NOTIMPL;
}

STDMETHODIMP CActiveXCtrl::GetContainer(IOleContainer** ppContainer)
{
	UITRACE("AX: CActiveXCtrl::GetContainer");	
	if( ppContainer == NULL )
		return E_POINTER;	
	*ppContainer = NULL;	
	HRESULT hr = E_NOTIMPL;	
	if( m_pUnkSite != NULL )
		hr = m_pUnkSite->QueryInterface(IID_IOleContainer, (LPVOID*) ppContainer);	
	if( FAILED(hr) ) 
		hr = QueryInterface(IID_IOleContainer, (LPVOID*) ppContainer);	
	return hr;
}

STDMETHODIMP CActiveXCtrl::ShowObject(void)
{
	UITRACE("AX: CActiveXCtrl::ShowObject");	
 	HDC hDC = ::GetDC(m_hWndHost); 	
 	if( hDC == NULL )
 		return E_FAIL; 	
 	if( m_pViewObject != NULL ) 
 		m_pViewObject->Draw(DVASPECT_CONTENT, -1, NULL, NULL, NULL, hDC, (RECTL*) &m_rcPos, (RECTL*) &m_rcPos, NULL, NULL); 	
 	::ReleaseDC(m_hWndHost, hDC);	
	return S_OK;
}

STDMETHODIMP CActiveXCtrl::OnShowWindow(BOOL fShow)
{
	UITRACE("AX: CActiveXCtrl::OnShowWindow");
	return E_NOTIMPL;
}

STDMETHODIMP CActiveXCtrl::RequestNewObjectLayout(void)
{
	UITRACE("AX: CActiveXCtrl::RequestNewObjectLayout");
	return E_NOTIMPL;
}


//////////////////////////////////////////////////////////////////////////
//IOleControlSite
STDMETHODIMP CActiveXCtrl::OnControlInfoChanged(void)      
{
	UITRACE("AX: CActiveXCtrl::OnControlInfoChanged");
	return S_OK;
}

STDMETHODIMP CActiveXCtrl::LockInPlaceActive(BOOL fLock)       
{
	UITRACE("AX: CActiveXCtrl::LockInPlaceActive");
	return S_OK;
}

STDMETHODIMP CActiveXCtrl::GetExtendedControl(IDispatch** ppDisp)        
{
	UITRACE("AX: CActiveXCtrl::GetExtendedControl");

	if( ppDisp == NULL ) 
		return E_POINTER;   

	if( m_pObject == NULL )
		return E_UNEXPECTED;

	return m_pObject->QueryInterface(IID_IDispatch, (LPVOID*) ppDisp);
}

STDMETHODIMP CActiveXCtrl::TransformCoords(POINTL* pPtlHimetric, POINTF* pPtfContainer, DWORD dwFlags)       
{
	UITRACE("AX: CActiveXCtrl::TransformCoords");
	return S_OK;
}

STDMETHODIMP CActiveXCtrl::TranslateAccelerator(MSG *pMsg, DWORD grfModifiers)
{
	UITRACE("AX: CActiveXCtrl::TranslateAccelerator");
	return S_FALSE;
}

STDMETHODIMP CActiveXCtrl::OnFocus(BOOL fGotFocus)
{
	UITRACE("AX: CActiveXCtrl::OnFocus");
	m_bFocused = (fGotFocus == TRUE);
	return S_OK;
}

STDMETHODIMP CActiveXCtrl::ShowPropertyFrame(void)
{
	UITRACE("AX: CActiveXCtrl::ShowPropertyFrame");
	return E_NOTIMPL;
}


//////////////////////////////////////////////////////////////////////////
//IOleInPlaceSiteWindowless
STDMETHODIMP CActiveXCtrl::CanWindowlessActivate(void)
{
	UITRACE("AX: CActiveXCtrl::CanWindowlessActivate");
	return S_OK;  // Yes, we can!!
}

STDMETHODIMP CActiveXCtrl::GetCapture(void)
{
	UITRACE("AX: CActiveXCtrl::GetCapture");	
 	if( m_pOwner == NULL ) 
 		return E_UNEXPECTED;	
	return m_bCaptured ? S_OK : S_FALSE;
}

STDMETHODIMP CActiveXCtrl::SetCapture(BOOL fCapture)
{
	UITRACE("AX: CActiveXCtrl::SetCapture");
	
 	if( m_pOwner == NULL ) 
 		return E_UNEXPECTED;	
	m_bCaptured = (fCapture == TRUE);	
	if( fCapture ) 
		::SetCapture(m_hWndHost);
	else 
		::ReleaseCapture();	
	return S_OK;
}

STDMETHODIMP CActiveXCtrl::GetFocus(void)
{
	UITRACE("AX: CActiveXCtrl::GetFocus");	
 	if( m_pOwner == NULL ) 
 		return E_UNEXPECTED;	
	return m_bFocused ? S_OK : S_FALSE;
}

STDMETHODIMP CActiveXCtrl::SetFocus(BOOL fFocus)
{
	UITRACE("AX: CActiveXCtrl::SetFocus");	
 	if( m_pOwner == NULL )
 		return E_UNEXPECTED;
	if(m_pOwner)
		m_pOwner->AxSetFocus(fFocus == TRUE);	
	m_bFocused = (fFocus == TRUE);	
	return S_OK;
}

STDMETHODIMP CActiveXCtrl::GetDC(LPCRECT pRect, DWORD grfFlags, HDC* phDC)
{
	UITRACE("AX: CActiveXCtrl::GetDC");	
	if( phDC == NULL ) 
		return E_POINTER;	
 	if( m_pOwner == NULL ) 
 		return E_UNEXPECTED;	
	*phDC = ::GetDC(m_hWndHost);	
	if( (grfFlags & OLEDC_PAINTBKGND) != 0 ) {
		CRect rcItem = m_rcPos;		
		if( !m_bWindowless ) 
		{
			rcItem.right = rcItem.Width();
			rcItem.bottom = rcItem.Height();
			rcItem.left=0;
			rcItem.top=0;
		}
		::FillRect(*phDC, &rcItem, (HBRUSH) (COLOR_WINDOW + 1));
	}	
	return S_OK;
}

STDMETHODIMP CActiveXCtrl::ReleaseDC(HDC hDC)
{
	UITRACE("AX: CActiveXCtrl::ReleaseDC");		
	::ReleaseDC(m_hWndHost, hDC);	
	return S_OK;
}

STDMETHODIMP CActiveXCtrl::InvalidateRect(LPCRECT pRect, BOOL fErase)
{
	UITRACE("AX: CActiveXCtrl::InvalidateRect");	
	if( m_hWndHost == NULL ) 
		return E_FAIL;	
	return ::InvalidateRect(m_hWndHost, pRect, fErase) ? S_OK : E_FAIL;
}

STDMETHODIMP CActiveXCtrl::InvalidateRgn(HRGN hRGN, BOOL fErase)
{
	UITRACE("AX: CActiveXCtrl::InvalidateRgn");	
 	if( m_pOwner == NULL ) 
 		return E_UNEXPECTED;	
	if( m_hWndHost == NULL ) 
		return E_FAIL;	
	return ::InvalidateRgn(m_hWndHost, hRGN, fErase) ? S_OK : E_FAIL;
}

STDMETHODIMP CActiveXCtrl::ScrollRect(INT dx, INT dy, LPCRECT pRectScroll, LPCRECT pRectClip)
{
	UITRACE("AX: CActiveXCtrl::ScrollRect");
	return S_OK;
}

STDMETHODIMP CActiveXCtrl::AdjustRect(LPRECT prc)
{
	UITRACE("AX: CActiveXCtrl::AdjustRect");
	return S_OK;
}

STDMETHODIMP CActiveXCtrl::OnDefWindowMessage(UINT msg, WPARAM wParam, LPARAM lParam, LRESULT* plResult)
{
	UITRACE("AX: CActiveXCtrl::OnDefWindowMessage");
	*plResult = ::DefWindowProc(m_hWndHost, msg, wParam, lParam);	
	return S_OK;
}



//////////////////////////////////////////////////////////////////////////
//IOleInPlaceSiteEx
STDMETHODIMP CActiveXCtrl::OnInPlaceActivateEx(BOOL* pfNoRedraw, DWORD dwFlags)        
{
	UITRACE("AX: CActiveXCtrl::OnInPlaceActivateEx");
	
	assert(m_pInPlaceObject==NULL);	
 	if( m_pOwner == NULL ) 
 		return E_UNEXPECTED;	
	if( m_pObject == NULL )
		return E_UNEXPECTED;	
	::OleLockRunning(m_pObject, TRUE, FALSE);
	HWND hWndFrame = m_pOwner->AxGetHWND();
	HRESULT hr = E_FAIL;	
	if( (m_pInPlaceObject == NULL) && (dwFlags & ACTIVATE_WINDOWLESS) != 0 ){
		m_bWindowless = true;
		hr = m_pObject->QueryInterface(IID_IOleInPlaceObjectWindowless, (LPVOID*) &m_pInPlaceObject);
		if(SUCCEEDED(hr)) {
			m_hWndHost = hWndFrame;
		}
	}
	
	if( (m_pInPlaceObject == NULL) && FAILED(hr) ) {
		m_bWindowless = false;
		hr = CreateActiveXWnd();		
		if( FAILED(hr) ) 
			return hr;		
		hr = m_pObject->QueryInterface(IID_IOleInPlaceObject, (LPVOID*) &m_pInPlaceObject);
	}
	
	if( m_pInPlaceObject != NULL ){
		CRect rcItem = m_rcPos;		
		if( !m_bWindowless ) 
		{
			rcItem.right = rcItem.Width();
			rcItem.bottom = rcItem.Height();
			rcItem.left=0;
			rcItem.top=0;
		}
		m_pInPlaceObject->SetObjectRects(&rcItem, &rcItem);
	}
	
	m_bInPlaceActive = SUCCEEDED(hr);
	
	return hr;
}

STDMETHODIMP CActiveXCtrl::OnInPlaceDeactivateEx(BOOL fNoRedraw)       
{
	UITRACE("AX: CActiveXCtrl::OnInPlaceDeactivateEx");
	m_bInPlaceActive = false;
	
	if( m_pInPlaceObject != NULL ) {
		m_pInPlaceObject->Release();
		m_pInPlaceObject = NULL;
	}
	
	if( m_pWindow != NULL ) {
		::DestroyWindow(*m_pWindow);
		m_pWindow = NULL;
	}
	
	return S_OK;
}

STDMETHODIMP CActiveXCtrl::RequestUIActivate(void)
{
	UITRACE("AX: CActiveXCtrl::RequestUIActivate");
	return S_OK;
}



//////////////////////////////////////////////////////////////////////////
//IOleInPlaceSite
STDMETHODIMP CActiveXCtrl::CanInPlaceActivate(void)       
{
	UITRACE("AX: CActiveXCtrl::CanInPlaceActivate");
	return S_OK;
}

STDMETHODIMP CActiveXCtrl::OnInPlaceActivate(void)
{
	UITRACE("AX: CActiveXCtrl::OnInPlaceActivate");	
	BOOL bDummy = FALSE;	
	return OnInPlaceActivateEx(&bDummy, 0);
}

STDMETHODIMP CActiveXCtrl::OnUIActivate(void)
{
	UITRACE("AX: CActiveXCtrl::OnUIActivate");	
	m_bUIActivated = true;	
	return S_OK;
}

STDMETHODIMP CActiveXCtrl::GetWindowContext(IOleInPlaceFrame** ppFrame, 
											IOleInPlaceUIWindow** ppDoc, 
											LPRECT lprcPosRect, 
											LPRECT lprcClipRect, 
											LPOLEINPLACEFRAMEINFO lpFrameInfo)
{
	UITRACE("AX: CActiveXCtrl::GetWindowContext");	
	if( ppDoc == NULL ) 
		return E_POINTER;
	if( ppFrame == NULL ) 
		return E_POINTER;	
	if( lprcPosRect == NULL ) 
		return E_POINTER;	
	if( lprcClipRect == NULL ) 
		return E_POINTER;	
	*ppFrame = new CActiveXFrameWnd(this->m_pOwner);
	*ppDoc = NULL;	
	lpFrameInfo->fMDIApp = FALSE;
	lpFrameInfo->hwndFrame = m_hWndHost;
	lpFrameInfo->haccel = 0;
	lpFrameInfo->cAccelEntries = 0;
	RECT r;
	::GetClientRect(m_hWndHost, &r);
	*lprcPosRect = r;
	*lprcClipRect = r;
	return S_OK;
}

STDMETHODIMP CActiveXCtrl::Scroll(SIZE scrollExtant)
{
	UITRACE("AX: CActiveXCtrl::Scroll");
	return E_NOTIMPL;
}

STDMETHODIMP CActiveXCtrl::OnUIDeactivate(BOOL fUndoable)
{
	UITRACE("AX: CActiveXCtrl::OnUIDeactivate");
	m_bUIActivated = false;
	return S_OK;
}

STDMETHODIMP CActiveXCtrl::OnInPlaceDeactivate(void)
{
	UITRACE("AX: CActiveXCtrl::OnInPlaceDeactivate");
	return OnInPlaceDeactivateEx(TRUE);
}

STDMETHODIMP CActiveXCtrl::DiscardUndoState(void)
{
	UITRACE("AX: CActiveXCtrl::DiscardUndoState");
	return E_NOTIMPL;
}

STDMETHODIMP CActiveXCtrl::DeactivateAndUndo(void)
{
	UITRACE("AX: CActiveXCtrl::DeactivateAndUndo");
	return E_NOTIMPL;
}

STDMETHODIMP CActiveXCtrl::OnPosRectChange(LPCRECT lprcPosRect)
{
	UITRACE("AX: CActiveXCtrl::OnPosRectChange");
	return E_NOTIMPL;
}



//////////////////////////////////////////////////////////////////////////
//IOleWindow
STDMETHODIMP CActiveXCtrl::GetWindow(HWND* phwnd)
{
	UITRACE("AX: CActiveXCtrl::GetWindow");
	
 	if( m_pOwner == NULL ) 
 		return E_UNEXPECTED;
	
	if( m_hWndHost == NULL ) 
		CreateActiveXWnd();
	
	if( m_hWndHost == NULL ) 
		return E_FAIL;
	
	*phwnd = m_hWndHost;
	
	return S_OK;
}

STDMETHODIMP CActiveXCtrl::ContextSensitiveHelp(BOOL fEnterMode)
{
	UITRACE("AX: CActiveXCtrl::ContextSensitiveHelp");
	return S_OK;
}



//////////////////////////////////////////////////////////////////////////
// IAdviseSink
STDMETHODIMP_(void) CActiveXCtrl::OnDataChange(FORMATETC* /*pFormatetc*/, STGMEDIUM* /*pStgmed*/)
{
}

STDMETHODIMP_(void) CActiveXCtrl::OnViewChange(DWORD /*dwAspect*/, LONG /*lindex*/)
{
}

STDMETHODIMP_(void) CActiveXCtrl::OnRename(IMoniker* /*pmk*/)
{
}

STDMETHODIMP_(void) CActiveXCtrl::OnSave(void)
{
}

STDMETHODIMP_(void) CActiveXCtrl::OnClose(void)
{
}



//////////////////////////////////////////////////////////////////////////
//IServiceProvider
STDMETHODIMP CActiveXCtrl::QueryService(REFGUID guidService, 
										REFIID riid, 
										void **ppvObject)
{
	return QueryInterface(riid, ppvObject);
}


//////////////////////////////////////////////////////////////////////////
//IOleContainer
STDMETHODIMP CActiveXCtrl::EnumObjects(DWORD grfFlags, IEnumUnknown** ppenum)
{
	UITRACE("AX: CActiveXCtrl::EnumObjects");
	
	if( ppenum == NULL ) 
		return E_POINTER;
	
 	if( m_pOwner == NULL )
 		return E_UNEXPECTED;
	
	*ppenum = new CActiveXEnum(m_pObject);
	
	return S_OK;
}

STDMETHODIMP CActiveXCtrl::LockContainer(BOOL fLock)
{
	UITRACE("AX: CActiveXCtrl::LockContainer");
	m_bLocked = fLock != FALSE;
	return S_OK;
}


//////////////////////////////////////////////////////////////////////////
//IParseDisplayName
STDMETHODIMP CActiveXCtrl::ParseDisplayName(IBindCtx *pbc, LPOLESTR pszDisplayName, ULONG* pchEaten, IMoniker** ppmkOut)
{
	UITRACE("AX: CActiveXCtrl::ParseDisplayName");
	return E_NOTIMPL;
}



//////////////////////////////////////////////////////////////////////////
//IBindHost
STDMETHODIMP CActiveXCtrl::CreateMoniker(LPOLESTR szName, 
										 IBindCtx* /*pBC*/,
										 IMoniker** ppmk, 
										 DWORD /*dwReserved*/)
{
	UITRACE(_T("AX: CActiveXCtrl::CreateMoniker\n"));
	return CreateURLMonikerEx(NULL, szName, ppmk, URL_MK_UNIFORM);
}

STDMETHODIMP CActiveXCtrl::MonikerBindToStorage(IMoniker *pMk, 
												IBindCtx *pBC,
												IBindStatusCallback *pBSC, 
												REFIID riid, 
												void **ppvObj)
{
	UITRACE(_T("AX: CActiveXCtrl::MonikerBindToObject\n"));
	HRESULT hr = E_FAIL;
	CUIAutoPtr<IBindCtx> RefBC(pBC);
	if (pBC == NULL) {
		// creates it if not present
		hr = CreateBindCtx(0, &RefBC.m_p);
	} else {
		pBC->AddRef();
	}

	if ( RefBC.m_p != NULL ) {
		if ( pBSC != NULL )	{
			hr = RegisterBindStatusCallback(RefBC.m_p, pBSC, NULL, 0);
		}
		hr = pMk->BindToStorage(RefBC.m_p, NULL, riid, ppvObj);
	}
	return hr;
}

STDMETHODIMP CActiveXCtrl::MonikerBindToObject(IMoniker* /*pMk*/, 
											   IBindCtx* /*pBC*/,
											   IBindStatusCallback* /*pBSC*/, 
											   REFIID /*riid*/, 
											   void** /*ppvObj*/)
{
	UITRACE(_T("AX: CActiveXCtrl::MonikerBindToObject\n"));
	return E_NOTIMPL;
}

//////////////////////////////////////////////////////////////////////////
HRESULT CActiveXCtrl::CreateActiveXWnd()
{
	if( m_hWndHost != NULL ) 
		return S_OK;	
	m_pWindow = new CActiveXWnd;	
	if( m_pWindow == NULL ) 
		return E_OUTOFMEMORY;	
	m_hWndHost = m_pWindow->Init(this, 
		m_pOwner->AxGetHWND(), 
		m_rcPos.right - m_rcPos.left,
		m_rcPos.bottom - m_rcPos.top);	
	return S_OK;
}


//////////////////////////////////////////////////////////////////////////
bool CActiveXCtrl::CreateControl( IActivXOwner* pOwner, CLSID clsid, const RECT& rcPos )
{
	IOleControl* pOleControl = NULL;
	HRESULT hr = ::CoCreateInstance(clsid, NULL, CLSCTX_ALL, IID_IOleControl, (LPVOID*) &pOleControl);
	if( FAILED(hr) )
		return false;

	pOleControl->QueryInterface(IID_IOleObject, (LPVOID*) &m_pObject);
	pOleControl->Release();
	if( m_pObject == NULL ) 
	 	return false;
		
	m_pOwner = pOwner;

	// More control creation stuff
	DWORD dwMiscStatus = 0;
	m_pObject->GetMiscStatus(DVASPECT_CONTENT, &dwMiscStatus);
	IOleClientSite* pOleClientSite = NULL;
	this->QueryInterface(IID_IOleClientSite, (LPVOID*) &pOleClientSite);
	CUIAutoPtr<IOleClientSite> RefOleClientSite = pOleClientSite;

	// Initialize control
	if( (dwMiscStatus & OLEMISC_SETCLIENTSITEFIRST) != 0 ) 
		m_pObject->SetClientSite(pOleClientSite);
	
	IPersistStreamInit* pPersistStreamInit = NULL;
	m_pObject->QueryInterface(IID_IPersistStreamInit, (LPVOID*) &pPersistStreamInit);
	
	if( pPersistStreamInit != NULL ) {
		hr = pPersistStreamInit->InitNew();
		pPersistStreamInit->Release();
	}
	
	if( FAILED(hr) ) {
		m_pObject->Release();
		m_pObject = NULL;		
		return false;
	}

	if( (dwMiscStatus & OLEMISC_SETCLIENTSITEFIRST) == 0 )
		m_pObject->SetClientSite(pOleClientSite);
	
	// Grab the view...
	hr = m_pObject->QueryInterface(IID_IViewObjectEx, (LPVOID*) &m_pViewObject);	
	if( FAILED(hr) )
		hr = m_pObject->QueryInterface(IID_IViewObject2, (LPVOID*) &m_pViewObject);	
	if( FAILED(hr) ) 
		hr = m_pObject->QueryInterface(IID_IViewObject, (LPVOID*) &m_pViewObject);
	
	//m_pViewObject->SetAdvise(DVASPECT_CONTENT, 0, static_cast<IAdviseSink*>(this));

	//SetPos(rcPos);

	// Activate and done...
	m_pObject->SetHostNames(OLESTR("UIActiveX"), NULL);
	
	if( (dwMiscStatus & OLEMISC_INVISIBLEATRUNTIME) == 0 ) {
		hr = m_pObject->DoVerb(OLEIVERB_INPLACEACTIVATE, 
			NULL, 
			static_cast<IOleClientSite*>(this), 
			0, \
			m_pOwner->AxGetHWND(), 
			&rcPos);
		
		::RedrawWindow(m_pOwner->AxGetHWND(), &rcPos, NULL, \
			RDW_INVALIDATE | RDW_UPDATENOW | RDW_ERASE | RDW_INTERNALPAINT | RDW_FRAME);
	} 
	
 	IObjectWithSite* pSite = NULL;
 	m_pObject->QueryInterface(IID_IObjectWithSite, (LPVOID*) &pSite); 	
 	if( pSite != NULL )	{
 		pSite->SetSite(static_cast<IOleClientSite*>(this));
 		pSite->Release();
 	}	

	return SUCCEEDED(hr);
}

void CActiveXCtrl::ReleaseControl(  )
{
	m_hWndHost = NULL;
	m_pOwner = NULL;
	if( m_pObject != NULL ) {
		IObjectWithSite* pSite = NULL;
		m_pObject->QueryInterface(IID_IObjectWithSite, (LPVOID*) &pSite);
		if( pSite != NULL ) {
			pSite->SetSite(NULL);
			pSite->Release();
		}
		m_pObject->Close(OLECLOSE_NOSAVE);
		m_pObject->SetClientSite(NULL);
		m_pObject->Release(); 
		m_pObject = NULL;
	}
}

HRESULT CActiveXCtrl::GetControl(const IID iid, LPVOID* ppRet)
{
	if( ppRet == NULL )
		return E_POINTER;
	if( m_pObject == NULL )
		return E_PENDING;
	return m_pObject->QueryInterface(iid, (LPVOID*) ppRet);
}



//////////////////////////////////////////////////////////////////////////
void CActiveXCtrl::DoPaint(HDC hDC, const RECT& rcPaint)
{
	if( m_bWindowless && m_pViewObject != NULL ) {
		m_pViewObject->Draw(DVASPECT_CONTENT,
			-1, 
			NULL, 
			NULL, 
			NULL, \
			hDC, 
			(RECTL*) &m_rcPos, 
			(RECTL*) &m_rcPos, 
			NULL, 
			NULL); 
	}
}

void CActiveXCtrl::SetPos( const RECT& rc )
{
	if( m_pObject == NULL ) 
		return;
	
	m_rcPos = rc;
	
	SIZE hmSize = { 0 };
	SIZE pxSize = { 0 };
	pxSize.cx = m_rcPos.right - m_rcPos.left;
	pxSize.cy = m_rcPos.bottom - m_rcPos.top;
	//UIPixelToHiMetric(&pxSize, &hmSize);

	//ATLINLINE ATLAPI_(void) AtlPixelToHiMetric(const SIZEL * lpSizeInPix, LPSIZEL lpSizeInHiMetric){...}
	AtlPixelToHiMetric(&pxSize, &hmSize);
	
	if( m_pObject != NULL )	{
		m_pObject->SetExtent(DVASPECT_CONTENT, &hmSize);
	}
	
	if( m_pInPlaceObject != NULL ) {
		CRect rcItem = m_rcPos;		
		if( !m_bWindowless )
		{
			rcItem.right = rcItem.Width();
			rcItem.bottom = rcItem.Height();
			rcItem.left=0;
			rcItem.top=0;
		}
		m_pInPlaceObject->SetObjectRects(&rcItem, &rcItem);
	}
	
	if( !m_bWindowless && m_pWindow){
		::MoveWindow(*m_pWindow, m_rcPos.left, m_rcPos.top, \
			m_rcPos.right - m_rcPos.left, m_rcPos.bottom - m_rcPos.top, TRUE);
	}	
}

RECT CActiveXCtrl::GetPos()
{
	return m_rcPos;
}

LRESULT CActiveXCtrl::MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled)
{
	//UIASSERT(m_bWindowless);
	
	if(!m_bWindowless )
		return 0;

	if( !m_bInPlaceActive )
		return 0;
	
	if( m_pInPlaceObject == NULL )
		return 0;
	
	bool bWasHandled = true;
	
	if( (uMsg >= WM_MOUSEFIRST && uMsg <= WM_MOUSELAST) || uMsg == WM_SETCURSOR ) {
		// Mouse message only go when captured or inside rect
		DWORD dwHitResult = m_bCaptured ? HITRESULT_HIT : HITRESULT_OUTSIDE;
		
		if( (dwHitResult == HITRESULT_OUTSIDE) \
			&& (m_pViewObject != NULL) ) {
			POINT ptMouse = { GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) };			
			m_pViewObject->QueryHitPoint(DVASPECT_CONTENT, &m_rcPos, ptMouse, 0, &dwHitResult);
		}
		
		if( dwHitResult != HITRESULT_HIT ) 
			return 0;
		
		if( uMsg == WM_SETCURSOR ) 
			bWasHandled = false;
	} else if( uMsg >= WM_KEYFIRST && uMsg <= WM_KEYLAST ) {
		// Keyboard messages just go when we have focus
		if( !m_bFocused ) 
			return 0;
	} else {
		switch( uMsg ) 
		{
		case WM_HELP:
		case WM_CONTEXTMENU:
			bWasHandled = false;
			break;

		//default:
		//	return 0;
		}
	}
	
	LRESULT lResult = 0;
	HRESULT hr = m_pInPlaceObject->OnWindowMessage(uMsg, wParam, lParam, &lResult);
	
	if( hr == S_OK )
		bHandled = bWasHandled;
	
   return lResult;
}

bool CActiveXCtrl::Advise(IUnknown* pObject, REFIID riid, IUnknown *pUnkSink, DWORD *pdwCookie)
{
	bool ret = false;
	IConnectionPointContainer *cpc = NULL;
	IConnectionPoint *cp = NULL;

	do 
	{
		if(pObject == NULL)
			break;

		if(!SUCCEEDED(pObject->QueryInterface(IID_IConnectionPointContainer, reinterpret_cast<void**>(&cpc) ) ))
			break;

		if(!SUCCEEDED(cpc->FindConnectionPoint(riid, &cp) ) )
			break;

		ret = SUCCEEDED(cp->Advise(pUnkSink, pdwCookie));
	} while (0);

	MyRelease(cp);
	MyRelease(cpc);

	return ret;
}

bool CActiveXCtrl::UnAdvise( IUnknown* pObject, REFIID riid, DWORD dwCookie )
{
	bool ret = false;
	IConnectionPointContainer	*cpc = NULL;
	IConnectionPoint			*cp = NULL;

	do 
	{
		if(pObject == NULL)
			break;

		if(!SUCCEEDED(pObject->QueryInterface(IID_IConnectionPointContainer, reinterpret_cast<void**>(&cpc) ) ))
			break;

		if(!SUCCEEDED(cpc->FindConnectionPoint(riid, &cp) ) )
			break;

		ret = SUCCEEDED(cp->Unadvise(dwCookie));
	} while (0);

	MyRelease(cp);
	MyRelease(cpc);

	return ret;
}


bool CActiveXCtrl::IsWindowless()
{
	return m_bWindowless;
}

bool CActiveXCtrl::IsFocus()
{
	return m_bFocused;
}

bool CActiveXCtrl::IsUIActivated()
{
	return m_bUIActivated;
}

bool CActiveXCtrl::IsInplaceActivated()
{
	return m_bInPlaceActive;
}

void CActiveXCtrl::SetActiveXWnd( CActiveXWnd* pWnd )
{
	this->m_pWindow = pWnd;
}