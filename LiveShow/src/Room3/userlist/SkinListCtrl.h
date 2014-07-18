#include "..\..\CommonLibrary\include\ui\ImageEx.h"
#if !defined(AFX_SKINLISTCTRL_H__50B87A28_CE54_4BAD_9355_7A7F4EDC89B9__INCLUDED_)
#define AFX_SKINLISTCTRL_H__50B87A28_CE54_4BAD_9355_7A7F4EDC89B9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SkinListCtrl.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSkinListCtrl window
/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
// CSkinListCtrl window
class CMemDC;
class CSkinListCtrl : public CListCtrl
{
// Construction
public:
	CSkinListCtrl();
// Attributes
protected:
// Operations
public:
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSkinListCtrl)
	protected:
	virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL
	BITMAP	m_bmp;
// Implementation
public:
	virtual ~CSkinListCtrl();
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDIS);
	afx_msg void MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct); 
	afx_msg void OnNMClick(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg BOOL OnNMRclick(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMHover(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnLvnEndScroll(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg BOOL OnToolNeedText(UINT id, NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg LRESULT OnMouseLeave(WPARAM wParam,LPARAM lParam);

	BOOL DeleteItem(int nItem);
	void DrawAnimation();
	
	DECLARE_MESSAGE_MAP()
protected:
	int m_iSelectedItem;//Ñ¡ÖÐÏî
    CFont m_fontItem;
	bool m_bWaitingList;
	bool m_bShowGuest;
	common::ui::ImageEx * m_pNormalImage;
	common::ui::ImageEx * m_pHoverImage;
	common::ui::ImageEx * m_pSelectImage;
	int m_nHoverItem;
	int m_narrColumnWidth[6];
public:
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnPaint();
};

/////////////////////////////////////////////////////////////////////////////
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SKINLISTCTRLEX_H__50B87A28_CE54_4BAD_9355_7A7F4EDC89B9__INCLUDED_)
