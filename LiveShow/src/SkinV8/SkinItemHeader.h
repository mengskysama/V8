
#pragma once

#include "SkinItem.h"
#include "SkinTextStyle.h"

// CSkinItemHeader ÃüÁîÄ¿±ê

class CSkinItemHeader : public CSkinItem
{
	DECLARE_DYNCREATE(CSkinItemHeader)

public:
	CSkinItemHeader();
	virtual ~CSkinItemHeader();

	virtual void OnFinalRelease();

protected:
	DECLARE_MESSAGE_MAP()
	DECLARE_OLECREATE(CSkinItemHeader)
	DECLARE_SKIN_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()

protected:
	enum
	{
		SKINBASE_DISPIDS(CSkinItemHeader),
		SKINITEM_DISPIDS(CSkinItemHeader),

        SKIN_DISPID(CSkinItemHeader, itemBkColorNormal),
        SKIN_DISPID(CSkinItemHeader, itemBkColorHover),
        SKIN_DISPID(CSkinItemHeader, itemBkColorDown),
        SKIN_DISPID(CSkinItemHeader, itemInterval),
        SKIN_DISPID(CSkinItemHeader, itemIntervalBkColor),
        SKIN_DISPID(CSkinItemHeader, tailItemBkColor),
        SKIN_DISPID(CSkinItemHeader, bitmapMargin),
        SKIN_DISPID(CSkinItemHeader, textMargin),
        SKIN_DISPID(CSkinItemHeader, showHeaderIcon),
	};

public:
    LONG GetitemBkColorNormal(void);
    void SetitemBkColorNormal(LONG newVal);
    LONG GetitemBkColorHover(void);
    void SetitemBkColorHover(LONG newVal);
    LONG GetitemBkColorDown(void);
    void SetitemBkColorDown(LONG newVal);

    LONG GetitemInterval(void);
    void SetitemInterval(LONG newVal);
    LONG GetitemIntervalBkColor(void);
    void SetitemIntervalBkColor(LONG newVal);

    LONG GettailItemBkColor(void);
    void SettailItemBkColor(LONG newVal);

    LONG GetbitmapMargin();
    void SetbitmapMargin(LONG newVal);

    LONG GettextMargin();
    void SettextMargin(LONG newVal);

    VARIANT_BOOL GetshowHeaderIcon();
    void SetshowHeaderIcon(VARIANT_BOOL newVal);

public:
	BOOL GetItemCount(int& count);
	BOOL GetItemText(int iItem, CString& str);
	BOOL GetItemRect(int iItem, CRect& rect);
    BOOL GetItemState(int iItem, UINT& state);
    BOOL GetItemImage(int iItem, int& iImage);
    BOOL GetImageList(HIMAGELIST& hImageList);
    BOOL GetBitmapMargin(LONG& margin);
    BOOL SetBitmapMargin(LONG margin);
    BOOL GetPointInItem(POINT& point, int& iItem);

public:
	virtual BOOL UpdateSkinItemProperty();
	virtual void DrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct);

protected:
    virtual BOOL OnNcPaint(UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* lpResult);
	virtual BOOL OnPaint(UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* lpResult);
	virtual BOOL OnDrawItem(UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* lpResult);
    virtual BOOL OnMouseMove(UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* lpResult);
    virtual BOOL OnMouseLeave(UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* lpResult);

protected:
    int m_iLastMouseInItem;
    //item
	LONG m_clrItemBkColorNormal;
	LONG m_clrItemBkColorHover;
	LONG m_clrItemBkColorDown;
    //iterval
    LONG m_nItemInterval;
    LONG m_clrItemIntervalBkColor;
    //tail Item
    LONG m_clrTailItemBkColor;
    //Text
	CSkinTextStyle m_styleText;
    LONG m_nTextMargin;
    BOOL m_bShowHeaderIcon;
};
