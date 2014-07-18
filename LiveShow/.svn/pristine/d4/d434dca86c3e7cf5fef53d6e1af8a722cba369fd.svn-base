#include "StdAfx.h"
#include "resource.h"
#include "assert.h"
#include "ximage.h"
#include "../../include/ui/ItemBar.h"
#include "../../include/utility/FileHelper.h"
#include "../../include/ui/UIHelper.h"

namespace common { namespace ui
{

#define WM_RIGHTARROW_DOWN	WM_USER + 1
#define WM_LEFTARROW_DOWN	WM_USER	+ 2
#define WM_DRAWBACKGROUND	WM_USER	+ 3

IMPLEMENT_DYNAMIC(CArrowButton, CWnd)

CArrowButton::CArrowButton()
{
	m_eType = eArrowType_Right;
	m_eState = eArrowButtonState_Normal;
	m_bTrackMouse	= false;
}

CArrowButton::~CArrowButton()
{
}

BEGIN_MESSAGE_MAP(CArrowButton, CWnd)
	ON_WM_PAINT()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_MESSAGE(WM_MOUSELEAVE, OnMouseLeave)
END_MESSAGE_MAP()

BOOL 
CArrowButton::Create(CWnd* pwndParent, CRect rc, UINT uId)
{
	return __super::Create(AfxRegisterWndClass(0), NULL, WS_CHILD, rc, pwndParent, uId);
}

void 
CArrowButton::OnPaint()
{
	CPaintDC dc(this); 

	int nModeOld = dc.SetBkMode(TRANSPARENT);

	CRect m_Rect;
	GetClientRect(&m_Rect);

	if ( m_eState == eArrowButtonState_Hover)
	{
		if (m_hIcon)
		{
			DrawIconEx(dc.GetSafeHdc(), m_Rect.left , m_Rect.top , m_hIcon, 
				m_Rect.Width(), m_Rect.Height(), 0, NULL, DI_NORMAL);
		}
		dc.Draw3dRect(m_Rect, RGB(255, 255, 255), RGB(0, 0, 0));
	}
	else if (m_eState == eArrowButtonState_Down)
	{
		m_Rect.OffsetRect(1,1);
		if (m_hIcon)
		{
			DrawIconEx(dc.GetSafeHdc(), m_Rect.left , m_Rect.top , m_hIcon, 
				m_Rect.Width(), m_Rect.Height(), 0, NULL, DI_NORMAL);
		}
		dc.Draw3dRect(m_Rect, RGB(0, 0, 0), RGB(255, 255, 255));
	}
	else if( m_eState == eArrowButtonState_Normal)
	{
		if (m_hIcon)
		{
			DrawIconEx(dc.GetSafeHdc(), m_Rect.left , m_Rect.top , m_hIcon, 
				m_Rect.Width(), m_Rect.Height(), 0, NULL, DI_NORMAL);
		}
	}

	dc.SetBkMode(nModeOld);
}

void 
CArrowButton::OnMouseMove(UINT nFlags, CPoint point)
{
	// 当鼠标移出该窗口的时候发送WM_MOUSELEAVE消息
	if (m_bTrackMouse == false)
	{
		TRACKMOUSEEVENT tme;
		tme.cbSize		= sizeof(tme);
		tme.hwndTrack	= GetSafeHwnd();
		tme.dwFlags		= TME_LEAVE;
		tme.dwHoverTime = 1;

		m_bTrackMouse = _TrackMouseEvent(&tme);	// post WM_MOUSELEAVE when mouse leave the wnd
	}

	m_eState = eArrowButtonState_Hover;

	CRect rect;
	GetClientRect(&rect);
	InvalidateRect(rect);
}

void 
CArrowButton::OnLButtonDown(UINT nFlags, CPoint point)
{
	m_eState = eArrowButtonState_Down;

	CRect rect;
	GetClientRect(&rect);
	InvalidateRect(rect);

	if( m_eType == eArrowType_Right)
		GetParent()->SendMessage( WM_RIGHTARROW_DOWN,0,0);
	else if( m_eType == eArrowType_Left)
		GetParent()->SendMessage( WM_LEFTARROW_DOWN, 0,0);
}

void 
CArrowButton::OnLButtonUp(UINT nFlags, CPoint point)
{
	m_eState = eArrowButtonState_Hover;

	CRect rect;
	GetClientRect(&rect);
	InvalidateRect(rect);
}

LRESULT 
CArrowButton::OnMouseLeave(WPARAM w, LPARAM l)
{
	m_bTrackMouse = false;

	m_eState = eArrowButtonState_Normal;

	CRect rect;
	GetClientRect(&rect);
	InvalidateRect(rect);

	return 0;
}


///////////////////////////////////////////////////////////
//			CToolItem
CToolItem::CToolItem(void)
{
	m_Rect = CRect(0,0,0,0);
	m_nItemIndex = 0;
	m_nWidth = 18;
	m_nHeight = 18;
	m_nLine	= 0;
	m_strTips	=	L"";
	m_bShow3State	=	FALSE;		//	默认是不显示三态效果
	m_eState = eState_Normal;

	m_ClickFunc	=	NULL;
	m_pParam	=	NULL;

	m_pParentWnd	=	NULL;
}

CToolItem::CToolItem( CToolItem& barItem)
{
	m_Rect = barItem.m_Rect;
	m_nWidth = barItem.m_nWidth;
	m_nHeight= barItem.m_nHeight;
	m_nLine	=	barItem.m_nLine;
	m_strTips	=	barItem.m_strTips;

	m_ClickFunc	=	barItem.m_ClickFunc;
	m_pParam	=	barItem.m_pParam;
	m_pParam	=	barItem.m_pParentWnd;
}

CToolItem::CToolItem( CToolItem* pbarItem)
{
	assert( pbarItem != NULL);
	m_Rect = pbarItem->m_Rect;
	m_nItemIndex = pbarItem->m_nItemIndex;

	m_nWidth	=	pbarItem->m_nWidth;
	m_nHeight	=	pbarItem->m_nHeight;
	m_nLine		=	pbarItem->m_nLine;
	m_strTips	=	pbarItem->m_strTips;

	m_ClickFunc	=	pbarItem->m_ClickFunc;
	m_pParam	=	pbarItem->m_pParam;
	m_pParentWnd	=	pbarItem->m_pParentWnd;
}

CToolItem::~CToolItem(void)
{
}

void CToolItem::SetTips(std::wstring	strTips)
{
	m_strTips	=	strTips;
}

void CToolItem::Draw(CDC* pDC)
{
	CRect rcIcon( m_Rect);
	std::wstring strPath;
	CxImage image;

	// ToolItem的Draw函数只实现最基本的三态函数，不会对皮肤的颜色进行变色,
	CRect rcBox( m_Rect);
	if ( m_eState == eState_Hover)
	{
		pDC->Draw3dRect(rcBox, RGB(245,245,245), RGB(64,64,64));
	}
	else if (m_eState == eState_Down)
	{
		pDC->Draw3dRect(rcBox, RGB(64,64,64), RGB(245,245,245));
	}
	else if( m_eState == eState_Normal)
	{
		CBrush whiteBrush( RGB(255,255,255));
		pDC->FrameRect(&rcBox, &whiteBrush);
		pDC->Draw3dRect(rcBox, RGB(255, 255, 255), RGB(255, 255, 255));
	}
}

void	
CToolItem::OnMouseMove(CDC* pDC, UINT nFlags, CPoint point)
{
	if( m_bShow3State)
		SetState( eState_Hover);

	Draw(pDC);
}

void	
CToolItem::OnLButtonDown(CDC* pDC, UINT nFlags, CPoint point)
{
	if( m_bShow3State)
		SetState( eState_Down);

	Draw(pDC);
}

void	
CToolItem::OnLButtonUp(CDC* pDC, UINT nFlags, CPoint point)
{
	if( m_bShow3State)
		SetState( eState_Normal);

	Draw(pDC);
}

LRESULT	
CToolItem::OnMouseLeave(CDC* pDC, WPARAM w, LPARAM l)
{
	if( m_bShow3State)
		SetState( eState_Normal);

	Draw(pDC);

	if( m_pParentWnd)
		m_pParentWnd->SendMessage( WM_MOUSEMOVE,0,0);

	return 0;
}

BOOL
CToolItem::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
	return TRUE;
}



///////////////////////////////////////////////////////
//			CToolItemBar


IMPLEMENT_DYNAMIC(CToolItemBar, CWnd)

CToolItemBar::CToolItemBar(void)
{
	m_bWrapper		=	false;		//	正常情况下不需要折叠
	m_nCurrentPage	=	0;
	m_nLine			=	0;
	m_nLineHeight	=	25;
	m_bTrackMouse	=	FALSE;
}

CToolItemBar::~CToolItemBar(void)
{
	DeleteItems();
}


BEGIN_MESSAGE_MAP(CToolItemBar, CWnd)
	ON_WM_PAINT()
	ON_MESSAGE(WM_RIGHTARROW_DOWN, OnRightArrowDown)
	ON_MESSAGE(WM_LEFTARROW_DOWN, OnLeftArrowDown)
	ON_WM_CREATE()
	ON_WM_ERASEBKGND()
	ON_WM_SIZE()

	ON_MESSAGE(WM_MOUSELEAVE, OnMouseLeave)
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_SETCURSOR()

END_MESSAGE_MAP()

BOOL 
CToolItemBar::Create(CWnd* pwndParent, CRect rc, UINT uId)
{
	m_nBkgColor = RGB(255,255,255);
	__super::Create(AfxRegisterWndClass(0), NULL, WS_CHILD, rc, pwndParent, uId);

	// 不折叠的情况下才会创建附加的调整按钮
	if( !m_bWrapper)
	{
		CRect rRectIconBar;
		GetClientRect(&rRectIconBar);

		m_rRightArrowButtonRect.left = rRectIconBar.right - 10;
		m_rRightArrowButtonRect.right = rRectIconBar.right - 2;
		m_rRightArrowButtonRect.top = rRectIconBar.top + 3;
		m_rRightArrowButtonRect.bottom = rRectIconBar.bottom - 1;
		m_rightArrowButton.Create( this, m_rRightArrowButtonRect, 0);
		m_rightArrowButton.m_hIcon = LoadIcon(AfxGetResourceHandle(), MAKEINTRESOURCE(IDI_ARROWRIGHT));
		m_rightArrowButton.ShowWindow( SW_HIDE);
		m_rightArrowButton.m_eType = eArrowType_Right;

		m_rLeftArrowButtonRect.left = m_rRightArrowButtonRect.left - 8;
		m_rLeftArrowButtonRect.right = m_rRightArrowButtonRect.left ;
		m_rLeftArrowButtonRect.top = m_rRightArrowButtonRect.top;
		m_rLeftArrowButtonRect.bottom = m_rRightArrowButtonRect.bottom;
		m_leftArrowButton.Create( this, m_rLeftArrowButtonRect, 1);
		m_leftArrowButton.m_hIcon = LoadIcon(AfxGetResourceHandle(), MAKEINTRESOURCE(IDI_ARROWLEFT));
		m_leftArrowButton.ShowWindow( SW_HIDE);
		m_leftArrowButton.m_eType = eArrowType_Left;
	}

	return TRUE;
}

void 
CToolItemBar::SetBkgColor( COLORREF uBkgColor)
{
	m_nBkgColor = uBkgColor;

	RECT rBarRect;
	GetClientRect(&rBarRect);
	InvalidateRect(&rBarRect, TRUE);
}

void	
CToolItemBar::SetWrapper(BOOL bWrapper)
{
	m_bWrapper = bWrapper;
}

CRect	
CToolItemBar::GetLastIconRect()
{
	if( m_vItems.size() > 0)
		return m_vItems[m_vItems.size() - 1]->m_Rect;
	else
		return CRect(0,0,0,0);
}

INT			
CToolItemBar::GetFirstButtonAtLine( UINT nLine)
{
	if( nLine > m_nLine)
		return -1;

	for( size_t i = 0; i < m_vItems.size(); i++)
	{
		CToolItem* pIcon = m_vItems[i];
		if( pIcon == NULL)
			continue;

		if( pIcon->m_nLine == nLine)
			return (INT)i;
	}

	return -1;
}

UINT
CToolItemBar::GetIconBarRight()
{
	RECT rRect;
	GetClientRect(&rRect);

	if( !m_bWrapper)
	{
		RECT rRightArrowButtonRect;
		m_rightArrowButton.GetClientRect(&rRightArrowButtonRect);

		RECT rLeftArrowButtonRect;
		m_leftArrowButton.GetClientRect(&rLeftArrowButtonRect);

		return rRect.right - ( rRightArrowButtonRect.right - rRightArrowButtonRect.left + H_ICON_SPACE)*2;
	}
	else
	{
		return rRect.right - H_ICON_SPACE;
	}
}

CRect	
CToolItemBar::GetNewIconRect( UINT nWidth , UINT nHeight)
{
	RECT rRect;
	GetClientRect(&rRect);

	// 不需要折叠
	RECT rIconRect = {0,0,0,0};

	if( !m_bWrapper)
	{
		UINT uRight = GetIconBarRight();

		if( m_vItems.size() > 0)
		{
			CToolItem* pItem = m_vItems[m_vItems.size() - 1];
			if( pItem )
			{
				CRect pItemRect = pItem->m_Rect;

				// 不允许Item的高度超过行高
				if( pItemRect.Height() > m_nLineHeight)
					pItemRect.bottom = pItemRect.top + m_nLineHeight;

				// 如果增加了新的Icon，它的长度超出了给定范围，那么该icon必须切换到下一页面
				if( pItemRect.right + H_ICON_SPACE + nWidth  > uRight)
				{
					m_nLine ++;

					rIconRect.left = rRect.left + LEFT_SPACE ;
					rIconRect.right = rIconRect.left + nWidth;
					rIconRect.top = rRect.top + V_ICON_SPACE;

					if( nHeight > m_nLineHeight)
						nHeight = m_nLineHeight;

					rIconRect.bottom = rIconRect.top + nHeight + 2;
				}
				// 不需要切换到下一页
				else
				{
					rIconRect.left = pItemRect.right + H_ICON_SPACE;
					rIconRect.right = pItemRect.right + H_ICON_SPACE + nWidth;
					rIconRect.top = rRect.top + V_ICON_SPACE;
	
					if( nHeight > m_nLineHeight)
						nHeight = m_nLineHeight;

					rIconRect.bottom = rIconRect.top + nHeight + 2;
				}
			}
			else
			{
				ASSERT(0);
			}
		}
		else
		{
			rIconRect.left = rRect.left + LEFT_SPACE ;
			rIconRect.right = rIconRect.left + nWidth;
			rIconRect.top = rRect.top + V_ICON_SPACE;

			if( nHeight > m_nLineHeight)
				nHeight = m_nLineHeight;
			rIconRect.bottom = rIconRect.top + nHeight + 2;
		}
	}
	else
	{
		if( m_vItems.size() > 0)
		{
			CToolItem* pItem = m_vItems[m_vItems.size() - 1];
			if( pItem )
			{
				CRect pItemRect = pItem->m_Rect;

				// 不允许Item的高度超过行高
				if( pItemRect.Height() > m_nLineHeight)
					pItemRect.bottom = pItemRect.top + m_nLineHeight;

				// 如果增加了新的Icon，它的长度超出了给定范围，那么该icon必须切换到下一行
				if( pItemRect.right + H_ICON_SPACE + nWidth  > GetIconBarRight())
				{
					m_nLine ++;

					rIconRect.left = rRect.left + LEFT_SPACE ;
					rIconRect.right = rIconRect.left + nWidth;
					rIconRect.top = rRect.top + V_ICON_SPACE + ( m_nLineHeight + V_ICON_SPACE)*( m_nLine - 1);
	
					if( nHeight > m_nLineHeight)
						nHeight = m_nLineHeight;

					rIconRect.bottom = rIconRect.top + nHeight;
				}
				// 不需要切换到下一页
				else
				{
					rIconRect.left = pItemRect.right + H_ICON_SPACE;
					rIconRect.right = pItemRect.right + H_ICON_SPACE + nWidth;
					rIconRect.top = pItemRect.top;

					if( nHeight > m_nLineHeight)
						nHeight = m_nLineHeight;

					rIconRect.bottom = pItemRect.top + nHeight;
				}
			}
			else
			{
				ASSERT(0);
			}
		}
		else
		{
			rIconRect.left = rRect.left + LEFT_SPACE ;
			rIconRect.right = rIconRect.left + nWidth;
			rIconRect.top = rRect.top + V_ICON_SPACE;

			if( nHeight > m_nLineHeight)
				nHeight = m_nLineHeight;

			rIconRect.bottom = rIconRect.top + nHeight;
		}
	}

	//CenterRect(&rIconRect, nHeight, nWidth, &rIconRect);

	return rIconRect;
}

CToolItem*	
CToolItemBar::GetItem( CPoint pt, UINT* pnIdx/* = NULL*/)
{
	// 从第二页开始计算
	UINT nBegin = 0;

	if( !m_bWrapper)
	{
// 		UINT nButtonNum = GetButtonsNumPerPage();
// 		nBegin = m_nCurrentPage*nButtonNum;
	}

	for (size_t i = nBegin; i < m_vItems.size(); i++)
	{
		CToolItem* prefer = m_vItems[i];

		if (prefer->m_Rect.PtInRect(pt))
		{
			if (pnIdx)
			{
				*pnIdx = (UINT)i;
			}

			return prefer;
		}
	}

	return NULL;
}


void	
CToolItemBar::AddItem( CToolItem* pIconButton)
{
	if( pIconButton == NULL)
	{
		assert(0);
		return;
	}

	pIconButton->SetParent(this);

	// 得到该Item在Bar上的位置
	CRect rButton = GetNewIconRect( pIconButton->GetWidth(), pIconButton->GetHeight());
	
	pIconButton->m_Rect = rButton;
	pIconButton->m_nLine = m_nLine;

	m_vItems.push_back( pIconButton);

	// 整个宽度 - 左边的间距 - 右边的间距 - 最右边的两个微调按钮的宽度
	if( !m_bWrapper)
	{
		if( m_nLine >= 1)
		{
			m_rightArrowButton.ShowWindow(SW_SHOW);
		}
	}

	Invalidate(TRUE);
}


void	
CToolItemBar::DeleteItems( )
{	
	for( size_t i=0;i < m_vItems.size(); i++)
	{
		CToolItem* pItem = m_vItems[i];
		if( pItem)
		{
			delete pItem;
			pItem = NULL;
		}
	}

	m_vItems.clear();

	m_nLine = 0;
}

void
CToolItemBar::OnSize(UINT nType, int x, int y)
{
	CWnd::OnSize(nType, x,y );

	// 不折叠的情况下才会创建附加的调整按钮
	if( !m_bWrapper)
	{
		CRect rRectIconBar;
		GetClientRect(&rRectIconBar);

		m_rRightArrowButtonRect.left = rRectIconBar.right - 10;
		m_rRightArrowButtonRect.right = rRectIconBar.right - 2;
		m_rRightArrowButtonRect.top = rRectIconBar.top + 3;
		m_rRightArrowButtonRect.bottom = rRectIconBar.bottom - 1;

		if( m_rightArrowButton.GetSafeHwnd() != NULL)
			m_rightArrowButton.MoveWindow(&m_rRightArrowButtonRect);

		m_rLeftArrowButtonRect.left = m_rRightArrowButtonRect.left - 8;
		m_rLeftArrowButtonRect.right = m_rRightArrowButtonRect.left ;
		m_rLeftArrowButtonRect.top = m_rRightArrowButtonRect.top;
		m_rLeftArrowButtonRect.bottom = m_rRightArrowButtonRect.bottom;

		if( m_leftArrowButton.GetSafeHwnd() != NULL)
			m_leftArrowButton.MoveWindow(&m_rLeftArrowButtonRect);
	}
}

void 
CToolItemBar::OnPaint()
{
	CPaintDC dc(this); 

	int nModeOld = dc.SetBkMode(TRANSPARENT);

	CRect rc;
	GetClientRect(rc);
	dc.FillSolidRect(rc, m_nBkgColor);

	// 滚动
	if( !m_bWrapper)
	{
		INT nBeginIndex  = GetFirstButtonAtLine( m_nCurrentPage);
		if( nBeginIndex != -1)
		{
			for (size_t i = nBeginIndex; i < m_vItems.size(); i++)
			{
				CToolItem* pIcon = m_vItems[i];
				if( pIcon && pIcon->m_nLine != m_nCurrentPage)
					break;

				pIcon->Draw(&dc);
			}
		}
	}
	else
	{
		for (size_t i = 0; i < m_vItems.size(); i++)
		{
			((CToolItem*)m_vItems[i])->Draw(&dc);
		}
	}

	dc.SetBkMode(nModeOld);
}

LRESULT	
CToolItemBar::OnRightArrowDown( WPARAM w, LPARAM l)
{
	if( m_nCurrentPage < m_nLine)
		m_nCurrentPage++;

	m_leftArrowButton.ShowWindow( SW_SHOW);

	if( m_nCurrentPage == m_nLine)
	{
		m_rightArrowButton.ShowWindow( SW_HIDE);

		CRect rRightArrowButtonRect;
		m_rightArrowButton.GetWindowRect(&rRightArrowButtonRect);
		ScreenToClient(&rRightArrowButtonRect);

		// 到达最后一页
		m_leftArrowButton.MoveWindow(&m_rRightArrowButtonRect, TRUE);
	}

	Invalidate(TRUE);

	return 0;
}

LRESULT 
CToolItemBar::OnLeftArrowDown( WPARAM w, LPARAM l)
{
	if( m_nCurrentPage > 0)
		m_nCurrentPage--;

	// 如果位于最后一页上，此时只有左箭头，没有右侧箭头
	if( m_nCurrentPage == 0)
	{
		// 将左侧箭头移动到它自己的位置
		m_leftArrowButton.ShowWindow( SW_HIDE);
	}
	else
	{
		m_leftArrowButton.MoveWindow(&m_rLeftArrowButtonRect, TRUE);
		m_leftArrowButton.ShowWindow( SW_SHOW);
	}

	m_rightArrowButton.ShowWindow( SW_SHOW);
	m_rightArrowButton.MoveWindow(&m_rRightArrowButtonRect, TRUE);

	Invalidate(TRUE);

	return 0;
}

BOOL 
CToolItemBar::OnEraseBkgnd(CDC* pDC)
{
	CWnd* pParent = GetParent();
	if( pParent )
	{
		CPoint pt( 0, 0 );
		MapWindowPoints( pParent, &pt, 1 );
		pt = pDC->OffsetWindowOrg( pt.x, pt.y );
		pParent->SendMessage( WM_DRAWBACKGROUND, ( WPARAM )pDC->m_hDC, 0L );
		pDC->SetWindowOrg( pt.x, pt.y );
	}

	return true;
}


void	
CToolItemBar::OnMouseMove(UINT nFlags, CPoint point)
{
	// 当鼠标移出该窗口的时候发送WM_MOUSELEAVE消息
	if (!m_bTrackMouse)
	{
		TRACKMOUSEEVENT tme;
		tme.cbSize		= sizeof(tme);
		tme.hwndTrack	= GetSafeHwnd();
		tme.dwFlags		= TME_LEAVE;
		tme.dwHoverTime = 1;

		m_bTrackMouse = _TrackMouseEvent(&tme);	// post WM_MOUSELEAVE when mouse leave the wnd

		if( m_strTips != L"")
		{
			ClientToScreen(&point);
			point.x += 10;
			point.y += 10;
			m_ctrlBarTip.SetHideDelay(-1);
			m_ctrlBarTip.Show(m_strTips.c_str(), NULL, &point);
		}
	}

	CToolItem* pHover = (CToolItem*)GetItem(point);
	if (pHover)
	{
		if ( GetHoverItem() != pHover && GetSelectedItem() != pHover)
		{
			ChangeItemState(pHover, eState_Hover);

			if( pHover->GetTips() != L"")
			{
				CPoint pt;
				CRect rectWindow;
				GetWindowRect(&rectWindow);

				pt.x = rectWindow.left + pHover->m_Rect.left + pHover->m_Rect.Width() + 3;
				pt.y = rectWindow.top + pHover->m_Rect.top + pHover->m_Rect.Height() ;

				m_ctrlButtonTip.SetHideDelay(-1);
				m_ctrlButtonTip.Show(pHover->GetTips().c_str(), NULL, &pt);
			}
		}
	}
	else
	{
		CToolItem* pHoverOld = (CToolItem*)GetHoverItem();
		if (pHoverOld)
		{
			ChangeItemState(pHoverOld, eState_Normal);

// 			if( GetParent())
// 				GetParent()->SendMessage( WM_PAINT);
			Invalidate(TRUE);
		}
		m_ctrlButtonTip.Hide();
	}

	CWnd::OnMouseMove(nFlags, point);
}


void	
CToolItemBar::OnLButtonDown(UINT nFlags, CPoint point)
{
	CToolItem* pHover = (CToolItem*)GetItem(point);
	if (pHover)
	{
		ChangeItemState(pHover, eState_Down);
		Invalidate(TRUE);

		if( pHover->m_ClickFunc != NULL)
		{
			if( pHover->m_pParam)
			{
				pHover->m_ClickFunc(pHover->m_pParam);
				if( m_ctrlButtonTip.BeHide() == FALSE)
					m_ctrlButtonTip.Hide();
			}
		}
	}

	CWnd::OnLButtonDown(nFlags, point);
}


void	
CToolItemBar::OnLButtonUp(UINT nFlags, CPoint point)
{
	CToolItem* pHover = (CToolItem*)GetItem(point);
	CToolItem* pSel = GetSelectedItem();

	if (pSel && pHover && pSel == pHover)
	{
		ChangeItemState(pSel, eState_Hover);

		// notify parent
		/*
		SRBIconBarNM* pnm = new SRBIconBarNM;
		pnm->idFrom = GetDlgCtrlID();
		pnm->hwndFrom = GetSafeHwnd();
		pnm->code = NM_CLICK;
		pnm->pButtonInfo = pSel;

		CWnd* pwndParent = GetParent();
		pwndParent->PostMessage(WM_NOTIFY, GetDlgCtrlID(), (LPARAM)pnm);
		*/
	}

	CWnd::OnLButtonUp(nFlags, point);
}

LRESULT			
CToolItemBar::OnMouseLeave(WPARAM w, LPARAM l)
{
	// 当鼠标移出该窗口的时候发送WM_MOUSELEAVE消
	CToolItem* pHover = GetHoverItem();
	if (pHover)
	{
		ChangeItemState(pHover, eState_Normal);
	}
	CToolItem* pSel = GetSelectedItem();
	if (pSel)
	{
		ChangeItemState(pSel, eState_Normal);
	}

	if( GetParent())
	{
		GetParent()->SendMessage( WM_PAINT, 0, 0);
		GetParent()->SendMessage( WM_MOUSEMOVE, 0, 0);
	}

	m_bTrackMouse = FALSE;
	m_ctrlBarTip.Hide();

	return 0;
}

BOOL
CToolItemBar::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
	POINT cursorPoint;
	GetCursorPos(&cursorPoint);
	ScreenToClient(&cursorPoint);

	CToolItem* pHover = (CToolItem*)GetItem(cursorPoint);
	if (pHover)
	{
		SetCursor(LoadCursor(NULL, IDC_HAND));
	}
	else
	{
		SetCursor(LoadCursor(NULL, IDC_ARROW));
	}

	return __super::OnSetCursor(pWnd, nHitTest, message);
}


CToolItem*	
CToolItemBar::GetSelectedItem( UINT* pnIdx)
{
	for (size_t i = 0; i < m_vItems.size(); i++)
	{
		CToolItem* prefer = (CToolItem*)m_vItems[i];
		if ( prefer && prefer->GetState() == eState_Down)
		{
			if (pnIdx)
			{
				*pnIdx = (UINT)i;
			}

			return prefer;
		}
	}

	return NULL;
}

CToolItem*	
CToolItemBar::GetHoverItem( UINT* pnIdx)
{
	for ( size_t i = 0; i < m_vItems.size(); i++)
	{
		CToolItem* prefer = (CToolItem*)m_vItems[i];

		if ( prefer && prefer->GetState() == eState_Hover )
		{
			if (pnIdx)
			{
				*pnIdx = (UINT)i;
			}

			return prefer;
		}
	}

	return NULL;
}

void		
CToolItemBar::ChangeItemState( CToolItem* pItem, EIconButtonState eNewState)
{
	if( !pItem)
		return;

	if( pItem->IsShow3State() == FALSE)
		return;

	if( pItem->GetState() == eNewState)
		return;

	CDC* pDC = GetDC();

	if ( eNewState == eState_Hover)
	{
		CToolItem* pHover = GetHoverItem();
		if (pHover)
		{
			pHover->SetState(eState_Normal);
			pHover->OnMouseLeave(pDC,0,0);
		}

		CToolItem* pSel = GetSelectedItem();
		if (pSel)
		{
			pSel->SetState(eState_Hover);
			pSel->OnMouseMove(pDC,0,CPoint(0,0));
		}

		pItem->SetState(eState_Hover);
		pItem->OnMouseMove(pDC,0,CPoint(0,0));
	}
	else if (eNewState == eState_Down)
	{
		CToolItem* pSel = GetSelectedItem();
		if (pSel)
		{
			pSel->SetState(eState_Normal);
			pSel->OnMouseLeave(pDC,0,0);
		}

		pItem->SetState(eState_Down);
		pItem->OnLButtonDown(pDC,0,CPoint(0,0));
	}
	else if (eNewState == eState_Normal)
	{
		pItem->SetState(eState_Normal);
		pItem->OnMouseLeave(pDC,0,0);
	}

	ReleaseDC(pDC);
}

}}