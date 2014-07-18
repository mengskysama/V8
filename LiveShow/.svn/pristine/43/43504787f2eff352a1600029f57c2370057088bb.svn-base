// ShowBasicSettingDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "..\Room.h"
#include "ShowBasicSettingDlg.h"
#include "skin_i.h"
#include "SkinUtil.h"
#include "SysSettingDlg.h"
#include "utility/ConfigManager.h"
#include "utility/FileHelper.h"
#include "utility\StringHelper.h"
#include "ui\C179MsgBox.h"
#include "..\personalsetting/VideoManager.h"

// CShowBasicSettingDlg 对话框

IMPLEMENT_DYNAMIC(CShowBasicSettingDlg, CDialog)

CShowBasicSettingDlg::CShowBasicSettingDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CShowBasicSettingDlg::IDD, pParent)
{
	m_pSkinEngine = NULL;
	::GetSkinEngine(&m_pSkinEngine);
}

CShowBasicSettingDlg::~CShowBasicSettingDlg()
{
}

void CShowBasicSettingDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_ATROOM_CHECK_AUTOLOGIN, m_autoLogin);
	DDX_Control(pDX, IDC_ATROOM_CHECK_AUTORUN, m_PowerRun);
	DDX_Control(pDX, IDC_ATROOM_CHECK_OPENSOUND, m_openSound);
}


BEGIN_MESSAGE_MAP(CShowBasicSettingDlg, CDialog)

	ON_SKIN_MESSAGE
	ON_WM_CREATE()
	ON_WM_SHOWWINDOW()
	ON_BN_CLICKED(IDC_ATROOM_CHECK_AUTOLOGIN, &CShowBasicSettingDlg::OnBnClickedCheck)
	ON_BN_CLICKED(IDC_ATROOM_CHECK_AUTORUN, &CShowBasicSettingDlg::OnBnClickedCheck)
	ON_BN_CLICKED(IDC_ATROOM_CHECK_OPENSOUND, &CShowBasicSettingDlg::OnBnClickedCheck)
END_MESSAGE_MAP()


// CShowBasicSettingDlg 消息处理程序

int CShowBasicSettingDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  在此添加您专用的创建代码
	ASSERT(NULL != m_pSkinEngine);
	if (NULL != m_pSkinEngine)
	{
		HRESULT hr = m_pSkinEngine->LoadSkin(GetSafeHwnd(),L"ShowBasicSettingDlg_AtRoom");
		ASSERT(hr== S_OK && __LINE__);
	}

	return 0;
}

LRESULT CShowBasicSettingDlg::OnSkinMessage(WPARAM wParam,LPARAM lParam)
{

	return TRUE;
}

//读取是否开机运行
bool CShowBasicSettingDlg::ReadRegeditAutoRun()
{
	//C:\Documents and Settings\用户名\「开始」菜单\程序\启动\白兔KTV

	TCHAR szBuffer[MAX_PATH];
	LPITEMIDLIST lpItemIdList;
	SHGetSpecialFolderLocation(0, CSIDL_STARTUP, &lpItemIdList);
	SHGetPathFromIDList(lpItemIdList, szBuffer);

	wchar_t szShortcutFile[MAX_PATH] = {0};
	wsprintf( szShortcutFile, _T("%s\\%s.lnk"), szBuffer, _T("白兔KTV"));

	BOOL bShortcutFile = common::utility::filehelper::IsFileExist(szShortcutFile);
	return bShortcutFile;

}
 //设置开机运行
void CShowBasicSettingDlg::SetRegeditAutoRun()
{
	//C:\Documents and Settings\用户名\「开始」菜单\程序\启动\白兔KTV

	TCHAR szBuffer[MAX_PATH] ;
	LPITEMIDLIST lpItemIdList ;
	SHGetSpecialFolderLocation( 0 , CSIDL_STARTUP , &lpItemIdList ) ;
	SHGetPathFromIDList( lpItemIdList , szBuffer ) ;

	wchar_t szShortcutFile[MAX_PATH] = {0} ;
	wsprintf( szShortcutFile , _T("%s\\%s.lnk") , szBuffer ,_T("白兔KTV") ) ;

	IShellLink* pShellLink = NULL ;
	if ( CoCreateInstance( CLSID_ShellLink , NULL , CLSCTX_INPROC_SERVER , IID_IShellLink ,(void**)&pShellLink ) == S_OK )
	{
		IPersistFile* pPersistFile = NULL ;
		if ( pShellLink->QueryInterface( IID_IPersistFile , (void**)&pPersistFile ) == S_OK )
		{
			std::wstring strModulePath = common::utility::filehelper::GetCurPath();
			std::wstring strModuleDir = common::utility::filehelper::GetParentDir(strModulePath);
			pShellLink->SetPath( strModulePath.c_str() ) ;
			pShellLink->SetWorkingDirectory( strModuleDir.c_str() ) ;
			pShellLink->SetShowCmd( SW_SHOWNORMAL ) ;
			pPersistFile->Save( szShortcutFile , TRUE ) ;
			pPersistFile->Release() ;
		}
		pShellLink->Release() ;

	}


}
//清删开机运行
void CShowBasicSettingDlg::ClearRegeditAutoRun()
{	

	//C:\Documents and Settings\用户名\「开始」菜单\程序\启动\白兔KTV

	TCHAR szBuffer[MAX_PATH];
	LPITEMIDLIST lpItemIdList;
	SHGetSpecialFolderLocation(0, CSIDL_STARTUP, &lpItemIdList);
	SHGetPathFromIDList(lpItemIdList, szBuffer);

	wchar_t szShortcutFile[MAX_PATH] = {0};
	wsprintf( szShortcutFile, _T("%s\\%s.lnk"), szBuffer, _T("白兔KTV"));

	DeleteFile(szShortcutFile);

}


void CShowBasicSettingDlg::OnOK()
{
	// TODO: 在此添加专用代码和/或调用基类

//	CDialog::OnOK();
}

void CShowBasicSettingDlg::OnCancel()
{
	// TODO: 在此添加专用代码和/或调用基类

//	CDialog::OnCancel();
}


LRESULT CShowBasicSettingDlg::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
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

void CShowBasicSettingDlg::Confirm()
{
	core::CurInfo m_MyCurInfo = GetCurInfo();

	//设置开机启动
	BOOL powerRun = m_PowerRun.GetCheck();	//开机启动复选框是否选中
	if(powerRun)
	{
		SetRegeditAutoRun();
	}
	else
	{
		ClearRegeditAutoRun();
	}

	//设置自动登录	
	bool isSave = false;
	UserInfoLogin userinfo;	
	CConfigManager::Instance()->GetUserInfo_Login(m_MyCurInfo.unUIN,userinfo);
	if(userinfo.bAutoLogin != (bool)m_autoLogin.GetCheck())
	{
		userinfo.bAutoLogin = (bool)m_autoLogin.GetCheck();	
		if(userinfo.bAutoLogin)
		{
			userinfo.bSavePassword = true;
		}
		isSave = true;
	}

	//设置打开声音
	if(userinfo.bOpenSound != (bool)m_openSound.GetCheck())
	{
		userinfo.bOpenSound = (bool)m_openSound.GetCheck();	
		isSave = true;	
	
		RoomAVLogic* pRoomAvLogic= VideoManager::GetInstance()->GetRoomAvLogic();
		if ( pRoomAvLogic != NULL )
		{
			pRoomAvLogic->SetAllSoundMute( !userinfo.bOpenSound);
		}
		
	}

	if(isSave)
	{
		CConfigManager::Instance()->SetUserInfo_Login(userinfo);
	}


}
void CShowBasicSettingDlg::Init()
{
	core::CurInfo m_MyCurInfo = GetCurInfo();
	BOOL autoLogin,powerRun,openSound;

	UserInfoLogin userinfo;	
	CConfigManager::Instance()->GetUserInfo_Login(m_MyCurInfo.unUIN,userinfo);		

	powerRun = (BOOL)ReadRegeditAutoRun();
	autoLogin = (BOOL)userinfo.bAutoLogin;
	openSound = (BOOL)userinfo.bOpenSound;
	m_autoLogin.SetCheck(autoLogin);
	m_PowerRun.SetCheck(powerRun);
	m_openSound.SetCheck(openSound);

}
void CShowBasicSettingDlg::OnShowWindow(BOOL bShow,UINT nStatus)
{
	CDialog::OnShowWindow(bShow, nStatus);

	if(bShow)  //显示个人基本设置
	{
		Init();
	}

}

void CShowBasicSettingDlg::OnBnClickedCheck()
{
	// TODO: 在此添加控件通知处理程序代码
	::SendMessage(GetParent()->GetSafeHwnd(),WM_NOTIFY_ROOM_SYSSET_MODIFY,0,0); //给给父窗口(菜单设置)发送设置对话框有修改的消息

}

BOOL CShowBasicSettingDlg::PreTranslateMessage(MSG* pMsg)
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

void CShowBasicSettingDlg::SetApplyEffect(BOOL bEffect)
{
	CComVariant var = bEffect;
	m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(), L"imgApplyEffect", L"visible", var);
}