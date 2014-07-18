#pragma once

#include "..\Resource.h"
#include "Skin_i.h"
#include "..\GlobalRoomDefine.h"

// CScrollBtnDlg 对话框
class CRoomParentDlg ;
class CScrollBtnDlg : public CDialog
{
	DECLARE_DYNAMIC(CScrollBtnDlg)

public:
	CScrollBtnDlg( CRoomParentDlg* pParentDlg , CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CScrollBtnDlg();

// 对话框数据
	enum { IDD = IDD_SCROLL_BTN_DLG };

	void Init( ENM_HTML_TYPE nHtmlType ) ;
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL OnInitDialog();

	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg LRESULT OnSkinMessage(WPARAM wParam,LPARAM lParam);
	afx_msg void OnKillFocus(CWnd* pNewWnd);

	void OnClickBtnScroll();
	void OnClickBtnClean();

	DECLARE_MESSAGE_MAP()
private:
	CComPtr<ISkinEngine> m_pSkinEngine;
	CRoomParentDlg* m_pParentDlg ;
	BOOL m_bScrool;//TRUE为滚动状态，FALSE为不滚动，初始为TRUE
	ENM_HTML_TYPE m_nHtmlType ;
};
