#include "../../StdAfx.h"

#include "Gdiplus.h"
#pragma comment( lib, "gdiplus.lib")

#include "../../../../include/Skin_i.h"
#include "../../../../include/Skin_i.c"

#include "../../include/ui/ImageImpl.h"
#include "../../include/ui/IImageManager.h"

#include <vector>

#pragma warning (push)
#pragma warning(disable : 4996)
#include "../../thirdlibrary/CxImage/include/ximage.h"
#pragma warning (pop)

#define NESTED_CLASS_OFFSET(c, m) (size_t)&(((c*)0)->m)

#define NESTED_CLASS_PROLOGUE(theClass, nestedClass) \
    theClass* pThis = \
    ((theClass*)((BYTE*)this - NESTED_CLASS_OFFSET(theClass, m_x##nestedClass)))

namespace common { namespace ui
{

class CxIStreamFile :
    public CxFile
{
public:
    CxIStreamFile(IStream* pIStream)
    {
        m_pIStream = pIStream;
    }

    virtual ~CxIStreamFile(void)
    {
    }

public:
    virtual bool Close()
    {
        m_pIStream = NULL;

        return true;
    }

    virtual size_t Read(void *buffer, size_t size, size_t count)
    {
        ASSERT(m_pIStream != NULL);
        if (m_pIStream == NULL)
            return 0;

#pragma warning (push)
#pragma warning(disable : 4245)
        ULONG readSize = -1;
        HRESULT hr = m_pIStream->Read(buffer, (ULONG)(size * count), &readSize);
#pragma warning(pop)
        if (FAILED(hr))
            return 0;

        ASSERT(readSize > 0);
        ASSERT(readSize < UINT_MAX);

        return (size_t)readSize;
    }

    virtual size_t Write(const void *buffer, size_t size, size_t count)
    {
        ASSERT(m_pIStream != NULL);
        if (m_pIStream == NULL)
            return 0;

#pragma warning (push)
#pragma warning(disable : 4245)
        ULONG writtenSize = -1;
        HRESULT hr = m_pIStream->Write(buffer, (ULONG)(size * count), &writtenSize);
#pragma warning(pop)
        if (FAILED(hr))
            return 0;

        ASSERT(writtenSize > 0);
        ASSERT(writtenSize < UINT_MAX);

        return (size_t)writtenSize;
    }

    virtual bool Seek(long offset, int origin)
    {
        ASSERT(m_pIStream != NULL);
        if (m_pIStream == NULL)
            return false;

        LARGE_INTEGER liOffset; 
        liOffset.QuadPart = offset;
        DWORD dwOrigin = origin;
        ULARGE_INTEGER newPosition; 
#pragma warning (push)
#pragma warning(disable : 4245)
        newPosition.QuadPart = -1;
#pragma warning(pop)
        HRESULT hr = m_pIStream->Seek(liOffset, dwOrigin, &newPosition);
        if (FAILED(hr))
            return false;

        return true;
    }

    virtual long Tell()
    {
        ASSERT(m_pIStream != NULL);
        if (m_pIStream == NULL)
            return 0;

        return 0;
    }

    virtual long Size()
    {
        ASSERT(m_pIStream != NULL);
        if (m_pIStream == NULL)
            return 0;

        STATSTG stg = {0};
        HRESULT hr = m_pIStream->Stat(&stg, 0);
        if (FAILED(hr))
            return 0;

        ASSERT(stg.cbSize.QuadPart > INT_MAX);
        return (long)(stg.cbSize.QuadPart);
    }

    virtual bool Flush()
    {
        ASSERT(m_pIStream != NULL);
        if (m_pIStream == NULL)
            return false;

        HRESULT hr = m_pIStream->Commit(STGC_DEFAULT);
        if (FAILED(hr))
            return false;

        return true;
    }

    virtual bool Eof()
    {
        ASSERT(m_pIStream != NULL);
        if (m_pIStream == NULL)
            return true;

        return false;
    }

    virtual long Error()
    {
        ASSERT(m_pIStream != NULL);
        if (m_pIStream == NULL)
            return 0;

        return 0;
    }

    virtual bool PutC(unsigned char c)
    {
        // Default implementation
        size_t nWrote = Write(&c, 1, 1);
        return (bool)(nWrote == 1);
    }

    virtual long GetC()
    {
        ASSERT(m_pIStream != NULL);
        if (m_pIStream == NULL)
            return 0;

        char c;
        size_t nWrote = Read(&c, 1, 1);
        ASSERT(nWrote == 1);

        return c;
    }

    virtual char* GetS(char *string, int n)
    {
        UNREFERENCED_PARAMETER(string);
        UNREFERENCED_PARAMETER(n);
        ASSERT(0);
        ASSERT(m_pIStream != NULL);
        if (m_pIStream == NULL)
            return NULL;

        return NULL;
    }

    virtual long Scanf(const char *format, void* output)
    {
        UNREFERENCED_PARAMETER(format);
        UNREFERENCED_PARAMETER(output);
        ASSERT(0);
        ASSERT(m_pIStream != NULL);
        if (m_pIStream == NULL)
            return 0;

        return 0;
    }

protected:
    CComPtr<IStream> m_pIStream;
};

ENUM_CXIMAGE_FORMATS CheckFormat(BYTE* pBuffer, long size)
{
    static CxImage sCheckCxImage;

    //先用文件头的特殊字符来判断
    if (size < 10)
        return CXIMAGE_FORMAT_UNKNOWN;
    const BYTE* cpBuffer = pBuffer;
    if (cpBuffer[0] == 'G' && cpBuffer[1] == 'I' && cpBuffer[2] == 'F') {
        return CXIMAGE_FORMAT_GIF;
    } else if (cpBuffer[1] == 'P' && cpBuffer[2] == 'N' && cpBuffer[3] == 'G') {
        return CXIMAGE_FORMAT_PNG;
    } else if ( (cpBuffer[6] == 'E' && cpBuffer[7] == 'x' && cpBuffer[8] == 'i' && cpBuffer[9] == 'f') || (cpBuffer[6] == 'J' && cpBuffer[7] == 'F' && cpBuffer[8] == 'I' && cpBuffer[9] == 'F') ) {
        return CXIMAGE_FORMAT_JPG;
    } else if (cpBuffer[0] == 'B' && cpBuffer[1] == 'M') {
        return CXIMAGE_FORMAT_BMP;
    }

#if CXIMAGE_SUPPORT_BMP
    CXIMAGE_FORMAT_BMP;// = 1,
    if (sCheckCxImage.CheckFormat(pBuffer, size, CXIMAGE_FORMAT_BMP))
        return CXIMAGE_FORMAT_BMP;
#endif

#if CXIMAGE_SUPPORT_GIF
    CXIMAGE_FORMAT_GIF;// = 2,
    if (sCheckCxImage.CheckFormat(pBuffer, size, CXIMAGE_FORMAT_GIF))
        return CXIMAGE_FORMAT_GIF;
#endif

#if CXIMAGE_SUPPORT_JPG
    CXIMAGE_FORMAT_JPG;// = 3,
    if (sCheckCxImage.CheckFormat(pBuffer, size, CXIMAGE_FORMAT_JPG))
        return CXIMAGE_FORMAT_JPG;
#endif

#if CXIMAGE_SUPPORT_PNG
    CXIMAGE_FORMAT_PNG;// = 4,
    if (sCheckCxImage.CheckFormat(pBuffer, size, CXIMAGE_FORMAT_PNG))
        return CXIMAGE_FORMAT_PNG;
#endif

#if CXIMAGE_SUPPORT_ICO
    CXIMAGE_FORMAT_ICO;// = 5,
    if (sCheckCxImage.CheckFormat(pBuffer, size, CXIMAGE_FORMAT_ICO))
        return CXIMAGE_FORMAT_ICO;
#endif

#if CXIMAGE_SUPPORT_TIF
    CXIMAGE_FORMAT_TIF;// = 6,
    if (sCheckCxImage.CheckFormat(pBuffer, size, CXIMAGE_FORMAT_TIF))
        return CXIMAGE_FORMAT_TIF;
#endif

#if CXIMAGE_SUPPORT_TGA
    CXIMAGE_FORMAT_TGA;// = 7,
    if (sCheckCxImage.CheckFormat(pBuffer, size, CXIMAGE_FORMAT_TGA))
        return CXIMAGE_FORMAT_TGA;
#endif

#if CXIMAGE_SUPPORT_PCX
    CXIMAGE_FORMAT_PCX;// = 8,
    if (sCheckCxImage.CheckFormat(pBuffer, size, CXIMAGE_FORMAT_PCX))
        return CXIMAGE_FORMAT_PCX;
#endif

#if CXIMAGE_SUPPORT_WBMP
    CXIMAGE_FORMAT_WBMP;// = 9,
    if (sCheckCxImage.CheckFormat(pBuffer, size, CXIMAGE_FORMAT_WBMP))
        return CXIMAGE_FORMAT_WBMP;
#endif

#if CXIMAGE_SUPPORT_WMF
    CXIMAGE_FORMAT_WMF;// = 10,
    if (sCheckCxImage.CheckFormat(pBuffer, size, CXIMAGE_FORMAT_WMF))
        return CXIMAGE_FORMAT_WMF;
#endif

#if CXIMAGE_SUPPORT_JP2
    CXIMAGE_FORMAT_JP2;// = 11,
    if (sCheckCxImage.CheckFormat(pBuffer, size, CXIMAGE_FORMAT_JP2))
        return CXIMAGE_FORMAT_JP2;
#endif

#if CXIMAGE_SUPPORT_JPC
    CXIMAGE_FORMAT_JPC;// = 12,
    if (sCheckCxImage.CheckFormat(pBuffer, size, CXIMAGE_FORMAT_JPC))
        return CXIMAGE_FORMAT_JPC;
#endif

#if CXIMAGE_SUPPORT_PGX
    CXIMAGE_FORMAT_PGX;// = 13,
    if (sCheckCxImage.CheckFormat(pBuffer, size, CXIMAGE_FORMAT_PGX))
        return CXIMAGE_FORMAT_PGX;
#endif

#if CXIMAGE_SUPPORT_PNM
    CXIMAGE_FORMAT_PNM;// = 14,
    if (sCheckCxImage.CheckFormat(pBuffer, size, CXIMAGE_FORMAT_PNM))
        return CXIMAGE_FORMAT_PNM;
#endif

#if CXIMAGE_SUPPORT_RAS
    CXIMAGE_FORMAT_RAS;// = 15,
    if (sCheckCxImage.CheckFormat(pBuffer, size, CXIMAGE_FORMAT_RAS))
        return CXIMAGE_FORMAT_RAS;
#endif

#if CXIMAGE_SUPPORT_JBG
    CXIMAGE_FORMAT_JBG;// = 16,
    if (sCheckCxImage.CheckFormat(pBuffer, size, CXIMAGE_FORMAT_JBG))
        return CXIMAGE_FORMAT_JBG;
#endif

#if CXIMAGE_SUPPORT_MNG
    CXIMAGE_FORMAT_MNG;// = 17,
    if (sCheckCxImage.CheckFormat(pBuffer, size, CXIMAGE_FORMAT_MNG))
        return CXIMAGE_FORMAT_MNG;
#endif

#if CXIMAGE_SUPPORT_SKA
    CXIMAGE_FORMAT_SKA;// = 18,
    if (sCheckCxImage.CheckFormat(pBuffer, size, CXIMAGE_FORMAT_SKA))
        return CXIMAGE_FORMAT_SKA;
#endif

#if CXIMAGE_SUPPORT_RAW
    CXIMAGE_FORMAT_RAW;// = 19,
    if (sCheckCxImage.CheckFormat(pBuffer, size, CXIMAGE_FORMAT_RAW))
        return CXIMAGE_FORMAT_RAW;
#endif

    //Unknown
    CXIMAGE_FORMAT_UNKNOWN;// = 0,
    return CXIMAGE_FORMAT_UNKNOWN;
}

ENUM_CXIMAGE_FORMATS CheckFormat(CxFile* pFile)
{
    static CxImage sCheckCxImage;

#if CXIMAGE_SUPPORT_BMP
    CXIMAGE_FORMAT_BMP;// = 1,
    if (sCheckCxImage.CheckFormat(pFile, CXIMAGE_FORMAT_BMP))
        return CXIMAGE_FORMAT_BMP;
#endif

#if CXIMAGE_SUPPORT_GIF
    CXIMAGE_FORMAT_GIF;// = 2,
    if (sCheckCxImage.CheckFormat(pFile, CXIMAGE_FORMAT_GIF))
        return CXIMAGE_FORMAT_GIF;
#endif

#if CXIMAGE_SUPPORT_JPG
    CXIMAGE_FORMAT_JPG;// = 3,
    if (sCheckCxImage.CheckFormat(pFile, CXIMAGE_FORMAT_JPG))
        return CXIMAGE_FORMAT_JPG;
#endif

#if CXIMAGE_SUPPORT_PNG
    CXIMAGE_FORMAT_PNG;// = 4,
    if (sCheckCxImage.CheckFormat(pFile, CXIMAGE_FORMAT_PNG))
        return CXIMAGE_FORMAT_PNG;
#endif

#if CXIMAGE_SUPPORT_ICO
    CXIMAGE_FORMAT_ICO;// = 5,
    if (sCheckCxImage.CheckFormat(pFile, CXIMAGE_FORMAT_ICO))
        return CXIMAGE_FORMAT_ICO;
#endif

#if CXIMAGE_SUPPORT_TIF
    CXIMAGE_FORMAT_TIF;// = 6,
    if (sCheckCxImage.CheckFormat(pFile, CXIMAGE_FORMAT_TIF))
        return CXIMAGE_FORMAT_TIF;
#endif

#if CXIMAGE_SUPPORT_TGA
    CXIMAGE_FORMAT_TGA;// = 7,
    if (sCheckCxImage.CheckFormat(pFile, CXIMAGE_FORMAT_TGA))
        return CXIMAGE_FORMAT_TGA;
#endif

#if CXIMAGE_SUPPORT_PCX
    CXIMAGE_FORMAT_PCX;// = 8,
    if (sCheckCxImage.CheckFormat(pFile, CXIMAGE_FORMAT_PCX))
        return CXIMAGE_FORMAT_PCX;
#endif

#if CXIMAGE_SUPPORT_WBMP
    CXIMAGE_FORMAT_WBMP;// = 9,
    if (sCheckCxImage.CheckFormat(pFile, CXIMAGE_FORMAT_WBMP))
        return CXIMAGE_FORMAT_WBMP;
#endif

#if CXIMAGE_SUPPORT_WMF
    CXIMAGE_FORMAT_WMF;// = 10,
    if (sCheckCxImage.CheckFormat(pFile, CXIMAGE_FORMAT_WMF))
        return CXIMAGE_FORMAT_WMF;
#endif

#if CXIMAGE_SUPPORT_JP2
    CXIMAGE_FORMAT_JP2;// = 11,
    if (sCheckCxImage.CheckFormat(pFile, CXIMAGE_FORMAT_JP2))
        return CXIMAGE_FORMAT_JP2;
#endif

#if CXIMAGE_SUPPORT_JPC
    CXIMAGE_FORMAT_JPC;// = 12,
    if (sCheckCxImage.CheckFormat(pFile, CXIMAGE_FORMAT_JPC))
        return CXIMAGE_FORMAT_JPC;
#endif

#if CXIMAGE_SUPPORT_PGX
    CXIMAGE_FORMAT_PGX;// = 13,
    if (sCheckCxImage.CheckFormat(pFile, CXIMAGE_FORMAT_PGX))
        return CXIMAGE_FORMAT_PGX;
#endif

#if CXIMAGE_SUPPORT_PNM
    CXIMAGE_FORMAT_PNM;// = 14,
    if (sCheckCxImage.CheckFormat(pFile, CXIMAGE_FORMAT_PNM))
        return CXIMAGE_FORMAT_PNM;
#endif

#if CXIMAGE_SUPPORT_RAS
    CXIMAGE_FORMAT_RAS;// = 15,
    if (sCheckCxImage.CheckFormat(pFile, CXIMAGE_FORMAT_RAS))
        return CXIMAGE_FORMAT_RAS;
#endif

#if CXIMAGE_SUPPORT_JBG
    CXIMAGE_FORMAT_JBG;// = 16,
    if (sCheckCxImage.CheckFormat(pFile, CXIMAGE_FORMAT_JBG))
        return CXIMAGE_FORMAT_JBG;
#endif

#if CXIMAGE_SUPPORT_MNG
    CXIMAGE_FORMAT_MNG;// = 17,
    if (sCheckCxImage.CheckFormat(pFile, CXIMAGE_FORMAT_MNG))
        return CXIMAGE_FORMAT_MNG;
#endif

#if CXIMAGE_SUPPORT_SKA
    CXIMAGE_FORMAT_SKA;// = 18,
    if (sCheckCxImage.CheckFormat(pFile, CXIMAGE_FORMAT_SKA))
        return CXIMAGE_FORMAT_SKA;
#endif

#if CXIMAGE_SUPPORT_RAW
    CXIMAGE_FORMAT_RAW;// = 19,
    if (sCheckCxImage.CheckFormat(pFile, CXIMAGE_FORMAT_RAW))
        return CXIMAGE_FORMAT_RAW;
#endif

    //Unknown
    CXIMAGE_FORMAT_UNKNOWN;// = 0,
    return CXIMAGE_FORMAT_UNKNOWN;
}

static HRESULT CreateStreamOnMemory(IN void* pSrc, IN INT iSize, OUT LPSTREAM FAR* ppstm)
{
    *ppstm = NULL;

    if (iSize < 0)
        return E_FAIL;

    HGLOBAL hGlobal = ::GlobalAlloc(GMEM_FIXED | GMEM_ZEROINIT, iSize);
    if (hGlobal == NULL)
        return E_FAIL;

    void* pDst = ::GlobalLock(hGlobal);
    memcpy_s(pDst, iSize, pSrc, iSize);
    ::GlobalUnlock(hGlobal);

    HRESULT hr = ::CreateStreamOnHGlobal(hGlobal, TRUE, ppstm);
    if (FAILED(hr)) {
        ::GlobalFree(hGlobal);
        return hr;
    }

    return hr;
}

typedef CxImage* CxImagePtr;
typedef Gdiplus::Bitmap* BitmapPtr;
typedef IImageManager* IImageManagerPtr;

#define CLASS_DATA_MEMBER_LIST(V) \
    V(CxImagePtr, pImage) \
    V(BitmapPtr, pBitmap) \
    V(IImageManagerPtr, pImageManager) \
    V(LONG, nRefCount)

DECLARE_CLASS_DATA_STRUCT(CImageImpl, CLASS_DATA_MEMBER_LIST)

CImageImpl::CImageImpl(IImageManager* pManager)
{
    NEW_CLASS_DATA(CImageImpl)
    USE_CLASS_DATA(CLASS_DATA_MEMBER_LIST);

    m_pImage = NULL;
    m_pBitmap = NULL;
    m_pImageManager = pManager;
    m_nRefCount = 1;
}

void CImageImpl::Destroy()
{
    delete this;
}

CImageImpl::~CImageImpl(void)
{
    Clear();

    DELETE_CLASS_DATA()
}

ULONG CImageImpl::AddRef()
{
    USE_CLASS_DATA(CLASS_DATA_MEMBER_LIST);

    return InterlockedIncrement(&m_nRefCount);
}

ULONG CImageImpl::Release()
{
    USE_CLASS_DATA(CLASS_DATA_MEMBER_LIST);

    LONG nRefCount = 0;
    nRefCount = InterlockedDecrement(&m_nRefCount);
    if (nRefCount == 0)
    {
        if (m_pImageManager)
        {
            m_pImageManager->NotifyWeak(this);
        }
        else
        {
            delete this;
        }
    }
    return nRefCount;
}

HRESULT CImageImpl::QueryInterface(REFIID iid, void** ppvObject)
{
    USE_CLASS_DATA(CLASS_DATA_MEMBER_LIST);

    if (IsBadWritePtr(ppvObject, sizeof(void*)))
        return E_POINTER;

    *ppvObject = NULL;

    if (IsEqualGUID(iid, IID_IUnknown))
    {
        *ppvObject = static_cast<IUnknown*>(this);
    }
    if (IsEqualGUID(iid, IID_IImage)) 
    {
        *ppvObject = static_cast<IImage*>(this);
    }
    if (IsEqualGUID(iid, IID_IAnimation))
    {
        *ppvObject = static_cast<IAnimation*>(&m_xAnimation);
    }
    if (IsEqualGUID(iid, IID_IImageEx))
    {
        *ppvObject = static_cast<IImageEx*>(&m_xImageEx);
    }

    if (*ppvObject != NULL)
    {
        AddRef();
        return S_OK;
    }

    return E_NOINTERFACE;
}

HRESULT CImageImpl::get_Width(UINT* pWidth)
{
    USE_CLASS_DATA(CLASS_DATA_MEMBER_LIST);

    *pWidth = 0;

    if (m_pImage == NULL)
        return S_FALSE;

    *pWidth = m_pImage->GetWidth();
    return S_OK;
}

HRESULT CImageImpl::get_Height(UINT* pHeight)
{
    USE_CLASS_DATA(CLASS_DATA_MEMBER_LIST);

    *pHeight = 0;

    if (m_pImage == NULL)
        return S_FALSE;

    *pHeight = m_pImage->GetHeight();
    return S_OK;
}

HRESULT CImageImpl::Render(HDC hDC, LPCRECT pDstRect, LPCRECT pSrcRect, LPCRECT pBoundRect)
{
    USE_CLASS_DATA(CLASS_DATA_MEMBER_LIST);

    UNREFERENCED_PARAMETER(pSrcRect);
    UNREFERENCED_PARAMETER(pBoundRect);

    if (m_pImage == NULL)
        return S_FALSE;

    BOOL bGray = m_pImage->IsGrayScale();
    UINT uFrames = 0;
    if (S_OK == GetNumFrames(&uFrames) && uFrames > 1) 
    {
        if (bGray)
        {
            //填充白色
            HBRUSH hBrush = ::CreateSolidBrush(RGB(255, 255, 255));
            ::FillRect(hDC, pDstRect, hBrush);
            ::DeleteObject(hBrush);

            try
            {
                //去alpha矩阵，未实现
                static Gdiplus::ColorMatrix removeAlphaMatrix = 
                {
                    1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
                    0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
                    0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
                    0.0f, 0.0f, 0.0f, 1.0f, 0.0f,
                    0.0f, 0.0f, 0.0f, 0.0f, 1.0f,
                };

                //去色矩阵
                static Gdiplus::ColorMatrix grayMatrix = 
                {
                    0.3f, 0.3f, 0.3f, 0.0f, 0.0f,
                    0.59f, 0.59f, 0.59f, 0.0f, 0.0f,
                    0.11f, 0.11f, 0.11f, 0.0f, 0.0f,
                    0.0f, 0.0f, 0.0f, 1.0f, 0.0f,
                    0.0f, 0.0f, 0.0f, 0.0f, 1.0f,
                };

                //选择第一帧
                GUID guidGif = Gdiplus::FrameDimensionTime;
                m_pBitmap->SelectActiveFrame(&guidGif, 0);

                CRect dstRect = *pDstRect;
                Gdiplus::Rect rcDraw(dstRect.left, dstRect.top, (dstRect.right - dstRect.left), (dstRect.bottom - dstRect.top));
                Gdiplus::Graphics graphics(hDC);
                Gdiplus::ImageAttributes *pImageAttributes = new Gdiplus::ImageAttributes();
                pImageAttributes->SetColorMatrix(&grayMatrix);
                Gdiplus::Status ret = graphics.DrawImage(m_pBitmap, 
                    rcDraw, 
                    0, 0, m_pBitmap->GetWidth(), m_pBitmap->GetHeight(), 
                    Gdiplus::UnitPixel, pImageAttributes);
                delete pImageAttributes;

                if (ret == Gdiplus::Ok)
                    return S_OK;
            }
            catch (...)
            {
            }
        } 
        else 
        {
            //填充白色
            ::FillRect(hDC, pDstRect, (HBRUSH)::GetStockObject(WHITE_BRUSH));

            if (S_OK == RenderFrame(hDC, 0, pDstRect))
                return S_OK;
        }

    }

    if (! m_pImage->AlphaIsValid()) 
    {
        if (! m_pImage->Stretch(hDC, *pDstRect))
            return S_FALSE;
    }
    else 
    {
        if (! m_pImage->Draw(hDC, *pDstRect))
            return S_FALSE;
    }

    return S_OK;
}

HRESULT CImageImpl::GetNumFrames(UINT* pNumFrames)
{
    USE_CLASS_DATA(CLASS_DATA_MEMBER_LIST);

    *pNumFrames = 0;
    if (m_pBitmap == NULL)
        return S_FALSE;

    UINT dimensionsCount = m_pBitmap->GetFrameDimensionsCount();
    GUID* pGUIDS = new GUID[dimensionsCount];
    m_pBitmap->GetFrameDimensionsList(pGUIDS, dimensionsCount);
    UINT nCount = m_pBitmap->GetFrameCount(pGUIDS);
    delete [] pGUIDS;

    *pNumFrames = nCount;
    return S_OK;
}

HRESULT CImageImpl::GetAnimationInfo(LPANIMATEINFO pAnimationInfo)
{
    USE_CLASS_DATA(CLASS_DATA_MEMBER_LIST);

    pAnimationInfo->bkColorIndex = -1;
    pAnimationInfo->bkColor = 0xFFFFFFFF;

    if (m_pBitmap == NULL)
        return S_FALSE;

    return S_OK;
}

HRESULT CImageImpl::GetFrameInfo(UINT uFrame, LPFRAMEINFO pFrameInfo)
{
    USE_CLASS_DATA(CLASS_DATA_MEMBER_LIST);

    pFrameInfo->delayTime = 0;
    pFrameInfo->disposalMethod = 2;
    pFrameInfo->bkColorIndex = -1;
    pFrameInfo->bkColor = 0xFFFFFFFF;

    if (m_pBitmap == NULL)
        return S_FALSE;

    UINT uAllFrame = 0;
    if (S_OK != GetNumFrames(&uAllFrame))
        return S_FALSE;

    ASSERT(uFrame >= 0 && uFrame < uAllFrame);
    if (uFrame < 0 || uFrame >= uAllFrame)
        return S_FALSE;

    //获得帧时间延迟
    UINT uSize = m_pBitmap->GetPropertyItemSize(PropertyTagFrameDelay);
    if (uSize <= 0)
        return S_FALSE;

    try
    {
        std::vector<BYTE> vecPropBuffer(uSize, 0);
        Gdiplus::PropertyItem* pPropertyItem = (Gdiplus::PropertyItem*)(&vecPropBuffer[0]);
        m_pBitmap->GetPropertyItem(PropertyTagFrameDelay, uSize, pPropertyItem);
        pFrameInfo->delayTime = ((int*)pPropertyItem->value)[uFrame] * 10;
    }
    catch (...)
    {
        return S_FALSE;
    }

    if (pFrameInfo->delayTime < 100)
        pFrameInfo->delayTime = 100;

    return S_OK;
}

HRESULT CImageImpl::RenderFrame(HDC hDC, UINT uFrame, LPCRECT pDstRect)
{
    USE_CLASS_DATA(CLASS_DATA_MEMBER_LIST);

    if (m_pBitmap == NULL)
        return S_FALSE;

    try
    {
        GUID guidGif = Gdiplus::FrameDimensionTime;
        m_pBitmap->SelectActiveFrame(&guidGif, uFrame);

        CRect dstRect = *pDstRect;
        Gdiplus::Rect rcDraw(dstRect.left, dstRect.top, (dstRect.right - dstRect.left), (dstRect.bottom - dstRect.top));
        Gdiplus::Graphics graphics(hDC);
        Gdiplus::Status ret = graphics.DrawImage(m_pBitmap, rcDraw);

        if (ret == Gdiplus::Ok)
            return S_OK;
    }
    catch (...)
    {
        return S_FALSE;
    }

    return S_FALSE;
}

HRESULT CImageImpl::RenderEx(HDC hDC, LPCRECT pDstRect, VARIANT_BOOL bFrame)
{
    USE_CLASS_DATA(CLASS_DATA_MEMBER_LIST);

    UNREFERENCED_PARAMETER(bFrame);

    if (m_pImage == NULL)
        return S_FALSE;

    if (m_pImage->Stretch(hDC, *pDstRect))
        return S_FALSE;

    return S_OK;
}

HRESULT CImageImpl::CreateHBITMAP(HDC hDC, HBITMAP* pHBITMAP)
{
    USE_CLASS_DATA(CLASS_DATA_MEMBER_LIST);

    if (m_pImage == NULL)
        return S_FALSE;

    if (IsBadWritePtr(pHBITMAP, sizeof(HBITMAP)))
        return E_POINTER;
    *pHBITMAP = NULL;

    INT width = m_pImage->GetWidth();
    INT height = m_pImage->GetHeight();

    HDC hMemDC = NULL;
    HBITMAP hBitmap = NULL;
    if (hDC == NULL) {
        HDC hScreenDC = ::GetDC(NULL);
        if (hScreenDC == NULL)
            return S_FALSE;
        hMemDC = ::CreateCompatibleDC(hScreenDC);
        hBitmap = ::CreateCompatibleBitmap(hScreenDC, width, height);
        ::ReleaseDC(NULL, hScreenDC);
    } else {
        hMemDC = ::CreateCompatibleDC(hDC);
        hBitmap = ::CreateCompatibleBitmap(hDC, width, height);
    }
    HBITMAP hOldBitmap = (HBITMAP)::SelectObject(hMemDC, hBitmap);
    CRect dstRect(0, 0, width, height);
    ::FillRect(hMemDC, &dstRect, (HBRUSH)::GetStockObject(WHITE_BRUSH));
    HRESULT hr = Render(hMemDC, &dstRect, NULL, NULL);
    ::SelectObject(hMemDC, hOldBitmap);
    ::DeleteDC(hMemDC);

    if (hr != S_OK) {
        ::DeleteObject(hBitmap);
        return S_FALSE;
    }

    *pHBITMAP = hBitmap;
    return S_OK;


    //if (m_pImage == NULL)
    //    return S_FALSE;

    //if (IsBadWritePtr(pHBITMAP, sizeof(HBITMAP)))
    //    return E_POINTER;
    //*pHBITMAP = NULL;

    //HBITMAP hBitmap = m_pImage->MakeBitmap(hDC);
    //if (hBitmap == NULL) 
    //    return S_FALSE;

    //*pHBITMAP = hBitmap;
    //return S_OK;
}

ULONG CImageImpl::XAnimation::AddRef()
{
    NESTED_CLASS_PROLOGUE(CImageImpl, Animation);

    return pThis->AddRef();
}

ULONG CImageImpl::XAnimation::Release()
{
    NESTED_CLASS_PROLOGUE(CImageImpl, Animation);

    return pThis->Release();
}

HRESULT CImageImpl::XAnimation::QueryInterface(REFIID iid, void** ppvObject)
{
    NESTED_CLASS_PROLOGUE(CImageImpl, Animation);

    return pThis->QueryInterface(iid, ppvObject);
}

HRESULT CImageImpl::XAnimation::GetNumFrames(UINT* pNumFrames)
{
    NESTED_CLASS_PROLOGUE(CImageImpl, Animation);

    return pThis->GetNumFrames(pNumFrames);
}

HRESULT CImageImpl::XAnimation::GetAnimationInfo(LPANIMATEINFO pAnimationInfo)
{
    NESTED_CLASS_PROLOGUE(CImageImpl, Animation);

    return pThis->GetAnimationInfo(pAnimationInfo);
}

HRESULT CImageImpl::XAnimation::GetFrameInfo(UINT uFrame, LPFRAMEINFO pFrameInfo)
{
    NESTED_CLASS_PROLOGUE(CImageImpl, Animation);

    return pThis->GetFrameInfo(uFrame, pFrameInfo);
}

HRESULT CImageImpl::XAnimation::RenderFrame(HDC hDC, UINT uFrame, LPCRECT pDstRect)
{
    NESTED_CLASS_PROLOGUE(CImageImpl, Animation);

    return pThis->RenderFrame(hDC, uFrame, pDstRect);
}

ULONG CImageImpl::XImageEx::AddRef()
{
    NESTED_CLASS_PROLOGUE(CImageImpl, ImageEx);

    return pThis->AddRef();
}

ULONG CImageImpl::XImageEx::Release()
{
    NESTED_CLASS_PROLOGUE(CImageImpl, ImageEx);

    return pThis->Release();
}

HRESULT CImageImpl::XImageEx::QueryInterface(REFIID iid, void** ppvObject)
{
    NESTED_CLASS_PROLOGUE(CImageImpl, ImageEx);

    return pThis->QueryInterface(iid, ppvObject);
}

HRESULT CImageImpl::XImageEx::RenderEx(HDC hDC, LPCRECT pDstRect, VARIANT_BOOL bFrame)
{
    NESTED_CLASS_PROLOGUE(CImageImpl, ImageEx);

    return pThis->RenderEx(hDC, pDstRect, bFrame);
}

HRESULT CImageImpl::XImageEx::CreateHBITMAP(HDC hDC, HBITMAP* pHBITMAP)
{
    NESTED_CLASS_PROLOGUE(CImageImpl, ImageEx);

    return pThis->CreateHBITMAP(hDC, pHBITMAP);
}

#ifdef _DEBUG
DWORD CImageImpl::GetRefCount() const
{
    USE_CLASS_DATA(CLASS_DATA_MEMBER_LIST);

    return m_nRefCount;
}
#endif

BOOL CImageImpl::Load(LPCTSTR lpzFileName)
{
    USE_CLASS_DATA(CLASS_DATA_MEMBER_LIST);

    Clear();

    CFile file;
    if (! file.Open(lpzFileName, CFile::modeRead | CFile::shareDenyWrite | CFile::typeBinary)) {
        ASSERT(0 && _T("找不到文件或文件拒绝访问"));
        return FALSE;
    }

    ULONGLONG ullLen = file.GetLength();
    if (ullLen >= UINT_MAX) {
        ASSERT(0 && _T("文件过大"));
        return FALSE;
    }

    UINT fileLen = (UINT)ullLen;
    if (fileLen == 0)
        return FALSE;

    UINT memLen = fileLen + 2;
    std::vector<char> buffer;
    buffer.resize(memLen, 0);
    char* pBuffer = &(buffer[0]);

    file.Read(pBuffer, fileLen);
    file.Close();

    return Load(pBuffer, fileLen);
}

BOOL CImageImpl::Load(const char* pBuffer, long lBufSize)
{
    USE_CLASS_DATA(CLASS_DATA_MEMBER_LIST);

    Clear();

    BYTE* pBuf = const_cast<BYTE*>((BYTE*)pBuffer);
    m_pImage = new CxImage;
    m_pImage->SetRetreiveAllFrames(true);
    ENUM_CXIMAGE_FORMATS enFormat = CheckFormat(pBuf, lBufSize);
    if (! m_pImage->Decode(pBuf, lBufSize, enFormat))
    {
        Clear();
        return FALSE;
    }

    LONG lNumFrames = m_pImage->GetNumFrames();
    if (lNumFrames <= 1)
        return TRUE;

    CComPtr<IStream> spStream;
    HRESULT hr = CreateStreamOnMemory(pBuf, lBufSize, &spStream);
    if (hr != S_OK)
    {
        return FALSE;
    }

    m_pBitmap = Gdiplus::Bitmap::FromStream(spStream);
    return TRUE;
}

BOOL CImageImpl::Load(IStream* pStream)
{
    USE_CLASS_DATA(CLASS_DATA_MEMBER_LIST);

    Clear();

    CxIStreamFile xFile(pStream);
    m_pImage = new CxImage;
    m_pImage->SetRetreiveAllFrames(true);
    ENUM_CXIMAGE_FORMATS enFormat = CheckFormat(&xFile);
    if (! m_pImage->Decode(&xFile, enFormat)) 
    {
        Clear();
        return FALSE;
    }

    LONG lNumFrames = m_pImage->GetNumFrames();
    if (lNumFrames <= 1)
        return TRUE;

    m_pBitmap = Gdiplus::Bitmap::FromStream(pStream);
    return TRUE;
}

BOOL CImageImpl::Load(HBITMAP hBitmap)
{
    USE_CLASS_DATA(CLASS_DATA_MEMBER_LIST);

    Clear();

    m_pImage = new CxImage;
    if (! m_pImage->CreateFromHBITMAP(hBitmap))
    {
        Clear();
        return FALSE;
    }

    return TRUE;
}

BOOL CImageImpl::Load(HICON hIcon)
{
    USE_CLASS_DATA(CLASS_DATA_MEMBER_LIST);

    Clear();

    m_pImage = new CxImage;
    if (! m_pImage->CreateFromHICON(hIcon))
    {
        Clear();
        return FALSE;
    }

    return TRUE;
}

void CImageImpl::Clear()
{
    USE_CLASS_DATA(CLASS_DATA_MEMBER_LIST);

    if (m_pImage != NULL)
    {
        delete m_pImage;
        m_pImage = NULL;
    }

    if (m_pBitmap != NULL)
    {
        delete m_pBitmap;
        m_pBitmap = NULL;
    }
}

BOOL CImageImpl::Copy(CImageImpl* pImage)
{
    USE_CLASS_DATA(CLASS_DATA_MEMBER_LIST);

    if (this == pImage)
        return TRUE;

    Clear();

    if (pImage->m_pData->pImage != NULL) 
    {
        m_pImage = new CxImage;
        m_pImage->Copy(*(pImage->m_pData->pImage));
    }

    if (pImage->m_pData->pBitmap != NULL)
    {
        m_pBitmap = pImage->m_pData->pBitmap->Clone(0, 0, m_pBitmap->GetWidth(), m_pBitmap->GetHeight(), m_pBitmap->GetPixelFormat());
    }

    return TRUE;
}

static CxImage::InterpolationMethod ToInterpolationMethod(int iMode)
{
    static const CxImage::InterpolationMethod imTable[] = 
    {
        CxImage::IM_NEAREST_NEIGHBOUR,//=1,
        CxImage::IM_BILINEAR,//		=2,
        CxImage::IM_BSPLINE,//		=3,
        CxImage::IM_BICUBIC,//		=4,
        CxImage::IM_BICUBIC2,//		=5,
        CxImage::IM_LANCZOS,//		=6,
        CxImage::IM_BOX,//			=7,
        CxImage::IM_HERMITE,//		=8,
        CxImage::IM_HAMMING,//		=9,
        CxImage::IM_SINC,//			=10,
        CxImage::IM_BLACKMAN,//		=11,
        CxImage::IM_BESSEL,//		=12,
        CxImage::IM_GAUSSIAN,//		=13,
        CxImage::IM_QUADRATIC,//	=14,
        CxImage::IM_MITCHELL,//		=15,
        CxImage::IM_CATROM,//		=16,
        CxImage::IM_HANNING,//		=17,
        CxImage::IM_POWER,//		=18
    };
    static const int imTableSize = sizeof(imTable) / sizeof(CxImage::InterpolationMethod);

    ASSERT(iMode >= 0 && iMode < imTableSize);
    if (iMode < 0 && iMode >= imTableSize)
        return CxImage::IM_NEAREST_NEIGHBOUR;

    return imTable[iMode];
}

BOOL CImageImpl::Resample(UINT uWidth, UINT uHeight, int iMode)
{
    USE_CLASS_DATA(CLASS_DATA_MEMBER_LIST);

    if (m_pImage != NULL)
    {
        m_pImage->Resample2(uWidth, uHeight, ToInterpolationMethod(iMode));
    }

    return FALSE;
}

BOOL CImageImpl::Gray()
{
    USE_CLASS_DATA(CLASS_DATA_MEMBER_LIST);

    if (m_pImage != NULL)
    {
        return m_pImage->GrayScale();
    }

    return FALSE;
}

CImageImpl* CImageImpl::Clone(IImageManager* pManager)
{
    USE_CLASS_DATA(CLASS_DATA_MEMBER_LIST);

    CImageImpl* pNew = new CImageImpl(pManager);
    if (m_pImage != NULL)
    {
        pNew->m_pData->pImage = new CxImage(*m_pImage);
    }

    if (m_pBitmap != NULL)
    {
        pNew->m_pData->pBitmap = m_pBitmap->Clone(0, 0, m_pBitmap->GetWidth(), m_pBitmap->GetHeight(), m_pBitmap->GetPixelFormat());
    }

    return pNew;
}

IUnknown* CImageImpl::GetUnknown()
{
    USE_CLASS_DATA(CLASS_DATA_MEMBER_LIST);

    return static_cast<IUnknown*>(this);
}

IImageManager* CImageImpl::DetachImageManager()
{
    USE_CLASS_DATA(CLASS_DATA_MEMBER_LIST);

    IImageManager* pManager = m_pImageManager;
    m_pImageManager = NULL;
    return pManager;
}

} } //namespace common::ui
