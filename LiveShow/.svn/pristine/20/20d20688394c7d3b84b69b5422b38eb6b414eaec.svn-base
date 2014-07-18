#include "StdAfx.h"
#include "ModifyNameTipWnd.h"
#include "..\..\CommonLibrary\include\utility\SystemHelper.h"

CModifyNameTipWnd::CModifyNameTipWnd(void)
{
	m_strTipsImgPath = L"";
	m_strCloseImgPath = L"";
}

CModifyNameTipWnd::~CModifyNameTipWnd(void)
{
	m_strTipsImgPath = L"";
	m_strCloseImgPath = L"";
}

BEGIN_MESSAGE_MAP(CModifyNameTipWnd, CWnd)
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
	ON_WM_CLOSE()
END_MESSAGE_MAP()

BOOL CModifyNameTipWnd::CreateEx(CWnd* pwndParent)
{
	if ( CWnd::CreateEx(WS_EX_TOOLWINDOW,AfxRegisterWndClass(0),_T("CoverWnd"),WS_POPUP,CRect(0, 0, 156, 116),pwndParent,0))
	{
		//窗口透明
		LONG dwstyle = GetWindowLong(GetSafeHwnd(),GWL_EXSTYLE) ;
		dwstyle |= WS_EX_LAYERED ;

		SetWindowLong( GetSafeHwnd() , GWL_EXSTYLE , dwstyle) ;

		m_blend.BlendOp = 0 ;
		m_blend.BlendFlags = 0 ;
		m_blend.AlphaFormat = 1 ;
		m_blend.SourceConstantAlpha = 255 ;

		//Draw();
		ShowWindow(SW_HIDE);
		return TRUE;
	}
	return FALSE;
}

void CModifyNameTipWnd::SetTipsImage(CString& strTipsImg)
{
	if (strTipsImg.IsEmpty())
	{
		return;
	}
	m_strTipsImgPath = strTipsImg;
}

void CModifyNameTipWnd::SetCloseImage(CString& strCloseImg)
{
	if (strCloseImg.IsEmpty())
	{
		return;
	}
	m_strCloseImgPath = strCloseImg;
	Draw();
}

void CModifyNameTipWnd::Draw()
{
	if (m_strTipsImgPath.IsEmpty() || m_strCloseImgPath.IsEmpty())
	{
		return;
	}
	CRect rect(0, 0, 0, 0);
	Image imgTips(m_strTipsImgPath);
	Image imgClose(m_strCloseImgPath);
	rect.right = imgTips.GetWidth();
	rect.bottom = imgTips.GetHeight();
	MoveWindow(&rect);
	CDC *pCDC = GetDC();
	HDC hdcTemp = pCDC->GetSafeHdc();
	HDC hdcMemory = CreateCompatibleDC( hdcTemp );
	HBITMAP hBitMap = CreateCompatibleBitmap( hdcTemp , rect.Width(), rect.Height() );
	HBITMAP hOld = (HBITMAP)SelectObject( hdcMemory , hBitMap );
	Graphics graph( hdcMemory );
	graph.DrawImage(&imgTips, 0, 0, rect.Width(), rect.Height());
	m_rcClose.left = rect.right - imgClose.GetWidth();
	m_rcClose.top = rect.top;
	m_rcClose.right = m_rcClose.left + imgClose.GetWidth();
	m_rcClose.bottom = m_rcClose.top + imgClose.GetHeight();
	graph.DrawImage(&imgClose, m_rcClose.left, m_rcClose.top, m_rcClose.Width(), m_rcClose.Height());

	BOOL bResult = BitBlt( hdcTemp , rect.left , rect.top , rect.Width() , rect.Height() , hdcMemory , 0 , 0 , SRCCOPY );

	POINT ptWinPos = { rect.left,rect.top };
	POINT ptSrc= {0,0};
	SIZE sizeWindow = {rect.Width(), rect.Height() };
	::UpdateLayeredWindow(m_hWnd, hdcTemp, &ptWinPos, &sizeWindow, hdcMemory, &ptSrc, 0, &m_blend, ULW_ALPHA);
	graph.ReleaseHDC(hdcMemory);
	SelectObject(hdcMemory, hOld);
	DeleteObject(hBitMap);
	DeleteDC(hdcMemory);
	ReleaseDC(pCDC);

}


void CModifyNameTipWnd::ShowModifyNameTipsWnd(CRect& rc)
{
	//Draw();
	CRect rcClient(0,0,0,0);
	GetWindowRect(&rcClient);
	MoveWindow(rc.left, rc.top, rcClient.Width(), rcClient.Height());
	ShowWindow(SW_SHOWNOACTIVATE);
}

void CModifyNameTipWnd::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (PtInRect(&m_rcClose, point))
	{
		HCURSOR hCur = LoadCursor(NULL, IDC_HAND);
		::SetCursor(hCur);
	}else
	{
		HCURSOR hCur = LoadCursor(NULL, IDC_ARROW);
		::SetCursor(hCur);
	}

	CWnd::OnMouseMove(nFlags, point);
}


void CModifyNameTipWnd::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (PtInRect(&m_rcClose, point))
	{
		::PostMessage(GetParent()->GetSafeHwnd(), WM_ONCLICK_MODIFYNAME_CLOSE, (WPARAM)this, 0);
	}

	CWnd::OnLButtonUp(nFlags, point);
}

void CModifyNameTipWnd::OnClose()  //屏蔽组合键关闭
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	return;
	CWnd::OnClose();
}
