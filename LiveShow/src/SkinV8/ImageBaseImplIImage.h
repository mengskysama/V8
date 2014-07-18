#pragma once
#include "imagebase.h"

struct IImage;
class CImageBaseImplIImage :
    public CImageBase
{
public:
    CImageBaseImplIImage(IImage* pImage);
    virtual ~CImageBaseImplIImage(void);

public:
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

private:
    CComPtr<IImage> m_spImage;
};
