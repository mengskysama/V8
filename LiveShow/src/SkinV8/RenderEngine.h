#pragma once

class CRenderEngine
{
private:
    CRenderEngine(){};
    ~CRenderEngine(){};

public:
    //////////////////////////////////////////////////////////////////////////
    //创建区域
    //////////////////////////////////////////////////////////////////////////
    //创建圆角矩形
    static HRGN CreateRountRectRgn(long left, long top, long right, long bottom, long diameter, bool bNULL = true);
    //创建上下圆角半径可分别设置的区域
    static HRGN CreateUpDownRoundRectRgn(long left, long top, long right, long bottom, long upDiameter, long downDiameter, bool bNULL = true);
    //创建上下圆角半径可分别设置的区域
    static HRGN CreateUpDownRoundRectRgn(long width, long height, long upDiameter, long downDiameter, bool bNULL = true);

    //////////////////////////////////////////////////////////////////////////
    //绘制线
    //////////////////////////////////////////////////////////////////////////
    //绘制直线
    static BOOL RenderLine(HDC hDC, const POINT& start, const POINT& end, LONG lineColor, LONG lineWidth = 1);
    //绘制矩形
    static BOOL RenderRect(HDC hDC, const RECT& rect, LONG color, LONG lineWidth = 1);
    //绘制矩形
    static BOOL Render3DRect(HDC hDC, const RECT& rect, LONG topLeftColor, LONG bottomRightColor);
    //绘制焦点矩形
    static BOOL RenderFocusRect(HDC hDC, const RECT& rect);
    //绘制区域
    static BOOL RenderRgn(HDC hDC, HRGN hRgn, LONG color, LONG lineWidth = 1);

    //////////////////////////////////////////////////////////////////////////
    //填充
    //////////////////////////////////////////////////////////////////////////
    //填充矩形
    static BOOL FillRect(HDC hDC, const RECT& rect, LONG color);
    //填充阴影矩形
    static BOOL FillShadowRect(HDC hDC, const RECT& rect, LONG diameter, LONG centerColor = 0xFF000000, LONG surroundColor = 0x00FFFFFF);
    //填充区域
    static BOOL FillRgn(HDC hDC, HRGN hRgn, LONG color);
};
