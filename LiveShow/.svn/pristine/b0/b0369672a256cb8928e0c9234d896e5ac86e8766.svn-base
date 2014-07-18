
#pragma once

#include "SkinBaseDefine.h"
#include "XmlCmdTarget.h"

// CSkinTextStyle 命令目标

class CSkinTextStyle : public CXmlCmdTarget
{
	DECLARE_DYNCREATE(CSkinTextStyle)

public:
	CSkinTextStyle();
	virtual ~CSkinTextStyle();
	CSkinTextStyle& operator = (const CSkinTextStyle& style);

	virtual void OnFinalRelease();

protected:
	DECLARE_MESSAGE_MAP()
	DECLARE_OLECREATE(CSkinTextStyle)
	DECLARE_SKIN_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()

protected:
	enum
	{
		SKIN_DISPID(CSkinTextStyle, color),
		SKIN_DISPID(CSkinTextStyle, bkColor),
		SKIN_DISPID(CSkinTextStyle, bkMode),
		SKIN_DISPID(CSkinTextStyle, fontSize),
		SKIN_DISPID(CSkinTextStyle, fontWeight),
		SKIN_DISPID(CSkinTextStyle, italic),
		SKIN_DISPID(CSkinTextStyle, underline),
		SKIN_DISPID(CSkinTextStyle, strikeOut),
		SKIN_DISPID(CSkinTextStyle, fontFamily),
		SKIN_DISPID(CSkinTextStyle, shadow),
		SKIN_DISPID(CSkinTextStyle, wordBreak),
		SKIN_DISPID(CSkinTextStyle, endEllipsis),
		SKIN_DISPID(CSkinTextStyle, noPrefix),
		SKIN_DISPID(CSkinTextStyle, singleLine),
		SKIN_DISPID(CSkinTextStyle, horizAlign),
		SKIN_DISPID(CSkinTextStyle, vertiAlign),
		SKIN_DISPID(CSkinTextStyle, beginLineMargin),
		SKIN_DISPID(CSkinTextStyle, lineMargin),
		SKIN_DISPID(CSkinTextStyle, endLineMargin),
		SKIN_DISPID(CSkinTextStyle, underlineMargin),
        SKIN_DISPID(CSkinTextStyle, ignoreCRLF),
	};

public:
	BOOL IsChanged();
	BOOL IsFontChanged();
	BOOL IsFormatChanged();
	UINT GetFormat();
	CFont* GetFont();
	UINT GetBkMode();
	BOOL UpdateFormat(HDC hDC);
	BOOL UpdateFont(HDC hDC);
	BOOL GetTextSize(HDC hDC, LPCTSTR strText, LONG& width, LONG& height);
	BOOL GetTextSize(HDC hDC, LPCTSTR strText, CSize& size);
	BOOL DrawText(HDC hDC, LPCTSTR lpzText, LPRECT lpDrawRect, LPRECT lpRealDrawRect = NULL);
    BOOL DrawTextEx(HDC hDC, LPCTSTR lpzText, LPRECT lpDrawRect, LONG clrText, LPRECT lpRealDrawRect = NULL);
    LONG HitTest(const CString& strText, LONG x, LONG y);

	static int GlideByReturn(const CString& str);

protected:
	BOOL InternalUpdateFont(HDC hDC);
	BOOL InternalUpdateFormat(HDC hDC);
	BOOL InternalGetTextSize(HDC hDC, LPCTSTR strText, LONG& width, LONG& height);

public:
	//////////////////////////////////////////////////////////////////////////
	//属性
	//////////////////////////////////////////////////////////////////////////
	//颜色
	LONG Getcolor(void);
	void Setcolor(LONG newVal);
	//背景颜色
	LONG GetbkColor();
	void SetbkColor(LONG newVal);
	//背景模式
	BSTR GetbkMode(void);
	void SetbkMode(LPCTSTR newVal);
	//大小
	LONG GetfontSize(void);
	void SetfontSize(LONG newVal);
	//weight
	LONG GetfontWeight(void);
	void SetfontWeight(LONG newVal);
	//斜体
	VARIANT_BOOL Getitalic(void);
	void Setitalic(VARIANT_BOOL newVal);
	//下划线
	VARIANT_BOOL Getunderline(void);
	void Setunderline(VARIANT_BOOL newVal);
	//删除线
	VARIANT_BOOL GetstrikeOut(void);
	void SetstrikeOut(VARIANT_BOOL newVal);
	//字体，如_T("宋体")
	BSTR GetfontFamily(void);
	void SetfontFamily(LPCTSTR newVal);
	//阴影
	VARIANT_BOOL Getshadow(void);
	void Setshadow(VARIANT_BOOL newVal);
	//word break
	VARIANT_BOOL GetwordBreak(void);
	void SetwordBreak(VARIANT_BOOL newVal);
	//end ellipsis
	VARIANT_BOOL GetendEllipsis(void);
	void SetendEllipsis(VARIANT_BOOL newVal);
	//no prefix
	VARIANT_BOOL GetnoPrefix(void);
	void SetnoPrefix(VARIANT_BOOL newVal);
	//单行
	VARIANT_BOOL GetsingleLine(void);
	void SetsingleLine(VARIANT_BOOL newVal);
	//水平对齐
	BSTR GethorizAlign(void);
	void SethorizAlign(LPCTSTR newVal);
	//垂直对齐
	BSTR GetvertiAlign(void);
	void SetvertiAlign(LPCTSTR newVal);
	//第一行与上边间距
	LONG GetbeginLineMargin(void);
	void SetbeginLineMargin(LONG newVal);
	//多行的行间距
	LONG GetlineMargin(void);
	void SetlineMargin(LONG newVal);
	//最后一行与下边间距
	LONG GetendLineMargin(void);
	void SetendLineMargin(LONG newVal);
	//下划线与文字的间距
	LONG GetunderlineMargin(void);
	void SetunderlineMargin(LONG newVal);
    //忽略\r\n
    VARIANT_BOOL GetignoreCRLF(void);
    void SetignoreCRLF(VARIANT_BOOL newVal);

protected:
	LOGFONT	m_lfFont;			//使用的字体结构
	BOOL	m_bUnderline;		//自绘的下划线
	HFONT	m_fFont;			//使用的字体
	LONG	m_clrColor;			//字的颜色
	LONG	m_clrBkColor;		//背景的颜色
	LONG	m_iFontSize;		//字体的大小
	UINT	m_uBkMode;			//背景模式，TRANSPARENT和OPAQUE
	UINT	m_uFormat;			//字显示的格式
	LONG	m_nBeginLineMargin;	//第一行与上边间距
	LONG	m_nLineMargin;		//多行的行间距
	LONG	m_nEndLineMargin;	//最后一行与下边间距
	LONG	m_nUnderlineMargin;	//下划线与文字的间距
	BOOL	m_bShadow;			//显示字的阴影
    BOOL    m_bIgnoreCRLF;      //忽略\r\n
	BOOL	m_bFontChanged;		//字体被改变，用来标识是否重建字体
	BOOL	m_bFormatChanged;	//字体排布格式改变
};

