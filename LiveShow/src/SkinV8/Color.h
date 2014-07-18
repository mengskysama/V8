#pragma once

#define RGB_LUMINANCE_RED    (0.2126)
#define RGB_LUMINANCE_GREEN  (0.7152)
#define RGB_LUMINANCE_BLUE   (0.0722)

#define RGB_LUMINANCE(r,g,b) ((r) * RGB_LUMINANCE_RED   + \
    (g) * RGB_LUMINANCE_GREEN + \
    (b) * RGB_LUMINANCE_BLUE))

struct HSLColor
{
    double h;
    double s;
    double l;
    double a;
};

struct RGBColor 
{
    double r;
    double g;
    double b;
    double a;
};

class Color
{
public:
    //HSL
    static RGBQUAD RGBtoHSL(RGBQUAD rgbClr);
    static RGBQUAD HSLtoRGB(RGBQUAD hslClr);
    static double HSLValue(double m1, double m2, double hue);
    static RGBColor HSLtoRGB(const HSLColor& hslClr);

    //HSV
    static RGBQUAD RGBtoHSV(RGBQUAD rgbClr);
    static RGBQUAD HSVtoRGB(RGBQUAD hsvClr);
};
