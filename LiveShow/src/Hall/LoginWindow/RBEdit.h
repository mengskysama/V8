#pragma once

#include "TipCtrl.h"

/*
@class		CRBEdit
@purpose	o)按字节数来设置最大可输入长度,当输入超限时，通知父窗口(WM_NOTIFY)
			o)自定义边框颜色
*/

// CRBEdit

enum
{
	RBEditNotify_InputExceed = WM_USER + 9001,
};

class CRBTipCtrl;

class CRBEdit : public CEdit
{
	DECLARE_DYNAMIC(CRBEdit)

public:
	CRBEdit();
	virtual ~CRBEdit();

	void SetLimit(int nLimitInBytes)
	{
		m_nLimitInBytes = nLimitInBytes;
	}

	int GetLimit()
	{
		return m_nLimitInBytes;
	}

	void SetBorderColor(COLORREF crBorder)
	{
		m_crBorder = crBorder;

		if (IsWindowVisible())
		{
			Invalidate();
		}
	}

	void SetBackColor(COLORREF crBack, COLORREF crBackGray)
	{
		m_crBack = crBack;
		m_crBackGray = crBackGray;

		CreateBackBrush();

		if (IsWindowVisible())
		{
			Invalidate();
		}
	}

	void SetBorderDrawFlag(bool bDraw)
	{
		m_bBorderDrawFlag = bDraw;

		if (IsWindowVisible())
		{
			Invalidate();
		}
	}

	void SetTextColor(COLORREF crText, COLORREF crTextGray)
	{
		m_crText = crText;
		m_crTextGray = crTextGray;

		if (IsWindowVisible())
		{
			Invalidate();
		}
	}

	void SetReplaceNewlineWithBlankFlag(bool b)
	{
		m_bReplaceNewlineWithBlankFlag = b;
	}

	void SetCharsetFobidden(CString sCharset, CString sCharsetFobiddenTip)
	{
		m_sCharsetFobidden = sCharset;
		m_sCharsetFobiddenTip = sCharsetFobiddenTip;
	}
	inline void ShowVertScrollBar(BOOL bShow)
	{
		if(m_bShowVert != bShow)
		{
			ShowScrollBar(SB_VERT, bShow);
			CRect rc;
			GetClientRect(&rc);
			rc.left = rc.right - 20;
			InvalidateRect(rc);
			m_bShowVert = bShow;
		}
	}
	 void SetAutoScroll(BOOL _autos = TRUE)
	 {
           m_autoscrollvisible = _autos;
		   return;
	 }
	void SetWindowText(LPCTSTR lpszString);
protected:
	int GetLengthInAnsi();
	int GetSelLengthInAnsi();
	int ReplaceSel(const wchar_t* szTextNew);
	void CreateBackBrush();

	void NotifyInputExceed(int nLimitNow, int nInputLenYouWant);
	bool IsInCharsetForbidden(wchar_t wc);
	void ShowCharsetForbiddenTip();
	int FindCharForbidden(CString s);

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
    afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg LRESULT OnPaste(WPARAM w, LPARAM l);
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg HBRUSH CtlColor(CDC* pDC, UINT nCtlColor);

protected:
	int m_nLimitInBytes;
	COLORREF m_crBorder;
	COLORREF m_crBack;
	COLORREF m_crBackGray;
	COLORREF m_crText;
	COLORREF m_crTextGray;
	CBrush m_brBack;
	CBrush m_brBackGray;
	BOOL m_bBorderDrawFlag;
	BOOL m_autoscrollvisible;
	bool m_bReplaceNewlineWithBlankFlag;
    
	CTipCtrl* m_pctrlTip;
	CString m_sCharsetFobiddenTip;
	CString m_sCharsetFobidden;
	afx_msg LRESULT OnCheckText(WPARAM wParam, LPARAM lParam);
private:
	void CheckScrolling(LPCTSTR lpszString);
	static const UINT UWM_CHECKTEXT;
	BOOL m_bShowVert;
};


