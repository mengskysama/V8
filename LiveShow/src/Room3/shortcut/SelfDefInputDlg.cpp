// SelfDefInputDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "..\Room.h"
#include "SelfDefInputDlg.h"
#include "Skin_i.h"
#include "SkinUtil.h"
#include "..\..\CommonLibrary\include\utility\SystemHelper.h"

#define SKIN_ON_CLICK_CLOSE           1
#define SKIN_ON_CLICK_BTN_CONFIRM     2

// CSelfDefInputDlg 对话框

IMPLEMENT_DYNAMIC(CSelfDefInputDlg, CDialog)

CSelfDefInputDlg::CSelfDefInputDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSelfDefInputDlg::IDD, pParent)
{
	m_pSkinEngine = NULL;
	::GetSkinEngine(&m_pSkinEngine);
}

CSelfDefInputDlg::~CSelfDefInputDlg()
{
}

void CSelfDefInputDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_SELF_DEF_INPUT, m_InputEdit);
}


BEGIN_MESSAGE_MAP(CSelfDefInputDlg, CDialog)
	ON_WM_CREATE()
	ON_SKIN_MESSAGE
END_MESSAGE_MAP()


// CSelfDefInputDlg 消息处理程序

int CSelfDefInputDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  在此添加您专用的创建代码
	ASSERT(NULL != m_pSkinEngine);
	if (NULL != m_pSkinEngine)
	{
		HRESULT hr = m_pSkinEngine->LoadSkin(GetSafeHwnd(),L"SelfDefInputDlg");
		ASSERT(hr== S_OK && __LINE__);
	}
	//标题字体首选微软雅黑
	std::wstring strFont;
	common::utility::systemhelper::GetDefaultFontName(strFont);
	CComVariant var = (CComVariant) strFont.c_str();
	m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(),L"textTitle",L"fontFamily",var);

	return 0;
}

BOOL CSelfDefInputDlg::PreTranslateMessage(MSG* pMsg)
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


LRESULT CSelfDefInputDlg::OnSkinMessage(WPARAM wParam, LPARAM lParam)
{
	switch(wParam)
	{
	case SKIN_ON_CLICK_CLOSE:
		{
           PostMessage(WM_CLOSE,NULL,NULL);
		}
		break;
	case SKIN_ON_CLICK_BTN_CONFIRM:
		{
           OutputDebugString(_T("\n$$$$$$$$$$$$$$$$确定$$$$$$$$$$$$$$$$$$\n"));
		   UpdateData() ;
		   m_InputEdit.GetWindowText( m_strInput ) ;
		   EndDialog( IDOK ) ;
		}
		break;
 
	default:break;
	}


    return TRUE;
}
CString CSelfDefInputDlg::GetInPutString() 
{
	return m_strInput ;
}