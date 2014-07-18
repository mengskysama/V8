#include "stdafx.h"			// PCH
#include "resource.h"
#include "../../include/ui/SimpleTipCtrl.h"		// Class interface
#include "../../include/ui/UIHelper.h"		// Class interface
#include <string>
#include "../../include/utility/StringHelper.h"		// Class interface
#include "utility/SystemHelper.h"

using namespace std;

namespace common { namespace ui
{


#define DEFAULT_SHOW_DELAY		2000	// 当鼠标放到窗口上的时候，延迟显示的时间
#define DEFAULT_HIDE_DELAY		3000	// 当Tips显示后自动消失的延迟时间

CSimpleTipCtrl::CSimpleTipCtrl()
{
	m_nShowDelay	= DEFAULT_SHOW_DELAY;
	m_nHideDelay	= DEFAULT_HIDE_DELAY;
	m_ptStartPos	= CPoint(0, 0);

	m_clrBkg		= RGB(255,255,235);
	m_clrBorder		= RGB(0,0,0);
	m_clrText		= RGB(0,0,0);

	m_bHide			= TRUE;

	memset( &m_stLogFont, 0x0, sizeof( LOGFONT));

	m_stLogFont.lfUnderline = (BYTE)FALSE;
	m_stLogFont.lfHeight = 12;
	m_stLogFont.lfWeight = 400;
	_tcscpy(m_stLogFont.lfFaceName, L"宋体");

	m_nWidth	=	60;			// 默认的限定长度是60
	m_bFitted	=	FALSE;		// 默认是自适应长度，即文字多长，tips的长度就多长
}

CSimpleTipCtrl::~CSimpleTipCtrl()
{
}


BEGIN_MESSAGE_MAP(CSimpleTipCtrl, CWnd)
	ON_WM_PAINT()
	ON_WM_TIMER()
	ON_WM_DESTROY()
	ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()

void		
CSimpleTipCtrl::SetBkgColor( COLORREF clrBkg)
{
	m_clrBkg = clrBkg;
	if (IsWindow(m_hWnd))
		RedrawWindow();
}

const COLORREF	
CSimpleTipCtrl::GetBkgColor()
{
	return m_clrBkg;
}

void		
CSimpleTipCtrl::SetBorderColor( COLORREF clrBorder)
{
	m_clrBorder = clrBorder;
	if (IsWindow(m_hWnd))
		RedrawWindow();
}

const COLORREF	
CSimpleTipCtrl::GetBorderColor()
{
	return m_clrBorder;
}

void		
CSimpleTipCtrl::SetTextColor( COLORREF clrText)
{
	m_clrText = clrText;
	if (IsWindow(m_hWnd))
		RedrawWindow();
}

const COLORREF	
CSimpleTipCtrl::GetTextColor()
{
	return m_clrText;
}

void		
CSimpleTipCtrl::SetTipsText( CString strText)
{
	m_strText = strText;
	if (IsWindow(m_hWnd))
		RedrawWindow();
}

const CString	
CSimpleTipCtrl::GetTipsText()
{
	return m_strText;
}

void	
CSimpleTipCtrl::SetFont( HFONT hFont)
{	
	::GetObject( hFont, sizeof(LOGFONT), &m_stLogFont);
}

const LOGFONT	
CSimpleTipCtrl::GetFont()
{
	return m_stLogFont;
}

BOOL 
CSimpleTipCtrl::Create(CWnd* pParentWnd) 
{
	BOOL	bSuccess;
	
	if( pParentWnd == NULL)
	{
		LPCTSTR szClass		= AfxRegisterWndClass(0);
		bSuccess = CreateEx( WS_EX_TOPMOST | WS_EX_TOOLWINDOW, szClass, NULL, WS_POPUP | WS_CLIPCHILDREN , 0, 0, 
			0, 0, NULL, NULL, NULL);
	}
	else
	{
		LPCTSTR szClass		= AfxRegisterWndClass(0);
		bSuccess = CreateEx( WS_EX_TOPMOST | WS_EX_TOOLWINDOW, szClass, NULL, WS_POPUP | WS_CLIPCHILDREN , 0, 0, 
			0, 0, pParentWnd->GetSafeHwnd(), NULL, NULL);
	}

	static CBrush brush(RGB(255,255,255));
	SetClassLong(this->m_hWnd,GCL_HBRBACKGROUND,(LONG)(HBRUSH)brush);

	return bSuccess;
}

void 
CSimpleTipCtrl::Show(CString szText, CWnd* pParent, CPoint *pt /*= NULL*/, UINT nWidth /* = 60*/)
{
	if (pt != NULL)
		m_ptStartPos = *pt;
	else
		GetCursorPos(&m_ptStartPos);

	if( GetSafeHwnd() != NULL || Create( NULL))
	{
		m_strText = szText;
		m_nTimer = SetTimer(timerShow, 400, NULL);
	}

	m_nWidth	=	nWidth;
	if( m_nWidth < 10)
		m_nWidth = 10;
}

BOOL	
CSimpleTipCtrl::BeHide()
{
	return m_bHide;
}

void
CSimpleTipCtrl::Hide()
{
	if( GetSafeHwnd() != NULL)
	{
		ShowWindow( SW_HIDE);
		KillTimer( timerShow);
	}
}

void 
CSimpleTipCtrl::OnPaint() 
{
	CRect rectCtrl;
	GetWindowRect( &rectCtrl);

	CPaintDC dc( this ); 
	dc.SetBkMode( TRANSPARENT);

	CDC MemDC;
	CBitmap MemBitmap;

	MemDC.CreateCompatibleDC (NULL);
	MemBitmap.CreateCompatibleBitmap(&dc,rectCtrl.Width(), rectCtrl.Height());

	/*CBitmap *pOldBit = */MemDC.SelectObject(&MemBitmap);
	MemDC.FillSolidRect(0,0, rectCtrl.Width(), rectCtrl.Height(),RGB(255,255,255));

	CSize windowSize;
	HRGN	hRegion;
	GetWindowRegion(&MemDC, &hRegion, &windowSize);

	// 填充矩形区域
	CBrush bkgBrush( m_clrBkg);
	MemDC.FillRgn( CRgn::FromHandle(hRegion), &bkgBrush);

	// 绘制矩形区域
	CBrush borderBrush( m_clrBorder);
	MemDC.FrameRgn( CRgn::FromHandle(hRegion), &borderBrush, 1, 1);

	CFont font;
	font.CreateFontIndirect(&m_stLogFont);

	m_strText = common::utility::stringhelper::TrimCopy(m_strText.GetBuffer(0)).c_str();

	CSize textSize = common::ui::GetTextExtent( MemDC.GetSafeHdc(), 
		m_strText, (HFONT)font.GetSafeHandle());

	// 对给定的字符进行分段处理
	wstring wstrDestString = L"";
	CRect	rectTipWindow;
	GetClientRect(&rectTipWindow);

	if( m_bFitted == TRUE)
	{
		if( textSize.cx > m_nWidth)
		{
			UINT nEndIndex  = 0;
			UINT nBeginIndex = 0;

			UINT nContentLength =m_strText.GetLength();
			while( nEndIndex <= nContentLength )
			{
				wstring wstrText = GetFittableString( MemDC.GetSafeHdc(), m_strText.GetBuffer(0),
					m_nWidth, nBeginIndex, (HFONT)font.GetSafeHandle(), (LONG*)&nEndIndex);
				wstrDestString += wstrText + wstring(L"\n");

				nBeginIndex = nEndIndex;
			}
			wstrDestString.erase( wstrDestString.length() - 1); 
		}
		else
		{
			wstrDestString = m_strText.GetBuffer(0);
		}
		common:utility::stringhelper::Trim(wstrDestString);

		UINT nRectHeight = ( textSize.cx%m_nWidth == 0 ? textSize.cx/m_nWidth : textSize.cx/m_nWidth + 1) * textSize.cy;

		rectTipWindow.left = 4;
		
		if( textSize.cx <= m_nWidth)
			rectTipWindow.right = rectTipWindow.left + m_nWidth;
		else
			rectTipWindow.right = rectTipWindow.left + textSize.cx;

		rectTipWindow.top = 4;
		rectTipWindow.bottom = rectTipWindow.top + nRectHeight;
	}
	else
	{
		wstrDestString = m_strText;
		rectTipWindow.left = 4;
		rectTipWindow.right = rectTipWindow.left + textSize.cx;
		rectTipWindow.top = 4;
		rectTipWindow.bottom = rectTipWindow.top + textSize.cy;
	}


	CFont* pOldFont = MemDC.SelectObject( &font);
	MemDC.SetBkMode( TRANSPARENT );
	MemDC.DrawText(wstrDestString.c_str(), &rectTipWindow, DT_VCENTER | DT_LEFT | DT_NOPREFIX);

	dc.BitBlt(0,0, rectCtrl.Width(), rectCtrl.Height(),&MemDC,0,0,SRCCOPY);

	//绘图完成后的清理
	MemDC.SelectObject( pOldFont);
	MemBitmap.DeleteObject();
	MemDC.DeleteDC();

	::DeleteObject(hRegion);
}

BOOL 
CSimpleTipCtrl::GetWindowRegion(CDC* pDC, HRGN* hRegion, CSize *Size /* = NULL */)
{
	ASSERT(pDC != NULL);
	ASSERT(hRegion != NULL);

	CFont font;
	font.CreateFontIndirect(&m_stLogFont);

	m_strText = common::utility::stringhelper::TrimCopy(m_strText.GetBuffer(0)).c_str();

	CSize textSize = common::ui::GetTextExtent( pDC->GetSafeHdc(), 
		m_strText, (HFONT)font.GetSafeHandle());
	textSize.cx += 8;

	if( m_bFitted == TRUE)
	{
		// 对给定的字符进行分段处理
		wstring wstrDestString = L"";
		if( textSize.cx > m_nWidth)
		{
			UINT nEndIndex  = 0;
			UINT nBeginIndex = 0;

			UINT nContentLength =m_strText.GetLength();
			while( nEndIndex <= nContentLength )
			{
				wstring wstrText = GetFittableString( pDC->GetSafeHdc(), m_strText.GetBuffer(0),
					m_nWidth, nBeginIndex, (HFONT)font.GetSafeHandle(), (LONG*)&nEndIndex);
				wstrDestString += wstrText + wstring(L"\n");

				nBeginIndex = nEndIndex;
			}
			wstrDestString.erase( wstrDestString.length() - 1); 
		}
		else
		{
			wstrDestString = m_strText.GetBuffer(0);
		}
		common::utility::stringhelper::Trim(wstrDestString);

		textSize.cy = ( textSize.cx%m_nWidth == 0 ? textSize.cx/m_nWidth : textSize.cx/m_nWidth + 1) * textSize.cy;

		if( textSize.cx < m_nWidth)
			textSize.cx = 4 + textSize.cx;
		else
			textSize.cx = 4 + m_nWidth;
	}
	
	textSize.cy += 8;

	*hRegion =  ::CreateRectRgn(0, 0,textSize.cx , textSize.cy);

	if (Size != NULL)
	{
		Size->cx	= textSize.cx;
		Size->cy	= textSize.cy;
	}

	return TRUE;
}

void 
CSimpleTipCtrl::OnTimer( UINT nIDEvent ) 
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
			::SetWindowRgn(m_hWnd, hRegion, TRUE);

			ShowWindow( SW_HIDE);

			SetWindowPos(&wndTop, 
				m_ptStartPos.x,
				m_ptStartPos.y, 
				WindowSize.cx, 
				WindowSize.cy, 
				SWP_NOACTIVATE | SWP_SHOWWINDOW);

			ReleaseDC(pDC);
			DeleteObject( hRegion);		

			m_bHide = FALSE;

            if( m_nHideDelay >= 0)
			    m_nTimer = SetTimer(timerHide, m_nHideDelay, NULL);

			break;
		}


	case timerHide:
		{
			KillTimer(m_nTimer);
			ShowWindow(SW_HIDE);
			m_bHide = TRUE;
			break;
		}
	}

	CWnd::OnTimer(nIDEvent);
}

void 
CSimpleTipCtrl::OnDestroy() 
{
	KillTimer(m_nTimer);
	CWnd::OnDestroy();
}

void 
CSimpleTipCtrl::OnLButtonDown(UINT nFlags, CPoint point)
{
    if (m_rcButtonRect.PtInRect(point))
        ShowWindow(SW_HIDE);
}

}}