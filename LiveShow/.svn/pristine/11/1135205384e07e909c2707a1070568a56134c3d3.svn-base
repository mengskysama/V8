#include "StdAfx.h"
#include "SkinMemDC.h"

CSkinMemDC::CSkinMemDC(HDC hDC, const RECT& rect, BOOL bCopy)
:
m_hDC(NULL),
m_hMemDC(NULL),
m_hOldBitmap(NULL),
m_hMemBitmap(NULL),
m_rcOrgRect(0, 0, 0, 0)
{
    if (hDC == NULL)
        return;

    m_hMemDC = ::CreateCompatibleDC(hDC);
    if (m_hMemDC == NULL)
        return;

    m_rcOrgRect = rect;
    m_hMemBitmap = ::CreateCompatibleBitmap(hDC, m_rcOrgRect.Width(), m_rcOrgRect.Height());
    ASSERT(m_hMemBitmap != NULL);

    m_hOldBitmap = (HBITMAP)::SelectObject(m_hMemDC, m_hMemBitmap);

    //Æ«ÒÆ×ø±ê
    CPoint oldOrg;
    ::OffsetViewportOrgEx(m_hMemDC, m_rcOrgRect.left, m_rcOrgRect.top, &oldOrg);

    if (bCopy) {
        ::BitBlt(m_hMemDC, m_rcOrgRect.left, m_rcOrgRect.top, m_rcOrgRect.Width(), m_rcOrgRect.Height(), 
        hDC, m_rcOrgRect.left, m_rcOrgRect.top, SRCCOPY);
    }

    m_hDC = hDC;
}

CSkinMemDC::~CSkinMemDC(void)
{
    if (m_hMemDC == NULL)
        return;

    ::BitBlt(m_hDC, m_rcOrgRect.left, m_rcOrgRect.top, m_rcOrgRect.Width(), m_rcOrgRect.Height(), 
        m_hMemDC, m_rcOrgRect.left, m_rcOrgRect.top, SRCCOPY);

    ::SelectObject(m_hMemDC, m_hOldBitmap);
    ::DeleteObject(m_hMemBitmap);
    ::DeleteDC(m_hMemDC);
}

CSkinMemDC::operator HDC()
{
    return m_hMemDC;
}
