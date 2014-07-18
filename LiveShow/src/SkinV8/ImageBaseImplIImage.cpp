#include "StdAfx.h"
#include "Skin_i.h"
#include "ImageBaseImplIImage.h"

CImageBaseImplIImage::CImageBaseImplIImage(IImage* pImage)
{
    m_spImage = pImage;
}

CImageBaseImplIImage::~CImageBaseImplIImage(void)
{
}

BOOL CImageBaseImplIImage::LoadFromFile(LPCTSTR filePathName)
{
    return FALSE;
}

BOOL CImageBaseImplIImage::LoadFromData(const char* pzData, long size)
{
    return FALSE;
}

BOOL CImageBaseImplIImage::Transform(const CImageTransformParam* pParam)
{
    return FALSE;
}

BOOL CImageBaseImplIImage::Draw(HDC hDC, LPCRECT lpDstRect, LPRECT lpClipRect, const CImageTransformParam* pParam, 
                  LONG clrTransparent, BOOL bAlpha, 
                  BOOL bSideStretch, LONG sideWidth, LONG sideHeight)
{
    if (m_spImage == NULL)
        return FALSE;

    HRESULT hr = m_spImage->Render(hDC, lpDstRect, NULL, lpClipRect);
    return (hr == S_OK);
}

LONG CImageBaseImplIImage::Width()
{
    if (m_spImage == NULL)
        return 0;

    UINT width = 0;
    HRESULT hr = m_spImage->get_Width(&width);
    return width;
}

LONG CImageBaseImplIImage::Height()
{
    if (m_spImage == NULL)
        return 0;

    UINT height = 0;
    HRESULT hr = m_spImage->get_Height(&height);
    return height;
}

BOOL CImageBaseImplIImage::GetSize(LONG& lSize)
{
    return FALSE;
}

//animation
LONG CImageBaseImplIImage::GetNumFrames()
{
    CComQIPtr<IAnimation> spAnimation = m_spImage;
    if (spAnimation == NULL)
        return FALSE;

    UINT uFrames = 0;
    HRESULT hr = spAnimation->GetNumFrames(&uFrames);
    return uFrames;
}

BOOL CImageBaseImplIImage::GetAnimationInfo(tagANIMATEINFO& animateInfo)
{
    CComQIPtr<IAnimation> spAnimation = m_spImage;
    if (spAnimation == NULL)
        return FALSE;

    HRESULT hr = spAnimation->GetAnimationInfo(&animateInfo);
    return (hr == S_OK);
}

BOOL CImageBaseImplIImage::GetFrameInfo(LONG iFrame, tagFRAMEINFO& frameInfo)
{
    CComQIPtr<IAnimation> spAnimation = m_spImage;
    if (spAnimation == NULL)
        return FALSE;

    if (iFrame < 0)
        return FALSE;

    UINT uFrames = iFrame;
    HRESULT hr = spAnimation->GetFrameInfo(uFrames, &frameInfo);
    return (hr == S_OK);
}

BOOL CImageBaseImplIImage::DrawFrame(HDC hDC, LONG iFrame, const RECT& dstRect)
{
    CComQIPtr<IAnimation> spAnimation = m_spImage;
    if (spAnimation == NULL)
        return FALSE;

    if (iFrame < 0)
        return FALSE;

    UINT uFrames = iFrame;
    HRESULT hr = spAnimation->RenderFrame(hDC, uFrames, &dstRect);
    return (hr == S_OK);
}

