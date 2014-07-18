#pragma once

#include "../Common.h"

struct IImage;
struct tagANIMATEINFO;
struct tagFRAMEINFO;

namespace common { namespace ui
{

class COMMON_INTERFACE CImageWrapper
{
public:
    CImageWrapper(void);
    CImageWrapper(const CImageWrapper& wrap);
    explicit CImageWrapper(IImage* pImage);
    virtual ~CImageWrapper(void);

public:
    CImageWrapper& operator = (const CImageWrapper& wrap);
    CImageWrapper& operator = (IImage* pIImage);
    void GetInterface(IUnknown** ppUnknown);

    bool operator <  (const CImageWrapper& image) const;
    bool operator == (const CImageWrapper& image) const;

public:
    void Attach(IImage* pImage);
    IImage* Detach();

public:
    BOOL IsValid();
    UINT GetWidth();
    UINT GetHeight();
    BOOL Render(HDC hDC, const RECT& dstRect, LPCRECT pSrcRect = NULL, LPCRECT pBoundRect = NULL);
    UINT GetNumFrames();
    BOOL GetAnimationInfo(tagANIMATEINFO& animationInfo);
    BOOL GetFrameInfo(UINT uFrame, tagFRAMEINFO& frameInfo);
    BOOL RenderFrame(HDC hDC, UINT uFrame, const RECT& dstRect);
    BOOL RenderEx(HDC hDC, LPCRECT pDstRect, VARIANT_BOOL bFrame);
    HBITMAP CreateHBITMAP(HDC hDC = NULL);

private:
    CComPtr<IImage> m_spImage;
};

} } //namespace common::ui
