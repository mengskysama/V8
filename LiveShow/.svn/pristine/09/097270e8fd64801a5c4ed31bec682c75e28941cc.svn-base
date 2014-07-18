#include "../../StdAfx.h"
#include "../../include/ui/ColorUtils.h"

#define RGB2GRAY(r,g,b) (((b)*117 + (g)*601 + (r)*306) >> 10)
#define  HSLMAX   255
#define  RGBMAX   255
#define HSLUNDEFINED (HSLMAX*2/3)

namespace common { namespace ui
{

double HuetoRGB(double m1, double m2, double h )
{
	if( h < 0 ) h += 1.0;
	if( h > 1 ) h -= 1.0;
	if( 6.0*h < 1 )
		return (m1+(m2-m1)*h*6.0);
	if( 2.0*h < 1 )
		return m2;
	if( 3.0*h < 2.0 )
		return (m1+(m2-m1)*((2.0/3.0)-h)*6.0);
	return m1;
}

COLORREF HSLtoRGB( double H, double S, double L )
{
	double r, g, b;
	double m1, m2;

	if(S == 0) 
	{
		r = g = b = L;
	} 
	else 
	{
		if(L <= 0.5)
			m2 = L*(1.0+S);
		else
			m2 = L+S-L*S;

		m1 = 2.0*L-m2;
		r = HuetoRGB(m1, m2, H+1.0/3.0);
		g = HuetoRGB(m1, m2, H);
		b = HuetoRGB(m1, m2, H-1.0/3.0);
	}
	return RGB((BYTE)(r*255), (BYTE)(g*255), (BYTE)(b*255));
}

void RGBtoHSL( COLORREF rgb, double *H, double *S, double *L )
{   
	double delta;
	double r = (double)GetRValue(rgb)/255;
	double g = (double)GetGValue(rgb)/255;
	double b = (double)GetBValue(rgb)/255;   
	double cmax = max(r, max(g, b));
	double cmin = min(r, min(g, b));   
	*L=(cmax+cmin)/2.0;   

	if(cmax==cmin) 
	{
		*S = 0;      
		*H = 0; // it's really undefined   
	} 
	else 
	{
		if(*L < 0.5) 
			*S = (cmax-cmin)/(cmax+cmin);      
		else
			*S = (cmax-cmin)/(2.0-cmax-cmin);      

		delta = cmax - cmin;
		if(r==cmax) 
			*H = (g-b)/delta;      
		else if(g==cmax)
			*H = 2.0 +(b-r)/delta;
		else          
			*H=4.0+(r-g)/delta;
		*H /= 6.0; 
		if(*H < 0.0)
			*H += 1;  
	}
}

float HueToRGB2(float n1,float n2, float hue)
{
	//<F. Livraghi> fixed implementation for HSL2RGB routine
	float rValue;

	if (hue > 360)
		hue = hue - 360;
	else if (hue < 0)
		hue = hue + 360;

	if (hue < 60)
		rValue = n1 + (n2-n1)*hue/60.0f;
	else if (hue < 180)
		rValue = n2;
	else if (hue < 240)
		rValue = n1+(n2-n1)*(240-hue)/60;
	else
		rValue = n1;

	return rValue;
}

RGBQUAD HSLtoRGB2(RGBQUAD lHSLColor)
{ 
	//<F. Livraghi> fixed implementation for HSL2RGB routine
	float h,s,l;
	float m1,m2;
	BYTE r,g,b;

	h = (float)lHSLColor.rgbRed * 360.0f/255.0f;
	s = (float)lHSLColor.rgbGreen/255.0f;
	l = (float)lHSLColor.rgbBlue/255.0f;

	if (l <= 0.5)	m2 = l * (1+s);
	else			m2 = l + s - l*s;

	m1 = 2 * l - m2;

	if (s == 0) {
		r=g=b=(BYTE)(l*255.0f);
	} else {
		r = (BYTE)(HueToRGB2(m1,m2,h+120) * 255.0f);
		g = (BYTE)(HueToRGB2(m1,m2,h) * 255.0f);
		b = (BYTE)(HueToRGB2(m1,m2,h-120) * 255.0f);
	}

	RGBQUAD rgb = {b,g,r,0};
	return rgb;
}

RGBQUAD RGBtoHSL2(RGBQUAD lRGBColor)
{
	BYTE R,G,B;					/* input RGB values */
	BYTE H,L,S;					/* output HSL values */
	BYTE cMax,cMin;				/* max and min RGB values */
	WORD Rdelta,Gdelta,Bdelta;	/* intermediate value: % of spread from max*/

	R = lRGBColor.rgbRed;	/* get R, G, and B out of DWORD */
	G = lRGBColor.rgbGreen;
	B = lRGBColor.rgbBlue;

	cMax = max( max(R,G), B);	/* calculate lightness */
	cMin = min( min(R,G), B);
	L = (BYTE)((((cMax+cMin)*HSLMAX)+RGBMAX)/(2*RGBMAX));

	if (cMax==cMin){			/* r=g=b --> achromatic case */
		S = 0;					/* saturation */
		H = HSLUNDEFINED;		/* hue */
	} else {					/* chromatic case */
		if (L <= (HSLMAX/2))	/* saturation */
			S = (BYTE)((((cMax-cMin)*HSLMAX)+((cMax+cMin)/2))/(cMax+cMin));
		else
			S = (BYTE)((((cMax-cMin)*HSLMAX)+((2*RGBMAX-cMax-cMin)/2))/(2*RGBMAX-cMax-cMin));
		/* hue */
		Rdelta = (WORD)((((cMax-R)*(HSLMAX/6)) + ((cMax-cMin)/2) ) / (cMax-cMin));
		Gdelta = (WORD)((((cMax-G)*(HSLMAX/6)) + ((cMax-cMin)/2) ) / (cMax-cMin));
		Bdelta = (WORD)((((cMax-B)*(HSLMAX/6)) + ((cMax-cMin)/2) ) / (cMax-cMin));

		if (R == cMax)
			H = (BYTE)(Bdelta - Gdelta);
		else if (G == cMax)
			H = (BYTE)((HSLMAX/3) + Rdelta - Bdelta);
		else /* B == cMax */
			H = (BYTE)(((2*HSLMAX)/3) + Gdelta - Rdelta);

		//		if (H < 0) H += HSLMAX;     //always false
		if (H > HSLMAX) H -= HSLMAX;
	}
	RGBQUAD hsl={L,S,H,0};
	return hsl;
}

void Colorize(BYTE hue, BYTE sat, float blend, RGBQUAD& src, RGBQUAD& dst)
{
	if (blend < 0.0f) blend = 0.0f;
	if (blend > 1.0f) blend = 1.0f;
	int a0 = (int)(256*blend);
	int a1 = 256 - a0;

	bool bFullBlend = false;
	if (blend > 0.999f)	bFullBlend = true;

	RGBQUAD color,hsl;

	if (bFullBlend){
		color = RGBtoHSL2(src);
		color.rgbRed=hue;
		color.rgbGreen=sat;
		dst = HSLtoRGB2(color);
	} else {
		color = src;
		hsl.rgbRed=hue;
		hsl.rgbGreen=sat;
		hsl.rgbBlue = (BYTE)RGB2GRAY(color.rgbRed,color.rgbGreen,color.rgbBlue);
		hsl = HSLtoRGB2(hsl);
		//BlendPixelColor(x,y,hsl,blend);
		//color.rgbRed = (BYTE)(hsl.rgbRed * blend + color.rgbRed * (1.0f - blend));
		//color.rgbBlue = (BYTE)(hsl.rgbBlue * blend + color.rgbBlue * (1.0f - blend));
		//color.rgbGreen = (BYTE)(hsl.rgbGreen * blend + color.rgbGreen * (1.0f - blend));
		color.rgbRed = (BYTE)((hsl.rgbRed * a0 + color.rgbRed * a1)>>8);
		color.rgbBlue = (BYTE)((hsl.rgbBlue * a0 + color.rgbBlue * a1)>>8);
		color.rgbGreen = (BYTE)((hsl.rgbGreen * a0 + color.rgbGreen * a1)>>8);
		dst = color;
	}
}

COLORREF CalculateColor(unsigned int uColor)
{
	RGBQUAD src, dst;
	src.rgbRed = DEF_R;
	src.rgbGreen = DEF_G;
	src.rgbBlue = DEF_B;

	Colorize(uColor&0x0ff, uColor>>8 & 0x0ff, ((double)(uColor>>16 & 0x0ff))/255.0, src, dst);
	return RGB(dst.rgbRed, dst.rgbGreen, dst.rgbBlue);
}

HBITMAP Colorize(CxImage* pImage, BYTE bHue, BYTE bSat, float bBlend)
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

} } //namespace common::ui
