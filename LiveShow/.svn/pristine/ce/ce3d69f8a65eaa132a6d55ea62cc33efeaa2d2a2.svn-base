
#pragma once

#include "SkinTextStyle.h"
#include "SkinBase.h"

#define SKIN_SCROLL_TIMER 1234
#define SKIN_SCROLL_ECLAPSE 50

// CSkinText command target
class CSkinItemDialog;

class CSkinText : public CSkinBase
{
	friend class CSkinEngine;
	DECLARE_DYNCREATE(CSkinText)

protected:
	CSkinText();
	virtual ~CSkinText();

	virtual void OnFinalRelease();

protected:
	DECLARE_MESSAGE_MAP()
	DECLARE_SKIN_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()

protected:
	enum 
	{
		SKINBASE_DISPIDS(CSkinText),

		SKIN_DISPID(CSkinText, lineMargin),
		SKIN_DISPID(CSkinText, textHeight),
		SKIN_DISPID(CSkinText, textWidth),
		SKIN_DISPID(CSkinText, scrollSpeed),
		SKIN_DISPID(CSkinText, scrollDirection),
		SKIN_DISPID(CSkinText, scrollable),
		SKIN_DISPID(CSkinText, vertiAlign),
		SKIN_DISPID(CSkinText, horizAlign),
		SKIN_DISPID(CSkinText, maxHeight),
		SKIN_DISPID(CSkinText, maxWidth),
		SKIN_DISPID(CSkinText, noPrefix),
		SKIN_DISPID(CSkinText, endEllipsis),
		SKIN_DISPID(CSkinText, singleLine),
		SKIN_DISPID(CSkinText, wordBreak),
		SKIN_DISPID(CSkinText, adjustEnable),
        SKIN_DISPID(CSkinText, ignoreCRLF),

        SKIN_DISPID(CSkinText, bkColorNormal),

		SKIN_DISPID(CSkinText, downColor),
		SKIN_DISPID(CSkinText, hoverColor),
		SKIN_DISPID(CSkinText, normalColor),
		SKIN_DISPID(CSkinText, bkColor),
		SKIN_DISPID(CSkinText, bkMode),

		SKIN_DISPID(CSkinText, fontFamily),
		SKIN_DISPID(CSkinText, strikeOut),
		SKIN_DISPID(CSkinText, underlineMargin),
		SKIN_DISPID(CSkinText, underline),
        SKIN_DISPID(CSkinText, normalUnderline),
		SKIN_DISPID(CSkinText, italic),
		SKIN_DISPID(CSkinText, fontWeight),
		SKIN_DISPID(CSkinText, fontSize),

        SKIN_DISPID(CSkinText, selStart),
        SKIN_DISPID(CSkinText, selEnd),

        SKIN_DISPID(CSkinText, GetSelText),
        SKIN_DISPID(CSkinText, Hittest),
	};

public:
	virtual BOOL OnMouseEnter();
	virtual BOOL OnMouseLeave();
	virtual BOOL OnLButtonDown(LONG x, LONG y, UINT flag);
	virtual BOOL OnLButtonUp(LONG x, LONG y, UINT flag);
	virtual BOOL OnClick(LONG iTab = 0);
	virtual BOOL OnTimer(UINT nIDEvent);

	virtual HCURSOR GetSkinCursor(LPPOINT lpPoint);

	BOOL UpdateFont(HDC hDC = NULL);
	BOOL GetTextSize(CSize& size, HDC hDC = NULL);
	BOOL AdjustRect(HDC hDC = NULL);
	void Draw(CDC* pDC, LPRECT lpInvalidRect = NULL, CRgn* pInvalidRgn = NULL);

protected:
	typedef enum enMouseState{
		MOUSE_NORMAL,
		MOUSE_HOVER,
		MOUSE_DOWN
	} MOUSE_STATE;

    LONG m_clrBkColorNormal;

	//各种状态下的文本风格
	CSkinTextStyle m_textStyleNormal;
	CSkinTextStyle m_textStyleHover;
	CSkinTextStyle m_textStyleDown;
	CSkinTextStyle m_textStyleDisabled;
    BOOL           m_bNormalUnderline;

	MOUSE_STATE		m_enMouseState;		//鼠标状态

	int				m_iMaxWidth;		//最大可调节宽度
	int				m_iMaxHeight;		//最大可调节高度
	bool			m_bAdjustEnable;	//是否可调节大小
	LONG			m_nLineMargin;		//行间距

	CRect			m_rcTextArea;

	UINT			m_uScrollFlag;		//滚动标识，包含是否滚动和滚动方向的信息
	LONG			m_nScrollOffset;	//滚动偏移
	LONG			m_nScrollSpeed;		//滚动速度，可正反两个方向，单位像素/SKIN_SCROLL_ECLAPSE(ms)
	UINT_PTR		m_tScrollTimer;		//滚动计时器

    LONG            m_nSelStart;        //选择开始
    LONG            m_nSelEnd;          //选择结束

public:
	//皮肤文字虚函数
	BSTR GetText(void);
	void SetText(LPCTSTR newVal);

    //背景颜色
    LONG GetbkColorNormal();
    void SetbkColorNormal(LONG newVal);

	//文字背景颜色
	LONG GetbkColor();
	void SetbkColor(LONG newVal);
	//文字背景模式
	BSTR GetbkMode(void);
	void SetbkMode(LPCTSTR newVal);

	//字体和大小
	LONG GetfontSize(void);
	void SetfontSize(LONG newVal);
	LONG GetfontWeight(void);
	void SetfontWeight(LONG newVal);
	VARIANT_BOOL Getitalic(void);
	void Setitalic(VARIANT_BOOL newVal);
	VARIANT_BOOL Getunderline(void);
	void Setunderline(VARIANT_BOOL newVal);
	VARIANT_BOOL GetstrikeOut(void);
	void SetstrikeOut(VARIANT_BOOL newVal);
	BSTR GetfontFamily(void);
	void SetfontFamily(LPCTSTR newVal);
	//颜色
	LONG GetnormalColor(void);
	void SetnormalColor(LONG newVal);
	LONG GethoverColor(void);
	void SethoverColor(LONG newVal);
	LONG GetdownColor(void);
	void SetdownColor(LONG newVal);
	//文字排布
	VARIANT_BOOL GetadjustEnable(void);
	void SetadjustEnable(VARIANT_BOOL newVal);
	VARIANT_BOOL GetwordBreak(void);
	void SetwordBreak(VARIANT_BOOL newVal);
	VARIANT_BOOL GetsingleLine(void);
	void SetsingleLine(VARIANT_BOOL newVal);
    VARIANT_BOOL GetnormalUnderline();
    void SetnormalUnderline(VARIANT_BOOL newVal);
	VARIANT_BOOL GetendEllipsis(void);
	void SetendEllipsis(VARIANT_BOOL newVal);
	VARIANT_BOOL GetnoPrefix(void);
	void SetnoPrefix(VARIANT_BOOL newVal);
	LONG GetmaxWidth(void);
	void SetmaxWidth(LONG newVal);
	LONG GetmaxHeight(void);
	void SetmaxHeight(LONG newVal);
	BSTR GethorizAlign(void);
	void SethorizAlign(LPCTSTR newVal);
	BSTR GetvertiAlign(void);
	void SetvertiAlign(LPCTSTR newVal);
	//滚动
	enum
	{
		SKIN_SCROLL_ENABLE = 0x00000001,
		SKIN_SCROLL_DIRECTION = 0x00000002
	};

	VARIANT_BOOL Getscrollable(void);
	void Setscrollable(VARIANT_BOOL newVal);
	BSTR GetscrollDirection(void);
	void SetscrollDirection(LPCTSTR newVal);
	LONG GetscrollSpeed(void);
	void SetscrollSpeed(LONG newVal);

	//字宽
	LONG GettextWidth(void);
	void SettextWidth(LONG newVal);	//不可用
	//字高
	LONG GettextHeight(void);
	void SettextHeight(LONG newVal);	//不可用

	//行间距
	LONG GetlineMargin(void);
	void SetlineMargin(LONG newVal);

	//下划线间距
	LONG GetunderlineMargin(void);
	void SetunderlineMargin(LONG newVal);
    //
    VARIANT_BOOL GetignoreCRLF(void);
    void SetignoreCRLF(VARIANT_BOOL newVal);

    LONG GetselStart(void);
    void SetselStart(LONG newVal);
    LONG GetselEnd(void);
    void SetselEnd(LONG newVal);

public:
    CString InternalGetSelText();
    LONG InternalHittest(LONG x, LONG y);

public:
    BSTR GetSelText();
    LONG Hittest(LONG x, LONG y);

#ifdef V8_JS_ENGINE_SUPPORT
    //v8实现
protected:
    v8::Handle<v8::Value> V8GetSelText(const v8::Arguments& args);
    v8::Handle<v8::Value> V8Hittest(const v8::Arguments& args);
protected:
    virtual v8::Handle<v8::Value> V8InternalCallMethod(const CString& funcName, const v8::Arguments& args);

#endif //V8_JS_ENGINE_SUPPORT
};
