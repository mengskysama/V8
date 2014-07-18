#include "StdAfx.h"
#include "RenderEngine.h"

#ifdef USE_GDIPLUS
using namespace Gdiplus;
//使用GDIPLUS绘制
//#define GDIPLUS_RENDER
#endif


//////////////////////////////////////////////////////////////////////////
//Helper函数
//////////////////////////////////////////////////////////////////////////
#ifdef GDIPLUS_RENDER

static void ScaleLine(PointF& start, PointF& end, REAL scale)
{
    PointF centerP = start + end;
    centerP.X /= 2.0f;
    centerP.Y /= 2.0f;
    REAL xOffset = start.X - centerP.X;
    REAL yOffset = start.Y - centerP.Y;
    start.X = centerP.X + xOffset * scale;
    start.Y = centerP.Y + yOffset * scale;
    end.X = centerP.X - xOffset * scale;
    end.Y = centerP.Y - yOffset * scale;
}

#endif //GDIPLUS_RENDER

static BOOL RenderLine(HDC hDC, const POINT& start, const POINT& end)
{
    ::MoveToEx(hDC, start.x, start.y, NULL);

    return ::LineTo(hDC, end.x, end.y);
}

//////////////////////////////////////////////////////////////////////////
//CRenderEngine成员函数
//////////////////////////////////////////////////////////////////////////
HRGN CRenderEngine::CreateRountRectRgn(long left, long top, long right, long bottom, long diameter, bool bNULL)
{
    return ::CreateRoundRectRgn(left, top, right, bottom, diameter, diameter);
}

HRGN CRenderEngine::CreateUpDownRoundRectRgn(long left, long top, long right, long bottom, long upDiameter, long downDiameter, bool bNULL)
{
    //为非负数
    if (upDiameter < 0)
        upDiameter = 0;
    if (downDiameter < 0)
        downDiameter = 0;

    if (bNULL && 
        upDiameter == 0 && 
        downDiameter == 0)
        return NULL;

#ifdef GDIPLUS_RENDER

    ////通过圆弧创建圆角矩形的路径
    //CRect rect(left, top, right, bottom);
    //Rect upRect(0, 0, upDiameter, upDiameter);
    //Rect downRect(0, 0, downDiameter, downDiameter);
    //GraphicsPath path;
    //Rect tempRect;
    //path.StartFigure();

    //tempRect = upRect; //右下
    //tempRect.Offset(rect.right - downDiameter, rect.bottom - downDiameter);
    //path.AddArc(tempRect, 0, 90);

    //tempRect = upRect; //左下
    //tempRect.Offset(rect.left, rect.bottom - downDiameter);
    //path.AddArc(tempRect, 90, 90);

    //tempRect = downRect; //左上
    //tempRect.Offset(rect.left, rect.top);
    //path.AddArc(tempRect, 180, 90);

    //tempRect = downRect; //右上
    //tempRect.Offset(rect.right - upDiameter, rect.top);
    //path.AddArc(tempRect, 270, 90);

    //path.CloseFigure();

    //用Bezier曲线创建圆角矩形的路径
    RectF rect((REAL)left, (REAL)top, (REAL)(right - left), (REAL)(bottom - top));
    GraphicsPath path;
    REAL upRadius = (REAL)upDiameter;
    REAL downRadius = (REAL)downDiameter;
    REAL upOffset = (REAL)upRadius * 0.3f;
    REAL downOffset = (REAL)downRadius * 0.3f;
    REAL scale = 0.3f;
    PointF start, control1, control2, end;
    path.StartFigure();

    //右下
    start = PointF(rect.GetRight(), rect.GetBottom() - downRadius - 3);
    end = PointF(rect.GetRight() - downRadius - 3, rect.GetBottom());
    control1 = PointF(start.X + downOffset, start.Y + downOffset);
    control2 = PointF(end.X + downOffset, end.Y + downOffset);
    ScaleLine(control1, control2, scale);
    path.AddBezier(start, control1, control2, end);

    //左下
    start = PointF(rect.GetLeft() + downRadius + 4, rect.GetBottom());
    end = PointF(rect.GetLeft(), rect.GetBottom() - downRadius);
    control1 = PointF(start.X - downOffset, start.Y + downOffset);
    control2 = PointF(end.X - downOffset, end.Y + downOffset);
    ScaleLine(control1, control2, scale);
    path.AddBezier(start, control1, control2, end);

    //左上
    start = PointF(rect.GetLeft(), rect.GetTop() + upRadius);
    end = PointF(rect.GetLeft() + upRadius, rect.GetTop());
    control1 = PointF(start.X - upOffset, start.Y - upOffset);
    control2 = PointF(end.X - upOffset, end.Y - upOffset);
    ScaleLine(control1, control2, scale);
    path.AddBezier(start, control1, control2, end);

    //右上
    start = PointF(rect.GetRight() - upRadius, rect.GetTop());
    end = PointF(rect.GetRight(), rect.GetTop() + upRadius + 4);
    control1 = PointF(start.X + upOffset, start.Y - upOffset);
    control2 = PointF(end.X + upOffset, end.Y - upOffset);
    ScaleLine(control1, control2, scale);
    path.AddBezier(start, control1, control2, end);

    path.CloseFigure();

    Region rgn(&path);
    Graphics graphic((HDC)NULL);
    return rgn.GetHRGN(&graphic);

#else 

    const long width = right - left;
    const long height = bottom - top;

    HRGN hResult = NULL;
    if (upDiameter == downDiameter) {
        hResult = ::CreateRoundRectRgn(left, top, right, bottom, upDiameter, upDiameter);
    } else {
        const long halfHeight = height / 2;

        //计算上半部分
        HRGN upRectRgn = ::CreateRectRgn(left, top, right, top+halfHeight);
        HRGN upRgn = NULL;
        if (upDiameter > 0){
            upRgn = ::CreateRoundRectRgn(left, top, right, bottom, upDiameter, upDiameter);
            ::CombineRgn(upRgn, upRgn, upRectRgn, RGN_AND);
            ::DeleteObject(upRectRgn);
        } else {
            upRgn = upRectRgn;
        }
        upRectRgn = NULL;

        //计算下半部分
        HRGN downRectRgn = ::CreateRectRgn(left, halfHeight, right, bottom);
        HRGN downRgn = NULL;
        if (downDiameter > 0){
            downRgn = ::CreateRoundRectRgn(left, top, right, bottom, downDiameter, downDiameter);
            ::CombineRgn(downRgn, downRgn, downRectRgn, RGN_AND);
            ::DeleteObject(downRectRgn);
        } else {
            downRgn = downRectRgn;
        }
        downRectRgn = NULL;

        ::CombineRgn(upRgn, upRgn, downRgn, RGN_OR);
        ::DeleteObject(downRgn);

        hResult = upRgn;
    }

    if (downDiameter == 7 && upDiameter == 7) {
        CRect rect;
        HRGN hDiff = NULL;

        rect = CRect(0, 0, 1, 1);
        rect.OffsetRect(1, 1);
        hDiff = ::CreateRectRgnIndirect(&rect);
        ::CombineRgn(hResult, hResult, hDiff, RGN_DIFF);
        ::DeleteObject(hDiff);

        rect = CRect(0, 0, 1, 1);
        rect.OffsetRect(width - 3, 1);
        hDiff = ::CreateRectRgnIndirect(&rect);
        ::CombineRgn(hResult, hResult, hDiff, RGN_DIFF);
        ::DeleteObject(hDiff);

        rect = CRect(0, 0, 1, 1);
        rect.OffsetRect(1, height - 3);
        hDiff = ::CreateRectRgnIndirect(&rect);
        ::CombineRgn(hResult, hResult, hDiff, RGN_DIFF);
        ::DeleteObject(hDiff);

        rect = CRect(0, 0, 1, 1);
        rect.OffsetRect(width - 3, height - 3);
        hDiff = ::CreateRectRgnIndirect(&rect);
        ::CombineRgn(hResult, hResult, hDiff, RGN_DIFF);
        ::DeleteObject(hDiff);
    }

    return hResult;

#endif //GDIPLUS_RENDER

}

HRGN CRenderEngine::CreateUpDownRoundRectRgn(long width, long height, long upDiameter, long downDiameter, bool bNULL)
{
#ifdef GDIPLUS_RENDER
    return CreateUpDownRoundRectRgn(0, 0, width, height, upDiameter, downDiameter, bNULL);
#else
    return CreateUpDownRoundRectRgn(0, 0, width+1, height+1, upDiameter, downDiameter, bNULL);
#endif //GDIPLUS_RENDER
}

BOOL CRenderEngine::RenderLine(HDC hDC, const POINT& start, const POINT& end, LONG lineColor, LONG lineWidth)
{
    HPEN hPen = ::CreatePen(PS_SOLID, lineWidth, lineColor);
    HPEN hOldPen = (HPEN)::SelectObject(hDC, (HGDIOBJ)hPen);
    BOOL ret = ::RenderLine(hDC, start, end);
    ::SelectObject(hDC, (HGDIOBJ)hOldPen);
    ::DeleteObject(hPen);
    return ret;
}

BOOL CRenderEngine::RenderRect(HDC hDC, const RECT& rect, LONG color, LONG lineWidth)
{
    HBRUSH hBrush = ::CreateSolidBrush(color);
    HRGN hRgn = ::CreateRectRgnIndirect(&rect);
    BOOL ret = ::FrameRgn(hDC, hRgn, hBrush, lineWidth, lineWidth);
    ::DeleteObject(hRgn);
    ::DeleteObject(hBrush);
    return ret;
}

BOOL CRenderEngine::Render3DRect(HDC hDC, const RECT& rect, LONG topLeftColor, LONG bottomRightColor)
{
    CDC* pDC = CDC::FromHandle(hDC);
    pDC->Draw3dRect(&rect, topLeftColor, bottomRightColor);
    return TRUE;
}

BOOL CRenderEngine::RenderFocusRect(HDC hDC, const RECT& rect)
{
    return ::DrawFocusRect(hDC, &rect);
}

BOOL CRenderEngine::RenderRgn(HDC hDC, HRGN hRgn, LONG color, LONG lineWidth)
{
    HBRUSH hBrush = ::CreateSolidBrush(color);
    BOOL ret = ::FrameRgn(hDC, hRgn, hBrush, lineWidth, lineWidth);
    ::DeleteObject(hBrush);
    return ret;
}

BOOL CRenderEngine::FillRect(HDC hDC, const RECT& rect, LONG color)
{
    HBRUSH hBrush = ::CreateSolidBrush(color);
    BOOL ret = ::FillRect(hDC, &rect, hBrush);
    ::DeleteObject(hBrush);
    return ret;
}

BOOL CRenderEngine::FillShadowRect(HDC hDC, const RECT& rect, LONG diameter, LONG centerColor, LONG surroundColor)
{
    if (hDC == NULL)
        return FALSE;

    if (diameter < 0)
        diameter = 0;

#ifdef GDIPLUS_RENDER
    //创建图形设备
    Graphics graphic(hDC);

    Rect radiusRect(0, 0, diameter, diameter);

    //创建圆角矩形的路径
    GraphicsPath path;
    Rect tempRect;
    path.StartFigure();
    tempRect = radiusRect; //右下
    tempRect.Offset(rect.right - diameter, rect.bottom - diameter);
    path.AddArc(tempRect, 0, 90);
    tempRect = radiusRect; //左下
    tempRect.Offset(rect.left, rect.bottom - diameter);
    path.AddArc(tempRect, 90, 90);
    tempRect = radiusRect; //左上
    tempRect.Offset(rect.left, rect.top);
    path.AddArc(tempRect, 180, 90);
    tempRect = radiusRect; //右上
    tempRect.Offset(rect.right - diameter, rect.top);
    path.AddArc(tempRect, 270, 90);
    path.CloseFigure();

    //创建路径画刷
    PathGradientBrush gradientBrush(&path);
    Color centerClr(centerColor);
    Color surroundClr(surroundColor);
    int surroundClrCount = 1;
    gradientBrush.SetCenterColor(centerClr);
    gradientBrush.SetSurroundColors(&surroundClr, &surroundClrCount);

    graphic.FillPath(&gradientBrush, &path);

#endif //GDIPLUS_RENDER

    return TRUE;
}

BOOL CRenderEngine::FillRgn(HDC hDC, HRGN hRgn, LONG color)
{
    HBRUSH hBrush = ::CreateSolidBrush(color);
    BOOL ret = ::FillRgn(hDC, hRgn, hBrush);
    ::DeleteObject(hBrush);
    return ret;
}
