#pragma once
#include "../../include/ExportMacro.h"

#define WM_USER_SPLITTER	(WM_USER + 1650)
class  COMMON_INTERFACE CSplitter : public CWnd
{
public:
	CSplitter();
	virtual ~CSplitter();

	void SetLimit(int nMinTop, int nMaxBottom);
	BOOL Create(DWORD dwStyle, LPCTSTR lpszSkinPath, const CRect& rect, 
		CWnd* pParentWnd, UINT uID, BOOL bHorizontal = TRUE);

	/**
	 *	是否是水平滚动条
	 */
	void	SetHorizontal( BOOL bH) { m_bHorizontal = bH;}
	BOOL	IsBeHorizontal() { return m_bHorizontal;}

	void	SetBkgColor( COLORREF uBkgColor);

protected:
	//{{AFX_MSG(CSplitter)
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnPaint();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);

	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	void	DrawDraggingBar(CPoint point, BOOL bEnter);

private:
	int			m_nMaxBottom;
	int			m_nMinTop;
	CPoint		m_ptOld;
	HCURSOR		m_hVCursor;
	HCURSOR		m_hHCursor;
	CRect		m_rectLast;
	BOOL		m_bDragging;
	BOOL		m_bHorizontal;
	COLORREF	m_nBkgColor;
};
