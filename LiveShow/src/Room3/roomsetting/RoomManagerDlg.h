#pragma once

#include "..\Resource.h"
#include "Skin_i.h"

#define WM_NOTIFY_ROOM_ROOMSET_MODIFY WM_USER + 401

// CRoomManagerDlg 对话框
class CAnnouncementDlg;
class CRoomSettingDlg;
class CMicSettingDlg;
class CUserMgrDlg;
class CTempAnnounceDlg;
class CRoomAnnounceDlg;
class CGetContributionDlg;

class CRoomManagerDlg : public CDialog
{
	DECLARE_DYNAMIC(CRoomManagerDlg)

public:
	CRoomManagerDlg( CRoomParentDlg* pParentDlg , CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CRoomManagerDlg();

// 对话框数据
	enum { IDD = IDD_ROOM_MANAGER };	
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg LRESULT OnSkinMessage(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnRoomSetModify(WPARAM wParam, LPARAM lParam);

	void NotifyCreateSkinItem();
    //void SetRoomerSysAddress();//设置室主后台的链接地址	
	void OnClickGetContribute();
	void OnClickBtnRoomerSys();//室主后台
	void OnClickBtnConfirm();//点击确定
	void OnClickBtnApply();//点击应用
	void SetApplyEffect(BOOL bEffect);//应用提交效果
	void EnableBtnApply(bool bEnable);
	void Confirm();

	DECLARE_MESSAGE_MAP()
private:
	CComPtr<ISkinEngine> m_pSkinEngine;
	
	CAnnouncementDlg * m_pAnnouncementDlg;
	CRoomSettingDlg * m_pRoomSettingDlg;
	CMicSettingDlg * m_pMicSettingDlg;
	CUserMgrDlg   * m_pUserMgrDlg;
	CDialog* m_pCurrentDlg;
	CRoomParentDlg* m_pParentDlg ;
	CTempAnnounceDlg * m_pTempAnnounceDlg;
	CRoomAnnounceDlg * m_pRoomAnnounceDlg;
	CGetContributionDlg * m_pGetContributeDlg;
	
    CString m_strCurItemBk;
public:
	void TurnToAnnouncePanel( CString& strText ) ;
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};
