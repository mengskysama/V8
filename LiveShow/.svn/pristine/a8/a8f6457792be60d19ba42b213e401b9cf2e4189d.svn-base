
#pragma once

#include "SkinTextStyle.h"

// CSkinGroup ÃüÁîÄ¿±ê

class CSkinGroup : public CSkinBase
{
	DECLARE_DYNCREATE(CSkinGroup)

public:
	CSkinGroup();
	virtual ~CSkinGroup();

	virtual void OnFinalRelease();

protected:
	DECLARE_MESSAGE_MAP()
	DECLARE_OLECREATE(CSkinGroup)
	DECLARE_SKIN_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
protected:
	enum
	{
		SKINBASE_DISPIDS(CSkinGroup),
		SKIN_DISPID(CSkinGroup, textColor),
		SKIN_DISPID(CSkinGroup, boxLineColor),
		SKIN_DISPID(CSkinGroup, boxMargin),
		SKIN_DISPID(CSkinGroup, boxRadius),
        SKIN_DISPID(CSkinGroup, textMargin),
	};

public:
	LONG GettextColor();
	void SettextColor(LONG newVal);
	LONG GetboxLineColor();
	void SetboxLineColor(LONG newVal);
	LONG GetboxMargin();
	void SetboxMargin(LONG newVal);
	LONG GetboxRadius();
	void SetboxRadius(LONG newVal);
    LONG GettextMargin();
    void SettextMargin(LONG newVal);

protected:
	virtual void Draw(CDC* pDC, LPRECT lpInvalidRect = NULL, CRgn* pInvalidRgn = NULL);

protected:
	CSkinTextStyle m_textStyle;
	LONG m_clrBoxLineColor;
	LONG m_nBoxMargin;
	LONG m_nBoxRadius;
    LONG m_nTextMargin;
};


