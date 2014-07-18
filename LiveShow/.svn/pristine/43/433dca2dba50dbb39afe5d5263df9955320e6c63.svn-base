// PrivateMicDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "..\Room.h"
#include "PrivateMicDlg.h"
#include "skin_i.h"
#include "SkinUtil.h"
#include "VideoDlg.h"
#include "..\RoomParentDlg.h"
#include "MicPopupDlg.h"
#include "PhotoDlg.h"
#include "..\GlobalRoomDefine.h"
#include "..\UserMessageDefine.h"
#include "..\SendEventManager.h"
#include "..\personalsetting\PersonalConfig.h"

#include "..\..\include\utility\FileHelper.h"
#include "..\..\include\utility\SystemHelper.h"

#define  SKIN_ON_CLICK_BTN_CLOSE      1
#define  NOTIFY_SKIN_IF_AUDIO         2
#define  NOTIFY_SKIN_VIDEO_DLG_HWND   3
// CPrivateMicDlg 对话框

IMPLEMENT_DYNAMIC(CPrivateMicDlg, CDialog)

CPrivateMicDlg::CPrivateMicDlg( CRoomParentDlg* pParentDlg ,int nIndex ,CWnd* pParent /*=NULL*/)
	: CDialog(CPrivateMicDlg::IDD, pParent),
	m_pParentDlg( pParentDlg ),
	m_nIndex( nIndex )
{
	m_pSkinEngine = NULL;
	::GetSkinEngine(&m_pSkinEngine);

	m_pVideoDlg = NULL;
	m_pMicPopupDlg = NULL ;
}

CPrivateMicDlg::~CPrivateMicDlg()
{
}

void CPrivateMicDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CPrivateMicDlg, CDialog)
	ON_WM_CREATE()
	ON_SKIN_MESSAGE
	ON_MESSAGE(WM_CHANGEVIDEO , ChangeVideo )
	ON_MESSAGE(WM_CHANGEAUDIO , ChangeAudio )
	ON_MESSAGE(WM_CHANGESIZE , ChangeSize )
	ON_MESSAGE(WM_PHOTO , Photo )
	ON_MESSAGE(WM_CLICK_VIDEO_DLG ,ClickVideoDlg)
	ON_MESSAGE(WM_MICPOPUPDLG_CLOSE ,MicPopupDlgClose)
	ON_WM_NCLBUTTONDOWN()
END_MESSAGE_MAP()


// CPrivateMicDlg 消息处理程序

int CPrivateMicDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  在此添加您专用的创建代码
	//Load Skin
	ASSERT(m_pSkinEngine != NULL);
	if (m_pSkinEngine != NULL){
		HRESULT result = m_pSkinEngine->LoadSkin(GetSafeHwnd(), L"PrivateMicDlg");
		ASSERT(result == S_OK && __LINE__);
	}
 
	//创建视频显示窗口
	m_pVideoDlg = new CVideoDlg( TRUE , this->m_hWnd ,m_pParentDlg->GetRoomAvLogic() );
	if (m_pVideoDlg)
	{
		m_pVideoDlg->Create(CVideoDlg::IDD,this);
		m_pVideoDlg->SetWindowPos(NULL,4,23,216,(162+3),SWP_SHOWWINDOW);
	}

	return 0;
}


LRESULT CPrivateMicDlg::OnSkinMessage(WPARAM wParam, LPARAM lParam)
{
	switch (wParam)
	{
	case SKIN_ON_CLICK_BTN_CLOSE:
		{
			m_pParentDlg->SendMessage( WM_PRIVATEMICDLG_CLOSE , m_unUserUIN , 0 ) ;
		}
        break;
	default:break;
	}

	return TRUE;
}

LRESULT CPrivateMicDlg::ChangeVideo( WPARAM wParam ,LPARAM lParam ) 
{
	core::CurInfo myCurInfo = GetCurInfo() ; 
	if ( lParam == BTN_STATUS_OPEN )
	{
		CSendEventManager::SendOpVideoAudioEvent( m_pParentDlg->GetRoomID(), OPERATEAVTYPE_CLOSE_VIDEO , wParam ) ;
	}
	else
	{
		CSendEventManager::SendOpVideoAudioEvent( m_pParentDlg->GetRoomID(), OPERATEAVTYPE_OPEN_VIDEO , wParam ) ;
	}
	return S_OK ;
}

LRESULT CPrivateMicDlg::ChangeAudio( WPARAM wParam ,LPARAM lParam ) 
{
	core::CurInfo myCurInfo = GetCurInfo() ; 
	if ( lParam == BTN_STATUS_OPEN )
	{
		CSendEventManager::SendOpVideoAudioEvent( m_pParentDlg->GetRoomID(), OPERATEAVTYPE_CLOSE_AUDIO , wParam ) ;
	}
	else
	{
		CSendEventManager::SendOpVideoAudioEvent( m_pParentDlg->GetRoomID(), OPERATEAVTYPE_OPEN_AUDIO , wParam ) ;
	}
	return S_OK ;
}

LRESULT CPrivateMicDlg::ChangeSize( WPARAM wParam ,LPARAM lParam ) 
{
	if ( lParam == BTN_STATUS_OPEN )
	{
		if ( m_pMicPopupDlg == NULL )
		{
			SwitchResourceToModule(_T("Room"));
			m_pMicPopupDlg = new CMicPopupDlg( this->m_hWnd , m_pParentDlg->GetRoomAvLogic() ) ;
			m_pMicPopupDlg->Create(CMicPopupDlg::IDD , NULL ) ;
		//	m_pMicPopupDlg->SetWindowPos( NULL , 700 , 300 , 434 , 363 , SWP_SHOWWINDOW  ) ;
		}
		else
		{
			m_pMicPopupDlg->ShowWindow( SW_SHOW ) ;
		}
		m_pMicPopupDlg->Init( m_strName , m_unUserUIN , m_unUser179ID ) ;
		RoomAVLogic* pRoomAVLogic = m_pParentDlg->GetRoomAvLogic() ;
		if ( pRoomAVLogic != NULL )
		{
			core::CurInfo myCurInfo = GetCurInfo() ;
			if ( myCurInfo.unUIN == m_unUserUIN )
			{
				pRoomAVLogic->LocalAVChg( m_pMicPopupDlg->m_pVideoDlg->GetSafeHwnd() ,m_pVideoDlg->GetSafeHwnd() ) ;
			}
			else
			{
				pRoomAVLogic->RecvAVChg( m_unUserUIN , m_pMicPopupDlg->m_pVideoDlg->GetSafeHwnd() , RECV_VIDEO_WIDTH , 
					RECV_VIDEO_HIEGHT , m_pVideoDlg->GetSafeHwnd() ) ;
			}			
			m_pVideoDlg->Invalidate() ;
		}
	}
	else
	{
		m_pMicPopupDlg->ShowWindow( SW_HIDE) ;
		RoomAVLogic* pRoomAVLogic = m_pParentDlg->GetRoomAvLogic() ;
		if ( pRoomAVLogic != NULL )
		{
			core::CurInfo myCurInfo = GetCurInfo() ;
			if ( myCurInfo.unUIN == m_unUserUIN )
			{
				pRoomAVLogic->LocalAVChg( m_pVideoDlg->GetSafeHwnd() , m_pMicPopupDlg->m_pVideoDlg->GetSafeHwnd() ) ;
			}
			else
			{
				pRoomAVLogic->RecvAVChg( m_unUserUIN , m_pVideoDlg->GetSafeHwnd() , RECV_VIDEO_WIDTH , 
					RECV_VIDEO_HIEGHT , m_pMicPopupDlg->m_pVideoDlg->GetSafeHwnd() ) ;
			}
		}
	}
	return S_OK ;
}

LRESULT CPrivateMicDlg::Photo( WPARAM wParam ,LPARAM lParam ) 
{
	if ( lParam == BTN_STATUS_OPEN )
	{
		RoomAVLogic* pRoomAVLogic = m_pParentDlg->GetRoomAvLogic() ;
		if ( pRoomAVLogic != NULL )
		{
			bool bResult = false ;
			core::CurInfo myCurInfo = GetCurInfo() ;
			if ( myCurInfo.unUIN == m_unUserUIN )
			{
				bResult = pRoomAVLogic->IsPhotoableLocalHwnd( GetMicHandle() ) ;
			}
			else
			{
				bResult = pRoomAVLogic->IsPhotoableRemoteHwnd( m_unUserUIN , GetMicHandle() ) ;
			}
			if ( bResult )
			{
				CTime currenttime = CTime::GetCurrentTime() ;
				wstring wstrcurdir = common::utility::systemhelper::Get179AppPath() + SNAPSHOT_DIR ;
				if ( common::utility::filehelper::CreateAllDirectory( wstrcurdir ) )
				{
					CString strDirPath( wstrcurdir.c_str() ) ;
					CString strFilePath ;
					strFilePath.Format(_T("%s%d%.2d%.2d%d.bmp") , strDirPath ,
						currenttime.GetYear() , currenttime.GetMonth() , currenttime.GetDay() ,GetTickCount() ) ;
					pRoomAVLogic->SavePhotoToBmp( GetMicHandle() , strFilePath ) ;

					CPhotoDlg *pPhotoDlg = new CPhotoDlg(strDirPath , strFilePath );//287*161大小
					if (pPhotoDlg)
					{   
						int cx = GetSystemMetrics(SM_CXSCREEN);
						int cy = GetSystemMetrics(SM_CYSCREEN);
						int clientCx = 287;
						int clientCy = 161;
						SwitchResourceToModule(_T("Room"));
						pPhotoDlg->Create(CPhotoDlg::IDD,NULL);
						pPhotoDlg->SetWindowPos(NULL,cx/2 - clientCx/2,cy/2 - clientCy/2,clientCx,clientCy,SWP_SHOWWINDOW);
						pPhotoDlg->ShowWindow(SW_SHOW);
					}
				}
			}
		}
	}
	return S_OK ;
}

LRESULT CPrivateMicDlg::ClickVideoDlg( WPARAM wParam ,LPARAM lParam )
{
	if ( m_unUserUIN != 0 )
	{
		IUserItem* pDstUserItem = m_pParentDlg->GetRoomBusiness()->GetUserManager()->SearchUserByUIN( m_unUserUIN ) ;
		if ( pDstUserItem != NULL )
		{
			CString strName = pDstUserItem->GetUserName().c_str() ;
			CString strUUID ;
			strUUID.Format(L"%u" , m_unUserUIN ) ;
			m_pParentDlg->OnClickNameLink( strName , strUUID ) ;
		}
	}
	return S_OK ;
}

LRESULT CPrivateMicDlg::MicPopupDlgClose( WPARAM wParam , LPARAM lParam ) 
{
	m_pVideoDlg->m_pToolBarDlg->SetBtnPopupStatus( BTN_STATUS_OPEN ) ;
	RoomAVLogic* pRoomAVLogic = m_pParentDlg->GetRoomAvLogic() ;
	if ( pRoomAVLogic != NULL )
	{
		core::CurInfo myCurInfo = GetCurInfo() ;
		if ( myCurInfo.unUIN == m_unUserUIN )
		{
			pRoomAVLogic->LocalAVChg( m_pVideoDlg->GetSafeHwnd() , m_pMicPopupDlg->m_pVideoDlg->GetSafeHwnd() ) ;
		}
		else
		{
			pRoomAVLogic->RecvAVChg( m_unUserUIN , m_pVideoDlg->GetSafeHwnd() , RECV_VIDEO_WIDTH ,
				RECV_VIDEO_HIEGHT , m_pMicPopupDlg->m_pVideoDlg->GetSafeHwnd() ) ;
		}
	}
	return S_OK ;
}

void CPrivateMicDlg::Init( uint32 unUIN  , uint32 un179ID  ,uint16 unStatus , CString strName ) 
{
	m_unUserUIN = unUIN ;
	m_unUser179ID = un179ID ;
	m_strName = strName ;
	CComVariant var = strName ;
	m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(),L"textUser",L"value",var);		
	m_pVideoDlg->m_bNeedHideTooBal = FALSE ;

	RoomAVLogic* pRoomAVLogic = m_pParentDlg->GetRoomAvLogic() ;
	if ( pRoomAVLogic != NULL )
	{
		core::CurInfo myCurInfo = GetCurInfo() ;
		if ( myCurInfo.unUIN == unUIN )
		{
			pRoomAVLogic->UpSelf( GetMicHandle() ) ;	
			pRoomAVLogic->LocalAVEffact( GetMicHandle() ,FALSE ) ;
			pRoomAVLogic->SetMixerControlMode( CPersonalConfig::GetInstance()->GetOnMicPurpose() ) ;
			if ( (unStatus & core::USERSTATUSTYPE_AUDIO) == 0 )
			{
				pRoomAVLogic->PauseLocalAudio( true ) ;	
			}
			if ( (unStatus & core::USERSTATUSTYPE_VIDEO) == 0 )
			{
				pRoomAVLogic->PauseLocalVideo( true ) ;	
			}
		}
		else
		{
			pRoomAVLogic->RecvAV( unUIN , GetMicHandle() , RECV_VIDEO_WIDTH ,RECV_VIDEO_HIEGHT ) ;
			if ( (unStatus & core::USERSTATUSTYPE_AUDIO) == 0 )
			{
				pRoomAVLogic->PauseRemoteAudio( unUIN , true ) ;
			}
			if ( (unStatus & core::USERSTATUSTYPE_VIDEO) == 0 )
			{
				pRoomAVLogic->PauseRemoteVideo( unUIN , true ) ;
			}
		}
	}
	m_pVideoDlg->Init( unUIN , unStatus ) ;
}


void CPrivateMicDlg::OnOpVideoAudioRsp( uint32 nOpAVType ,uint32 unUIN ) 
{
	switch( nOpAVType )
	{
	case OPERATEAVTYPE_OPEN_VIDEO:
		{			
			m_pVideoDlg->OpenVideoRsp( unUIN ) ;
			OpenVideoRsp( unUIN ) ;
		}
		break;
	case OPERATEAVTYPE_OPEN_AUDIO:
		{			
			m_pVideoDlg->OpenAudioRsp( unUIN ) ;
			OpenAudioRsp( unUIN ) ;
		}
		break;
	case OPERATEAVTYPE_CLOSE_VIDEO:
		{				
			m_pVideoDlg->CloseVideoRsp( unUIN ) ;
			CloseVideo( unUIN ) ;
		}
		break;
	case OPERATEAVTYPE_CLOSE_AUDIO:
		{			
			m_pVideoDlg->CloseAudioRsp( unUIN ) ;
			CloseAudio( unUIN ) ;
		}
		break;
	default:break;
	}
}

void CPrivateMicDlg::OnOpVideoAudioNotify( uint32 nOpAVType ,uint32 unUIN ) 
{
	switch( nOpAVType )
	{
	case OPERATEAVTYPE_OPEN_VIDEO:
		{			
			m_pVideoDlg->OpenVideoNotify( unUIN ) ;
			OpenVideoNotify( unUIN ) ;
		}
		break;
	case OPERATEAVTYPE_OPEN_AUDIO:
		{			
			m_pVideoDlg->OpenAudioNotify( unUIN ) ;
			OpenAudioNotify( unUIN ) ;
		}
		break;
	case OPERATEAVTYPE_CLOSE_VIDEO:
		{			
			m_pVideoDlg->CloseVideoNotify( unUIN ) ;
			CloseVideo( unUIN ) ;	
		}
		break;
	case OPERATEAVTYPE_CLOSE_AUDIO:
		{			
			m_pVideoDlg->CloseAudioNotify( unUIN ) ;
			CloseAudio( unUIN ) ;
		}
		break;
	default:break;
	}
}


void CPrivateMicDlg::OpenVideoRsp( uint32 unUIN ) 
{
	core::CurInfo myCurInfo = GetCurInfo() ; 
	RoomAVLogic* pRoomAVLogic = m_pParentDlg->GetRoomAvLogic() ;
	if ( myCurInfo.unUIN == unUIN  && m_unUserUIN == unUIN )
	{
		if ( m_pVideoDlg->m_unMicStatus& core::USERSTATUSTYPE_VIDEO )
		{
			if ( pRoomAVLogic != NULL )
			{
				pRoomAVLogic->PauseLocalVideo( false ) ;				
			}
		}
	}
	else
	{
		if ( m_pVideoDlg->m_unLookStatus& core::USERSTATUSTYPE_VIDEO )
		{
			if ( pRoomAVLogic != NULL )
			{
				pRoomAVLogic->PauseRemoteVideo( unUIN , false ) ;
			}
		}
	}

}

void CPrivateMicDlg::OpenVideoNotify( uint32 unUIN ) 
{
	RoomAVLogic* pRoomAVLogic = m_pParentDlg->GetRoomAvLogic() ;
	if ( m_pVideoDlg->m_unLookStatus&core::USERSTATUSTYPE_VIDEO )
	{
		if ( pRoomAVLogic != NULL )
		{
			pRoomAVLogic->PauseRemoteVideo( unUIN , false ) ;
		}
	}
}

void CPrivateMicDlg::CloseVideo( uint32 unUIN ) 
{
	core::CurInfo myCurInfo = GetCurInfo() ; 
	RoomAVLogic* pRoomAVLogic = m_pParentDlg->GetRoomAvLogic() ;
	if ( pRoomAVLogic == NULL )
	{
		return ;
	}
	if ( m_pMicPopupDlg != NULL && m_pMicPopupDlg->IsWindowVisible() )
	{
		if ( myCurInfo.unUIN == unUIN && m_unUserUIN == unUIN )
		{
			pRoomAVLogic->LocalAVChg( m_pVideoDlg->GetSafeHwnd() , m_pMicPopupDlg->m_pVideoDlg->GetSafeHwnd() ) ;
		}
		else
		{
			pRoomAVLogic->RecvAVChg( unUIN , m_pVideoDlg->GetSafeHwnd() , RECV_VIDEO_WIDTH , 
				RECV_VIDEO_HIEGHT , m_pMicPopupDlg->m_pVideoDlg->GetSafeHwnd() ) ;
		}		
		m_pMicPopupDlg->ShowWindow( SW_HIDE ) ;
	}
	if ( myCurInfo.unUIN == unUIN )
	{
		pRoomAVLogic->PauseLocalVideo( true ) ;
	}
	else
	{
		if (  (m_pVideoDlg->m_unMicStatus&core::USERSTATUSTYPE_VIDEO) == 0 )
		{
			pRoomAVLogic->PauseRemoteVideo( unUIN , true ) ;
		}
		else
		{
			if ( (m_pVideoDlg->m_unLookStatus& core::USERSTATUSTYPE_VIDEO) == 0 )
			{
				pRoomAVLogic->PauseRemoteVideo( unUIN , true ) ;
			}
		}
	}
}


void CPrivateMicDlg::OpenAudioRsp( uint32 unUIN ) 
{
	core::CurInfo myCurInfo = GetCurInfo() ; 
	RoomAVLogic* pRoomAVLogic = m_pParentDlg->GetRoomAvLogic() ;
	if ( myCurInfo.unUIN == unUIN  && m_unUserUIN == unUIN )
	{
		if ( m_pVideoDlg->m_unMicStatus& core::USERSTATUSTYPE_AUDIO )
		{
			if ( pRoomAVLogic != NULL )
			{
				pRoomAVLogic->PauseLocalAudio( false ) ;				
			}
		}
	}
	else
	{
		if ( m_pVideoDlg->m_unLookStatus& core::USERSTATUSTYPE_AUDIO )
		{
			if ( pRoomAVLogic != NULL )
			{
				pRoomAVLogic->PauseRemoteAudio( unUIN , false ) ;
			}
		}
	}
}

void CPrivateMicDlg::OpenAudioNotify( uint32 unUIN ) 
{
	RoomAVLogic* pRoomAVLogic = m_pParentDlg->GetRoomAvLogic() ;
	if ( m_pVideoDlg->m_unLookStatus&core::USERSTATUSTYPE_AUDIO )
	{
		if ( pRoomAVLogic != NULL )
		{
			pRoomAVLogic->PauseRemoteAudio( unUIN , false ) ;
		}
	}
}

void CPrivateMicDlg::CloseAudio( uint32 unUIN ) 
{
	core::CurInfo myCurInfo = GetCurInfo() ; 
	RoomAVLogic* pRoomAVLogic = m_pParentDlg->GetRoomAvLogic() ;
	if ( myCurInfo.unUIN == unUIN )
	{
		pRoomAVLogic->PauseLocalAudio( true ) ;
	}
	else
	{
		if (  (m_pVideoDlg->m_unMicStatus&core::USERSTATUSTYPE_AUDIO) == 0 )
		{
			pRoomAVLogic->PauseRemoteAudio( unUIN , true ) ;
		}
		else
		{
			if ( (m_pVideoDlg->m_unLookStatus& core::USERSTATUSTYPE_AUDIO) == 0 )
			{
				pRoomAVLogic->PauseRemoteAudio( unUIN , true ) ;
			}
		}
	}
}


HWND CPrivateMicDlg::GetMicHandle() 
{
	if ( m_pMicPopupDlg != NULL && m_pMicPopupDlg->IsWindowVisible() )
	{
		return m_pMicPopupDlg->m_pVideoDlg->GetSafeHwnd() ;
	}
	else
	{
		return m_pVideoDlg->GetSafeHwnd() ;
	}
}

BOOL CPrivateMicDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	
	// TODO:  在此添加额外的初始化
	NotifyCreateSkinItem();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void CPrivateMicDlg::NotifyCreateSkinItem()
{
	if (m_pVideoDlg->GetSafeHwnd())
	{
		NOTIFY_SKIN_HELPER(NOTIFY_SKIN_VIDEO_DLG_HWND,m_pVideoDlg->GetSafeHwnd());
	}
}


void CPrivateMicDlg::SetDefault()
{
	if ( m_pMicPopupDlg != NULL && m_pMicPopupDlg->IsWindowVisible() )
	{
		m_pMicPopupDlg->ShowWindow( SW_HIDE ) ;
	}
	CComVariant var = L"" ;
	m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(),L"textUser",L"value",var);
	m_pVideoDlg->SetToDefault();
	m_pVideoDlg->m_bNeedHideTooBal = TRUE ;
}

int CPrivateMicDlg::GetIndex() 
{
	return m_nIndex ;
}

void CPrivateMicDlg::CleanAVLogic() 
{
	RoomAVLogic* pRoomAVLogic = m_pParentDlg->GetRoomAvLogic() ;
	if ( pRoomAVLogic != NULL )
	{
		core::CurInfo myCurInfo = GetCurInfo() ; 
		if ( myCurInfo.unUIN == m_unUserUIN )
		{
			pRoomAVLogic->DownSelf() ;
		}
		else
		{
			pRoomAVLogic->RemoveAV( m_unUserUIN ) ;
		}
	}
}


BOOL CPrivateMicDlg::PreTranslateMessage(MSG* pMsg)
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

void CPrivateMicDlg::OnNcLButtonDown(UINT nHitTest, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if ( m_pVideoDlg != NULL )
	{
		m_pVideoDlg->HideToolBarDlg() ;
	}	
	CDialog::OnNcLButtonDown(nHitTest, point);
}
