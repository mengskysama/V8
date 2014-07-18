#include "StdAfx.h"
#include "AllUserListCtrl.h"
#include "ListIconManager.h"
#include "..\UserMessageDefine.h"
#include "..\RoomRightDlg.h"
#include "..\RoomBottomLeftDlg.h"

CAllUserListCtrl::CAllUserListCtrl(void)
{
	m_bShowGuest = false;
	m_pGuestBtnItem = NULL;
}

CAllUserListCtrl::~CAllUserListCtrl(void)
{
	if (m_pGuestBtnItem)
	{
		delete m_pGuestBtnItem;
		m_pGuestBtnItem = NULL;
	}
}

BEGIN_MESSAGE_MAP(CAllUserListCtrl, CUserListCtrl)
	ON_NOTIFY_REFLECT(NM_CLICK, &CAllUserListCtrl::OnNMClick)
END_MESSAGE_MAP()

bool CAllUserListCtrl::InitUserListCtrl(std::wstring strRoomName, bool bShowRoomName)
{
	__super::InitUserListCtrl(strRoomName, bShowRoomName);

	AddGuestBtn();
	return true;
}

bool CAllUserListCtrl::InsertUserItem( IUserItem * pUserItem )
{
	if (pUserItem->GetUserVipLevel() == core::VIPLEVEL_NONE && m_bShowGuest == false)
	{
		return true;
	}

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
	if (pUserItem->GetUserVipLevel() == core::VIPLEVEL_REGISTER && pUserItem->GetUserRichLevel() == 0)
	{
		memset(&FindInfo, 0, sizeof(FindInfo));
		FindInfo.flags = LVFI_PARAM;
		FindInfo.lParam = (LPARAM)NULL;
		nRow = FindItem(&FindInfo);
	}

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

void CAllUserListCtrl::ShowGuestList()
{
	m_bShowGuest = true;

	//CRoomRightDlg * pRoomRightDlg = (CRoomRightDlg*)GetParent();
	CRoomBottomLeftDlg * pRoomBottomLeftDlg = (CRoomBottomLeftDlg*)GetParent();
	if (pRoomBottomLeftDlg != NULL)
	{
		std::vector<IUserItem*> vecGuestList;
		pRoomBottomLeftDlg->GetUserManager()->GetGuestList(vecGuestList);

		SetRedraw(FALSE);

		for (std::vector<IUserItem*>::iterator it = vecGuestList.begin(); it != vecGuestList.end(); ++it)
		{
			InsertUserItem(*it);
		}

		SetRedraw(TRUE);
	}
}

void CAllUserListCtrl::HideGuestList()
{
	m_bShowGuest = false;

	LVFINDINFO FindInfo;
	memset(&FindInfo, 0, sizeof(FindInfo));
	FindInfo.flags = LVFI_PARAM;
	FindInfo.lParam = (LPARAM)NULL;
	int iItem = FindItem(&FindInfo);
	int nCount = GetItemCount();
	if (iItem != -1)
	{
		SetRedraw(FALSE);
		for (int i = nCount - 1; i > iItem; i--)
		{
			DeleteItem(i);
		}

		SetRedraw(TRUE);
	}

	Invalidate(FALSE);
}

void CAllUserListCtrl::UpdateGuestBtnIcon( int nItem, bool bShow )
{
	//ImageManager::getInstance()->DeleteIcon(-1);

	//ICONPATH vecIconPath;
	//vecIconPath.push_back(L"");
	//if (bShow)
	//{
	//	vecIconPath.push_back(ListIconManager::GetIconPathByIndex(USERLIST_ICON_GUEST_HIDE));
	//}
	//else
	//{
	//	vecIconPath.push_back(ListIconManager::GetIconPathByIndex(USERLIST_ICON_GUEST_SHOW));
	//}
	//vecIconPath.push_back(ListIconManager::GetIconPathByIndex(USERLIST_ICON_GUEST));
	//vecIconPath.push_back(L"");
	//vecIconPath.push_back(L"");

	//ImageManager::getInstance()->AddIcon(-1, vecIconPath);
}

void CAllUserListCtrl::AddGuestBtn()
{
	int nRow = GetItemCount();
	int nItem = InsertItem(nRow, L"");
	SetItemText(nItem, 4, L"游客(0)人");

	m_nGuestBtnItem = nItem;
}

void CAllUserListCtrl::OnNMClick(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;

	//CRoomDlg * pRoomDlg = (CRoomDlg*)GetParent();
	NMLISTVIEW* pMListView = (NMLISTVIEW*) pNMHDR;
	m_iSelectedItem = pMListView->iItem;//获取选中项

    if (m_iSelectedItem >= 0 && m_iSelectedItem < (this->GetItemCount()))
    {
		//int iSubItem = pMListView->iSubItem;//获取子项
		//int columnCount = this->GetHeaderCtrl()->GetItemCount();//获得总列数
		//if (iSubItem >= 0 && iSubItem < columnCount)
		//{
		//	if (2 == iSubItem)//点击的是等级图标
		//	{//在此做相应处理
		//		//pRoomDlg->ShowLevelInfo();
		//	}
		//	if (3 == iSubItem)//点击的是相册
		//	{//在此做相应处理

		//	}
		//}
		IUserItem *pUserItem = (IUserItem*)GetItemData( m_iSelectedItem ) ;
		if ( pUserItem != NULL )
		{
			CWnd * pParentWnd = GetParent();
			if (pParentWnd != NULL)
			{
				pParentWnd->SendMessage(WM_CLICK_USERLIST, pMListView->iSubItem, (LPARAM)pUserItem);
			}
		}
		else
		{
			OnClickGuestBtn(m_iSelectedItem);
		}
    }
}

void CAllUserListCtrl::OnClickGuestBtn(int nItem)
{
	m_bShowGuest = !m_bShowGuest;
	if (m_bShowGuest)
	{
		ShowGuestList();
	}
	else
	{
		HideGuestList();
	}
}

void CAllUserListCtrl::UpdateGuestBtnText(int nCount)
{
	LVFINDINFO FindInfo;
	memset(&FindInfo, 0, sizeof(FindInfo));
	FindInfo.flags = LVFI_PARAM;
	FindInfo.lParam = (LPARAM)NULL;
	int iItem = FindItem(&FindInfo);
	if (iItem != -1)
	{
		CString strText;
		strText.Format(L"游客(%d)人", nCount);
		SetItemText(iItem, 4, strText);
	}
}

void CAllUserListCtrl::UpdateGuestCount(int nCount)
{
	UpdateGuestBtnText(nCount);
}

void CAllUserListCtrl::MoveToUserItem( IUserItem * pUserItem )
{
	if (!m_bShowGuest)
	{
		OnClickGuestBtn(0);
	}

	int nItem = FindUserItem(pUserItem);
	if (nItem != -1)
	{
		MoveTo(nItem);
	}
}