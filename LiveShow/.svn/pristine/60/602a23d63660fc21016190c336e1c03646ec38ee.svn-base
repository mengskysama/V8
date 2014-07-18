#include "stdafx.h"

#ifdef USE_CDIB

#include "./dib.h"
#include <windowsx.h>

CDib::CDib()
{
	m_pBMI = NULL;
	m_pBits = NULL;
	m_pMaskBits = NULL;
	m_hPalette = NULL;
	m_nBitCount = 0;    
}

CDib::~CDib()
{
	Free();    
}

void CDib::Free()
{
    if (m_pBits)
	{
		delete [] m_pBits;
		m_pBits = NULL;
	}

	if (m_pMaskBits)
	{
		delete [] m_pMaskBits;
		m_pMaskBits = NULL;
	}

	if (m_pBMI)
	{
		delete [] m_pBMI;
		m_pBMI = NULL;
	}
	if (m_hPalette)
	{
		DeleteObject(m_hPalette);
		m_hPalette = NULL;
	}   
}

BOOL CDib::Paint(HDC hDC, LPRECT lpDCRect, LPRECT lpDIBRect)
{
    if (!m_pBMI)
		return (FALSE); // Modified by AddLog 2002.11.25 18:59

	HPALETTE hPal = NULL;
	HPALETTE hOldPal = NULL;

	if (m_hPalette != NULL)
	{
		// Select as background since we have
		// already realized in forground if needed
		hOldPal = ::SelectPalette(hDC, m_hPalette, TRUE);
	}

	/* Make sure to use the stretching mode best for color pictures */
	::SetStretchBltMode(hDC, COLORONCOLOR);

	//根据biHeight对dibrect进行转换
	CRect rc;
	::CopyRect(&rc,lpDIBRect);

	if(m_pBMI->bmiHeader.biHeight > 0)
	{
		int height = Height();		
		rc.top = height - rc.top;
		rc.bottom = height - rc.bottom;
		height = rc.top;
		rc.top = rc.bottom;
		rc.bottom = height;
	}

	/* Determine whether to call StretchDIBits() or SetDIBitsToDevice() */
	BOOL bSuccess;
	/*
	if ((RECTWIDTH(lpDCRect)  == RECTWIDTH(lpDIBRect)) &&
	   (RECTHEIGHT(lpDCRect) == RECTHEIGHT(lpDIBRect)))
		bSuccess = ::SetDIBitsToDevice(hDC,        // hDC
								   lpDCRect->left,             // DestX
								   lpDCRect->top,              // DestY
								   RECTWIDTH(lpDCRect),        // nDestWidth
								   RECTHEIGHT(lpDCRect),       // nDestHeight
								   lpDIBRect->left,            // SrcX
								   (int)Height() -
									  lpDIBRect->top -
									  RECTHEIGHT(lpDIBRect),     // SrcY
								   0,                          // nStartScan
								   (WORD)Height(),             // nNumScans
								   m_pBits,                    // lpBits
								   m_pBMI,                     // lpBitsInfo
								   DIB_RGB_COLORS);            // wUsage
   else*/
	if(m_pMaskBits==NULL)
	{
	  bSuccess = ::StretchDIBits(hDC,            // hDC
							   lpDCRect->left,               // DestX
							   lpDCRect->top,                // DestY
							   RECTWIDTH(lpDCRect),          // nDestWidth
							   RECTHEIGHT(lpDCRect),         // nDestHeight
							   rc.left,              // SrcX
							   rc.top,               // SrcY
							   RECTWIDTH(&rc),         // wSrcWidth
							   RECTHEIGHT(&rc),        // wSrcHeight
							   m_pBits,                      // lpBits
							   m_pBMI,                       // lpBitsInfo
							   DIB_RGB_COLORS,               // wUsage
							   SRCCOPY);                     // dwROP
	}
	else
	{

	  bSuccess = ::StretchDIBits(hDC,            // hDC
							   lpDCRect->left,               // DestX
							   lpDCRect->top,                // DestY
							   RECTWIDTH(lpDCRect),          // nDestWidth
							   RECTHEIGHT(lpDCRect),         // nDestHeight
							   rc.left,              // SrcX
							   rc.top,               // SrcY
							   RECTWIDTH(&rc),         // wSrcWidth
							   RECTHEIGHT(&rc),        // wSrcHeight
							   m_pMaskBits,                      // lpBits
							   m_pBMI,                       // lpBitsInfo
							   DIB_RGB_COLORS,               // wUsage
							   SRCAND);                     // dwROP

	  bSuccess = ::StretchDIBits(hDC,            // hDC
							   lpDCRect->left,               // DestX
							   lpDCRect->top,                // DestY
							   RECTWIDTH(lpDCRect),          // nDestWidth
							   RECTHEIGHT(lpDCRect),         // nDestHeight
							   rc.left,              // SrcX
							   rc.top,               // SrcY
							   RECTWIDTH(&rc),         // wSrcWidth
							   RECTHEIGHT(&rc),        // wSrcHeight
							   m_pBits,                      // lpBits
							   m_pBMI,                       // lpBitsInfo
							   DIB_RGB_COLORS,               // wUsage
							   SRCPAINT);                     // dwROP

	}

	/* Reselect old palette */
	if (hOldPal != NULL)
	{
		::SelectPalette(hDC, hOldPal, TRUE);
	}

   return (bSuccess); // Modified by AddLog 2002.11.25 18:59
}

BOOL  CDib::Show(HDC hDC,int x,int y)
{
    CRect rcDC;
	::SetRect(&rcDC, x, y, x+Width(), y+Height());

	CRect rcDib;
	SetRect(&rcDib,0,0,Width(),Height());

	return (Paint(hDC,&rcDC,&rcDib)); // Modified by AddLog 2002.11.25 18:59
}

BOOL CDib::CreatePalette()
{
    if (!m_pBMI)
		return (FALSE); // Modified by AddLog 2002.11.25 18:59

   //get the number of colors in the DIB
   WORD wNumColors = GetColorNum();

   if (wNumColors != 0)
   {
		// allocate memory block for logical palette
		HANDLE hLogPal = ::GlobalAlloc(GHND, sizeof(LOGPALETTE) + sizeof(PALETTEENTRY)*(wNumColors));

		// if not enough memory, clean up and return NULL
		if (hLogPal == 0)
		{
			return (FALSE);
		}

		LPLOGPALETTE lpPal = (LPLOGPALETTE)::GlobalLock((HGLOBAL)hLogPal);

		// set version and number of palette entries
		lpPal->palVersion = PALVERSION;
		lpPal->palNumEntries = (WORD)(wNumColors);

		for (int i = 0; i < (int)wNumColors; i++)
		{
			lpPal->palPalEntry[i].peRed = m_pBMI->bmiColors[i].rgbRed;
			lpPal->palPalEntry[i].peGreen = m_pBMI->bmiColors[i].rgbGreen;
			lpPal->palPalEntry[i].peBlue = m_pBMI->bmiColors[i].rgbBlue;
			lpPal->palPalEntry[i].peFlags = 0;
		}
/*
		lpPal->palPalEntry[wNumColors-1].peRed = 0;
		lpPal->palPalEntry[wNumColors-1].peGreen = 0;
		lpPal->palPalEntry[wNumColors-1].peBlue = 0;
		lpPal->palPalEntry[wNumColors-1].peFlags = 0;

		lpPal->palPalEntry[wNumColors-2].peRed = 255;
		lpPal->palPalEntry[wNumColors-2].peGreen = 255;
		lpPal->palPalEntry[wNumColors-2].peBlue = 255;
		lpPal->palPalEntry[wNumColors-2].peFlags = 0;
*/
		/* create the palette and get handle to it */
		if (m_hPalette)
		{
			::DeleteObject(m_hPalette);
		}
		m_hPalette = ::CreatePalette(lpPal);
		::GlobalUnlock((HGLOBAL) hLogPal);
		::GlobalFree((HGLOBAL) hLogPal);
		return (TRUE); // Modified by AddLog 2002.11.25 18:59
	}

	return (TRUE); // Modified by AddLog 2002.11.25 18:59
}

WORD CDib::PaletteSize() const
{
    if (!m_pBMI)
		return (0); // Modified by AddLog 2002.11.25 18:59

	return (GetColorNum() * sizeof(RGBQUAD)); // Modified by AddLog 2002.11.25 18:59
}

WORD CDib::GetColorNum() const
{
    if (!m_pBMI)
		return (0); // Modified by AddLog 2002.11.25 18:59

	WORD wBitCount;  // DIB bit count

	/*  The number of colors in the color table can be less than
	 *  the number of bits per pixel allows for (i.e. lpbi->biClrUsed
	 *  can be set to some value).
	 *  If this is the case, return the appropriate value.
	 */

	DWORD dwClrUsed;

	dwClrUsed = m_pBMI->bmiHeader.biClrUsed;
	if (dwClrUsed != 0)
		return ((WORD)dwClrUsed); // Modified by AddLog 2002.11.25 18:59

	/*  Calculate the number of colors in the color table based on
	 *  the number of bits per pixel for the DIB.
	 */
	wBitCount = m_pBMI->bmiHeader.biBitCount;

	/* return number of colors based on bits per pixel */
	switch (wBitCount)
	{
		case 1:
			return (2); 

		case 4:
			return (16);

		case 8:
			return (256); 

		default:
			return (0); 
	}
}

BOOL CDib::Read(HANDLE hFile)
{
    Free();

	BITMAPFILEHEADER bmfHeader;

	DWORD dwRead;
	if(!ReadFile(hFile,(LPSTR)&bmfHeader, sizeof(bmfHeader),&dwRead,NULL) ||
		dwRead!=sizeof(bmfHeader) ||
		bmfHeader.bfType!=DIB_HEADER_MARKER )
		return (0);

	DWORD dwReadBytes = sizeof(bmfHeader);

	DWORD nSize = bmfHeader.bfOffBits-sizeof(BITMAPFILEHEADER);
	m_pBMI = (LPBITMAPINFO)new BYTE[nSize + 256*sizeof(RGBQUAD)];
	if( !m_pBMI )
		return (0);

	if(!ReadFile(hFile,m_pBMI, nSize,&dwRead,NULL) ||
		dwRead!=nSize )
	{
		Free();
		return (0); 
	}

	m_nBitCount = GetFileSize(hFile,NULL) - bmfHeader.bfOffBits;

	m_pBits = new BYTE[m_nBitCount];

	if( !m_pBits )
	{
		Free();
		return (0); 
	}

	if(! ReadFile(hFile,m_pBits, m_nBitCount,&dwRead,NULL) ||
		dwRead!=m_nBitCount )
	{
		Free();
		return (0); 
	}

	m_nLineWidth = ( Width()*3 + 3 )/4 * 4;

	CreatePalette();

	return (TRUE); 
}

BOOL CDib::ReadInFile(LPCTSTR szFileName)
{
    BOOL b = FALSE;
	HANDLE hFile = CreateFile(szFileName,GENERIC_READ,FILE_SHARE_READ,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);
	if(hFile)
	{
		b = Read(hFile);
		CloseHandle(hFile);
	}
	else
	{
		ASSERT(FALSE);
	}

	return(b); 
}

BOOL CDib::StretchShow(HDC hDC,int x,int y, int cx,int cy)
{
    CRect rcDC,rcDib;

	SetRect(&rcDC, x, y, x+cx, y+cy);

	SetRect(&rcDib,0,0,Width(),Height());

	return (Paint(hDC,&rcDC,&rcDib));
}


RGBQUAD RGB2RGBQUAD(COLORREF cr)
{
    RGBQUAD c;
	c.rgbRed = GetRValue(cr);
	c.rgbGreen = GetGValue(cr);
	c.rgbBlue = GetBValue(cr);
	c.rgbReserved=0;

	return (c); 
}

BOOL CDib::SetTransparent(COLORREF crLow,COLORREF crHi)
{
	ASSERT(m_pMaskBits==NULL);

	if(m_pMaskBits)
	{
		delete [] m_pMaskBits;
		m_pMaskBits = NULL;
	}

	if(crLow==-1)
		return (FALSE); 

	BOOL bOk = FALSE;

	m_pMaskBits = new BYTE[m_nBitCount];

	if(!m_pMaskBits)
		return (FALSE); 

	memcpy(m_pMaskBits,m_pBits,m_nBitCount);

	if(m_hPalette)
	{
		UINT n = GetNearestIndex(crLow);
		UINT nBlack = GetNearestIndex(0);
		UINT nWhite = GetNearestIndex(RGB(255,255,255));
		if(n!=CLR_INVALID)
		{
			BYTE* p = m_pBits;
			UINT i = 0;
			for(i=0;i<m_nBitCount;i++)
			{
				if(p[i]==n)
				{
					p[i] = nBlack;
					bOk = TRUE;
				}
			}
			p = m_pMaskBits;
			for(i=0;i<m_nBitCount;i++)
			{
				if(p[i]==n)
					p[i] = nWhite;
				else
					p[i] = nBlack;
			}
		}
	}
	else
	{
		BYTE lr = GetRValue(crLow);
		BYTE lg = GetGValue(crLow);
		BYTE lb = GetBValue(crLow);

		BYTE hr = GetRValue(crHi);
		BYTE hg = GetGValue(crHi);
		BYTE hb = GetBValue(crHi);

		struct tagRGB
		{
			BYTE b;
			BYTE g;
			BYTE r;
		};

		tagRGB* p;
		tagRGB* p2;

		for(UINT y=0;y<Height();y++)
		{
			p = (tagRGB*)(m_pBits + y * m_nLineWidth );
			p2 = (tagRGB*)(m_pMaskBits + y * m_nLineWidth );

			for(UINT x=0;x<Width();x++)
			{
				if(p->r>=lr && p->r<=hr &&
					p->g>=lg && p->g<=hg &&
					p->b>=lb && p->b<=hb)
				{
					p->r = 0;
					p->g = 0;
					p->b = 0;

					p2->r = 255;
					p2->g = 255;
					p2->b = 255;
				}
				else
				{
					p2->r = 0;
					p2->g = 0;
					p2->b = 0;
				}

				p ++;
				p2 ++;
			}
		}
	}

	return(bOk); 
}

UINT CDib::GetNearestIndex(COLORREF cr)
{
    int nColor = GetColorNum();
	if ((m_pBMI==NULL)||nColor==0)
		return (0); 

	return (GetNearestPaletteIndex(m_hPalette,cr)); 
}

BOOL CDib::IsTransparent()
{
    return (m_pMaskBits!=NULL); 
}

DWORD CDib::Width()
{	
	if (!m_pBMI)
		return 0;
	return m_pBMI->bmiHeader.biWidth;
}

DWORD CDib::Height()
{
	if (!m_pBMI)
		return 0;
	return abs(m_pBMI->bmiHeader.biHeight);
}

#endif //USE_CDIB