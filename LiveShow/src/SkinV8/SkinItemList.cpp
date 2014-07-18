// SkinItemList.cpp : 实现文件
//

#include "stdafx.h"
#include "SkinEngine.h"
#include "SkinItemHeader.h"
#include "SkinItemList.h"
#include "ImageBase.h"
#include "RenderEngine.h"

#pragma warning (disable:4311)

// CSkinItemList

IMPLEMENT_DYNCREATE(CSkinItemList, CSkinItem)
CSkinItemList::CSkinItemList()
{
	EnableAutomation();
	
	// 为了使应用程序在 OLE 自动化对象处于活动状态时保持
	//	运行，构造函数调用 AfxOleLockApp。

    m_clrItemBkColorNormal = RGB(255, 255, 255);
    m_clrItemBkColorSelected = ::GetSysColor(COLOR_HIGHLIGHT);

    m_clrItemTextColorNormal = -1;
    m_clrItemTextColorSelected = -1;

    m_pCheckIcon = NULL;
    m_pUnCheckIcon = NULL;

    m_bShowHeaderIcon = FALSE;
    m_bCustomDraw = FALSE;

    m_lSubItemTextMargin = 2;
    m_bSubItemShowNoIcon = TRUE;
    m_bSubItemIconSelectedMask = FALSE;

	AfxOleLockApp();
}

CSkinItemList::~CSkinItemList()
{
	// 为了在用 OLE 自动化创建所有对象后终止应用程序，
	// 	析构函数调用 AfxOleUnlockApp。
	
	AfxOleUnlockApp();
}

void CSkinItemList::OnFinalRelease()
{
	// 释放了对自动化对象的最后一个引用后，将调用
	// OnFinalRelease。基类将自动
	// 删除该对象。在调用该基类之前，请添加您的
	// 对象所需的附加清除代码。

	CCmdTarget::OnFinalRelease();
}

BEGIN_MESSAGE_MAP(CSkinItemList, CSkinItem)
END_MESSAGE_MAP()


BEGIN_SKIN_DISPATCH_MAP(CSkinItemList, CSkinItem)
	SKINBASE_DISP_PROPERTIES(CSkinItemList)
	SKINITEM_DISP_PROPERTIES(CSkinItemList)

    SKIN_DISP_PROPERTY(CSkinItemList, itemBkColorNormal, VT_I4)
    SKIN_DISP_PROPERTY(CSkinItemList, itemBkColorSelected, VT_I4)
    SKIN_DISP_PROPERTY(CSkinItemList, itemTextColorNormal, VT_I4)
    SKIN_DISP_PROPERTY(CSkinItemList, itemTextColorSelected, VT_I4)
    SKIN_DISP_PROPERTY(CSkinItemList, subItemTextMargin, VT_I4)
    SKIN_DISP_PROPERTY(CSkinItemList, subItemShowNoIcon, VT_BOOL)
    SKIN_DISP_PROPERTY(CSkinItemList, subItemIconSelectedMask, VT_BOOL)
    SKIN_DISP_PROPERTY(CSkinItemList, checkIcon, VT_BSTR)
    SKIN_DISP_PROPERTY(CSkinItemList, uncheckIcon, VT_BSTR)
    SKIN_DISP_PROPERTY(CSkinItemList, showHeaderIcon, VT_BOOL)
    SKIN_DISP_PROPERTY(CSkinItemList, customDraw, VT_BOOL)

END_SKIN_DISPATCH_MAP(CSkinItemList)

// 注意: 我们添加 IID_ISkinItemList 支持
//以支持来自 VBA 的类型安全绑定。此 IID 必须同附加到 .IDL 文件中的
//调度接口的 GUID 匹配。

// {678EE389-A980-47E1-B076-60F652337566}
static const IID IID_ISkinItemList =
{ 0x678EE389, 0xA980, 0x47E1, { 0xB0, 0x76, 0x60, 0xF6, 0x52, 0x33, 0x75, 0x66 } };

BEGIN_INTERFACE_MAP(CSkinItemList, CSkinItem)
	INTERFACE_PART(CSkinItemList, IID_ISkinItemList, Dispatch)
END_INTERFACE_MAP()

// {915ED03C-D2F8-45EF-8296-3B59DF57BCC1}
IMPLEMENT_OLECREATE_FLAGS(CSkinItemList, "Skin.SkinItemList", afxRegApartmentThreading, 0x915ed03c, 0xd2f8, 0x45ef, 0x82, 0x96, 0x3b, 0x59, 0xdf, 0x57, 0xbc, 0xc1)


// CSkinItemList 消息处理程序
LONG CSkinItemList::GetitemBkColorNormal(void)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());
    return m_clrItemBkColorNormal;
}

void CSkinItemList::SetitemBkColorNormal(LONG newVal)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());
    if (m_clrItemBkColorNormal == newVal)
        return;

    m_clrItemBkColorNormal = newVal;
    InvalidateRect();
}

LONG CSkinItemList::GetitemBkColorSelected(void)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());
    return m_clrItemBkColorSelected;
}

void CSkinItemList::SetitemBkColorSelected(LONG newVal)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());
    if (m_clrItemBkColorSelected == newVal)
        return;

    m_clrItemBkColorSelected = newVal;
    InvalidateRect();
}

LONG CSkinItemList::GetitemTextColorNormal(void)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());
    return m_clrItemTextColorNormal;
}

void CSkinItemList::SetitemTextColorNormal(LONG newVal)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());
    if (m_clrItemTextColorNormal == newVal)
        return;

    m_clrItemTextColorNormal = newVal;
    InvalidateRect();
}

LONG CSkinItemList::GetitemTextColorSelected(void)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());
    return m_clrItemTextColorSelected;
}

void CSkinItemList::SetitemTextColorSelected(LONG newVal)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());
    if (m_clrItemTextColorSelected == newVal)
        return;

    m_clrItemTextColorSelected = newVal;
    InvalidateRect();
}

LONG CSkinItemList::GetsubItemTextMargin(void)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());
    return m_lSubItemTextMargin;
}

void CSkinItemList::SetsubItemTextMargin(LONG newVal)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());
    if (m_lSubItemTextMargin == newVal)
        return;

    m_lSubItemTextMargin = newVal;
    InvalidateRect();
}

VARIANT_BOOL CSkinItemList::GetsubItemShowNoIcon()
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());

    return (m_bSubItemShowNoIcon != FALSE) ? (VARIANT_TRUE) : (VARIANT_FALSE);
}

void CSkinItemList::SetsubItemShowNoIcon(VARIANT_BOOL newVal)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());
    BOOL bNewVal = (newVal != VARIANT_FALSE);
    if (m_bSubItemShowNoIcon == bNewVal)
        return;

    m_bSubItemShowNoIcon = bNewVal;
    InvalidateRect();
}

VARIANT_BOOL CSkinItemList::GetsubItemIconSelectedMask()
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());

    return (m_bSubItemIconSelectedMask != FALSE) ? (VARIANT_TRUE) : (VARIANT_FALSE);
}

void CSkinItemList::SetsubItemIconSelectedMask(VARIANT_BOOL newVal)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());
    BOOL bNewVal = (newVal != VARIANT_FALSE);
    if (m_bSubItemIconSelectedMask == bNewVal)
        return;

    m_bSubItemIconSelectedMask = bNewVal;
    InvalidateRect();
}

BSTR CSkinItemList::GetcheckIcon()
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());

    // TODO: 在此添加调度处理程序代码
    CString str;
    return str.AllocSysString();
}

void CSkinItemList::SetcheckIcon(LPCTSTR newVal)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());

    // TODO: 在此添加属性处理程序代码
    CString str;
    SetImageHelper(m_pCheckIcon, str, newVal, FALSE);
}

BSTR CSkinItemList::GetuncheckIcon()
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());

    // TODO: 在此添加调度处理程序代码
    CString str;
    return str.AllocSysString();
}

void CSkinItemList::SetuncheckIcon(LPCTSTR newVal)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());

    // TODO: 在此添加属性处理程序代码
    CString str;
    SetImageHelper(m_pUnCheckIcon, str, newVal, FALSE);
}

VARIANT_BOOL CSkinItemList::GetshowHeaderIcon()
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());
    CSkinItemHeader* pHeaderItem = (CSkinItemHeader*)GetSubSkin(_T("HeaderItem"));
    if (pHeaderItem != NULL)
        return pHeaderItem->GetshowHeaderIcon();

    return (m_bShowHeaderIcon != FALSE) ? (VARIANT_TRUE) : (VARIANT_FALSE);
}

void CSkinItemList::SetshowHeaderIcon(VARIANT_BOOL newVal)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());

    //设置HeaderItem的属性
    CSkinItemHeader* pHeaderItem = (CSkinItemHeader*)GetSubSkin(_T("HeaderItem"));
    if (pHeaderItem != NULL)
        pHeaderItem->SetshowHeaderIcon(newVal);

    BOOL bNewVal = (newVal != VARIANT_FALSE);
    if (m_bShowHeaderIcon == bNewVal)
        return;

    m_bShowHeaderIcon = bNewVal;
    InvalidateRect();
}

VARIANT_BOOL CSkinItemList::GetcustomDraw()
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());

    return (m_bCustomDraw != FALSE) ? (VARIANT_TRUE) : (VARIANT_FALSE);
}

void CSkinItemList::SetcustomDraw(VARIANT_BOOL newVal)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());

    BOOL bNewVal = (newVal != VARIANT_FALSE);
    if (m_bCustomDraw == bNewVal)
        return;

    m_bCustomDraw = bNewVal;
    InvalidateRect();
}

BOOL CSkinItemList::UpdateSkinItemHwnd()
{
    if (! CSkinItem::UpdateSkinItemHwnd())
        return FALSE;

    HWND hHeader = NULL;
    if (GetHeaderCtrl(hHeader) && hHeader != NULL) {
        CSkinItemHeader* pHeaderItem = (CSkinItemHeader*)GetEngine()->CreateSkinByClassName(_T("CSkinItemHeader"));
        pHeaderItem->Setname(_T("HeaderItem"));
        pHeaderItem->SetshowHeaderIcon(this->GetshowHeaderIcon());
        pHeaderItem->SetuseOriginPosition(VARIANT_TRUE);
        pHeaderItem->Attach(hHeader);
        //pHeaderItem->Sethwnd((LONG)hHeader);
        AddSubSkin(pHeaderItem);
    }

    return TRUE;
}

BOOL CSkinItemList::UpdateSkinItemProperty()
{
	if (! CSkinItem::UpdateSkinItemProperty())
		return FALSE;

    CSkinItem* pHeaderItem = (CSkinItem*)GetSubSkin(_T("HeaderItem"));
    if (pHeaderItem != NULL) {
        pHeaderItem->UpdateSkinItemProperty();
    }

	return TRUE;
}

BOOL CSkinItemList::GetHeaderCtrl(HWND& hHeader)
{
    if (! ::IsWindow(m_hWnd))
        return FALSE;

    hHeader = ListView_GetHeader(m_hWnd);
    return TRUE;
}

BOOL CSkinItemList::GetItemCount(int& count)
{
    if (! ::IsWindow(m_hWnd))
        return FALSE;

    count = 0;
    count = ListView_GetItemCount(m_hWnd);
    return FALSE;
}

BOOL CSkinItemList::GetItemRect(int iItem, RECT& rect, int iArea)
{
    if (! ::IsWindow(m_hWnd))
        return FALSE;
    
    return ListView_GetItemRect(m_hWnd, iItem, &rect, iArea);
}

BOOL CSkinItemList::GetSubItemRect(int iItem, int iSubItem, RECT& rect, int iArea)
{
    if (! ::IsWindow(m_hWnd))
        return FALSE;

    return ListView_GetSubItemRect(m_hWnd, iItem, iSubItem, iArea, &rect);
}

BOOL CSkinItemList::GetItemState(int iItem, UINT mask, UINT& state)
{
    if (! ::IsWindow(m_hWnd))
        return FALSE;

    state = ListView_GetItemState(m_hWnd, iItem, mask);
    return TRUE;
}

BOOL CSkinItemList::GetItemCheckState(int iItem, UINT& state)
{
    if (! ::IsWindow(m_hWnd))
        return FALSE;

    state = ListView_GetCheckState(m_hWnd, iItem);
    return TRUE;
}

BOOL CSkinItemList::GetItemData(int iItem, INT& date)
{
    if (! ::IsWindow(m_hWnd))
        return FALSE;

    //LVITEM lvItem;
    //memset(&lvItem, 0, sizeof(lvItem));
    //lvItem.iItem = iItem;
    //lvItem.iSubItem = iSubItem;
    //lvItem.mask = LVIF_IMAGE;
    //lvItem.stateMask = 0;
    //lvItem.iImage = -1;

    //BOOL ret = ListView_GetItem(m_hWnd, &lvItem);

    //iImage = lvItem.iImage;

    return FALSE;
}

BOOL CSkinItemList::GetSubItemText(int iItem, int iSubItem, CString& text)
{
    if (! ::IsWindow(m_hWnd))
        return FALSE;

    ListView_GetItemText(m_hWnd, iItem, iSubItem, text.GetBuffer(1024), 1023);
    text.ReleaseBuffer();
    return TRUE;
}

BOOL CSkinItemList::GetSubItemImage(int iItem, int iSubItem, int& iImage)
{
    if (! ::IsWindow(m_hWnd))
        return FALSE;

    LVITEM lvItem;
    memset(&lvItem, 0, sizeof(lvItem));
    lvItem.iItem = iItem;
    lvItem.iSubItem = iSubItem;
    lvItem.mask = LVIF_IMAGE;
    lvItem.stateMask = 0;
    lvItem.iImage = -1;

    BOOL ret = ListView_GetItem(m_hWnd, &lvItem);

    iImage = lvItem.iImage;

    return ret;
}

BOOL CSkinItemList::GetNormalImageList(HIMAGELIST& hImageList)
{
    hImageList = NULL;

    if (! ::IsWindow(m_hWnd))
        return FALSE;

    hImageList = ListView_GetImageList(m_hWnd, LVSIL_NORMAL);
    return TRUE;
}

BOOL CSkinItemList::GetSmallImageList(HIMAGELIST& hImageList)
{
    hImageList = NULL;

    if (! ::IsWindow(m_hWnd))
        return FALSE;

    hImageList = ListView_GetImageList(m_hWnd, LVSIL_SMALL);
    return TRUE;
}

BOOL CSkinItemList::GetStateImageList(HIMAGELIST& hImageList)
{
    hImageList = NULL;

    if (! ::IsWindow(m_hWnd))
        return FALSE;

    hImageList = ListView_GetImageList(m_hWnd, LVSIL_STATE);
    return TRUE;
}

BOOL CSkinItemList::HitHest(POINT pt, int& iItem, int& iSubItem)
{
    iItem = -1;
    iSubItem = -1;

    if (! ::IsWindow(m_hWnd))
        return FALSE;

    LVHITTESTINFO hitTestInfo;
    memset(&hitTestInfo, 0, sizeof(hitTestInfo));
    hitTestInfo.pt = pt;
    int ret = ListView_HitTest(m_hWnd, &hitTestInfo);
    iItem = hitTestInfo.iItem;
    iSubItem = hitTestInfo.iSubItem;

    return ret != -1;
}

BOOL CSkinItemList::RedrawItems(int start, int end)
{
    if (! ::IsWindow(m_hWnd))
        return FALSE;

    return ListView_RedrawItems(m_hWnd, start, end);
}

BOOL CSkinItemList::GetExtendedStyle(UINT mask, UINT& style)
{
    style = 0;

    if (! ::IsWindow(m_hWnd))
        return FALSE;

    style = mask & ListView_GetExtendedListViewStyle(m_hWnd);
    return TRUE;
}

BOOL CSkinItemList::SetExtendedStyle(UINT mask, UINT style)
{
    if (! ::IsWindow(m_hWnd))
        return FALSE;

    ListView_SetExtendedListViewStyleEx(m_hWnd, mask, style);
    return TRUE;
}

BOOL CSkinItemList::IsGridLines()
{
    UINT style = 0;
    if (GetExtendedStyle(LVS_EX_GRIDLINES, style)
        && style)
        return TRUE;
    return FALSE;
}

BOOL CSkinItemList::IsFullRowSelected()
{
    UINT style = 0;
    if (GetExtendedStyle(LVS_EX_FULLROWSELECT, style)
        && style)
        return TRUE;
    return FALSE;
}

BOOL CSkinItemList::IsCheck()
{
    UINT style = 0;
    if (GetExtendedStyle(LVS_EX_CHECKBOXES, style)
        && style)
        return TRUE;
    return FALSE;
}

void CSkinItemList::DrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct)
{
    // TODO:  添加您的代码以绘制指定项
    int iItem = lpDrawItemStruct->itemID;
    HDC hDC = lpDrawItemStruct->hDC;
    CRect itemRect = lpDrawItemStruct->rcItem;
    BOOL bSelected = (lpDrawItemStruct->itemState & LVIS_SELECTED) != 0;
    UINT uCheckState = FALSE;
    GetItemCheckState(iItem, uCheckState);
    BOOL bFocused = (lpDrawItemStruct->itemState & LVIS_FOCUSED) != 0;
    BOOL bFullRowSelected = IsFullRowSelected();
    HIMAGELIST hImageList = NULL;
    GetSmallImageList(hImageList);

    BOOL bCheck = IsCheck();

    //绘制item背景
    CRenderEngine::FillRect(hDC, itemRect, m_clrItemBkColorNormal);
    CRect selectRect = itemRect;
    if (bSelected) {
        GetItemRect(iItem, selectRect, LVIR_SELECTBOUNDS);
        CRenderEngine::FillRect(hDC, selectRect, m_clrItemBkColorSelected);
    }
    //if (bSelected && bFullRowSelected)
    //    CRenderEngine::FillRect(hDC, itemRect, m_clrItemBkColorSelected);
    //else 
    //    CRenderEngine::FillRect(hDC, itemRect, m_clrItemBkColorNormal);

    //绘制SubItem
    LVCOLUMN lvc;
    memset(&lvc, 0, sizeof(lvc));
    lvc.mask = LVCF_WIDTH | LVCF_IMAGE;
    for (int i=0; ListView_GetColumn(m_hWnd, i, &lvc); i++) {
        CRect subItemRect(0, 0, 0, 0);
        if (! GetSubItemRect(iItem, i, subItemRect))
            continue;

        //计算图标绘制区域
        //int subItemImage = -1;
        //GetSubItemImage(iItem, i, subItemImage);
        //CRect subItemImageRect = subItemRect;
        //subItemImageRect.right = subItemImageRect.left;
        //if (hImageList != NULL && subItemImage >= 0) {
        //    IMAGEINFO info;
        //    ::ImageList_GetImageInfo(hImageList, subItemImage, &info);
        //    subItemImageRect.right += info.rcImage.right - info.rcImage.left;
        //}
        int subItemImage = -1;
        GetSubItemImage(iItem, i, subItemImage);
        CRect subItemImageRect = subItemRect;
        GetSubItemRect(iItem, i, subItemImageRect, LVIR_ICON);
        if (! m_bSubItemShowNoIcon && (subItemImage < 0 || hImageList == NULL) )
            subItemImageRect.right = subItemImageRect.left;

        //计算文本绘制区域
        CString subItemText;
        GetSubItemText(iItem, i, subItemText);
        CRect subItemTextRect = subItemRect;
        subItemTextRect.left = subItemImageRect.right + m_lSubItemTextMargin;
        CSize subItemTextSize(0, 0);
        m_textStyleNormal.GetTextSize(hDC, subItemText, subItemTextSize);
        subItemTextRect.right = subItemTextRect.left + subItemTextSize.cx;
        if (subItemTextRect.right > subItemRect.right)
            subItemTextRect.right = subItemRect.right;

        //CString subItemText;
        //GetSubItemText(iItem, i, subItemText);
        //CRect subSelectRect = subItemRect;
        //CRect subItemTextRect = subItemRect;
        //GetSubItemRect(iItem, i, subSelectRect, LVIR_LABEL);
        //subItemTextRect.SubtractRect(&subSelectRect, &subItemImageRect);

        //计算subItem的背景区域
        CRect itemBackRect = subItemTextRect + subItemImageRect;

        //绘制背景
        if (bSelected && i == 0 && !bFullRowSelected)
            CRenderEngine::FillRect(hDC, itemBackRect, m_clrItemBkColorSelected);

        //绘制check状态和图标
        if (bCheck && i == 0) {
            CRect checkRect(0, 0, 12, 12);
            checkRect.OffsetRect(subItemRect.left, subItemRect.top);
            checkRect.OffsetRect(6, (subItemRect.Height() - checkRect.Height()) / 2);
            CImageBase* pImage = uCheckState ? m_pCheckIcon: m_pUnCheckIcon;
            if (pImage)
                pImage->Draw(hDC, checkRect);
        }
        if (hImageList != NULL && subItemImage >= 0) {
            BOOL bShowMask = m_bSubItemIconSelectedMask && bSelected;
            ::ImageList_Draw(hImageList, subItemImage, hDC, subItemImageRect.left, subItemImageRect.top, 
                (bShowMask ? ILD_SELECTED : ILD_NORMAL));
        }

        //绘制文本
        if (m_bCustomDraw) {
            NMLVCUSTOMDRAW NMLVCustomDraw = {0};
            NMLVCustomDraw.nmcd.hdr.hwndFrom = m_hWnd;
            NMLVCustomDraw.nmcd.hdr.idFrom = GetDlgCtrlID(m_hWnd);
            NMLVCustomDraw.nmcd.hdr.code = NM_CUSTOMDRAW;
            NMLVCustomDraw.nmcd.hdc = lpDrawItemStruct->hDC;
            NMLVCustomDraw.nmcd.dwItemSpec = iItem;
            NMLVCustomDraw.nmcd.dwDrawStage = CDDS_ITEMPREPAINT | CDDS_SUBITEM;
            NMLVCustomDraw.nmcd.uItemState = lpDrawItemStruct->itemState;
            NMLVCustomDraw.nmcd.rc = lpDrawItemStruct->rcItem;
            NMLVCustomDraw.iSubItem = i;
            LRESULT ret = ::SendMessage(::GetParent(m_hWnd), WM_NOTIFY, GetDlgCtrlID(m_hWnd), (LPARAM)(&NMLVCustomDraw));
            m_textStyleNormal.DrawTextEx(hDC, subItemText, &subItemTextRect, NMLVCustomDraw.clrText);
        } else {
            if (bSelected) {
                if (m_clrItemTextColorSelected != -1)
                    m_textStyleNormal.DrawTextEx(hDC, subItemText, &subItemTextRect, m_clrItemTextColorSelected);
                else 
                    m_textStyleNormal.DrawText(hDC, subItemText, &subItemTextRect);
            } else {
                if (m_clrItemTextColorNormal != -1)
                    m_textStyleNormal.DrawTextEx(hDC, subItemText, &subItemTextRect, m_clrItemTextColorNormal);
                else 
                    m_textStyleNormal.DrawText(hDC, subItemText, &subItemTextRect);
            }
        }

        //绘制Item的FocusRect
        if (bFocused && i == 0 && !bFullRowSelected)
            CRenderEngine::RenderFocusRect(hDC, subItemTextRect);

        memset(&lvc, 0, sizeof(lvc));
        lvc.mask = LVCF_WIDTH | LVCF_IMAGE;
    }

    if (bFocused && bFullRowSelected)
        CRenderEngine::RenderFocusRect(hDC, selectRect);
}

void CSkinItemList::DrawBackground(CDC* pDC, LPRECT lpInvalidRect, CRgn* pInvalidRgn, BOOL bChildSkin)
{
    CRect backRect = GetSkinAbsRect();
    if (lpInvalidRect != NULL)
        backRect = *lpInvalidRect;
    if (m_clrBkColorNormal != -1)
        CRenderEngine::FillRect(*pDC, backRect, m_clrBkColorNormal);
}

void CSkinItemList::DrawForeground(CDC* pDC, LPRECT lpInvalidRect, CRgn* pInvalidRgn)
{
    CRect clipRect = GetSkinAbsRect();
    if (lpInvalidRect != NULL)
        clipRect = *lpInvalidRect;

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

        drawStruct.CtlType = ODT_LISTVIEW;
        drawStruct.CtlID = ctrlID;
        drawStruct.itemID = i;
        drawStruct.itemAction = ODA_DRAWENTIRE;
        drawStruct.itemState = LVIS_SELECTED;
        GetItemState(i, LVIS_SELECTED | LVIS_FOCUSED, drawStruct.itemState);
        drawStruct.hwndItem = m_hWnd;
        drawStruct.hDC = *pDC;
        drawStruct.rcItem = itemRect;
        drawStruct.itemData;
        DrawItem(ctrlID, &drawStruct);
    }
}

void CSkinItemList::DrawFrame(CDC* pDC, LPRECT lpInvalidRect, CRgn* pInvalidRgn)
{
    //绘制边框
    if (m_clrBorderLineColor != -1) {
        CRect wndRect = GetSkinAbsRect();
        CRenderEngine::RenderRect(*pDC, wndRect, m_clrBorderLineColor, m_nBorderLineWidth);
    }
}

void CSkinItemList::Draw(CDC* pDC, LPRECT lpInvalidRect, CRgn* pInvalidRgn)
{
    DrawBackground(pDC, lpInvalidRect, pInvalidRgn);
    DrawForeground(pDC, lpInvalidRect, pInvalidRgn);
}

BOOL CSkinItemList::OnNcPaint(UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* lpResult)
{
    *lpResult = DefWndProc(uMsg, wParam, lParam);

    HDC hDC = ::GetWindowDC(m_hWnd);

    CDC* pDC = CDC::FromHandle(hDC);

    DrawFrame(pDC);

    ::ReleaseDC(m_hWnd, hDC);

    return TRUE;
}

BOOL CSkinItemList::OnPaint(UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* lpResult)
{
    ASSERT(::IsWindow(m_hWnd));
    if (! ::IsWindow(m_hWnd))
        return FALSE;

    PAINTSTRUCT paintStruct;
    HDC hDC = ::BeginPaint(m_hWnd, &paintStruct);

    CDC* pDC = CDC::FromHandle(hDC);

    CRect invalidRect = paintStruct.rcPaint;
    Draw(pDC, &invalidRect);

    ::EndPaint(m_hWnd, &paintStruct);

    return TRUE;
}

BOOL CSkinItemList::OnLButtonDown(UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* lpResult)
{
    //绘制代码导致第一项显示不正确，原因未明，特殊处理一下
    LONG x = GETXPARAM(lParam);
    LONG y = GETYPARAM(lParam);
    UINT flag = (UINT)wParam;
    int iItem = -1;
    int iSubItem = -1;
    if (HitHest(CPoint(x, y), iItem, iSubItem) && iItem == 0) {
        RedrawItems(0, 0);
    }

    return CSkinItem::OnLButtonDown(uMsg, wParam, lParam, lpResult);
}

BOOL CSkinItemList::OnDrawItem(UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* lpResult)
{
	int nIDCtl = (int)wParam;
	LPDRAWITEMSTRUCT lpDrawItemStruct = (LPDRAWITEMSTRUCT)lParam;
	DrawItem(nIDCtl, lpDrawItemStruct);
	return FALSE;
}

BOOL CSkinItemList::OnLVMSetItemState(UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* lpResult)
{
    int iItem = (int)wParam;
    LPLVITEM plv = (LPLVITEM)lParam;
    if (iItem = 0 || iItem == -1) {
        RedrawItems(0, 0);
    }

    return FALSE;
}
