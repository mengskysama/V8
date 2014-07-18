// SysSettingDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "..\Room.h"
#include "SysSettingDlg.h"
#include "ShowPersonalDlg.h"
#include "ShowAudioSettingDlg.h"
#include "ShowBasicSettingDlg.h"
#include "ShowMyAccountDlg.h"
#include "ShowSoundSettingDlg.h"
#include "ShowMessageSettingDlg.h"
#include "ShowMyRoomDlg.h"
#include "ShowPwdSafeDlg.h"
#include "ShowFansDlg.h"
#include "ShowBankAccountDlg.h"
#include "skin_i.h"
#include "SkinUtil.h"
#include "HallDefine.h"
#include "..\personalsetting/PersonalConfig.h"
#include "utility/SystemHelper.h"
#include "ui\C179MsgBox.h"


#define SKIN_ON_CLICK_CLOSE  1
#define SKIN_ON_CLICK_OK  2

#define SKIN_ON_CLICK_TAB_PERSONAL_SETTING  3
#define SKIN_ON_CLICK_TAB_SYSTEM_SETTING  4

#define SKIN_NOTIFY_PERSONAL_DLG_HWND 7
#define SKIN_NOTIFY_BASICSETTING_DLG_HWND 9
#define SKIN_NOTIFY_AUDIOSETTING_DLG_HWND 10
#define SKIN_NOTIFY_PWDSAFE_DLG_HWND 12
#define SKIN_NOTIFY_MYROOM_DLG_HWND 13
#define SKIN_ON_CLICK_BTN_CHANGE_HEAD 14
#define SKIN_ON_CLICK_APPLY 15
#define SKIN_NOTIFY_MYACCOUNT_DLG_HWND  16
#define SKIN_NOTIFY_SOUND_SETTIND_DLG_HWND 17
#define SKIN_NOTIFY_MESSAGE_SETTING_DLG_HWND 18
#define SKIN_ON_CLICK_MINIMIZE             19
#define SKIN_NOTIFY_SHOW_PERSONAL_ONLY       20
#define SKIN_NOTIFY_SHOW_SYS_ONLY           21
#define SKIN_NOTIFY_FANS_DLG_HWND        22
#define SKIN_NOTIFY_FANS_DLG_SHOW        23
#define SKIN_NOTIFY_BANKACCOUNT_DLG_HWND        24
#define SKIN_NOTIFY_BANKACCOUNT_DLG_SHOW        25

// CSysSettingDlg 对话框

#define  SUB_DLG_WIDTH 595
#define  SUB_DLG_HEIGHT 378
#define  SUB_DLG_X_POS  157
#define  SUB_DLG_Y_POS  67

#define  DLG_WIDTH 579
#define  DLG_HEIGHT 442

#define SYSSETTING_MODIFY_TIP _T("您已经对设置做了修改，")
#define SYSSETTING_MODIFY_EX_TIP _T("是否保存？                        ")

#define SYSSETTING_EFFECT_TIMER_IDEVENT 1
#define SYSSETTING_EFFECT_TIMER_INTERVALTIME 500


IMPLEMENT_DYNAMIC(CSysSettingDlg, CDialog)

CSysSettingDlg::CSysSettingDlg(CWnd* pParent /*=NULL*/)
: CDialog(CSysSettingDlg::IDD, pParent)
{
	curShowDlg = NULL;
	m_pSkinEngine = NULL;
	::GetSkinEngine(&m_pSkinEngine);


	m_strCurItemBk = L"imgBasicInfoBk";
	m_nCurPersonalItem = -1;
	m_nCurSysItem = -1;
	m_isClose = false;

}

CSysSettingDlg::~CSysSettingDlg()
{
	for (MAPOFDLG::iterator it = m_mShowDlg.begin(); it != m_mShowDlg.end(); it++)
	{
		CDialog *pDlg = it->second;
		if(pDlg != NULL)
		{
			pDlg->DestroyWindow();
			delete pDlg;
			pDlg = NULL;
		}
		//m_mShowDlg.erase(it);
	}
	m_mShowDlg.clear();
}

void CSysSettingDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CSysSettingDlg, CDialog)
	ON_WM_CREATE()
	ON_SKIN_MESSAGE
	ON_WM_SHOWWINDOW()
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CSysSettingDlg 消息处理程序

int CSysSettingDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;

	curShowDlg = CreateShowPersonalDlg();

	// TODO:  在此添加您专用的创建代码
	ASSERT(NULL != m_pSkinEngine);
	if (NULL != m_pSkinEngine)
	{
		HRESULT hr = m_pSkinEngine->LoadSkin(GetSafeHwnd(),L"SysSettingDlg_AtRoom");
		ASSERT(hr== S_OK && __LINE__);
	}

	//标题字体首选微软雅黑
	/*std::wstring strFont;
	common::utility::systemhelper::GetDefaultFontName(strFont);
	CComVariant var = (CComVariant) strFont.c_str();
	m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(),L"textTitle",L"fontFamily",var);*/

	ModifyStyleEx(0,WS_EX_TOOLWINDOW);

	return 0;
}

void CSysSettingDlg::OnShowWindow(BOOL bShow,UINT nStatus)
{
	CDialog::OnShowWindow(bShow, nStatus);

	if(bShow)
	{
		CenterWindow(GetParent());

		BtnApplyEnable(false);  //应用按扭不可用

		core::CurInfo myCurInfo = GetCurInfo() ;		
		Hall::HeadImage headImage ;
		headImage.m_bMan = myCurInfo.bMan ;
		headImage.m_unUIN = myCurInfo.unUIN ;
		if(myCurInfo.enmAccountType == core::ACCOUNTTYPE_VISITOR)
		{
			headImage.m_isGuest = true;
		}
		else
		{
			headImage.m_isGuest = false;
		}
		
		g_RoomModule->m_pModuleManager->CallService(Hall::SERVICE_VALUE_HALL_UPDATE_HEAD_IMAGE_PATH, (param)&headImage);
		CComVariant var = CString(headImage.m_HeadImagePath.c_str()) ;
		m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(), L"UserHead", L"absoluteImage", var );	
	
		if((myCurInfo.unBusiness & core::BUSINESSTYPE_SONGER) || (myCurInfo.unBusiness & core::BUSINESSTYPE_DANCER) || (myCurInfo.unBusiness & core::BUSINESSTYPE_HOST) )  
		{
			//该用户为艺人，显示粉丝排行榜			
			 NOTIFY_SKIN_HELPER(SKIN_NOTIFY_FANS_DLG_SHOW,(LONG)1);//传1进去  显示 我的粉丝

		}
		else
		{
			//该用户不为艺人，不显示粉丝排行榜			
			 NOTIFY_SKIN_HELPER(SKIN_NOTIFY_FANS_DLG_SHOW,(LONG)0);//传0进去 不显示 我的粉丝

		}

		if(myCurInfo.unBusiness & core::BUSINESSTYPE_AGENT )  
		{
			//该用户为销售，显示银行账号			
			NOTIFY_SKIN_HELPER(SKIN_NOTIFY_BANKACCOUNT_DLG_SHOW,(LONG)1);//传1进去  显示 银行账号	

		}
		else
		{
			//该用户不为销售，不显示银行账号		
			NOTIFY_SKIN_HELPER(SKIN_NOTIFY_BANKACCOUNT_DLG_SHOW,(LONG)0);//传0进去 不显示 银行账号	

		}
		
	}

}


LRESULT CSysSettingDlg::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	switch(message)
	{	
	case WM_NOTIFY_ROOM_SYSSET_MODIFY:  //设置对话框有修改时的消息
		{
			BtnApplyEnable(true);
		}
		break;

	case WM_NOTIFY_ROOM_SYSSET_MODIFY_OK:
		{
			if(m_isClose)
			{
				ShowWindow(SW_HIDE);
			}			
			
		}
		break;
	default:
		break;
	}

	return CDialog::WindowProc(message, wParam, lParam);
}


void CSysSettingDlg::OnGetHeadImage(uint32 unUIN)
{
	core::CurInfo myCurInfo = GetCurInfo() ;
	if(unUIN == myCurInfo.unUIN)
	{
		Hall::HeadImage headImage ;
		headImage.m_bMan = myCurInfo.bMan ;
		headImage.m_unUIN = myCurInfo.unUIN ;
		if(myCurInfo.enmAccountType == core::ACCOUNTTYPE_VISITOR)
		{
			headImage.m_isGuest = true;
		}
		else
		{
			headImage.m_isGuest = false;
		}
		g_RoomModule->m_pModuleManager->CallService(Hall::SERVICE_VALUE_HALL_GET_HEAD_IMAGE_PATH, (param)&headImage);
		CComVariant var = (CString)headImage.m_HeadImagePath.c_str() ;
		m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(), L"UserHead", L"absoluteImage", var );
	}

}



LRESULT CSysSettingDlg::OnSkinMessage(WPARAM wParam, LPARAM lParam)
{
	switch(wParam)
	{
	case SKIN_ON_CLICK_CLOSE://点取消也响应到这里
		{
			OnClickClocse();

		}
		break;
	case SKIN_ON_CLICK_OK:  //点击了"菜单设置"对话框的中"确定"按扭
		{          
			OnClickBtnOK();			
		}
		break;
	case SKIN_ON_CLICK_TAB_PERSONAL_SETTING: //点击了"菜单设置"对话框中的"个人设置"下的选项
		{			  
			if (m_nCurPersonalItem != (int)lParam )
			{
				OnClickTabPersonalSetting(lParam);
				m_nCurPersonalItem = (int)lParam;
				m_nCurSysItem = -1;
			}

		}
		break;
	case SKIN_ON_CLICK_TAB_SYSTEM_SETTING:  //点击了"菜单设置"对话框中的"系统设置" 下的选项
		{		
			if (m_nCurSysItem != (int)lParam)
			{
				OnClickTabSystemSetting(lParam);
				m_nCurSysItem = (int)lParam;
				m_nCurPersonalItem = -1;
			}

		}
		break;
	case SKIN_ON_CLICK_BTN_CHANGE_HEAD:
		{
			OnClickBtnChangeHead();
		}
		break;
	case SKIN_ON_CLICK_APPLY:
		{
			OnClickBtnApply();
		}
		break;
	case SKIN_ON_CLICK_MINIMIZE:
		{
			ShowWindow(SW_MINIMIZE);
		}
		break;	

	default:break;
	}

	return TRUE;
}

void CSysSettingDlg::OpenDlg(SysSettingDlgType type)
{	
	CDialog * pShowDlg = NULL;
	CComVariant var;
	switch(type)
	{
	case SysSettingDlgType::Show_User_Info_Dlg:
		{
			if (m_strCurItemBk != L"")
			{
				var = FALSE;
				m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(), m_strCurItemBk.GetBuffer(), L"visible", var);
				var = TRUE;
				m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(), L"imgBasicInfoBk", L"visible", var);
				m_strCurItemBk = L"imgBasicInfoBk";
			}

			pShowDlg = CreateShowPersonalDlg();
			curShowDlg->ShowWindow(SW_HIDE);
			pShowDlg->ShowWindow(SW_SHOW);
		//	pShowDlg->ActivateTopParent();
			curShowDlg = pShowDlg;
			m_nCurPersonalItem = 0;
			m_nCurSysItem = -1;
		}
		break;

	case SysSettingDlgType::Show_Basic_Setting_Dlg:
		{			
			if (m_strCurItemBk != L"")
			{
				var = FALSE;
				m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(), m_strCurItemBk.GetBuffer(), L"visible", var);
				var = TRUE;
				m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(), L"imgBasicSetBk", L"visible", var);
				m_strCurItemBk = L"imgBasicSetBk";
			}
			pShowDlg = CreateShowBasicSettingDlg();
			curShowDlg->ShowWindow(SW_HIDE);
			pShowDlg->ShowWindow(SW_SHOW);
			curShowDlg = pShowDlg;
			m_nCurSysItem = 0;
			m_nCurPersonalItem = -1;
		}
		break;
	case SysSettingDlgType::Show_Audio_Setting_Dlg:
		{		
			if (m_strCurItemBk != L"")
			{
				var = FALSE;
				m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(), m_strCurItemBk.GetBuffer(), L"visible", var);
				var = TRUE;
				m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(), L"imgVideoSetBk", L"visible", var);
				m_strCurItemBk = L"imgVideoSetBk";
			}
			pShowDlg = CreateShowAudioSettingDlg();
			curShowDlg->ShowWindow(SW_HIDE);
			pShowDlg->ShowWindow(SW_SHOW);
			curShowDlg = pShowDlg;
			m_nCurSysItem = 1;
			m_nCurPersonalItem = -1;
		}
		break;
#if 0
		////////////////////////安全设置
	case SysSettingDlgType::Show_Harassment_Setting_Dlg:
		{
			var = 0;
			m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(), L"tabSafeSetting", L"tabItemSelected", var);
			var = -1;
			m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(), L"tabPersonalSetting", L"tabItemSelected", var);
			m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(), L"tabSystemSetting", L"tabItemSelected", var);
			m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(), L"tabMyRoomSetting", L"tabItemSelected", var);

			pShowDlg = CreateShowHarassmentDlg();
			curShowDlg->ShowWindow(SW_HIDE);
			pShowDlg->ShowWindow(SW_SHOW);
			curShowDlg = pShowDlg;
		}
		break;
#endif

	case SysSettingDlgType::Show_PwdSafe_Setting_Dlg:
		{	
			if (m_strCurItemBk != L"")
			{
				var = FALSE;
				m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(), m_strCurItemBk.GetBuffer(), L"visible", var);
				var = TRUE;
				m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(), L"imgPwdSafeBk", L"visible", var);
				m_strCurItemBk = L"imgPwdSafeBk";
			}
			pShowDlg = CreateShowPwdSafeDlg();
			curShowDlg->ShowWindow(SW_HIDE);
			pShowDlg->ShowWindow(SW_SHOW);
			curShowDlg = pShowDlg;
			m_nCurPersonalItem = 3;
			m_nCurSysItem = -1;
		}
		break;	
	case SysSettingDlgType::Show_MgrRoom_Setting_Dlg:
		{	
			if (m_strCurItemBk != L"")
			{
				var = FALSE;
				m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(), m_strCurItemBk.GetBuffer(), L"visible", var);
				var = TRUE;
				m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(), L"imgMyRoomBk", L"visible", var);
				m_strCurItemBk = L"imgMyRoomBk";
			}
			pShowDlg = CreateShowMyRoomDlg();
			curShowDlg->ShowWindow(SW_HIDE);
			pShowDlg->ShowWindow(SW_SHOW);
			curShowDlg = pShowDlg;
			m_nCurPersonalItem = 1;
			m_nCurSysItem = -1;
		}
		break;
	case SysSettingDlgType::Show_BankAccount_Dlg:
		{	
			if (m_strCurItemBk != L"")
			{
				var = FALSE;
				m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(), m_strCurItemBk.GetBuffer(), L"visible", var);
				var = TRUE;
				m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(), L"imgBankAccountBk", L"visible", var);
				m_strCurItemBk = L"imgBankAccountBk";
			}
			pShowDlg = CreateShowBankAccountDlg();
			curShowDlg->ShowWindow(SW_HIDE);
			pShowDlg->ShowWindow(SW_SHOW);
			curShowDlg = pShowDlg;
			m_nCurPersonalItem = 1;
			m_nCurSysItem = -1;
		}
		break;
	case SysSettingDlgType::Show_My_Account_Dlg:
		{
			if (m_strCurItemBk != L"")
			{
				var = FALSE;
				m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(), m_strCurItemBk.GetBuffer(), L"visible", var);
				var = TRUE;
				m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(), L"imgMyAccountBk", L"visible", var);
				m_strCurItemBk = L"imgMyAccountBk";
			}
			pShowDlg = CreateShowMyAccountDlg();
			curShowDlg->ShowWindow(SW_HIDE);
			pShowDlg->ShowWindow(SW_SHOW);
			curShowDlg = pShowDlg;
			m_nCurPersonalItem = 2;
			m_nCurSysItem = -1;
		}
		break;
	case SysSettingDlgType::Show_Fans_Dlg:
		{
			if (m_strCurItemBk != L"")
			{
				var = FALSE;
				m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(), m_strCurItemBk.GetBuffer(), L"visible", var);
				var = TRUE;
				m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(), L"imgFansBk", L"visible", var);
				m_strCurItemBk = L"imgFansBk";
			}
			pShowDlg = CreateShowFansDlg();
			curShowDlg->ShowWindow(SW_HIDE);
			pShowDlg->ShowWindow(SW_SHOW);
			curShowDlg = pShowDlg;
			m_nCurPersonalItem = 2;
			m_nCurSysItem = -1;
		}
		break;
	case SysSettingDlgType::Show_Sound_Setting_Dlg:
		{
			if (m_strCurItemBk != L"")
			{
				var = FALSE;
				m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(), m_strCurItemBk.GetBuffer(), L"visible", var);
				var = TRUE;
				m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(), L"imgAudioSetBk", L"visible", var);
				m_strCurItemBk = L"imgAudioSetBk";
			}
			pShowDlg = CreateShowSoundSettingDlg();
			curShowDlg->ShowWindow(SW_HIDE);
			pShowDlg->ShowWindow(SW_SHOW);
			curShowDlg = pShowDlg;
			m_nCurSysItem = 2;
			m_nCurPersonalItem = -1;
		}
		break;
	case SysSettingDlgType::Show_Message_Setting_Dlg:
		{	
			if (m_strCurItemBk != L"")
			{
				var = FALSE;
				m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(), m_strCurItemBk.GetBuffer(), L"visible", var);
				var = TRUE;
				m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(), L"imgMsgSetBk", L"visible", var);
				m_strCurItemBk = L"imgMsgSetBk";
			}
			pShowDlg = CreateShowMessageSettingDlg();
			curShowDlg->ShowWindow(SW_HIDE);
			pShowDlg->ShowWindow(SW_SHOW);
			curShowDlg = pShowDlg;
			m_nCurSysItem = 3;
			m_nCurPersonalItem = -1;
		}
		break;

	default:break;
	}

}

void CSysSettingDlg::OnClickTabPersonalSetting(int iTab)
{
	if (0 == iTab)
		OpenDlg(Show_User_Info_Dlg);
	if (1 == iTab)
		OpenDlg(Show_MgrRoom_Setting_Dlg);
	if (2 == iTab)
		OpenDlg(Show_My_Account_Dlg);
	if (3 == iTab)
		OpenDlg(Show_PwdSafe_Setting_Dlg);
	if (4 == iTab)
		OpenDlg(Show_Fans_Dlg);
	if (5 == iTab)
		OpenDlg(Show_BankAccount_Dlg);
}

void CSysSettingDlg::OnClickTabSystemSetting(int iTab)
{
	if (0 == iTab)
		OpenDlg(Show_Basic_Setting_Dlg);
	if (1 == iTab)
		OpenDlg(Show_Audio_Setting_Dlg);
	if (2 == iTab)
		OpenDlg(Show_Sound_Setting_Dlg);
	if (3 == iTab)
		OpenDlg(Show_Message_Setting_Dlg);
}

void CSysSettingDlg::OnClickBtnChangeHead()
{
	Hall::HeadSetting * pEvent = new Hall::HeadSetting();
	pEvent->m_parentHwnd = GetParent()->m_hWnd;

	IModuleManager* pModuleManager = g_RoomModule->m_pModuleManager;
	if(pModuleManager != NULL)
	{
		pModuleManager->PushEvent(MakeEvent<MODULE_ID_ROOM>()(Hall::EVENT_VALUE_HALL_HEAD_UPLOAD, MODULE_ID_HALL,0, 0, 0, pEvent));  //头像上传
	}

}

void CSysSettingDlg::OnTimer(UINT_PTR nIDEvent)
{	
	if(nIDEvent == SYSSETTING_EFFECT_TIMER_IDEVENT)
	{
		SetApplyEffect(FALSE);  
		KillTimer(SYSSETTING_EFFECT_TIMER_IDEVENT);
	}	

}

void CSysSettingDlg::OnClickBtnApply()
{
	m_isClose = false;
	SetApplyEffect(TRUE);
	SetTimer(SYSSETTING_EFFECT_TIMER_IDEVENT,SYSSETTING_EFFECT_TIMER_INTERVALTIME,NULL);
	CDialog *pwdSafeDlg;
	if(PwdSafeDlgIsShow(pwdSafeDlg)) //当前显示对话框是否为密码安全
	{
		curShowDlg->SendMessage(WM_NOTIFY_ROOM_SYSSET_CLICKAPPLI,NULL,NULL);  //给密码安全对话框发送"应用"按扭消息
	}
	else
	{
		MAPOFDLG::iterator it;
		for(it = m_mShowDlg.begin(); it != m_mShowDlg.end(); it++)
		{
			CDialog* pDlg = it->second;
			if(pDlg != pwdSafeDlg)
			{
				pDlg->SendMessage(WM_NOTIFY_ROOM_SYSSET_CLICKAPPLI,NULL,NULL);  //给除了密码安全对话框之外的所有设置对话框发送"应用"按扭消息
			}

		}		

	}	

	BtnApplyEnable(false);
	CPersonalConfig::GetInstance()->Save();
	g_RoomModule->SetAllRoomVideoCfg( CPersonalConfig::GetInstance()->GetUseVideoDeviceIndex() ) ;

	

}

void CSysSettingDlg::SetApplyEffect(BOOL bEffect)
{
	if (curShowDlg != NULL)
	{
		switch(m_nCurPersonalItem)
		{
		case 0://基本资料
			{
				CShowPersonalDlg* pDlg = dynamic_cast<CShowPersonalDlg*> (curShowDlg);
				if (pDlg != NULL)
				{
					pDlg->SetApplyEffect(bEffect);
				}
			}
			break;
		case 1://我的房间
			{
				CShowMyRoomDlg* pDlg = dynamic_cast<CShowMyRoomDlg*> (curShowDlg);
				if (pDlg != NULL)
				{
					pDlg->SetApplyEffect(bEffect);
				}
			}
			break;
		case 2://我的帐户
			{
				CShowMyAccountDlg* pDlg = dynamic_cast<CShowMyAccountDlg*> (curShowDlg);
				if (pDlg != NULL)
				{
					pDlg->SetApplyEffect(bEffect);
				}
			}
			break;
		case 3://密码安全
			{
				CShowPwdSafeDlg* pDlg = dynamic_cast<CShowPwdSafeDlg*> (curShowDlg);
				if (pDlg != NULL)
				{
					pDlg->SetApplyEffect(bEffect);
				}
			}
			break;
		case 4://粉丝排行
			{
				CShowFansDlg* pDlg = dynamic_cast<CShowFansDlg*> (curShowDlg);
				if (pDlg != NULL)
				{
					pDlg->SetApplyEffect(bEffect);
				}
			}
			break;
		case 5://银行账号
			{
				CShowBankAccountDlg* pDlg = dynamic_cast<CShowBankAccountDlg*> (curShowDlg);
				if (pDlg != NULL)
				{
					pDlg->SetApplyEffect(bEffect);
				}
			}
			break;
		default:break;
		}

		switch(m_nCurSysItem)
		{
		case 0://常规设置
			{
				CShowBasicSettingDlg* pDlg = dynamic_cast<CShowBasicSettingDlg*> (curShowDlg);
				if (pDlg != NULL)
				{
					pDlg->SetApplyEffect(bEffect);
				}
			}
			break;
		case 1://视频设置
			{
				CShowAudioSettingDlg* pDlg = dynamic_cast<CShowAudioSettingDlg*> (curShowDlg);
				if (pDlg != NULL)
				{
					pDlg->SetApplyEffect(bEffect);
				}
			}
			break;
		case 2://音频设置
			{
				CShowSoundSettingDlg* pDlg = dynamic_cast<CShowSoundSettingDlg*> (curShowDlg);
				if (pDlg != NULL)
				{
					pDlg->SetApplyEffect(bEffect);
				}
			}
			break;
		case 3://消息设置
			{
				CShowMessageSettingDlg* pDlg = dynamic_cast<CShowMessageSettingDlg*> (curShowDlg);
				if (pDlg != NULL)
				{
					pDlg->SetApplyEffect(bEffect);
				}
			}
			break;
		default:break;
		}

	}
}

//点击取消
void CSysSettingDlg::OnClickClocse()
{
	CComVariant var;
	m_pSkinEngine->GetSubSkinProp(GetSafeHwnd(), L"op_apply", L"disabled", &var);
	if (!var.boolVal)  //应用按扭可用
	{
		int ret = C179MsgBox::Show( this, NewMsgBox::MSGBOX_ICO_OK_CANCEL_TWO_STRING ,NewMsgBox::ICONTYPE_WARNING , SYSSETTING_MODIFY_TIP, SYSSETTING_MODIFY_EX_TIP);
		if (ret == RET_OK)
		{
			OnClickBtnOK();
		}
		else
		{
			//PostMessage(WM_CLOSE,NULL,NULL);
			ShowWindow(SW_HIDE);
		}

	}
	else
	{
		//PostMessage(WM_CLOSE,NULL,NULL);
		ShowWindow(SW_HIDE);
	}

}

//点击确定
void CSysSettingDlg::OnClickBtnOK()
{
	m_isClose = true;
	CComVariant var;
	m_pSkinEngine->GetSubSkinProp(GetSafeHwnd(), L"op_apply", L"disabled", &var);
	if (!var.boolVal)  //应用按扭可用
	{
		SetApplyEffect(TRUE);
		SetTimer(SYSSETTING_EFFECT_TIMER_IDEVENT,SYSSETTING_EFFECT_TIMER_INTERVALTIME,NULL);

		CDialog *pwdSafeDlg;
		if(PwdSafeDlgIsShow(pwdSafeDlg)) //当前显示对话框是否为密码安全
		{
			curShowDlg->SendMessage(WM_NOTIFY_ROOM_SYSSET_CLICKOK,NULL,NULL);  //给密码安全对话框发送"确定"按扭消息
		}
		else
		{			

			MAPOFDLG::iterator it;
			for(it = m_mShowDlg.begin(); it != m_mShowDlg.end(); it++)
			{
				CDialog* pDlg = it->second;
				if(pDlg != pwdSafeDlg)
				{
					pDlg->SendMessage(WM_NOTIFY_ROOM_SYSSET_CLICKOK,NULL,NULL);  //给除了密码安全对话框之外的所有设置对话框发送"确定"按扭消息
				}

			}
			SetApplyEffect(true);			
			//PostMessage(WM_CLOSE,NULL,NULL);
			ShowWindow(SW_HIDE);

		}

		BtnApplyEnable(false);

		CPersonalConfig::GetInstance()->Save();
		g_RoomModule->SetAllRoomVideoCfg( CPersonalConfig::GetInstance()->GetUseVideoDeviceIndex() ) ;
	}
	else
	{
		//PostMessage(WM_CLOSE, NULL, NULL);
		ShowWindow(SW_HIDE);
	}

	
}

//当前显示对话框是否为密码安全
bool CSysSettingDlg::PwdSafeDlgIsShow(CDialog *&pwdSafeDlg)
{
	bool IsPwd = false;

	pwdSafeDlg = NULL;
	MAPOFDLG::iterator it;
	it = m_mShowDlg.find(Show_PwdSafe_Setting_Dlg);
	if(it != m_mShowDlg.end())  //找到密码安全对话框
	{

		CDialog* ShowPwdSafeDlg =  it->second;
		if(curShowDlg == ShowPwdSafeDlg)  //若当前显示为密码安全对话框
		{
			IsPwd = true;

		}
		pwdSafeDlg = ShowPwdSafeDlg;

	}

	return IsPwd;
}

//应用按扭是否可用 ( 设置对话框有修改时按扭可用)
void CSysSettingDlg::BtnApplyEnable(bool isModify)
{
	CComVariant var = isModify?L"FALSE":L"TRUE";
	m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(),L"op_apply",L"disabled",var);  //“应用”按扭是否可用

}
void CSysSettingDlg::SetToPersonalSetting()
{
	NOTIFY_SKIN_HELPER(SKIN_NOTIFY_SHOW_PERSONAL_ONLY,(LONG)0);
	OpenDlg(Show_User_Info_Dlg);
}

void CSysSettingDlg::SetToSysSetting()
{
	m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(), L"headBorder", L"visible", (CComVariant)FALSE);
	m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(), L"UserHead", L"visible", (CComVariant)FALSE);
	m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(), L"btnChangeHead", L"visible", (CComVariant)FALSE);
	NOTIFY_SKIN_HELPER(SKIN_NOTIFY_SHOW_SYS_ONLY,(LONG)0);
	OpenDlg(Show_Basic_Setting_Dlg);
}


void CSysSettingDlg::NotifyCreateSkinItem()
{	
	CDialog * pShowDlg = NULL;
	//////////////////	个人资料
	pShowDlg = CreateShowPersonalDlg();

	if (NULL != pShowDlg->GetSafeHwnd())
	{
		NOTIFY_SKIN_HELPER(SKIN_NOTIFY_PERSONAL_DLG_HWND, pShowDlg->GetSafeHwnd());
	}
	/////////////////////基本设置

	pShowDlg = CreateShowBasicSettingDlg();
	if (NULL != pShowDlg->GetSafeHwnd())
	{
		NOTIFY_SKIN_HELPER(SKIN_NOTIFY_BASICSETTING_DLG_HWND, pShowDlg->GetSafeHwnd());
	}
	/////////////////////////视频设置	

	pShowDlg = CreateShowAudioSettingDlg();
	if (NULL != pShowDlg->GetSafeHwnd())
	{
		NOTIFY_SKIN_HELPER(SKIN_NOTIFY_AUDIOSETTING_DLG_HWND, pShowDlg->GetSafeHwnd());
	}

	////////////////////////////密码安全
	pShowDlg = CreateShowPwdSafeDlg();
	if (NULL != pShowDlg->GetSafeHwnd())
	{
		NOTIFY_SKIN_HELPER(SKIN_NOTIFY_PWDSAFE_DLG_HWND, pShowDlg->GetSafeHwnd());
	}
	//////////////////////////////	我的房间

	pShowDlg = CreateShowMyRoomDlg();
	if (NULL != pShowDlg->GetSafeHwnd())
	{
		NOTIFY_SKIN_HELPER(SKIN_NOTIFY_MYROOM_DLG_HWND, pShowDlg->GetSafeHwnd());
	}

	//////////////////////////////	银行账号
	pShowDlg = CreateShowBankAccountDlg();
	if (NULL != pShowDlg->GetSafeHwnd())
	{
		NOTIFY_SKIN_HELPER(SKIN_NOTIFY_BANKACCOUNT_DLG_HWND, pShowDlg->GetSafeHwnd());
	}

	//////////////////////////////	我的帐号
	pShowDlg = CreateShowMyAccountDlg();
	if (NULL != pShowDlg->GetSafeHwnd())
	{
		NOTIFY_SKIN_HELPER(SKIN_NOTIFY_MYACCOUNT_DLG_HWND, pShowDlg->GetSafeHwnd());
	}
	//////////////////////////////	粉丝排行
	pShowDlg = CreateShowFansDlg();
	if (NULL != pShowDlg->GetSafeHwnd())
	{
		NOTIFY_SKIN_HELPER(SKIN_NOTIFY_FANS_DLG_HWND, pShowDlg->GetSafeHwnd());
	}
	
	//////////////////////////////	音频设置

	pShowDlg = CreateShowSoundSettingDlg();
	if (NULL != pShowDlg->GetSafeHwnd())
	{
		NOTIFY_SKIN_HELPER(SKIN_NOTIFY_SOUND_SETTIND_DLG_HWND, pShowDlg->GetSafeHwnd());
	}
	//////////////////////////////	消息设置

	pShowDlg = CreateShowMessageSettingDlg();
	if (NULL != pShowDlg->GetSafeHwnd())
	{
		NOTIFY_SKIN_HELPER(SKIN_NOTIFY_MESSAGE_SETTING_DLG_HWND, pShowDlg->GetSafeHwnd());
	}
}


BOOL CSysSettingDlg::PreTranslateMessage(MSG* pMsg)
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

//创建个人资料
CDialog* CSysSettingDlg::CreateShowPersonalDlg()
{	
	CDialog* ShowPersonalDlg = NULL;
	MAPOFDLG::iterator it;
	it = m_mShowDlg.find(Show_User_Info_Dlg);
	if(it == m_mShowDlg.end())
	{
		SwitchResourceToModule(_T("Room"));
		CDialog* pShowDlg = NULL;

		//个人资料
		pShowDlg = new CShowPersonalDlg;
		pShowDlg->Create(CShowPersonalDlg::IDD,this);
		pShowDlg->MoveWindow( SUB_DLG_X_POS,SUB_DLG_Y_POS,SUB_DLG_WIDTH,SUB_DLG_HEIGHT ) ;
	//	pShowDlg->SetWindowPos(NULL,SUB_DLG_X_POS,SUB_DLG_Y_POS,SUB_DLG_WIDTH,SUB_DLG_HEIGHT,SWP_SHOWWINDOW);
		pShowDlg->ShowWindow(SW_HIDE);
		m_mShowDlg.insert(pair<UINT,CDialog*>(Show_User_Info_Dlg,pShowDlg));
		ShowPersonalDlg = pShowDlg;

	}	
	else
	{
		ShowPersonalDlg =  it->second;
	}

	return ShowPersonalDlg;

}

//创建基本设置
CDialog* CSysSettingDlg::CreateShowBasicSettingDlg()
{
	CDialog* ShowBasicSettingDlg = NULL;
	MAPOFDLG::iterator it;
	it = m_mShowDlg.find(Show_Basic_Setting_Dlg);
	if(it == m_mShowDlg.end())
	{
		SwitchResourceToModule(_T("Room"));
		CDialog* pShowDlg = NULL;

		//基本设置
		pShowDlg = new CShowBasicSettingDlg;
		pShowDlg->Create(CShowBasicSettingDlg::IDD,this);
		pShowDlg->MoveWindow( SUB_DLG_X_POS,SUB_DLG_Y_POS,SUB_DLG_WIDTH,SUB_DLG_HEIGHT ) ;
	//	pShowDlg->SetWindowPos(NULL,SUB_DLG_X_POS,SUB_DLG_Y_POS,SUB_DLG_WIDTH,SUB_DLG_HEIGHT,SWP_SHOWWINDOW);
		pShowDlg->ShowWindow(SW_HIDE);
		m_mShowDlg.insert(pair<UINT,CDialog*>(Show_Basic_Setting_Dlg,pShowDlg));
		ShowBasicSettingDlg = pShowDlg;

	}
	else
	{
		ShowBasicSettingDlg =  it->second;
	}

	return ShowBasicSettingDlg;

}

//创建视频设置
CDialog* CSysSettingDlg::CreateShowAudioSettingDlg()
{
	CDialog* ShowAudioSettingDlg = NULL;
	MAPOFDLG::iterator it;
	it = m_mShowDlg.find(Show_Audio_Setting_Dlg);
	if(it == m_mShowDlg.end())
	{
		SwitchResourceToModule(_T("Room"));
		CDialog* pShowDlg = NULL;

		//视频设置
		pShowDlg = new CShowAudioSettingDlg;
		pShowDlg->Create(CShowAudioSettingDlg::IDD,this);
		pShowDlg->MoveWindow( SUB_DLG_X_POS,SUB_DLG_Y_POS,SUB_DLG_WIDTH,SUB_DLG_HEIGHT ) ;
		pShowDlg->ShowWindow(SW_HIDE);
		m_mShowDlg.insert(pair<UINT,CDialog*>(Show_Audio_Setting_Dlg,pShowDlg));
		ShowAudioSettingDlg = pShowDlg;

	}	
	else
	{
		ShowAudioSettingDlg =  it->second;
	}

	return ShowAudioSettingDlg;

}


//创建密码安全
CDialog* CSysSettingDlg::CreateShowPwdSafeDlg()
{
	CDialog* ShowPwdSafeDlg = NULL;
	MAPOFDLG::iterator it;
	it = m_mShowDlg.find(Show_PwdSafe_Setting_Dlg);
	if(it == m_mShowDlg.end())
	{
		SwitchResourceToModule(_T("Room"));
		CDialog* pShowDlg = NULL;

		//密码安全
		pShowDlg = new CShowPwdSafeDlg;
		pShowDlg->Create(CShowPwdSafeDlg::IDD,this);
		pShowDlg->MoveWindow( SUB_DLG_X_POS,SUB_DLG_Y_POS,SUB_DLG_WIDTH,SUB_DLG_HEIGHT ) ;
		pShowDlg->ShowWindow(SW_HIDE);
		m_mShowDlg.insert(pair<UINT,CDialog*>(Show_PwdSafe_Setting_Dlg,pShowDlg));
		ShowPwdSafeDlg = pShowDlg;
	}	
	else
	{
		ShowPwdSafeDlg =  it->second;
	}

	return ShowPwdSafeDlg;

}

//创建我的房间
CDialog* CSysSettingDlg::CreateShowMyRoomDlg()
{
	CDialog* ShowMyRoomDlg = NULL;
	MAPOFDLG::iterator it;
	it = m_mShowDlg.find(Show_MgrRoom_Setting_Dlg);
	if(it == m_mShowDlg.end())
	{
		SwitchResourceToModule(_T("Room"));
		CDialog* pShowDlg = NULL;

		//管理房间
		pShowDlg = new CShowMyRoomDlg;
		pShowDlg->Create(CShowMyRoomDlg::IDD,this);
		pShowDlg->MoveWindow( SUB_DLG_X_POS,SUB_DLG_Y_POS,SUB_DLG_WIDTH,SUB_DLG_HEIGHT ) ;
		pShowDlg->ShowWindow(SW_HIDE);
		m_mShowDlg.insert(pair<UINT,CDialog*>(Show_MgrRoom_Setting_Dlg,pShowDlg));
		ShowMyRoomDlg = pShowDlg;
	}	
	else
	{
		ShowMyRoomDlg =  it->second;
	}

	return ShowMyRoomDlg;

}


//创建银行账号
CDialog* CSysSettingDlg::CreateShowBankAccountDlg()
{
	CDialog* ShowBankAccountDlg = NULL;
	MAPOFDLG::iterator it;
	it = m_mShowDlg.find(Show_BankAccount_Dlg);
	if(it == m_mShowDlg.end())
	{
		SwitchResourceToModule(_T("Room"));
		CDialog* pShowDlg = NULL;

		//银行账号
		pShowDlg = new CShowBankAccountDlg;
		pShowDlg->Create(CShowBankAccountDlg::IDD,this);
		pShowDlg->MoveWindow( SUB_DLG_X_POS,SUB_DLG_Y_POS,SUB_DLG_WIDTH,SUB_DLG_HEIGHT ) ;
		pShowDlg->ShowWindow(SW_HIDE);
		m_mShowDlg.insert(pair<UINT,CDialog*>(Show_BankAccount_Dlg,pShowDlg));
		ShowBankAccountDlg = pShowDlg;
	}	
	else
	{
		ShowBankAccountDlg =  it->second;
	}

	return ShowBankAccountDlg;

}

//创建我的账户
CDialog* CSysSettingDlg::CreateShowMyAccountDlg()
{
	CDialog* ShowMyAccountDlg = NULL;
	MAPOFDLG::iterator it;
	it = m_mShowDlg.find(Show_My_Account_Dlg);
	if(it == m_mShowDlg.end())
	{
		SwitchResourceToModule(_T("Room"));
		CDialog* pShowDlg = NULL;

		//我的账户
		pShowDlg = new CShowMyAccountDlg;
		pShowDlg->Create(CShowMyAccountDlg::IDD,this);
		pShowDlg->MoveWindow( SUB_DLG_X_POS,SUB_DLG_Y_POS,SUB_DLG_WIDTH,SUB_DLG_HEIGHT ) ;
		pShowDlg->ShowWindow(SW_HIDE);
		m_mShowDlg.insert(pair<UINT,CDialog*>(Show_My_Account_Dlg,pShowDlg));
		ShowMyAccountDlg = pShowDlg;
	}	
	else
	{
		ShowMyAccountDlg =  it->second;
	}

	return ShowMyAccountDlg;
}

//创建或获取粉丝排行
CDialog* CSysSettingDlg::CreateShowFansDlg()
{
	CDialog* ShowFansDlg = NULL;
	MAPOFDLG::iterator it;
	it = m_mShowDlg.find(Show_Fans_Dlg);
	if(it == m_mShowDlg.end())
	{
		SwitchResourceToModule(_T("Room"));
		CDialog* pShowDlg = NULL;

		//我的账户
		pShowDlg = new CShowFansDlg;
		pShowDlg->Create(CShowFansDlg::IDD,this);
		pShowDlg->MoveWindow( SUB_DLG_X_POS,SUB_DLG_Y_POS,SUB_DLG_WIDTH,SUB_DLG_HEIGHT ) ;
		pShowDlg->ShowWindow(SW_HIDE);
		m_mShowDlg.insert(pair<UINT,CDialog*>(Show_Fans_Dlg,pShowDlg));
		ShowFansDlg = pShowDlg;
	}	
	else
	{
		ShowFansDlg =  it->second;
	}

	return ShowFansDlg;
}

//创建音频设置
CDialog* CSysSettingDlg::CreateShowSoundSettingDlg()
{
	CDialog* ShowSoundSettingDlg = NULL;
	MAPOFDLG::iterator it;
	it = m_mShowDlg.find(Show_Sound_Setting_Dlg);
	if(it == m_mShowDlg.end())
	{
		SwitchResourceToModule(_T("Room"));
		CDialog* pShowDlg = NULL;

		//音频设置
		pShowDlg = new CShowSoundSettingDlg;
		pShowDlg->Create(CShowSoundSettingDlg::IDD,this);
		pShowDlg->MoveWindow( SUB_DLG_X_POS,SUB_DLG_Y_POS,SUB_DLG_WIDTH,SUB_DLG_HEIGHT ) ;
		pShowDlg->ShowWindow(SW_HIDE);
		m_mShowDlg.insert(pair<UINT,CDialog*>(Show_Sound_Setting_Dlg,pShowDlg));
		ShowSoundSettingDlg = pShowDlg;
	}	
	else
	{
		ShowSoundSettingDlg =  it->second;
	}

	return ShowSoundSettingDlg;
}

//创建消息设置
CDialog* CSysSettingDlg::CreateShowMessageSettingDlg()
{
	CDialog* ShowMessageSettingDlg = NULL;
	MAPOFDLG::iterator it;
	it = m_mShowDlg.find(Show_Message_Setting_Dlg);
	if(it == m_mShowDlg.end())
	{
		SwitchResourceToModule(_T("Room"));
		CDialog* pShowDlg = NULL;

		//消息设置
		pShowDlg = new CShowMessageSettingDlg;
		pShowDlg->Create(CShowMessageSettingDlg::IDD,this);
		pShowDlg->MoveWindow( SUB_DLG_X_POS,SUB_DLG_Y_POS,SUB_DLG_WIDTH,SUB_DLG_HEIGHT ) ;
		pShowDlg->ShowWindow(SW_HIDE);
		m_mShowDlg.insert(pair<UINT,CDialog*>(Show_Message_Setting_Dlg,pShowDlg));
		ShowMessageSettingDlg = pShowDlg;
	}	
	else
	{
		ShowMessageSettingDlg =  it->second;
	}

	return ShowMessageSettingDlg;
}


//获取粉丝数据返回响应函数	
void CSysSettingDlg::OnGetFansRsp(Event const&evt)
{
	bool IsFans = false;

	CShowFansDlg* pFansDlg = NULL;
	MAPOFDLG::iterator it;
	it = m_mShowDlg.find(Show_Fans_Dlg);
	if(it != m_mShowDlg.end())  //找到粉丝排行对话框
	{

		CDialog* ShowFansDlg =  it->second;
		if(curShowDlg == ShowFansDlg)  //若当前显示为粉丝排行对话框
		{
			IsFans = true;

		}
		pFansDlg = (CShowFansDlg*)ShowFansDlg;

	}
	
	if(IsFans)
	{
		pFansDlg->OnGetFansRsp(evt);
	}

}