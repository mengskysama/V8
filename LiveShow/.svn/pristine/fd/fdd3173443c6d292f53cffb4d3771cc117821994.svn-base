#include "StdAfx.h"
#include "../../include/ui/UITreeProgress.h"
#include "../../include/ui/IUITreeContainer.h"
#include "../../include/ui/UIHelper.h"

namespace common { namespace ui
{

CUITreeProgress::CUITreeProgress(IUITreeContainer* pContainer)
:
IUITreeNode(pContainer),
m_bBitmapProgress(FALSE),
m_lProgress(0),
m_szBorderOffset(2, 2),
m_clrBorderColor(RGB(214, 214, 214)),
m_clrProgressColor(RGB(99, 152, 202)),
m_hBorderBitmap(NULL),
m_hProgressBitmap(NULL),
m_szBorderCorner(2, 2),
m_szProgressCorner(2, 2)
{
}

CUITreeProgress::~CUITreeProgress(void)
{
}

void CUITreeProgress::Draw(CDC* pDC, const RECT& rcBound)
{
    CRect rect = GetRect();
    ParentToChild(rect);

    //IUITreeContainer* pContainer = GetContainer();

    //计算绘制区域
    CRect rcProgress = rect;
    rcProgress.DeflateRect(m_szBorderOffset.cx, m_szBorderOffset.cy);
    rcProgress.right = rcProgress.left + (rcProgress.Width() * m_lProgress) / 100;
    CRect rcBorder = rect;

    //绘制位图进度条
    if (m_bBitmapProgress && m_hBorderBitmap)
    {
        if (m_hProgressBitmap)
        {
            DrawRoundBitmap(*pDC, m_hProgressBitmap, rcProgress, m_szProgressCorner.cx, m_szProgressCorner.cy);
        }
        if (m_hBorderBitmap)
        {
            DrawRoundBitmap(*pDC, m_hBorderBitmap, rcBorder, m_szBorderOffset.cx, m_szBorderOffset.cy);
        }
        return;
    }

    //绘制填充的进度条
    pDC->Draw3dRect(&rect, m_clrBorderColor, m_clrBorderColor);
    pDC->FillSolidRect(&rcProgress, m_clrProgressColor);
}

void CUITreeProgress::SetRect(const RECT& rect)
{
    IUITreeNode::SetRect(rect);
}

void CUITreeProgress::CalcSize(tagSIZE& size)
{
    if (m_bBitmapProgress)
    {
        CSize szBitmap(0, 0);
        if (::GetBitmapDimensionEx(m_hBorderBitmap, &szBitmap))
        {
            size = szBitmap;
            return;
        }
    }
    size.cx = 200;
    size.cy = 8;
}

BOOL CUITreeProgress::GetBitmapProgress()
{
    return m_bBitmapProgress;
}

void CUITreeProgress::SetBitmapProgress(BOOL bTrue)
{
    m_bBitmapProgress = bTrue;
}

LONG CUITreeProgress::GetProgress()
{
    return m_lProgress;
}

void CUITreeProgress::SetProgress(LONG lProgress)
{
    LONG lNewProgress = lProgress;
    if (lNewProgress < 0)
        lNewProgress = 0;
    if (lNewProgress > 100)
        lNewProgress = 100;

    if (lNewProgress != m_lProgress)
    {
        m_lProgress = lNewProgress;
        IUITreeContainer* pContainer = GetContainer();
        pContainer->Redraw(this);
    }
}

tagSIZE CUITreeProgress::GetBorderOffset()
{
    return m_szBorderOffset;
}

void CUITreeProgress::SetBorderOffset(const tagSIZE& szOffset)
{
    m_szBorderOffset = szOffset;
}

LONG CUITreeProgress::GetBorderColor()
{
    return m_clrBorderColor;
}

void CUITreeProgress::SetBorderColor(LONG clrColor)
{
    m_clrBorderColor = clrColor;
}

LONG CUITreeProgress::GetProgressColor()
{
    return m_clrProgressColor;
}

void CUITreeProgress::SetProgressColor(LONG clrColor)
{
    m_clrProgressColor = clrColor;
}

HBITMAP CUITreeProgress::GetBorderBitmap()
{
    return m_hBorderBitmap;
}

void CUITreeProgress::SetBorderBitmap(HBITMAP hBitmap)
{
    m_hBorderBitmap = hBitmap;
}

HBITMAP CUITreeProgress::GetProgressBitmap()
{
    return m_hProgressBitmap;
}

void CUITreeProgress::SetProgressBitmap(HBITMAP hBitmap)
{
    m_hProgressBitmap = hBitmap;
}

tagSIZE CUITreeProgress::GetBorderCorner()
{
    return m_szBorderCorner;
}

void CUITreeProgress::SetBorderCorner(const tagSIZE& szCorner)
{
    m_szBorderCorner = szCorner;
}

tagSIZE CUITreeProgress::GetProgressCorner()
{
    return m_szProgressCorner;
}

void CUITreeProgress::SetProgressCorner(const tagSIZE& szCorner)
{
    m_szProgressCorner = szCorner;
}

} } //namespace common::ui
