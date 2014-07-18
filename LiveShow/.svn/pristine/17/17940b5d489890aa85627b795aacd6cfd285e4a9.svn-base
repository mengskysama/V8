// EnteringRoomDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "../Hall.h"
#include "EnteringRoomDlg.h"
#include "skin_i.h"
#include "SkinUtil.h"
#include "HallDlg.h"
#include "../GlobalManager.h"
#include "..\CommonLibrary\include\ui\C179MsgBox.h"

// CEnteringRoomDlg 对话框

IMPLEMENT_DYNAMIC(CEnteringRoomDlg, CDialog)

CEnteringRoomDlg::CEnteringRoomDlg(CHallDlg *hallDlg,CEnterRoomRequest *enterRoom,CWnd* pParent /*=NULL*/)
	: CDialog(CEnteringRoomDlg::IDD, pParent)
{
	m_pHallDlg = hallDlg;
	m_pPic = NULL ; 
	m_enterRoom = enterRoom;
	m_hrgn = NULL;
	m_imgTopLeft = NULL;
	m_imgTopMid = NULL;
	m_imgTopRight = NULL;
	m_imgMidLeft = NULL;
	m_imgMidRight = NULL;
	m_imgBottomLeft = NULL;
	m_imgBottomMid = NULL;
	m_imgBottomRight = NULL;
}

CEnteringRoomDlg::~CEnteringRoomDlg()
{
	if ( m_pPic != NULL )
	{
		delete m_pPic ;
		m_pPic = NULL;
	}

	if ( m_imgTopLeft != NULL )
	{
		delete m_imgTopLeft ;
		m_imgTopLeft = NULL ;
	}
	if ( m_imgTopMid != NULL )
	{
		delete m_imgTopMid ;
		m_imgTopMid = NULL ;
	}
	if ( m_imgTopRight != NULL )
	{
		delete m_imgTopRight ;
		m_imgTopRight = NULL ;
	}
	if ( m_imgMidLeft != NULL )
	{
		delete m_imgMidLeft ;
		m_imgMidLeft = NULL ;
	}
	if ( m_imgMidRight != NULL )
	{
		delete m_imgMidRight ;
		m_imgMidRight = NULL ;
	}
	if ( m_imgBottomLeft != NULL )
	{
		delete m_imgBottomLeft ;
		m_imgBottomLeft = NULL ;
	}
	if ( m_imgBottomMid != NULL )
	{
		delete m_imgBottomMid ;
		m_imgBottomMid = NULL ;
	}
	if ( m_imgBottomRight != NULL )
	{
		delete m_imgBottomRight ;
		m_imgBottomRight = NULL ;
	}

	if (m_hrgn)
	{
		DeleteObject(m_hrgn);
	}
}

void CEnteringRoomDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CEnteringRoomDlg, CDialog)
	ON_SKIN_MESSAGE
	ON_WM_CREATE()
	ON_WM_SHOWWINDOW()
	ON_WM_PAINT()
	ON_WM_TIMER()
	ON_WM_DESTROY()
END_MESSAGE_MAP()


// CEnteringRoomDlg 消息处理程序

int CEnteringRoomDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  在此添加您专用的创建代码
	//DragonZ:去掉边框
	CString strTemp = common::utility::systemhelper::Get179AppPath().c_str();	
	CString strPath;
	strPath.Format(L"%sresource\\defer\\Hall_imgEnteringRoomTopLeft.png", strTemp);
	m_imgTopLeft = new ImageEx(strPath.GetBuffer());
	strPath.Format(L"%sresource\\defer\\Hall_imgEnteringRoomTopMid.png", strTemp);
	m_imgTopMid = new ImageEx(strPath.GetBuffer());
	strPath.Format(L"%sresource\\defer\\Hall_imgEnteringRoomTopRight.png", strTemp);
	m_imgTopRight = new ImageEx(strPath.GetBuffer());
	strPath.Format(L"%sresource\\defer\\Hall_imgEnteringRoomMidLeft.png", strTemp);
	m_imgMidLeft = new ImageEx(strPath.GetBuffer());
	strPath.Format(L"%sresource\\defer\\Hall_imgEnteringRoomMidRight.png", strTemp);
	m_imgMidRight = new ImageEx(strPath.GetBuffer());
	strPath.Format(L"%sresource\\defer\\Hall_imgEnteringRoomBottomLeft.png", strTemp);
	m_imgBottomLeft = new ImageEx(strPath.GetBuffer());
	strPath.Format(L"%sresource\\defer\\Hall_imgEnteringRoomBottomMid.png", strTemp);
	m_imgBottomMid = new ImageEx(strPath.GetBuffer());
	strPath.Format(L"%sresource\\defer\\Hall_imgEnteringRoomBottomRight.png", strTemp);
	m_imgBottomRight = new ImageEx(strPath.GetBuffer());
    
	return 0;
}

BOOL CEnteringRoomDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	//屏蔽ESC按键和ENTER键
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

LRESULT CEnteringRoomDlg::OnSkinMessage(WPARAM wParam, LPARAM lParam)
{
	switch(wParam)
	{

		
	default:break;
	}

	return TRUE;
}

void CEnteringRoomDlg::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialog::OnShowWindow(bShow, nStatus);
	if ( bShow )
	{		
		CString strDeferPicPath = DeferPicPathMgr->GetDefaultPicPathRandom() ;
		if ( m_pPic != NULL )
		{
			delete m_pPic ;
			m_pPic = NULL ;
		}
		m_pPic = new ImageEx( strDeferPicPath ) ;
		if ( m_pPic )
		{
			SetWindowPos( NULL , 0 , 0 , m_pPic->GetWidth() + m_imgMidLeft->GetWidth() * 2 + 1, 
				          m_pPic->GetHeight() + m_imgTopMid->GetHeight() * 2 + 1, SWP_NOMOVE ) ;
			/*SetWindowPos( NULL , 0 , 0 , m_pPic->GetWidth(), 
				m_pPic->GetHeight(), SWP_NOMOVE ) ;*/
			if (m_hrgn)
			{
				DeleteObject(m_hrgn);
			}
			CRect rc(0,0,0,0);
			GetClientRect(&rc);
			m_hrgn = CreateRoundRectRgn(rc.left, rc.top, rc.right, rc.bottom, 4, 4);
			SetWindowRgn(m_hrgn, TRUE);
			
			SetTimer( ENTER_ROOM_TIMER , 100 , NULL ) ;
		}
	}
	else
	{
		KillTimer( ENTER_ROOM_TIMER ) ;
	}
	// TODO: 在此处添加消息处理程序代码
	CenterWindow(m_pHallDlg); //居中显示
}

void CEnteringRoomDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: 在此处添加消息处理程序代码
	// 不为绘图消息调用 CDialog::OnPaint()
	
	//中间
	CRect rcClient ;
	GetClientRect( &rcClient ) ;
	Graphics    graphics(dc);

	ImageAttributes imgAtt;
	imgAtt.SetWrapMode(WrapModeTileFlipXY);//GDI+在位伸图片时会自动加上渐变效果。但此处不需要，所以得加上此属性

	//去掉边框
	//左上、上中、右上
	graphics.DrawImage(m_imgTopLeft,Rect(0, 0, m_imgTopLeft->GetWidth(), m_imgTopLeft->GetHeight()), 
		                            0, 0, m_imgTopLeft->GetWidth(),  m_imgTopLeft->GetHeight(),UnitPixel,&imgAtt);//没拉抻

	graphics.DrawImage(m_imgTopMid, Rect(m_imgTopLeft->GetWidth(), 0, rcClient.Width() - m_imgTopLeft->GetWidth() * 2, m_imgTopMid->GetHeight()), 
		                            0, 0, rcClient.Width() - m_imgTopLeft->GetWidth() * 2, m_imgTopMid->GetHeight(), UnitPixel,&imgAtt);

	graphics.DrawImage(m_imgTopRight, Rect(rcClient.right - m_imgTopRight->GetWidth() - 1, 0, m_imgTopRight->GetWidth(), m_imgTopRight->GetHeight()), 
		                             0, 0, m_imgTopRight->GetWidth(), m_imgTopRight->GetHeight(), UnitPixel,&imgAtt);

	//左中、右中
	graphics.DrawImage(m_imgMidLeft, Rect(0, m_imgTopMid->GetHeight(), m_imgMidLeft->GetWidth(), rcClient.Height() - m_imgTopMid->GetHeight() * 2), 
		                             0, 0, m_imgMidLeft->GetWidth(), rcClient.Height() - m_imgTopMid->GetHeight() * 2, UnitPixel,&imgAtt);

	graphics.DrawImage(m_imgMidRight, Rect(rcClient.right - m_imgMidRight->GetWidth() - 1, m_imgTopMid->GetHeight(), m_imgMidRight->GetWidth(), rcClient.Height() - m_imgTopMid->GetHeight() * 2), 
		                              0, 0,  m_imgMidRight->GetWidth(), rcClient.Height() - m_imgTopMid->GetHeight() * 2,UnitPixel,&imgAtt);

	//左下、下中、右下
	graphics.DrawImage(m_imgBottomLeft,Rect(0, rcClient.bottom - m_imgBottomLeft->GetHeight() - 1, m_imgBottomLeft->GetWidth(), m_imgBottomLeft->GetHeight()), 
		                               0, 0, m_imgBottomLeft->GetWidth(), m_imgBottomLeft->GetHeight(), UnitPixel,&imgAtt);

	graphics.DrawImage(m_imgBottomMid, Rect(m_imgMidLeft->GetWidth(), rcClient.bottom - m_imgBottomMid->GetHeight() - 1, rcClient.Width() - m_imgMidLeft->GetWidth() * 2, m_imgBottomMid->GetHeight()), 
		                               0, 0,rcClient.Width() - m_imgMidLeft->GetWidth() * 2, m_imgBottomMid->GetHeight(), UnitPixel,&imgAtt);

	graphics.DrawImage(m_imgBottomRight,Rect(rcClient.right - m_imgBottomRight->GetWidth() - 1, rcClient.bottom - m_imgBottomRight->GetHeight() - 1, m_imgBottomRight->GetWidth(), m_imgBottomRight->GetHeight()),
		                                0, 0, m_imgBottomRight->GetWidth(), m_imgBottomRight->GetHeight(), UnitPixel,&imgAtt);
	//中间
	graphics.DrawImage( m_pPic->GetCurImage() ,Rect(m_imgMidLeft->GetWidth(), m_imgTopMid->GetHeight(), rcClient.Width() - m_imgMidLeft->GetWidth() * 2 - 1 , rcClient.Height() - m_imgTopMid->GetHeight() * 2 - 1),
		                        0, 0,  rcClient.Width() - m_imgMidLeft->GetWidth() * 2 - 1 , rcClient.Height() - m_imgTopMid->GetHeight() * 2 - 1,UnitPixel,&imgAtt) ;
	/*graphics.DrawImage( m_pPic->GetCurImage() ,Rect(0, 0, rcClient.Width(), rcClient.Height()),
		0, 0,  rcClient.Width(), rcClient.Height(),UnitPixel,&imgAtt) ;*/
}

void CEnteringRoomDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	//CDialog::OnTimer(nIDEvent);

	if ( nIDEvent == ENTER_ROOM_TIMER )
	{
		if ( m_pPic != NULL && m_pPic->IsAnimatedGIF() )
		{
			m_pPic->DrawFrameGIF() ;
		}
		CRect Rect ;
		GetClientRect( &Rect ) ;
		InvalidateRect( &Rect ,FALSE ) ;
	}
	
	
}

void CEnteringRoomDlg::OnDestroy()
{
	KillTimer( ENTER_ROOM_TIMER ) ;
	CDialog::OnDestroy();

	// TODO: 在此处添加消息处理程序代码
}


