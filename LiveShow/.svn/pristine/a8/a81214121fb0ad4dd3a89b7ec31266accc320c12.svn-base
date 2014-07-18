// AudioTestStep1.cpp : 实现文件
//

#include "stdafx.h"
#include "..\Room.h"
#include "ShowAudioTestStep1.h"
#include "Skin_i.h"
#include "SkinUtil.h"
#include "utility/systemhelper.h"
#include "..\personalsetting/VideoManager.h"
#include <MMSystem.h>
#pragma comment(lib, "Winmm.lib")

#define SKIN_ON_CLICK_BTN_TEST	1
#define SKIN_ON_CLICK_BTN_STOP	2
// CAudioTestStep1 对话框

IMPLEMENT_DYNAMIC(CShowAudioTestStep1, CDialog)

CShowAudioTestStep1::CShowAudioTestStep1( CWnd* pParent /*=NULL*/)
	: CDialog(CShowAudioTestStep1::IDD, pParent)
{
	m_pSkinEngine = NULL;
	::GetSkinEngine(&m_pSkinEngine);
}

CShowAudioTestStep1::~CShowAudioTestStep1()
{
	OnClickStop();
}

void CShowAudioTestStep1::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//DDX_Control(pDX, IDC_ATROOM_COMBO_LOUDSPEAKER, m_comboLoudSpeaker);
}


BEGIN_MESSAGE_MAP(CShowAudioTestStep1, CDialog)
	ON_WM_CREATE()
	ON_SKIN_MESSAGE
	ON_WM_SHOWWINDOW()
	ON_WM_HSCROLL()
END_MESSAGE_MAP()


// CAudioTestStep1 消息处理程序

int CShowAudioTestStep1::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  在此添加您专用的创建代码
	ASSERT(NULL != m_pSkinEngine);
	if (NULL != m_pSkinEngine)
	{
		HRESULT hr = m_pSkinEngine->LoadSkin(GetSafeHwnd(),L"AudioTestStep1Dlg_AtRoom");
		ASSERT(hr== S_OK && __LINE__);
	}

	return 0;
}

LRESULT CShowAudioTestStep1::OnSkinMessage(WPARAM wParam, LPARAM lParam)
{
	switch(wParam)
	{
	case SKIN_ON_CLICK_BTN_TEST:
		{
			OnClickTest();
		}
		break;
	case SKIN_ON_CLICK_BTN_STOP:
		{
			OnClickStop();
		}
		break;
	default:break;
	}

	return TRUE;
}

BOOL CShowAudioTestStep1::PreTranslateMessage(MSG* pMsg)
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


void CShowAudioTestStep1::LoadAudioOutputDeviceList()
{
	Room_Get_Audio_Device_List audiolist;
	memset(&audiolist, 0x0, sizeof(Room_Get_Audio_Device_List));
	audiolist.bInput = false;

	IModuleManager * pModuleManager = g_RoomModule->m_pModuleManager;
	if (pModuleManager != NULL)
	{
		pModuleManager->CallService(Room::SERVICE_ROOM_GETAUDIO_DEVICE_LIST, (param)&audiolist);
	}

	((CComboBox*)GetDlgItem(IDC_ATROOM_COMBO_LOUDSPEAKER))->ResetContent();
	for(int i=0;i<audiolist.iDeviceNum;i++)
	{
		((CComboBox*)GetDlgItem(IDC_ATROOM_COMBO_LOUDSPEAKER))->AddString(audiolist.wsDeviceName[i]);
	}

	if(audiolist.iDeviceNum > 0)
	{
		((CComboBox*)GetDlgItem(IDC_ATROOM_COMBO_LOUDSPEAKER))->SetCurSel(0);
	}

	((CComboBox*)GetDlgItem(IDC_ATROOM_COMBO_LOUDSPEAKER))->EnableWindow(FALSE);

	RoomAVLogic* pRoomAvLogic= VideoManager::GetInstance()->GetRoomAvLogic();	
	if (pRoomAvLogic != NULL)
	{
		unsigned int nVolume = 0;
		if (pRoomAvLogic->GetMixerControlTotalVolume(nVolume))
		{
			((CSliderCtrl*)GetDlgItem(IDC_ATROOM_SLIDER_LOUDSPEAKER))->SetPos(nVolume);
		}
		ASSERT(nVolume <= 255);
	}
}


BOOL CShowAudioTestStep1::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	((CSliderCtrl*)GetDlgItem(IDC_ATROOM_SLIDER_LOUDSPEAKER))->SetRange(0, 255, TRUE);
	((CSliderCtrl*)GetDlgItem(IDC_ATROOM_SLIDER_LOUDSPEAKER))->SetPageSize(25);
	LoadAudioOutputDeviceList();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CShowAudioTestStep1::OnClickTest()
{
	std::wstring strPath = common::utility::systemhelper::Get179AppPath();
	strPath += L"resource\\sound\\ding.wav";
	::PlaySound(strPath.c_str(), NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);

}

void CShowAudioTestStep1::OnClickStop()
{
	::PlaySound(NULL,NULL,0);
}

void CShowAudioTestStep1::StopTest()
{
	::PlaySound(NULL, NULL, 0);
	m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(), L"btnTest", L"visible", (CComVariant)TRUE);
	m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(), L"btnStop", L"visible", (CComVariant)FALSE);
}
void CShowAudioTestStep1::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (GetDlgItem(IDC_ATROOM_SLIDER_LOUDSPEAKER) == pScrollBar)
	{		

		int nPos = ((CSliderCtrl*)GetDlgItem(IDC_ATROOM_SLIDER_LOUDSPEAKER))->GetPos();
		RoomAVLogic* pRoomAvLogic= VideoManager::GetInstance()->GetRoomAvLogic();		
		if (pRoomAvLogic != NULL)
		{
			pRoomAvLogic->SetMixerControlTotalVolume(nPos);
			((CSliderCtrl*)GetDlgItem(IDC_ATROOM_SLIDER_LOUDSPEAKER))->Invalidate(FALSE);
		}

	}

	CDialog::OnHScroll(nSBCode, nPos, pScrollBar);
}

void CShowAudioTestStep1::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialog::OnShowWindow(bShow, nStatus);

	// TODO: 在此处添加消息处理程序代码
	if (!bShow)
	{
		StopTest();
	}
}


