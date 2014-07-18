#pragma once
#include "afxwin.h"
#include "../Common.h"

class COMMON_INTERFACE CVDCComboBox :
	public CComboBox
{
public:
	CVDCComboBox(void);
	~CVDCComboBox(void);
	DECLARE_MESSAGE_MAP()

protected:
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
private:
	HBRUSH m_hBrush;
	BOOL m_bReadOnly;
};
