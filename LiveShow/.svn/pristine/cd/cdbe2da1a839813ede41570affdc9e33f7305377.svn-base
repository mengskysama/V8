// AnnouncementDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "..\Room.h"
#include "AnnouncementDlg.h"
#include "Skin_i.h"
#include "SkinUtil.h"
#include "TempAnnounceDlg.h"
#include "RoomAnnounceDlg.h"
#include "..\RoomParentDlg.h"

#define  NOTIFY_SKIN_TEMP_ANNOUNCE_DLG     1
#define  NOTIFY_SKIN_ROOM_ANNOUNCE_DLG     2
#define  SKIN_ON_CLICK_TAB_TEMPANNOUNCE    3
#define  SKIN_ON_CLICK_TAB_ROOMANNOUNCE    4
// CAnnouncementDlg 对话框

IMPLEMENT_DYNAMIC(CAnnouncementDlg, CDialog)

CAnnouncementDlg::CAnnouncementDlg( CRoomParentDlg* pParentDlg , CWnd* pParent /*=NULL*/)
	: CDialog(CAnnouncementDlg::IDD, pParent),
	m_pParentDlg(pParentDlg)
{
    m_pSkinEngine = NULL;
	::GetSkinEngine(&m_pSkinEngine);

	m_pTempAnnounceDlg = NULL;
	m_pRoomAnnounceDlg = NULL;
}

CAnnouncementDlg::~CAnnouncementDlg()
{
	if (m_pTempAnnounceDlg != NULL)
	{
		delete m_pTempAnnounceDlg;
		m_pTempAnnounceDlg = NULL;
	}
	if (m_pRoomAnnounceDlg != NULL)
	{
		delete m_pRoomAnnounceDlg;
		m_pRoomAnnounceDlg = NULL;
	}
}

void CAnnouncementDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CAnnouncementDlg, CDialog)
	ON_WM_CREATE()
	ON_SKIN_MESSAGE
END_MESSAGE_MAP()


// CAnnouncementDlg 消息处理程序

int CAnnouncementDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  在此添加您专用的创建代码
	ASSERT(m_pSkinEngine != NULL);
	if (NULL != m_pSkinEngine)
	{
		HRESULT hr = m_pSkinEngine->LoadSkin(GetSafeHwnd(),L"AnnouncementDlg");
		ASSERT(hr == S_OK && __LINE__);
	}
    //创建临时公告子对话框
    m_pTempAnnounceDlg = new CTempAnnounceDlg(m_pParentDlg);
	if (m_pTempAnnounceDlg)
	{
		m_pTempAnnounceDlg->Create(CTempAnnounceDlg::IDD,this);
		m_pTempAnnounceDlg->SetWindowPos(NULL,30,60,355,272,SWP_SHOWWINDOW);
		m_pTempAnnounceDlg->ShowWindow(SW_SHOW);
	}
    //创建房间公告子对话框
	m_pRoomAnnounceDlg = new CRoomAnnounceDlg(m_pParentDlg);
	if (m_pRoomAnnounceDlg)
	{
		m_pRoomAnnounceDlg->Create(CRoomAnnounceDlg::IDD,this);
		m_pRoomAnnounceDlg->SetWindowPos(NULL,30,60,355,272,SWP_SHOWWINDOW);
		m_pRoomAnnounceDlg->ShowWindow(SW_HIDE);
	}


	return 0;
}


LRESULT CAnnouncementDlg::OnSkinMessage(WPARAM wParam, LPARAM lParam)
{
    switch(wParam)
	{
	case SKIN_ON_CLICK_TAB_TEMPANNOUNCE:
		{
           m_pRoomAnnounceDlg->ShowWindow(SW_HIDE);
		   m_pTempAnnounceDlg->ShowWindow(SW_SHOW);
		}
		break;
	case SKIN_ON_CLICK_TAB_ROOMANNOUNCE:
		{
           m_pTempAnnounceDlg->ShowWindow(SW_HIDE);
		   m_pRoomAnnounceDlg->ShowWindow(SW_SHOW);
		}
		break;
	default:break;
	}


	return TRUE;
}

BOOL CAnnouncementDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	NotifyCreateSkinItem();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void CAnnouncementDlg::NotifyCreateSkinItem()
{
    if (m_pTempAnnounceDlg->GetSafeHwnd())
    {
		NOTIFY_SKIN_HELPER(NOTIFY_SKIN_TEMP_ANNOUNCE_DLG,m_pTempAnnounceDlg->GetSafeHwnd());
    }
    if (m_pRoomAnnounceDlg->GetSafeHwnd())
    {
		NOTIFY_SKIN_HELPER(NOTIFY_SKIN_ROOM_ANNOUNCE_DLG,m_pRoomAnnounceDlg->GetSafeHwnd());
    }
}
BOOL CAnnouncementDlg::PreTranslateMessage(MSG* pMsg)
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

void CAnnouncementDlg::TurnToTempAnnouncePanel( CString& strText ) 
{
	m_pTempAnnounceDlg->SetInputAnnounce( strText ) ;
	CComVariant var = true;
	m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(),L"tabTempAnnouncement",L"checked",var);
	var = false ;
	m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(),L"tabRoomAnnouncement",L"checked",var);
	m_pRoomAnnounceDlg->ShowWindow(SW_HIDE);
	if ( !m_pTempAnnounceDlg->IsWindowVisible() )
	{
		m_pTempAnnounceDlg->ShowWindow(SW_SHOW);
	}		
}

void CAnnouncementDlg::Refresh()
{
	if (m_pRoomAnnounceDlg != NULL)
	{
		m_pRoomAnnounceDlg->Refresh();
	}
}