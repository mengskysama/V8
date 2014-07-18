#pragma once
#include "UserListCtrl.h"

class CManagerListCtrl : public CUserListCtrl
{
public:
	CManagerListCtrl(void);
	virtual ~CManagerListCtrl(void);

	virtual bool UpdateUserList();

protected:
	static int CALLBACK SortFun(LPARAM lParam1, LPARAM lParam2, LPARAM lParamList);
public:
	DECLARE_MESSAGE_MAP()
	afx_msg void OnNMClick(NMHDR *pNMHDR, LRESULT *pResult);
};
