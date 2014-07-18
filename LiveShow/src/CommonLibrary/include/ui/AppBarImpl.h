#pragma once

#include "../Common.h"

//#include "AppBar.h"
///////////////////////////////////////////////////////////////////////////////////////////////////////////
//	class CAppBarImpl
//		give derived window the ability to dock on any side of the screen
//		gain behavior similar with Windows Task bar
//      inherited on specified kind of wnd
//  author:longing hu
//  corp 51.com 
//  mail:chinalonging@163.com
//  work on MFC environment,it make the dialogue derived 
//  from the class action as slider bar on either side of the screen;
//  date 08/JAN/2010
#define WM_SLIDEOUT_END     WM_USER + 500
#define IMPLMENT_DIALOGUEBASE_FUNCTOR(TWnd)\
	template  BOOL CAppBarImpl<TWnd>::IsAppWndHanging();\
    template  CAppBarImpl<TWnd>::CAppBarImpl(UINT ,CWnd* );\
    template  CAppBarImpl<TWnd>::~CAppBarImpl();\

class CAppBar;

namespace common { namespace ui
{

template<typename TBaseWnd>
class CAppBarImpl:public TBaseWnd 
{
    typedef TBaseWnd T_Base ;
public:
	CAppBarImpl(void);
	CAppBarImpl(UINT nIDTemplate,CWnd* pParent = NULL);	

	/*
    检测窗口是否处于处于无靠边的悬挂状态
	@return BOOL 返回是否空悬状态
	*/
	BOOL IsAppWndHanging();
	/*
	窗口靠边滑入过程结束，以后处于隐藏态
	@return void 无返回值
	*/
	virtual void OnBarSlideredIn()
	{
		;
	}
	/*
	窗口靠边滑出过程结束，以后处于隐藏态
	@return void 无返回值
	*/
	virtual void OnBarSlideredOut()
	{
		;
	}
protected:	
	virtual ~CAppBarImpl(void);
	DECLARE_MESSAGE_MAP()
	afx_msg void OnWindowPosChanged(WINDOWPOS* lpwndpos);
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg LRESULT OnNcHitTest(CPoint point);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg LRESULT OnAppBarNotify( WPARAM wParam, LPARAM lParam /*bHandled*/);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnNcLButtonUp(UINT nHitTest, CPoint point);
	afx_msg void OnMoving(UINT fwSide, LPRECT pRect);
	afx_msg void OnSizing(UINT fwSide, LPRECT pRect);
 	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);	
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg LRESULT OnSlideoutEnd(WPARAM w, LPARAM l);
	afx_msg LRESULT OnSlideinEnd(WPARAM w, LPARAM l);
private:
	CAppBar * m_appbar;
	int g_bDlgStatus;
private:
	/*
	控制移动窗口过程中调整窗口位置
	@CRect*   [in|out] lpRect     窗口矩形的指针参数
	@return BOOL 返回是否靠边状态
	*/
	BOOL AdjustPos(CRect* lpRect);

	/*
	控制移动窗口过程中测试dock状态
	@UINT   [in] fwSide     标准响应函数的边界信息
	@LPRECT [in] pRect      目标区的矩形位置
    @BOOL   [in] bIsMoving  是否用户控制移动
	@return void 返回空值
	*/
	void  FixMoving(UINT fwSide, LPRECT pRect,BOOL bIsMoving = TRUE);


	/*
	控制拖动窗口边界过程中测试dock状态
	@UINT   [in] fwSide     标准响应函数的边界信息
	@LPRECT [in] pRect      目标区的矩形位置
	@return void 返回空值
	*/
	void  FixSizing(UINT fwSide, LPRECT pRect);

	/*
	控制拖动窗口边界过程中中调整窗口位置
	@UINT   [in] fwSide     标准响应函数的边界信息
	@LPRECT [in] pRect      目标区的矩形位置
	@return void 返回空值
	*/
	void  FixBorderSizing(UINT fwSide, LPRECT pRect);

	INT  m_edgeHeight;      //边缘高度
	INT  m_edgeWidth;       //边缘宽度
	BOOL m_isSizeChanged;   //窗口大小是否改变了
	UINT m_hideMode;


};
//#include "AppBarImpl.cpp"

} } //namespace common::ui
#ifdef _SUPPORT_COMMON_EXPORT
#else 
#include "..\CommonLibrary\src\ui\AppBarImpl.cpp"
#endif