// AudioTestDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "..\Room.h"
#include "ShowAudioTestDlg.h"
#include "Skin_i.h"
#include "SkinUtil.h"
#include "ShowAudioTestStep1.h"
#include "ShowAudioTestStep2.h"
#include "utility/SystemHelper.h"

#define  SKIN_ON_CLICK_CLOSE          1
#define  SKIN_NOTIFY_STEP1_HWND       2
#define  SKIN_NOTIFY_STEP2_HWND       3
#define  SKIN_ON_CLICK_BTN_CANCEL     4
#define  SKIN_ON_CLICK_BTN_PRESTEP    5
#define  SKIN_ON_CLICK_BTN_NEXTSTEP   6
#define  SKIN_ON_CLICK_BTN_DONE       7
// CAudioTestDlg 对话框

IMPLEMENT_DYNAMIC(CShowAudioTestDlg, CDialog)

CShowAudioTestDlg::CShowAudioTestDlg( CWnd* pParent /*=NULL*/)
	: CDialog(CShowAudioTestDlg::IDD, pParent)
{
	m_pSkinEngine = NULL;
	::GetSkinEngine(&m_pSkinEngine);

	m_pAudioTestStep1 = NULL;
	m_pAudioTestStep2 = NULL;
}

CShowAudioTestDlg::~CShowAudioTestDlg()
{
	if(m_pAudioTestStep1 != NULL)
	{
		m_pAudioTestStep1->DestroyWindow();
		delete m_pAudioTestStep1;
		m_pAudioTestStep1 = NULL;
	}

	if(m_pAudioTestStep2 != NULL)
	{
		m_pAudioTestStep2->DestroyWindow();
		delete m_pAudioTestStep2;
		m_pAudioTestStep2 = NULL;
	}
}

void CShowAudioTestDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CShowAudioTestDlg, CDialog)
	ON_WM_CREATE()
	ON_SKIN_MESSAGE
END_MESSAGE_MAP()


// CAudioTestDlg 消息处理程序

int CShowAudioTestDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  在此添加您专用的创建代码
	ASSERT(NULL != m_pSkinEngine);
	if (NULL != m_pSkinEngine)
	{
		HRESULT hr = m_pSkinEngine->LoadSkin(GetSafeHwnd(),L"AudioTestDlg_AtRoom");
		ASSERT(hr== S_OK && __LINE__);
	}
    //创建子窗口
    m_pAudioTestStep1 = new CShowAudioTestStep1();
	if (m_pAudioTestStep1)
	{
		m_pAudioTestStep1->Create(CShowAudioTestStep1::IDD,this);
		m_pAudioTestStep1->SetWindowPos(NULL,0,0,400,306,SWP_SHOWWINDOW);
		m_pAudioTestStep1->ShowWindow(SW_SHOW);
	}

	m_pAudioTestStep2 = new CShowAudioTestStep2();
	if (m_pAudioTestStep1)
	{
		m_pAudioTestStep2->Create(CShowAudioTestStep2::IDD,this);
		m_pAudioTestStep2->SetWindowPos(NULL,0,0,400,306,SWP_SHOWWINDOW);
		m_pAudioTestStep2->ShowWindow(SW_HIDE);
	}

	//标题字体首选微软雅黑
	std::wstring strFont;
	common::utility::systemhelper::GetDefaultFontName(strFont);
	CComVariant var = (CComVariant) strFont.c_str();
	m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(),L"textTitle",L"fontFamily",var);


	return 0;
}

void CShowAudioTestDlg::StopTest()
{
	m_pAudioTestStep1->StopTest();
	m_pAudioTestStep2->StopTest();
}

LRESULT CShowAudioTestDlg::OnSkinMessage(WPARAM wParam, LPARAM lParam)
{
	switch(wParam)
	{
	case SKIN_ON_CLICK_CLOSE:
		{
           OnClickCancel();
		}
		break;
	case SKIN_ON_CLICK_BTN_CANCEL:
		{
           OnClickCancel();
		}
		break;
	case SKIN_ON_CLICK_BTN_PRESTEP:
		{
           OnClickPreStep();
		}
		break;
	case SKIN_ON_CLICK_BTN_NEXTSTEP:
		{
           OnClickNextStep();
		}
		break;
	case SKIN_ON_CLICK_BTN_DONE:
		{
           OnClickDone();
		}
		break;
	default:break;
	}


	return TRUE;
}

VOID CShowAudioTestDlg::NotifyCreateSkinItem()
{
	if (m_pAudioTestStep1->GetSafeHwnd())
	{
		NOTIFY_SKIN_HELPER(SKIN_NOTIFY_STEP1_HWND,m_pAudioTestStep1->GetSafeHwnd());
	}

	if (m_pAudioTestStep2->GetSafeHwnd())
	{
		NOTIFY_SKIN_HELPER(SKIN_NOTIFY_STEP2_HWND,m_pAudioTestStep2->GetSafeHwnd());
	}
}

BOOL CShowAudioTestDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	NotifyCreateSkinItem();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void CShowAudioTestDlg::OnClickCancel()
{
    if (this->GetSafeHwnd())
    {
		this->ShowWindow(SW_HIDE);
		CComVariant var = L"取消";
		m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(),L"btnFront",L"label",var);
		var = L"下一步";
		m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(),L"btnBack",L"label",var);
		var = TRUE;
		m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(),L"itemAudioSetStep1Dlg",L"visible",var);
		var = FALSE;
		m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(),L"itemAudioSetStep2Dlg",L"visible",var);

		StopTest();
    }
}

void CShowAudioTestDlg::OnClickPreStep()
{
    if (m_pAudioTestStep1->GetSafeHwnd() && m_pAudioTestStep2->GetSafeHwnd())
    {
		m_pAudioTestStep1->ShowWindow(SW_SHOW);
		m_pAudioTestStep2->ShowWindow(SW_HIDE);
		CComVariant var = L"取消";
		m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(),L"btnFront",L"label",var);
		var = L"下一步";
		m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(),L"btnBack",L"label",var);
		var = TRUE;
		m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(),L"itemAudioSetStep1Dlg",L"visible",var);
		var = FALSE;
		m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(),L"itemAudioSetStep2Dlg",L"visible",var);
    }
}

void CShowAudioTestDlg::OnClickNextStep()
{
	if (m_pAudioTestStep1->GetSafeHwnd() && m_pAudioTestStep2->GetSafeHwnd())
	{
		m_pAudioTestStep1->ShowWindow(SW_HIDE);
		m_pAudioTestStep2->ShowWindow(SW_SHOW);
		CComVariant var = L"上一步";
		m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(),L"btnFront",L"label",var);
		var = L"完成";
		m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(),L"btnBack",L"label",var);
		var = FALSE;
		m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(),L"itemAudioSetStep1Dlg",L"visible",var);
		var = TRUE;
		m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(),L"itemAudioSetStep2Dlg",L"visible",var);
	}
}

void CShowAudioTestDlg::OnClickDone()
{
	//StopTest();
	//ShowWindow(SW_HIDE);
	OnClickCancel();
}


BOOL CShowAudioTestDlg::PreTranslateMessage(MSG* pMsg)
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

