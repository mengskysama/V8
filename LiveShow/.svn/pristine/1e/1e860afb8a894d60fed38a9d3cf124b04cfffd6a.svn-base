// ShowPwdSafeDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "..\Room.h"
#include "ShowPwdSafeDlg.h"
#include "skin_i.h"
#include "SkinUtil.h"
#include "SysSettingDlg.h"
#include "ui\C179MsgBox.h"
#include "HallDefine.h"

// CShowPwdSafeDlg 对话框

IMPLEMENT_DYNAMIC(CShowPwdSafeDlg, CDialog)

CShowPwdSafeDlg::CShowPwdSafeDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CShowPwdSafeDlg::IDD, pParent)
{
	m_pSkinEngine = NULL;
	::GetSkinEngine(&m_pSkinEngine);
}

CShowPwdSafeDlg::~CShowPwdSafeDlg()
{
}

void CShowPwdSafeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CShowPwdSafeDlg, CDialog)
	ON_WM_CREATE()
	ON_SKIN_MESSAGE
	ON_WM_SHOWWINDOW()
END_MESSAGE_MAP()


// CShowPwdSafeDlg 消息处理程序

int CShowPwdSafeDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  在此添加您专用的创建代码
	ASSERT(NULL != m_pSkinEngine);
	if (NULL != m_pSkinEngine)
	{
		HRESULT hr = m_pSkinEngine->LoadSkin(GetSafeHwnd(),L"ShowPwdSafeDlg_AtRoom");
		ASSERT(hr== S_OK && __LINE__);
	}

	return 0;
}


BOOL CShowPwdSafeDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_ModifyPwd.CreateFromStatic(IDC_ATROOM_HTML_PWDSAFE,this);
	CComVariant var = (LONG)m_ModifyPwd.GetSafeHwnd();
	m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(),L"itemHtmlPwdSafe",L"hwnd",var);
	Init();

	return TRUE;
}

void CShowPwdSafeDlg::Init()
{

	Hall::WebInterfaceUrl weburl;
	weburl.m_WebWithPara = true;
	weburl.m_WebInterfaceKeyName = SYSSET_INI_KEYNAME_MENUSETPASSWORDSAFE;
	g_RoomModule->m_pModuleManager->CallService(Hall::SERVICE_VALUE_HALL_GET_WEBINTERFACEURL, (param)&weburl);
	m_ModifyPwd.SetUrl(weburl.m_WebInterfaceUrl,false,true);
	m_ModifyPwd.UpdateURL();
}

LRESULT CShowPwdSafeDlg::OnSkinMessage(WPARAM wParam,LPARAM lParam)
{


	return TRUE;
}

void CShowPwdSafeDlg::OnOK()
{
	// TODO: 在此添加专用代码和/或调用基类

//	CDialog::OnOK();
}

void CShowPwdSafeDlg::OnCancel()
{
	// TODO: 在此添加专用代码和/或调用基类

//	CDialog::OnCancel();
}


BOOL CShowPwdSafeDlg::PreTranslateMessage(MSG* pMsg)
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


void CShowPwdSafeDlg::OnShowWindow(BOOL bShow,UINT nStatus)
{
	CDialog::OnShowWindow(bShow, nStatus);

	if(bShow)
	{
		Init();
	}

}


LRESULT CShowPwdSafeDlg::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	switch(message)
	{
	case WM_NOTIFY_ROOM_SYSSET_CLICKOK:  //“确定”按扭的消息		
	case WM_NOTIFY_ROOM_SYSSET_CLICKAPPLI:  // "应用"按扭的消息
		{
			Confirm();
		}
		break;	
	case WM_MISSION_SHOW_MESSAGEBOX:  //拦截messagebox消息
		{
			CString sMessage = (LPCTSTR)wParam;
			if(sMessage == _T("修改成功"))
			{
				::SendMessage(GetParent()->GetSafeHwnd(),WM_NOTIFY_ROOM_SYSSET_MODIFY_OK,0,0); //给给父窗口发送设置密码修改成功的消息
			}
		}
		break;
	case WM_MISSION_HYPERLINK_SYSSETTING:  //个人资料有修改
		{
			::SendMessage(GetParent()->GetSafeHwnd(),WM_NOTIFY_ROOM_SYSSET_MODIFY,0,0); //给给父窗口发送设置对话框有修改的消息
		}
		break;
	default:
		break;
	}

	return CDialog::WindowProc(message, wParam, lParam);
}

void CShowPwdSafeDlg::Confirm()
{
	_variant_t pwdResult;
	m_ModifyPwd.CallJScript(_T("dosubmit"),&pwdResult);

}

void CShowPwdSafeDlg::SetApplyEffect(BOOL bEffect)
{
	CComVariant var = bEffect;
	m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(), L"imgApplyEffect", L"visible", var);
}