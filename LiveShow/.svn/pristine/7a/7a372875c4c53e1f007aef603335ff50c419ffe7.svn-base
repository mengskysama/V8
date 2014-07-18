// VideoSettingDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "..\Room.h"
#include "VideoSettingDlg.h"
#include "skin_i.h"
#include "SkinUtil.h"
#include "AudioVideoSetting.h"
#include "utility/SystemHelper.h"
#include "..\GlobalRoomDefine.h"
#include "utility/URLEncode.h"

#define SKIN_NOTIFY_VEDIO_CHANGE 1

// CAudioDlg 对话框

IMPLEMENT_DYNAMIC(CVideoSettingDlg, CDialog)

CVideoSettingDlg::CVideoSettingDlg(CWnd* pParent /*=NULL*/)
: CDialog(CVideoSettingDlg::IDD, pParent)
{
	m_pSkinEngine = NULL;
	m_iDevice = -1;
	::GetSkinEngine(&m_pSkinEngine);
}

CVideoSettingDlg::~CVideoSettingDlg()
{
}


void CVideoSettingDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BOOL CVideoSettingDlg::PreTranslateMessage(MSG* pMsg)
{
	if(pMsg->message == WM_KEYDOWN)   
	{   
		switch ((int)pMsg->wParam)   
		{   
		case VK_RETURN:
			return TRUE;   
		}   
	} 
	if(pMsg->message == WM_SYSKEYDOWN )   
	{   
		if(((GetKeyState(VK_MENU) & (1 << (sizeof (SHORT)*8-1))) != 0 ))
		{
			if(pMsg->wParam == 'H')
			{
				return FALSE;
			}
			else if(pMsg->wParam == 'C')
			{
				return FALSE;
			}
			else if(pMsg->wParam == 'A')
			{
				return FALSE;
			}
		}
	} 

	return CDialog::PreTranslateMessage(pMsg);
}


BEGIN_MESSAGE_MAP(CVideoSettingDlg, CDialog)
	ON_WM_CREATE()
	ON_WM_SHOWWINDOW()
END_MESSAGE_MAP()

// CAudioDlg 消息处理程序

void CVideoSettingDlg::StartVideo(int iDevice)
{
	SendCloseVideo();  //若以前有开启过摄像头，则先关闭
	m_iDevice = iDevice;
	SendSetVideo();
	SendStartVideo(); //发送开启摄像头
	if ( CVideoSetting::IsCameraUsing( m_iDevice ) )
	{
		std::wstring strModulePath = common::utility::systemhelper::Get179AppPath();
		common::CURLEncode urlEncode;
		strModulePath = urlEncode.URLEncodeLocalPath(strModulePath);

		std::wstring strPath = strModulePath + IMG_VIDEO_USING_PNG_PATH ;

		CComVariant var = strPath.c_str() ;
		m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(), L"imgVedioBig", L"absoluteImage", var);  //频调节面板是否可用
	}
	else
	{
		CComVariant var = L"Global_vedio_big.png" ;
		m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(), L"imgVedioBig", L"image", var);  //频调节面板是否可用
	}
	Invalidate();
}

void CVideoSettingDlg::CloseVideo()
{	
	SendCloseVideo();  //关闭摄像头
}

//显示已开启视频的属性对话框
void CVideoSettingDlg::ShowVideoAdjust()
{

	if(m_iDevice != -1)
	{
		Show_Video_Ajust_Dlg videoAjust;
		memset(&videoAjust, 0x0, sizeof(Show_Video_Ajust_Dlg));
		videoAjust.iDeviceID = m_iDevice;
		videoAjust.hParentWnd = GetParent()->m_hWnd;
		CVideoSetting::ShowVideoAjustDlg(videoAjust);
		
	}
}

//发送开启视频请求
void CVideoSettingDlg::SendStartVideo()
{	
	if(m_iDevice != -1)
	{
		Start_Video_Device videoDe;
		memset(&videoDe, 0x0, sizeof(Start_Video_Device));
		videoDe.iDeviceId = m_iDevice;
		videoDe.hWnd = this->m_hWnd;
		CVideoSetting::TestStartVideoDevice(videoDe);

	}
	
}

//发送关闭视频请求
void CVideoSettingDlg::SendCloseVideo()
{
	if(m_iDevice != -1)
	{		
		Stop_Video_Device videoDe;
		memset(&videoDe, 0x0, sizeof(Stop_Video_Device));
		videoDe.iDeviceId = m_iDevice;
		videoDe.hWnd = this->m_hWnd;
		CVideoSetting::TestStopVideoDevice(videoDe);

		m_iDevice = -1;
	}
}

int CVideoSettingDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  在此添加您专用的创建代码
	//Load Skin
	ASSERT(m_pSkinEngine != NULL);
	if (m_pSkinEngine != NULL){
		HRESULT result = m_pSkinEngine->LoadSkin(GetSafeHwnd(), L"AudioDlg");
		ASSERT(result == S_OK && __LINE__);
	}

	return 0;
}

void CVideoSettingDlg::OnVedioChanged()
{
	NOTIFY_SKIN_HELPER(SKIN_NOTIFY_VEDIO_CHANGE, (LONG)0);
}

void CVideoSettingDlg::OnDestroy()
{
	CDialog::OnDestroy();
}

void CVideoSettingDlg::OnShowWindow(BOOL bShow,UINT nStatus)
{
	CDialog::OnShowWindow(bShow, nStatus);
}

void CVideoSettingDlg::SendSetVideo()
{	

	if(m_iDevice != -1)
	{
		Set_Video_Device videoDe;
		memset(&videoDe, 0x0, sizeof(Set_Video_Device));
		videoDe.iDeviceId = m_iDevice;
		CVideoSetting::TestSetVideoDevice(videoDe);
	}
}

