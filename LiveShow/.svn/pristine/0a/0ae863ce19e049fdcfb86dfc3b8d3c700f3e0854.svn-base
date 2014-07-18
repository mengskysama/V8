#pragma once
#include "Resource.h"
#include "Skin_i.h"

// CCheckUpdateDlg 对话框

class CCheckUpdateDlg : public CDialog
{
	DECLARE_DYNAMIC(CCheckUpdateDlg)

public:
	CCheckUpdateDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CCheckUpdateDlg();

// 对话框数据
	enum { IDD = IDD_CHECK_UPDATE_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL PreTranslateMessage(MSG* pMsg);

	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg LRESULT OnSkinMessage(WPARAM wParam,LPARAM lParam);

	DECLARE_MESSAGE_MAP()
private:
	CComPtr<ISkinEngine> m_pSkinEngine;
};
