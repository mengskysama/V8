#pragma once
#include "..\..\CommonLibrary/include/ui/SimpleTipCtrl.h"
// CSearchEdit

class CSearchDlg;
class CSearchEdit : public CEdit
{
	DECLARE_DYNAMIC(CSearchEdit)

public:
	CSearchEdit();
	virtual ~CSearchEdit();

protected:
	DECLARE_MESSAGE_MAP()

private:
	BOOL IsSbExist(uin _uin);
public:
	void SendBySel();

public:
	BOOL m_bNotEnChange;


public:
	BOOL m_bFakeLoseFocus;

public:
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg BOOL OnEnChange();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg LRESULT OnMouseLeave(WPARAM wParam,LPARAM lParam);
	afx_msg LRESULT OnMouseHover(WPARAM wParam,LPARAM lParam);

	void OnOpenDialog(uint32 uUIN);

	void PopToolTip();
	void ShowToolTip(CString pszTooltip);

private:
	//CRBSimpleTipCtrl m_ToolTip;
	HWND            m_hToolTip;
	BOOL			m_bLeave;
	BOOL            m_bTipShow;

	CToolTipCtrl m_ToolTip;
public:
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);

	uint32 m_tribeId;

	
	
};


