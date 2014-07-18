#pragma once
#include "CrashReportSrc/crashrpt/src/CrashHandler.h"

// CMainDlg dialog

class CMainDlg : public CDialog
{
	DECLARE_DYNAMIC(CMainDlg)

public:
	CMainDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CMainDlg();

// Dialog Data
	enum { IDD = IDD_MAINDLG };

	CFont m_fontHeader;
	TStrStrMap  *m_pUDFiles;      // Files <name,desc>

	CString m_sText;
	HICON   m_Icon;
	HICON   m_TitleIcon;
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	CString m_sDescription;
	afx_msg void OnPaint();
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
};
