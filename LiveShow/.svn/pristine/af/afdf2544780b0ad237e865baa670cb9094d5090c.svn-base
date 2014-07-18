// RoomBottomLeftDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "Room.h"
#include "RoomBottomLeftDlg.h"
#include "skin_i.h"
#include "SkinUtil.h"
#include "selfinfoandset\SelfInfoAndSetDlg.h"
#include "userlist\UserListMenu.h"
#include "..\CommonLibrary\include\utility\SystemHelper.h"
#include "..\CommonLibrary\include\utility\StringHelper.h"

#define  SKIN_NOTIFY_MARK_DLG				1
#define  SKIN_NOTIFY_SEARCH_DLG_HWND		2
#define  SKIN_ON_CLICK_TABBTN_MGR			3
#define  SKIN_ON_CLICK_TABBTN_USER			4
#define  SKIN_ON_CLICK_TABBTN_MIC			5
#define  SKIN_ON_CLICK_BTN_SEARCH			6
#define  SKIN_ON_CLICK_ICON_INSTRUCTION		7
#define  SKIN_ON_CLICK_BTN_WHEREIAM			8
#define  SKIN_NOTIFY_SEARCH_LIST			9
#define  SKIN_NOTIFY_SHOW_USERITEM			10
#define  SKIN_NOTIFY_SELFINFOANDSET_DLG     11

// CRoomBottomLeftDlg 对话框

IMPLEMENT_DYNAMIC(CRoomBottomLeftDlg, CDialog)

CRoomBottomLeftDlg::CRoomBottomLeftDlg(CRoomParentDlg* pParentDlg, CWnd* pParent /*=NULL*/)
	: CDialog(CRoomBottomLeftDlg::IDD, pParent),
	m_pParentDlg(pParentDlg)
{
	m_pSkinEngine = NULL;
	::GetSkinEngine(&m_pSkinEngine);

	m_pRoomBusiness = NULL;

	m_pSelfInfoAndSetDlg = NULL;
	m_pSearchDlg = NULL;
	//m_pIconIntroducDlg = NULL;
}

CRoomBottomLeftDlg::~CRoomBottomLeftDlg()
{
	SAFE_DELETE_DIALOG(m_pSelfInfoAndSetDlg);
}

void CRoomBottomLeftDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_ROOM_USER_LIST, m_UserListCtrl);
	DDX_Control(pDX, IDC_ROOM_MANAGER_LIST, m_ManagerListCtrl);
	DDX_Control(pDX, IDC_ROOM_MIC_LIST, m_MicListCtrl);
	DDX_Control(pDX, IDC_ROOM_SEARCH_LIST, m_SearchListCtrl);
	DDX_Control(pDX, IDC_ROOM_AGENT_LIST, m_AgentListCtrl);
}


BEGIN_MESSAGE_MAP(CRoomBottomLeftDlg, CDialog)
	ON_WM_CREATE()
	ON_SKIN_MESSAGE
	ON_MESSAGE(WM_SEARCHEDIT_CHANGE, OnSearchEditChange)
	ON_MESSAGE(WM_CLICK_USERLIST, OnClickUserList)
	ON_MESSAGE(WM_DBCLICK_USERLIST, OnDBClickUserList)
	ON_NOTIFY(NM_RCLICK, IDC_ROOM_USER_LIST, OnNMRclickRoomUserList)
	ON_NOTIFY(NM_RCLICK, IDC_ROOM_MANAGER_LIST, OnNMRclickRoomManagerList)
	ON_NOTIFY(NM_RCLICK, IDC_ROOM_MIC_LIST, OnNMRclickRoomMicList)
	ON_NOTIFY(NM_RCLICK, IDC_ROOM_SEARCH_LIST, OnNMRclickRoomSearchList)
	ON_NOTIFY(NM_RCLICK, IDC_ROOM_AGENT_LIST, OnNMRclickRoomAgentList)
END_MESSAGE_MAP()


// CRoomBottomLeftDlg 消息处理程序

int CRoomBottomLeftDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  在此添加您专用的创建代码
	//LoadSkin
	ASSERT(NULL != m_pSkinEngine);
	if (NULL != m_pSkinEngine)
	{
		HRESULT hr = m_pSkinEngine->LoadSkin(GetSafeHwnd(),L"RoomBottomLeftDlg");
		ASSERT(hr== S_OK && __LINE__);
	}

	return 0;
}

BOOL CRoomBottomLeftDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	//个人信息及操作设置窗口
	m_pSelfInfoAndSetDlg = new CSelfInfoAndSetDlg(m_pParentDlg, this);
	m_pSelfInfoAndSetDlg->Create(CSelfInfoAndSetDlg::IDD, this);
	NOTIFY_SKIN_HELPER(SKIN_NOTIFY_SELFINFOANDSET_DLG, m_pSelfInfoAndSetDlg->GetSafeHwnd());

	//用户列表初始化
	m_UserListCtrl.InitUserListCtrl(L"", false );				//用户列表初始化
	m_ManagerListCtrl.InitUserListCtrl(L"");					//管理列表初始化
	m_MicListCtrl.InitUserListCtrl(L"");						//麦序列表初始化
	m_SearchListCtrl.InitUserListCtrl(L"");						//搜索列表初始化
	m_AgentListCtrl.InitUserListCtrl(L"");						//销售列表初始化

	m_UserListCtrl.ShowWindow(SW_SHOW);							//默认显示的是用户列表
	m_ManagerListCtrl.ShowWindow(SW_HIDE);
	m_MicListCtrl.ShowWindow(SW_HIDE);
	m_SearchListCtrl.ShowWindow(SW_HIDE);
	m_AgentListCtrl.ShowWindow(SW_HIDE);

	//搜索框
	m_pSearchDlg = new CSearchDlg;
	if (m_pSearchDlg == NULL || ! m_pSearchDlg->Create(CSearchDlg::IDD, this))
		return -1;

	if (m_pSearchDlg->GetSafeHwnd() != NULL)
	{
		NOTIFY_SKIN_HELPER(SKIN_NOTIFY_SEARCH_DLG_HWND, m_pSearchDlg->GetSafeHwnd());
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


LRESULT CRoomBottomLeftDlg::OnSkinMessage(WPARAM wParam,LPARAM lParam)
{
	switch(wParam)
	{
	case SKIN_ON_CLICK_TABBTN_MGR:
		{
			OnClickTabBtnMgr();
		}
		break;
	case SKIN_ON_CLICK_TABBTN_USER:
		{
			OnClickTabBtnUser();
		}
		break;
	case SKIN_ON_CLICK_TABBTN_MIC:
		{
			OnClickTabBtnMic();
		}
		break;
	case SKIN_ON_CLICK_BTN_SEARCH:
		{
			OnClickBtnSearch((int)lParam);
		}
		break;
	case SKIN_ON_CLICK_ICON_INSTRUCTION:
		{
			//OnClickBtnIconIntro((int)lParam);
			OnClickTabBtnAgt();
		}
		break;
	case SKIN_ON_CLICK_BTN_WHEREIAM:
		{
			OnClickBtnWhereIAm();
		}
		break;
	default:break;
	}

	return TRUE;
}

UserManager * CRoomBottomLeftDlg::GetUserManager()
{
	if (m_pRoomBusiness != NULL)
	{
		return m_pRoomBusiness->GetUserManager();
	}

	return NULL;
}

IUserItem * CRoomBottomLeftDlg::GetSelfUserItem()
{
	if (GetUserManager() != NULL)
	{
		return GetUserManager()->GetSelfUserItem();
	}

	return NULL;
}

void CRoomBottomLeftDlg::OnClickOrderSong()
{
	m_pParentDlg->OnClickOrderSong();
}

void CRoomBottomLeftDlg::SetBottomUserInfo(IUserItem* pUserItem)
{
	if (pUserItem == NULL)
	{
		return;
	}
	if (m_pSelfInfoAndSetDlg == NULL || m_pSelfInfoAndSetDlg->GetSafeHwnd() == NULL)
	{
		return;
	}
	m_pSelfInfoAndSetDlg->SetBottomUserInfo(pUserItem);
}

void CRoomBottomLeftDlg::OnAddUserIsMgr( IUserItem * pUserItem )
{
	m_pSelfInfoAndSetDlg->OnAddUser(pUserItem);
}

void CRoomBottomLeftDlg::OnUpdateUserIsMgr( IUserItem * pUserItem )
{
	m_pSelfInfoAndSetDlg->OnUpdateUser(pUserItem);
}

void CRoomBottomLeftDlg::OnClickTabBtnMgr()
{
	if (m_SearchListCtrl.IsWindowVisible())
	{
		SearchUserList(m_pSearchDlg->m_strEdit.GetBuffer(0), USERLISTTYPE_MANAGER);
	}
	else
	{
		m_UserListCtrl.ShowWindow(SW_HIDE);
		m_ManagerListCtrl.ShowWindow(SW_SHOW);
		m_MicListCtrl.ShowWindow(SW_HIDE);
		m_AgentListCtrl.ShowWindow(SW_HIDE);
	}
}

void CRoomBottomLeftDlg::OnClickTabBtnUser()
{
	if (m_SearchListCtrl.IsWindowVisible())
	{
		SearchUserList(m_pSearchDlg->m_strEdit.GetBuffer(0), USERLISTTYPE_ALL);
	}
	else
	{
		m_UserListCtrl.ShowWindow(SW_SHOW);
		m_ManagerListCtrl.ShowWindow(SW_HIDE);
		m_MicListCtrl.ShowWindow(SW_HIDE);
		m_AgentListCtrl.ShowWindow(SW_HIDE);
	}
}

void CRoomBottomLeftDlg::OnClickTabBtnMic()
{
	if (m_SearchListCtrl.IsWindowVisible())
	{
		SearchUserList(m_pSearchDlg->m_strEdit.GetBuffer(0), USERLISTTYPE_MIC);
	}
	else
	{
		m_UserListCtrl.ShowWindow(SW_HIDE);
		m_ManagerListCtrl.ShowWindow(SW_HIDE);
		m_MicListCtrl.ShowWindow(SW_SHOW);
		m_AgentListCtrl.ShowWindow(SW_HIDE);
	}
}


void CRoomBottomLeftDlg::OnClickTabBtnAgt()
{
	if (m_SearchListCtrl.IsWindowVisible())
	{
		SearchUserList(m_pSearchDlg->m_strEdit.GetBuffer(0), USERLISTTYPE_AGENT);
	}
	else
	{
		m_UserListCtrl.ShowWindow(SW_HIDE);
		m_ManagerListCtrl.ShowWindow(SW_HIDE);
		m_MicListCtrl.ShowWindow(SW_HIDE);
		m_AgentListCtrl.ShowWindow(SW_SHOW);
	}
}

void CRoomBottomLeftDlg::OnClickBtnSearch(int checked)
{
	if (0 == checked)
	{
		if (m_pSearchDlg != NULL)
		{
			m_pSearchDlg->OnLogOff();
		}
	}
}

void CRoomBottomLeftDlg::SearchUserList( std::wstring strKey, ENM_UserListType enmListType )
{
	m_SearchListCtrl.ShowWindow(SW_SHOW);
	std::vector<IUserItem*>  vecUserItem;
	if (GetSelfUserItem() != NULL)
	{
		GetUserManager()->SmartSearch(enmListType, strKey, vecUserItem);
	}
	if (vecUserItem.size() != 0)
	{
		m_SearchListCtrl.InsertUserItem(vecUserItem);
	}
	else
	{
		m_SearchListCtrl.DeleteAllItems();
	}
}

void CRoomBottomLeftDlg::UpdateUserListTabInfo()
{
	std::wstring strTab = L"管理/" + common::utility::stringhelper::IntToString(GetManagerListCount());
	CComVariant var(strTab.c_str());
	if(m_pSkinEngine != NULL)
		m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(), L"tabBtnMgr", L"label", var);

	strTab = L"嘉宾/" + common::utility::stringhelper::IntToString(GetUserListCount());
	var = strTab.c_str();
	if(m_pSkinEngine != NULL)
		m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(), L"tabBtnUser", L"label", var);

	strTab = L"麦序/" + common::utility::stringhelper::IntToString(GetMicListCount());
	var = strTab.c_str();
	if(m_pSkinEngine != NULL)
		m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(), L"tabBtnMicList", L"label", var);

	m_UserListCtrl.UpdateGuestCount(GetGuestCount());
}

int CRoomBottomLeftDlg::GetUserListCount()
{
	if (GetUserManager() != NULL)
	{
		return GetUserManager()->GetAllUserCount();
	}

	return 0;
}

int CRoomBottomLeftDlg::GetManagerListCount()
{
	//if (GetUserManager() != NULL)
	//{
	//	return GetUserManager()->GetAdminCount();
	//}
	return m_ManagerListCtrl.GetItemCount();

	//return 0;
}

int CRoomBottomLeftDlg::GetMicListCount()
{
	if (GetUserManager() != NULL)
	{
		return GetUserManager()->GetWaitingCount();
	}

	return 0;
}

int CRoomBottomLeftDlg::GetGuestCount()
{
	if (GetUserManager() != NULL)
	{
		return GetUserManager()->GetGuestCount();
	}

	return 0;
}

void CRoomBottomLeftDlg::OnGetUserList( std::map<uint32, IUserItem*> & mapAllUser )
{
	for (MapUserItem::iterator it = mapAllUser.begin(); it != mapAllUser.end(); ++it)
	{
		IUserItem * pUserItem = it->second;
		m_UserListCtrl.InsertUserItem(pUserItem);
	}
}

void CRoomBottomLeftDlg::OnGetAdminList( std::map<uint32, IUserItem*> & mapAdminUser )
{
	for (MapUserItem::iterator it = mapAdminUser.begin(); it != mapAdminUser.end(); ++it)
	{
		IUserItem * pUserItem = it->second;
		m_ManagerListCtrl.InsertUserItem(pUserItem);
	}
}

void CRoomBottomLeftDlg::OnGetWaitingList( std::map<uint32, IUserItem*> & mapWaitingUser )
{
	for (MapUserItem::iterator it = mapWaitingUser.begin(); it != mapWaitingUser.end(); ++it)
	{
		IUserItem * pUserItem = it->second;
		m_MicListCtrl.InsertUserItem(pUserItem);
	}

	UpdateUserListTabInfo();
}

void CRoomBottomLeftDlg::OnAddUser( IUserItem * pUserItem )
{
	ASSERT(pUserItem != NULL);
	if (!pUserItem->IsHideLogin() || pUserItem->GetUserUIN() == GetCurInfo().unUIN)
	{
		m_UserListCtrl.InsertUserItem(pUserItem);

		if (pUserItem->GetUserTitleLevel() > core::TITLELEVEL_NONE)
		{
			m_ManagerListCtrl.InsertUserItem(pUserItem);
		}

		if (pUserItem->IsWaiting())
		{
			m_MicListCtrl.InsertUserItem(pUserItem);
		}

		if (pUserItem->IsAgent())
		{
			m_AgentListCtrl.InsertUserItem(pUserItem);
		}
	}

	//保留在RoomRightDlg
	//int nIndex = pUserItem->GetPublicMicIndex() ;
	//if ( nIndex != 0 )
	//{
	//	int iComboIndex = m_GiftSendToCombo.InsertString(0 ,pUserItem->GetUserName().c_str());
	//	m_GiftSendToCombo.SetItemData( iComboIndex ,pUserItem->GetUserUIN());
	//	int nCurSel = m_GiftSendToCombo.GetCurSel() ;
	//	if ( nCurSel != CB_ERR )
	//	{
	//		//默认选中低序公麦
	//		uint32 unUIN = m_GiftSendToCombo.GetItemData( nCurSel ) ;
	//		IUserItem* pCurSelItem = GetUserManager()->SearchUserByUIN( unUIN ) ;
	//		if ( pCurSelItem != NULL )
	//		{
	//			int nCurSelIndex = pCurSelItem->GetPublicMicIndex() ;
	//			if ( nCurSelIndex != 0 && nCurSelIndex < nIndex )
	//			{
	//				iComboIndex = nCurSel ;
	//			}
	//		}
	//	}
	//	m_GiftSendToCombo.SetCurSel( iComboIndex );		
	//}

}

void CRoomBottomLeftDlg::OnUpdateUser( IUserItem * pUserItem )
{
	ASSERT(pUserItem != NULL);
	m_UserListCtrl.UpdateUserItem(pUserItem);

	if (pUserItem->GetUserTitleLevel() > core::TITLELEVEL_NONE)
	{
		m_ManagerListCtrl.UpdateUserItem(pUserItem);
	}

	if (pUserItem->IsWaiting())
	{
		m_MicListCtrl.UpdateUserItem(pUserItem);
	}

	if (pUserItem->IsAgent())
	{
		m_AgentListCtrl.UpdateUserItem(pUserItem);
	}
}

void CRoomBottomLeftDlg::OnDeleteUser( IUserItem * pUserItem )
{
	ASSERT(pUserItem != NULL);
	m_UserListCtrl.DeleteUserItem(pUserItem);

	if (pUserItem->GetUserTitleLevel() > core::TITLELEVEL_NONE)
	{
		m_ManagerListCtrl.DeleteUserItem(pUserItem);
	}

	if (pUserItem->IsWaiting())
	{
		m_MicListCtrl.DeleteUserItem(pUserItem);
	}

	if (pUserItem->IsAgent())
	{
		m_AgentListCtrl.DeleteUserItem(pUserItem);
	}

	m_SearchListCtrl.DeleteUserItem(pUserItem);

	//保留在RoomRightDlg
	//int nIndex = m_GiftSendToCombo.FindString( 0, pUserItem->GetUserName().c_str());
	//if (nIndex != CB_ERR)
	//{
	//	m_GiftSendToCombo.DeleteString(nIndex);
	//	m_GiftSendToCombo.Invalidate(TRUE);
	//}
}

void CRoomBottomLeftDlg::OnAddAdminUser( IUserItem * pUserItem )
{
	ASSERT(pUserItem != NULL);
	m_ManagerListCtrl.InsertUserItem(pUserItem);
}

void CRoomBottomLeftDlg::OnDeleteAdminUser( IUserItem * pUserItem )
{
	ASSERT(pUserItem != NULL);
	m_ManagerListCtrl.DeleteUserItem(pUserItem);
}

void CRoomBottomLeftDlg::OnAddWaitingUser( IUserItem * pUserItem )
{
	ASSERT(pUserItem != NULL);
	m_MicListCtrl.InsertUserItem(pUserItem);
}

void CRoomBottomLeftDlg::OnDeleteWaitingUser( IUserItem * pUserItem )
{
	ASSERT(pUserItem != NULL);
	m_MicListCtrl.DeleteUserItem(pUserItem);
}

void CRoomBottomLeftDlg::OnUserListCountChange()
{
	UpdateUserListTabInfo();
}

void CRoomBottomLeftDlg::ShowUserItem( IUserItem * pUserItem )
{
	ASSERT(pUserItem != NULL);
	m_UserListCtrl.MoveToUserItem(pUserItem);
	NOTIFY_SKIN_HELPER(SKIN_NOTIFY_SHOW_USERITEM,(LONG)0);
}

void CRoomBottomLeftDlg::OnGetUserListComplete()
{
	m_UserListCtrl.SetRedraw(TRUE);
}

void CRoomBottomLeftDlg::OnGetUserListBegin()
{
	m_UserListCtrl.SetRedraw(FALSE);
}

void CRoomBottomLeftDlg::OnClickBtnWhereIAm()
{
	IUserItem * pSelfUserItem = GetSelfUserItem();
	if (pSelfUserItem != NULL)
	{
		if (m_UserListCtrl.IsWindowVisible())
		{
			m_UserListCtrl.MoveToUserItem(pSelfUserItem);
		}
		else if (m_ManagerListCtrl.IsWindowVisible())
		{
			m_ManagerListCtrl.MoveToUserItem(pSelfUserItem);
		}
		else if (m_MicListCtrl.IsWindowVisible())
		{
			m_MicListCtrl.MoveToUserItem(pSelfUserItem);
		}
		else if (m_SearchListCtrl.IsWindowVisible())
		{
			m_SearchListCtrl.MoveToUserItem(pSelfUserItem);
		}
		else if (m_AgentListCtrl.IsWindowVisible())
		{
			m_AgentListCtrl.MoveToUserItem(pSelfUserItem);
		}
	}
}

void CRoomBottomLeftDlg::SetRoomBusiness( RoomBusiness * pRoomBusiness )
{
	ASSERT(pRoomBusiness != NULL);
	m_pRoomBusiness = pRoomBusiness;
}

LRESULT CRoomBottomLeftDlg::OnSearchEditChange( WPARAM wParam ,LPARAM lParam )
{
	CString strTemp = (LPCTSTR)lParam;
	if (strTemp != L"")
	{
		ENM_UserListType enmType = USERLISTTYPE_ALL;
		if (m_UserListCtrl.IsWindowVisible())
		{
			enmType = USERLISTTYPE_ALL;
			m_UserListCtrl.ShowWindow(SW_HIDE);
		}
		else if (m_ManagerListCtrl.IsWindowVisible())
		{
			enmType = USERLISTTYPE_MANAGER;
			m_ManagerListCtrl.ShowWindow(SW_HIDE);
		}
		else if (m_MicListCtrl.IsWindowVisible())
		{
			enmType = USERLISTTYPE_MIC;
			m_MicListCtrl.ShowWindow(SW_HIDE);
		}
		else if (m_AgentListCtrl.IsWindowVisible())
		{
			enmType = USERLISTTYPE_AGENT;
			m_AgentListCtrl.ShowWindow(SW_HIDE);
		}
		else if (m_SearchListCtrl.IsWindowVisible())
		{
			CComVariant var;
			if(m_pSkinEngine != NULL)
				m_pSkinEngine->GetSubSkinProp(GetSafeHwnd(), L"tabBtnUser", L"checked", &var);
			if (var.boolVal)
			{
				enmType = USERLISTTYPE_ALL;
			}
			if(m_pSkinEngine != NULL)
				m_pSkinEngine->GetSubSkinProp(GetSafeHwnd(), L"tabBtnMgr", L"checked", &var);
			if (var.boolVal)
			{
				enmType = USERLISTTYPE_MANAGER;
			}
			if(m_pSkinEngine != NULL)
				m_pSkinEngine->GetSubSkinProp(GetSafeHwnd(), L"tabBtnMicList", L"checked", &var);
			if (var.boolVal)
			{
				enmType = USERLISTTYPE_MIC;
			}
		}

		SearchUserList(strTemp.GetBuffer(0), enmType);
	}
	else
	{
		if (m_SearchListCtrl.IsWindowVisible() && strTemp == L"")
		{
			NOTIFY_SKIN_HELPER(SKIN_NOTIFY_SEARCH_LIST, (LONG)0);
		}
	}
	return S_OK;
}

LRESULT CRoomBottomLeftDlg::OnClickUserList( WPARAM wParam, LPARAM lParam )
{
	IUserItem * pUserItem = (IUserItem*)lParam;
	ASSERT(pUserItem != NULL);
	m_pParentDlg->SendMessage(WM_CLICK_USERLIST, wParam , lParam);

	//点击销售列表的销售图标
	if (wParam == 0 && m_AgentListCtrl.IsWindowVisible())
	{
		CString strUrl;
		strUrl.Format(L"%s&saler=%d", m_pParentDlg->GetRoomBusiness()->GetRoomPayUrl(), pUserItem->GetUser179ID());
		common::utility::systemhelper::OpenUrl( strUrl.GetBuffer() ) ;
	}

	core::CurInfo myCurInfo = GetCurInfo() ;
	if (pUserItem->GetUserUIN() == myCurInfo.unUIN)
	{
		return S_OK;
	}
	//SetSendObj( pUserItem->GetUserName().c_str() , pUserItem->GetUserUIN() ) ;
	return S_OK;
}

LRESULT CRoomBottomLeftDlg::OnDBClickUserList( WPARAM wParam, LPARAM lParam )
{
	IUserItem * pUserItem = (IUserItem*)lParam;
	ASSERT(pUserItem != NULL);
	m_pParentDlg->SendMessage(WM_DBCLICK_USERLIST, wParam , lParam);

	return S_OK;
}

void CRoomBottomLeftDlg::OnNMRclickRoomUserList(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 在此添加控件通知处理程序代码
	NMITEMACTIVATE * pListNotify = (NMITEMACTIVATE*)pNMHDR;

	if (pListNotify->iItem != -1)
	{
		IUserItem * pUserItem = (IUserItem*)m_UserListCtrl.GetItemData(pListNotify->iItem);
		UserListMenu * pMenu = new UserListMenu(m_pParentDlg);
		if (pUserItem != NULL)
		{
			m_pParentDlg->m_pSelectUserItem = pUserItem;

			IUserItem * pSelfItem = GetSelfUserItem();
			if (pSelfItem != NULL)
			{
				pMenu->PopupMenu(pSelfItem, pUserItem);
			}
		}
		else
		{
			pMenu->PopupGuestMenu();
		}

		delete pMenu;
		pMenu = NULL;
	}
	*pResult = 0;
}

void CRoomBottomLeftDlg::OnNMRclickRoomManagerList(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 在此添加控件通知处理程序代码
	NMITEMACTIVATE * pListNotify = (NMITEMACTIVATE*)pNMHDR;
	if (pListNotify->iItem != -1)
	{
		IUserItem * pUserItem = (IUserItem*)m_ManagerListCtrl.GetItemData(pListNotify->iItem);
		if (pUserItem != NULL)
		{
			m_pParentDlg->m_pSelectUserItem = pUserItem;
			UserListMenu * pMenu = new UserListMenu(m_pParentDlg);
			IUserItem * pSelfItem = GetSelfUserItem();
			if (pSelfItem != NULL && pSelfItem->GetUserTitleLevel() > core::TITLELEVEL_NONE)
			{
				pMenu->PopupMenu(pSelfItem, pUserItem);
			}

			delete pMenu;
			pMenu = NULL;
		}
	}
	*pResult = 0;
}

void CRoomBottomLeftDlg::OnNMRclickRoomMicList(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 在此添加控件通知处理程序代码
	NMITEMACTIVATE * pListNotify = (NMITEMACTIVATE*)pNMHDR;
	if (pListNotify->iItem != -1)
	{
		IUserItem * pUserItem = (IUserItem*)m_MicListCtrl.GetItemData(pListNotify->iItem);
		if (pUserItem != NULL)
		{
			m_pParentDlg->m_pSelectUserItem = pUserItem;
			UserListMenu * pMenu = new UserListMenu(m_pParentDlg);
			IUserItem * pSelfItem = GetSelfUserItem();
			if (pSelfItem != NULL && pSelfItem->GetUserTitleLevel() > core::TITLELEVEL_NONE)
			{
				pMenu->PopupMenu(pSelfItem, pUserItem);
			}

			delete pMenu;
			pMenu = NULL;
		}
	}
	*pResult = 0;
}

void CRoomBottomLeftDlg::OnNMRclickRoomSearchList(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 在此添加控件通知处理程序代码
	NMITEMACTIVATE * pListNotify = (NMITEMACTIVATE*)pNMHDR;
	if (pListNotify->iItem != -1)
	{
		IUserItem * pUserItem = (IUserItem*)m_SearchListCtrl.GetItemData(pListNotify->iItem);
		if (pUserItem != NULL)
		{
			m_pParentDlg->m_pSelectUserItem = pUserItem;
			UserListMenu * pMenu = new UserListMenu(m_pParentDlg);
			IUserItem * pSelfItem = GetSelfUserItem();
			if (pSelfItem != NULL)
			{
				pMenu->PopupMenu(pSelfItem, pUserItem);
			}

			delete pMenu;
			pMenu = NULL;
		}
	}
	*pResult = 0;
}

void CRoomBottomLeftDlg::OnCycleTrigger()
{
	ImageManager::getInstance()->DrawAnimation();

	if (m_UserListCtrl.GetSafeHwnd() && m_UserListCtrl.IsWindowVisible() && !m_pParentDlg->IsCoverWndVisible() && !m_pParentDlg->IsGuideWndVisible())
	{
		m_UserListCtrl.DrawAnimation();
		return;
	}
	if (m_ManagerListCtrl.GetSafeHwnd() && m_ManagerListCtrl.IsWindowVisible() && !m_pParentDlg->IsCoverWndVisible() && !m_pParentDlg->IsGuideWndVisible())
	{
		m_ManagerListCtrl.DrawAnimation();
		return;
	}
	if (m_ManagerListCtrl.GetSafeHwnd() && m_MicListCtrl.IsWindowVisible() && !m_pParentDlg->IsCoverWndVisible() && !m_pParentDlg->IsGuideWndVisible())
	{
		m_MicListCtrl.DrawAnimation();
		return;
	}
	if (m_SearchListCtrl.GetSafeHwnd() && m_SearchListCtrl.IsWindowVisible() && !m_pParentDlg->IsCoverWndVisible() && !m_pParentDlg->IsGuideWndVisible())
	{
		m_SearchListCtrl.DrawAnimation();
		return;
	}
}

void CRoomBottomLeftDlg::OnClickIconIntroClose()
{
	CComVariant var = FALSE;
	if(m_pSkinEngine != NULL)
		m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(),L"btnIconInstruction",L"checked",var);
}

void CRoomBottomLeftDlg::OnClickNameLink( CString& strName , uint32 unUUID )
{
	IUserItem * pUserItem = GetUserManager()->SearchUserByUIN(unUUID);
	if (pUserItem != NULL)
	{
		if (m_UserListCtrl.GetSafeHwnd() && m_UserListCtrl.IsWindowVisible())
		{
			m_UserListCtrl.MoveToUserItem(pUserItem);
			return;
		}
		if (m_ManagerListCtrl.GetSafeHwnd() && m_ManagerListCtrl.IsWindowVisible())
		{
			m_ManagerListCtrl.MoveToUserItem(pUserItem);
			return;
		}
		if (m_ManagerListCtrl.GetSafeHwnd() && m_MicListCtrl.IsWindowVisible())
		{
			m_MicListCtrl.MoveToUserItem(pUserItem);
			return;
		}
		if (m_SearchListCtrl.GetSafeHwnd() && m_SearchListCtrl.IsWindowVisible())
		{
			m_SearchListCtrl.MoveToUserItem(pUserItem);
			return;
		}
	}
}

void CRoomBottomLeftDlg::OnNMRclickRoomAgentList( NMHDR *pNMHDR, LRESULT *pResult )
{
	// TODO: 在此添加控件通知处理程序代码
	NMITEMACTIVATE * pListNotify = (NMITEMACTIVATE*)pNMHDR;

	if (pListNotify->iItem != -1)
	{
		IUserItem * pUserItem = (IUserItem*)m_UserListCtrl.GetItemData(pListNotify->iItem);
		UserListMenu * pMenu = new UserListMenu(m_pParentDlg);
		if (pUserItem != NULL)
		{
			m_pParentDlg->m_pSelectUserItem = pUserItem;

			IUserItem * pSelfItem = GetSelfUserItem();
			if (pSelfItem != NULL)
			{
				pMenu->PopupMenu(pSelfItem, pUserItem);
			}
		}
		else
		{
			pMenu->PopupGuestMenu();
		}

		delete pMenu;
		pMenu = NULL;
	}
	*pResult = 0;
}