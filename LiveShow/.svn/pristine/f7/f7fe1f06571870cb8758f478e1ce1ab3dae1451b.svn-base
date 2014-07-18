#include "stdafx.h"
#include <WindowsX.h>
#include "UIHelper.h"
#include "ColorAdjuster.h"

/// 整除吗？
inline BOOL IsMod(int nDividend, int nDivisor)
{
	return (nDividend % nDivisor) == 0;
}

HANDLE NSUIHelper::LoadImageFromFile(IN LPCTSTR szFilePath, UINT uImageType/* = IMAGE_BITMAP*/)
{
	return LoadImage(NULL, szFilePath, uImageType, 0, 0, LR_LOADFROMFILE);
}

void NSUIHelper::CenterRect( LPRECT prcContainer, UINT nHeight, UINT nWidth, LPRECT prcDest)
{
	// 计算合适的绘制比例
	DWORD dwImgWidth	=	nWidth;
	DWORD dwImgHeight	=	nHeight;

	DWORD dwContainerWidth	=	prcContainer->right - prcContainer->left;
	DWORD dwContainerHeight	=	prcContainer->bottom - prcContainer->top;

	DWORD dwXDest, dwYDest, dwXDestWidth, dwYDestHeight;

	const float fRatio = (float)dwContainerHeight/dwContainerWidth;

	// 如果图片的区域小于容器的区域
	if( dwImgWidth < dwContainerWidth && dwImgHeight < dwContainerHeight)
	{
		dwXDest = prcContainer->left + (dwContainerWidth - dwImgWidth)/2;
		dwYDest = prcContainer->top  + (dwContainerHeight - dwImgHeight)/2;

		dwXDestWidth	=	dwImgWidth;
		dwYDestHeight	=	dwImgHeight;
	}
	else
	{
		const float fImgRatio = (float)dwImgHeight/dwImgWidth;

		// 高度大于宽度
		if(fImgRatio > fRatio)
		{
			dwXDestWidth = dwContainerHeight/fImgRatio;

			if( dwContainerWidth > dwXDestWidth)
			{
				dwXDest = prcContainer->left + (dwContainerWidth - dwXDestWidth)/2;
			}
			else
			{
				dwXDest = prcContainer->left + ( dwXDestWidth - dwContainerWidth)/2;
			}

			dwYDest = prcContainer->top;
			dwYDestHeight = dwContainerHeight;
		}
		else
		{
			dwXDest = prcContainer->left;
			dwXDestWidth = dwContainerWidth;
			dwYDestHeight = dwContainerWidth * fImgRatio;

			if( dwYDestHeight > dwContainerHeight)
			{
				dwYDest = prcContainer->top + (dwYDestHeight - dwContainerHeight)/2;
			}
			else
			{
				dwYDest = prcContainer->top + (dwContainerHeight - dwYDestHeight)/2;
			}
		}
	}

	prcDest->left = dwXDest;
	prcDest->top  = dwYDest;
	SetWidth(prcDest, dwXDestWidth);
	SetHeight(prcDest, dwYDestHeight);
}
/// 输出位图(对BitBlt/StretchBlt的封装)
/*
@param	nSrcWidth和nSrcHeight等于0时,使用位图的宽和高来代替
*/
BOOL NSUIHelper::OutputBitmap(	CDC* pDC, CBitmap* pBmpOutput, int x, int y, int nWidth, int nHeight, 
								   EPictureStretchType eStretchType/* = eStretchType_NoStretch*/, COLORREF crClippingColor/* = (-1)*/,
								   int xSrc/* = 0*/, int ySrc/* = 0*/, int nSrcWidth/* = -1*/, int nSrcHeight/* = -1*/)
{
	if (!pDC || !pBmpOutput)
	{
		ASSERT(FALSE);
		return FALSE;
	}

	BOOL bRet = FALSE;

	BITMAP bm;
	pBmpOutput->GetBitmap(&bm);
	const int nBmpWidth = (nSrcWidth == -1 ? bm.bmWidth : nSrcWidth);
	const int nBmpHeight = (nSrcHeight == -1 ? bm.bmHeight : nSrcHeight);

	CDC dcMem;
	if (!dcMem.CreateCompatibleDC(pDC))
	{
		ASSERT(0);
		return false;
	}
	CBitmap* pOldBmp = dcMem.SelectObject(pBmpOutput);

	switch(eStretchType)
	{
	case eStretchType_NoStretch:
		{
			if (crClippingColor != (-1))
			{
				bRet = pDC->TransparentBlt(x, y, nWidth, nHeight, &dcMem, xSrc, ySrc, nBmpWidth, nBmpHeight, crClippingColor);
			}
			else
			{
				bRet = pDC->BitBlt(x, y, nWidth, nHeight, &dcMem, xSrc, ySrc, SRCCOPY);
			}
		}
		break;
	case eStretchType_Stretch:
		{
			if (crClippingColor != (-1))
			{
				bRet = pDC->TransparentBlt(x, y, nWidth, nHeight, &dcMem, xSrc, ySrc, 
					nSrcWidth > 0 ? nSrcWidth : nBmpWidth, nSrcHeight > 0 ? nSrcHeight: nBmpHeight, crClippingColor);
			}
			else
			{
				bRet = pDC->StretchBlt(x, y, nWidth, nHeight, &dcMem, xSrc, ySrc, 
					nSrcWidth > 0 ? nSrcWidth : nBmpWidth, nSrcHeight > 0 ? nSrcHeight: nBmpHeight, SRCCOPY);
			}
		}
		break;
	case eStretchType_Tile:
		{
			const int nBmpWidth2 = nSrcWidth != -1 ? nSrcWidth : bm.bmWidth;
			const int nBmpHeight2 = nSrcHeight != -1 ? nSrcHeight : bm.bmHeight;

			int nRow = nHeight / nBmpHeight2 + !IsMod(nHeight, nBmpHeight2);
			int nCol = nWidth / nBmpWidth2 + !IsMod(nWidth, nBmpWidth2);

			for (int r = 0; r < nRow; r++)
			{
				for (int c = 0; c < nCol; c++)
				{
					if (crClippingColor != (-1))
					{
						pDC->TransparentBlt(x + c * nBmpWidth2, y + r * nBmpHeight2, nBmpWidth2, nBmpHeight2, 
							&dcMem, xSrc, ySrc, nBmpWidth2, nBmpHeight2, crClippingColor);
					}
					else
					{
						pDC->BitBlt(x + c * nBmpWidth2, y + r * nBmpHeight2, nBmpWidth2, nBmpHeight2, 
							&dcMem, xSrc, ySrc, SRCCOPY);
					}
				}
			}

			bRet = TRUE;
		}
		break;
	default:
		ASSERT(FALSE);
		bRet = FALSE;
	}

	dcMem.SelectObject(pOldBmp);

	return bRet;
}

BOOL NSUIHelper::OutputBitmap(	CDC* pDC, CBitmap* pBmpOutput, const CRect& rect, 
										  EPictureStretchType eStretchType/* = eStretchType_NoStretch*/, COLORREF crClippingColor/* = (-1)*/,
										  int xSrc/* = 0*/, int ySrc/* = 0*/, int nSrcWidth/* = -1*/, int nSrcHeight/* = -1*/)
{
	return OutputBitmap(pDC, pBmpOutput, rect.left, rect.top, rect.Width(), rect.Height(), 
		eStretchType, crClippingColor, xSrc, ySrc, nSrcWidth, nSrcHeight);
}

BOOL NSUIHelper::OutputBitmap(	CDC* pDC, UINT idBmpOutput, const CRect& rect, 
										  EPictureStretchType eStretchType/* = eStretchType_NoStretch*/, COLORREF crClippingColor/* = (-1)*/,
										  int xSrc/* = 0*/, int ySrc/* = 0*/, int nSrcWidth/* = -1*/, int nSrcHeight/* = -1*/)
{
	CBitmap bmp;

	bool bRet = bmp.LoadBitmap(idBmpOutput) && bmp.GetSafeHandle() && OutputBitmap(pDC, &bmp, rect.left, rect.top, rect.Width(), rect.Height(), 
					eStretchType, crClippingColor, xSrc, ySrc, nSrcWidth, nSrcHeight);
	bmp.DeleteObject();

	return bRet;
}

BOOL NSUIHelper::OutputBitmap(	CDC* pDC, LPCTSTR szPath, const CRect& rect, 
							  EPictureStretchType eStretchType/* = eStretchType_NoStretch*/, COLORREF crClippingColor/* = (-1)*/,
							  int xSrc/* = 0*/, int ySrc/* = 0*/, int nSrcWidth/* = -1*/, int nSrcHeight/* = -1*/)
{
	HBITMAP h = (HBITMAP)LoadImageFromFile(szPath);
	ASSERT(h);
	CBitmap* b = CBitmap::FromHandle(h);
	ASSERT(b);
	BOOL bRet = OutputBitmap(pDC, b, rect, eStretchType, crClippingColor, xSrc, ySrc, nSrcWidth, nSrcHeight);

	DeleteBitmap(h);
	return bRet;
}

BITMAP NSUIHelper::GetBitmapInfo(UINT nBmpId)
{
	BITMAP bmRet;
	memset(&bmRet, 0, sizeof bmRet);

	CBitmap bmp;
	if (bmp.LoadBitmap(nBmpId))
	{
		bmp.GetObject(sizeof bmRet, &bmRet);
	}

	return bmRet;
}

BITMAP NSUIHelper::GetBitmapInfo(LPCTSTR szBmpPath)
{
	BITMAP bmRet;
	memset(&bmRet, 0, sizeof bmRet);
	
	HANDLE handle = LoadImageFromFile(szBmpPath);
	if (handle)
	{
		GetObject(handle, sizeof bmRet, &bmRet);
		::DeleteObject(handle);
	}

	return bmRet;
}

BITMAP NSUIHelper::GetBitmapInfo(HBITMAP hBmp)
{
	BITMAP bmRet;
	memset(&bmRet, 0, sizeof bmRet);

	CBitmap* p = CBitmap::FromHandle(hBmp);
	if (p)
	{
		p->GetObject(sizeof bmRet, &bmRet);
	}

	return bmRet;
}

void NSUIHelper::DrawRectangleBox(CDC* pDC, CRect rc, COLORREF crBoxLine, int nLineWidth/* = 1*/)
{
	CPen pen(PS_SOLID, nLineWidth, crBoxLine);
	CPen* pOldPen = pDC->SelectObject(&pen);
	pDC->MoveTo(rc.TopLeft());
	pDC->LineTo(rc.right, rc.top);
	pDC->LineTo(rc.BottomRight());
	pDC->LineTo(rc.left, rc.bottom);
	pDC->LineTo(rc.TopLeft());

	pDC->SelectObject(pOldPen);
}

void NSUIHelper::DrawLine(CDC* pDC, CPoint ptFrom, CPoint ptTo, COLORREF crLine, int nLineWidth/* = 1*/)
{
	CPen pen(PS_SOLID, nLineWidth, crLine);
	CPen* pOldPen = pDC->SelectObject(&pen);
	pDC->MoveTo(ptFrom);
	pDC->LineTo(ptTo);

	pDC->SelectObject(pOldPen);
}

BOOL NSUIHelper::TrackMouseLeave(HWND hwndTrack)
{
	ASSERT(::IsWindow(hwndTrack));

	TRACKMOUSEEVENT tme;
	tme.cbSize		= sizeof(tme);
	tme.hwndTrack	= hwndTrack;
	tme.dwFlags		= TME_LEAVE|TME_HOVER;
	tme.dwHoverTime = 1;

	BOOL bRet = _TrackMouseEvent(&tme);	// post WM_MOUSELEAVE when mouse leave the wnd
	ASSERT(bRet);

	return bRet;
}

void NSUIHelper::CreateCharFormatWithColor(CHARFORMAT &cfDefault,DWORD dwEffects,LONG	yHeight,
												 BYTE bCharSet,TCHAR szFaceName[],COLORREF crTextColor)
{
	cfDefault.dwMask = CFM_CHARSET | CFM_FACE | CFM_SIZE | CFM_OFFSET | CFM_COLOR;
	cfDefault.dwMask ^= CFM_ITALIC ^ CFM_BOLD ^ CFM_STRIKEOUT ^ CFM_UNDERLINE;
	cfDefault.dwEffects = dwEffects;
	cfDefault.yHeight = yHeight;
	cfDefault.bCharSet = bCharSet;
	cfDefault.bPitchAndFamily = FIXED_PITCH | FF_MODERN;
	cfDefault.yOffset = 0;
	cfDefault.crTextColor = crTextColor;
	_tcscpy(cfDefault.szFaceName,szFaceName);
	cfDefault.cbSize = sizeof(cfDefault);
}

#include "ximage/ximage.h"
HICON NSUIHelper::CxImage2Icon(CxImage* pImage)
{
	HICON hIconRet = NULL;

	HBITMAP hMonoBitmap = CreateBitmap(pImage->GetWidth(),pImage->GetHeight(),1,1,NULL);
	ICONINFO ii;
	ii.fIcon = FALSE;  // Change fIcon to TRUE to create an alpha icon
	ii.xHotspot = 0;
	ii.yHotspot = 0;
	ii.hbmMask = hMonoBitmap;

	ii.hbmColor = (HBITMAP)pImage->MakeBitmap();
	// Create the alpha cursor with the alpha DIB section.
	hIconRet = CreateIconIndirect(&ii);
	DeleteObject(hMonoBitmap);

	return hIconRet;
}

CSize NSUIHelper::GetTextExtent(IN CDC* pDC, IN CString sText, IN CFont* pFont/* = NULL*/)
{
	ASSERT(pDC != NULL);

	CFont* pOldFont = NULL;
	if (pFont)
	{
		pOldFont = pDC->SelectObject(pFont);
	}

	if( sText == L"")
		sText = L"a";

	CSize sz(0, 0);
	::GetTextExtentPoint32(*pDC, sText, sText.GetLength(), &sz);

	if (pFont)
	{
		pDC->SelectObject(pOldFont);
	}

	return sz;

	//CFont* pFontOld = NULL;
	//if (pFont)
	//{
	//	pFontOld = pDC->SelectObject(pFont);
	//}
	//if (0 == pDC->DrawText(sText, &rcRet, DT_CALCRECT | DT_NOPREFIX))
	//{
	//	ASSERT(0);
	//}

	//if (pFont)
	//{
	//	pDC->SelectObject(pFontOld);
	//}

	//return rcRet.Size();
}

bool NSUIHelper::StretchBmp(CDC* pdc, HBITMAP hBmp, CRect rcDest, CRect rcSrc, COLORREF crTransparent/* = NO_TRANSPARENT*/)
{
	CBitmap* pBmp = CBitmap::FromHandle(hBmp);
	
	bool bRet = false;

	CDC dcMem;
	if (!dcMem.CreateCompatibleDC(pdc))
	{
		ASSERT(0);
		return false;
	}

	CBitmap* pOldBmp = dcMem.SelectObject(pBmp);

	if (crTransparent == NO_TRANSPARENT)
	{
		bRet = pdc->StretchBlt(rcDest.left, rcDest.top, rcDest.Width(), rcDest.Height(), &dcMem, rcSrc.left, rcSrc.top, rcSrc.Width(), rcSrc.Height(), SRCCOPY);
	}
	else
	{
		/*
		LONG uColorOut = 0xff00ff;

		CComPtr<ISkinEngine> pSkinEngine = NULL;
		GetSkinEngine(&pSkinEngine);
		ASSERT(pSkinEngine);
		UINT uType = 0;
		LONG uHue = 0;
		LONG uSat = 0;
		float uBlend = 0;
		LONG uTemp = 0;
		pSkinEngine->GetDefaultImageTransformParam(&uType, &uTemp, &uTemp, &uHue, &uSat, &uBlend, &uTemp, &uTemp, &uTemp);
		pSkinEngine->GetTransformedColor(0xff00ff, &uColorOut, uType, uHue, uSat, uBlend, 0, 0, 0);

		bRet = pdc->TransparentBlt(rcDest.left, rcDest.top, rcDest.Width(), rcDest.Height(), &dcMem, rcSrc.left, rcSrc.top, rcSrc.Width(), rcSrc.Height(), uColorOut);
		*/

		bRet = pdc->TransparentBlt(rcDest.left, rcDest.top, rcDest.Width(), rcDest.Height(), &dcMem, rcSrc.left, rcSrc.top, rcSrc.Width(), rcSrc.Height(), crTransparent);
	}

	//ASSERT(bRet);
	dcMem.SelectObject(pOldBmp);


	return bRet;
}

bool NSUIHelper::StretchBmp(CDC* pdc, UINT uBmpId, CRect rcDest, CRect rcSrc)
{
	bool bRet = false;

	CBitmap bmp;
	bmp.LoadBitmap(uBmpId);

	CDC dcMem;
	if (!dcMem.CreateCompatibleDC(pdc))
	{
		ASSERT(0);
		return false;
	}
	CBitmap* pOldBmp = dcMem.SelectObject(&bmp);

	bRet = pdc->StretchBlt(rcDest.left, rcDest.top, rcDest.Width(), rcDest.Height(), &dcMem, rcSrc.left, rcSrc.top, rcSrc.Width(), rcSrc.Height(), SRCCOPY);

	dcMem.SelectObject(pOldBmp);

	return bRet;
}

void NSUIHelper::StretchBmpWithRoundRectangle(CDC* pdc, HBITMAP hbmp, CRect rcDest, CSize szCorner/* = CSize(3, 3)*/, COLORREF crTransparent/* = NO_TRANSPARENT*/)
{
	if (!hbmp)
	{
		//ASSERT(0);
		return;
	}

	int nWidthSrc = GetBitmapInfo(hbmp).bmWidth;
	int nHeightSrc = GetBitmapInfo(hbmp).bmHeight;

	const CRect rcCornerLTSrc = CRect(CPoint(0, 0), szCorner);
	const CRect rcCornerTRSrc = CRect(CPoint(nWidthSrc - szCorner.cx, 0), szCorner);
	const CRect rcCornerRBSrc = CRect(CPoint(nWidthSrc - szCorner.cx, nHeightSrc - szCorner.cy), szCorner);
	const CRect rcCornerBLSrc = CRect(CPoint(0, nHeightSrc - szCorner.cy), szCorner);
	const CRect rcBorderLeftSrc = CRect(0, szCorner.cy, szCorner.cx, nHeightSrc - szCorner.cy);
	const CRect rcBorderTopSrc = CRect(szCorner.cx, 0, nWidthSrc - szCorner.cx, szCorner.cy);
	const CRect rcBorderRightSrc = CRect(nWidthSrc - szCorner.cx, szCorner.cy, nWidthSrc, nHeightSrc - szCorner.cy);
	const CRect rcBorderBottomSrc = CRect(szCorner.cx, nHeightSrc - szCorner.cy, nWidthSrc - szCorner.cx, nHeightSrc);
	const CRect rcMidSrc = CRect(szCorner.cx, szCorner.cy, nWidthSrc - szCorner.cx, nHeightSrc - szCorner.cy);

	const int nWidthDest = rcDest.Width();
	const int nHeightDest = rcDest.Height();

	CRect rcCornerLTDest = CRect(CPoint(0, 0), szCorner);
	CRect rcCornerTRDest = CRect(CPoint(nWidthDest - szCorner.cx, 0), szCorner);
	CRect rcCornerRBDest = CRect(CPoint(nWidthDest - szCorner.cx, nHeightDest - szCorner.cy), szCorner);
	CRect rcCornerBLDest = CRect(CPoint(0, nHeightDest - szCorner.cy), szCorner);
	CRect rcBorderLeftDest = CRect(0, szCorner.cy, szCorner.cx, nHeightDest - szCorner.cy);
	CRect rcBorderTopDest = CRect(szCorner.cx, 0, nWidthDest - szCorner.cx, szCorner.cy);
	CRect rcBorderRightDest = CRect(nWidthDest - szCorner.cx, szCorner.cy, nWidthDest, nHeightDest - szCorner.cy);
	CRect rcBorderBottomDest = CRect(szCorner.cx, nHeightDest - szCorner.cy, nWidthDest - szCorner.cx, nHeightDest);
	CRect rcMidDest = CRect(szCorner.cx, szCorner.cy, nWidthDest - szCorner.cx, nHeightDest - szCorner.cy);

	//#define PaintPiece(Piece) pDib->Paint(pdc->GetSafeHdc(), (RECT*)&Piece##Dest, (RECT*)&Piece##Src);
#define PaintPiece(Piece) \
	{ \
		Piece##Dest.OffsetRect(rcDest.TopLeft()); \
		StretchBmp(pdc, hbmp, Piece##Dest, Piece##Src, crTransparent); \
	}

	PaintPiece(rcCornerLT);
	PaintPiece(rcCornerTR);
	PaintPiece(rcCornerRB);
	PaintPiece(rcCornerBL);
	PaintPiece(rcBorderLeft);
	PaintPiece(rcBorderTop);
	PaintPiece(rcBorderRight);
	PaintPiece(rcBorderBottom);
	PaintPiece(rcMid);
}

std::vector<wstring> 
NSUIHelper::SplitterStringByPixel( wstring wstrText, CDC* pDC, CFont* pFont, CRect rect, UINT uLineSpace,BOOL bCompact)
{
	wstring wstrTemp = wstrText;

	std::vector<wstring> v;

	UINT nWidth = rect.Width();
	UINT nHeight = rect.Height();

	// 计算字符串"..."的像素长度
	CSize suspensionSize = GetTextExtent(pDC, L"...", pFont);
	suspensionSize.cy += uLineSpace;

	int i;
	for( i = 0; i < wstrTemp.length(); i++)
	{
		wstring wstrLeader = wstrTemp.substr(0, i+1);
		CSize textSize = GetTextExtent(pDC, wstrLeader.c_str(), pFont);
		textSize.cy += uLineSpace;

		// 如果当前行的下一行会超出矩形的区域
		if( ( v.size() + 2) * suspensionSize.cy > nHeight )
		{
			nWidth = rect.Width() - suspensionSize.cx;
		}

		if( textSize.cx >= nWidth)
		{
			wstrLeader = wstrTemp.substr(0,i);
			if( ( v.size() + 2) * textSize.cy > nHeight )
			{
				wstrLeader += L"...";
				v.push_back(wstrLeader);
				break;
			}
			else
			{
				v.push_back(wstrLeader);
				wstrTemp = wstrTemp.substr( i, wstrTemp.length());
				i = -1;
			}
		}
	}

	if( i == wstrTemp.length() && wstrTemp != L"")
		v.push_back( wstrTemp);

	return v;
}


int NSUIHelper::GetIconIndexForFileType(wstring sFileext/* = L"txt"*/, bool bLarge/* = false*/, bool bOpen/* = false*/)
{
	wstring sFooFile = L"foo." + sFileext;

	SHFILEINFO shfi;
	memset(&shfi, 0, sizeof(shfi));
	SHGetFileInfo(sFooFile.c_str(), FILE_ATTRIBUTE_NORMAL, &shfi, sizeof(shfi)
		,SHGFI_SYSICONINDEX | (bLarge ? SHGFI_LARGEICON : SHGFI_SMALLICON) | (bOpen ? SHGFI_OPENICON : 0) | SHGFI_USEFILEATTRIBUTES);

	return shfi.iIcon;
}

CSize NSUIHelper::GetIconSize(HICON hicon)
{
	ICONINFO ii;
	GetIconInfo(hicon, &ii);
	BITMAP bm = GetBitmapInfo(ii.hbmMask);
	BITMAP bm2 = GetBitmapInfo(ii.hbmColor);

	return CSize(bm.bmWidth, bm.bmHeight);
}

UINT NSUIHelper::GetCharWidthInfo( CDC* pDC, CFont* pFont, wstring strText  )
{
	if ( strText == L"")
		return 0;

	if( pDC == NULL)
		return FALSE;

	CFont* pOldFont = pDC->SelectObject(pFont);

	UINT nWidth = 0;

	CSize szRet;
	if (GetTextExtentPoint32(pDC->GetSafeHdc(), strText.c_str(), (int)strText.length(), &szRet))
		nWidth = szRet.cx;
	else
		nWidth = 0;

	pDC->SelectObject(pOldFont);

	return nWidth;
}

wstring
NSUIHelper::GetFittableStringFromFront( CDC* pDC, 
									   CFont* pFont, 
									   wstring wstrText, 
									   UINT nWidth, 
									   UINT nBeginIndex, 
									   UINT& nEndIndex)
{
	if( nBeginIndex >= wstrText.length())
	{
		nEndIndex = wstrText.length() + 1;
		return L"";
	}

	int i = 0;
	for( ; i <= (int)wstrText.length(); i++)
	{
		wstring strTemp = wstrText.substr( nBeginIndex, i);
		UINT nTextWidth = GetCharWidthInfo( pDC, pFont, strTemp);
		if( nTextWidth >= nWidth )
		{
			i--;
			break;
		}
	}

	wstring m_strShowText = wstrText.substr( nBeginIndex, i);
	nEndIndex = nBeginIndex + i - 1 ;

	return m_strShowText;
}

HBITMAP 
NSUIHelper::Colorize(CxImage* pImage, BYTE bHue, BYTE bSat, float bBlend)
{
	if( pImage == NULL || !pImage->IsValid())
		return NULL;

	const static long r = 99;
	const static long g = 155;
	const static long b = 233;

	float blend2 = sqrt( sqrt(bBlend));
	if( blend2 > 1.0f)
		blend2 = 1.0f;

	long newR = (long)(r*(1.0-blend2));
	long newG = (long)(g*(1.0-blend2));
	long newB = (long)(b*(1.0-blend2));

	pImage->ShiftRGB( -newR, -newG, -newB);
	pImage->Colorize( bHue, bSat, blend2);

	return pImage->MakeBitmap();
}