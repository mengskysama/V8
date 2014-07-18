// ChangeExitDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "../Hall.h"
#include "ChangeExitDlg.h"
#include "skin_i.h"
#include "SkinUtil.h"
#include "HallDlg.h"
#include "../CommonLibrary/include/utility/SystemHelper.h"

#define  SKIN_ON_CLICK_CLOSE 1
#define  SKIN_ON_CLICK_CHANGEUSER 2
#define  SKIN_ON_CLICK_EXIT 3

// CChangeExitDlg 对话框

IMPLEMENT_DYNAMIC(CChangeExitDlg, CDialog)

CChangeExitDlg::CChangeExitDlg(IModuleManager *pModuleManager,CWnd* pParent /*=NULL*/)
	: CDialog(CChangeExitDlg::IDD, pParent)
{
	m_pModuleManager = NULL;
	m_pModuleManager = pModuleManager;

    m_pSkinEngine = NULL;
	::GetSkinEngine(&m_pSkinEngine);
}

CChangeExitDlg::~CChangeExitDlg()
{
}

void CChangeExitDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CChangeExitDlg, CDialog)
	ON_WM_CREATE()
	ON_WM_CLOSE()
	ON_SKIN_MESSAGE
	ON_WM_SHOWWINDOW()
END_MESSAGE_MAP()


// CChangeExitDlg 消息处理程序

int CChangeExitDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  在此添加您专用的创建代码
	ASSERT(NULL != m_pSkinEngine);
	if (NULL != m_pSkinEngine)
	{
		HRESULT hr = m_pSkinEngine->LoadSkin(GetSafeHwnd(),L"ChangeExitDlg");
		ASSERT(hr== S_OK && __LINE__);
	}
    
	//标题字体首选微软雅黑
	std::wstring strFont;
	common::utility::systemhelper::GetDefaultFontName(strFont);
	CComVariant var = (CComVariant) strFont.c_str();
	m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(),L"textTitle",L"fontFamily",var);

	return 0;
}

void CChangeExitDlg::OnClose()
{
	GetParent()->EnableWindow(TRUE);
	ShowWindow(SW_HIDE);	
}

LRESULT CChangeExitDlg::OnSkinMessage(WPARAM wParam, LPARAM lParam)
{
	switch (wParam)
	{
	  case SKIN_ON_CLICK_CLOSE:
		   {
			   OnClose();
		   } 
		   break;
	  case SKIN_ON_CLICK_CHANGEUSER:
		   {
		       SendMessage(WM_CLOSE,NULL,NULL);
			   ::PostMessage(GetParent()->GetSafeHwnd(),WM_NOTIFY_HALL_CHANGE_USER,0,0);
		   }
		   break;
	  case SKIN_ON_CLICK_EXIT:
		   {
			   SendMessage(WM_CLOSE,NULL,NULL);
			   ::PostMessage(GetParent()->GetSafeHwnd(),WM_NOTIFY_HALL_EXIT,0,0);
		   }
		   break;

	  default:
		  break;
	}

	return TRUE;
}

void CChangeExitDlg::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialog::OnShowWindow(bShow, nStatus);
	if(bShow)
	{
		CenterWindow(GetParent());	//居中显示
	}	
}
