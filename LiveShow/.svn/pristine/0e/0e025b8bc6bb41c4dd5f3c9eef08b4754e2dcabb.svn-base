#ifndef _MEMDC_H_
#define _MEMDC_H_

//////////////////////////////////////////////////
// CMemDC - memory DC
//
// Author: Keith Rule
// Email:  keithr@europa.com
// Copyright 1996-1997, Keith Rule
//
// You may freely use or modify this code provided this
// Copyright is included in all derived versions.
//
// This class implements a memory Device Context

class CMemDC : public CDC 
{
private:
	CBitmap*	m_bitmap;
	CBitmap*	m_oldBitmap;
	CDC*		m_pDC;
	CRect		m_rcBounds;
	DWORD		m_dwRop; 
public:
	CMemDC(CDC* pDC, const CRect& rcBounds, CBitmap * pBmp = NULL, DWORD dwRop = SRCCOPY) : CDC()
	{
		CreateCompatibleDC(pDC);
		if (pBmp == NULL)
		{
			m_bitmap = new CBitmap;
			m_bitmap->CreateCompatibleBitmap(pDC, rcBounds.Width(), rcBounds.Height());
			m_oldBitmap = SelectObject(m_bitmap);
		}
		else
		{
			m_bitmap = NULL;
			m_oldBitmap = SelectObject(pBmp);
		}
		m_pDC = pDC;
		m_rcBounds = rcBounds;
		m_dwRop = dwRop;
	}

	~CMemDC() 
	{
		m_pDC->BitBlt(m_rcBounds.left, m_rcBounds.top, m_rcBounds.Width(), m_rcBounds.Height(), 
					this, 0/*m_rcBounds.left*/, 0/*m_rcBounds.top*/, m_dwRop);
		SelectObject(m_oldBitmap);
		if (m_bitmap != NULL) 
		{
			m_bitmap->DeleteObject();
			delete m_bitmap;
			m_bitmap = NULL;
		}
			
	}
	CMemDC* operator->() 
	{
		return this;
	}
	// Allow usage as a pointer
	operator CMemDC*() {return this;}
};

#endif