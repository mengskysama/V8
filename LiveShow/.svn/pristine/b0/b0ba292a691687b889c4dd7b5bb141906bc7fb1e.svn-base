#include "StdAfx.h"
#include "ManagerListCtrl.h"
#include "..\UserMessageDefine.h"

CManagerListCtrl::CManagerListCtrl(void)
{
}

CManagerListCtrl::~CManagerListCtrl(void)
{
}

BEGIN_MESSAGE_MAP(CManagerListCtrl, CUserListCtrl)
	ON_NOTIFY_REFLECT(NM_CLICK, &CManagerListCtrl::OnNMClick)
END_MESSAGE_MAP()

int CALLBACK CManagerListCtrl::SortFun( LPARAM lParam1, LPARAM lParam2, LPARAM lParamList )
{
	IUserItem * pUserItem1 = (IUserItem*)lParam1;
	IUserItem * pUserItem2 = (IUserItem*)lParam2;
	
	ASSERT(pUserItem1 != NULL);
	ASSERT(pUserItem2 != NULL);

	if (pUserItem1->GetUserTitleLevel() == pUserItem2->GetUserTitleLevel())
	{
		return (pUserItem1->GetUser179ID() > pUserItem2->GetUser179ID()) ? 1 : -1;
	}
	else
	{
		return (pUserItem1->GetUserTitleLevel() < pUserItem2->GetUserTitleLevel()) ? 1 : -1;
	}

	//return (pUserItem1->GetUser179ID() > pUserItem2->GetUser179ID()) ? 1 : -1;
}

bool CManagerListCtrl::UpdateUserList()
{
	SortItems(SortFun, (LPARAM)this);

	return true;
}

void CManagerListCtrl::OnNMClick(NMHDR *pNMHDR, LRESULT *pResult)
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