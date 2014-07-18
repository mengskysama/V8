// CShowMessageSettingDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "..\Room.h"
#include "ShowMessageSettingDlg.h"
#include "..\personalsetting/PersonalConfig.h"
#include "skin_i.h"
#include "SkinUtil.h"

// CShowMessageSettingDlg 对话框

IMPLEMENT_DYNAMIC(CShowMessageSettingDlg, CDialog)

CShowMessageSettingDlg::CShowMessageSettingDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CShowMessageSettingDlg::IDD, pParent)
{
	m_pSkinEngine = NULL;
	::GetSkinEngine(&m_pSkinEngine);
}

CShowMessageSettingDlg::~CShowMessageSettingDlg()
{
}

void CShowMessageSettingDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CShowMessageSettingDlg, CDialog)
	ON_SKIN_MESSAGE
	ON_WM_CREATE()
	ON_BN_CLICKED(IDC_ATROOM_RADIO_JUST_HIGHER_VIP, &CShowMessageSettingDlg::OnBnClickedCheck)
	ON_BN_CLICKED(IDC_ATROOM_RADIO_JUST_HIGHER_HUANGGUAN, &CShowMessageSettingDlg::OnBnClickedCheck)
	ON_BN_CLICKED(IDC_ATROOM_RADIO_REFUSE_ALL_PEOPLE, &CShowMessageSettingDlg::OnBnClickedCheck)
	ON_BN_CLICKED(IDC_ATROOM_CHECK_FORBID_GIFT_MSG, &CShowMessageSettingDlg::OnBnClickedCheck)
	ON_BN_CLICKED(IDC_ATROOM_CHECK_OPEN_SPEAKER_SCROLL, &CShowMessageSettingDlg::OnBnClickedCheck)
	ON_BN_CLICKED(IDC_ATROOM_CHECK_FORBID_BIG_GIFT_SHOW, &CShowMessageSettingDlg::OnBnClickedCheck)
	ON_BN_CLICKED(IDC_ATROOM_CHECK_FORBID_SPEAKER_SOUND, &CShowMessageSettingDlg::OnBnClickedCheck)
	ON_BN_CLICKED(IDC_ATROOM_CHECK_FORBID_PRIVATE_TALK, &CShowMessageSettingDlg::OnBnClickedCheck)
	ON_BN_CLICKED(IDC_ATROOM_CHECK_FORBID_PUBLIC_TALK, &CShowMessageSettingDlg::OnBnClickedCheck)
	ON_BN_CLICKED(IDC_ATROOM_CHECK_OPEN_MSG_SCROLL, &CShowMessageSettingDlg::OnBnClickedCheck)
	ON_BN_CLICKED(IDC_ATROOM_CHECK_FORBID_USER_IN_OUT, &CShowMessageSettingDlg::OnBnClickedCheck)
	ON_BN_CLICKED(IDC_ATROOM_CHECK_FORBID_LUCKY_SOUND, &CShowMessageSettingDlg::OnBnClickedCheck)
	ON_WM_SHOWWINDOW()
END_MESSAGE_MAP()


// CShowMessageSettingDlg 消息处理程序

int CShowMessageSettingDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  在此添加您专用的创建代码
	ASSERT(NULL != m_pSkinEngine);
	if (NULL != m_pSkinEngine)
	{
		HRESULT hr = m_pSkinEngine->LoadSkin(GetSafeHwnd(),L"ShowMessageSettingDlg_AtRoom");
		ASSERT(hr== S_OK && __LINE__);
	}

	return 0;
}

LRESULT CShowMessageSettingDlg::OnSkinMessage(WPARAM wParam,LPARAM lParam)
{


	return TRUE;
}

void CShowMessageSettingDlg::OnBnClickedCheck()
{
	// TODO: 在此添加控件通知处理程序代码
	::SendMessage(GetParent()->GetSafeHwnd(),WM_NOTIFY_ROOM_SYSSET_MODIFY,0,0); //给给父窗口(菜单设置)发送设置对话框有修改的消息

}

void CShowMessageSettingDlg::OnOK()
{
	// TODO: 在此添加专用代码和/或调用基类

//	CDialog::OnOK();
}

void CShowMessageSettingDlg::OnCancel()
{
	// TODO: 在此添加专用代码和/或调用基类

//	CDialog::OnCancel();
}

BOOL CShowMessageSettingDlg::PreTranslateMessage(MSG* pMsg)
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

void CShowMessageSettingDlg::Init()
{
	ENM_P2PPermission nP2PPermission  = CPersonalConfig::GetInstance()->GetP2PPermission() ;
	switch( nP2PPermission )
	{
	case P2P_PERMISSION_REFUSE_ANYONE:
		{
			((CButton*)GetDlgItem(IDC_ATROOM_RADIO_JUST_HIGHER_VIP))->SetCheck( FALSE ) ;
			((CButton*)GetDlgItem(IDC_ATROOM_RADIO_JUST_HIGHER_HUANGGUAN))->SetCheck( FALSE ) ;
			((CButton*)GetDlgItem(IDC_ATROOM_RADIO_REFUSE_ALL_PEOPLE))->SetCheck( TRUE ) ;
		}
		break;
	case P2P_PERMISSION_ABOVE_VIP:
		{
			((CButton*)GetDlgItem(IDC_ATROOM_RADIO_JUST_HIGHER_VIP))->SetCheck( TRUE ) ;
			((CButton*)GetDlgItem(IDC_ATROOM_RADIO_JUST_HIGHER_HUANGGUAN))->SetCheck( FALSE ) ;
			((CButton*)GetDlgItem(IDC_ATROOM_RADIO_REFUSE_ALL_PEOPLE))->SetCheck( FALSE ) ;
		}
		break;
	case P2P_PERMISSION_ABOVE_CROWN:
		{
			((CButton*)GetDlgItem(IDC_ATROOM_RADIO_JUST_HIGHER_VIP))->SetCheck( FALSE ) ;
			((CButton*)GetDlgItem(IDC_ATROOM_RADIO_JUST_HIGHER_HUANGGUAN))->SetCheck( TRUE ) ;
			((CButton*)GetDlgItem(IDC_ATROOM_RADIO_REFUSE_ALL_PEOPLE))->SetCheck( FALSE ) ;
		}
		break;
	default:
		{
			((CButton*)GetDlgItem(IDC_ATROOM_RADIO_JUST_HIGHER_VIP))->SetCheck( TRUE ) ;
			((CButton*)GetDlgItem(IDC_ATROOM_RADIO_JUST_HIGHER_HUANGGUAN))->SetCheck( FALSE ) ;
			((CButton*)GetDlgItem(IDC_ATROOM_RADIO_REFUSE_ALL_PEOPLE))->SetCheck( FALSE ) ;
		}
		break;
	}
	if (GetCurInfo().enmVIPLevel < core::VIPLEVEL_VIP)
	{
		((CButton*)GetDlgItem(IDC_ATROOM_RADIO_JUST_HIGHER_VIP))->EnableWindow(FALSE);
		((CButton*)GetDlgItem(IDC_ATROOM_RADIO_JUST_HIGHER_HUANGGUAN))->EnableWindow(FALSE);
		((CButton*)GetDlgItem(IDC_ATROOM_RADIO_REFUSE_ALL_PEOPLE))->EnableWindow(FALSE);
	}

	if (CPersonalConfig::GetInstance()->GetIsBlockPublicMsg())
	{
		((CButton*)GetDlgItem(IDC_ATROOM_CHECK_FORBID_PUBLIC_TALK))->SetCheck(TRUE);
	}
	else
	{
		((CButton*)GetDlgItem(IDC_ATROOM_CHECK_FORBID_PUBLIC_TALK))->SetCheck(FALSE);
	}

	if (CPersonalConfig::GetInstance()->GetIsBlockPrivateMsg())
	{
		((CButton*)GetDlgItem(IDC_ATROOM_CHECK_FORBID_PRIVATE_TALK))->SetCheck(TRUE);
	}
	else
	{
		((CButton*)GetDlgItem(IDC_ATROOM_CHECK_FORBID_PRIVATE_TALK))->SetCheck(FALSE);
	}

	if (GetCurInfo().enmVIPLevel < core::VIPLEVEL_VIP)
	{
		((CButton*)GetDlgItem(IDC_ATROOM_CHECK_FORBID_LUCKY_SOUND))->EnableWindow(FALSE);
	}

	if (CPersonalConfig::GetInstance()->GetIsBlockLuckySound())
	{
		((CButton*)GetDlgItem(IDC_ATROOM_CHECK_FORBID_LUCKY_SOUND))->SetCheck(TRUE);
	}
	else
	{
		((CButton*)GetDlgItem(IDC_ATROOM_CHECK_FORBID_LUCKY_SOUND))->SetCheck(FALSE);
	}


	/*if (CPersonalConfig::GetInstance()->GetIsAutoReply())
	{
		((CButton*)GetDlgItem(IDC_CHECK_AUTORESPONSE))->SetCheck(TRUE);
	}
	else
	{
		((CButton*)GetDlgItem(IDC_CHECK_AUTORESPONSE))->SetCheck(FALSE);
	}*/
	if (GetCurInfo().enmVIPLevel < core::VIPLEVEL_VIP)
	{
		((CButton*)GetDlgItem(IDC_ATROOM_CHECK_FORBID_SPEAKER_SOUND))->EnableWindow(FALSE);
	}

	if (CPersonalConfig::GetInstance()->GetIsBlockSpeakerSound())
	{
		((CButton*)GetDlgItem(IDC_ATROOM_CHECK_FORBID_SPEAKER_SOUND))->SetCheck(TRUE);
	}
	else
	{
		((CButton*)GetDlgItem(IDC_ATROOM_CHECK_FORBID_SPEAKER_SOUND))->SetCheck(FALSE);
	}

	if (CPersonalConfig::GetInstance()->GetIsBlockGiftMsg())
	{
		((CButton*)GetDlgItem(IDC_ATROOM_CHECK_FORBID_GIFT_MSG))->SetCheck(TRUE);
	}
	else
	{
		((CButton*)GetDlgItem(IDC_ATROOM_CHECK_FORBID_GIFT_MSG))->SetCheck(FALSE);
	}

	if (CPersonalConfig::GetInstance()->GetIsAutoScrollSpeaker())
	{
		((CButton*)GetDlgItem(IDC_ATROOM_CHECK_OPEN_SPEAKER_SCROLL))->SetCheck(TRUE);
	}
	else
	{
		((CButton*)GetDlgItem(IDC_ATROOM_CHECK_OPEN_SPEAKER_SCROLL))->SetCheck(FALSE);
	}

	if (CPersonalConfig::GetInstance()->GetIsBlockUserInOutMsg())
	{
		((CButton*)GetDlgItem(IDC_ATROOM_CHECK_FORBID_USER_IN_OUT))->SetCheck(TRUE);
	}
	else
	{
		((CButton*)GetDlgItem(IDC_ATROOM_CHECK_FORBID_USER_IN_OUT))->SetCheck(FALSE);
	}

}


LRESULT CShowMessageSettingDlg::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	switch(message)
	{
	case WM_NOTIFY_ROOM_SYSSET_CLICKOK:  //“确定”按扭的消息		
	case WM_NOTIFY_ROOM_SYSSET_CLICKAPPLI:  // "应用"按扭的消息
		{
			Confirm();
		}
		break;	
	default:
		break;
	}

	return CDialog::WindowProc(message, wParam, lParam);
}

void CShowMessageSettingDlg::Confirm()
{
	if ( ((CButton*)GetDlgItem(IDC_ATROOM_RADIO_JUST_HIGHER_VIP))->GetCheck() == TRUE )
	{
		CPersonalConfig::GetInstance()->SetP2PPermission( P2P_PERMISSION_ABOVE_VIP ) ;
	}
	else if ( ((CButton*)GetDlgItem(IDC_ATROOM_RADIO_JUST_HIGHER_HUANGGUAN))->GetCheck() == TRUE )
	{
		CPersonalConfig::GetInstance()->SetP2PPermission( P2P_PERMISSION_ABOVE_CROWN ) ;
	}
	else if ( ((CButton*)GetDlgItem(IDC_ATROOM_RADIO_REFUSE_ALL_PEOPLE))->GetCheck() == TRUE )
	{
		CPersonalConfig::GetInstance()->SetP2PPermission( P2P_PERMISSION_REFUSE_ANYONE ) ;
	}

	if (((CButton*)GetDlgItem(IDC_ATROOM_CHECK_FORBID_PUBLIC_TALK))->GetCheck() == TRUE)
	{
		CPersonalConfig::GetInstance()->SetIsBlockPublicMsg(true);
	}
	else
	{
		CPersonalConfig::GetInstance()->SetIsBlockPublicMsg(false);
	}

	if (((CButton*)GetDlgItem(IDC_ATROOM_CHECK_FORBID_PRIVATE_TALK))->GetCheck() == TRUE)
	{
		CPersonalConfig::GetInstance()->SetIsBlockPrivateMsg(true);
	}
	else
	{
		CPersonalConfig::GetInstance()->SetIsBlockPrivateMsg(false);
	}


	//if (((CButton*)GetDlgItem(IDC_CHECK_AUTORESPONSE))->GetCheck() == TRUE)
	//{
	//	CPersonalConfig::GetInstance()->SetIsAutoReply(true);
	//}
	//else
	//{
	//	CPersonalConfig::GetInstance()->SetIsAutoReply(false);
	//}

	if (((CButton*)GetDlgItem(IDC_ATROOM_CHECK_FORBID_SPEAKER_SOUND))->GetCheck() == TRUE)
	{
		CPersonalConfig::GetInstance()->SetIsBlockSpeakerSound(true);
	}
	else
	{
		CPersonalConfig::GetInstance()->SetIsBlockSpeakerSound(false);
	}

	if (((CButton*)GetDlgItem(IDC_ATROOM_CHECK_FORBID_GIFT_MSG))->GetCheck() == TRUE)
	{
		CPersonalConfig::GetInstance()->SetIsBlockGiftMsg(true);
	}
	else
	{
		CPersonalConfig::GetInstance()->SetIsBlockGiftMsg(false);
	}


	if (((CButton*)GetDlgItem(IDC_ATROOM_CHECK_FORBID_USER_IN_OUT))->GetCheck() == TRUE)
	{
		CPersonalConfig::GetInstance()->SetIsBlockUserInOutMsg(true);
	}
	else
	{
		CPersonalConfig::GetInstance()->SetIsBlockUserInOutMsg(false);
	}

	if (((CButton*)GetDlgItem(IDC_ATROOM_CHECK_FORBID_LUCKY_SOUND))->GetCheck() == TRUE)
	{
		CPersonalConfig::GetInstance()->SetIsBlockLuckySound(true);
	}
	else
	{
		CPersonalConfig::GetInstance()->SetIsBlockLuckySound(false);
	}
}

BOOL CShowMessageSettingDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	Init();

	return TRUE;  // return TRUE unless you set the focus to a control
}

void CShowMessageSettingDlg::Refresh()
{
	Init();
}
void CShowMessageSettingDlg::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialog::OnShowWindow(bShow, nStatus);

	if (bShow)
	{
		Refresh();
	}
}

void CShowMessageSettingDlg::SetApplyEffect(BOOL bEffect)
{
	CComVariant var = bEffect;
	m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(), L"imgApplyEffect", L"visible", var);
}