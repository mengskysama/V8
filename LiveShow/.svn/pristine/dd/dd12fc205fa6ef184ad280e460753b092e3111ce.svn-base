#include "StdAfx.h"
#include "IconMaker.h"

typedef struct tagICONIMAGE
{  
    UINT	Width;
    UINT	Height;
    UINT	Colors;
    LPBYTE	lpBits;
    DWORD	dwNumBytes;
    LPBITMAPINFO	lpbi;
    LPBYTE	lpXOR;
    LPBYTE	lpAND;
}ICONIMAGE,*LPICONIMAGE;

typedef struct tagICONDIRENTRY
{
    BYTE	bWidth;
    BYTE	bHeight;
    BYTE	bColorCount;
    BYTE	bReserved;
    WORD	wPlanes;
    WORD	wBitCount;
    DWORD	dwBytesInRes;
    DWORD	dwImageOffset;
}ICONDIRENTRY,*LPICONDIRENTRY;


UINT CheckIconHead(const LPBYTE pBuff, LONG lSize, LONG& lPos)
{
    lPos = 0;
    WORD input;
    if(lSize <= sizeof(WORD) * 3)
        return (UINT)-1;
    memcpy(&input, pBuff + lPos, sizeof(WORD));
    lPos += sizeof(WORD);
    if (input != 0)
        return (UINT)-1;
    memcpy(&input, pBuff + lPos, sizeof(WORD));
    lPos += sizeof(WORD);
    if (input != 1)
        return (UINT)-1;
    memcpy(&input, pBuff + lPos, sizeof(WORD));
    lPos += sizeof(WORD);
    return input;
}

UINT CheckCursorHead(const LPBYTE pBuff, LONG lSize, LONG& lPos)
{
    lPos = 0;
    WORD input;
    if(lSize <= sizeof(WORD) * 3)
        return (UINT)-1;
    memcpy(&input, pBuff + lPos, sizeof(WORD));
    lPos += sizeof(WORD);
    if (input != 0)
        return (UINT)-1;
    memcpy(&input, pBuff + lPos, sizeof(WORD));
    lPos += sizeof(WORD);
    if (input != 2)
        return (UINT)-1;
    memcpy(&input, pBuff + lPos, sizeof(WORD));
    lPos += sizeof(WORD);
    return input;
}

DWORD DIBNumColors(LPSTR lpbi)
{ 
    WORD wBitCount;
    DWORD dwClrUsed = ((LPBITMAPINFOHEADER)lpbi)->biClrUsed;
    if (dwClrUsed)
        return (DWORD)dwClrUsed;
    wBitCount = ((LPBITMAPINFOHEADER)lpbi)->biBitCount;
    switch(wBitCount)
    {
    case 1:
        return 2;
    case 4:
        return 16;
    case 8:
        return 256;
    case 16:
        return 65536;
    case 24:
        return 0;
    case 32:
        return 0;
    default:
        return 0;
    }
    return 0;
}

DWORD PaletteSize(LPSTR lpbi)
{
    return (DIBNumColors(lpbi) * sizeof(RGBQUAD));
}

LPSTR FindDIBBits(LPSTR lpbi)
{
    return (lpbi + *(LPDWORD)lpbi + PaletteSize(lpbi));
}

#define WIDTHBYTES(bits) ((((bits) + 31) >> 5) << 2)

DWORD BytesPerLine(LPBITMAPINFOHEADER lpBMIH)
{
    return WIDTHBYTES(lpBMIH->biWidth * lpBMIH->biPlanes * lpBMIH->biBitCount);
}

HICON MakeIconFromResource(LPICONIMAGE lpIcon, BOOL bIcon)
{
    HICON hIcon = NULL;

    if( lpIcon == NULL )
        return NULL;
    if( lpIcon->lpBits == NULL )
        return NULL;

    const DWORD dwWidth = (*(LPBITMAPINFOHEADER)(lpIcon->lpBits)).biWidth;
    const DWORD dwHeight = (*(LPBITMAPINFOHEADER)(lpIcon->lpBits)).biHeight/2;

    if (lpIcon->Colors == 32) {
        //BITMAPV5HEADER
        BITMAPV5HEADER biHeader = {0};
        biHeader.bV5Size = sizeof(BITMAPV5HEADER);
        biHeader.bV5Width = dwWidth;
        biHeader.bV5Height = dwHeight;
        biHeader.bV5Planes = 1;
        biHeader.bV5BitCount = 32;
        biHeader.bV5Compression = BI_BITFIELDS;
        biHeader.bV5RedMask = 0x00FF0000;
        biHeader.bV5GreenMask = 0x0000FF00;
        biHeader.bV5BlueMask = 0x000000FF;
        biHeader.bV5AlphaMask = 0xFF000000;
		biHeader.bV5CSType = LCS_sRGB;
		biHeader.bV5Intent = LCS_GM_ABS_COLORIMETRIC;
		biHeader.bV5Reserved = 0;

        void* lpBits = NULL;
        HDC hDC = ::GetDC(NULL);
        HBITMAP hBitmap = ::CreateDIBSection(hDC, (BITMAPINFO*)&biHeader, DIB_RGB_COLORS, &lpBits, NULL, 0);
        HBITMAP hMonoBitmap = ::CreateBitmap(dwWidth, dwHeight, 1, 1, NULL);
        ::ReleaseDC(NULL, hDC);

        if (lpBits != NULL) {
            DWORD* lpDst = (DWORD*)lpBits;
            DWORD* lpSrc = (DWORD*)(lpIcon->lpXOR);
            for (DWORD i=0; i<dwHeight; i++) {
                for (DWORD j=0; j<dwWidth; j++) {
                    *lpDst ++ = *lpSrc++;
                }
            }
            ICONINFO iconInfo = {0};
            iconInfo.fIcon = TRUE;
            iconInfo.xHotspot = 0;
            iconInfo.yHotspot = 0;
            iconInfo.hbmColor = hBitmap;
            iconInfo.hbmMask = hMonoBitmap;
            HICON h32Icon = ::CreateIconIndirect(&iconInfo);
            if (h32Icon) {
                ::DeleteObject(hBitmap);
                ::DeleteObject(hMonoBitmap);
                return h32Icon;
            }
        }

        ::DeleteObject(hBitmap);
        ::DeleteObject(hMonoBitmap);
    }

    hIcon = CreateIconFromResourceEx( lpIcon->lpBits, lpIcon->dwNumBytes, bIcon, 0x00030000, 
        dwWidth, dwHeight, 0 );

    if( hIcon == NULL ) {
        if(lpIcon->lpbi->bmiHeader.biBitCount != 16) {
            hIcon = CreateIconFromResource( lpIcon->lpBits, lpIcon->dwNumBytes, bIcon, 0x00030000 );
        }
    }

    return hIcon;
}

HICON MakeIconFromBuf(LPBYTE pBuf, LONG lDataSize, LONG iconSize)
{
    if (pBuf == NULL 
        || lDataSize == 0)
        return NULL;

    LONG lPos;
    UINT iconCount = CheckIconHead(pBuf, lDataSize, lPos);
    if (iconCount == (UINT)-1)
        return NULL;

    //判断操作系统版本，低于2000（含2000）的系统不支持32位
    OSVERSIONINFO osvi = {0};
    osvi.dwOSVersionInfoSize = sizeof(osvi);
    ::GetVersionEx(&osvi);
    BOOL bUnder2000 = (osvi.dwPlatformId == VER_PLATFORM_WIN32_WINDOWS) 
        || (osvi.dwMajorVersion == 4)
        || (osvi.dwPlatformId == VER_PLATFORM_WIN32_NT && osvi.dwMajorVersion == 5 && osvi.dwMinorVersion == 0);

    LPBYTE pEntry = pBuf + lPos;
    if (iconSize > 0) {
        //查找符合大小的，颜色位数最深的
        LPICONDIRENTRY pStartEntry = (LPICONDIRENTRY)pEntry;
        LPICONDIRENTRY pFinded = NULL;
        LONG lBitCount = -1;
        for (UINT i=0; i<iconCount; i++, pStartEntry++) {
            if (bUnder2000 && pStartEntry->wBitCount > 24)
                continue;
            if (pStartEntry->bHeight == iconSize && pStartEntry->wBitCount > lBitCount) {
                pFinded = pStartEntry;
                lBitCount = pStartEntry->wBitCount;
            }
        }
        if (pFinded != NULL)
            pEntry = (LPBYTE)pFinded;
    }

    LPICONDIRENTRY lpIDE = (LPICONDIRENTRY)malloc(sizeof(ICONDIRENTRY));
    if (lpIDE == NULL)
        return NULL;
    memcpy(lpIDE, pEntry, sizeof(ICONDIRENTRY));
    LPICONIMAGE lpIconImage = (LPICONIMAGE)malloc(sizeof(ICONIMAGE));
    if (lpIconImage == NULL) {
        free(lpIDE);
        return NULL;
    }
    lpIconImage->lpBits = (LPBYTE)malloc(lpIDE->dwBytesInRes);
    if (lpIconImage->lpBits == NULL) {
        free(lpIconImage);
        free(lpIDE);
        return NULL;
    }
    memcpy(lpIconImage->lpBits, pBuf + lpIDE->dwImageOffset, lpIDE->dwBytesInRes);
    lpIconImage->lpbi = (LPBITMAPINFO)lpIconImage->lpBits;
    lpIconImage->dwNumBytes = lpIDE->dwBytesInRes;
    lpIconImage->Width = lpIconImage->lpbi->bmiHeader.biWidth;
    lpIconImage->Height = (lpIconImage->lpbi->bmiHeader.biHeight)/2;
    lpIconImage->Colors = lpIconImage->lpbi->bmiHeader.biPlanes * lpIconImage->lpbi->bmiHeader.biBitCount;
    lpIconImage->lpXOR = (LPBYTE)FindDIBBits((LPSTR)lpIconImage->lpbi);
    lpIconImage->lpAND = lpIconImage->lpXOR + (lpIconImage->Height*BytesPerLine((LPBITMAPINFOHEADER)lpIconImage->lpbi));
    HICON hIcon = MakeIconFromResource(lpIconImage, TRUE);
    free(lpIconImage->lpBits);
    free(lpIconImage);
    free(lpIDE);

    return hIcon;
}

HCURSOR MakeCursorFromBuf(LPBYTE pBuf, LONG lDataSize)
{
    if (pBuf == NULL 
        || lDataSize == 0)
        return NULL;

    LONG lPos;
    if (CheckCursorHead(pBuf, lDataSize, lPos) == (UINT)-1)
        return NULL;

    LPICONDIRENTRY lpIDE = (LPICONDIRENTRY)malloc(sizeof(ICONDIRENTRY));
    if (lpIDE == NULL)
        return NULL;
    memcpy(lpIDE, pBuf + lPos, sizeof(ICONDIRENTRY));
    LPICONIMAGE lpIconImage = (LPICONIMAGE)malloc(sizeof(ICONIMAGE));
    if (lpIconImage == NULL) {
        free(lpIDE);
        return NULL;
    }
    lpIconImage->lpBits = (LPBYTE)malloc(lpIDE->dwBytesInRes);
    if (lpIconImage->lpBits == NULL) {
        free(lpIconImage);
        free(lpIDE);
        return NULL;
    }
    memcpy(lpIconImage->lpBits, pBuf + lpIDE->dwImageOffset, lpIDE->dwBytesInRes);
    lpIconImage->lpbi = (LPBITMAPINFO)lpIconImage->lpBits;
    lpIconImage->dwNumBytes = lpIDE->dwBytesInRes;
    lpIconImage->Width = lpIconImage->lpbi->bmiHeader.biWidth;
    lpIconImage->Height = (lpIconImage->lpbi->bmiHeader.biHeight)/2;
    lpIconImage->Colors = lpIconImage->lpbi->bmiHeader.biPlanes * lpIconImage->lpbi->bmiHeader.biBitCount;
    const LONG nBytesPerLine = BytesPerLine((LPBITMAPINFOHEADER)lpIconImage->lpbi);
    lpIconImage->lpXOR = (LPBYTE)FindDIBBits((LPSTR)lpIconImage->lpbi);
    lpIconImage->lpAND = lpIconImage->lpXOR + (lpIconImage->Height*nBytesPerLine);

    HCURSOR hCursor = NULL;
    ICONINFO cursorInfo = {0};
    HICON hIcon = MakeIconFromResource(lpIconImage, TRUE);
    if (hIcon == NULL) {
        goto CLEAR;
    }
    if (! ::GetIconInfo(hIcon, &cursorInfo)) {
        ::DeleteObject(cursorInfo.hbmColor);
        ::DeleteObject(cursorInfo.hbmMask);
        ::DestroyIcon(hIcon);
        goto CLEAR;
    }

    cursorInfo.fIcon = FALSE;
    cursorInfo.xHotspot = lpIDE->wPlanes;
    cursorInfo.yHotspot = lpIDE->wBitCount;
    hCursor = (HCURSOR)::CreateIconIndirect(&cursorInfo);
    ::DeleteObject(cursorInfo.hbmColor);
    ::DeleteObject(cursorInfo.hbmMask);
    ::DestroyIcon(hIcon);

CLEAR:
    free(lpIconImage->lpBits);
    free(lpIconImage);
    free(lpIDE);

    return hCursor;
}

