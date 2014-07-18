#pragma once

#include <vector>
#include <map>
#include "afxcmn.h"
#include "ximage.h"
#include "..\GlobalManager.h"

class CEmotionListCtrl :
	public CListCtrl
{
public:
	CEmotionListCtrl(void);
	~CEmotionListCtrl(void);

protected:
    virtual void PreSubclassWindow();
	virtual BOOL PreTranslateMessage(MSG* pMsg);

    afx_msg void OnNMCustomdraw(NMHDR *pNMHDR, LRESULT *pResult); 
	afx_msg void OnNMClick(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg BOOL OnToolNeedText(UINT id, NMHDR* pNMHDR, LRESULT* pResult);

	void DrawBorder(CDC* pDC,const CRect& rcBorder,COLORREF color,int iWidth);
	void Resize(long cx, long cy, RECT rcOuter, RECT& rcRet);
	void DrawHoverBorder(CDC* pDC,const CRect& rcBorder,COLORREF color,int iWidth);
  
	DECLARE_MESSAGE_MAP()
public:
	void Initialize();
	int  AddImage(int iIndex,CxImage * pImage, bool bCaiTiao = false);
	BOOL SetListData(int iGroup = 0,int iPage = 0);//默认显示第一组的第一页
	void DeleteAll() ;//清空信息
	int GetCurGroup(){return m_curGroup;}
	int GetCurPage(){return m_curPage;}
	int GetTotalPageInCurGroup(){return m_totalPageInCurGroup;}

private:
	CImageList m_imageList;
	CImageList m_imageCaiTiaoList;
	std::vector<CxImage*> m_pVectImgs;
	std::vector<CxImage*> m_pVectCaiTiaoImgs;

	CToolTipCtrl m_OwnToolTipCtrl;
	int m_lastToolTip;
	CRect m_oldHoveRect;
	map<int,EmotionItem*> m_mapEmotion ; //每个ITEM对应信息，即每个表情信息
	int m_curGroup;//当前显示的那组
	int m_curPage;//当前页
	int m_totalPageInCurGroup;//当前组的总页数

	bool m_bOldHoveChangeed ;
	bool m_bCurShowCaiTiao; //当前显示彩条
};
