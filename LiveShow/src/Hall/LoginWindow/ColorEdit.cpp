// ColorEdit.cpp : 实现文件
//

#include "stdafx.h"
#include "ColorEdit.h"


#include "../CommonLibrary/include/ui/UIHelper.h"

using namespace common;

#define  DEFAULT_TEXT_COLOR              RGB(0, 0, 0)                 //黑色
#define  DEFAULT_BACKGND_COLOR           RGB(255, 255, 255)           //白色
#define  DEFAULT_DISABLE_BACKGND_COLOR           RGB(192, 192, 192)           //白色

// CColorEdit

IMPLEMENT_DYNAMIC(CColorEdit, CEdit)
CColorEdit::CColorEdit()
           :m_crText(DEFAULT_TEXT_COLOR),
		    m_crBackGnd(DEFAULT_BACKGND_COLOR),
			m_crDisableBackGnd(DEFAULT_DISABLE_BACKGND_COLOR)
{
	m_crBorder = RGB(92,132,255);
}

CColorEdit::~CColorEdit()
{
	m_brBackGnd.DeleteObject();
}

void CColorEdit::SetBackColor(COLORREF rgb)
{
	m_crBackGnd = rgb;
	Invalidate(TRUE);
}

void CColorEdit::SetDisableBackColor( COLORREF rgb)
{
	m_crDisableBackGnd	=	rgb;
	Invalidate(TRUE);
}

void CColorEdit::SetTextColor(COLORREF rgb)
{
	m_crText = rgb;
	Invalidate(TRUE);
}


BEGIN_MESSAGE_MAP(CColorEdit, CEdit)
	ON_WM_CTLCOLOR_REFLECT()
	ON_WM_CREATE()
	ON_WM_NCPAINT()
END_MESSAGE_MAP()

HBRUSH CColorEdit::CtlColor(CDC* pDC, UINT nCtlColor)
{
	// TODO:  在此更改 DC 的任何属性
	pDC->SetTextColor(m_crText);

	if (m_brBackGnd.GetSafeHandle())
	{
		m_brBackGnd.DeleteObject();
	}

	if( (GetStyle() & WS_DISABLED ) == WS_DISABLED)
	{
		pDC->SetBkColor(m_crDisableBackGnd);
		m_brBackGnd.CreateSolidBrush(m_crDisableBackGnd);
	}
	else
	{
		pDC->SetBkColor(m_crBackGnd);
		m_brBackGnd.CreateSolidBrush(m_crBackGnd);
	}

	return m_brBackGnd;
}

void CColorEdit::OnPaint()
{	
	CRect rcThis;
	GetClientRect(rcThis);
	CDC* pdc = GetDC();
	pdc->FillSolidRect(rcThis, 0xFF0000);
	ReleaseDC(pdc);

	CEdit::OnPaint();
}

int CColorEdit::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CEdit::OnCreate(lpCreateStruct) == -1)
		return -1;

	return 0;
}

void CColorEdit::PreSubclassWindow()
{
	CEdit::PreSubclassWindow();
}

void CColorEdit::OnNcPaint()
{
	CDC* pdcParent = GetParent()->GetDC();
	CRect rcThis;
	GetWindowRect(rcThis);
	GetParent()->ScreenToClient(rcThis);
	pdcParent->Draw3dRect(rcThis, m_crBorder, m_crBorder);
	ReleaseDC(pdcParent);
}

BOOL CColorEdit::OnEraseBkgnd(CDC* pDC)
{
	CDC* pdcParent = GetParent()->GetDC();

	ui::CClientRect rcThisClient(this);
	ui::CWindowRect rcThisWindow(this);
	if (rcThisClient.Width() == rcThisWindow.Width())
	{
		rcThisWindow.InflateRect(2, 2);
	}

	GetParent()->ScreenToClient(rcThisWindow);
	pdcParent->Draw3dRect(rcThisWindow, m_crBorder, m_crBorder);
	ReleaseDC(pdcParent);

	return TRUE;
}
