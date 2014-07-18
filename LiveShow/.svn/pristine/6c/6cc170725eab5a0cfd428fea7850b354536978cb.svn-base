// PayDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "..\Room.h"
#include "PayDlg.h"
#include "skin_i.h"
#include "SkinUtil.h"

#define  SKIN_ON_CLICK_CLOSE        1
// CPayDlg 对话框

IMPLEMENT_DYNAMIC(CPayDlg, CDialog)

CPayDlg::CPayDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CPayDlg::IDD, pParent)
{
	m_pSkinEngine = NULL;
	::GetSkinEngine(&m_pSkinEngine);
}

CPayDlg::~CPayDlg()
{
}

void CPayDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CPayDlg, CDialog)
	ON_WM_CREATE()
	ON_SKIN_MESSAGE
END_MESSAGE_MAP()


// CPayDlg 消息处理程序

int CPayDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  在此添加您专用的创建代码
	ASSERT(NULL != m_pSkinEngine);
	if (NULL != m_pSkinEngine)
	{
		HRESULT hr = m_pSkinEngine->LoadSkin(GetSafeHwnd(),L"PayDlg");
		ASSERT(hr== S_OK && __LINE__);
	}

	return 0;
}

BOOL CPayDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	m_htmlPay.CreateFromStatic(IDC_ROOM_HTML_PAY,this);
	CComVariant var = (LONG)m_htmlPay.GetSafeHwnd();
	m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(),L"itemHtmlPay",L"hwnd",var);
	InitPayBrowser();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

BOOL CPayDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
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

void CPayDlg::InitPayBrowser()
{
	CString strUrl = L"about:blank";
	m_htmlPay.SetUrl(std::wstring(strUrl.GetBuffer()), true);
	m_htmlPay.UpdateURL();
}

LRESULT CPayDlg::OnSkinMessage(WPARAM wParam,LPARAM lParam)
{
	switch(wParam)
	{
	case SKIN_ON_CLICK_CLOSE:
		{
			ShowWindow(SW_HIDE);
		}
		break;
	default:break;
	}

	return TRUE;
}

void CPayDlg::SetTheUrl( CString strUrl )
{
	if (m_htmlPay.GetSafeHwnd() == NULL)
	{
		return;
	}

	m_htmlPay.SetUrl(std::wstring(strUrl.GetBuffer()), true);
	m_htmlPay.UpdateURL();
}