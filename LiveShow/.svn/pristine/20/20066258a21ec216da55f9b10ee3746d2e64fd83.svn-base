// P2PChatDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "..\Room.h"
#include "P2PChatDlg.h"
#include "skin_i.h"
#include "SkinUtil.h"
#include "VideoDlg.h"
#include "..\RoomParentDlg.h"
#include "MicPopupDlg.h"
#include "PhotoDlg.h"
#include "..\UserMessageDefine.h"
#include "..\GlobalRoomDefine.h"
#include "..\SendEventManager.h"
#include "..\personalsetting\PersonalConfig.h"

#include "..\..\include\utility\FileHelper.h"
#include "..\..\include\utility\SystemHelper.h"

#define SKIN_ON_CLICK_CLOSE                     1
#define SKIN_ON_CLICK_BTN_ENDCHAT               2
#define NOTIFY_SKIN_OP_VIDEO_DLG_HWND           3
#define NOTIFY_SKIN_SELF_VIDEO_DLG_HWND         4
// CP2PChatDlg 对话框

IMPLEMENT_DYNAMIC(CP2PChatDlg, CDialog)

CP2PChatDlg::CP2PChatDlg( CRoomParentDlg* pParentDlg ,uint32 unSrcUIN ,CWnd* pParent /*=NULL*/)
	: CDialog(CP2PChatDlg::IDD, pParent),
	m_pParentDlg( pParentDlg ),
	m_unOpSrcUIN( unSrcUIN ) 
{
	m_pSkinEngine = NULL;
	::GetSkinEngine(&m_pSkinEngine);

	m_pOpVideoDlg = NULL;
	m_pSelfVideoDlg = NULL;
	m_pOpMicPopupDlg = m_pSelfMicPopupDlg = NULL ;
}

CP2PChatDlg::~CP2PChatDlg()
{
	if ( m_pOpVideoDlg != NULL )
	{
		m_pOpVideoDlg->DestroyWindow() ;
		delete m_pOpVideoDlg ;
	}
	if ( m_pOpMicPopupDlg != NULL )
	{
		m_pOpMicPopupDlg->DestroyWindow() ;
		delete m_pOpMicPopupDlg ;
	}
	if ( m_pSelfVideoDlg != NULL )
	{
		m_pSelfVideoDlg->DestroyWindow() ;
		delete m_pSelfVideoDlg ;
	}
	if ( m_pSelfMicPopupDlg != NULL )
	{
		m_pSelfMicPopupDlg->DestroyWindow() ;
		delete m_pSelfMicPopupDlg ;
	}
}

void CP2PChatDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CP2PChatDlg, CDialog)
	ON_WM_CREATE()
	ON_SKIN_MESSAGE
	ON_WM_NCDESTROY()
	ON_MESSAGE(WM_CHANGEVIDEO , ChangeVideo )
	ON_MESSAGE(WM_CHANGEAUDIO , ChangeAudio )
	ON_MESSAGE(WM_CHANGESIZE , ChangeSize )
	ON_MESSAGE(WM_PHOTO , Photo )
	ON_MESSAGE(WM_CLICK_VIDEO_DLG ,ClickVideoDlg)
	ON_MESSAGE(WM_MICPOPUPDLG_CLOSE ,MicPopupDlgClose)
END_MESSAGE_MAP()


// CP2PChatDlg 消息处理程序

int CP2PChatDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  在此添加您专用的创建代码
	//Load Skin
	ASSERT(m_pSkinEngine != NULL);
	if (m_pSkinEngine != NULL){
		HRESULT result = m_pSkinEngine->LoadSkin(GetSafeHwnd(), L"P2PChatDlg");
		ASSERT(result == S_OK && __LINE__);
	}
    //创建视频子对话框 
	m_pOpVideoDlg = new CVideoDlg( TRUE ,this->m_hWnd , m_pParentDlg->GetRoomAvLogic() );
	if (m_pOpVideoDlg)
	{   
		SwitchResourceToModule(_T("Room"));
		m_pOpVideoDlg->Create(CVideoDlg::IDD,this);
		m_pOpVideoDlg->SetWindowPos(NULL,16,36,280,(LONG)(210*(1+HEIGHT_ADD_PERSENT)),SWP_SHOWWINDOW);
		m_pOpVideoDlg->ShowWindow(SW_SHOW);
	}
    
	m_pSelfVideoDlg = new CVideoDlg( TRUE ,this->m_hWnd , m_pParentDlg->GetRoomAvLogic() );
	if (m_pSelfVideoDlg)
	{   
		SwitchResourceToModule(_T("Room"));
		m_pSelfVideoDlg->Create(CVideoDlg::IDD,this);
		m_pSelfVideoDlg->SetWindowPos(NULL,334,36,162,121*(1+HEIGHT_ADD_PERSENT) ,SWP_SHOWWINDOW);
		m_pSelfVideoDlg->ShowWindow(SW_SHOW);
	}
 

	return 0;
}

LRESULT CP2PChatDlg::OnSkinMessage(WPARAM wParam, LPARAM lParam)
{ 
	switch(wParam)
	{
	case SKIN_ON_CLICK_CLOSE:
		{
           OnClickClose();
		}
		break;
	case SKIN_ON_CLICK_BTN_ENDCHAT:
		{
           OnClickEndChat();
		}
		break;
	default:break;
	}

	return TRUE;
}

LRESULT CP2PChatDlg::ChangeVideo( WPARAM wParam ,LPARAM lParam )
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

LRESULT CP2PChatDlg::ChangeAudio( WPARAM wParam ,LPARAM lParam ) 
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

LRESULT CP2PChatDlg::ChangeSize( WPARAM wParam ,LPARAM lParam )
{
	core::CurInfo myCurInfo = GetCurInfo() ; 
	if ( lParam == BTN_STATUS_OPEN )
	{
		if ( wParam == myCurInfo.unUIN )
		{
			if ( m_pSelfMicPopupDlg == NULL )
			{
				SwitchResourceToModule(_T("Room"));
				m_pSelfMicPopupDlg = new CMicPopupDlg( this->m_hWnd ,m_pParentDlg->GetRoomAvLogic() ) ;
				m_pSelfMicPopupDlg->Create(CMicPopupDlg::IDD , NULL ) ;
			//	m_pSelfMicPopupDlg->SetWindowPos( NULL , 700 , 300 , 481 , 403 , SWP_SHOWWINDOW ) ;
			}
			else
			{
				m_pSelfMicPopupDlg->ShowWindow( SW_SHOW ) ;
			}
			m_pSelfMicPopupDlg->Init( CString(myCurInfo.strName.c_str()) , myCurInfo.unUIN ,myCurInfo.un179id ) ;
			RoomAVLogic* pRoomAVLogic = m_pParentDlg->GetRoomAvLogic() ;
			if ( pRoomAVLogic != NULL )
			{
				pRoomAVLogic->LocalAVChg( m_pSelfMicPopupDlg->m_pVideoDlg->GetSafeHwnd() , m_pSelfVideoDlg->GetSafeHwnd() ) ;
				m_pSelfVideoDlg->Invalidate() ;
			}
		}
		else
		{
			if ( m_pOpMicPopupDlg == NULL )
			{
				SwitchResourceToModule(_T("Room"));
				m_pOpMicPopupDlg = new CMicPopupDlg( this->m_hWnd , m_pParentDlg->GetRoomAvLogic() ) ;
				m_pOpMicPopupDlg->Create(CMicPopupDlg::IDD , NULL ) ;
			//	m_pOpMicPopupDlg->SetWindowPos( NULL , 700 , 300 , 481 , 403 , SWP_SHOWWINDOW ) ;
			}
			else
			{
				m_pOpMicPopupDlg->ShowWindow( SW_SHOW ) ;
			}
			m_pOpMicPopupDlg->Init( m_strOpName , m_unOpSrcUIN , m_unOp179ID ) ;
			RoomAVLogic* pRoomAVLogic = m_pParentDlg->GetRoomAvLogic() ;
			if ( pRoomAVLogic != NULL )
			{
				pRoomAVLogic->RecvAVChg( wParam , m_pOpMicPopupDlg->m_pVideoDlg->GetSafeHwnd() , RECV_VIDEO_WIDTH , RECV_VIDEO_HIEGHT , m_pOpVideoDlg->GetSafeHwnd() ) ;
				m_pOpVideoDlg->Invalidate() ;
			}
		}
	}
	else
	{
		if ( wParam == myCurInfo.unUIN )
		{
			m_pSelfMicPopupDlg->ShowWindow( SW_HIDE) ;
			RoomAVLogic* pRoomAVLogic = m_pParentDlg->GetRoomAvLogic() ;
			if ( pRoomAVLogic != NULL )
			{
				pRoomAVLogic->LocalAVChg( m_pSelfVideoDlg->GetSafeHwnd() ,m_pSelfMicPopupDlg->m_pVideoDlg->GetSafeHwnd() ) ;
			}
		}
		else
		{
			m_pOpMicPopupDlg->ShowWindow( SW_HIDE) ;
			RoomAVLogic* pRoomAVLogic = m_pParentDlg->GetRoomAvLogic() ;
			if ( pRoomAVLogic != NULL )
			{
				pRoomAVLogic->RecvAVChg( wParam , m_pOpVideoDlg->GetSafeHwnd() , RECV_VIDEO_WIDTH , RECV_VIDEO_HIEGHT , m_pOpMicPopupDlg->m_pVideoDlg->GetSafeHwnd() ) ;
			}
		}
	}	
	
	return S_OK ;
}

LRESULT CP2PChatDlg::Photo( WPARAM wParam ,LPARAM lParam )
{

	if ( lParam == BTN_STATUS_OPEN )
	{
		RoomAVLogic* pRoomAVLogic = m_pParentDlg->GetRoomAvLogic() ;
		if ( pRoomAVLogic != NULL )
		{
			bool bResult = false ; 
			core::CurInfo myCurInfo = GetCurInfo() ; 
			if ( wParam == myCurInfo.unUIN )
			{
				bResult = pRoomAVLogic->IsPhotoableLocalHwnd( GetMicHandle( TRUE ) ) ;
			}
			else
			{
				bResult = pRoomAVLogic->IsPhotoableRemoteHwnd( wParam , GetMicHandle( FALSE ) ) ;
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
					if ( wParam == myCurInfo.unUIN )
					{
						pRoomAVLogic->SavePhotoToBmp( GetMicHandle(TRUE) , strFilePath ) ;
					}
					else
					{
						pRoomAVLogic->SavePhotoToBmp( GetMicHandle(FALSE) , strFilePath ) ;
					}					

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

LRESULT CP2PChatDlg::ClickVideoDlg( WPARAM wParam ,LPARAM lParam )
{
	if ( wParam != 0 )
	{
		IUserItem* pDstUserItem = m_pParentDlg->GetRoomBusiness()->GetUserManager()->SearchUserByUIN( wParam ) ;
		if ( pDstUserItem != NULL )
		{
			CString strName = pDstUserItem->GetUserName().c_str() ;
			CString strUUID ;
			strUUID.Format(L"%u" , wParam ) ;
			m_pParentDlg->OnClickNameLink( strName , strUUID ) ;
		}
	}
	return S_OK ;
}

LRESULT CP2PChatDlg::MicPopupDlgClose( WPARAM wParam , LPARAM lParam ) 
{
	core::CurInfo myCurInfo = GetCurInfo() ; 
	if ( wParam == myCurInfo.unUIN )
	{
		m_pSelfVideoDlg->m_pToolBarDlg->SetBtnPopupStatus( BTN_STATUS_OPEN ) ;
		RoomAVLogic* pRoomAVLogic = m_pParentDlg->GetRoomAvLogic() ;
		if ( pRoomAVLogic != NULL )
		{
			pRoomAVLogic->LocalAVChg( m_pSelfVideoDlg->GetSafeHwnd() ,m_pSelfMicPopupDlg->m_pVideoDlg->GetSafeHwnd() ) ;
		}
	}
	else
	{
		m_pOpVideoDlg->m_pToolBarDlg->SetBtnPopupStatus( BTN_STATUS_OPEN ) ;
		RoomAVLogic* pRoomAVLogic = m_pParentDlg->GetRoomAvLogic() ;
		if ( pRoomAVLogic != NULL )
		{
			pRoomAVLogic->RecvAVChg( wParam , m_pOpVideoDlg->GetSafeHwnd() , RECV_VIDEO_WIDTH , 
				RECV_VIDEO_HIEGHT , m_pOpMicPopupDlg->m_pVideoDlg->GetSafeHwnd() ) ;
		}
	}
	return S_OK ;
}

BOOL CP2PChatDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	NotifyCreateSkinItem();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void CP2PChatDlg::NotifyCreateSkinItem()
{
	if (m_pOpVideoDlg->GetSafeHwnd())
	{
		NOTIFY_SKIN_HELPER(NOTIFY_SKIN_OP_VIDEO_DLG_HWND,m_pOpVideoDlg->GetSafeHwnd());
	}
	if (m_pSelfVideoDlg->GetSafeHwnd())
	{
		NOTIFY_SKIN_HELPER(NOTIFY_SKIN_SELF_VIDEO_DLG_HWND,m_pSelfVideoDlg->GetSafeHwnd());
	}
}

void CP2PChatDlg::OnClickClose()
{
	m_pParentDlg->SendMessage( WM_P2PMICDLG_CLOSE , m_unOpSrcUIN , 0 ) ;
}

void CP2PChatDlg::OnClickEndChat()
{
    OnClickClose();
}

void CP2PChatDlg::Init( uint32 unUIN , uint32 un179ID , uint16 unStatus , CString strName ) 
{
	m_pOpVideoDlg->Init( unUIN , unStatus ) ;
	m_unOpSrcUIN = unUIN ;
	m_strOpName = strName ;
	m_unOp179ID = un179ID ;
	m_pOpVideoDlg->m_bNeedHideTooBal = FALSE ;

	CString steTemp ;
	steTemp.Format( L"与%s视频聊天中" ,strName ) ;
	CComVariant var = steTemp ;
	m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(),L"textChatTitle",L"text",var);	

	RoomAVLogic* pRoomAVLogic = m_pParentDlg->GetRoomAvLogic() ;
	if ( pRoomAVLogic != NULL )
	{
		pRoomAVLogic->RecvAV( unUIN , GetMicHandle( FALSE ) ) ;
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

void CP2PChatDlg::InitMySelf( uint32 unUIN ,uint16 unStatus ) 
{
	m_pSelfVideoDlg->Init( unUIN , unStatus ) ;
	m_pSelfVideoDlg->m_bNeedHideTooBal = FALSE ;

	RoomAVLogic* pRoomAVLogic = m_pParentDlg->GetRoomAvLogic() ;
	if ( pRoomAVLogic != NULL )
	{
		pRoomAVLogic->UpSelf( GetMicHandle( TRUE) ) ;
		pRoomAVLogic->LocalAVEffact( GetMicHandle(TRUE) , FALSE ) ;
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
}


void CP2PChatDlg::OnOpVideoAudioRsp( uint32 nOpAVType ,uint32 unUIN )
{
	core::CurInfo myCurInfo = GetCurInfo() ; 
	switch( nOpAVType )
	{
	case core::OPERATEAVTYPE_OPEN_VIDEO:
		{
			OpenVideo( unUIN ) ;
			if ( myCurInfo.unUIN == unUIN )
			{
				m_pSelfVideoDlg->OpenVideoRsp(unUIN) ;	
			}
			else
			{
				m_pOpVideoDlg->OpenVideoRsp(unUIN) ;	
			}
		}
		break;
	case core::OPERATEAVTYPE_OPEN_AUDIO:
		{
			OpenAudio( unUIN ) ;
			if ( myCurInfo.unUIN == unUIN )
			{
				m_pSelfVideoDlg->OpenAudioRsp(unUIN) ;	
			}
			else
			{
				m_pOpVideoDlg->OpenAudioRsp(unUIN) ;	
			}
		}
		break;
	case core::OPERATEAVTYPE_CLOSE_VIDEO:
		{
			CloseVideo( unUIN ) ;
			if ( myCurInfo.unUIN == unUIN )
			{
				m_pSelfVideoDlg->CloseVideoRsp(unUIN) ;
			}
			else
			{
				m_pOpVideoDlg->CloseVideoRsp(unUIN) ;
			}
		}
		break;
	case core::OPERATEAVTYPE_CLOSE_AUDIO:
		{
			CloseAudio( unUIN ) ;
			if ( myCurInfo.unUIN == unUIN )
			{
				m_pSelfVideoDlg->CloseAudioRsp(unUIN) ;
			}
			else
			{
				m_pOpVideoDlg->CloseAudioRsp(unUIN) ;
			}
		}
		break;
	default:break;
	}
}

void CP2PChatDlg::OnOpVideoAudioNotify( uint32 nOpAVType ,uint32 unUIN ) 
{
	core::CurInfo myCurInfo = GetCurInfo() ; 
	switch( nOpAVType )
	{
	case core::OPERATEAVTYPE_OPEN_VIDEO:
		{
			OpenVideo( unUIN ) ;
			if ( myCurInfo.unUIN == unUIN )
			{
				m_pSelfVideoDlg->OpenVideoNotify(unUIN ) ;	
			}
			else
			{
				m_pOpVideoDlg->OpenVideoNotify(unUIN ) ;	
			}
		}
		break;
	case core::OPERATEAVTYPE_OPEN_AUDIO:
		{
			OpenAudio( unUIN ) ;
			if ( myCurInfo.unUIN == unUIN )
			{
				m_pSelfVideoDlg->OpenAudioNotify(unUIN) ;	
			}
			else
			{
				m_pOpVideoDlg->OpenAudioNotify(unUIN) ;	
			}
		}
		break;
	case core::OPERATEAVTYPE_CLOSE_VIDEO:
		{
			CloseVideo( unUIN ) ;
			if ( myCurInfo.unUIN == unUIN )
			{
				m_pSelfVideoDlg->CloseVideoNotify(unUIN) ;
			}
			else
			{
				m_pOpVideoDlg->CloseVideoNotify(unUIN) ;
			}
		}
		break;
	case core::OPERATEAVTYPE_CLOSE_AUDIO:
		{
			CloseAudio( unUIN ) ;
			if ( myCurInfo.unUIN == unUIN )
			{
				m_pSelfVideoDlg->CloseAudioNotify(unUIN) ;
			}
			else
			{
				m_pOpVideoDlg->CloseAudioNotify(unUIN) ;
			}
		}
		break;
	default:break;
	}
}

HWND CP2PChatDlg::GetMicHandle(BOOL bSelf)
{
	if (bSelf)
	{
		return m_pSelfVideoDlg->GetSafeHwnd();
	}else
	{
		return m_pOpVideoDlg->GetSafeHwnd();
	}
}

void CP2PChatDlg::SetDefault(BOOL bSelf)
{ 
    if (bSelf)
    {
		m_pSelfVideoDlg->SetToDefault();
    }else
	{
		m_pOpVideoDlg->SetToDefault();
	}
}

uint32 CP2PChatDlg::GetOppsiteUIN() 
{
	return m_unOpSrcUIN ;
}


void CP2PChatDlg::CleanAVLogic() 
{
	RoomAVLogic* pRoomAVLogic = m_pParentDlg->GetRoomAvLogic() ;
	if ( pRoomAVLogic != NULL )
	{
		pRoomAVLogic->DownSelf() ;
		pRoomAVLogic->RemoveAV( m_unOpSrcUIN ) ;
	}
}

BOOL CP2PChatDlg::PreTranslateMessage(MSG* pMsg)
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

void CP2PChatDlg::OnNcDestroy()
{
	CDialog::OnNcDestroy();
	// TODO: 在此处添加消息处理程序代码
}

void CP2PChatDlg::OpenVideo( uint32 unUIN ) 
{
	core::CurInfo myCurInfo = GetCurInfo() ; 
	RoomAVLogic* pRoomAVLogic = m_pParentDlg->GetRoomAvLogic() ;
	if ( myCurInfo.unUIN == unUIN )
	{
		if ( pRoomAVLogic != NULL )
		{
			pRoomAVLogic->PauseLocalVideo( false ) ;				
		}			
	}
	else
	{
		if ( pRoomAVLogic != NULL )
		{
			pRoomAVLogic->PauseRemoteVideo( unUIN , false ) ;
		}
	}
}

void CP2PChatDlg::CloseVideo( uint32 unUIN ) 
{
	core::CurInfo myCurInfo = GetCurInfo() ; 
	RoomAVLogic* pRoomAVLogic = m_pParentDlg->GetRoomAvLogic() ;
	if ( pRoomAVLogic == NULL )
	{
		return ;
	}

	if ( myCurInfo.unUIN == unUIN )
	{
		if ( m_pSelfMicPopupDlg != NULL && m_pSelfMicPopupDlg->IsWindowVisible() )
		{
			pRoomAVLogic->LocalAVChg( m_pSelfVideoDlg->GetSafeHwnd() ,m_pSelfMicPopupDlg->m_pVideoDlg->GetSafeHwnd() ) ;
			m_pSelfMicPopupDlg->ShowWindow( SW_HIDE ) ;
		}		
		pRoomAVLogic->PauseLocalVideo( true ) ;
	}
	else
	{
		if ( m_pOpMicPopupDlg != NULL && m_pOpMicPopupDlg->IsWindowVisible() )
		{
			pRoomAVLogic->RecvAVChg( unUIN , m_pOpVideoDlg->GetSafeHwnd() , RECV_VIDEO_WIDTH , 
				RECV_VIDEO_HIEGHT , m_pOpMicPopupDlg->m_pVideoDlg->GetSafeHwnd() ) ;
			m_pOpMicPopupDlg->ShowWindow( SW_HIDE ) ;
		}
		pRoomAVLogic->PauseRemoteVideo( unUIN , true ) ;
	}
}

void CP2PChatDlg::OpenAudio( uint32 unUIN ) 
{
	core::CurInfo myCurInfo = GetCurInfo() ; 
	RoomAVLogic* pRoomAVLogic = m_pParentDlg->GetRoomAvLogic() ;
	if ( myCurInfo.unUIN == unUIN )
	{
		if ( pRoomAVLogic != NULL )
		{
			pRoomAVLogic->PauseLocalAudio( false ) ;				
		}
	}
	else
	{
		if ( pRoomAVLogic != NULL )
		{
			pRoomAVLogic->PauseRemoteAudio( unUIN , false ) ;
		}
	}
}

void CP2PChatDlg::CloseAudio( uint32 unUIN ) 
{
	core::CurInfo myCurInfo = GetCurInfo() ; 
	RoomAVLogic* pRoomAVLogic = m_pParentDlg->GetRoomAvLogic() ;
	if ( myCurInfo.unUIN == unUIN )
	{
		if ( pRoomAVLogic != NULL )
		{
			pRoomAVLogic->PauseLocalAudio( true ) ;				
		}
	}
	else
	{
		if ( pRoomAVLogic != NULL )
		{
			pRoomAVLogic->PauseRemoteAudio( unUIN , true ) ;
		}
	}
}