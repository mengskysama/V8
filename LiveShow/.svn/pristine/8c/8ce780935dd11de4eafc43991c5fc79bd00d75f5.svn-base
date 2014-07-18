#pragma once

#include "../Common.h"
#include "./ClassDataMacro.h"

#include "ImageWrapper.h"

namespace common { namespace ui
{

/**
@class IAnimatorCallback
@brief 动画绘制回调接口
*/
class IAnimatorCallback
{
public:
    /**
    @fn virtual void PrePaint(HDC hDC, const RECT& rcRect)
    @brief 绘制前接口
    */
    virtual void PrePaint(HDC /*hDC*/, const RECT& /*rcRect*/) {};

    /**
    @fn virtual void PostPaint(HDC hDC, const RECT& rcRect)
    @brief 绘制后接口
    */
    virtual void PostPaint(HDC /*hDC*/, const RECT& /*rcRect*/) {};
};

/**
@class CAnimator
*/
class COMMON_INTERFACE CAnimator
{
public:
    /**
    @fn CAnimator()
    @brief 构造函数
    */
    CAnimator();

    /**
    @fn virtual ~CAnimator()
    @brief 析构函数
    */
    virtual ~CAnimator();

public:
    /**
    @fn CImageWrapper GetImage()
    @brief 获得图像
    @return 返回图像
    */
    CImageWrapper GetImage();

    /**
    @fn void SetImage(CImageWrapper image)
    @brief 设置图像
    @param [in] iw 图像
    */
    void SetImage(CImageWrapper image);

    /**
    @fn BOOL Load(LPCTSTR lpzFilePath)
    @brief 从文件加载
    @param [in] lpzFilePath 文件路径
    @return 是否加载成功
    */
    BOOL Load(LPCTSTR lpzFilePath);

    /**
    @fn void SetPaintInfo(HWND hWnd, const RECT& rcDraw)
    @brief 设置要绘制的窗口信息
    @param [in] hWnd 窗口句柄
    @param [in] rcDraw 要绘制的区域
    */
    void SetPaintInfo(HWND hWnd, const RECT& rcDraw);

    /** 
    @addtogroup BackgroundColor
    @brief 背景色
    */ 
    //@{
    COLORREF GetBackgroundColor();
    void SetBackgroundColor(COLORREF clrColor);
    //@}

    /**
    @fn void Play()
    @brief 播放
    */
    void Play();

    /**
    @fn void Stop()
    @brief 停止
    */
    void Stop();

    /**
    @fn BOOL IsAnimating()
    @brief 是否在播放
    @return 是否在播放
    */
    BOOL IsAnimating();

    /**
    @fn BOOL Draw(HDC hDC, const RECT& rcDraw)
    @brief 绘制当前帧
    @param [in] hDC DC
    @param [in] rcDraw 绘制的区域
    @return 是否绘制成功
    */
    BOOL Draw(HDC hDC, const RECT& rcDraw);

    /**
    @fn void SetAnimatorCallback(IAnimatorCallback* pCallback)
    @brief 设置动画回调
    @param [in] pCallback 回调接口
    */
    void SetAnimatorCallback(IAnimatorCallback* pCallback);

public:
    /**
    @fn void Animate()
    @brief 动画事件调用
    */
    void Animate();

private:
    DECLARE_CLASS_DATA(CAnimator)
};

/**
@class CAnimatorWnd
@brief 动画控件的窗口实现
*/
class COMMON_INTERFACE CAnimatorWnd :
    public CWnd,
    public CAnimator
{
    DECLARE_DYNAMIC(CAnimatorWnd)

public:
    /**
    @fn CAnimatorWnd()
    @brief 构造函数
    */
    CAnimatorWnd();

    /**
    @fn ~CAnimatorWnd()
    @brief 析构函数
    */
    ~CAnimatorWnd();

public:
    /**
    @fn BOOL Create(CWnd* pParent, const RECT& rcRect, UINT uID)
    @brief 创建窗口
    @param [in] pParent 父窗口
    @param [in] rcRect 创建的区域
    @param [in] uID 控件ID
    @return 是否创建成功
    */
    BOOL Create(CWnd* pParent, const RECT& rcRect, UINT uID);

protected:
    DECLARE_MESSAGE_MAP()

protected:
    /** 
    @addtogroup 消息响应
    @brief 消息响应
    */ 
    //@{
    /**
    @fn int OnCreate(LPCREATESTRUCT lpCreateStruct)
    @brief WM_CREATE响应
    */
    int OnCreate(LPCREATESTRUCT lpCreateStruct);

    /**
    @fn void OnDestroy()
    @brief WM_DESTROY响应
    */
    void OnDestroy();

    /**
    @fn void OnSize(UINT nType, int cx, int cy)
    @brief WM_SIZE响应
    */
    void OnSize(UINT nType, int cx, int cy);

    /**
    @fn BOOL OnEraseBkgnd(CDC* pDC)
    @brief WM_ERASEBKGND响应
    */
    BOOL OnEraseBkgnd(CDC* pDC);

    /**
    @fn void OnPaint()
    @brief WM_PAINT响应
    */
    void OnPaint();
    //@}
};

} } //namespace common::ui
