#pragma once
#include "Resource.h"
#include "Skin_i.h"
#include "RoomParentDlg.h"
#include "userlist\AllUserListCtrl.h"
#include "userlist\ManagerListCtrl.h"
#include "userlist\WaitingUserList.h"
#include "usermanager\UserManager.h"
#include "RoomBusiness.h"
#include "usermanager\UserItem.h"
#include "userlist\SearchDlg.h"
#include "userlist\IconIntroductionDlg.h"

// CRoomBottomLeftDlg 对话框
class CRoomParentDlg;
class CSelfInfoAndSetDlg;
class CRoomBottomLeftDlg : public CDialog
{
	DECLARE_DYNAMIC(CRoomBottomLeftDlg)

public:
	CRoomBottomLeftDlg(CRoomParentDlg* pParentDlg, CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CRoomBottomLeftDlg();

// 对话框数据
	enum { IDD = IDD_ROOM_BOTTOMLEFT_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();

	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg LRESULT OnSkinMessage(WPARAM wParam,LPARAM lParam);

	DECLARE_MESSAGE_MAP()
public:
	//用户列表相关
	void OnGetUserList(std::map<uint32, IUserItem*> & mapAllUser);
	void OnGetAdminList(std::map<uint32, IUserItem*> & mapAdminUser);
	void OnGetWaitingList(std::map<uint32, IUserItem*> & mapWaitingUser);
	void OnAddUser(IUserItem * pUserItem);
	void OnUpdateUser(IUserItem * pUserItem);
	void OnDeleteUser(IUserItem * pUserItem);
	void OnAddAdminUser(IUserItem * pUserItem);
	void OnDeleteAdminUser(IUserItem * pUserItem);
	void OnAddWaitingUser(IUserItem * pUserItem);
	void OnDeleteWaitingUser(IUserItem * pUserItem);
	void OnUserListCountChange();
	void ShowUserItem(IUserItem * pUserItem);
	void OnGetUserListBegin();
	void OnGetUserListComplete();
	void OnClickBtnWhereIAm();                     								//点击我在哪
	void OnClickIconIntroClose();
	void OnClickNameLink( CString& strName , uint32 unUUID ) ;

	void OnClickOrderSong();
	//个人信息及相关操作设置
	void SetBottomUserInfo(IUserItem* pUserItem);
	CSelfInfoAndSetDlg* GetSelfInfoAndSetDlg(){ return m_pSelfInfoAndSetDlg; }

	void OnAddUserIsMgr(IUserItem * pUserItem);
	void OnUpdateUserIsMgr(IUserItem * pUserItem);

	//业务逻辑
	void SetRoomBusiness(RoomBusiness * pRoomBusiness);

	void OnCycleTrigger();

public:
	afx_msg LRESULT OnSearchEditChange(WPARAM wParam ,LPARAM lParam);			//用户列表搜索框响应
	afx_msg LRESULT OnClickUserList(WPARAM wParam, LPARAM lParam);				//点击用户列表
	afx_msg LRESULT OnDBClickUserList(WPARAM wParam, LPARAM lParam);			//双击用户列表
	afx_msg void OnNMRclickRoomUserList(NMHDR *pNMHDR, LRESULT *pResult);		//右键点击用户列表
	afx_msg void OnNMRclickRoomManagerList(NMHDR *pNMHDR, LRESULT *pResult);	//右键点击管理列表
	afx_msg void OnNMRclickRoomMicList(NMHDR *pNMHDR, LRESULT *pResult);		//右键点击麦序列表
	afx_msg void OnNMRclickRoomSearchList(NMHDR *pNMHDR, LRESULT *pResult);		//右键点击搜索列表
	afx_msg void OnNMRclickRoomAgentList(NMHDR *pNMHDR, LRESULT *pResult);		//右键点击搜索列表

protected:
	//用户列表相关
	void OnClickTabBtnMgr();					   								//点击tab管理
	void OnClickTabBtnUser();                      								//点击tab用户
	void OnClickTabBtnMic();                       								//点击tab麦序
	void OnClickTabBtnAgt();													//点击tab销售
	void OnClickBtnSearch(int checked);            								//点击搜索
	void OnClickBtnIconIntro(int checked);         								//点击图标介绍
	void SearchUserList(std::wstring strKey, ENM_UserListType enmListType);		//搜索用户列表
	void UpdateUserListTabInfo();												//更新用户列表Tab页人数
	int GetUserListCount();														//获得所有用户数量
	int GetManagerListCount();													//获得管理用户数量
	int GetMicListCount();														//获得排麦用户数量
	int GetGuestCount();														//获得游客用户数量

private:
	CComPtr<ISkinEngine> m_pSkinEngine;
	CRoomParentDlg * m_pParentDlg;												//房间主窗口
	RoomBusiness * m_pRoomBusiness;												//房间业务逻辑

	//用户列表
	CAllUserListCtrl m_UserListCtrl;											//用户列表控件
	CManagerListCtrl m_ManagerListCtrl;											//管理列表控件
	CWaitingUserList m_MicListCtrl;												//麦序列表控件
	CUserListCtrl m_SearchListCtrl;												//搜索列表控件
	CUserListCtrl m_AgentListCtrl;												//销售列表控件
	CSearchDlg * m_pSearchDlg;													//用户列表搜索框
	//CIconIntroductionDlg* m_pIconIntroducDlg;									//图标说明框

public:
	UserManager * GetUserManager();												//获取用户管理
	IUserItem * GetSelfUserItem();												//获取自己信息
	//个人信息及设置
	CSelfInfoAndSetDlg * m_pSelfInfoAndSetDlg; 
};
