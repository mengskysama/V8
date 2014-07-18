// ShowAudioSettingDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "..\Room.h"
#include "ShowAudioSettingDlg.h"
#include "skin_i.h"
#include "SkinUtil.h"
#include "utility/SystemHelper.h"
#include "ui\C179MsgBox.h"
#include "utility/ConfigManager.h"
#include "utility/StringHelper.h"
#include "..\personalsetting/PersonalConfig.h"
#include "..\personalsetting/VideoManager.h"

// CShowAudioSettingDlg 对话框

#define  SKIN_NOTIFY_AUDIO_DLG_HWND     1
#define  SKIN_ON_CLICK_BTN_REFRESH      2
#define  SKIN_ON_CLICK_BTN_PIC_QUALITY  3

IMPLEMENT_DYNAMIC(CShowAudioSettingDlg, CDialog)

CShowAudioSettingDlg::CShowAudioSettingDlg(CWnd* pParent /*=NULL*/)
: CDialog(CShowAudioSettingDlg::IDD, pParent)
{
	m_pAudioDlg = NULL;
	m_pSkinEngine = NULL;
	::GetSkinEngine(&m_pSkinEngine);
}

CShowAudioSettingDlg::~CShowAudioSettingDlg()
{
}

void CShowAudioSettingDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_ATROOM_COMBO_CAMERA, m_VideoList);
}


BEGIN_MESSAGE_MAP(CShowAudioSettingDlg, CDialog)

	ON_SKIN_MESSAGE
	ON_WM_CREATE()
	ON_WM_SHOWWINDOW()
	ON_CBN_SELCHANGE(IDC_ATROOM_COMBO_CAMERA, &CShowAudioSettingDlg::OnCbnSelchangeComboCamera)
END_MESSAGE_MAP()


// CShowAudioSettingDlg 消息处理程序

int CShowAudioSettingDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  在此添加您专用的创建代码
	ASSERT(NULL != m_pSkinEngine);
	if (NULL != m_pSkinEngine)
	{
		HRESULT hr = m_pSkinEngine->LoadSkin(GetSafeHwnd(),L"ShowAudioSettingDlg_AtRoom");
		ASSERT(hr== S_OK && __LINE__);
	}
	//创建视频子对话框
	SwitchResourceToModule(_T("Room"));
	m_pAudioDlg = new CVideoSettingDlg;
	if (m_pAudioDlg)
	{
		m_pAudioDlg->Create(CVideoSettingDlg::IDD,this);
		m_pAudioDlg->SetWindowPos(NULL,35,300,216,165,SWP_SHOWWINDOW);
		m_pAudioDlg->ShowWindow(SW_SHOW);
	}

	return 0;
}


LRESULT CShowAudioSettingDlg::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
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

void CShowAudioSettingDlg::Confirm()
{
	core::CurInfo m_MyCurInfo = GetCurInfo();
	CString curAudio;
	int nCurSel = m_VideoList.GetCurSel();
	VInfo stCurInfo;
	if (!VideoManager::GetInstance()->GetVideoInfoByIndex(nCurSel, stCurInfo))
	{
		ASSERT(0);
	}
	CString sCurAudio,sOldAudio;
	sCurAudio.Format(_T("%s"),stCurInfo.strGUID.c_str());
	CConfigManager::Instance()->GetUserInfo_Setting(m_MyCurInfo.unUIN,USERINFO_PERSONALSETTING_USEVIDEODEVICE,sOldAudio);
	if(sCurAudio != sOldAudio)
	{
		CConfigManager::Instance()->SetUserInfo_Setting(m_MyCurInfo.unUIN,USERINFO_PERSONALSETTING_USEVIDEODEVICE,sCurAudio);
		if (!stCurInfo.bVirtual)  //是否为虚拟视频
		{
			CPersonalConfig::GetInstance()->SetIsUseVirtualCam(false);
		}
		else
		{
			CPersonalConfig::GetInstance()->SetIsUseVirtualCam(true);
		}

		CPersonalConfig::GetInstance()->SetUseVideoDevice(stCurInfo.strGUID);
		CPersonalConfig::GetInstance()->SetUseVideoDeviceIndex(stCurInfo.nDeviceID);
	}	

}

void CShowAudioSettingDlg::Init()
{
	core::CurInfo m_MyCurInfo = GetCurInfo();		

	VecVideoInfo vecVideoList = VideoManager::GetInstance()->GetVideoList();

	m_VideoList.ResetContent();
	for (int i = 0; i < vecVideoList.size(); i++)
	{
		m_VideoList.AddString(vecVideoList[i].strName.c_str());
	}

	if(vecVideoList.size() > 0)
	{
		CString sUserVideo;
		CConfigManager::Instance()->GetUserInfo_Setting(m_MyCurInfo.unUIN,USERINFO_PERSONALSETTING_USEVIDEODEVICE,sUserVideo);
		wstring wCurVideo = sUserVideo;

		VInfo stInfo;
		if (VideoManager::GetInstance()->GetVideoInfoByGUID(wCurVideo, stInfo))
		{
			m_VideoList.SetCurSel(stInfo.nIndex);
			m_pAudioDlg->StartVideo(stInfo.nDeviceID);
		}
		else
		{
			VideoManager::GetInstance()->GetVideoInfoByIndex(0, stInfo);
			m_VideoList.SetCurSel(0);
			m_pAudioDlg->StartVideo(stInfo.nDeviceID);
		}

		CComVariant var = stInfo.bVirtual?L"true":L"false";  //是否为虚拟视频
		m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(), L"btnPicQuality", L"disabled", var);  //频调节面板是否可用

	}
	else
	{
		m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(), L"btnPicQuality", L"disabled", (CComVariant)true);  //频调节面板是否可用
	}

}

void CShowAudioSettingDlg::OnShowWindow(BOOL bShow,UINT nStatus)
{
	CDialog::OnShowWindow(bShow, nStatus);

	if(bShow)  //显示视频
	{	
		Init();		
	}
	else  //关闭视频
	{
		CloseVideoSet();

	}

}


LRESULT CShowAudioSettingDlg::OnSkinMessage(WPARAM wParam,LPARAM lParam)
{
	switch(wParam)
	{
	case SKIN_ON_CLICK_BTN_REFRESH:
		{
			OnClickBtnRefresh();
		}
		break;
	case SKIN_ON_CLICK_BTN_PIC_QUALITY:
		{
			OnClickBtnPicQuality();
		}
		break;
	default:break;
	}

	return TRUE;
}


void CShowAudioSettingDlg::OnOK()
{
	// TODO: 在此添加专用代码和/或调用基类

	//	CDialog::OnOK();
}

void CShowAudioSettingDlg::OnCancel()
{
	// TODO: 在此添加专用代码和/或调用基类

	CloseVideoSet();
	//	CDialog::OnCancel();
}


void CShowAudioSettingDlg::NotifyCreateSkinItem()
{
	if (m_pAudioDlg->GetSafeHwnd() != NULL)
	{
		NOTIFY_SKIN_HELPER(SKIN_NOTIFY_AUDIO_DLG_HWND, m_pAudioDlg->GetSafeHwnd());
	}
}

//关闭摄像头
void CShowAudioSettingDlg::CloseVideoSet()
{
	m_pAudioDlg->CloseVideo();  //关闭摄像头

}
BOOL CShowAudioSettingDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	NotifyCreateSkinItem();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CShowAudioSettingDlg::OnCbnSelchangeComboCamera()
{
	// TODO: 在此添加控件通知处理程序代码

	int iDevice = m_VideoList.GetCurSel();
	if(iDevice != -1)
	{
		VInfo stInfo;
		if (VideoManager::GetInstance()->GetVideoInfoByIndex(iDevice, stInfo))
		{
			m_pAudioDlg->StartVideo(stInfo.nDeviceID);

			CComVariant var = stInfo.bVirtual?L"true":L"false";  //是否为虚拟视频		
			m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(), L"btnPicQuality", L"disabled", var);  //频调节面板是否可用			

			::SendMessage(GetParent()->GetSafeHwnd(),WM_NOTIFY_ROOM_SYSSET_MODIFY,0,0); //给给父窗口(菜单设置)发送设置对话框有修改的消息
		}		
	}


}


void CShowAudioSettingDlg::OnClickBtnRefresh()
{
	CloseVideoSet();	

	core::CurInfo m_MyCurInfo = GetCurInfo();		

	int nCurSel = m_VideoList.GetCurSel();
	VInfo stCurInfo;
	if (!VideoManager::GetInstance()->GetVideoInfoByIndex(nCurSel, stCurInfo))
	{
		ASSERT(0);
	}

	VideoManager::GetInstance()->ReloadVideoDevice();
	VecVideoInfo vecVideoList = VideoManager::GetInstance()->GetVideoList();

	m_VideoList.ResetContent();
	for (int i = 0; i < vecVideoList.size(); i++)
	{
		m_VideoList.AddString(vecVideoList[i].strName.c_str());
	}

	if(vecVideoList.size() > 0)
	{
		VInfo stInfo;
		if (VideoManager::GetInstance()->GetVideoInfoByGUID(stCurInfo.strGUID, stInfo))
		{
			m_VideoList.SetCurSel(stInfo.nIndex);
			m_pAudioDlg->StartVideo(stInfo.nDeviceID);
		}
		else
		{
			VideoManager::GetInstance()->GetVideoInfoByIndex(0, stInfo);
			m_VideoList.SetCurSel(0);
			m_pAudioDlg->StartVideo(stInfo.nDeviceID);
		}

		CComVariant var = stInfo.bVirtual?L"true":L"false";  //是否为虚拟视频
		m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(), L"btnPicQuality", L"disabled", var);  //频调节面板是否可用

	}
	else
	{
		m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(), L"btnPicQuality", L"disabled", (CComVariant)true);  //频调节面板是否可用
	}

}

void CShowAudioSettingDlg::OnClickBtnPicQuality()
{
	m_pAudioDlg->ShowVideoAdjust(); //显示已开启视频的属性对话框

	::SendMessage(GetParent()->GetSafeHwnd(),WM_NOTIFY_ROOM_SYSSET_MODIFY,0,0); //给给父窗口(菜单设置)发送设置对话框有修改的消息
}
BOOL CShowAudioSettingDlg::PreTranslateMessage(MSG* pMsg)
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

void CShowAudioSettingDlg::SetApplyEffect(BOOL bEffect)
{
	CComVariant var = bEffect;
	m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(), L"imgApplyEffect", L"visible", var);
}