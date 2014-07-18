#pragma once

#include "..\src\CommonLibrary\thirdlibrary\CxImage\include\ximage.h"
#include "RectTrackerEx.h"

#define  WM_TRACKER_MOVE   WM_USER + 100

#define  MIN_ZOOM_FACTOR             0.298f   //最小的放大缩小倍数
#define  MAX_ZOOM_FACTOR             3.43711f //最大的放大缩小倍数
#define  ZOOM_FACTOR_PER_SLIDER_POS  1.025    //SLIDER每滑动一个单位长度的放大缩小倍数          

enum EAlignMode
{
	eAMode_Normal,								//原始大小
	eAMode_SizeToCtrlIfLarge,					//全部显示
};

class CTrackerCtrl
	: public CWnd
{
public:
	bool Create(CWnd* pwndParent, CRect rc, UINT uId);
	CTrackerCtrl(void);
	~CTrackerCtrl(void);

public:
	void SetBitmap(HBITMAP hBmp);

private:
	HBITMAP m_hBmp;
	bool    m_bDrag;
	CPoint  m_PrePoint;
	int     m_iStartx;		//图片的相对位置
	int     m_iStarty;	
	CRectTrackerEx m_tracker;

	float m_ZoomFactor;    //缩放
	float m_ZoomFactorDiv;	//缩放的倒数
	float m_PreZoomFactor;
  
	CxImage  img;
	CxImage  imgSmall;
	CxImage  imgRate;

	bool  m_bOriginal;
	bool  m_bFullScreen;
public:
	DECLARE_MESSAGE_MAP()
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);


	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg LRESULT OnTrackerMove(WPARAM wParam,LPARAM lParam);
	afx_msg LRESULT OnMouseLeave(WPARAM wParam,LPARAM lParam);
public:
	void  OnZoomOut();               //缩小 
	void  OnZoomIn();		         //放大
	void  OnOriginal();		         //原始大小
	void  OnFullScreen();	         //全屏显示
	void  SetDrag(bool bDrag);       //是否可拖
	void  SetZoomFactor(int pos);    //设置放大倍数
	float GetZoomFactor();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
};
