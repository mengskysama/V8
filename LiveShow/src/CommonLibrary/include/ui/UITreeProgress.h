#pragma once

#include "../Common.h"
#include "iuitreenode.h"

namespace common { namespace ui
{

class IUITreeContainer;

class COMMON_INTERFACE CUITreeProgress :
    public IUITreeNode
{
public:
    CUITreeProgress(IUITreeContainer* pContainer);
    virtual ~CUITreeProgress(void);

public:
    virtual void Draw(CDC* pDC, const RECT& rcBound);
    virtual void SetRect(const RECT& rect);
    virtual void CalcSize(tagSIZE& size);

public:
    BOOL GetBitmapProgress();
    void SetBitmapProgress(BOOL bTrue);
    LONG GetProgress();
    void SetProgress(LONG lProgress);
    tagSIZE GetBorderOffset();
    void SetBorderOffset(const tagSIZE& szOffset);
    LONG GetBorderColor();
    void SetBorderColor(LONG clrColor);
    LONG GetProgressColor();
    void SetProgressColor(LONG clrColor);
    HBITMAP GetBorderBitmap();
    void SetBorderBitmap(HBITMAP hBitmap);
    HBITMAP GetProgressBitmap();
    void SetProgressBitmap(HBITMAP hBitmap);
    tagSIZE GetBorderCorner();
    void SetBorderCorner(const tagSIZE& szCorner);
    tagSIZE GetProgressCorner();
    void SetProgressCorner(const tagSIZE& szCorner);

protected:
    BOOL m_bBitmapProgress;     //是否是位图进度调

    LONG m_lProgress;           //进度

    //
    CSize m_szBorderOffset;     //进度条和边框的距离

    //非位图进度条属性
    LONG m_clrBorderColor;      //进度条的边线颜色
    LONG m_clrProgressColor;    //进度条的颜色

    //位图进度条属性
    HBITMAP m_hBorderBitmap;    //进度条的外框位图
    HBITMAP m_hProgressBitmap;  //进度条的位图
    CSize m_szBorderCorner;     //外框的边角尺寸
    CSize m_szProgressCorner;   //进度条的边角尺寸
};

} } //namespace common::ui
