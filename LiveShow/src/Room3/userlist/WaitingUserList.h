#pragma once
#include "userlistctrl.h"

class CWaitingUserList : public CUserListCtrl
{
public:
	CWaitingUserList(void);
	virtual ~CWaitingUserList(void);

	virtual bool InsertUserItem(IUserItem * pUserItem);
	virtual bool UpdateUserItem(IUserItem * pUserItem);

	afx_msg BOOL OnToolNeedText(UINT id, NMHDR* pNMHDR, LRESULT* pResult);

	DECLARE_MESSAGE_MAP()
	afx_msg void OnNMClick(NMHDR *pNMHDR, LRESULT *pResult);
};
