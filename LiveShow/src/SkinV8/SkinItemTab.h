
#pragma once

#include "SkinTextStyle.h"
#include "SkinItem.h"


// CSkinItemTab ÃüÁîÄ¿±ê

class CSkinItemTab : public CSkinItem
{
	DECLARE_DYNCREATE(CSkinItemTab)

public:
	CSkinItemTab();
	virtual ~CSkinItemTab();

	virtual void OnFinalRelease();

protected:
	DECLARE_MESSAGE_MAP()
	DECLARE_OLECREATE(CSkinItemTab)
	DECLARE_SKIN_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()

protected:
	enum
	{
		SKINBASE_DISPIDS(CSkinItemTab),
		SKINITEM_DISPIDS(CSkinItemTab),

		SKIN_DISPID(CSkinItemTab, tabTextColor),
		SKIN_DISPID(CSkinItemTab, normalTabImage),
		SKIN_DISPID(CSkinItemTab, selectedTabImage),
		SKIN_DISPID(CSkinItemTab, tabItemWidth),
		SKIN_DISPID(CSkinItemTab, tabItemHeight),
	};
protected:
	LONG GetTabItemCount();
	LONG GetTabItemCurSel();
	BOOL GetTabItemRect(LONG iItem, RECT& rect);
	BOOL GetTabItemText(LONG iItem, CString& text);
	BOOL SetTabItemSize(const SIZE& size);
	BOOL GetAllTabItemRgn(CRgn& rgn);

protected:
	virtual void DrawBackground(CDC* pDC, LPRECT lpInvalidRect = NULL, CRgn* pInvalidRgn = NULL, BOOL bChildSkin = FALSE);
	virtual void DrawForeground(CDC* pDC, LPRECT lpInvalidRect = NULL, CRgn* pInvalidRgn = NULL);
	virtual void DrawFrame(CDC* pDC, LPRECT lpInvalidRect = NULL, CRgn* pInvalidRgn = NULL);
	virtual void Draw(CDC* pDC, LPRECT lpInvalidRect = NULL, CRgn* pInvalidRgn = NULL);
	virtual BOOL OnNcPaint(UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* lpResult);
	virtual BOOL OnPaint(UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* lpResult);

protected:
	LONG GettabItemWidth();
	void SettabItemWidth(LONG newVal);
	LONG GettabItemHeight();
	void SettabItemHeight(LONG newVal);
	LONG GettabTextColor();
	void SettabTextColor(LONG newVal);
	BSTR GetnormalTabImage();
	void SetnormalTabImage(LPCTSTR newVal);
	BSTR GetselectedTabImage();
	void SetselectedTabImage(LPCTSTR newVal);

protected:
	CSkinTextStyle m_textStyleTab;
	CImageBase*	m_pNormalTabImage;
	CString		m_strNormalTabImage;
	CImageBase*	m_pSelectedTabImage;
	CString		m_strSelectedTabImage;
	CSize		m_szItemSize;
};

