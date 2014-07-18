#pragma once

#include "../Common.h"
#include "./ClassDataMacro.h"

struct IImage;
struct tagANIMATEINFO;
struct tagFRAMEINFO;

namespace common { namespace ui
{

class IImageManager;

/**
@class CImageImpl
@brief IImage的实现
*/
class COMMON_INTERFACE CImageImpl : 
    public IImage
{
public:
    /**
    @fn CImageImpl(IImageManager* pManager)
    @brief 构造函数
    @param [in] pManager 图片管理对象
    */
    CImageImpl(IImageManager* pManager = NULL);

    /**
    @fn void Destroy()
    @brief 销毁函数
    */
    void Destroy();

protected:
    /**
    @fn ~CImageImpl(void)
    @brief 析构函数，为保护访问，防止栈构造
    */
    virtual ~CImageImpl(void);

    //for IUnknown and IImage
public:
    /** 
    @addtogroup IImage
    @brief  IImage 实现 
    */ 
    //@{
    STDMETHOD_(ULONG, AddRef)();
    STDMETHOD_(ULONG, Release)();
    STDMETHOD(QueryInterface)(REFIID iid, void** ppvObject);
    STDMETHOD(get_Width)(UINT* pWidth);
    STDMETHOD(get_Height)(UINT* pHeight);
    STDMETHOD(Render)(HDC hDC, LPCRECT pDstRect, LPCRECT pSrcRect = NULL, LPCRECT pBoundRect = NULL);
    //@}

    /** 
    @addtogroup IAnimation
    @brief  IAnimation 实现 
    */ 
    //@{
    STDMETHOD(GetNumFrames)(UINT* pNumFrames);
    STDMETHOD(GetAnimationInfo)(LPANIMATEINFO pAnimationInfo);
    STDMETHOD(GetFrameInfo)(UINT uFrame, LPFRAMEINFO pFrameInfo);
    STDMETHOD(RenderFrame)(HDC hDC, UINT uFrame, LPCRECT pDstRect);
    //@}

    /** 
    @addtogroup IImageEx
    @brief  IImageEx 实现 
    */ 
    //@{
    STDMETHOD(RenderEx)(HDC hDC, LPCRECT pDstRect, VARIANT_BOOL bFrame);
    STDMETHOD(CreateHBITMAP)(HDC hDC, HBITMAP* pHBITMAP);
    //@}

private:
    /**
    @class XAnimation
    @brief IAnimation 内嵌类
    */
    class XAnimation : public IAnimation
    {
    public:
        STDMETHOD_(ULONG, AddRef)();
        STDMETHOD_(ULONG, Release)();
        STDMETHOD(QueryInterface)(REFIID iid, void** ppvObject);
        STDMETHOD(GetNumFrames)(UINT* pNumFrames);
        STDMETHOD(GetAnimationInfo)(LPANIMATEINFO pAnimationInfo);
        STDMETHOD(GetFrameInfo)(UINT uFrame, LPFRAMEINFO pFrameInfo);
        STDMETHOD(RenderFrame)(HDC hDC, UINT uFrame, LPCRECT pDstRect);
    };

    /**
    @class XImageEx
    @brief IImageEx 内嵌类
    */
    class XImageEx : public IImageEx
    {
    public:
        STDMETHOD_(ULONG, AddRef)();
        STDMETHOD_(ULONG, Release)();
        STDMETHOD(QueryInterface)(REFIID iid, void** ppvObject);
        STDMETHOD(RenderEx)(HDC hDC, LPCRECT pDstRect, VARIANT_BOOL bFrame);
        STDMETHOD(CreateHBITMAP)(HDC hDC, HBITMAP* pHBITMAP);
    };

public:
#ifdef _DEBUG
    /**
    @fn DWORD GetRefCount() const
    @brief 获得引用计数，用于调试
    @return 返回引用计数
    */
    DWORD GetRefCount() const;
#endif

public:
    /**
    @fn BOOL Load(LPCTSTR lpzFileName)
    @brief 从文件加载
    @param [in] lpzFileName 文件路径名
    @return 是否加载成功
    */
    BOOL Load(LPCTSTR lpzFileName);

    /**
    @fn BOOL Load(const char* pBuffer, long lBufSize)
    @brief 从内存加载
    @param [in] pBuffer 内存指针
    @param [in] lBufSize 内存长度
    @return 是否加载成功
    */
    BOOL Load(const char* pBuffer, long lBufSize);

    /**
    @fn BOOL Load(IStream* pStream)
    @brief 从IStream加载
    @param [in] pSteam ISteam指针
    @return 是否加载成功
    */
    BOOL Load(IStream* pStream);

    /**
    @fn BOOL Load(HBITMAP hBitmap)
    @brief 从位图加载
    @param [in] hBitmap 位图
    @return 是否加载成功
    */
    BOOL Load(HBITMAP hBitmap);

    /**
    @fn BOOL Load(HICON hIcon)
    @brief 从图标加载
    @param [in] hIcon 图标
    @return 是否加载成功
    */
    BOOL Load(HICON hIcon);

    /**
    @fn BOOL Resample(UINT uWidth, UINT uHeight, int iMode = 1)
    @brief 重采样
    @param [in] uWidth 采样后的宽度
    @param [in] uHeight 采样后的高度
    @param [in] iMode 采样的模式
    @return 是否采样成功
    */
    BOOL Resample(UINT uWidth, UINT uHeight, int iMode = 1);

    /**
    @fn BOOL Gray()
    @brief 是图像灰度化
    @return 是否成功
    */
    BOOL Gray();

    /**
    @fn BOOL Copy(CImageImpl* pImage)
    @brief 拷贝图像
    @param [in] pImage 拷贝源图像
    @return 是否拷贝成功
    */
    BOOL Copy(CImageImpl* pImage);

    /**
    @fn CImageImpl* Clone(IImageManager* pManager)
    @brief 克隆
    @param [in] pManager 图片管理器指针
    @return 新的图片对象
    */
    CImageImpl* Clone(IImageManager* pManager);

    /**
    @fn IUnknown* GetUnknown()
    @brief 获得IUnknown指针
    @return 返回IUnknown指针
    */
    IUnknown* GetUnknown();

    /**
    @fn IImageManager* DetachImageManager()
    @brief 分离图片管理
    @return 返回托管的图片管理对象
    */
    IImageManager* DetachImageManager();

protected:
    /**
    @fn void Clear()
    @brief 清空对象
    */
    void Clear();

private:
    XAnimation m_xAnimation;
    XImageEx m_xImageEx;
    DECLARE_CLASS_DATA(CImageImpl);
};

} } //namespace common::ui
