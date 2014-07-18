#pragma once


#include "../Common.h"
#include <string>

namespace common { namespace ui
{
    /**
    @fn BOOL ClientToScreen(HWND hWnd, LPRECT lpRect)
    @brief 客户区坐标转换为屏幕区坐标
    @param [in] hWnd 窗口句柄
    @param [in, out] lpRect 待转换的矩形
    @return 是否成功
    */
    inline BOOL ClientToScreen(HWND hWnd, LPRECT lpRect)
    {
        ::ClientToScreen(hWnd, (LPPOINT)(&(lpRect->left)));
        return ::ClientToScreen(hWnd, (LPPOINT)(&(lpRect->right)));
    };

    /**
    @fn BOOL ScreenToClient(HWND hWnd, LPRECT lpRect)
    @brief 屏幕区坐标转换为客户区坐标
    @param [in] hWnd 窗口句柄
    @param [in, out] lpRect 待转换的矩形
    @return 是否成功
    */
    inline BOOL ScreenToClient(HWND hWnd, LPRECT lpRect)
    {
        ::ScreenToClient(hWnd, (LPPOINT)(&(lpRect->left)));
        return ::ScreenToClient(hWnd, (LPPOINT)(&(lpRect->right)));
    };

    /**
    @fn void SetRectWidth(LPRECT lpRect, LONG nWidthNew, BOOL bFromLeft = TRUE)
    旧版 void SetWidth(LPRECT lpRect, LONG nWidthNew, BOOL bFromLeft = TRUE)
    @brief 设置矩形宽度
    @param [in, out] lpRect 待设置的矩形
    @param [in] nWidthNew 新的宽度
    @param [in] bFromLeft 是否参考左边
    */
    inline void SetRectWidth(LPRECT lpRect, LONG nWidthNew, BOOL bFromLeft = TRUE)
    {
        ASSERT(lpRect);
        if (bFromLeft)
        {
            lpRect->right = lpRect->left + nWidthNew;
        }
        else
        {
            lpRect->left = lpRect->right - nWidthNew;
        }
    };

    /**
    @fn void SetRectHeight(LPRECT lpRect, LONG nHeightNew, BOOL bFromTop = TRUE)
    旧版 void SetHeight(LPRECT lpRect, LONG nHeightNew, BOOL bFromTop = TRUE)
    @brief 设置矩形宽度
    @param [in, out] lpRect 待设置的矩形
    @param [in] nHeightNew 新的高度
    @param [in] bFromTop 是否参考上边
    */
    inline void SetRectHeight(LPRECT lpRect, LONG nHeightNew, BOOL bFromTop = TRUE)
    {
        ASSERT(lpRect);
        if (bFromTop)
        {
            lpRect->bottom = lpRect->top + nHeightNew;
        }
        else
        {
            lpRect->top = lpRect->bottom - nHeightNew;
        }
    };

    /**
    @fn void CenterRectY(LPRECT lpRect, LPCRECT lpRef, LONG nHeight)
    @brief Y方向居中矩形
    @param [in, out] lpRect 待设置的矩形
    @param [in] lpRef 参考的矩形
    @param [in] nHeight 新的高度
    */
    inline void CenterRectY(LPRECT lpRect, LPCRECT lpRef, LONG nHeight)
    {
        //ASSERT((lpRef->bottom - lpRef->top) > 0);
        int nMidY = (lpRef->top + lpRef->bottom) / 2;
        lpRect->top = nMidY - nHeight / 2;
        lpRect->bottom = lpRect->top + nHeight;
    };

    /**
    @fn void CenterRectX(LPRECT lpRect, LPCRECT lpRef, LONG nWidth)
    @brief X方向居中矩形
    @param [in, out] lpRect 待设置的矩形
    @param [in] lpRef 参考的矩形
    @param [in] nWidth 新的宽度
    */
    inline void CenterRectX(LPRECT lpRect, LPCRECT lpRef, LONG nWidth)
    {
        ASSERT((lpRef->right - lpRef->left) > 0);
        int nMidX = (lpRef->left + lpRef->right) / 2;
        lpRect->left = nMidX - nWidth / 2;
        lpRect->right = lpRect->left + nWidth;
    };

    /**
    @fn void CenterRect(LPRECT lpRect, LPCRECT lpRef, LONG nWidth, LONG nHeight)
    @brief 居中矩形
    @param [in, out] lpRect 待设置的矩形
    @param [in] lpRef 参考的矩形
    @param [in] nWidth 新的宽度
    @param [in] nHeight 新的高度
    */
    inline void CenterRect(LPRECT lpRect, LPCRECT lpRef, LONG nWidth, LONG nHeight)
    {
        CenterRectX(lpRect, lpRef, nWidth);
        CenterRectY(lpRect, lpRef, nHeight);
    };

    /**
    @fn void FrameCenterRect(LPRECT lpDest, LPCRECT lpFrame, LONG nWidth, LONG nHeight, BOOL bFill = FALSE)
    旧版 void CenterRect( LPRECT prcContainer, UINT nHeight, UINT nWidth, LPRECT prcDest)
    @brief 居中矩形，如超过外框范围，则缩小
    @param [in, out] lpDest 待设置的矩形
    @param [in] lpFrame 参考的矩形
    @param [in] nWidth 宽度
    @param [in] nHeight 高度
    @param [in] bFill 是否填充，为真时，即使不超过外框范围也放大到外框尺寸，但比例保持不变
    */
    inline void FrameCenterRect(LPRECT lpDest, LPCRECT lpFrame, LONG nWidth, LONG nHeight, BOOL bFill = FALSE)
    {
        ASSERT(lpFrame);
        ASSERT(lpDest);
        LONG nFrameWidth = lpFrame->right - lpFrame->left;
        LONG nFrameHeight = lpFrame->bottom - lpFrame->top;
        if ((nWidth > nFrameWidth || nHeight > nFrameHeight) && bFill) {
            *lpDest = *lpFrame;
            if ( (nFrameHeight * nWidth) > (nFrameWidth * nHeight) ) {
                //LONG newWidth = (nFrameWidth * nWidth) / nWidth;
                LONG newHeight = (nFrameWidth * nHeight) / nWidth;
                lpDest->top += (nFrameHeight - newHeight) / 2;
                lpDest->bottom = lpDest->top + newHeight;
            } else {
                LONG newWidth = (nFrameHeight * nWidth) / nHeight;
                //LONG newHeight = (nFrameHeight * nHeight) / nHeight;
                lpDest->left += (nFrameWidth - newWidth) / 2;
                lpDest->right = lpDest->left + newWidth;
            }
        } else {
            *lpDest = *lpFrame;
            ::InflateRect(lpDest, - (nFrameWidth - nWidth) / 2, - (nFrameHeight - nHeight) / 2);
        }
    };

    /**
    @fn void FrameCenterRect(LPRECT lpDest, LPCRECT lpFrame, BOOL bFill = FALSE)
    @brief 居中矩形，如超过外框范围，则缩小
    @param [in, out] lpDest 待设置的矩形
    @param [in] lpFrame 参考的矩形
    @param [in] bFill 是否填充，为真时，即使不超过外框范围也放大到外框尺寸，但比例保持不变
    */
    inline void FrameCenterRect(LPRECT lpDest, LPCRECT lpFrame, BOOL bFill = FALSE)
    {
        LONG nWidth = lpDest->right - lpDest->left;
        LONG nHeight = lpDest->bottom - lpDest->top;
        FrameCenterRect(lpDest, lpFrame, nWidth, nHeight, bFill);
    };

} } //namespace common::ui

namespace common { namespace ui
{
    /**
    @class CClientRect
    @brief 获得窗口的客户区矩形
    */
    class CClientRect : public CRect
    {
    public:
        /**
        @fn CClientRect(HWND hWnd, HWND hWndRef = NULL)
        @brief 构造函数
        @param [in] hWnd 要获取客户区的窗口
        @param [in] hWndRef 参考窗口
        */
        CClientRect(HWND hWnd, HWND hWndRef = NULL)
        {
            CalcRect(hWnd, hWndRef);
        };

        /**
        @fn CClientRect(CWnd* pWnd, CWnd* pWndRef = NULL)
        @brief 构造函数，MFC支持
        @param [in] pWnd 要获取客户区的窗口
        @param [in] pWndRef 参考窗口
        */
        CClientRect(CWnd* pWnd, CWnd* pWndRef = NULL)
        {
            ASSERT(pWnd != NULL);
            CalcRect(pWnd->GetSafeHwnd(), ((pWndRef != NULL) ? (pWndRef->GetSafeHwnd()) : (NULL)));
        };

    protected:
        /**
        @fn void CalcRect(HWND hWnd, HWND hWndRef)
        @brief 计算要获取的矩形
        @param [in] pWnd 要获取客户区的窗口
        @param [in] pWndRef 参考窗口
        */
        void CalcRect(HWND hWnd, HWND hWndRef)
        {
            ASSERT(::IsWindow(hWnd));
            ASSERT(hWndRef == NULL || ::IsWindow(hWndRef));
            ::GetClientRect(hWnd, this);
            if (hWndRef == NULL) 
                return;

            ClientToScreen(hWnd, this);
            ScreenToClient(hWndRef, this);
        };
    };

    /**
    @class CWindowRect
    @brief 获得窗口的矩形区
    */
    class CWindowRect : public CRect
    {
    public:
        /**
        @fn CWindowRect(HWND hWnd, HWND hWndRef = NULL)
        @brief 构造函数
        @param [in] hWnd 要获取窗口区的窗口
        */
        CWindowRect(HWND hWnd)
        {
            CalcRect(hWnd);
        };

        /**
        @fn CWindowRect(HWND hWnd, HWND hWndRef = NULL)
        @brief 构造函数，MFC支持
        @param [in] hWnd 要获取窗口区的窗口
        */
        CWindowRect(CWnd* pWnd)
        {
            ASSERT(pWnd != NULL);
            CalcRect(pWnd->GetSafeHwnd());
        };

    protected:
        /**
        @fn void CalcRect(HWND hWnd)
        @brief 计算要获取的矩形
        @param [in] pWnd 要获取窗口区的窗口
        */
        void CalcRect(HWND hWnd)
        {
            ASSERT(::IsWindow(hWnd));
            ::GetWindowRect(hWnd, this);
        };
    };

    /**
    @class CCursorPos
    @brief 获取鼠标当前相对窗口客户区的坐标
    */
    class CCursorPos : public CPoint
    {
    public:
        /**
        @fn CCursorPos(HWND hWnd = NULL)
        @brief 构造函数
        @param [in] hWndRef 参考窗口
        */
        CCursorPos(HWND hWndRef)
        {
            CalcPos(hWndRef);
        };

        /**
        @fn CCursorPos(CWnd* pWndRef = NULL)
        @brief 构造函数，MFC支持
        @param [in] pWndRef 参考窗口
        */
        CCursorPos(CWnd* pWndRef = NULL)
        {
            CalcPos((pWndRef != NULL) ? (pWndRef->GetSafeHwnd()) : (NULL));
        };

    protected:
        /**
        @fn void CalcRect(HWND hWnd)
        @brief 计算鼠标当前坐标
        @param [in] hWnd 参考窗口
        */
        void CalcPos(HWND hWnd)
        {
            ASSERT(hWnd == NULL || ::IsWindow(hWnd));
            ::GetCursorPos(this);
            if (hWnd == NULL)
                return;

            ::ScreenToClient(hWnd, this);
        };
    };
} } //namespace common::ui

#define NO_TRANSPARENT (-1)

namespace common { namespace ui
{
    /**
    @fn HANDLE LoadImageFromFile(LPCTSTR lpzFilePath, UINT uType = IMAGE_BITMAP)
    @brief 从文件加载图片，默认为位图
    @param [in] lpzFilePath 文件路径
    @param [in] uType 要加载的图片类型，默认为IMAGE_BITMAP
    @return 图片的句柄
    */
    HANDLE COMMON_INTERFACE LoadImageFromFile(LPCTSTR lpzFilePath, UINT uType = IMAGE_BITMAP);

    /**
    @fn HICON LoadIconFromModule()
    @brief 从模块加载图标
    @param [in] uID 图标资源ID
    @param [in] lpzModuleName 模块名字
    @return 加载的图标句柄
    */
    HICON COMMON_INTERFACE LoadIconFromModule(UINT uID, LPCTSTR lpzModuleName);

    /**
    @fn BOOL GetBitmapInfo(HBITMAP hBitmap, LPBITMAP lpBitmap)
    @brief 获得位图信息
    @param [in] hBitmap 位图句柄
    @param [out] bitmap BITMAP结构
    @return 是否获取成功
    */
    BOOL COMMON_INTERFACE GetBitmapInfo(HBITMAP hBitmap, BITMAP& bitmap);

    /**
    @fn SIZE GetBitmapSize(HBITMAP hBitmap)
    @brief 获得位图的尺寸
    @param [in] hBitmap 要绘制的位图
    @return 位图的尺寸
    */
    SIZE COMMON_INTERFACE GetBitmapSize(HBITMAP hBitmap);

    /**
    @fn SIZE GetIconSize(HICON hIcon)
    @brief 获得图标的大小
    @param [in] hIcon 图标句柄
    @return 图标的大小
    */
    SIZE COMMON_INTERFACE GetIconSize(HICON hIcon);

    /**
    @fn HICON CreateIconFromBitmap(HBITMAP hBitmap)
    旧版 HICON CxImage2Icon(CxImage* pImage)
    @brief 从位图创建图标
    @param [in] hBitmap 创建所需的位图
    @return 创建的Icon句柄
    */
    HICON COMMON_INTERFACE CreateIconFromBitmap(HBITMAP hBitmap);

    /**
    @fn BOOL DrawBitmapEx(HDC hDC, HBITMAP hBitmap, const RECT& dstRect, const RECT& srcRect, COLORREF clrTransColor = NO_TRANSPARENT)
    旧版 bool StretchBmp(CDC* pdc, HBITMAP hBmp, CRect rcDest, CRect rcSrc, COLORREF crTransparent = NO_TRANSPARENT)
    @brief 绘制位图的某个区域到DC的目标区域上
    @param [in] hDC 目标DC
    @param [in] hBitmap 位图
    @param [in] dstRect DC的目标区域
    @param [in] srcRect 位图的目标区域
    @param [in] clrTransColor 透明色
    @return 是否绘制成功
    */
    BOOL COMMON_INTERFACE DrawBitmapEx(HDC hDC, HBITMAP hBitmap, const RECT& dstRect, const RECT& srcRect, COLORREF clrTransColor = NO_TRANSPARENT);

    /**
    @fn BOOL DrawBitmap(HDC hDC, HBITMAP hBitmap, const RECT& dstRect, COLORREF clrTransColor = NO_TRANSPARENT)
    旧版 BOOL OutputBitmap(	CDC* pDC, CBitmap* pBmpOutput, int x, int y, int nWidth, int nHeight, 
            EPictureStretchType eStretchType = eStretchType_NoStretch, COLORREF crClippingColor = (-1),
            int xSrc = 0, int ySrc = 0, int nSrcWidth = -1, int nSrcHeight = -1)
    @brief 绘制位图
    @param [in] hDC 目标DC
    @param [in] hBitmap 要绘制的位图
    @param [in] dstRect 要绘制的矩形区域
    @param [in] clrTransColor 透明色，-1为无透明色
    @return 是否绘制成功
    */
    BOOL COMMON_INTERFACE DrawBitmap(HDC hDC, HBITMAP hBitmap, const RECT& dstRect, COLORREF clrTransColor = NO_TRANSPARENT);

    /**
    @fn BOOL DrawBitmap(HDC hDC, LPCTSTR lpzBitmapFile, const RECT& dstRect, COLORREF clrTransColor = NO_TRANSPARENT);
    @brief 绘制位图文件
    @param [in] hDC 目标DC
    @param [in] lpzBitmapFile 要绘制的位图文件
    @param [in] dstRect 要绘制的矩形区域
    @param [in] clrTransColor 透明色，-1为无透明色
    @return 是否绘制成功
    */
    BOOL COMMON_INTERFACE DrawBitmap(HDC hDC, LPCTSTR lpzBitmapFile, const RECT& dstRect, COLORREF clrTransColor = NO_TRANSPARENT);

    /**
    @fn BOOL DrawRoundBitmap(HDC hDC, HBITMAP hBitmap, const RECT& dstRect, SIZE szSide = {0, 0}, COLORREF clrTransColor = NO_TRANSPARENT)
    旧版 void UTILITY_API StretchBmpWithRoundRectangle(CDC* pdc, HBITMAP hbmp, CRect rcDest, CSize szCorner = CSize(3, 3), COLORREF crTransparent = NO_TRANSPARENT)
    @param [in] hDC 目标DC
    @param [in] hBitmap 位图
    @param [in] dstRect DC的目标区域
    @param [in] nSideWidth 边缘的宽度
    @param [in] nSideHeight 边缘的高度
    @param [in] clrTransColor 透明色
    @return 是否绘制成功
    */
    BOOL COMMON_INTERFACE DrawRoundBitmap(HDC hDC, HBITMAP hBitmap, const RECT& dstRect, LONG nSideWidth = 0, LONG nSideHeight = 0, COLORREF clrTransColor = NO_TRANSPARENT);

    /**
    @fn HBITMAP MixBitmap(HBITMAP hDstMix, HBITMAP hSrcMix, LONG xOffset, LONG yOffset)
    @brief 合并两个位图
    @param [in] hDstMix 目标位图
    @param [in] hSrcMix 源位图
    @param [in] xOffset x位移
    @param [in] yOffset y位移
    @return 合并后的位图
    */
    HBITMAP COMMON_INTERFACE MixBitmap(HBITMAP hDstMix, HBITMAP hSrcMix, LONG xOffset, LONG yOffset);

    /**
    @fn HBITMAP FrameBitmap(HBITMAP hBmp, COLORREF clrColor, LONG lineWidth = 1)
    @brief 给位图加边框，输出位图比源位图的大小大2倍的线宽
    @param [in] hBmp 源位图
    @param [in] clrInColor 内边框颜色
    @param [in] clrOutColor 外边框颜色
    @param [in] inLineWidth 内边框的宽度
    @param [in] outLineWidth 外边框的宽度
    @return 加边框后的位图
    */
    HBITMAP COMMON_INTERFACE FrameBitmap(HBITMAP hBmp, COLORREF clrInColor, COLORREF clrOutColor, UINT inLineWidth = 1, UINT outLineWidth = 1);

    /**
    @fn BOOL DrawRect(HDC hDC, const RECT& rcRect, COLORREF clrColor, int lineWidth = 1)
    @brief 绘制矩形
    @param [in] hDC 目标DC
    @param [in] rcRect 要绘制的矩形
    @param [in] clrColor 要绘制的颜色
    @param [in] lineWidth 绘制的线宽
    @return 是否绘制成功
    */
    BOOL COMMON_INTERFACE DrawRect(HDC hDC, const RECT& rcRect, COLORREF clrColor, int lineWidth = 1);

    /**
    @fn BOOL DrawLine(HDC hDC, const POINT& ptStart, const POINT& ptEnd, COLORREF clrColor, int lineWidth = 1)
    @brief 绘制线
    @param [in] hDC 目标DC
    @param [in] ptStart 线的起始点
    @param [in] ptEnd 线的终止点
    @param [in] clrColor 绘制的颜色
    @param [in] lineWidth 绘制的线宽
    @return 是否绘制成功
    */
    BOOL COMMON_INTERFACE DrawLine(HDC hDC, const POINT& ptStart, const POINT& ptEnd, COLORREF clrColor, int lineWidth = 1);

    /**
    @fn void FillGradient(HDC hDC, LPCRECT lpRect, COLORREF colorStart, COLORREF colorFinish, BOOL bHorz = TRUE)
    @brief 绘制线
    @param [in] hDC 目标DC
    @param [in] lpRect 区域
    @param [in] colorStart 起始色
    @param [in] colorFinish 终止色
    @param [in] bHorz 方向
    @return 是否绘制成功
    */    
    void COMMON_INTERFACE FillGradient(HDC hDC, LPCRECT lpRect, COLORREF colorStart, COLORREF colorFinish, BOOL bHorz = TRUE);

    /**
    @fn BOOL TrackMouseEvent(HWND hWnd)
    @brief 跟踪窗口的鼠标消息，产生WM_MOUSELEAVE和WM_MOUSEHOVER消息
    @param [in] hWnd 要跟踪的窗口
    @return 是否成功
    */
    BOOL COMMON_INTERFACE TrackMouseEvent(HWND hWnd);

    /**
    @fn BOOL CreateRichEditCharFormat(CHARFORMAT& cf, DWORD dwEffect, LONG lHeight, BYTE charSet, LPCTSTR lpzFaceName, COLORREF clrColor)
    旧版 void CreateCharFormatWithColor(CHARFORMAT &cfDefault, DWORD dwEffects, LONG yHeight, BYTE bCharSet, TCHAR szFaceName[], COLORREF crTextColor)
    @brief 生成RichEdit的CharFormat
    @param [out] cf 生成的CharFormat
    @param [in] dwEffects 效果
    @param [in] lHeight 字的高度
    @param [in] charSet 字符集
    @param [in] lpzFaceName 字体的名字
    @param [in] clrColor 字体的颜色
    @return 是否创建成功
    */
    BOOL COMMON_INTERFACE CreateRichEditCharFormat(CHARFORMAT& cf, DWORD dwEffects, LONG lHeight, BYTE charSet, LPCTSTR lpzFaceName, COLORREF clrColor);

    /**
    @fn SIZE GetTextExtent(HDC hDC, LPCTSTR lpzText, HFONT hFont = NULL)
    旧版 CSize GetTextExtent(IN CDC* pDC, IN CString sText, IN CFont* pFont = NULL)
    @brief 计算在目标DC和目标字体上绘制文字所需的大小，函数不影响DC和字体的状态
    @param [in] hDC 目标DC
    @param [in] lpzText 要计算的字符串
    @param [in] hFont 字体
    @return 返回绘制文字所需的大小
    */
    SIZE COMMON_INTERFACE GetTextExtent(HDC hDC, LPCTSTR lpzText, HFONT hFont = NULL);

    /**
    @fn std::wstring GetFittableString(HDC hDC, LPCTSTR lpzText, LONG nWidth, LONG nStart, HFONT hFont = NULL, LPLONG lpEnd = NULL)
    旧版 wstring GetFittableStringFromFront( CDC* pDC,
            CFont* pFont,
            wstring wstrText, 
            UINT nWidth, 
            UINT nBeginIndex, 
            UINT& nEndIndex)
    @brief 获得能在目标DC上指定宽度内绘制的字串的子字串
    @param [in] hDC 目标DC
    @param [in] lpzText 目标字符串
    @param [in] nWidth 指定的宽度
    @param [in] nStart 指定的开始
    @param [in] hFont 字体
    @param [out] lpEnd 子字符串的结束位置
    @return 返回子字符串
    */
    std::wstring COMMON_INTERFACE GetFittableString(HDC hDC, LPCTSTR lpzText, LONG nWidth, LONG nStart, HFONT hFont = NULL, LPLONG lpEnd = NULL);

    /**
    @fn int GetIconIndexFileType(LPCTSTR lpzExt, BOOL bLarge, BOOL bOpen)
    旧版 int GetIconIndexForFileType(wstring sFileext = L"txt", bool bLarge = false, bool bOpen = false)
    @param [in] lpzExt 文件后缀
    @param [in] bLarge 是否大图标
    @param [in] bOpen 是否打开状态
    @return 图标的索引值
    */
    int COMMON_INTERFACE GetFileTypeIconIndex(LPCTSTR lpzExt, BOOL bLarge, BOOL bOpen);
} } //namespace common::ui

namespace common { namespace ui
{
    /**
    @fn HRESULT CreateStreamOnMemory(IN void* pSrc, IN INT iSize, OUT LPSTREAM FAR* ppstm)
    @brief 从内存创建IStream对象，但此IStream对象并不与该内存关联
    @param [in] pSrc 内存首地址
    @param [in] iSize 内存的长度
    @param [out] ppstm 创建的IStream对象
    @return 返回COM错误代码
    */
    HRESULT COMMON_INTERFACE CreateStreamOnMemory(IN void* pSrc, IN INT iSize, OUT LPSTREAM FAR* ppstm);
} } //namespace common::ui

namespace common { namespace ui
{
    /**
    @fn INT ShowOpenFileDlg(OUT std::wstring& wstrFilePath, 
    IN OUT PBOOL pbReadOnly = NULL, 
    IN LPCTSTR lpszDefExt = NULL,
    IN LPCTSTR lpszFileName = NULL,
    IN LPCTSTR lpszFilter = NULL,	// eg: "图象文件(*.bmp;*.jpg;*.gif)|*.bmp;*.jpg;*.gif|所有文件(*.*)|*.*||"
    IN DWORD dwFlags = OFN_ENABLESIZING,
    IN CWnd* pParentWnd = NULL
    )
    @brief 打开文件对话框
    @param [out] wstrFilePath 文件路径
    @param [in, out] pbReadOnly 是否只读
    @param [in] lpszDefExt 文件后缀
    @param [in] lpszFileName 文件名
    @param [in] lpszFilter 文件过滤
    @param [in] dwFlags 标志位
    @param [in] hParentWnd 父窗口
    @return 返回码
    */
    INT COMMON_INTERFACE ShowOpenFileDlg(OUT std::wstring& wstrFilePath, 
        IN OUT PBOOL pbReadOnly = NULL, 
        IN LPCTSTR lpszDefExt = NULL,
        IN LPCTSTR lpszFileName = NULL,
        IN LPCTSTR lpszFilter = NULL,	// eg: "图象文件(*.bmp;*.jpg;*.gif)|*.bmp;*.jpg;*.gif|所有文件(*.*)|*.*||"
        IN DWORD dwFlags = OFN_ENABLESIZING,
        IN HWND hParentWnd = NULL
        );

    /**
    @fn INT ShowSaveFileDlg(OUT std::wstring& wstrFilePath, 
    IN LPCTSTR lpszDefExt = NULL,
    IN LPCTSTR lpszFileName = NULL,
    IN LPCTSTR lpszFilter = NULL,	// eg: "图象文件(*.bmp;*.jpg;*.gif)|*.bmp;*.jpg;*.gif|所有文件(*.*)|*.*||"
    IN DWORD dwFlags = OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
    IN CWnd* pParentWnd = NULL
    )
    @brief 保存文件对话框
    @param [out] wstrFilePath 文件路径
    @param [in] lpszDefExt 文件后缀
    @param [in] lpszFileName 文件名
    @param [in] lpszFilter 文件过滤
    @param [in] dwFlags 标志位
    @param [in] hParentWnd 父窗口
    @return 返回码
    */
    INT COMMON_INTERFACE ShowSaveFileDlg(OUT std::wstring& wstrFilePath, 
        IN LPCTSTR lpszDefExt = NULL,
        IN LPCTSTR lpszFileName = NULL,
        IN LPCTSTR lpszFilter = NULL,	// eg: "图象文件(*.bmp;*.jpg;*.gif)|*.bmp;*.jpg;*.gif|所有文件(*.*)|*.*||"
        IN DWORD dwFlags = OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
        IN HWND hParentWnd = NULL
        );

} } //namespace common::ui

namespace common { namespace ui
{
    /**
    @class CMemoryDC
    @brief 内存DC类
    */
    class CMemoryDC : public CDC
    {
    public:
        /**
        @fn CMemoryDC(HDC hDC, RECT& rcPaint)
        @brief 构造函数
        @param [in] hDC 原DC
        @param [in] rcPaint 绘制的区域
        */
        CMemoryDC(HDC hDC, const RECT& rcPaint) : m_hDCOriginal(hDC), m_pOldBmp(NULL)
        {
            m_rcPaint = rcPaint;
            CreateCompatibleDC(CDC::FromHandle(m_hDCOriginal));
            ATLASSERT(m_hDC != NULL);
            m_bmp.CreateCompatibleBitmap(CDC::FromHandle(m_hDCOriginal), m_rcPaint.right - m_rcPaint.left, m_rcPaint.bottom - m_rcPaint.top);
            ATLASSERT(m_bmp.m_hObject != NULL);
            m_pOldBmp = SelectObject(&m_bmp);
            SetViewportOrg(-m_rcPaint.left, -m_rcPaint.top);
        };

        /**
        @fn ~CMemoryDC()
        @brief 析构函数
        */
        ~CMemoryDC()
        {
            ::BitBlt(m_hDCOriginal, m_rcPaint.left, m_rcPaint.top, m_rcPaint.right - m_rcPaint.left, m_rcPaint.bottom - m_rcPaint.top, m_hDC, m_rcPaint.left, m_rcPaint.top, SRCCOPY);
            SelectObject(m_pOldBmp);
			m_bmp.DeleteObject();
        };

    public:
        // Data members
        HDC m_hDCOriginal;
        RECT m_rcPaint;
        CBitmap m_bmp;
        CBitmap* m_pOldBmp;
    };
} } //namespace common::ui
