
#pragma once

#include "SkinItem.h"

// CSkinItemList ÃüÁîÄ¿±ê

class CSkinItemList : public CSkinItem
{
	friend class CSkinEngine;
	DECLARE_DYNCREATE(CSkinItemList)

protected:
	CSkinItemList();
	virtual ~CSkinItemList();

	virtual void OnFinalRelease();

protected:
	DECLARE_MESSAGE_MAP()
	DECLARE_OLECREATE(CSkinItemList)
	DECLARE_SKIN_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()

protected:
	enum
	{
		SKINBASE_DISPIDS(CSkinItemList),
		SKINITEM_DISPIDS(CSkinItemList),

        SKIN_DISPID(CSkinItemList, itemBkColorNormal),
        SKIN_DISPID(CSkinItemList, itemBkColorSelected),
        SKIN_DISPID(CSkinItemList, itemTextColorNormal),
        SKIN_DISPID(CSkinItemList, itemTextColorSelected),
        SKIN_DISPID(CSkinItemList, subItemTextMargin),
        SKIN_DISPID(CSkinItemList, subItemShowNoIcon),
        SKIN_DISPID(CSkinItemList, subItemIconSelectedMask),
        SKIN_DISPID(CSkinItemList, checkIcon),
        SKIN_DISPID(CSkinItemList, uncheckIcon),
        SKIN_DISPID(CSkinItemList, showHeaderIcon),
        SKIN_DISPID(CSkinItemList, customDraw),
	};
public:
    LONG GetitemBkColorNormal(void);
    void SetitemBkColorNormal(LONG newVal);
    LONG GetitemBkColorSelected(void);
    void SetitemBkColorSelected(LONG newVal);
    LONG GetitemTextColorNormal(void);
    void SetitemTextColorNormal(LONG newVal);
    LONG GetitemTextColorSelected(void);
    void SetitemTextColorSelected(LONG newVal);
    LONG GetsubItemTextMargin(void);
    void SetsubItemTextMargin(LONG newVal);
    VARIANT_BOOL GetsubItemShowNoIcon();
    void SetsubItemShowNoIcon(VARIANT_BOOL newVal);
    VARIANT_BOOL GetsubItemIconSelectedMask();
    void SetsubItemIconSelectedMask(VARIANT_BOOL newVal);
    BSTR GetcheckIcon();
    void SetcheckIcon(LPCTSTR newVal);
    BSTR GetuncheckIcon();
    void SetuncheckIcon(LPCTSTR newVal);
    VARIANT_BOOL GetshowHeaderIcon();
    void SetshowHeaderIcon(VARIANT_BOOL newVal);
    VARIANT_BOOL GetcustomDraw();
    void SetcustomDraw(VARIANT_BOOL newVal);

public:
    BOOL GetHeaderCtrl(HWND& hHeader);
    BOOL GetItemCount(int& count);
    BOOL GetItemRect(int iItem, RECT& rect, int iArea = LVIR_BOUNDS);
    BOOL GetItemState(int iItem, UINT mask, UINT& state);
    BOOL GetItemCheckState(int iItem, UINT& state);
    BOOL GetItemData(int iItem, INT& date);
    BOOL GetSubItemRect(int iItem, int iSubItem, RECT& rect, int iArea = LVIR_BOUNDS);
    BOOL GetSubItemText(int iItem, int iSubItem, CString& text);
    BOOL GetSubItemImage(int iItem, int iSubItem, int& iImage);
    BOOL GetNormalImageList(HIMAGELIST& hImageList);
    BOOL GetSmallImageList(HIMAGELIST& hImageList);
    BOOL GetStateImageList(HIMAGELIST& hImageList);
    BOOL HitHest(POINT pt, int& iItem, int& iSubItem);
    BOOL GetExtendedStyle(UINT mask, UINT& style);
    BOOL SetExtendedStyle(UINT mask, UINT style);
    BOOL IsGridLines();
    BOOL IsFullRowSelected();
    BOOL IsCheck();

    BOOL RedrawItems(int start, int end);

public:
    virtual BOOL UpdateSkinItemHwnd();
    virtual BOOL UpdateSkinItemProperty();
	virtual void DrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct);

protected:
    virtual BOOL OnLButtonDown(UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* lpResult);
    virtual BOOL OnNcPaint(UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* lpResult);
	virtual BOOL OnPaint(UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* lpResult);
	virtual BOOL OnDrawItem(UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* lpResult);
    virtual BOOL OnLVMSetItemState(UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* lpResult);

    virtual void DrawBackground(CDC* pDC, LPRECT lpInvalidRect = NULL, CRgn* pInvalidRgn = NULL, BOOL bChildSkin = FALSE);
    virtual void DrawForeground(CDC* pDC, LPRECT lpInvalidRect = NULL, CRgn* pInvalidRgn = NULL);
    virtual void DrawFrame(CDC* pDC, LPRECT lpInvalidRect = NULL, CRgn* pInvalidRgn = NULL);
    virtual void Draw(CDC* pDC, LPRECT lpInvalidRect = NULL, CRgn* pInvalidRgn = NULL);

protected:
    LONG m_clrItemBkColorNormal;
    LONG m_clrItemBkColorSelected;
    LONG m_clrItemTextColorNormal;
    LONG m_clrItemTextColorSelected;
    LONG m_lSubItemTextMargin;
    BOOL m_bSubItemShowNoIcon;
    BOOL m_bSubItemIconSelectedMask;
    CImageBase* m_pCheckIcon;
    CImageBase* m_pUnCheckIcon;
    BOOL m_bShowHeaderIcon;
    BOOL m_bCustomDraw;
};
