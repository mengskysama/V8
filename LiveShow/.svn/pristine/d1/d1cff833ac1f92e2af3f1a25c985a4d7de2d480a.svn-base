#include "StdAfx.h"

#include "Skin_i.h"
#include <vector>
#include "assert.h"

#ifdef CXIMAGE_SUPPORT

#include "ImageTransformParam.h"
#include "atlimage.h"
#include "FileData.h"
#include ".\ImageBaseImplCxImage.h"
#include "Color.h"
#include "ColorAdjuster.h"

#ifdef USE_GDIPLUS
#include "xIStreamFile.h"
#endif

CImageBaseImplCxImage::CImageBaseImplCxImage(void)
:
m_pOriCxImage(NULL),
m_pTransformedCxImage(NULL)
{
#ifdef USE_GDIPLUS_ANIMATION
	m_pAnimationImage = NULL;
#else
	m_pAnimationCxImage = NULL;
#endif
}

CImageBaseImplCxImage::CImageBaseImplCxImage(const CImageBaseImplCxImage& r)
{

}

CImageBaseImplCxImage::~CImageBaseImplCxImage(void)
{
	//释放
	if (m_pOriCxImage != NULL) {
		delete m_pOriCxImage;
		m_pOriCxImage = NULL;
	}

#ifdef USE_GDIPLUS_ANIMATION
	if (m_pAnimationImage != NULL) {
		delete m_pAnimationImage;
		m_pAnimationImage = NULL;
	}
#else
	if (m_pAnimationCxImage != NULL) {
		delete m_pAnimationCxImage;
		m_pAnimationCxImage = NULL;
	}
#endif

	if (m_pTransformedCxImage != NULL) {
		delete m_pTransformedCxImage;
		m_pTransformedCxImage = NULL;
	}
}

CImageBaseImplCxImage& CImageBaseImplCxImage::operator = (const CImageBaseImplCxImage& r)
{
	if (this != &r){
		//do nothing
	}
	return *this;
}

ENUM_CXIMAGE_FORMATS CImageBaseImplCxImage::CheckFormat(BYTE* pBuffer, long size)
{
	static CxImage sCheckCxImage;

    //先用文件头的特殊字符来判断
    if (size < 10)
        return CXIMAGE_FORMAT_UNKNOWN;
    const BYTE* cpBuffer = pBuffer;
    if (cpBuffer[0] == 'G' && cpBuffer[1] == 'I' && cpBuffer[2] == 'F') {
        return CXIMAGE_FORMAT_GIF;
    } else if (cpBuffer[1] == 'P' && cpBuffer[2] == 'N' && cpBuffer[3] == 'G') {
        return CXIMAGE_FORMAT_PNG;
    } else if ( (cpBuffer[6] == 'E' && cpBuffer[7] == 'x' && cpBuffer[8] == 'i' && cpBuffer[9] == 'f') || (cpBuffer[6] == 'J' && cpBuffer[7] == 'F' && cpBuffer[8] == 'I' && cpBuffer[9] == 'F') ) {
        return CXIMAGE_FORMAT_JPG;
    } else if (cpBuffer[0] == 'B' && cpBuffer[1] == 'M') {
        return CXIMAGE_FORMAT_BMP;
    }

#if CXIMAGE_SUPPORT_BMP
	CXIMAGE_FORMAT_BMP;// = 1,
	if (sCheckCxImage.CheckFormat(pBuffer, size, CXIMAGE_FORMAT_BMP))
		return CXIMAGE_FORMAT_BMP;
#endif

#if CXIMAGE_SUPPORT_GIF
	CXIMAGE_FORMAT_GIF;// = 2,
	if (sCheckCxImage.CheckFormat(pBuffer, size, CXIMAGE_FORMAT_GIF))
		return CXIMAGE_FORMAT_GIF;
#endif

#if CXIMAGE_SUPPORT_JPG
	CXIMAGE_FORMAT_JPG;// = 3,
	if (sCheckCxImage.CheckFormat(pBuffer, size, CXIMAGE_FORMAT_JPG))
		return CXIMAGE_FORMAT_JPG;
#endif

#if CXIMAGE_SUPPORT_PNG
	CXIMAGE_FORMAT_PNG;// = 4,
	if (sCheckCxImage.CheckFormat(pBuffer, size, CXIMAGE_FORMAT_PNG))
		return CXIMAGE_FORMAT_PNG;
#endif

#if CXIMAGE_SUPPORT_ICO
	CXIMAGE_FORMAT_ICO;// = 5,
	if (sCheckCxImage.CheckFormat(pBuffer, size, CXIMAGE_FORMAT_ICO))
		return CXIMAGE_FORMAT_ICO;
#endif

#if CXIMAGE_SUPPORT_TIF
	CXIMAGE_FORMAT_TIF;// = 6,
	if (sCheckCxImage.CheckFormat(pBuffer, size, CXIMAGE_FORMAT_TIF))
		return CXIMAGE_FORMAT_TIF;
#endif

#if CXIMAGE_SUPPORT_TGA
	CXIMAGE_FORMAT_TGA;// = 7,
	if (sCheckCxImage.CheckFormat(pBuffer, size, CXIMAGE_FORMAT_TGA))
		return CXIMAGE_FORMAT_TGA;
#endif

#if CXIMAGE_SUPPORT_PCX
	CXIMAGE_FORMAT_PCX;// = 8,
	if (sCheckCxImage.CheckFormat(pBuffer, size, CXIMAGE_FORMAT_PCX))
		return CXIMAGE_FORMAT_PCX;
#endif

#if CXIMAGE_SUPPORT_WBMP
	CXIMAGE_FORMAT_WBMP;// = 9,
	if (sCheckCxImage.CheckFormat(pBuffer, size, CXIMAGE_FORMAT_WBMP))
		return CXIMAGE_FORMAT_WBMP;
#endif

#if CXIMAGE_SUPPORT_WMF
	CXIMAGE_FORMAT_WMF;// = 10,
	if (sCheckCxImage.CheckFormat(pBuffer, size, CXIMAGE_FORMAT_WMF))
		return CXIMAGE_FORMAT_WMF;
#endif

#if CXIMAGE_SUPPORT_JP2
	CXIMAGE_FORMAT_JP2;// = 11,
	if (sCheckCxImage.CheckFormat(pBuffer, size, CXIMAGE_FORMAT_JP2))
		return CXIMAGE_FORMAT_JP2;
#endif

#if CXIMAGE_SUPPORT_JPC
	CXIMAGE_FORMAT_JPC;// = 12,
	if (sCheckCxImage.CheckFormat(pBuffer, size, CXIMAGE_FORMAT_JPC))
		return CXIMAGE_FORMAT_JPC;
#endif

#if CXIMAGE_SUPPORT_PGX
	CXIMAGE_FORMAT_PGX;// = 13,
	if (sCheckCxImage.CheckFormat(pBuffer, size, CXIMAGE_FORMAT_PGX))
		return CXIMAGE_FORMAT_PGX;
#endif

#if CXIMAGE_SUPPORT_PNM
	CXIMAGE_FORMAT_PNM;// = 14,
	if (sCheckCxImage.CheckFormat(pBuffer, size, CXIMAGE_FORMAT_PNM))
		return CXIMAGE_FORMAT_PNM;
#endif

#if CXIMAGE_SUPPORT_RAS
	CXIMAGE_FORMAT_RAS;// = 15,
	if (sCheckCxImage.CheckFormat(pBuffer, size, CXIMAGE_FORMAT_RAS))
		return CXIMAGE_FORMAT_RAS;
#endif

#if CXIMAGE_SUPPORT_JBG
	CXIMAGE_FORMAT_JBG;// = 16,
	if (sCheckCxImage.CheckFormat(pBuffer, size, CXIMAGE_FORMAT_JBG))
		return CXIMAGE_FORMAT_JBG;
#endif

#if CXIMAGE_SUPPORT_MNG
	CXIMAGE_FORMAT_MNG;// = 17,
	if (sCheckCxImage.CheckFormat(pBuffer, size, CXIMAGE_FORMAT_MNG))
		return CXIMAGE_FORMAT_MNG;
#endif

#if CXIMAGE_SUPPORT_SKA
	CXIMAGE_FORMAT_SKA;// = 18,
	if (sCheckCxImage.CheckFormat(pBuffer, size, CXIMAGE_FORMAT_SKA))
		return CXIMAGE_FORMAT_SKA;
#endif

#if CXIMAGE_SUPPORT_RAW
	CXIMAGE_FORMAT_RAW;// = 19,
	if (sCheckCxImage.CheckFormat(pBuffer, size, CXIMAGE_FORMAT_RAW))
		return CXIMAGE_FORMAT_RAW;
#endif

	//Unknown
	CXIMAGE_FORMAT_UNKNOWN;// = 0,
	return CXIMAGE_FORMAT_UNKNOWN;
}

BOOL CImageBaseImplCxImage::LoadFromFile(LPCTSTR filePathName)
{
    CFileData fileData;
    if (! fileData.LoadFromFile(filePathName))
        return FALSE;

    return LoadFromData(fileData, fileData.GetLength());
}

BOOL CImageBaseImplCxImage::LoadFromData(const char* pzData, long size)
{
	//去掉const
	BYTE* pBuffer = (BYTE*) const_cast<char*>(pzData);

	//获得格式
	ENUM_CXIMAGE_FORMATS imgFormat = CheckFormat(pBuffer, size);
	if (imgFormat == CXIMAGE_FORMAT_UNKNOWN)
		return FALSE;

	//释放原来的
	if (m_pOriCxImage != NULL) {
		delete m_pOriCxImage;
		m_pOriCxImage = NULL;
	}
#ifdef USE_GDIPLUS_ANIMATION
	if (m_pAnimationImage != NULL) {
		delete m_pAnimationImage;
		m_pAnimationImage = NULL;
	}
#else
	if (m_pAnimationCxImage != NULL) {
		delete m_pAnimationCxImage;
		m_pAnimationCxImage = NULL;
	}
#endif

	//载入新的文件
	m_pOriCxImage = new CxImage;
	m_pOriCxImage->SetRetreiveAllFrames(true);
	if (m_pOriCxImage == NULL || !m_pOriCxImage->Decode(pBuffer, size, imgFormat) || !m_pOriCxImage->IsValid()) {
		delete m_pOriCxImage;
		m_pOriCxImage = NULL;
		return FALSE;
	}
	//如果多帧加载所有帧
	LONG lNumFrames = m_pOriCxImage->GetNumFrames();
	if (lNumFrames <= 1)
		return TRUE;

#ifdef USE_GDIPLUS_ANIMATION
	CComPtr<IStream> spIStream;
	HRESULT hr = ::CreateStreamOnMemory(pBuffer, size, &spIStream);
	if (FAILED(hr))
		return FALSE;

	m_pAnimationImage = Gdiplus::Image::FromStream(spIStream);
	return TRUE;
#else
	m_pAnimationCxImage = new CxImage;
	if (m_pAnimationCxImage == NULL)
		return FALSE;
	m_pAnimationCxImage->SetRetreiveAllFrames(true);
	m_pAnimationCxImage->SetFrame(lNumFrames - 1);
	if (!m_pAnimationCxImage->Decode(pBuffer, size, imgFormat) || !m_pAnimationCxImage->IsValid()) {
		delete m_pAnimationCxImage;
		m_pAnimationCxImage = NULL;
		return FALSE;
	}
#endif

	return TRUE;
}

BOOL CImageBaseImplCxImage::Transform(const CImageTransformParam* pParam)
{
	ASSERT(pParam != NULL && m_pOriCxImage != NULL);
	if (pParam == NULL || m_pOriCxImage == NULL)
		return FALSE;

	if (m_transformParam == *pParam)
		return TRUE;

	//删除变换图
	if (m_pTransformedCxImage != NULL){
		delete m_pTransformedCxImage;
		m_pTransformedCxImage = NULL;
	}

	//拷贝原始图像
	m_pTransformedCxImage = new CxImage;
	m_pTransformedCxImage->Copy(*m_pOriCxImage);

	BOOL bSuc = FALSE;
	switch (pParam->enTransformType)
	{
	case TRANSFORM_TYPE_NONE:
		bSuc = TRUE;
		break;
	case TRANSFORM_TYPE_SMOOTH:
		bSuc = m_pTransformedCxImage->Resample(pParam->width, pParam->height, 0);
		break;
	case TRANSFORM_TYPE_COLORIZE:
		bSuc = TransformImage(m_pTransformedCxImage, pParam);
		break;
	case TRANSFORM_TYPE_SHIFTRGB:
		bSuc = m_pTransformedCxImage->ShiftRGB(pParam->r, pParam->g, pParam->b);
		break;
	}

	if (bSuc) {
		//拷贝变换参数
		m_transformParam = *pParam;
	} else {
		delete m_pTransformedCxImage;
		m_pTransformedCxImage = new CxImage;
		m_pTransformedCxImage->Copy(*m_pOriCxImage);
		m_transformParam.enTransformType = TRANSFORM_TYPE_NONE;
	}
	return bSuc;
}

BOOL CImageBaseImplCxImage::TransformParam(const CImageTransformParam& originParam, CImageTransformParam& transformParam)
{
	transformParam = originParam;
	return TRUE;
}

RGBQUAD CImageBaseImplCxImage::TransformColor(RGBQUAD oldRGBColor, BYTE h, BYTE s, BYTE l)
{
    RGBQUAD oldHSLColor = CxImage::RGBtoHSL(oldRGBColor);

    double oldH = oldHSLColor.rgbRed;
    double oldS = oldHSLColor.rgbGreen;
    double oldL = oldHSLColor.rgbBlue;

    double newH = oldH;
    double newS = oldS;
    double newL = oldL;

    double deltaH = h - 127.0;
    double deltaS = s - 127.0;
    double deltaL = l - 127.0;

    double ratioH = deltaH / 127.0;
    double ratioS = deltaS / 127.0;
    double ratioL = deltaL / 127.0;

    newH += deltaH;
    if (newH > 255.0)
        newH -= 255.0;
    if (newH < 0.0)
        newH += 255.0;
    newH = h;

    if (ratioS < 0) {
        newS = oldS + oldS * ratioS;
    } else {
        newS = oldS + (255.0 - oldS) * ratioS;
    }

    if (newS < 0.0)
        newS = 0.0;
    if (newS > 255.0)
        newS = 255.0;

    if (ratioL < 0) {
        newL = oldL + oldL * ratioL;
    } else {
        newL = oldL + (255.0 - oldL) * ratioL;
    }

    RGBQUAD result;
    result.rgbRed = (BYTE)newH;
    result.rgbGreen = (BYTE)newS;
    result.rgbBlue = (BYTE)newL;

    return CxImage::HSLtoRGB(result);
}

RGBQUAD CImageBaseImplCxImage::TransformColor2(RGBQUAD oldRGBColor, BYTE h, BYTE s, BYTE l)
{
    RGBQUAD oldHSLColor = Color::RGBtoHSV(oldRGBColor);

    double oldH = oldHSLColor.rgbRed;
    double oldS = oldHSLColor.rgbGreen;
    double oldL = oldHSLColor.rgbBlue;

    double newH = oldH;
    double newS = oldS;
    double newL = oldL;

    double deltaH = h - 128.0;
    double deltaS = s - 128.0;
    double deltaL = l - 128.0;

    double ratioH = deltaH / 128.0;
    double ratioS = deltaS / 128.0;
    double ratioL = deltaL / 128.0;

    newH += deltaH;
    if (newH > 256.0)
        newH -= 256.0;
    if (newH < 0.0)
        newH += 256.0;

    if (ratioS < 0) {
        newS = oldS + oldS * ratioS;
    } else {
        newS = oldS + (256.0 - oldS) * ratioS;
    }

    if (newS < 0.0)
        newS = 0.0;
    if (newS > 256.0)
        newS = 256.0;

    if (ratioL < 0) {
        newL = oldL + oldL * ratioL;
    } else {
        newL = oldL + (256.0 - oldL) * ratioL;
    }

    RGBQUAD result;
    result.rgbRed = (BYTE)newH;
    result.rgbGreen = (BYTE)newS;
    result.rgbBlue = (BYTE)newL;

    return Color::HSVtoRGB(result);
}

BOOL CImageBaseImplCxImage::TransformImage(CxImage* pImage, const CImageTransformParam* pParam)
{
#define TRANSFORM_METHOD 4

#if (TRANSFORM_METHOD == 1)
    //方案1
    {
        BOOL bSuc = pImage->Colorize(pParam->hue, pParam->sat, pParam->blend);
        return bSuc;
    }
#endif

#if (TRANSFORM_METHOD == 2)
    //方案2
    {
        BYTE h = pParam->hue;
        BYTE s = pParam->sat;
        BYTE l = (BYTE)(pParam->blend * 255);
        const int width = pImage->GetWidth();
        const int height = pImage->GetHeight();
        for (int i=0; i<height; i++) {
            for (int j=0; j<width; j++) {
                RGBQUAD RGBClr = pImage->GetPixelColor(j, i, false);
                RGBClr = TransformColor(RGBClr, h, s, l);
                pImage->SetPixelColor(j, i, RGBClr, false);
            }
        }

        return TRUE;
    }
#endif

#if (TRANSFORM_METHOD == 3)
    //方案3
    {
        //界面的原色调
        const static long r = 99;
        const static long g = 143;
        const static long b = 200;

        float blend = sqrt(sqrt(pParam->blend));
        if (blend > 1.0f)
            blend = 1.0f;

        long newR = (long) (r * (1.0 - blend));
        long newG = (long) (g * (1.0 - blend));
        long newB = (long) (b * (1.0 - blend));

        const RGBQUAD clrNoTrans = {255, 255, 255, 255};
        const RGBQUAD clrChange = {255, 255, 255, 255};
        const RGBQUAD clrNoChange = {0, 0, 0, 0};

        CxImage maskImage;
        maskImage.Copy(*pImage);

        //计算mask
        const int width = maskImage.GetWidth();
        const int height = maskImage.GetHeight();
        for (int i=0; i<height; i++) {
            for (int j=0; j<width; j++) {
                RGBQUAD clr = maskImage.GetPixelColor(j, i, false);
                if (clr.rgbRed == clrNoTrans.rgbRed
                    && clr.rgbGreen == clrNoTrans.rgbGreen
                    && clr.rgbBlue == clrNoTrans.rgbBlue) {
                        maskImage.SetPixelColor(j, i, clrNoChange, false);
                } else {
                    maskImage.SetPixelColor(j, i, clrChange, false);
                }
            }
        }

        pImage->ShiftRGB(-newR, -newG, -newB);

        BOOL bSuc =  pImage->Colorize(pParam->hue, pParam->sat, blend);

        //恢复不变换颜色
        for (int i=0; i<height; i++) {
            for (int j=0; j<width; j++) {
                RGBQUAD clr = maskImage.GetPixelColor(j, i, false);
                if (clr.rgbRed == clrNoChange.rgbRed
                    && clr.rgbGreen == clrNoChange.rgbGreen
                    && clr.rgbBlue == clrNoChange.rgbBlue) {
                        pImage->SetPixelColor(j, i, clrNoTrans, false);
                }
            }
        }

        return bSuc;
    }
#endif

#if (TRANSFORM_METHOD == 4)
    //方案2
    {

        const RGBQUAD clrNoTrans = {255, 255, 255, 255};
        const RGBQUAD clrChange = {255, 255, 255, 255};
        const RGBQUAD clrNoChange = {0, 0, 0, 0};

        CxImage maskImage;
        maskImage.Copy(*pImage);

        //计算mask
        const int width = maskImage.GetWidth();
        const int height = maskImage.GetHeight();
        for (int i=0; i<height; i++) {
            for (int j=0; j<width; j++) {
                RGBQUAD clr = maskImage.GetPixelColor(j, i, false);
                if (clr.rgbRed == clrNoTrans.rgbRed
                    && clr.rgbGreen == clrNoTrans.rgbGreen
                    && clr.rgbBlue == clrNoTrans.rgbBlue) {
                        maskImage.SetPixelColor(j, i, clrNoChange, false);
                } else {
                    maskImage.SetPixelColor(j, i, clrChange, false);
                }
            }
        }

        BYTE h = pParam->hue;
        BYTE s = pParam->sat;
        BYTE l = (BYTE)(pParam->blend * 255);
        double hue = (h) * 360.0 / 255.0;
        double sat = (s) * 100.0 / 255.0;
        double light = (l - 128.0) * 100.0 / 128.0;
        static CColorAdjuster colorAdjuster;
        colorAdjuster.Init(hue, sat, light);
        static CColorGradationAdjuster colorGradationAdjuster;
        colorGradationAdjuster.Init(140, 255, 0, 255);
        for (int i=0; i<height; i++) {
            for (int j=0; j<width; j++) {
                RGBQUAD RGBClr = pImage->GetPixelColor(j, i, false);
                RGBClr = colorAdjuster.Adjust(RGBClr);
                //RGBClr = colorGradationAdjuster.Adjust(RGBClr);
                pImage->SetPixelColor(j, i, RGBClr, false);
            }
        }

        //恢复不变换颜色
        for (int i=0; i<height; i++) {
            for (int j=0; j<width; j++) {
                RGBQUAD clr = maskImage.GetPixelColor(j, i, false);
                if (clr.rgbRed == clrNoChange.rgbRed
                    && clr.rgbGreen == clrNoChange.rgbGreen
                    && clr.rgbBlue == clrNoChange.rgbBlue) {
                        pImage->SetPixelColor(j, i, clrNoTrans, false);
                }
            }
        }

        return TRUE;
    }
#endif
}

BOOL CImageBaseImplCxImage::TransformColor(const CImageTransformParam* pParam, LONG color, LONG& transformColor)
{
	transformColor = color;

	if (color == -1)
		return TRUE;

	if (pParam->enTransformType != TRANSFORM_TYPE_COLORIZE)
		return TRUE;

	static CxImage imageForColor(1, 1, 24, CXIMAGE_FORMAT_BMP);
	RGBQUAD quadColor = CxImage::RGBtoRGBQUAD(color);
	imageForColor.SetPixelColor(0, 0, quadColor);
	TransformImage(&imageForColor, pParam);
	quadColor = imageForColor.GetPixelColor(0, 0);

	transformColor = RGB(quadColor.rgbRed, quadColor.rgbGreen, quadColor.rgbBlue);
	return TRUE;
}

BOOL CImageBaseImplCxImage::DrawTransparentCxImageHelper(HDC hDC, LPCRECT lpDstRect, CxImage* pDrawImage, LPRECT lpClipRect, LONG clrTransparent)
{
    LONG left = lpDstRect->left;
    LONG top = lpDstRect->top;
    LONG width = (lpDstRect->right - lpDstRect->left);
    LONG height = (lpDstRect->bottom - lpDstRect->top);
    COLORREF transColor = RGB(GetRValue(clrTransparent), 
        GetGValue(clrTransparent), 
        GetBValue(clrTransparent));

#if WINVER >= 0x0500

#ifdef USE_GDIPLUS
    //CImage绘制透明效果
    CImage image;
	HBITMAP hBitmap = pDrawImage->MakeBitmap(hDC);
	if(hBitmap != NULL)
	{
		image.Attach(hBitmap);

		if(lpClipRect == NULL)
		{
			return image.TransparentBlt(hDC, *lpDstRect, transColor);
		}

		//DragonZ:支持画指定区域图片
		CRect clipRc = *lpClipRect;
		BOOL ret;
		if (clipRc.IsRectEmpty())
		{
			ret = image.TransparentBlt(hDC, *lpDstRect, transColor);
		}else
		{
			ret = image.TransparentBlt(hDC, *lpDstRect, *lpClipRect, transColor);
		}
		///////////
		return ret;
	}
	else
	{
		assert(0);
		return FALSE;
	}	
#else
    //CxImage绘制透明效果
    pDrawImage->SetTransColor(CxImage::RGBtoRGBQUAD(transColor));
    pDrawImage->SetTransIndex(0);
	//DragonZ:支持画指定区域图片
	if(lpClipRect == NULL)
	{
		return pDrawImage->Draw(hDC, *lpDstRect);
	}
	CRect clipRc = *lpClipRect;
    BOOL ret;
	if (clipRc.IsRectEmpty())
	{
		ret = pDrawImage->Draw(hDC, *lpDstRect);
	}else
	{
		ret = pDrawImage->Draw(hDC, *lpDstRect, lpClipRect);
	}
	///////////
    pDrawImage->SetTransIndex(-1);
    ASSERT(ret);
    return ret;
#endif

#else
    COLORREF oldBkColor = ::SetBkColor(hDC, RGB(255,255,255));
    COLORREF oldTextColor = ::SetTextColor(hDC, RGB(0,0,0));

    HDC imageDC = ::CreateCompatibleDC(hDC);
    HBITMAP imageBmp = ::CreateCompatibleBitmap(hDC, width, height);
    HGDIOBJ pImageOldBmp = ::SelectObject(imageDC, (HGDIOBJ)imageBmp);
    pDrawImage->Stretch(imageDC, CRect(0, 0, width, height));

    HDC maskDC = ::CreateCompatibleDC(hDC);
    HBITMAP maskBmp = ::CreateBitmap(width, height, 1, 1, NULL);
    HGDIOBJ pOldMaskBmp = ::SelectObject(maskDC, (HGDIOBJ)maskBmp);

    ::SetBkColor(imageDC, transColor);
    ::BitBlt(maskDC, 0, 0, width, height, imageDC, 0, 0, SRCCOPY);
    ::BitBlt(hDC, left, top, width, height, imageDC, 0, 0, SRCINVERT);
    ::BitBlt(hDC, left, top, width, height, maskDC, 0, 0, SRCAND);
    ::BitBlt(hDC, left, top, width, height, imageDC, 0, 0, SRCINVERT);

    ::SelectObject(imageDC, pImageOldBmp);
    ::SelectObject(maskDC, pOldMaskBmp);

    ::DeleteObject(imageBmp);
    ::DeleteObject(maskBmp);

    ::DeleteDC(imageDC);
    ::DeleteDC(maskDC);

    ::SetBkColor(hDC, oldBkColor);
    ::SetTextColor(hDC, oldTextColor);

    return TRUE;
#endif //WINVER >= 0x0500
}

BOOL CImageBaseImplCxImage::DrawAlphaCxImageHelper(HDC hDC, LPCRECT lpDstRect, CxImage* pDrawImage, LPRECT lpClipRect)
{
    if (! pDrawImage->AlphaIsValid())
        return FALSE;

    //使用CxImage绘制
	//DragonZ:支持SkinImage画指定区域图片
	CRect clipRect = *lpClipRect;
	BOOL ret;
	if (clipRect.IsRectEmpty())
	{
		ret = pDrawImage->Draw(hDC, *lpDstRect);
	}else
	{
		ret = pDrawImage->Draw(hDC, *lpDstRect, lpClipRect);
	}
	/////////////////
    ASSERT(ret);
    return ret;

    //使用Gdiplus绘制
    //创建IStream
    CComPtr<IStream> pStream = NULL;
    ::CreateStreamOnHGlobal(NULL, TRUE, &pStream);
    if (pStream == NULL)
        return FALSE;

    //转换
    ULARGE_INTEGER size;
    size.QuadPart = pDrawImage->GetSize();
    pStream->SetSize(size);
    CxIStreamFile xFile(pStream);
    pDrawImage->Encode(&xFile, CXIMAGE_SUPPORT_BMP);
    Gdiplus::Bitmap bitmap(pStream);

    //绘制
    Gdiplus::Graphics graphic(hDC);
    Gdiplus::Status status = graphic.DrawImage(&bitmap, lpDstRect->left, lpDstRect->top, 
        (lpDstRect->right - lpDstRect->left), (lpDstRect->bottom - lpDstRect->top));

    return (status == Gdiplus::Ok);
}

BOOL CImageBaseImplCxImage::DrawOriginCxImageHelper(HDC hDC, LPCRECT lpDstRect, CxImage* pDrawImage, LPRECT lpClipRect)
{
    BOOL ret = pDrawImage->Stretch(hDC, *lpDstRect);
    ASSERT(ret);
    return ret;
}

BOOL CImageBaseImplCxImage::DrawCxImageHelper(HDC hDC, LPCRECT lpDstRect, CxImage* pDrawImage, LPRECT lpClipRect, LONG clrTransparent, BOOL bAlpha)
{
    if (bAlpha && DrawAlphaCxImageHelper(hDC, lpDstRect, pDrawImage, lpClipRect))
        return TRUE;

    if (clrTransparent != -1 && DrawTransparentCxImageHelper(hDC, lpDstRect, pDrawImage, lpClipRect, clrTransparent))
        return TRUE;

    return DrawOriginCxImageHelper(hDC, lpDstRect, pDrawImage, lpClipRect);
}

BOOL CImageBaseImplCxImage::Draw(HDC hDC, LPCRECT lpDstRect, LPRECT lpClipRect, const CImageTransformParam* pParam, 
                                 LONG clrTransparent, BOOL bAlpha,
                                 BOOL bSideStretch, LONG sideWidth, LONG sideHeight)
{
	ASSERT(m_pOriCxImage != NULL && lpDstRect != NULL);
	if (m_pOriCxImage == NULL || lpDstRect == NULL)
		return FALSE;

    CRect destRect = *lpDstRect;
    if (destRect.IsRectEmpty())
        return FALSE;

	CxImage* pDrawImage = NULL;
	if (pParam == NULL){
		pDrawImage = m_pOriCxImage;
	} else {
		BOOL bRet = Transform(pParam);
		ASSERT(bRet);
		pDrawImage = m_pTransformedCxImage;
		if (pDrawImage == NULL)
			pDrawImage = m_pOriCxImage;
	}

	ASSERT(pDrawImage != NULL);
	if (pDrawImage == NULL)
		return FALSE;

    if (bSideStretch) {
        return DrawCxImageHelper(hDC, lpDstRect, pDrawImage, lpClipRect, clrTransparent, bAlpha);
    } else {
        CRect srcRect;
        CRect dstRect;
        CSize srcSize = CSize(pDrawImage->GetWidth(), pDrawImage->GetHeight());
        CSize dstSize = CSize(lpDstRect->right - lpDstRect->left, lpDstRect->bottom - lpDstRect->top);
        CSize szOffset = CSize(lpDstRect->left, lpDstRect->top);
        CxImage tmpImg;

        if (sideHeight*2 > dstSize.cy || sideWidth*2 > dstSize.cx) {
            return DrawCxImageHelper(hDC, lpDstRect, pDrawImage, lpClipRect, clrTransparent, bAlpha);
        }

        LONG srcTop = 0;
        LONG srcBottom = sideHeight;
        LONG dstTop = 0;
        LONG dstBottom = sideHeight;

        //上左
        srcRect = CRect(0, srcTop, sideWidth, srcBottom);
        dstRect = CRect(0, dstTop, sideWidth, dstBottom);
        dstRect.OffsetRect(szOffset);
        pDrawImage->Crop(srcRect, &tmpImg);
        DrawCxImageHelper(hDC, &dstRect, &tmpImg, NULL, clrTransparent, bAlpha);
        //上中
        srcRect = CRect(sideWidth, srcTop, (srcSize.cx - sideWidth), srcBottom);
        dstRect = CRect(sideWidth, dstTop, (dstSize.cx - sideWidth), dstBottom);
        dstRect.OffsetRect(szOffset);
        pDrawImage->Crop(srcRect, &tmpImg);
        DrawCxImageHelper(hDC, &dstRect, &tmpImg, NULL, clrTransparent, bAlpha);
        //上右
        srcRect = CRect((srcSize.cx - sideWidth), srcTop, srcSize.cx, srcBottom);
        dstRect = CRect((dstSize.cx - sideWidth), dstTop, dstSize.cx, dstBottom);
        dstRect.OffsetRect(szOffset);
        pDrawImage->Crop(srcRect, &tmpImg);
        DrawCxImageHelper(hDC, &dstRect, &tmpImg, NULL, clrTransparent, bAlpha);


        srcTop = sideHeight;
        srcBottom = srcSize.cy - sideHeight;
        dstTop = sideHeight;
        dstBottom = dstSize.cy - sideHeight;

        //中左
        srcRect = CRect(0, srcTop, sideWidth, srcBottom);
        dstRect = CRect(0, dstTop, sideWidth, dstBottom);
        dstRect.OffsetRect(szOffset);
        pDrawImage->Crop(srcRect, &tmpImg);
        DrawCxImageHelper(hDC, &dstRect, &tmpImg, NULL, clrTransparent, bAlpha);
        //中中
        srcRect = CRect(sideWidth, srcTop, (srcSize.cx - sideWidth), srcBottom);
        dstRect = CRect(sideWidth, dstTop, (dstSize.cx - sideWidth), dstBottom);
        dstRect.OffsetRect(szOffset);
        pDrawImage->Crop(srcRect, &tmpImg);
        DrawCxImageHelper(hDC, &dstRect, &tmpImg, NULL, clrTransparent, bAlpha);
        //中右
        srcRect = CRect((srcSize.cx - sideWidth), srcTop, srcSize.cx, srcBottom);
        dstRect = CRect((dstSize.cx - sideWidth), dstTop, dstSize.cx, dstBottom);
        dstRect.OffsetRect(szOffset);
        pDrawImage->Crop(srcRect, &tmpImg);
        DrawCxImageHelper(hDC, &dstRect, &tmpImg, NULL, clrTransparent, bAlpha);


        srcTop = srcSize.cy - sideHeight;
        srcBottom = srcSize.cy;
        dstTop = dstSize.cy - sideHeight;
        dstBottom = dstSize.cy;

        //下左
        srcRect = CRect(0, srcTop, sideWidth, srcBottom);
        dstRect = CRect(0, dstTop, sideWidth, dstBottom);
        dstRect.OffsetRect(szOffset);
        pDrawImage->Crop(srcRect, &tmpImg);
        DrawCxImageHelper(hDC, &dstRect, &tmpImg, NULL, clrTransparent, bAlpha);
        //下中
        srcRect = CRect(sideWidth, srcTop, (srcSize.cx - sideWidth), srcBottom);
        dstRect = CRect(sideWidth, dstTop, (dstSize.cx - sideWidth), dstBottom);
        dstRect.OffsetRect(szOffset);
        pDrawImage->Crop(srcRect, &tmpImg);
        DrawCxImageHelper(hDC, &dstRect, &tmpImg, NULL, clrTransparent, bAlpha);
        //下右
        srcRect = CRect((srcSize.cx - sideWidth), srcTop, srcSize.cx, srcBottom);
        dstRect = CRect((dstSize.cx - sideWidth), dstTop, dstSize.cx, dstBottom);
        dstRect.OffsetRect(szOffset);
        pDrawImage->Crop(srcRect, &tmpImg);
        DrawCxImageHelper(hDC, &dstRect, &tmpImg, NULL, clrTransparent, bAlpha);

        return TRUE;
    }
}

LONG CImageBaseImplCxImage::Width()
{
	ASSERT(m_pOriCxImage != NULL);
	if (m_pOriCxImage == NULL)
		return -1;

	return m_pOriCxImage->GetWidth();
}

LONG CImageBaseImplCxImage::Height()
{
	ASSERT(m_pOriCxImage != NULL);
	if (m_pOriCxImage == NULL)
		return -1;

	return m_pOriCxImage->GetHeight();
}

BOOL CImageBaseImplCxImage::GetSize(LONG& lSize)
{
	lSize = sizeof(CImageBaseImplCxImage);
	if (m_pOriCxImage != NULL) {
		lSize += m_pOriCxImage->GetSize();
	}
	if (m_pTransformedCxImage != NULL) {
		lSize += m_pTransformedCxImage->GetSize();
	}
	return TRUE;
}

LONG CImageBaseImplCxImage::GetNumFrames()
{
#ifdef USE_GDIPLUS_ANIMATION
	if (m_pAnimationImage == NULL)
		return 0;

	UINT dimensionsCount = m_pAnimationImage->GetFrameDimensionsCount();
	GUID* pGUIDS = new GUID[dimensionsCount];
	m_pAnimationImage->GetFrameDimensionsList(pGUIDS, dimensionsCount);
	UINT nCount = m_pAnimationImage->GetFrameCount(pGUIDS);
	delete [] pGUIDS;
	return nCount;
#else
	if (m_pAnimationCxImage == NULL)
		return 0;

	return m_pAnimationCxImage->GetNumFrames();
#endif
}

BOOL CImageBaseImplCxImage::GetAnimationInfo(tagANIMATEINFO& animateInfo)
{
#ifdef USE_GDIPLUS_ANIMATION
	animateInfo.bkColorIndex = -1;
	animateInfo.bkColor = 0xFFFFFFFF;

	if (m_pAnimationImage == NULL)
		return FALSE;

	return TRUE;
#else
	animateInfo.bkColorIndex = -1;
    animateInfo.bkColor = 0xFFFFFFFF;

	if (m_pAnimationCxImage == NULL)
		return FALSE;

	animateInfo.bkColorIndex = m_pAnimationCxImage->GetTransIndex();
	animateInfo.bkColor = m_pAnimationCxImage->GetTransColor();
	return TRUE;
#endif
}

BOOL CImageBaseImplCxImage::GetFrameInfo(LONG iFrame, tagFRAMEINFO& frameInfo)
{
#ifdef USE_GDIPLUS_ANIMATION
	frameInfo.delayTime = 0;
	frameInfo.disposalMethod = 2;
	frameInfo.bkColorIndex = -1;
	frameInfo.bkColor = 0xFFFFFFFF;

    if (m_pAnimationImage == NULL)
		return FALSE;

	ASSERT(iFrame >= 0 && iFrame < GetNumFrames());
	if (iFrame < 0 || iFrame >= GetNumFrames())
		return FALSE;

	//获得帧时间延迟
	UINT uSize = m_pAnimationImage->GetPropertyItemSize(PropertyTagFrameDelay);
    if (uSize <= 0)
        return FALSE;

    try  {
        std::vector<BYTE> vecPropBuffer(uSize, 0);
        Gdiplus::PropertyItem* pPropertyItem = (Gdiplus::PropertyItem*)(&vecPropBuffer[0]);
        m_pAnimationImage->GetPropertyItem(PropertyTagFrameDelay, uSize, pPropertyItem);
        frameInfo.delayTime = ((int*)pPropertyItem->value)[iFrame] * 10;
    } catch (...) {
        return FALSE;
    }

    if (frameInfo.delayTime < 100)
        frameInfo.delayTime = 100;

	return TRUE;
#else

	if (m_pAnimationCxImage == NULL)
		return FALSE;

	LONG numFrames = m_pAnimationCxImage->GetNumFrames();
	ASSERT(iFrame >= 0 && iFrame < numFrames);
	if (iFrame < 0 || iFrame >= numFrames)
		return FALSE;

	CxImage *pFrame = m_pAnimationCxImage->GetFrame(iFrame);
	ASSERT(pFrame != NULL);
	if (pFrame == NULL)
		return FALSE;

	frameInfo.delayTime = pFrame->GetFrameDelay() * 10;
	frameInfo.disposalMethod = pFrame->GetDisposalMethod();
	frameInfo.bkColorIndex = pFrame->GetTransIndex();
    RGBQUAD rgb = pFrame->GetTransColor();
	frameInfo.bkColor = RGB(rgb.rgbRed, rgb.rgbGreen, rgb.rgbBlue);
	return TRUE;
#endif
}

BOOL CImageBaseImplCxImage::DrawFrame(HDC hDC, LONG iFrame, const RECT& dstRect)
{
#ifdef USE_GDIPLUS_ANIMATION
	if (m_pAnimationImage == NULL)
		return FALSE;

    try {
        GUID guidGif = Gdiplus::FrameDimensionTime;
        m_pAnimationImage->SelectActiveFrame(&guidGif, iFrame);

        Gdiplus::Rect rcDraw(dstRect.left, dstRect.top, (dstRect.right - dstRect.left), (dstRect.bottom - dstRect.top));
        Gdiplus::Graphics graphics(hDC);
        Gdiplus::Status ret = graphics.DrawImage(m_pAnimationImage, rcDraw);

        return (ret == Gdiplus::Ok);
    } catch (...) {
        return FALSE;
    }
#else
	if (m_pAnimationCxImage == NULL)
		return FALSE;

	LONG numFrames = m_pAnimationCxImage->GetNumFrames();
	ASSERT(iFrame >= 0 && iFrame < numFrames);
	if (iFrame < 0 || iFrame >= numFrames)
		return FALSE;

	CxImage *pFrame = m_pAnimationCxImage->GetFrame(iFrame);
	ASSERT(pFrame != NULL);
	if (pFrame == NULL)
		return FALSE;

	//目标区域的大小
	LONG dstWidth = dstRect.right - dstRect.left;
	LONG dstHeight = dstRect.bottom - dstRect.top;
	//Gif图片的原始大小
	LONG oriWidth = m_pAnimationCxImage->GetWidth();
	LONG oriHeight = m_pAnimationCxImage->GetHeight();
	//获得帧偏移和帧大小
	LONG frameOffsetX = 0;
	LONG frameOffsetY = 0;
	pFrame->GetOffset(&frameOffsetX, &frameOffsetY);
	LONG frameWidth = pFrame->GetWidth();
	LONG frameHeight = pFrame->GetHeight();
	//根据缩放比例计算帧的绘制区域
	CRect frameDestRect = dstRect;
	frameDestRect.left += frameOffsetX * dstWidth / oriWidth;
	frameDestRect.top += frameOffsetY * dstHeight / oriHeight;
	frameDestRect.right = frameDestRect.left + frameWidth * dstWidth / oriWidth;
	frameDestRect.bottom = frameDestRect.top + frameHeight * dstHeight / oriHeight;

	return pFrame->Draw(hDC, frameDestRect);
#endif
}

#endif //CXIMAGE_SUPPORT