#include "StdAfx.h"
#include "TipsButton.h"
#include "179TipCtrl.h"

CTipsButton::CTipsButton(void)
{
	m_strTip = L"";
	m_pTipCtrl = NULL;
}

CTipsButton::~CTipsButton(void)
{
	if (m_pTipCtrl != NULL)
	{
		m_pTipCtrl->DestroyWindow();
		delete m_pTipCtrl;
		m_pTipCtrl =  NULL;
	}
}

BEGIN_MESSAGE_MAP(CTipsButton, CButton)
	ON_WM_MOUSEMOVE()
	ON_MESSAGE(WM_MOUSEHOVER, OnMouseHover)
	ON_MESSAGE(WM_MOUSELEAVE, OnMouseLeave)
	ON_WM_CREATE()
END_MESSAGE_MAP()

void CTipsButton::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	TRACKMOUSEEVENT tme;
	tme.cbSize = sizeof(tme);
	tme.hwndTrack = m_hWnd;
	tme.dwFlags = TME_LEAVE | TME_HOVER;
	tme.dwHoverTime = 1;
    _TrackMouseEvent(&tme);

	CButton::OnMouseMove(nFlags, point);
}

HRESULT CTipsButton::OnMouseHover(WPARAM w, LPARAM l)
{
	if (m_strTip != L"")
	{
		CRect rect(0,0,0,0);
        GetWindowRect(&rect);
		CPoint pt(rect.left,rect.bottom);
		if (m_pTipCtrl != NULL)
		{
			m_pTipCtrl->SetTipText(m_strTip);
			int nWidth,nHeight;
			m_pTipCtrl->ShowTip(5,nWidth,nHeight);
			m_pTipCtrl->MoveWindow(pt.x,pt.y,nWidth,nHeight,TRUE);
			m_pTipCtrl->ShowWindow(SW_SHOWNOACTIVATE);
		}
	}
	
	return TRUE;
}

HRESULT CTipsButton::OnMouseLeave(WPARAM w, LPARAM l)
{
	if (m_pTipCtrl!=NULL && m_pTipCtrl->GetSafeHwnd() && m_pTipCtrl->IsWindowVisible())
	{
		m_pTipCtrl->ShowWindow(SW_HIDE);
		//m_pTipCtrl->SetWindowPos(NULL,0,0,0,0,SWP_HIDEWINDOW|SWP_NOACTIVATE);
	}

	return 0;
}

void CTipsButton::SetBtnTip(CString strTip)
{
	if (m_pTipCtrl == NULL)
	{
		m_pTipCtrl = new C179TipCtrl(eArrowPos::ARROW_TOP_LEFT);
		m_pTipCtrl->CreateEx(this,0);
	}

	m_strTip = strTip;
}

void CTipsButton::HideBtnTip()
{
	if (m_pTipCtrl && m_pTipCtrl->GetSafeHwnd() && m_pTipCtrl->IsWindowVisible())
	{
		m_pTipCtrl->ShowWindow(SW_HIDE);
	}
}