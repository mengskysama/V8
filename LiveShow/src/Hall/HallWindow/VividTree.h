#pragma once

#include "resource.h"
#include "imodulemanager.h"
#include "ExpatHallTree.h"
#include <map>

class VividTree : public CTreeCtrl
{
	DECLARE_DYNAMIC(VividTree)

public:
	VividTree();
	virtual ~VividTree();
private:

	CRect m_rect;
	HTREEITEM m_selectedItem,m_hoverItem;
	HWND m_parentHWMD;
	CFont m_fontParentNode;//父结点字体
	CFont m_fontChildNode;//子结点字体
	CFont m_fontParentNodeNum;//父结点人数字体
	CFont m_fontChildNodeNum;//子结点人数字体
	Font * m_fontParentName;
	Font * m_fontChildName;
	Font * m_fontParentNum;
	Font * m_fontChildNum;
	CString m_strImgBk;//背景图
	CString m_strParentNodeHoverItemBk;//根结点Hover图片
	CString m_strChildNodeHoverItemBk;//非根结点Hover图片
	CString m_strParentNodeSelItemBk;//根结点Selected图片
	CString m_strChildNodeSelItemBk;//非根结点Selected图片
	CString m_strParentNodeNormalItemBk;//根结点正常态图片
	CString m_strExpand;//父结点展开时的图标
	CString m_strContract;//父结点收缩时的图标
	bool m_bInClearStack;//是否在Clear()的调用栈中

	void DrawBackGround( CDC* pDC );
	void DrawItems( CDC* pDC );
	void DrawItemBk(CDC* pDC, CRect& rcItemBk, CString& imgPath);
	void DrawIcon(CDC* pDC, CRect& rcItem, CString strIcon);
	//void DrawItemString(CDC* pDC, CRect& rcItem, BOOL bName, BOOL bSel, BOOL bHasParent);
	void DrawName(CDC* pDC, CRect&rcItem, CString strName, BOOL bSel, BOOL bHasParent);
	void DrawNumber(CDC* pDC, CRect&rcItem,CString strNum,  BOOL bSel, BOOL bHasParent);
	CString GetDefaultButton();
	CString GetExpConPath(HTREEITEM hItem);
	CString GetItemBmp(HTREEITEM hItem);
protected:
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnPaint();
	afx_msg void OnTvnItemexpanding(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);

    void OnClickItem(HTREEITEM hItem);
	void SetSelItem(HTREEITEM selItem);
	void SetTheFont();

	DECLARE_MESSAGE_MAP()
public:
	void SetRoomView(HTREEITEM &hparent,std::list<hallNode*> &RoomNode);
	void SetParentHWMD(HWND parentHWND);
	void Clear();
	void SetAllToUnSelected();
	void SetBkImgPath(CString imgPath);
	bool ItemIsVisible( HTREEITEM item );

};