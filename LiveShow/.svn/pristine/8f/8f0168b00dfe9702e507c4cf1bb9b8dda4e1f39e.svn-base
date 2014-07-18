#pragma once

class CColorAdjuster
{
public:
    //构造，析构
    CColorAdjuster(void);
    ~CColorAdjuster(void);

public:
    //初始化
    void Init(double hue, double sat, double light);
    //调整
    RGBQUAD Adjust(RGBQUAD color);

protected:
    double m_hue;
    double m_saturation;
    double m_lightness;

    int m_lumRedLookup[256];
    int m_lumGreenLookup[256];
    int m_lumBlueLookup[256];

    int m_finalRedLookup[256];
    int m_finalGreenLookup[256];
    int m_finalBlueLookup[256];
};

class CColorGradationAdjuster
{
public:
    CColorGradationAdjuster(void);
    ~CColorGradationAdjuster(void);

public:
    void Init(BYTE minInput, BYTE maxInput, BYTE minOutput, BYTE maxOutput);
    RGBQUAD Adjust(RGBQUAD color);

protected:
    int m_RedLookup[256];
    int m_GreenLookup[256];
    int m_BlueLookup[256];
};