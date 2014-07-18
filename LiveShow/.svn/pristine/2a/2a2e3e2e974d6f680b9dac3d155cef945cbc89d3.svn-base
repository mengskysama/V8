#pragma once
#include "UserListCtrl.h"

class CAllUserListCtrl : public CUserListCtrl
{
public:
	CAllUserListCtrl(void);
	virtual ~CAllUserListCtrl(void);

	virtual bool InitUserListCtrl(std::wstring strRoomName, bool bShowRoomName);
	virtual bool InsertUserItem(IUserItem * pUserItem);
	//移动到pUserItem所在行，并选中状态
	virtual void MoveToUserItem(IUserItem * pUserItem);
	
	//更新游客数量
	void UpdateGuestCount(int nCount = 0);

protected:
	//现实游客列表
	void ShowGuestList();
	//隐藏游客列表
	void HideGuestList();
	//更新按钮展开或者收起
	void UpdateGuestBtnIcon(int nItem, bool bShow);
	//更新游客按钮内容，游客数量
	void UpdateGuestBtnText(int nCount = 0);
	//添加游客展开收起按钮
	void AddGuestBtn();
	//点击游客按钮响应
	void OnClickGuestBtn(int nItem);

private:
	std::vector<IUserItem*> m_vecGuestItem;
	//bool m_bShowGuest;
	int m_nGuestBtnItem;
	IUserItem * m_pGuestBtnItem;

public:
	DECLARE_MESSAGE_MAP()
	afx_msg void OnNMClick(NMHDR *pNMHDR, LRESULT *pResult);
};
