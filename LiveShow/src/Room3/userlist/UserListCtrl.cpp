#include "StdAfx.h"
#include "UserListCtrl.h"
#include "utility/StringHelper.h"
#include "ListIconManager.h"
#include "..\gift/GiftManager.h"
#include "..\GlobalManager.h"
#include "..\UserMessageDefine.h"

#define DYNAMIC_HSCROLLBAR_SHOW_TIMER 1

CUserListCtrl::CUserListCtrl(void)
{
	m_strRoomName = L"";
	m_bShowRoomName = false;
	m_bTimerRunning = false;
}

CUserListCtrl::~CUserListCtrl(void)
{

}

BEGIN_MESSAGE_MAP(CUserListCtrl, CSkinListCtrl)
	ON_NOTIFY_REFLECT(NM_CLICK, &CUserListCtrl::OnNMClick)
	ON_NOTIFY_REFLECT(NM_DBLCLK, &CUserListCtrl::OnNMDblclk)
	ON_WM_SIZE()
	ON_WM_TIMER()
END_MESSAGE_MAP()

bool CUserListCtrl::InitUserListCtrl(std::wstring strRoomName, bool bShowRoomName/* = false*/)
{
	DWORD dwStyle = 0;
	LONG lStyle;
	lStyle = GetWindowLong(m_hWnd, GWL_STYLE);//获取当前窗口style
	lStyle = lStyle | LVS_REPORT; //设置style 
	SetWindowLong(m_hWnd, GWL_STYLE, lStyle);//设置style
	dwStyle = GetExtendedStyle();
	dwStyle = dwStyle|LVS_EX_FULLROWSELECT | LVS_NOCOLUMNHEADER | LVS_EX_TRACKSELECT /*| LVS_EX_INFOTIP*//*|LVS_EX_GRIDLINES*/;//LVS_EX_TRACKSELECT这个是用于鼠标HOVER时
	dwStyle &= ~LVS_SHOWSELALWAYS;
	SetExtendedStyle(dwStyle); //设置扩展风格 

	//m_narrColumnWidth[0] = 21;
	//m_narrColumnWidth[1] = 40;
	//m_narrColumnWidth[2] = 21;
	//m_narrColumnWidth[3] = 21;
	//m_narrColumnWidth[4] = 87;
	//m_narrColumnWidth[5] = 65;

	m_narrColumnWidth[0] = 21;
	m_narrColumnWidth[1] = 37;
	m_narrColumnWidth[2] = 18;
	m_narrColumnWidth[3] = 18;
	m_narrColumnWidth[4] = 85;
	m_narrColumnWidth[5] = 76;

	InsertColumn(0,L"VIP",LVCFMT_CENTER,m_narrColumnWidth[0]);
	InsertColumn(1,L"MIC",LVCFMT_CENTER,m_narrColumnWidth[1]);    
	InsertColumn(2,L"LEVEL",LVCFMT_CENTER,m_narrColumnWidth[2]);
	InsertColumn(3,L"PIC",LVCFMT_CENTER,m_narrColumnWidth[3]);  
	InsertColumn(4,L"NAME",LVCFMT_LEFT,m_narrColumnWidth[4]);
	InsertColumn(5,L"ID",LVCFMT_LEFT,m_narrColumnWidth[5]);

	m_strRoomName = strRoomName;
	m_bShowRoomName = bShowRoomName;

	return true;
}

bool CUserListCtrl::InsertUserItem( IUserItem * pUserItem )
{
	ASSERT(pUserItem != NULL);

	LVFINDINFO FindInfo;
	memset(&FindInfo, 0, sizeof(FindInfo));
	FindInfo.flags = LVFI_PARAM;
	FindInfo.lParam = (LPARAM)pUserItem;
	int iItem = FindItem(&FindInfo);
	if (iItem != -1)
	{
		UpdateUserItem(pUserItem);
		return true;
	}

	int nRow = GetItemCount();
	iItem = InsertItem(LVIF_TEXT | LVIF_PARAM, nRow, GetDisplayText(pUserItem, 0).c_str(), 0, 0, 0, (LPARAM)pUserItem);
	for (int i = 1; i < COLUMN_COUT; i++)
	{
		SetItemText(iItem, i, GetDisplayText(pUserItem, i).c_str());
	}

	IGiftItem * pGiftItem = GiftManager::GetInstance()->SearchStampByID(pUserItem->GetUserStampID());
	if (pGiftItem != NULL)
	{
		ImageManager::getInstance()->AddStamp(pGiftItem->GetID(), pGiftItem->GetSmallImagePath());
	}

	if(pUserItem->GetUserVipLevel() >= core::VIPLEVEL_VIP ||
		pUserItem->GetUserRichLevel() > 0)
	{
		UpdateUserList();
	}

	UpdateColumnWidth();

	return true;
}

bool CUserListCtrl::InsertUserItem( std::vector<IUserItem*> vecUserItem )
{
	DeleteAllItems();
	for (std::vector<IUserItem*>::iterator it = vecUserItem.begin(); it != vecUserItem.end(); ++it)
	{
		IUserItem * pUserItem = *it;
		if (pUserItem != NULL)
		{
			InsertUserItem(pUserItem);
		}
	}

	UpdateColumnWidth();

	return true;
}

bool CUserListCtrl::UpdateUserItem( IUserItem * pUserItem )
{
	ASSERT(pUserItem != NULL);

	LVFINDINFO FindInfo;
	memset(&FindInfo, 0, sizeof(FindInfo));
	FindInfo.flags = LVFI_PARAM;
	FindInfo.lParam = (LPARAM)pUserItem;
	int iItem = FindItem(&FindInfo);
	if (iItem == -1)
	{
		return false;
	}

	SetItem(iItem, 0, LVIF_TEXT, GetDisplayText(pUserItem, 0).c_str(), 0, 0, 0, 0);
	for (int i = 1; i < COLUMN_COUT; i++)
	{
		SetItemText(iItem, i, GetDisplayText(pUserItem, i).c_str());
	}

	IGiftItem * pGiftItem = GiftManager::GetInstance()->SearchStampByID(pUserItem->GetUserStampID());
	if (pGiftItem != NULL)
	{
		ImageManager::getInstance()->AddStamp(pGiftItem->GetID(), pGiftItem->GetSmallImagePath());
	}
	
	UpdateUserList();

	return true;
}

bool CUserListCtrl::DeleteUserItem( IUserItem * pUserItem )
{
	ASSERT(pUserItem != NULL);

	LVFINDINFO FindInfo;
	memset(&FindInfo, 0, sizeof(FindInfo));
	FindInfo.flags = LVFI_PARAM;
	FindInfo.lParam = (LPARAM)pUserItem;
	int iItem = FindItem(&FindInfo);
	if (iItem != -1)
	{
		DeleteItem(iItem);

		UpdateColumnWidth();

		return true;
	}

	return false;
}

bool CUserListCtrl::UpdateUserList()
{
	SortItems(SortFun, (LPARAM)this);

	return true;
}

std::wstring CUserListCtrl::GetDisplayText( IUserItem * pUserItem, uint32 unColumn )
{
	ASSERT(pUserItem != NULL);

	switch (unColumn)
	{
	case COLUNM_VIP_LEVEL:
		{
			return L"";
		}
		break;
	case COLUNM_MIC_STATUS:
		{
			return L"";
		}
		break;
	case COLUNM_USER_LEVEL:
		{
			return L"";
		}
		break;
	case COLUNM_USER_PIC:
		{
			if (pUserItem->GetUserTitleLevel() > core::TITLELEVEL_TEMP_MANAGER && m_bShowRoomName)
			{
				return m_strRoomName;
			}
			else
			{
				return L"";
			}
		}
		break;
	case COLUNM_USER_NAME:
		{
			return pUserItem->GetUserName();
		}
		break;
	case COLUNM_USER_ID:
		{
			return common::utility::stringhelper::IntToString(pUserItem->GetUser179ID());
		}
		break;
	default:
		break;
	}

	return L"";
}

int CALLBACK CUserListCtrl::SortFun( LPARAM lParam1, LPARAM lParam2, LPARAM lParamList )
{
	IUserItem * pUserItem1 = (IUserItem*)lParam1;
	IUserItem * pUserItem2 = (IUserItem*)lParam2;

	if (pUserItem1 == NULL)
	{
		if (pUserItem2->GetUserVipLevel() == core::VIPLEVEL_NONE)
		{
			return -1;
		}
		return 1;
	}

	if (pUserItem2 == NULL)
	{
		if (pUserItem1->GetUserVipLevel() == core::VIPLEVEL_NONE)
		{
			return 1;
		}
		return -1;
	}

	uint8 vip1 = pUserItem1->GetUserVipLevel();
	uint8 vip2 = pUserItem2->GetUserVipLevel();

	if (vip1 == vip2)
	{
		uint8 rich1 = pUserItem1->GetUserRichLevel();
		uint8 rich2 = pUserItem2->GetUserRichLevel();

		if (rich1 == rich2)
		{
			return (pUserItem1->GetUser179ID() > pUserItem2->GetUser179ID()) ? 1 : -1;
		}
		else
		{
			return (rich1 < rich2) ? 1 : -1;
		}
	}
	else
	{
		return (vip1 < vip2) ? 1 : -1;
	}

	return 0;
}

int CUserListCtrl::FindUserItem( IUserItem * pUserItem )
{
	ASSERT(pUserItem != NULL);

	LVFINDINFO FindInfo;
	memset(&FindInfo, 0, sizeof(FindInfo));
	FindInfo.flags = LVFI_PARAM;
	FindInfo.lParam = (LPARAM)pUserItem;
	return FindItem(&FindInfo);
}

void CUserListCtrl::MoveTo( int nItem )
{
	if (m_iSelectedItem >= 0)
	{
		SetItemState(m_iSelectedItem, 0, LVIS_SELECTED | LVIS_FOCUSED);
	}
	SetItemState(nItem, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);
	m_iSelectedItem = nItem;
	EnsureVisible(nItem, false);
}

void CUserListCtrl::MoveToUserItem( IUserItem * pUserItem )
{
	int nItem = FindUserItem(pUserItem);
	if (nItem != -1)
	{
		MoveTo(nItem);
	}
}


void CUserListCtrl::OnNMClick(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;

	NMLISTVIEW* pMListView = (NMLISTVIEW*) pNMHDR;
	m_iSelectedItem = pMListView->iItem;//获取选中项

	if (m_iSelectedItem >= 0 && m_iSelectedItem < (this->GetItemCount()))
	{
		IUserItem *pUserItem = (IUserItem*)GetItemData( m_iSelectedItem ) ;
		if ( pUserItem != NULL )
		{
			CWnd * pParentWnd = GetParent();
			if (pParentWnd != NULL)
			{
				pParentWnd->SendMessage(WM_CLICK_USERLIST, pMListView->iSubItem, (LPARAM)pUserItem);
			}
		}
	}
}

void CUserListCtrl::OnNMDblclk(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;

	NMLISTVIEW * pMListView = (NMLISTVIEW*) pNMHDR;

	int nItem = pMListView->iItem;
	if (nItem >= 0 && nItem < GetItemCount())
	{
		IUserItem * pUserItem = (IUserItem*)GetItemData(nItem);
		if (pUserItem != NULL)
		{
			CWnd * pParentWnd = GetParent();
			if (pParentWnd != NULL)
			{
				pParentWnd->SendMessage(WM_DBCLICK_USERLIST, pMListView->iSubItem, (LPARAM)pUserItem);
			}
		}
	}
}

void CUserListCtrl::UpdateColumnWidth(bool doRightNow)
{
	if(!doRightNow)
	{
		if(!m_bTimerRunning)
		{
			SetTimer(DYNAMIC_HSCROLLBAR_SHOW_TIMER, 5000, NULL);
			m_bTimerRunning = true;
		}
	}
	else
	{
		if(GetCountPerPage() < GetItemCount())
		{
			m_narrColumnWidth[5] = 59;
			SetColumnWidth(5, m_narrColumnWidth[5]);
		}
		else
		{
			m_narrColumnWidth[5] = 76;
			SetColumnWidth(5, m_narrColumnWidth[5]);
		}
	}
}

void CUserListCtrl::OnSize(UINT nType, int cx, int cy)
{
	CSkinListCtrl::OnSize(nType, cx, cy);

	SetTimer(DYNAMIC_HSCROLLBAR_SHOW_TIMER, 50, NULL);
}

void CUserListCtrl::OnTimer(UINT_PTR nIDEvent)
{
	m_bTimerRunning = false;

	if (DYNAMIC_HSCROLLBAR_SHOW_TIMER == nIDEvent)
	{
		KillTimer(DYNAMIC_HSCROLLBAR_SHOW_TIMER);

		UpdateColumnWidth(true);
	}

	CSkinListCtrl::OnTimer(nIDEvent);
}
