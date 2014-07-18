#pragma once

class CSkinMemDC
{
public:
    CSkinMemDC(HDC hDC, const RECT& rect, BOOL bCopy = TRUE);
    ~CSkinMemDC(void);
    operator HDC();
private:
    HDC m_hDC;
    HDC m_hMemDC;
    HBITMAP m_hOldBitmap;
    HBITMAP m_hMemBitmap;
    CRect m_rcOrgRect;
};
