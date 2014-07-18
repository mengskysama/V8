#pragma once

#include "..\Resource.h"
#include "Skin_i.h"

// CRoomSettingDlg 对话框
class CRoomParentDlg ;
class CRoomSettingDlg : public CDialog
{
	DECLARE_DYNAMIC(CRoomSettingDlg)

public:
	CRoomSettingDlg( CRoomParentDlg* pParentDlg , CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CRoomSettingDlg();

	void OnConfirm();
	void Refresh();

// 对话框数据
	enum { IDD = IDD_ROOM_SETTING_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);

	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg LRESULT OnSkinMessage(WPARAM wParam,LPARAM lParam);
	void NotifyParentModify();

	void InitDlgData();

	DECLARE_MESSAGE_MAP()
public:
	void SetApplyEffect(BOOL bEffect);
private:
	CComPtr<ISkinEngine> m_pSkinEngine;
	CRoomParentDlg* m_pParentDlg ;
	bool m_bInited;
public:
	afx_msg void OnEnChangeEditRoomName();
	afx_msg void OnEnChangeEditRoomTheme();
	afx_msg void OnBnClickedRadioRoomstateClose();
	afx_msg void OnBnClickedRadioRoomstateOpen();
	afx_msg void OnBnClickedRadioPublicchatClose();
	afx_msg void OnBnClickedRadioPublicchatOpen();
	afx_msg void OnBnClickedRadioCaiziClose();
	afx_msg void OnBnClickedRadioCaiziOpen();
	afx_msg void OnBnClickedRadioUserinoutClose();
	afx_msg void OnBnClickedRadioUserinoutOpen();
	afx_msg void OnEnChangeEditRoomPwd();
};
