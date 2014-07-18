#pragma once
#include "GiftManager.h"
#include "GiftTipDlg.h"
#include "ximage.h"

class CGiftListCtrl : public CListCtrl
{
public:
	CGiftListCtrl(void);
	CGiftListCtrl(std::wstring strTabName);
	virtual ~CGiftListCtrl(void);

//	afx_msg void OnLvnGetInfoTip(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMClick(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMCustomdraw(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg LRESULT OnMouseLeave(WPARAM wParam,LPARAM lParam);
	afx_msg BOOL OnToolNeedText(UINT id, NMHDR* pNMHDR, LRESULT* pResult);

	virtual void PreSubclassWindow();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual bool InitGiftListCtrl();
	virtual bool SetListData(VecGiftItem & vecGiftList);
	virtual void ReloadListData();

protected:
	DECLARE_MESSAGE_MAP()

protected:
	void AddImage(std::wstring strImagePath);
	void ShowTip(CPoint itempt, IGiftItem* pGiftItem, int nCount = 0);
	void ReleaseAllItems();
	virtual void InitListData();

protected:
	CImageList m_imageGiftList;
    CToolTipCtrl m_OwnToolTipCtrl;
	std::wstring m_strTabName;
	static CGiftTipDlg * s_pGiftTipDlg;
	std::vector<CxImage*> m_pVectImgs;
	CxImage * m_pGiftWordsImg;
	CxImage * m_pNormalBorder;
	CxImage * m_pHoverBorder;
	int m_nHoverItem;

protected:
	int m_lastToolTip;
	CFont m_font;
};
