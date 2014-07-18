#pragma once

#include "..\Resource.h"
#include "Skin_i.h"
#include "afxwin.h"
#include "..\avlogic\RoomAVLogic.h"
// CShowBasicSettingDlg 对话框

class CShowBasicSettingDlg : public CDialog
{
	DECLARE_DYNAMIC(CShowBasicSettingDlg)

public:
	CShowBasicSettingDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CShowBasicSettingDlg();

// 对话框数据
	enum { IDD = IDD_ATROOM_SHOW_BASIC_SETTING_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual void OnOK();
	virtual void OnCancel();
	virtual BOOL PreTranslateMessage(MSG* pMsg);

	virtual void OnShowWindow(BOOL bShow,UINT nStatus);
    afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg LRESULT OnSkinMessage(WPARAM wParam,LPARAM lParam);
	LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);	

	void SetRegeditAutoRun();  //设置开机运行   C:\Documents and Settings\用户名\「开始」菜单\程序\启动
	void ClearRegeditAutoRun(); //清删开机运行
	bool ReadRegeditAutoRun();  //读取是否开机运行

	void Confirm();
	void Init();

	DECLARE_MESSAGE_MAP()

private:
	CComPtr<ISkinEngine> m_pSkinEngine;


	CString m_curLoginUserName;  //当前登录账号
	CString m_curAutoLogin;
	CString m_curPowerRun;
	CString m_curOpenSound;

	CButton m_autoLogin;
	CButton m_PowerRun;
	CButton m_openSound;
public:
	afx_msg void OnBnClickedCheck();  //复选框点击响应函数
	void SetApplyEffect(BOOL bEffect);//设置点击应用的效果
	
};
