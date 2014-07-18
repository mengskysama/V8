// SelfInfoAndSetDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "..\Room.h"
#include "SelfInfoAndSetDlg.h"
#include "skin_i.h"
#include "SkinUtil.h"
#include "HallDefine.h"
#include "..\RoomBottomLeftDlg.h"
//#include "..\RoomLeftDlg.h"
#include "..\RoomTopManager.h"
#include "..\userlist\ListIconManager.h"
#include "..\..\CommonLibrary\include\utility\SystemHelper.h"
#include "..\userlist\UserListMenu.h"
#include "..\..\CommonLibrary\include\ui\C179MsgBox.h"
#include "..\richlevel\RichLevelManager.h"
#include "..\syssetting\SysSettingDlg.h"
#include "..\roomsetting\RoomManagerDlg.h"
#include "..\autoreply\AutoReplyConfig.h"
#include "..\autoreply\AutoReplySettingDlg.h"
#include "..\SysSetting\ShowAudioTestDlg.h"
#include "..\personalsetting\PersonalConfig.h"
#include "VeritySliderDlg.h"

#define   SKIN_ON_CLICK_HEAD				1
#define   SKIN_ON_CLICK_USERLEVEL			2
#define   SKIN_ON_CLICK_USERROLE			3
#define   SKIN_ON_CLICK_SYSSETTING			4
#define   SKIN_ON_CLICK_ROOMMANAGER			5
#define   SKIN_ON_CLICK_ON_OFF_LINE			6
#define   SKIN_ON_CLICK_ICON_MICON			7
#define   SKIN_ON_CLICK_ICON_MICOFF			8
#define   SKIN_ON_CLICK_ICON_SPEAKERON		9
#define   SKIN_ON_CLICK_ICON_SPEAKEROFF		10
#define   SKIN_ON_CLICK_BTN_AUDIOSET		11

#define   NOTIFY_SKIN_TO_ONSIZE				20
#define   SKIN_NOTIFY_MICSLIDER_DLG         21
#define   SKIN_NOTIFY_SPEAKERSLIDER_DLG     22

#define   VERITY_SLIDER_DLG_WIDTH           15
#define   VERITY_SLIDER_DLG_HEIGHT			100

#define  WM_CLICK_AUDIOSET_MENU   WM_USER+5000 
// CSelfInfoAndSetDlg 对话框

IMPLEMENT_DYNAMIC(CSelfInfoAndSetDlg, CDialog)

CSelfInfoAndSetDlg::CSelfInfoAndSetDlg(CRoomParentDlg* pRoomParentDlg, CWnd* pParent /*=NULL*/)
	: CDialog(CSelfInfoAndSetDlg::IDD, pParent),
	m_pRoomParentDlg(pRoomParentDlg)
{
	m_pSkinEngine = NULL;
	::GetSkinEngine(&m_pSkinEngine);

	m_pRichLevelBtn = NULL;
	m_pSysSettingDlg = NULL;
	m_pRoomManagerDlg = NULL;
	m_pAudioTest = NULL;
	m_pMicSliderDlg = NULL;
	m_pSpeakerSliderDlg = NULL;
}

CSelfInfoAndSetDlg::~CSelfInfoAndSetDlg()
{
	SAFE_DELETE_DIALOG(m_pRichLevelBtn);
	SAFE_DELETE_DIALOG(m_pSysSettingDlg);
	SAFE_DELETE_DIALOG(m_pRoomManagerDlg);
	SAFE_DELETE_DIALOG(m_pAudioTest);
	SAFE_DELETE_DIALOG(m_pMicSliderDlg);
	SAFE_DELETE_DIALOG(m_pSpeakerSliderDlg);
}

void CSelfInfoAndSetDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CSelfInfoAndSetDlg, CDialog)
	ON_WM_CREATE()
	ON_SKIN_MESSAGE
	ON_WM_MOUSEMOVE()
	ON_MESSAGE(WM_CLICK_AUTOREPLY_MENU, OnClickBtnAutoReplyMsg)
	ON_MESSAGE(WM_CLICK_AUDIOSET_MENU, OnClickMenuAudioSet)
	ON_MESSAGE(WM_ONVSCROLL_VERTI_SLIDER , &CSelfInfoAndSetDlg::OnVScrollSlider)
	ON_MESSAGE(COMMONLIB_UM_NONDOMODAL_MSGBOX_GOTOREGISTER, &CSelfInfoAndSetDlg::OnClickMsgBoxGoToRegister)
END_MESSAGE_MAP()


// CSelfInfoAndSetDlg 消息处理程序


int CSelfInfoAndSetDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  在此添加您专用的创建代码
	//LoadSkin
	ASSERT(NULL != m_pSkinEngine);
	if (NULL != m_pSkinEngine)
	{
		HRESULT hr = m_pSkinEngine->LoadSkin(GetSafeHwnd(),L"SelfInfoAndSetDlg");
		ASSERT(hr== S_OK && __LINE__);
	}

	return 0;
}

BOOL CSelfInfoAndSetDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	DrawBtnRichLevel();
	SetMyStatus( !( AutoReplyMgr->IsAutoReply() ) );

	//CRect rcTemp;
	if (m_pMicSliderDlg == NULL)
	{
		m_pMicSliderDlg = new CVeritySliderDlg(this);
		m_pMicSliderDlg->Create(CVeritySliderDlg::IDD, NULL);
		//m_pSkinEngine->GetSubSkinPos(GetSafeHwnd(), L"imgMicOn", &(rcTemp.left), &(rcTemp.top), &(rcTemp.right), &(rcTemp.bottom));
		//m_pMicSliderDlg->MoveWindow(0, 0, 38, 140);
		m_pMicSliderDlg->ShowWindow(SW_HIDE);
		//NOTIFY_SKIN_HELPER(SKIN_NOTIFY_MICSLIDER_DLG, m_pMicSliderDlg->GetSafeHwnd());
	}

	if (m_pSpeakerSliderDlg == NULL)
	{
		m_pSpeakerSliderDlg = new CVeritySliderDlg(this);
		m_pSpeakerSliderDlg->Create(CVeritySliderDlg::IDD, NULL);
		//m_pSkinEngine->GetSubSkinPos(GetSafeHwnd(), L"imgSpeakerOn", &(rcTemp.left), &(rcTemp.top), &(rcTemp.right), &(rcTemp.bottom));
		//m_pSpeakerSliderDlg->MoveWindow(40, 0, 38, 140);
		m_pSpeakerSliderDlg->ShowWindow(SW_HIDE);
		//NOTIFY_SKIN_HELPER(SKIN_NOTIFY_SPEAKERSLIDER_DLG, m_pSpeakerSliderDlg->GetSafeHwnd());
	}

	InitSlider();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


LRESULT CSelfInfoAndSetDlg::OnSkinMessage(WPARAM wParam,LPARAM lParam)
{
	switch(wParam)
	{
	case SKIN_ON_CLICK_HEAD:
		{
			OnClickHead();
		}
		break;
	case SKIN_ON_CLICK_USERLEVEL:
		{
			OnClickUserLevel();
		}
		break;
	case SKIN_ON_CLICK_USERROLE:
		{
			OnClickUserRole();
		}
		break;
	case SKIN_ON_CLICK_SYSSETTING:
		{
			OnClickSysSetting();
		}
		break;
	case SKIN_ON_CLICK_ROOMMANAGER:
		{
			OnClickRoomManager();
		}
		break;
	case SKIN_ON_CLICK_ON_OFF_LINE:
		{
			OnClickOnOffLine();
		}
		break;
	case SKIN_ON_CLICK_ICON_MICON:
		{
			//CRoomLeftDlg抽象为CRoomTopManager
			//m_pRoomParentDlg->GetRoomLeftDlg()->CloseMic() ;
			m_pRoomParentDlg->GetRoomTopManager()->CloseMic();
			g_RoomModule->CloseMic();
		}
		break;
	case SKIN_ON_CLICK_ICON_MICOFF:
		{
			//CRoomLeftDlg抽象为CRoomTopManager
			//m_pRoomParentDlg->GetRoomLeftDlg()->OpenMic() ;
			m_pRoomParentDlg->GetRoomTopManager()->OpenMic();
			g_RoomModule->OpenMic();
		}
		break;
	case SKIN_ON_CLICK_ICON_SPEAKERON:
		{
			OnClickBtnCloseSound();
		}
		break;
	case SKIN_ON_CLICK_ICON_SPEAKEROFF:
		{
			OnClickBtnOpenSound();
		}
		break;
	case SKIN_ON_CLICK_BTN_AUDIOSET:
		{
			OnClickBtnAudioSet();
		}
		break;
	default:break;
	}

	return TRUE;
}

void CSelfInfoAndSetDlg::OnClickHead()
{
	//AfxMessageBox(L"点击头像");
	if (m_pRoomParentDlg == NULL || GetCurInfo().enmVIPLevel == core::VIPLEVEL_NONE)//游客
	{
		return;
	}
	m_pRoomParentDlg->ShowSelfUserInfo();
}

void CSelfInfoAndSetDlg::OnMouseMove( UINT nFlags, CPoint point )
{
	if (!IsWindowVisible())
		return;

	CRect rcHead(0,0,0,0);
	m_pSkinEngine->GetSubSkinPos(GetSafeHwnd(), L"imgMyHeadCtrlBack_N", &(rcHead.left), &(rcHead.top), &(rcHead.right), &(rcHead.bottom));
	//ScreenToClient(&point);
	BOOL bHoverHead = PtInRect(&rcHead, point);
	m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(), L"imgMyHeadCtrlBack_N", L"visible", (CComVariant)(!bHoverHead));
	m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(), L"imgMyHeadCtrlBack_H", L"visible", (CComVariant)(bHoverHead));
	
	if (m_pMicSliderDlg != NULL)
	{	
		CRect rcMic, rcSlider;
		CPoint pt = point;
		ClientToScreen(&pt);
		m_pSkinEngine->GetSubSkinPos(GetSafeHwnd(), L"imgMicOn", &(rcMic.left), &(rcMic.top), &(rcMic.right), &(rcMic.bottom));
		rcSlider.left = rcMic.left + 1;
		rcSlider.top = rcMic.top - VERITY_SLIDER_DLG_HEIGHT;
		rcSlider.right = rcSlider.left + VERITY_SLIDER_DLG_WIDTH;
		rcSlider.bottom = rcSlider.top + VERITY_SLIDER_DLG_HEIGHT;
		ClientToScreen(&rcSlider);
		if ( PtInRect(&rcMic, point) )
		{
			ClientToScreen(&rcMic);
			m_pMicSliderDlg->MoveWindow(&rcSlider);
			m_pMicSliderDlg->ShowWindow(SW_SHOWNOACTIVATE);
		}else
		{
			if (!PtInRect(&rcSlider, pt))
			{
				m_pMicSliderDlg->ShowWindow(SW_HIDE);
			}
		}
	}

	if (m_pSpeakerSliderDlg != NULL)
	{	
		CRect rcSpeaker, rcSlider;
		CPoint pt = point;
		ClientToScreen(&pt);
		m_pSkinEngine->GetSubSkinPos(GetSafeHwnd(), L"imgSpeakerOn", &(rcSpeaker.left), &(rcSpeaker.top), &(rcSpeaker.right), &(rcSpeaker.bottom));
		rcSlider.left = rcSpeaker.left + 1;
		rcSlider.top = rcSpeaker.top - VERITY_SLIDER_DLG_HEIGHT;
		rcSlider.right = rcSlider.left + VERITY_SLIDER_DLG_WIDTH;
		rcSlider.bottom = rcSlider.top + VERITY_SLIDER_DLG_HEIGHT;
		ClientToScreen(&rcSlider);
		if ( PtInRect(&rcSpeaker, point) )
		{
			ClientToScreen(&rcSpeaker);
			m_pSpeakerSliderDlg->MoveWindow(&rcSlider);
			m_pSpeakerSliderDlg->ShowWindow(SW_SHOWNOACTIVATE);
		}else
		{
			if (!PtInRect(&rcSlider, pt))
			{
				m_pSpeakerSliderDlg->ShowWindow(SW_HIDE);
			}
		}
	}

	CDialog::OnMouseMove(nFlags, point);
}

void CSelfInfoAndSetDlg::SetBottomUserInfo( IUserItem* pUserItem )
{
	if (pUserItem == NULL)
	{
		return;
	}
	SetUserHead(pUserItem);
	SetUserLevel(pUserItem);
	SetRichLevel(pUserItem);
	SetUserRole(pUserItem);
	SetUserName(pUserItem);
}

void CSelfInfoAndSetDlg::SetUserHead( IUserItem* pUserItem )
{
	Hall::HeadImage headImage;
	headImage.m_bMan = pUserItem->IsMale();
	headImage.m_isGuest = (pUserItem->GetUserVipLevel() == core::VIPLEVEL_NONE ? true : false);
	headImage.m_unUIN = pUserItem->GetUserUIN();
	g_RoomModule->m_pModuleManager->CallService(Hall::SERVICE_VALUE_HALL_UPDATE_HEAD_IMAGE_PATH, (param)&headImage);
	m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(), L"imgMyHead", L"absoluteImage", (CComVariant)(CString)headImage.m_HeadImagePath.c_str());
}

void CSelfInfoAndSetDlg::SetUserLevel( IUserItem* pUserItem )
{
	std::wstring strPath = ListIconManager::GetVIPIconStatic(pUserItem);
	m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(), L"imgUserLevel", L"absIconImage", (CComVariant)strPath.c_str());
	std::wstring strTip;
	if (pUserItem->GetUserVipLevel() > core::VIPLEVEL_REGISTER)
	{
		std::wstring strVip = ListIconManager::GetIconTipByVipLevel(pUserItem->GetUserVipLevel());
		strTip = L"您是 " + strVip + L" 用户";
	}
	else
	{
		strTip = L"升级为VIP";
	}
	m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(), L"imgUserLevel", L"tip", (CComVariant)strTip.c_str());
}

void CSelfInfoAndSetDlg::OnClickUserLevel()
{
	common::utility::systemhelper::OpenUrl(m_pRoomParentDlg->GetRoomBusiness()->GetUpdateVipUrl().GetBuffer()) ;
}	

void CSelfInfoAndSetDlg::SetRichLevel(IUserItem *pUserItem)
{
	if (m_pRichLevelBtn == NULL)
	{
		return;
	}
	if (pUserItem->IsOfficial() || (pUserItem->GetUserVipLevel() == core::VIPLEVEL_NONE) )
	{
		return;
	}

	std::wstring strRichLevelIco = RichLevelManager::GetRichLevelImgPath(pUserItem);
	if (!strRichLevelIco.empty())
	{
		CString strTemp;
		strTemp.Format(L"%s", strRichLevelIco.c_str());
		m_pRichLevelBtn->SetGifBtnGifIcon(strTemp);
		CRect rcGifBtn;
		m_pRichLevelBtn->GetWindowRect(&rcGifBtn);
		m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(), L"imgRichLevel", L"width", (CComVariant)rcGifBtn.Width());
		m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(), L"imgRichLevel", L"height", (CComVariant)rcGifBtn.Height());

		CComVariant var;
		m_pSkinEngine->GetSubSkinProp(GetSafeHwnd(), L"imgRichLevel", L"visible", &var);
		MoveBtnRichLevel(var.boolVal);

		SetRichLevelTip();
		NOTIFY_SKIN_HELPER(NOTIFY_SKIN_TO_ONSIZE, (LONG)0);
	}
}

void CSelfInfoAndSetDlg::SetRichLevelTip()
{
	if (m_pRichLevelBtn == NULL)
	{
		return;
	}
	CString strTemp, strCurLevelName, strNextLevelName;
	if (GetCurInfo().unRichLevel == 0)
	{
		strTemp.Format(L"快去消费吧！指挥辣妹跟你玩,还能升级哦！");
	}else
	{
		if (GetCurInfo().unRichLevel >= 25)//已到最高级别
		{
			strTemp.Format(L"您现在是 %s", RichLevelManager::GetRichLevelNameByLevel(25).c_str());
		}else if (GetCurInfo().unRichLevel >= 0 && GetCurInfo().unRichLevel < 25)
		{
			uint32 nMoney = GetCurInfo().unRichUpdateMoney;
			strCurLevelName.Format( L"%s", RichLevelManager::GetRichLevelNameByLevel(GetCurInfo().unRichLevel).c_str() );
			strNextLevelName.Format( L"%s", RichLevelManager::GetRichLevelNameByLevel(GetCurInfo().unRichLevel + 1).c_str() );
			strTemp.Format(L"您现在是 %s，离升级到 %s 还差%u币",strCurLevelName.GetBuffer(), strNextLevelName.GetBuffer(), nMoney);
		}
	}
	m_pRichLevelBtn->SetGifBtnTipText(strTemp);
}

void CSelfInfoAndSetDlg::DrawBtnRichLevel()
{
	if ( (GetCurInfo().unBusiness&core::BUSINESSTYPE_AGENT) || (GetCurInfo().unBusiness&core::BUSINESSTYPE_OPERATOR) 
		|| (GetCurInfo().unBusiness&core::BUSINESSTYPE_AD_CLEANER) || (GetCurInfo().unBusiness&core::BUSINESSTYPE_PATROLMAN) ) //官方人員
	{
		return;
	}
	
	if (GetCurInfo().enmVIPLevel == core::VIPLEVEL_NONE)//游客
	{
		return;
	}

	SwitchResourceToModule(_T("Room"));
	m_pRichLevelBtn = new common::ui::GifButton();
	CString strTemp = L"";
	if (m_pRichLevelBtn->CreateEx(this, strTemp))
	{
		MoveBtnRichLevel(FALSE);
	}
}

void CSelfInfoAndSetDlg::MoveBtnRichLevel(BOOL bShow)
{
	if (m_pRichLevelBtn != NULL && m_pRichLevelBtn->GetSafeHwnd() != NULL)
	{
		if (!bShow)
		{
			m_pRichLevelBtn->ShowWindow(SW_HIDE);
		}else
		{
			CRect rc;
			m_pSkinEngine->GetSubSkinPos(GetSafeHwnd(), L"imgRichLevel", &(rc.left), &(rc.top), &(rc.right), &(rc.bottom));
			ClientToScreen(&rc);
			m_pRichLevelBtn->ShowGifBtn(rc);
		}
	}
}

void CSelfInfoAndSetDlg::OnClickUserRole() //点击后定位用户列表中的位置
{
	if (GetParent() != NULL)
	{
		( (CRoomBottomLeftDlg *)GetParent() )->ShowUserItem( m_pRoomParentDlg->GetRoomBusiness()->GetUserManager()->GetSelfUserItem() );
	}
}

void CSelfInfoAndSetDlg::SetUserRole( IUserItem* pUserItem )
{
	std::wstring strTemp = ListIconManager::GetThirdColumnIcon(pUserItem);
	m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(), L"imgUserRole", L"absIconImage", (CComVariant)strTemp.c_str());

	strTemp = L"您在此房间的身份为：" + ListIconManager::GetIconTipByUser(pUserItem);
	m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(), L"imgUserRole", L"tip", (CComVariant)strTemp.c_str());
}

void CSelfInfoAndSetDlg::OnClickSysSetting()
{
	CreateSysSettingDlg();	
	if(m_pSysSettingDlg)
	{
		m_pSysSettingDlg->OpenDlg(Show_Basic_Setting_Dlg);
		m_pSysSettingDlg->CenterWindow(m_pRoomParentDlg);
		m_pSysSettingDlg->ShowWindow(SW_SHOW);
	}
}

void CSelfInfoAndSetDlg::OnClickRoomManager()
{
	SwitchResourceToModule(_T("Room"));
	if (m_pRoomManagerDlg == NULL)
	{
		//创建房间管理窗口
		m_pRoomManagerDlg = new CRoomManagerDlg(m_pRoomParentDlg);
		if (m_pRoomManagerDlg)
		{
			m_pRoomManagerDlg->Create(CRoomManagerDlg::IDD,this);
			m_pRoomManagerDlg->SetWindowPos(NULL, 0, 0, 754, 519, SWP_SHOWWINDOW);
			m_pRoomManagerDlg->CenterWindow(m_pRoomParentDlg);
			return;
		}
	}

	if (m_pRoomManagerDlg != NULL && m_pRoomManagerDlg->GetSafeHwnd())
	{
		m_pRoomManagerDlg->ShowWindow(SW_SHOW);
		m_pRoomManagerDlg->CenterWindow(m_pRoomParentDlg);
		m_pRoomManagerDlg->SetForegroundWindow();
	}
}

//创建设置对话框
void CSelfInfoAndSetDlg::CreateSysSettingDlg()
{
	if(m_pSysSettingDlg == NULL)
	{
		//创建菜单设置
		m_pSysSettingDlg = new CSysSettingDlg(m_pRoomParentDlg);
		SwitchResourceToModule(_T("Room"));
		m_pSysSettingDlg->Create(CSysSettingDlg::IDD,this);
		m_pSysSettingDlg->SetToSysSetting();		
		m_pSysSettingDlg->ShowWindow(SW_HIDE);
	}
}

void CSelfInfoAndSetDlg::ShowSysSetting(SysSettingDlgType enmType)
{
	CreateSysSettingDlg();	
	if(m_pSysSettingDlg)
	{
		m_pSysSettingDlg->OpenDlg(enmType);
		m_pSysSettingDlg->CenterWindow(m_pRoomParentDlg);
		m_pSysSettingDlg->ShowWindow(SW_SHOW);
	}
}

void CSelfInfoAndSetDlg::OnAddUser( IUserItem * pUserItem )
{
	ASSERT(pUserItem != NULL);
	//是自己
	if (pUserItem->GetUserUIN() == GetCurInfo().unUIN)
	{
		if (pUserItem->GetUserTitleLevel() >= core::TITLELEVEL_MANAGER)
		{
			CComVariant var = FALSE;
			m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(), L"btnRoomManager", L"disabled", var);
		}
	}
}

void CSelfInfoAndSetDlg::OnUpdateUser( IUserItem * pUserItem )
{
	ASSERT(pUserItem != NULL);
	if (pUserItem->GetUserUIN() == GetCurInfo().unUIN)
	{
		CComVariant var = !(pUserItem->GetUserTitleLevel() >= core::TITLELEVEL_MANAGER);
		m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(), L"btnRoomManager", L"disabled", var);
	}
}

void CSelfInfoAndSetDlg::SetUserName( IUserItem* pUserItem )
{
	m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(), L"textUserName", L"text", (CComVariant)pUserItem->GetUserName().c_str());
}

void CSelfInfoAndSetDlg::OnClickOnOffLine()
{
	CRect rcBtnStatus(0, 0, 0, 0);
	m_pSkinEngine->GetSubSkinPos(GetSafeHwnd(), L"btnStatus", &(rcBtnStatus.left), &(rcBtnStatus.top), &(rcBtnStatus.right), &(rcBtnStatus.bottom));
	CPoint pt(rcBtnStatus.left, rcBtnStatus.bottom);
	ClientToScreen(&pt);

	common::ui::CMenuEx* pm = new common::ui::CMenuEx();
	pm->CreatePopupMenu();
	pm->SetWidth(80);

	CImageList *pImgList = new CImageList;
	pImgList->Create(11, 11, TRUE | ILC_COLOR24, 16, 0);
	pm->SetImageList(pImgList);
	if (pImgList==NULL)
	{
		ASSERT(0);
		delete pm;
		return;
	}
	AutoReplyMenu autoReplyMenu(this) ;
	SwitchResourceToModule(_T("Room"));

	int iIndex = pImgList->Add(AfxGetApp()->LoadIcon(IDI_STATUS_ONLINE));
	pm->AppendMenuEx(0, ID_STATUS_ONLINE, L"在线", iIndex);
	iIndex = pImgList->Add(AfxGetApp()->LoadIcon(IDI_STATUS_LEAVE));
	pm->AppendMenuEx(MF_POPUP, (UINT)autoReplyMenu.GetAutoReplyMenu()->GetSafeHmenu(), L"离开",  iIndex);

	int nReturn = pm->TrackPopupMenu(TPM_LEFTALIGN | TPM_RETURNCMD, pt.x, pt.y, this);
	SendMessage(WM_CLICK_AUTOREPLY_MENU, 0, nReturn);
	autoReplyMenu.DeleteReplyMenu();
	pm->DestroyMenu();
	delete pm;
	pm = NULL;
}

LRESULT CSelfInfoAndSetDlg::OnClickBtnAutoReplyMsg( WPARAM wParam, LPARAM lParam )
{
	int nIndex = lParam ;
	if (nIndex == 0)
	{
		return S_FALSE;
	}
	core::CurInfo myCurInfo = GetCurInfo() ;
	if (myCurInfo.enmVIPLevel == core::VIPLEVEL_REGISTER)//注册用户
	{
		C179MsgBox::Show(this, NewMsgBox::MSGBOX_OK_ONE_STRING_WITH_LINK, L"只有VIP以上用户才能设置自动回复", L"", L"", L"马上升级VIP", m_pRoomParentDlg->GetRoomBusiness()->GetUpdateVipUrl().GetBuffer(0));
		return S_FALSE;
	} 
	if (myCurInfo.enmVIPLevel == core::VIPLEVEL_NONE)//游客
	{
		C179MsgBox::Show(this, NewMsgBox::MSGBOX_GUEST_OPERATE, L"游客不能设置自动回复，", L"想要畅玩白兔KTV？", L"", L"快去注册吧", m_pRoomParentDlg->GetRoomBusiness()->GetRegisteUrl().GetBuffer());
		return S_FALSE;
	}

	if (ID_STATUS_ONLINE == nIndex)
	{
		OnMenuOnline();
	}
	else
	{
		if (nIndex == SET_AUTOREPLY_MENU)//点击自定义
		{	
			SwitchResourceToModule(_T("Room"));
			CAutoReplySettingDlg pDlg;
			if (IDOK == pDlg.DoModal())
			{
				if (pDlg.GetAutoReplyChange())
				{
					AutoReplyMgr->UpdateAutoReplyList(pDlg.GetNewAutoReply());
				}
				if (pDlg.GetAutoReplyState())//开启自动回复
				{
					if (pDlg.GetSelectedItem() < 0)
					{
						AutoReplyMgr->SetAutoReply(false);
						g_RoomModule->UpdateAllRoomStatus(TRUE);
					}else
					{
						AutoReplyMgr->SetAutoReply(true);
						AutoReplyMgr->UpdateAutoReplyList(pDlg.GetNewAutoReply());
						g_RoomModule->UpdateAllRoomStatus(FALSE);
					}
				}else
				{
					AutoReplyMgr->SetAutoReply(false);
				}
			}
		}
		else if (nIndex == CANCEL_AUTOREPLY_MENU)//点击无回复消息
		{
			AutoReplyMgr->SetAutoReply(FALSE);
			g_RoomModule->UpdateAllRoomStatus(TRUE);
		}
		else //选择了自动回复的语句
		{
			stAutoReplyMenuItem stItem;
			AutoReplyMgr->SetCheckedByIndex(nIndex - 1);
			g_RoomModule->UpdateAllRoomStatus(FALSE);
		}
	}

	return S_OK;
}

void CSelfInfoAndSetDlg::OnMenuOnline()//点击在线
{
	AutoReplyMgr->SetAutoReply(FALSE);
	g_RoomModule->UpdateAllRoomStatus(TRUE);
}

void CSelfInfoAndSetDlg::SetMyStatus(BOOL bOnLine)
{
	if (bOnLine)//设置为在线状态
	{
		CComVariant var = L"RoomV3_btnIconStatusOnLine.png";
		m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(), L"btnStatus", L"iconImage", var);
		var = L"在线";
		m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(), L"btnStatus", L"tip", var);
	}else
	{
		CComVariant var = L"RoomV3_btnIconStatusLeave.png";
		m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(), L"btnStatus", L"iconImage", var);
		var = L"离开";
		m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(), L"btnStatus", L"tip", var);
	}

}

void CSelfInfoAndSetDlg::CloseMic()
{
	if ( ( m_pRoomParentDlg->GetRoomAvLogic() ) != NULL )
	{
		CComVariant var = false;
		m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(),L"imgMicOn",L"visible",var);
		var = true;
		m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(),L"imgMicOff",L"visible",var);
		m_pRoomParentDlg->GetRoomAvLogic() ->SetAllMicMute( true ) ;
	}
}

void CSelfInfoAndSetDlg::OpenMic()
{
	if ( ( m_pRoomParentDlg->GetRoomAvLogic() ) != NULL )
	{
		CComVariant var = true;
		m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(),L"imgMicOn",L"visible",var);
		var = false;
		m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(),L"imgMicOff",L"visible",var);
		 ( m_pRoomParentDlg->GetRoomAvLogic() )->SetAllMicMute( false ) ;
	}
}

void CSelfInfoAndSetDlg::OnClickBtnAudioSet()
{
	CRect rcBtnAudioSet(0, 0, 0, 0);
	m_pSkinEngine->GetSubSkinPos(GetSafeHwnd(), L"btnAudioSet", &(rcBtnAudioSet.left), &(rcBtnAudioSet.top), &(rcBtnAudioSet.right), &(rcBtnAudioSet.bottom));
	CPoint pt(rcBtnAudioSet.left, rcBtnAudioSet.bottom);
	ClientToScreen(&pt);

	common::ui::CMenuEx* pm = new common::ui::CMenuEx();
	common::ui::CMenuEx* pmSub = new common::ui::CMenuEx();
	pm->CreatePopupMenu();
	pmSub->CreatePopupMenu();
	pm->SetWidth(80);
	pmSub->SetWidth(80);

	EnMixerControlMode enmMode = (EnMixerControlMode)(CPersonalConfig::GetInstance()->GetOnMicPurpose());

	if (enmMode == MIXER_CONTROL_SING || enmMode == MIXER_CONTROL_OTHER)
	{
		pmSub->AppendMenuEx(MF_STRING | MF_CHECKED, 1, L"唱歌");
	}
	else
	{
		pmSub->AppendMenuEx(MF_STRING, 1, L"唱歌");
	}
	if (enmMode == MIXER_CONTROL_CHAT)
	{
		pmSub->AppendMenuEx(MF_STRING | MF_CHECKED, 2, L"聊天");
	}
	else
	{
		pmSub->AppendMenuEx(MF_STRING, 2, L"聊天");
	}
	if (enmMode == MIXER_CONTROL_PLAY)
	{
		pmSub->AppendMenuEx(MF_STRING | MF_CHECKED, 3, L"放歌");
	}
	else
	{
		pmSub->AppendMenuEx(MF_STRING, 3, L"放歌");
	}

	pm->AppendMenuEx(MF_POPUP, (UINT)pmSub->GetSafeHmenu(), L"音频模式");
	pm->AppendMenuEx(MF_STRING, ID_AUDIOSET_TEST, L"音频测试");
	pm->AppendMenuEx(MF_STRING, ID_AUDIOSET_SETTING, L"音频设置");

	int nReturn = pm->TrackPopupMenu(TPM_LEFTALIGN | TPM_RETURNCMD, pt.x, pt.y, this);

	SendMessage(WM_CLICK_AUDIOSET_MENU, 0, nReturn);

	pmSub->DestroyMenu();
	delete pmSub;
	pmSub = NULL;

	pm->DestroyMenu();
	delete pm;
	pm = NULL;
}

LRESULT CSelfInfoAndSetDlg::OnClickMenuAudioSet( WPARAM wParam, LPARAM lParam )
{
	int nIndex = lParam;
	if (nIndex == ID_AUDIOSET_TEST)//点了音频测试
	{
		ShowAudioTest();
	}
	else if(nIndex == ID_AUDIOSET_SETTING)//点了音频设置
	{
		ShowSysSetting(Show_Sound_Setting_Dlg);
	}
	else if (nIndex == 1)//唱歌
	{
		CPersonalConfig::GetInstance()->SetOnMicPurpose(MIXER_CONTROL_SING);
		( m_pRoomParentDlg->GetRoomAvLogic() )->SetMixerControlSingMode(true);
	}
	else if (nIndex == 2)//聊天
	{
		CPersonalConfig::GetInstance()->SetOnMicPurpose(MIXER_CONTROL_CHAT);
		( m_pRoomParentDlg->GetRoomAvLogic() )->SetMixerControlChatMode(true);
	}
	else if (nIndex == 3)//放歌
	{
		CPersonalConfig::GetInstance()->SetOnMicPurpose(MIXER_CONTROL_PLAY);
		( m_pRoomParentDlg->GetRoomAvLogic() )->SetMixerControlPlayMode(true);
	}

	return S_OK;
}

void CSelfInfoAndSetDlg::ShowAudioTest()
{
	if (m_pAudioTest == NULL)
	{
		SwitchResourceToModule(_T("Room"));
		m_pAudioTest = new CShowAudioTestDlg;
		m_pAudioTest->Create(CShowAudioTestDlg::IDD,NULL);
		m_pAudioTest->CenterWindow(m_pRoomParentDlg);
		m_pAudioTest->ShowWindow(SW_SHOW);
	}
	else
	{
		if(m_pAudioTest->GetSafeHwnd() && (!m_pAudioTest->IsWindowVisible()))
			m_pAudioTest->ShowWindow(SW_SHOW);
	}
}

void CSelfInfoAndSetDlg::OnMyHeadUpdate()
{
	Hall::HeadImage headImage;
	headImage.m_bMan = GetCurInfo().bMan;
	headImage.m_unUIN = GetCurInfo().unUIN;
	headImage.m_isGuest = (GetCurInfo().enmVIPLevel == core::VIPLEVEL_NONE ? true : false);
	g_RoomModule->m_pModuleManager->CallService(Hall::SERVICE_VALUE_HALL_GET_HEAD_IMAGE_PATH, (param)&headImage);
	m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(), L"imgMyHead", L"absoluteImage", (CComVariant)(CString)headImage.m_HeadImagePath.c_str());
}

void CSelfInfoAndSetDlg::OnClickBtnCloseSound()
{
	if ( ( m_pRoomParentDlg->GetRoomAvLogic() ) != NULL )
	{
		CComVariant var = false;
		m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(),L"imgSpeakerOn",L"visible",var);
		var = true;
		m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(),L"imgSpeakerOff",L"visible",var);
		( m_pRoomParentDlg->GetRoomAvLogic() )->SetSondMute( true ) ;
	}
}

void CSelfInfoAndSetDlg::OnClickBtnOpenSound()
{
	if ( ( m_pRoomParentDlg->GetRoomAvLogic() )  != NULL )
	{
		CComVariant var = true;
		m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(),L"imgSpeakerOn",L"visible",var);
		var = false;
		m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(),L"imgSpeakerOff",L"visible",var);
		( m_pRoomParentDlg->GetRoomAvLogic() )->SetSondMute( false ) ;
	}
}

void CSelfInfoAndSetDlg::InitSlider()
{
	if (( m_pRoomParentDlg->GetRoomAvLogic() )  == NULL)
	{
		return;
	}
	if (m_pMicSliderDlg == NULL || m_pSpeakerSliderDlg == NULL)
	{
		return;
	}

	m_pSpeakerSliderDlg->SetRange( 0 , 255 ,TRUE) ;
	m_pSpeakerSliderDlg->SetPageSize(25);
	m_pMicSliderDlg->SetRange( 0 , 255 ,TRUE) ;
	m_pMicSliderDlg->SetPageSize(25);
	int speaker = CPersonalConfig::GetInstance()->GetRoomSpeakerVolume();
	int mic = CPersonalConfig::GetInstance()->GetRoomMicVolume();
	m_pRoomParentDlg->GetRoomAvLogic()->SetSpeakerRoomVolume(speaker);
	m_pRoomParentDlg->GetRoomAvLogic()->SetMicRoomAllVolume(mic);
	m_pSpeakerSliderDlg->SetPos(speaker);
	m_pMicSliderDlg->SetPos(mic);

}

LRESULT CSelfInfoAndSetDlg::OnVScrollSlider( WPARAM wParam ,LPARAM lParam )
{
	if((CVeritySliderDlg*)wParam == m_pMicSliderDlg)
	{
		int mic = m_pMicSliderDlg->GetPos();
		if ( m_pRoomParentDlg->GetRoomAvLogic() != NULL)
		{
			m_pRoomParentDlg->GetRoomAvLogic()->SetMicRoomAllVolume(mic);
		}
		m_pRoomParentDlg->GetRoomBusiness()->UpdateAllRoomMic(mic);
		CPersonalConfig::GetInstance()->SetRoomMicVolume(mic);
	}else if ((CVeritySliderDlg*)wParam == m_pSpeakerSliderDlg)
	{
		int speaker = m_pSpeakerSliderDlg->GetPos();
		if ( m_pRoomParentDlg->GetRoomAvLogic() != NULL )
		{
			m_pRoomParentDlg->GetRoomAvLogic()->SetSpeakerRoomVolume(speaker);
		}
		CPersonalConfig::GetInstance()->SetRoomSpeakerVolume(speaker);
	}

	return S_OK;
}

void CSelfInfoAndSetDlg::SetRoomMicVolume( uint32 unMicVolume )
{
	m_pMicSliderDlg->SetPos( unMicVolume ) ;
}

LRESULT CSelfInfoAndSetDlg::OnClickMsgBoxGoToRegister( WPARAM wParam, LPARAM lParam )
{
	if ((CSelfInfoAndSetDlg*)wParam == this)
	{
		m_pRoomParentDlg->OnClickGotoRegister();
	}

	return TRUE;
}