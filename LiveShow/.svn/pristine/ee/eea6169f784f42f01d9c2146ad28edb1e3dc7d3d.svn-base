
#pragma once

#include <vector>
#include <utility>
#include "SkinTextStyle.h"

// CSkinLibretto 命令目标
class CImageBase;
class CSkinLibretto : public CSkinBase
{
	friend class CSkinEngine;
	DECLARE_DYNCREATE(CSkinLibretto)

protected:
	CSkinLibretto();
	virtual ~CSkinLibretto();

	virtual void OnFinalRelease();

protected:
	DECLARE_MESSAGE_MAP()
	DECLARE_OLECREATE(CSkinLibretto)
	DECLARE_SKIN_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()

protected:
	enum 
	{
		SKINBASE_DISPIDS(CSkinLibretto),

		SKIN_DISPID(CSkinLibretto, backgroundImg),
		SKIN_DISPID(CSkinLibretto, EndDisplay),
		SKIN_DISPID(CSkinLibretto, SetCurLibretto),

		SKIN_DISPID(CSkinLibretto, hlColor),

		SKIN_DISPID(CSkinLibretto, hlFontFamily),
		SKIN_DISPID(CSkinLibretto, hlStrikeOut),
		SKIN_DISPID(CSkinLibretto, hlUnderline),
		SKIN_DISPID(CSkinLibretto, hlItalic),
		SKIN_DISPID(CSkinLibretto, hlFontWeight),
		SKIN_DISPID(CSkinLibretto, hlFontSize),

		SKIN_DISPID(CSkinLibretto, AppendLibretto),
		SKIN_DISPID(CSkinLibretto, ClearLibretto),
		SKIN_DISPID(CSkinLibretto, vertiAlign),
		SKIN_DISPID(CSkinLibretto, horizAlign),
		SKIN_DISPID(CSkinLibretto, maxHeight),
		SKIN_DISPID(CSkinLibretto, maxWidth),	
		SKIN_DISPID(CSkinLibretto, singleLine),
		SKIN_DISPID(CSkinLibretto, wordBreak),
		SKIN_DISPID(CSkinLibretto, adjustEnable),

		SKIN_DISPID(CSkinLibretto, color),

		SKIN_DISPID(CSkinLibretto, fontFamily),
		SKIN_DISPID(CSkinLibretto, strikeOut),
		SKIN_DISPID(CSkinLibretto, underline),
		SKIN_DISPID(CSkinLibretto, italic),
		SKIN_DISPID(CSkinLibretto, fontWeight),
		SKIN_DISPID(CSkinLibretto, fontSize),

	};

public:

	BOOL UpdateFont(HDC hDC);
	BOOL UpdateHlFont(HDC hDC);
	void Draw(CDC* pDC, LPRECT lpInvalidRect = NULL, CRgn* pInvalidRgn = NULL);

protected:
	typedef enum enMouseState{
		MOUSE_NORMAL,
		MOUSE_HOVER,
		MOUSE_DOWN
	} MOUSE_STATE;

	std::vector<std::pair<int, CString> >
						m_vecLibretto;	//要显示的歌词，按时间顺序排列
	int					m_iCurLibre;	//当前显示歌词的位置
	bool				m_bDisplay;

	CSkinTextStyle		m_textStyleNormal;
	CSkinTextStyle		m_textStyleHighlight;

	CString			m_strBkImg;			//背景图片
	CImageBase*		m_pBkImg;			//背景图片指针

public:
	//一般歌词的字体和大小
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
	//一般歌词的颜色
	LONG Getcolor(void);
	void Setcolor(LONG newVal);

	//高亮歌词的字体和大小
	LONG GethlFontSize(void);
	void SethlFontSize(LONG newVal);
	LONG GethlFontWeight(void);
	void SethlFontWeight(LONG newVal);
	VARIANT_BOOL GethlItalic(void);
	void SethlItalic(VARIANT_BOOL newVal);
	VARIANT_BOOL GethlUnderline(void);
	void SethlUnderline(VARIANT_BOOL newVal);
	VARIANT_BOOL GethlStrikeOut(void);
	void SethlStrikeOut(VARIANT_BOOL newVal);
	BSTR GethlFontFamily(void);
	void SethlFontFamily(LPCTSTR newVal);
	//高亮歌词的颜色
	LONG GethlColor(void);
	void SethlColor(LONG newVal);

	//文字排布
	VARIANT_BOOL GetadjustEnable(void);
	void SetadjustEnable(VARIANT_BOOL newVal);
	VARIANT_BOOL GetwordBreak(void);
	void SetwordBreak(VARIANT_BOOL newVal);
	VARIANT_BOOL GetsingleLine(void);
	void SetsingleLine(VARIANT_BOOL newVal);
	BSTR GethorizAlign(void);
	void SethorizAlign(LPCTSTR newVal);
	BSTR GetvertiAlign(void);
	void SetvertiAlign(LPCTSTR newVal);protected:

	//背景图片
	BSTR GetbackgroundImg(void);
	void SetbackgroundImg(LPCTSTR newVal);

protected:
	//Dispatch方法
	void ClearLibretto(void);
	void AppendLibretto(LPCTSTR libre);
	void SetCurLibretto(LONG index);
	void EndDisplay(void);

#ifdef V8_JS_ENGINE_SUPPORT
    //v8实现
    v8::Handle<v8::Value> V8ClearLibretto(const v8::Arguments& args);
    v8::Handle<v8::Value> V8AppendLibretto(const v8::Arguments& args);
    v8::Handle<v8::Value> V8SetCurLibretto(const v8::Arguments& args);
    v8::Handle<v8::Value> V8EndDisplay(const v8::Arguments& args);
    virtual v8::Handle<v8::Value> V8InternalCallMethod(const CString& funcName, const v8::Arguments& args);
#endif //V8_JS_ENGINE_SUPPORT

};
