// PublicMicDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "..\Room.h"
#include "PublicMicDlg.h"
#include "skin_i.h"
#include "SkinUtil.h"
#include "VideoDlg.h"
#include "..\RoomParentDlg.h"
#include "MicPopupDlg.h"
#include "PhotoDlg.h"
#include "..\GlobalRoomDefine.h"
#include "..\userlist\UserListMenu.h"
#include "..\UserMessageDefine.h"
#include "..\SendEventManager.h"
#include "..\RoomLeftDlg.h"
#include "..\personalsetting\PersonalConfig.h"

#include "..\..\CommonLibrary\include\utility\SystemHelper.h"
#include "..\..\include\utility\FileHelper.h"

#define  NOTIFY_SKIN_IF_AUDIO           1
#define  SKIN_NOTIFY_VIDEO_DLG_HWND     2
#define  SKIN_ON_CLICK_BTN_FLOWER       3
#define  SKIN_ON_CLICK_BTN_CHOOSE_GIFT  4
#define  SKIN_ON_CLICK_BTN_MIC_MENU     5
#define  SKIN_ON_CLICK_USERNAME         6
#define  SKIN_ON_RCLICK_USERNAME        7
#define  SKIN_ON_CLICK_ORDER_SONG     8
// CPublicMicDlg 对话框

IMPLEMENT_DYNAMIC(CPublicMicDlg, CDialog)

CPublicMicDlg::CPublicMicDlg( CRoomParentDlg* pParentDlg ,int16 nIndex ,CWnd* pParent /*=NULL*/)
	: CDialog(CPublicMicDlg::IDD, pParent),
	m_pParentDlg( pParentDlg ),
	m_nIndex( nIndex )
{
	m_unUserUIN = 0 ;
	m_pSkinEngine = NULL;
	::GetSkinEngine(&m_pSkinEngine);

	m_pVideoDlg = NULL;
	m_pMicPopupDlg = NULL ;
	m_nPublicMicTime = 0 ;
}

CPublicMicDlg::~CPublicMicDlg()
{
	if ( m_pVideoDlg != NULL )
	{
		m_pVideoDlg->DestroyWindow();
		delete m_pVideoDlg ;
		m_pVideoDlg = NULL;
	}
	if ( m_pMicPopupDlg != NULL )
	{
		m_pMicPopupDlg->DestroyWindow();
		delete m_pMicPopupDlg ;
		m_pMicPopupDlg = NULL;
	}
}

void CPublicMicDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CPublicMicDlg, CDialog)
	ON_WM_CREATE()
	ON_SKIN_MESSAGE
	ON_MESSAGE(WM_CHANGEVIDEO , ChangeVideo )
	ON_MESSAGE(WM_CHANGEAUDIO , ChangeAudio )
	ON_MESSAGE(WM_CHANGESIZE , ChangeSize )
	ON_MESSAGE(WM_PHOTO , Photo )
	ON_MESSAGE(WM_CLICK_VIDEO_DLG ,ClickVideoDlg)
	ON_MESSAGE(WM_MICPOPUPDLG_CLOSE ,MicPopupDlgClose)
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CPublicMicDlg 消息处理程序

int CPublicMicDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  在此添加您专用的创建代码
	//Load Skin
	ASSERT(m_pSkinEngine != NULL);
	if (m_pSkinEngine != NULL){
		HRESULT result = m_pSkinEngine->LoadSkin(GetSafeHwnd(), L"PublicMicDlg");
		ASSERT(result == S_OK && __LINE__);
	}
    
	//创建视频显示窗口
	m_pVideoDlg = new CVideoDlg( TRUE , this->m_hWnd , m_pParentDlg->GetRoomAvLogic() );
	if (m_pVideoDlg)
	{
		m_pVideoDlg->Create(CVideoDlg::IDD,this);
		if (m_nIndex == 1)//大麦窗口
		{
			m_pVideoDlg->SetWindowPos(NULL, 0, 0, 360, 270, SWP_SHOWWINDOW);//视频窗口大概4:3比例
			m_pVideoDlg->SetBigMic(TRUE);
		}else
		{
			m_pVideoDlg->SetWindowPos(NULL, 0, 0, 192, 144, SWP_SHOWWINDOW);//视频窗口大概4:3比例
		}
	}
	return 0;
}

LRESULT CPublicMicDlg::OnSkinMessage(WPARAM wParam, LPARAM lParam)
{
	switch (wParam)
	{
	case SKIN_ON_CLICK_BTN_FLOWER:
		{
		}
		break;
	case SKIN_ON_CLICK_BTN_CHOOSE_GIFT:
		{
			OutputDebugString(L"\n###################选择礼物###################\n");
		}
		break;
	case SKIN_ON_CLICK_BTN_MIC_MENU:
		{
			MicDlgMenu micdlgmenu(m_pParentDlg) ;
			IUserItem* pMyItem = m_pParentDlg->GetRoomBusiness()->GetUserManager()->GetSelfUserItem() ;
			if ( pMyItem == NULL )
			{
				break;
			}
			if ( m_unUserUIN != 0 )
			{				
				IUserItem* pDstUserItem = m_pParentDlg->GetRoomBusiness()->GetUserManager()->SearchUserByUIN( m_unUserUIN ) ;
				if ( pDstUserItem != NULL )
				{					
					micdlgmenu.PopupMenu( pMyItem , m_nIndex , pDstUserItem ) ;				
				}
			}
			else
			{
				micdlgmenu.PopupMenu( pMyItem , m_nIndex ) ;
			}
            OutputDebugString(L"\n###################功能菜单###################\n");
		}
		break;
	case SKIN_ON_CLICK_USERNAME:
		{
			CString strName = m_strName ;
			if ( !strName.IsEmpty() )
			{
				CString strUUID ;
				strUUID.Format(L"%d" , m_unUserUIN ) ;
				m_pParentDlg->OnClickNameLink( strName , strUUID ) ;
			}
            OutputDebugString(L"\n###################左键单击用户名###################\n");
		}
		break;
	case SKIN_ON_RCLICK_USERNAME:
		{
//		if ( m_unUserUIN != 0 )
//			{
//				IUserItem* pUserItem = m_pParentDlg->GetRoomBusiness()->GetUserManager()->SearchUserByUIN( m_unUserUIN ) ;
//				if ( pUserItem != NULL )
//				{
//					UserListMenu * pMenu = new UserListMenu(m_pParentDlg);
//					if ( pMenu != NULL )
//					{
//						m_pParentDlg->m_pSelectUserItem = pUserItem ;
//						IUserItem * pSelfItem = m_pParentDlg->GetRoomBusiness()->GetUserManager()->GetSelfUserItem();
//						if (pSelfItem != NULL)
//						{
//							pMenu->PopupMenu( pSelfItem, pUserItem ) ;
//						}
//						delete pMenu;
//						pMenu = NULL;
//					}
//				}
//				OutputDebugString(L"\n###################右键单击用户名###################\n");
//			}         
		} 
		break;
	case SKIN_ON_CLICK_ORDER_SONG:
		{
			OnClickOrderSong();
		}
		break;
	default:break;
	}

	return TRUE;
}

LRESULT CPublicMicDlg::ChangeVideo( WPARAM wParam ,LPARAM lParam ) 
{
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

LRESULT CPublicMicDlg::ChangeAudio( WPARAM wParam ,LPARAM lParam ) 
{
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

LRESULT CPublicMicDlg::ChangeSize( WPARAM wParam ,LPARAM lParam ) 
{
	if ( lParam == BTN_STATUS_OPEN )
	{
		if ( m_pMicPopupDlg == NULL )
		{
			SwitchResourceToModule(_T("Room"));
			m_pMicPopupDlg = new CMicPopupDlg( this->m_hWnd , m_pParentDlg->GetRoomAvLogic() ) ;
			m_pMicPopupDlg->Create(CMicPopupDlg::IDD , NULL ) ;
		//	m_pMicPopupDlg->SetWindowPos( NULL , 700 , 300 , 434 , 363 , SWP_SHOWWINDOW ) ;
		}                                                                                  //而嵌在里头的videodlg大小为432*324,  
		else                                                                               //保证视频质量要求为4:3
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

LRESULT CPublicMicDlg::Photo( WPARAM wParam ,LPARAM lParam ) 
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
					CString strDirPath (wstrcurdir.c_str());
					CString strFilePath ;
					strFilePath.Format(_T("%s%d%2d%2d%d.bmp") , strDirPath ,
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

LRESULT CPublicMicDlg::ClickVideoDlg( WPARAM wParam ,LPARAM lParam ) 
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

LRESULT CPublicMicDlg::MicPopupDlgClose( WPARAM wParam , LPARAM lParam ) 
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
			pRoomAVLogic->RecvAVChg( m_unUserUIN , m_pVideoDlg->GetSafeHwnd() , RECV_VIDEO_WIDTH , RECV_VIDEO_HIEGHT , m_pMicPopupDlg->m_pVideoDlg->GetSafeHwnd() ) ;
		}		
	}
	return S_OK ;
}

void CPublicMicDlg::Init( uint32 unUIN  , uint32 un179ID ,uint16 unStatus , CString strName ,uint32 unTime ,bool bActor) 
{	
	CComVariant var = strName ;
	m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(),L"btnUserName",L"label",var);

	var = true ;
	m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(),L"btnUserName",L"visible",var);

	SetBtnOrderSongVisible(bActor);//是艺人就显示点歌按钮

	m_unUserUIN = unUIN ;
	m_unUser179ID = un179ID ;
	m_strName = strName ;
	m_pVideoDlg->m_bNeedHideTooBal = FALSE ;
	RoomAVLogic* pRoomAVLogic = m_pParentDlg->GetRoomAvLogic() ;
	if ( pRoomAVLogic != NULL )
	{
		core::CurInfo myCurInfo = GetCurInfo() ;
		if ( myCurInfo.unUIN == unUIN )
		{
			enuVIDEO_TYPE enType = VIDEO_PRIV_MIC;
			if (m_nIndex == 2 || m_nIndex==3)
			{
				enType = VIDEO_EDGE_MIC;
			}
			else if(m_nIndex == 1)
			{
				enType = VIDEO_CENT_MIC;
			}

			pRoomAVLogic->UpSelf( GetMicHandle() ,enType) ;
			pRoomAVLogic->LocalAVEffact( GetMicHandle() , FALSE ) ;
			pRoomAVLogic->SetMixerControlMode( CPersonalConfig::GetInstance()->GetOnMicPurpose() ) ;
			if ( (unStatus & core::USERSTATUSTYPE_AUDIO) == 0 )
			{
				pRoomAVLogic->PauseLocalAudio( true ) ;
				m_pParentDlg->CloseMic() ;
			}
			else
			{
				SetPublicMicEffect( TRUE ) ;
				m_pParentDlg->OpenMic() ;
			}
			if ( (unStatus & core::USERSTATUSTYPE_VIDEO) == 0 )
			{
				pRoomAVLogic->PauseLocalVideo( true ) ;	
			}
		}
		else
		{
			enuVIDEO_TYPE enType = VIDEO_PRIV_MIC;
			if (m_nIndex == 2 || m_nIndex==3)
			{
				enType = VIDEO_EDGE_MIC;
			}
			else if(m_nIndex == 1)
			{
				enType = VIDEO_CENT_MIC;
			}

			pRoomAVLogic->RecvAV( unUIN , GetMicHandle() ,320 ,240 , enType ) ;
			if ( (unStatus & core::USERSTATUSTYPE_AUDIO) == 0 )
			{
				pRoomAVLogic->PauseRemoteAudio( unUIN , true ) ;
			}
			else
			{
				SetPublicMicEffect( TRUE ) ;
			}
			if ( (unStatus & core::USERSTATUSTYPE_VIDEO) == 0 )
			{
				pRoomAVLogic->PauseRemoteVideo( unUIN , true ) ;
			}
		}
	}
	m_pVideoDlg->Init( unUIN ,unStatus ) ;
	if ( unTime != 0 )
	{
		m_nPublicMicTime = unTime ;
		SetTimer( PUBLIC_TIMER , 1000 , NULL ) ;
	}
}


void CPublicMicDlg::OnOpVideoAudioRsp( uint32 nOpAVType ,uint32 unUIN )
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
			core::CurInfo myCurInfo = GetCurInfo() ;
			if ( myCurInfo.unUIN == unUIN )
			{
				m_pParentDlg->OpenMic() ;
			}
		}
		break;
	case OPERATEAVTYPE_CLOSE_VIDEO:
		{				
			m_pVideoDlg->CloseVideoRsp( unUIN ) ;
			OnCloseVideo( unUIN ) ;			
		}
		break;
	case OPERATEAVTYPE_CLOSE_AUDIO:
		{			
			m_pVideoDlg->CloseAudioRsp( unUIN ) ;
			OnCloseAudio( unUIN ) ;
			core::CurInfo myCurInfo = GetCurInfo() ;
			if ( myCurInfo.unUIN == unUIN )
			{
				m_pParentDlg->CloseMic() ;
			}
		}
		break;
	default:break;
	}
}

void CPublicMicDlg::OnOpVideoAudioNotify( uint32 nOpAVType ,uint32 unUIN ) 
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
			OnCloseVideo( unUIN ) ;
		}
		break;
	case OPERATEAVTYPE_CLOSE_AUDIO:
		{			
			m_pVideoDlg->CloseAudioNotify( unUIN ) ;
			OnCloseAudio( unUIN ) ;
		}
		break;
	default:break;
	}
}


void CPublicMicDlg::OpenVideoRsp( uint32 unUIN ) 
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

void CPublicMicDlg::OpenVideoNotify( uint32 unUIN ) 
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

void CPublicMicDlg::OnCloseVideo( uint32 unUIN ) 
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


void CPublicMicDlg::OpenAudioRsp( uint32 unUIN ) 
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
			SetPublicMicEffect( TRUE ) ;
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

void CPublicMicDlg::OpenAudioNotify( uint32 unUIN ) 
{
	RoomAVLogic* pRoomAVLogic = m_pParentDlg->GetRoomAvLogic() ;
	if ( m_pVideoDlg->m_unMicStatus&core::USERSTATUSTYPE_AUDIO )
	{
		SetPublicMicEffect( TRUE ) ;
	}
	if ( m_pVideoDlg->m_unLookStatus&core::USERSTATUSTYPE_AUDIO )
	{
		if ( pRoomAVLogic != NULL )
		{
			pRoomAVLogic->PauseRemoteAudio( unUIN , false ) ;
		}
	}
}

void CPublicMicDlg::OnCloseAudio( uint32 unUIN ) 
{
	core::CurInfo myCurInfo = GetCurInfo() ; 
	RoomAVLogic* pRoomAVLogic = m_pParentDlg->GetRoomAvLogic() ;
	if ( myCurInfo.unUIN == unUIN )
	{
		pRoomAVLogic->PauseLocalAudio( true ) ;
		if (  (m_pVideoDlg->m_unMicStatus&core::USERSTATUSTYPE_AUDIO) == 0 )
		{
			SetPublicMicEffect( FALSE ) ;
		}		
	}
	else
	{
		if (  (m_pVideoDlg->m_unMicStatus&core::USERSTATUSTYPE_AUDIO) == 0 )
		{
			pRoomAVLogic->PauseRemoteAudio( unUIN , true ) ;
			SetPublicMicEffect( FALSE ) ;
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

void CPublicMicDlg::SetPublicMicEffect( BOOL bOnMic)
{
	//DragonZ:V2.22后无此效果
	/*CRoomLeftDlg* pRoomLeftDlg = m_pParentDlg->GetRoomLeftDlg() ;
	if ( pRoomLeftDlg != NULL )
	{
		pRoomLeftDlg->SetPublicMicEffect( m_nIndex , bOnMic ) ;
	}*/
}

HWND CPublicMicDlg::GetMicHandle() 
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

void CPublicMicDlg::NotifyCreateSkinItem()
{
	if (m_pVideoDlg->GetSafeHwnd())
	{
		NOTIFY_SKIN_HELPER(SKIN_NOTIFY_VIDEO_DLG_HWND, m_pVideoDlg->GetSafeHwnd());
	}
}


BOOL CPublicMicDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	NotifyCreateSkinItem();

	CComVariant var = L"" ;
	m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(),L"btnUserName",L"label",var);
	var = FALSE ;
	m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(),L"btnUserName" ,L"visible" ,var) ;

	if (m_nIndex == 1)//大视频
	{
		m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(),L"imgPublicMicBigBk" ,L"visible" , (CComVariant)TRUE) ;
		m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(),L"imgPublicMicSmallBk" ,L"visible" , (CComVariant)FALSE) ;
	}else
	{
		m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(),L"imgPublicMicBigBk" ,L"visible" , (CComVariant)FALSE) ;
		m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(),L"imgPublicMicSmallBk" ,L"visible" , (CComVariant)TRUE) ;
	}
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CPublicMicDlg::SetDefault()
{
	SetPublicMicEffect( FALSE ) ;
	m_pVideoDlg->m_bNeedHideTooBal = TRUE ;
	if ( m_pMicPopupDlg != NULL && m_pMicPopupDlg->IsWindowVisible() )
	{
		m_pMicPopupDlg->ShowWindow( SW_HIDE ) ;
	}
	m_nPublicMicTime = m_unUserUIN = 0 ;
	CComVariant var = L"" ;
	m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(),L"btnUserName",L"label",var);
	var = false ;
	m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(),L"btnUserName" ,L"visible" ,var) ;
	m_pVideoDlg->SetToDefault( TRUE );	
	KillTimer( PUBLIC_TIMER ) ;
	
}

void CPublicMicDlg::CleanAVLogic() 
{
	RoomAVLogic* pRoomAVLogic = m_pParentDlg->GetRoomAvLogic() ;
	if ( pRoomAVLogic != NULL && m_unUserUIN != 0 )
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

void CPublicMicDlg::CloseVideo() 
{
	if ( m_unUserUIN != 0 )
	{
		CSendEventManager::SendOpVideoAudioEvent( m_pParentDlg->GetRoomID(), OPERATEAVTYPE_CLOSE_VIDEO , m_unUserUIN ) ;
	}	
}

void CPublicMicDlg::OpenVideo() 
{
	if ( m_unUserUIN != 0 )
	{
		CSendEventManager::SendOpVideoAudioEvent( m_pParentDlg->GetRoomID(), OPERATEAVTYPE_OPEN_VIDEO , m_unUserUIN ) ;
	}
}

BOOL CPublicMicDlg::PreTranslateMessage(MSG* pMsg)
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

void CPublicMicDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	switch( nIDEvent )
	{
	case PUBLIC_TIMER:
		{
			m_nPublicMicTime -= 1;
			core::CurInfo myCurInfo = GetCurInfo() ;
			if ( m_nPublicMicTime<= 0 )
			{
				KillTimer( PUBLIC_TIMER ) ;
				if ( myCurInfo.unUIN == m_unUserUIN )
				{
					m_pParentDlg->GetHtmlOutPutManager()->InsertPrivateTips( _T("【提示】： ") ,L"您的麦时已到"  , COLOR_OPERATE ) ;
					m_pParentDlg->FangMai( false ) ; //放麦
				}
			}
			else
			{				
				CString strTemp ;
				if ( m_nPublicMicTime > 60 )
				{
					strTemp.Format( L"%s(%d分钟)" , m_strName , m_nPublicMicTime/60 +1 ) ;
				}						
				else if ( m_nPublicMicTime == 60 )
				{
					strTemp.Format( L"%s(1分钟)" , m_strName ) ;
				}
				else
				{
					strTemp.Format( L"%s(%d秒)" , m_strName , m_nPublicMicTime) ;
				}
				CComVariant var = strTemp ;
				m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(),L"btnUserName",L"label",var);	
			}
		}
		break;
	}
	CDialog::OnTimer(nIDEvent);
}


void CPublicMicDlg::OnClickOrderSong()
{
	if (m_pParentDlg == NULL || m_pParentDlg->GetSafeHwnd() == NULL)
	{
		return;
	}
	m_pParentDlg->OnClickOrderSong(m_nIndex);

}

void CPublicMicDlg::SetBtnOrderSongVisible(bool bShow)
{
	CComVariant var = bShow;
	m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(),L"btnOrderSong",L"visible",var);
}