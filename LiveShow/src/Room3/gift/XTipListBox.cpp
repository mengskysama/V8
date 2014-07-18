// XTipListBox.cpp  Version 1.0
//
// Author:  Hans Dietrich
//          hdietrich2@hotmail.com
//
// Description:
//     XTipListBox.cpp implements CXTipListBox, a class to display
//     tooltips in a listbox.
//
// History
//     Version 1.0 - 2003 June 30
//     - Initial public release
//
// This software is released into the public domain.  You are free to use it 
// in any way you like.
//
// This software is provided "as is" with no expressed or implied warranty.  
// I accept no liability for any damage or loss of business that this software 
// may cause.
//
///////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "XTipListBox.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

///////////////////////////////////////////////////////////////////////////////
// CXTipListBox

BEGIN_MESSAGE_MAP(CXTipListBox, CListBox)
	//{{AFX_MSG_MAP(CXTipListBox)
	ON_WM_MOUSEMOVE()
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

///////////////////////////////////////////////////////////////////////////////
// ctor
CXTipListBox::CXTipListBox()
{
	m_hWndToolTip = NULL;
}

///////////////////////////////////////////////////////////////////////////////
// PreSubclassWindow
void CXTipListBox::PreSubclassWindow() 
{
	TRACE(_T("in CXTipListBox::PreSubclassWindow\n"));

	CListBox::PreSubclassWindow();

	// create tooltip
	m_hWndToolTip = ::CreateWindowEx(WS_EX_TOPMOST,
							  TOOLTIPS_CLASS,
							  NULL,
							  TTS_NOPREFIX | TTS_ALWAYSTIP,
							  CW_USEDEFAULT,
							  CW_USEDEFAULT,
							  CW_USEDEFAULT,
							  CW_USEDEFAULT,
							  m_hWnd,
							  NULL,
							  NULL,
							  NULL);
	ASSERT(m_hWndToolTip);

	// initialize toolinfo struct
	memset(&m_ToolInfo, 0, sizeof(m_ToolInfo));
	m_ToolInfo.cbSize = sizeof(m_ToolInfo);
	m_ToolInfo.uFlags = TTF_TRACK | TTF_TRANSPARENT;
	m_ToolInfo.hwnd = m_hWnd;

	// add list box
	::SendMessage(m_hWndToolTip, TTM_SETMAXTIPWIDTH, 0, SHRT_MAX);
	::SendMessage(m_hWndToolTip, TTM_ADDTOOL, 0, (LPARAM) (LPTOOLINFO) &m_ToolInfo);
	::SendMessage(m_hWndToolTip, TTM_SETTIPBKCOLOR, ::GetSysColor(COLOR_HIGHLIGHT), 0);
	::SendMessage(m_hWndToolTip, TTM_SETTIPTEXTCOLOR, ::GetSysColor(COLOR_HIGHLIGHTTEXT), 0);

	// reduce top & bottom margins
	CRect rectMargins(0,-1,0,-1);
	::SendMessage(m_hWndToolTip, TTM_SETMARGIN, 0, (LPARAM)&rectMargins);

	// set font
	CFont *pFont = GetFont();
	::SendMessage(m_hWndToolTip, WM_SETFONT, (WPARAM)(HFONT)*pFont, FALSE);

	// remove border (listbox items only)
	LONG lStyle = ::GetWindowLong(m_hWndToolTip, GWL_STYLE);
	lStyle &= ~WS_BORDER;
	::SetWindowLong(m_hWndToolTip, GWL_STYLE, lStyle);
}

///////////////////////////////////////////////////////////////////////////////
// OnMouseMove
void CXTipListBox::OnMouseMove(UINT nFlags, CPoint point) 
{
	CRect rectClient;
	GetClientRect(&rectClient);

	if (rectClient.PtInRect(point))
	{
		CPoint pointScreen;
		::GetCursorPos(&pointScreen);
		BOOL bOutside = FALSE;
		int nItem = ItemFromPoint(point, bOutside);  // calculate listbox item number (if any)

		if (!bOutside && (nItem >= 0))
		{
			CString strText = _T("");
			GetText(nItem, strText);
			m_ToolInfo.lpszText = (LPTSTR)(LPCTSTR)strText;

			CRect rect;
			GetItemRect(nItem, &rect);
			ClientToScreen(&rect);

			HDC hDC = ::GetDC(m_hWnd);
			ASSERT(hDC);

			CFont *pFont = GetFont();
			HFONT hOldFont = (HFONT) ::SelectObject(hDC, (HFONT) *pFont);

			SIZE size;
			::GetTextExtentPoint32(hDC, strText, strText.GetLength(), &size);
			::SelectObject(hDC, hOldFont);
			::ReleaseDC(m_hWnd, hDC);

			//TRACE(_T("<%s>  cx=%d  width=%d\n"), strText, size.cx, rect.Width());
			if (size.cx > (rect.Width() - 3))
			{
				::SendMessage(m_hWndToolTip, TTM_UPDATETIPTEXT, 0, (LPARAM) &m_ToolInfo);
				::SendMessage(m_hWndToolTip, TTM_TRACKPOSITION, 0, 
					(LPARAM) MAKELONG(rect.left, rect.top));
				::SendMessage(m_hWndToolTip, TTM_TRACKACTIVATE, TRUE, (LPARAM)(LPTOOLINFO) &m_ToolInfo);
				SetTimer(1, 80, NULL);	// set timer for out-of-rect detection
			}
			else
			{
				::SendMessage(m_hWndToolTip, TTM_TRACKACTIVATE, FALSE, (LPARAM)(LPTOOLINFO) &m_ToolInfo);
			}
		}
	}
	else
	{
		::SendMessage(m_hWndToolTip, TTM_TRACKACTIVATE, FALSE, (LPARAM)(LPTOOLINFO) &m_ToolInfo);
	}
	
	CListBox::OnMouseMove(nFlags, point);
}

///////////////////////////////////////////////////////////////////////////////
// OnTimer
void CXTipListBox::OnTimer(UINT nIDEvent) 
{
	CPoint point;
	::GetCursorPos(&point);
	ScreenToClient(&point);

	CRect rectClient;
	GetClientRect(&rectClient);

	DWORD dwStyle = GetStyle();
	if ((!rectClient.PtInRect(point)) || ((dwStyle & WS_VISIBLE) == 0))
	{
		TRACE(_T("not in listbox =====\n"));
		KillTimer(nIDEvent);
		::SendMessage(m_hWndToolTip, TTM_TRACKACTIVATE, FALSE, (LPARAM)(LPTOOLINFO) &m_ToolInfo);
	}
}

