#include "StdAfx.h"
#include "ui\Splitter.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


CSplitter::CSplitter() : m_rectLast(0, 0, 0, 0), m_ptOld(0, 0)
{
	m_bHorizontal = TRUE;
	m_bDragging = FALSE;

	m_hHCursor = ::AfxGetApp()->LoadCursor(MAKEINTRESOURCE(AFX_IDC_HSPLITBAR));
	m_hVCursor = ::AfxGetApp()->LoadCursor(MAKEINTRESOURCE(AFX_IDC_VSPLITBAR));

	m_nMinTop = 0;
	m_nMaxBottom = 10000;	
}

CSplitter::~CSplitter()
{
	if (m_hHCursor != NULL)
	{
		::DestroyCursor(m_hHCursor);
		m_hHCursor = NULL;
	}

	if (m_hVCursor != NULL)
	{
		::DestroyCursor(m_hVCursor);
		m_hVCursor = NULL;
	}
}

BEGIN_MESSAGE_MAP(CSplitter, CWnd)
	//{{AFX_MSG_MAP(CSplitter)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_PAINT()
	ON_WM_MOUSEMOVE()
	ON_WM_SETCURSOR()
	ON_WM_ERASEBKGND()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSplitter message handlers

void CSplitter::SetBkgColor( COLORREF uBkgColor)
{
	m_nBkgColor = uBkgColor;
	
	Invalidate( TRUE);
}

void CSplitter::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default

	SetCapture();
	m_bDragging = TRUE;
	m_ptOld = point;
	DrawDraggingBar(point, TRUE);

}

void CSplitter::OnLButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default

	if (m_bDragging)
	{
		DrawDraggingBar(point, FALSE);

		if(point == m_ptOld)
		{
			ReleaseCapture();

			//test
			Invalidate();

			m_bDragging = FALSE;

			return;
		}

		CWnd* pParent = GetParent();
		ASSERT(pParent != NULL);

		CRect rect;	
		GetWindowRect(rect);

		int nHeight = rect.Height();
		int nWidth = rect.Width();
		if (m_bHorizontal)
		{
			int nTmpY = point.y - m_ptOld.y;

			rect.top += nTmpY;
			rect.bottom = rect.top + nHeight;

			pParent->ScreenToClient(rect);

			if (rect.top < m_nMinTop)
			{
				rect.top = m_nMinTop;
				rect.bottom = rect.top + nHeight;
			}

			if (rect.bottom > m_nMaxBottom)
			{
				rect.bottom = m_nMaxBottom;
				rect.top = rect.bottom - nHeight;
			}

			MoveWindow(rect);
		}
		else
		{
			int nTmpX = point.x - m_ptOld.x;

			rect.left += nTmpX;
			rect.right = rect.left + nWidth;

			pParent->ScreenToClient(rect);

			if (rect.left < m_nMinTop)
			{
				rect.left = m_nMinTop;
				rect.right = rect.left + nWidth;
			}

			if (rect.right > m_nMaxBottom)
			{
				rect.right = m_nMaxBottom;
				rect.left = rect.right - nWidth;
			}

			MoveWindow(rect);
		}

		ReleaseCapture();

		//test
		Invalidate();

		//pParent->Invalidate();//DragonZ:原版这里是没被注释掉的。但因为会导致父窗口的闪烁，刷新其实在js的OnSize里有作。所以注释掉。

		m_bDragging = FALSE;
        if(m_bHorizontal)
		  pParent->SendMessage(WM_USER_SPLITTER, rect.top, 0);
		else
		  pParent->SendMessage(WM_USER_SPLITTER, rect.left, 0);
	}
	else
	{
		CWnd::OnLButtonUp(nFlags, point);
	}	
}

void CSplitter::OnPaint() 
{

	CPaintDC paint_dc(this); // device context for painting

	CDC dc;
	dc.CreateCompatibleDC(&paint_dc);

	CRect rect;
	GetClientRect(rect);

	dc.FillSolidRect(rect, RGB(255, 255, 255));

}

BOOL CSplitter::Create(DWORD dwStyle, LPCTSTR lpszSkinPath, const CRect& rect, CWnd* pParentWnd, UINT uID, BOOL bHorizontal)
{
	m_bHorizontal = bHorizontal;

	return CWnd::Create(NULL, _T("Splitter"), dwStyle, rect, pParentWnd, uID);
}

void CSplitter::OnMouseMove(UINT nFlags, CPoint point) 
{

	if (((nFlags & MK_LBUTTON) != 0) && m_bDragging)
	{
		DrawDraggingBar(point, FALSE);
	}
	else
	{
		CWnd::OnMouseMove(nFlags, point);
	}
}

void CSplitter::DrawDraggingBar(CPoint point, BOOL bEnter)
{
	CWnd* pParent = GetParent();
	ASSERT(pParent != NULL);

	CWindowDC dc(pParent);
	CDC* pDC=&dc;

	if (pDC == NULL)
		return;

	CRect rect;
	GetWindowRect(rect);

	CSize size(0, 0);

	if (m_bHorizontal)
	{	
		int nTmpY = point.y - m_ptOld.y;

		int nHeight = rect.Height();

		rect.top += nTmpY;
		rect.bottom = rect.top + nHeight;

		pParent->ScreenToClient(rect);

		if (rect.top < m_nMinTop)
		{
			rect.top = m_nMinTop;
			rect.bottom = rect.top + nHeight;
		}

		if (rect.bottom > m_nMaxBottom)
		{
			rect.bottom = m_nMaxBottom;
			rect.top = rect.bottom - nHeight;
		}

		size.cx = rect.Width();
		size.cy = rect.Height();		
	}
	else
	{
		int nTmpX = point.x - m_ptOld.x;

		int nWidth = rect.Width();

		rect.left += nTmpX;
		rect.right = rect.left + nWidth;

		pParent->ScreenToClient(rect);

		if (rect.left < m_nMinTop)
		{
			rect.left = m_nMinTop;
			rect.right = rect.left + nWidth;
		}

		if (rect.right > m_nMaxBottom)
		{
			rect.right = m_nMaxBottom;
			rect.left = rect.right - nWidth;
		}

		size.cx = rect.Width();
		size.cy = rect.Height();		
	}

	if (bEnter)
	{
		pDC->DrawDragRect(rect, size, NULL, size);
	}
	else
	{
		pDC->DrawDragRect(rect, size, &m_rectLast, size);
	}

	pDC = NULL;
	m_rectLast = rect;
}

BOOL CSplitter::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{
	::SetCursor(m_bHorizontal ? m_hVCursor : m_hHCursor);

	return TRUE;
}

void CSplitter::SetLimit(int nMinTop, int nMaxBottom)
{
	m_nMinTop = nMinTop;
	m_nMaxBottom = nMaxBottom;
}

BOOL CSplitter::OnEraseBkgnd(CDC* pDC) 
{
	return TRUE;
}
