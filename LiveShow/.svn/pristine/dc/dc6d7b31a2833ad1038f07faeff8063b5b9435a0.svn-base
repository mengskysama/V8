// TheMoreDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "..\Room.h"
#include "TheMoreDlg.h"
#include "Skin_i.h"
#include "SkinUtil.h"
#include "..\RoomParentDlg.h"
#include "..\RoomMidDlg.h"

#define  SKIN_ON_CLICK_BTN_CLEANSCREEN			1
#define  SKIN_ON_CLICK_BTN_STAMP				2
#define  SKIN_ON_CLICK_BTN_COPYURL				3

// CTheMoreDlg 对话框

IMPLEMENT_DYNAMIC(CTheMoreDlg, CDialog)

CTheMoreDlg::CTheMoreDlg(CRoomParentDlg* pRoomParentDlg, CWnd* pParent /*=NULL*/)
	: CDialog(CTheMoreDlg::IDD, pParent),
	  m_pRoomParentDlg(pRoomParentDlg)
{
	m_pSkinEngine = NULL;
	::GetSkinEngine(&m_pSkinEngine);
}

CTheMoreDlg::~CTheMoreDlg()
{
}

void CTheMoreDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CTheMoreDlg, CDialog)
	ON_WM_CREATE()
	ON_SKIN_MESSAGE
END_MESSAGE_MAP()


// CTheMoreDlg 消息处理程序

int CTheMoreDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  在此添加您专用的创建代码
	ASSERT(m_pSkinEngine != NULL);
	if (NULL != m_pSkinEngine)
	{
		HRESULT hr = m_pSkinEngine->LoadSkin(GetSafeHwnd(),L"TheMoreDlg");
		ASSERT(hr == S_OK && __LINE__);
	}
	ModifyStyleEx( WS_EX_APPWINDOW | WS_EX_TOOLWINDOW, 0 ) ;
	return 0;
}

BOOL CTheMoreDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

BOOL CTheMoreDlg::PreTranslateMessage(MSG* pMsg)
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

LRESULT CTheMoreDlg::OnSkinMessage(WPARAM wParam,LPARAM lParam)
{
	switch(wParam)
	{
	case SKIN_ON_CLICK_BTN_STAMP:
		{
			OnClickBtnStamp();
			ShowWindow(SW_HIDE);
		}
		break;
	case SKIN_ON_CLICK_BTN_CLEANSCREEN:
		{
			OnClickBtnCleanScreen();
			ShowWindow(SW_HIDE);
		}
		break;
	case SKIN_ON_CLICK_BTN_COPYURL:
		{
			OnClickBtnCopyUrl();
			ShowWindow(SW_HIDE);
		}
		break;
	default:break;
	}

	return TRUE;
}

void CTheMoreDlg::OnClickBtnStamp()
{
	ASSERT(m_pRoomParentDlg != NULL);

	m_pRoomParentDlg->OnClickStamp();
}

void CTheMoreDlg::OnClickBtnCleanScreen()
{
	ASSERT(m_pRoomParentDlg != NULL);

	m_pRoomParentDlg->GetRoomMidDlg()->OnClickBtnCleanScreen();
}

void CTheMoreDlg::OnClickBtnCopyUrl()
{
	ASSERT(m_pRoomParentDlg != NULL);

	m_pRoomParentDlg->GetHtmlOutPutManager()->OnCopyRoomUrl();	
}