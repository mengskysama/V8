#include "StdAfx.h"
#include "WaitingUserList.h"
#include "ListIconManager.h"
#include "utility/StringHelper.h"
#include "..\gift/GiftManager.h"
#include "..\UserMessageDefine.h"
#include "..\GlobalManager.h"

CWaitingUserList::CWaitingUserList(void)
{
	m_bWaitingList = true;
}

CWaitingUserList::~CWaitingUserList(void)
{
}

BEGIN_MESSAGE_MAP(CWaitingUserList, CUserListCtrl)
	ON_NOTIFY_EX(TTN_NEEDTEXT, 0, OnToolNeedText)
	ON_NOTIFY_REFLECT(NM_CLICK, &CWaitingUserList::OnNMClick)
END_MESSAGE_MAP()

bool CWaitingUserList::InsertUserItem( IUserItem * pUserItem )
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

	//ICONPATH vecIconPath;
	//vecIconPath.push_back(ListIconManager::GetIconPathByIndex(pUserItem->GetListIconIndex(0)));
	//vecIconPath.push_back(ListIconManager::GetIconPathByIndex(USERLIST_ICON_WAIT_MIC));
	//vecIconPath.push_back(ListIconManager::GetIconPathByIndex(pUserItem->GetListIconIndex(2)));
	//vecIconPath.push_back(ListIconManager::GetIconPathByIndex(pUserItem->GetListIconIndex(3)));

	//IGiftItem * pGiftItem = GiftManager::GetInstance()->SearchStampByID(pUserItem->GetUserStampID());
	//if (pGiftItem != NULL)
	//{
	//	vecIconPath.push_back(pGiftItem->GetSmallImagePath());
	//}
	//else
	//{
	//	vecIconPath.push_back(L"");
	//}

	//ImageManager::getInstance()->AddIcon(pUserItem->GetUserUIN(), vecIconPath);

	//UpdateUserList();

	return true;
}

BOOL CWaitingUserList::OnToolNeedText( UINT id, NMHDR* pNMHDR, LRESULT* pResult )
{
	CRect rc;
	GetWindowRect(&rc);
	CPoint pot;
	GetCursorPos(&pot);
	pot.x = pot.x - rc.left;
	pot.y = pot.y - rc.top;

	LVHITTESTINFO LVHTestInfo;
	LVHTestInfo.pt = pot;
	HitTest(&LVHTestInfo);
	int iItem = LVHTestInfo.iItem;//获得行
	SubItemHitTest(&LVHTestInfo);
	int iSubItem = LVHTestInfo.iSubItem;//获得列	

	if (iItem >= 0 && iItem < GetItemCount())
	{
		IUserItem * pUserItem = (IUserItem*)GetItemData(iItem);//获取该选中项信息

		if (pUserItem != NULL && pUserItem->GetUserUIN() != 0)
		{
			int columnCount = GetHeaderCtrl()->GetItemCount();//获得总列数
			if (iSubItem >= 0 && iSubItem < columnCount)
			{	
				if (iSubItem < 4)
				{
					std::wstring strTip;
					if (iSubItem == 1)
					{
						strTip = ListIconManager::GetIconTipByIndex(USERLIST_ICON_WAIT_MIC);
					}
					else
					{
						strTip = ListIconManager::GetIconTipByIndex(pUserItem->GetListIconIndex(iSubItem));
					}
					if (strTip != L"")
					{
						TOOLTIPTEXTW* pTT = (TOOLTIPTEXT*) pNMHDR;
						wcscpy(pTT->szText,strTip.c_str());
					}
				}
				else if (4 == iSubItem)//昵称
				{//显示相关信息
					std::wstring strName = pUserItem->GetUserName() + L"(" + common::utility::stringhelper::IntToString(pUserItem->GetUser179ID()) + L")";
					TOOLTIPTEXTW* pTT = (TOOLTIPTEXT*) pNMHDR;
					wcscpy(pTT->szText,strName.c_str());
				}
			}
		}
	}

	*pResult = 1;

	return TRUE;
}

bool CWaitingUserList::UpdateUserItem( IUserItem * pUserItem )
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

	//ImageManager::getInstance()->DeleteIcon(pUserItem->GetUserUIN());

	//ICONPATH vecIconPath;
	//vecIconPath.push_back(ListIconManager::GetIconPathByIndex(pUserItem->GetListIconIndex(0)));
	//vecIconPath.push_back(ListIconManager::GetIconPathByIndex(USERLIST_ICON_WAIT_MIC));
	//vecIconPath.push_back(ListIconManager::GetIconPathByIndex(pUserItem->GetListIconIndex(2)));
	//vecIconPath.push_back(ListIconManager::GetIconPathByIndex(pUserItem->GetListIconIndex(3)));

	//IGiftItem * pGiftItem = GiftManager::GetInstance()->SearchStampByID(pUserItem->GetUserStampID());
	//if (pGiftItem != NULL)
	//{
	//	vecIconPath.push_back(pGiftItem->GetSmallImagePath());
	//}
	//else
	//{
	//	vecIconPath.push_back(L"");
	//}

	//ImageManager::getInstance()->AddIcon(pUserItem->GetUserUIN(), vecIconPath);

//	UpdateUserList();

	return true;
}

void CWaitingUserList::OnNMClick(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;

	//CRoomDlg * pRoomDlg = (CRoomDlg*)GetParent();
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