#pragma once

#ifdef GDIPLUS_BITMAP_SUPPORT

#include "imagebase.h"
#include "ImageTransformParam.h"

class CImageBaseImplBitmap :
	public CImageBase
{
public:
	CImageBaseImplBitmap(void);
	virtual ~CImageBaseImplBitmap(void);
public:
	static BOOL TransformColor(const CImageTransformParam* pParam, LONG color, LONG& transformColor);

	virtual BOOL LoadFromFile(LPCTSTR filePathName);
	virtual BOOL LoadFromData(const char* pzData, long size);
	virtual BOOL Transform(const CImageTransformParam* pParam);
	virtual BOOL Draw(HDC hDC, LPCRECT lpDstRect, LPRECT lpClipRect = NULL, const CImageTransformParam* pParam = NULL, 
        LONG clrTransparent = -1, BOOL bAlpha = FALSE, 
        BOOL bSideStretch = TRUE, LONG sideWidth = 0, LONG sideHeight = 0);
	virtual LONG Width();
	virtual LONG Height();
protected:
    Gdiplus::Bitmap* m_pOriBitmap;		//原始图像
	Gdiplus::Bitmap* m_pTransformedBitmap;		//变换后图像，包括插值变换，颜色变换
	CImageTransformParam m_transformParam;	//变换后图像的变换参数
};

#endif //GDIPLUS_BITMAP_SUPPORT
