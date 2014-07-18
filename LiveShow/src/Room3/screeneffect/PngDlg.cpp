// PngDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "..\Room.h"
#include "PngDlg.h"
#include "..\RoomParentDlg.h"
#include "..\..\include\utility\SystemHelper.h"
#include "..\..\include\utility\FileHelper.h"
#include "utility/URLEncode.h"
// CPngDlg 对话框

IMPLEMENT_DYNAMIC(CPngDlg, CDialog)

CPngDlg::CPngDlg( CRoomParentDlg* pParentDlg , CWnd* pParent /*=NULL*/)
	: CDialog(CPngDlg::IDD, pParent),
	m_pParentDlg( pParentDlg)
{
	UpdateLayeredWindow = NULL ;
	m_nPictureIndex = 1 ;
	m_nCount = 1 ;
	m_bFinish = false;
	m_hInst = NULL;
}

CPngDlg::~CPngDlg()
{
	if (m_hInst)
	{
		FreeLibrary(m_hInst);
	}
}

void CPngDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CPngDlg, CDialog)
	ON_WM_CREATE()
	ON_WM_TIMER()
	ON_WM_ACTIVATE()
END_MESSAGE_MAP()


// CPngDlg 消息处理程序

BOOL CPngDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

int CPngDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  在此添加您专用的创建代码
	LONG dwstyle = GetWindowLong(GetSafeHwnd(),GWL_EXSTYLE) ;
	dwstyle |= WS_EX_LAYERED ;
	SetWindowLong( GetSafeHwnd() , GWL_EXSTYLE , dwstyle) ;
	m_hInst = LoadLibrary(_T("User32.DLL") ) ;

	m_blend.BlendOp = 0 ;
	m_blend.BlendFlags = 0 ;
	m_blend.AlphaFormat = 1 ;
	m_blend.SourceConstantAlpha = 255 ;
	UpdateLayeredWindow = (FUpdateLayeredWindow)GetProcAddress( m_hInst ,("UpdateLayeredWindow") ) ;
	return 0;
}

void CPngDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	switch ( nIDEvent )
	{
	case ROOM_DRAW_FIREWORK_TIMER_ID:
		{
			if ( m_nCount >= ROOM_DRAW_MAX_COUNT )
			{
				KillTimer( ROOM_DRAW_FIREWORK_TIMER_ID ) ;
				DrawWhiteGround() ;
				ShowWindow( SW_HIDE ) ;
				m_bFinish = true;
			}
			else
			{
				TimerDrawFireWork() ; 
			}			  
		}
		break;
	case ROOM_DRAW_SALVO_TIMER_ID:
		{
			if ( m_nCount >= ROOM_DRAW_MAX_COUNT )
			{
				KillTimer( ROOM_DRAW_SALVO_TIMER_ID ) ;
				DrawWhiteGround() ;
				ShowWindow( SW_HIDE ) ;
				m_bFinish = true;
			}
			else
			{
				TimerDrawSalvo() ;
			}			
		}
		break;
	case ROOM_DRAW_PLUTUS_TIMER_ID:
		{
			if ( m_nCount >= ROOM_DRAW_MAX_COUNT )
			{
				KillTimer( ROOM_DRAW_PLUTUS_TIMER_ID ) ;
				DrawWhiteGround() ;
				ShowWindow( SW_HIDE ) ;
				m_bFinish = true;
			}
			else
			{
				TimerDrawPlutus() ;
			}
		}
		break;
	default:break;
	}
	CDialog::OnTimer(nIDEvent);
}

void CPngDlg::Init( ENM_DRAW_TYPE nType ) 
{
	KillTimer( ROOM_DRAW_PLUTUS_TIMER_ID ) ;
	KillTimer( ROOM_DRAW_FIREWORK_TIMER_ID ) ;
	KillTimer( ROOM_DRAW_SALVO_TIMER_ID ) ;
	DrawWhiteGround() ;
	m_nCount = 1 ;
	m_nPictureIndex = 1 ;
	m_bFinish = false;
	switch( nType )
	{
	case DRAW_FIREWORK:
		{
			SetTimer( ROOM_DRAW_FIREWORK_TIMER_ID ,ROOM_DRAW_FIREWORK_INTERVAL , NULL ) ;
		}
		break;
	case DRAW_SLAVO:
		{
			SetTimer( ROOM_DRAW_SALVO_TIMER_ID , ROOM_DRAW_FIREWORK_INTERVAL , NULL ) ;
		}
		break;
	case DRAW_PLUTUS:
		{
			SetTimer( ROOM_DRAW_PLUTUS_TIMER_ID , ROOM_DRAW_PLUTUS_INTERVAL , NULL ) ;
		}
	default:break;
	}
}

//绘制烟花
void CPngDlg::TimerDrawFireWork() 
{
	std::wstring strModulePath = common::utility::systemhelper::Get179AppPath();
	common::CURLEncode urlEncode;
	strModulePath = urlEncode.URLEncodeLocalPath(strModulePath);
	std::wstring strFireWorkPath = strModulePath + FIREWORK_DIR;
	CString strTemp ;
	strTemp.Format( L"%d.png" ,m_nPictureIndex ) ;
	strFireWorkPath += strTemp.GetBuffer() ;
	if ( !common::utility::filehelper::IsExist( strFireWorkPath.c_str() ) )
	{
		return ;
	}
	++m_nPictureIndex ;
	if ( m_nPictureIndex == 22 )
	{
		++m_nCount ;
		m_nPictureIndex = 1 ;
	}
	Draw(strFireWorkPath) ;

}

//绘制礼炮
void CPngDlg::TimerDrawSalvo() 
{
	std::wstring strModulePath = common::utility::systemhelper::Get179AppPath();
	common::CURLEncode urlEncode;
	strModulePath = urlEncode.URLEncodeLocalPath(strModulePath);
	std::wstring strFireWorkPath = strModulePath + FIREWORK_DIR;
	CString strTemp ;
	strTemp.Format( L"%d.png" ,m_nPictureIndex ) ;
	strFireWorkPath += strTemp.GetBuffer() ;
	if ( !common::utility::filehelper::IsExist( strFireWorkPath.c_str() ) )
	{
		return ;
	}
	++m_nPictureIndex ;
	if ( m_nPictureIndex == 22 )
	{
		++m_nCount ;
		m_nPictureIndex = 1 ;
	}
	Draw(strFireWorkPath) ;
}

//绘制财神
void CPngDlg::TimerDrawPlutus() 
{
	std::wstring strModulePath = common::utility::systemhelper::Get179AppPath();
	common::CURLEncode urlEncode;
	strModulePath = urlEncode.URLEncodeLocalPath(strModulePath);
	std::wstring strFireWorkPath = strModulePath + FIREWORK_DIR;
	CString strTemp ;
	strTemp.Format( L"a%d.png" ,m_nPictureIndex ) ;
	strFireWorkPath += strTemp.GetBuffer() ;
	if ( !common::utility::filehelper::IsExist( strFireWorkPath.c_str() ) )
	{
		return ;
	}
	++m_nPictureIndex ;
	if ( m_nPictureIndex == 9 )
	{
		++m_nCount ;
		m_nPictureIndex = 1 ;
	}
	Draw(strFireWorkPath) ;
}

//画白色背景
void CPngDlg::DrawWhiteGround() 
{
	CRect rect ;
	GetWindowRect( &rect ) ;
	CDC *pCDC = GetDC() ;
	HDC hdcTemp = pCDC->GetSafeHdc();
	HDC hdcMemory = CreateCompatibleDC( hdcTemp ) ;
	HBITMAP hBitMap = CreateCompatibleBitmap( hdcTemp , rect.Width(), rect.Height() ) ;
	SelectObject( hdcMemory , hBitMap ) ;
	POINT ptWinPos = { rect.left,rect.top } ;
	POINT ptSrc= {0,0} ;
	SIZE sizeWindow = {rect.Width() , rect.Height() } ;
	UpdateLayeredWindow( m_hWnd , hdcTemp , &ptWinPos ,&sizeWindow , hdcMemory ,&ptSrc , 0 ,&m_blend , 2 ) ;
	DeleteObject( hBitMap) ;
	DeleteDC( hdcMemory ) ;
	ReleaseDC(pCDC);
}

void CPngDlg::Draw( std::wstring& strFilePath )
{
	Image image( strFilePath.c_str() ) ;

	CRect rect ;
	GetWindowRect( &rect ) ;
	CDC *pCDC = GetDC() ;
	HDC hdcTemp = pCDC->GetSafeHdc();
	HDC hdcMemory = CreateCompatibleDC( hdcTemp ) ;
	HBITMAP hBitMap = CreateCompatibleBitmap( hdcTemp , rect.Width(), rect.Height() ) ;
	HBITMAP hOldBitMap = (HBITMAP)SelectObject( hdcMemory , hBitMap ) ;
	Graphics graph( hdcMemory ) ;
	//使用GDI载入PNG图片
	graph.DrawImage( &image , 0 , 0 ,image.GetWidth() , image.GetHeight() ) ;

	BOOL bResult = BitBlt( hdcTemp , 0 , 0 , rect.Width() , rect.Height() , hdcMemory , 0 , 0 , SRCCOPY ) ;

	POINT ptWinPos = { rect.left,rect.top } ;
	POINT ptSrc= {0,0} ;
	SIZE sizeWindow = {rect.Width() , rect.Height() } ;
	UpdateLayeredWindow( m_hWnd , hdcTemp , &ptWinPos ,&sizeWindow , hdcMemory ,&ptSrc , 0 ,&m_blend , 2 ) ;
	graph.ReleaseHDC( hdcMemory ) ;
	SelectObject( hdcMemory , hOldBitMap ) ;
	DeleteObject( hBitMap) ;
	DeleteDC( hdcMemory ) ;
	ReleaseDC( pCDC ) ;
}

void CPngDlg::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
{
	CDialog::OnActivate(nState, pWndOther, bMinimized);
	//if ( m_pParentDlg->IsIconic() )
	//{
	//	m_pParentDlg->SetActiveWindow() ;
	//}
	// TODO: 在此处添加消息处理程序代码
}
