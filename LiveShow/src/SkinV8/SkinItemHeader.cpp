// SkinItemHeader.cpp : 实现文件
//

#include "stdafx.h"
#include "SkinItemHeader.h"
#include "RenderEngine.h"


// CSkinItemHeader

IMPLEMENT_DYNCREATE(CSkinItemHeader, CSkinItem)
CSkinItemHeader::CSkinItemHeader()
{
	EnableAutomation();
	
	// 为了使应用程序在 OLE 自动化对象处于活动状态时保持
	//	运行，构造函数调用 AfxOleLockApp。

    m_clrItemBkColorNormal = RGB(234, 234, 234);
    m_clrItemBkColorHover = RGB(234, 234, 234);
    m_clrItemBkColorDown = RGB(234, 234, 234);

    m_nItemInterval = 2;
    m_clrItemIntervalBkColor = RGB(255, 255, 255);

    m_clrTailItemBkColor = RGB(255, 255, 255);

    m_nTextMargin = 4;

    m_bShowHeaderIcon = FALSE;

    m_iLastMouseInItem = -1;

	AfxOleLockApp();
}

CSkinItemHeader::~CSkinItemHeader()
{
	// 为了在用 OLE 自动化创建所有对象后终止应用程序，
	// 	析构函数调用 AfxOleUnlockApp。
	
	AfxOleUnlockApp();
}


void CSkinItemHeader::OnFinalRelease()
{
	// 释放了对自动化对象的最后一个引用后，将调用
	// OnFinalRelease。基类将自动
	// 删除该对象。在调用该基类之前，请添加您的
	// 对象所需的附加清除代码。

	CSkinItem::OnFinalRelease();
}


BEGIN_MESSAGE_MAP(CSkinItemHeader, CSkinItem)
END_MESSAGE_MAP()


BEGIN_SKIN_DISPATCH_MAP(CSkinItemHeader, CSkinItem)
	SKINBASE_DISP_PROPERTIES(CSkinItemHeader)
	SKINITEM_DISP_PROPERTIES(CSkinItemHeader)

    SKIN_DISP_PROPERTY(CSkinItemHeader, itemBkColorNormal, VT_I4)
    SKIN_DISP_PROPERTY(CSkinItemHeader, itemBkColorHover, VT_I4)
    SKIN_DISP_PROPERTY(CSkinItemHeader, itemBkColorDown, VT_I4)
    SKIN_DISP_PROPERTY(CSkinItemHeader, itemInterval, VT_I4)
    SKIN_DISP_PROPERTY(CSkinItemHeader, itemIntervalBkColor, VT_I4)
    SKIN_DISP_PROPERTY(CSkinItemHeader, tailItemBkColor, VT_I4)
    SKIN_DISP_PROPERTY(CSkinItemHeader, bitmapMargin, VT_I4)
    SKIN_DISP_PROPERTY(CSkinItemHeader, textMargin, VT_I4)
    SKIN_DISP_PROPERTY(CSkinItemHeader, showHeaderIcon, VT_BOOL)

END_SKIN_DISPATCH_MAP(CSkinItemHeader)

// 注意: 我们添加 IID_ISkinItemHeader 支持
//以支持来自 VBA 的类型安全绑定。此 IID 必须同附加到 .IDL 文件中的
//调度接口的 GUID 匹配。

// {8904F929-D3FA-4F95-9B50-274ED8A50DE9}
static const IID IID_ISkinItemHeader =
{ 0x8904F929, 0xD3FA, 0x4F95, { 0x9B, 0x50, 0x27, 0x4E, 0xD8, 0xA5, 0xD, 0xE9 } };

BEGIN_INTERFACE_MAP(CSkinItemHeader, CSkinItem)
	INTERFACE_PART(CSkinItemHeader, IID_ISkinItemHeader, Dispatch)
END_INTERFACE_MAP()

// {8CE3297B-8EDE-4BFB-9EB7-5361C1492FFC}
IMPLEMENT_OLECREATE_FLAGS(CSkinItemHeader, "Skin.SkinItemHeader", afxRegApartmentThreading, 0x8ce3297b, 0x8ede, 0x4bfb, 0x9e, 0xb7, 0x53, 0x61, 0xc1, 0x49, 0x2f, 0xfc)


// CSkinItemHeader 消息处理程序
BOOL CSkinItemHeader::GetItemCount(int& count)
{
	if (! ::IsWindow(m_hWnd))
		return FALSE;

	count = Header_GetItemCount(m_hWnd);
	return TRUE;
}

BOOL CSkinItemHeader::GetItemText(int iItem, CString& str)
{
	if (! ::IsWindow(m_hWnd))
		return FALSE;

	HDITEM hdi;
	memset(&hdi, 0, sizeof(HDITEM));
	hdi.mask = HDI_TEXT;
	hdi.pszText = str.GetBuffer(1024);
	hdi.cchTextMax = 1023;

	BOOL bGet = Header_GetItem(m_hWnd, iItem, &hdi);

	str.ReleaseBuffer();

	return bGet;
}

BOOL CSkinItemHeader::GetItemRect(int iItem, CRect& rect)
{
	if (! ::IsWindow(m_hWnd))
		return FALSE;

	return Header_GetItemRect(m_hWnd, iItem, &rect);
}

BOOL CSkinItemHeader::GetItemState(int iItem, UINT& state)
{
    if (! ::IsWindow(m_hWnd))
        return FALSE;

    CPoint cursorPt(0, 0);
    ::GetCursorPos(&cursorPt);
    ::ScreenToClient(m_hWnd, &cursorPt);

    CRect itemRect(0, 0, 0, 0);
    if (! GetItemRect(iItem, itemRect))
        return FALSE;

    BOOL bMouseIn = itemRect.PtInRect(cursorPt);
    BOOL bCaptured = (::GetCapture() == m_hWnd);

    if (bMouseIn) {
        if (bCaptured) {
            state = ODS_SELECTED;
        } else {
            state = ODS_HOTLIGHT;
        }
    } else {
        state = ODS_DEFAULT;
    }

    return TRUE;

    //HDITEM itm;
    //::memset(&itm, 0, sizeof(itm));

    //Header_GetItem(m_hWnd, iItem, &rect);
    //state = ODS_DEFAULT;

    //return TRUE;
}

BOOL CSkinItemHeader::GetItemImage(int iItem, int& iImage)
{
    if (! ::IsWindow(m_hWnd))
        return FALSE;

    HDITEM hdi;
    memset(&hdi, 0, sizeof(HDITEM));
    hdi.mask = HDI_IMAGE;
    hdi.iImage = -1;

    BOOL bGet = Header_GetItem(m_hWnd, iItem, &hdi);

    iImage = bGet ? (hdi.iImage) : (-1);

    return bGet;
}

BOOL CSkinItemHeader::GetImageList(HIMAGELIST& hImageList)
{
    if (! ::IsWindow(m_hWnd))
        return FALSE;

    hImageList = Header_GetImageList(m_hWnd);
    return TRUE;
}

BOOL CSkinItemHeader::GetBitmapMargin(LONG& margin)
{
    if (! ::IsWindow(m_hWnd))
        return FALSE;

#if (_WIN32_IE >= 0x0500)
    margin = Header_GetBitmapMargin(m_hWnd);
    return TRUE;
#else
    margin = 0;
    return FALSE;
#endif
}

BOOL CSkinItemHeader::SetBitmapMargin(LONG margin)
{
    if (! ::IsWindow(m_hWnd))
        return FALSE;

#if (_WIN32_IE >= 0x0500)
    Header_SetBitmapMargin(m_hWnd, margin);
    return TRUE;
#else
    return FALSE;
#endif
}

BOOL CSkinItemHeader::GetPointInItem(POINT& point, int& iItem)
{
    iItem = -1;

    if (! ::IsWindow(m_hWnd))
        return FALSE;

    int count = 0;
    if (! GetItemCount(count))
        return FALSE;

    for (int i=0; i<count; i++) {
        CRect itemRect;
        if (! GetItemRect(i, itemRect))
            continue;
        if (itemRect.PtInRect(point)) {
            iItem = i;
            break;
        }
    }

    return TRUE;
}

BOOL CSkinItemHeader::UpdateSkinItemProperty()
{
	if (! CSkinItem::UpdateSkinItemProperty())
		return FALSE;

	return TRUE;
}

void CSkinItemHeader::DrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	if (lpDrawItemStruct->CtlType != ODT_HEADER)
		return;

    HDC hDC = lpDrawItemStruct->hDC;

    int itemID = lpDrawItemStruct->itemID;

	CRect itemRect = lpDrawItemStruct->rcItem;

	UINT itemState = lpDrawItemStruct->itemState;

    LONG itemBkColor = RGB(123, 123, 123);
    if (m_clrItemBkColorNormal != -1)
        itemBkColor = m_clrItemBkColorNormal;
    if (itemState == ODS_DEFAULT && m_clrItemBkColorNormal != -1) {
        itemBkColor = m_clrItemBkColorNormal;
    }
    if (itemState == ODS_HOTLIGHT && m_clrItemBkColorHover != -1) {
        itemBkColor = m_clrItemBkColorHover;
    }
    if (itemState == ODS_SELECTED && m_clrItemBkColorDown != -1) {
        itemBkColor = m_clrItemBkColorDown;
    }

    if (GetColorTransform())
        GetEngine()->TransformColor(itemBkColor, itemBkColor);

    CString itemText;
    GetItemText(itemID, itemText);

    //Item之间的间隙
    LONG leftInterval = m_nItemInterval;
    LONG rightInterval = m_nItemInterval - leftInterval;
    if (itemID == 0)
        leftInterval = 0;

    //绘制Item背景色
    CRect bkRect = itemRect;
    bkRect.left += leftInterval;
    bkRect.right -= rightInterval;
    CRenderEngine::FillRect(hDC, bkRect, itemBkColor);

    //绘制间隙
    LONG clrIntervalColor = m_clrItemIntervalBkColor;
    if (GetColorTransform())
        GetEngine()->TransformColor(clrIntervalColor, clrIntervalColor);
    CRect intervalRect = itemRect;
    intervalRect.right = intervalRect.left + leftInterval;
    CRenderEngine::FillRect(hDC, intervalRect, clrIntervalColor);
    intervalRect = itemRect;
    intervalRect.left = intervalRect.right - rightInterval;
    CRenderEngine::FillRect(hDC, intervalRect, clrIntervalColor);

    //绘制图标
    HIMAGELIST hImageList = NULL;
    GetImageList(hImageList);
    int iImage = -1;
    GetItemImage(itemID, iImage);
    CRect imageRect(0, 0, 0, 0);
    imageRect.OffsetRect(bkRect.left, bkRect.top);
    IMAGEINFO imgInfo = {0};
    if (m_bShowHeaderIcon && hImageList != NULL && iImage >= 0 && ImageList_GetImageInfo(hImageList, iImage, &imgInfo)) {
        imageRect = imgInfo.rcImage;
        imageRect.OffsetRect(-imageRect.left, -imageRect.top);
        imageRect.OffsetRect(bkRect.left, bkRect.top);
        LONG imageMargin = GetbitmapMargin();
        imageRect.OffsetRect(imageMargin + 4, (bkRect.Height() - imageRect.Height()) / 2);
        ::ImageList_Draw(hImageList, iImage, hDC, imageRect.left, imageRect.top, 
            ILD_NORMAL);
    }

    //绘制文字
    CRect textRect = bkRect;
    textRect.left = imageRect.right + m_nTextMargin;
    m_textStyleNormal.DrawText(hDC, itemText, textRect);
}

BOOL CSkinItemHeader::OnMouseMove(UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* lpResult)
{
    LONG x = GETXPARAM(lParam);
    LONG y = GETYPARAM(lParam);
    UINT flag = (UINT)wParam;

    CPoint pt(x, y);
    int iItem = -1;
    GetPointInItem(pt, iItem);
    if (m_iLastMouseInItem != iItem) { //鼠标所在的item发生改变
        if (m_iLastMouseInItem != -1) {
            CRect rect;
            if (GetItemRect(m_iLastMouseInItem, rect)) {
                InvalidateRect(&rect);
            }
        }
        if (iItem != -1) {
            CRect rect;
            if (GetItemRect(iItem, rect)) {
                InvalidateRect(&rect);
            }
        }
        m_iLastMouseInItem = iItem;
    }

    return CSkinItem::OnMouseMove(uMsg, wParam, lParam, lpResult);
}

BOOL CSkinItemHeader::OnMouseLeave(UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* lpResult)
{
    CRect rect;
    if (m_iLastMouseInItem != -1 && GetItemRect(m_iLastMouseInItem, rect)) {
        InvalidateRect(&rect);
    }
    m_iLastMouseInItem = -1;

    return CSkinItem::OnMouseLeave(uMsg, wParam, lParam, lpResult);
}

BOOL CSkinItemHeader::OnNcPaint(UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* lpResult)
{
    return FALSE;
}

BOOL CSkinItemHeader::OnPaint(UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* lpResult)
{
    ASSERT(::IsWindow(m_hWnd));
    if (! ::IsWindow(m_hWnd))
        return FALSE;

    PAINTSTRUCT paintStruct;
    HDC hDC = ::BeginPaint(m_hWnd, &paintStruct);
    if (hDC == NULL)
        return FALSE;

    const CRect clipRect = paintStruct.rcPaint;

    HDC hMemDC = ::CreateCompatibleDC(hDC);
    HBITMAP hMemBitmap = ::CreateCompatibleBitmap(hDC, clipRect.Width(), clipRect.Height());
    HBITMAP hOldBitmap = (HBITMAP)::SelectObject(hMemDC, hMemBitmap);
    ::OffsetViewportOrgEx(hMemDC, -clipRect.left, -clipRect.top, NULL);

    LONG clrBkColor = m_clrItemIntervalBkColor;
    if (GetColorTransform())
        GetEngine()->TransformColor(clrBkColor, clrBkColor);
    CRenderEngine::FillRect(hMemDC, clipRect, clrBkColor);

    DRAWITEMSTRUCT drawStruct;
    ::memset(&drawStruct, 0, sizeof(drawStruct));
    int ctrlID = ::GetDlgCtrlID(m_hWnd);

    int itemCount = 0;
    GetItemCount(itemCount);
    for (int i=0; i<itemCount; i++) {
        CRect itemRect(0, 0, 0, 0);
        if (! GetItemRect(i, itemRect))
            continue;

        CRect intersectRect;
        intersectRect.IntersectRect(&clipRect, &itemRect);
        if (intersectRect.IsRectEmpty())
            continue;

        drawStruct.CtlType = ODT_HEADER;
        drawStruct.CtlID = ctrlID;
        drawStruct.itemID = i;
        drawStruct.itemAction = ODA_DRAWENTIRE;
        drawStruct.itemState = ODS_DEFAULT;
        GetItemState(i, drawStruct.itemState);
        drawStruct.hwndItem = m_hWnd;
        drawStruct.hDC = hMemDC;
        drawStruct.rcItem = itemRect;
        drawStruct.itemData;
        DrawItem(ctrlID, &drawStruct);
    }

    ::OffsetViewportOrgEx(hMemDC, clipRect.left, clipRect.top, NULL);
    ::BitBlt(hDC, clipRect.left, clipRect.top, clipRect.Width(), clipRect.Height(), hMemDC, 0, 0, SRCCOPY);

    ::SelectObject(hMemDC, hOldBitmap);
    ::DeleteObject(hMemBitmap);
    ::DeleteDC(hMemDC);

    ::EndPaint(m_hWnd, &paintStruct);

	return FALSE;
}

BOOL CSkinItemHeader::OnDrawItem(UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* lpResult)
{
	int nIDCtl = (int)wParam;
	LPDRAWITEMSTRUCT lpDrawItemStruct = (LPDRAWITEMSTRUCT)lParam;
	DrawItem(nIDCtl, lpDrawItemStruct);
	return TRUE;
}

LONG CSkinItemHeader::GetitemBkColorNormal(void)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());
    return m_clrItemBkColorNormal;
}

void CSkinItemHeader::SetitemBkColorNormal(LONG newVal)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());
    if (m_clrItemBkColorNormal == newVal)
        return;

    m_clrItemBkColorNormal = newVal;
    InvalidateRect();
}

LONG CSkinItemHeader::GetitemBkColorHover(void)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());
    return m_clrItemBkColorHover;
}

void CSkinItemHeader::SetitemBkColorHover(LONG newVal)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());
    if (m_clrItemBkColorHover == newVal)
        return;

    m_clrItemBkColorHover = newVal;
    InvalidateRect();
}

LONG CSkinItemHeader::GetitemBkColorDown(void)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());
    return m_clrItemBkColorDown;
}

void CSkinItemHeader::SetitemBkColorDown(LONG newVal)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());
    if (m_clrItemBkColorDown == newVal)
        return;

    m_clrItemBkColorDown = newVal;
    InvalidateRect();
}

LONG CSkinItemHeader::GetitemInterval(void)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());
    return m_nItemInterval;
}

void CSkinItemHeader::SetitemInterval(LONG newVal)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());
    if (newVal < 0)
        return;
    if (m_nItemInterval == newVal)
        return;

    m_nItemInterval = newVal;
    InvalidateRect();
}

LONG CSkinItemHeader::GetitemIntervalBkColor(void)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());
    return m_clrItemIntervalBkColor;
}

void CSkinItemHeader::SetitemIntervalBkColor(LONG newVal)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());
    if (m_clrItemIntervalBkColor == newVal)
        return;

    m_clrItemIntervalBkColor = newVal;
    InvalidateRect();
}

LONG CSkinItemHeader::GettailItemBkColor(void)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());
    return m_clrTailItemBkColor;
}

void CSkinItemHeader::SettailItemBkColor(LONG newVal)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());
    if (m_clrItemIntervalBkColor == newVal)
        return;

    m_clrTailItemBkColor = newVal;
    InvalidateRect();
}

LONG CSkinItemHeader::GetbitmapMargin()
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());
    LONG margin = 0;
    GetBitmapMargin(margin);
    return margin;
}

void CSkinItemHeader::SetbitmapMargin(LONG newVal)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());
    SetBitmapMargin(newVal);
}

LONG CSkinItemHeader::GettextMargin()
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());
    return m_nTextMargin;
}

void CSkinItemHeader::SettextMargin(LONG newVal)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());
    if (m_nTextMargin == newVal)
        return;

    m_nTextMargin = newVal;
    InvalidateRect();
}

VARIANT_BOOL CSkinItemHeader::GetshowHeaderIcon()
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());
    return (m_bShowHeaderIcon != FALSE) ? (VARIANT_TRUE) : (VARIANT_FALSE);
}

void CSkinItemHeader::SetshowHeaderIcon(VARIANT_BOOL newVal)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());
    BOOL bNewVal = (newVal != VARIANT_FALSE);
    if (m_bShowHeaderIcon == bNewVal)
        return;

    m_bShowHeaderIcon = bNewVal;
    InvalidateRect();
}
