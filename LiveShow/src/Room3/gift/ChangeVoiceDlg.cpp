// ChangeVoiceDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "..\Room.h"
#include "SkinUtil.h"
#include "ChangeVoiceDlg.h"
#include "..\UserMessageDefine.h"

#define SKIN_ON_CLICK_CLOSE 1

// CChangeVoiceDlg 对话框

IMPLEMENT_DYNAMIC(CChangeVoiceDlg, CDialog)

CChangeVoiceDlg::CChangeVoiceDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CChangeVoiceDlg::IDD, pParent)
{
	m_pSkinEngine = NULL;
	::GetSkinEngine(&m_pSkinEngine);
}

CChangeVoiceDlg::~CChangeVoiceDlg()
{
}

void CChangeVoiceDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_CHANGEVOICE, m_changeVoiceListCtrl);
}


BEGIN_MESSAGE_MAP(CChangeVoiceDlg, CDialog)
	ON_WM_CREATE()
	ON_SKIN_MESSAGE
END_MESSAGE_MAP()


// CChangeVoiceDlg 消息处理程序

int CChangeVoiceDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  在此添加您专用的创建代码
	ASSERT(m_pSkinEngine != NULL);
	if (NULL != m_pSkinEngine)
	{
		HRESULT hr = m_pSkinEngine->LoadSkin(GetSafeHwnd(),L"ChangeVoiceDlg");
		ASSERT(hr == S_OK && __LINE__);
	}

	return 0;
}

BOOL CChangeVoiceDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	m_changeVoiceListCtrl.Initialize();
	DWORD dwstyle = m_changeVoiceListCtrl.GetExtendedStyle();
	dwstyle = 0;
	m_changeVoiceListCtrl.SetExtendedStyle(dwstyle);
	m_changeVoiceListCtrl.SetItemState(0, LVIS_SELECTED, LVIS_SELECTED);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

LRESULT CChangeVoiceDlg::OnSkinMessage( WPARAM wParam,LPARAM lParam )
{
	switch(wParam)
	{
	case SKIN_ON_CLICK_CLOSE:
		{
			ShowWindow( SW_HIDE ) ;
		}
		break;
	default:break;
	}


	return TRUE;
}

void CChangeVoiceDlg::OnSelChangeVoice( IGiftItem * pGiftItem )
{
	ASSERT(pGiftItem != NULL);

	CWnd * pParentWnd = GetParent();
	if (pParentWnd != NULL)
	{
		pParentWnd->SendMessage(WM_CLICK_CHANGEVOICELIST, 0, (LPARAM)pGiftItem);
	}
	ShowWindow(SW_HIDE);
}