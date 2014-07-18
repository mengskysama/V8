#include "stdafx.h"
#include "..\Room.h"
#include "PaodaoStaitc.h"
#include "..\UserMessageDefine.h"
#include "..\gift\GiftManager.h"
#include "..\..\CommonLibrary\include\utility\SystemHelper.h"
#include "..\..\CommonLibrary\include\ui\UIHelper.h"
#include "PaodaoBtnDlg.h"

// CPaodaoStaitc

IMPLEMENT_DYNAMIC(CPaodaoStaitc, CStatic)

CPaodaoStaitc::CPaodaoStaitc( CRoomParentDlg* pParentDlg )
:m_pParentDlg( pParentDlg )
{
	m_pPic = NULL;
	//m_pBackImage = NULL;
	m_pOldBitmap = NULL;
	m_nLastScrollSpeed = m_nScrollSpeed = 0 ;
	common::utility::systemhelper::GetDefaultFontName( m_strFont );
	m_pPaodaoBtnDlg = NULL ;
}

CPaodaoStaitc::~CPaodaoStaitc()
{
	if ( m_pPaodaoBtnDlg != NULL )
	{
		m_pPaodaoBtnDlg->DestroyWindow() ;
		delete m_pPaodaoBtnDlg ;
	}
	if ( m_pPic != NULL )
	{
		delete m_pPic ;
	}
	//if (m_pBackImage != NULL)
	//{
	//	delete m_pBackImage;
	//}
	for (std::vector<HDC>::iterator it = m_vecHDC.begin(); it != m_vecHDC.end(); ++it)
	{
		DeleteDC(*it);
	}
	m_vecHDC.clear();

	if (m_pOldBitmap && m_MainBitmap.GetSafeHandle() && m_MainDC.GetSafeHdc())
	{
		m_MainDC.SelectObject(m_pOldBitmap);
		m_MainBitmap.DeleteObject();
		m_MainDC.DeleteDC();
	}
}


BEGIN_MESSAGE_MAP(CPaodaoStaitc, CStatic)
	ON_WM_PAINT()
	ON_WM_TIMER()
	ON_WM_CREATE()
	ON_WM_MOUSEMOVE()
	ON_WM_SIZE()
	ON_MESSAGE(WM_MOUSELEAVE,OnMouseLeave)
	ON_WM_KILLFOCUS()
	ON_WM_ERASEBKGND()
	ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()

void CPaodaoStaitc::OnTimer(UINT_PTR nIDEvent)
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
		if ( m_pPic->IsAnimatedGIF() )
		{
			m_pPic->DrawFrameGIF() ;
		}
		
	}
	CStatic::OnTimer(nIDEvent);
}

int CPaodaoStaitc::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CStatic::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  在此添加您专用的创建代码
	m_pPaodaoBtnDlg = new CPaodaoBtnDlg(m_pParentDlg , this ) ;
	m_pPaodaoBtnDlg->Create( CPaodaoBtnDlg::IDD , this ) ;

	return 0;
}

void CPaodaoStaitc::OnSize(UINT nType, int cx, int cy) 
{
	CStatic::OnSize(nType, cx, cy);


	// OnSize automatically gets called during the setup of the control
	GetClientRect(m_rectClient);
	// destroy and recreate the main bitmap
	CClientDC dc(this);
	if (m_pOldBitmap && m_MainBitmap.GetSafeHandle() && m_MainDC.GetSafeHdc())
	{
		m_MainDC.SelectObject(m_pOldBitmap);
		m_MainBitmap.DeleteObject();
		m_MainBitmap.CreateCompatibleBitmap(&dc, m_rectClient.Width(), m_rectClient.Height());
		m_pOldBitmap = m_MainDC.SelectObject(&m_MainBitmap);
	}
	if ( !m_vPaodaoInfo.empty() )
	{
		PaodaoInfo paodaoinfo ;
		paodaoinfo = m_vPaodaoInfo.front() ;
		Measure( paodaoinfo , false ) ;
		m_nScrollPos -= (m_nOldWidth - cx);
		//InvalidateCtrl() ;
	}
	m_nOldWidth = cx;
	InvalidateCtrl() ;	
}

void CPaodaoStaitc::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	dc.BitBlt(0, 0, m_rectClient.Width(), m_rectClient.Height(), &m_MainDC, 0, 0, SRCCOPY);	
}

void CPaodaoStaitc::Measure( PaodaoInfo& paodaoinfo , bool bInit ) 
{
	CPaintDC dc(this) ;
	Graphics    graphics(dc);	
	if ( bInit )
	{
		if ( m_pPic != NULL )
		{
			delete m_pPic ;
			m_pPic = NULL ;
		}
		m_pPic = new common::ui::ImageEx( paodaoinfo.strLargeImagePath ) ;
		if ( m_pPic == NULL )
		{
			return ;
		}

		//if ( m_pBackImage != NULL)
		//{
		//	delete m_pBackImage;
		//	m_pBackImage = NULL;
		//}
		//std::wstring strPath = common::utility::systemhelper::Get179AppPath() + L"resource\\other\\Room_PaodaoBk.png";
		//m_pBackImage = new common::ui::ImageEx(strPath.c_str());
		//if (m_pBackImage == NULL)
		//{
		//	return;
		//}

		for (std::vector<HDC>::iterator it = m_vecHDC.begin(); it != m_vecHDC.end(); ++it)
		{
			DeleteDC(*it);
		}

		m_vecHDC.clear();
	}
	m_nTextSize = 0 ;
	float fTextSize = 0.0f;
	m_nPicSize = MeasureImage( paodaoinfo.unCount ) ;
	fTextSize += MeasureSrcName( graphics , paodaoinfo.strSrcName ) ;
	m_nSrcNamePos = fTextSize + m_nPicSize ;
	fTextSize += MeasureSrcUUID( graphics , paodaoinfo.strSrcUUID ) ;
	fTextSize += MeasureSendTo( graphics , L"送给") ;
	fTextSize += MeasureDstName( graphics , paodaoinfo.strDstName ) ;
	fTextSize += MeasureDstUUID( graphics , paodaoinfo.strDstUUID ) ;
	fTextSize += MeasureCount( graphics , paodaoinfo.unCount , paodaoinfo.strUnitName ) ;
	fTextSize += MeasureGiftName( graphics , paodaoinfo.strGiftName ) ;
	if (  !paodaoinfo.strGiftWord.IsEmpty() )
	{
		fTextSize += MeasureGiftSay( graphics ) ;
		fTextSize += MeasureGiftWord( graphics , paodaoinfo.strGiftWord ) ;
	}	
	fTextSize += MeasureTime( graphics , paodaoinfo.strTime ) ;
	m_nTextSize = fTextSize + 1;//float 到 int 的误差
	if ( bInit )
	{
		m_nOldWidth = m_rectClient.Width();
		m_nScrollPos = m_rectClient.Width() + m_nTextSize + m_nPicSize ;
	}
}

void CPaodaoStaitc::InvalidateCtrl()
{
	CClientDC dc(this);

	// if we don't have one yet, set up a memory dc for the control 
	if (m_MainDC.GetSafeHdc() == NULL)
	{
		m_MainDC.CreateCompatibleDC(&dc);
		m_MainBitmap.CreateCompatibleBitmap(&dc, m_rectClient.Width(), m_rectClient.Height());
		m_pOldBitmap = m_MainDC.SelectObject(&m_MainBitmap);
	}
	DoScrollDraw( &dc ) ;
	InvalidateRect(m_rectClient);
}

void CPaodaoStaitc::DoScrollDraw( CDC *pDC ) 
{
	//LARGE_INTEGER freq, t1, t2;
	//QueryPerformanceFrequency(&freq);
	//QueryPerformanceCounter(&t1);
	m_MainDC.FillSolidRect(m_rectClient, RGB(219,226,235));

	// draw Credits on the hidden Picture	
	if ( !m_vPaodaoInfo.empty() )
	{
		PaodaoInfo paodaoinfo = m_vPaodaoInfo.front() ;

		int nPosX = m_nScrollPos - m_nTextSize - m_nPicSize ;
		if (nPosX > -(m_nTextSize+m_nPicSize) )
		{
			//Graphics    graphics(memDC);
			//Draw( graphics , paodaoinfo , nPosX ) ;
			if (  m_vPaodaoInfo.size() >= 2 && ( nPosX < ( m_rectClient.Width()  - m_nSrcNamePos) )  )
			{
				m_nScrollSpeed = (5*PAODAO_MOVE_SPEED) ;
			}
			else
			{
				m_nScrollSpeed = PAODAO_MOVE_SPEED ;
			}
		}
		else
		{
			m_nScrollPos = m_rectClient.Width() +m_nTextSize + m_nPicSize ;
			if ( m_vPaodaoInfo.size() >= 2 )
			{
				m_vPaodaoInfo.pop_front() ;
				paodaoinfo = m_vPaodaoInfo.front() ;			
				Measure( paodaoinfo , true ) ;

				FirstDraw();
			}
			if ( m_vPaodaoInfo.size() == 1 )
			{
				m_nScrollSpeed = PAODAO_MOVE_SPEED ;
			}
		}
	}

	//m_MainDC.BitBlt( 0 , 0 ,m_rectClient.Width() , m_rectClient.Height() , &memDC, 0, 0,  SRCPAINT);	
	int nPosX = m_nScrollPos - m_nTextSize - m_nPicSize ;
	if (m_vecHDC.size() > 0)
	{
		BitBlt(m_MainDC, nPosX, 0, m_rectClient.Width() - nPosX, m_rectClient.Height(), m_vecHDC[m_pPic->GetCurrentFramePos()], 0, 0, SRCCOPY);
	}

	//QueryPerformanceCounter(&t2);
	//double d = ((double)t2.QuadPart - (double)t1.QuadPart) / (double)freq.QuadPart;
	//TRACE(L"\r\n?????????????????? %f ??????????????????", d);
}

void CPaodaoStaitc::Draw( Graphics& graphics  , PaodaoInfo& PaodaoInfo , float fPosX ) 
{
	//LARGE_INTEGER freq, t1, t2;
	//QueryPerformanceFrequency(&freq);
	//QueryPerformanceCounter(&t1);

	float fReturnValue = fPosX ;
	fReturnValue = DrawImage( graphics , PaodaoInfo.unCount , fReturnValue ) ;
	fReturnValue = DrawSrcName( graphics , PaodaoInfo.strSrcName , fReturnValue ) ;
	//fReturnValue = DrawSrcUUID( graphics , PaodaoInfo.strSrcUUID , fReturnValue ) ;
	fReturnValue = DrawSendTo( graphics , CString(L"送给") , fReturnValue );
	fReturnValue = DrawDstName( graphics , PaodaoInfo.strDstName , fReturnValue ) ;
	//fReturnValue = DrawDstUUID( graphics , PaodaoInfo.strDstUUID , fReturnValue ) ;
	fReturnValue = DrawCount( graphics , PaodaoInfo.unCount , PaodaoInfo.strUnitName ,fReturnValue ) ;
	fReturnValue = DrawGiftName( graphics , PaodaoInfo.strGiftName , fReturnValue ) ;
	if ( !PaodaoInfo.strGiftWord.IsEmpty() )
	{
		fReturnValue = DrawGiftSay( graphics , fReturnValue ) ;
		fReturnValue = DrawGiftWord( graphics , PaodaoInfo.strGiftWord , fReturnValue ) ;
	}	
	fReturnValue = DrawTime( graphics , PaodaoInfo.strTime , fReturnValue ) ;

	//QueryPerformanceCounter(&t2);
	//double d = ((double)t2.QuadPart - (double)t1.QuadPart) / (double)freq.QuadPart;
	//TRACE(L"\r\n?????????????????? %f ??????????????????", d);
}

void CPaodaoStaitc::InsertPaoDaoText(CString& strSrcName , CString& strDstName , CString& strGiftWord ,uint32 unSrc179ID , 
					  uint32 unDst179ID , uint16 unGiftID ,uint16 unCount , CString& strTime ) 
{
	IGiftItem* pGiftItem = GiftManager::GetInstance()->SearchGiftByID( unGiftID ) ;
	if ( pGiftItem == NULL )
	{
		return ;
	}
	PaodaoInfo paodaoinfo ;
	paodaoinfo.strDstName = strDstName ;
	paodaoinfo.strSrcName = strSrcName ;
	paodaoinfo.strTime = strTime ;
	CString strTemp ;
	strTemp.Format( L"(%u)" , unSrc179ID ) ;
	paodaoinfo.strSrcUUID = strTemp ;
	strTemp.Format( L"(%u)" , unDst179ID ) ;
	paodaoinfo.strDstUUID = strTemp ;
	//paodaoinfo.strGiftName = pGiftItem->GetName().c_str() ;
	paodaoinfo.strGiftName = pGiftItem->GetRichName().c_str();			//奢侈礼物用完整名字
	paodaoinfo.strUnitName = pGiftItem->GetUnitName().c_str() ;
	paodaoinfo.unCount = unCount ;
	paodaoinfo.strLargeImagePath = pGiftItem->GetLargeImagePath().c_str() ;
	paodaoinfo.strGiftWord = strGiftWord ;
	if ( m_vPaodaoInfo.empty() )
	{
		Measure( paodaoinfo , true ) ;
		m_nScrollSpeed = PAODAO_MOVE_SPEED ;
	}
	m_vPaodaoInfo.push_back( paodaoinfo ) ;

	if (m_vPaodaoInfo.size() == 1)
	{
		FirstDraw();
	}

	SetTimer( PAODAO_DRAW_GIF_TIMER , PAODAO_DRAW_GIF_TIMER_INTERVAL , NULL ) ;
}

void CPaodaoStaitc::PauseMove() 
{
	m_nLastScrollSpeed = m_nScrollSpeed ;
	KillTimer( PAODAO_DRAW_GIF_TIMER ) ;
}

void CPaodaoStaitc::StartMove() 
{
	if (m_vPaodaoInfo.size() == 0)
	{
		return;
	}
	m_nScrollSpeed = m_nLastScrollSpeed ;
	SetTimer( PAODAO_DRAW_GIF_TIMER ,PAODAO_DRAW_GIF_TIMER_INTERVAL , NULL ) ;
}

void CPaodaoStaitc::OnDeleteUser( IUserItem * pUserItem )
{
	if ( m_pPaodaoBtnDlg != NULL )
	{
		m_pPaodaoBtnDlg->OnDeleteUser( pUserItem ) ;
	}
}

void CPaodaoStaitc::OnClickNameLink( CString& strName , uint32 unUUID )
{
	if ( m_pPaodaoBtnDlg != NULL )
	{
		m_pPaodaoBtnDlg->OnClickNameLink( strName ,unUUID ) ;
	}	
}

void CPaodaoStaitc::OnRoomRightSendToChange( CString& strSelText ) 
{
	if ( m_pPaodaoBtnDlg != NULL )
	{
		m_pPaodaoBtnDlg->OnRoomRightSendToChange( strSelText ) ;
	}	
}

void CPaodaoStaitc::PreSubclassWindow()
{
	// TODO: 在此添加专用代码和/或调用基类
	ModifyStyle( 0 , SS_NOTIFY ) ;
	CStatic::PreSubclassWindow();
}

float CPaodaoStaitc::MeasureImage( uint16 unCount ) 
{
	return m_pPic->GetWidth()* unCount ;
}

float CPaodaoStaitc::MeasureGiftName( Graphics& graphics , CString& strTemp ) 
{
	FontFamily  fontFamily( m_strFont.c_str() );
	Font        font(&fontFamily, 34, FontStyleBold, UnitPixel);
	PointF      pointF( 0 , 18.0f);
	RectF boundRect;
	graphics.MeasureString( strTemp , strTemp.GetLength() ,&font ,pointF , &boundRect ) ;
	return boundRect.Width ;
}

float CPaodaoStaitc::MeasureSrcName( Graphics& graphics , CString& strTemp ) 
{
	FontFamily  fontFamily( m_strFont.c_str() );
	Font        font(&fontFamily, 34, FontStyleBold, UnitPixel);
	PointF      pointF( 0 , 18.0f);
	RectF boundRect;
	graphics.MeasureString( strTemp , strTemp.GetLength() ,&font ,pointF , &boundRect ) ;
	return boundRect.Width ;
}

float CPaodaoStaitc::MeasureSrcUUID( Graphics& graphics , CString& strTemp ) 
{
	FontFamily  fontFamily( m_strFont.c_str() );
	Font        font(&fontFamily, 22, FontStyleBold, UnitPixel);
	PointF      pointF( 0 , 30.0f);
	RectF boundRect;
	graphics.MeasureString( strTemp , strTemp.GetLength() ,&font ,pointF , &boundRect ) ;
	return boundRect.Width ;
}

float CPaodaoStaitc::MeasureSendTo( Graphics& graphics , CString strTemp ) 
{
	FontFamily  fontFamily( m_strFont.c_str() );
	Font        font(&fontFamily, 22, FontStyleBold, UnitPixel);
	PointF      pointF( 0 , 30.0f);
	RectF boundRect;
	graphics.MeasureString( strTemp , strTemp.GetLength() ,&font ,pointF , &boundRect ) ;
	return boundRect.Width ;
}

float CPaodaoStaitc::MeasureDstName( Graphics& graphics , CString& strTemp ) 
{
	FontFamily  fontFamily( m_strFont.c_str() );
	Font        font(&fontFamily, 34, FontStyleBold, UnitPixel);
	PointF      pointF( 0 , 18.0f);
	RectF boundRect;
	graphics.MeasureString( strTemp , strTemp.GetLength() ,&font ,pointF , &boundRect ) ;
	return boundRect.Width ;
}

float CPaodaoStaitc::MeasureDstUUID( Graphics& graphics , CString& strTemp ) 
{
	FontFamily  fontFamily( m_strFont.c_str() );
	Font        font(&fontFamily, 22, FontStyleRegular, UnitPixel);
	PointF      pointF( 0 , 30.0f);
	RectF boundRect;
	graphics.MeasureString( strTemp , strTemp.GetLength() ,&font ,pointF , &boundRect ) ;
	return boundRect.Width ;
}

float CPaodaoStaitc::MeasureCount( Graphics& graphics , uint16 unCount , CString& strGiftUnit ) 
{
	CString strTemp ;
	strTemp.Format( L"%u%s" , unCount , strGiftUnit.GetBuffer(0) ) ;
	FontFamily  fontFamily( m_strFont.c_str() );
	Font        font(&fontFamily, 22, FontStyleBold, UnitPixel);
	PointF      pointF( 0 , 30.0f);
	RectF boundRect;
	graphics.MeasureString( strTemp , strTemp.GetLength() ,&font ,pointF , &boundRect ) ;
	return boundRect.Width ;
}

float CPaodaoStaitc::MeasureGiftSay(Graphics& graphics ) 
{
	RectF boundRect;
	CString strTemp = L"并对TA说：" ;
	FontFamily  fontFamily( m_strFont.c_str() );
	Font        font(&fontFamily, 34, FontStyleBold, UnitPixel);
	PointF      pointF( 0 , 18.0f);
	graphics.MeasureString( strTemp , strTemp.GetLength() ,&font ,pointF , &boundRect ) ;
	return boundRect.Width ;
}

float CPaodaoStaitc::MeasureGiftWord( Graphics& graphics , CString& strTemp ) 
{
	RectF boundRect;
	if ( !strTemp.IsEmpty() )
	{
		FontFamily  fontFamily( m_strFont.c_str() );
		Font        font(&fontFamily, 34, FontStyleBold, UnitPixel);
		PointF      pointF( 0 , 18.0f);
		graphics.MeasureString( strTemp , strTemp.GetLength() ,&font ,pointF , &boundRect ) ;
	}
	return boundRect.Width ;
}

float CPaodaoStaitc::MeasureTime( Graphics& graphics , CString& strTemp ) 
{
	FontFamily  fontFamily( m_strFont.c_str() );
	Font        font(&fontFamily, 22, FontStyleBold, UnitPixel);
	PointF      pointF( 0 , 30.0f);
	RectF boundRect;
	graphics.MeasureString( strTemp , strTemp.GetLength() ,&font ,pointF , &boundRect ) ;
	return boundRect.Width ;
}

float CPaodaoStaitc::DrawImage( Graphics& graphics , uint16 unCount , float fPosX ) 
{
	float fReturnValue = fPosX ;
	for ( uint16 i = 0 ;i < unCount ; ++i )
	{
		graphics.DrawImage( m_pPic , fReturnValue ,0.0f , m_pPic->GetWidth() , m_pPic->GetHeight() ) ;
		fReturnValue += m_pPic->GetWidth() ;
	}
	return fReturnValue ;
}

float CPaodaoStaitc::DrawGiftName( Graphics& graphics , CString& strTemp , float fPosX ) 
{
	float fReturnValue = fPosX ;
	SolidBrush  brush(Color(255, 59, 133));
	FontFamily  fontFamily( m_strFont.c_str() );
	Font        font(&fontFamily, 34, FontStyleBold, UnitPixel);
	PointF      pointF( fPosX , 3.0f );
	RectF boundRect;
	graphics.MeasureString( strTemp , strTemp.GetLength() ,&font ,pointF , &boundRect ) ;
	graphics.DrawString( strTemp , -1, &font, pointF, &brush );
	fReturnValue += boundRect.Width ;
	return fReturnValue ;
}

float CPaodaoStaitc::DrawSrcName( Graphics& graphics , CString& strTemp , float fPosX ) 
{
	float fReturnValue = fPosX ;
	SolidBrush  brush(Color(255, 59, 133));
	FontFamily  fontFamily( m_strFont.c_str() );
	Font        font(&fontFamily, 34, FontStyleBold, UnitPixel);
	PointF      pointF( fPosX , 4.0f );
	RectF boundRect;
	graphics.MeasureString( strTemp , strTemp.GetLength() ,&font ,pointF , &boundRect ) ;
	graphics.DrawString( strTemp , -1, &font, pointF, &brush );
	fReturnValue += boundRect.Width ;
	return fReturnValue ;
}

float CPaodaoStaitc::DrawSrcUUID( Graphics& graphics , CString& strTemp , float fPosX ) 
{
	float fReturnValue = fPosX ;
	SolidBrush  brush(Color(255, 122, 166));
	FontFamily  fontFamily( m_strFont.c_str() );
	Font        font(&fontFamily, 22, FontStyleBold, UnitPixel);
	PointF      pointF( fPosX , 17.0f );
	RectF boundRect;
	graphics.MeasureString( strTemp , strTemp.GetLength() ,&font ,pointF , &boundRect ) ;
	graphics.DrawString( strTemp , -1, &font, pointF, &brush );
	fReturnValue += boundRect.Width ;
	return fReturnValue ;
}

float CPaodaoStaitc::DrawSendTo(  Graphics& graphics , CString& strTemp , float fPosX ) 
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

float CPaodaoStaitc::DrawDstName( Graphics& graphics , CString& strTemp , float fPosX ) 
{
	float fReturnValue = fPosX ;
	SolidBrush  brush(Color(255, 120, 0));
	FontFamily  fontFamily( m_strFont.c_str() );
	Font        font(&fontFamily, 34, FontStyleBold, UnitPixel);
	PointF      pointF( fPosX , 3.0f );
	RectF boundRect;
	graphics.MeasureString( strTemp , strTemp.GetLength() ,&font ,pointF , &boundRect ) ;
	graphics.DrawString( strTemp , -1, &font, pointF, &brush );
	fReturnValue += boundRect.Width ;
	return fReturnValue ;
}

float CPaodaoStaitc::DrawDstUUID( Graphics& graphics , CString& strTemp , float fPosX ) 
{
	float fReturnValue = fPosX ;
	SolidBrush  brush(Color(255, 154, 65));
	FontFamily  fontFamily( m_strFont.c_str() );
	Font        font(&fontFamily, 22, FontStyleRegular, UnitPixel);
	PointF      pointF( fPosX ,  17.0f );
	RectF boundRect;
	graphics.MeasureString( strTemp , strTemp.GetLength() ,&font ,pointF , &boundRect ) ;
	graphics.DrawString( strTemp , -1, &font, pointF, &brush );
	fReturnValue += boundRect.Width ;
	return fReturnValue ;
}

float CPaodaoStaitc::DrawCount( Graphics& graphics , uint16 unCount ,  CString& strGiftUnit ,float fPosX ) 
{
	CString strTemp ;
	strTemp.Format( L"%u%s" , unCount , strGiftUnit.GetBuffer(0) ) ;
	float fReturnValue = fPosX ;
	SolidBrush  brush(Color(255, 59, 133));
	FontFamily  fontFamily( m_strFont.c_str() );
	Font        font(&fontFamily, 22, FontStyleBold, UnitPixel);
	PointF      pointF( fPosX , 17.0f );
	RectF boundRect;
	graphics.MeasureString( strTemp , strTemp.GetLength() ,&font ,pointF , &boundRect ) ;
	graphics.DrawString( strTemp , -1, &font, pointF, &brush );
	fReturnValue += boundRect.Width ;
	return fReturnValue ;
}

float CPaodaoStaitc::DrawGiftSay(Graphics& graphics , float fPosX ) 
{
	CString strTemp = L"并对TA说：" ;
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

float CPaodaoStaitc::DrawGiftWord( Graphics& graphics , CString& strTemp , float fPosX ) 
{
	float fReturnValue = fPosX ;
	SolidBrush  brush(Color(255, 59, 133));
	FontFamily  fontFamily( m_strFont.c_str() );
	Font        font(&fontFamily, 34, FontStyleBold, UnitPixel);
	PointF      pointF( fPosX , 3.0f );
	RectF boundRect;
	graphics.MeasureString( strTemp , strTemp.GetLength() ,&font ,pointF , &boundRect ) ;
	graphics.DrawString( strTemp , -1, &font, pointF, &brush );
	fReturnValue += boundRect.Width ;
	return fReturnValue ;
}

float CPaodaoStaitc::DrawTime( Graphics& graphics , CString& strTemp , float fPosX ) 
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

void CPaodaoStaitc::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	TRACKMOUSEEVENT tme ;
	tme.cbSize = sizeof( tme ) ;
	tme.dwFlags = TME_LEAVE;
	tme.dwHoverTime = 1 ;
	tme.hwndTrack = m_hWnd ;
	::TrackMouseEvent(&tme) ;
	PauseMove() ;
	CRect Rect ;
	GetWindowRect( &Rect ) ;
	m_pPaodaoBtnDlg->MoveWindow( Rect.right - 90 , Rect.bottom - 30 , 83 , 21 ) ;
	m_pPaodaoBtnDlg->ShowWindow( SW_SHOWNOACTIVATE ) ;
	//SetFocus() ;
	CStatic::OnMouseMove(nFlags, point);
}

HRESULT CPaodaoStaitc::OnMouseLeave( WPARAM wParam , LPARAM lParam )
{	
	common::ui::CCursorPos pt;
	CRect Rect ;
	GetClientRect( &Rect ) ;
	ClientToScreen( &Rect ) ;

	if ( !Rect.PtInRect( pt ) )
	{
		if (  m_pPaodaoBtnDlg != NULL && m_pPaodaoBtnDlg->GetSafeHwnd() != NULL && m_pPaodaoBtnDlg->IsWindowVisible() )
		{
			m_pPaodaoBtnDlg->ShowWindow( SW_HIDE ) ;
		}		
		StartMove() ;
	}
	return S_OK;
}


void CPaodaoStaitc::OnKillFocus(CWnd* pNewWnd)
{
	CStatic::OnKillFocus(pNewWnd);
	common::ui::CCursorPos pt;
	CRect Rect ;
	GetClientRect( &Rect ) ;
	ClientToScreen( &Rect ) ;

	if ( !Rect.PtInRect( pt ) )
	{
		if (  m_pPaodaoBtnDlg != NULL && m_pPaodaoBtnDlg->GetSafeHwnd() != NULL && m_pPaodaoBtnDlg->IsWindowVisible() )
		{
			m_pPaodaoBtnDlg->ShowWindow( SW_HIDE ) ;
		}		
		StartMove() ;
	}
	// TODO: 在此处添加消息处理程序代码
}

BOOL CPaodaoStaitc::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	return TRUE ;
	return CStatic::OnEraseBkgnd(pDC);
}

void CPaodaoStaitc::FirstDraw()
{
	//LARGE_INTEGER freq, t1, t2;
	//QueryPerformanceFrequency(&freq);
	//QueryPerformanceCounter(&t1);

	if (m_vPaodaoInfo.empty())
	{
		return;
	}

	PaodaoInfo paodaoinfo = m_vPaodaoInfo.front() ;

	CClientDC dc(this);
	UINT nFrames = m_pPic->GetFrameCounts();

	HDC hdcText;
	hdcText = CreateCompatibleDC(dc.GetSafeHdc());
	HBITMAP hBitMapText = CreateCompatibleBitmap(dc.GetSafeHdc(), m_nTextSize, m_rectClient.Height());
	HBITMAP hOldText = (HBITMAP)SelectObject(hdcText, hBitMapText);

	ImageAttributes imAtt;
	Graphics graphText(hdcText);

	graphText.SetTextRenderingHint(TextRenderingHintAntiAlias);
	graphText.Clear(Color(219,226,235));
	//graphText.DrawImage(m_pBackImage, RectF(m_rectClient.left, m_rectClient.top, m_rectClient.Width(), m_rectClient.Height()), 0, 0, m_pBackImage->GetWidth(), m_pBackImage->GetHeight(), UnitPixel, &imAtt);
	float fReturnValue = 0.0f ;
	DrawWithoutGift( graphText, paodaoinfo, fReturnValue );

	for (int i = 0; i < nFrames; i++)
	{
		HDC hdcMem;
		hdcMem = CreateCompatibleDC(dc.GetSafeHdc());
		HBITMAP hBitMap = CreateCompatibleBitmap(dc.GetSafeHdc(), m_nPicSize + m_nTextSize, m_rectClient.Height());
		HBITMAP hOld = (HBITMAP)SelectObject(hdcMem, hBitMap);

		//BitBlt(hdcMem, m_nPicSize, 0, m_nTextSize, m_rectClient.Height(), hdcText, 0, 0, SRCCOPY);
		Graphics graph(hdcMem);
		graph.Clear(Color(219,226,235));
		
		//graph.DrawImage(m_pBackImage, RectF(m_rectClient.left, m_rectClient.top, m_rectClient.Width(), m_rectClient.Height()), 0, 0, m_pBackImage->GetWidth(), m_pBackImage->GetHeight(), UnitPixel, &imAtt);
		//Draw(graph, paodaoinfo, 0);
		DrawImage(graph, paodaoinfo.unCount, 0);
		m_pPic->DrawNextFrame();

		BitBlt(hdcMem, m_nPicSize, 0, m_nTextSize, m_rectClient.Height(), hdcText, 0, 0, SRCCOPY);

		m_vecHDC.push_back(hdcMem);

		graph.ReleaseHDC(hdcMem);
		SelectObject(hdcMem, hOld);
		DeleteObject(hBitMap);
	}

	graphText.ReleaseHDC(hdcText);
	SelectObject(hdcText, hOldText);
	DeleteObject(hBitMapText);
	DeleteDC(hdcText);

	//QueryPerformanceCounter(&t2);
	//double d = ((double)t2.QuadPart - (double)t1.QuadPart) / (double)freq.QuadPart;
	//TRACE(L"\r\n---------------------- %f --------------------", d);
}

void CPaodaoStaitc::DrawWithoutGift( Graphics& graphics , PaodaoInfo& PaodaoInfo , float& fReturnValue )
{
	//fReturnValue = DrawImage( graphics , PaodaoInfo.unCount , fReturnValue ) ;
	fReturnValue = DrawSrcName( graphics , PaodaoInfo.strSrcName , fReturnValue ) ;
	//fReturnValue = DrawSrcUUID( graphics , PaodaoInfo.strSrcUUID , fReturnValue ) ;
	fReturnValue = DrawSendTo( graphics , CString(L"送给") , fReturnValue );
	fReturnValue = DrawDstName( graphics , PaodaoInfo.strDstName , fReturnValue ) ;
	//fReturnValue = DrawDstUUID( graphics , PaodaoInfo.strDstUUID , fReturnValue ) ;
	fReturnValue = DrawCount( graphics , PaodaoInfo.unCount , PaodaoInfo.strUnitName ,fReturnValue ) ;
	fReturnValue = DrawGiftName( graphics , PaodaoInfo.strGiftName , fReturnValue ) ;
	if ( !PaodaoInfo.strGiftWord.IsEmpty() )
	{
		fReturnValue = DrawGiftSay( graphics , fReturnValue ) ;
		fReturnValue = DrawGiftWord( graphics , PaodaoInfo.strGiftWord , fReturnValue ) ;
	}	
	fReturnValue = DrawTime( graphics , PaodaoInfo.strTime , fReturnValue ) ;
}

void CPaodaoStaitc::UpdateGiftList()
{
	if (m_pPaodaoBtnDlg != NULL)
	{
		m_pPaodaoBtnDlg->UpdateGiftList();
	}
}

void CPaodaoStaitc::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	SetFocus();

	CStatic::OnLButtonDown(nFlags, point);
}
