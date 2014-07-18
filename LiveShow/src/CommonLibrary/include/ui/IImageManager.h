#pragma once

namespace common { namespace ui
{

class CImageImpl;

/**
@class IImageManager
@brief  IImage管理类
*/
class IImageManager
{
public:
    /**
    @fn void NotifyWeak(CImageImpl* pThis)
    @brief 通知管理类，该CImageImpl对象已无引用
    @param [in] pThis 无引用的CImageImpl对象
    */
    virtual void NotifyWeak(CImageImpl* pThis) = 0;
};

} } //namespace common::ui
