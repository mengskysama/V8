#pragma once
#include "afxwin.h"

enum eArrowPos
{
	ARROW_TOP_LEFT        = 1,   //箭头在左上方
	ARROW_BOTTOM_LEFT,           //左下方      
	ARROW_BOTTOM_MIDDLE,		 //中下方
};

#define WM_TIP_CLICK_BTN	  WM_USER + 260 //点击C179TipCtrl中的链接按钮
#define WM_TIP_CLICK_CLOSE	  WM_USER + 261 //点击C179TipCtrl中的关闭按钮

class CLoginDlg;
class C179TipCtrl :
	public CWnd
{
public:
	C179TipCtrl( eArrowPos arrowpos );
	~C179TipCtrl(void);


	DECLARE_MESSAGE_MAP()
protected:
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC) ;

	void OnClickBtn();
	void OnClickClose();

public:
	BOOL CreateEx(CWnd* pwndParent, UINT uId);

	void SetArrowPos(int iPos);//设置箭头位置
	void SetBorderColor(COLORREF borderColor);//外框颜色
	void SetTipText(CString strText);//设置文字内容
	void SetTipTextColor(COLORREF textColor);//设置文字颜色
	void SetButtonText(CString strBtnText);//设置按钮
	void SetButtonTextColor(COLORREF btnColor);//设置按钮颜色
	void SetClose(BOOL bClose);//设置是否要关闭按钮
	void SetTipBkColor(COLORREF bkColor);

	void ShowTip( int nArrowLeft , int& nWidth , int& nHeight );
	BOOL isHided(){ return m_bHided ; } ;
private:
	CWnd* m_pParentWnd ;
	BOOL m_bHided ;
	int m_arrowPos;
	COLORREF m_colorBorder;
	COLORREF m_bkColor;
	COLORREF m_bkBeginColor;
	COLORREF m_bkEndColor;

	CString m_strTipText;
    COLORREF m_colorTipText;

	CString m_strBtnText;
	COLORREF m_colorBtnText;

	BOOL m_bClose;
	CString m_strImgPath;

	CRect m_rcTipText;
	CRect m_rcBtnText;
	CRect m_rcIconClose;
	CFont m_font;
	HRGN  m_showRgn;
};
