#pragma once
#include "ximage.h"

// CChangeVoiceListCtrl

class CChangeVoiceListCtrl : public CListCtrl
{
	DECLARE_DYNAMIC(CChangeVoiceListCtrl)

public:
	CChangeVoiceListCtrl();
	virtual ~CChangeVoiceListCtrl();

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnNMCustomdraw(NMHDR *pNMHDR, LRESULT *pResult);

public:
	void Initialize();

protected:
	void SetListData();
	void Resize(long cx, long cy, RECT rcOuter, RECT& rcRet);
	void DrawHoverBorder(CDC* pDC,const CRect& rcBorder,COLORREF color,int iWidth);

private:
	CImageList m_imageList;
	CRect m_oldHoveRect;
	CToolTipCtrl m_OwnToolTipCtrl;
	int m_lastToolTip;
	std::vector<CxImage*> m_pVectImgs;
	CFont m_font;
	int m_nHoverItem;
	CxImage * m_pHoverImage;
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
protected:
	virtual void PreSubclassWindow();
public:
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnNMClick(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
};


