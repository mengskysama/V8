// AudioTestStep2.cpp : 实现文件
//

#include "stdafx.h"
#include "..\Room.h"
#include "ShowAudioTestStep2.h"
#include "Skin_i.h"
#include "SkinUtil.h"

// CAudioTestStep2 对话框
#define CHECK_AUTIO_LEVEL	501

IMPLEMENT_DYNAMIC(CShowAudioTestStep2, CDialog)

CShowAudioTestStep2::CShowAudioTestStep2(CWnd* pParent /*=NULL*/)
	: CDialog(CShowAudioTestStep2::IDD, pParent)
{
	m_pSkinEngine = NULL;
	m_bWavInEnable = false;
	::GetSkinEngine(&m_pSkinEngine);
}

CShowAudioTestStep2::~CShowAudioTestStep2()
{
}

void CShowAudioTestStep2::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//DDX_Control(pDX, IDC_ATROOM_COMBO_MIC, m_comboMic);
}


BEGIN_MESSAGE_MAP(CShowAudioTestStep2, CDialog)
	ON_WM_CREATE()
	ON_SKIN_MESSAGE
	ON_WM_HSCROLL()
	ON_WM_TIMER()
	ON_WM_SHOWWINDOW()
END_MESSAGE_MAP()


// CAudioTestStep2 消息处理程序

int CShowAudioTestStep2::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  在此添加您专用的创建代码
	ASSERT(NULL != m_pSkinEngine);
	if (NULL != m_pSkinEngine)
	{
		HRESULT hr = m_pSkinEngine->LoadSkin(GetSafeHwnd(),L"AudioTestStep2Dlg_AtRoom");
		ASSERT(hr== S_OK && __LINE__);
	}

	m_pRoomAvLogic = VideoManager::GetInstance()->GetRoomAvLogic();	
	return 0;
}


LRESULT CShowAudioTestStep2::OnSkinMessage(WPARAM wParam, LPARAM lParam)
{
	switch(wParam)
	{

	default:break;
	}

	return TRUE;
}
BOOL CShowAudioTestStep2::PreTranslateMessage(MSG* pMsg)
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


void CShowAudioTestStep2::LoadAudioInputDeviceList()
{
	Room_Get_Audio_Device_List audiolist;
	memset(&audiolist, 0x0, sizeof(Room_Get_Audio_Device_List));
	audiolist.bInput = true;

	IModuleManager * pModuleManager = g_RoomModule->m_pModuleManager;
	if (pModuleManager != NULL)
	{
		pModuleManager->CallService(Room::SERVICE_ROOM_GETAUDIO_DEVICE_LIST, (param)&audiolist);
	}

	((CComboBox*)GetDlgItem(IDC_ATROOM_COMBO_MIC))->ResetContent();
	for(int i=0;i<audiolist.iDeviceNum;i++)
	{
		((CComboBox*)GetDlgItem(IDC_ATROOM_COMBO_MIC))->AddString(audiolist.wsDeviceName[i]);
	}

	if(audiolist.iDeviceNum > 0)
	{
		((CComboBox*)GetDlgItem(IDC_ATROOM_COMBO_MIC))->SetCurSel(0);
	}

	((CComboBox*)GetDlgItem(IDC_ATROOM_COMBO_MIC))->EnableWindow(FALSE);
		
	if (m_pRoomAvLogic != NULL)
	{
		unsigned int nPos;
		m_pRoomAvLogic->GetMixerControlWavInVolume(nPos);
		((CSliderCtrl*)GetDlgItem(IDC_ATROOM_SLIDER_MICFONE))->SetPos(nPos);
	}
}


BOOL CShowAudioTestStep2::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	LoadAudioInputDeviceList();

	((CSliderCtrl*)GetDlgItem(IDC_ATROOM_SLIDER_MICFONE))->SetRange(0, 255, TRUE);
	((CSliderCtrl*)GetDlgItem(IDC_ATROOM_SLIDER_MICFONE))->SetPageSize(25);

	//DrawLevelProgess(100,66);
	//SetTimer(CHECK_AUTIO_LEVEL, 500, NULL);
	RoomAVLogic* pRoomAvLogic= VideoManager::GetInstance()->GetRoomAvLogic();		
	if (pRoomAvLogic != NULL)
	{
		if (pRoomAvLogic->GetMixerControlWavInEnable())
		{
			m_bWavInEnable = true;
		}
		else
		{
			((CSliderCtrl*)GetDlgItem(IDC_ATROOM_SLIDER_MICFONE))->SetPos(255);
			((CSliderCtrl*)GetDlgItem(IDC_ATROOM_SLIDER_MICFONE))->EnableWindow(FALSE);
		}

	}
	

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CShowAudioTestStep2::DrawLevelProgess(int nTotal,int nFinish) //绘制音量波形
{
	CRect rcTemp,rcLevelProgress;
	m_pSkinEngine->GetSubSkinPos(GetSafeHwnd(),L"progBack",&(rcTemp.left),&(rcTemp.top),&(rcTemp.right),&(rcTemp.bottom));	

	if(nTotal == 0)
		nTotal = 1;
	int nWidth = rcTemp.right - rcTemp.left;
	int nFullLength = (int)((( nFinish*100.0)/nTotal*nWidth)/100.0);
	int nWhiteLength = nWidth - nFullLength;

	CComVariant varFullLen,varWhiteLen;
	CString tempLength;
	tempLength.Format(_T("%d"),nFullLength);
	varFullLen = (CComVariant) tempLength;
	tempLength.Format(_T("%d"),nWhiteLength);
	varWhiteLen = (CComVariant) tempLength;

	m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(),L"progFull",L"width",varFullLen);
	m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(),L"progWhite",L"width",varWhiteLen);
}


void CShowAudioTestStep2::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (GetDlgItem(IDC_ATROOM_SLIDER_MICFONE) == pScrollBar)
	{
		int nPos = ((CSliderCtrl*)GetDlgItem(IDC_ATROOM_SLIDER_MICFONE))->GetPos();
		if (m_pRoomAvLogic != NULL && m_bWavInEnable)
		{
			m_pRoomAvLogic->SetMixerControlWavInVolume(nPos);
			((CSliderCtrl*)GetDlgItem(IDC_ATROOM_SLIDER_MICFONE))->Invalidate(FALSE);
		}
	}

	CDialog::OnHScroll(nSBCode, nPos, pScrollBar);
}


void CShowAudioTestStep2::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (CHECK_AUTIO_LEVEL == nIDEvent)
	{
		if (m_pRoomAvLogic != NULL)
		{
			unsigned int unInput, unOutput;
			m_pRoomAvLogic->GetSoundLevel(unOutput, unInput);

			DrawLevelProgess(255, unInput);
		}
	}

	CDialog::OnTimer(nIDEvent);
}
void CShowAudioTestStep2::StopTest()
{
	if (m_pRoomAvLogic != NULL)
	{
		m_pRoomAvLogic->TestSafeCloseInputDevice();
		m_pRoomAvLogic->TestCheckSoundDevice(false);
	}
	KillTimer(CHECK_AUTIO_LEVEL);
}
void CShowAudioTestStep2::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialog::OnShowWindow(bShow, nStatus);

	// TODO: 在此处添加消息处理程序代码
	if (bShow)
	{
		if (m_pRoomAvLogic != NULL)
		{
			m_pRoomAvLogic->TestSetAudioDevice(0, 0);
			m_pRoomAvLogic->TestCheckSoundDevice(true);
		}
		SetTimer(CHECK_AUTIO_LEVEL, 50, NULL);
	}
	else
	{
		StopTest();
	}
}
