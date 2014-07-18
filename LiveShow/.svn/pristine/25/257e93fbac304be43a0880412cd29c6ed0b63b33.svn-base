// SearchEdit.cpp : implementation file
//

#include "stdafx.h"

#include "SearchEdit.h"

// CSearchEdit
#define DEFAULT_TEXT _T("房间号/房间名(至少输入5个字符)")

IMPLEMENT_DYNAMIC(CSearchEdit, CEdit)

CSearchEdit::CSearchEdit()
{
	m_hToolTip = NULL;
	m_bNotEnChange = FALSE;
	m_bFakeLoseFocus = FALSE;
	m_bLeave = TRUE;
	m_bTipShow = FALSE;
}

CSearchEdit::~CSearchEdit()
{
	if (m_hToolTip){
		::DestroyWindow(m_hToolTip);
		m_hToolTip = NULL;
	}
}


BEGIN_MESSAGE_MAP(CSearchEdit, CEdit)
	ON_WM_KEYDOWN()
	ON_WM_SETFOCUS()
	ON_WM_KILLFOCUS()
	ON_CONTROL_REFLECT(EN_CHANGE, &CSearchEdit::OnEnChange)
	ON_WM_MOUSEWHEEL()
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_CREATE()
	ON_MESSAGE(WM_MOUSELEAVE, OnMouseLeave)
	ON_MESSAGE(WM_MOUSEHOVER, OnMouseHover)
END_MESSAGE_MAP()



// CSearchEdit message handlers


LRESULT CSearchEdit::OnMouseLeave(WPARAM wParam,LPARAM lParam)
{
	PopToolTip();
	m_bLeave = true;
	return 0;
}

LRESULT CSearchEdit::OnMouseHover(WPARAM wParam,LPARAM lParam)
{
	
	return 0;
}	




void CSearchEdit::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	m_bNotEnChange = FALSE;

	CEdit::OnKeyDown(nChar, nRepCnt, nFlags);
}



void CSearchEdit::OnSetFocus(CWnd* pOldWnd)
{	
	TRACE(" CSearchEdit::OnSetFocus\n");
	CEdit::OnSetFocus(pOldWnd);
	

	CString strText;
	GetWindowText(strText);
	if (strText == DEFAULT_TEXT)
	{
		SetWindowText(_T(""));
		//this->HideCaret();
	}
	else
	{
		if (!m_bFakeLoseFocus)
		{
			SetSel(0, -1);

			CString strText;
			GetWindowText(strText);
			int iFind = strText.ReverseFind(')') ;
			if (iFind == (strText.GetLength()-1))
			{
				return;
			}

		
		}
	}
}

void CSearchEdit::OnKillFocus(CWnd* pNewWnd)
{
	TRACE(" CSearchEdit::OnKillFocus\n");
	CEdit::OnKillFocus(pNewWnd);
	m_bFakeLoseFocus = TRUE;

	CString strText;
	GetWindowText(strText);
	if (strText.IsEmpty())
	{
		SetWindowText(DEFAULT_TEXT);
	}


}



void CSearchEdit::OnEnChange()
{
	if (m_bNotEnChange)
	{
		m_bNotEnChange = false;
		return;
	}

	CString strText;
	GetWindowText(strText);
	if (strText == DEFAULT_TEXT)
	{
		return;
	}

}

BOOL CSearchEdit::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg && pMsg->message == WM_KEYDOWN)
	{
		if (pMsg->wParam == 0x0d)
		{
			m_bNotEnChange = TRUE;
			SendBySel();
		}
	}
	return CEdit::PreTranslateMessage(pMsg);
}



BOOL CSearchEdit::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	
	return CEdit::OnMouseWheel(nFlags, zDelta, pt);
}



void CSearchEdit::SendBySel()
{

}

void CSearchEdit::OnOpenDialog(uint32 uUIN)
{

}

void CSearchEdit::OnLButtonDown(UINT nFlags, CPoint point)
{
	TRACE(" CSearchEdit::OnLButtonDown\n");
	if (GetFocus() != this || m_bFakeLoseFocus)
	{
		m_bFakeLoseFocus = FALSE;
		TRACE(" CSearchEdit::设置焦点\n");
		SetFocus();
		SetSel(0, -1);
		CString strText;
		GetWindowText(strText);
		if (strText == DEFAULT_TEXT)
		{
			SetWindowText(_T(""));
			this->ShowCaret();
		}
	}
	else
	{
		CEdit::OnLButtonDown(nFlags, point);
	}
}


BOOL CSearchEdit::IsSbExist(uin _uin)
{

	return FALSE;
}




void CSearchEdit::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	//用于检测mouse leave hover
	TRACKMOUSEEVENT tme;
	tme.cbSize = sizeof(tme);
	tme.dwFlags  = TME_HOVER | TME_LEAVE ;
	tme.hwndTrack = m_hWnd;
	tme.dwHoverTime = HOVER_DEFAULT;
	::_TrackMouseEvent(&tme);
	//end


	if (!m_bTipShow)
	{
		CPoint   pt;   
		GetCursorPos(&pt);		
		ShowToolTip(DEFAULT_TEXT);
	}

	CEdit::OnMouseMove(nFlags, point);
}

int CSearchEdit::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CEdit::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  在此添加您专用的创建代码
	

	return 0;
}

void CSearchEdit::ShowToolTip(CString pszTooltip)
{
	TOOLINFO	ti;
	LRESULT		bRes;

	if(m_hToolTip == NULL)
	{
		CRect rc;
		INITCOMMONCONTROLSEX iccex;

		::GetClientRect(this->m_hWnd,&rc);
		iccex.dwICC = ICC_WIN95_CLASSES;
		iccex.dwSize = sizeof(INITCOMMONCONTROLSEX);
		bRes = InitCommonControlsEx(&iccex);

		m_hToolTip = CreateWindowEx(WS_EX_TOPMOST, TOOLTIPS_CLASS,
			NULL,WS_POPUP | TTS_NOPREFIX | TTS_ALWAYSTIP,
			CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, NULL,
			NULL, NULL, NULL);

		::SetWindowPos(m_hToolTip, HWND_TOPMOST, 0, 0, 0, 0,
			SWP_NOACTIVATE | SWP_NOMOVE | SWP_NOSIZE);

		memset(&ti, 0, sizeof(TOOLINFO));
		ti.cbSize = sizeof(TOOLINFO);
		ti.uFlags = TTF_SUBCLASS;
		ti.hwnd   = this->m_hWnd;
		ti.uId    = 1;
		ti.hinst  = NULL;
		ti.lpszText = _T("");
		ti.rect.left   = rc.left;
		ti.rect.right  = rc.right;
		ti.rect.top    = rc.top;
		ti.rect.bottom = rc.bottom;
		bRes = ::SendMessage(m_hToolTip, TTM_ADDTOOL, 0, (LPARAM)&ti);
	}

	CRect rc;
	::GetClientRect(this->m_hWnd, &rc);
	memset(&ti,0,sizeof(TOOLINFO));
	ti.cbSize = sizeof(TOOLINFO);
	ti.uFlags = TTF_SUBCLASS;
	ti.hwnd   = this->m_hWnd;
	ti.uId    = 1;
	ti.hinst  = NULL;
	ti.lpszText = _T("");
	ti.rect.left   = rc.left;
	ti.rect.right  = rc.right;
	ti.rect.top    = rc.top;
	ti.rect.bottom = rc.bottom;
	bRes = ::SendMessage(m_hToolTip, TTM_SETTOOLINFO, 0, (LPARAM)&ti);	

	::SendMessage(m_hToolTip, TTM_ACTIVATE, TRUE, 0);

	memset(&ti, 0, sizeof(TOOLINFO));
	ti.cbSize = sizeof(TOOLINFO);
	ti.hwnd   = this->m_hWnd;
	ti.uId    = 1;

	LPTSTR lpzTipText = const_cast<LPTSTR>((LPCTSTR)pszTooltip);
	ti.lpszText = lpzTipText;
	::SendMessage(m_hToolTip, TTM_UPDATETIPTEXT, 0, (LPARAM)&ti);
	m_bTipShow = true;
}

void CSearchEdit::PopToolTip()
{
	::SendMessage(m_hToolTip, TTM_ACTIVATE, FALSE, 0);
	m_bTipShow = false;
}
