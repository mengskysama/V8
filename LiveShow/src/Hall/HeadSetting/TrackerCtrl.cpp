#include "stdafx.h"
#include "../Hall.h"
#include "TrackerCtrl.h"
#include "..\src\CommonLibrary\include\ui\UIHelper.h"
#include "PicTracker.h"

#define  TRACKER_WIDTH      369    //这里的宽高与PicTracker.cpp定义的宽高都差4个象素大小，只为间隔开来
#define  TRACKER_HEIGHT     243

CTrackerCtrl::CTrackerCtrl(void)
{
	m_tracker.SetCtrl(this);
	m_hBmp = NULL;
	m_tracker.m_nStyle= CRectTracker::solidLine;
	m_bDrag = false;
	m_iStartx = 0;			//默认图片和面板的原点对齐
	m_iStarty = 0;
	m_PrePoint = CPoint(0,0);
	m_ZoomFactor = 1;
	m_bOriginal = false;
	m_bFullScreen = false;
	m_PreZoomFactor = 1;
}

CTrackerCtrl::~CTrackerCtrl(void)
{
	if( m_hBmp)
	{
		DeleteObject( m_hBmp);
		m_hBmp = NULL;
	}
}

bool CTrackerCtrl::Create(CWnd* pwndParent, CRect rc, UINT uId)
{
	//Tracker大小为  180*180
	m_tracker.m_rect.left = rc.Width()/2 - 90;//55;
	m_tracker.m_rect.right = rc.Width()/2 + 90;//55;
	m_tracker.m_rect.top = rc.Height()/2 - 90;//55;
	m_tracker.m_rect.bottom = rc.Height()/2 + 90;//55;
    //限制其大小位置
	CRect rectTrackerLimit;
	rectTrackerLimit = rc;
	rectTrackerLimit.left -= 12;
	rectTrackerLimit.right -= 15;
	rectTrackerLimit.top  -=  67;
	rectTrackerLimit.bottom -=  70;
	m_tracker.SetLimitRect(rectTrackerLimit);
	return CWnd::Create(AfxRegisterWndClass(0), NULL, WS_VISIBLE | WS_CHILD, rc, pwndParent, uId);
}

BEGIN_MESSAGE_MAP(CTrackerCtrl, CWnd)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_SETCURSOR()
	ON_WM_LBUTTONUP()
	ON_MESSAGE(WM_TRACKER_MOVE,OnTrackerMove)
	ON_MESSAGE(WM_MOUSELEAVE, OnMouseLeave)
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()

LRESULT CTrackerCtrl::OnTrackerMove(WPARAM wParam,LPARAM lParam)
{
	Invalidate();
	return 0;
}

LRESULT CTrackerCtrl::OnMouseLeave(WPARAM wParam,LPARAM lParam)
{
	m_bDrag = false;
	return 0;
}

void CTrackerCtrl::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: 在此处添加消息处理程序代码
	// 不为绘图消息调用 CWnd::OnPaint()
	common::ui::CClientRect rcThis(this);

	if (m_hBmp != NULL)
	{
		CBitmap bitmapmem;          
		CBitmap *pOldBit;
		CDC m_pMemDC;
		m_pMemDC.CreateCompatibleDC(&dc); 
		bitmapmem.CreateCompatibleBitmap(&dc, rcThis.Width(), rcThis.Height());
		pOldBit=m_pMemDC.SelectObject(&bitmapmem);

		CRect rect;
		GetWindowRect(&rect);
		ScreenToClient(&rect);
		HBRUSH bgBrush = ::CreateSolidBrush(RGB(255,255,255));
		FillRect(m_pMemDC,&rect,bgBrush);
		DeleteObject(bgBrush);
        //第二、三个参数要加负号，是因为其表示的位置不是相对于m_pMemDC的坐标系
		imgRate.Draw(m_pMemDC,-m_iStartx,-m_iStarty,imgRate.GetWidth(),imgRate.GetHeight(),&rcThis,true);
		
		dc.BitBlt(rcThis.left,rcThis.top,rcThis.Width(),rcThis.Height(),&m_pMemDC,0,0,SRCCOPY); 
		m_pMemDC.SelectObject(pOldBit);
		pOldBit->DeleteObject();
		m_pMemDC.DeleteDC();
		bitmapmem.DeleteObject();

		CRect rectTracker;
		m_tracker.GetTrueRect(&rectTracker);
		rectTracker.left += m_iStartx;
		rectTracker.top += m_iStarty;
		rectTracker.right += m_iStartx;
		rectTracker.bottom += m_iStarty;

		imgSmall.Clear();
		CxImage imgTemp;
		imgRate.Crop(rectTracker,&imgTemp);//不能直接使用imgSmall 否则会出现几列多余的像素没有被清除掉
		imgSmall = imgTemp;
		((CPicTracker*)GetParent())->m_picShowCtrl.SetBitmap(imgSmall.MakeBitmap());
		((CPicTracker*)GetParent())->m_picShowCtrl2.SetBitmap(imgSmall.MakeBitmap());
		if (rectTracker.left < 0)
		{
			((CPicTracker*)GetParent())->m_picShowCtrl.SetStartx(-rectTracker.left);
			((CPicTracker*)GetParent())->m_picShowCtrl2.SetStartx(-rectTracker.left);
		}
		else
		{
			((CPicTracker*)GetParent())->m_picShowCtrl.SetStartx(0);
			((CPicTracker*)GetParent())->m_picShowCtrl2.SetStartx(0);
		}
		if (rectTracker.top<0)
		{
			((CPicTracker*)GetParent())->m_picShowCtrl.SetStarty(-rectTracker.top);
			((CPicTracker*)GetParent())->m_picShowCtrl2.SetStarty(-rectTracker.top);
		}
		else
		{
			((CPicTracker*)GetParent())->m_picShowCtrl.SetStarty(0);
			((CPicTracker*)GetParent())->m_picShowCtrl2.SetStarty(0);
		}
		((CPicTracker*)GetParent())->m_picShowCtrl.Invalidate();
		((CPicTracker*)GetParent())->m_picShowCtrl2.Invalidate();
		m_tracker.Draw(&dc);
	}
}

void CTrackerCtrl::SetBitmap(HBITMAP hBmp)
{
	if( m_hBmp)
	{
		DeleteObject( m_hBmp);
		m_hBmp = NULL;
	}

	m_hBmp = hBmp;
	img.CreateFromHBITMAP(m_hBmp);
	if (img.GetWidth()/img.GetHeight() > TRACKER_WIDTH/TRACKER_HEIGHT)//图片比较"宽"的情况，对图片宽度进行缩放到tracker的"宽度"，然后高度同比例缩放
	{
		if (img.GetWidth()*MIN_ZOOM_FACTOR > TRACKER_WIDTH)
		{
			CxImage imgRate;
			int width = TRACKER_WIDTH*MAX_ZOOM_FACTOR;
			int height = (float)width*(float)(img.GetHeight())/(float)img.GetWidth();
			img.Resample2(width,height,CxImage::InterpolationMethod::IM_NEAREST_NEIGHBOUR,CxImage::OverflowMethod::OM_REPEAT,&imgRate);
			img = imgRate;
		}
	}
	else //图片比较"高"的情况,对图片高度进行缩放到tracker的"高度"，然后宽度同比例缩放
	{
		if (img.GetHeight()*MIN_ZOOM_FACTOR > TRACKER_HEIGHT)
		{
			CxImage imgRate;
			int height = TRACKER_HEIGHT*MAX_ZOOM_FACTOR;
			int width = (float)height*(float)(img.GetWidth())/(float)img.GetHeight();
			img.Resample2(width,height,CxImage::InterpolationMethod::IM_NEAREST_NEIGHBOUR,CxImage::OverflowMethod::OM_REPEAT,&imgRate);
			img = imgRate;
		}
	}
	
	//加载进来的时候应该处于全屏状态
	OnFullScreen();
}

void CTrackerCtrl::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	m_bDrag = true;
	if(!m_tracker.Track(this,point,TRUE))
	{
		SetCapture();
	}
	m_PrePoint = point;
	CWnd::OnLButtonDown(nFlags, point);
}

void CTrackerCtrl::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	//用于检测mouse leave
	TRACKMOUSEEVENT tme;
	tme.cbSize = sizeof(tme);
	tme.dwFlags  = TME_HOVER | TME_LEAVE;
	tme.hwndTrack = m_hWnd;
	tme.dwHoverTime = HOVER_DEFAULT;
	::_TrackMouseEvent(&tme);
	//end

	if (m_bDrag)
	{
		CRect rcThis;
		GetClientRect(&rcThis);
		if (imgRate.GetWidth()>=rcThis.Width())
		{
			if (point.x > m_PrePoint.x)
			{
				m_iStartx = (m_iStartx -(  point.x - m_PrePoint.x ))<0?m_iStartx:(m_iStartx -(  point.x - m_PrePoint.x ));
			}
			else
			{
				m_iStartx = (m_iStartx -(  point.x - m_PrePoint.x ) + rcThis.Width())>imgRate.GetWidth()?m_iStartx:(m_iStartx -(  point.x - m_PrePoint.x ));
			}
		}
		else
		{
			if (point.x < m_PrePoint.x)
			{
				m_iStartx = (m_iStartx -(  point.x - m_PrePoint.x ))>0?m_iStartx:(m_iStartx -(  point.x - m_PrePoint.x ));
			}
			else
			{
				m_iStartx = (m_iStartx -(  point.x - m_PrePoint.x ) + rcThis.Width())<imgRate.GetWidth()?m_iStartx:(m_iStartx -(  point.x - m_PrePoint.x ));
			}
		}

		if (imgRate.GetHeight()>=rcThis.Height())
		{
			if (point.y > m_PrePoint.y)
			{
				m_iStarty = (m_iStarty -(  point.y - m_PrePoint.y ))<0?m_iStarty:(m_iStarty -(  point.y - m_PrePoint.y ));
			}
			else
			{
				m_iStarty = (m_iStarty -(  point.y - m_PrePoint.y )  + rcThis.Height())>imgRate.GetHeight()?m_iStarty:(m_iStarty -(  point.y - m_PrePoint.y));
			}
		}
		else
		{
			if (point.y < m_PrePoint.y)
			{
				m_iStarty = (m_iStarty -(  point.y - m_PrePoint.y ))>0?m_iStarty:(m_iStarty -(  point.y - m_PrePoint.y ));
			}
			else
			{
				m_iStarty = (m_iStarty -(  point.y - m_PrePoint.y )  + rcThis.Height())<imgRate.GetHeight()?m_iStarty:(m_iStarty -(  point.y - m_PrePoint.y));
			}
		}
		m_PrePoint = point;
		Invalidate();
	}
	else
	{
		ReleaseCapture();
	}
	CWnd::OnMouseMove(nFlags, point);
}

BOOL CTrackerCtrl::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (pWnd == this && m_tracker.SetCursor(this, nHitTest))
		return TRUE;
	if (m_bDrag)
	{
		SetCursor(AfxGetApp()->LoadCursor(IDC_CURSOR1));
	}
	else
	{
		SetCursor(AfxGetApp()->LoadCursor(IDC_CURSOR2));
	}
	return CWnd::OnSetCursor(pWnd, nHitTest, message);
}

void CTrackerCtrl::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	m_bDrag = false;
	ReleaseCapture();
	CWnd::OnLButtonUp(nFlags, point);
}

void CTrackerCtrl::OnZoomOut()
{
	m_bOriginal = false;
	m_bFullScreen = false;

	if (m_ZoomFactor<=MIN_ZOOM_FACTOR) return;
	

	m_ZoomFactor /= ZOOM_FACTOR_PER_SLIDER_POS;
	CRect  rcThis;
	GetClientRect(rcThis);

	if (m_ZoomFactor<1)//缩小
	{
		bool	bRet = img.Resample2(img.GetWidth()*m_ZoomFactor,img.GetHeight()*m_ZoomFactor,CxImage::InterpolationMethod::IM_NEAREST_NEIGHBOUR,CxImage::OverflowMethod::OM_REPEAT,&imgRate);
		if (bRet == false)
		{
			TRACE(_T("采样失败\n\r"));
			return;
		}
	}
	else//放大
	{
		 bool	bRet = img.Resample(img.GetWidth()*m_ZoomFactor,img.GetHeight()*m_ZoomFactor,1,&imgRate);
		 if (bRet == false)
		 {
			 TRACE(_T("采样失败\n\r"));
			 return;
		 }
	}
	CRect rectTracker;
	m_tracker.GetTrueRect(&rectTracker);
	//由于tracker的框是可拖动，所以大缩放时，是靠近tracker框的中心进行缩放的，而不是外框中心。所以计算startx和starty时要注意
	float fStartx = (m_ZoomFactor/m_PreZoomFactor)*((rectTracker.left+rectTracker.right)/2 + m_iStartx) -  (rectTracker.left+rectTracker.right)/2;
	m_iStartx = fStartx>0?(fStartx*2+1)/2:(fStartx*2-1)/2;
	
	float fStarty = ((rectTracker.top + rectTracker.bottom)/2 + m_iStarty)*(m_ZoomFactor/m_PreZoomFactor) - (rectTracker.top + rectTracker.bottom)/2;
	m_iStarty = fStarty>0?(fStarty*2+1)/2:(fStarty*2-1)/2;
	m_PreZoomFactor = m_ZoomFactor;
	Invalidate();
}

void CTrackerCtrl::OnZoomIn()//计算方法类似OnZoomOut()
{
	m_bOriginal = false;
	m_bFullScreen = false;

	if (m_ZoomFactor>=MAX_ZOOM_FACTOR) return;
	
	m_ZoomFactor*=ZOOM_FACTOR_PER_SLIDER_POS;
	CRect  rcThis;
	GetClientRect(rcThis);
	CxImage imgTemp;

	if (m_ZoomFactor<1)
	{
		bool	bRet = img.Resample2(img.GetWidth()*m_ZoomFactor,img.GetHeight()*m_ZoomFactor,CxImage::InterpolationMethod::IM_NEAREST_NEIGHBOUR,CxImage::OverflowMethod::OM_REPEAT,&imgRate);
		if (bRet == false)
		{
			TRACE(_T("采样失败\n\r"));
			return;
		}
	}
	else
	{
		bool	bRet = img.Resample(img.GetWidth()*m_ZoomFactor,img.GetHeight()*m_ZoomFactor,1,&imgRate);
		if (bRet == false)
		{
			TRACE(_T("采样失败\n\r"));
			return;
		}
	}
	CRect rectTracker;
	m_tracker.GetTrueRect(&rectTracker);
	//计算公式： (a-x)/(a-b) = k;
	float fStartx = (m_ZoomFactor/m_PreZoomFactor)*((rectTracker.left+rectTracker.right)/2 + m_iStartx) -  (rectTracker.left+rectTracker.right)/2;
	m_iStartx = fStartx>0?(fStartx*2+1)/2:(fStartx*2-1)/2;

	float fStarty = ((rectTracker.top + rectTracker.bottom)/2 + m_iStarty)*(m_ZoomFactor/m_PreZoomFactor) - (rectTracker.top + rectTracker.bottom)/2;
	m_iStarty = fStarty>0?(fStarty*2+1)/2:(fStarty*2-1)/2;

	m_PreZoomFactor = m_ZoomFactor;
	Invalidate();
}

void CTrackerCtrl::OnOriginal()
{
	CRect  rcThis;
	GetClientRect(rcThis);

	m_bOriginal = true;
	m_bFullScreen = false;
	m_ZoomFactor  = 1;
	m_PreZoomFactor = 1;
	bool	bRet = img.Resample(img.GetWidth()*m_ZoomFactor,img.GetHeight()*m_ZoomFactor,1,&imgRate);
	m_iStartx = imgRate.GetWidth() - rcThis.Width();
	m_iStartx /= 2;
	m_iStarty = imgRate.GetHeight() - rcThis.Height();
	m_iStarty /= 2;
	Invalidate();
}

void CTrackerCtrl::OnFullScreen()
{
	CRect  rcThis;
	GetClientRect(rcThis);
	m_bFullScreen = true;
	m_bOriginal = false;
	
	float rate = (float)(img.GetWidth())/(float)(img.GetHeight());
	float fClient = (float)(rcThis.Width())/(float)(rcThis.Height());
	float hScale = (float)(img.GetHeight())/(float)(rcThis.Height());
	float wScale = (float)(img.GetWidth())/(float)(rcThis.Width());
	if (hScale<=1&&wScale<=1)
	{
		//如果原始图片比外框小，则按原始处理
		OnOriginal();
		return;
	}
	//以下类似SetBitmap()的一种特殊情况
	if(rate < fClient)//图片比较"高"的情况,对图片高度进行缩放到tracker的高度，然后宽度同比例缩放
	{
		img.Resample2(img.GetWidth()/hScale,img.GetHeight()/hScale,CxImage::InterpolationMethod::IM_NEAREST_NEIGHBOUR,CxImage::OverflowMethod::OM_REPEAT,&imgRate);
		m_ZoomFactor  = m_PreZoomFactor = 1/hScale;
	}
	else//图片比较"宽"的情况，对图片宽度进行缩放到tracker的宽度，然后高度同比例缩放
	{
		img.Resample2(img.GetWidth()/wScale,img.GetHeight()/wScale,CxImage::InterpolationMethod::IM_NEAREST_NEIGHBOUR,CxImage::OverflowMethod::OM_REPEAT,&imgRate);
		m_ZoomFactor  = m_PreZoomFactor = 1/wScale;
	}
	m_iStartx = imgRate.GetWidth() - rcThis.Width();
	m_iStartx/=2;

	m_iStarty = imgRate.GetHeight() - rcThis.Height();
	m_iStarty/=2;
	Invalidate();
}

BOOL CTrackerCtrl::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	return TRUE;
}

void CTrackerCtrl::SetDrag(bool bDrag)
{
	m_bDrag = bDrag;
}

float CTrackerCtrl::GetZoomFactor()
{
	return m_ZoomFactor;
}

void CTrackerCtrl::SetZoomFactor(int pos)
{
	if (pos == 50)//原始大小
	{
		m_ZoomFactor = 1;
	}
	else if (pos<50)
	{
		m_ZoomFactor = 1;
		for (int i=0; i<50 - pos; i++)
		{
			m_ZoomFactor /= ZOOM_FACTOR_PER_SLIDER_POS;
		}
	}
	else 
	{
		m_ZoomFactor = 1;
		for ( int i=0; i<pos - 50; i++)
		{
			m_ZoomFactor *= ZOOM_FACTOR_PER_SLIDER_POS;
		}
	}
	CRect  rcThis;
	GetClientRect(rcThis);
	//DragonZ:以下的if-else的代码段在原版(from Rainbow)中是一模一样的，改之
//原版：
	/*if (m_ZoomFactor<1)
	{   
		bool	bRet = img.Resample(img.GetWidth()*m_ZoomFactor,img.GetHeight()*m_ZoomFactor,1,&imgRate);
		if (bRet == false)
		{
			TRACE(_T("采样失败\n\r"));
			return;
		}
	}
	else
	{
		bool	bRet = img.Resample(img.GetWidth()*m_ZoomFactor,img.GetHeight()*m_ZoomFactor,1,&imgRate);
		if (bRet == false)
		{
			TRACE(_T("采样失败\n\r"));
			return;
		}
	}*/

	//DragonZ:
	bool bRet = img.Resample(img.GetWidth()*m_ZoomFactor,img.GetHeight()*m_ZoomFactor,1,&imgRate);
	//bool bRet = true;
	if (bRet == false)
	{
		TRACE(_T("采样失败\n\r"));
		return;
	}

	CRect rectTracker;
	m_tracker.GetTrueRect(&rectTracker);
	float fStartx = (m_ZoomFactor/m_PreZoomFactor)*((rectTracker.left+rectTracker.right)/2 + m_iStartx) -  (rectTracker.left+rectTracker.right)/2;
	m_iStartx = fStartx>0?(fStartx*2+1)/2:(fStartx*2-1)/2;

	float fStarty = ((rectTracker.top + rectTracker.bottom)/2 + m_iStarty)*(m_ZoomFactor/m_PreZoomFactor) - (rectTracker.top + rectTracker.bottom)/2;
	m_iStarty = fStarty>0?(fStarty*2+1)/2:(fStarty*2-1)/2;
	m_PreZoomFactor = m_ZoomFactor;
	Invalidate();
}
