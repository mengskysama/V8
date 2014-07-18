// RoomManagerDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "..\Room.h"
#include "RoomManagerDlg.h"
#include "Skin_i.h"
#include "SkinUtil.h"
#include "AnnouncementDlg.h"
#include "RoomSettingDlg.h"
#include "MicSettingDlg.h"
#include "UserMgrDlg.h"
#include "..\..\CommonLibrary\include\utility\SystemHelper.h"
#include "..\..\CommonLibrary/include/ui/C179MsgBox.h"
#include "GetContributionDlg.h"
#include "TempAnnounceDlg.h"
#include "RoomAnnounceDlg.h"
#include "..\RoomParentDlg.h"

#define  SKIN_ON_CLICK_CLOSE              1
#define  NOTIFY_SKIN_ANNOUNCEMENT_DLG     2
#define  NOTIFY_SKIN_ROOMSETTING_DLG      3
#define  SKIN_ON_CLICK_TAB_TEMP_ANNOUNCE  4
#define  SKIN_ON_CLICK_TAB_ROOMSETTING    5
#define  SKIN_ON_CLICK_TAB_MICSETTING     6
#define  SKIN_ON_CLICK_TAB_USERMGR        7
#define  NOTIFY_SKIN_MICSETTING_DLG       8
#define  NOTIFY_SKIN_USERMGR_DLG          9 
#define  SKIN_ON_CLICK_BTN_GET_CONTRIBUTE 10
#define  SKIN_ON_CLICK_TAB_ROOM_ANNOUNCE  11
#define  NOTIFY_SKIN_TEMP_ANNOUNCEMENT_DLG       12
#define  NOTIFY_SKIN_ROOM_ANNOUNCEMENT_DLG       13
#define SKIN_ON_CLICK_BTN_ROOMER_SYS             14  //室主后台
#define SKIN_ON_CLICK_BTN_CONFIRM                15
#define SKIN_ON_CLICK_BTN_APPLY                  16


#define  CHILD_DLG_WIDTH  595
#define  CHILD_DLG_HEIGHT 378
#define  CHILD_DLG_LEFT   157
#define  CHILD_DLG_TOP    67

#define  CONTRIBUTE_DLG_WIDTH        350
#define  CONTRIBUTE_DLG_HEIGHT       240

#define	ROOMMANAGER_EFFECT_TIMER_IDEVENT 1
#define ROOMMANAGER_EFFECT_TIMER_INTERVALTIME 500

// CRoomManagerDlg 对话框

IMPLEMENT_DYNAMIC(CRoomManagerDlg, CDialog)

CRoomManagerDlg::CRoomManagerDlg( CRoomParentDlg* pParentDlg , CWnd* pParent /*=NULL*/)
	: CDialog(CRoomManagerDlg::IDD, pParent),
	m_pParentDlg(pParentDlg)
{
    m_pSkinEngine = NULL;
	::GetSkinEngine(&m_pSkinEngine);

	m_pAnnouncementDlg = NULL;
	m_pRoomSettingDlg = NULL;
	m_pMicSettingDlg = NULL;
	m_pUserMgrDlg  = NULL;
	m_pTempAnnounceDlg = NULL;
	m_pRoomAnnounceDlg = NULL;
	m_pGetContributeDlg = NULL;

	m_strCurItemBk = L"";
}

CRoomManagerDlg::~CRoomManagerDlg()
{
	if (m_pAnnouncementDlg != NULL)
	{
		delete m_pAnnouncementDlg;
		m_pAnnouncementDlg = NULL;
	}
	if (m_pRoomSettingDlg != NULL)
	{
		delete m_pRoomSettingDlg;
		m_pRoomSettingDlg = NULL;
	}
	if (m_pMicSettingDlg != NULL)
	{
		delete m_pMicSettingDlg;
		m_pMicSettingDlg = NULL;
	}
	if (m_pUserMgrDlg != NULL)
	{
		delete m_pUserMgrDlg;
		m_pUserMgrDlg = NULL;
	}
	if (m_pTempAnnounceDlg != NULL)
	{
		delete m_pTempAnnounceDlg;
		m_pTempAnnounceDlg = NULL;
	}
	if (m_pRoomAnnounceDlg)
	{
		delete m_pRoomAnnounceDlg;
		m_pRoomAnnounceDlg = NULL;
	}
	if (m_pGetContributeDlg)
	{
		delete m_pGetContributeDlg;
		m_pGetContributeDlg = NULL;
	}
}

void CRoomManagerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CRoomManagerDlg, CDialog)
	ON_WM_CREATE()
	ON_SKIN_MESSAGE
	ON_WM_SHOWWINDOW()
	ON_WM_TIMER()
	ON_MESSAGE(WM_NOTIFY_ROOM_SYSSET_MODIFY, OnRoomSetModify)
END_MESSAGE_MAP()


// CRoomManagerDlg 消息处理程序

int CRoomManagerDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  在此添加您专用的创建代码
	ASSERT(m_pSkinEngine != NULL);
	if (m_pSkinEngine != NULL){
		HRESULT result = m_pSkinEngine->LoadSkin(GetSafeHwnd(), L"RoomManagerDlg");
		ASSERT(result == S_OK && __LINE__);
	}

	//标题字体首选微软雅黑
	std::wstring strFont;
	common::utility::systemhelper::GetDefaultFontName(strFont);
	CComVariant var = (CComVariant) strFont.c_str();
	m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(),L"textTitle",L"fontFamily",var);

	return 0;
}

LRESULT CRoomManagerDlg::OnSkinMessage(WPARAM wParam, LPARAM lParam)
{
    switch(wParam)
	{
	case SKIN_ON_CLICK_CLOSE:
		{
          if (this->IsWindowVisible())
          {
			  this->ShowWindow(SW_HIDE);
			  //每一次打开是在临时公告
			  m_pCurrentDlg->ShowWindow(SW_HIDE);
			  m_pTempAnnounceDlg->ShowWindow(SW_SHOW);
			  m_pTempAnnounceDlg->ClearInputCtrlValue() ;
			  m_pRoomAnnounceDlg->ClearInputCtrlValue() ;
			  m_pCurrentDlg = m_pTempAnnounceDlg;

			  CComVariant var = FALSE;
			  m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(), m_strCurItemBk.GetBuffer(), L"visible", var);
			  m_strCurItemBk = L"imgTempAnnounceBk";
			  var = TRUE;
			  m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(), m_strCurItemBk.GetBuffer(), L"visible", var);

          }
		}
		break;
	case SKIN_ON_CLICK_TAB_TEMP_ANNOUNCE:
		{
			if (m_pCurrentDlg != m_pTempAnnounceDlg)
			{
				m_pCurrentDlg->ShowWindow(SW_HIDE);
				m_pTempAnnounceDlg->ShowWindow(SW_SHOW);
				m_pCurrentDlg = m_pTempAnnounceDlg;
				if (m_strCurItemBk != L"")
				{
					CComVariant var = FALSE;
					m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(), m_strCurItemBk.GetBuffer(), L"visible", var);
					var = TRUE;
					m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(), L"imgTempAnnounceBk", L"visible", var);
					m_strCurItemBk = L"imgTempAnnounceBk";
				}
			}
			
		}
		break;
	case SKIN_ON_CLICK_TAB_ROOM_ANNOUNCE:
		{
			if (m_pCurrentDlg != m_pRoomAnnounceDlg)
			{
				m_pCurrentDlg->ShowWindow(SW_HIDE);
				m_pRoomAnnounceDlg->ShowWindow(SW_SHOW);
				m_pCurrentDlg = m_pRoomAnnounceDlg;
				if (m_strCurItemBk != L"")
				{
					CComVariant var = FALSE;
					m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(), m_strCurItemBk.GetBuffer(), L"visible", var);
					var = TRUE;
					m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(), L"imgRoomAnnounceBk", L"visible", var);
					m_strCurItemBk = L"imgRoomAnnounceBk";
				}
			}	
		}
		break;
	case SKIN_ON_CLICK_TAB_ROOMSETTING:
		{
			if (m_pCurrentDlg != m_pRoomSettingDlg)
			{
				m_pCurrentDlg->ShowWindow(SW_HIDE);
				m_pRoomSettingDlg->ShowWindow(SW_SHOW);
				m_pCurrentDlg = m_pRoomSettingDlg;
				if (m_strCurItemBk != L"")
				{
					CComVariant var = FALSE;
					m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(), m_strCurItemBk.GetBuffer(), L"visible", var);
					var = TRUE;
					m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(), L"imgRoomSettingBk", L"visible", var);
					m_strCurItemBk = L"imgRoomSettingBk";
				}
			}	
		}
		break;
	case SKIN_ON_CLICK_TAB_MICSETTING:
		{
			if (m_pCurrentDlg != m_pMicSettingDlg)
			{
				m_pCurrentDlg->ShowWindow(SW_HIDE);
				m_pMicSettingDlg->ShowWindow(SW_SHOW);
				m_pCurrentDlg = m_pMicSettingDlg;
				if (m_strCurItemBk != L"")
				{
					CComVariant var = FALSE;
					m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(), m_strCurItemBk.GetBuffer(), L"visible", var);
					var = TRUE;
					m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(), L"imgMicSettingBk", L"visible", var);
					m_strCurItemBk = L"imgMicSettingBk";
				}
			}
		}
		break;
	case SKIN_ON_CLICK_TAB_USERMGR://这个是迎宾设置
		{
			if (m_pCurrentDlg != m_pUserMgrDlg)
			{
				m_pCurrentDlg->ShowWindow(SW_HIDE);
				m_pUserMgrDlg->ShowWindow(SW_SHOW);
				m_pCurrentDlg = m_pUserMgrDlg;
				if (m_strCurItemBk != L"")
				{
					CComVariant var = FALSE;
					m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(), m_strCurItemBk.GetBuffer(), L"visible", var);
					var = TRUE;
					m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(), L"imgWelcomeSettingBk", L"visible", var);
					m_strCurItemBk = L"imgWelcomeSettingBk";
				}
			}	
		}
		break;
	case SKIN_ON_CLICK_BTN_GET_CONTRIBUTE:
		{
            //OutputDebugString(L"\n$$$$$$$$$$$$$$$$$$$$$ 领贡献 $$$$$$$$$$$$$$$$$$$$\n");
			OnClickGetContribute();
		}
		break;
	case SKIN_ON_CLICK_BTN_ROOMER_SYS:
		{
			OnClickBtnRoomerSys();
		}
		break;
	case SKIN_ON_CLICK_BTN_CONFIRM:
		{
			OnClickBtnConfirm();
		}
		break;
	case SKIN_ON_CLICK_BTN_APPLY:
		{
			OnClickBtnApply();
		}
		break;
	default:break;
	}

	return TRUE;
}

BOOL CRoomManagerDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	ModifyStyleEx(0,WS_EX_TOOLWINDOW);
	//ModifyStyleEx(0,WS_DISABLED);	//WS_DISABLED应该用ModifyStyle，这里相当于调用了ModifyStyleEx(0, WS_EX_NOACTIVATE);

	NotifyCreateSkinItem();
    //SetRoomerSysAddress();

	//临时公告子窗口
	m_pTempAnnounceDlg = new CTempAnnounceDlg(m_pParentDlg);
	if (m_pTempAnnounceDlg)
	{
		m_pTempAnnounceDlg->Create(CTempAnnounceDlg::IDD,this);
		m_pTempAnnounceDlg->MoveWindow( CHILD_DLG_LEFT,CHILD_DLG_TOP,CHILD_DLG_WIDTH,CHILD_DLG_HEIGHT) ;
	//	m_pTempAnnounceDlg->SetWindowPos(this,CHILD_DLG_LEFT,CHILD_DLG_TOP,CHILD_DLG_WIDTH,CHILD_DLG_HEIGHT,SWP_SHOWWINDOW);
		m_pTempAnnounceDlg->ShowWindow(SW_SHOW);//默认显示临时公告
		m_pCurrentDlg = m_pTempAnnounceDlg;
		m_strCurItemBk = L"imgTempAnnounceBk";
		CComVariant var = TRUE;
		m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(), m_strCurItemBk.GetBuffer(), L"visible", var);
	}
	//房间公告子窗口
	m_pRoomAnnounceDlg = new CRoomAnnounceDlg(m_pParentDlg);
	if (m_pRoomAnnounceDlg)
	{
		m_pRoomAnnounceDlg->Create(CRoomAnnounceDlg::IDD,this);
		m_pRoomAnnounceDlg->MoveWindow( CHILD_DLG_LEFT,CHILD_DLG_TOP,CHILD_DLG_WIDTH,CHILD_DLG_HEIGHT ) ;
	//	m_pRoomAnnounceDlg->SetWindowPos(this,CHILD_DLG_LEFT,CHILD_DLG_TOP,CHILD_DLG_WIDTH,CHILD_DLG_HEIGHT,SWP_HIDEWINDOW);
	}
	//房间设置子窗口
	m_pRoomSettingDlg = new CRoomSettingDlg(m_pParentDlg);
	if (m_pRoomSettingDlg)
	{
		m_pRoomSettingDlg->Create(CRoomSettingDlg::IDD,this);
		m_pRoomSettingDlg->MoveWindow( CHILD_DLG_LEFT,CHILD_DLG_TOP,CHILD_DLG_WIDTH,CHILD_DLG_HEIGHT ) ;
	//	m_pRoomSettingDlg->SetWindowPos(this,CHILD_DLG_LEFT,CHILD_DLG_TOP,CHILD_DLG_WIDTH,CHILD_DLG_HEIGHT,SWP_HIDEWINDOW);
	}
	//排麦设置子窗口
	m_pMicSettingDlg = new CMicSettingDlg(m_pParentDlg);
	if (m_pMicSettingDlg)
	{
		m_pMicSettingDlg->Create(CMicSettingDlg::IDD,this);
		m_pMicSettingDlg->MoveWindow( CHILD_DLG_LEFT,CHILD_DLG_TOP,CHILD_DLG_WIDTH,CHILD_DLG_HEIGHT ) ;
	//	m_pMicSettingDlg->SetWindowPos(this,CHILD_DLG_LEFT,CHILD_DLG_TOP,CHILD_DLG_WIDTH,CHILD_DLG_HEIGHT,SWP_HIDEWINDOW);
	}
	//用户管理子窗口
	m_pUserMgrDlg = new CUserMgrDlg(m_pParentDlg);
	if (m_pUserMgrDlg)
	{
		m_pUserMgrDlg->Create(CUserMgrDlg::IDD,this);
		m_pUserMgrDlg->MoveWindow( CHILD_DLG_LEFT,CHILD_DLG_TOP,CHILD_DLG_WIDTH,CHILD_DLG_HEIGHT ) ;
	//	m_pUserMgrDlg->SetWindowPos(this,CHILD_DLG_LEFT,CHILD_DLG_TOP,CHILD_DLG_WIDTH,CHILD_DLG_HEIGHT,SWP_HIDEWINDOW);
	}

	if (m_pParentDlg != NULL)
	{
		IUserItem * pUserItem = m_pParentDlg->GetRoomBusiness()->GetUserManager()->GetSelfUserItem();
		if (pUserItem != NULL)
		{
			if (pUserItem->GetUserTitleLevel() == core::TITLELEVEL_OWNER)
			{
				CComVariant var = FALSE;
				m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(), L"btnRoomerSys", L"disabled",var);
				//var = TRUE;
				//m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(), L"btnGetContribution", L"disabled", var);
			}

			if (pUserItem->GetUserTitleLevel() == core::TITLELEVEL_SUPER_MANAGER)
			{
				CComVariant var = TRUE;
				m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(), L"btnRoomerSys", L"disabled",var);
				m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(), L"btnGetContribution", L"disabled", var);
			}
		}
	}

	EnableBtnApply(false);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CRoomManagerDlg::NotifyCreateSkinItem()
{

    if (m_pTempAnnounceDlg->GetSafeHwnd())
    {
		NOTIFY_SKIN_HELPER(NOTIFY_SKIN_TEMP_ANNOUNCEMENT_DLG,m_pTempAnnounceDlg->GetSafeHwnd());
    }
	if (m_pRoomAnnounceDlg->GetSafeHwnd())
	{
		NOTIFY_SKIN_HELPER(NOTIFY_SKIN_ROOM_ANNOUNCEMENT_DLG,m_pRoomAnnounceDlg->GetSafeHwnd());
	}
	if (m_pRoomSettingDlg->GetSafeHwnd())
	{
		NOTIFY_SKIN_HELPER(NOTIFY_SKIN_ROOMSETTING_DLG,m_pRoomSettingDlg->GetSafeHwnd());
	}
    if (m_pMicSettingDlg->GetSafeHwnd())
    {
		NOTIFY_SKIN_HELPER(NOTIFY_SKIN_MICSETTING_DLG,m_pMicSettingDlg->GetSafeHwnd());
    }
    if (m_pUserMgrDlg->GetSafeHwnd())
    {
		NOTIFY_SKIN_HELPER(NOTIFY_SKIN_USERMGR_DLG,m_pUserMgrDlg->GetSafeHwnd());
    }
}

//void CRoomManagerDlg::SetRoomerSysAddress()
//{
//	CComVariant var = L"www.51.com/company";
//	m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(),L"btnRoomerSys",L"hyperLinkAddress",var);
//}

BOOL CRoomManagerDlg::PreTranslateMessage(MSG* pMsg)
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

//显示到 公告信息 一栏
void CRoomManagerDlg::TurnToAnnouncePanel( CString& strText ) 
{
	m_pCurrentDlg->ShowWindow(SW_HIDE);
	m_pTempAnnounceDlg->ShowWindow(SW_SHOW);
	m_pCurrentDlg = m_pTempAnnounceDlg;
	CComVariant var;
	var = -1;
	m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(), L"tabSetting", L"tabItemSelected", var);
	var = 0;
	m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(), L"tabAnnouncement", L"tabItemSelected", var);

	m_pTempAnnounceDlg->SetInputAnnounce( strText ) ;
}


void CRoomManagerDlg::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialog::OnShowWindow(bShow, nStatus);

	if (bShow)
	{
	//	m_pAnnouncementDlg->Refresh();
		m_pRoomSettingDlg->Refresh();
		m_pMicSettingDlg->Refresh();
		m_pUserMgrDlg->Refresh();
	}
	else
	{
		EnableBtnApply(false);
	}

	// TODO: 在此处添加消息处理程序代码
}

void CRoomManagerDlg::OnClickGetContribute()
{
	SwitchResourceToModule(_T("Room"));

	IUserItem * pUserItem = m_pParentDlg->GetRoomBusiness()->GetUserManager()->GetSelfUserItem();
	if (pUserItem != NULL)
	{
		if (pUserItem->GetUserTitleLevel() == core::TITLELEVEL_OWNER)
		{
			Hall::WebInterfaceUrl weburl;
			weburl.m_WebWithPara = false;
			weburl.m_WebInterfaceKeyName = SYSSET_INI_KEYNAME_MASTERGARDEN;
			g_RoomModule->m_pModuleManager->CallService(Hall::SERVICE_VALUE_HALL_GET_WEBINTERFACEURL, (param)&weburl);
			C179MsgBox::Show(this, NewMsgBox::MSGBOX_OK_ONE_STRING_WITH_LINK, L"请前往室主后台领取您的专有贡献", L"", L"", L"立即前往",
				weburl.m_WebInterfaceUrl.c_str());

			return;
		}
	}
	
	if (m_pGetContributeDlg == NULL)
	{
		m_pGetContributeDlg = new CGetContributionDlg(this, m_pParentDlg->GetRoomID());
		m_pGetContributeDlg->Create(CGetContributionDlg::IDD, this);
		m_pGetContributeDlg->CenterWindow();
		m_pGetContributeDlg->ShowWindow(SW_SHOW);
		return;
	}

	if (m_pGetContributeDlg != NULL && m_pGetContributeDlg->GetSafeHwnd())
	{
		m_pGetContributeDlg->ShowWindow(SW_SHOW);
		m_pGetContributeDlg->SetForegroundWindow();
	}
}

void CRoomManagerDlg::OnClickBtnRoomerSys()
{
	Hall::WebInterfaceUrl weburl;
	weburl.m_WebWithPara = false;
	weburl.m_WebInterfaceKeyName = SYSSET_INI_KEYNAME_ROOMMASTER;
	g_RoomModule->m_pModuleManager->CallService(Hall::SERVICE_VALUE_HALL_GET_WEBINTERFACEURL, (param)&weburl);
	common::utility::systemhelper::OpenUrl(weburl.m_WebInterfaceUrl) ;
}

void CRoomManagerDlg::OnClickBtnConfirm()
{
	Confirm();
	if (this->IsWindowVisible())
	{
		this->ShowWindow(SW_HIDE);
		//每一次打开是在临时公告
		m_pCurrentDlg->ShowWindow(SW_HIDE);
		m_pTempAnnounceDlg->ShowWindow(SW_SHOW);
		m_pTempAnnounceDlg->ClearInputCtrlValue() ;
		m_pRoomAnnounceDlg->ClearInputCtrlValue() ;
		m_pCurrentDlg = m_pTempAnnounceDlg;

		CComVariant var = FALSE;
		m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(), m_strCurItemBk.GetBuffer(), L"visible", var);
		m_strCurItemBk = L"imgTempAnnounceBk";
		var = TRUE;
		m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(), m_strCurItemBk.GetBuffer(), L"visible", var);
	}
}	

void CRoomManagerDlg::OnClickBtnApply()
{
	SetApplyEffect(TRUE);
	SetTimer(ROOMMANAGER_EFFECT_TIMER_IDEVENT,ROOMMANAGER_EFFECT_TIMER_INTERVALTIME,NULL);

	Confirm();
	EnableBtnApply(false);
}

void CRoomManagerDlg::SetApplyEffect(BOOL bEffect)
{
	if (m_pCurrentDlg != NULL)
	{
		if (m_pCurrentDlg == m_pTempAnnounceDlg)//临时公告
		{
			m_pTempAnnounceDlg->SetApplyEffect(bEffect);
		}else if (m_pCurrentDlg == m_pRoomAnnounceDlg)//房间公告
		{
			m_pRoomAnnounceDlg->SetApplyEffect(bEffect);
		}else if (m_pCurrentDlg == m_pRoomSettingDlg)//房间设置
		{
			m_pRoomSettingDlg->SetApplyEffect(bEffect);
		}else if (m_pCurrentDlg == m_pMicSettingDlg)//排麦设置
		{
			m_pMicSettingDlg->SetApplyEffect(bEffect);
		}else if (m_pCurrentDlg == m_pUserMgrDlg)//迎宾设置
		{
			m_pUserMgrDlg->SetApplyEffect(bEffect);
		}
	}
}
void CRoomManagerDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if(nIDEvent == ROOMMANAGER_EFFECT_TIMER_IDEVENT)
	{
		SetApplyEffect(FALSE);
		KillTimer(ROOMMANAGER_EFFECT_TIMER_IDEVENT);
	}	

	CDialog::OnTimer(nIDEvent);
}

LRESULT CRoomManagerDlg::OnRoomSetModify( WPARAM wParam, LPARAM lParam )
{
	EnableBtnApply(true);
	return 0;
}

void CRoomManagerDlg::EnableBtnApply( bool bEnable )
{
	CComVariant var = bEnable ? false : true;
	m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(),L"btnBottomApply",L"disabled",var); 
}

void CRoomManagerDlg::Confirm()
{
	if (m_pCurrentDlg != NULL)
	{
		if (m_pCurrentDlg == m_pRoomSettingDlg)//房间设置
		{
			m_pRoomSettingDlg->OnConfirm();
		}
		else if (m_pCurrentDlg == m_pMicSettingDlg)//排麦设置
		{
			m_pMicSettingDlg->OnConfirm();
		}
		else if (m_pCurrentDlg == m_pUserMgrDlg)//迎宾设置
		{
			m_pUserMgrDlg->OnClickSave();
		}
	}
}