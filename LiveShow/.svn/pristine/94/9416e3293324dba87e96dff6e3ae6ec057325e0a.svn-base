#pragma once

#include "SkinBase.h"
#include "SkinTextStyle.h"
#include "SkinTextStyleDefine.h"
#include <vector>

// CSkinTab command target

class CSkinTab : public CSkinBase
{
	DECLARE_DYNCREATE(CSkinTab)

public:
	CSkinTab();
	virtual ~CSkinTab();

	virtual void OnFinalRelease();

protected:
	DECLARE_MESSAGE_MAP()
    DECLARE_SKIN_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()

	enum 
	{
		SKINBASE_DISPIDS(CSkinTab),

        SKIN_DISPID(CSkinTab, tabStyle),
        SKIN_DISPID(CSkinTab, tabSkinControl),
        SKIN_DISPID(CSkinTab, imageBackground),
        SKIN_DISPID(CSkinTab, imageNormal),
        SKIN_DISPID(CSkinTab, imageHover),
        SKIN_DISPID(CSkinTab, imageDown),
        SKIN_DISPID(CSkinTab, imageSelected),
        SKIN_DISPID(CSkinTab, imageSeparator),
        SKIN_DISPID(CSkinTab, tabItemOffset),

        SKIN_DISPID(CSkinTab, hasButton),
        SKIN_DISPID(CSkinTab, buttonDisabled),
        SKIN_DISPID(CSkinTab, buttonImageNormal),
        SKIN_DISPID(CSkinTab, buttonImageHover),
        SKIN_DISPID(CSkinTab, buttonImageDown),
        SKIN_DISPID(CSkinTab, buttonImageDisabled),
        SKIN_DISPID(CSkinTab, buttonTopMargin),
        SKIN_DISPID(CSkinTab, buttonLeftMargin),

        SKIN_DISPID(CSkinTab, textMargin),
        SKIN_DISPID(CSkinTab, minItemWidth),
        SKIN_DISPID(CSkinTab, maxItemWidth),
        SKIN_DISPID(CSkinTab, minItemHeight),
        SKIN_DISPID(CSkinTab, maxItemHeight),
        SKIN_DISPID(CSkinTab, imageCornerWidth),
        SKIN_DISPID(CSkinTab, imageCornerHeight),
        SKIN_DISPID(CSkinTab, tabOverlap),
        SKIN_DISPID(CSkinTab, tabItemSelected),
        SKIN_DISPID(CSkinTab, AddTabItem),
        SKIN_DISPID(CSkinTab, RemoveTabItem),
        SKIN_DISPID(CSkinTab, GetTabItemCount),
        SKIN_DISPID(CSkinTab, GetTabItemRange),
        SKIN_DISPID(CSkinTab, GetTabItemText),
        SKIN_DISPID(CSkinTab, SetTabItemText),
        SKIN_DISPID(CSkinTab, GetTabItemSkin),
        SKIN_DISPID(CSkinTab, SetTabItemSkin),
        SKIN_DISPID(CSkinTab, HitTest),

        SKIN_TEXT_STYLE_PROP_PREFIX_DISPIDS(CSkinTab, text)
	};

protected:
    virtual BOOL OnMouseEnter();
    virtual BOOL OnMouseLeave();
    virtual BOOL OnMouseMove(LONG x, LONG y, UINT flag);
    virtual BOOL OnLButtonDown(LONG x, LONG y, UINT flag);
    virtual BOOL OnLButtonUp(LONG x, LONG y, UINT flag);

    void Draw(CDC* pDC, LPRECT lpInvalidRect = NULL, CRgn* pInvalidRgn = NULL);

protected:
    struct SkinTabItemInfo
    {
        SkinTabItemInfo()
        {
            strText = _T("");
            strSkinName = _T("");
            rcRect = CRect(0, 0, 0, 0);
        };
        CString strText;
        CString strSkinName;
        CRect rcRect;
    };

    enum SKIN_TAB_TYPE
    {
        HORIZ_TAB,
        VERTI_TAB,
    };

public:
    // Ù–‘
    DECLARE_SKIN_TEXT_STYLE_PREFIX_PROPERTIES(text)

    BSTR GettabStyle();
    void SettabStyle(LPCTSTR newVal);
    VARIANT_BOOL GettabSkinControl();
    void SettabSkinControl(VARIANT_BOOL newVal);

    BSTR GetimageBackground();
    void SetimageBackground(LPCTSTR newVal);

    BSTR GetimageNormal();
    void SetimageNormal(LPCTSTR newVal);
    BSTR GetimageHover();
    void SetimageHover(LPCTSTR newVal);
    BSTR GetimageDown();
    void SetimageDown(LPCTSTR newVal);
    BSTR GetimageSelected();
    void SetimageSelected(LPCTSTR newVal);

    BSTR GetimageSeparator();
    void SetimageSeparator(LPCTSTR newVal);

    VARIANT_BOOL GethasButton();
    void SethasButton(VARIANT_BOOL newVal);
    VARIANT_BOOL GetbuttonDisabled();
    void SetbuttonDisabled(VARIANT_BOOL newVal);
    BSTR GetbuttonImageNormal();
    void SetbuttonImageNormal(LPCTSTR newVal);
    BSTR GetbuttonImageHover();
    void SetbuttonImageHover(LPCTSTR newVal);
    BSTR GetbuttonImageDown();
    void SetbuttonImageDown(LPCTSTR newVal);
    BSTR GetbuttonImageDisabled();
    void SetbuttonImageDisabled(LPCTSTR newVal);

    LONG GetbuttonTopMargin();
    void SetbuttonTopMargin(LONG newVal);
    LONG GetbuttonLeftMargin();
    void SetbuttonLeftMargin(LONG newVal);

    LONG GettextMargin();
    void SettextMargin(LONG newVal);

    LONG GetminItemWidth();
    void SetminItemWidth(LONG newVal);
    LONG GetmaxItemWidth();
    void SetmaxItemWidth(LONG newVal);
    LONG GetminItemHeight();
    void SetminItemHeight(LONG newVal);
    LONG GetmaxItemHeight();
    void SetmaxItemHeight(LONG newVal);

    LONG GetimageCornerWidth();
    void SetimageCornerWidth(LONG newVal);
    LONG GetimageCornerHeight();
    void SetimageCornerHeight(LONG newVal);

    LONG GettabOverlap();
    void SettabOverlap(LONG newVal);

    LONG GettabItemSelected();
    void SettabItemSelected(LONG newVal);

    LONG GettabItemOffset();
    void SettabItemOffset(LONG newVal);

    //∑Ω∑®
    VARIANT_BOOL AddTabItem(LONG nPos);
    VARIANT_BOOL RemoveTabItem(LONG nPos);
    LONG GetTabItemCount();
    LONG GetTabItemRange();
    BSTR GetTabItemText(LONG nItem);
    VARIANT_BOOL SetTabItemText(LONG nItem, LPCTSTR lpzText);
    BSTR GetTabItemSkin(LONG nItem);
    VARIANT_BOOL SetTabItemSkin(LONG nItem, LPCTSTR lpzName);
    LONG HitTest(LONG x, LONG y);

#ifdef V8_JS_ENGINE_SUPPORT
    v8::Handle<v8::Value> V8AddTabItem(const v8::Arguments& args);
    v8::Handle<v8::Value> V8GetTabItemCount(const v8::Arguments& args);
    v8::Handle<v8::Value> V8GetTabItemRange(const v8::Arguments& args);
    v8::Handle<v8::Value> V8RemoveTabItem(const v8::Arguments& args);
    v8::Handle<v8::Value> V8GetTabItemText(const v8::Arguments& args);
    v8::Handle<v8::Value> V8SetTabItemText(const v8::Arguments& args);
    v8::Handle<v8::Value> V8GetTabItemSkin(const v8::Arguments& args);
    v8::Handle<v8::Value> V8SetTabItemSkin(const v8::Arguments& args);
    v8::Handle<v8::Value> V8HitTest(const v8::Arguments& args);
    virtual v8::Handle<v8::Value> V8InternalCallMethod(const CString& funcName, const v8::Arguments& args);
#endif //V8_JS_ENGINE_SUPPORT

protected:
    void UpdateTabItemRect();
    void UpdateTabItemSkin(LONG iSelected);
    LONG TabItemHitTest(POINT pt);

protected:
    CImageBase* GetImageHelper(CImageBase* pImage0, CImageBase* pImage1, CImageBase* pImage2, CImageBase* pImage3);

protected:
    SKIN_TAB_TYPE m_enTabType;
    BOOL m_bTabSkinControl;

    CImageBase* m_pImageBackground;

    CImageBase* m_pImageNormal;
    CImageBase* m_pImageHover;
    CImageBase* m_pImageDown;
    CImageBase* m_pImageSelected;

    CImageBase* m_pImageSeparator;

    BOOL m_bHasButton;
    BOOL m_bButtonDisabled;
    CImageBase* m_pButtonImageNormal;
    CImageBase* m_pButtonImageHover;
    CImageBase* m_pButtonImageDown;
    CImageBase* m_pButtonImageDisabled;
    CRect m_rcButton;
    LONG m_nButtonLeftMargin;
    LONG m_nButtonTopMargin;

    LONG m_nTextMargin;

    LONG m_nMinItemWidth;
    LONG m_nMaxItemWidth;
    LONG m_nMinItemHeight;
    LONG m_nMaxItemHeight;
    LONG m_nTabOverlap;
    LONG m_nImageCornerWidth;
    LONG m_nImageCornerHeight;
    std::vector<SkinTabItemInfo> m_vecTabItems;
    LONG m_nTabItemSelected;
    LONG m_nTabItemHot;
    LONG m_nTabItemClick;

    LONG m_nTabItemOffset;

    CSkinTextStyle m_textStyle;
};
