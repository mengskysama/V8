#pragma once

#include "..\Resource.h"
#include "Skin_i.h"
#include "afxwin.h"
#include "..\..\include/ui/mlhtmlview.h"

// CTaskDlg 对话框
class CRoomParentDlg;
class CTaskDlg : public CDialog
{
	DECLARE_DYNAMIC(CTaskDlg)

public:
	CTaskDlg(CRoomParentDlg* pParent = NULL);   // 标准构造函数
	virtual ~CTaskDlg();

// 对话框数据
	enum { IDD = IDD_ROOM_TASK_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual void OnCancel();
	virtual void OnOK();
    virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);	

	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg LRESULT OnSkinMessage(WPARAM wParam,LPARAM lParam);
	afx_msg LRESULT OnLoadHTMLComplete(WPARAM wParam,LPARAM lParam);  

	void OnClick(LPARAM lParam);

	void InitBrowser();

	void VerifyEmail();
	void GetReward(CString& name);

	DECLARE_MESSAGE_MAP()
public:
	void UpdateTaskInfo();
private:
	CComPtr<ISkinEngine> m_pSkinEngine;
	CMLHtmlView m_htmlTaskInfo;
	CRoomParentDlg * m_pRoomParentDlg;
};
