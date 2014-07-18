
#pragma once

#include "SkinTextStyle.h"
#include "SkinBase.h"

typedef enum enSkinButtonStyle
{
	BUTTONSTYLE_NON = 0,
	BUTTONSTYLE_NORMAL,
	BUTTONSTYLE_CHECK,
} SKINBUTTON_STYLE;

typedef enum enSkinButtonTextPlacement
{
    TOP,
    RIGHT,
    LEFT,
    BOTTOM,
} TEXT_PLACEMENT;

// CSkinButton command target
class CImageBase;
class CSkinItemDialog;
class CSkinButton : public CSkinBase
{
	friend class CSkinEngine;
	DECLARE_DYNCREATE(CSkinButton)

protected:
	CSkinButton();
	virtual ~CSkinButton();

	virtual void OnFinalRelease();

protected:
	DECLARE_MESSAGE_MAP()
	DECLARE_SKIN_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()

protected:
	enum 
	{
		SKINBASE_DISPIDS(CSkinButton),

		SKIN_DISPID(CSkinButton, hyperLinkAddress),
		SKIN_DISPID(CSkinButton, hyperLink),
		SKIN_DISPID(CSkinButton, checked),
        SKIN_DISPID(CSkinButton, textColorTransform),
        SKIN_DISPID(CSkinButton, textHotColorTransform),
		SKIN_DISPID(CSkinButton, useDefaultSize),
		SKIN_DISPID(CSkinButton, style),
		SKIN_DISPID(CSkinButton, cmdID),
		SKIN_DISPID(CSkinButton, tip),

        SKIN_DISPID(CSkinButton, textPlacement),
		SKIN_DISPID(CSkinButton, textMargin),
		SKIN_DISPID(CSkinButton, labelUnderlineMargin),	
		SKIN_DISPID(CSkinButton, labelVertiAlign),	
		SKIN_DISPID(CSkinButton, labelHorizAlign),		
		SKIN_DISPID(CSkinButton, labelSingleLine),	
		SKIN_DISPID(CSkinButton, labelWordBreak),	
        SKIN_DISPID(CSkinButton, labelEndEllipsis),	
		SKIN_DISPID(CSkinButton, labelDisabledColor),
		SKIN_DISPID(CSkinButton, labelDownColor),
		SKIN_DISPID(CSkinButton, labelHoverColor),
		SKIN_DISPID(CSkinButton, labelNormalColor),
		SKIN_DISPID(CSkinButton, labelFontFamily),
		SKIN_DISPID(CSkinButton, labelStrikeOut),	
		SKIN_DISPID(CSkinButton, labelUnderline),	
        SKIN_DISPID(CSkinButton, labelNormalUnderline),	
		SKIN_DISPID(CSkinButton, labelItalic),	
		SKIN_DISPID(CSkinButton, labelFontWeight),
		SKIN_DISPID(CSkinButton, labelFontSize),

		SKIN_DISPID(CSkinButton, iconMargin),
        SKIN_DISPID(CSkinButton, iconTopMargin),
        SKIN_DISPID(CSkinButton, iconColorTransform),
        SKIN_DISPID(CSkinButton, iconAlpha),
        SKIN_DISPID(CSkinButton, iconImageCheckedNormal),
        SKIN_DISPID(CSkinButton, iconImageCheckedHover),
        SKIN_DISPID(CSkinButton, iconImageCheckedDown),
        SKIN_DISPID(CSkinButton, iconImageCheckedDisabled),
        SKIN_DISPID(CSkinButton, iconImageUncheckedNormal),
        SKIN_DISPID(CSkinButton, iconImageUncheckedHover),
        SKIN_DISPID(CSkinButton, iconImageUncheckedDown),
        SKIN_DISPID(CSkinButton, iconImageUncheckedDisabled),

        SKIN_DISPID(CSkinButton, absIconImageUncheckedNormal),
        SKIN_DISPID(CSkinButton, iconHeight),
        SKIN_DISPID(CSkinButton, iconWidth),
        SKIN_DISPID(CSkinButton, backIconMargin),
        SKIN_DISPID(CSkinButton, backIconTopMargin),
        SKIN_DISPID(CSkinButton, backIconImageCheckedNormal),
        SKIN_DISPID(CSkinButton, backIconImageCheckedHover),
        SKIN_DISPID(CSkinButton, backIconImageCheckedDown),
        SKIN_DISPID(CSkinButton, backIconImageCheckedDisabled),
        SKIN_DISPID(CSkinButton, backIconImageUncheckedNormal),
        SKIN_DISPID(CSkinButton, backIconImageUncheckedHover),
        SKIN_DISPID(CSkinButton, backIconImageUncheckedDown),
        SKIN_DISPID(CSkinButton, backIconImageUncheckedDisabled),
        SKIN_DISPID(CSkinButton, backIconHeight),
        SKIN_DISPID(CSkinButton, backIconWidth),
		SKIN_DISPID(CSkinButton, disabledImage),
		SKIN_DISPID(CSkinButton, downImage),
		SKIN_DISPID(CSkinButton, hoverImage),
		SKIN_DISPID(CSkinButton, normalImage),
        SKIN_DISPID(CSkinButton, sideStretch),
        SKIN_DISPID(CSkinButton, sideStretchWidth),
        SKIN_DISPID(CSkinButton, sideStretchHeight),
        SKIN_DISPID(CSkinButton, hotColorTransform),

        SKIN_DISPID(CSkinButton, downOffset),
        SKIN_DISPID(CSkinButton, showBkAlways),

		SKIN_DISPID(CSkinButton, clickable),
		SKIN_DISPID(CSkinButton, handleTip),

		SKIN_DISPID(CSkinButton, maxTipWidth)	
	};

public:
	virtual BOOL OnMouseEnter();
	virtual BOOL OnMouseLeave();
	virtual BOOL OnLButtonDown(LONG x, LONG y, UINT flag);
	virtual BOOL OnLButtonUp(LONG x, LONG y, UINT flag);
	virtual BOOL OnClick(LONG iTab = 0);

	virtual HCURSOR GetSkinCursor(LPPOINT lpPoint);

	virtual BOOL OnSetFocus(CSkinBase* pOldFocus);
	virtual BOOL OnKillFocus(CSkinBase* pNewFocus);

	void Draw(CDC* pDC, LPRECT lpInvalidRect = NULL, CRgn* pInvalidRgn = NULL);

protected:
	static BOOL GetOpenCommand(CString& command);
	BOOL UpdateLabelFont(HDC hDC);
	void ShowToolTip(CString pszTooltip, LONG maxTipWidth = 0);
	void PopToolTip();

public:
	VARIANT_BOOL Getchecked();
	void Setchecked(VARIANT_BOOL newVal);
    VARIANT_BOOL GettextColorTransform();
    void SettextColorTransform(VARIANT_BOOL newVal);
	BSTR GetnormalImage();
	void SetnormalImage(LPCTSTR newVal);
	BSTR GethoverImage();
	void SethoverImage(LPCTSTR newVal);
	BSTR GetdownImage();
	void SetdownImage(LPCTSTR newVal);
	BSTR GetdisabledImage();
	void SetdisabledImage(LPCTSTR newVal);
    VARIANT_BOOL GetsideStretch();
    void SetsideStretch(VARIANT_BOOL newVal);
    LONG GetsideStretchWidth();
    void SetsideStretchWidth(LONG newVal);
    LONG GetsideStretchHeight();
    void SetsideStretchHeight(LONG newVal);
    VARIANT_BOOL GethotColorTransform();
    void SethotColorTransform(VARIANT_BOOL newVal);

    VARIANT_BOOL GeticonColorTransform();
    void SeticonColorTransform(VARIANT_BOOL newVal);
    VARIANT_BOOL GeticonAlpha();
    void SeticonAlpha(VARIANT_BOOL newVal);
	BSTR GeticonImageUncheckedNormal();
	void SeticonImageUncheckedNormal(LPCTSTR newVal);
    BSTR GeticonImageUncheckedHover();
    void SeticonImageUncheckedHover(LPCTSTR newVal);
    BSTR GeticonImageUncheckedDown();
    void SeticonImageUncheckedDown(LPCTSTR newVal);
    BSTR GeticonImageUncheckedDisabled();
    void SeticonImageUncheckedDisabled(LPCTSTR newVal);
    BSTR GeticonImageCheckedNormal();
    void SeticonImageCheckedNormal(LPCTSTR newVal);
    BSTR GeticonImageCheckedHover();
    void SeticonImageCheckedHover(LPCTSTR newVal);
    BSTR GeticonImageCheckedDown();
    void SeticonImageCheckedDown(LPCTSTR newVal);
    BSTR GeticonImageCheckedDisabled();
    void SeticonImageCheckedDisabled(LPCTSTR newVal);

    BSTR GetabsIconImageUncheckedNormal();
    void SetabsIconImageUncheckedNormal(LPCTSTR newVal);

    BSTR GetbackIconImageUncheckedNormal();
    void SetbackIconImageUncheckedNormal(LPCTSTR newVal);
    BSTR GetbackIconImageUncheckedHover();
    void SetbackIconImageUncheckedHover(LPCTSTR newVal);
    BSTR GetbackIconImageUncheckedDown();
    void SetbackIconImageUncheckedDown(LPCTSTR newVal);
    BSTR GetbackIconImageUncheckedDisabled();
    void SetbackIconImageUncheckedDisabled(LPCTSTR newVal);
    BSTR GetbackIconImageCheckedNormal();
    void SetbackIconImageCheckedNormal(LPCTSTR newVal);
    BSTR GetbackIconImageCheckedHover();
    void SetbackIconImageCheckedHover(LPCTSTR newVal);
    BSTR GetbackIconImageCheckedDown();
    void SetbackIconImageCheckedDown(LPCTSTR newVal);
    BSTR GetbackIconImageCheckedDisabled();
    void SetbackIconImageCheckedDisabled(LPCTSTR newVal);

	LONG GeticonMargin();
	void SeticonMargin(LONG newVal);
    LONG GeticonTopMargin();
    void SeticonTopMargin(LONG newVal);
    LONG GetbackIconMargin();
    void SetbackIconMargin(LONG newVal);
    LONG GetbackIconTopMargin();
    void SetbackIconTopMargin(LONG newVal);

    LONG GeticonWidth();
    void SeticonWidth(LONG newVal);
    LONG GeticonHeight();
    void SeticonHeight(LONG newVal);
    LONG GetbackIconWidth();
    void SetbackIconWidth(LONG newVal);
    LONG GetbackIconHeight();
    void SetbackIconHeight(LONG newVal);

    VARIANT_BOOL GettextHotColorTransform();
    void SettextHotColorTransform(VARIANT_BOOL newVal);

	LONG GetlabelFontSize();
	void SetlabelFontSize(LONG newVal);
	LONG GetlabelFontWeight(void);
	void SetlabelFontWeight(LONG newVal);
	VARIANT_BOOL GetlabelItalic();
	void SetlabelItalic(VARIANT_BOOL newVal);
	VARIANT_BOOL GetlabelUnderline();
	void SetlabelUnderline(VARIANT_BOOL newVal);
    VARIANT_BOOL GetlabelNormalUnderline();
    void SetlabelNormalUnderline(VARIANT_BOOL newVal);
	VARIANT_BOOL GetlabelStrikeOut();
	void SetlabelStrikeOut(VARIANT_BOOL newVal);
	BSTR GetlabelFontFamily();
	void SetlabelFontFamily(LPCTSTR newVal);
	LONG GetlabelNormalColor();
	void SetlabelNormalColor(LONG newVal);
	LONG GetlabelHoverColor();
	void SetlabelHoverColor(LONG newVal);
	LONG GetlabelDownColor();
	void SetlabelDownColor(LONG newVal);
	LONG GetlabelDisabledColor();
	void SetlabelDisabledColor(LONG newVal);
	VARIANT_BOOL GetlabelWordBreak();
	void SetlabelWordBreak(VARIANT_BOOL newVal);
    VARIANT_BOOL GetlabelEndEllipsis();
    void SetlabelEndEllipsis(VARIANT_BOOL newVal);
	VARIANT_BOOL GetlabelSingleLine();
	void SetlabelSingleLine(VARIANT_BOOL newVal);
	BSTR GetlabelHorizAlign();
	void SetlabelHorizAlign(LPCTSTR newVal);
	BSTR GetlabelVertiAlign();
	void SetlabelVertiAlign(LPCTSTR newVal);
	//下划线间距
	LONG GetlabelUnderlineMargin(void);
	void SetlabelUnderlineMargin(LONG newVal);

	LONG GettextMargin();
	void SettextMargin(LONG newVal);

    BSTR GettextPlacement(void);
    void SettextPlacement(LPCTSTR newVal);

	VARIANT_BOOL GethyperLink(void);
	void SethyperLink(VARIANT_BOOL newVal);
	BSTR GethyperLinkAddress(void);
	void SethyperLinkAddress(LPCTSTR newVal);

	BSTR Gettip();
	void Settip(LPCTSTR newVal);

	LONG GetcmdID();
	void SetcmdID(LONG newVal);

	BSTR Getstyle();
	void Setstyle(LPCTSTR newVal);

	VARIANT_BOOL GetuseDefaultSize();
	void SetuseDefaultSize(VARIANT_BOOL newVal);

    LONG GetdownOffset();
    void SetdownOffset(LONG newVal);

    VARIANT_BOOL GetshowBkAlways();
    void SetshowBkAlways(VARIANT_BOOL newVal);

	VARIANT_BOOL Getclickable();
	void Setclickable(VARIANT_BOOL newVal);

	VARIANT_BOOL GethandleTip();
	void SethandleTip(VARIANT_BOOL newVal);

	LONG GetmaxTipWidth(void);
	void SetmaxTipWidth(LONG newVal);

protected:
    CImageBase* GetButtonImageHelper(CImageBase* pNormal, CImageBase* pHover, CImageBase* pDown);

protected:
	typedef enum enButtonState{
		BUTTON_STATE_NORMAL,
		BUTTON_STATE_DOWN,
		BUTTON_STATE_LIGHT	
	} BUTTON_STATE;

	typedef enum enButtonFlag{
		UseDefaultSize = 0x000000001
	}BUTTON_FLAG;

	//背景状态图片
	CImageBase*			m_pNormalImage;
	CImageBase*			m_pHoverImage;
	CImageBase*			m_pDownImage;
	CImageBase*			m_pDisabledImage;
	CString			m_strNormalImage;
	CString			m_strHoverImage;
	CString			m_strDownImage;
	CString			m_strDisabledImage;
    BOOL            m_bSideStretch;
    LONG            m_lSideStretchWidth;
    LONG            m_lSideStretchHeight;
    BOOL            m_bHotColorTransform;

    BOOL            m_bTextHotColorTransform;

	//图标图片
	CImageBase*			m_pIconImageUncheckedNormal;
	CString			m_strIconImageUncheckedNormal;
    CImageBase*			m_pIconImageUncheckedHover;
    CString			m_strIconImageUncheckedHover;
    CImageBase*			m_pIconImageUncheckedDown;
    CString			m_strIconImageUncheckedDown;
    CImageBase*			m_pIconImageUncheckedDisabled;
    CString			m_strIconImageUncheckedDisabled;

    CImageBase*			m_pIconImageCheckedNormal;
    CString			m_strIconImageCheckedNormal;
    CImageBase*			m_pIconImageCheckedHover;
    CString			m_strIconImageCheckedHover;
    CImageBase*			m_pIconImageCheckedDown;
    CString			m_strIconImageCheckedDown;
    CImageBase*			m_pIconImageCheckedDisabled;
    CString			m_strIconImageCheckedDisabled;

    CImageBase*     m_pAbsIconImageUncheckedNormal;
    CString         m_strAbsIconImageUncheckedNormal;

	LONG			m_nIconMargin;
    CSize		    m_szIcon;
    BOOL            m_bIconColorTransform;
    BOOL            m_bIconAlpha;
    LONG            m_nIconTopMargin;

    //后部图标图片
    CImageBase*			m_pBackIconImageUncheckedNormal;
    CString			m_strBackIconImageUncheckedNormal;
    CImageBase*			m_pBackIconImageUncheckedHover;
    CString			m_strBackIconImageUncheckedHover;
    CImageBase*			m_pBackIconImageUncheckedDown;
    CString			m_strBackIconImageUncheckedDown;
    CImageBase*			m_pBackIconImageUncheckedDisabled;
    CString			m_strBackIconImageUncheckedDisabled;

    CImageBase*			m_pBackIconImageCheckedNormal;
    CString			m_strBackIconImageCheckedNormal;
    CImageBase*			m_pBackIconImageCheckedHover;
    CString			m_strBackIconImageCheckedHover;
    CImageBase*			m_pBackIconImageCheckedDown;
    CString			m_strBackIconImageCheckedDown;
    CImageBase*			m_pBackIconImageCheckedDisabled;
    CString			m_strBackIconImageCheckedDisabled;

    LONG			m_nBackIconMargin;
    CSize		    m_szBackIcon;
    LONG            m_nBackIconTopMargin;

    //提示
	CString			m_strTip;
	LONG            m_nMaxTipWidth;
	

	//各种状态下的文本风格
	CSkinTextStyle m_textStyleNormal;
	CSkinTextStyle m_textStyleHover;
	CSkinTextStyle m_textStyleDown;
	CSkinTextStyle m_textStyleDisabled;
    BOOL m_bLabelNormalUnderline;
	//margin
	LONG			m_nTextMargin;

    TEXT_PLACEMENT  m_enTextPlacement;

	//超链接
	BOOL			m_bHyperLink;
	CString			m_strHyperLinkAddress;

	//命令ID
	DWORD			m_nCmdID;

	//鼠标状态
	BUTTON_STATE	m_nMouseState;

	//样式
	SKINBUTTON_STYLE m_eSBStyle;

	//check状态
	BOOL			 m_bChecked;

    BOOL             m_bTextColorTransform;

    LONG             m_nDownOffset;

    BOOL             m_bShowBkAlways;

	//标识
	UINT			 m_uButtonFlag;

	BOOL			 m_bClickable;

	CRect			 m_rcRealRect;
	BOOL			 m_bHandleTip;
};
