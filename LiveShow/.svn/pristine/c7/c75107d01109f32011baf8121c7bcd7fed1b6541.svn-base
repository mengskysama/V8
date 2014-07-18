// ShowBankAccountDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "..\Room.h"
#include "ShowBankAccountDlg.h"
#include "skin_i.h"
#include "SkinUtil.h"
#include "HallDefine.h"


// CShowBankAccountDlg 对话框

IMPLEMENT_DYNAMIC(CShowBankAccountDlg, CDialog)

CShowBankAccountDlg::CShowBankAccountDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CShowBankAccountDlg::IDD, pParent)
{
	m_pSkinEngine = NULL;
	::GetSkinEngine(&m_pSkinEngine);

}


CShowBankAccountDlg::~CShowBankAccountDlg()
{
}

void CShowBankAccountDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CShowBankAccountDlg, CDialog)
	ON_WM_CREATE()
	ON_SKIN_MESSAGE
	ON_WM_SHOWWINDOW()
END_MESSAGE_MAP()


// CShowBankAccountDlg 消息处理程序


int CShowBankAccountDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  在此添加您专用的创建代码
	ASSERT(NULL != m_pSkinEngine);
	if (NULL != m_pSkinEngine)
	{
		HRESULT hr = m_pSkinEngine->LoadSkin(GetSafeHwnd(),L"ShowBankAccountDlg_AtRoom");
		ASSERT(hr== S_OK && __LINE__);
	}

	return 0;
}


LRESULT CShowBankAccountDlg::OnSkinMessage(WPARAM wParam,LPARAM lParam)
{


	return TRUE;
}

void CShowBankAccountDlg::OnOK()
{
	// TODO: 在此添加专用代码和/或调用基类

	//	CDialog::OnOK();
}

void CShowBankAccountDlg::OnCancel()
{
	// TODO: 在此添加专用代码和/或调用基类

	//	CDialog::OnCancel();
}

BOOL CShowBankAccountDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	//创建显示对应所有房间浏览器
	m_BankAccount.CreateFromStatic(IDC_ATROOM_HTML_SHOW_BANKACCOUNT,this);
	CComVariant var = (LONG)m_BankAccount.GetSafeHwnd();
	m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(),L"itemHtmlBankAccount",L"hwnd",var);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void CShowBankAccountDlg::OnShowWindow(BOOL bShow,UINT nStatus)
{
	CDialog::OnShowWindow(bShow, nStatus);

	if(bShow)
	{
		Init();
	}

}


LRESULT CShowBankAccountDlg::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	switch(message)
	{
	case WM_NOTIFY_ROOM_SYSSET_CLICKOK:  //“确定”按扭的消息		
	case WM_NOTIFY_ROOM_SYSSET_CLICKAPPLI:  // "应用"按扭的消息
		{

		}
		break;	
	default:
		break;
	}

	return CDialog::WindowProc(message, wParam, lParam);
}

void CShowBankAccountDlg::Init()
{
	Hall::WebInterfaceUrl weburl;
	weburl.m_WebWithPara = true;
	weburl.m_WebInterfaceKeyName = SYSSET_INT_KEYNAME_MENUSETBANKACCOUNT;
	g_RoomModule->m_pModuleManager->CallService(Hall::SERVICE_VALUE_HALL_GET_WEBINTERFACEURL, (param)&weburl);
	m_BankAccount.SetUrl(weburl.m_WebInterfaceUrl,false,true);
	m_BankAccount.UpdateURL();
}

BOOL CShowBankAccountDlg::PreTranslateMessage(MSG* pMsg)
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

void CShowBankAccountDlg::SetApplyEffect(BOOL bEffect)
{
	CComVariant var = bEffect;
	m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(), L"imgApplyEffect", L"visible", var);
}