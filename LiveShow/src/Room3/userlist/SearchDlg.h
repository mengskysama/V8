#pragma once
#include "..\Resource.h"
#include "searchedit.h"
#include "Skin_i.h"

class  CSearchDlg : public CDialog
{
	DECLARE_DYNAMIC(CSearchDlg)

public:
	CSearchDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CSearchDlg();

// Dialog Data
	enum { IDD = IDD_SEARCH_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

public:
	virtual BOOL OnInitDialog();
	afx_msg LRESULT OnSkinMessage(WPARAM wParam, LPARAM lParam);
	
public:
	void OnSearchClick();
	void OnMove(int x, int y);
	void OnLogOff();
	
protected:
	virtual void OnOK();
	virtual void OnCancel();

public:
	CString m_strEdit;
	CSearchEdit m_editSearch;
    CBrush m_EditBrush;
	CComPtr<ISkinEngine> m_pSkinEngine;
	uint32 m_tribeId;

public:
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);

protected:
	virtual void PostNcDestroy();
public:
	afx_msg void OnEnChangeEditSearch();
};
