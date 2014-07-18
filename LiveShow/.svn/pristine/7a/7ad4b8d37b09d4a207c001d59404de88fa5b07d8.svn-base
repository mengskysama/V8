#pragma once

#include "Resource.h"
#include "Skin_i.h"
// CAboutDlg 对话框

class CAboutDlg : public CDialog
{
	DECLARE_DYNAMIC(CAboutDlg)

public:
	CAboutDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg LRESULT OnSkinMessage(WPARAM wParam, LPARAM lParam);

	void OnClickBtnLink();//官网链接
	void OnClickBtnUserAgreement();//查看用户协议 

	DECLARE_MESSAGE_MAP()
private:
	CComPtr<ISkinEngine> m_pSkinEngine; 
public:
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
};
