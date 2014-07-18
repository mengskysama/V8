// SkinItem.cpp : implementation file
//

#include "stdafx.h"
#include "SkinProxy.h"
#include "SkinItem.h"
#include "SkinItemDialog.h"
#include "SkinEngine.h"
#include "RenderEngine.h"
#include "ImageBase.h"
#include ".\skinitem.h"

#include "Types.h"

#include <map>

using namespace std;

#ifndef WM_THEMECHANGED
#define WM_THEMECHANGED 0x031A
#endif

#ifdef V8_JS_ENGINE_SUPPORT
using namespace v8;
#endif //V8_JS_ENGINE_SUPPORT

#pragma warning (disable:4311)
#pragma warning (disable:4312)

// CSkinItem

IMPLEMENT_DYNCREATE(CSkinItem, CSkinBase)

map<HWND, CSkinItem*> CSkinItem::s_mapSkinItem;

CSkinItem::CSkinItem()
:
m_hWnd(NULL),
m_hToolTip(NULL),
m_pOldWndProc(NULL),
m_pPrevMouseInSkin(NULL),
m_pSkinActive(NULL),
m_pSkinFocus(NULL),
m_pSkinCapture(NULL),
m_pSkinMenuOwner(NULL),
m_pSkinMoveSize(NULL),
m_pSkinCaret(NULL)
{
	EnableAutomation();

	m_bColorTransform = TRUE;

	m_enFindHwnd = FIND_HWND_NONE;
	m_hFindHwnd = NULL;
	m_nItemID = 0;
	m_strWndClassName = _T("");
	m_ptFindPoint = CPoint(0, 0);

    m_bUseSkinPaint = TRUE;
	m_bSelfOnPaint = TRUE;
    m_bSelfOnNcPaint = TRUE;
	m_bSelfOnEraseBkgnd = TRUE;
    m_bSelfOnSetFocus = FALSE;
    m_lEraseBkMode = -1;
	m_bSelfOnSize = TRUE;
    m_bSelfOnCtlColor = FALSE;
    m_bBkColorTransform = TRUE;
	m_bClipAllChild = FALSE;
	m_bRecieveCtrlBreak = FALSE;

	m_buseOriginPosition = FALSE;
    m_buseOriginText = TRUE;

	m_clrBkColorNormal = RGB(255, 255, 255);
	m_clrBkColorHover = -1;
	m_clrBkColorFocus = -1;
	m_clrBkColorDisabled = RGB(233, 233, 233);

	m_hSmallIcon = NULL;
	m_hBigIcon = NULL;
	m_bShowIcon = FALSE;

    BACKGROUND_IMAGE_LIST(CSkinItem, CLEAR_IMAGE_PROPERTY_VAR);

	m_bLayered = FALSE;
	m_nOpacity = 100;

	m_bTopMost = -1;

    m_bTrackMouse = TRUE;
    m_bMouseEntered = FALSE;

    SKINITEM_GUARD_LIST(CLEAR_SKINITEM_GUARD);

    m_bBkDrag = FALSE;
    m_ptDragPoint = CPoint(-1, -1);
    m_bDragging = FALSE;

    m_bSkinScrollBar = TRUE;
    m_bSkinScrollStretchPage = FALSE;
    m_rcSkinScrollPage = CRect(0, 0, 0, 0);
    m_lSkinScrollBarChild = 0;
    m_lSkinScrollBarWidth = 15;
    m_lSkinScrollBarHeight = 15;
    m_lSkinScrollBarSideWidth = 3;
    m_lSkinScrollBarSideHeight = 3;
    SCROLLBAR_IMAGE_LIST(CSkinItem, CLEAR_IMAGE_PROPERTY_VAR);
}

vector<SKINITEM_STACK_FRAME> CSkinItem::s_vecSkinItemStack;

BOOL CSkinItem::StackTrace(UINT nResr, SKINITEM_STACK_FRAME& frame)
{
    vector<SKINITEM_STACK_FRAME>::size_type len = s_vecSkinItemStack.size();
    if (nResr >= len)
        return FALSE;

    frame = s_vecSkinItemStack[len - nResr - 1];
    return TRUE;
}

BOOL CSkinItem::IsInStack(UINT uMsg)
{
    vector<SKINITEM_STACK_FRAME>::size_type len = s_vecSkinItemStack.size();
    for (vector<SKINITEM_STACK_FRAME>::size_type i=0; i<len; i++) {
        const SKINITEM_STACK_FRAME& frame = s_vecSkinItemStack[i];
        if (frame.uMsg == uMsg)
            return TRUE;
    }
    return FALSE;
}

BOOL CSkinItem::IsInStack(HWND hWnd, UINT uMsg)
{
    vector<SKINITEM_STACK_FRAME>::size_type len = s_vecSkinItemStack.size();
    for (vector<SKINITEM_STACK_FRAME>::size_type i=0; i<len; i++) {
        const SKINITEM_STACK_FRAME& frame = s_vecSkinItemStack[i];
        if (frame.hWnd == hWnd && frame.uMsg == uMsg)
            return TRUE;
    }
    return FALSE;
}

BOOL CSkinItem::IsOnStackBottom(UINT uMsg)
{
    vector<SKINITEM_STACK_FRAME>::size_type len = s_vecSkinItemStack.size();
    if (len <= 0)
        return FALSE;

    const SKINITEM_STACK_FRAME& frame = s_vecSkinItemStack[0];
    return (frame.uMsg == uMsg);
}

BOOL CSkinItem::IsOnStackBottom(HWND hWnd, UINT uMsg)
{
    vector<SKINITEM_STACK_FRAME>::size_type len = s_vecSkinItemStack.size();
    if (len <= 0)
        return FALSE;

    const SKINITEM_STACK_FRAME& frame = s_vecSkinItemStack[0];
    return (frame.hWnd == hWnd && frame.uMsg == uMsg);
}

CSkinItem::~CSkinItem()
{
	Detach();

    if (m_hToolTip != NULL){
        ::DestroyWindow(m_hToolTip);
        m_hToolTip = NULL;
    }

	if (m_hSmallIcon != NULL){
		::DestroyIcon(m_hSmallIcon);
		m_hSmallIcon = NULL;
	}
	if (m_hBigIcon != NULL){
		::DestroyIcon(m_hBigIcon);
		m_hBigIcon = NULL;
	}
}

void CSkinItem::OnFinalRelease()
{
	// When the last reference for an automation object is released
	// OnFinalRelease is called.  The base class will automatically
	// deletes the object.  Add additional cleanup required for your
	// object before calling the base class.

	CCmdTarget::OnFinalRelease();
}


BEGIN_MESSAGE_MAP(CSkinItem, CSkinBase)
END_MESSAGE_MAP()


BEGIN_SKIN_DISPATCH_MAP(CSkinItem, CSkinBase)
	SKINBASE_DISP_PROPERTIES(CSkinItem)
	SKINITEM_DISP_PROPERTIES(CSkinItem)

END_SKIN_DISPATCH_MAP(CSkinItem)

// Note: we add support for IID_ISkinItem to support typesafe binding
//  from VBA.  This IID must match the GUID that is attached to the 
//  dispinterface in the .IDL file.

// {D52A4535-1BB2-497D-95C1-76D33D0D122E}
static const IID IID_ISkinItem =
{ 0xD52A4535, 0x1BB2, 0x497D, { 0x95, 0xC1, 0x76, 0xD3, 0x3D, 0xD, 0x12, 0x2E } };

BEGIN_INTERFACE_MAP(CSkinItem, CSkinBase)
	INTERFACE_PART(CSkinItem, IID_ISkinItem, Dispatch)
END_INTERFACE_MAP()


// CSkinItem message handlers
BOOL CSkinItem::MoveSkinItemHelper(const RECT& rc, BOOL bRepaint)
{
    if (! ::IsWindow(m_hWnd))
        return FALSE;

    CRect rect = rc;

    return ::SetWindowPos(m_hWnd, NULL, rect.left, rect.top, rect.Width(), rect.Height(), SWP_NOZORDER | SWP_NOACTIVATE | SWP_NOOWNERZORDER);
}

BOOL CSkinItem::GetSkinItemRectHelper(RECT& rect)
{
    ASSERT(::IsWindow(m_hWnd));
    if (! ::IsWindow(m_hWnd))
        return FALSE;

    ::GetWindowRect(m_hWnd, &rect);

    BOOL bChild = TRUE;
    GetSkinItemLongHelper(GWL_STYLE, WS_CHILD, bChild);
    if (! bChild)
        return TRUE;

    HWND hParent = ::GetParent(m_hWnd);
    CPoint topLeft(rect.left, rect.top);
    CPoint bottomDown(rect.right, rect.bottom);
    ::ScreenToClient(hParent, &topLeft);
    ::ScreenToClient(hParent, &bottomDown);
    rect.left = topLeft.x; rect.top = topLeft.y;
    rect.right = bottomDown.x; rect.bottom = bottomDown.y;
    return TRUE;
}

BOOL CSkinItem::SetSkinItemRedrawHelper(BOOL bRedraw)
{
    ASSERT(::IsWindow(m_hWnd));
    if (! ::IsWindow(m_hWnd))
        return FALSE;

    return (FALSE != ::SendMessage(m_hWnd, WM_SETREDRAW, (WPARAM)(bRedraw ? (TRUE) : (FALSE)), 0));
}

BOOL CSkinItem::GetSkinItemTextHelper(CString& strText)
{
    if (! ::IsWindow(m_hWnd))
        return FALSE;

    LONG len = ::GetWindowTextLength(m_hWnd);
    BOOL bSuc = (len == ::GetWindowText(m_hWnd, strText.GetBuffer(len + 1), len + 1));
    strText.ReleaseBuffer();
    ASSERT(bSuc);
    return bSuc;
}

BOOL CSkinItem::SetSkinItemTextHelper(LPCTSTR strText)
{
    if (! ::IsWindow(m_hWnd))
        return FALSE;

    return ::SetWindowText(m_hWnd, strText);
}

BOOL CSkinItem::MoveWindow(LPCRECT lpRect, BOOL bRepaint)
{
	if (! ::IsWindow(m_hWnd))
		return FALSE;

	CRect rect(0, 0, 0, 0);
	if (lpRect != NULL)
		rect = *lpRect;
	else
		rect = GetSkinRect();

    return MoveSkinItemHelper(rect, bRepaint);
}

HWND CSkinItem::GetSafeHwnd()
{
	return ::IsWindow(m_hWnd) ? m_hWnd : NULL;
}

BOOL CSkinItem::InvalidateRect(LPCRECT lpRects, BOOL bErase)
{
	if (! ::IsWindow(m_hWnd))
		return FALSE;

	return ::InvalidateRect(m_hWnd, lpRects, bErase);
}

LONG CSkinItem::GetitemID()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	return m_nItemID;
}

void CSkinItem::SetitemID(long newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	if (m_nItemID == newVal)
		return;

	m_nItemID = newVal;
	m_enFindHwnd = FIND_HWND_CTRL_ID;
	UpdateSkinItem();
}

BSTR CSkinItem::GetclassName()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	return m_strWndClassName.AllocSysString();
}

void CSkinItem::SetclassName(LPCTSTR newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	m_strWndClassName = newVal;
	m_enFindHwnd = FIND_HWND_WND_CLASS_TEXT;
}

LONG CSkinItem::GetfindX()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	return m_ptFindPoint.x;
}

void CSkinItem::SetfindX(LONG newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	 m_ptFindPoint.x = newVal;
	 m_enFindHwnd = FIND_HWND_WND_POS;
}

LONG CSkinItem::GetfindY()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	return m_ptFindPoint.y;
}

void CSkinItem::SetfindY(LONG newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	m_ptFindPoint.y = newVal;
	m_enFindHwnd = FIND_HWND_WND_POS;
}

VARIANT_BOOL CSkinItem::GetuseSkinPaint()
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());
    return m_bUseSkinPaint ? (VARIANT_TRUE) : (VARIANT_FALSE);
}

void CSkinItem::SetuseSkinPaint(VARIANT_BOOL newVal)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());
    newVal = (newVal != VARIANT_FALSE);
    m_bUseSkinPaint = newVal;
    InvalidateRect();
}

VARIANT_BOOL CSkinItem::GetselfOnPaint()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	return m_bSelfOnPaint ? (VARIANT_TRUE) : (VARIANT_FALSE);
}

void CSkinItem::SetselfOnPaint(VARIANT_BOOL newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	newVal = (newVal != VARIANT_FALSE);
	m_bSelfOnPaint = newVal;
	InvalidateRect();
}

VARIANT_BOOL CSkinItem::GetselfOnNcPaint()
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());
    return m_bSelfOnNcPaint ? (VARIANT_TRUE) : (VARIANT_FALSE);
}

void CSkinItem::SetselfOnNcPaint(VARIANT_BOOL newVal)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());
    newVal = (newVal != VARIANT_FALSE);
    m_bSelfOnNcPaint = newVal;
    InvalidateRect();
}

VARIANT_BOOL CSkinItem::GetselfOnEraseBkgnd()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	return m_bSelfOnEraseBkgnd ? (VARIANT_TRUE) : (VARIANT_FALSE);
}

void CSkinItem::SetselfOnEraseBkgnd(VARIANT_BOOL newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	newVal = (newVal != VARIANT_FALSE);
	m_bSelfOnEraseBkgnd = newVal;
	InvalidateRect();
}

VARIANT_BOOL CSkinItem::GetselfOnSetFocus()
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());
    return m_bSelfOnSetFocus ? (VARIANT_TRUE) : (VARIANT_FALSE);
}

void CSkinItem::SetselfOnSetFocus(VARIANT_BOOL newVal)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());
    newVal = (newVal != VARIANT_FALSE);
    m_bSelfOnSetFocus = newVal;
    InvalidateRect();
}

BSTR CSkinItem::GeteraseBkMode()
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());

    CString strResult;

    // TODO: 在此添加调度处理程序代码
    switch (m_lEraseBkMode)
    {
    case TRANSPARENT:
        strResult = _T("transparent");
        break;
    case OPAQUE:
        strResult = _T("opaque");
        break;
    default:
        strResult = _T("default");
        break;
    }
    return strResult.AllocSysString();
}

void CSkinItem::SeteraseBkMode(LPCTSTR newVal)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());

    // TODO: 在此添加属性处理程序代码
    CString strMode = newVal;
    LONG mode = 0;

    if (strMode.CompareNoCase(_T("transparent")) == 0) {
        mode = TRANSPARENT;
    } else if (strMode.CompareNoCase(_T("opaque")) == 0) {
        mode = OPAQUE;
    } else if (strMode.CompareNoCase(_T("bkmode_last")) == 0) {
        mode = OPAQUE;
    } else if (strMode.CompareNoCase(_T("default")) == 0) {
        mode = -1;
    }
    if (m_lEraseBkMode == mode)
        return;

    if (mode == 0)
        return;

    m_lEraseBkMode = mode;
    InvalidateRect();
}

VARIANT_BOOL CSkinItem::GetselfOnSize()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	return m_bSelfOnSize ? (VARIANT_TRUE) : (VARIANT_FALSE);
}

void CSkinItem::SetselfOnSize(VARIANT_BOOL newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	newVal = (newVal != VARIANT_FALSE);
	m_bSelfOnSize = newVal;
	InvalidateRect();
}

VARIANT_BOOL CSkinItem::GetselfOnCtlColor()
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());
    return m_bSelfOnCtlColor ? (VARIANT_TRUE) : (VARIANT_FALSE);
}

void CSkinItem::SetselfOnCtlColor(VARIANT_BOOL newVal)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());
    newVal = (newVal != VARIANT_FALSE);
    m_bSelfOnCtlColor = newVal;
    InvalidateRect();
}

VARIANT_BOOL CSkinItem::GetbkColorTransform()
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());
    return m_bBkColorTransform ? (VARIANT_TRUE) : (VARIANT_FALSE);
}

void CSkinItem::SetbkColorTransform(VARIANT_BOOL newVal)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());
    newVal = (newVal != VARIANT_FALSE);
    m_bBkColorTransform = newVal;
    InvalidateRect();
}

VARIANT_BOOL CSkinItem::GetclipAllChild()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	return m_bClipAllChild ? (VARIANT_TRUE) : (VARIANT_FALSE);
}

void CSkinItem::SetclipAllChild(VARIANT_BOOL newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	newVal = (newVal != VARIANT_FALSE);
	m_bClipAllChild = newVal;
	InvalidateRect();
}

LONG CSkinItem::Gethwnd()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	return (LONG)m_hFindHwnd;
}

void CSkinItem::Sethwnd(LONG newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	if (m_hFindHwnd == (HWND)newVal)
		return;

	m_hFindHwnd = (HWND)newVal;

	m_enFindHwnd = FIND_HWND_HWND;

	UpdateSkinItem();
}

LONG CSkinItem::GetrestoredWidth()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	WINDOWPLACEMENT wndPlace;
	if (! GetSkinItemPlacementHelper(wndPlace))
		return -1;

	CRect normalRect = wndPlace.rcNormalPosition;
	return normalRect.Width();
}

void CSkinItem::SetrestoredWidth(LONG newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	if (newVal <= 0)
		return;

	WINDOWPLACEMENT wndPlace;
	if (! GetSkinItemPlacementHelper(wndPlace))
		return;

	CRect normalRect = wndPlace.rcNormalPosition;
	if (normalRect.Width() == newVal)
		return;

	wndPlace.rcNormalPosition.right = wndPlace.rcNormalPosition.left + newVal;
	SetSkinItemPlacementHelper(wndPlace);
}

LONG CSkinItem::GetrestoredHeight()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	WINDOWPLACEMENT wndPlace;
	if (! GetSkinItemPlacementHelper(wndPlace))
		return -1;

	CRect normalRect = wndPlace.rcNormalPosition;
	return normalRect.Height();
}

void CSkinItem::SetrestoredHeight(LONG newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	if (newVal <= 0)
		return;

	WINDOWPLACEMENT wndPlace;
	if (! GetSkinItemPlacementHelper(wndPlace))
		return;

	CRect normalRect = wndPlace.rcNormalPosition;
	if (normalRect.Height() == newVal)
		return;

	wndPlace.rcNormalPosition.bottom = wndPlace.rcNormalPosition.top + newVal;
	SetSkinItemPlacementHelper(wndPlace);
}

void CSkinItem::Update(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: 在此添加调度处理程序代码
	UpdateSkinItem();
}

BOOL CSkinItem::UpdateFont()
{
	ASSERT(::IsWindow(m_hWnd));
	if (!::IsWindow(m_hWnd))
		return FALSE;

	CWnd* pWnd = CWnd::FromHandle(m_hWnd);
	CWindowDC wndDC(pWnd);

	BOOL bRet = m_textStyleNormal.UpdateFont(wndDC.GetSafeHdc());

	return bRet;
}

BOOL CSkinItem::Attach(HWND hWnd)
{
	ASSERT(m_hWnd == NULL);
	if (m_hWnd != NULL)
		return FALSE;

	ASSERT(hWnd != NULL && ::IsWindow(hWnd));
	if (hWnd == NULL || !::IsWindow(hWnd))
		return FALSE;

	//判断该句柄是否已被皮肤对象绑定
	CSkinItem* pDupItem = CSkinItem::FromHandlePermanent(hWnd);
	if (pDupItem != NULL){
		CSkinBase* pParent = GetSkinParent();
		ASSERT(pParent->IsKindOf(RUNTIME_CLASS(CSkinItemDialog)));
		if (pParent->IsKindOf(RUNTIME_CLASS(CSkinItemDialog))) {
			((CSkinItemDialog*)pParent)->OnDupSkinItem(hWnd, this);
		}
		return FALSE;
	}

	m_hWnd = (HWND)hWnd;
	m_pOldWndProc = (WNDPROC)::SetWindowLong( m_hWnd, GWL_WNDPROC, (LONG)_WndProc );
	s_mapSkinItem.insert(pair<HWND, CSkinItem*>(m_hWnd, this));

	return TRUE;
}

HWND CSkinItem::Detach(void)
{
	if (m_hWnd == NULL)
		return NULL;

	HWND oldWnd = m_hWnd;

	ASSERT( ::IsWindow(m_hWnd) );
	if (::IsWindow(m_hWnd)){
		WNDPROC proc = (WNDPROC)::SetWindowLong( m_hWnd, GWL_WNDPROC, (LONG)m_pOldWndProc );
		//ASSERT(proc == CSkinItem::_WndProc);
	}

	//
	map<HWND, CSkinItem*>::const_iterator finded = s_mapSkinItem.find(m_hWnd);
	ASSERT(finded != s_mapSkinItem.end());
	s_mapSkinItem.erase(m_hWnd);

	m_pOldWndProc = NULL;
	m_hWnd = NULL;

	return oldWnd;
}

BOOL CSkinItem::UpdateSkinRgn(LONG width, LONG height, BOOL bReDraw)
{
    SKINITEM_GUARD(UpdateRgn);

	HRGN newRgn = CRenderEngine::CreateUpDownRoundRectRgn(width, height, 
		m_nUpCornerRadius, m_nDownCornerRadius);

	if (m_hSkinRgn != NULL){
		::DeleteObject(m_hSkinRgn);
		m_hSkinRgn = NULL;
	}

	BOOL bWnd = ::IsWindow(m_hWnd);
	if (bWnd){
		if (newRgn != NULL){
			m_hSkinRgn = ::CreateRectRgnIndirect(CRect(0, 0, 0, 0));
			::CombineRgn(m_hSkinRgn, newRgn, NULL, RGN_COPY);
		}
        //判断原来区域是不是为空
        HRGN hTemp = ::CreateRectRgnIndirect(CRect(0, 0, 0, 0));
        int ret = ::GetWindowRgn(m_hWnd, hTemp);
        ::DeleteObject(hTemp);
        if (ret != ERROR || newRgn != NULL)
		    ::SetWindowRgn(m_hWnd, newRgn, bReDraw);
	} else {
		m_hSkinRgn = newRgn;
		newRgn = NULL;
	}

	return bWnd;
}

BOOL CSkinItem::IsFocused()
{
    if (! ::IsWindow(m_hWnd))
        return FALSE;

    return ::GetFocus() == m_hWnd;
}

BOOL CSkinItem::IsHot()
{
    return m_bMouseEntered;
}

BOOL CSkinItem::GetSkinItemLongHelper(int nIndex, DWORD nLong, BOOL& bVal)
{
	bVal = FALSE;

	if (! ::IsWindow(m_hWnd))
		return FALSE;

	DWORD l = ::GetWindowLong(m_hWnd, nIndex);

	bVal = ((l & nLong) == nLong);

	return TRUE;
}

BOOL CSkinItem::SetSkinItemLongHelper(int nIndex, DWORD nLong, BOOL bVal)
{
	if (! ::IsWindow(m_hWnd))
		return FALSE;

	//判断时否相同
	bVal = (bVal != FALSE);
	DWORD oldLong = ::GetWindowLong(m_hWnd, nIndex);
	BOOL bTrue = (oldLong & nLong) == nLong;
	if (bVal == bTrue)
		return TRUE;

	//设置新的
	//有些属性需要通过SetWindowPos或ShowWindow或EnableWindow来设置
	if ((nIndex == GWL_EXSTYLE) && ((nLong&WS_EX_TOPMOST) != 0)){
		if (bVal) {
			::SetWindowPos(m_hWnd, HWND_TOPMOST, 0, 0, 0, 0, 
				SWP_NOACTIVATE | SWP_NOCOPYBITS | SWP_NOMOVE | 
				SWP_NOOWNERZORDER | SWP_NOREPOSITION | SWP_NOREDRAW | 
				SWP_NOSENDCHANGING | SWP_NOSIZE /*| SWP_NOZORDER*/);
		} else {
			::SetWindowPos(m_hWnd, HWND_NOTOPMOST, 0, 0, 0, 0, 
				SWP_NOACTIVATE | SWP_NOCOPYBITS | SWP_NOMOVE | 
				SWP_NOOWNERZORDER | SWP_NOREPOSITION | SWP_NOREDRAW | 
				SWP_NOSENDCHANGING | SWP_NOSIZE /*| SWP_NOZORDER*/);
		}
	}

	if ((nIndex == GWL_STYLE) && ((nLong&WS_VISIBLE) != 0)){
		if (bVal) {
			::ShowWindow(m_hWnd, SW_SHOW);
		} else {
            ::ShowWindow(m_hWnd, SW_HIDE);
        }
        if (nLong == WS_VISIBLE)
            return TRUE;
    }
	if ((nIndex == GWL_STYLE) && ((nLong&WS_DISABLED) != 0)){
		if (bVal) {
			::EnableWindow(m_hWnd, FALSE);
		} else {
            ::EnableWindow(m_hWnd, TRUE);
        }
    }

	//一般属性
	DWORD newLong = oldLong;
	if (bVal)
		newLong |= nLong;
	else
		newLong &= (~nLong);
	::SetWindowLong(m_hWnd, nIndex, newLong);

    if ((nIndex == GWL_STYLE) && ((nLong&WS_CAPTION) != 0)){
        ::SetWindowPos(m_hWnd, NULL, 0, 0, 0, 0, 
            SWP_NOSIZE | SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE | SWP_NOSENDCHANGING | SWP_NOREPOSITION | SWP_FRAMECHANGED);
    }

    //if ((nIndex == GWL_STYLE) && ((nLong&WS_BORDER) != 0)){
    //    ::SetWindowPos(m_hWnd, NULL, 0, 0, 0, 0, 
    //        SWP_NOSIZE | SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE | SWP_NOSENDCHANGING | SWP_NOREPOSITION | SWP_FRAMECHANGED);
    //}

    //if ((nIndex == GWL_STYLE) && ((nLong&WS_DLGFRAME) != 0)){
    //    ::SetWindowPos(m_hWnd, NULL, 0, 0, 0, 0, 
    //        SWP_NOSIZE | SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE | SWP_NOSENDCHANGING | SWP_NOREPOSITION | SWP_FRAMECHANGED);
    //}

	//验证设置是否成功
	newLong = ::GetWindowLong(m_hWnd, nIndex);
	bTrue = (newLong & nLong) == nLong;

	return (bVal == bTrue);
}

BOOL CSkinItem::GetSkinItemPlacementHelper(WINDOWPLACEMENT& wndPlace)
{
	if (! ::IsWindow(m_hWnd))
		return FALSE;

	wndPlace.length = sizeof(WINDOWPLACEMENT);
	return ::GetWindowPlacement(m_hWnd, &wndPlace);
}

BOOL CSkinItem::SetSkinItemPlacementHelper(const WINDOWPLACEMENT& wndPlace)
{
	if (! ::IsWindow(m_hWnd))
		return FALSE;

	const_cast<WINDOWPLACEMENT&>(wndPlace).length = sizeof(WINDOWPLACEMENT);
	return ::SetWindowPlacement(m_hWnd, &wndPlace);
}

HWND CSkinItem::GetChildItemHelper(HWND hParentWnd, LONG nID, LPCTSTR wndClassName, LPCTSTR wndText, POINT pt, UINT uFindFlag)
{
	if (! ::IsWindow(hParentWnd))
		return NULL;

	//直接获得对话框子窗口
	if (uFindFlag == FIND_HWND_CTRL_ID)
		return GetDlgItem(hParentWnd, nID);

	EnumChildProcStruct param;
	param.hParentWnd = hParentWnd;
	param.nID = nID;
	param.wndClassName = wndClassName;
	param.wndText = wndText;
	param.pt = pt;
	param.uFindFlag = uFindFlag;
	param.hResultWnd = NULL;
	EnumChildWindows(hParentWnd, (WNDENUMPROC)_EnumChildProc, (LPARAM)(&param));

	return param.hResultWnd;
}

BOOL CSkinItem::GetAllChildRgnHelper(HWND hParent, HRGN& hRgn, BOOL bVisible)
{
	if (!::IsWindow(hParent))
		return FALSE;

	if (hRgn != NULL) {
		::DeleteObject(hRgn);
		hRgn = NULL;
	}

	HWND hNextChild = NULL;
	hNextChild = ::GetWindow(hParent, GW_CHILD);
	while (hNextChild != NULL) {
		if (::IsWindowVisible(hNextChild) || (!bVisible)) {
			if (hRgn == NULL) {
				hRgn = ::CreateRectRgn(0, 0, 0, 0);
			}
			WINDOWPLACEMENT wndPlace;
			wndPlace.length = sizeof(WINDOWPLACEMENT);
			if (::GetWindowPlacement(hNextChild, &wndPlace)) {
				HRGN hTempRgn = ::CreateRectRgnIndirect( &(wndPlace.rcNormalPosition) );
				::CombineRgn(hRgn, hRgn, hTempRgn, RGN_OR);
				::DeleteObject(hTempRgn);
			}
		}
		hNextChild = ::GetWindow(hNextChild, GW_HWNDNEXT);
	}

	return TRUE;
}

BOOL CSkinItem::_EnumChildProc(HWND hWnd, LPARAM lParam)
{
	EnumChildProcStruct* pParam = (EnumChildProcStruct*)lParam;
	ASSERT(pParam != NULL);
	if (pParam == NULL)
		return TRUE;

	CWnd* pWnd = CWnd::FromHandle(hWnd);
	ASSERT(pWnd != NULL);
	if (pWnd == NULL)
		return TRUE;

	if ((pParam->uFindFlag&FIND_HWND_CTRL_ID) &&
		pParam->nID != GetDlgCtrlID(hWnd) )
		return TRUE;

	TCHAR className[256] = {0};
	::GetClassName(hWnd, className, 255);
	CString strClassName = className;
	if ((pParam->uFindFlag&FIND_HWND_WND_CLASS_TEXT) &&
		strClassName.Compare(pParam->wndClassName) != 0 )
		return TRUE;

	CString strWndText;
	pWnd->GetWindowText(strWndText);
	if ((pParam->uFindFlag&FIND_HWND_WND_CLASS_TEXT) &&
		strWndText.Compare(pParam->wndText) != 0 )
		return TRUE;

	CRect wndRect;
	::GetWindowRect(hWnd, &wndRect);
	CRect parentRect;
	::GetWindowRect(pParam->hParentWnd, &parentRect);
	wndRect.OffsetRect(-parentRect.left, -parentRect.top);
	if ((pParam->uFindFlag&FIND_HWND_WND_POS) &&
		! wndRect.PtInRect(pParam->pt))
		return TRUE;

	pParam->hResultWnd = hWnd;
	return FALSE;
}

BSTR CSkinItem::GetText()
{
    if (! IsWindow(m_hWnd))
        return CSkinBase::GetText();

    CWnd* pWnd = CWnd::FromHandle(m_hWnd);
    CString wndText;
    pWnd->GetWindowText(wndText);
    return wndText.AllocSysString();
}

void CSkinItem::SetText(LPCTSTR newVal)
{
    if (IsWindow(m_hWnd)) {
        CWnd* pWnd = CWnd::FromHandle(m_hWnd);
        pWnd->SetWindowText(newVal);
    }

    CSkinBase::SetText(newVal);
}

BOOL CSkinItem::GetVisible()
{
	if (! ::IsWindow(m_hWnd))
		return CSkinBase::GetVisible();

	BOOL bTrue;
	GetSkinItemLongHelper(GWL_STYLE, WS_VISIBLE, bTrue);
	return bTrue;
}

void CSkinItem::SetVisible(BOOL newVal)
{
	newVal = (newVal != FALSE);
	m_bVisible = newVal;
	SetSkinItemLongHelper(GWL_STYLE, WS_VISIBLE, m_bVisible);
}

BOOL CSkinItem::GetDisabled()
{
	if (! ::IsWindow(m_hWnd))
		return CSkinBase::GetDisabled();

	BOOL bTrue;
	GetSkinItemLongHelper(GWL_STYLE, WS_DISABLED, bTrue);
	return bTrue;
}

void CSkinItem::SetDisabled(BOOL newVal)
{
	newVal = (newVal != FALSE);
	m_bDisabled = newVal;
	SetSkinItemLongHelper(GWL_STYLE, WS_DISABLED, m_bDisabled);
	InvalidateRect();
}

BOOL CSkinItem::GetTabStop()
{
	BOOL bTrue;
	GetSkinItemLongHelper(GWL_STYLE, WS_TABSTOP, bTrue);
	return bTrue;
}

void CSkinItem::SetTabStop(BOOL newVal)
{
	newVal = (newVal != FALSE);
	m_bTabStop = newVal;
	SetSkinItemLongHelper(GWL_STYLE, WS_TABSTOP, m_bTabStop);
}

//skin坐标转屏幕坐标
BOOL CSkinItem::SkinToScreen(POINT& point)
{
	ASSERT(::IsWindow(m_hWnd));
	if (! ::IsWindow(m_hWnd))
		return FALSE;

	return ClientToScreen(m_hWnd, &point);
}

BOOL CSkinItem::SkinToScreen(LONG& x, LONG& y)
{
	CPoint p(x, y);
	if (! SkinToScreen(p))
		return FALSE;

	x = p.x;
	y = p.y;

	return TRUE;
}

BOOL CSkinItem::SkinToScreen(RECT& rect)
{
	ASSERT(::IsWindow(m_hWnd));
	if (! ::IsWindow(m_hWnd))
		return FALSE;
	
	CPoint p(0, 0);
	if (! ClientToScreen(m_hWnd, &p))
		return FALSE;

	::OffsetRect(&rect, -p.x, -p.y);

	return TRUE;
}

//屏幕坐标转skin坐标
BOOL CSkinItem::ScreenToSkin(POINT& point)
{
	ASSERT(::IsWindow(m_hWnd));
	if (! ::IsWindow(m_hWnd))
		return FALSE;

	return ScreenToClient(m_hWnd, &point);
}

BOOL CSkinItem::ScreenToSkin(LONG& x, LONG& y)
{
	CPoint p(x, y);
	if (! ScreenToSkin(p))
		return FALSE;

	x = p.x;
	y = p.y;

	return TRUE;
}

BOOL CSkinItem::ScreenToSkin(RECT& rect)
{
	ASSERT(::IsWindow(m_hWnd));
	if (! ::IsWindow(m_hWnd))
		return FALSE;

	CPoint p(0, 0);
	if (! ScreenToClient(m_hWnd, &p))
		return FALSE;

	::OffsetRect(&rect, p.x, p.y);

	return TRUE;
}

BOOL CSkinItem::UpdateSkinItem()
{
	//更新句柄
	if (! UpdateSkinItemHwnd())
        return FALSE;

	//更新属性
	UpdateSkinItemProperty();

    //触发脚本事件
    CSkinBase::OnUpdateHwnd();

	return TRUE;
}

BOOL CSkinItem::UpdateSkinItemHwnd()
{
	HWND hWnd = NULL;
	CSkinBase* pParent = GetSkinParent();
	switch (m_enFindHwnd) 
	{
	case FIND_HWND_NONE:
		hWnd = NULL;
		break;
	case FIND_HWND_HWND:
		hWnd = m_hFindHwnd;
		break;
	case FIND_HWND_CTRL_ID:
		if (pParent != NULL) {
			hWnd = GetChildItemHelper(pParent->GetSkinHwnd(), 
				GetitemID(), NULL, NULL, m_ptFindPoint, FIND_HWND_CTRL_ID);
		}
		break;
	case FIND_HWND_WND_CLASS_TEXT:
		if (pParent != NULL) {
			hWnd = GetChildItemHelper(pParent->GetSkinHwnd(), 
				GetitemID(), m_strWndClassName, m_strText, m_ptFindPoint, FIND_HWND_WND_CLASS_TEXT);
		}
		break;
	case FIND_HWND_WND_POS:
		if (pParent != NULL) {
			hWnd = GetChildItemHelper(pParent->GetSkinHwnd(), 
				0, NULL, NULL, m_ptFindPoint, FIND_HWND_WND_POS);
		}
		break;
	}

    if (hWnd == NULL)
        return FALSE;

	Detach();

 #ifdef CUSTOM_SCROLLBAR_SUPPORT
    if (m_bSkinScrollBar) {
        HWND scrollBarHwnd = hWnd;
        if (m_lSkinScrollBarChild != -1) {
            LONG generation = m_lSkinScrollBarChild;
            while (generation > 0) {
                scrollBarHwnd = ::GetWindow(scrollBarHwnd, GW_CHILD);
                generation --;
            }
        } else {
            HWND hChild = scrollBarHwnd;
            while (hChild) {
                hChild = ::GetWindow(hChild, GW_CHILD);
                if (hChild)
                    scrollBarHwnd = hChild;
            }
        }
        InitializeCoolSB(scrollBarHwnd);
        CoolSB_SetStyle(scrollBarHwnd, SB_VERT, CSBS_HOTTRACKED);
        CoolSB_SetStyle(scrollBarHwnd, SB_HORZ, CSBS_HOTTRACKED);
        CoolSB_SetSize(scrollBarHwnd, SB_VERT, m_lSkinScrollBarHeight, m_lSkinScrollBarWidth);
        CoolSB_SetSize(scrollBarHwnd, SB_HORZ, m_lSkinScrollBarHeight, m_lSkinScrollBarWidth);
        CoolSB_SetMinThumbSize(scrollBarHwnd, SB_VERT, 20);
        CoolSB_SetMinThumbSize(scrollBarHwnd, SB_HORZ, 20);
    }
#endif

	return Attach(hWnd);
}

BOOL CSkinItem::UpdateSkinItemProperty()
{
	//ASSERT(::IsWindow(m_hWnd));
	if (!::IsWindow(m_hWnd))
		return FALSE;

	if (m_buseOriginPosition) {
        CRect rect = CRect(0, 0, 0, 0);
        GetSkinItemRectHelper(rect);
        m_rcSkinRect = rect;
    } else {
        MoveWindow(&m_rcSkinRect, FALSE);
    }

    if (m_buseOriginText) {
        GetSkinItemTextHelper(m_strText);
    } else {
        SetSkinItemTextHelper(m_strText);
    }

	return TRUE;
}

CSkinItem* CSkinItem::FromHandlePermanent(HWND hWnd)
{
	map<HWND, CSkinItem*>::const_iterator finded = s_mapSkinItem.find(hWnd);
	if (finded != s_mapSkinItem.end()){
		return finded->second;
	}
	return NULL;
}

CSkinItem* CSkinItem::FromHandle(HWND hWnd, CSkinEngine* pEngine)
{
	CSkinItem* pItem = CSkinItem::FromHandlePermanent(hWnd);
	if (pItem != NULL)
		return pItem;

	if (pEngine != NULL)
		pItem = (CSkinItem*) pEngine->CreateSkinByClassName(_T("CSkinItem"));
	ASSERT(pItem != NULL);
	if (pItem == NULL)
		return NULL;

	pItem->Sethwnd((LONG)hWnd);
	return pItem;
}

void CSkinItem::Visit(ICSkinItemVisitor* pVisitor)
{
	map<HWND, CSkinItem*>::const_iterator iter = s_mapSkinItem.begin(),
		last = s_mapSkinItem.end();
	for (; iter!=last; iter++) {
		pVisitor->Visit(iter->second);
	}
}

//过滤消息
BOOL CSkinItem::PreTranslateMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    if (uMsg == WM_KEYDOWN) {
        CSkinBase* pFocus = NULL;
        BOOL bGetFocus = GetSkinFocus(&pFocus);
        if (! bGetFocus || pFocus == NULL)
            return FALSE;

        switch (wParam)
        {
        case VK_TAB:
            {
                if (pFocus->OnKeyDown((UINT)wParam, (UINT)lParam))
                    return TRUE;

                //查看SHIFT键是否按下
                SHORT keyState = ::GetKeyState(VK_SHIFT);
                BOOL bShiftDown = (keyState < 0);
                CSkinBase* pNext = NULL;
                pNext = pFocus->GetNextTabStopSkinInSkinTree(!bShiftDown);
                if (pNext != NULL)
                    pNext->SetFocusSkin();
                return TRUE;
            }
            break;
        case VK_UP:
            {
                if (pFocus->OnKeyDown((UINT)wParam, (UINT)lParam))
                    return TRUE;

                CSkinBase* pNext = NULL;
                pNext = pFocus->GetNextTabStopSkinInSkinTree(FALSE);
                if (pNext != NULL)
                    pNext->SetFocusSkin();
                return TRUE;
            }
            break;
        case VK_DOWN:
            {
                if (pFocus->OnKeyDown((UINT)wParam, (UINT)lParam))
                    return TRUE;

                CSkinBase* pNext = NULL;
                pNext = pFocus->GetNextTabStopSkinInSkinTree(TRUE);
                if (pNext != NULL)
                    pNext->SetFocusSkin();
                return TRUE;
            }
            break;
        case VK_RETURN:
            {
                if (pFocus->OnKeyDown((UINT)wParam, (UINT)lParam))
                    return TRUE;

                return FALSE;
            }
            break;
        }
    }

    return FALSE;
}

LRESULT CALLBACK CSkinItem::_WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	CSkinItem* pItem = CSkinItem::FromHandlePermanent(hWnd);
	ASSERT(pItem != NULL);
	if (pItem == NULL)
		return S_OK;

	return pItem->WndProc(uMsg, wParam, lParam);
}

LRESULT CSkinItem::WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    SKINITEM_STACK_GUARD(uMsg, wParam, lParam);

	LRESULT lResult = 0;
	BOOL bProcessed = FALSE;

	switch (uMsg)
	{
	case WM_IM_NOTIFY:
		bProcessed = OnSkinNotify(uMsg, wParam, lParam, &lResult);
		break;

	case WM_PAINT:
        if (m_bUseSkinPaint)
		    bProcessed = OnPaint(uMsg, wParam, lParam, &lResult);
		break;

    case WM_SYNCPAINT:
        bProcessed = OnSyncPaint(uMsg, wParam, lParam, &lResult);
        break;

    case WM_ENTERSIZEMOVE:
        bProcessed = OnEnterSizeMove(uMsg, wParam, lParam, &lResult);
        break;

    case WM_EXITSIZEMOVE:
        bProcessed = OnExitSizeMove(uMsg, wParam, lParam, &lResult);
        break;

	case WM_SIZING:
		bProcessed = OnSizing(uMsg, wParam, lParam, &lResult);
		break;

	case WM_SIZE:
		bProcessed = OnSize(uMsg, wParam, lParam, &lResult);
		break;

	case WM_CREATE:
		bProcessed = OnCreate(uMsg, wParam, lParam, &lResult);
		break;

    case WM_NCCREATE:
        bProcessed = OnNcCreate(uMsg, wParam, lParam, &lResult);
        break;

    case WM_CLOSE:
        bProcessed = OnClose(uMsg, wParam, lParam, &lResult);
        break;

	case WM_DESTROY:
		bProcessed = OnDestroy(uMsg, wParam, lParam, &lResult);
		break;

	case WM_NCDESTROY:
		bProcessed = OnNcDestroy(uMsg, wParam, lParam, &lResult);
		break;

	case WM_WINDOWPOSCHANGING:
		bProcessed = OnWindowPosChanging(uMsg, wParam, lParam, &lResult);
		break;

	case WM_WINDOWPOSCHANGED:
		bProcessed = OnWindowPosChanged(uMsg, wParam, lParam, &lResult);
		break;

	case WM_MOVING:
		bProcessed = OnMoving(uMsg, wParam, lParam, &lResult);
		break;

	case WM_MOVE:
		bProcessed = OnMove(uMsg, wParam, lParam, &lResult);
		break;

	case WM_NOTIFY:
		bProcessed = OnNotify(uMsg, wParam, lParam, &lResult);
		break;

	case WM_SETCURSOR:
		bProcessed = OnSetCursor(uMsg, wParam, lParam, &lResult);
		break;

	case WM_NCPAINT:
        if (m_bUseSkinPaint)
    		bProcessed = OnNcPaint(uMsg, wParam, lParam, &lResult);
		break;

	case WM_ACTIVATEAPP:
		lResult = 0;
		bProcessed = TRUE;
		break;

	case WM_NCACTIVATE:
		bProcessed = OnNcActivate(uMsg, wParam, lParam, &lResult);
		break;

	case WM_NCHITTEST:
		bProcessed = OnNcHitTest(uMsg, wParam, lParam, &lResult);
		break;

	case WM_NCCALCSIZE:
		bProcessed = OnNcCalcSize(uMsg, wParam, lParam, &lResult);
		break;

	case WM_NCLBUTTONDOWN:
		bProcessed = OnNcLButtonDown(uMsg, wParam, lParam, &lResult);
		break;

	case WM_NCLBUTTONDBLCLK:
		bProcessed = OnNcLButtonDblClk(uMsg, wParam, lParam, &lResult);
		break;

    case WM_NCLBUTTONUP:
        bProcessed = OnNcLButtonUp(uMsg, wParam, lParam, &lResult);
        break;

    case WM_NCRBUTTONDOWN:
        bProcessed = OnNcRButtonDown(uMsg, wParam, lParam, &lResult);
        break;

    case WM_NCRBUTTONDBLCLK:
        bProcessed = OnNcRButtonDblClk(uMsg, wParam, lParam, &lResult);
        break;

    case WM_NCRBUTTONUP:
        bProcessed = OnNcRButtonUp(uMsg, wParam, lParam, &lResult);
        break;

	case WM_NCMOUSEMOVE:
		bProcessed = OnNcMouseMove(uMsg, wParam, lParam, &lResult);
		break;

	case WM_NCMOUSELEAVE:
		bProcessed = OnNcMouseLeave(uMsg, wParam, lParam, &lResult);
		break;

	case WM_NCMOUSEHOVER:
		bProcessed = OnNcMouseHover(uMsg, wParam, lParam, &lResult);
		break;

	case WM_CAPTURECHANGED:
		bProcessed = OnCaptureChanged(uMsg, wParam, lParam, &lResult);
		break;

	case WM_LBUTTONDOWN:
		bProcessed = OnLButtonDown(uMsg, wParam, lParam, &lResult);
		break;

	case WM_LBUTTONUP:
		bProcessed = OnLButtonUp(uMsg, wParam, lParam, &lResult);
		break;

    case WM_LBUTTONDBLCLK:
        bProcessed = OnLButtonDblClk(uMsg, wParam, lParam, &lResult);
        break;

    case WM_RBUTTONDOWN:
        bProcessed = OnRButtonDown(uMsg, wParam, lParam, &lResult);
        break;

    case WM_RBUTTONUP:
        bProcessed = OnRButtonUp(uMsg, wParam, lParam, &lResult);
        break;

    case WM_RBUTTONDBLCLK:
        bProcessed = OnRButtonDblClk(uMsg, wParam, lParam, &lResult);
        break;

	case WM_MOUSEMOVE:
		bProcessed = OnMouseMove(uMsg, wParam, lParam, &lResult);
		break;

    case WM_MOUSEENTER:
        bProcessed = OnMouseEnter(uMsg, wParam, lParam, &lResult);
        break;

	case WM_MOUSELEAVE:
		bProcessed = OnMouseLeave(uMsg, wParam, lParam, &lResult);
		break;

	case WM_MOUSEHOVER:
		bProcessed = OnMouseHover(uMsg, wParam, lParam, &lResult);
		break;

	case WM_GETMINMAXINFO:
		bProcessed = OnGetMinMaxInfo(uMsg, wParam, lParam, &lResult);
		break;

	case WM_INITDIALOG:
		bProcessed = OnInitDialog(uMsg, wParam, lParam, &lResult);
		break;

	case WM_TIMER:
		bProcessed = OnTimer(uMsg, wParam, lParam, &lResult);
		break;

	case WM_CTLCOLORMSGBOX:
	case WM_CTLCOLOREDIT:
	case WM_CTLCOLORLISTBOX:
	case WM_CTLCOLORBTN:
	case WM_CTLCOLORDLG:
	case WM_CTLCOLORSCROLLBAR:
	case WM_CTLCOLORSTATIC:
		bProcessed = OnCtlColor(uMsg, wParam, lParam, &lResult);
		break;

	case WM_ERASEBKGND:
		bProcessed = OnEraseBkgnd(uMsg, wParam, lParam, &lResult);
		break;

	case WM_GETICON:
		bProcessed = OnGetIcon(uMsg, wParam, lParam, &lResult);
		break;

	case WM_SETICON:
		bProcessed = OnSetIcon(uMsg, wParam, lParam, &lResult);
		break;

	case WM_GETTEXTLENGTH:
		bProcessed = OnGetTextLength(uMsg, wParam, lParam, &lResult);
		break;

	case WM_GETTEXT:
		bProcessed = OnGetText(uMsg, wParam, lParam, &lResult);
		break;

	case WM_SETTEXT:
		bProcessed = OnSetText(uMsg, wParam, lParam, &lResult);
		break;

    case WM_THEMECHANGED:
        bProcessed = OnThemeChanged(uMsg, wParam, lParam, &lResult);
        break;

	case WM_STYLECHANGING:
		bProcessed = OnStyleChanging(uMsg, wParam, lParam, &lResult);
		break;

	case WM_STYLECHANGED:
		bProcessed = OnStyleChanged(uMsg, wParam, lParam, &lResult);
		break;

	case WM_PARENTNOTIFY:
		bProcessed = OnParantNotify(uMsg, wParam, lParam, &lResult);
		break;

	case WM_SHOWWINDOW:
		bProcessed = OnShowWindow(uMsg, wParam, lParam, &lResult);
		break;

	case WM_ENABLE:
		bProcessed = OnEnable(uMsg, wParam, lParam, &lResult);
		break;

	case WM_COMMAND:
		bProcessed = OnCommand(uMsg, wParam, lParam, &lResult);
		break;

    case WM_SYSCOMMAND:
        bProcessed = OnSysCommand(uMsg, wParam, lParam, &lResult);
        break;

	case WM_KEYDOWN:
		bProcessed = OnKeyDown(uMsg, wParam, lParam, &lResult);
		break;

	case WM_CHAR:
		bProcessed = OnChar(uMsg, wParam, lParam, &lResult);
		break;

	case WM_KEYUP:
		bProcessed = OnKeyUp(uMsg, wParam, lParam, &lResult);
		break;

	case WM_ACTIVATE:
		bProcessed = OnActivate(uMsg, wParam, lParam, &lResult);
		break;

	case WM_SETFOCUS:
		bProcessed = OnSetFocus(uMsg, wParam, lParam, &lResult);
		break;

	case WM_KILLFOCUS:
		bProcessed = OnKillFocus(uMsg, wParam, lParam, &lResult);
		break;

	case WM_DRAWITEM:
        if (m_bUseSkinPaint)
		    bProcessed = OnDrawItem(uMsg, wParam, lParam, &lResult);
		break;

	case WM_MEASUREITEM:
        bProcessed = OnMeasureItem(uMsg, wParam, lParam, &lResult);
		break;

    case WM_UPDATEUISTATE:
        bProcessed = OnUpdateUIState(uMsg, wParam, lParam, &lResult);
        break;

    //Button
    case BM_SETSTATE:
        bProcessed = OnBMSetState(uMsg, wParam, lParam, &lResult);
        break;

    case BM_SETCHECK:
        bProcessed = OnBMSetCheck(uMsg, wParam, lParam, &lResult);
        break;

    //ListView
    case LVM_SETITEMSTATE:
        bProcessed = OnLVMSetItemState(uMsg, wParam, lParam, &lResult);
        break;

	default:
		bProcessed = FALSE;
		break;
	}

	//未处理则调用默认处理函数
	if (bProcessed){
		return lResult;
	}else{
		return DefWndProc(uMsg, wParam, lParam);
	}
}

LRESULT CSkinItem::DefWndProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	return ::CallWindowProc(m_pOldWndProc, m_hWnd, uMsg, wParam, lParam);
}

BOOL CSkinItem::CtlColor(CDC* pDC, CWnd* pWnd, HBRUSH& hBrush)
{
    if (m_bSelfOnCtlColor)
        return FALSE;

	hBrush = NULL;
	BOOL bCtlColor = FALSE;
	CRect skinRect = GetSkinAbsRect();
	m_brushBk.DeleteObject();
	if ( ! GetDisabled()){
		if (m_clrBkColorNormal != -1){
            LONG transedColor = m_clrBkColorNormal;
            if (m_bColorTransform && m_bBkColorTransform)
                GetEngine()->TransformColor(transedColor, transedColor);
			pDC->SetBkColor(transedColor);
			m_brushBk.CreateSolidBrush(transedColor);
			bCtlColor = TRUE;
		}
        if (GettextColorNormal() != -1) {
            pDC->SetTextColor(GettextColorNormal());
        }
		if (m_pbkImageNormal != NULL){
			m_pbkImageNormal->Draw(*pDC, &skinRect, NULL, GetImageParam());
		}
	}else{
		if (m_clrBkColorDisabled != -1){
            LONG transedColor = m_clrBkColorDisabled;
            if (m_bColorTransform && m_bBkColorTransform)
                GetEngine()->TransformColor(transedColor, transedColor);
            pDC->SetBkColor(transedColor);
			m_brushBk.CreateSolidBrush(transedColor);
			bCtlColor = TRUE;
		}
        if (GettextColorDisabled() != -1) {
            pDC->SetTextColor(GettextColorDisabled());
        }
		if (m_pbkImageDisabled != NULL){
			m_pbkImageDisabled->Draw(*pDC, &skinRect, NULL, GetImageParam());
		}
	}
	hBrush = m_brushBk;
	return bCtlColor;
}

void CSkinItem::DrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	
}

void CSkinItem::MeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct)
{
	
}

BOOL CSkinItem::OnSkinNotify(UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* lpResult)
{
	return FALSE;
}

BOOL CSkinItem::OnPaint(UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* lpResult)
{
	return !GetselfOnPaint();
}

BOOL CSkinItem::OnSyncPaint(UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* lpResult)
{
	*lpResult = 0;
    return TRUE;
}

BOOL CSkinItem::OnEnterSizeMove(UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* lpResult)
{
    SKINITEM_STACK_FRAME frame = {0};
    BOOL bSize = FALSE;
    BOOL bSuc = StackTrace(1, frame);
    ASSERT(bSuc);
    bSize = (frame.uMsg == WM_SYSCOMMAND && (frame.wParam & 0xFFF0) == SC_SIZE);

    CSkinBase::OnEnterSizeMove(bSize);

    return FALSE;
}

BOOL CSkinItem::OnExitSizeMove(UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* lpResult)
{
    SKINITEM_STACK_FRAME frame = {0};
    BOOL bSize = FALSE;
    BOOL bSuc = StackTrace(1, frame);
    ASSERT(bSuc);
    bSize = (frame.uMsg == WM_SYSCOMMAND && (frame.wParam & 0xFFF0) == SC_SIZE);

    CSkinBase::OnExitSizeMove(bSize);

    return FALSE;
}

BOOL CSkinItem::OnSizing(UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* lpResult)
{
	return FALSE;
}

BOOL CSkinItem::OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* lpResult)
{
    long width = LOWORD(lParam);
	long height = HIWORD(lParam);

    //更新窗口区域
    BOOL bVisible = GetVisible();
    UpdateSkinRgn(GetWidth(), GetHeight(), bVisible);
    if (bVisible)
        InvalidateRect(NULL);

    if (GetselfOnSize()) {
        *lpResult = DefWndProc(uMsg, wParam, lParam);
    }

	return TRUE;
}

BOOL CSkinItem::OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* lpResult)
{
	return FALSE;
}

BOOL CSkinItem::OnNcCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* lpResult)
{
	return FALSE;
}

BOOL CSkinItem::OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* lpResult)
{
    return FALSE;
}

BOOL CSkinItem::OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* lpResult)
{
    return FALSE;
}

BOOL CSkinItem::OnNcDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* lpResult)
{
    //从窗口树上删除自己
    CSkinProxy* pProxy = GetSkinProxy();
    if (pProxy != NULL) {
        pProxy->RemoveClient();
    } else {
        CSkinBase* pParent = GetSkinParent();
        if (pParent != NULL) {
            pParent->RemoveSubSkin(this);
        }
    }

    //调用默认窗口过程
    *lpResult = DefWndProc(uMsg, wParam, lParam);
    HWND hWnd = Detach();

#ifdef CUSTOM_SCROLLBAR_SUPPORT
    if (m_bSkinScrollBar) {
        ::UninitializeCoolSB(hWnd);
    }
#endif

    //Destroy self
    if (GetEngine() != NULL) {	//皮肤引擎未卸载
        GetEngine()->DestroySkin(this);
    }

    return TRUE;
}

BOOL CSkinItem::OnWindowPosChanging(UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* lpResult)
{
	return FALSE;
}

BOOL CSkinItem::OnWindowPosChanged(UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* lpResult)
{
	*lpResult = 0;

	WINDOWPOS* pWndPos = (WINDOWPOS*)lParam;
	ASSERT(pWndPos != NULL);
	if (pWndPos == NULL)
		return FALSE;

	//捕获窗口位置
	m_rcSkinRect.left = pWndPos->x;
	m_rcSkinRect.top = pWndPos->y;
	m_rcSkinRect.right = m_rcSkinRect.left + pWndPos->cx;
	m_rcSkinRect.bottom = m_rcSkinRect.top + pWndPos->cy;

	//由默认处理函数继续处理
	return FALSE;
}

BOOL CSkinItem::OnMoving(UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* lpResult)
{
	return FALSE;
}

BOOL CSkinItem::OnMove(UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* lpResult)
{
	return FALSE;
}

BOOL CSkinItem::OnNotify(UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* lpResult)
{
    int iID = (UINT)wParam;
    NMHDR* pNMHDR = (NMHDR*)lParam;
    HWND hwndFrom = pNMHDR->hwndFrom;
    UINT_PTR idFrom = pNMHDR->idFrom;
    UINT code = pNMHDR->code;
#ifdef CUSTOM_SCROLLBAR_SUPPORT
    if (code == NM_COOLSB_CUSTOMDRAW) {
        CSkinItem* pItem = CSkinItem::FromHandlePermanent(hwndFrom);
        BOOL bProcessed = FALSE;
        if (pItem != NULL) {
            bProcessed = pItem->OnCoolSBCustomDraw(pNMHDR, lpResult);
            if (!bProcessed) {
                bProcessed = OnCoolSBCustomDraw(pNMHDR, lpResult);
            }
        }
        return bProcessed;
    }
#endif

    if (code == NM_CUSTOMDRAW) {
        CSkinItem* pItem = CSkinItem::FromHandlePermanent(hwndFrom);
        BOOL bProcessed = FALSE;
        if (pItem != NULL) {
            bProcessed = pItem->OnCustomDraw(pNMHDR, lpResult);
            if (!bProcessed) {
                bProcessed = OnCustomDraw(pNMHDR, lpResult);
            }
        }
        return bProcessed;
    }

	return FALSE;
}

BOOL CSkinItem::OnNcHitTest(UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* lpResult)
{
	return FALSE;
}

BOOL CSkinItem::OnNcLButtonDown(UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* lpResult)
{
	return FALSE;
}

BOOL CSkinItem::OnNcLButtonDblClk(UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* lpResult)
{
	return FALSE;
}

BOOL CSkinItem::OnNcLButtonUp(UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* lpResult)
{
    return FALSE;
}

BOOL CSkinItem::OnNcRButtonDown(UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* lpResult)
{
    return FALSE;
}

BOOL CSkinItem::OnNcRButtonDblClk(UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* lpResult)
{
    return FALSE;
}

BOOL CSkinItem::OnNcRButtonUp(UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* lpResult)
{
    return FALSE;
}

BOOL CSkinItem::OnNcMouseMove(UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* lpResult)
{
	return FALSE;
}

BOOL CSkinItem::OnNcMouseLeave(UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* lpResult)
{
	return FALSE;
}

BOOL CSkinItem::OnNcMouseHover(UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* lpResult)
{
	return FALSE;
}

BOOL CSkinItem::OnNcPaint(UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* lpResult)
{
    *lpResult = 0;

	return !GetselfOnNcPaint();
}

BOOL CSkinItem::OnNcActivate(UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* lpResult)
{
	return FALSE;
}

BOOL CSkinItem::OnNcCalcSize(UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* lpResult)
{
	return FALSE;
}

BOOL CSkinItem::OnLButtonDown(UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* lpResult)
{
	LONG x = GETXPARAM(lParam);
	LONG y = GETYPARAM(lParam);
	UINT flag = (UINT)wParam;

    //背景拖动支持
    HWND hAncestor = ::GetAncestor(m_hWnd, GA_ROOT);
    if (m_bBkDrag && !::IsZoomed(hAncestor)) {
        ::SetCapture(m_hWnd);
        m_bDragging = TRUE;
        m_ptDragPoint = CPoint(x, y);
        m_ptOldDragPoint = m_ptDragPoint;
        SkinToScreen(m_ptDragPoint);
        ::ScreenToClient(hAncestor, &m_ptDragPoint);
    }

	CSkinBase::OnLButtonDown(x, y, flag);

	return FALSE;
}

BOOL CSkinItem::OnLButtonUp(UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* lpResult)
{
    LONG x = GETXPARAM(lParam);
    LONG y = GETYPARAM(lParam);
    UINT flag = (UINT)wParam;

    //背景拖动支持
    HWND hAncestor = ::GetAncestor(m_hWnd, GA_ROOT);
    if (m_bDragging) {
        if (::GetCapture() == m_hWnd) {
            ::ReleaseCapture();
        }
        m_bDragging = FALSE;
        m_ptDragPoint = CPoint(-1, -1);
    }

    return FALSE;
}

BOOL CSkinItem::OnLButtonDblClk(UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* lpResult)
{
    return FALSE;
}

BOOL CSkinItem::OnRButtonDown(UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* lpResult)
{
    return FALSE;
}

BOOL CSkinItem::OnRButtonUp(UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* lpResult)
{
    return FALSE;
}

BOOL CSkinItem::OnRButtonDblClk(UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* lpResult)
{
    return FALSE;
}

BOOL CSkinItem::OnMouseMove(UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* lpResult)
{
    if (m_bTrackMouse) {
        TRACKMOUSEEVENT tme;
        tme.cbSize = sizeof(tme);
        tme.dwFlags  = TME_HOVER | TME_LEAVE;
        tme.hwndTrack = m_hWnd;
        tme.dwHoverTime = HOVER_DEFAULT;
        ::_TrackMouseEvent(&tme);
    }

    //背景拖动支持
    LONG x = GETXPARAM(lParam);
    LONG y = GETYPARAM(lParam);
    UINT flag = (UINT)wParam;
    HWND hAncestor = ::GetAncestor(m_hWnd, GA_ROOT);
    SHORT lbState = ::GetAsyncKeyState(VK_LBUTTON);
    BOOL bLButtonDown = (lbState & 0x8001) != 0;
    if (m_bDragging && bLButtonDown) {
        CPoint curPoint(x, y);
        if (curPoint != m_ptOldDragPoint) {
            SkinToScreen(curPoint);
            CRect ancestorRect;
            ::GetWindowRect(hAncestor, &ancestorRect);
            ::OffsetRect(&ancestorRect, -ancestorRect.left, -ancestorRect.top);
            ::OffsetRect(&ancestorRect, curPoint.x, curPoint.y);
            ::OffsetRect(&ancestorRect, -m_ptDragPoint.x, -m_ptDragPoint.y);
            ::SendMessage(hAncestor, WM_MOVING, 0, (LPARAM)(&ancestorRect));
            ::SetWindowPos(hAncestor, NULL, ancestorRect.left, ancestorRect.top, ancestorRect.Width(), ancestorRect.Height(), 0);
        }
        m_ptOldDragPoint = curPoint;
    }

    if (! m_bMouseEntered) {
        ::SendMessage(m_hWnd, WM_MOUSEENTER, wParam, lParam);
    }

    return FALSE;
}

BOOL CSkinItem::OnMouseEnter(UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* lpResult)
{
    m_bMouseEntered = TRUE;
    return FALSE;
}

BOOL CSkinItem::OnMouseLeave(UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* lpResult)
{
    m_bMouseEntered = FALSE;
	CSkinBase::OnMouseLeave();
	return FALSE;
}

BOOL CSkinItem::OnMouseHover(UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* lpResult)
{
	return FALSE;
}

BOOL CSkinItem::OnGetMinMaxInfo(UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* lpResult)
{
	return FALSE;
}

BOOL CSkinItem::OnSetCursor(UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* lpResult)
{
	return FALSE;
}

BOOL CSkinItem::OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* lpResult)
{
	return FALSE;
}

BOOL CSkinItem::OnCtlColor(UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* lpResult)
{
	return FALSE;
}

BOOL CSkinItem::OnTimer(UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* lpResult)
{
	return FALSE;
}

BOOL CSkinItem::OnEraseBkgnd(UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* lpResult)
{
	*lpResult = TRUE;

	if (!GetselfOnEraseBkgnd()) {
		HDC hDC = (HDC)wParam;
        if (m_lEraseBkMode > 0)
            ::SetBkMode(hDC, m_lEraseBkMode);
        CRect rect;
        ::GetClientRect(m_hWnd, &rect);
        if ( ! GetDisabled()){
            if (GetbkColorNormal() != -1){
                LONG transedColor = m_clrBkColorNormal;
                if (m_bColorTransform)
                    GetEngine()->TransformColor(transedColor, transedColor);
                CRenderEngine::FillRect(hDC, rect, transedColor);
            }
            if (m_pbkImageNormal != NULL){
                m_pbkImageNormal->Draw(hDC, &rect, NULL, GetImageParam());
            }
        }else{
            if (GetbkColorDisabled() != -1){
                LONG transedColor = m_clrBkColorDisabled;
                if (m_bColorTransform)
                    GetEngine()->TransformColor(transedColor, transedColor);
                CRenderEngine::FillRect(hDC, rect, transedColor);
            }
            if (m_pbkImageDisabled != NULL){
                m_pbkImageDisabled->Draw(hDC, &rect, NULL, GetImageParam());
            }
        }
	}

	return !GetselfOnEraseBkgnd();
}

BOOL CSkinItem::OnGetIcon(UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* lpResult)
{
	switch(wParam)
	{
	case ICON_BIG:
		*lpResult = (LRESULT)m_hBigIcon;
		return TRUE;
		break;
	case ICON_SMALL:
		*lpResult = (LRESULT)m_hSmallIcon;
		return TRUE;
		break;
#if(_WIN32_WINNT >= 0x0501)
	case ICON_SMALL2:
		return FALSE;
		break;
#endif /* _WIN32_WINNT >= 0x0501 */
	default:
		break;
	}
	return FALSE;
}

BOOL CSkinItem::OnSetIcon(UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* lpResult)
{
	HICON hNew = (HICON)lParam;
	switch(wParam)
	{
	case ICON_BIG:
		if (hNew != m_hBigIcon){
			::DestroyIcon(m_hBigIcon);
			m_hBigIcon = hNew;
		}
		break;
	case ICON_SMALL:
		if (hNew != m_hSmallIcon){
			::DestroyIcon(m_hSmallIcon);
			m_hSmallIcon = hNew;
		}
		break;
#if(_WIN32_WINNT >= 0x0501)
	case ICON_SMALL2:
		break;
#endif /* _WIN32_WINNT >= 0x0501 */
	default:
		break;
	}
	return FALSE;
}

BOOL CSkinItem::OnGetTextLength(UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* lpResult)
{
	return FALSE;
}

BOOL CSkinItem::OnGetText(UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* lpResult)
{
	return FALSE;
}

BOOL CSkinItem::OnSetText(UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* lpResult)
{
	*lpResult = DefWndProc(uMsg, wParam, lParam);

	if (*lpResult == TRUE){
		TCHAR* pStr = (TCHAR*)lParam;
		CString tempStr(pStr);
		m_strText = tempStr;
	}
	CSkinBase::OnSetText();
	return TRUE;
}

BOOL CSkinItem::OnThemeChanged(UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* lpResult)
{
    return TRUE;
}

BOOL CSkinItem::OnStyleChanging(UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* lpResult)
{
	return FALSE;
}

BOOL CSkinItem::OnStyleChanged(UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* lpResult)
{
	return FALSE;
}

BOOL CSkinItem::OnParantNotify(UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* lpResult)
{
	return FALSE;
}

BOOL CSkinItem::OnShowWindow(UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* lpResult)
{
    //调用默认的处理
    *lpResult = DefWndProc(uMsg, wParam, lParam);

    //基类处理
    CSkinBase::OnShowWindow((wParam != FALSE));

    //返回，不再做处理
    return TRUE;
}

BOOL CSkinItem::OnCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* lpResult)
{
	return FALSE;
}

BOOL CSkinItem::OnSysCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* lpResult)
{
    return FALSE;
}

BOOL CSkinItem::OnKeyDown(UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* lpResult)
{
 	*lpResult = 0;

	CSkinBase* pFocus = NULL;
	BOOL bGetFocus = GetSkinFocus(&pFocus);
	ASSERT(bGetFocus && pFocus != NULL);
	if (! bGetFocus || pFocus == NULL)
		return FALSE;

	return pFocus->OnKeyDown((UINT)wParam, (UINT)lParam);
}

BOOL CSkinItem::OnChar(UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* lpResult)
{
	*lpResult = 0;

	CSkinBase* pFocus = NULL;
	BOOL bGetFocus = GetSkinFocus(&pFocus);
	ASSERT(bGetFocus && pFocus != NULL);
	if (! bGetFocus || pFocus == NULL)
		return FALSE;

	return pFocus->OnChar((UINT)wParam, (UINT)lParam);
}

BOOL CSkinItem::OnKeyUp(UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* lpResult)
{
	*lpResult = 0;

	CSkinBase* pFocus = NULL;
	BOOL bGetFocus = GetSkinFocus(&pFocus);
	ASSERT(bGetFocus && pFocus != NULL);
	if (! bGetFocus || pFocus == NULL)
		return FALSE;

	return pFocus->OnKeyUp((UINT)wParam, (UINT)lParam);
}

BOOL CSkinItem::OnActivate(UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* lpResult)
{
	*lpResult = 0;

    //调用默认
    *lpResult = DefWndProc(uMsg, wParam, lParam);

	//WM_ACTIVATE只对顶级窗口，不需考虑子皮肤窗口
	HWND hOtherWnd = (HWND)lParam;
	CSkinItem* pItem = CSkinItem::FromHandlePermanent(hOtherWnd);
	CSkinBase::OnActivate(pItem, (UINT)wParam);

    return TRUE;
}

BOOL CSkinItem::OnSetFocus(UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* lpResult)
{
	*lpResult = 0;

    if (m_bSelfOnSetFocus)
        return FALSE;

	//失去焦点的皮肤元素
	HWND hOldFocus = (HWND)wParam;
	CSkinItem* pItem = CSkinItem::FromHandlePermanent(hOldFocus);
	CSkinBase* pBase = NULL;
	if (pItem != NULL) {
		pItem->GetSkinFocus(&pBase);
	}

	BOOL bRet = FALSE;
	if (m_pSkinFocus != NULL) {
		bRet = m_pSkinFocus->OnSetFocus(pBase);
	} else {
		bRet = CSkinBase::OnSetFocus(pBase);
	}
	return bRet;
}

BOOL CSkinItem::OnKillFocus(UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* lpResult)
{
	*lpResult = 0;

	//获得焦点的皮肤元素
	HWND hNewFocus = (HWND)wParam;
	CSkinItem* pItem = CSkinItem::FromHandlePermanent(hNewFocus);
	CSkinBase* pBase = NULL;
	if (pItem != NULL) {
		pItem->GetSkinFocus(&pBase);
	}

	BOOL bRet = FALSE;
	if (m_pSkinFocus != NULL) {
		bRet = m_pSkinFocus->OnKillFocus(pBase);
	} else {
		bRet = CSkinBase::OnKillFocus(pBase);
	}
	return bRet;
}

BOOL CSkinItem::OnCaptureChanged(UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* lpResult)
{
	*lpResult = 0;

	//获得Capture的元素
	HWND hNewCapture = (HWND)wParam;
	CSkinItem* pItem = CSkinItem::FromHandlePermanent(hNewCapture);
	CSkinBase* pBase = NULL;
	if (pItem != NULL) {
		pItem->GetSkinCapture(&pBase);
	}

	BOOL bRet = FALSE;
	if (m_pSkinCapture != NULL) {
		bRet = m_pSkinCapture->OnCaptureChanged(pBase);
	} else {
		bRet = CSkinBase::OnCaptureChanged(pBase);
	}
	m_pSkinCapture = NULL;
	return bRet;
}

BOOL CSkinItem::OnEnable(UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* lpResult)
{
	*lpResult = 0;
	return CSkinBase::OnEnable((BOOL)wParam);
}

BOOL CSkinItem::OnDrawItem(UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* lpResult)
{
	return FALSE;
}

BOOL CSkinItem::OnMeasureItem(UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* lpResult)
{
	return FALSE;
}

BOOL CSkinItem::OnUpdateUIState(UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* lpResult)
{
    return TRUE;
}

//Button
BOOL CSkinItem::OnBMSetState(UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* lpResult)
{
    return FALSE;
}

BOOL CSkinItem::OnBMSetCheck(UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* lpResult)
{
    return FALSE;
}

//ListView
BOOL CSkinItem::OnLVMSetItemState(UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* lpResult)
{
    return FALSE;
}

#ifdef CUSTOM_SCROLLBAR_SUPPORT

BOOL CSkinItem::OnCoolSBCustomDraw(NMHDR* pNMHDR, LRESULT* lpResult)
{
    NMCSBCUSTOMDRAW* pCoolSBNM = (NMCSBCUSTOMDRAW*)pNMHDR;
    LRESULT& result = *lpResult;
    result = CDRF_DODEFAULT;

    const DWORD dwDrawStage = pCoolSBNM->dwDrawStage;
    const HDC hDC = pCoolSBNM->hdc;
    const CRect rcItem = pCoolSBNM->rect;
    const UINT uItem = pCoolSBNM->uItem;
    const UINT uState = pCoolSBNM->uState;
    const UINT uBar = pCoolSBNM->nBar;

    CImageBase* pImage = GetScrollBarImageHelper(uBar, uItem, uState);

    if (dwDrawStage == CDDS_PREPAINT) {
        result = CDRF_SKIPDEFAULT;
        return TRUE;
    } else if (dwDrawStage == CDDS_POSTPAINT) {
        result = CDRF_SKIPDEFAULT;
        return TRUE;
    } else if (dwDrawStage == CDDS_PREERASE) {
        result = CDRF_SKIPDEFAULT;
        return TRUE;
    } else if (dwDrawStage == CDDS_POSTERASE) {
        result = CDRF_SKIPDEFAULT;
        return TRUE;
    } else if (dwDrawStage == CDDS_ITEMPREPAINT) {
        switch (uBar)
        {
        case SB_BOTH:
        case SB_VERT:
        case SB_HORZ:
            {
                if (pImage != NULL) {
                    if (m_bSkinScrollStretchPage) {
                        if (uItem == HTSCROLL_PAGEGUP) {
                            m_rcSkinScrollPage = rcItem;
                        } else if (uItem == HTSCROLL_PAGEGDOWN) {
                            //设置剪切区域
                            int iOldDC = ::SaveDC(hDC);
                            HRGN hUpRgn = ::CreateRectRgnIndirect(&m_rcSkinScrollPage);
                            HRGN hDownRgn = ::CreateRectRgnIndirect(&rcItem);
                            HRGN hClipRgn = ::CreateRectRgnIndirect(&m_rcSkinScrollPage);
                            ::CombineRgn(hClipRgn, hUpRgn, hDownRgn, RGN_OR);
                            ::SelectClipRgn(hDC, hClipRgn);
                            if (uBar == SB_VERT) {
                                m_rcSkinScrollPage.bottom = rcItem.bottom;
                            } else if (uBar == SB_HORZ) {
                                m_rcSkinScrollPage.right = rcItem.right;
                            }
                            pImage->Draw(hDC, &m_rcSkinScrollPage, NULL, GetImageParam(), -1, FALSE, 
                                FALSE, m_lSkinScrollBarSideWidth, m_lSkinScrollBarSideHeight);
                            ::RestoreDC(hDC, iOldDC);
                            ::DeleteObject(hUpRgn);
                            ::DeleteObject(hDownRgn);
                            ::DeleteObject(hClipRgn);
                            m_rcSkinScrollPage.SetRectEmpty();
                        } else {
                            pImage->Draw(hDC, &rcItem, NULL, GetImageParam(), -1, FALSE, 
                                FALSE, m_lSkinScrollBarSideWidth, m_lSkinScrollBarSideHeight);
                        }
                    } else {
                        pImage->Draw(hDC, &rcItem, NULL, GetImageParam(), -1, FALSE, 
                            FALSE, m_lSkinScrollBarSideWidth, m_lSkinScrollBarSideHeight);
                    }
                }
            }
            break;
        }
        result = CDRF_SKIPDEFAULT;
        return TRUE;
    } else if (dwDrawStage == CDDS_ITEMPOSTPAINT) {
        result = CDRF_SKIPDEFAULT;
        return TRUE;
    } else if (dwDrawStage == CDDS_ITEMPREERASE) {
        result = CDRF_SKIPDEFAULT;
        return TRUE;
    } else if (dwDrawStage == CDDS_ITEMPOSTERASE) {
        result = CDRF_SKIPDEFAULT;
        return TRUE;
    } else {
        result = CDRF_SKIPDEFAULT;
        return TRUE;
    }

    result = CDRF_SKIPDEFAULT;
    return TRUE;
}

CImageBase* CSkinItem::GetScrollBarImageHelper(UINT uBar, UINT uItem, UINT uState)
{
    CImageBase* scrollBarImages[33] = {
#define GET_SCROLLBAR_IMAGE_VAR(theClass, propName) \
    m_p##propName, 
    SCROLLBAR_IMAGE_LIST(CSkinItem, GET_SCROLLBAR_IMAGE_VAR)
#undef GET_SCROLLBAR_IMAGE_VAR
    };
    UINT barBase = 0;
    switch (uBar) 
    {
    case SB_VERT:
        barBase = 0;
        break;
    case SB_HORZ:
        barBase = 15;
        break;
    case SB_BOTH:
        barBase = 30;
        break;
    default:
        return NULL;
        break;
    }
    UINT stateBase = 0;
    switch (uState)
    {
    case CDIS_HOT:
        stateBase = 1;
        break;
    case CDIS_SELECTED:
        stateBase = 2;
        break;
    }
    UINT itemBase = 0;
    switch (uItem)
    {
    case HTSCROLL_UP: //HTSCROLL_LEFT
        itemBase = 0;
        break;
    case HTSCROLL_DOWN: //HTSCROLL_RIGHT
        itemBase = 3;
        break;
    case HTSCROLL_THUMB: //HTSCROLL_THUMB
        itemBase = 6;
        break;
    case HTSCROLL_PAGEGUP: //HTSCROLL_PAGELEFT
        itemBase = 9;
        break;
    case HTSCROLL_PAGEGDOWN: //HTSCROLL_PAGERIGHT
        itemBase = 12;
        break;
    }
    int index = barBase + itemBase + stateBase;
    BOOL bValid = (index >= 0 && index < (sizeof(scrollBarImages) / sizeof(CImageBase*)));
    ASSERT(bValid);
    if (bValid)
        return scrollBarImages[index];

    return NULL;
}

#endif //CUSTOM_SCROLLBAR_SUPPORT

BOOL CSkinItem::OnCustomDraw(NMHDR *pNMHDR, LRESULT *pResult)
{
    return FALSE;
}

BOOL CSkinItem::GetGUIThreadHwndHelper(HWND hCurrentWnd, GUI_THREAD_HWND flag, HWND& hWnd)
{
	GUITHREADINFO guiThreadInfo;
	guiThreadInfo.cbSize = sizeof(GUITHREADINFO);
	if (! GetGUIThreadInfo(GetWindowThreadProcessId(hCurrentWnd, NULL), &guiThreadInfo))
		return FALSE;

	hWnd = NULL;
	switch (flag)
	{
	case GUI_THREAD_HWND_ACTIVE:
		hWnd = guiThreadInfo.hwndActive;
		ASSERT(hWnd == ::GetActiveWindow());
		break;
	case GUI_THREAD_HWND_FOCUS:
		hWnd = guiThreadInfo.hwndFocus;
		ASSERT(hWnd == ::GetFocus());
		break;
	case GUI_THREAD_HWND_CAPTURE:
		hWnd = guiThreadInfo.hwndCapture;
		ASSERT(hWnd == ::GetCapture());
		break;
	case GUI_THREAD_HWND_MENUOWNER:
		hWnd = guiThreadInfo.hwndMenuOwner;
		break;
	case GUI_THREAD_HWND_MOVESIZE:
		hWnd = guiThreadInfo.hwndMoveSize;
		break;
	case GUI_THREAD_HWND_CARET:
		hWnd = guiThreadInfo.hwndCaret;
		break;
	default:
		return FALSE;
	}
	return TRUE;
}

BOOL CSkinItem::GetGUIThreadIDHelper(HWND hWnd, DWORD& threadId)
{
	if (! IsWindow(hWnd))
		return FALSE;

	threadId = ::GetWindowThreadProcessId(hWnd, NULL);

	return TRUE;
}

BOOL CSkinItem::IsInGUIThreadOf(HWND hWnd)
{
	DWORD currentThreadId = ::GetCurrentThreadId();
	DWORD hWndThreadId = NULL;
	if (! GetGUIThreadIDHelper(hWnd, hWndThreadId))
		return FALSE;
	return (hWndThreadId == currentThreadId);
}

BOOL CSkinItem::IsSkinHwndItemOf(CSkinBase* pBase)
{
	return (this == pBase->GetSkinHwndItem());
}

void CSkinItem::ShowToolTip(CString pszTooltip, const RECT& rcTip, LONG maxTipWidth)
{
    TOOLINFO	ti;
    LRESULT		bRes;

    if(m_hToolTip == NULL)
    {
        CRect rc;
        INITCOMMONCONTROLSEX iccex;

        ::GetClientRect(m_hWnd, &rc);
        iccex.dwICC = ICC_WIN95_CLASSES;
        iccex.dwSize = sizeof(INITCOMMONCONTROLSEX);
        bRes = InitCommonControlsEx(&iccex);

        m_hToolTip = CreateWindowEx(WS_EX_TOPMOST, TOOLTIPS_CLASS,
            NULL,WS_POPUP | TTS_NOPREFIX | TTS_ALWAYSTIP,
            CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, NULL,
            NULL, NULL, NULL);

        ::SetWindowPos(m_hToolTip, HWND_TOPMOST, 0, 0, 0, 0,
            SWP_NOACTIVATE | SWP_NOMOVE | SWP_NOSIZE);

        memset(&ti, 0, sizeof(TOOLINFO));
        ti.cbSize = sizeof(TOOLINFO);
        ti.uFlags = TTF_SUBCLASS;
        ti.hwnd   = m_hWnd;
        ti.uId    = 1;
        ti.hinst  = NULL;
        ti.lpszText = _T("");
        ti.rect.left   = rc.left;
        ti.rect.right  = rc.right;
        ti.rect.top    = rc.top;
        ti.rect.bottom = rc.bottom;
        bRes = ::SendMessage(m_hToolTip, TTM_ADDTOOL, 0, (LPARAM)&ti);
		if (maxTipWidth > 0) //设置最大宽度
		{
			::SendMessage(m_hToolTip, TTM_SETMAXTIPWIDTH, 0, maxTipWidth);
		}
    }

    CRect rc;
    ::GetClientRect(GetSkinHwnd(), &rc);
    memset(&ti,0,sizeof(TOOLINFO));
    ti.cbSize = sizeof(TOOLINFO);
    ti.uFlags = TTF_SUBCLASS;
    ti.hwnd   = GetSkinHwnd();
    ti.uId    = 1;
    ti.hinst  = NULL;
    ti.lpszText = _T("");
    ti.rect   = rcTip;
    bRes = ::SendMessage(m_hToolTip, TTM_SETTOOLINFO, 0, (LPARAM)&ti);	

    ::SendMessage(m_hToolTip, TTM_ACTIVATE, TRUE, 0);

    memset(&ti, 0, sizeof(TOOLINFO));
    ti.cbSize = sizeof(TOOLINFO);
    ti.hwnd   = GetSkinHwnd();
    ti.uId    = 1;

    LPTSTR lpzTipText = const_cast<LPTSTR>((LPCTSTR)pszTooltip);
    ti.lpszText = lpzTipText;
    ::SendMessage(m_hToolTip, TTM_UPDATETIPTEXT, 0, (LPARAM)&ti);
}

void CSkinItem::PopToolTip()
{
    if (! ::IsWindow(m_hToolTip))
        return;

    ::SendMessage(m_hToolTip, TTM_ACTIVATE, FALSE, 0);
}

BOOL CSkinItem::GetSkinActive(CSkinBase** ppSkin)
{
	if (ppSkin != NULL)
		*ppSkin = NULL;

	if (!::IsWindow(m_hWnd))
		return FALSE;

	//HWND hWnd = NULL;
	//if (! GetGUIThreadHwndHelper(m_hWnd, GUI_THREAD_HWND_ACTIVE, hWnd))
	//	return FALSE;

	//if (m_hWnd != hWnd)
	//	return FALSE;

	if (m_pSkinActive != NULL) {
		ASSERT(IsSkinHwndItemOf(m_pSkinActive));
		if (! IsSkinHwndItemOf(m_pSkinActive))
			return FALSE;
	}

	if (ppSkin != NULL) {
		*ppSkin = (m_pSkinActive != NULL) ? m_pSkinActive : this;
	}

	return TRUE;
}

BOOL CSkinItem::GetSubSkinActive(CSkinBase** ppSkin)
{
	if (ppSkin != NULL)
		*ppSkin = NULL;

	if (!::IsWindow(m_hWnd))
		return FALSE;

	if (m_pSkinActive != NULL) {
		ASSERT(IsSkinHwndItemOf(m_pSkinActive));
		if (! IsSkinHwndItemOf(m_pSkinActive))
			return FALSE;
	}

	if (ppSkin != NULL && (this != m_pSkinActive)) {
		*ppSkin = m_pSkinActive;
	}

	return TRUE;
}

BOOL CSkinItem::SetSkinActive(CSkinBase* pNewSkin)
{
	//if (! GetSkinActive())
	//	return FALSE;

	if (pNewSkin != NULL) {
		ASSERT(IsSkinHwndItemOf(pNewSkin));
		if (! IsSkinHwndItemOf(pNewSkin))
			return FALSE;
	}

	m_pSkinActive = pNewSkin;

	return TRUE;
}

BOOL CSkinItem::GetSkinFocus(CSkinBase** ppSkin)
{
	if (ppSkin != NULL)
		*ppSkin = NULL;

	if (!::IsWindow(m_hWnd))
		return FALSE;

	if (m_pSkinFocus != NULL) {
		ASSERT(IsSkinHwndItemOf(m_pSkinFocus));
		if (! IsSkinHwndItemOf(m_pSkinFocus))
			return FALSE;
	}

	if (ppSkin != NULL) {
		*ppSkin = (m_pSkinFocus != NULL) ? m_pSkinFocus : this;
	}

	return TRUE;
}

BOOL CSkinItem::GetSubSkinFocus(CSkinBase** ppSkin)
{
	if (ppSkin != NULL)
		*ppSkin = NULL;

	if (!::IsWindow(m_hWnd))
		return FALSE;

	if (m_pSkinFocus != NULL) {
		ASSERT(IsSkinHwndItemOf(m_pSkinFocus));
		if (! IsSkinHwndItemOf(m_pSkinFocus))
			return FALSE;
	}

	if (ppSkin != NULL && (this != m_pSkinFocus)) {
		*ppSkin = m_pSkinFocus;
	}

	return TRUE;
}

BOOL CSkinItem::SetSkinFocus(CSkinBase* pNewSkin)
{
	if (pNewSkin != NULL) {
		ASSERT(IsSkinHwndItemOf(pNewSkin));
		if (! IsSkinHwndItemOf(pNewSkin))
			return FALSE;
	}

	m_pSkinFocus = pNewSkin;

	return TRUE;
}

BOOL CSkinItem::GetSkinCapture(CSkinBase** ppSkin)
{
	if (ppSkin != NULL)
		*ppSkin = NULL;

	if (!::IsWindow(m_hWnd))
		return FALSE;

	if (m_pSkinCapture != NULL) {
		ASSERT(IsSkinHwndItemOf(m_pSkinCapture));
		if (! IsSkinHwndItemOf(m_pSkinCapture))
			return FALSE;
	}

	if (ppSkin != NULL) {
		*ppSkin = (m_pSkinCapture != NULL) ? m_pSkinCapture : this;
	}

	return TRUE;
}

BOOL CSkinItem::GetSubSkinCapture(CSkinBase** ppSkin)
{
	if (ppSkin != NULL)
		*ppSkin = NULL;

	if (!::IsWindow(m_hWnd))
		return FALSE;

	if (m_pSkinCapture != NULL) {
		ASSERT(IsSkinHwndItemOf(m_pSkinCapture));
		if (! IsSkinHwndItemOf(m_pSkinCapture))
			return FALSE;
	}

	if (ppSkin != NULL&& (this != m_pSkinCapture)) {
		*ppSkin = m_pSkinCapture;
	}

	return TRUE;
}

BOOL CSkinItem::SetSkinCapture(CSkinBase* pNewSkin)
{
	if (pNewSkin != NULL) {
		ASSERT(IsSkinHwndItemOf(pNewSkin));
		if (! IsSkinHwndItemOf(pNewSkin))
			return FALSE;
	}

	m_pSkinCapture = pNewSkin;

	return TRUE;
}

BOOL CSkinItem::GetSkinMenuOwner(CSkinBase** ppSkin)
{
	if (ppSkin != NULL)
		*ppSkin = NULL;

	if (!::IsWindow(m_hWnd))
		return FALSE;

	if (m_pSkinMenuOwner != NULL) {
		ASSERT(IsSkinHwndItemOf(m_pSkinMenuOwner));
		if (! IsSkinHwndItemOf(m_pSkinMenuOwner))
			return FALSE;
	}

	if (ppSkin != NULL) {
		*ppSkin = (m_pSkinMenuOwner != NULL) ? m_pSkinMenuOwner : this;
	}

	return TRUE;
}

BOOL CSkinItem::GetSubSkinMenuOwner(CSkinBase** ppSkin)
{
	if (ppSkin != NULL)
		*ppSkin = NULL;

	if (!::IsWindow(m_hWnd))
		return FALSE;

	if (m_pSkinMenuOwner != NULL) {
		ASSERT(IsSkinHwndItemOf(m_pSkinMenuOwner));
		if (! IsSkinHwndItemOf(m_pSkinMenuOwner))
			return FALSE;
	}

	if (ppSkin != NULL && (this != m_pSkinMenuOwner)) {
		*ppSkin = m_pSkinMenuOwner;
	}

	return TRUE;
}

BOOL CSkinItem::SetSkinMenuOwner(CSkinBase* pNewSkin)
{
	if (pNewSkin != NULL) {
		ASSERT(IsSkinHwndItemOf(pNewSkin));
		if (! IsSkinHwndItemOf(pNewSkin))
			return FALSE;
	}

	m_pSkinMenuOwner = pNewSkin;

	return TRUE;
}

BOOL CSkinItem::GetSkinMoveSize(CSkinBase** ppSkin)
{
	if (ppSkin != NULL)
		*ppSkin = NULL;

	if (!::IsWindow(m_hWnd))
		return FALSE;

	if (m_pSkinMoveSize != NULL) {
		ASSERT(IsSkinHwndItemOf(m_pSkinMoveSize));
		if (! IsSkinHwndItemOf(m_pSkinMoveSize))
			return FALSE;
	}

	if (ppSkin != NULL) {
		*ppSkin = (m_pSkinMoveSize != NULL) ? m_pSkinMoveSize : this;
	}

	return TRUE;
}

BOOL CSkinItem::GetSubSkinMoveSize(CSkinBase** ppSkin)
{
	if (ppSkin != NULL)
		*ppSkin = NULL;

	if (!::IsWindow(m_hWnd))
		return FALSE;

	if (m_pSkinMoveSize != NULL) {
		ASSERT(IsSkinHwndItemOf(m_pSkinMoveSize));
		if (! IsSkinHwndItemOf(m_pSkinMoveSize))
			return FALSE;
	}

	if (ppSkin != NULL && (this != m_pSkinMoveSize)) {
		*ppSkin = m_pSkinMoveSize;
	}

	return TRUE;
}

BOOL CSkinItem::SetSkinMoveSize(CSkinBase* pNewSkin)
{
	if (pNewSkin != NULL) {
		ASSERT(IsSkinHwndItemOf(pNewSkin));
		if (! IsSkinHwndItemOf(pNewSkin))
			return FALSE;
	}

	m_pSkinMoveSize = pNewSkin;

	return TRUE;
}

BOOL CSkinItem::GetSkinCaret(CSkinBase** ppSkin)
{
	if (ppSkin != NULL)
		*ppSkin = NULL;

	if (!::IsWindow(m_hWnd))
		return FALSE;

	if (m_pSkinCaret != NULL) {
		ASSERT(IsSkinHwndItemOf(m_pSkinCaret));
		if (! IsSkinHwndItemOf(m_pSkinCaret))
			return FALSE;
	}

	if (ppSkin != NULL) {
		*ppSkin = (m_pSkinCaret != NULL) ? m_pSkinCaret : this;
	}

	return TRUE;
}

BOOL CSkinItem::GetSubSkinCaret(CSkinBase** ppSkin)
{
	if (ppSkin != NULL)
		*ppSkin = NULL;

	if (!::IsWindow(m_hWnd))
		return FALSE;

	if (m_pSkinCaret != NULL) {
		ASSERT(IsSkinHwndItemOf(m_pSkinCaret));
		if (! IsSkinHwndItemOf(m_pSkinCaret))
			return FALSE;
	}

	if (ppSkin != NULL && (this != m_pSkinCaret)) {
		*ppSkin = m_pSkinCaret;
	}

	return TRUE;
}

BOOL CSkinItem::SetSkinCaret(CSkinBase* pNewSkin)
{
	if (pNewSkin != NULL) {
		ASSERT(IsSkinHwndItemOf(pNewSkin));
		if (! IsSkinHwndItemOf(pNewSkin))
			return FALSE;
	}

	m_pSkinCaret = pNewSkin;

	return TRUE;
}

BOOL CSkinItem::GetActiveSubSkin(CSkinItem** ppItem, CSkinBase** ppSkin)
{
	if (ppItem != NULL)
		*ppItem = NULL;
	if (ppSkin != NULL)
		*ppSkin = NULL;

	ASSERT(IsInGUIThreadOf(m_hWnd));
	if (! IsInGUIThreadOf(m_hWnd))
		return FALSE;

	HWND hWnd = ::GetActiveWindow();
	if (hWnd == NULL)
		return TRUE;

	CSkinItem* pItem = CSkinItem::FromHandlePermanent(hWnd);
	if (pItem == NULL)
		return TRUE;

	if (ppItem != NULL)
		*ppItem = pItem;

	CSkinBase* pSkin = NULL;
	if (! pItem->GetSkinActive(&pSkin))
		return TRUE;

	if (ppSkin != NULL)
		*ppSkin = pSkin;

	return TRUE;
}

BOOL CSkinItem::SetActiveSubSkin(CSkinBase* pSkin, CSkinBase** ppPrevSkin)
{
	CSkinItem* pOldItem = NULL;
	CSkinBase* pOldSkin = NULL;
	if (! GetActiveSubSkin(&pOldItem, &pOldSkin))
		return FALSE;

	if (ppPrevSkin != NULL) 
		*ppPrevSkin = pOldSkin;

	ASSERT(pSkin != NULL);
	if (pSkin == NULL)
		return FALSE;

	ASSERT(IsSkinHwndItemOf(pSkin));
	if (! IsSkinHwndItemOf(pSkin))
		return FALSE;

	if (pSkin == pOldSkin)
		return TRUE;

	ASSERT(::IsWindow(m_hWnd));
	if (! ::IsWindow(m_hWnd))
		return FALSE;

	if (m_pSkinActive != NULL) {
		ASSERT(IsSkinHwndItemOf(m_pSkinActive));
	}

	if (::GetActiveWindow() != m_hWnd) {	//不是当前ActiveWindow，则尝试设置
		SetSkinActive(pSkin);
		::SetActiveWindow(m_hWnd);
		//设置原来的为NULL
		if (::GetActiveWindow() != m_hWnd) {
			SetSkinActive(NULL);
			return FALSE;
		}
		if (pSkin == this)
			SetSkinActive(NULL);
		return TRUE;
	} else {
		if (pOldSkin != NULL) {
			pOldSkin->OnActivate(pSkin, WA_INACTIVE);
			pOldItem->SetSkinActive(NULL);
		}
		pSkin->OnActivate(pOldSkin, WA_ACTIVE);
		if (pSkin == this)
			SetSkinActive(NULL);
		else
			SetSkinActive(pSkin);
		return TRUE;
	}

	return TRUE;
}

BOOL CSkinItem::GetFocusSubSkin(CSkinItem** ppItem, CSkinBase** ppSkin)
{
	if (ppItem != NULL)
		*ppItem = NULL;
	if (ppSkin != NULL)
		*ppSkin = NULL;

	ASSERT(IsInGUIThreadOf(m_hWnd));
	if (! IsInGUIThreadOf(m_hWnd))
		return FALSE;

	HWND hWnd = ::GetFocus();
	if (hWnd == NULL)
		return TRUE;

	CSkinItem* pItem = CSkinItem::FromHandlePermanent(hWnd);
	if (pItem == NULL)
		return TRUE;

	if (ppItem != NULL)
		*ppItem = pItem;

	CSkinBase* pSkin = NULL;
	if (! pItem->GetSkinFocus(&pSkin))
		return TRUE;

	if (ppSkin != NULL)
		*ppSkin = pSkin;

	return TRUE;
}

BOOL CSkinItem::SetFocusSubSkin(CSkinBase* pSkin, CSkinBase** ppPrevSkin)
{
	CSkinItem* pOldItem = NULL;
	CSkinBase* pOldSkin = NULL;
 	if (! GetFocusSubSkin(&pOldItem, &pOldSkin))
		return FALSE;

	if (ppPrevSkin != NULL) 
		*ppPrevSkin = pOldSkin;

	ASSERT(pSkin != NULL);
	if (pSkin == NULL)
		return FALSE;

	ASSERT(IsSkinHwndItemOf(pSkin));
	if (! IsSkinHwndItemOf(pSkin))
		return FALSE;

	if (pSkin == pOldSkin)
		return TRUE;

	ASSERT(::IsWindow(m_hWnd));
	if (! ::IsWindow(m_hWnd))
		return FALSE;

	if (m_pSkinFocus != NULL) {
		ASSERT(IsSkinHwndItemOf(m_pSkinFocus));
	}

	if (::GetFocus() != m_hWnd) {	//不是当前ActiveWindow，则尝试设置
		SetSkinFocus(pSkin);
		::SetFocus(m_hWnd);
		//设置原来的为NULL
		if (::GetFocus() != m_hWnd) {
			SetSkinFocus(NULL);
			return FALSE;
		}
		if (pSkin == this)
			SetSkinFocus(NULL);
		return TRUE;
	} else {
		if (pOldSkin != NULL) {
			pOldSkin->OnKillFocus(pSkin);
			pOldItem->SetSkinFocus(NULL);
		}
		pSkin->OnSetFocus(pOldSkin);
		if (pSkin == this)
			SetSkinFocus(NULL);
		else
			SetSkinFocus(pSkin);
		return TRUE;
	}

	return TRUE;
}

BOOL CSkinItem::GetCaptureSubSkin(CSkinItem** ppItem, CSkinBase** ppSkin)
{
	if (ppItem != NULL)
		*ppItem = NULL;
	if (ppSkin != NULL)
		*ppSkin = NULL;

	ASSERT(IsInGUIThreadOf(m_hWnd));
	if (! IsInGUIThreadOf(m_hWnd))
		return FALSE;

	HWND hWnd = ::GetCapture();
	if (hWnd == NULL)
		return TRUE;

	CSkinItem* pItem = CSkinItem::FromHandlePermanent(hWnd);
	if (pItem == NULL)
		return TRUE;

	if (ppItem != NULL)
		*ppItem = pItem;

	CSkinBase* pSkin = NULL;
	if (! pItem->GetSkinCapture(&pSkin))
		return TRUE;

	if (ppSkin != NULL)
		*ppSkin = pSkin;

	return TRUE;
}

BOOL CSkinItem::SetCaptureSubSkin(CSkinBase* pSkin, CSkinBase** ppPrevSkin)
{
	CSkinItem* pOldItem = NULL;
	CSkinBase* pOldSkin = NULL;
	if (! GetCaptureSubSkin(&pOldItem, &pOldSkin))
		return FALSE;

	if (ppPrevSkin != NULL) 
		*ppPrevSkin = pOldSkin;

	ASSERT(pSkin != NULL);
	if (pSkin == NULL)
		return FALSE;

	ASSERT(IsSkinHwndItemOf(pSkin));
	if (! IsSkinHwndItemOf(pSkin))
		return FALSE;

	if (pSkin == pOldSkin)
		return TRUE;

	ASSERT(::IsWindow(m_hWnd));
	if (! ::IsWindow(m_hWnd))
		return FALSE;

	if (m_pSkinCapture != NULL) {
		ASSERT(IsSkinHwndItemOf(m_pSkinCapture));
	}

	if (::GetCapture() != m_hWnd) {	//不是当前ActiveWindow，则尝试设置
		SetSkinCapture(pSkin);
		::SetCapture(m_hWnd);
		//设置原来的为NULL
		if (::GetCapture() != m_hWnd) {
			SetSkinCapture(NULL);
			return FALSE;
		}
		if (pSkin == this)
			SetSkinCapture(NULL);
		return TRUE;
	} else {
		if (pOldSkin != NULL) {
			pOldSkin->OnCaptureChanged(pSkin);
			pOldItem->SetSkinCapture(NULL);
		}
		//pSkin->OnCaptureChanged(pOldSkin);
		if (pSkin == this)
			SetSkinCapture(NULL);
		else
			SetSkinCapture(pSkin);
		return TRUE;
	}

	return TRUE;
}

BOOL CSkinItem::ReleaseCaptureSubSkin(CSkinBase* pSkin)
{
	ASSERT(IsSkinHwndItemOf(pSkin));
	if (! IsSkinHwndItemOf(pSkin))
		return FALSE;

	ASSERT(::IsWindow(m_hWnd));
	if (! ::IsWindow(m_hWnd))
		return FALSE;

	if (m_pSkinCapture != pSkin)
		return FALSE;

	ASSERT(::GetCapture() == m_hWnd);
	if (::GetCapture() == m_hWnd)
		::ReleaseCapture();

	m_pSkinCapture = NULL;

	return TRUE;
}

//////////////////////////////////////////////////////////////////////////
//控件颜色、背景
//////////////////////////////////////////////////////////////////////////
BSTR CSkinItem::GetbkMode(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CString strResult;

	// TODO: 在此添加调度处理程序代码
	return m_textStyleNormal.GetbkMode();
}

void CSkinItem::SetbkMode(LPCTSTR newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: 在此添加属性处理程序代码
	m_textStyleNormal.SetbkMode(newVal);
	InvalidateRect();
}

LONG CSkinItem::GetbkColorNormal(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: 在此添加调度处理程序代码
	return m_clrBkColorNormal;
}

void CSkinItem::SetbkColorNormal(LONG newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: 在此添加属性处理程序代码
	if (m_clrBkColorNormal == newVal)
		return;

	m_clrBkColorNormal = newVal;
	InvalidateRect();
}

LONG CSkinItem::GetbkColorHover(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: 在此添加调度处理程序代码
	return m_clrBkColorHover;
}

void CSkinItem::SetbkColorHover(LONG newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: 在此添加属性处理程序代码
	if (m_clrBkColorHover == newVal)
		return;

	m_clrBkColorHover = newVal;
	InvalidateRect();
}

LONG CSkinItem::GetbkColorFocus(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: 在此添加调度处理程序代码
	return m_clrBkColorFocus;
}

void CSkinItem::SetbkColorFocus(LONG newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: 在此添加属性处理程序代码
	if (m_clrBkColorFocus == newVal)
		return;

	m_clrBkColorFocus = newVal;
	InvalidateRect();
}

LONG CSkinItem::GetbkColorDisabled(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: 在此添加调度处理程序代码
	return m_clrBkColorDisabled;
}

void CSkinItem::SetbkColorDisabled(LONG newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: 在此添加属性处理程序代码
	if (m_clrBkColorDisabled == newVal)
		return;

	m_clrBkColorDisabled = newVal;
	InvalidateRect();
}

VARIANT_BOOL CSkinItem::GetuseOriginPosition()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: 在此添加调度处理程序代码
	return m_buseOriginPosition ? (VARIANT_TRUE) : (VARIANT_FALSE);
}

void CSkinItem::SetuseOriginPosition(VARIANT_BOOL newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	newVal = (newVal != VARIANT_FALSE);

	if (m_buseOriginPosition == newVal)
		return;

	m_buseOriginPosition = newVal;
	InvalidateRect(NULL);
}

VARIANT_BOOL CSkinItem::GetuseOriginText()
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());

    // TODO: 在此添加调度处理程序代码
    return m_buseOriginText ? (VARIANT_TRUE) : (VARIANT_FALSE);
}

void CSkinItem::SetuseOriginText(VARIANT_BOOL newVal)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());
    newVal = (newVal != VARIANT_FALSE);

    if (m_buseOriginText == newVal)
        return;

    m_buseOriginText = newVal;
}

LONG CSkinItem::GettextColorNormal(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: 在此添加调度处理程序代码
	return m_textStyleNormal.Getcolor();
}

void CSkinItem::SettextColorNormal(LONG newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: 在此添加属性处理程序代码
	m_textStyleNormal.Setcolor(newVal);
	if (m_textStyleNormal.IsFontChanged())
		InvalidateRect();
}

LONG CSkinItem::GettextColorHover(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: 在此添加调度处理程序代码
	return m_textStyleHover.Getcolor();
}

void CSkinItem::SettextColorHover(LONG newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: 在此添加属性处理程序代码
	m_textStyleHover.Setcolor(newVal);
	if (m_textStyleHover.IsFontChanged())
		InvalidateRect();
}

LONG CSkinItem::GettextColorDown(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: 在此添加调度处理程序代码
	return m_textStyleDown.Getcolor();
}

void CSkinItem::SettextColorDown(LONG newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: 在此添加属性处理程序代码
	m_textStyleDown.Setcolor(newVal);
	if (m_textStyleDown.IsFontChanged())
		InvalidateRect();
}

LONG CSkinItem::GettextColorDisabled(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: 在此添加调度处理程序代码
	return m_textStyleDisabled.Getcolor();
}

void CSkinItem::SettextColorDisabled(LONG newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: 在此添加属性处理程序代码
	m_textStyleDisabled.Setcolor(newVal);
	if (m_textStyleDisabled.IsFontChanged())
		InvalidateRect();
}

LONG CSkinItem::GetfontSize(void)
{
	return m_textStyleNormal.GetfontSize();
}

void CSkinItem::SetfontSize(LONG newVal)
{
	m_textStyleNormal.SetfontSize(newVal);
	InvalidateRect();
}

LONG CSkinItem::GetfontWeight(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: 在此添加调度处理程序代码
	return m_textStyleNormal.GetfontWeight();
}

void CSkinItem::SetfontWeight(LONG newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: 在此添加调度处理程序代码
	m_textStyleNormal.SetfontWeight(newVal);
	InvalidateRect();
}

VARIANT_BOOL CSkinItem::Getitalic(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: 在此添加调度处理程序代码
	return m_textStyleNormal.Getitalic();
}

void CSkinItem::Setitalic(VARIANT_BOOL newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: 在此添加属性处理程序代码
	m_textStyleNormal.Setitalic(newVal);
	InvalidateRect();
}

VARIANT_BOOL CSkinItem::Getunderline(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: 在此添加调度处理程序代码
	return m_textStyleNormal.Getunderline();
}

void CSkinItem::Setunderline(VARIANT_BOOL newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: 在此添加属性处理程序代码
	m_textStyleNormal.Setunderline(newVal);
	InvalidateRect();
}

VARIANT_BOOL CSkinItem::GetstrikeOut(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: 在此添加调度处理程序代码
	return m_textStyleNormal.GetstrikeOut();
}

void CSkinItem::SetstrikeOut(VARIANT_BOOL newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: 在此添加属性处理程序代码
	m_textStyleNormal.SetstrikeOut(newVal);
	InvalidateRect();
}

BSTR CSkinItem::GetfontFamily(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: 在此添加调度处理程序代码
	return m_textStyleNormal.GetfontFamily();
}

void CSkinItem::SetfontFamily(LPCTSTR newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: 在此添加调度处理程序代码
	m_textStyleNormal.SetfontFamily(newVal);
	InvalidateRect();
}

VARIANT_BOOL CSkinItem::GetwordBreak(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: 在此添加调度处理程序代码
	return m_textStyleNormal.GetwordBreak();
}

void CSkinItem::SetwordBreak(VARIANT_BOOL newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: 在此添加属性处理程序代码
	m_textStyleNormal.SetwordBreak(newVal);
	InvalidateRect();
}

VARIANT_BOOL CSkinItem::GetsingleLine(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: 在此添加调度处理程序代码
	return m_textStyleNormal.GetsingleLine();
}

void CSkinItem::SetsingleLine(VARIANT_BOOL newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: 在此添加属性处理程序代码
	m_textStyleNormal.SetsingleLine(newVal);
	InvalidateRect();
}

BSTR CSkinItem::GettextHorizAlign(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: 在此添加调度处理程序代码
	return m_textStyleNormal.GethorizAlign();
}

void CSkinItem::SettextHorizAlign(LPCTSTR newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: 在此添加属性处理程序代码
	m_textStyleNormal.SethorizAlign(newVal);
	InvalidateRect();
}

BSTR CSkinItem::GettextVertiAlign(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: 在此添加调度处理程序代码
	return m_textStyleNormal.GetvertiAlign();
}

void CSkinItem::SettextVertiAlign(LPCTSTR newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: 在此添加属性处理程序代码
	m_textStyleNormal.SetvertiAlign(newVal);
	InvalidateRect();
}

//透明度相关
VARIANT_BOOL CSkinItem::Getlayered(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: 在此添加属性处理程序代码
	return m_bLayered ? (VARIANT_TRUE) : (VARIANT_FALSE);
}

void CSkinItem::Setlayered(VARIANT_BOOL newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: 在此添加属性处理程序代码
	newVal = (newVal != VARIANT_FALSE);

	if (m_bLayered == newVal)
		return;

	m_bLayered = newVal;
	if (! ::IsWindow(m_hWnd))
		return;

	SetSkinItemLongHelper(GWL_EXSTYLE, WS_EX_LAYERED, m_bLayered);
}

LONG CSkinItem::Getopacity()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: 在此添加属性处理程序代码
	return m_nOpacity;
}

void CSkinItem::Setopacity(LONG newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: 在此添加属性处理程序代码
	if (newVal < 0 || newVal > 100)
		return;

	m_nOpacity = newVal;

	if (!Getlayered()){
		Setlayered(VARIANT_TRUE);
	}

	SetLayeredWindowAttributes(m_hWnd, RGB(0, 0, 0), (BYTE)((255 * m_nOpacity) / 100), LWA_ALPHA);
}

BSTR CSkinItem::GetsmallIcon(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: 在此添加属性处理程序代码
	ASSERT(0 && _T("CSkinItem::GetsmallIcon不可用"));
	CString str;
	return str.AllocSysString();
}

void CSkinItem::SetsmallIcon(LPCTSTR newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	HICON hIcon = GetIconHelper(newVal, 16);
	if (hIcon == NULL)
		return;

	if (m_hSmallIcon != NULL){
		::DestroyIcon(m_hSmallIcon);
		m_hSmallIcon = NULL;
	}

	m_hSmallIcon = hIcon;
	if (m_hSmallIcon != NULL){
		HICON hOldIcon = (HICON)::SendMessage(m_hWnd, WM_SETICON, ICON_SMALL, (LPARAM)m_hSmallIcon);
		::DestroyIcon(hOldIcon);
	}
}

BSTR CSkinItem::GetbigIcon(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: 在此添加属性处理程序代码
	ASSERT(0 && _T("CSkinItem::GetbigIcon不可用"));
	CString str;
	return str.AllocSysString();
}

void CSkinItem::SetbigIcon(LPCTSTR newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	HICON hIcon = GetIconHelper(newVal, 32);
	if (hIcon == NULL)
		return;

	if (m_hBigIcon != NULL){
		::DestroyIcon(m_hBigIcon);
		m_hBigIcon = NULL;
	}

	m_hBigIcon = hIcon;
	if (m_hBigIcon != NULL){
		HICON hOldIcon = (HICON)::SendMessage(m_hWnd, WM_SETICON, ICON_BIG, (LPARAM)m_hBigIcon);
		::DestroyIcon(hOldIcon);
	}
}

VARIANT_BOOL CSkinItem::GetshowIcon(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: 在此添加属性处理程序代码
	return m_bShowIcon ? (VARIANT_TRUE) : (VARIANT_FALSE);
}

void CSkinItem::SetshowIcon(VARIANT_BOOL newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	newVal = (newVal != VARIANT_FALSE);

	if (m_bShowIcon == newVal)
		return;

	m_bShowIcon = newVal;
	InvalidateRect();
}

VARIANT_BOOL CSkinItem::GettopMost(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: 在此添加属性处理程序代码
	return m_bTopMost ? (VARIANT_TRUE) : (VARIANT_FALSE);
}

void CSkinItem::SettopMost(VARIANT_BOOL newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	newVal = (newVal != VARIANT_FALSE);

	if (m_bTopMost == newVal)
		return;

	m_bTopMost = newVal;
	if (! ::IsWindow(m_hWnd))
		return;

	SetSkinItemLongHelper(GWL_EXSTYLE, WS_EX_TOPMOST, m_bTopMost);
	InvalidateRect();
}

VARIANT_BOOL CSkinItem::GetrecieveCtrlBreak(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: 在此添加属性处理程序代码
	return m_bRecieveCtrlBreak ? (VARIANT_TRUE) : (VARIANT_FALSE);
}

void CSkinItem::SetrecieveCtrlBreak(VARIANT_BOOL newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	newVal = (newVal != VARIANT_FALSE);

	if (m_bRecieveCtrlBreak == newVal)
		return;

	m_bRecieveCtrlBreak = newVal;
}

VARIANT_BOOL CSkinItem::GetbkDrag(void)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());

    return m_bBkDrag ? (VARIANT_TRUE) : (VARIANT_FALSE);
}

void CSkinItem::SetbkDrag(VARIANT_BOOL newVal)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());

    newVal = (newVal != VARIANT_FALSE);
    if (m_bBkDrag == newVal)
        return;

    m_bBkDrag = newVal;
}

VARIANT_BOOL CSkinItem::GettrackMouse(void)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());

    return m_bTrackMouse ? (VARIANT_TRUE) : (VARIANT_FALSE);
}

void CSkinItem::SettrackMouse(VARIANT_BOOL newVal)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());

    newVal = (newVal != VARIANT_FALSE);
    if (m_bTrackMouse == newVal)
        return;

    m_bTrackMouse = newVal;
}

VARIANT_BOOL CSkinItem::GetskinScrollBar()
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());

    return m_bSkinScrollBar ? (VARIANT_TRUE) : (VARIANT_FALSE);
}

void CSkinItem::SetskinScrollBar(VARIANT_BOOL newVal)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());

    newVal = (newVal != VARIANT_FALSE);
    if (m_bSkinScrollBar == newVal)
        return;

    m_bSkinScrollBar = newVal;
}

VARIANT_BOOL CSkinItem::GetskinScrollStretchPage()
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());

    return m_bSkinScrollStretchPage ? (VARIANT_TRUE) : (VARIANT_FALSE);
}

void CSkinItem::SetskinScrollStretchPage(VARIANT_BOOL newVal)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());

    newVal = (newVal != VARIANT_FALSE);
    if (m_bSkinScrollStretchPage == newVal)
        return;

    m_bSkinScrollStretchPage = newVal;
}

LONG CSkinItem::GetskinScrollBarChild()
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());

    return m_lSkinScrollBarChild;
}

void CSkinItem::SetskinScrollBarChild(LONG newVal)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());

    // TODO: 在此添加调度处理程序代码
    if (m_lSkinScrollBarChild == newVal)
        return;

    m_lSkinScrollBarChild = newVal;
}

LONG CSkinItem::GetskinScrollBarWidth()
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());

    return m_lSkinScrollBarWidth;
}

void CSkinItem::SetskinScrollBarWidth(LONG newVal)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());

    // TODO: 在此添加调度处理程序代码
    if (m_lSkinScrollBarWidth == newVal)
        return;

    m_lSkinScrollBarWidth = newVal;
    InvalidateRect();
}

LONG CSkinItem::GetskinScrollBarHeight()
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());

    return m_lSkinScrollBarHeight;
}

void CSkinItem::SetskinScrollBarHeight(LONG newVal)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());

    // TODO: 在此添加调度处理程序代码
    if (m_lSkinScrollBarHeight == newVal)
        return;

    m_lSkinScrollBarHeight = newVal;
    InvalidateRect();
}

LONG CSkinItem::GetskinScrollBarSideWidth()
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());

    return m_lSkinScrollBarSideWidth;
}

void CSkinItem::SetskinScrollBarSideWidth(LONG newVal)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());

    // TODO: 在此添加调度处理程序代码
    if (m_lSkinScrollBarSideWidth == newVal)
        return;

    m_lSkinScrollBarSideWidth = newVal;
    InvalidateRect();
}

LONG CSkinItem::GetskinScrollBarSideHeight()
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());

    return m_lSkinScrollBarSideHeight;
}

void CSkinItem::SetskinScrollBarSideHeight(LONG newVal)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());

    // TODO: 在此添加调度处理程序代码
    if (m_lSkinScrollBarSideHeight == newVal)
        return;

    m_lSkinScrollBarSideHeight = newVal;
    InvalidateRect();
}

BACKGROUND_IMAGE_LIST(CSkinItem, IMPLEMENT_IMAGE_PROPERTY);

SCROLLBAR_IMAGE_LIST(CSkinItem, IMPLEMENT_IMAGE_PROPERTY);

LONG CSkinItem::SendSkinMessage(LONG lCode, LONG lParam)
{
	if (!::IsWindow(m_hWnd))
		return 0;

	return (LONG)::SendMessage(m_hWnd, WM_IMSKIN_NOTIFY, lCode,(LPARAM)lParam);
}

VARIANT_BOOL CSkinItem::PostSkinMessage(LONG lCode, LONG lParam)
{
	if (!::IsWindow(m_hWnd))
		return (VARIANT_FALSE);

	BOOL ret = ::PostMessage(m_hWnd, WM_IMSKIN_NOTIFY, lCode,(LPARAM)lParam);
	return ret ? (VARIANT_TRUE) : (VARIANT_FALSE);
}

void CSkinItem::DebugMessage(VARIANT* pvarShow)
{
	CComVariant var(*pvarShow);
	var.ChangeType(VT_BSTR);
	CString strDbg(var.bstrVal);
	HWND hBoxParent = NULL;
	if (::IsWindow(m_hWnd))
		hBoxParent = m_hWnd;
	::MessageBox(hBoxParent, strDbg, _T("skin"), MB_OK | MB_TOPMOST);
}

VARIANT_BOOL CSkinItem::IsIconic()
{
	if (!::IsWindow(m_hWnd))
		return VARIANT_FALSE;

	BOOL bTrue = ::IsIconic(m_hWnd);
	return bTrue ? (VARIANT_TRUE) : (VARIANT_FALSE);
}

VARIANT_BOOL CSkinItem::IsBinded()
{
	return ::IsWindow(m_hWnd) ? (VARIANT_TRUE) : (VARIANT_FALSE);
}

VARIANT_BOOL CSkinItem::IsMenuOwner()
{
    HWND hMenuOwner = NULL;
    CSkinItem::GetGUIThreadHwndHelper(m_hWnd, CSkinItem::GUI_THREAD_HWND_MENUOWNER, hMenuOwner);
    return (hMenuOwner != NULL && hMenuOwner == m_hWnd) ? (VARIANT_TRUE) : (VARIANT_FALSE);
}

VARIANT_BOOL CSkinItem::IsZoomed()
{
	if (!::IsWindow(m_hWnd))
		return VARIANT_FALSE;

	BOOL bTrue = ::IsZoomed(m_hWnd);
	return bTrue ? (VARIANT_TRUE) : (VARIANT_FALSE);
}

VARIANT_BOOL CSkinItem::Show(LPCTSTR showPara)
{
	if (!::IsWindow(m_hWnd))
		return (VARIANT_FALSE);

	CString strShow(showPara);
	if (strShow.CompareNoCase(_T("maximize")) == 0){
		return ::ShowWindow(m_hWnd, SW_MAXIMIZE) ? (VARIANT_TRUE) : (VARIANT_FALSE);
	}else if (strShow.CompareNoCase(_T("minimize")) == 0){
		return ::ShowWindow(m_hWnd, SW_MINIMIZE) ? (VARIANT_TRUE) : (VARIANT_FALSE);
	}else if (strShow.CompareNoCase(_T("restore")) == 0){
		return ::ShowWindow(m_hWnd, SW_RESTORE) ? (VARIANT_TRUE) : (VARIANT_FALSE);
	}else if (strShow.CompareNoCase(_T("show")) == 0){
		return ::ShowWindow(m_hWnd, SW_SHOW) ? (VARIANT_TRUE) : (VARIANT_FALSE);
	}else if (strShow.CompareNoCase(_T("hide")) == 0){
		return ::ShowWindow(m_hWnd, SW_HIDE) ? (VARIANT_TRUE) : (VARIANT_FALSE);
	}

	return (VARIANT_FALSE);
}

VARIANT_BOOL CSkinItem::Invalidate(void)
{
	return InvalidateRect() ? (VARIANT_TRUE) : (VARIANT_FALSE);
}

LONG CSkinItem::GetKeyState(LONG uKey)
{
    return (LONG)::GetKeyState(uKey);
}

#ifdef V8_JS_ENGINE_SUPPORT
Handle<Value> CSkinItem::V8SendSkinMessage(const Arguments& args)
{
    HandleScope handleScope;

    LONG ret = SendSkinMessage(Handle<Integer>::Cast(args[0])->Int32Value(), Handle<Integer>::Cast(args[1])->Int32Value());

    Handle<Value> result = Integer::New(ret);

    return handleScope.Close(result);
}

Handle<Value> CSkinItem::V8PostSkinMessage(const Arguments& args)
{
    HandleScope handleScope;

    Handle<Value> result =  PostSkinMessage(Handle<Integer>::Cast(args[0])->Int32Value(), Handle<Integer>::Cast(args[1])->Int32Value())
        ? (Boolean::New(true)) : (Boolean::New(false));

    return handleScope.Close(result);
}

Handle<Value> CSkinItem::V8DebugMessage(const Arguments& args)
{
    HandleScope handleScope;

    CComVariant var = ValueToVariant(args[0]);
    DebugMessage(&var);

    Handle<Value> result = v8::Undefined();

    return handleScope.Close(result);
}

Handle<Value> CSkinItem::V8IsIconic(const Arguments& args)
{
    HandleScope handleScope;

    Handle<Value> result = IsIconic() ? (Boolean::New(true)) : (Boolean::New(false));

    return handleScope.Close(result);
}

Handle<Value> CSkinItem::V8IsZoomed(const Arguments& args)
{
    HandleScope handleScope;

    Handle<Value> result = IsZoomed() ? (Boolean::New(true)) : (Boolean::New(false));

    return handleScope.Close(result);
}

Handle<Value> CSkinItem::V8IsBinded(const Arguments& args)
{
	HandleScope handleScope;

	Handle<Value> result = IsBinded() ? (Boolean::New(true)) : (Boolean::New(false));

	return handleScope.Close(result);
}

Handle<Value> CSkinItem::V8IsMenuOwner(const Arguments& args)
{
    HandleScope handleScope;

    Handle<Value> result = IsMenuOwner() ? (Boolean::New(true)) : (Boolean::New(false));

    return handleScope.Close(result);
}

Handle<Value> CSkinItem::V8Show(const Arguments& args)
{
    HandleScope handleScope;

    Handle<Value> result = Show(ValueToCString(Handle<String>::Cast(args[0]))) ? (Boolean::New(true)) : (Boolean::New(false));

    return handleScope.Close(result);
}

Handle<Value> CSkinItem::V8Invalidate(const Arguments& args)
{
    HandleScope handleScope;

    Handle<Value> result = Invalidate() ? (Boolean::New(true)) : (Boolean::New(false));

    return handleScope.Close(result);
}

v8::Handle<v8::Value> CSkinItem::V8GetKeyState(const v8::Arguments& args)
{
    HandleScope handleScope;

    CComVariant varKeyValue = ValueToVariant(Handle<Integer>::Cast(args[0]));

    LONG lState = GetKeyState(varKeyValue.intVal);
    Handle<Value> result = Integer::New(lState);

    return handleScope.Close(result);
}

Handle<Value> CSkinItem::V8InternalCallMethod(const CString& funcName, const Arguments& args)
{
    if (funcName.CompareNoCase(_T("SendSkinMessage")) == 0) {
        return V8SendSkinMessage(args);
    } else if (funcName.CompareNoCase(_T("PostSkinMessage")) == 0) {
        return V8PostSkinMessage(args);
    } else if (funcName.CompareNoCase(_T("DebugMessage")) == 0) {
        return V8DebugMessage(args);
    } else if (funcName.CompareNoCase(_T("IsIconic")) == 0) {
        return V8IsIconic(args);
    } else if (funcName.CompareNoCase(_T("IsZoomed")) == 0) {
        return V8IsZoomed(args);
	} else if (funcName.CompareNoCase(_T("IsBinded")) == 0) {
		return V8IsBinded(args);
    } else if (funcName.CompareNoCase(_T("IsMenuOwner")) == 0) {
        return V8IsMenuOwner(args);
    } else if (funcName.CompareNoCase(_T("Show")) == 0) {
        return V8Show(args);
    } else if (funcName.CompareNoCase(_T("Invalidate")) == 0) {
        return V8Invalidate(args);
    } else if (funcName.CompareNoCase(_T("GetKeyState")) == 0) {
        return V8GetKeyState(args);
    } 

    return CSkinBase::V8InternalCallMethod(funcName, args);
}

#endif //V8_JS_ENGINE_SUPPORT
