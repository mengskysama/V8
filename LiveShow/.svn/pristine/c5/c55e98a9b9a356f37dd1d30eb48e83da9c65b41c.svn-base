// SofaNoFlashDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "..\Room.h"
#include "skin_i.h"
#include "SkinUtil.h"
#include "SofaNoFlashDlg.h"
#include "..\GlobalRoomDefine.h"
#include "..\..\CommonLibrary\include\utility\SystemHelper.h"

#define SKIN_ON_CLICK_BTN_GETFLASH	1

// CSofaNoFlashDlg 对话框

IMPLEMENT_DYNAMIC(CSofaNoFlashDlg, CDialog)

CSofaNoFlashDlg::CSofaNoFlashDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSofaNoFlashDlg::IDD, pParent)
{
	m_pSkinEngine = NULL;
	::GetSkinEngine(&m_pSkinEngine);
}

CSofaNoFlashDlg::~CSofaNoFlashDlg()
{
}

void CSofaNoFlashDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CSofaNoFlashDlg, CDialog)
	ON_WM_CREATE()
	ON_SKIN_MESSAGE
END_MESSAGE_MAP()


// CSofaNoFlashDlg 消息处理程序

int CSofaNoFlashDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  在此添加您专用的创建代码
	ASSERT(NULL != m_pSkinEngine);
	if (NULL != m_pSkinEngine)
	{
		HRESULT hr = m_pSkinEngine->LoadSkin(GetSafeHwnd(),L"SofaNoFlashDlg");
		ASSERT(hr== S_OK && __LINE__);
	}

	return 0;
}

LRESULT CSofaNoFlashDlg::OnSkinMessage( WPARAM wParam,LPARAM lParam )
{
	switch(wParam)
	{
	case SKIN_ON_CLICK_BTN_GETFLASH:
		{
			common::utility::systemhelper::OpenUrl(URL_GETFLASH);
		}
		break;
	default:
		break;
	}

	return TRUE;
}