#pragma once

#include "..\Resource.h"
#include "Skin_i.h"
#include "..\..\include/ui/mlhtmlview.h"

// CPresentDetailDlg 对话框

class CPresentDetailDlg : public CDialog
{
	DECLARE_DYNAMIC(CPresentDetailDlg)

public:
	CPresentDetailDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CPresentDetailDlg();

// 对话框数据
	enum { IDD = IDD_PRESENT_DETAIL_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);

	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg LRESULT OnSkinMessage(WPARAM wParam,LPARAM lParam);

	void InitBrowser();

	DECLARE_MESSAGE_MAP()
private:
	CComPtr<ISkinEngine> m_pSkinEngine;
	CMLHtmlView m_htmlPresentDetail;
public:
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
};
