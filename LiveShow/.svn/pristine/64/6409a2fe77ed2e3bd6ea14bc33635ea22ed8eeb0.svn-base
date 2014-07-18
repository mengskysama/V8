// SkinToolTipCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "Skin.h"
#include "SkinToolTipCtrl.h"
#include "ImageBase.h"


// CSkinToolTipCtrl

IMPLEMENT_DYNCREATE(CSkinToolTipCtrl, CWnd)

CSkinToolTipCtrl::CSkinToolTipCtrl()
{
    //客户区域大小
    m_szClient = CSize(0, 0);

    //背景
    m_pBackImage = NULL;

    //图标
    m_pIconImage = NULL;
    m_rcIcon = CRect(0, 0, 0, 0);

    //标题
    m_strCaption;
    m_styleCaption;
    m_rcCaption = CRect(0, 0, 0, 0);

    //文字
    m_strText;
    m_styleText;
    m_rcText = CRect(0, 0, 0, 0);

    //按钮
    m_strButton;
    m_styleButton;
    m_rcButton = CRect(0, 0, 0, 0);

    //最大宽度和高度，-1为不限
    m_nMaxWidth = 400;
    m_nMaxHeight = -1;

    //指示点
    m_ptPos = CPoint(0, 0);

    //弹出位置偏好
    m_bLeftPreferred = TRUE;  //左偏好
    m_bTopPreferred = TRUE;   //上偏好

    m_lHorizMargin = 4;
    m_lVertiMargin = 4;
}

CSkinToolTipCtrl::~CSkinToolTipCtrl()
{
}

BOOL CSkinToolTipCtrl::Create(CWnd* pParent)
{
    LPCTSTR className = AfxRegisterWndClass(
        CS_DBLCLKS,
        AfxGetApp()->LoadCursor(IDC_ARROW), (HBRUSH)(COLOR_WINDOW ));

    HWND hParent = (pParent != NULL) ? (pParent->m_hWnd) : (::GetDesktopWindow());
    if (! CWnd::CreateEx(
        WS_EX_TOPMOST | WS_EX_TOOLWINDOW,
        className, 
        NULL, 
        WS_CHILD | WS_POPUP,
        0, 
        0, 
        0, 
        0,
        hParent,
        NULL))
        return FALSE;

    return TRUE;
}

CImageBase* CSkinToolTipCtrl::GetBackImage()
{
    return m_pBackImage;
}

void CSkinToolTipCtrl::SetBackImage(CImageBase* pImage)
{
    m_pBackImage = pImage;
}

CImageBase* CSkinToolTipCtrl::GetIconImage()
{
    return m_pIconImage;
}

void CSkinToolTipCtrl::SetIconImage(CImageBase* pImage)
{
    m_pIconImage = pImage;

    Layout();
}

CString CSkinToolTipCtrl::GetCaption()
{
    return m_strCaption;
}

void CSkinToolTipCtrl::SetCaption(LPCTSTR lpzCaption)
{
    m_strCaption = lpzCaption;

    Layout();
}

CString CSkinToolTipCtrl::GetText()
{
    return m_strText;
}

void CSkinToolTipCtrl::SetText(LPCTSTR lpzText)
{
    m_strText = lpzText;

    Layout();
}

CString CSkinToolTipCtrl::GetButtonText()
{
    return m_strButton;
}

void CSkinToolTipCtrl::SetButtonText(LPCTSTR lpzButton)
{
    m_strButton = lpzButton;

    Layout();
}

LONG CSkinToolTipCtrl::GetMaxWidth()
{
    return m_nMaxWidth;
}

void CSkinToolTipCtrl::SetMaxWidth(LONG lMaxWidth)
{
    m_nMaxWidth = lMaxWidth;
}

LONG CSkinToolTipCtrl::GetMaxHeight()
{
    return m_nMaxHeight;
}

void CSkinToolTipCtrl::SetMaxHeight(LONG lMaxHeight)
{
    m_nMaxHeight = lMaxHeight;
}

POINT CSkinToolTipCtrl::GetPos()
{
    return m_ptPos;
}

void CSkinToolTipCtrl::SetPos(POINT pt)
{
    m_ptPos = pt;
}

BOOL CSkinToolTipCtrl::GetLeftPreferred()
{
    return m_bLeftPreferred ? (TRUE) : (FALSE);
}

void CSkinToolTipCtrl::SetLeftPreferred(BOOL bLeft)
{
    m_bLeftPreferred = bLeft ? (TRUE) : (FALSE);
}

BOOL CSkinToolTipCtrl::GetTopPreferred()
{
    return m_bTopPreferred ? (TRUE) : (FALSE);
}

void CSkinToolTipCtrl::SetTopPreferred(BOOL bTop)
{
    m_bTopPreferred = bTop ? (TRUE) : (FALSE);
}

SIZE CSkinToolTipCtrl::Layout()
{
    CSize sz(0, 0);

    if (! IsWindow(m_hWnd))
        return sz;

    CWindowDC wndDC(this);

    LONG totalWidth = 0;
    LONG totalHeight = 0;

    CSize iconSize(0, 0);
    if (m_pIconImage != NULL) {
        iconSize.cx = m_pIconImage->Width();
        iconSize.cy = m_pIconImage->Height();
    }

    CSize captionSize(0, 0);
    m_styleCaption.GetTextSize(wndDC, m_strCaption, captionSize);

    CSize textSize(0, 0);
    m_styleText.GetTextSize(wndDC, m_strText, textSize);

    CSize buttonSize(0, 0);
    m_styleButton.GetTextSize(wndDC, m_strButton, buttonSize);

    LONG textWidth = max(captionSize.cx, textSize.cx);
    LONG totalSingleWidth = iconSize.cx + textWidth;
    LONG totalHorizMargin = m_lHorizMargin * 2;
    if (iconSize.cx >= 0)
        totalHorizMargin += m_lHorizMargin;
    totalSingleWidth += totalHorizMargin;
    if (iconSize.cx >= 0)
        totalSingleWidth += m_lHorizMargin;
    totalWidth = totalSingleWidth;
    if (m_nMaxWidth >= 0 && totalSingleWidth > m_nMaxWidth) {
        totalWidth = m_nMaxWidth;
    }

    LONG textMaxWidth = textWidth - (totalSingleWidth - totalWidth);

    LONG nLine = (textSize.cx + textMaxWidth - 1) / textMaxWidth;
    textSize.cy = textSize.cy * nLine;

    LONG captionAndTextHeight = captionSize.cy + textSize.cy;
    totalHeight = max(captionAndTextHeight, iconSize.cy) + buttonSize.cy;

    LONG totalVertiMargin = m_lVertiMargin * 2;
    if (iconSize.cy > 0 || captionSize.cy > 0 || textSize.cy > 0)
        totalVertiMargin += m_lVertiMargin;
    if (buttonSize.cy > 0)
        totalVertiMargin += m_lVertiMargin;

    totalHeight += totalVertiMargin;

    if (m_nMaxHeight >= 0 && totalHeight > m_nMaxHeight) {
        totalHeight = m_nMaxHeight;
    }

    sz.cx = totalWidth;
    sz.cy = totalHeight;

    m_rcIcon = CRect(0, 0, 0, 0);
    m_rcCaption = CRect(0, 0, 0, 0);
    m_rcText = CRect(0, 0, 0, 0);
    m_rcButton = CRect(0, 0, 0, 0);

    if (m_pIconImage != NULL) {
        m_rcIcon = CRect(0, 0, iconSize.cx, iconSize.cy);
        //m_rcIcon.OffsetRect();
    }

    return sz;
}

BEGIN_MESSAGE_MAP(CSkinToolTipCtrl, CWnd)
    ON_WM_PAINT()
    ON_WM_CREATE()
    ON_WM_LBUTTONDOWN()
    ON_WM_SIZE()
    ON_WM_TIMER()
    ON_WM_KILLFOCUS()
    ON_WM_NCCALCSIZE()
END_MESSAGE_MAP()



// CSkinToolTipCtrl message handlers

void CSkinToolTipCtrl::OnPaint()
{
    CPaintDC dc(this); // device context for painting
    // TODO: Add your message handler code here
    CRect rect;
    GetClientRect(&rect);
    if (m_pBackImage != NULL) {
        m_pBackImage->Draw(dc, &rect);
    }

    if (m_pIconImage != NULL) {
        m_pBackImage->Draw(dc, &m_rcIcon);
    }

    if (! m_strCaption.IsEmpty()) {
        m_styleCaption.DrawText(dc, m_strCaption, &m_rcCaption);
    }

    if (! m_strText.IsEmpty()) {
        m_styleText.DrawText(dc, m_strText, &m_rcText);
    }

    if (! m_strButton.IsEmpty()) {
        m_styleButton.DrawText(dc, m_strButton, &m_rcButton);
    }
    // Do not call CWnd::OnPaint() for painting messages
}

int CSkinToolTipCtrl::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
    if (CWnd::OnCreate(lpCreateStruct) == -1)
        return -1;

    // TODO:  Add your specialized creation code here
    Layout();

    return 0;
}

void CSkinToolTipCtrl::OnLButtonDown(UINT nFlags, CPoint point)
{
    // TODO: Add your message handler code here and/or call default
    if (! m_strButton.IsEmpty() && m_rcButton.PtInRect(point)) {
        //
    }

    CWnd::OnLButtonDown(nFlags, point);
}

void CSkinToolTipCtrl::OnSize(UINT nType, int cx, int cy)
{
    CWnd::OnSize(nType, cx, cy);

    // TODO: Add your message handler code here
}

void CSkinToolTipCtrl::OnTimer(UINT_PTR nIDEvent)
{
    // TODO: Add your message handler code here and/or call default

    CWnd::OnTimer(nIDEvent);
}

void CSkinToolTipCtrl::OnKillFocus(CWnd* pNewWnd)
{
    CWnd::OnKillFocus(pNewWnd);

    // TODO: Add your message handler code here
}

void CSkinToolTipCtrl::OnNcCalcSize(BOOL bCalcValidRects, NCCALCSIZE_PARAMS* lpncsp)
{
    // TODO: Add your message handler code here and/or call default
    if (bCalcValidRects) {
        CRect oldClientRect = lpncsp->rgrc[2];
        CRect oldWndRect = lpncsp->rgrc[1];
        CRect newClientRect = CRect(0, 0, m_szClient.cx, m_szClient.cy);
    } else {
        
    }

    CWnd::OnNcCalcSize(bCalcValidRects, lpncsp);
}
