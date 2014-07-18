// VideoDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "..\Room.h"
#include "VideoDlg.h"
#include "Skin_i.h"
#include "SkinUtil.h"
#include "..\avlogic\RoomAVLogic.h"
#define  NOTIFY_SKIN_UPDATE_WND 1
#include "..\UserMessageDefine.h"
#include "ximage.h"
#include "..\..\include\utility\SystemHelper.h"
#include "..\GlobalRoomDefine.h"
#include "utility/URLEncode.h"
#include "..\..\CommonLibrary\include\ui\UIHelper.h"
#include "..\personalsetting\PersonalConfig.h"

// CVideoDlg 对话框

CxImage* CVideoDlg::m_spImage = NULL;
CxImage* CVideoDlg::m_usingImage = NULL ;
CxImage* CVideoDlg::m_preparingImage = NULL;
CxImage* CVideoDlg::m_bigSpImage= NULL;
CxImage* CVideoDlg::m_bigUsingImage= NULL;
CxImage* CVideoDlg::m_bigPreparingImage= NULL;
IMPLEMENT_DYNAMIC(CVideoDlg, CDialog)

CVideoDlg::CVideoDlg( BOOL bToolBar , HWND hParent ,RoomAVLogic *RoomAVLogic  ,CWnd* pParent /*=NULL*/)
	: CDialog(CVideoDlg::IDD, pParent),
	m_bToolBar( bToolBar ) ,
	m_hParent(hParent),
	m_pRoomAVLogic( RoomAVLogic )
{
	m_unUserUIN = 0 ;
	m_bChangeVideo = m_bChangeAudio = m_bChangeSize =TRUE ;
	m_pToolBarDlg = NULL ;
	m_pSkinEngine = NULL;
	m_bNeedHideTooBal = TRUE ;
	::GetSkinEngine(&m_pSkinEngine);
	m_bInited = FALSE ;
	m_bPreparingVideo = FALSE;
	m_unSelfUIN = GetCurInfo().unUIN ;
	m_bBigMic = FALSE;
}

CVideoDlg::~CVideoDlg()
{
	if ( m_pToolBarDlg != NULL )
	{
		m_pToolBarDlg->DestroyWindow() ;
		delete m_pToolBarDlg ;
	}
}

void CVideoDlg::ReleaseImage()
{
	if(m_spImage != NULL)
	{
		delete m_spImage;
		m_spImage = NULL;
	}
	if ( m_usingImage != NULL )
	{
		delete m_usingImage ;
		m_usingImage = NULL ;
	}
	if (m_preparingImage != NULL)
	{
		delete m_preparingImage;
		m_preparingImage = NULL;
	}
	if(m_bigSpImage != NULL)
	{
		delete m_bigSpImage;
		m_bigSpImage = NULL;
	}
	if ( m_bigUsingImage != NULL )
	{
		delete m_bigUsingImage ;
		m_bigUsingImage = NULL ;
	}
	if (m_bigPreparingImage != NULL)
	{
		delete m_bigPreparingImage;
		m_bigPreparingImage = NULL;
	}
}

void CVideoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CVideoDlg, CDialog)
	ON_WM_CREATE()
	ON_WM_MOUSEMOVE()
	ON_MESSAGE(WM_MOUSELEAVE,OnMouseLeave)
	ON_WM_ERASEBKGND()
	ON_WM_KILLFOCUS()
	ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()


// CVideoDlg 消息处理程序

int CVideoDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;
	if ( m_bToolBar )
	{
		m_pToolBarDlg = new CMicToolBarDlg( this ) ;
		m_pToolBarDlg->Create(CMicToolBarDlg::IDD , this) ;
	}	

	// TODO:  在此添加您专用的创建代码
	//ASSERT(NULL != m_pSkinEngine);
	//if (NULL != m_pSkinEngine)
	//{
	//	HRESULT hr = m_pSkinEngine->LoadSkin(GetSafeHwnd(),L"VideoDlg");
	//	ASSERT(hr== S_OK && __LINE__);
	//}

	return 0;
}

void CVideoDlg::HideToolBarDlg() 
{
	if ( m_bToolBar && m_pToolBarDlg != NULL && m_pToolBarDlg->GetSafeHwnd() )
	{
		common::ui::CCursorPos pt;
		CRect rect ;
		GetWindowRect(&rect);
		CWnd* pWnd = WindowFromPoint(pt);//鼠标所在窗口
		BOOL bIn = (pWnd == this) || (pWnd == m_pToolBarDlg);
		if (!bIn)
		{
			m_pToolBarDlg->ShowWindow( SW_HIDE ) ;
			return;
		}
		if ( m_pToolBarDlg->IsWindowVisible() && !rect.PtInRect(pt))
		{
			m_pToolBarDlg->ShowWindow( SW_HIDE ) ;
		}
	}
}

void CVideoDlg::SetToDefault( BOOL bExitShow )
{
	if ( bExitShow )
	{
		m_unUserUIN = 0 ;
		if ( m_pToolBarDlg != NULL && m_pToolBarDlg->IsWindowVisible() )
		{
			m_pToolBarDlg->ShowWindow( SW_HIDE ) ;
		}
		m_bPreparingVideo = FALSE;
	}
	Invalidate() ;
}


void CVideoDlg::ChangeVideo( int nBtnState ) 
{
	::SendMessage( m_hParent , WM_CHANGEVIDEO , m_unUserUIN , nBtnState ) ;
}

void CVideoDlg::ChangeAudio( int nBtnState ) 
{
	::SendMessage( m_hParent , WM_CHANGEAUDIO , m_unUserUIN , nBtnState ) ;
}

void CVideoDlg::ChangeSize( int nBtnState ) 
{
	::SendMessage( m_hParent , WM_CHANGESIZE , m_unUserUIN , nBtnState ) ;
}


//拍照
void CVideoDlg::Photo() 
{
	::SendMessage( m_hParent , WM_PHOTO , m_unUserUIN , 0 ) ;
}

void CVideoDlg::Init( uint32 unUIN  ,uint16 unStatus  ) 
{
	m_unLookStatus = m_unMicStatus = 0 ;
	m_unMicStatus = unStatus ;
	if ( m_bToolBar && m_pToolBarDlg != NULL )
	{
		m_pToolBarDlg->InitBtn( unUIN ,unStatus ) ;
	}
	m_unLookStatus |= core::USERSTATUSTYPE_VIDEO ;
	m_unLookStatus &= (~core::USERSTATUSTYPE_AUDIO) ;
	m_unUserUIN = unUIN ;
	m_bInited = TRUE ;
	m_bPreparingVideo = TRUE;
	Invalidate() ;
}

void CVideoDlg::Init( uint32 unUIN ) 
{
	m_unUserUIN = unUIN ;
	Invalidate() ;
}

void CVideoDlg::OpenVideo( ) 
{
	if ( m_bToolBar && m_pToolBarDlg != NULL )
	{
		m_pToolBarDlg->SetBtnVideoStatus( BTN_STATUS_OPEN ) ;
		m_pToolBarDlg->SetBtnPopupStatus( BTN_STATUS_OPEN ) ;
		m_pToolBarDlg->SetBtnPhotoStatus( BTN_STATUS_OPEN ) ;
	}
	Invalidate() ;
}

void CVideoDlg::OpenVideoRsp( uint32 unUIN ) 
{
	if ( m_unSelfUIN == unUIN  && m_unUserUIN == unUIN )
	{
		m_unMicStatus |= core::USERSTATUSTYPE_VIDEO ;
	}
	else
	{
		m_unLookStatus |= core::USERSTATUSTYPE_VIDEO ;
	}

	if ( m_unSelfUIN == unUIN )
	{
		OpenVideo() ;
	}
	else
	{

		if ( (m_unLookStatus&core::USERSTATUSTYPE_VIDEO) == 0 )
		{
			CloseVideoEnable() ; 
		}
		else
		{
			OpenVideo() ;
		}
	}
}

void CVideoDlg::OpenVideoNotify( uint32 unUIN )
{
	if ( m_unUserUIN == unUIN )
	{
		m_unMicStatus |= core::USERSTATUSTYPE_VIDEO ;
	}
	else
	{
		m_unLookStatus |= core::USERSTATUSTYPE_VIDEO ;
	}
	if ( m_unSelfUIN == unUIN )
	{
		OpenVideo() ;
	}
	else
	{

		if ( (m_unLookStatus&core::USERSTATUSTYPE_VIDEO) == 0 )
		{
			CloseVideoEnable() ; 
		}
		else
		{
			OpenVideo() ;
		}
	}
}
void CVideoDlg::CloseVideoDisable() 
{
	if ( m_bToolBar && m_pToolBarDlg != NULL )
	{
		m_pToolBarDlg->SetBtnVideoStatus( BTN_STATUS_CLOSE , FALSE ) ;
		m_pToolBarDlg->SetBtnVideoStatus( BTN_STATUS_CLICKDISABLE ) ;
		m_pToolBarDlg->SetBtnPopupStatus( BTN_STATUS_CLOSE , FALSE ) ;
		m_pToolBarDlg->SetBtnPopupStatus( BTN_STATUS_CLICKDISABLE ) ;
		m_pToolBarDlg->SetBtnPhotoStatus( BTN_STATUS_CLOSE , FALSE ) ;
		m_pToolBarDlg->SetBtnPhotoStatus( BTN_STATUS_CLICKDISABLE ) ;
	}
	SetToDefault() ;
}

void CVideoDlg::CloseVideoEnable() 
{
	if ( m_bToolBar && m_pToolBarDlg != NULL )
	{
		m_pToolBarDlg->SetBtnVideoStatus( BTN_STATUS_CLOSE ) ;
		m_pToolBarDlg->SetBtnPopupStatus( BTN_STATUS_CLOSE , FALSE ) ;
		m_pToolBarDlg->SetBtnPopupStatus( BTN_STATUS_CLICKDISABLE ) ;
		m_pToolBarDlg->SetBtnPhotoStatus( BTN_STATUS_CLOSE , FALSE ) ;
		m_pToolBarDlg->SetBtnPhotoStatus( BTN_STATUS_CLICKDISABLE ) ;
	}
	SetToDefault() ;
}

void CVideoDlg::OpenAudio() 
{
	if ( m_bToolBar && m_pToolBarDlg != NULL )
	{
		m_pToolBarDlg->SetBtnAudioStatus( BTN_STATUS_OPEN ) ;
	}
}


void CVideoDlg::OpenAudioRsp( uint32 unUIN ) 
{
	if ( m_unSelfUIN == unUIN && m_unUserUIN == unUIN )
	{
		m_unMicStatus |= core::USERSTATUSTYPE_AUDIO ;
	}
	else
	{
		m_unLookStatus |= core::USERSTATUSTYPE_AUDIO ;
	}
	if ( m_unSelfUIN == unUIN )
	{
		OpenAudio() ;
	}
	else
	{
		if ( (m_unLookStatus&core::USERSTATUSTYPE_AUDIO) == 0 )
		{
			CloseAudioEnable() ; 
		}
		else
		{
			OpenAudio() ;
		}
	}
}

void CVideoDlg::OpenAudioNotify( uint32 unUIN ) 
{
	if ( m_unUserUIN == unUIN )
	{
		m_unMicStatus |= core::USERSTATUSTYPE_AUDIO ;
	}
	else
	{
		m_unLookStatus |= core::USERSTATUSTYPE_AUDIO ;
	}
	if ( m_bInited )
	{
		m_unLookStatus |= core::USERSTATUSTYPE_AUDIO ;
		m_bInited = FALSE ;
	}
	if ( m_unSelfUIN == unUIN )
	{
		OpenAudio() ;
	}
	else
	{
		if ( (m_unLookStatus&core::USERSTATUSTYPE_AUDIO) == 0 )
		{
			CloseAudioEnable() ; 
		}
		else
		{
			OpenAudio() ;
		}
	}
}

void CVideoDlg::CloseAudioDisable()
{
	if ( m_bToolBar && m_pToolBarDlg != NULL )
	{
		m_pToolBarDlg->SetBtnAudioStatus( BTN_STATUS_CLOSE , FALSE ) ;
		m_pToolBarDlg->SetBtnAudioStatus( BTN_STATUS_CLICKDISABLE ) ;
	}
}

void CVideoDlg::CloseAudioEnable()
{
	if ( m_bToolBar && m_pToolBarDlg != NULL )
	{
		m_pToolBarDlg->SetBtnAudioStatus( BTN_STATUS_CLOSE ) ;
	}
}


void CVideoDlg::CloseVideoRsp( uint32 unUIN ) 
{
	if ( m_unSelfUIN == unUIN && m_unUserUIN == unUIN )
	{
		m_unMicStatus &= (~core::USERSTATUSTYPE_VIDEO) ;
	}
	else
	{
		m_unLookStatus &= (~core::USERSTATUSTYPE_VIDEO) ;
	}	
	if ( m_unSelfUIN== unUIN )
	{
		CloseVideoEnable() ;
	}
	else
	{
		if (  (m_unMicStatus&core::USERSTATUSTYPE_VIDEO) == 0 )
		{
			CloseVideoDisable() ;
		}
		else
		{
			if ( (m_unLookStatus&core::USERSTATUSTYPE_VIDEO) == 0 )
			{
				CloseVideoEnable() ;
			}
		}
	}
}

void CVideoDlg::CloseVideoNotify( uint32 unUIN ) 
{
	if ( m_unUserUIN == unUIN )
	{
		m_unMicStatus &= (~core::USERSTATUSTYPE_VIDEO) ;
	}
	else
	{
		m_unLookStatus &= (~core::USERSTATUSTYPE_VIDEO) ;
	}	
	if ( m_unSelfUIN == unUIN )
	{
		CloseVideoEnable() ;
	}
	else
	{
		if (  (m_unMicStatus&core::USERSTATUSTYPE_VIDEO) == 0 )
		{
			CloseVideoDisable() ;
		}
		else
		{
			if ( (m_unLookStatus&core::USERSTATUSTYPE_VIDEO) == 0 )
			{
				CloseVideoEnable() ;
			}
		}
	}
}


void CVideoDlg::CloseAudioRsp( uint32 unUIN ) 
{
	if ( m_unSelfUIN == unUIN && m_unUserUIN == unUIN )
	{
		m_unMicStatus &= (~core::USERSTATUSTYPE_AUDIO) ;
	}
	else
	{
		m_unLookStatus &= (~core::USERSTATUSTYPE_AUDIO) ;
	}

	if ( m_unSelfUIN == unUIN )
	{
		CloseAudioEnable() ;
	}
	else
	{
		if ( (m_unMicStatus&core::USERSTATUSTYPE_AUDIO) == 0 )
		{
			CloseAudioDisable() ;
		}
		else
		{
			if ( (m_unLookStatus&core::USERSTATUSTYPE_AUDIO) == 0 )
			{
				CloseAudioEnable() ;
			}
		}

	}
}

void CVideoDlg::CloseAudioNotify( uint32 unUIN ) 
{
	if ( m_unUserUIN == unUIN )
	{
		m_unMicStatus &= (~core::USERSTATUSTYPE_AUDIO) ;
	}
	else
	{
		m_unLookStatus &= (~core::USERSTATUSTYPE_AUDIO) ;
	}
	if ( m_unSelfUIN == unUIN )
	{
		CloseAudioEnable() ;
	}
	else
	{
		if ( (m_unMicStatus&core::USERSTATUSTYPE_AUDIO) == 0 )
		{
			CloseAudioDisable() ;
		}
		else
		{
			if ( (m_unLookStatus&core::USERSTATUSTYPE_AUDIO) == 0 )
			{
				CloseAudioEnable() ;
			}
		}

	}
}

void CVideoDlg::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if ( m_bToolBar )
	{
		CRect thisrect ;
		GetClientRect(&thisrect) ;
		if ( thisrect.PtInRect(point) )
		{
			if ( !m_pToolBarDlg->IsWindowVisible() && !m_bNeedHideTooBal )
			{
				ClientToScreen(&thisrect) ;
				m_pToolBarDlg->MoveWindow( thisrect.left , thisrect.top , thisrect.right-thisrect.left , 25 ) ;
				m_pToolBarDlg->ShowWindow( SW_SHOWNOACTIVATE ) ;
			//	m_pToolBarDlg->SetFocus() ;
				SetFocus() ;
			}
		}

		TRACKMOUSEEVENT tme ;
		tme.cbSize = sizeof( tme ) ;
		tme.dwFlags = TME_LEAVE ;
		tme.dwHoverTime = 1 ;
		tme.hwndTrack = m_hWnd ;
		::TrackMouseEvent(&tme) ;
	}
	CDialog::OnMouseMove(nFlags, point);
}

HRESULT CVideoDlg::OnMouseLeave( WPARAM wParam , LPARAM lParam )
{
	HideToolBarDlg() ;
	//if ( m_bToolBar )
	//{
	//	common::ui::CCursorPos pt;
	//	CRect rect ;
	//	GetClientRect( &rect ) ;
	//	ClientToScreen( &rect ) ;
	//	if ( m_pToolBarDlg->IsWindowVisible() &&  !rect.PtInRect(pt) )
	//	{
	//		m_pToolBarDlg->ShowWindow( SW_HIDE ) ;
	//	}
	//}
	return S_OK ;
}

BOOL CVideoDlg::PreTranslateMessage(MSG* pMsg)
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

CxImage* CVideoDlg::GetDefaultImage()
{
	if(m_spImage == NULL)
	{
		std::wstring strModulePath = common::utility::systemhelper::Get179AppPath();
		common::CURLEncode urlEncode;
		strModulePath = urlEncode.URLEncodeLocalPath(strModulePath);

		std::wstring strBkPath = strModulePath + IMG_VIDEO_PNG_PATH ;
		CxImage *pImage = new CxImage() ;
		if ( pImage == NULL )
		{
			return NULL ;
		}
		if ( !pImage->LoadW( strBkPath.c_str() ) )
		{
			delete pImage ;
			pImage = NULL;
		}
		else
		{
			m_spImage = pImage;
		}
	}

	return m_spImage;
}


CxImage* CVideoDlg::GetUsingImage() 
{
	if(m_usingImage == NULL)
	{
		std::wstring strModulePath = common::utility::systemhelper::Get179AppPath();
		common::CURLEncode urlEncode;
		strModulePath = urlEncode.URLEncodeLocalPath(strModulePath);
		std::wstring strPath = strModulePath + IMG_VIDEO_USING_PNG_PATH ;
		CxImage *pImage = new CxImage() ;
		if ( pImage == NULL )
		{
			return NULL ;
		}
		if ( !pImage->LoadW( strPath.c_str() ) )
		{
			delete pImage ;
			pImage = NULL;
		}
		else
		{
			m_usingImage = pImage;
		}
	}

	return m_usingImage;
}


CxImage* CVideoDlg::GetPreparingImage() 
{
	if(m_preparingImage == NULL)
	{
		std::wstring strModulePath = common::utility::systemhelper::Get179AppPath();
		common::CURLEncode urlEncode;
		strModulePath = urlEncode.URLEncodeLocalPath(strModulePath);
		std::wstring strPath = strModulePath + IMG_VIDEO_PREPARING_PNG_PATH ;
		CxImage *pImage = new CxImage() ;
		if ( pImage == NULL )
		{
			return NULL;
		}
		if ( !pImage->LoadW( strPath.c_str() ) )
		{
			delete pImage ;
			pImage = NULL;
		}
		else
		{
			m_preparingImage = pImage;
		}
	}

	return m_preparingImage;
}

BOOL CVideoDlg::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	BOOL bResult = FALSE ;
	if(m_unUserUIN != 0)
	{
		if ( m_unSelfUIN == m_unUserUIN )
		{       
			bResult = m_pRoomAVLogic->LocalPaintBk( GetSafeHwnd() ) ;
		}
		else
		{
			bResult = m_pRoomAVLogic->RemotePaintBk( m_unUserUIN , GetSafeHwnd() ) ;
		}
	}

	if ( !bResult )
	{
		CxImage *pImage = NULL ;
		if ( m_unSelfUIN == m_unUserUIN && m_pRoomAVLogic->GetCameraStatus( CPersonalConfig::GetInstance()->GetUseVideoDeviceIndex() ) == kVideo_IsUsing )
		{
			pImage = m_bBigMic ? CVideoDlg::GetBigUsingImage() : CVideoDlg::GetUsingImage();
		}
		else if (!m_bPreparingVideo && m_unUserUIN == 0)//无人在麦上
		{
			pImage = m_bBigMic ? CVideoDlg::GetBigDefaultImage() : CVideoDlg::GetDefaultImage();
		}else
		{
			pImage = m_bBigMic ? CVideoDlg::GetBigPreparingImage() : CVideoDlg::GetPreparingImage();
		}		
		
		if(pImage == NULL)
		{
			return TRUE;
		}

		m_bPreparingVideo = FALSE;

		CRect CClientRect ;
		GetClientRect( &CClientRect ) ;
		CDC memDC ;
		memDC.CreateCompatibleDC( pDC ) ;
		CBitmap memBitmap;
		memBitmap.CreateCompatibleBitmap(pDC, CClientRect.Width(), CClientRect.Height() );
		CBitmap* pOldBitmap = memDC.SelectObject(&memBitmap);
		pImage->Draw( memDC.GetSafeHdc() , CClientRect , NULL , TRUE ) ;
		pDC->BitBlt( 0 , 0 ,CClientRect.Width() ,CClientRect.Height() , &memDC , 0 , 0 ,SRCCOPY ) ;
		memDC.SelectObject(pOldBitmap);
		memBitmap.DeleteObject() ;
		memDC.DeleteDC() ;
	}
	return TRUE ;
	return CDialog::OnEraseBkgnd(pDC);
}

void CVideoDlg::OnKillFocus(CWnd* pNewWnd)
{
	CDialog::OnKillFocus(pNewWnd);
	HideToolBarDlg() ;
	//if ( m_bToolBar && m_pToolBarDlg != NULL && m_pToolBarDlg->GetSafeHwnd())
	//{
	//	common::ui::CCursorPos pt;
	//	CRect rect ;
	//	GetClientRect( &rect ) ;
	//	ClientToScreen( &rect ) ;
	//	if ( m_pToolBarDlg->IsWindowVisible() &&  !rect.PtInRect(pt) )
	//	{
	//		m_pToolBarDlg->ShowWindow( SW_HIDE ) ;
	//	}
	//}
	// TODO: 在此处添加消息处理程序代码
}

void CVideoDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	::SendMessage( m_hParent , WM_CLICK_VIDEO_DLG , m_unUserUIN , 0 ) ;
	CDialog::OnLButtonDown(nFlags, point);
}

CxImage* CVideoDlg::GetBigDefaultImage()
{

	if(m_bigSpImage == NULL)
	{
		std::wstring strModulePath = common::utility::systemhelper::Get179AppPath();
		common::CURLEncode urlEncode;
		strModulePath = urlEncode.URLEncodeLocalPath(strModulePath);
		std::wstring strBkPath = strModulePath + IMG_VIDEO_PNG_PATH_BIG ;
		CxImage *pImage = new CxImage() ;
		if ( pImage == NULL )
		{
			return NULL ;
		}
		if ( !pImage->LoadW( strBkPath.c_str() ) )
		{
			delete pImage ;
			pImage = NULL;
		}
		else
		{
			m_bigSpImage = pImage;
		}
	}

	return m_bigSpImage;
}

CxImage* CVideoDlg::GetBigUsingImage()
{
	if(m_bigUsingImage == NULL)
	{
		std::wstring strModulePath = common::utility::systemhelper::Get179AppPath();
		common::CURLEncode urlEncode;
		strModulePath = urlEncode.URLEncodeLocalPath(strModulePath);

		std::wstring strPath = strModulePath + IMG_VIDEO_USING_PNG_PATH_BIG ;
		CxImage *pImage = new CxImage() ;
		if ( pImage == NULL )
		{
			return NULL ;
		}
		if ( !pImage->LoadW( strPath.c_str() ) )
		{
			delete pImage ;
			pImage = NULL;
		}
		else
		{
			m_bigUsingImage = pImage;
		}
	}

	return m_bigUsingImage;
}

CxImage* CVideoDlg::GetBigPreparingImage()
{
	if(m_bigPreparingImage == NULL)
	{
		std::wstring strModulePath = common::utility::systemhelper::Get179AppPath();
		common::CURLEncode urlEncode;
		strModulePath = urlEncode.URLEncodeLocalPath(strModulePath);
		std::wstring strPath = strModulePath + IMG_VIDEO_PREPARING_PNG_PATH_BIG;
		CxImage *pImage = new CxImage() ;
		if ( pImage == NULL )
		{
			return NULL;
		}
		if ( !pImage->LoadW( strPath.c_str() ) )
		{
			delete pImage ;
			pImage = NULL;
		}
		else
		{
			m_bigPreparingImage = pImage;
		}
	}

	return m_bigPreparingImage;
}

void CVideoDlg::SetBigMic( BOOL bBigMic )
{
	m_bBigMic = bBigMic;
}