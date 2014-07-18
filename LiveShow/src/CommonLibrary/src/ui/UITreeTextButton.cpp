#include "StdAfx.h"
#include "../../include/ui/UITreeTextButton.h"
#include "../../include/ui/IUITreeContainer.h"

namespace common { namespace ui
{

CUITreeTextButton::CUITreeTextButton(IUITreeContainer* pContainer)
:
CUITreeButtonNode(pContainer),
m_iFont(-1),
m_iClrNormal(-1),
m_iClrHighlight(-1),
m_bNormalUnderline(false),
m_bHighlightUnderline(true),
m_iUnderlineMargin(0)
{
}

CUITreeTextButton::~CUITreeTextButton(void)
{
}

void CUITreeTextButton::Draw(CDC* pDC, const RECT& rcBound)
{
    CRect rect = GetRect();
    ParentToChild(rect);

    IUITreeContainer* pContainer = GetContainer();
    int oldMode = pDC->SetBkMode(TRANSPARENT);
    COLORREF textColor = RGB(0, 0, 0);
    if (IsHot())
        textColor = pContainer->GetColor(m_iClrHighlight);
    else 
        textColor = pContainer->GetColor(m_iClrNormal);
    int oldTextColor = pDC->SetTextColor(textColor);

    //绘制文本
    CSelectGdiHelper<HFONT> helper(*pDC, pContainer->GetFont(m_iFont));
    CSize szSize(0, 0);
    ::GetTextExtentPoint32(*pDC, m_strText, m_strText.GetLength(), &szSize);
    if (szSize.cx > rect.Width())
        szSize.cx = rect.Width();
    CRect textRect(0, 0, szSize.cx, szSize.cy);
    textRect.OffsetRect(0, (rect.Height() - textRect.Height()) / 2);
    pDC->DrawText(m_strText, &textRect, DT_LEFT | DT_SINGLELINE | DT_END_ELLIPSIS | DT_NOPREFIX);

    //绘制下划线
    if (   (IsHot() && m_bHighlightUnderline)
        || (!IsHot() && m_bNormalUnderline)
        ) {
        CPen pen(PS_SOLID, 1, textColor);
        CSelectGdiHelper<HPEN> linePenHelper(*pDC, (HPEN)pen);
        CPoint start(textRect.left, textRect.bottom + m_iUnderlineMargin);
        CPoint end(textRect.right, textRect.bottom + m_iUnderlineMargin);
        pDC->MoveTo(start);
        pDC->LineTo(end);
    }

    pDC->SetBkMode(oldMode);
    pDC->SetTextColor(oldTextColor);
}

void CUITreeTextButton::SetRect(const RECT& rect)
{
    return IUITreeNode::SetRect(rect);
}

void CUITreeTextButton::CalcSize(tagSIZE& size)
{
    IUITreeContainer* pContainer = GetContainer();
    HFONT hFont = NULL;
    if (pContainer)
        hFont = pContainer->GetFont(m_iFont);

    HDC hDC = ::GetDC(NULL);
    CSize szSize(0, 0);
    if (hFont) {
        CSelectGdiHelper<HFONT> selector(hDC, hFont);
        ::GetTextExtentPoint32(hDC, m_strText, m_strText.GetLength(), &szSize);
    } else {
        ::GetTextExtentPoint32(hDC, m_strText, m_strText.GetLength(), &szSize);
    }
    ::ReleaseDC(NULL, hDC);
    if (szSize.cx != 0 || size.cy != 0) {
        szSize.cx += 1;
        szSize.cy += 1;
    }

    size = szSize;
}

CString CUITreeTextButton::GetText()
{
    return m_strText;
}

void CUITreeTextButton::SetText(LPCTSTR lpzText)
{
    m_strText = lpzText;
}

int CUITreeTextButton::GetFont()
{
    return m_iFont;
}

void CUITreeTextButton::SetFont(int iFont)
{
    m_iFont = iFont;
}

int CUITreeTextButton::GetNormalColor()
{
    return m_iClrNormal;
}

void CUITreeTextButton::SetNormalColor(int iColor)
{
    m_iClrNormal = iColor;
}

int CUITreeTextButton::GetHighlightColor()
{
    return m_iClrHighlight;
}

void CUITreeTextButton::SetHighlightColor(int iColor)
{
    m_iClrHighlight = iColor;
}

bool CUITreeTextButton::GetNormalUnderline()
{
    return m_bNormalUnderline;
}

void CUITreeTextButton::SetNormalUnderline(bool bTrue)
{
    m_bNormalUnderline = bTrue;
}

bool CUITreeTextButton::GetHighlightUnderline()
{
    return m_bHighlightUnderline;
}

void CUITreeTextButton::SetHighlightUnderline(bool bTrue)
{
    m_bHighlightUnderline = bTrue;
}

int CUITreeTextButton::GetUnderlineMargin()
{
    return m_iUnderlineMargin;
}

void CUITreeTextButton::SetUnderlineMargin(int iMargin)
{
    m_iUnderlineMargin = iMargin;
}

} } //namespace common::ui
