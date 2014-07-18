// ShowHarassmentDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "..\Room.h"
#include "ShowSoundSettingDlg.h"
#include "skin_i.h"
#include "SkinUtil.h"
#include "ShowAudioTestDlg.h"
#include "..\personalsetting/PersonalConfig.h"
#include "utility\SystemHelper.h"
#include "..\GlobalRoomDefine.h"
#include "..\SendEventManager.h"

#define SKIN_ON_CLICK_AUDIO_TEST               1
#define SKIN_ON_CLICK_MORE_AUDIO_SETTING       2
#define SKIN_ON_CLICK_GOTO_FRESH_ROOM          3
// CShowSoundSetting 对话框

#define SYSSET_INI_APPNAME_BEGINNERHELP	L"BeginnerHelp"
#define SYSSET_INI_KEYNAME_ROOMID L"RoomID"

IMPLEMENT_DYNAMIC(CShowSoundSettingDlg, CDialog)

CShowSoundSettingDlg::CShowSoundSettingDlg(CWnd* pParent /*=NULL*/)
: CDialog(CShowSoundSettingDlg::IDD, pParent)
{
	m_pSkinEngine = NULL;
	::GetSkinEngine(&m_pSkinEngine);

	m_pAudioTest = NULL;
}

CShowSoundSettingDlg::~CShowSoundSettingDlg()
{
	if (m_pAudioTest != NULL)
	{
		m_pAudioTest->DestroyWindow();
		delete m_pAudioTest;
		m_pAudioTest = NULL;
	}
}

void CShowSoundSettingDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CShowSoundSettingDlg, CDialog)
	ON_SKIN_MESSAGE
	ON_WM_CREATE()
	ON_WM_HSCROLL()
	ON_WM_SHOWWINDOW()
	ON_BN_CLICKED(IDC_ATROOM_RADIO_SING, &CShowSoundSettingDlg::OnBnClickedCheck)
	ON_BN_CLICKED(IDC_ATROOM_RADIO_CHAT_WITH_PEOPLE, &CShowSoundSettingDlg::OnBnClickedCheck)
	ON_BN_CLICKED(IDC_ATROOM_RADIO_PLAY_SONG, &CShowSoundSettingDlg::OnBnClickedCheck)
END_MESSAGE_MAP()


// CShowHarassmentDlg 消息处理程序

int CShowSoundSettingDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  在此添加您专用的创建代码
	ASSERT(NULL != m_pSkinEngine);
	if (NULL != m_pSkinEngine)
	{
		HRESULT hr = m_pSkinEngine->LoadSkin(GetSafeHwnd(),L"ShowSoundSettingDlg_AtRoom");
		ASSERT(hr== S_OK && __LINE__);
	}

	return 0;
}

LRESULT CShowSoundSettingDlg::OnSkinMessage(WPARAM wParam,LPARAM lParam)
{
	switch(wParam)
	{
	case SKIN_ON_CLICK_AUDIO_TEST:
		{
			OnClickBtnAudioTest();
		}
		break;
	case SKIN_ON_CLICK_MORE_AUDIO_SETTING:
		{	
			OnClickBtnMoreAudioSetting();
		}
		break;
	case SKIN_ON_CLICK_GOTO_FRESH_ROOM:
		{
			OnClickBtnGotoFreshRoom();
		}
	default:break;
	}

	return TRUE;
}

void CShowSoundSettingDlg::OnOK()
{
	// TODO: 在此添加专用代码和/或调用基类

	//	CDialog::OnOK();
}

void CShowSoundSettingDlg::OnCancel()
{
	// TODO: 在此添加专用代码和/或调用基类

	//	CDialog::OnCancel();
}

BOOL CShowSoundSettingDlg::PreTranslateMessage(MSG* pMsg)
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

//
//void CShowSoundSettingDlg::SetToDefault()
//{
//	((CButton*)GetDlgItem(IDC_RADIO_SING))->SetCheck(FALSE);
//	((CButton*)GetDlgItem(IDC_RADIO_CHAT))->SetCheck(TRUE);
//	((CButton*)GetDlgItem(IDC_RADIO_PLAYMUSIC))->SetCheck(FALSE);
//}

void CShowSoundSettingDlg::Init()
{
	RoomAVLogic* pRoomAvLogic= VideoManager::GetInstance()->GetRoomAvLogic();
	if ( pRoomAvLogic == NULL )
	{
		return;
	}

	//EnMixerControlMode enmMode;		

	//if(pRoomAvLogic->GetMixerControlMode(enmMode))
	//{
	//	m_enmMode = enmMode;
	//}

	////IDC_ATROOM_RADIO_SING  唱歌单选框
	////IDC_ATROOM_RADIO_CHAT_WITH_PEOPLE  与人聊天单选框
	////IDC_ATROOM_RADIO_PLAY_SONG  在麦上放歌单选框

	//switch (enmMode)
	//{
	//case MIXER_CONTROL_SING:
	//	{		
	//		((CButton*)GetDlgItem(IDC_ATROOM_RADIO_SING))->SetCheck(TRUE);
	//		((CButton*)GetDlgItem(IDC_ATROOM_RADIO_CHAT_WITH_PEOPLE))->SetCheck(FALSE);
	//		((CButton*)GetDlgItem(IDC_ATROOM_RADIO_PLAY_SONG))->SetCheck(FALSE);
	//	}
	//	break;
	//case MIXER_CONTROL_CHAT:
	//	{
	//		((CButton*)GetDlgItem(IDC_ATROOM_RADIO_SING))->SetCheck(FALSE);
	//		((CButton*)GetDlgItem(IDC_ATROOM_RADIO_CHAT_WITH_PEOPLE))->SetCheck(TRUE);
	//		((CButton*)GetDlgItem(IDC_ATROOM_RADIO_PLAY_SONG))->SetCheck(FALSE);
	//	}
	//	break;
	//case MIXER_CONTROL_PLAY:
	//	{
	//		((CButton*)GetDlgItem(IDC_ATROOM_RADIO_SING))->SetCheck(FALSE);
	//		((CButton*)GetDlgItem(IDC_ATROOM_RADIO_CHAT_WITH_PEOPLE))->SetCheck(FALSE);
	//		((CButton*)GetDlgItem(IDC_ATROOM_RADIO_PLAY_SONG))->SetCheck(TRUE);
	//	}
	//	break;
	//default:
	//	{
	//		if (!((CButton*)GetDlgItem(IDC_ATROOM_RADIO_SING))->GetCheck() && !((CButton*)GetDlgItem(IDC_ATROOM_RADIO_CHAT_WITH_PEOPLE))->GetCheck() && !((CButton*)GetDlgItem(IDC_ATROOM_RADIO_PLAY_SONG))->GetCheck())
	//		{
	//			m_enmMode = MIXER_CONTROL_CHAT;
	//			((CButton*)GetDlgItem(IDC_ATROOM_RADIO_CHAT_WITH_PEOPLE))->SetCheck(TRUE);
	//			CPersonalConfig::GetInstance()->SetOnMicPurpose(ON_MIC_PURPOSE_SING);				
	//			pRoomAvLogic->SetMixerControlSingMode(true);
	//		}
	//		else
	//		{
	//			switch (m_enmMode)
	//			{
	//			case MIXER_CONTROL_SING:
	//				{
	//					((CButton*)GetDlgItem(IDC_ATROOM_RADIO_SING))->SetCheck(TRUE);
	//					((CButton*)GetDlgItem(IDC_ATROOM_RADIO_CHAT_WITH_PEOPLE))->SetCheck(FALSE);
	//					((CButton*)GetDlgItem(IDC_ATROOM_RADIO_PLAY_SONG))->SetCheck(FALSE);
	//				}
	//				break;
	//			case MIXER_CONTROL_CHAT:
	//				{
	//					((CButton*)GetDlgItem(IDC_ATROOM_RADIO_SING))->SetCheck(FALSE);
	//					((CButton*)GetDlgItem(IDC_ATROOM_RADIO_CHAT_WITH_PEOPLE))->SetCheck(TRUE);
	//					((CButton*)GetDlgItem(IDC_ATROOM_RADIO_PLAY_SONG))->SetCheck(FALSE);
	//				}
	//				break;
	//			case MIXER_CONTROL_PLAY:
	//				{
	//					((CButton*)GetDlgItem(IDC_ATROOM_RADIO_SING))->SetCheck(FALSE);
	//					((CButton*)GetDlgItem(IDC_ATROOM_RADIO_CHAT_WITH_PEOPLE))->SetCheck(FALSE);
	//					((CButton*)GetDlgItem(IDC_ATROOM_RADIO_PLAY_SONG))->SetCheck(TRUE);
	//				}
	//				break;
	//			}
	//		}
	//	}
	//	break;
	//}
	EnMixerControlMode enmMode = (EnMixerControlMode)CPersonalConfig::GetInstance()->GetOnMicPurpose();
	switch (enmMode)
	{
	case MIXER_CONTROL_SING:

		{
			((CButton*)GetDlgItem(IDC_ATROOM_RADIO_SING))->SetCheck(TRUE);
			((CButton*)GetDlgItem(IDC_ATROOM_RADIO_CHAT_WITH_PEOPLE))->SetCheck(FALSE);
			((CButton*)GetDlgItem(IDC_ATROOM_RADIO_PLAY_SONG))->SetCheck(FALSE);
		}
		break;
	case MIXER_CONTROL_CHAT:
		{
			((CButton*)GetDlgItem(IDC_ATROOM_RADIO_SING))->SetCheck(FALSE);
			((CButton*)GetDlgItem(IDC_ATROOM_RADIO_CHAT_WITH_PEOPLE))->SetCheck(TRUE);
			((CButton*)GetDlgItem(IDC_ATROOM_RADIO_PLAY_SONG))->SetCheck(FALSE);
		}
		break;
	case MIXER_CONTROL_PLAY:
		{
			((CButton*)GetDlgItem(IDC_ATROOM_RADIO_SING))->SetCheck(FALSE);
			((CButton*)GetDlgItem(IDC_ATROOM_RADIO_CHAT_WITH_PEOPLE))->SetCheck(FALSE);
			((CButton*)GetDlgItem(IDC_ATROOM_RADIO_PLAY_SONG))->SetCheck(TRUE);
		}
		break;
	default:
		{
			((CButton*)GetDlgItem(IDC_ATROOM_RADIO_SING))->SetCheck(TRUE);
			((CButton*)GetDlgItem(IDC_ATROOM_RADIO_CHAT_WITH_PEOPLE))->SetCheck(FALSE);
			((CButton*)GetDlgItem(IDC_ATROOM_RADIO_PLAY_SONG))->SetCheck(FALSE);
		}
		break;
	}

	unsigned int nPos = 0;		
	if (pRoomAvLogic->GetMixerControlTotalEnable())
	{
		if (pRoomAvLogic->GetMixerControlTotalVolume(nPos))
		{
			((CSliderCtrl*)GetDlgItem(IDC_ATROOM_SLIDER_TOTAL_VOLUMN))->SetPos(nPos);
		}
	}
	else
	{
		((CSliderCtrl*)GetDlgItem(IDC_ATROOM_SLIDER_TOTAL_VOLUMN))->SetPos(255);
		((CSliderCtrl*)GetDlgItem(IDC_ATROOM_SLIDER_TOTAL_VOLUMN))->EnableWindow(FALSE);
	}


	if (pRoomAvLogic->GetMixerControlWavoutEnable())
	{
		if (pRoomAvLogic->GetMixerControlWavoutVolume(nPos))
		{
			((CSliderCtrl*)GetDlgItem(IDC_ATROOM_SLIDER_SONG_ACCOMPANY))->SetPos(nPos);
		}
	}
	else
	{
		((CSliderCtrl*)GetDlgItem(IDC_ATROOM_SLIDER_SONG_ACCOMPANY))->SetPos(255);
		((CSliderCtrl*)GetDlgItem(IDC_ATROOM_SLIDER_SONG_ACCOMPANY))->EnableWindow(FALSE);
	}

	if (pRoomAvLogic->GetMixerControlOutmicEnable())
	{
		if (pRoomAvLogic->GetMixerControlOutmicVolume(nPos))
		{
			((CSliderCtrl*)GetDlgItem(IDC_ATROOM_SLIDER_VOLUMN_MIC))->SetPos(nPos);
		}
	}
	else
	{
		((CSliderCtrl*)GetDlgItem(IDC_ATROOM_SLIDER_VOLUMN_MIC))->SetPos(255);
		((CSliderCtrl*)GetDlgItem(IDC_ATROOM_SLIDER_VOLUMN_MIC))->EnableWindow(FALSE);
	}


	if (pRoomAvLogic->GetMixerControlMixerEnable())
	{
		if (pRoomAvLogic->GetMixerControlMixerVolume(nPos))
		{
			((CSliderCtrl*)GetDlgItem(IDC_ATROOM_SLIDER_MIX))->SetPos(nPos);
		}
	}
	else
	{
		((CSliderCtrl*)GetDlgItem(IDC_ATROOM_SLIDER_MIX))->SetPos(255);
		((CSliderCtrl*)GetDlgItem(IDC_ATROOM_SLIDER_MIX))->EnableWindow(FALSE);
	}

	if (pRoomAvLogic->GetMixerControlInmicEnable())
	{
		if (pRoomAvLogic->GetMixerControlInmicVolume(nPos))
		{
			((CSliderCtrl*)GetDlgItem(IDC_ATROOM_SLIDER_RECORD_MIC))->SetPos(nPos);
		}
	}
	else
	{
		((CSliderCtrl*)GetDlgItem(IDC_ATROOM_SLIDER_RECORD_MIC))->SetPos(255);
		((CSliderCtrl*)GetDlgItem(IDC_ATROOM_SLIDER_RECORD_MIC))->EnableWindow(FALSE);
	}


	/*if (CPersonalConfig::GetInstance()->GetIsStrengthenMic())  //加强复选框
	{
		((CButton*)GetDlgItem(IDC_CHECK_STRONGER))->SetCheck(TRUE);
	}
	else
	{
		((CButton*)GetDlgItem(IDC_CHECK_STRONGER))->SetCheck(FALSE);
	}*/


}


LRESULT CShowSoundSettingDlg::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	switch(message)
	{
	case WM_NOTIFY_ROOM_SYSSET_CLICKOK:  //消息“确定”按扭的消息
	case WM_NOTIFY_ROOM_SYSSET_CLICKAPPLI: //消息“应用”按扭的消息
		{
			Confirm();
		}
		break;
	
	default:
		break;
	}

	return CDialog::WindowProc(message, wParam, lParam);
}


BOOL CShowSoundSettingDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化	
	((CSliderCtrl*)GetDlgItem(IDC_ATROOM_SLIDER_TOTAL_VOLUMN))->SetRange(0, 255, TRUE); //总音量
	((CSliderCtrl*)GetDlgItem(IDC_ATROOM_SLIDER_TOTAL_VOLUMN))->SetPageSize(25);
	((CSliderCtrl*)GetDlgItem(IDC_ATROOM_SLIDER_SONG_ACCOMPANY))->SetRange(0, 255, TRUE); //伴奏
	((CSliderCtrl*)GetDlgItem(IDC_ATROOM_SLIDER_SONG_ACCOMPANY))->SetPageSize(25);
	((CSliderCtrl*)GetDlgItem(IDC_ATROOM_SLIDER_VOLUMN_MIC))->SetRange(0, 255, TRUE); //音量控制的麦克风
	((CSliderCtrl*)GetDlgItem(IDC_ATROOM_SLIDER_VOLUMN_MIC))->SetPageSize(25);
	((CSliderCtrl*)GetDlgItem(IDC_ATROOM_SLIDER_MIX))->SetRange(0, 255, TRUE); //立体声混音
	((CSliderCtrl*)GetDlgItem(IDC_ATROOM_SLIDER_MIX))->SetPageSize(25);
	((CSliderCtrl*)GetDlgItem(IDC_ATROOM_SLIDER_RECORD_MIC))->SetRange(0, 255, TRUE);  //录音控制的麦克风
	((CSliderCtrl*)GetDlgItem(IDC_ATROOM_SLIDER_RECORD_MIC))->SetPageSize(25);
	Init();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void CShowSoundSettingDlg::OnBnClickedCheck()
{
	::SendMessage(GetParent()->GetSafeHwnd(),WM_NOTIFY_ROOM_SYSSET_MODIFY,0,0); //给给父窗口(菜单设置)发送设置对话框有修改的消息
}

void CShowSoundSettingDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	//总音量
	RoomAVLogic* pRoomAvLogic= VideoManager::GetInstance()->GetRoomAvLogic();
	if (pRoomAvLogic != NULL)
	{
		if (GetDlgItem(IDC_ATROOM_SLIDER_TOTAL_VOLUMN) == pScrollBar)
		{
			int nPos = ((CSliderCtrl*)GetDlgItem(IDC_ATROOM_SLIDER_TOTAL_VOLUMN))->GetPos();			
			pRoomAvLogic->SetMixerControlTotalVolume(nPos);
			((CSliderCtrl*)GetDlgItem(IDC_ATROOM_SLIDER_TOTAL_VOLUMN))->Invalidate(FALSE);			

		}
		//伴奏
		if (GetDlgItem(IDC_ATROOM_SLIDER_SONG_ACCOMPANY) == pScrollBar)
		{
			int nPos = ((CSliderCtrl*)GetDlgItem(IDC_ATROOM_SLIDER_SONG_ACCOMPANY))->GetPos();			
			pRoomAvLogic->SetMixerControlWavoutVolume(nPos);
			((CSliderCtrl*)GetDlgItem(IDC_ATROOM_SLIDER_SONG_ACCOMPANY))->Invalidate(FALSE);

		}
		//音量控制的麦克风
		if (GetDlgItem(IDC_ATROOM_SLIDER_VOLUMN_MIC) == pScrollBar)
		{
			int nPos = ((CSliderCtrl*)GetDlgItem(IDC_ATROOM_SLIDER_VOLUMN_MIC))->GetPos();			
			pRoomAvLogic->SetMixerControlOutmicVolume(nPos);
			((CSliderCtrl*)GetDlgItem(IDC_ATROOM_SLIDER_VOLUMN_MIC))->Invalidate(FALSE);
		}
		//立体声混音
		if (GetDlgItem(IDC_ATROOM_SLIDER_MIX) == pScrollBar)
		{
			int nPos = ((CSliderCtrl*)GetDlgItem(IDC_ATROOM_SLIDER_MIX))->GetPos();			
			pRoomAvLogic->SetMixerControlMixerVolume(nPos);
			((CSliderCtrl*)GetDlgItem(IDC_ATROOM_SLIDER_MIX))->Invalidate(FALSE);
		}
		//录音控制的麦克风
		if (GetDlgItem(IDC_ATROOM_SLIDER_RECORD_MIC) == pScrollBar)
		{
			int nPos = ((CSliderCtrl*)GetDlgItem(IDC_ATROOM_SLIDER_RECORD_MIC))->GetPos();			
			pRoomAvLogic->SetMixerControlInmicVolume(nPos);
			((CSliderCtrl*)GetDlgItem(IDC_ATROOM_SLIDER_RECORD_MIC))->Invalidate(FALSE);
		}

		::SendMessage(GetParent()->GetSafeHwnd(),WM_NOTIFY_ROOM_SYSSET_MODIFY,0,0); //给给父窗口(菜单设置)发送设置对话框有修改的消息

	}	
	

	CDialog::OnHScroll(nSBCode, nPos, pScrollBar);
}

void CShowSoundSettingDlg::OnClickBtnAudioTest()
{
	if (m_pAudioTest == NULL)
	{
		SwitchResourceToModule(_T("Room"));
		m_pAudioTest = new CShowAudioTestDlg;
		m_pAudioTest->Create(CShowAudioTestDlg::IDD,NULL);
		int cx = GetSystemMetrics(SM_CXSCREEN);
		int cy = GetSystemMetrics(SM_CYSCREEN);
		int clientCx = 446;
		int clientCy = 438;
		m_pAudioTest->SetWindowPos(NULL,cx/2 - clientCx/2,cy/2 - clientCy/2,clientCx,clientCy,SWP_SHOWWINDOW);
	}else
	{
		if(m_pAudioTest->GetSafeHwnd() && (!m_pAudioTest->IsWindowVisible()))
			m_pAudioTest->ShowWindow(SW_SHOW);
	}
}

void CShowSoundSettingDlg::OnClickBtnMoreAudioSetting()
{
	common::utility::systemhelper::OS_VERSION osVer = common::utility::systemhelper::GetOsVersion();
	if (osVer == common::utility::systemhelper::OS_WIN7 || osVer == common::utility::systemhelper::OS_VISTA)
	{
		ShellExecute(NULL, _T("open"), L"SndVol.exe", NULL, NULL, SW_SHOWNORMAL);
	}
	else
	{
		ShellExecute(NULL, _T("open"), L"sndvol32.exe", NULL, NULL, SW_SHOWNORMAL);
	}

}

void CShowSoundSettingDlg::OnClickBtnGotoFreshRoom()
{
	CString strIniPath =  CString(common::utility::systemhelper::Get179AppPath().c_str()) + SYSTEM_SET_INI_PATH;

	unsigned int unRoomID = GetPrivateProfileInt(SYSSET_INI_APPNAME_BEGINNERHELP,SYSSET_INI_KEYNAME_ROOMID, 0 ,strIniPath);

	if (unRoomID != 0)
	{
		CSendEventManager::SendEnterRoomRequest(unRoomID);		
	}

}

void CShowSoundSettingDlg::Confirm()
{
	RoomAVLogic* pRoomAvLogic= VideoManager::GetInstance()->GetRoomAvLogic();
	if ( pRoomAvLogic == NULL )
	{
		return;
	}


	if (((CButton*)GetDlgItem(IDC_ATROOM_RADIO_SING))->GetCheck() == TRUE) //选中唱歌
	{
		CPersonalConfig::GetInstance()->SetOnMicPurpose(MIXER_CONTROL_SING);		
		pRoomAvLogic->SetMixerControlSingMode(true);
		
		m_enmMode = MIXER_CONTROL_SING;
	}
	else if (((CButton*)GetDlgItem(IDC_ATROOM_RADIO_CHAT_WITH_PEOPLE))->GetCheck() == TRUE)  ///选中与人聊天
	{
		CPersonalConfig::GetInstance()->SetOnMicPurpose(MIXER_CONTROL_CHAT);
		pRoomAvLogic->SetMixerControlChatMode(true);		
		m_enmMode = MIXER_CONTROL_CHAT;
	}
	else if (((CButton*)GetDlgItem(IDC_ATROOM_RADIO_PLAY_SONG))->GetCheck() == TRUE) ///选中在麦上放歌
	{
		CPersonalConfig::GetInstance()->SetOnMicPurpose(MIXER_CONTROL_PLAY);
		pRoomAvLogic->SetMixerControlPlayMode(true);		
		m_enmMode = MIXER_CONTROL_PLAY;
	}

	int iPos = ((CSliderCtrl*)GetDlgItem(IDC_ATROOM_SLIDER_TOTAL_VOLUMN))->GetPos();  //总音量
	CPersonalConfig::GetInstance()->SetGeneralVolume(iPos);

	iPos = ((CSliderCtrl*)GetDlgItem(IDC_ATROOM_SLIDER_SONG_ACCOMPANY))->GetPos(); //伴奏
	CPersonalConfig::GetInstance()->SetAccompanyVolume(iPos);

	iPos = ((CSliderCtrl*)GetDlgItem(IDC_ATROOM_SLIDER_VOLUMN_MIC))->GetPos(); //音量控制的麦克风
	CPersonalConfig::GetInstance()->SetMicVolume(iPos);

	iPos = ((CSliderCtrl*)GetDlgItem(IDC_ATROOM_SLIDER_MIX))->GetPos(); //立体声混音
	CPersonalConfig::GetInstance()->SetRecordMixVolume(iPos);

	iPos = ((CSliderCtrl*)GetDlgItem(IDC_ATROOM_SLIDER_RECORD_MIC))->GetPos(); //录音控制的麦克风
	CPersonalConfig::GetInstance()->SetRecordMicVolume(iPos);

	//g_RoomModule->UpdateAllRoomAudioMode();
}

void CShowSoundSettingDlg::OnShowWindow(BOOL bShow,UINT nStatus)
{
	CDialog::OnShowWindow(bShow, nStatus);

	if(bShow)  //显示音频设置
	{	
		Init();
	}
}

void CShowSoundSettingDlg::SetApplyEffect(BOOL bEffect)
{
	CComVariant var = bEffect;
	m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(), L"imgApplyEffect", L"visible", var);
}