#pragma once
#include "skinlistctrl.h"
//#include "iuseritem.h"
#include "..\usermanager/UserItem.h"

#define COLUMN_COUT	6

enum ENM_UserListColumn
{
	COLUNM_VIP_LEVEL	= 0,
	COLUNM_MIC_STATUS	= 1,
	COLUNM_USER_LEVEL	= 2,
	COLUNM_USER_PIC		= 3,
	COLUNM_USER_NAME	= 4,
	COLUNM_USER_ID		= 5,
};

class CUserListCtrl : public CSkinListCtrl
{
public:
	CUserListCtrl(void);
	virtual ~CUserListCtrl(void);

	//初始化用户列表
	virtual bool InitUserListCtrl(std::wstring strRoomName, bool bShowRoomName = false);

	//插入单个用户
	virtual bool InsertUserItem(IUserItem * pUserItem);
	//插入批量用户
	virtual bool InsertUserItem(std::vector<IUserItem*> vecUserItem);
	//更新用户
	virtual bool UpdateUserItem(IUserItem * pUserItem);
	//删除用户
	virtual bool DeleteUserItem(IUserItem * pUserItem);
	//查找用户
	virtual int FindUserItem(IUserItem * pUserItem);
	//移动到nItem所在行，并选中状态
	virtual void MoveTo(int nItem);
	//移动到pUserItem所在行，并选中状态
	virtual void MoveToUserItem(IUserItem * pUserItem);
	//更新列表，实现排序
	virtual bool UpdateUserList();

protected:
	//根据pUserItem获取每一栏的显示内容
	std::wstring GetDisplayText(IUserItem * pUserItem, uint32 unColumn);
	//排序回调函数
	static int CALLBACK SortFun(LPARAM lParam1, LPARAM lParam2, LPARAM lParamList);
	
	DECLARE_MESSAGE_MAP()
	afx_msg void OnNMClick(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMDblclk(NMHDR *pNMHDR, LRESULT *pResult);

	bool m_bTimerRunning;
	void UpdateColumnWidth(bool doRightNow = false);

	std::wstring m_strRoomName;
	bool m_bShowRoomName;
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};
