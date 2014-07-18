#pragma once

#include "..\Resource.h"
#include "Skin_i.h"
#include "afxwin.h"
#include "..\..\include/ui/mlhtmlview.h"
// CShowPwdSafeDlg 对话框

class CShowPwdSafeDlg : public CDialog
{
	DECLARE_DYNAMIC(CShowPwdSafeDlg)

public:
	CShowPwdSafeDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CShowPwdSafeDlg();

// 对话框数据
	enum { IDD = IDD_ATROOM_SHOW_PWD_SAFE_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual void OnOK();
	virtual void OnCancel();
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual void OnShowWindow(BOOL bShow,UINT nStatus);
	LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg LRESULT OnSkinMessage(WPARAM wParam,LPARAM lParam);

	void Confirm();
	void Init();	

	DECLARE_MESSAGE_MAP()
public:
	void SetApplyEffect(BOOL bEffect);//设置点击应用的效果

private:
	CComPtr<ISkinEngine> m_pSkinEngine;
	CMLHtmlView m_ModifyPwd;
};
