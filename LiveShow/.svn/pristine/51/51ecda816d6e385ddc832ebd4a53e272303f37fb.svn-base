#include "StdAfx.h"

#ifdef GDIPLUS_BITMAP_SUPPORT

#include "ImageBaseImplBitmap.h"

using namespace Gdiplus;

CImageBaseImplBitmap::CImageBaseImplBitmap(void)
{
	m_pOriBitmap = NULL;
	m_pTransformedBitmap = NULL;
}

CImageBaseImplBitmap::~CImageBaseImplBitmap(void)
{
	if (m_pOriBitmap != NULL){
		delete m_pOriBitmap;
		m_pOriBitmap = NULL;
	}

	if (m_pTransformedBitmap != NULL) {
		delete m_pTransformedBitmap;
		m_pTransformedBitmap = NULL;
	}
}

BOOL CImageBaseImplBitmap::LoadFromFile(LPCTSTR filePathName)
{
	//释放原来的
	if (m_pOriBitmap != NULL){
		delete m_pOriBitmap;
		m_pOriBitmap = NULL;
	}

	if (m_pTransformedBitmap != NULL) {
		delete m_pTransformedBitmap;
		m_pTransformedBitmap = NULL;
	}

	//载入新的文件
	CT2W wFilePathName(filePathName);
	m_pOriBitmap = Bitmap::FromFile(wFilePathName);
	if (m_pOriBitmap == NULL)
		return FALSE;

	return TRUE;
}

BOOL CImageBaseImplBitmap::LoadFromData(const char* pzData, long size)
{
	//释放原来的
	return FALSE;
}

BOOL CImageBaseImplBitmap::Transform(const CImageTransformParam* pParam)
{
	ASSERT(pParam != NULL && m_pOriBitmap != NULL);
	if (pParam == NULL || m_pOriBitmap == NULL)
		return FALSE;

	if (m_transformParam == *pParam)
		return TRUE;

	//删除变换图
	if (m_pTransformedBitmap != NULL){
		delete m_pTransformedBitmap;
		m_pTransformedBitmap = NULL;
	}

	//拷贝原始图像
	Rect rc(0, 0, m_pOriBitmap->GetWidth(), m_pOriBitmap->GetHeight());
	m_pTransformedBitmap = m_pOriBitmap->Clone(rc, PixelFormatDontCare);

	BOOL bSuc = FALSE;
	switch (pParam->enTransformType)
	{
	case TRANSFORM_TYPE_NONE:
		bSuc = TRUE;
		break;
	case TRANSFORM_TYPE_SMOOTH:
		{
			bSuc = TRUE;//m_pTransformedCxImage->Resample(pParam->width, pParam->height, 0);
		}
		break;
	case TRANSFORM_TYPE_COLORIZE:
		{
			bSuc = FALSE;
#ifdef GDIPVER
#if GDIPVER >= 0x0110
			HueSaturationLightness hsl;
			HueSaturationLightnessParams hslParam;
			int temp = pParam->hue;
			hslParam.hueLevel = (temp - 128) * 360 / 256;
			temp = pParam->sat;
			hslParam.saturationLevel = (temp - 128) * 200 / 256;
			hslParam.lightnessLevel = (INT)((pParam->blend - 0.5) * 200);
			hsl.SetParameters(&hslParam);
			bSuc = (m_pTransformedBitmap->ApplyEffect(&hsl, NULL) == Ok);
#endif
#endif
		}
		break;
	case TRANSFORM_TYPE_SHIFTRGB:
		bSuc = FALSE;
		break;
	}

	if (bSuc) {
		//拷贝变换参数
		m_transformParam = *pParam;
	} else {
		delete m_pTransformedBitmap;
		m_pTransformedBitmap = m_pOriBitmap->Clone(rc, PixelFormatDontCare);
		m_transformParam.enTransformType = TRANSFORM_TYPE_NONE;
	}

	return bSuc;
}

BOOL CImageBaseImplBitmap::TransformColor(const CImageTransformParam* pParam, LONG color, LONG& transformColor)
{
	transformColor = color;

	if (color == -1)
		return TRUE;

	if (pParam->enTransformType != TRANSFORM_TYPE_COLORIZE)
		return FALSE;

	BOOL bSuc = FALSE;

#ifdef GDIPVER
#if GDIPVER >= 0x0110
	static Bitmap bitmap(1, 1, PixelFormat24bppRGB);
	Color clr(GetRValue(color), GetGValue(color), GetBValue(color));
	bitmap.SetPixel(0, 0, clr);
	HueSaturationLightness hsl;
	HueSaturationLightnessParams hslParam;
	int temp = pParam->hue;
	hslParam.hueLevel = (temp - 128) * 360 / 256;
	temp = pParam->sat;
	hslParam.saturationLevel = (temp - 128) * 200 / 256;
	hslParam.lightnessLevel = (INT)((pParam->blend - 0.5) * 200);
	hsl.SetParameters(&hslParam);
	bSuc = (bitmap.ApplyEffect(&hsl, NULL) == Ok);
	if (bSuc) {
		bitmap.GetPixel(0, 0, &clr);
		transformColor = RGB(clr.GetR(), clr.GetG(), clr.GetB());
	}
#endif
#endif

	return bSuc;
}

BOOL CImageBaseImplBitmap::Draw(HDC hDC, LPCRECT lpDstRect, LPRECT lpClipRect, const CImageTransformParam* pParam, 
                                LONG clrTransparent, BOOL bAlpha, 
                                BOOL bSideStretch, LONG sideWidth, LONG sideHeight)
{
	Bitmap* pDrawImage = NULL;
	if (pParam == NULL){
		pDrawImage = m_pOriBitmap;
	} else {
		BOOL bRet = Transform(pParam);
		ASSERT(bRet);
		pDrawImage = m_pTransformedBitmap;
		if (pDrawImage == NULL)
			pDrawImage = m_pOriBitmap;
	}

	ASSERT(pDrawImage != NULL);
	if (pDrawImage == NULL)
		return FALSE;

	Graphics graphic(hDC);
	Status status = graphic.DrawImage(pDrawImage, lpDstRect->left, lpDstRect->top, 
		(lpDstRect->right - lpDstRect->left), (lpDstRect->bottom - lpDstRect->top));

	return (status == Ok);
}

LONG CImageBaseImplBitmap::Width()
{
	ASSERT(m_pOriBitmap != NULL);
	if (m_pOriBitmap == NULL)
		return -1;

	return m_pOriBitmap->GetWidth();
}

LONG CImageBaseImplBitmap::Height()
{
	ASSERT(m_pOriBitmap != NULL);
	if (m_pOriBitmap == NULL)
		return -1;

	return m_pOriBitmap->GetHeight();
}

#endif //GDIPLUS_BITMAP_SUPPORT
