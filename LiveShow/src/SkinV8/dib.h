#pragma once

#ifdef USE_CDIB

#define PALVERSION   0x300
#define DIB_HEADER_MARKER   ((WORD) ('M' << 8) | 'B')
#define RECTWIDTH(lpRect)     ((lpRect)->right - (lpRect)->left)
#define RECTHEIGHT(lpRect)    ((lpRect)->bottom - (lpRect)->top)

class  CDib
{
public:
	CDib();
	 virtual ~CDib();
protected:
	LPBYTE m_pBits;
	DWORD m_nBitCount;
	DWORD m_nLineWidth;
	LPBYTE m_pMaskBits;
public:	
	LPBITMAPINFO m_pBMI;
	HPALETTE m_hPalette;
public:
	BOOL IsTransparent();
	BOOL SetTransparent(COLORREF crLow,COLORREF crHi);

	DWORD Width();
	
	DWORD Height();
	WORD  GetColorNum() const;

	BOOL  Paint(HDC, LPRECT, LPRECT);
	BOOL  Show(HDC hDC,int x,int y);
	BOOL  StretchShow(HDC hDC,int x,int y, int cx,int cy);
	BOOL Read(HANDLE hFile);
	BOOL ReadInFile(LPCTSTR szFileName);
	BOOL  CheckValid()
	{ 
		return (m_pBMI != NULL); 
	}
protected:
	BOOL  CreatePalette();
	WORD  PaletteSize() const;
	void Free();
	UINT GetNearestIndex(COLORREF c);
};



#endif //USE_CDIB