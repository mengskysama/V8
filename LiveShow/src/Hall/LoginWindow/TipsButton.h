#pragma once
#include "afxwin.h"

class C179TipCtrl; 

class CTipsButton :
	public CButton
{
public:
	CTipsButton(void);
	~CTipsButton(void);
	DECLARE_MESSAGE_MAP()
protected:
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg HRESULT OnMouseHover( WPARAM w, LPARAM l);
	afx_msg HRESULT OnMouseLeave( WPARAM w, LPARAM l);
public:
	void SetBtnTip(CString strTip);
	void HideBtnTip();
private:
	CString m_strTip;
    C179TipCtrl *m_pTipCtrl;
};
