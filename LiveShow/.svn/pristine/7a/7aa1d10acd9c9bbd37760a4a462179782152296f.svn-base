/********************************************************************
created:	2006/04/12
created:	12:4:2006   16:41
file base:	UIString
file ext:	cpp
author:		Garret Lian

  purpose:	
*********************************************************************/
#ifndef __DIRECTUI_ACTIVEXBASE_H__
#define __DIRECTUI_ACTIVEXBASE_H__


/////////////////////////////////////////////////////////////////////////////////////
class CActiveXWnd;
class IActivXOwner;


//CUIAutoPtr
template< class T >
class CUIAutoPtr
{
public:
	CUIAutoPtr(T* p) : m_p(p) { };
	~CUIAutoPtr() { if( m_p != NULL ) m_p->Release(); };
	T* Detach() { T* t = m_p; m_p = NULL; return t; };
	T* m_p;
};


/////////////////////////////////////////////////////////////////////////////////////
class CActiveXCtrl :
	public IObjectWithSite,
	public IOleClientSite,
	public IOleControlSite,
	public IOleInPlaceSiteWindowless,
	public IAdviseSink,
	public IServiceProvider,
	public IOleContainer,
	public IBindHost
{
public:
	CActiveXCtrl();
	virtual ~CActiveXCtrl();
	
public:
	// IUnknown
	STDMETHOD_(ULONG, AddRef) ();
	STDMETHOD_(ULONG, Release) ();
	STDMETHOD(QueryInterface)(REFIID riid, LPVOID *ppvObject);
	
	// IObjectWithSite
	STDMETHOD(SetSite)(IUnknown *pUnkSite);
	STDMETHOD(GetSite)(REFIID riid, LPVOID* ppvSite);

	// IOleClientSite
	STDMETHOD(SaveObject)(void);       
	STDMETHOD(GetMoniker)(DWORD dwAssign, DWORD dwWhichMoniker, IMoniker** ppmk);
	STDMETHOD(GetContainer)(IOleContainer** ppContainer);        
	STDMETHOD(ShowObject)(void);        
	STDMETHOD(OnShowWindow)(BOOL fShow);        
	STDMETHOD(RequestNewObjectLayout)(void);
	
	// IOleControlSite
	STDMETHOD(OnControlInfoChanged)(void);      
	STDMETHOD(LockInPlaceActive)(BOOL fLock);       
	STDMETHOD(GetExtendedControl)(IDispatch** ppDisp);        
	STDMETHOD(TransformCoords)(POINTL* pPtlHimetric, POINTF* pPtfContainer, DWORD dwFlags);       
	STDMETHOD(TranslateAccelerator)(MSG* pMsg, DWORD grfModifiers);
	STDMETHOD(OnFocus)(BOOL fGotFocus);
	STDMETHOD(ShowPropertyFrame)(void);

	// IOleInPlaceSiteWindowless
	STDMETHOD(CanWindowlessActivate)(void);
	STDMETHOD(GetCapture)(void);
	STDMETHOD(SetCapture)(BOOL fCapture);
	STDMETHOD(GetFocus)(void);
	STDMETHOD(SetFocus)(BOOL fFocus);
	STDMETHOD(GetDC)(LPCRECT pRect, DWORD grfFlags, HDC* phDC);
	STDMETHOD(ReleaseDC)(HDC hDC);
	STDMETHOD(InvalidateRect)(LPCRECT pRect, BOOL fErase);
	STDMETHOD(InvalidateRgn)(HRGN hRGN, BOOL fErase);
	STDMETHOD(ScrollRect)(INT dx, INT dy, LPCRECT pRectScroll, LPCRECT pRectClip);
	STDMETHOD(AdjustRect)(LPRECT prc);
	STDMETHOD(OnDefWindowMessage)(UINT msg, WPARAM wParam, LPARAM lParam, LRESULT* plResult);
	
	// IOleInPlaceSiteEx
	STDMETHOD(OnInPlaceActivateEx)(BOOL *pfNoRedraw, DWORD dwFlags);        
	STDMETHOD(OnInPlaceDeactivateEx)(BOOL fNoRedraw);       
	STDMETHOD(RequestUIActivate)(void);
	
	// IOleInPlaceSite
	STDMETHOD(CanInPlaceActivate)(void);       
	STDMETHOD(OnInPlaceActivate)(void);        
	STDMETHOD(OnUIActivate)(void);
	STDMETHOD(GetWindowContext)(
		IOleInPlaceFrame** ppFrame, 
		IOleInPlaceUIWindow** ppDoc, 
		LPRECT lprcPosRect, 
		LPRECT lprcClipRect, 
		LPOLEINPLACEFRAMEINFO lpFrameInfo);
	STDMETHOD(Scroll)(SIZE scrollExtant);
	STDMETHOD(OnUIDeactivate)(BOOL fUndoable);
	STDMETHOD(OnInPlaceDeactivate)(void);
	STDMETHOD(DiscardUndoState)( void);
	STDMETHOD(DeactivateAndUndo)( void);
	STDMETHOD(OnPosRectChange)(LPCRECT lprcPosRect);
	
	// IOleWindow
	STDMETHOD(GetWindow)(HWND* phwnd);
	STDMETHOD(ContextSensitiveHelp)(BOOL fEnterMode);

	//IAdviseSink
	STDMETHOD_(void, OnDataChange)(
		FORMATETC* /*pFormatetc*/,
		STGMEDIUM* /*pStgmed*/);
	STDMETHOD_(void, OnViewChange)(DWORD /*dwAspect*/, LONG /*lindex*/);
	STDMETHOD_(void, OnRename)(IMoniker* /*pmk*/);
	STDMETHOD_(void, OnSave)(void);
	STDMETHOD_(void, OnClose)(void);

	// IServiceProvider
	STDMETHOD(QueryService)(REFGUID guidService, REFIID riid, void **ppvObject);

	// IOleContainer
	STDMETHOD(EnumObjects)(DWORD grfFlags, IEnumUnknown** ppenum);
	STDMETHOD(LockContainer)(BOOL fLock);
	
	// IParseDisplayName
	STDMETHOD(ParseDisplayName)(IBindCtx* pbc, 
		LPOLESTR pszDisplayName, 
		ULONG* pchEaten, 
		IMoniker** ppmkOut);
	
	// IBindHost
	STDMETHOD(CreateMoniker)(
		LPOLESTR szName, 
		IBindCtx* pBC,
		IMoniker** ppmk,
		DWORD dwReserved);
	STDMETHOD(MonikerBindToStorage)(
		IMoniker *pMk, 
		IBindCtx *pBC,
		IBindStatusCallback *pBSC, 
		REFIID riid, 
		void **ppvObj);
	STDMETHOD(MonikerBindToObject)(
		IMoniker* pMk, 
		IBindCtx* pBC,
		IBindStatusCallback* pBSC,
		REFIID riid, 
		void** ppvObj);

public:
	virtual bool CreateControl( IActivXOwner* pOwner, CLSID clsid, const RECT& rcPos );
	virtual void ReleaseControl();
	virtual HRESULT GetControl(const IID iid, LPVOID* ppRet);

	static bool Advise( IUnknown* pObject,REFIID riid, IUnknown *pUnkSink, DWORD *pdwCookie);
	static bool UnAdvise( IUnknown* pObject, REFIID riid, DWORD dwCookie);

	static void MyRelease(IUnknown* pObject);

	virtual LRESULT MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled);

	virtual void DoPaint(HDC hDC, const RECT& rcPaint);
	void SetPos( const RECT& rc );
	RECT GetPos();

	bool IsWindowless();
	bool IsUIActivated();
	bool IsInplaceActivated();
	bool IsFocus();

	void SetActiveXWnd(CActiveXWnd* pWnd);

//protected:
	HRESULT CreateActiveXWnd();

protected:
	volatile LONG m_dwRef;
	
	IActivXOwner* m_pOwner;
	CActiveXWnd* m_pWindow;
	
	HWND m_hWndHost;

	RECT m_rcPos;

	IOleObject* m_pObject;
	IUnknown* m_pUnkSite;
	IViewObjectEx* m_pViewObject;
	IOleInPlaceObjectWindowless* m_pInPlaceObject;
	
	bool m_bLocked;
	bool m_bFocused;
	bool m_bCaptured;
	bool m_bUIActivated;
	bool m_bInPlaceActive;
	bool m_bWindowless;
};


//////////////////////////////////////////////////////////////////////////
class IActivXOwner 
{
public:
	virtual HWND AxGetHWND() = 0;
	virtual void AxSetFocus(bool bFocus) = 0;
};


#endif // __DIRECTUI_ACTIVEXBASE_H__