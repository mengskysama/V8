#include "stdafx.h"
#include "..\Room.h"
#include "FlyLuxuryWnd.h"
#include "..\UserMessageDefine.h"
#include "..\gift/GiftManager.h"
#include "..\..\CommonLibrary\include\utility\SystemHelper.h"
#include "..\..\CommonLibrary/include/ui/UIHelper.h"
#include "..\paodao/PaodaoBtnDlg.h"

// CPaodaoStaitc

IMPLEMENT_DYNAMIC(CFlyLuxuryWnd, CWnd)

CFlyLuxuryWnd::CFlyLuxuryWnd( CRoomParentDlg* pParentDlg )
:m_pParentDlg( pParentDlg )
{
	m_pFlyLuxuryImg = NULL;
	m_pGiftImg = NULL;
	m_nLastScrollSpeed = m_nScrollSpeed = 0 ;
	m_bFinish = false;
	m_nOldWidth = 0;
	m_bInit = true;
	common::utility::systemhelper::GetDefaultFontName( m_strFont );
}

CFlyLuxuryWnd::~CFlyLuxuryWnd()
{
	if ( m_pFlyLuxuryImg != NULL )
	{
		delete m_pFlyLuxuryImg;
		m_pFlyLuxuryImg = NULL;
	}

	if (m_pGiftImg != NULL)
	{
		delete m_pGiftImg;
		m_pGiftImg = NULL;
	}

	for (std::vector<HDC>::iterator it = m_vecHDC.begin(); it != m_vecHDC.end(); ++it)
	{
		DeleteDC(*it);
	}

	m_vecHDC.clear();
}


BEGIN_MESSAGE_MAP(CFlyLuxuryWnd, CWnd)
	ON_WM_TIMER()
	ON_WM_CREATE()
	ON_WM_SIZE()
END_MESSAGE_MAP()

void CFlyLuxuryWnd::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	InvalidateCtrl() ;
	//if ( nIDEvent == PAODAO_MOVE_TIMER )
	//{		
	//	// move text up one pixel
	//	m_nScrollPos = m_nScrollPos - m_nScrollSpeed ;
	//}
	if ( nIDEvent == PAODAO_DRAW_GIF_TIMER )
	{
		m_nScrollPos = m_nScrollPos - m_nScrollSpeed ;
		if ( m_pGiftImg->IsAnimatedGIF() )
		{
			m_pGiftImg->DrawFrameGIF() ;
		}
		
	}
	CWnd::OnTimer(nIDEvent);
}

int CFlyLuxuryWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  在此添加您专用的创建代码
	//LONG dwstyle = GetWindowLong(GetSafeHwnd(),GWL_EXSTYLE);
	//dwstyle |= WS_EX_LAYERED | WS_EX_TOOLWINDOW ;
	//SetWindowLong( GetSafeHwnd() , GWL_EXSTYLE , dwstyle);

	m_blend.BlendOp = 0 ;
	m_blend.BlendFlags = 0 ;
	m_blend.AlphaFormat = 1 ;
	m_blend.SourceConstantAlpha = 255 ;

	return 0;
}

void CFlyLuxuryWnd::OnSize(UINT nType, int cx, int cy) 
{
	CWnd::OnSize(nType, cx, cy);

	if (cx == 0 || cy == 0)
	{
		return;
	}

	GetClientRect(m_rectClient);
	m_nScrollPos -= (m_nOldWidth - cx);
	m_nOldWidth = cx;
	//InvalidateCtrl() ;	
}

void CFlyLuxuryWnd::Measure( PaodaoInfo& paodaoinfo , bool bInit ) 
{
	CPaintDC dc(this) ;
	Graphics    graphics(dc);	
	if ( bInit )
	{
		if ( m_pGiftImg != NULL )
		{
			delete m_pGiftImg ;
			m_pGiftImg = NULL ;
		}
		m_pGiftImg = new common::ui::ImageEx( paodaoinfo.strLargeImagePath ) ;
		if ( m_pGiftImg == NULL )
		{
			return ;
		}

		if ( m_pFlyLuxuryImg == NULL)
		{
			std::wstring strPath = common::utility::systemhelper::Get179AppPath() + L"resource\\other\\FlyLuxury.png";
			m_pFlyLuxuryImg = new common::ui::ImageEx(strPath.c_str());
		}
	}
	m_nTextSize = 0 ;
	float fTextSize = 0.0f;
	m_nPicSize = MeasureImage( m_pFlyLuxuryImg ) ;
	fTextSize += MeasureSrcName( graphics , paodaoinfo.strSrcName );
	fTextSize += MeasureSrcUUID( graphics , paodaoinfo.strSrcUUID );
	fTextSize += MeasureSendTo( graphics , paodaoinfo.strSendDesc);
	fTextSize += MeasureDstName( graphics , paodaoinfo.strDstName );
	fTextSize += MeasureDstUUID( graphics , paodaoinfo.strDstUUID );
	fTextSize += MeasureCount( graphics , paodaoinfo.unCount , paodaoinfo.strUnitName );
	fTextSize += MeasureGiftName( graphics , paodaoinfo.strGiftName );
	m_nPicSize += MeasureImage( m_pGiftImg ) ;
	m_nTextSize = fTextSize + 1;
	//m_nTextSize += MeasureTime( graphics , paodaoinfo.strTime ) ;	
	if ( bInit )
	{
		m_nOldWidth = m_rectClient.Width();
		m_nScrollPos = m_rectClient.Width() + m_nTextSize + m_nPicSize ;
	}
}

void CFlyLuxuryWnd::InvalidateCtrl()
{
	CClientDC dc(this);

	// if we don't have one yet, set up a memory dc for the control 
	DoScrollDraw( &dc ) ;
	//InvalidateRect(m_rectClient);
}

void CFlyLuxuryWnd::DoScrollDraw( CDC *pDC ) 
{
	if (m_bInit)
	{
		//LARGE_INTEGER freq, t1, t2;
		//QueryPerformanceFrequency(&freq);
		//QueryPerformanceCounter(&t1);
		UINT nFrames = m_pGiftImg->GetFrameCounts();
		HDC hdcText;
		hdcText = CreateCompatibleDC(pDC->GetSafeHdc());
		HBITMAP hBitMapText = CreateCompatibleBitmap(pDC->GetSafeHdc(), m_pFlyLuxuryImg->GetWidth() + m_nTextSize, m_rectClient.Height());
		HBITMAP hOldText = (HBITMAP)SelectObject(hdcText, hBitMapText);
		Graphics graphText(hdcText);

		graphText.SetTextRenderingHint(TextRenderingHintAntiAlias);
		float fReturnValue = 0.0f ;
		DrawWithoutGift( graphText, m_PaodaoInfo, fReturnValue );

		for (int i = 0; i < nFrames; i++)
		{
			HDC hdcMem;
			hdcMem = CreateCompatibleDC(pDC->GetSafeHdc());
			//HBITMAP hBitMap = CreateCompatibleBitmap(pDC->GetSafeHdc(), m_rectClient.Width(), m_rectClient.Height());
			HBITMAP hBitMap = CreateCompatibleBitmap(pDC->GetSafeHdc(), m_nTextSize + m_nPicSize, m_rectClient.Height());
			HBITMAP hOld = (HBITMAP)SelectObject(hdcMem, hBitMap);

			BitBlt(hdcMem, 0, 0, m_pFlyLuxuryImg->GetWidth() + m_nTextSize, m_rectClient.Height(), hdcText, 0, 0, SRCCOPY);
			Graphics graph(hdcMem);

			//graph.SetTextRenderingHint(TextRenderingHintAntiAlias);
			//Draw(graph, m_PaodaoInfo, 0);
			DrawImage( graph, m_pGiftImg, fReturnValue );
			m_pGiftImg->DrawNextFrame();

			m_vecHDC.push_back(hdcMem);

			graph.ReleaseHDC(hdcMem);
			SelectObject(hdcMem, hOld);
			DeleteObject(hBitMap);
		}

		m_bInit = false;

		graphText.ReleaseHDC(hdcText);
		SelectObject(hdcText, hOldText);
		DeleteObject(hBitMapText);
		DeleteDC(hdcText);
		//QueryPerformanceCounter(&t2);
		//double d = ((double)t2.QuadPart - (double)t1.QuadPart) / (double)freq.QuadPart;
		//TRACE(L"\r\n?????????????????? %f ??????????????????", d);
	}

	// draw Credits on the hidden Picture	
	int nPosX = m_nScrollPos - m_nTextSize - m_nPicSize ;
	if (nPosX < -(m_nTextSize+m_nPicSize) )
	{
		m_bFinish = true;
		KillTimer(PAODAO_DRAW_GIF_TIMER);
	}

	CDC *pCDC = GetDC() ;
	HDC hdcTemp = pCDC->GetSafeHdc();
	HDC hdcMemory = CreateCompatibleDC(hdcTemp) ;
	HBITMAP hBitMap = CreateCompatibleBitmap( hdcTemp , m_rectClient.Width(), m_rectClient.Height() ) ;
	HBITMAP hOld = (HBITMAP)SelectObject( hdcMemory , hBitMap ) ;

	BitBlt(hdcMemory, nPosX, 0, m_rectClient.Width() - nPosX, m_rectClient.Height(), m_vecHDC[m_pGiftImg->GetCurrentFramePos()], 0, 0, SRCCOPY);
	CRect rc;
	GetWindowRect(&rc);
	POINT ptWinPos = { rc.left,rc.top } ;
	POINT ptSrc= {0,0} ;
	SIZE sizeWindow = {rc.Width() , rc.Height() } ;
	::UpdateLayeredWindow( m_hWnd , NULL , &ptWinPos ,&sizeWindow , hdcMemory ,&ptSrc , 0 ,&m_blend , ULW_ALPHA ) ;

	SelectObject( hdcMemory , hOld ) ;
	DeleteObject( hBitMap) ;
	DeleteDC( hdcMemory ) ;
	ReleaseDC(pCDC);
}

void CFlyLuxuryWnd::Draw( Graphics& graphics  , PaodaoInfo& PaodaoInfo , float fPosX ) 
{
	float fReturnValue = fPosX ;
	fReturnValue = DrawImage( graphics, m_pFlyLuxuryImg, fReturnValue );
	fReturnValue = DrawSrcName( graphics , PaodaoInfo.strSrcName , fReturnValue ) ;
	fReturnValue = DrawSrcUUID( graphics , PaodaoInfo.strSrcUUID , fReturnValue ) ;
	fReturnValue = DrawSendTo( graphics , PaodaoInfo.strSendDesc , fReturnValue );
	fReturnValue = DrawDstName( graphics , PaodaoInfo.strDstName , fReturnValue ) ;
	fReturnValue = DrawDstUUID( graphics , PaodaoInfo.strDstUUID , fReturnValue ) ;
	fReturnValue = DrawCount( graphics , PaodaoInfo.unCount , PaodaoInfo.strUnitName ,fReturnValue ) ;
	fReturnValue = DrawGiftName( graphics , PaodaoInfo.strGiftName , fReturnValue ) ;
	fReturnValue = DrawImage( graphics, m_pGiftImg, fReturnValue );
}

void CFlyLuxuryWnd::InsertPaoDaoText(CString& strSrcName , CString& strDstName , uint32 unSrc179ID , 
					  uint32 unDst179ID , uint16 unGiftID ,uint16 unCount ) 
{
	IGiftItem* pGiftItem = GiftManager::GetInstance()->SearchGiftByID( unGiftID ) ;
	if ( pGiftItem == NULL )
	{
		return ;
	}
	m_PaodaoInfo.strDstName = strDstName ;
	m_PaodaoInfo.strSrcName = strSrcName ;
	//m_PaodaoInfo.strTime = strTime ;
	CString strTemp ;
	strTemp.Format( L"(%u)" , unSrc179ID ) ;
	m_PaodaoInfo.strSrcUUID = strTemp ;
	strTemp.Format( L"(%u)" , unDst179ID ) ;
	m_PaodaoInfo.strDstUUID = strTemp ;
	//m_PaodaoInfo.strGiftName = pGiftItem->GetName().c_str() ;
	m_PaodaoInfo.strGiftName = pGiftItem->GetRichName().c_str();		//奢侈礼物用完整名字
	m_PaodaoInfo.strUnitName = pGiftItem->GetUnitName().c_str() ;
	m_PaodaoInfo.strSendDesc = pGiftItem->GetStrSendDesc().c_str() ;
	m_PaodaoInfo.unCount = unCount ;
	m_PaodaoInfo.strLargeImagePath = pGiftItem->GetLargeImagePath().c_str() ;

	Measure( m_PaodaoInfo , true ) ;
	m_nScrollSpeed = PAODAO_MOVE_SPEED;

	CRect rc;
	GetClientRect(rc);
	m_nScrollPos -= (m_nOldWidth - rc.right);

	SetTimer( PAODAO_DRAW_GIF_TIMER , PAODAO_DRAW_GIF_TIMER_INTERVAL , NULL ) ;
}

float CFlyLuxuryWnd::MeasureImage( uint16 unCount ) 
{
	return m_pGiftImg->GetWidth()* unCount ;
}

float CFlyLuxuryWnd::MeasureImage( common::ui::ImageEx * pImage )
{
	return pImage->GetWidth();
}

float CFlyLuxuryWnd::MeasureGiftName( Graphics& graphics , CString& strTemp ) 
{
	FontFamily  fontFamily( m_strFont.c_str() );
	Font        font(&fontFamily, 32, FontStyleBoldItalic, UnitPixel);
	PointF      pointF( 0 , 18.0f);
	RectF boundRect;
	graphics.MeasureString( strTemp , strTemp.GetLength() ,&font ,pointF , &boundRect ) ;
	return boundRect.Width ;
}

float CFlyLuxuryWnd::MeasureSrcName( Graphics& graphics , CString& strTemp ) 
{
	FontFamily  fontFamily( m_strFont.c_str() );
	Font        font(&fontFamily, 30, FontStyleBoldItalic, UnitPixel);
	PointF      pointF( 0 , 18.0f);
	RectF boundRect;
	graphics.MeasureString( strTemp , strTemp.GetLength() ,&font ,pointF , &boundRect ) ;
	return boundRect.Width ;
}

float CFlyLuxuryWnd::MeasureSrcUUID( Graphics& graphics , CString& strTemp ) 
{
	FontFamily  fontFamily( m_strFont.c_str() );
	Font        font(&fontFamily, 24, FontStyleBoldItalic, UnitPixel);
	PointF      pointF( 0 , 30.0f);
	RectF boundRect;
	graphics.MeasureString( strTemp , strTemp.GetLength() ,&font ,pointF , &boundRect ) ;
	return boundRect.Width ;
}

float CFlyLuxuryWnd::MeasureSendTo( Graphics& graphics , CString strTemp ) 
{
	FontFamily  fontFamily( m_strFont.c_str() );
	Font        font(&fontFamily, 23, FontStyleBoldItalic, UnitPixel);
	PointF      pointF( 0 , 30.0f);
	RectF boundRect;
	graphics.MeasureString( strTemp , strTemp.GetLength() ,&font ,pointF , &boundRect ) ;
	return boundRect.Width ;
}

float CFlyLuxuryWnd::MeasureDstName( Graphics& graphics , CString& strTemp ) 
{
	FontFamily  fontFamily( m_strFont.c_str() );
	Font        font(&fontFamily, 27, FontStyleBoldItalic, UnitPixel);
	PointF      pointF( 0 , 18.0f);
	RectF boundRect;
	graphics.MeasureString( strTemp , strTemp.GetLength() ,&font ,pointF , &boundRect ) ;
	return boundRect.Width ;
}

float CFlyLuxuryWnd::MeasureDstUUID( Graphics& graphics , CString& strTemp ) 
{
	FontFamily  fontFamily( m_strFont.c_str() );
	Font        font(&fontFamily, 24, FontStyleBoldItalic, UnitPixel);
	PointF      pointF( 0 , 30.0f);
	RectF boundRect;
	graphics.MeasureString( strTemp , strTemp.GetLength() ,&font ,pointF , &boundRect ) ;
	return boundRect.Width ;
}

float CFlyLuxuryWnd::MeasureCount( Graphics& graphics , uint16 unCount , CString& strGiftUnit ) 
{
	CString strTemp ;
	strTemp.Format( L"%u%s" , unCount , strGiftUnit.GetBuffer(0) ) ;
	FontFamily  fontFamily( m_strFont.c_str() );
	Font        font(&fontFamily, 30, FontStyleBoldItalic, UnitPixel);
	PointF      pointF( 0 , 30.0f);
	RectF boundRect;
	graphics.MeasureString( strTemp , strTemp.GetLength() ,&font ,pointF , &boundRect ) ;
	return boundRect.Width ;
}

float CFlyLuxuryWnd::MeasureTime( Graphics& graphics , CString& strTemp ) 
{
	FontFamily  fontFamily( m_strFont.c_str() );
	Font        font(&fontFamily, 22, FontStyleBold, UnitPixel);
	PointF      pointF( 0 , 30.0f);
	RectF boundRect;
	graphics.MeasureString( strTemp , strTemp.GetLength() ,&font ,pointF , &boundRect ) ;
	return boundRect.Width ;
}

float CFlyLuxuryWnd::DrawImage( Graphics& graphics , uint16 unCount , float fPosX ) 
{
	float fReturnValue = fPosX ;
	for ( uint16 i = 0 ;i < unCount ; ++i )
	{
		graphics.DrawImage( m_pGiftImg , fReturnValue ,0.0f , m_pGiftImg->GetWidth() , m_pGiftImg->GetHeight() ) ;
		fReturnValue += m_pGiftImg->GetWidth() ;
	}
	return fReturnValue ;
}

float CFlyLuxuryWnd::DrawImage( Graphics& graphics ,common::ui::ImageEx * pImage ,float fPosX )
{
	float fReturnValue = fPosX ;
	float fPosY = m_pFlyLuxuryImg->GetHeight() - pImage->GetHeight();
	graphics.DrawImage( pImage , fReturnValue ,fPosY , pImage->GetWidth() , pImage->GetHeight() ) ;
	fReturnValue += pImage->GetWidth() ;
	return fReturnValue ;
}

float CFlyLuxuryWnd::DrawGiftName( Graphics& graphics , CString& strTemp , float fPosX ) 
{
	float fReturnValue = fPosX ;
	FontFamily  fontFamily( m_strFont.c_str() );
	Font        font(&fontFamily, 32, FontStyleBoldItalic, UnitPixel);
	PointF      pointF( fPosX , 0.0f );
	RectF boundRect;
	graphics.MeasureString( strTemp , strTemp.GetLength() ,&font ,pointF , &boundRect ) ;
	float fOffsetY = m_pFlyLuxuryImg->GetHeight() - boundRect.Height;
	pointF.Y = fOffsetY;
	boundRect.Offset(0.0f, fOffsetY);

	//阴影
	SolidBrush solidbrushshadow(Color(0, 146, 221));
	graphics.DrawString(strTemp, -1, &font, PointF(pointF.X + 2, pointF.Y + 3), &solidbrushshadow);

	//描边
	SolidBrush solidbrushstroke(Color(255, 255, 255));
	StringFormat stringFormat(0, LANG_NEUTRAL);
	int nOffsetX[8] = {1,1,1,0,-1,-1,-1,0};
	int nOffsetY[8] = {-1,0,1,1,1,0,-1,-1};
	int nOffsetIndex = 0;
	do 
	{
		RectF rcTemp(boundRect);
		rcTemp.Offset(nOffsetX[nOffsetIndex], nOffsetY[nOffsetIndex]);
		graphics.DrawString(strTemp, -1, &font, rcTemp, &stringFormat, &solidbrushstroke);
		nOffsetIndex += 1;
	} while (nOffsetIndex < 8);

	LinearGradientBrush brush(boundRect, Color(255, 174, 0), Color(240, 0, 56), LinearGradientModeVertical);
	graphics.DrawString(strTemp, -1, &font, pointF, &brush);

	fReturnValue += boundRect.Width ;
	return fReturnValue ;
}

float CFlyLuxuryWnd::DrawSrcName( Graphics& graphics , CString& strTemp , float fPosX ) 
{
	float fReturnValue = fPosX ;
	FontFamily  fontFamily( m_strFont.c_str() );
	Font        font(&fontFamily, 30, FontStyleBoldItalic, UnitPixel);
	PointF      pointF( fPosX , 0.0f );
	RectF boundRect;
	graphics.MeasureString( strTemp , strTemp.GetLength() ,&font ,pointF , &boundRect ) ;
	float fOffsetY = m_pFlyLuxuryImg->GetHeight() - boundRect.Height;
	pointF.Y = fOffsetY;
	boundRect.Offset(0.0f, fOffsetY);

	//阴影
	SolidBrush solidbrushshadow(Color(255, 99, 143));
	graphics.DrawString(strTemp, -1, &font, PointF(pointF.X + 2, pointF.Y + 3), &solidbrushshadow);

	//描边
	SolidBrush solidbrushstroke(Color(159, 71, 0));
	StringFormat stringFormat(0, LANG_NEUTRAL);
	int nOffsetX[8] = {1,1,1,0,-1,-1,-1,0};
	int nOffsetY[8] = {-1,0,1,1,1,0,-1,-1};
	int nOffsetIndex = 0;
	do 
	{
		RectF rcTemp(boundRect);
		rcTemp.Offset(nOffsetX[nOffsetIndex], nOffsetY[nOffsetIndex]);
		graphics.DrawString(strTemp, -1, &font, rcTemp, &stringFormat, &solidbrushstroke);
		nOffsetIndex += 1;
	} while (nOffsetIndex < 8);

	LinearGradientBrush brush(boundRect, Color(255, 253, 226), Color(255, 207, 41), LinearGradientModeVertical);
	graphics.DrawString(strTemp, -1, &font, pointF, &brush);

	fReturnValue += boundRect.Width ;
	return fReturnValue ;

}

float CFlyLuxuryWnd::DrawSrcUUID( Graphics& graphics , CString& strTemp , float fPosX ) 
{
	float fReturnValue = fPosX ;
	FontFamily  fontFamily( m_strFont.c_str() );
	Font        font(&fontFamily, 24, FontStyleBoldItalic, UnitPixel);
	PointF      pointF( fPosX , 0.0f );
	RectF boundRect;
	graphics.MeasureString( strTemp , strTemp.GetLength() ,&font ,pointF , &boundRect ) ;
	float fOffsetY = m_pFlyLuxuryImg->GetHeight() - boundRect.Height;
	pointF.Y = fOffsetY;
	boundRect.Offset(0.0f, fOffsetY);

	//阴影
	SolidBrush solidbrushshadow(Color(255, 99, 143));
	graphics.DrawString(strTemp, -1, &font, PointF(pointF.X + 2, pointF.Y + 3), &solidbrushshadow);

	//描边
	SolidBrush solidbrushstroke(Color(159, 71, 0));
	StringFormat stringFormat(0, LANG_NEUTRAL);
	int nOffsetX[8] = {1,1,1,0,-1,-1,-1,0};
	int nOffsetY[8] = {-1,0,1,1,1,0,-1,-1};
	int nOffsetIndex = 0;
	do 
	{
		RectF rcTemp(boundRect);
		rcTemp.Offset(nOffsetX[nOffsetIndex], nOffsetY[nOffsetIndex]);
		graphics.DrawString(strTemp, -1, &font, rcTemp, &stringFormat, &solidbrushstroke);
		nOffsetIndex += 1;
	} while (nOffsetIndex < 8);

	LinearGradientBrush brush(boundRect, Color(255, 253, 226), Color(255, 207, 41), LinearGradientModeVertical);
	graphics.DrawString(strTemp, -1, &font, pointF, &brush);

	fReturnValue += boundRect.Width ;
	return fReturnValue ;
}

float CFlyLuxuryWnd::DrawSendTo(  Graphics& graphics , CString& strTemp , float fPosX ) 
{		
	float fReturnValue = fPosX ;
	FontFamily  fontFamily( m_strFont.c_str() );
	Font        font(&fontFamily, 23, FontStyleBoldItalic, UnitPixel);
	PointF      pointF( fPosX , 0.0f );
	RectF boundRect;
	graphics.MeasureString( strTemp , strTemp.GetLength() ,&font ,pointF , &boundRect ) ;
	float fOffsetY = m_pFlyLuxuryImg->GetHeight() - boundRect.Height;
	pointF.Y = fOffsetY;
	boundRect.Offset(0.0f, fOffsetY);

	//阴影
	SolidBrush solidbrushshadow(Color(226, 133, 0));
	graphics.DrawString(strTemp, -1, &font, PointF(pointF.X + 2, pointF.Y + 2), &solidbrushshadow);

	//描边
	SolidBrush solidbrushstroke(Color(255, 255, 255));
	StringFormat stringFormat(0, LANG_NEUTRAL);
	int nOffsetX[8] = {1,1,1,0,-1,-1,-1,0};
	int nOffsetY[8] = {-1,0,1,1,1,0,-1,-1};
	int nOffsetIndex = 0;
	do 
	{
		RectF rcTemp(boundRect);
		rcTemp.Offset(nOffsetX[nOffsetIndex], nOffsetY[nOffsetIndex]);
		graphics.DrawString(strTemp, -1, &font, rcTemp, &stringFormat, &solidbrushstroke);
		nOffsetIndex += 1;
	} while (nOffsetIndex < 8);

	LinearGradientBrush brush(boundRect, Color(255, 51, 109), Color(255, 51, 109), LinearGradientModeVertical);
	graphics.DrawString(strTemp, -1, &font, pointF, &brush);

	fReturnValue += boundRect.Width ;
	return fReturnValue ;
}

float CFlyLuxuryWnd::DrawDstName( Graphics& graphics , CString& strTemp , float fPosX ) 
{
	float fReturnValue = fPosX ;
	FontFamily  fontFamily( m_strFont.c_str() );
	Font        font(&fontFamily, 27, FontStyleBoldItalic, UnitPixel);
	PointF      pointF( fPosX , 0.0f );
	RectF boundRect;
	graphics.MeasureString( strTemp , strTemp.GetLength() ,&font ,pointF , &boundRect ) ;
	float fOffsetY = m_pFlyLuxuryImg->GetHeight() - boundRect.Height;
	pointF.Y = fOffsetY;
	boundRect.Offset(0.0f, fOffsetY);

	//阴影
	SolidBrush solidbrushshadow(Color(255, 99, 143));
	graphics.DrawString(strTemp, -1, &font, PointF(pointF.X + 2, pointF.Y + 3), &solidbrushshadow);

	//描边
	SolidBrush solidbrushstroke(Color(159, 71, 0));
	StringFormat stringFormat(0, LANG_NEUTRAL);
	int nOffsetX[8] = {1,1,1,0,-1,-1,-1,0};
	int nOffsetY[8] = {-1,0,1,1,1,0,-1,-1};
	int nOffsetIndex = 0;
	do 
	{
		RectF rcTemp(boundRect);
		rcTemp.Offset(nOffsetX[nOffsetIndex], nOffsetY[nOffsetIndex]);
		graphics.DrawString(strTemp, -1, &font, rcTemp, &stringFormat, &solidbrushstroke);
		nOffsetIndex += 1;
	} while (nOffsetIndex < 8);

	LinearGradientBrush brush(boundRect, Color(255, 253, 226), Color(255, 207, 41), LinearGradientModeVertical);
	graphics.DrawString(strTemp, -1, &font, pointF, &brush);

	fReturnValue += boundRect.Width ;
	return fReturnValue ;
}

float CFlyLuxuryWnd::DrawDstUUID( Graphics& graphics , CString& strTemp , float fPosX ) 
{
	float fReturnValue = fPosX ;
	FontFamily  fontFamily( m_strFont.c_str() );
	Font        font(&fontFamily, 24, FontStyleBoldItalic, UnitPixel);
	PointF      pointF( fPosX , 0.0f );
	RectF boundRect;
	graphics.MeasureString( strTemp , strTemp.GetLength() ,&font ,pointF , &boundRect ) ;
	float fOffsetY = m_pFlyLuxuryImg->GetHeight() - boundRect.Height;
	pointF.Y = fOffsetY;
	boundRect.Offset(0.0f, fOffsetY);

	//阴影
	SolidBrush solidbrushshadow(Color(255, 99, 143));
	graphics.DrawString(strTemp, -1, &font, PointF(pointF.X + 2, pointF.Y + 3), &solidbrushshadow);

	//描边
	SolidBrush solidbrushstroke(Color(159, 71, 0));
	StringFormat stringFormat(0, LANG_NEUTRAL);
	int nOffsetX[8] = {1,1,1,0,-1,-1,-1,0};
	int nOffsetY[8] = {-1,0,1,1,1,0,-1,-1};
	int nOffsetIndex = 0;
	do 
	{
		RectF rcTemp(boundRect);
		rcTemp.Offset(nOffsetX[nOffsetIndex], nOffsetY[nOffsetIndex]);
		graphics.DrawString(strTemp, -1, &font, rcTemp, &stringFormat, &solidbrushstroke);
		nOffsetIndex += 1;
	} while (nOffsetIndex < 8);

	LinearGradientBrush brush(boundRect, Color(255, 253, 226), Color(255, 207, 41), LinearGradientModeVertical);
	graphics.DrawString(strTemp, -1, &font, pointF, &brush);

	fReturnValue += boundRect.Width ;
	return fReturnValue ;
}

float CFlyLuxuryWnd::DrawCount( Graphics& graphics , uint16 unCount ,  CString& strGiftUnit ,float fPosX ) 
{
	CString strTemp ;
	strTemp.Format( L"%u%s" , unCount , strGiftUnit.GetBuffer(0) ) ;

	float fReturnValue = fPosX ;
	FontFamily  fontFamily( m_strFont.c_str() );
	Font        font(&fontFamily, 30, FontStyleBoldItalic, UnitPixel);
	PointF      pointF( fPosX , 0.0f );
	RectF boundRect;
	graphics.MeasureString( strTemp , strTemp.GetLength() ,&font ,pointF , &boundRect ) ;
	float fOffsetY = m_pFlyLuxuryImg->GetHeight() - boundRect.Height;
	pointF.Y = fOffsetY;
	boundRect.Offset(0.0f, fOffsetY);

	//阴影
	SolidBrush solidbrushshadow(Color(226, 133, 0));
	graphics.DrawString(strTemp, -1, &font, PointF(pointF.X + 2, pointF.Y + 3), &solidbrushshadow);

	//描边
	SolidBrush solidbrushstroke(Color(255, 255, 255));
	StringFormat stringFormat(0, LANG_NEUTRAL);
	int nOffsetX[8] = {1,1,1,0,-1,-1,-1,0};
	int nOffsetY[8] = {-1,0,1,1,1,0,-1,-1};
	int nOffsetIndex = 0;
	do 
	{
		RectF rcTemp(boundRect);
		rcTemp.Offset(nOffsetX[nOffsetIndex], nOffsetY[nOffsetIndex]);
		graphics.DrawString(strTemp, -1, &font, rcTemp, &stringFormat, &solidbrushstroke);
		nOffsetIndex += 1;
	} while (nOffsetIndex < 8);

	LinearGradientBrush brush(boundRect, Color(255, 51, 109), Color(255, 51, 109), LinearGradientModeVertical);
	graphics.DrawString(strTemp, -1, &font, pointF, &brush);

	fReturnValue += boundRect.Width ;
	return fReturnValue ;
}

float CFlyLuxuryWnd::DrawTime( Graphics& graphics , CString& strTemp , float fPosX ) 
{
	float fReturnValue = fPosX ;
	SolidBrush  brush(Color(113, 33, 33));
	FontFamily  fontFamily( m_strFont.c_str() );
	Font        font(&fontFamily, 22, FontStyleBold, UnitPixel);
	PointF      pointF( fPosX , 17.0f );
	RectF boundRect;
	graphics.MeasureString( strTemp , strTemp.GetLength() ,&font ,pointF , &boundRect ) ;
	graphics.DrawString( strTemp , -1, &font, pointF, &brush );
	fReturnValue += boundRect.Width ;
	return fReturnValue ;
}

void CFlyLuxuryWnd::DrawWithoutGift( Graphics& graphics , PaodaoInfo& PaodaoInfo , float& fReturnValue )
{
	fReturnValue = DrawImage( graphics, m_pFlyLuxuryImg, fReturnValue );
	fReturnValue = DrawSrcName( graphics , PaodaoInfo.strSrcName , fReturnValue ) ;
	fReturnValue = DrawSrcUUID( graphics , PaodaoInfo.strSrcUUID , fReturnValue ) ;
	fReturnValue = DrawSendTo( graphics , PaodaoInfo.strSendDesc , fReturnValue );
	fReturnValue = DrawDstName( graphics , PaodaoInfo.strDstName , fReturnValue ) ;
	fReturnValue = DrawDstUUID( graphics , PaodaoInfo.strDstUUID , fReturnValue ) ;
	fReturnValue = DrawCount( graphics , PaodaoInfo.unCount , PaodaoInfo.strUnitName ,fReturnValue ) ;
	fReturnValue = DrawGiftName( graphics , PaodaoInfo.strGiftName , fReturnValue ) ;
}