// ShowPersonalDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "..\Room.h"
#include "ShowPersonalDlg.h"
#include "skin_i.h"
#include "SkinUtil.h"
#include "utility\SystemHelper.h"
#include "utility\StringHelper.h"
#include "SysSettingDlg.h"
#include "HallDefine.h"
#include "ui\C179MsgBox.h"

// CShowPersonalDlg 对话框

IMPLEMENT_DYNAMIC(CShowPersonalDlg, CDialog)

CShowPersonalDlg::CShowPersonalDlg(CWnd* pParent /*=NULL*/)
: CDialog(CShowPersonalDlg::IDD, pParent)
{
	m_pSkinEngine = NULL;
	::GetSkinEngine(&m_pSkinEngine);
}

CShowPersonalDlg::~CShowPersonalDlg()
{
}

void CShowPersonalDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CShowPersonalDlg, CDialog)
	ON_WM_CREATE()
	ON_SKIN_MESSAGE
	ON_WM_SHOWWINDOW()
END_MESSAGE_MAP()


// CShowPersonalDlg 消息处理程序

int CShowPersonalDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  在此添加您专用的创建代码
	ASSERT(NULL != m_pSkinEngine);
	if (NULL != m_pSkinEngine)
	{
		HRESULT hr = m_pSkinEngine->LoadSkin(GetSafeHwnd(),L"ShowPersonalDlg_AtRoom");
		ASSERT(hr== S_OK && __LINE__);
	}

	return 0;
}


LRESULT CShowPersonalDlg::OnSkinMessage(WPARAM wParam,LPARAM lParam)
{

	return TRUE;
}
void CShowPersonalDlg::OnCancel()
{
	// TODO: 在此添加专用代码和/或调用基类
	//	CDialog::OnCancel();
}

void CShowPersonalDlg::OnOK()
{
	// TODO: 在此添加专用代码和/或调用基类

	//	CDialog::OnOK();
}

BOOL CShowPersonalDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	m_htmlPersonalInfo.CreateFromStatic(IDC_ATROOM_HTML_PERSONAL_INFO,this);
	CComVariant var = (LONG)m_htmlPersonalInfo.GetSafeHwnd();
	m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(),L"itemHtmlPersonalInfo",L"hwnd",var);


	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CShowPersonalDlg::Confirm()
{
	_variant_t pwdResult;
	m_htmlPersonalInfo.CallJScript(_T("dosubmit"),&pwdResult);
}
void CShowPersonalDlg::OnShowWindow(BOOL bShow,UINT nStatus)
{
	CDialog::OnShowWindow(bShow, nStatus);

	if(bShow)
	{
		Init();
	}

}


BOOL CShowPersonalDlg::PreTranslateMessage(MSG* pMsg)
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

void CShowPersonalDlg::Init()
{	
	Hall::WebInterfaceUrl weburl;
	weburl.m_WebWithPara = true;
	weburl.m_WebInterfaceKeyName = SYSSET_INI_KEYNAME_MENUSETPERSONALINFO;
	g_RoomModule->m_pModuleManager->CallService(Hall::SERVICE_VALUE_HALL_GET_WEBINTERFACEURL, (param)&weburl);
	m_htmlPersonalInfo.SetUrl(weburl.m_WebInterfaceUrl,false,true);
	m_htmlPersonalInfo.UpdateURL();
}


LRESULT CShowPersonalDlg::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	switch(message)
	{
	case WM_NOTIFY_ROOM_SYSSET_CLICKOK:  //“确定”按扭的消息
	case WM_NOTIFY_ROOM_SYSSET_CLICKAPPLI: //"应用"按扭的消息
		{
			//::PostMessage(GetParent()->m_hWnd,WM_CLOSE,NULL,NULL);  //给父窗口(菜单设置)发送退出消息
			Confirm();
		}
		break;
	case WM_MISSION_SHOW_MESSAGEBOX:  //拦截messagebox消息
		{
			CString sMessage = (LPCTSTR)wParam;			
			C179MsgBox::Show(GetParent(), NewMsgBox::MSGBOX_OK_ONE_STRING, sMessage);

		}
		break;
	case WM_MISSION_HYPERLINK_SYSSETTING:  //个人资料有修改
		{
			::SendMessage(GetParent()->GetSafeHwnd(),WM_NOTIFY_ROOM_SYSSET_MODIFY,0,0); //给给父窗口(菜单设置)发送设置对话框有修改的消息
		}
		break;
	default:
		break;
	}

	return CDialog::WindowProc(message, wParam, lParam);
}

void CShowPersonalDlg::SetApplyEffect(BOOL bEffect)
{
	CComVariant var = bEffect;
	m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(), L"imgApplyEffect", L"visible", var);
}