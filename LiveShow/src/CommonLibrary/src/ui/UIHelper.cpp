#include "../../StdAfx.h"
#include "../../include/ui/UIHelper.h"
#include "../../include/ui/PPDrawManager.h"

#pragma warning (push)
#pragma warning(disable : 4996)
#include "../../thirdlibrary/CxImage/include/ximage.h"
#pragma warning (pop)

HINSTANCE SwitchResourceToModule(LPCTSTR lpzModule)
{
    HINSTANCE hResOld = AfxGetResourceHandle();	// return
    HINSTANCE hResNew = NULL;

    CString strModule = lpzModule;
    if (strModule.Right(4).CompareNoCase(_T(".dll")) == 0)
    {
        hResNew = GetModuleHandle(strModule);
    }
    else
    {
#if _DEBUG
        hResNew = GetModuleHandle(strModule + _T("d.dll"));
#else
        hResNew = GetModuleHandle(strModule + _T(".dll"));
#endif
    }

    ASSERT(hResNew);
    AfxSetResourceHandle(hResNew);

    return hResOld;
};

namespace common { namespace ui
{
    HANDLE LoadImageFromFile(LPCTSTR lpzFilePath, UINT uType)
    {
        return ::LoadImage(NULL, lpzFilePath, uType, 0, 0, LR_LOADFROMFILE);
    };

    HICON LoadIconFromModule(UINT uID, LPCTSTR lpzModuleName)
    {
        HICON hRet = NULL;

        HINSTANCE hResOld = SwitchResourceToModule(lpzModuleName);
        {
            hRet = LoadIcon(AfxGetResourceHandle(), MAKEINTRESOURCE(uID));
        }
        AfxSetResourceHandle(hResOld);

        return hRet;
    };

    BOOL GetBitmapInfo(HBITMAP hBitmap, BITMAP& bitmap)
    {
        BITMAP bmpInfo = {0};
        int ret = ::GetObject(hBitmap, sizeof(bmpInfo), &bmpInfo);
        if (ret <= 0)
            return FALSE;

        bitmap = bmpInfo;
        return TRUE;
    };

    SIZE GetBitmapSize(HBITMAP hBitmap)
    {
        SIZE szSize = {0};
        BITMAP bmpInfo;
        if (! GetBitmapInfo(hBitmap, bmpInfo))
            return szSize;

        szSize.cx = bmpInfo.bmWidth;
        szSize.cy = bmpInfo.bmHeight;
        return szSize;
    };

    SIZE GetIconSize(HICON hIcon)
    {
        SIZE szSize = {0};
        ICONINFO icoInfo = {0};
        if (! ::GetIconInfo(hIcon, &icoInfo))
            return szSize;

        return GetBitmapSize(icoInfo.hbmColor);
    };

    HICON CreateIconFromBitmap(HBITMAP hBitmap)
    {
        BITMAP bmpInfo = {0};
        if (! GetBitmapInfo(hBitmap, bmpInfo))
            return NULL;

        HICON hIcon = NULL;

        HBITMAP hMonoBitmap = ::CreateBitmap(bmpInfo.bmWidth, bmpInfo.bmHeight, 1, 1, NULL);
        ICONINFO ii = {0};
        ii.fIcon = FALSE;
        ii.xHotspot = 0;
        ii.yHotspot = 0;
        ii.hbmMask = hMonoBitmap;
        ii.hbmColor = hBitmap;
        hIcon = ::CreateIconIndirect(&ii);
        ::DeleteObject(hMonoBitmap);

        return hIcon;
    };

    BOOL DrawBitmapEx(HDC hDC, HBITMAP hBitmap, const RECT& dstRect, const RECT& srcRect, COLORREF clrTransColor)
    {
        BITMAP bmpInfo = {0};
        if (! GetBitmapInfo(hBitmap, bmpInfo))
            return FALSE;

        HDC hMemDC = ::CreateCompatibleDC(hDC);
        if (hMemDC == NULL)
            return FALSE;

        HBITMAP hOldBmp = (HBITMAP)::SelectObject(hMemDC, hBitmap);

        BOOL bSuc = FALSE;
        if (clrTransColor == NO_TRANSPARENT) {
            bSuc = ::StretchBlt(hDC, 
                dstRect.left, dstRect.top, (dstRect.right - dstRect.left), (dstRect.bottom - dstRect.top), 
                hMemDC, 
                srcRect.left, srcRect.top, (srcRect.right - srcRect.left), (srcRect.bottom - srcRect.top), 
                SRCCOPY);
        } else {
            bSuc = ::TransparentBlt(hDC, 
                dstRect.left, dstRect.top, (dstRect.right - dstRect.left), (dstRect.bottom - dstRect.top), 
                hMemDC, 
                srcRect.left, srcRect.top, (srcRect.right - srcRect.left), (srcRect.bottom - srcRect.top), 
                clrTransColor);
        }

        ::SelectObject(hMemDC, hOldBmp);
        ::DeleteDC(hMemDC);

        return bSuc;
    };

    BOOL DrawBitmap(HDC hDC, HBITMAP hBitmap, const RECT& dstRect, COLORREF clrTransColor)
    {
        BITMAP bmpInfo = {0};
        if (! GetBitmapInfo(hBitmap, bmpInfo))
            return FALSE;

        RECT srcRect = {0, 0, bmpInfo.bmWidth, bmpInfo.bmHeight};
        return DrawBitmapEx(hDC, hBitmap, dstRect, srcRect, clrTransColor);
    };

    BOOL DrawBitmap(HDC hDC, LPCTSTR lpzBitmapFile, const RECT& dstRect, COLORREF clrTransColor)
    {
        HBITMAP hBitmap = (HBITMAP)LoadImageFromFile(lpzBitmapFile);
        if (hBitmap == NULL)
            return FALSE;

        CBitmap bmp;
        bmp.Attach(hBitmap);
        return DrawBitmap(hDC, hBitmap, dstRect, clrTransColor);
    }

    BOOL DrawRoundBitmap(HDC hDC, HBITMAP hBitmap, const RECT& dstRect, LONG nSideWidth, LONG nSideHeight, COLORREF clrTransColor)
    {
        BITMAP bmpInfo = {0};
        if (! GetBitmapInfo(hBitmap, bmpInfo))
            return FALSE;

        LONG bmpWidth = bmpInfo.bmWidth;
        LONG bmpHeight = bmpInfo.bmHeight;

        LONG dstWidth = dstRect.right - dstRect.left;
        LONG dstHeight = dstRect.bottom - dstRect.top;

        LONG minSideWidth = nSideWidth * 2;
        LONG minSideHeight = nSideHeight * 2;

        if (nSideWidth <= 0 || nSideHeight <= 0)
            return DrawBitmap(hDC, hBitmap, dstRect, clrTransColor);

        if (minSideWidth > bmpWidth || minSideHeight > bmpHeight)
            return DrawBitmap(hDC, hBitmap, dstRect, clrTransColor);

        if (minSideWidth > dstWidth || minSideHeight > dstHeight)
            return DrawBitmap(hDC, hBitmap, dstRect, clrTransColor);

        RECT rcDst = {0};
        RECT rcSrc = {0};
        BOOL bSuc = FALSE;

        //分成9块绘制
        //上左
        rcSrc.left = 0; rcSrc.right = nSideWidth;
        rcSrc.top = 0; rcSrc.bottom = nSideHeight;
        rcDst.left = 0; rcDst.right = nSideWidth;
        rcDst.top = 0; rcDst.bottom = nSideHeight;
        ::OffsetRect(&rcDst, dstRect.left, dstRect.top);
        bSuc = DrawBitmapEx(hDC, hBitmap, rcDst, rcSrc, clrTransColor);
        //上中
        rcSrc.left = nSideWidth; rcSrc.right = bmpWidth - nSideWidth;
        rcSrc.top = 0; rcSrc.bottom = nSideHeight;
        rcDst.left = nSideWidth; rcDst.right = dstWidth - nSideWidth;
        rcDst.top = 0; rcDst.bottom = nSideHeight;
        ::OffsetRect(&rcDst, dstRect.left, dstRect.top);
        bSuc = DrawBitmapEx(hDC, hBitmap, rcDst, rcSrc, clrTransColor);
        //上右
        rcSrc.left = bmpWidth - nSideWidth; rcSrc.right = bmpWidth;
        rcSrc.top = 0; rcSrc.bottom = nSideHeight;
        rcDst.left = dstWidth - nSideWidth; rcDst.right = dstWidth;
        rcDst.top = 0; rcDst.bottom = nSideHeight;
        ::OffsetRect(&rcDst, dstRect.left, dstRect.top);
        bSuc = DrawBitmapEx(hDC, hBitmap, rcDst, rcSrc, clrTransColor);

        //中左
        rcSrc.left = 0; rcSrc.right = nSideWidth;
        rcSrc.top = nSideHeight; rcSrc.bottom = bmpHeight - nSideHeight;
        rcDst.left = 0; rcDst.right = nSideWidth;
        rcDst.top = nSideHeight; rcDst.bottom = dstHeight - nSideHeight;
        ::OffsetRect(&rcDst, dstRect.left, dstRect.top);
        bSuc = DrawBitmapEx(hDC, hBitmap, rcDst, rcSrc, clrTransColor);
        //中中
        rcSrc.left = nSideWidth; rcSrc.right = bmpWidth - nSideWidth;
        rcSrc.top = nSideHeight; rcSrc.bottom = bmpHeight - nSideHeight;
        rcDst.left = nSideWidth; rcDst.right = dstWidth - nSideWidth;
        rcDst.top = nSideHeight; rcDst.bottom = dstHeight - nSideHeight;
        ::OffsetRect(&rcDst, dstRect.left, dstRect.top);
        bSuc = DrawBitmapEx(hDC, hBitmap, rcDst, rcSrc, clrTransColor);
        //中右
        rcSrc.left = bmpWidth - nSideWidth; rcSrc.right = bmpWidth;
        rcSrc.top = nSideHeight; rcSrc.bottom = bmpHeight - nSideHeight;
        rcDst.left = dstWidth - nSideWidth; rcDst.right = dstWidth;
        rcDst.top = nSideHeight; rcDst.bottom = dstHeight - nSideHeight;
        ::OffsetRect(&rcDst, dstRect.left, dstRect.top);
        bSuc = DrawBitmapEx(hDC, hBitmap, rcDst, rcSrc, clrTransColor);

        //下左
        rcSrc.left = 0; rcSrc.right = nSideWidth;
        rcSrc.top = bmpHeight - nSideHeight; rcSrc.bottom = bmpHeight;
        rcDst.left = 0; rcDst.right = nSideWidth;
        rcDst.top = dstHeight - nSideHeight; rcDst.bottom = dstHeight;
        ::OffsetRect(&rcDst, dstRect.left, dstRect.top);
        bSuc = DrawBitmapEx(hDC, hBitmap, rcDst, rcSrc, clrTransColor);
        //下中
        rcSrc.left = nSideWidth; rcSrc.right = bmpWidth - nSideWidth;
        rcSrc.top = bmpHeight - nSideHeight; rcSrc.bottom = bmpHeight;
        rcDst.left = nSideWidth; rcDst.right = dstWidth - nSideWidth;
        rcDst.top = dstHeight - nSideHeight; rcDst.bottom = dstHeight;
        ::OffsetRect(&rcDst, dstRect.left, dstRect.top);
        bSuc = DrawBitmapEx(hDC, hBitmap, rcDst, rcSrc, clrTransColor);
        //下右
        rcSrc.left = bmpWidth - nSideWidth; rcSrc.right = bmpWidth;
        rcSrc.top = bmpHeight - nSideHeight; rcSrc.bottom = bmpHeight;
        rcDst.left = dstWidth - nSideWidth; rcDst.right = dstWidth;
        rcDst.top = dstHeight - nSideHeight; rcDst.bottom = dstHeight;
        ::OffsetRect(&rcDst, dstRect.left, dstRect.top);
        bSuc = DrawBitmapEx(hDC, hBitmap, rcDst, rcSrc, clrTransColor);

        return TRUE;
    };

    HBITMAP MixBitmap(HBITMAP hDstMix, HBITMAP hSrcMix, LONG xOffset, LONG yOffset)
    {
        CxImage ximage;
        ximage.CreateFromHBITMAP(hDstMix);
        ximage.IncreaseBpp(24);

        CxImage ximageStatus;
        ximageStatus.CreateFromHBITMAP(hSrcMix);

        ximage.MixFrom(ximageStatus, xOffset, yOffset);

        return ximage.MakeBitmap();
    }

    HBITMAP FrameBitmap(HBITMAP hBmp, COLORREF clrInColor, COLORREF clrOutColor, UINT inLineWidth, UINT outLineWidth)
    {
        CxImage ximage;
        ximage.CreateFromHBITMAP(hBmp);
        ximage.IncreaseBpp(24);

        UINT totalWidth = inLineWidth + outLineWidth;
        if (totalWidth == 0)
            return ximage.MakeBitmap();

        LONG newWidth = ximage.GetWidth() + totalWidth * 2;
        LONG newHeight = ximage.GetHeight() + totalWidth * 2;

        RGBQUAD expandClr;
        expandClr.rgbBlue = GetBValue(clrInColor);
        expandClr.rgbGreen = GetGValue(clrInColor);
        expandClr.rgbRed = GetRValue(clrInColor);
        expandClr.rgbReserved = 0;
        ximage.Expand(newWidth, newHeight, expandClr);

        ximage.DrawLine(1, (newWidth - 1), 1, 1, clrOutColor);
        ximage.DrawLine(1, 1, 1, (newHeight - 1), clrOutColor);
        ximage.DrawLine((newWidth - 1), (newWidth - 1), 1, (newHeight - 1), clrOutColor);
        ximage.DrawLine((newWidth - 1), 1, (newHeight - 1), (newHeight - 1), clrOutColor);

        return ximage.MakeBitmap();
    }

    BOOL DrawRect(HDC hDC, const RECT& rcRect, COLORREF clrColor, int lineWidth)
    {
        HPEN hPen = ::CreatePen(PS_SOLID, lineWidth, clrColor);
        HPEN hOldPen = (HPEN)::SelectObject(hDC, hPen);

        ::MoveToEx(hDC, rcRect.left, rcRect.top, NULL);
        ::LineTo(hDC, rcRect.right - 1, rcRect.top);
        ::LineTo(hDC, rcRect.right - 1, rcRect.bottom - 1);
        ::LineTo(hDC, rcRect.left, rcRect.bottom - 1);
        ::LineTo(hDC, rcRect.left, rcRect.top);

        ::SelectObject(hDC, hOldPen);
        ::DeleteObject(hPen);

        return TRUE;
    };

    BOOL DrawLine(HDC hDC, const POINT& ptStart, const POINT& ptEnd, COLORREF clrColor, int lineWidth)
    {
        HPEN hPen = ::CreatePen(PS_SOLID, lineWidth, clrColor);
        HPEN hOldPen = (HPEN)::SelectObject(hDC, hPen);

        ::MoveToEx(hDC, ptStart.x, ptStart.y, NULL);
        ::LineTo(hDC, ptEnd.x, ptEnd.y);

        ::SelectObject(hDC, hOldPen);
        ::DeleteObject(hPen);

        return TRUE;
    };

    void FillGradient(HDC hDC, LPCRECT lpRect, COLORREF colorStart, COLORREF colorFinish, BOOL bHorz)
    {
        CPPDrawManager mgr;
        mgr.FillGradient(hDC, lpRect, colorStart, colorFinish, bHorz);
    }

    BOOL TrackMouseEvent(HWND hWnd)
    {
        ASSERT(::IsWindow(hWnd));
        if (! ::IsWindow(hWnd))
            return FALSE;

        TRACKMOUSEEVENT tme;
        tme.cbSize		= sizeof(tme);
        tme.hwndTrack	= hWnd;
        tme.dwFlags		= TME_LEAVE | TME_HOVER;
        tme.dwHoverTime = HOVER_DEFAULT;

        return _TrackMouseEvent(&tme);
    };

    BOOL CreateRichEditCharFormat(CHARFORMAT& cf, DWORD dwEffects, LONG lHeight, BYTE charSet, LPCTSTR lpzFaceName, COLORREF clrColor)
    {
        cf.dwMask = CFM_CHARSET | CFM_FACE | CFM_SIZE | CFM_OFFSET | CFM_COLOR;
        cf.dwMask ^= CFM_ITALIC ^ CFM_BOLD ^ CFM_STRIKEOUT ^ CFM_UNDERLINE;
        cf.dwEffects = dwEffects;
        cf.yHeight = lHeight;
        cf.bCharSet = charSet;
        cf.bPitchAndFamily = FIXED_PITCH | FF_MODERN;
        cf.yOffset = 0;
        cf.crTextColor = clrColor;
        _tcscpy_s(cf.szFaceName, LF_FACESIZE, lpzFaceName);
        cf.cbSize = sizeof(cf);

        return TRUE;
    };

    SIZE GetTextExtent(HDC hDC, LPCTSTR lpzText, HFONT hFont)
    {
        HGDIOBJ hOldGdiObj = NULL;
        if (hFont != NULL) {
            hOldGdiObj = ::SelectObject(hDC, hFont);
        }

        SIZE szText = {0};
        ::GetTextExtentPoint32(hDC, lpzText, (int)_tcsclen(lpzText), &szText);

        if (hFont != NULL) {
            ::SelectObject(hDC, hOldGdiObj);
        }

        return szText;
    };

    std::wstring GetFittableString(HDC hDC, LPCTSTR lpzText, LONG nWidth, LONG nStart, HFONT hFont, LPLONG lpEnd)
    {
        std::wstring wstrText = lpzText;
        LONG nEnd = 0;
        std::wstring::size_type len = wstrText.length();
        if(nStart >= (LONG)len) {
            nEnd = (LONG)len + 1;
            if (lpEnd != NULL)
                *lpEnd = nEnd;
            return L"";
        };

        HGDIOBJ hOldGdiObj = NULL;
        if (hFont != NULL) {
            hOldGdiObj = ::SelectObject(hDC, hFont);
        }

        wstrText = wstrText.substr(nStart);
        INT iFit = 0;
        SIZE sz = {0};
        ::GetTextExtentExPoint(hDC, wstrText.c_str(), (INT)wstrText.length(), nWidth, &iFit, NULL, &sz);

        if (hFont != NULL) {
            ::SelectObject(hDC, hOldGdiObj);
        }

        wstrText = wstrText.substr(0, iFit);
        nEnd = nStart + iFit;
        if (lpEnd != NULL)
            *lpEnd = nEnd;

        return wstrText;
    };

    int GetFileTypeIconIndex(LPCTSTR lpzExt, BOOL bLarge, BOOL bOpen)
    {
        std::wstring wstrFooFile = L"foo.";
        wstrFooFile += lpzExt;

        SHFILEINFO shfi = {0};
        SHGetFileInfo(wstrFooFile.c_str(), FILE_ATTRIBUTE_NORMAL, &shfi, sizeof(shfi)
            ,SHGFI_SYSICONINDEX | (bLarge ? SHGFI_LARGEICON : SHGFI_SMALLICON) | (bOpen ? SHGFI_OPENICON : 0) | SHGFI_USEFILEATTRIBUTES);

        return shfi.iIcon;
    };
} } //namespace common::ui

namespace common { namespace ui
{
    HRESULT CreateStreamOnMemory(IN void* pSrc, IN INT iSize, OUT LPSTREAM FAR* ppstm)
    {
        *ppstm = NULL;

        if (iSize < 0)
            return E_FAIL;

        HGLOBAL hGlobal = ::GlobalAlloc(GMEM_FIXED | GMEM_ZEROINIT, iSize);
        if (hGlobal == NULL)
            return E_FAIL;

        void* pDst = ::GlobalLock(hGlobal);
        memcpy_s(pDst, iSize, pSrc, iSize);
        ::GlobalUnlock(hGlobal);

        HRESULT hr = ::CreateStreamOnHGlobal(hGlobal, TRUE, ppstm);
        if (FAILED(hr)) {
            ::GlobalFree(hGlobal);
            return hr;
        }

        return hr;
    }
} } //namespace common::ui

namespace common { namespace ui
{
    INT ShowOpenFileDlg(OUT std::wstring& wstrFilePath, 
        IN OUT PBOOL pbReadOnly, 
        IN LPCTSTR lpszDefExt,
        IN LPCTSTR lpszFileName,
        IN LPCTSTR lpszFilter,	// eg: "图象文件(*.bmp;*.jpg;*.gif)|*.bmp;*.jpg;*.gif|所有文件(*.*)|*.*||"
        IN DWORD dwFlags,
        IN HWND hParentWnd
        )
    {
        if (pbReadOnly)
        {
            *pbReadOnly = FALSE;
        }

        INT nRet = IDCANCEL;

        CFileDialog dlg(TRUE, lpszDefExt, lpszFileName, dwFlags | OFN_ENABLESIZING, lpszFilter, CWnd::FromHandle(hParentWnd));
        dlg.m_ofn.lpstrInitialDir = wstrFilePath.c_str();
        nRet = (INT)dlg.DoModal();
        if (nRet == IDOK)
        {
            wstrFilePath = dlg.GetPathName();
            if (pbReadOnly)
            {
                *pbReadOnly = dlg.GetReadOnlyPref();
            }
        }

        return nRet;
    };

    INT ShowSaveFileDlg(OUT std::wstring& wstrFilePath, 
        IN LPCTSTR lpszDefExt,
        IN LPCTSTR lpszFileName,
        IN LPCTSTR lpszFilter,	// eg: "图象文件(*.bmp;*.jpg;*.gif)|*.bmp;*.jpg;*.gif|所有文件(*.*)|*.*||"
        IN DWORD dwFlags,
        IN HWND hParentWnd
        )
    {
        wstrFilePath.clear();
        INT nRet = IDCANCEL;

        CFileDialog dlg(FALSE, lpszDefExt, lpszFileName, dwFlags | OFN_ENABLESIZING, lpszFilter, CWnd::FromHandle(hParentWnd));
        nRet = (INT)dlg.DoModal();
        if (nRet == IDOK)
        {
            wstrFilePath = dlg.GetPathName();
        }

        return nRet;
    };
} } //namespace common::ui
