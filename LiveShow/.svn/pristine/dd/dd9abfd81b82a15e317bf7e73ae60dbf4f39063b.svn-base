// SkinTab.cpp : implementation file
//

#include "stdafx.h"
#include ".\SkinTab.h"
#include "ImageBase.h"
#include "Types.h"

using namespace std;

#ifdef V8_JS_ENGINE_SUPPORT
using namespace v8;
#endif //V8_JS_ENGINE_SUPPORT

// CSkinTab

IMPLEMENT_DYNCREATE(CSkinTab, CSkinBase)
CSkinTab::CSkinTab()
{
    m_bBackground = FALSE;

    m_enTabType = HORIZ_TAB;
    m_bTabSkinControl = FALSE;

    m_pImageBackground = NULL;

    m_pImageNormal = NULL;
    m_pImageHover = NULL;
    m_pImageDown = NULL;
    m_pImageSelected = NULL;

    m_pImageSeparator = NULL;

    m_bHasButton = FALSE;
    m_bButtonDisabled = FALSE;
    m_pButtonImageNormal = NULL;
    m_pButtonImageHover = NULL;
    m_pButtonImageDown = NULL;
    m_pButtonImageDisabled = NULL;
    m_rcButton = CRect(0, 0, 0, 0);
    m_nButtonLeftMargin = 0;
    m_nButtonTopMargin = 0;

    m_nTextMargin = 0;

    m_nTabItemOffset = 0;

    m_nMinItemWidth = 30;
    m_nMaxItemWidth = 100;

    m_nMinItemHeight = 20;
    m_nMaxItemHeight = 40;

    m_nTabOverlap = 4;
    m_nImageCornerWidth = 3;
    m_nImageCornerHeight = 3;

    m_nTabItemSelected = 0;
    m_nTabItemHot = -1;
    m_nTabItemClick = -1;

	EnableAutomation();
}

CSkinTab::~CSkinTab()
{
}


void CSkinTab::OnFinalRelease()
{
	// When the last reference for an automation object is released
	// OnFinalRelease is called.  The base class will automatically
	// deletes the object.  Add additional cleanup required for your
	// object before calling the base class.

	CCmdTarget::OnFinalRelease();
}


BEGIN_MESSAGE_MAP(CSkinTab, CSkinBase)
END_MESSAGE_MAP()


BEGIN_SKIN_DISPATCH_MAP(CSkinTab, CSkinBase)
	SKINBASE_DISP_PROPERTIES(CSkinTab)

    SKIN_DISP_PROPERTY(CSkinTab, tabStyle, VT_BSTR)
    SKIN_DISP_PROPERTY(CSkinTab, tabSkinControl, VT_BOOL)
    SKIN_DISP_PROPERTY(CSkinTab, imageBackground, VT_BSTR)
    SKIN_DISP_PROPERTY(CSkinTab, imageNormal, VT_BSTR)
    SKIN_DISP_PROPERTY(CSkinTab, imageHover, VT_BSTR)
    SKIN_DISP_PROPERTY(CSkinTab, imageDown, VT_BSTR)
    SKIN_DISP_PROPERTY(CSkinTab, imageSelected, VT_BSTR)
    SKIN_DISP_PROPERTY(CSkinTab, imageSeparator, VT_BSTR)
    SKIN_DISP_PROPERTY(CSkinTab, tabItemOffset, VT_I4)

    SKIN_DISP_PROPERTY(CSkinTab, hasButton, VT_BOOL)
    SKIN_DISP_PROPERTY(CSkinTab, buttonDisabled, VT_BOOL)
    SKIN_DISP_PROPERTY(CSkinTab, buttonImageNormal, VT_BSTR)
    SKIN_DISP_PROPERTY(CSkinTab, buttonImageHover, VT_BSTR)
    SKIN_DISP_PROPERTY(CSkinTab, buttonImageDown, VT_BSTR)
    SKIN_DISP_PROPERTY(CSkinTab, buttonImageDisabled, VT_BSTR)
    SKIN_DISP_PROPERTY(CSkinTab, buttonTopMargin, VT_I4)
    SKIN_DISP_PROPERTY(CSkinTab, buttonLeftMargin, VT_I4)

    SKIN_DISP_PROPERTY(CSkinTab, textMargin, VT_I4)
    SKIN_DISP_PROPERTY(CSkinTab, minItemWidth, VT_I4)
    SKIN_DISP_PROPERTY(CSkinTab, maxItemWidth, VT_I4)
    SKIN_DISP_PROPERTY(CSkinTab, minItemHeight, VT_I4)
    SKIN_DISP_PROPERTY(CSkinTab, maxItemHeight, VT_I4)
    SKIN_DISP_PROPERTY(CSkinTab, imageCornerWidth, VT_I4)
    SKIN_DISP_PROPERTY(CSkinTab, imageCornerHeight, VT_I4)
    SKIN_DISP_PROPERTY(CSkinTab, tabOverlap, VT_I4)
    SKIN_DISP_PROPERTY(CSkinTab, tabItemSelected, VT_I4)
    SKIN_DISP_FUNCTION(CSkinTab, AddTabItem, VT_BOOL, VTS_I4)
    SKIN_DISP_FUNCTION(CSkinTab, RemoveTabItem, VT_BOOL, VTS_I4)
    SKIN_DISP_FUNCTION(CSkinTab, GetTabItemText, VT_BSTR, VTS_I4)
    SKIN_DISP_FUNCTION(CSkinTab, GetTabItemCount, VT_I4, VTS_NONE)
    SKIN_DISP_FUNCTION(CSkinTab, GetTabItemRange, VT_I4, VTS_NONE)
    SKIN_DISP_FUNCTION(CSkinTab, SetTabItemText, VT_BOOL, VTS_I4 VTS_BSTR)
    SKIN_DISP_FUNCTION(CSkinTab, GetTabItemSkin, VT_BSTR, VTS_I4)
    SKIN_DISP_FUNCTION(CSkinTab, SetTabItemSkin, VT_BOOL, VTS_I4 VTS_BSTR)
    SKIN_DISP_FUNCTION(CSkinTab, HitTest, VT_I4, VTS_I4 VTS_I4)

    SKIN_TEXT_STYLE_PROP_PREFIX_PROPERTIES(CSkinTab, text)

END_SKIN_DISPATCH_MAP(CSkinTab)

// Note: we add support for IID_ISkinTab to support typesafe binding
//  from VBA.  This IID must match the GUID that is attached to the 
//  dispinterface in the .IDL file.

// {80C22F40-9F02-45F0-A608-4D10DF73EBE7}
static const IID IID_ISkinTab =
{ 0x80C22F40, 0x9F02, 0x45F0, { 0xA6, 0x8, 0x4D, 0x10, 0xDF, 0x73, 0xEB, 0xE7 } };

BEGIN_INTERFACE_MAP(CSkinTab, CSkinBase)
	INTERFACE_PART(CSkinTab, IID_ISkinTab, Dispatch)
END_INTERFACE_MAP()


// CSkinTab message handlers
BOOL CSkinTab::OnMouseEnter()
{
    InvalidateRect();
    return CSkinBase::OnMouseEnter();    
}

BOOL CSkinTab::OnMouseLeave()
{
    m_nTabItemHot = -1;
    InvalidateRect();
    return CSkinBase::OnMouseLeave();
}

BOOL CSkinTab::OnMouseMove(LONG x, LONG y, UINT flag)
{
    LONG iTabItem = TabItemHitTest(CPoint(x, y));
    if (m_nTabItemClick == -1) {
        if (m_nTabItemHot != iTabItem) {
            m_nTabItemHot = iTabItem;
            InvalidateRect();
        }
    } else {
        if (m_nTabItemHot != -1) {
            m_nTabItemHot = -1;
            InvalidateRect();
        }
    }

    return CSkinBase::OnMouseMove(x, y, flag);
}

BOOL CSkinTab::OnLButtonDown(LONG x, LONG y, UINT flag)
{
    LONG iTabItem = TabItemHitTest(CPoint(x, y));
    if (m_nTabItemClick != iTabItem) {
        m_nTabItemClick = iTabItem;
        InvalidateRect();
    }

    if (IsPointInSkinRgn(x, y) && iTabItem != -1) {
        SetCaptureSkin();
    } else {
        ReleaseCaptureSkin();
    }

    return CSkinBase::OnLButtonDown(x, y, flag);
}

BOOL CSkinTab::OnLButtonUp(LONG x, LONG y, UINT flag)
{
    LONG iTabItem = TabItemHitTest(CPoint(x, y));

    ReleaseCaptureSkin();
    if ( IsPointInSkinRgn(x, y)) {
        if (m_nTabItemClick == iTabItem) {
            if (m_bTabSkinControl) {
                m_nTabItemSelected = m_nTabItemClick;
                UpdateTabItemSkin(m_nTabItemSelected);
            }
            OnClick(m_nTabItemClick);
        }
    } else {
        OnMouseLeave();
    }

    if (m_nTabItemClick != -1) {
        m_nTabItemClick = -1;
        InvalidateRect();
    }

    return CSkinBase::OnLButtonUp(x, y, flag);
}

void CSkinTab::Draw(CDC* pDC, LPRECT lpInvalidRect, CRgn* pInvalidRgn)
{
    const CRect skinRect = GetSkinRect();

    //绘制背景
    if (m_pImageBackground != NULL) {
        CRect bkRect = skinRect;
        DrawImageAlphaHelper(m_pImageBackground, *pDC, &bkRect, NULL, m_bAlpha);
    }

    UpdateTabItemRect();

    //绘制非选中的tab
    vector<SkinTabItemInfo>::size_type vecSize = m_vecTabItems.size();
    for (vector<SkinTabItemInfo>::size_type i=0; i<vecSize; i++) {
        CRect itemRect = m_vecTabItems[i].rcRect;
		CRect rcButton = m_rcButton;
		if ((m_bHasButton&&itemRect.right > rcButton.left)
			||itemRect.right >skinRect.right
			)
		{
			break;//绘制的按钮超出范围，终止
		}
        itemRect.OffsetRect(skinRect.left, skinRect.top);
        if (m_nTabItemSelected != i) {
            if (m_nTabItemClick == i) {
                DrawImageAlphaSideHelper(m_pImageDown, *pDC, &itemRect, NULL, m_bAlpha, FALSE, m_nImageCornerWidth, m_nImageCornerHeight);
            } else if (m_nTabItemHot == i) {
                DrawImageAlphaSideHelper(m_pImageHover, *pDC, &itemRect, NULL, m_bAlpha, FALSE, m_nImageCornerWidth, m_nImageCornerHeight);
            } else {
                DrawImageAlphaSideHelper(m_pImageNormal, *pDC, &itemRect, NULL, m_bAlpha, FALSE, m_nImageCornerWidth, m_nImageCornerHeight);
            }
            CRect textRect = itemRect;
            textRect.left += m_nTextMargin;
            textRect.right -= m_nTextMargin;
            m_textStyle.DrawText(*pDC, m_vecTabItems[i].strText, &textRect);
        }
    }

    //绘制分隔条
    for (vector<SkinTabItemInfo>::size_type i=0; i<vecSize; i++) {
        CRect itemRect = m_vecTabItems[i].rcRect;
		CRect rcButton = m_rcButton;
		if ((m_bHasButton&&itemRect.right > rcButton.left)
			||itemRect.right >skinRect.right
			)
		{
			break;//绘制的按钮超出范围，终止
		}
        itemRect.OffsetRect(skinRect.left, skinRect.top);

        //绘制分隔条
        if (m_pImageSeparator != NULL) {
            CRect rcSeparator(0, 0, m_pImageSeparator->Width(), m_pImageSeparator->Height());
            rcSeparator.OffsetRect((itemRect.right - m_nTabOverlap / 2 - rcSeparator.Width() / 2), 
                itemRect.top + (itemRect.Height() - rcSeparator.Height()));
            DrawImageAlphaSideHelper(m_pImageSeparator, *pDC, &rcSeparator, NULL, m_bAlpha, FALSE, m_nImageCornerWidth, m_nImageCornerHeight);
        }
    }

    //绘制选中的Tab
    if (m_nTabItemSelected >= 0 && m_nTabItemSelected < (int)m_vecTabItems.size()) {
        CImageBase* pImage = m_pImageSelected;
        if (pImage == NULL)
            pImage = m_pImageNormal;

        CRect itemRect = m_vecTabItems[m_nTabItemSelected].rcRect;
		CRect rcButton = m_rcButton;
		if ((m_bHasButton&&itemRect.right > rcButton.left)
			||itemRect.right >skinRect.right
			)
		{
			;//绘制的按钮超出范围，终止
		}
		else
		{
			itemRect.OffsetRect(skinRect.left, skinRect.top);
			DrawImageAlphaSideHelper(pImage, *pDC, &itemRect, NULL, m_bAlpha, FALSE, m_nImageCornerWidth, m_nImageCornerHeight);
			CRect textRect = itemRect;
			textRect.left += m_nTextMargin;
			textRect.right -= m_nTextMargin;
			m_textStyle.DrawText(*pDC, m_vecTabItems[m_nTabItemSelected].strText, &textRect);
		}

    }

    //绘制按钮
    CRect rcButton = m_rcButton;
    rcButton.OffsetRect(skinRect.left, skinRect.top);
    if (m_bHasButton && !rcButton.IsRectEmpty()) {
        if (m_bButtonDisabled) {
            DrawImageAlphaSideHelper(m_pButtonImageDisabled, *pDC, &rcButton, NULL, m_bAlpha, TRUE);
        } else if (m_nTabItemClick == -2) {
            DrawImageAlphaSideHelper(m_pButtonImageDown, *pDC, &rcButton, NULL, m_bAlpha, TRUE);
        } else if (m_nTabItemHot == -2) {
            DrawImageAlphaSideHelper(m_pButtonImageHover, *pDC, &rcButton, NULL, m_bAlpha, TRUE);
        } else {
            DrawImageAlphaSideHelper(m_pButtonImageNormal, *pDC, &rcButton, NULL, m_bAlpha, TRUE);
        }
    }
}

void CSkinTab::UpdateTabItemRect()
{
    if (m_nTabItemOffset < 0)
        m_nTabItemOffset = 0;
    if (m_nTabItemOffset >= (LONG)m_vecTabItems.size())
        m_nTabItemOffset = (LONG)m_vecTabItems.size() - 1;

    if (m_enTabType == HORIZ_TAB) {

        //计算原始tabItem的位置
        CRect skinRect = GetSkinRect();
        vector<SkinTabItemInfo>::size_type vecSize = m_vecTabItems.size();
        CRect itemRect(0, 0, 0, skinRect.Height());
        itemRect.OffsetRect(m_nTabOverlap, 0);
        LONG tabItemOffset = 0;
        for (vector<SkinTabItemInfo>::size_type i=0; i<vecSize; i++) {
            SkinTabItemInfo& tabInfo = m_vecTabItems[i];
            itemRect.OffsetRect(itemRect.Width() - m_nTabOverlap, 0);
            CSize szText(0, 0);
            m_textStyle.GetTextSize(NULL, tabInfo.strText, szText);
            LONG itemWidth = szText.cx + 20;
            if (itemWidth < m_nMinItemWidth)
                itemWidth = m_nMinItemWidth;
            if (itemWidth > m_nMaxItemWidth)
                itemWidth = m_nMaxItemWidth;
            itemRect.right = itemRect.left + itemWidth;
            tabInfo.rcRect = itemRect;
            if (m_nTabItemOffset == i)
                tabItemOffset = itemRect.left;
        }

        //偏移
        for (vector<SkinTabItemInfo>::size_type i=0; i<vecSize; i++) {
            SkinTabItemInfo& tabInfo = m_vecTabItems[i];
            tabInfo.rcRect.OffsetRect(-tabItemOffset, 0);
        }
        itemRect.OffsetRect(-tabItemOffset, 0);

        //定位button
        CRect rcButton(0, 0, 0, 0);
        m_rcButton = rcButton;
        if (m_bHasButton) {
            CImageBase* pButtonImage = GetImageHelper(m_pButtonImageNormal, m_pButtonImageHover, m_pButtonImageDown, m_pButtonImageDisabled);
            if (pButtonImage != NULL) {
                rcButton.right = pButtonImage->Width();
                rcButton.bottom = pButtonImage->Height();
            }
            rcButton.OffsetRect(itemRect.right, 0);
            rcButton.OffsetRect(m_nButtonLeftMargin, m_nButtonTopMargin);
            if (rcButton.right > skinRect.Width()) {
                rcButton.OffsetRect(skinRect.Width() - rcButton.right, 0);
            }

            m_rcButton = rcButton;
        }
    } else if (m_enTabType == VERTI_TAB) {
        m_rcButton = CRect(0, 0, 0, 0);
        CRect skinRect = GetSkinRect();
        vector<SkinTabItemInfo>::size_type vecSize = m_vecTabItems.size();
        CRect itemRect(0, 0, skinRect.Width(), 0);
        itemRect.OffsetRect(0, m_nTabOverlap);
        for (vector<SkinTabItemInfo>::size_type i=0; i<vecSize; i++) {
            SkinTabItemInfo& tabInfo = m_vecTabItems[i];
            itemRect.OffsetRect(0, itemRect.Height() - m_nTabOverlap);
            CSize szText(0, 0);
            m_textStyle.GetTextSize(NULL, tabInfo.strText, szText);
            LONG itemHeight = szText.cy + 20;
            CImageBase* pImage = GetImageHelper(m_pImageNormal, m_pImageHover, m_pImageDown, m_pImageSelected);
            if (pImage != NULL)
                itemHeight = pImage->Height();
            if (itemHeight < m_nMinItemHeight)
                itemHeight = m_nMinItemHeight;
            if (itemHeight > m_nMaxItemHeight)
                itemHeight = m_nMaxItemHeight;
            itemRect.bottom = itemRect.top + itemHeight;
            tabInfo.rcRect = itemRect;
        }
    }
}

void CSkinTab::UpdateTabItemSkin(LONG iSelected)
{
    CSkinBase* pParent = GetSkinParent();
    if (pParent == NULL)
        return;
    vector<SkinTabItemInfo>::size_type vecSize = m_vecTabItems.size();
    for (vector<SkinTabItemInfo>::size_type i=0; i<vecSize; i++) {
        SkinTabItemInfo& tabInfo = m_vecTabItems[i];
        VARIANT_BOOL bVisible = (i == iSelected) ? (VARIANT_TRUE) : (VARIANT_FALSE);
        CSkinBase* pSkin = pParent->GetSubSkin(tabInfo.strSkinName);
        if (pSkin == NULL)
            continue;
        pSkin->Setvisible(bVisible);
    }
}

LONG CSkinTab::TabItemHitTest(POINT pt)
{
    CRect skinRect = GetSkinRect();
    CPoint point = pt;
    point.Offset(-skinRect.left, -skinRect.top);

    if (m_bHasButton && m_rcButton.PtInRect(point))
        return -2;

    vector<SkinTabItemInfo>::size_type vecSize = m_vecTabItems.size();
    if (m_nTabItemSelected >= 0 && m_nTabItemSelected < (LONG)vecSize) {
        if (m_vecTabItems[m_nTabItemSelected].rcRect.PtInRect(point))
            return m_nTabItemSelected;
    }

    for (vector<SkinTabItemInfo>::size_type i=0; i<vecSize; i++) {

        if (m_vecTabItems[i].rcRect.PtInRect(point))
		{		
			CRect rcButton = m_rcButton;
			CRect itemRect = m_vecTabItems[i].rcRect;
			if ((m_bHasButton&&itemRect.right > rcButton.left)
				||itemRect.right >skinRect.right
				)
			{
				return -1;//绘制的按钮超出范围，终止
			}
			else
                return (LONG)i;
		}
    }

    return -1;
}

CImageBase* CSkinTab::GetImageHelper(CImageBase* pImage0, CImageBase* pImage1, CImageBase* pImage2, CImageBase* pImage3)
{
    if (pImage0 != NULL)
        return pImage0;

    if (pImage1 != NULL)
        return pImage1;

    if (pImage2 != NULL)
        return pImage2;

    if (pImage3 != NULL)
        return pImage3;

    return NULL;
}

IMPLEMENT_SKIN_TEXT_STYLE_PREFIX_PROPERTIES(CSkinTab, text, m_textStyle);

BSTR CSkinTab::GettabStyle()
{
    switch (m_enTabType)
    {
    case HORIZ_TAB:
        return L"horiz";
    case VERTI_TAB:
        return L"verti";
    default:
        ASSERT(0);
        return L"";
    }
}

void CSkinTab::SettabStyle(LPCTSTR newVal)
{
    CString strType = newVal;
    if (strType.CompareNoCase(_T("horiz")) == 0) {
        if (m_enTabType == HORIZ_TAB)
            return;
        m_enTabType = HORIZ_TAB;
        UpdateTabItemRect();
        InvalidateRect();
        return;
    } else if (strType.CompareNoCase(_T("verti")) == 0) {
        if (m_enTabType == VERTI_TAB)
            return;
        m_enTabType = VERTI_TAB;
        UpdateTabItemRect();
        InvalidateRect();
        return;
    }
}

VARIANT_BOOL CSkinTab::GettabSkinControl()
{
    return m_bTabSkinControl ? (VARIANT_TRUE) : (VARIANT_FALSE);
}

void CSkinTab::SettabSkinControl(VARIANT_BOOL newVal)
{
    BOOL bNew = (newVal != 0);
    if (m_bTabSkinControl == bNew)
        return;

    m_bTabSkinControl = bNew;
    InvalidateRect();
}

BSTR CSkinTab::GetimageBackground()
{
    return L"";
}

void CSkinTab::SetimageBackground(LPCTSTR newVal)
{
    CString strTemp;
    CSkinBase::SetImageHelper(m_pImageBackground, strTemp, newVal);
}

BSTR CSkinTab::GetimageNormal()
{
    return L"";
}

void CSkinTab::SetimageNormal(LPCTSTR newVal)
{
    CString strTemp;
    CSkinBase::SetImageHelper(m_pImageNormal, strTemp, newVal);
}

BSTR CSkinTab::GetimageHover()
{
    return L"";
}

void CSkinTab::SetimageHover(LPCTSTR newVal)
{
    CString strTemp;
    CSkinBase::SetImageHelper(m_pImageHover, strTemp, newVal);
}

BSTR CSkinTab::GetimageDown()
{
    return L"";
}

void CSkinTab::SetimageDown(LPCTSTR newVal)
{
    CString strTemp;
    CSkinBase::SetImageHelper(m_pImageDown, strTemp, newVal);
}

BSTR CSkinTab::GetimageSelected()
{
    return L"";
}

void CSkinTab::SetimageSelected(LPCTSTR newVal)
{
    CString strTemp;
    CSkinBase::SetImageHelper(m_pImageSelected, strTemp, newVal);
}

BSTR CSkinTab::GetimageSeparator()
{
    return L"";
}

void CSkinTab::SetimageSeparator(LPCTSTR newVal)
{
    CString strTemp;
    CSkinBase::SetImageHelper(m_pImageSeparator, strTemp, newVal);
}

VARIANT_BOOL CSkinTab::GethasButton()
{
    return m_bHasButton ? (VARIANT_TRUE) : (VARIANT_FALSE);
}

void CSkinTab::SethasButton(VARIANT_BOOL newVal)
{
    BOOL bNew = (newVal != 0);
    if (m_bHasButton == bNew)
        return;

    m_bHasButton = bNew;
    InvalidateRect();
}

VARIANT_BOOL CSkinTab::GetbuttonDisabled()
{
    return m_bHasButton ? (VARIANT_TRUE) : (VARIANT_FALSE);
}

void CSkinTab::SetbuttonDisabled(VARIANT_BOOL newVal)
{
    BOOL bNew = (newVal != 0);
    if (m_bButtonDisabled == bNew)
        return;

    m_bButtonDisabled = bNew;
    InvalidateRect();
}

BSTR CSkinTab::GetbuttonImageNormal()
{
    return L"";
}

void CSkinTab::SetbuttonImageNormal(LPCTSTR newVal)
{
    CString strTemp;
    CSkinBase::SetImageHelper(m_pButtonImageNormal, strTemp, newVal);
}

BSTR CSkinTab::GetbuttonImageHover()
{
    return L"";
}

void CSkinTab::SetbuttonImageHover(LPCTSTR newVal)
{
    CString strTemp;
    CSkinBase::SetImageHelper(m_pButtonImageHover, strTemp, newVal);
}

BSTR CSkinTab::GetbuttonImageDown()
{
    return L"";
}

void CSkinTab::SetbuttonImageDown(LPCTSTR newVal)
{
    CString strTemp;
    CSkinBase::SetImageHelper(m_pButtonImageDown, strTemp, newVal);
}

BSTR CSkinTab::GetbuttonImageDisabled()
{
    return L"";
}

void CSkinTab::SetbuttonImageDisabled(LPCTSTR newVal)
{
    CString strTemp;
    CSkinBase::SetImageHelper(m_pButtonImageDisabled, strTemp, newVal);
}

LONG CSkinTab::GetbuttonTopMargin()
{
    return m_nButtonTopMargin;
}

void CSkinTab::SetbuttonTopMargin(LONG newVal)
{
    if (m_nButtonTopMargin == newVal)
        return;

    m_nButtonTopMargin = newVal;
    UpdateTabItemRect();
    InvalidateRect();
}

LONG CSkinTab::GetbuttonLeftMargin()
{
    return m_nButtonTopMargin;
}

void CSkinTab::SetbuttonLeftMargin(LONG newVal)
{
    if (m_nButtonLeftMargin == newVal)
        return;

    m_nButtonLeftMargin = newVal;
    UpdateTabItemRect();
    InvalidateRect();
}

LONG CSkinTab::GettextMargin()
{
    return m_nTextMargin;
}

void CSkinTab::SettextMargin(LONG newVal)
{
    if (m_nTextMargin == newVal)
        return;

    m_nTextMargin = newVal;
    UpdateTabItemRect();
    InvalidateRect();
}

LONG CSkinTab::GetminItemWidth()
{
    return m_nMinItemWidth;
}

void CSkinTab::SetminItemWidth(LONG newVal)
{
    if (m_nMinItemWidth == newVal)
        return;

    m_nMinItemWidth = newVal;
    UpdateTabItemRect();
    InvalidateRect();
}

LONG CSkinTab::GetmaxItemWidth()
{
    return m_nMaxItemWidth;
}

void CSkinTab::SetmaxItemWidth(LONG newVal)
{
    if (m_nMaxItemWidth == newVal)
        return;

    m_nMaxItemWidth = newVal;
    UpdateTabItemRect();
    InvalidateRect();
}

LONG CSkinTab::GetminItemHeight()
{
    return m_nMinItemHeight;
}

void CSkinTab::SetminItemHeight(LONG newVal)
{
    if (m_nMinItemHeight == newVal)
        return;

    m_nMinItemHeight = newVal;
    UpdateTabItemRect();
    InvalidateRect();
}

LONG CSkinTab::GetmaxItemHeight()
{
    return m_nMaxItemHeight;
}

void CSkinTab::SetmaxItemHeight(LONG newVal)
{
    if (m_nMaxItemHeight == newVal)
        return;

    m_nMaxItemHeight = newVal;
    UpdateTabItemRect();
    InvalidateRect();
}

LONG CSkinTab::GetimageCornerWidth()
{
    return m_nImageCornerWidth;
}

void CSkinTab::SetimageCornerWidth(LONG newVal)
{
    if (m_nImageCornerWidth == newVal)
        return;

    m_nImageCornerWidth = newVal;
    InvalidateRect();
}

LONG CSkinTab::GetimageCornerHeight()
{
    return m_nImageCornerHeight;
}

void CSkinTab::SetimageCornerHeight(LONG newVal)
{
    if (m_nImageCornerHeight == newVal)
        return;

    m_nImageCornerHeight = newVal;
    InvalidateRect();
}

LONG CSkinTab::GettabOverlap()
{
    return m_nTabOverlap;
}

void CSkinTab::SettabOverlap(LONG newVal)
{
    if (m_nTabOverlap == newVal)
        return;

    m_nTabOverlap = newVal;
    UpdateTabItemRect();
    InvalidateRect();
}

LONG CSkinTab::GettabItemSelected()
{
    return m_nTabItemSelected;
}

void CSkinTab::SettabItemSelected(LONG newVal)
{
    if (newVal != -1 && (newVal < 0 || newVal >= (LONG)m_vecTabItems.size()))
        return;

    if (m_nTabItemSelected == newVal)
        return;

    m_nTabItemSelected = newVal;
    if (m_bTabSkinControl) {
        UpdateTabItemSkin(m_nTabItemSelected);
    }
    InvalidateRect();
}

LONG CSkinTab::GettabItemOffset()
{
    return m_nTabItemOffset;
}

void CSkinTab::SettabItemOffset(LONG newVal)
{
    if (newVal < 0 || newVal >= (LONG)m_vecTabItems.size())
        return;

    if (m_nTabItemOffset == newVal)
        return;

    m_nTabItemOffset = newVal;
    UpdateTabItemRect();
    InvalidateRect();
}

//方法
VARIANT_BOOL CSkinTab::AddTabItem(LONG nPos)
{
    if (nPos < 0 && nPos != -1)
        return VARIANT_FALSE;

    if (nPos >= 0 && nPos > (LONG)m_vecTabItems.size())
        return VARIANT_FALSE;

    SkinTabItemInfo itemInfo;
    if (nPos == -1) {
        m_vecTabItems.push_back(itemInfo);
    } else {
        m_vecTabItems.insert(m_vecTabItems.begin() + nPos, itemInfo);
    }

    UpdateTabItemRect();
    InvalidateRect();
    return VARIANT_TRUE;
}

VARIANT_BOOL CSkinTab::RemoveTabItem(LONG nPos)
{
    if (nPos < 0 || nPos >= (LONG)m_vecTabItems.size())
        return VARIANT_FALSE;

    m_vecTabItems.erase(m_vecTabItems.begin() + nPos);

    UpdateTabItemRect();
    InvalidateRect();
    return VARIANT_TRUE;
}

LONG CSkinTab::GetTabItemCount()
{
    return (LONG)m_vecTabItems.size();
}

LONG CSkinTab::GetTabItemRange()
{
    if (m_vecTabItems.size() <= 0)
        return 0;

    size_t firstIndex = 0;
    size_t lastIndex = m_vecTabItems.size() - 1;
    if (m_enTabType == HORIZ_TAB) {
        return m_vecTabItems[lastIndex].rcRect.right - m_vecTabItems[firstIndex].rcRect.left;
    } else if (m_enTabType == VERTI_TAB) {
        return m_vecTabItems[lastIndex].rcRect.bottom - m_vecTabItems[firstIndex].rcRect.top;
    }

    ASSERT(0);
    return 0;
}

BSTR CSkinTab::GetTabItemText(LONG nItem)
{
    if (nItem < 0 || nItem >= (LONG)m_vecTabItems.size())
        return L"";

    return m_vecTabItems[nItem].strText.AllocSysString();
}

VARIANT_BOOL CSkinTab::SetTabItemText(LONG nItem, LPCTSTR lpzText)
{
    if (nItem < 0 || nItem >= (LONG)m_vecTabItems.size())
        return VARIANT_FALSE;

    m_vecTabItems[nItem].strText = lpzText;
    UpdateTabItemRect();
    InvalidateRect();
    return VARIANT_TRUE;
}

BSTR CSkinTab::GetTabItemSkin(LONG nItem)
{
    if (nItem < 0 || nItem >= (LONG)m_vecTabItems.size())
        return L"";

    return m_vecTabItems[nItem].strSkinName.AllocSysString();
}

VARIANT_BOOL CSkinTab::SetTabItemSkin(LONG nItem, LPCTSTR lpzName)
{
    if (nItem < 0 || nItem >= (LONG)m_vecTabItems.size())
        return VARIANT_FALSE;

    m_vecTabItems[nItem].strSkinName = lpzName;
    return VARIANT_TRUE;
}

LONG CSkinTab::HitTest(LONG x, LONG y)
{
    CPoint pt(x, y);
    return TabItemHitTest(pt);
}

#ifdef V8_JS_ENGINE_SUPPORT
Handle<Value> CSkinTab::V8AddTabItem(const Arguments& args)
{
    HandleScope handleScope;

    LONG arg0 = Handle<Integer>::Cast(args[0])->Int32Value();
    VARIANT_BOOL ret = AddTabItem(arg0);
    Handle<Value> result = ret ? (Boolean::New(true)) : (Boolean::New(false));

    return handleScope.Close(result);
}

Handle<Value> CSkinTab::V8RemoveTabItem(const Arguments& args)
{
    HandleScope handleScope;

    LONG arg0 = Handle<Integer>::Cast(args[0])->Int32Value();
    VARIANT_BOOL ret = RemoveTabItem(arg0);
    Handle<Value> result = ret ? (Boolean::New(true)) : (Boolean::New(false));

    return handleScope.Close(result);
}

Handle<Value> CSkinTab::V8GetTabItemCount(const Arguments& args)
{
    HandleScope handleScope;

    LONG count = GetTabItemCount();
    Handle<Value> result = Integer::New(count);

    return handleScope.Close(result);
}

Handle<Value> CSkinTab::V8GetTabItemRange(const Arguments& args)
{
    HandleScope handleScope;

    LONG count = GetTabItemRange();
    Handle<Value> result = Integer::New(count);

    return handleScope.Close(result);
}

Handle<Value> CSkinTab::V8GetTabItemText(const Arguments& args)
{
    HandleScope handleScope;

    LONG arg0 = Handle<Integer>::Cast(args[0])->Int32Value();

    BSTR ret = GetTabItemText(arg0);

    CComBSTR bstrRet;
    bstrRet.Attach(ret);
    CComVariant varRet = bstrRet;
    Handle<Value> result = VariantToValue(varRet);

    return handleScope.Close(result);
}

Handle<Value> CSkinTab::V8SetTabItemText(const Arguments& args)
{
    HandleScope handleScope;

    LONG arg0 = Handle<Integer>::Cast(args[0])->Int32Value();
    CString arg1 = ValueToCString(args[1]);

    VARIANT_BOOL ret = SetTabItemText(arg0, arg1);

    Handle<Value> result = ret ? (Boolean::New(true)) : (Boolean::New(false));

    return handleScope.Close(result);
}

Handle<Value> CSkinTab::V8GetTabItemSkin(const Arguments& args)
{
    HandleScope handleScope;

    LONG arg0 = Handle<Integer>::Cast(args[0])->Int32Value();

    BSTR ret = GetTabItemSkin(arg0);

    CComBSTR bstrRet;
    bstrRet.Attach(ret);
    CComVariant varRet = bstrRet;
    Handle<Value> result = VariantToValue(varRet);

    return handleScope.Close(result);
}

Handle<Value> CSkinTab::V8SetTabItemSkin(const Arguments& args)
{
    HandleScope handleScope;

    LONG arg0 = Handle<Integer>::Cast(args[0])->Int32Value();
    CString arg1 = ValueToCString(args[1]);

    VARIANT_BOOL ret = SetTabItemSkin(arg0, arg1);

    Handle<Value> result = ret ? (Boolean::New(true)) : (Boolean::New(false));

    return handleScope.Close(result);
}

Handle<Value> CSkinTab::V8HitTest(const Arguments& args)
{
    HandleScope handleScope;

    LONG arg0 = Handle<Integer>::Cast(args[0])->Int32Value();
    LONG arg1 = Handle<Integer>::Cast(args[1])->Int32Value();

    LONG ret = HitTest(arg0, arg1);

    Handle<Value> result = Integer::New(ret);

    return handleScope.Close(result);
}

Handle<Value> CSkinTab::V8InternalCallMethod(const CString& funcName, const Arguments& args)
{
    if (funcName.CompareNoCase(_T("AddTabItem")) == 0) {
        return V8AddTabItem(args);
    } else if (funcName.CompareNoCase(_T("RemoveTabItem")) == 0) {
        return V8RemoveTabItem(args);
    } else if (funcName.CompareNoCase(_T("GetTabItemCount")) == 0) {
        return V8GetTabItemCount(args);
    } else if (funcName.CompareNoCase(_T("GetTabItemRange")) == 0) {
        return V8GetTabItemRange(args);
    } else if (funcName.CompareNoCase(_T("GetTabItemText")) == 0) {
        return V8GetTabItemText(args);
    } else if (funcName.CompareNoCase(_T("SetTabItemText")) == 0) {
        return V8SetTabItemText(args);
    } else if (funcName.CompareNoCase(_T("GetTabItemSkin")) == 0) {
        return V8GetTabItemSkin(args);
    } else if (funcName.CompareNoCase(_T("SetTabItemSkin")) == 0) {
        return V8SetTabItemSkin(args);
    } else if (funcName.CompareNoCase(_T("HitTest")) == 0) {
        return V8HitTest(args);
    }

    return CSkinBase::V8InternalCallMethod(funcName, args);
}
#endif //V8_JS_ENGINE_SUPPORT