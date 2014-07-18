#pragma once

#include "../Common.h"
#include "IUITreeContainer.h"
#include "SimpleTipCtrl.h"
#include "./UIHelper.h"
#include <map>
#include <vector>
#include "../../../../include/coolscroll.h"

#ifdef _DEBUG 
#pragma comment (lib, "CoolSBd.lib") 
#else 
#pragma comment (lib, "CoolSB.lib") 
#endif //_DEBUG

namespace common { namespace ui
{

#define SUBCLASS_PROLOGUE(className) \
    T* pT = static_cast<T*>(this); (pT); \
    className* pThis = static_cast<className*>(this); (pThis); \

typedef struct tagTREENODE
{
    IUITreeNode* pNode;
    UINT id;
    UINT reserved;
}TREENODE , *HTREENODE;

template<class T>
class IUITreeContainerWndImpl : 
    public CWnd, 
    public IUITreeContainer
{
public:
    typedef T SubClassType;

protected:
    DECLARE_MESSAGE_MAP()

#define RESOURCE_LIST(V) \
    V(COLORREF, int, -1) \
    V(HICON, DestroyIcon, NULL) \
    V(HFONT, DeleteObject, NULL) \
    V(HCURSOR, DestroyCursor, NULL) \
    V(HPEN, DeleteObject, NULL) \
    V(HBRUSH, DeleteObject, NULL) \
    V(HBITMAP, DeleteObject, NULL) \

public:
    IUITreeContainerWndImpl()
    {
        m_hDefaultCursor = ::LoadCursor(NULL, IDC_ARROW);
    };

    virtual ~IUITreeContainerWndImpl()
    {
        ::DeleteObject(m_hDefaultCursor);
        m_hDefaultCursor = NULL;
#define RESOURCE_CLEAR(type, dtor, invalid) \
        Clear##type();
        RESOURCE_LIST(RESOURCE_CLEAR);
#undef RESOURCE_CLEAR
    };

public:
    //创建控件
    BOOL Create(CWnd* pParent, const RECT& rect, UINT uID);

public:
    //滚动
    BOOL GetScrollInfo(int fnBar, LPSCROLLINFO lpsi);
    int SetScrollInfo(int fnBar, LPSCROLLINFO lpsi, BOOL fRedraw);
    BOOL GetScrollPos(LONG& x, LONG& y);
    BOOL SetScrollPos(LONG x, LONG y);
    BOOL GetScrollSize(LONG& width, LONG& height);
    BOOL SetScrollSize(LONG width, LONG height);

public:
    //坐标转换
    BOOL WindowToTree(POINT& pt);
    BOOL WindowToTree(RECT& pt);
    BOOL TreeToWindow(POINT& pt);
    BOOL TreeToWindow(RECT& pt);

protected:
    afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
    afx_msg void OnDestroy();
    afx_msg void OnNcCalcSize(BOOL bCalcValidRects, NCCALCSIZE_PARAMS* lpncsp);
    afx_msg BOOL OnEraseBkgnd(CDC* pDC);
    afx_msg void OnPaint();
    afx_msg void OnSize(UINT nType, int cx, int cy);
    afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
    afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
    afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
    afx_msg void OnMouseMove(UINT nFlags, CPoint point);
    afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
    afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
    afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
    afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
    afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
    afx_msg void OnRButtonDblClk(UINT nFlags, CPoint point);
    afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
    afx_msg LRESULT OnMouseHover(WPARAM wParam, LPARAM lParam);
    afx_msg LRESULT OnMouseLeave(WPARAM wParam, LPARAM lParam);

    virtual BOOL PreTranslateMessage(MSG* pMsg)
    {
        return CWnd::PreTranslateMessage(pMsg);
    };

public:
    //IUITreeNode, IUITreeContainer
    virtual void OnChildNotify(IUITreeNode* pChild, UINT uCode, LPTNN lpTNN) 
    {
        SUBCLASS_PROLOGUE(IUITreeContainer);

        IUITreeContainer::OnChildNotify(pChild, uCode, lpTNN);

        if (uCode == TNN_POSCHANGED) {
            CRect newRect = pThis->GetRect();
            SetScrollSize(newRect.Width(), newRect.Height());
            Invalidate();
        }
    };

    virtual HWND GetHWND() 
    {
        SUBCLASS_PROLOGUE(IUITreeContainer);

        return GetSafeHwnd();
    };

    virtual bool EnsureVisible(IUITreeNode* pNode)
    {
        CRect rect = pNode->GetRect();
        pNode->ParentToChild(rect);
        pNode->ChildToAncestor(rect);
        CRect wndRect;
        GetClientRect(&wndRect);
        LONG x = 0, y = 0;
        GetScrollPos(x, y);
        wndRect.OffsetRect(x, y);
        CRect intersectRect;
        intersectRect.IntersectRect(&wndRect, &rect);
        if (intersectRect.Height() < rect.Height()) {
            SetScrollPos(0, rect.top - wndRect.Height() / 2);//
        }
        return true;
    };

    virtual bool ShowTip(IUITreeNode* pNode, LPCTSTR lpzText, LPCTSTR /*lpzTitle*/)
    {
        CRect rect = pNode->GetRect();
        pNode->ParentToChild(rect);
        pNode->ChildToAncestor(rect);
        TreeToWindow(rect);
        ClientToScreen(&rect);

        if( m_toolTips.BeHide() == FALSE)
            m_toolTips.Hide();

        CPoint pt( rect.left + 25, rect.top + rect.Height() + 8);
        m_toolTips.SetFont( GetFont());
		m_toolTips.SetFitted(TRUE);
        m_toolTips.Show( lpzText, this, &pt, 340);

        return true;
    };

    virtual void HideTip()
    {
        if( m_toolTips.GetSafeHwnd())
        {
            m_toolTips.TerminateTimer( CSimpleTipCtrl::timerShow);
            m_toolTips.Hide();
        }
    };

    virtual void GetOffset(POINT& offset)
    {
        offset.x = 0;
        offset.y = 0;
        WindowToTree(offset);
    };

    virtual COLORREF GetColor(int iIndex = 0)
    {
        return GetCOLORREF(iIndex);
    };

    virtual HFONT GetFont(int iIndex = 0)
    {
        return GetHFONT(iIndex);
    };

    virtual HICON GetIcon(int iIndex = 0)
    {
        return GetHICON(iIndex);
    };

    virtual HCURSOR GetCursor(int iIndex = 0)
    {
        return GetHCURSOR(iIndex);
    };

    virtual HPEN GetPen(int iIndex = 0)
    {
        return GetHPEN(iIndex);
    };

    virtual HBRUSH GetBrush(int iIndex = 0)
    {
        return GetHBRUSH(iIndex);
    };

    virtual HBITMAP GetBitmap(int iIndex = 0)
    {
        return GetHBITMAP(iIndex);
    };

    virtual LRESULT SendMessage(UINT /*uMsg*/, IUITreeNode* /*pSender*/, WPARAM /*wParam = 0*/, LPARAM /*lParam = 0*/)
    {
        return 0;
    };

protected:
#define RESOURCE_FUNC(type, dtor, invalid) \
    type Get##type(int index) { \
        std::map<int, type>::iterator finded = m_map##type.find(index); \
        if (finded != m_map##type.end()) \
            return finded->second; \
        return (type)invalid; \
    }; \
    void Set##type(int index, type value) { \
        std::map<int, type>::iterator finded = m_map##type.find(index); \
        if (value == invalid) { \
            if (finded != m_map##type.end()) { \
                dtor(finded->second); \
                m_map##type.erase(finded); \
            } \
        } else { \
            if (finded != m_map##type.end()) { \
                dtor(finded->second); \
            } \
            m_map##type[index] = value; \
        }; \
    }; \
    void Clear##type() { \
        std::map<int, type>::iterator iter = m_map##type.begin(), last = m_map##type.end(); \
        for (; iter!=last; iter++) { \
            dtor(iter->second); \
        }; \
        m_map##type.clear(); \
    };
    RESOURCE_LIST(RESOURCE_FUNC);
#undef RESOURCE_FUNC

protected:
    IUITreeNode* GetNode(HTREENODE hNode)
    {
        std::map<HTREENODE, IUITreeNode*>::iterator finded = m_mapNode.find(hNode);
        if (finded == m_mapNode.end())
            return NULL;

        ASSERT(finded->first->pNode == finded->second);
        return finded->second;
    };

public:
    std::vector<HTREENODE> GetAllNode()
    {
        std::vector<HTREENODE> result;

        std::map<HTREENODE, IUITreeNode*>::iterator iter = m_mapNode.begin(),
            last = m_mapNode.end();
        for (; iter!=last; iter++) {
            result.push_back(iter->first);
        }

        return result;
    };

    std::vector<HTREENODE> GetAllSelected()
    {
        SUBCLASS_PROLOGUE(IUITreeContainer);

        std::vector<IUITreeNode*> vecAllNode = pThis->GetSelected();

        std::vector<HTREENODE> vecAllTreeNode;

        std::vector<IUITreeNode*>::iterator iter = vecAllNode.begin(),
            last = vecAllNode.end();
        for (; iter!=last; iter++) {
            vecAllTreeNode.push_back(FindNode(*iter));
        }

        return vecAllTreeNode;
    }

protected:
    HTREENODE FindNode(IUITreeNode* pNode)
    {
        std::map<HTREENODE, IUITreeNode*>::iterator iter = m_mapNode.begin(),
            last = m_mapNode.end();
        for (; iter!=last; iter++) {
            if (iter->second == pNode)
                return iter->first;
        }

        return NULL;
    };

    HTREENODE AddNode(IUITreeNode* pNode)
    {
        HTREENODE hNode = new TREENODE;
        hNode->pNode = pNode;
        hNode->id = 0;
        m_mapNode.insert(std::pair<HTREENODE, IUITreeNode*>(hNode, (IUITreeNode*)pNode));
        return hNode;
    };

    BOOL RemoveNode(HTREENODE hNode)
    {
        if (GetNode(hNode) == NULL)
            return FALSE;

        delete hNode;
        m_mapNode.erase(hNode);
        return TRUE;
    };

public:
    BOOL RemoveAll()
    {
        SUBCLASS_PROLOGUE(IUITreeContainer);

        while (m_mapNode.size() > 0) {
            std::map<HTREENODE, IUITreeNode*>::iterator iter = m_mapNode.begin();
            pT->Remove(iter->first);
        }
        return TRUE;
    };

    //设置节点数据
    BOOL SetNodeData(HTREENODE hNode, INT_PTR data)
    {
        IUITreeNode* pNode = GetNode(hNode);
        if (pNode == NULL)
            return FALSE;

        pNode->SetData(data);
        return TRUE;
    };

    //获得节点数据
    BOOL GetNodeData(HTREENODE hNode, INT_PTR& data)
    {
        IUITreeNode* pNode = GetNode(hNode);
        if (pNode == NULL)
            return FALSE;

        data = pNode->GetData();
        return TRUE;
    };

private:
    HCURSOR m_hDefaultCursor;
    std::map<HTREENODE, IUITreeNode*> m_mapNode;
    CSimpleTipCtrl	m_toolTips;
#define DECLARE_RESOURCE_MAP(type, dtor, invalid) \
    std::map<int, type> m_map##type;
    RESOURCE_LIST(DECLARE_RESOURCE_MAP)
#undef DECLARE_RESOURCE_MAP
};

// IUITreeContainerWndImpl
BEGIN_TEMPLATE_MESSAGE_MAP(IUITreeContainerWndImpl, SubClassType, CWnd)
    ON_WM_CREATE()
    ON_WM_DESTROY()
    ON_WM_ERASEBKGND()
    ON_WM_PAINT()
    ON_WM_SIZE()
    ON_WM_VSCROLL()
    ON_WM_HSCROLL()
    ON_WM_LBUTTONDOWN()
    ON_WM_LBUTTONDBLCLK()
    ON_WM_LBUTTONUP()
    ON_WM_RBUTTONDOWN()
    ON_WM_RBUTTONUP()
    ON_WM_RBUTTONDBLCLK()
    ON_WM_MOUSEWHEEL()
    ON_WM_MOUSEMOVE()
    ON_WM_NCCALCSIZE()
    ON_WM_SETCURSOR()
    ON_MESSAGE(WM_MOUSEHOVER, OnMouseHover)
    ON_MESSAGE(WM_MOUSELEAVE, OnMouseLeave)
END_MESSAGE_MAP()


// IUITreeContainerWndImpl message handlers
template<typename T>
BOOL IUITreeContainerWndImpl<T>::Create(CWnd* pParent, const RECT& rect, UINT uID)
{
    return CWnd::Create(AfxRegisterWndClass(CS_DBLCLKS), _T(""), (WS_CHILD | WS_VISIBLE | WS_VSCROLL), rect, pParent, uID);
}

template<typename T>
BOOL IUITreeContainerWndImpl<T>::GetScrollInfo(int fnBar, LPSCROLLINFO lpsi)
{
    return CoolSB_GetScrollInfo(m_hWnd, fnBar, lpsi);
}

template<typename T>
int IUITreeContainerWndImpl<T>::SetScrollInfo(int fnBar, LPSCROLLINFO lpsi, BOOL fRedraw)
{
    return CoolSB_SetScrollInfo(m_hWnd, fnBar, lpsi, fRedraw);
}

template<typename T>
BOOL IUITreeContainerWndImpl<T>::GetScrollPos(LONG& x, LONG& y)
{
    x = 0;
    y = CoolSB_GetScrollPos(GetSafeHwnd(), SB_VERT);
    return TRUE;
}

template<typename T>
BOOL IUITreeContainerWndImpl<T>::SetScrollPos(LONG /*x*/, LONG y)
{
    CoolSB_SetScrollPos(GetSafeHwnd(), SB_VERT, y, TRUE);
    return TRUE;
}

template<typename T>
BOOL IUITreeContainerWndImpl<T>::GetScrollSize(LONG& width, LONG& height)
{
    int min = 0, max = 0;
    CoolSB_GetScrollRange(GetSafeHwnd(), SB_VERT, &min, &max);
    width = 0;
    height = max - min;
    return TRUE;
}

template<typename T>
BOOL IUITreeContainerWndImpl<T>::SetScrollSize(LONG /*width*/, LONG height)
{
    SCROLLINFO info = {sizeof(info), SIF_ALL};
    CRect rect;
    GetClientRect(&rect);
    if (height > rect.Height()) {
        CoolSB_GetScrollInfo(GetSafeHwnd(), SB_VERT, &info);
        info.nMin = 0;
        info.nMax = height;
        info.nPage = rect.Height() - 3;
    } 

    return CoolSB_SetScrollInfo(GetSafeHwnd(), SB_VERT, &info, TRUE);
}

template<typename T>
BOOL IUITreeContainerWndImpl<T>::WindowToTree(POINT& pt)
{
    CPoint p;
    if (! GetScrollPos(p.x, p.y))
        return FALSE;

    pt.x += p.x;
    pt.y += p.y;
    return TRUE;
}

template<typename T>
BOOL IUITreeContainerWndImpl<T>::WindowToTree(RECT& pt)
{
    CPoint p;
    if (! GetScrollPos(p.x, p.y))
        return FALSE;

    ::OffsetRect(&pt, p.x, p.y);
    return TRUE;
}

template<typename T>
BOOL IUITreeContainerWndImpl<T>::TreeToWindow(POINT& pt)
{
    CPoint p;
    if (! GetScrollPos(p.x, p.y))
        return FALSE;

    pt.x -= p.x;
    pt.y -= p.y;
    return TRUE;
}

template<typename T>
BOOL IUITreeContainerWndImpl<T>::TreeToWindow(RECT& pt)
{
    CPoint p;
    if (! GetScrollPos(p.x, p.y))
        return FALSE;

    ::OffsetRect(&pt, -p.x, -p.y);
    return TRUE;
}

template<typename T>
int IUITreeContainerWndImpl<T>::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
    if (__super::OnCreate(lpCreateStruct) == -1)
        return -1;

    // TODO:  Add your specialized creation code here
    // CoolSB初始化，由皮肤引擎统一加载
    //InitializeCoolSB(GetSafeHwnd());
    //CoolSB_SetStyle(GetSafeHwnd(), SB_VERT, CSBS_HOTTRACKED);
    //CoolSB_SetSize(GetSafeHwnd(), SB_VERT, 15, 15);
    //CoolSB_SetMinThumbSize(GetSafeHwnd(), SB_VERT, 20);

    SUBCLASS_PROLOGUE(IUITreeContainer);

    CRect rect = pThis->GetRect();
    SetScrollSize(rect.Width(), rect.Height());

    m_toolTips.SetShowDelay( 1000);

    return 0;
}

template<typename T>
void IUITreeContainerWndImpl<T>::OnDestroy()
{
    __super::OnDestroy();

    // TODO: Add your message handler code here
    // CoolSB卸载，由皮肤引擎统一卸载
    //UninitializeCoolSB(GetSafeHwnd());

    SUBCLASS_PROLOGUE(IUITreeContainer);

    return;
}

template<typename T>
void IUITreeContainerWndImpl<T>::OnNcCalcSize(BOOL bCalcValidRects, NCCALCSIZE_PARAMS* lpncsp)
{
    // TODO: Add your message handler code here and/or call default

    __super::OnNcCalcSize(bCalcValidRects, lpncsp);

    SUBCLASS_PROLOGUE(IUITreeContainer);

    return;
}

template<typename T>
BOOL IUITreeContainerWndImpl<T>::OnEraseBkgnd(CDC* /*pDC*/)
{
    // TODO: Add your message handler code here and/or call default

    SUBCLASS_PROLOGUE(IUITreeContainer);

    return TRUE;
}

template<typename T>
void IUITreeContainerWndImpl<T>::OnPaint()
{
    SUBCLASS_PROLOGUE(IUITreeContainer);

    CPaintDC dc(this); // device context for painting

    //Invalid区域，List区域
    CRect rcInvalid = dc.m_ps.rcPaint;
    CMemoryDC memDC(dc.GetSafeHdc(), rcInvalid);

    //填充背景色
    memDC.FillSolidRect(&rcInvalid, RGB(255, 255, 255));

    CPoint offset(0, 0);
    CRect rcDraw = rcInvalid;
    TreeToWindow(offset);
    WindowToTree(rcDraw);

    COffsetDC offsetDC(memDC.GetSafeHdc(), offset);
    pThis->Draw(&memDC, rcDraw);
    offsetDC.Dispose();
}

template<typename T>
void IUITreeContainerWndImpl<T>::OnSize(UINT nType, int cx, int cy)
{
    __super::OnSize(nType, cx, cy);

    // TODO: Add your message handler code here
    SUBCLASS_PROLOGUE(IUITreeContainer);

    CRect rect = pThis->GetRect();
    CRect clientRect;
    ::GetClientRect(GetSafeHwnd(), &clientRect);
    rect.left = clientRect.left;
    rect.right = clientRect.right;
    pThis->SetRect(rect);
    SetScrollSize(rect.Width(), rect.Height());
}

template<typename T>
void IUITreeContainerWndImpl<T>::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
    // TODO: Add your message handler code here and/or call default
    SUBCLASS_PROLOGUE(IUITreeContainer);

    // TODO: Add your message handler code here and/or call default
    SCROLLINFO info = {sizeof(info), SIF_ALL};
    GetScrollInfo(SB_VERT, &info);

    switch(nSBCode)
    {
    case SB_BOTTOM:
        info.nPos = info.nMax - info.nPage;
        break;
    case SB_TOP:
        info.nPos = 0;
        break;
    case SB_THUMBPOSITION:
        info.nPos = info.nTrackPos; 
        break;
    case SB_THUMBTRACK:
        info.nPos = nPos;
        break;
    case SB_PAGEDOWN:
        info.nPos += info.nPage;
        break;
    case SB_PAGEUP:
        info.nPos -= info.nPage;
        break;
    case SB_LINEDOWN:
        info.nPos += 40;
        break;
    case SB_LINEUP:
        info.nPos -= 40;
        break;
    default:
        break;
    }

    if (info.nPos < 0) {
        info.nPos = 0;
    }
    int temp = info.nMax - info.nPage;
    if (info.nPos > temp) {
        info.nPos = temp;
    }

    SetScrollInfo(SB_VERT, &info, TRUE);
    Invalidate();

    __super::OnVScroll(nSBCode, nPos, pScrollBar);
}

template<typename T>
void IUITreeContainerWndImpl<T>::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
    // TODO: Add your message handler code here and/or call default
    SUBCLASS_PROLOGUE(IUITreeContainer);

    __super::OnHScroll(nSBCode, nPos, pScrollBar);
}

template<typename T>
BOOL IUITreeContainerWndImpl<T>::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
    // TODO: Add your message handler code here and/or call default
    SUBCLASS_PROLOGUE(IUITreeContainer);

    // TODO: Add your message handler code here and/or call default
    CPoint point;
    ::GetCursorPos(&point);
    ScreenToClient(&point);
    CPoint pt = point;
    BOOL bSuc = WindowToTree(pt);
    ASSERT(bSuc);

    HCURSOR hCursor = NULL;
    IUITreeNode* pNode = pThis->GetCapture();
    if (pNode == NULL) {
        pNode = pThis->GetDescendant(pt, TNS_VISIBLE, TNS_VISIBLE);
    }
    if (pNode != NULL) {
        pNode->AncestorToChild(pt);
        hCursor = pNode->GetCursor(pt);
    }

    if (hCursor == NULL)
        hCursor = m_hDefaultCursor;

    ::SetCursor(hCursor);

    return __super::OnSetCursor(pWnd, nHitTest, message);
}

template<typename T>
void IUITreeContainerWndImpl<T>::OnMouseMove(UINT nFlags, CPoint point)
{
    // TODO: Add your message handler code here and/or call default
    SUBCLASS_PROLOGUE(IUITreeContainer);

    // TODO: Add your message handler code here and/or call default
    TRACKMOUSEEVENT tme;
    tme.cbSize = sizeof(tme);
    tme.dwFlags  = TME_HOVER | TME_LEAVE;
    tme.hwndTrack = m_hWnd;
    tme.dwHoverTime = HOVER_DEFAULT;
    ::_TrackMouseEvent(&tme);

    CPoint pt = point;
    BOOL bSuc = WindowToTree(pt);
    ASSERT(bSuc);
    pThis->OnMouseMove(pt, nFlags);

    __super::OnMouseMove(nFlags, point);
}

template<typename T>
void IUITreeContainerWndImpl<T>::OnLButtonDown(UINT nFlags, CPoint point)
{
    // TODO: Add your message handler code here and/or call default
    SUBCLASS_PROLOGUE(IUITreeContainer);

    // TODO: Add your message handler code here and/or call default
    SetFocus();

    CPoint pt = point;
    BOOL bSuc = WindowToTree(pt);
    ASSERT(bSuc);
    pThis->OnLButtonDown(pt, nFlags);

    __super::OnLButtonDown(nFlags, point);
}

template<typename T>
void IUITreeContainerWndImpl<T>::OnLButtonUp(UINT nFlags, CPoint point)
{
    // TODO: Add your message handler code here and/or call default
    SUBCLASS_PROLOGUE(IUITreeContainer);

    // TODO: Add your message handler code here and/or call default
    CPoint pt = point;
    BOOL bSuc = WindowToTree(pt);
    ASSERT(bSuc);
    pThis->OnLButtonUp(pt, nFlags);

    __super::OnLButtonUp(nFlags, point);
}

template<typename T>
void IUITreeContainerWndImpl<T>::OnLButtonDblClk(UINT nFlags, CPoint point)
{
    // TODO: Add your message handler code here and/or call default
    SUBCLASS_PROLOGUE(IUITreeContainer);

    // TODO: Add your message handler code here and/or call default
    CPoint pt = point;
    BOOL bSuc = WindowToTree(pt);
    ASSERT(bSuc);
    pThis->OnLButtonDblClk(pt, nFlags);

    __super::OnLButtonDblClk(nFlags, point);
}

template<typename T>
void IUITreeContainerWndImpl<T>::OnRButtonDown(UINT nFlags, CPoint point)
{
    // TODO: Add your message handler code here and/or call default
    SUBCLASS_PROLOGUE(IUITreeContainer);

    // TODO: Add your message handler code here and/or call default
    CPoint pt = point;
    BOOL bSuc = WindowToTree(pt);
    ASSERT(bSuc);
    pThis->OnRButtonDown(pt, nFlags);

    __super::OnRButtonDown(nFlags, point);
}

template<typename T>
void IUITreeContainerWndImpl<T>::OnRButtonUp(UINT nFlags, CPoint point)
{
    // TODO: Add your message handler code here and/or call default
    SUBCLASS_PROLOGUE(IUITreeContainer);

    // TODO: Add your message handler code here and/or call default
    CPoint pt = point;
    BOOL bSuc = WindowToTree(pt);
    ASSERT(bSuc);
    pThis->OnRButtonUp(pt, nFlags);

    __super::OnRButtonUp(nFlags, point);
}

template<typename T>
void IUITreeContainerWndImpl<T>::OnRButtonDblClk(UINT nFlags, CPoint point)
{
    // TODO: Add your message handler code here and/or call default
    SUBCLASS_PROLOGUE(IUITreeContainer);

    // TODO: Add your message handler code here and/or call default
    CPoint pt = point;
    BOOL bSuc = WindowToTree(pt);
    ASSERT(bSuc);
    pThis->OnRButtonDblClk(pt, nFlags);

    __super::OnRButtonDblClk(nFlags, point);
}

template<typename T>
BOOL IUITreeContainerWndImpl<T>::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
    // TODO: Add your message handler code here and/or call default
    SUBCLASS_PROLOGUE(IUITreeContainer);

    // TODO: Add your message handler code here and/or call default
    SCROLLINFO info = {sizeof(info), SIF_ALL};
    GetScrollInfo(SB_VERT, &info);

    if (info.nMax > info.nMin) {
        int iPos = info.nPos - zDelta/120 * 40;
        if (iPos < info.nMin) {
            iPos = info.nMin;
        }
        int temp = info.nMax - info.nPage;
        if (iPos > temp) {
            iPos = temp;
        }
        CPoint oldPos;
        GetScrollPos(oldPos.x, oldPos.y);
        if (iPos != oldPos.y) {
            info.nPos = iPos;
            SetScrollInfo(SB_VERT, &info, TRUE);
            Invalidate();
        }
    }

    return __super::OnMouseWheel(nFlags, zDelta, pt);
}

template<typename T>
LRESULT IUITreeContainerWndImpl<T>::OnMouseHover(WPARAM /*wParam*/, LPARAM /*lParam*/)
{
    SUBCLASS_PROLOGUE(IUITreeContainer);

    CPoint pt;
    GetCursorPos(&pt);
    ScreenToClient(&pt);
    BOOL bSuc = WindowToTree(pt);
    ASSERT(bSuc);
    pThis->OnMouseHover(pt, 0);

    return 0;
}

template<typename T>
LRESULT IUITreeContainerWndImpl<T>::OnMouseLeave(WPARAM /*wParam*/, LPARAM /*lParam*/)
{
    SUBCLASS_PROLOGUE(IUITreeContainer);

    CPoint pt;
    GetCursorPos(&pt);
    ScreenToClient(&pt);
    BOOL bSuc = WindowToTree(pt);
    ASSERT(bSuc);
    pThis->OnMouseLeave(pt, 0);

    return 0;
}

typedef struct tagNMTREELISTITEM
{
    NMHDR hdr;
    HTREENODE hNode;
} NMTREELISTITEM, NMTL, *LPNMTREELISTITEM, *LPNMTL;

template <typename T, typename ItemType, typename ItemTypeProperties>
class IUITreeListContainerWndImpl :
    public IUITreeContainerWndImpl<T>
{
public:
    IUITreeListContainerWndImpl()
    :
    m_pListRoot(NULL)
    {
        m_pListRoot = new IUIListTreeNode(this);
        AddChild(m_pListRoot);
    }
    ~IUITreeListContainerWndImpl()
    {
        RemoveAll();
    }

public:
    HTREENODE AddItem(const ItemTypeProperties& data)
    {
        ItemType* pNode = new ItemType(this);
        
        pNode->SetProperties(data);

        CSize szNode(0, 0);
        pNode->CalcSize(szNode);
        CRect rcRect = GetRect();
        szNode.cx = rcRect.Width();
        m_pListRoot->AddListChild(pNode, -1, szNode);

        return AddNode(pNode);
    }

    BOOL UpdateItem(HTREENODE hNode, const ItemTypeProperties& data)
    {
        ItemType* pNode = static_cast<ItemType*>(GetNode(hNode));
        if (pNode == NULL)
            return FALSE;

        if (! pNode->IsChildOf(m_pListRoot))
            return FALSE;

        pNode->SetProperties(data);
        CSize szNode(0, 0);
        pNode->CalcSize(szNode);
        CRect rcRect = GetRect();
        szNode.cx = rcRect.Width();

        return m_pListRoot->UpdateListChild(pNode, szNode);
    }

    BOOL Remove(HTREENODE hNode)
    {
        IUITreeNode* pNode = static_cast<IUITreeNode*>(GetNode(hNode));
        if (pNode == NULL)
            return FALSE;

        if (! pNode->IsChildOf(m_pListRoot))
            return FALSE;

        m_pListRoot->RemoveListChild(pNode);
        return RemoveNode(hNode);
    }

protected:
    LRESULT SendMessage(UINT uMsg, IUITreeNode* pSender, WPARAM wParam = 0, LPARAM lParam = 0)
    {
        UNREFERENCED_PARAMETER(lParam);

        if (uMsg == WM_NOTIFY) 
        {
            INT_PTR data = pSender->GetData();
            UNREFERENCED_PARAMETER(data);
            NMTL hrt = {0};
            hrt.hdr.hwndFrom = GetSafeHwnd();
            hrt.hdr.code = (UINT)wParam;
            hrt.hdr.idFrom = GetDlgCtrlID();
            hrt.hNode = FindNode(pSender);
            return ::SendMessage(::GetParent(GetSafeHwnd()), WM_NOTIFY, GetDlgCtrlID(), (LPARAM)&hrt);
        }

        return 0;
    }

protected:
    IUIListTreeNode* m_pListRoot;
};

} } //namespace common::ui
