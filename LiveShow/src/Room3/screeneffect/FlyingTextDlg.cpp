// FlyingTextDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "..\Room.h"
#include "FlyingTextDlg.h"

#define ROOM_DRAW_FLYINGTEXT_TIMER_ID		30060

// CFlyingTextDlg 对话框

IMPLEMENT_DYNAMIC(CFlyingTextDlg, CDialog)

CFlyingTextDlg::CFlyingTextDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CFlyingTextDlg::IDD, pParent)
{
	m_bFinish = true;
	m_hdcMemory1 = NULL;
	m_hdcMemory2 = NULL;
	m_nTotalWidth = 0;
	m_nDrawIndex = 0;
}

CFlyingTextDlg::~CFlyingTextDlg()
{
	m_trackInfo.Release();
	DeleteDC(m_hdcMemory1);
	DeleteDC(m_hdcMemory2);
}

void CFlyingTextDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CFlyingTextDlg, CDialog)
	ON_WM_CREATE()
	ON_WM_TIMER()
	ON_WM_SIZE()
	ON_WM_CLOSE()
END_MESSAGE_MAP()


// CFlyingTextDlg 消息处理程序

int CFlyingTextDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  在此添加您专用的创建代码
	LONG dwstyle = GetWindowLong(GetSafeHwnd(),GWL_EXSTYLE);
	dwstyle |= WS_EX_LAYERED | WS_EX_TOOLWINDOW | WS_EX_TOPMOST;
	SetWindowLong( GetSafeHwnd() , GWL_EXSTYLE , dwstyle);
	//m_hInst = LoadLibrary(_T("User32.DLL") );

	m_blend.BlendOp = 0 ;
	m_blend.BlendFlags = 0 ;
	m_blend.AlphaFormat = 1 ;
	m_blend.SourceConstantAlpha = 255 ;
	//UpdateLayeredWindow = (FUpdateLayeredWindow)GetProcAddress( m_hInst ,("UpdateLayeredWindow") ) ;VC6.0才需要从User32.dll中调用这个函数

	return 0;
}

void CFlyingTextDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (nIDEvent == ROOM_DRAW_FLYINGTEXT_TIMER_ID && !m_bFinish)
	{
		Draw();
	}

	CDialog::OnTimer(nIDEvent);
}

void CFlyingTextDlg::Show( std::wstring strNickName, std::wstring strText, ENM_FlyingText_Type enmType )
{
	flying_text_info * pFlyingInfo = ScreenEffectManager::GetFlyingInfo(enmType);
	if (pFlyingInfo == NULL)
	{
		OutputDebugString(L"CFlyingTextDlg::Show GetFlyingInfo Error");
		return;
	}

	m_bFinish = false;

	Init();

	m_trackInfo.strNickName = strNickName;
	m_trackInfo.strContent = strText;
	InitRes(pFlyingInfo);
	FirstDraw();

	SetTimer(ROOM_DRAW_FLYINGTEXT_TIMER_ID, 100, NULL);
}

void CFlyingTextDlg::Draw()
{
	//LARGE_INTEGER freq, t1, t2;
	//QueryPerformanceFrequency(&freq);
	//QueryPerformanceCounter(&t1);

	CRect rect ;
	GetWindowRect( &rect ) ;

	CDC *pCDC = GetDC() ;
	HDC hdcTemp = pCDC->GetSafeHdc();
	HDC hdcMemory = CreateCompatibleDC(hdcTemp) ;
	HBITMAP hBitMap = CreateCompatibleBitmap( hdcTemp , rect.Width(), rect.Height() ) ;
	HBITMAP hOld = (HBITMAP)SelectObject( hdcMemory , hBitMap ) ;

	//循环显示效果，暂时不用
	//if (m_nPos < 0)
	//{
	//	do 
	//	{
	//		RectF rcTemp(boundRect);
	//		rcTemp.Offset(nOffsetX[nIndex], nOffsetY[nIndex]);
	//		graph.DrawString(m_strText.c_str(), m_strText.length(), &m_Font, rcTemp, &stringFormat, &solidBrush);
	//		nIndex += 1;
	//	} while (nIndex < 8);
	//	graph.DrawString(m_strText.c_str(), m_strText.length(), &m_Font, PointF(rect.Width()+m_nPos,30), &brush);
	//	if ((-m_nPos) > boundRect.Width)
	//	{
	//		m_nPos = rect.Width() + m_nPos - 8;
	//	}
	//}

	//BOOL bResult = BitBlt( hdcTemp , 0 , 0 , rect.Width() , rect.Height() , hdcMemory , 0 , 0 , SRCCOPY ) ;

	if (m_nDrawIndex < 3)
	{
		BitBlt(hdcMemory, rect.Width() - m_trackInfo.nCurPosX, 0, m_trackInfo.nCurPosX, rect.Height(), m_hdcMemory1, 0, 0, SRCCOPY);
	}
	else
	{
		BitBlt(hdcMemory, rect.Width() - m_trackInfo.nCurPosX, 0, m_trackInfo.nCurPosX, rect.Height(), m_hdcMemory2, 0, 0, SRCCOPY);
	}

	POINT ptWinPos = { rect.left, rect.top } ;
	m_trackInfo.nCurPosX += 10;
	if (m_trackInfo.nCurPosX - rect.Width() > m_nTotalWidth)
	{
		m_bFinish = true;
		KillTimer(ROOM_DRAW_FLYINGTEXT_TIMER_ID);
	}
	POINT ptSrc= {0,0} ;
	SIZE sizeWindow = {rect.Width() , rect.Height() } ;

	::UpdateLayeredWindow( m_hWnd , NULL , &ptWinPos ,&sizeWindow , hdcMemory ,&ptSrc , 0 ,&m_blend , ULW_ALPHA ) ;

	m_nDrawIndex++;
	if (m_nDrawIndex > 5)
	{
		m_nDrawIndex = 0;
	}

	//QueryPerformanceCounter(&t2);
	//double d = ((double)t2.QuadPart - (double)t1.QuadPart) / (double)freq.QuadPart;
	//TRACE(L"\r\n?????????????????? %f ??????????????????", d);

	SelectObject( hdcMemory , hOld ) ;
	DeleteObject( hBitMap) ;
	DeleteDC( hdcMemory ) ;
	ReleaseDC(pCDC);
}

int CFlyingTextDlg::DrawTitleImage( Graphics & graph, int nIndex, int nPosX, int nPosY )
{
	Image * pImage = NULL;
	if (nIndex < m_trackInfo.vecImageList.size())
	{
		pImage = m_trackInfo.vecImageList[nIndex];
		graph.DrawImage(pImage, nPosX, nPosY, pImage->GetWidth(), pImage->GetHeight());

		return pImage->GetWidth();
	}

	return 0;
}

int CFlyingTextDlg::DrawNickName( Graphics & graph, int nIndex, int nPosX, int nPosY )
{
	//int nNickPosY = nPosY + m_pFlyingInfo->stContent.nFontSize - m_pFlyingInfo->stNickName.nFontSize;
	int nNickPosY = nPosY + (int)m_trackInfo.pContentFont->GetSize() - (int)m_trackInfo.pNickNameFont->GetSize();
	RectF boundRect;
	graph.MeasureString(m_trackInfo.strNickName.c_str(),m_trackInfo.strNickName.length(), m_trackInfo.pNickNameFont, PointF(nPosX, nNickPosY), &boundRect);
	if (m_trackInfo.pFlyingInfo->stNickName.bStroke)
	{
		Color strokecolor;
		strokecolor.SetFromCOLORREF(m_trackInfo.pFlyingInfo->stNickName.dwStrokeColor);
		SolidBrush solidBrush(strokecolor);
		StringFormat stringFormat(0, LANG_NEUTRAL);
		int nOffsetX[8] = {1,1,1,0,-1,-1,-1,0};
		int nOffsetY[8] = {-1,0,1,1,1,0,-1,-1};
		int nOffsetIndex = 0;
		do 
		{
			RectF rcTemp(boundRect);
			rcTemp.Offset(nOffsetX[nOffsetIndex], nOffsetY[nOffsetIndex]);
			graph.DrawString(m_trackInfo.strNickName.c_str(), m_trackInfo.strNickName.length(), m_trackInfo.pNickNameFont, rcTemp, &stringFormat, &solidBrush);
			nOffsetIndex += 1;
		} while (nOffsetIndex < 8);
	}
	Color begincolor, endcolor;
	begincolor.SetFromCOLORREF(m_trackInfo.pFlyingInfo->stNickName.dwBeginColor);
	endcolor.SetFromCOLORREF(m_trackInfo.pFlyingInfo->stNickName.dwEndColor);
	LinearGradientBrush brush(boundRect, begincolor, endcolor, (LinearGradientMode)m_trackInfo.pFlyingInfo->stNickName.nGradientMode);
	graph.DrawString(m_trackInfo.strNickName.c_str(), m_trackInfo.strNickName.length(), m_trackInfo.pNickNameFont, PointF(nPosX, nNickPosY), &brush);

	return boundRect.Width;
}

int CFlyingTextDlg::DrawSay( Graphics & graph, int nIndex, int nPosX, int nPosY )
{
	//int nSayPosY = nPosY + m_pFlyingInfo->stContent.nFontSize - m_pFlyingInfo->stSay.nFontSize;
	int nSayPosY = nPosY + (int)m_trackInfo.pContentFont->GetSize() - (int)m_trackInfo.pSayFont->GetSize() + 4;
	RectF boundRect;
	std::wstring strSay = L"说：";
	graph.MeasureString(strSay.c_str(),strSay.length(), m_trackInfo.pSayFont, PointF(nPosX, nSayPosY), &boundRect);
	if (m_trackInfo.pFlyingInfo->stSay.bStroke)
	{
		Color strokecolor;
		strokecolor.SetFromCOLORREF(m_trackInfo.pFlyingInfo->stSay.dwStrokeColor);
		SolidBrush solidBrush(strokecolor);
		StringFormat stringFormat(0, LANG_NEUTRAL);
		int nOffsetX[8] = {1,1,1,0,-1,-1,-1,0};
		int nOffsetY[8] = {-1,0,1,1,1,0,-1,-1};
		int nOffsetIndex = 0;
		do 
		{
			RectF rcTemp(boundRect);
			rcTemp.Offset(nOffsetX[nOffsetIndex], nOffsetY[nOffsetIndex]);
			graph.DrawString(strSay.c_str(), strSay.length(), m_trackInfo.pSayFont, rcTemp, &stringFormat, &solidBrush);
			nOffsetIndex += 1;
		} while (nOffsetIndex < 8);
	}
	Color begincolor, endcolor;
	begincolor.SetFromCOLORREF(m_trackInfo.pFlyingInfo->stSay.dwBeginColor);
	endcolor.SetFromCOLORREF(m_trackInfo.pFlyingInfo->stSay.dwEndColor);
	LinearGradientBrush brush(boundRect, begincolor, endcolor, (LinearGradientMode)m_trackInfo.pFlyingInfo->stSay.nGradientMode);
	graph.DrawString(strSay.c_str(), strSay.length(), m_trackInfo.pSayFont, PointF(nPosX, nSayPosY), &brush);

	return boundRect.Width;
}

int CFlyingTextDlg::DrawContent( Graphics & graph, int nIndex, int nPosX, int nPosY )
{
	int nContentPosY = nPosY + 3;
	RectF boundRect;
	graph.MeasureString(m_trackInfo.strContent.c_str(),m_trackInfo.strContent.length(), m_trackInfo.pContentFont, PointF(nPosX, nContentPosY), &boundRect);
	if (m_trackInfo.pFlyingInfo->stContent.bShadow)
	{
		Color shadowcolor;
		shadowcolor.SetFromCOLORREF(m_trackInfo.pFlyingInfo->stContent.dwShadowColor);
		SolidBrush solodbrush(shadowcolor);
		graph.DrawString(m_trackInfo.strContent.c_str(), m_trackInfo.strContent.length(), m_trackInfo.pContentFont, PointF(nPosX + 2, nContentPosY + 2), &solodbrush);
	}

	if (m_trackInfo.pFlyingInfo->stContent.bStroke)
	{
		Color strokecolor;
		strokecolor.SetFromCOLORREF(m_trackInfo.pFlyingInfo->stContent.dwStrokeColor);
		SolidBrush solidBrush(strokecolor);
		StringFormat stringFormat(0, LANG_NEUTRAL);
		int nOffsetX[8] = {1,1,1,0,-1,-1,-1,0};
		int nOffsetY[8] = {-1,0,1,1,1,0,-1,-1};
		int nOffsetIndex = 0;
		do 
		{
			RectF rcTemp(boundRect);
			rcTemp.Offset(nOffsetX[nOffsetIndex], nOffsetY[nOffsetIndex]);
			graph.DrawString(m_trackInfo.strContent.c_str(), m_trackInfo.strContent.length(), m_trackInfo.pContentFont, rcTemp, &stringFormat, &solidBrush);
			nOffsetIndex += 1;
		} while (nOffsetIndex < 8);
	}
	Color begincolor, endcolor;
	begincolor.SetFromCOLORREF(m_trackInfo.pFlyingInfo->stContent.dwBeginColor);
	endcolor.SetFromCOLORREF(m_trackInfo.pFlyingInfo->stContent.dwEndColor);
	LinearGradientBrush brush(boundRect, begincolor, endcolor, (LinearGradientMode)m_trackInfo.pFlyingInfo->stContent.nGradientMode);
	//SolidBrush solodbrush2(Color(0,0,0));
	//graph.DrawString(m_trackInfo.strContent.c_str(), m_trackInfo.strContent.length(), m_trackInfo.pContentFont, PointF(nPosX + 2, nPosY + 2), &solodbrush2);
	graph.DrawString(m_trackInfo.strContent.c_str(), m_trackInfo.strContent.length(), m_trackInfo.pContentFont, PointF(nPosX, nContentPosY), &brush);

	return boundRect.Width;
}

void CFlyingTextDlg::InitRes(flying_text_info * pFlyingInfo)
{
	m_trackInfo.pFlyingInfo = pFlyingInfo;
	int nFontStyle = 0;
	if (pFlyingInfo->stNickName.bBold)
	{
		nFontStyle |= FontStyleBold;
	}
	if (pFlyingInfo->stNickName.bItalic)
	{
		nFontStyle |= FontStyleItalic;
	}
	if (pFlyingInfo->stNickName.bUnderline)
	{
		nFontStyle |= FontStyleUnderline;
	}
	m_trackInfo.pNickNameFont = new Font(pFlyingInfo->stNickName.strFontFamily.c_str(), pFlyingInfo->stNickName.nFontSize, nFontStyle, UnitPixel);

	nFontStyle = 0;
	if (pFlyingInfo->stSay.bBold)
	{
		nFontStyle |= FontStyleBold;
	}
	if (pFlyingInfo->stSay.bItalic)
	{
		nFontStyle |= FontStyleItalic;
	}
	if (pFlyingInfo->stSay.bUnderline)
	{
		nFontStyle |= FontStyleUnderline;
	}
	m_trackInfo.pSayFont = new Font(pFlyingInfo->stSay.strFontFamily.c_str(), pFlyingInfo->stSay.nFontSize, nFontStyle, UnitPixel);

	nFontStyle = 0;
	if (pFlyingInfo->stContent.bBold)
	{
		nFontStyle |= FontStyleBold;
	}
	if (pFlyingInfo->stContent.bItalic)
	{
		nFontStyle |= FontStyleItalic;
	}
	if (pFlyingInfo->stContent.bUnderline)
	{
		nFontStyle |= FontStyleUnderline;
	}
	m_trackInfo.pContentFont = new Font(pFlyingInfo->stContent.strFontFamily.c_str(), pFlyingInfo->stContent.nFontSize, nFontStyle, UnitPixel);

	for (std::vector<std::wstring>::iterator it = pFlyingInfo->vecStrImagePath.begin(); it != pFlyingInfo->vecStrImagePath.end(); ++it)
	{
		std::wstring strPath = *it;
		Image * pImage = new Image(strPath.c_str());

		m_trackInfo.vecImageList.push_back(pImage);
	}
}

void CFlyingTextDlg::Init()
{
	m_trackInfo.nCurPosX = 0;
}

void CFlyingTextDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码
}

void CFlyingTextDlg::FirstDraw()
{
	static CRect rect ;
	GetWindowRect( &rect ) ;

	CDC *pCDC = GetDC() ;
	HDC hdcTemp = pCDC->GetSafeHdc();
	m_hdcMemory1 = CreateCompatibleDC(hdcTemp) ;
	HBITMAP hBitMap = CreateCompatibleBitmap( hdcTemp , rect.Width(), rect.Height() ) ;
	HBITMAP hOld = (HBITMAP)SelectObject( m_hdcMemory1 , hBitMap ) ;
	Graphics graph( m_hdcMemory1 ) ;
	m_hdcMemory2 = CreateCompatibleDC(hdcTemp);
	HBITMAP hBitMap2 = CreateCompatibleBitmap( hdcTemp , rect.Width(), rect.Height() ) ;
	HBITMAP hOld2 = (HBITMAP)SelectObject( m_hdcMemory2 , hBitMap2 ) ;
	Graphics graph2( m_hdcMemory2 ) ;

	graph.SetTextRenderingHint(TextRenderingHintAntiAlias);//文字抗锯齿效果，在win7不加这个会有锯齿
	graph2.SetTextRenderingHint(TextRenderingHintAntiAlias);
	int nImageWidth;
	int nNickNameWidth;
	int nSayWidth;
	int nContentWidth;

	nImageWidth = DrawTitleImage(graph, 0, 0, m_trackInfo.nStartPosY);
	DrawTitleImage(graph2, 1, 0, m_trackInfo.nStartPosY);
	nNickNameWidth = DrawNickName(graph, 0, nImageWidth, m_trackInfo.nStartPosY);
	DrawNickName(graph2, 0, nImageWidth, m_trackInfo.nStartPosY);
	nSayWidth = DrawSay(graph, 0, nImageWidth + nNickNameWidth, m_trackInfo.nStartPosY);
	DrawSay(graph2, 0, nImageWidth + nNickNameWidth, m_trackInfo.nStartPosY);
	nContentWidth = DrawContent(graph, 0, nImageWidth + nNickNameWidth + nSayWidth, m_trackInfo.nStartPosY);
	DrawContent(graph2, 0, nImageWidth + nNickNameWidth + nSayWidth, m_trackInfo.nStartPosY);

	m_nTotalWidth = nImageWidth + nNickNameWidth + nSayWidth + nContentWidth;

	//m_trackInfo[i].nCurPosX -= 10;
	//if ((-m_trackInfo[i].nCurPosX) > (nImageWidth + nNickNameWidth + nSayWidth + nContentWidth))
	//{
	//	m_trackInfo[i].Release();
	//	m_trackInfo[i].nCurPosX = rect.Width();
	//	m_trackInfo[i].bInUse = false;
	//	m_bTrackFull = false;
	//}

	graph.ReleaseHDC(m_hdcMemory1);
	SelectObject(m_hdcMemory1, hOld);
	graph2.ReleaseHDC(m_hdcMemory2);
	SelectObject(m_hdcMemory2, hOld2);
	DeleteObject(hBitMap);
	DeleteObject(hBitMap2);
	//DeleteDC( hdcMemory );
	ReleaseDC(pCDC);
}

BOOL CFlyingTextDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	if ( pMsg->message == WM_KEYDOWN)
	{
		switch(pMsg->wParam)
		{
		case VK_ESCAPE:
		case VK_RETURN:

			return TRUE ;
		default: break;
		}	
	}


	return CDialog::PreTranslateMessage(pMsg);
}

void CFlyingTextDlg::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	//CDialog::OnClose();
}
