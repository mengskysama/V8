#pragma once
#include "afxwin.h"

class CCoverWnd :
	public CWnd
{
public:
	CCoverWnd(void);
	~CCoverWnd(void);
	
protected:
	void DrawCoverWndBk();

public:
	BOOL CreateEx(CWnd* pwndParent, CRect& rc);
	void ShowCoverWnd(CRect& rc);//改变遮罩框大小及位置 
private:
	//关于窗口透明
	BLENDFUNCTION m_blend ;

	CString m_strBkImgPath;
public:
	DECLARE_MESSAGE_MAP()
};
