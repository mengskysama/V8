// RoomLeftDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "Room.h"
#include "RoomLeftDlg.h"
#include "skin_i.h"
#include "SkinUtil.h"
#include "micwindow\PublicMicDlg.h"
#include "RoomParentDlg.h"
#include "personalsetting/PersonalConfig.h"
#include "roomsetting/RoomManagerDlg.h"
#include "SendEventManager.h"
#include "micwindow\MicToolBarDlg.h"

#include "..\CommonLibrary\include\utility\SystemHelper.h"
#include "..\CommonLibrary\include\ui\C179MsgBox.h"

#define NOTIFY_SKIN_ROOM_PUBLIC_MIC1_DLG   1
#define NOTIFY_SKIN_ROOM_PUBLIC_MIC2_DLG   2
#define NOTIFY_SKIN_ROOM_PUBLIC_MIC3_DLG   3
#define SKIN_NOTIFY_PUBLICMIC_DIS          4
#define SKIN_ON_CLICK_PAIMIC               5
#define SKIN_ON_CLICK_ICON_MIC0            6
#define SKIN_ON_CLICK_ICON_MIC1            7
#define SKIN_ON_CLICK_ICON_SPEAKER0        8
#define SKIN_ON_CLICK_ICON_SPEAKER1        9
#define SKIN_ON_CLICK_BTN_USERSETTING      10
#define SKIN_ON_CLICK_BTN_ROOMMGR          11

#define THE_DLG_EVERITHING_HEIGHT          641  //三个公麦高(198*3) + (排麦.top 到 本对话框.bottom的高)(47)，用于计算不同分辨率下公麦间距
#define MAX_PUBLIC_MIC_DIS                 50

#define ROOM_MANAGER_WIDTH	579
#define ROOM_MANAGER_HEIGHT	442
// CRoomLeftDlg 对话框

IMPLEMENT_DYNAMIC(CRoomLeftDlg, CDialog)

CRoomLeftDlg::CRoomLeftDlg( CRoomParentDlg* pParentDlg,CWnd* pParent /*=NULL*/)
	: CDialog(CRoomLeftDlg::IDD, pParent),
	m_pParentDlg(pParentDlg)
{
	m_pSkinEngine = NULL;
	::GetSkinEngine(&m_pSkinEngine);
	m_nPaiMaiBtn_Status = PAIMAIBTN_ANY ;
	//m_pRoomManagerDlg = NULL ;
	//m_pSysSettingDlg = NULL;

	m_menuPaiMai.CreatePopupMenu() ;
	m_menuPaiMai.AppendMenuEx( MF_STRING , ROOMDLG_PAIMAI_PUBLIC ,_T("公麦") ) ;
	m_menuPaiMai.AppendMenuEx( MF_STRING , ROIMDLG_PAIMAI_PRIVATE , _T("私麦") ) ;

}

CRoomLeftDlg::~CRoomLeftDlg()
{
	m_menuPaiMai.DestroyMenu() ;
	map<int ,CPublicMicDlg*>::iterator iter_public ;
	for ( iter_public = m_mPublicMicDlg.begin() ; iter_public != m_mPublicMicDlg.end() ; ++iter_public )
	{
		CPublicMicDlg* pPublicMicDlg = iter_public->second ;
		SAFE_DELETE_DIALOG(pPublicMicDlg);
	}
	//SAFE_DELETE_DIALOG(m_pRoomManagerDlg) ;
	//SAFE_DELETE_DIALOG(m_pSysSettingDlg) ;
}

void CRoomLeftDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//DDX_Control(pDX, IDC_SLIDER_ROOM_MIC, m_MicSliderCtrl);
	//DDX_Control(pDX, IDC_SLIDER_ROOM_SPEAKER, m_SpeakerSliderCtrl);
}


BEGIN_MESSAGE_MAP(CRoomLeftDlg, CDialog)
	ON_WM_CREATE()
	ON_SKIN_MESSAGE
	ON_WM_HSCROLL()
	ON_WM_SIZE()
END_MESSAGE_MAP()


// CRoomLeftDlg 消息处理程序

int CRoomLeftDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  在此添加您专用的创建代码
	//LoadSkin
	ASSERT(NULL != m_pSkinEngine);
	if (NULL != m_pSkinEngine)
	{
		HRESULT hr = m_pSkinEngine->LoadSkin(GetSafeHwnd(),L"RoomLeftDlg");
		ASSERT(hr== S_OK && __LINE__);
	}

	return 0;
}

BOOL CRoomLeftDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
   
	// TODO:  在此添加额外的初始化

	//创建3个公麦窗口
	for(int i=1;i<=3;++i)
	{
		CPublicMicDlg *pPublicMicDlg = new CPublicMicDlg(m_pParentDlg , i ) ;
		if (pPublicMicDlg)
		{
			pPublicMicDlg->Create(CPublicMicDlg::IDD,this);
			/*if (2 == i)
			{
				pPublicMicDlg->SetWindowPos(NULL,2, ROOM_PUBLIC_MIC1_TOP+(i-1)*(ROOM_PUBLIC_MIC_HEIGHT+2),
					ROOM_PUBLIC_MIC_WIDTH,ROOM_PUBLIC_MIC_HEIGHT,SWP_SHOWWINDOW);
			}else
			{
				pPublicMicDlg->SetWindowPos(NULL,ROOM_PUBLIC_MIC1_LEFT,ROOM_PUBLIC_MIC1_TOP+(i-1)*(ROOM_PUBLIC_MIC_HEIGHT+2),
					ROOM_PUBLIC_MIC_WIDTH,ROOM_PUBLIC_MIC_HEIGHT,SWP_SHOWWINDOW);
			}*/
			
			m_mPublicMicDlg[i] = pPublicMicDlg;
		}
	}

    NotifyCreateSkinItem();
	//InitMicSlider() ;

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

BOOL CRoomLeftDlg::PreTranslateMessage(MSG* pMsg)
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


LRESULT CRoomLeftDlg::OnSkinMessage(WPARAM wParam,LPARAM lParam)
{
    switch(wParam)
	{
	case SKIN_ON_CLICK_PAIMIC:
		{
            OnClickPaiMic();
 		}
		break;
	case SKIN_ON_CLICK_ICON_MIC0:
		{			
			//OnClickBtnCloseMic() ;
		}
		break;
	case SKIN_ON_CLICK_ICON_MIC1:
		{
			//OnClickBtnOpenMic() ;			
		}
		break;
	case SKIN_ON_CLICK_ICON_SPEAKER0:
		{
			//OnClickBtnCloseSound() ;			
		}
		break;
	case SKIN_ON_CLICK_ICON_SPEAKER1:
		{			
			//OnClickBtnOpenSound() ;
		}
		break;
	case SKIN_ON_CLICK_BTN_USERSETTING:
		{
			//OnClickUserSetting();
		}
		break;
	case SKIN_ON_CLICK_BTN_ROOMMGR:
		{
			//OnClickRoomMgr();
		}
		break;

	default:break;
	}

	return TRUE;
}

void CRoomLeftDlg::NotifyCreateSkinItem()
{
	map<int , CPublicMicDlg*>::iterator iter ;
	int i = 0 ;
	for ( iter = m_mPublicMicDlg.begin() ; iter != m_mPublicMicDlg.end() ; ++iter ,++i)
	{
		CPublicMicDlg* pPbulicMicDlg = iter->second ;
		if ( pPbulicMicDlg->GetSafeHwnd() != NULL )
		{
			NotifySkinHelper( GetSafeHwnd() ,NOTIFY_SKIN_ROOM_PUBLIC_MIC1_DLG+i , pPbulicMicDlg->GetSafeHwnd() ) ;
		}
	}
}
//
//void CRoomLeftDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
//{
//	// TODO: 在此添加消息处理程序代码和/或调用默认值
//	/*CSliderCtrl* pSlide = (CSliderCtrl*)pScrollBar;
//	UINT uId = pScrollBar->GetDlgCtrlID();
//	if (uId == IDC_SLIDER_ROOM_MIC)
//	{
//		int mic =m_MicSliderCtrl.GetPos() ;
//		if ( m_pParentDlg->GetRoomAvLogic()!= NULL )
//		{
//			m_pParentDlg->GetRoomAvLogic()->SetMicRoomAllVolume( mic ) ;
//			m_MicSliderCtrl.Invalidate(FALSE);
//		}
//		m_pParentDlg->GetRoomBusiness()->UpdateAllRoomMic( mic ) ;
//		CPersonalConfig::GetInstance()->SetRoomMicVolume( mic ) ;
//	}
//	else if (uId == IDC_SLIDER_ROOM_SPEAKER )
//	{
//		int speaker = m_SpeakerSliderCtrl.GetPos() ;
//		if ( m_pParentDlg->GetRoomAvLogic() != NULL )
//		{
//			m_pParentDlg->GetRoomAvLogic()->SetSpeakerRoomVolume( speaker ) ; 
//			m_SpeakerSliderCtrl.Invalidate(FALSE);
//		}
//		CPersonalConfig::GetInstance()->SetRoomSpeakerVolume( speaker ) ;
//	}*/
//
//	//CDialog::OnHScroll(nSBCode, nPos, pScrollBar);
//}

void CRoomLeftDlg::SetPublicMicDis()
{
	if (IsWindowVisible())
	{
		CRect clientRc(0,0,0,0);
		//ClientRect(&clientRc);
		m_pSkinEngine->GetSubSkinPos(GetSafeHwnd(),L"Window",&(clientRc.left),&(clientRc.top),&(clientRc.right),&(clientRc.bottom));

		int publicMicDis =  0;
		if ((publicMicDis = clientRc.Height() - THE_DLG_EVERITHING_HEIGHT) > 0)
		{
			if (publicMicDis/2 <= MAX_PUBLIC_MIC_DIS)
			{
				//NOTIFY_SKIN_HELPER(SKIN_NOTIFY_PUBLICMIC_DIS,(LONG)publicMicDis/2);
				NOTIFY_SKIN_HELPER(SKIN_NOTIFY_PUBLICMIC_DIS,(LONG)2);
			}
			else
			{
				NOTIFY_SKIN_HELPER(SKIN_NOTIFY_PUBLICMIC_DIS,MAX_PUBLIC_MIC_DIS);
			}
		}
	}
}

void CRoomLeftDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码
	SetPublicMicDis();
}

void CRoomLeftDlg::OnClickPaiMic()
{
	core::CurInfo myCurInfo = GetCurInfo() ;
	if ( myCurInfo.enmVIPLevel == core::VIPLEVEL_NONE )
	{
		C179MsgBox::Show( this, NewMsgBox::MSGBOX_GUEST_OPERATE , L"游客不能排麦，" , L"想要畅玩白兔KTV？" , L"" , L"快去注册吧" , 
			m_pParentDlg->GetRoomBusiness()->GetRegisteUrl() ); 
		return ;
	}	
	switch( m_nPaiMaiBtn_Status)
	{
	case PAIMAIBTN_ANY:
		{
			DisableBtnPaiMai() ;
			CSendEventManager::SendRequestShow(m_pParentDlg->GetRoomID(), REQUESTSHOWTYPE_ANY);
			
		}
		break;
	case PAIMAIBTN_FANGMAI:
		{
			m_pParentDlg->FangMai( true ) ;
		}
		break;
	default:break ;
	}
}

//创建设置对话框
//void CRoomLeftDlg::CreateSysSettingDlg()
//{
//	if(m_pSysSettingDlg == NULL)
//	{
//		//创建菜单设置
//		m_pSysSettingDlg = new CSysSettingDlg(m_pParentDlg);
//		SwitchResourceToModule(_T("Room"));
//		m_pSysSettingDlg->Create(CSysSettingDlg::IDD,this);
//		m_pSysSettingDlg->SetToSysSetting();		
//		m_pSysSettingDlg->ShowWindow(SW_HIDE);
//	
//	}
//	
//}

//void CRoomLeftDlg::OnClickUserSetting()
//{
//	CreateSysSettingDlg();	
//	if(m_pSysSettingDlg)
//	{
//		m_pSysSettingDlg->OpenDlg(Show_Basic_Setting_Dlg);
//		m_pSysSettingDlg->CenterWindow(m_pParentDlg);
//		m_pSysSettingDlg->ShowWindow(SW_SHOW);
//	}
//}
//
//void CRoomLeftDlg::OnClickRoomMgr()
//{
//	SwitchResourceToModule(_T("Room"));
//	if (m_pRoomManagerDlg == NULL)
//	{
//		//创建房间管理窗口
//		m_pRoomManagerDlg = new CRoomManagerDlg(m_pParentDlg);
//		if (m_pRoomManagerDlg)
//		{
//			m_pRoomManagerDlg->Create(CRoomManagerDlg::IDD,this);
//			m_pRoomManagerDlg->SetWindowPos(NULL, 0, 0, ROOM_MANAGER_WIDTH, ROOM_MANAGER_HEIGHT, SWP_SHOWWINDOW);
//			m_pRoomManagerDlg->CenterWindow(m_pParentDlg);
//			return;
//		}
//	}
//
//	if (m_pRoomManagerDlg != NULL && m_pRoomManagerDlg->GetSafeHwnd())
//	{
//		m_pRoomManagerDlg->ShowWindow(SW_SHOW);
//		m_pRoomManagerDlg->CenterWindow(m_pParentDlg);
//		m_pRoomManagerDlg->SetForegroundWindow();
//	}
//}

//void CRoomLeftDlg::OnClickBtnCloseMic() //关闭麦克风
//{
//	g_RoomModule->CloseMic() ;
//}
//
//void CRoomLeftDlg::OnClickBtnOpenMic() 
//{
//	g_RoomModule->OpenMic() ;
//}
//
//void CRoomLeftDlg::OnClickBtnCloseSound() //关闭音量
//{
//	if (  m_pParentDlg->GetRoomAvLogic() != NULL )
//	{
//		CComVariant var = false;
//		m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(),L"imgSpeaker0",L"visible",var);
//		var = true;
//		m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(),L"imgSpeaker1",L"visible",var);
//		 m_pParentDlg->GetRoomAvLogic()->SetSondMute( true ) ;
//	}
//}
//
//void CRoomLeftDlg::OnClickBtnOpenSound() 
//{
//	if ( m_pParentDlg->GetRoomAvLogic() != NULL )
//	{
//		CComVariant var = true;
//		m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(),L"imgSpeaker0",L"visible",var);
//		var = false;
//		m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(),L"imgSpeaker1",L"visible",var);
//		 m_pParentDlg->GetRoomAvLogic()->SetSondMute( false ) ;
//	}
//}

//void CRoomLeftDlg::InitMicSlider()
//{
//	if ( m_pParentDlg->GetRoomAvLogic() != NULL )
//	{
//		m_SpeakerSliderCtrl.SetRange( 0 , 255 ) ;
//		m_MicSliderCtrl.SetRange( 0 , 255 ) ;
//		int speaker = CPersonalConfig::GetInstance()->GetRoomSpeakerVolume() ;
//		int mic = CPersonalConfig::GetInstance()->GetRoomMicVolume() ;
//		m_pParentDlg->GetRoomAvLogic()->SetSpeakerVolume( speaker ) ;
//		m_pParentDlg->GetRoomAvLogic()->SetMicRoomAllVolume( mic ) ;
//		m_SpeakerSliderCtrl.SetPos(  speaker ) ;
//		m_MicSliderCtrl.SetPos( mic ) ;
//	}
//}

//进房时公麦列表
void CRoomLeftDlg::InitPublicMic( std::vector<core::room_user_info>& vecPubMic ) 
{
	

	std::vector<core::room_user_info>::iterator iter ;
	for ( iter = vecPubMic.begin() ; iter != vecPubMic.end() ; ++iter )
	{			
		uint32 micIndex = 0;
		BOOL bActor = (   (iter->unBusiness & core::BUSINESSTYPE_SONGER) 
					   || (iter->unBusiness & core::BUSINESSTYPE_DANCER) 
					   || (iter->unBusiness & core::BUSINESSTYPE_HOST) );//是否是艺人
		if ( iter->unStatus & core::USERSTATUSTYPE_PUBLIC_1 )
		{			
			m_mPublicMicDlg[1]->Init( iter->unUIN , iter->un179id , iter->unStatus , iter->strName.c_str() , 0, bActor) ;
			micIndex = 1;
		}
		else if ( iter->unStatus & core::USERSTATUSTYPE_PUBLIC_2 )
		{
			m_mPublicMicDlg[2]->Init( iter->unUIN , iter->un179id ,iter->unStatus ,  iter->strName.c_str() , 0, bActor) ;
			micIndex = 2;
		}
		else if ( iter->unStatus & core::USERSTATUSTYPE_PUBLIC_3 )
		{
			m_mPublicMicDlg[3]->Init( iter->unUIN , iter->un179id , iter->unStatus , iter->strName.c_str() , 0, bActor) ;
			micIndex = 3;
		}

		
		if ( micIndex != 0 && ( (iter->unBusiness & core::BUSINESSTYPE_SONGER) 
								|| (iter->unBusiness & core::BUSINESSTYPE_DANCER) 
								|| (iter->unBusiness & core::BUSINESSTYPE_HOST) ) )//如果公麦上的是艺人
		{
			 m_pParentDlg->DrawBtnFans(micIndex);
			// m_pParentDlg->DrawBtnUnDealSong(micIndex);
		}

		
	}
}
//
//void CRoomLeftDlg::OnAddUser( IUserItem * pUserItem ) 
//{
//	ASSERT(pUserItem != NULL);
//	//是自己
//	if (pUserItem->GetUserUIN() == GetCurInfo().unUIN)
//	{
//		if (pUserItem->GetUserTitleLevel() >= core::TITLELEVEL_MANAGER)
//		{
//			CComVariant var = FALSE;
//			m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(), L"btnRoomMgr", L"disabled", var);
//		}
//	}
//}
//
//void CRoomLeftDlg::OnUpdateUser(IUserItem * pUserItem)
//{
//	ASSERT(pUserItem != NULL);
//	if (pUserItem->GetUserUIN() == GetCurInfo().unUIN)
//	{
//		if (pUserItem->GetUserTitleLevel() >= core::TITLELEVEL_MANAGER)
//		{
//			CComVariant var = FALSE;
//			m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(), L"btnRoomMgr", L"disabled", var);
//		}
//		else
//		{
//			CComVariant var = TRUE;
//			m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(), L"btnRoomMgr", L"disabled", var);
//		}
//	}
//}

void CRoomLeftDlg::ClosePublicMic(uint32 UIN ) 
{
	IUserItem* pUserItem = m_pParentDlg->GetRoomBusiness()->GetUserManager()->SearchUserByUIN( UIN ) ;
	if ( pUserItem == NULL )
	{
		return ;
	}
	uint16 nIndex = pUserItem->GetPublicMicIndex() ; 
	if ( nIndex != 0 )
	{
		m_mPublicMicDlg[nIndex]->CleanAVLogic() ;
		m_mPublicMicDlg[nIndex]->SetDefault() ;
		m_mPublicMicDlg[nIndex]->SetBtnOrderSongVisible(false);
		m_pParentDlg->DeleteFansBtn(UIN);
		m_pParentDlg->DeleteUnDealSongBtn(UIN);
	}
}

void CRoomLeftDlg::DisableBtnPaiMai() 
{
	CComVariant var = true ;
	m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(),L"btnPaiMic",L"disabled",var );
}

void CRoomLeftDlg::EnablePaiMai() 
{
	CComVariant var = false ;
	m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(),L"btnPaiMic",L"disabled",var );
}

void CRoomLeftDlg::SetPaiMaiBtnStatus( ENM_PaiMaiBtn_Status nStatus ) 
{
	CComVariant varPicture ;
	CComVariant varText ;
	switch( nStatus)
	{
	case PAIMAIBTN_ANY:
		{
			//varPicture = L"Room_btnPaiMic_N.png" ;
			varPicture = TRUE;
			varText = L"排麦" ;			
		}
		break;
	case PAIMAIBTN_PAIMAI:
		{
			//varPicture = L"Room_btnPaiMic_N.png" ;
			varPicture = TRUE;
			varText = L"排麦" ;
		}
		break;
	case PAIMAIBTN_FANGMAI:
		{
			//varPicture = L"Room_btnFangMic.png" ;
			varPicture = FALSE;
			varText = L"放麦" ;
		}
		break;
	default:break;
	}
	m_nPaiMaiBtn_Status = nStatus ;
	//m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(),L"btnPaiMic",L"normalimage",varPicture);
	//m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(),L"btnPaiMic",L"hoverImage",varPicture);
	//m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(),L"btnPaiMic",L"downImage",varPicture);

	m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(),L"btnPaiMic",L"visible",varPicture);
	CComVariant varTemp = !varPicture.boolVal;
	m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(),L"btnFangMic",L"visible",varTemp);

	m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(),L"btnPaiMic",L"tip",varText);
}

//void CRoomLeftDlg::SetRoomMicVolume( uint32 unMicVolume ) 
//{
//	/*m_MicSliderCtrl.SetPos( unMicVolume ) ;
//	m_MicSliderCtrl.Invalidate(FALSE);*/
//}

void CRoomLeftDlg::CloseMic() 
{
	core::CurInfo myCurInfo = GetCurInfo() ; 
	IUserItem* pUserIterm = m_pParentDlg->GetRoomBusiness()->GetUserManager()->SearchUserByUIN( myCurInfo.unUIN ) ;
	if ( pUserIterm != NULL )
	{
		int unIndex = pUserIterm->GetPublicMicIndex() ;
		if ( unIndex>=1 && unIndex<= 3 )
		{
			m_mPublicMicDlg[unIndex]->ChangeAudio( myCurInfo.unUIN , BTN_STATUS_OPEN ) ;
		}
	}
}

void CRoomLeftDlg::OpenMic() 
{
	core::CurInfo myCurInfo = GetCurInfo() ; 
	IUserItem* pUserIterm = m_pParentDlg->GetRoomBusiness()->GetUserManager()->SearchUserByUIN( myCurInfo.unUIN ) ;
	if ( pUserIterm != NULL )
	{
		int unIndex = pUserIterm->GetPublicMicIndex() ;
		if ( unIndex>=1 && unIndex<= 3 )
		{
			m_mPublicMicDlg[unIndex]->ChangeAudio( myCurInfo.unUIN , BTN_STATUS_CLOSE ) ;
		}
	}
}

uint32 CRoomLeftDlg::GetPulicMicUINByIndex( int nIndex )
{
	if ( nIndex>=1 && nIndex<= 3 )
	{
		return m_mPublicMicDlg[nIndex]->GetUserUIN() ;
	}
	else
	{
		return 0 ;
	}
}
//
//void CRoomLeftDlg::OnClickTempNotice( CString& strTempNotice , CString& strUUID ) 
//{
//	OnClickRoomMgr();
//	if ( m_pRoomManagerDlg != NULL )
//	{
//		m_pRoomManagerDlg->TurnToAnnouncePanel( strTempNotice ) ;
//	}
//}

void CRoomLeftDlg::OnRequestShow_NeedSelect()
{
	CRect rcTemp;
	m_pSkinEngine->GetSubSkinPos(GetSafeHwnd(),L"btnPaiMic",&(rcTemp.left),&(rcTemp.top),
		&(rcTemp.right),&(rcTemp.bottom));
	ClientToScreen( &rcTemp ) ;
	int nreturn = m_menuPaiMai.TrackPopupMenu( TPM_LEFTALIGN | TPM_BOTTOMALIGN|TPM_RETURNCMD  ,rcTemp.right-20 ,rcTemp.top+5 , this ) ;  //排麦下拉菜单
	if ( nreturn != 0 )
	{			
		if ( nreturn == ROOMDLG_PAIMAI_PUBLIC )
		{
			CSendEventManager::SendRequestShow( m_pParentDlg->GetRoomID(), REQUESTSHOWTYPE_PUBLIC ) ;
		}
		else if ( nreturn == ROIMDLG_PAIMAI_PRIVATE )
		{
			CSendEventManager::SendRequestShow( m_pParentDlg->GetRoomID(), REQUESTSHOWTYPE_PRIVATE ) ;
		}		
	}
	EnablePaiMai() ;
}

void CRoomLeftDlg::OnRequestShow_Failed()
{
	SetPaiMaiBtnStatus( PAIMAIBTN_ANY ) ;
	EnablePaiMai() ;
}

void CRoomLeftDlg::OnRequestShow_Public(uint8 unIndex , uint32 unTime )
{
	core::CurInfo myCurInfo = GetCurInfo() ; 
	IUserItem* pUserIterm = m_pParentDlg->GetRoomBusiness()->GetUserManager()->SearchUserByUIN( myCurInfo.unUIN ) ;
	if ( unIndex>=1 && unIndex<=3 && pUserIterm != NULL )
	{
		m_mPublicMicDlg[unIndex]->Init( pUserIterm->GetUserUIN() ,pUserIterm->GetUser179ID() , pUserIterm->GetUserStatus() ,pUserIterm->GetUserName().c_str() , unTime , pUserIterm->IsActor()) ;
		SetPaiMaiBtnStatus( PAIMAIBTN_FANGMAI ) ;
		EnablePaiMai() ;
	}
}

void CRoomLeftDlg::OnRequestShow_WaitingList()
{
	SetPaiMaiBtnStatus( PAIMAIBTN_FANGMAI ) ;
	EnablePaiMai() ;
}


void CRoomLeftDlg::OnRequestShow_PublicNotify( uint32 unSrcUIN , uint8 unIndex , uint16 unTime )
{
	IUserItem* pSrcUserItem = m_pParentDlg->GetRoomBusiness()->GetUserManager()->SearchUserByUIN( unSrcUIN ) ;
	if ( pSrcUserItem == NULL )
	{
		return ;
	}
	if (  unIndex>=1 && unIndex<= 3 )
	{
		m_mPublicMicDlg[unIndex]->Init( unSrcUIN  , pSrcUserItem->GetUser179ID() ,pSrcUserItem->GetUserStatus() , pSrcUserItem->GetUserName().c_str() , unTime , pSrcUserItem->IsActor()) ;
	}
	core::CurInfo myCurInfo = GetCurInfo() ; 
	if ( myCurInfo.unUIN == unSrcUIN )
	{
		OnRequestShow_PublicNotifySelf() ;  //从麦序中主动上麦时会收到Notify消息
	}
}

void CRoomLeftDlg::OnRequestShow_PublicNotifySelf() 
{
	SetPaiMaiBtnStatus( PAIMAIBTN_FANGMAI ) ;
	EnablePaiMai() ;
}

void CRoomLeftDlg::OnRequestShow_Private()
{
	SetPaiMaiBtnStatus( PAIMAIBTN_FANGMAI ) ;
	EnablePaiMai() ;
}

void CRoomLeftDlg::OnExitShowNotify( uint32 srcuin )
{
	core::CurInfo myCurInfo = GetCurInfo() ;
	if ( myCurInfo.unUIN == srcuin )
	{		
		SetPaiMaiBtnStatus( PAIMAIBTN_ANY ) ;
		EnablePaiMai() ;
	}
}

void CRoomLeftDlg::OnPushOnShow_Success( uint32 unDstUIN , uint8 unIndex )
{
	IUserItem* pDstUserItem = m_pParentDlg->GetRoomBusiness()->GetUserManager()->SearchUserByUIN( unDstUIN ) ;
	if ( pDstUserItem == NULL )
	{
		return ;
	}
	core::CurInfo myCurInfo = GetCurInfo() ;
	if (  unIndex>=1 && unIndex<= 3 )
	{
		m_mPublicMicDlg[unIndex]->Init( unDstUIN  , pDstUserItem->GetUser179ID() ,pDstUserItem->GetUserStatus() , pDstUserItem->GetUserName().c_str() , 0, pDstUserItem->IsActor()) ;
		if ( myCurInfo.unUIN == unDstUIN )
		{
			SetPaiMaiBtnStatus( PAIMAIBTN_FANGMAI ) ;
		}
	}
}

void CRoomLeftDlg::OnPullOffShowRsp_Success( IUserItem* pDstUserItem )
{
	core::CurInfo myCurInfo = GetCurInfo() ;
	if ( myCurInfo.unUIN == pDstUserItem->GetUserUIN() && pDstUserItem->GetPublicMicIndex() != 0 )
	{		
		SetPaiMaiBtnStatus( PAIMAIBTN_ANY ) ;
		DisableBtnPaiMai() ;
	}
}

void CRoomLeftDlg::OnOpVideoAudioRsp( uint16 nIndex ,uint32 nOpAVType ,uint32 unUIN )
{
	m_mPublicMicDlg[nIndex]->OnOpVideoAudioRsp( nOpAVType , unUIN ) ;
}

void CRoomLeftDlg::OnOpVideoAudioNotify( uint16 nIndex , uint32 nOpAVType ,uint32 unUIN ) 
{
	m_mPublicMicDlg[nIndex]->OnOpVideoAudioNotify( nOpAVType , unUIN ) ;
}

void CRoomLeftDlg::OnSetMicStatus( int16 nIndex , int16 nStatus )
{
	if ( nIndex >=1 && nIndex<=3 && nStatus == PUBLIC_MIC_STATUS_CLOSE )
	{
		uint32 unDstUIN = GetPulicMicUINByIndex( nIndex ) ;
		if (unDstUIN != 0 )
		{
			ClosePublicMic( unDstUIN ) ;
			m_pParentDlg->GetRoomBusiness()->OnUserStatusChange( unDstUIN , USERSTATUSTYPE_PUBLIC_1|USERSTATUSTYPE_PUBLIC_2|
				USERSTATUSTYPE_PUBLIC_3 ,false ) ;
			OnExitShowNotify( unDstUIN ) ;
		}
	}
}

void CRoomLeftDlg::SetPublicMicEffect(int nMic, BOOL bOnMic)
{
	if (nMic >=1 && nMic <= 3)
	{
		CString strOnMic = L"";
		CString strOutMic = L"";
		strOnMic.Format(L"imgPublicOnMic%dBk", nMic);
		strOutMic.Format(L"imgPublicOutMic%dBk", nMic);
		CComVariant var = bOnMic;
		m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(), strOnMic.GetBuffer(), L"visible", var);
		var = !bOnMic;
		m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(), strOutMic.GetBuffer(), L"visible", var);
	}
}

void CRoomLeftDlg::CleanAVLogic() 
{
	map<int ,CPublicMicDlg*>::iterator iter_public ;
	for ( iter_public = m_mPublicMicDlg.begin() ; iter_public != m_mPublicMicDlg.end() ; ++iter_public )
	{
		CPublicMicDlg* pPublicMicDlg = iter_public->second ;
		pPublicMicDlg->CleanAVLogic() ;
	}
}

//关闭公屏上的视频
void CRoomLeftDlg::CloseVideo() 
{
	map<int ,CPublicMicDlg*>::iterator iter_public ;
	for ( iter_public = m_mPublicMicDlg.begin() ; iter_public != m_mPublicMicDlg.end() ; ++iter_public )
	{
		CPublicMicDlg* pPublicMicDlg = iter_public->second ;
		pPublicMicDlg->CloseVideo() ;
	}
}

void CRoomLeftDlg::OpenVideo() 
{
	map<int ,CPublicMicDlg*>::iterator iter_public ;
	for ( iter_public = m_mPublicMicDlg.begin() ; iter_public != m_mPublicMicDlg.end() ; ++iter_public )
	{
		CPublicMicDlg* pPublicMicDlg = iter_public->second ;
		pPublicMicDlg->OpenVideo() ;
	}
}

//void CRoomLeftDlg::ShowUserSetting( SysSettingDlgType enmType )
//{
//	CreateSysSettingDlg();	
//	if(m_pSysSettingDlg)
//	{
//		m_pSysSettingDlg->OpenDlg(enmType);
//		m_pSysSettingDlg->CenterWindow(m_pParentDlg);
//		m_pSysSettingDlg->ShowWindow(SW_SHOW);
//	}
//}

void CRoomLeftDlg::GetPublicMicRect(int micIndex, CRect &rcMic)
{
	if (micIndex >= 1 && micIndex <= 3)
	{
		CString strTemp;
		strTemp.Format(L"itemPublicMic%d", micIndex);
		m_pSkinEngine->GetSubSkinPos(GetSafeHwnd(), strTemp.GetBuffer(), &(rcMic.left), &(rcMic.top), &(rcMic.right), &(rcMic.bottom));
		ClientToScreen(&rcMic);
	}
	
}