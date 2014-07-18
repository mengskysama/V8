// PresentDetailDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "..\Room.h"
#include "PresentDetailDlg.h"
#include "Skin_i.h"
#include "SkinUtil.h"
#include "HallDefine.h"
#include "..\GlobalRoomDefine.h"
#include "..\..\CommonLibrary\include\utility\SystemHelper.h"

#define  SKIN_ON_CLICK_CLOSE       1
// CPresentDetailDlg 对话框

IMPLEMENT_DYNAMIC(CPresentDetailDlg, CDialog)

CPresentDetailDlg::CPresentDetailDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CPresentDetailDlg::IDD, pParent)
{
	m_pSkinEngine = NULL;
	::GetSkinEngine(&m_pSkinEngine);
}

CPresentDetailDlg::~CPresentDetailDlg()
{
}

void CPresentDetailDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CPresentDetailDlg, CDialog)
	ON_WM_CREATE()
	ON_SKIN_MESSAGE
	ON_WM_SHOWWINDOW()
END_MESSAGE_MAP()


// CPresentDetailDlg 消息处理程序

BOOL CPresentDetailDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	m_htmlPresentDetail.CreateFromStatic(IDC_HTML_PRESENT_DETAIL,this);
	CComVariant var = (LONG)m_htmlPresentDetail.GetSafeHwnd();
	var = (LONG)m_htmlPresentDetail.GetSafeHwnd();
	m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(),L"itemHtmlPresentDetail",L"hwnd",var);

	InitBrowser();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

BOOL CPresentDetailDlg::PreTranslateMessage(MSG* pMsg)
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

int CPresentDetailDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  在此添加您专用的创建代码
	//LoadSkin
	ASSERT(NULL != m_pSkinEngine);
	if (NULL != m_pSkinEngine)
	{
		HRESULT hr = m_pSkinEngine->LoadSkin(GetSafeHwnd(),L"PresentDetailDlg");
		ASSERT(hr== S_OK && __LINE__);
	}

	//标题字体首选微软雅黑
	std::wstring strFont;
	common::utility::systemhelper::GetDefaultFontName(strFont);
	CComVariant var = (CComVariant) strFont.c_str();
	m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(),L"textTitle",L"fontFamily",var);

	return 0;
}

void CPresentDetailDlg::InitBrowser()
{
	Hall::WebInterfaceUrl weburl;
	weburl.m_WebWithPara = true;
	weburl.m_WebInterfaceKeyName = SYSSET_INI_KEYNAME_GIFTDETAIL;
	g_RoomModule->m_pModuleManager->CallService(Hall::SERVICE_VALUE_HALL_GET_WEBINTERFACEURL, (param)&weburl);
	std::wstring strUrl = weburl.m_WebInterfaceUrl ;
	//std::wstring strUrl = L"about:blank";

	m_htmlPresentDetail.SetUrl(strUrl);
	m_htmlPresentDetail.UpdateURL();

}

LRESULT CPresentDetailDlg::OnSkinMessage(WPARAM wParam, LPARAM lParam)
{
    switch(wParam)
	{
	case SKIN_ON_CLICK_CLOSE:
		{
            //DestroyWindow();
			ShowWindow(SW_HIDE);
		}
		break;
	default:break;
	}


	return TRUE;
}

void CPresentDetailDlg::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialog::OnShowWindow(bShow, nStatus);

	// TODO: 在此处添加消息处理程序代码
	if (bShow)
	{
		InitBrowser();
	}
}
