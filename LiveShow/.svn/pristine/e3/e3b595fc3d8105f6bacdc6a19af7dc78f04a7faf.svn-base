#include "StdAfx.h"
#include "Color.h"
#include "ColorAdjuster.h"

CColorAdjuster::CColorAdjuster(void)
{
    m_hue = 0.0;
    m_saturation = 0.0;
    m_lightness = 0.0;

    Init(180.0, 50.0, 0.0);
}

CColorAdjuster::~CColorAdjuster(void)
{
}

void CColorAdjuster::Init(double hue, double sat, double light)
{
    if (m_hue == hue && m_saturation == sat && m_lightness == light)
        return;

    m_hue = hue;
    m_saturation = sat;
    m_lightness = light;

    HSLColor hsl;
    hsl.h = m_hue / 360.0;
    hsl.s = m_saturation / 100.0;
    RGBColor rgb;
    for (int i=0; i<256; i++) {

        //初始化颜色权值查找表
        m_lumRedLookup[i] = (int)(i * RGB_LUMINANCE_RED);
        m_lumGreenLookup[i] = (int)(i * RGB_LUMINANCE_GREEN);
        m_lumBlueLookup[i] = (int)(i * RGB_LUMINANCE_BLUE);

        //初始化输出颜色查找表
        hsl.l = i / 255.0;
        rgb = Color::HSLtoRGB(hsl);
        m_finalRedLookup[i] = (int)(255.0 * rgb.r);
        m_finalGreenLookup[i] = (int)(255.0 * rgb.g);
        m_finalBlueLookup[i] = (int)(255.0 * rgb.b);

    }
}

RGBQUAD CColorAdjuster::Adjust(RGBQUAD color)
{
    RGBQUAD result;

    int lum = m_lumRedLookup[color.rgbRed] + m_lumGreenLookup[color.rgbGreen] + m_lumBlueLookup[color.rgbBlue];
    if (m_lightness > 0) {
        lum = (int) ((double) lum * (100.0 - m_lightness) / 100.0);
        lum += (int) (255 - (100.0 - m_lightness) * 255.0 / 100.0);
    } else if (m_lightness < 0) {
        lum = (int) ((double) lum * (m_lightness + 100.0) / 100.0);
    }

    ASSERT(lum >= 0 && lum < 256);
    result.rgbRed = m_finalRedLookup[lum];
    result.rgbGreen = m_finalGreenLookup[lum];
    result.rgbBlue = m_finalBlueLookup[lum];
    result.rgbReserved = color.rgbReserved;

    return result;
}


CColorGradationAdjuster::CColorGradationAdjuster(void)
{
    for (int i=0; i<256; i++) {
        m_RedLookup[i] = i;
        m_GreenLookup[i] = i;
        m_BlueLookup[i] = i;
    }
}

CColorGradationAdjuster::~CColorGradationAdjuster(void)
{
    
}

void CColorGradationAdjuster::Init(BYTE minInput, BYTE maxInput, BYTE minOutput, BYTE maxOutput)
{
    ASSERT(maxInput >= minInput);
    ASSERT(maxOutput >= minOutput);

    for (int i=0; i<256; i++) {
        m_RedLookup[i] = i;
        m_GreenLookup[i] = i;
        m_BlueLookup[i] = i;
    }

    for (int i=0; i<minInput; i++) {
        m_RedLookup[i] = minOutput;
        m_GreenLookup[i] = minOutput;
        m_BlueLookup[i] = minOutput;
    }
    for (int i=minInput; i<maxInput; i++) {
        double output = (double)minOutput + floor(
            (double)(i - minInput) / (double)(maxInput - minInput) * (double)(maxOutput - minOutput) + 0.5);
        BYTE byteOutput = (BYTE)output;
        m_RedLookup[i] = byteOutput;
        m_GreenLookup[i] = byteOutput;
        m_BlueLookup[i] = byteOutput;
    }
    for (int i=maxInput; i<256; i++) {
        m_RedLookup[i] = maxOutput;
        m_GreenLookup[i] = maxOutput;
        m_BlueLookup[i] = maxOutput;
    }
}

RGBQUAD CColorGradationAdjuster::Adjust(RGBQUAD color)
{
    RGBQUAD result = color;

    result.rgbRed = (BYTE)m_RedLookup[result.rgbRed];
    result.rgbGreen = (BYTE)m_GreenLookup[result.rgbGreen];
    result.rgbBlue = (BYTE)m_BlueLookup[result.rgbBlue];

    return result;
}
