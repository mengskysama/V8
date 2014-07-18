#include "../../StdAfx.h"
#pragma once

#include "..\..\include\ui\AppBarImpl.h"
#include "AppBar.h"

#define HM_NONE     0   //不收缩
#define HM_TOP      1   //向上收缩
#define HM_BOTTOM   2   //向下收缩
#define HM_LEFT     3   //向左收缩
#define HM_RIGHT    4   //向右收缩
#define DETASTEP 10
#define INTERVAL    20  //触发粘附时鼠标与屏幕边界的最小间隔,单位为象素
#define INFALTE     10  //触发收缩时鼠标与窗口边界的最小间隔,单位为象素
/*
默认构造函数，一般为CWnd
@void 无构造函数
无返回值
*/
namespace common { namespace ui
{
template<typename TBaseWnd>
CAppBarImpl<TBaseWnd>::CAppBarImpl(void):public TBaseWnd
{
	m_appbar = new CAppBar();
	g_bDlgStatus = 0;
	m_hideMode = CAppBar::APPBAR_DOCKING_NONE;

}

/*
重载构造函数，一般为CDialog为基类的子类
@UINT  [in]  nIDTemplate 无构造函数
@CWnd* [in]  pParent 无构造函数
无返回值
*/
template<typename TBaseWnd>
CAppBarImpl<TBaseWnd>::CAppBarImpl(UINT nIDTemplate,CWnd* pParent):TBaseWnd(nIDTemplate,pParent)
{
	m_appbar = new CAppBar();
	g_bDlgStatus = 0;
	m_hideMode = CAppBar::APPBAR_DOCKING_NONE;
}
//template<>
//CAppBarImpl<CDialog>::CAppBarImpl(UINT nIDTemplate,CWnd* pParent):CDialog(nIDTemplate,pParent)
//{
//	m_appbar = new CAppBar();
//	g_bDlgStatus = 0;
//}
/*
默认析造函数
@void 无构造函数
无返回值
*/
template<typename TBaseWnd>
CAppBarImpl<TBaseWnd>::~CAppBarImpl(void)
{
	if (m_appbar)
		delete m_appbar;
}
BEGIN_TEMPLATE_MESSAGE_MAP(CAppBarImpl,TBaseWnd,TBaseWnd)
	ON_WM_WINDOWPOSCHANGED()
	ON_WM_ACTIVATE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_NCHITTEST()
	ON_WM_TIMER()
	ON_MESSAGE(APPBAR_CALLBACK, OnAppBarNotify)
	ON_WM_SHOWWINDOW()
	ON_WM_NCLBUTTONUP()
	ON_WM_MOVING()
	ON_WM_SYSCOMMAND()
	ON_WM_SIZING()
	ON_WM_CREATE()
	ON_MESSAGE(WM_SLIDEOUT_END, OnSlideoutEnd)
	ON_MESSAGE(WM_SLIDEIN_END, OnSlideinEnd)
END_MESSAGE_MAP()

/*
窗口位置改变标准消息响应函数
@WINDOWPOS*  [in|out]  nIDTemplate 窗口位置信息
@return void 返回值为空
*/
template<typename TBaseWnd>
void CAppBarImpl<TBaseWnd>::OnWindowPosChanged(WINDOWPOS* lpwndpos)
{
	__super::OnWindowPosChanged(lpwndpos);
	m_appbar->OnWindowPosChanged(lpwndpos);
}

/*
窗口状态激活标准消息响应函数
@UINT  [in] nState       状态标记值
@CWnd* [in] pWndOther    失去焦点的窗口
@BOOL  [in] bMinimized   窗口是否最小化
@return void 返回值为空
*/
template<typename TBaseWnd>
void CAppBarImpl<TBaseWnd>::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
{
	__super::OnActivate(nState, pWndOther, bMinimized);
	m_appbar->OnActivate(nState, pWndOther, bMinimized);
}

/*
窗口内鼠标按下标准消息响应函数
@UINT   [in] nFlags       状态标记值
@CPoint [in] point        当前光标客户端位置
@return void 返回值为空
*/
template<typename TBaseWnd>
void CAppBarImpl<TBaseWnd>::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	SetCapture();
	m_appbar->OnLButtonDown(nFlags, point);
	PostMessage(WM_NCLBUTTONDOWN,HTCAPTION,0);	
	__super::OnLButtonDown(nFlags, point);
}

/*
窗口内鼠标松开标准消息响应函数
@UINT   [in] nFlags       状态标记值
@CPoint [in] point        当前光标客户端位置
@return void 返回值为空
*/
template<typename TBaseWnd>
void CAppBarImpl<TBaseWnd>::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	m_appbar->OnLButtonUp(nFlags, point);
	ReleaseCapture();
	__super::OnLButtonUp(nFlags, point);
}

/*
窗口内鼠标移动标准消息响应函数
@UINT   [in] nFlags       状态标记值
@CPoint [in] point        当前光标客户端位置
@return void 返回值为空
*/
template<typename TBaseWnd>
void CAppBarImpl<TBaseWnd>::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	//    m_appbar->OnMouseMove(nFlags, point);
	__super::OnMouseMove(nFlags, point);
}

/*
窗口击中测试标准消息响应函数
@CPoint [in] point        当前光标客户端位置
@return void 返回值为空
*/
template<typename TBaseWnd>
LRESULT CAppBarImpl<TBaseWnd>::OnNcHitTest(CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	m_appbar->OnNcHitTest(CDialog::OnNcHitTest(point));
	return __super::OnNcHitTest(point);
}

/*
窗口消息处理标准消息响应函数
@UINT_PTR [in] nIDEvent        消息ID标记值
@return void 返回值为空
*/
template<typename TBaseWnd>
void CAppBarImpl<TBaseWnd>::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if(nIDEvent>TIMER_BASE)
		m_appbar->OnTimer(nIDEvent);
	__super::OnTimer(nIDEvent);
}

/*
窗口内鼠标移动标准消息响应函数
@WPARAM   [in] wParam       消息ID标记
@LPARAM   [in] lParam       消息参数结构指针
@return LRESULT 返回值为空
*/
template<typename TBaseWnd>
LRESULT CAppBarImpl<TBaseWnd>::OnAppBarNotify( WPARAM wParam, LPARAM lParam/*bHandled*/)
{
	BOOL isFinsih = TRUE;
	m_appbar->OnAppBarNotify(0,wParam,lParam,isFinsih);
	return 0;

}

/*
窗口内鼠标移动标准消息响应函数
@BOOL   [in] bShow       消息ID标记
@UINT   [in] nStatus     消息参数结构指针
@return LRESULT 返回值为空
*/
template<typename TBaseWnd>
void CAppBarImpl<TBaseWnd>::OnShowWindow(BOOL bShow, UINT nStatus)
{
	__super::OnShowWindow(bShow, nStatus);
}

/*
窗口标题栏鼠标弹起标准消息响应函数
@UINT   [in] nHitTest     击中状态标记值
@CPoint [in] point        当前光标客户端位置
@return void 返回值为空
*/
template<typename TBaseWnd>
void CAppBarImpl<TBaseWnd>::OnNcLButtonUp(UINT nHitTest, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	//m_appbar->m_bDraging = false;
	LRESULT rs =  OnNcHitTest(point);
	m_appbar->SliderWindow(m_hideMode);
	__super::OnNcLButtonUp(nHitTest, point);
}

/*
窗口移动时标准消息响应函数
@UINT   [in] nHitTest     击中状态标记值
@CPoint [in] point        当前光标客户端位置
@return void 返回值为空
*/
template<typename TBaseWnd>
void CAppBarImpl<TBaseWnd>::OnMoving(UINT fwSide, LPRECT pRect)
{
	FixMoving(fwSide,  pRect);   
	__super::OnMoving(fwSide, pRect);
	if((!m_appbar->GetDragContent())&&(m_hideMode == CAppBar::APPBAR_DOCKING_NONE)
		||m_appbar->GetDragContent())
		m_appbar->SliderWindow(m_hideMode);
	// TODO: 在此处添加消息处理程序代码

}
template<typename TBaseWnd>
BOOL CAppBarImpl<TBaseWnd>::AdjustPos(CRect* lpRect)
{//自动靠边
	int iSX=GetSystemMetrics(SM_CXFULLSCREEN);
	int iSY=GetSystemMetrics(SM_CYFULLSCREEN);
	RECT rWorkArea;
	BOOL bResult = SystemParametersInfo(SPI_GETWORKAREA,  
		sizeof(RECT),
		&rWorkArea,
		0);    
	CRect rcWA;
	if(!bResult)
	{//如果调用不成功就利用GetSystemMetrics获取屏幕面积
		rcWA=CRect(0,0,iSX,iSY);
	}
	else
		rcWA=rWorkArea;

	int iX=lpRect->left;
	int iY=lpRect->top;

	if(iX < rcWA.left + DETASTEP && iX!=rcWA.left)
	{//调整左
		//pWnd->SetWindowPos(NULL,rcWA.left,iY,0,0,SWP_NOSIZE);
		lpRect->OffsetRect(rcWA.left-iX,0);

		g_bDlgStatus=1;
		AdjustPos(lpRect);
		return TRUE;
	}
	if(iY < rcWA.top + DETASTEP && iY!=rcWA.top)
	{//调整上
		//pWnd->SetWindowPos(NULL ,iX,rcWA.top,0,0,SWP_NOSIZE);
		lpRect->OffsetRect(0,rcWA.top-iY);

		g_bDlgStatus=2;
		AdjustPos(lpRect);
		return TRUE;
	}
	if(iX + lpRect->Width() > rcWA.right - DETASTEP && iX !=rcWA.right-lpRect->Width())
	{//调整右
		//pWnd->SetWindowPos(NULL ,rcWA.right-rcW.Width(),iY,0,0,SWP_NOSIZE);
		lpRect->OffsetRect(rcWA.right-lpRect->right,0);

		g_bDlgStatus=3;
		AdjustPos(lpRect);
		return TRUE;
	}
	if(iY + lpRect->Height() > rcWA.bottom - DETASTEP && iY !=rcWA.bottom-lpRect->Height())
	{//调整下
		//pWnd->SetWindowPos(NULL ,iX,rcWA.bottom-rcW.Height(),0,0,SWP_NOSIZE);

		g_bDlgStatus=4;	
		lpRect->OffsetRect(0,rcWA.bottom-lpRect->bottom);
		return TRUE;
	}
	//	

	UINT uFlag = CAppBar::APPBAR_DOCKING_NONE;
	switch(g_bDlgStatus)	
	{
	case 1:
		uFlag = CAppBar::APPBAR_DOCKING_LEFT;
		break;
	case 2:
		uFlag = CAppBar::APPBAR_DOCKING_TOP;
		break;
	case 3:
		uFlag = CAppBar::APPBAR_DOCKING_RIGHT;
		break;
	case 4:
		uFlag = CAppBar::APPBAR_DOCKING_BOTTOM;
		break;
	}
	m_appbar->SliderWindow(uFlag);
	return FALSE;
}
template<typename TBaseWnd>
void  CAppBarImpl<TBaseWnd>::FixBorderSizing(UINT fwSide, LPRECT pRect)
{
	INT screenHeight = GetSystemMetrics(SM_CYSCREEN);
	INT screenWidth  = GetSystemMetrics(SM_CXSCREEN);
	INT height = pRect->bottom - pRect->top;
	INT width  = pRect->right - pRect->left;
	int Interval = 1;
	if (pRect->top <= Interval)
	{   //粘附在上边
		//		pRect->bottom = height - m_edgeHeight;
		//		pRect->top = -m_edgeHeight;
		m_hideMode = CAppBar::APPBAR_DOCKING_TOP;
	}
	else if(pRect->bottom >= (screenHeight - Interval ))
	{   //粘附在下边
		//		pRect->top = screenHeight - height;
		//		pRect->bottom = screenHeight;
		m_hideMode = CAppBar::APPBAR_DOCKING_BOTTOM;
	}
	else if (pRect->left < Interval)
	{	//粘附在左边	
		if(!m_isSizeChanged)
		{
			CRect tRect;
			GetWindowRect(tRect);
			//			m_oldWndHeight = tRect.Height();			
		}
		//		pRect->right = width;
		//		pRect->left = 0;

		//		m_isSizeChanged = TRUE;
		m_hideMode = CAppBar::APPBAR_DOCKING_LEFT;
	}
	else if(pRect->right >= (screenWidth - Interval))
	{   //粘附在右边
		if(!m_isSizeChanged)
		{
			CRect tRect;
			GetWindowRect(tRect);
			//			m_oldWndHeight = tRect.Height();						
		}
		//		pRect->left = screenWidth - width;
		//		pRect->right = screenWidth;

		//		m_isSizeChanged = TRUE;
		m_hideMode = CAppBar::APPBAR_DOCKING_RIGHT;
	}
	else
	{   //不粘附
		m_hideMode = CAppBar::APPBAR_DOCKING_NONE;

	}
};
template<typename TBaseWnd>
void  CAppBarImpl<TBaseWnd>::FixMoving(UINT fwSide, LPRECT pRect,BOOL bIsMoving)
{

	POINT curPos;
	GetCursorPos(&curPos);
	int Interval = 0;
	if (bIsMoving)
		Interval = INTERVAL;

	INT screenHeight = GetSystemMetrics(SM_CYSCREEN);
	INT screenWidth  = GetSystemMetrics(SM_CXSCREEN);
	INT height = pRect->bottom - pRect->top;
	INT width  = pRect->right - pRect->left;

	if (curPos.y <= Interval)
	{   //粘附在上边
		pRect->bottom = height - m_edgeHeight;
		pRect->top = -m_edgeHeight;
		m_hideMode = CAppBar::APPBAR_DOCKING_TOP;
	}
	else if(curPos.y >= (screenHeight - Interval ))
	{   //粘附在下边
		pRect->top = screenHeight - height;
		pRect->bottom = screenHeight;
		m_hideMode = CAppBar::APPBAR_DOCKING_BOTTOM;
	}
	else if (curPos.x < Interval)
	{	//粘附在左边	
		if(!m_isSizeChanged)
		{
			CRect tRect;
			GetWindowRect(tRect);
			//			m_oldWndHeight = tRect.Height();			
		}
		pRect->right = width;
		pRect->left = 0;

		m_isSizeChanged = TRUE;
		m_hideMode = CAppBar::APPBAR_DOCKING_LEFT;
	}
	else if(curPos.x >= (screenWidth - Interval))
	{   //粘附在右边
		if(!m_isSizeChanged)
		{
			CRect tRect;
			GetWindowRect(tRect);
			//			m_oldWndHeight = tRect.Height();						
		}
		pRect->left = screenWidth - width;
		pRect->right = screenWidth;

		m_isSizeChanged = TRUE;
		m_hideMode = CAppBar::APPBAR_DOCKING_RIGHT;
	}
	else
	{   //不粘附
		m_hideMode = CAppBar::APPBAR_DOCKING_NONE;

	}
	//m_appbar->SliderWindow(m_hideMode);
}
template<typename TBaseWnd>
void CAppBarImpl<TBaseWnd>::OnSysCommand(UINT nID, LPARAM lParam)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	__super::OnSysCommand(nID, lParam);
	if( (nID & 0xFFF0) == SC_MOVE )
	{
		PostMessage(WM_NCLBUTTONUP, HTCAPTION, lParam);
	}
}
template<typename TBaseWnd>
void CAppBarImpl<TBaseWnd>::OnSizing(UINT fwSide, LPRECT pRect)
{
	__super::OnSizing(fwSide, pRect);
	if(m_appbar->m_uCurrentDockingSide == CAppBar::APPBAR_DOCKING_NONE)
	{
		FixBorderSizing( fwSide,  pRect);
		m_appbar->SliderWindow(m_hideMode);
	}
	else 
	{
		FixSizing( fwSide,  pRect);
		if(m_hideMode != CAppBar::APPBAR_DOCKING_NONE)
			m_appbar->SliderWindow(m_hideMode);
		else 
			m_appbar->m_uCurrentDockingSide = CAppBar::APPBAR_DOCKING_NONE;

	}

	// TODO: 在此处添加消息处理程序代码
}
template<typename TBaseWnd>
void  CAppBarImpl<TBaseWnd>::FixSizing(UINT fwSide, LPRECT pRect)
{
	int screenHeight = GetSystemMetrics(SM_CYSCREEN);
	int screenWidth  = GetSystemMetrics(SM_CXSCREEN);
	if(pRect->left>INTERVAL&&pRect->top>INTERVAL&&
		pRect->right<screenWidth-INTERVAL&&pRect->bottom<screenHeight-INTERVAL )
	{
		m_hideMode = CAppBar::APPBAR_DOCKING_NONE;
		//	 MessageBox(L"hehe");
	}
	return;
}
template<typename TBaseWnd>
int CAppBarImpl<TBaseWnd>::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (__super::OnCreate(lpCreateStruct) == -1)
		return -1;

	m_appbar->SetHwnd(m_hWnd);
	m_appbar->InitAppBar(CAppBar::APPBAR_DOCKING_ALL, true);
	m_appbar->SetKeepSize(true);
	m_appbar->SetAutoHide(true);
	m_edgeHeight = GetSystemMetrics(SM_CYEDGE);
	m_edgeWidth  = GetSystemMetrics(SM_CXFRAME);

	SystemParametersInfo(SPI_GETDRAGFULLWINDOWS,TRUE,NULL,0) ;   //drag window not just a rect
	return 0;
}
template<typename TBaseWnd>
 BOOL CAppBarImpl<TBaseWnd>::IsAppWndHanging()
{
	if (m_appbar)
	{
		return m_appbar->IsAppWndHanging();
	}
	return TRUE;
}
template<typename TBaseWnd>
 LRESULT CAppBarImpl<TBaseWnd>::OnSlideoutEnd(WPARAM w, LPARAM l)
 {
	 OnBarSlideredOut();
	 return 0;
 }

 template<typename TBaseWnd>
 LRESULT CAppBarImpl<TBaseWnd>::OnSlideinEnd(WPARAM w, LPARAM l)
 {
     OnBarSlideredIn();
	 return 0;
 }

 }}