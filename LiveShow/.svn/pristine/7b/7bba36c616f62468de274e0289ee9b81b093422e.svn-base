#include "StdAfx.h"
#include "CoverWnd.h"
#include "..\..\CommonLibrary\include\utility\SystemHelper.h"

CCoverWnd::CCoverWnd(void)
{
	m_strBkImgPath = L"";
}

CCoverWnd::~CCoverWnd(void)
{
	m_strBkImgPath = L"";
}

BEGIN_MESSAGE_MAP(CCoverWnd, CWnd)
END_MESSAGE_MAP()

BOOL CCoverWnd::CreateEx(CWnd* pwndParent, CRect& rc)
{
	if ( CWnd::CreateEx(WS_EX_TOOLWINDOW,AfxRegisterWndClass(0),_T("CoverWnd"),WS_POPUP|WS_DISABLED,rc,pwndParent,0))
	{
		//窗口透明
		LONG dwstyle = GetWindowLong(GetSafeHwnd(),GWL_EXSTYLE) ;
		dwstyle |= WS_EX_LAYERED ;

		SetWindowLong( GetSafeHwnd() , GWL_EXSTYLE , dwstyle) ;

		m_blend.BlendOp = 0 ;
		m_blend.BlendFlags = 0 ;
		m_blend.AlphaFormat = 1 ;
		m_blend.SourceConstantAlpha = 255 ;

		CString strTemp = common::utility::systemhelper::Get179AppPath().c_str();
		m_strBkImgPath.Format(L"%sresource\\other\\Global_imgCover.png",strTemp);
		
		DrawCoverWndBk();
		
		return TRUE;
	}
	return FALSE;
}

void CCoverWnd::DrawCoverWndBk()
{
	CRect rect;
	GetWindowRect( &rect );
	Image image(m_strBkImgPath);
	CDC *pCDC = GetDC();
	HDC hdcTemp = pCDC->GetSafeHdc();
	HDC hdcMemory = CreateCompatibleDC( hdcTemp );
	HBITMAP hBitMap = CreateCompatibleBitmap( hdcTemp , rect.Width(), rect.Height() );
	HBITMAP hOld = (HBITMAP)SelectObject( hdcMemory , hBitMap );
	Graphics graph( hdcMemory );
	ImageAttributes imgAtt;
	imgAtt.SetWrapMode(WrapModeTileFlipXY);//GDI+在位伸图片时会自动加上渐变效果。但此处不需要，所以得加上此属性
	//graph.DrawImage(&image, 0, 0, rect.Width(), rect.Height());
	Rect rcTemp(0, 0, rect.Width(), rect.Height());
	graph.DrawImage(&image, rcTemp, 0, 0, rect.Width(), rect.Height(),UnitPixel,&imgAtt);

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


void CCoverWnd::ShowCoverWnd(CRect& rc)
{
	MoveWindow(&rc);
	DrawCoverWndBk();
}


