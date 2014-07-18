#pragma once
#include "afxwin.h"

#define  WM_ONCLICK_MODIFYNAME_CLOSE         WM_USER + 460

class CModifyNameTipWnd :
	public CWnd
{
public:
	CModifyNameTipWnd(void);
	~CModifyNameTipWnd(void);

protected:
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnClose();

	void Draw();

	DECLARE_MESSAGE_MAP()
public:
	BOOL CreateEx(CWnd* pwndParent);
	void SetTipsImage(CString& strTipsImg);
	void SetCloseImage(CString& strCloseImg);
	void ShowModifyNameTipsWnd(CRect& rc);//改变遮罩框大小及位置 
private:
	//关于窗口透明
	BLENDFUNCTION m_blend ;

	CString m_strTipsImgPath;
	CString m_strCloseImgPath;
	CRect m_rcClose;
};
