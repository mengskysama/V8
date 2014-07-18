#pragma once

#include "..\Resource.h"
#include "Skin_i.h"
#include "..\..\include/ui/mlhtmlview.h"

#define SKIN_ON_CLICK_CLOSE 1
// CIconIntroductionDlg 对话框
class CRoomBottomLeftDlg;
class CIconIntroductionDlg : public CDialog
{
	DECLARE_DYNAMIC(CIconIntroductionDlg)

public:
	CIconIntroductionDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CIconIntroductionDlg();

// 对话框数据
	enum { IDD = IDD_ICON_INTRUDUCTION_DLG };

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
	CMLHtmlView m_htmlIconIntroduce;

	CRoomBottomLeftDlg * m_pParentDlg;
	
};
