#pragma once

#include "..\Resource.h"
#include "Skin_i.h"
#include "afxcmn.h"
#include "StampListCtrl.h"
#include "ximage.h"

// CStampDlg 对话框

class CRoomDlg ;
class CStampDlg : public CDialog
{
	DECLARE_DYNAMIC(CStampDlg)

public:
	CStampDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CStampDlg();

// 对话框数据
	enum { IDD = IDD_STAMP_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL OnInitDialog();

	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg LRESULT OnSkinMessage(WPARAM wParam,LPARAM lParam);
	afx_msg void OnSize(UINT nType, int cx, int cy);

	void ShowStamp();
	void OnClickStampTab(int iTab);

	DECLARE_MESSAGE_MAP()

public:
	void OnSelStamp(int iStampGroup,int iStampIndex);
private:
	CComPtr<ISkinEngine> m_pSkinEngine;

	BOOL m_bShowStamp;
	CStampListCtrl m_stampListCtrl;
	//CWnd* m_pParentWnd ;
	
public:
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
};
