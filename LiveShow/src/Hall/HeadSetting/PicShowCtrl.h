#pragma once
#include "afxwin.h"

class CPicShowCtrl :
	public CWnd
{
public:
	bool Create(CWnd* pwndParent, CRect rc, UINT uId);
	CPicShowCtrl(void);
	~CPicShowCtrl(void);

public:
	void SetBitmap(HBITMAP hBmp);
	bool SaveFile(CString strPath);
	void SetStartx(int iStartx);
	void SetStarty(int iStarty);

private:
	HBITMAP m_hBmp;
	int m_iStartx;
	int m_iStarty;
public:
	DECLARE_MESSAGE_MAP()
	afx_msg void OnPaint();
};
