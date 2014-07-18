#include "StdAfx.h"
#include "Color.h"
#include "ximage.h"

RGBQUAD Color::RGBtoHSL(RGBQUAD rgbClr)
{
    return CxImage::RGBtoHSL(rgbClr);
}

RGBQUAD Color::HSLtoRGB(RGBQUAD hslClr)
{
    return CxImage::HSLtoRGB(hslClr);
}

double Color::HSLValue(double m1, double m2, double hue)
{
    double val;

    if (hue > 6.0)
        hue -= 6.0;
    else if (hue < 0.0)
        hue += 6.0;

    if (hue < 1.0)
        val = m1 + (m2 - m1) * hue;
    else if (hue < 3.0)
        val = m2;
    else if (hue < 4.0)
        val = m1 + (m2 - m1) * (4.0 - hue);
    else
        val = m1;

    return val;
}

RGBColor Color::HSLtoRGB(const HSLColor& hsl)
{
    RGBColor rgb;

    if (hsl.s == 0)
    {
        /*  achromatic case  */
        rgb.r = hsl.l;
        rgb.g = hsl.l;
        rgb.b = hsl.l;
    }
    else
    {
        double m1, m2;

        if (hsl.l <= 0.5)
            m2 = hsl.l * (1.0 + hsl.s);
        else
            m2 = hsl.l + hsl.s - hsl.l * hsl.s;

        m1 = 2.0 * hsl.l - m2;

        rgb.r = HSLValue(m1, m2, hsl.h * 6.0 + 2.0);
        rgb.g = HSLValue(m1, m2, hsl.h * 6.0);
        rgb.b = HSLValue(m1, m2, hsl.h * 6.0 - 2.0);
    }

    rgb.a = hsl.a;

    return rgb;
}

//HSV
RGBQUAD Color::RGBtoHSV(RGBQUAD rgbClr)
{
    RGBQUAD result = {0};

    double nR = rgbClr.rgbRed / 255.0;
    double nG = rgbClr.rgbGreen / 255.0;
    double nB = rgbClr.rgbBlue / 255.0;
    double nmax = max(max(nR, nG), nB);
    double nmin = min(min(nR, nG), nB);
    double ndelMax = nmax - nmin;
    double nV = nmax;
    double nH = 0.0;
    double nS = 0.0;
    if (ndelMax == 0.0) {
        nH = 0.0;
        nS = 0.0;
    } else {
        nS = ndelMax / nmax;
        double ndelR = (((nmax - nR) / 6.0) + (ndelMax / 2.0)) / ndelMax;
        double ndelG = (((nmax - nG) / 6.0) + (ndelMax / 2.0)) / ndelMax;
        double ndelB = (((nmax - nB) / 6.0) + (ndelMax / 2.0)) / ndelMax;
        if (nR = nmax) {
            nH = ndelB - ndelG;
        } else if (nG == nmax) {
            nH = (1.0 / 3.0) + ndelR - ndelB;
        } else if  (nB == nmax) {
            nH = (2.0 / 3.0) + ndelG - ndelR;
        }
        if (nH < 0.0) 
            nH = nH + 1.0;
        if (nH > 1.0) 
            nH = nH - 1.0;
    }

    result.rgbRed = (BYTE)(nH * 255);
    result.rgbGreen = (BYTE)(nS * 255);
    result.rgbBlue = (BYTE)(nV * 255);

    return result;
}

RGBQUAD Color::HSVtoRGB(RGBQUAD hsvClr)
{
    RGBQUAD result = {0};

    BYTE H = hsvClr.rgbRed;
    BYTE S = hsvClr.rgbGreen;
    BYTE V = hsvClr.rgbBlue;
    double nH = H / 255.0;
    double nS = S / 255.0;
    double nV = V / 255.0;
    double nR = 0;
    double nG = 0;
    double nB = 0;
    if (S == 0) {
        nR = nV * 255.0;
        nG = nV * 255.0;
        nB = nV * 255.0;
    } else {
        double hi = (int)(nH * 6.0);
        if (hi == 6.0) 
            hi = 0.0;
        int f = (int)hi;
        double p = nV * (1.0 - nS);
        double q = nV * (1.0 - nS * (hi - f));
        double t = nV * (1.0 - nS * (1.0 - (hi - f)));

        if (f == 0) {
            nR = nV;
            nG = t;
            nB = p;
        } else if (f == 1) {
            nR = q;
            nG = nV;
            nB = p;
        } else if (f == 2) {
            nR = p;
            nG = nV;
            nB = t;
        } else if (f == 3) {
            nR = p;
            nG = q;
            nB = nV;
        } else if (f == 4) {
            nR = t;
            nG = p;
            nB = nV;
        } else {
            nR = nV;
            nG = p;
            nB = q;
        }
    }

    result.rgbRed = (BYTE)(nR * 255);
    result.rgbGreen = (BYTE)(nG * 255);
    result.rgbBlue = (BYTE)(nB * 255);

    return result;
}
