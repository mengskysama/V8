#include "StdAfx.h"
#include "../../include/ui/UITreeIconButton.h"
#include "../../include/ui/IUITreeContainer.h"
#include "../../include/ui/uihelper.h"

namespace common { namespace ui
{

CUITreeIconButton::CUITreeIconButton(IUITreeContainer* pContainer)
:
CUITreeButtonNode(pContainer),
m_iIcon(-1),
m_iHoverIcon(-1),
m_iClickIcon(-1),
m_iBitmap(-1),
m_iCheckedBitmap(-1),
m_iCheckedIcon(-1),
m_bCheckable(true)
{
    SetRect(CRect(0, 0, 16, 16));
}

CUITreeIconButton::~CUITreeIconButton(void)
{
}

void CUITreeIconButton::Draw(CDC* pDC, const RECT& rcBound)
{
    CRect rect = GetRect();
    ParentToChild(rect);

    IUITreeContainer* pContainer = GetContainer();
    CSelectGdiHelper<HFONT> helper(*pDC, pContainer->GetFont());

    //»æÖÆÍ¼±ê
    HICON hIcon = pContainer->GetIcon(m_iIcon);
    if (IsChecked() && m_bCheckable) {
        hIcon = pContainer->GetIcon(m_iCheckedIcon);
    } else {
        HICON hHotOrPushedIcon = NULL;
        if (IsPushed()) {
            hHotOrPushedIcon = pContainer->GetIcon(m_iClickIcon);
        } else if (IsHot()) {
            hHotOrPushedIcon = pContainer->GetIcon(m_iHoverIcon);
        }
        if (hHotOrPushedIcon != NULL)
            hIcon = hHotOrPushedIcon;
    }

    if (hIcon != NULL) {
        ::DrawIconEx(*pDC, 0, 0, hIcon, rect.Width(), rect.Height(), 0, NULL, DI_NORMAL);
    }

	int iBmp = IsChecked() ? (m_iCheckedBitmap) : (m_iBitmap);
	HBITMAP hBmp = pContainer->GetBitmap(iBmp);
	if (hBmp != NULL) {
		DrawBitmap(pDC->GetSafeHdc(), hBmp, rect);
	}
}

void CUITreeIconButton::SetRect(const RECT& rect)
{
    IUITreeNode::SetRect(rect);
}

void CUITreeIconButton::CalcSize(tagSIZE& size)
{
    size.cx = 16;
    size.cy = 16;
}

int CUITreeIconButton::GetIcon()
{
    return m_iIcon;
}

void CUITreeIconButton::SetIcon(int iIcon)
{
    m_iIcon = iIcon;
}

int CUITreeIconButton::GetHoverIcon()
{
    return m_iHoverIcon;
}

void CUITreeIconButton::SetHoverIcon(int iIcon)
{
    m_iHoverIcon = iIcon;
}

int CUITreeIconButton::GetClickIcon()
{
    return m_iClickIcon;
}

void CUITreeIconButton::SetClickIcon(int iIcon)
{
    m_iClickIcon = iIcon;
}

int CUITreeIconButton::GetCheckedIcon()
{
    return m_iCheckedIcon;
}

void CUITreeIconButton::SetCheckedIcon(int iIcon)
{
    m_iCheckedIcon = iIcon;
}

void CUITreeIconButton::SetBitmap(int iBmp)
{
	m_iBitmap = iBmp;
}

int CUITreeIconButton::GetBitmap(int iBmp)
{
	return m_iBitmap;
}

void CUITreeIconButton::SetCheckedBitmap(int iBmp)
{
	m_iCheckedBitmap = iBmp;
}

int CUITreeIconButton::GetCheckedBitmap(int iBmp)
{
	return m_iCheckedBitmap;
}

bool CUITreeIconButton::GetCheckable()
{
	return m_bCheckable;
}

void CUITreeIconButton::SetCheckable(bool bTrue)
{
	m_bCheckable = (bTrue != false);
}

} } //namespace common::ui
