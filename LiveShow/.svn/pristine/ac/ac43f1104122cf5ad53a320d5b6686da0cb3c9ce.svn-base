#include "stdafx.h"			// PCH
#include "TipCtrl.h"		// Class interface
#include "resource.h"
#include "../CommonLibrary/include/utility/SystemHelper.h"
#include "../CommonLibrary/include/ui/UIHelper.h"

#define CX_ROUNDED				12		// Tip horizontal roundness
#define CY_ROUNDED				10		// Tip vertical roundness
#define CX_LEADER				25		// Width of tip lead
#define CY_LEADER				25		// Height of tip lead
#define CX_ICON_MARGIN			5		// Width of margin between icon and tip text


#define DEFAULT_SHOW_DELAY		2000	// 当鼠标放到窗口上的时候，延迟显示的时间
#define DEFAULT_HIDE_DELAY		2000	// 当Tips显示后自动消失的延迟时间

CTipCtrl::CTipCtrl()
{
	//common::utility::systemhelper::SwitchResource sr(L"CommonUI");
	m_hIcon			= LoadIcon(AfxGetResourceHandle(), MAKEINTRESOURCE(IDI_TIPS));

	m_IconSize		= CSize(16, 16);
	m_bShowIcon		= TRUE;

	m_nShowDelay	= DEFAULT_SHOW_DELAY;
	m_nHideDelay	= DEFAULT_HIDE_DELAY;
	m_ptStartPos	= CPoint(0, 0);
	m_bLeft			= TRUE;

	m_clrBkg		= RGB(255,255,205);
	m_clrBorder		= RGB(149,148,144);
	m_clrText		= RGB(0,0,0);

    m_bClickClose   = TRUE;

	m_strButtonText	= "";
	m_pFont			= CFont::FromHandle((HFONT)::GetStockObject(DEFAULT_GUI_FONT));

	m_pAssociateWnd	= NULL;
}


CTipCtrl::~CTipCtrl()
{
}

BEGIN_MESSAGE_MAP(CTipCtrl, CWnd)
	ON_WM_PAINT()
	ON_WM_TIMER()
	ON_WM_DESTROY()
	ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()


void 
CTipCtrl::SetButtonText(CString strText)
{
	m_strButtonText = strText;
}

const CString 
CTipCtrl::GetButtonText()
{
	return m_strButtonText;
}

void 
CTipCtrl::SetLeft(BOOL bLeft)
{
	m_bLeft = bLeft;
	if (IsWindow(m_hWnd))
		RedrawWindow();
}


void 
CTipCtrl::SetIcon( HICON hIcon, CSize iconSize)
{
	m_hIcon = hIcon;
	m_IconSize = iconSize;

 	if (IsWindow(m_hWnd))
 		RedrawWindow();
}

void 
CTipCtrl::SetClickClose(BOOL bLeft)
{
    m_bClickClose = bLeft;
}

const BOOL 
CTipCtrl::GetClickClose()
{
    return m_bClickClose;
}

void	
CTipCtrl::SetAssociateWnd( CWnd* pAssociateWnd)
{
	m_pAssociateWnd = pAssociateWnd;
}

const CWnd*   
CTipCtrl::GetAssociateWnd()
{
	return m_pAssociateWnd;
}

BOOL 
CTipCtrl::Create(CWnd* pParentWnd) 
{
	BOOL	bSuccess;
	ASSERT(pParentWnd != NULL);

	LPCTSTR szClass		= AfxRegisterWndClass(0);
	bSuccess = CreateEx(NULL, szClass, NULL, WS_POPUP, 0, 0, 0, 0, pParentWnd->GetSafeHwnd(), NULL, NULL);
	return bSuccess;
}

void 
CTipCtrl::OnPaint() 
{
	CPaintDC dc( this ); 
	dc.SetBkMode( TRANSPARENT);

	CRect	rectTipWindow;
	GetClientRect(&rectTipWindow);

	HRGN	hRegion;
	GetWindowRegion(&dc, &hRegion);

	// 填充矩形区域
	CBrush bkgBrush( m_clrBkg);
	dc.FillRgn( CRgn::FromHandle(hRegion), &bkgBrush);

	// 绘制矩形区域
	CBrush borderBrush( m_clrBorder);
	dc.FrameRgn( CRgn::FromHandle(hRegion), &borderBrush, 1, 1);

	rectTipWindow.DeflateRect(CX_ROUNDED, CY_ROUNDED, 0, 0);
	if (m_bShowIcon && m_hIcon != NULL)
		rectTipWindow.left = rectTipWindow.left + m_IconSize.cx + CX_ICON_MARGIN;
	
	CFont* pSysFont = (CFont *)dc.SelectObject( m_pFont);
	dc.SetBkMode( TRANSPARENT );
	dc.DrawText(m_strText, &rectTipWindow, DT_TOP | DT_LEFT | DT_NOPREFIX);

	if( m_strButtonText != L"")
	{
		// 绘制文本
		LONG oldTextColor = dc.SetTextColor(RGB(0, 0, 255));
		CSize szButton = common::ui::GetTextExtent(dc.GetSafeHdc(), m_strButtonText, (HFONT)m_pFont->GetSafeHandle());
		m_rcButtonRect = CRect(0, 0, szButton.cx, szButton.cy);
		m_rcButtonRect.OffsetRect((rectTipWindow.Width() - m_rcButtonRect.Width() - 8), 
			(rectTipWindow.Height() - m_rcButtonRect.Height() - 18));
		dc.DrawText(m_strButtonText, m_rcButtonRect, DT_SINGLELINE | DT_LEFT | DT_VCENTER | DT_NOPREFIX);
		dc.SetTextColor(oldTextColor);

		//绘制下划线
		CPen linePen(PS_SOLID, 1, RGB(0, 0, 255));
		CPen* pOldPen = dc.SelectObject(&linePen);
		dc.MoveTo(CPoint(m_rcButtonRect.left, m_rcButtonRect.bottom));
		dc.LineTo(CPoint(m_rcButtonRect.right, m_rcButtonRect.bottom));
		dc.SelectObject(pOldPen);
	}

	if( m_bShowIcon)
	{
		if (m_hIcon != NULL)
			DrawIconEx(dc.m_hDC, CX_ROUNDED, CY_ROUNDED - 2, m_hIcon, m_IconSize.cx + 1, m_IconSize.cy + 1, 0, NULL, DI_NORMAL);
	}


	::DeleteObject(hRegion);
	dc.SelectObject(pSysFont);
	
}

BOOL 
CTipCtrl::GetWindowRegion(CDC* pDC, HRGN* hRegion, CSize *Size /* = NULL */)
{
	ASSERT(pDC != NULL);
	ASSERT(hRegion != NULL);

	CRect	rcWnd;
	CFont* pSysFont = (CFont *)pDC->SelectObject(m_pFont);
	pDC->DrawText(m_strText, &rcWnd, DT_CALCRECT);
	pDC->SelectObject(pSysFont);

	rcWnd.InflateRect(CX_ROUNDED, CY_ROUNDED);

	if ( m_bShowIcon && m_hIcon != NULL)
		rcWnd.right = rcWnd.right + m_IconSize.cx + CX_ICON_MARGIN;

	if (rcWnd.Height() < m_IconSize.cy)
		rcWnd.bottom = rcWnd.top + m_IconSize.cy;

	rcWnd.bottom += 4;

	if( m_strButtonText != "")
		rcWnd.bottom += 16;

	POINT	ptLeader[3];
	if( m_bLeft == TRUE)
	{
		ptLeader[0].x	= CX_ROUNDED;
		ptLeader[0].y	= rcWnd.Height() - CY_ROUNDED;

		ptLeader[1].x	= ptLeader[0].x;
		ptLeader[1].y	= ptLeader[0].y + CY_LEADER;

		ptLeader[2].x	= ptLeader[0].x + CX_LEADER;
		ptLeader[2].y	= rcWnd.Height() - CY_ROUNDED;
	}
	else
	{
		ptLeader[0].x	= rcWnd.Width() - CX_ROUNDED;
		ptLeader[0].y	= rcWnd.Height() - CY_ROUNDED;

		ptLeader[1].x	= ptLeader[0].x;
		ptLeader[1].y	= ptLeader[0].y + CY_LEADER;

		ptLeader[2].x	= ptLeader[0].x - CX_LEADER;
		ptLeader[2].y	= rcWnd.Height() - CY_ROUNDED;
	}


	CRgn CaptionRegion;
	CaptionRegion.CreateRoundRectRgn(0, 0, rcWnd.Width(), rcWnd.Height(), CX_ROUNDED, CY_ROUNDED);

	CRgn LeaderRegion;
	LeaderRegion.CreatePolygonRgn(ptLeader, 3, ALTERNATE);

	*hRegion =  ::CreateRectRgn(0, 0, rcWnd.Width(), rcWnd.Height() + CY_LEADER);
	CombineRgn(*hRegion, CaptionRegion.operator HRGN(), LeaderRegion.operator HRGN(), RGN_OR);

	CaptionRegion.DeleteObject();
	LeaderRegion.DeleteObject();

	if (Size != NULL)
	{
		Size->cx	= rcWnd.Width();
		Size->cy	= rcWnd.Height() + CY_LEADER;
	}
	
	return TRUE;
}

void 
CTipCtrl::OnTimer( UINT nIDEvent ) 
{
	switch (nIDEvent)
	{

	case timerShow:
		{
			KillTimer(m_nTimer);

			CDC* pDC = GetDC();
			CSize	WindowSize;
			HRGN	hRegion;
			GetWindowRegion(pDC, &hRegion, &WindowSize);
			ReleaseDC(pDC);
			::SetWindowRgn(m_hWnd, hRegion, TRUE);

			if( m_bLeft == FALSE)
			{
				SetWindowPos(&wndTop, m_ptStartPos.x - ( WindowSize.cx - CX_ROUNDED), 
					m_ptStartPos.y - ( WindowSize.cy - CY_ROUNDED), WindowSize.cx, WindowSize.cy, 
					SWP_NOACTIVATE | SWP_SHOWWINDOW);
			}
			else
			{
				SetWindowPos(&wndTop, m_ptStartPos.x -
					CX_ROUNDED, 
					m_ptStartPos.y - WindowSize.cy + CY_ROUNDED, WindowSize.cx, WindowSize.cy, 
					SWP_NOACTIVATE | SWP_SHOWWINDOW);
			}

            if( m_nHideDelay >= 0)
			    m_nTimer = SetTimer(timerHide, m_nHideDelay, NULL);

			break;
		}


	case timerHide:
		{
			KillTimer(m_nTimer);
			ShowWindow(SW_HIDE);
			break;
		}
	}

	CWnd::OnTimer(nIDEvent);
	
}

void 
CTipCtrl::OnDestroy() 
{
	KillTimer(m_nTimer);
	CWnd::OnDestroy();
}

void 
CTipCtrl::OnLButtonDown(UINT nFlags, CPoint point)
{
    if (!GetButtonText().IsEmpty() && m_rcButtonRect.PtInRect(point))
        ShowWindow(SW_HIDE);
    if (m_bClickClose)
	    ShowWindow( SW_HIDE);
}