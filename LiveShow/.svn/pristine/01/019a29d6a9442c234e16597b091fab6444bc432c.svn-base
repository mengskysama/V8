#pragma once

#ifdef CXIMAGE_SUPPORT

#ifdef USE_GDIPLUS
#define USE_GDIPLUS_ANIMATION
#endif

#include "imagebase.h"
#include "ImageTransformParam.h"

class CxImage;
class CImageRepository;
class CImageTransformParam;
class CImageBaseImplCxImage :
	public CImageBase
{
	friend class CImageRepository;
public:
	CImageBaseImplCxImage(void);
	explicit CImageBaseImplCxImage(const CImageBaseImplCxImage& r);
	virtual ~CImageBaseImplCxImage(void);
	CImageBaseImplCxImage& operator = (const CImageBaseImplCxImage& r);
public:
	static ENUM_CXIMAGE_FORMATS CheckFormat(BYTE* pBuffer, long size);
public:
    static RGBQUAD TransformColor(RGBQUAD oldRGBColor, BYTE h, BYTE s, BYTE l);
    static RGBQUAD TransformColor2(RGBQUAD oldRGBColor, BYTE h, BYTE s, BYTE l);
	static BOOL TransformColor(const CImageTransformParam* pParam, LONG color, LONG& transformColor);
	static BOOL TransformParam(const CImageTransformParam& originParam, CImageTransformParam& transformParam);
	static BOOL TransformImage(CxImage* pImage, const CImageTransformParam* pParam);

	virtual BOOL LoadFromFile(LPCTSTR filePathName);
	virtual BOOL LoadFromData(const char* pzData, long size);
	virtual BOOL Transform(const CImageTransformParam* pParam);
	virtual BOOL Draw(HDC hDC, LPCRECT lpDstRect, LPRECT lpClipRect = NULL, const CImageTransformParam* pParam = NULL, 
        LONG clrTransparent = -1, BOOL bAlpha = FALSE, 
        BOOL bSideStretch = TRUE, LONG sideWidth = 0, LONG sideHeight = 0);
	virtual LONG Width();
	virtual LONG Height();
	virtual BOOL GetSize(LONG& lSize);

	//animation
	virtual LONG GetNumFrames();
	virtual BOOL GetAnimationInfo(tagANIMATEINFO& animateInfo);
	virtual BOOL GetFrameInfo(LONG iFrame, tagFRAMEINFO& frameInfo);
	virtual BOOL DrawFrame(HDC hDC, LONG iFrame, const RECT& dstRect);

protected:
    BOOL DrawTransparentCxImageHelper(HDC hDC, LPCRECT lpDstRect, CxImage* pDrawImage, LPRECT lpClipRect = NULL, LONG clrTransparent = -1);
    BOOL DrawAlphaCxImageHelper(HDC hDC, LPCRECT lpDstRect, CxImage* pDrawImage, LPRECT lpClipRect = NULL);
    BOOL DrawOriginCxImageHelper(HDC hDC, LPCRECT lpDstRect, CxImage* pDrawImage, LPRECT lpClipRect = NULL);
    BOOL DrawCxImageHelper(HDC hDC, LPCRECT lpDstRect, CxImage* pDrawImage, LPRECT lpClipRect = NULL, LONG clrTransparent = -1, BOOL bAlpha = FALSE);

protected:
	CxImage* m_pOriCxImage;		//原始图像
	CxImage* m_pTransformedCxImage;		//变换后图像，包括插值变换，颜色变换
#ifdef USE_GDIPLUS_ANIMATION
	Gdiplus::Image* m_pAnimationImage;
#else
	CxImage* m_pAnimationCxImage;		//动画图像，含多帧
#endif
	CImageTransformParam m_transformParam;	//变换后图像的变换参数
};

#endif //CXIMAGE_SUPPORT
