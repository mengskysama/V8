#include "stdafx.h"

#include "../../../../include/Skin_i.h"
#include "../../include/ui/ImageWrapper.h"

namespace common { namespace ui
{

CImageWrapper::CImageWrapper(void)
:
m_spImage(NULL)
{
}

CImageWrapper::~CImageWrapper(void)
{
}

CImageWrapper::CImageWrapper(const CImageWrapper& wrap)
{
    m_spImage = wrap.m_spImage;
}

CImageWrapper& CImageWrapper::operator = (const CImageWrapper& wrap)
{
    if (this == &wrap)
        return *this;

    m_spImage = wrap.m_spImage;
    return *this;
}

CImageWrapper& CImageWrapper::operator = (IImage* pIImage)
{
    if (m_spImage == pIImage)
        return *this;

    m_spImage = pIImage;
    return *this;
}

void CImageWrapper::GetInterface(IUnknown** ppUnknown)
{
    *ppUnknown = NULL;
    if (m_spImage)
        m_spImage->QueryInterface(IID_IUnknown, (void**)ppUnknown);
}

bool CImageWrapper::operator <  (const CImageWrapper& image) const
{
    return m_spImage < image.m_spImage;
}

bool CImageWrapper::operator == (const CImageWrapper& image) const
{
    return m_spImage == image.m_spImage;
}

void CImageWrapper::Attach(IImage* pImage)
{
    m_spImage.Attach(pImage);
}

IImage* CImageWrapper::Detach()
{
    return m_spImage.Detach();
}

CImageWrapper::CImageWrapper(IImage* pImage)
{
    m_spImage = pImage;
}

BOOL CImageWrapper::IsValid()
{
    return (m_spImage != NULL);
}

UINT CImageWrapper::GetWidth()
{
    if (m_spImage == NULL)
        return 0;

    UINT width = 0;
    HRESULT hr = m_spImage->get_Width(&width);
    if (hr != S_OK)
        return 0;

    return width;
}

UINT CImageWrapper::GetHeight()
{
    if (m_spImage == NULL)
        return 0;

    UINT height = 0;
    HRESULT hr = m_spImage->get_Height(&height);
    if (hr != S_OK)
        return 0;

    return height;
}

BOOL CImageWrapper::Render(HDC hDC, const RECT& dstRect, LPCRECT pSrcRect, LPCRECT pBoundRect)
{
    if (m_spImage == NULL)
        return FALSE;

    HRESULT hr = m_spImage->Render(hDC, &dstRect, pSrcRect, pBoundRect);
    if (hr != S_OK)
        return FALSE;

    return TRUE;
}

UINT CImageWrapper::GetNumFrames()
{
    CComQIPtr<IAnimation> spAnimation = m_spImage;
    if (spAnimation == NULL)
        return 0;

    UINT uFrame = 0;
    HRESULT hr = spAnimation->GetNumFrames(&uFrame);
    if (hr != S_OK)
        return 0;

    return uFrame;
}

BOOL CImageWrapper::GetAnimationInfo(ANIMATEINFO& animationInfo)
{
    CComQIPtr<IAnimation> spAnimation = m_spImage;
    if (spAnimation == NULL)
        return FALSE;

    HRESULT hr = spAnimation->GetAnimationInfo(&animationInfo);
    if (hr != S_OK)
        return FALSE;

    return TRUE;
}

BOOL CImageWrapper::GetFrameInfo(UINT uFrame, FRAMEINFO& frameInfo)
{
    CComQIPtr<IAnimation> spAnimation = m_spImage;
    if (spAnimation == NULL)
        return FALSE;

    HRESULT hr = spAnimation->GetFrameInfo(uFrame, &frameInfo);
    if (hr != S_OK)
        return FALSE;

    return TRUE;
}

BOOL CImageWrapper::RenderFrame(HDC hDC, UINT uFrame, const RECT& dstRect)
{
    CComQIPtr<IAnimation> spAnimation = m_spImage;
    if (spAnimation == NULL)
        return FALSE;

    HRESULT hr = spAnimation->RenderFrame(hDC, uFrame, &dstRect);
    if (hr != S_OK)
        return FALSE;

    return TRUE;
}

BOOL CImageWrapper::RenderEx(HDC hDC, LPCRECT pDstRect, VARIANT_BOOL bFrame)
{
    CComQIPtr<IImageEx> spImageEx = m_spImage;
    if (spImageEx == NULL)
        return FALSE;

    HRESULT hr = spImageEx->RenderEx(hDC, pDstRect, bFrame);
    if (hr != S_OK)
        return FALSE;

    return TRUE;
}

HBITMAP CImageWrapper::CreateHBITMAP(HDC hDC)
{
    CComQIPtr<IImageEx> spImageEx = m_spImage;
    if (spImageEx == NULL)
        return NULL;

    HBITMAP hBitmap = NULL;
    HRESULT hr = spImageEx->CreateHBITMAP(hDC, &hBitmap);
    if (hr != S_OK)
        return NULL;

    return hBitmap;
}

} } //namespace common::ui
