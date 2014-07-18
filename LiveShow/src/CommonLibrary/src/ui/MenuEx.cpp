// MenuEx.cpp : 实现文件
//

#include "../../StdAfx.h"
#include "../../include/ui/MenuEx.h"
#include "../../include/ui/UIHelper.h"
#include "../../../../include/Skin_i.h"
#include "utility/SystemHelper.h"
#include <vector>

namespace common { namespace ui
{

typedef std::vector<MENUITEMINFOEX*> MenuItemInfoVector;
typedef std::vector<CMenuEx*> MenuExVector;

#define CLASS_DATA_MEMBER_LIST(V) \
    V(MenuItemInfoVector, vecItemInfo) \
    V(MenuExVector, vecSubMenu) \
    V(CImageList*, pImageList) \
    V(CImageList*, pImageListSelected) \
    V(CImageList*, pImageListDisabled) \
    V(std::vector<IImage*>*, pImageVector) \
    V(std::vector<IImage*>*, pImageVectorSelected) \
    V(std::vector<IImage*>*, pImageVectorDisabled) \
    V(LONG, nWidth) \
    V(LONG, nHeight) \
    V(LONG, nSeparatorHeight) \
    V(LONG, nLeftSideWidth) \
    V(CFont, Font) \
    V(COLORREF, clrTextColor) \
    V(COLORREF, clrTextColorSelected) \
    V(COLORREF, clrTextColorDisabled) \
    V(COLORREF, clrBkColor) \
    V(COLORREF, clrBkColorSelected) \
    V(COLORREF, clrBorderColorSelected) \
    V(COLORREF, clrLeftSideColorLeft) \
    V(COLORREF, clrLeftSideColorRight) \
    V(COLORREF, clrSeparatorColor) \
    V(COLORREF, clrSeparatorColorSide) \
    V(COLORREF, clrTransparentColor) \
    V(BOOL, bNoPrefix) \


DECLARE_CLASS_DATA_STRUCT(CMenuEx, CLASS_DATA_MEMBER_LIST)

IMPLEMENT_DYNAMIC(CMenuEx, CMenu)

CMenuEx::CMenuEx(void)
{
    NEW_CLASS_DATA(CMenuEx);

    USE_CLASS_DATA(CLASS_DATA_MEMBER_LIST);

    m_pImageList = NULL;
    m_pImageListSelected = NULL;
    m_pImageListDisabled = NULL;

    m_pImageVector = NULL;
    m_pImageVectorSelected = NULL;
    m_pImageVectorDisabled = NULL;

    m_nWidth = -1;
    m_nHeight = -1;
    m_nSeparatorHeight = -1;
    m_nLeftSideWidth = 21;

	m_Font.CreateFont(12, 0, 0, 0, 400, 0, 0, 0, DEFAULT_CHARSET, 0, 0, 0, 0, _T("宋体"));

    m_clrTextColor = RGB(0, 0, 0);
    m_clrTextColorSelected = RGB(0, 0, 0);
    m_clrTextColorDisabled = RGB(192, 192, 192);

    m_clrBkColor = RGB(255, 255, 255);
    m_clrBkColorSelected = RGB(207, 231, 255);

    m_clrBorderColorSelected = RGB(134, 165, 196);

    m_clrLeftSideColorLeft = RGB(195, 221, 236);
    m_clrLeftSideColorRight = RGB(227, 238, 245);

    m_clrSeparatorColor = RGB(155, 179, 210);
    m_clrSeparatorColorSide = RGB(255, 255, 254);

    m_clrTransparentColor = (COLORREF)NO_TRANSPARENT;

    m_bNoPrefix = FALSE;
}

CMenuEx::~CMenuEx(void)
{
    USE_CLASS_DATA(CLASS_DATA_MEMBER_LIST);

    DestroyMenu();

    DELETE_CLASS_DATA();
}

CMenuEx* CMenuEx::NewPopupMenu(HMENU hOldPopup)
{
    HMENU hNewPopup = ::CreatePopupMenu();
    CMenuEx* pNewPopup = new CMenuEx();
    if (! pNewPopup->Attach(hNewPopup)) {
        delete pNewPopup;
        pNewPopup = NULL;
        ::DestroyMenu(hNewPopup);
        return NULL;
    }

    CMenu* pOldPopup = CMenu::FromHandle(hOldPopup);
    for (int i=0; i<::GetMenuItemCount(hOldPopup); i++) {

        //获取信息
        MENUITEMINFOEX* pInfo = new MENUITEMINFOEX;
        pInfo->uMask = 0;
        pInfo->iImage = -1;
        pInfo->uID = pOldPopup->GetMenuItemID(i);
        CString strItem;
        pOldPopup->GetMenuString(i, strItem, MF_BYPOSITION);
        pInfo->wstrText = (LPCTSTR)strItem;

        if (pInfo->uID == 0) {
            ::AppendMenu(hNewPopup, MF_OWNERDRAW | MF_SEPARATOR, 0, (LPCTSTR)pInfo);
        } else if (pInfo->uID == -1) {
            HMENU hOldSubPopup = ::GetSubMenu(hOldPopup, i);
            CMenuEx* pNewSubPopup = NULL;
            if (hOldSubPopup != NULL) {
                pNewSubPopup = NewPopupMenu(hOldSubPopup);
                ASSERT(pNewSubPopup != NULL);
                if (pNewSubPopup != NULL) {
                    pNewPopup->m_pData->vecSubMenu.push_back(pNewSubPopup);
                }
            }
#pragma warning(push)
#pragma warning (disable:4311)
            HMENU hNewSubPopup = (pNewSubPopup != NULL) ? (pNewSubPopup->GetSafeHmenu()) : (NULL);
            ::AppendMenu(hNewPopup, MF_OWNERDRAW | MF_POPUP | MF_BYPOSITION, (UINT)(hNewSubPopup), (LPCTSTR)pInfo);
#pragma warning(pop)
        } else {
            ::AppendMenu(hNewPopup, MF_OWNERDRAW, pInfo->uID, (LPCTSTR)pInfo);
        }

        pNewPopup->m_pData->vecItemInfo.push_back(pInfo);
    }

    return pNewPopup;
}

static COLOR_TRANSFORM_PROC s_pfnColorTransformProc = NULL;

COLORREF TransformColor(COLORREF clr)
{
    if (s_pfnColorTransformProc == NULL)
        return clr;

    return (*s_pfnColorTransformProc)(clr);
}

void CMenuEx::SetColorTransformProc(COLOR_TRANSFORM_PROC pfnColorTransform)
{
    s_pfnColorTransformProc = pfnColorTransform;
}

BOOL CMenuEx::LoadMenu(HMENU hMenu)
{
    USE_CLASS_DATA(CLASS_DATA_MEMBER_LIST);

    HMENU hNewMenu = ::CreateMenu();
    if (! Attach(hNewMenu)) {
        ::DestroyMenu(hNewMenu);
        return FALSE;
    }

    CMenu* pMenu = CMenu::FromHandle(hMenu);
    for (int i=0; i<::GetMenuItemCount(hMenu); i++) {

        //获取信息
        MENUITEMINFOEX* pInfo = new MENUITEMINFOEX;
        pInfo->uMask = 0;
        pInfo->iImage = -1;
        pInfo->uID = pMenu->GetMenuItemID(i);
        CString strItem;
        pMenu->GetMenuString(i, strItem, MF_BYPOSITION);
        pInfo->wstrText = (LPCTSTR)strItem;

        if (pInfo->uID == 0) {
            ::AppendMenu(hNewMenu, MF_OWNERDRAW | MF_SEPARATOR, 0, (LPCTSTR)pInfo);
        } else if (pInfo->uID == -1) {
            HMENU hOldPopup = ::GetSubMenu(hMenu, i);
            CMenuEx* pNewPopup = NULL;
            if (hOldPopup != NULL) {
                pNewPopup = NewPopupMenu(hOldPopup);
                ASSERT(pNewPopup != NULL);
                if (pNewPopup != NULL) {
                    m_vecSubMenu.push_back(pNewPopup);
                }
            }
#pragma warning(push)
#pragma warning (disable:4311)
            HMENU hNewPopup = (pNewPopup != NULL) ? (pNewPopup->GetSafeHmenu()) : (NULL);
            ::AppendMenu(hNewMenu, MF_OWNERDRAW | MF_POPUP | MF_BYPOSITION, (UINT)(hNewPopup), (LPCTSTR)pInfo);
#pragma warning(pop)
        } else {
            ::AppendMenu(hNewMenu, MF_OWNERDRAW, pInfo->uID, (LPCTSTR)pInfo);
        }

        m_vecItemInfo.push_back(pInfo);
    }

    return TRUE;
}

BOOL CMenuEx::LoadMenu(UINT nIDResource)
{
    USE_CLASS_DATA(CLASS_DATA_MEMBER_LIST);

    CMenu menu;
    if (! menu.LoadMenu(nIDResource))
        return FALSE;

    return LoadMenu((HMENU)menu);
}

BOOL CMenuEx::LoadMenu(LPCTSTR lpszResourceName)
{
    USE_CLASS_DATA(CLASS_DATA_MEMBER_LIST);

    CMenu menu;
    if (! menu.LoadMenu(lpszResourceName))
        return FALSE;

    return LoadMenu((HMENU)menu);
}

BOOL CMenuEx::LoadMenu(const CMenu& menu)
{
    USE_CLASS_DATA(CLASS_DATA_MEMBER_LIST);

    return LoadMenu((HMENU)menu);
}

BOOL CMenuEx::DestroyMenu()
{
    USE_CLASS_DATA(CLASS_DATA_MEMBER_LIST);

    std::vector<CMenuEx*>::iterator iter = m_vecSubMenu.begin(),
        last = m_vecSubMenu.end();
    for (; iter!=last; iter++) {
        delete *iter;
    }
    m_vecSubMenu.clear();

    std::vector<MENUITEMINFOEX*>::iterator iter2 = m_vecItemInfo.begin(),
        last2 = m_vecItemInfo.end();
    for (; iter2!=last2; iter2++) {
        delete *iter2;
    }
    m_vecItemInfo.clear();

    return __super::DestroyMenu();
}

CImageList* CMenuEx::GetImageList()
{
    USE_CLASS_DATA(CLASS_DATA_MEMBER_LIST);

    return m_pImageList;
}

void CMenuEx::SetImageList(CImageList* pImageList)
{
    USE_CLASS_DATA(CLASS_DATA_MEMBER_LIST);

    m_pImageList = pImageList;
};

CImageList* CMenuEx::GetImageListSelected()
{
    USE_CLASS_DATA(CLASS_DATA_MEMBER_LIST);

    return m_pImageListSelected;
}

void CMenuEx::SetImageListSelected(CImageList* pImageList)
{
    USE_CLASS_DATA(CLASS_DATA_MEMBER_LIST);

    m_pImageListSelected = pImageList;
};

CImageList* CMenuEx::GetImageListDisabled()
{
    USE_CLASS_DATA(CLASS_DATA_MEMBER_LIST);

    return m_pImageListDisabled;
}

void CMenuEx::SetImageListDisabled(CImageList* pImageList)
{
    USE_CLASS_DATA(CLASS_DATA_MEMBER_LIST);

    m_pImageListDisabled = pImageList;
};

std::vector<IImage*>* CMenuEx::GetImageVector()
{
    USE_CLASS_DATA(CLASS_DATA_MEMBER_LIST);

    return m_pImageVector;
}

void CMenuEx::SetImageVector(std::vector<IImage*>* pImageList)
{
    USE_CLASS_DATA(CLASS_DATA_MEMBER_LIST);

    m_pImageVector = pImageList;
};

std::vector<IImage*>* CMenuEx::GetImageVectorSelected()
{
    USE_CLASS_DATA(CLASS_DATA_MEMBER_LIST);

    return m_pImageVectorSelected;
}

void CMenuEx::SetImageVectorSelected(std::vector<IImage*>* pImageList)
{
    USE_CLASS_DATA(CLASS_DATA_MEMBER_LIST);

    m_pImageVectorSelected = pImageList;
};

std::vector<IImage*>* CMenuEx::GetImageVectorDisabled()
{
    USE_CLASS_DATA(CLASS_DATA_MEMBER_LIST);

    return m_pImageVectorDisabled;
}

void CMenuEx::SetImageVectorDisabled(std::vector<IImage*>* pImageList)
{
    USE_CLASS_DATA(CLASS_DATA_MEMBER_LIST);

    m_pImageVectorDisabled = pImageList;
};

CMenuEx* CMenuEx::GetSubMenu(int nPosition)
{
    USE_CLASS_DATA(CLASS_DATA_MEMBER_LIST);

    CMenu* pSubMenu = CMenu::GetSubMenu(nPosition);
    if (pSubMenu == NULL)
        return NULL;

    ASSERT(pSubMenu->IsKindOf(RUNTIME_CLASS(CMenuEx)));
    if (! pSubMenu->IsKindOf(RUNTIME_CLASS(CMenuEx)))
        return NULL;

    return (CMenuEx*)pSubMenu;
};

LONG CMenuEx::GetWidth()
{
    USE_CLASS_DATA(CLASS_DATA_MEMBER_LIST);

    return m_nWidth;
}

void CMenuEx::SetWidth(LONG nWidth)
{
    USE_CLASS_DATA(CLASS_DATA_MEMBER_LIST);

    m_nWidth = nWidth;
}

LONG CMenuEx::GetHeight()
{
    USE_CLASS_DATA(CLASS_DATA_MEMBER_LIST);

    return m_nHeight;
}

void CMenuEx::SetHeight(LONG nHeight)
{
    USE_CLASS_DATA(CLASS_DATA_MEMBER_LIST);

    m_nHeight = nHeight;
}

LONG CMenuEx::GetSeparatorHeight()
{
    USE_CLASS_DATA(CLASS_DATA_MEMBER_LIST);

    return m_nSeparatorHeight;
}

void CMenuEx::SetSeparatorHeight(LONG nHeight)
{
    USE_CLASS_DATA(CLASS_DATA_MEMBER_LIST);

    m_nSeparatorHeight = nHeight;
}

LONG CMenuEx::GetLeftSideWidth()
{
    USE_CLASS_DATA(CLASS_DATA_MEMBER_LIST);

    return m_nLeftSideWidth;
}

void CMenuEx::SetLeftSideWidth(LONG nWidth)
{
    USE_CLASS_DATA(CLASS_DATA_MEMBER_LIST);

    m_nLeftSideWidth = nWidth;
}

BOOL CMenuEx::AppendMenuEx(UINT nFlags, UINT nIDNewItem, LPCTSTR lpszNewItem, int iIndex)
{
    USE_CLASS_DATA(CLASS_DATA_MEMBER_LIST);

    MENUITEMINFOEX *pMenuItem = new MENUITEMINFOEX;
    if (lpszNewItem != NULL)
        pMenuItem->wstrText = lpszNewItem;
    pMenuItem->uID = nIDNewItem;
    pMenuItem->iImage = iIndex;

    nFlags &= ~MF_STRING;
    nFlags |= MF_OWNERDRAW;

    m_vecItemInfo.push_back(pMenuItem);

    return CMenu::AppendMenu(nFlags, nIDNewItem, (LPCTSTR)pMenuItem);
}

BOOL CMenuEx::ModifyMenuEx(UINT nPosition, UINT nFlags, UINT nIDNewItem, LPCTSTR lpszNewItem, int iImage)
{
    USE_CLASS_DATA(CLASS_DATA_MEMBER_LIST);

    UINT uItemID = (nFlags & MF_BYPOSITION) ? (GetMenuItemID((int)nPosition)) : (nPosition);

    MENUITEMINFOEX* pInfo = NULL;
    bool bFinded = false;
    for (size_t i=0; i<m_vecItemInfo.size(); i++) {
        pInfo = m_vecItemInfo[i];
        if (pInfo->uID == uItemID) {
            if (lpszNewItem != NULL)
                pInfo->wstrText = lpszNewItem;
            if (nIDNewItem != 0)
                pInfo->uID = nIDNewItem;
            if (iImage > NO_MODIFY_IMAGE)
                pInfo->iImage = iImage;
            bFinded = true;
            break;
        }
    }

    if (! bFinded)
        return FALSE;

    ASSERT(pInfo != NULL);
    nFlags &= ~MF_STRING;
    nFlags |= MF_OWNERDRAW;

    return CMenu::ModifyMenu(nPosition, nFlags, pInfo->uID, (LPCTSTR)pInfo);
}

BOOL CMenuEx::RemoveMenuEx(UINT nPosition, UINT nFlags)
{
    USE_CLASS_DATA(CLASS_DATA_MEMBER_LIST);

    UINT uItemID = (nFlags & MF_BYPOSITION) ? (GetMenuItemID((int)nPosition)) : (nPosition);

    std::vector<MENUITEMINFOEX*>::iterator iter = m_vecItemInfo.begin(),
        last = m_vecItemInfo.end();
    for (; iter!=last; iter++) {
        MENUITEMINFOEX* pInfo = *iter;
        if (pInfo->uID == uItemID) {
            delete pInfo;
            m_vecItemInfo.erase(iter);
            break;
        }
    }

    return CMenu::RemoveMenu(nPosition, nFlags);
}

COLORREF CMenuEx::GetTextColor()
{
    USE_CLASS_DATA(CLASS_DATA_MEMBER_LIST);

    return m_clrTextColor;
}

void CMenuEx::SetTextColor(COLORREF clrColor)
{
    USE_CLASS_DATA(CLASS_DATA_MEMBER_LIST);

    m_clrTextColor = clrColor;
}

COLORREF CMenuEx::GetTextColorSelected()
{
    USE_CLASS_DATA(CLASS_DATA_MEMBER_LIST);

    return m_clrTextColorSelected;
}

void CMenuEx::SetTextColorSelected(COLORREF clrColor)
{
    USE_CLASS_DATA(CLASS_DATA_MEMBER_LIST);

    m_clrTextColorSelected = clrColor;
}

COLORREF CMenuEx::GetTextColorDisabled()
{
    USE_CLASS_DATA(CLASS_DATA_MEMBER_LIST);

    return m_clrTextColorDisabled;
}

void CMenuEx::SetTextColorDisabled(COLORREF clrColor)
{
    USE_CLASS_DATA(CLASS_DATA_MEMBER_LIST);

    m_clrTextColorDisabled = clrColor;
}

COLORREF CMenuEx::GetBkColor()
{
    USE_CLASS_DATA(CLASS_DATA_MEMBER_LIST);

    return m_clrBkColor;
}

void CMenuEx::SetBkColor(COLORREF clrColor)
{
    USE_CLASS_DATA(CLASS_DATA_MEMBER_LIST);

    m_clrBkColor = clrColor;
}

COLORREF CMenuEx::GetBkColorSelected()
{
    USE_CLASS_DATA(CLASS_DATA_MEMBER_LIST);

    return m_clrBkColorSelected;
}

void CMenuEx::SetBkColorSelected(COLORREF clrColor)
{
    USE_CLASS_DATA(CLASS_DATA_MEMBER_LIST);

    m_clrBkColorSelected = clrColor;
}

COLORREF CMenuEx::GetBorderColorSelected()
{
    USE_CLASS_DATA(CLASS_DATA_MEMBER_LIST);

    return m_clrBorderColorSelected;
}

void CMenuEx::SetBorderColorSelected(COLORREF clrColor)
{
    USE_CLASS_DATA(CLASS_DATA_MEMBER_LIST);

    m_clrBorderColorSelected = clrColor;
}

COLORREF CMenuEx::GetLeftSideColorLeft()
{
    USE_CLASS_DATA(CLASS_DATA_MEMBER_LIST);

    return m_clrLeftSideColorLeft;
}

void CMenuEx::SetLeftSideColorLeft(COLORREF clrColor)
{
    USE_CLASS_DATA(CLASS_DATA_MEMBER_LIST);

    m_clrLeftSideColorLeft = clrColor;
}

COLORREF CMenuEx::GetLeftSideColorRight()
{
    USE_CLASS_DATA(CLASS_DATA_MEMBER_LIST);

    return m_clrLeftSideColorRight;
}

void CMenuEx::SetLeftSideColorRight(COLORREF clrColor)
{
    USE_CLASS_DATA(CLASS_DATA_MEMBER_LIST);

    m_clrLeftSideColorRight = clrColor;
}

COLORREF CMenuEx::GetSeparatorColor()
{
    USE_CLASS_DATA(CLASS_DATA_MEMBER_LIST);

    return m_clrSeparatorColor;
}

void CMenuEx::SetSeparatorColor(COLORREF clrColor)
{
    USE_CLASS_DATA(CLASS_DATA_MEMBER_LIST);

    m_clrSeparatorColor = clrColor;
}

COLORREF CMenuEx::GetSeparatorColorSide()
{
    USE_CLASS_DATA(CLASS_DATA_MEMBER_LIST);

    return m_clrSeparatorColorSide;
}

void CMenuEx::SetSeparatorColorSide(COLORREF clrColor)
{
    USE_CLASS_DATA(CLASS_DATA_MEMBER_LIST);

    m_clrSeparatorColorSide = clrColor;
}

BOOL CMenuEx::GetNoPrefix()
{
    USE_CLASS_DATA(CLASS_DATA_MEMBER_LIST);

    return m_bNoPrefix;
}

void CMenuEx::SetNoPrefix(BOOL bTrue)
{
    USE_CLASS_DATA(CLASS_DATA_MEMBER_LIST);

    m_bNoPrefix = bTrue;
}

COLORREF CMenuEx::GetTransparentColor()
{
    USE_CLASS_DATA(CLASS_DATA_MEMBER_LIST);

    return m_clrTransparentColor;
}

void CMenuEx::SetTransparentColor(COLORREF clrColor)
{
    USE_CLASS_DATA(CLASS_DATA_MEMBER_LIST);

    m_clrTransparentColor = clrColor;
}

void CMenuEx::MeasureItem(LPMEASUREITEMSTRUCT lpMIS)
{
    USE_CLASS_DATA(CLASS_DATA_MEMBER_LIST);

    LPMENUITEMINFOEX pItemInfo = (LPMENUITEMINFOEX)lpMIS->itemData;

    //分隔条
    if (lpMIS->itemID == 0) {
        lpMIS->itemWidth = 0;
        lpMIS->itemHeight = 5;
        if (m_nSeparatorHeight > -1)
            lpMIS->itemHeight = m_nSeparatorHeight;
        return;
    }

    //计算文字大小
    HDC hDC = ::GetDC(NULL);
    SIZE szText = GetTextExtent(hDC, pItemInfo->wstrText.c_str(), m_Font);
    ::ReleaseDC(NULL, hDC);

    lpMIS->itemWidth = szText.cx + m_nLeftSideWidth + 20;
    lpMIS->itemHeight = szText.cy + 7;

    if (m_nWidth > -1) 
        lpMIS->itemWidth = m_nWidth;

    if (m_nHeight > -1)
        lpMIS->itemHeight = m_nHeight;
}

void CMenuEx::DrawItem(LPDRAWITEMSTRUCT lpDIS)
{
    USE_CLASS_DATA(CLASS_DATA_MEMBER_LIST);

    CDC	dc;
    dc.Attach(lpDIS->hDC);
    CRect rect(lpDIS->rcItem);
    LPMENUITEMINFOEX lpItemInfo = (LPMENUITEMINFOEX)lpDIS->itemData;

    //选择字体和文字颜色，背景透明
    CFont* pFontOld = dc.SelectObject(&m_Font);
    COLORREF clrTextColor = (lpDIS->itemState & ODS_SELECTED) ? (GetTextColorSelected()) : (GetTextColor());
    if (lpDIS->itemState & ODS_GRAYED)
        clrTextColor = GetTextColorDisabled();
    COLORREF clrOldTextColor = dc.SetTextColor(clrTextColor);
    int iOldBkMode = dc.SetBkMode(TRANSPARENT);

    //设定背景色
    CBrush brush(GetBkColor());
    dc.FillRect(&rect, &brush);
    //左侧栏背景
    CRect rcLeft = rect;
    rcLeft.right = rcLeft.left + m_nLeftSideWidth;
    FillGradient(dc, rcLeft, TransformColor(m_clrLeftSideColorLeft), TransformColor(m_clrLeftSideColorRight), TRUE);
    //选中背景
    if (lpDIS->itemState & ODS_SELECTED) {
        CBrush brushSel(TransformColor(GetBkColorSelected()));
        CRect rcSel = rect;
        dc.FillRect(&rcSel, &brushSel);
        DrawRect(dc, rcSel, TransformColor(GetBorderColorSelected()));
    }

    //绘制Check标志或图标
    if (lpDIS->itemState & ODS_CHECKED) {
        CString strCheck = _T("√");
        SIZE szCheck = GetTextExtent(dc, strCheck, m_Font);
        CRect rcCheck(0, 0, szCheck.cx, szCheck.cy);
        FrameCenterRect(rcCheck, rcLeft);
        dc.DrawText(strCheck, rcCheck, DT_LEFT | DT_EXPANDTABS | DT_VCENTER | DT_SINGLELINE);
    } else { //绘制图标
        if (GetImageList() != NULL) {
            CImageList* pImageList = GetImageList();
            if (lpDIS->itemState & ODS_SELECTED)
                pImageList = GetImageListSelected();
            if (lpDIS->itemState & ODS_GRAYED)
                pImageList = GetImageListDisabled();
            if (pImageList == NULL)
                pImageList = GetImageList();
            if (pImageList != NULL && lpItemInfo->iImage >= 0) {
                int iImage = lpItemInfo->iImage;
                int iCount = pImageList->GetImageCount();
                ASSERT(iImage < iCount);
                if (iImage < iCount) {
                    IMAGEINFO imgInfo = {0};
                    pImageList->GetImageInfo(iImage, &imgInfo);
                    CRect rcImage = imgInfo.rcImage;
                    FrameCenterRect(rcImage, rcLeft);
                    pImageList->Draw(&dc, iImage, rcImage.TopLeft(), ILD_NORMAL);
                }
            }
        } else if (GetImageVector() != NULL) {
            std::vector<IImage*>* pImageList = GetImageVector();
            if (lpDIS->itemState & ODS_SELECTED)
                pImageList = GetImageVectorSelected();
            if (lpDIS->itemState & ODS_GRAYED)
                pImageList = GetImageVectorDisabled();
            if (pImageList == NULL)
                pImageList = GetImageVector();
            if (pImageList != NULL && lpItemInfo->iImage >= 0) {
                int iImage = lpItemInfo->iImage;
                int iCount = (int)pImageList->size();
                ASSERT(iImage < iCount);
                if (iImage < iCount) {
                    IImage* pImage = (*pImageList)[iImage];
                    UINT width = 0;
                    UINT height = 0;
                    pImage->get_Width(&width);
                    pImage->get_Height(&height);
                    CRect rcImage(0, 0, width, height);
                    FrameCenterRect(rcImage, rcLeft);
                    pImage->Render(dc, rcImage, NULL, NULL);
                }
            }
        }
    }

    CRect rcText = rect;
    rcText.left += rcLeft.right;
    if(lpItemInfo->uID == 0) //分隔条
    {
        CRect rcLeftSep(0, 0, 0, 0);
        rcLeftSep.left = rcText.left;
        rcLeftSep.top = rcText.top + rcText.Height() / 2;
        rcLeftSep.right = rcLeftSep.left + rcText.Width() / 2;
        rcLeftSep.bottom = rcLeftSep.top + 1;
        FillGradient(dc, rcLeftSep, TransformColor(m_clrSeparatorColorSide), TransformColor(m_clrSeparatorColor), TRUE);

        CRect rcRightSep(0, 0, 0, 0);
        rcRightSep.left = rcLeftSep.right;
        rcRightSep.top = rcText.top + rcText.Height() / 2;
        rcRightSep.right = rcLeftSep.right + rcText.Width() / 2;
        rcRightSep.bottom = rcRightSep.top + 1;
        FillGradient(dc, rcRightSep, TransformColor(m_clrSeparatorColor), TransformColor(m_clrSeparatorColorSide), TRUE);
    }
    else
    {
        //BOOL bSelected = lpDIS->itemState & ODS_SELECTED;
        //BOOL bChecked = lpDIS->itemState & ODS_CHECKED;
        //BOOL bGrayed = lpDIS->itemState & ODS_GRAYED;
        //BOOL bHasImage = (lpItemInfo->iImage != -1);

        //绘制菜单文本
        CRect rcTextDraw = rcText;
        rcTextDraw.top += 1;
        rcTextDraw.left += 4;
        CString strText = lpItemInfo->wstrText.c_str();
        UINT uFormat = DT_LEFT | DT_EXPANDTABS | DT_VCENTER | DT_SINGLELINE;
        if (GetNoPrefix()) 
            uFormat |= DT_NOPREFIX;
        dc.DrawText(strText, rcTextDraw, uFormat);
    }

    //恢复DC设置
    dc.SetBkMode(iOldBkMode);
    dc.SetTextColor(clrOldTextColor);
    dc.SelectObject(pFontOld);
    dc.Detach();
}

} } //namespace common::ui
