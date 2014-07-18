// CheckUpdateDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "../Hall.h"
#include "CheckUpdateDlg.h"
#include "Skin_i.h"
#include "SkinUtil.h"


// CCheckUpdateDlg 对话框

IMPLEMENT_DYNAMIC(CCheckUpdateDlg, CDialog)

CCheckUpdateDlg::CCheckUpdateDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CCheckUpdateDlg::IDD, pParent)
{
	m_pSkinEngine = NULL;
	::GetSkinEngine(&m_pSkinEngine);
}

CCheckUpdateDlg::~CCheckUpdateDlg()
{
}

void CCheckUpdateDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CCheckUpdateDlg, CDialog)
	ON_WM_CREATE()
	ON_SKIN_MESSAGE
END_MESSAGE_MAP()


// CCheckUpdateDlg 消息处理程序

int CCheckUpdateDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  在此添加您专用的创建代码
	ASSERT(m_pSkinEngine != NULL);
	if (NULL != m_pSkinEngine)
	{
		HRESULT hr = m_pSkinEngine->LoadSkin(GetSafeHwnd(),L"CheckUpdateDlg");
		ASSERT(hr == S_OK && __LINE__);
	}

	return 0;
}

LRESULT CCheckUpdateDlg::OnSkinMessage(WPARAM wParam,LPARAM lParam)
{
	switch(wParam)
	{

	default:break;
	}

	return TRUE;
}

BOOL CCheckUpdateDlg::PreTranslateMessage(MSG* pMsg)
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
