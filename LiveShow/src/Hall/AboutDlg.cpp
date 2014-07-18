// AboutDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "Hall.h"
#include "AboutDlg.h"
#include "skin_i.h"
#include "SkinUtil.h"
#include "..\CommonLibrary\include\utility\SystemHelper.h"
#include "SysSettingIni.h"

#define  SKIN_ON_CLICK_CLOSE          1
#define  SKIN_ON_CLICK_BTN_LINK       2
#define  SKIN_ON_CLICK_BTN_USER_AGREEMENT  3

// CAboutDlg 对话框

IMPLEMENT_DYNAMIC(CAboutDlg, CDialog)

CAboutDlg::CAboutDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CAboutDlg::IDD, pParent)
{
	m_pSkinEngine = NULL;
	::GetSkinEngine(&m_pSkinEngine);
}

CAboutDlg::~CAboutDlg()
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	ON_WM_CREATE()
	ON_SKIN_MESSAGE
	ON_WM_SHOWWINDOW()
END_MESSAGE_MAP()


// CAboutDlg 消息处理程序

int CAboutDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  在此添加您专用的创建代码
	//Load Skin
	ASSERT(m_pSkinEngine != NULL);
	if (m_pSkinEngine != NULL){
		HRESULT result = m_pSkinEngine->LoadSkin(GetSafeHwnd(), L"AboutDlg");
		ASSERT(result == S_OK && __LINE__);
	}

	return 0;
}

LRESULT CAboutDlg::OnSkinMessage(WPARAM wParam, LPARAM lParam)
{
	switch(wParam)
	{
	case SKIN_ON_CLICK_CLOSE:
		{
			CDialog::OnOK();
		}
		break;
	case SKIN_ON_CLICK_BTN_LINK:
		{
			OnClickBtnLink();
		}
		break;
	case SKIN_ON_CLICK_BTN_USER_AGREEMENT:
		{
			OnClickBtnUserAgreement();
		}
		break;
	default:break;
	}

	return S_OK;
}
void CAboutDlg::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialog::OnShowWindow(bShow, nStatus);

	// TODO: 在此处添加消息处理程序代码
	CString curVerInfo = CAutoUpdate::GetCurVersionInfo();  //从配置文件中获取当前版本号
	CComVariant var = curVerInfo;
	m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(),L"textVersion",L"text",var);  //设置客户端当前版本
}

void CAboutDlg::OnClickBtnLink()
{
	CString s179Official;
	CSysSettingIni::GetWebInterfaceUrl(SYSSET_INI_KEYNAME_HALL179OFFICIAL,s179Official);
	s179Official = AppendUrlParamet(s179Official);
	common::utility::systemhelper::OpenUrl(wstring(s179Official));   //使用浏览器打开79官方首页
}

void CAboutDlg::OnClickBtnUserAgreement()
{
	CString strUserAgreement;
	CSysSettingIni::GetWebInterfaceUrl(SYSSET_INI_KEYNAME_USERAGREEMENT,strUserAgreement);
	strUserAgreement = AppendUrlParamet(strUserAgreement);
	common::utility::systemhelper::OpenUrl(wstring(strUserAgreement));   //使用浏览器打开用户协议
}