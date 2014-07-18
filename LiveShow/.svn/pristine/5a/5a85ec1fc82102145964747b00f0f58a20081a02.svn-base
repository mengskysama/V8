#pragma once

#include "..\Resource.h"
#include "Skin_i.h"
#include "afxwin.h"

// CEditAutoReplyDlg 对话框

class CEditAutoReplyDlg : public CDialog
{
	DECLARE_DYNAMIC(CEditAutoReplyDlg)

public:
	CEditAutoReplyDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CEditAutoReplyDlg();

// 对话框数据
	enum { IDD = IDD_EDIT_AUTOREPLY_DLG };

	CString GetInPutString();
	void SetAutoReplyEditTitle(CString strTitle);
	void SetEditText(CString strEdit);
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL PreTranslateMessage(MSG* pMsg);

	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg LRESULT OnSkinMessage(WPARAM wParam,LPARAM lParam);	
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);

	DECLARE_MESSAGE_MAP()

private:
	CComPtr<ISkinEngine>  m_pSkinEngine;
	CEdit m_InputEdit;

	CString m_strInput;
	CString m_strTitle;
};
