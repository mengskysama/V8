#pragma once

#include "..\Resource.h"
#include "Skin_i.h"
// CShowHarassmentDlg 对话框

class CShowMessageSettingDlg : public CDialog
{
	DECLARE_DYNAMIC(CShowMessageSettingDlg)

public:
	CShowMessageSettingDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CShowMessageSettingDlg();

	void Refresh();

// 对话框数据
	enum { IDD = IDD_ATROOM_SHOW_MESSAGE_SETTING_DLG};

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual void OnOK();
	virtual void OnCancel();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);	

	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg LRESULT OnSkinMessage(WPARAM wParam,LPARAM lParam);
	afx_msg void OnBnClickedCheck();  //复选框点击响应函数

	void Init();
	void Confirm();

	DECLARE_MESSAGE_MAP()
public:
	void SetApplyEffect(BOOL bEffect);//设置点击应用的效果

private:
	CComPtr<ISkinEngine> m_pSkinEngine;
	
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
};
