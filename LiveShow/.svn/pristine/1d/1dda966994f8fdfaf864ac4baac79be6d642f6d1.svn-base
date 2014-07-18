#pragma once

#include "../Common.h"
#include "ximage.h"

#define DEF_R	143
#define DEF_G	172
#define DEF_B	219

namespace common { namespace ui
{

/**
@fn double HuetoRGB(double m1, double m2, double h )
@brief Hue色调转RGB
@param [in] m1 m1
@param [in] m2 m2
@param [in] h
@return 转换后的值
*/
double COMMON_INTERFACE HuetoRGB(double m1, double m2, double h);

/**
@fn COLORREF HSLtoRGB( double H, double S, double L)
@brief HSL颜色转RGB空间
@param [in] H Hue
@param [in] S Saturation
@param [in] L Lightness
@return RGB值
*/
COLORREF COMMON_INTERFACE HSLtoRGB(double H, double L, double S);

/**
@fn void RGBtoHSL(COLORREF rgb, double *H, double *S, double *L)
@brief RGB空间颜色转HSL空间颜色
@param [in] rgb RGB颜色
@param [out] H Hue
@param [out] S Saturation
@param [out] L Lightness
*/
void COMMON_INTERFACE RGBtoHSL(COLORREF rgb, double *H, double *S, double *L);

/**
@fn void Colorize(BYTE hue, BYTE sat, float blend, RGBQUAD& src, RGBQUAD& dst)
@brief 调整色调，用于皮肤变色
@param [in] hue 色调调整
@param [in] sat 饱和度调整
@param [in] blend 亮度调整
@param [in] src 调整前RGB颜色
@param [out] dst 调整后RGB颜色
*/
void COMMON_INTERFACE Colorize(BYTE hue, BYTE sat, float blend, RGBQUAD& src, RGBQUAD& dst);

/**
@fn void Colorize(BYTE hue, BYTE sat, float blend, RGBQUAD& src, RGBQUAD& dst)
@brief 调整图片色调，用于皮肤变色
@param [in] pImage	需要调整的图片
@param [in] hue 色调调整
@param [in] sat 饱和度调整
@param [in] blend 亮度调整
@param [in] src 调整前RGB颜色
@param [out] dst 调整后RGB颜色
*/
HBITMAP COMMON_INTERFACE Colorize(CxImage* pImage, BYTE bHue, BYTE bSat, float bBlend);

} } //namespace common::ui
