#pragma once

#include <vector>
#include <map>
#include "afxcmn.h"
#include "ximage.h"
#include "..\gift/GiftManager.h"

struct stStamp  //盖章结构
{
	CString stampPath;//路径
	CString stampName;//名称
};

struct stStampGroup //盖章组结构
{
	CString groupName;//组名
	std::vector<stStamp*> groupStamp;//该组下的所有盖章
};

typedef  std::vector<stStampGroup*>   VecStampGroup;
typedef  std::vector<stStamp*>    VecStamp;
typedef  std::map<int,stStamp*>   MapStamp;

class CStampListCtrl :
	public CListCtrl
{
public:
	CStampListCtrl(void);
	~CStampListCtrl(void);

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
	int  AddImage(int iIndex,CxImage * pImage);
	BOOL SetListData(int iGroup = 0);//默认显示第一组
	void DeleteAll() ;//清空信息
private:
	CImageList m_imageList;
	std::vector<CxImage*> m_pVectImgs;

	CToolTipCtrl m_OwnToolTipCtrl;
	int m_lastToolTip;
	CRect m_oldHoveRect;
	std::map< int , IGiftItem* > m_mapEmotion ;
   // MapStamp m_mapEmotion;//每个ITEM对应信息，即每个盖章信息
	int m_curGroup;//当前显示的那组
	
};
