#pragma once

#include <list>
#include <map>
#include <set>

#include "SkinItem.h"

// CSkinItemDialog command target
class CImageBase;
class CSkinItemDialog : public CSkinItem
{
	friend class CSkinEngine;
	DECLARE_DYNCREATE(CSkinItemDialog)

protected:
	CSkinItemDialog();
	virtual ~CSkinItemDialog();

	virtual void OnFinalRelease();

protected:
	DECLARE_MESSAGE_MAP()
	DECLARE_SKIN_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()

protected:
	enum 
	{
		SKINBASE_DISPIDS(CSkinItemDialog),
		SKINITEM_DISPIDS(CSkinItemDialog),

		SKIN_DISPID(CSkinItemDialog, titleHeight),
		SKIN_DISPID(CSkinItemDialog, titleInactive),
		SKIN_DISPID(CSkinItemDialog, titleMaskImage),
		SKIN_DISPID(CSkinItemDialog, borderHorizImage),
		SKIN_DISPID(CSkinItemDialog, borderVertiImage),
		SKIN_DISPID(CSkinItemDialog, borderColor),
		SKIN_DISPID(CSkinItemDialog, border),
		SKIN_DISPID(CSkinItemDialog, sizeBorder),
		SKIN_DISPID(CSkinItemDialog, sizeCorner),
		SKIN_DISPID(CSkinItemDialog, max),
		SKIN_DISPID(CSkinItemDialog, maxHeight),
		SKIN_DISPID(CSkinItemDialog, maxWidth),
		SKIN_DISPID(CSkinItemDialog, minWidth),
		SKIN_DISPID(CSkinItemDialog, minHeight),
		SKIN_DISPID(CSkinItemDialog, initCenterWindow),
		SKIN_DISPID(CSkinItemDialog, maximizable),
		SKIN_DISPID(CSkinItemDialog, minimizable),
		SKIN_DISPID(CSkinItemDialog, resizable),
        SKIN_DISPID(CSkinItemDialog, movable),
        SKIN_DISPID(CSkinItemDialog, removeNC),
        SKIN_DISPID(CSkinItemDialog, bkHitTest),
        SKIN_DISPID(CSkinItemDialog, sysTickCount),
        SKIN_DISPID(CSkinItemDialog, eraseBk),
        SKIN_DISPID(CSkinItemDialog, eraseBkOnce),
		SKIN_DISPID(CSkinItemDialog, EndPaint),
		SKIN_DISPID(CSkinItemDialog, BeginPaint),
	};

public:
	BOOL AttachHwnd(HWND hWnd);

	BOOL OnDupSkinItem(HWND hWnd, CSkinItem* pBase);

public:
	BOOL GetTitleRect(RECT& rect);
	//更新皮肤属性，在皮肤新附着窗口时调用
	virtual BOOL UpdateSkinItemProperty();
public:
	virtual void DrawBackground(CDC* pDC, LPRECT lpInvalidRect = NULL, CRgn* pInvalidRgn = NULL, BOOL bChildSkin = FALSE);
	virtual void DrawForeground(CDC* pDC, LPRECT lpInvalidRect = NULL, CRgn* pInvalidRgn = NULL);
	virtual void DrawFrame(CDC* pDC, LPRECT lpInvalidRect = NULL, CRgn* pInvalidRgn = NULL);
	virtual void Draw(CDC* pDC, LPRECT lpInvalidRect = NULL, CRgn* pInvalidRgn = NULL);
	virtual BOOL InvalidateRect(LPCRECT lpRects = NULL, BOOL bErase = TRUE);

public:
	//Dispatch属性虚函数
    virtual void SetName(LPCTSTR newVal);

	//Dispatch属性
	VARIANT_BOOL Getmax(void);
	void Setmax(VARIANT_BOOL newVal);
	LONG GettitleHeight(void);
	void SettitleHeight(LONG lVal);
	VARIANT_BOOL GettitleInactive(void);
	void SettitleInactive(VARIANT_BOOL newVal);
	BSTR GettitleMaskImage(void);
	void SettitleMaskImage(LPCTSTR newVal);
	LONG GetmaxHeight(void);
	void SetmaxHeight(LONG newVal);
	LONG GetmaxWidth(void);
	void SetmaxWidth(LONG newVal);
	LONG GetminHeight(void);
	void SetminHeight(LONG newVal);
	LONG GetminWidth(void);
	void SetminWidth(LONG newVal);
	LONG GetsizeBorder(void);
	void SetsizeBorder(LONG newVal);
	LONG GetsizeCorner(void);
	void SetsizeCorner(LONG newVal);
	LONG Getborder(void);
	void Setborder(LONG newVal);
	LONG GetborderColor(void);
	void SetborderColor(LONG newVal);
	BSTR GetborderHorizImage(void);
	void SetborderHorizImage(LPCTSTR newVal);
	BSTR GetborderVertiImage(void);
	void SetborderVertiImage(LPCTSTR newVal);
	VARIANT_BOOL Getresizable(void);
	void Setresizable(VARIANT_BOOL newVal);
    VARIANT_BOOL Getmovable(void);
    void Setmovable(VARIANT_BOOL newVal);
	VARIANT_BOOL Getminimizable(void);
	void Setminimizable(VARIANT_BOOL newVal);
	VARIANT_BOOL Getmaximizable(void);
	void Setmaximizable(VARIANT_BOOL newVal);
	VARIANT_BOOL GetinitCenterWindow(void);
	void SetinitCenterWindow(VARIANT_BOOL newVal);
    VARIANT_BOOL GetremoveNC(void);
    void SetremoveNC(VARIANT_BOOL newVal);
    BSTR GetbkHitTest(void);
    void SetbkHitTest(LPCTSTR newVal);
    ULONG GetsysTickCount();
    void SetsysTickCount(ULONG time);

    VARIANT_BOOL GeteraseBk(void);
    void SeteraseBk(VARIANT_BOOL newVal);
    VARIANT_BOOL GeteraseBkOnce(void);
    void SeteraseBkOnce(VARIANT_BOOL newVal);

	//Dispatch方法
	void BeginPaint();
	void EndPaint();

#ifdef V8_JS_ENGINE_SUPPORT
    //v8实现
    v8::Handle<v8::Value> V8BeginPaint(const v8::Arguments& args);
    v8::Handle<v8::Value> V8EndPaint(const v8::Arguments& args);
    virtual v8::Handle<v8::Value> V8InternalCallMethod(const CString& funcName, const v8::Arguments& args);
#endif //V8_JS_ENGINE_SUPPORT

protected:
	//消息处理函数
	virtual BOOL OnSkinNotify(UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* lpResult);
	virtual BOOL OnPaint(UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* lpResult);
	virtual BOOL OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* lpResult);
	virtual BOOL OnActivate(UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* lpResult);
	virtual BOOL OnNcHitTest(UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* lpResult);
	virtual BOOL OnNcLButtonDown(UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* lpResult);
	virtual BOOL OnNcLButtonDblClk(UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* lpResult);
    virtual BOOL OnNcLButtonUp(UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* lpResult);
    virtual BOOL OnNcRButtonDown(UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* lpResult);
    virtual BOOL OnNcRButtonDblClk(UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* lpResult);
    virtual BOOL OnNcRButtonUp(UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* lpResult);
	virtual BOOL OnNcMouseMove(UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* lpResult);
	virtual BOOL OnNcMouseLeave(UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* lpResult);
	virtual BOOL OnNcPaint(UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* lpResult);
	virtual BOOL OnNcActivate(UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* lpResult);
	virtual BOOL OnNcCalcSize(UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* lpResult);
    virtual BOOL OnLButtonDown(UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* lpResult);
    virtual BOOL OnLButtonUp(UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* lpResult);
    virtual BOOL OnLButtonDblClk(UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* lpResult);
    virtual BOOL OnRButtonDown(UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* lpResult);
    virtual BOOL OnRButtonUp(UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* lpResult);
    virtual BOOL OnRButtonDblClk(UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* lpResult);
	virtual BOOL OnMouseMove(UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* lpResult);
    virtual BOOL OnMouseHover(UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* lpResult);
    virtual BOOL OnMouseEnter(UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* lpResult);
	virtual BOOL OnMouseLeave(UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* lpResult);
	virtual BOOL OnCaptureChanged(UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* lpResult);
	virtual BOOL OnGetMinMaxInfo(UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* lpResult);
	virtual BOOL OnSetFocus(UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* lpResult);
	virtual BOOL OnSetCursor(UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* lpResult);
	virtual BOOL OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* lpResult);
	virtual BOOL OnCtlColor(UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* lpResult);
    virtual BOOL OnEraseBkgnd(UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* lpResult);
    virtual BOOL OnSysCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* lpResult);

    virtual BOOL OnNcMouseMove(LONG x, LONG y, UINT flag);
    virtual BOOL OnNcLButtonDown(LONG x, LONG y, UINT flag);
    virtual BOOL OnNcLButtonDblClk(LONG x, LONG y, UINT flag);
    virtual BOOL OnNcMouseLeave();
	virtual BOOL OnMouseMove(LONG x, LONG y, UINT flag);
	virtual BOOL OnLButtonDown(LONG x, LONG y, UINT flag);
	virtual BOOL OnLButtonUp(LONG x, LONG y, UINT flag);
	virtual BOOL OnLButtonDblClk(LONG x, LONG y, UINT flag);
	virtual BOOL OnRButtonDown(LONG x, LONG y, UINT flag);
	virtual BOOL OnRButtonUp(LONG x, LONG y, UINT flag);
	virtual BOOL OnRButtonDblClk(LONG x, LONG y, UINT flag);
	virtual BOOL OnMButtonDown(LONG x, LONG y, UINT flag);
	virtual BOOL OnMButtonUp(LONG x, LONG y, UINT flag);
	virtual BOOL OnMButtonDblClk(LONG x, LONG y, UINT flag);
	virtual BOOL OnMouseWheel(LONG x, LONG y, UINT flag);
	virtual BOOL OnMouseHover(LONG x, LONG y, UINT flag);
	virtual BOOL OnMouseEnter();
	virtual BOOL OnMouseLeave();

protected:
    BOOL							m_bSkin;
	LONG							m_nMinWidth;
	LONG							m_nMaxWidth;
	LONG							m_nMinHeight;
	LONG							m_nMaxHeight;

	//边缘
	LONG							m_nSizeBorder;
	LONG							m_nSizeCorner;

	//标题栏高度
	LONG							m_nTitleHeight;

	//边框
	LONG							m_nBorderWidth;
	LONG							m_clrBorderColor;
	CString							m_strBorderHorizImage;
	CString							m_strBorderVertiImage;
	CImageBase*						m_pBorderHorizImage;
	CImageBase*						m_pBorderVertiImage;
	CString							m_strTitleMaskImage;
	CImageBase*						m_pTitleMaskImage;

    BOOL                            m_bRemoveNC;       //去除WS_CAPTION属性
	BOOL							m_bResizable;			//可放缩
    BOOL                            m_bMovable;
	BOOL							m_bMinimizable;
	BOOL							m_bMaximizable;
    UINT                            m_uBkHitTest;         //背景的HitTest

	BOOL							m_bTitleInactive;

	BOOL							m_bInitCenterWindow;	//初始时

	BOOL							m_bUseDrawBuffer;		//使用绘制缓存
															//在OnSize中置为true，在OnPaint后被置为false
															//true时用双缓存绘制，false不用双缓存

	BOOL							m_bNcActivate;
    BOOL                            m_bEraseBk;             //
    BOOL                            m_bEraseBkOnce;           //
    LONG                            m_lEraseBkCount;          //
};
