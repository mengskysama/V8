
#pragma once


// CSkinItemCombo ÃüÁîÄ¿±ê

class CSkinItemCombo : public CSkinItem
{
	friend class CSkinEngine;
	DECLARE_DYNCREATE(CSkinItemCombo)

protected:
	CSkinItemCombo();
	virtual ~CSkinItemCombo();

	virtual void OnFinalRelease();

protected:
	DECLARE_MESSAGE_MAP()
	DECLARE_OLECREATE(CSkinItemCombo)
	DECLARE_SKIN_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()

protected:
	enum 
	{
		SKINBASE_DISPIDS(CSkinItemCombo),
		SKINITEM_DISPIDS(CSkinItemCombo),

		SKIN_DISPID(CSkinItemCombo, maxDropHeight),
        SKIN_DISPID(CSkinItemCombo, curSel),
		SKIN_DISPID(CSkinItemCombo, hasBorder),
		SKIN_DISPID(CSkinItemCombo, thickBorder),
        SKIN_DISPID(CSkinItemCombo, stretchButton),
        SKIN_DISPID(CSkinItemCombo, outerBorder),
        SKIN_DISPID(CSkinItemCombo, innerBorder),
		SKIN_DISPID(CSkinItemCombo, buttonBorder),
		SKIN_DISPID(CSkinItemCombo, buttonBorderMargin),
        SKIN_DISPID(CSkinItemCombo, buttonIconXOffset),
        SKIN_DISPID(CSkinItemCombo, buttonIconYOffset),
        SKIN_DISPID(CSkinItemCombo, borderColorNormal),
        SKIN_DISPID(CSkinItemCombo, borderColorHover),
        SKIN_DISPID(CSkinItemCombo, borderColorFocus),
        SKIN_DISPID(CSkinItemCombo, borderColorDisabled),
        SKIN_DISPID(CSkinItemCombo, outerBorderColorNormal),
        SKIN_DISPID(CSkinItemCombo, outerBorderColorHover),
        SKIN_DISPID(CSkinItemCombo, outerBorderColorFocus),
        SKIN_DISPID(CSkinItemCombo, outerBorderColorDisabled),
        SKIN_DISPID(CSkinItemCombo, innerBorderColorNormal),
        SKIN_DISPID(CSkinItemCombo, innerBorderColorHover),
        SKIN_DISPID(CSkinItemCombo, innerBorderColorFocus),
        SKIN_DISPID(CSkinItemCombo, innerBorderColorDisabled),
		SKIN_DISPID(CSkinItemCombo, buttonImageNormal),
        SKIN_DISPID(CSkinItemCombo, buttonImageHover),
		SKIN_DISPID(CSkinItemCombo, buttonImagePressed),
        SKIN_DISPID(CSkinItemCombo, buttonImageDisabled),
        SKIN_DISPID(CSkinItemCombo, buttonIconNormal),
        SKIN_DISPID(CSkinItemCombo, buttonIconHover),
        SKIN_DISPID(CSkinItemCombo, buttonIconPressed),
        SKIN_DISPID(CSkinItemCombo, buttonIconDisabled),
        SKIN_DISPID(CSkinItemCombo, focusBorder),
	};

protected:
	BOOL MoveWindow(LPCRECT lpRect = NULL, BOOL bRepaint = FALSE);

public:
	LONG GetmaxDropHeight(void);
	void SetmaxDropHeight(LONG newVal);
    LONG GetcurSel();
    void SetcurSel(LONG newVal);
	VARIANT_BOOL GethasBorder();
	void SethasBorder(VARIANT_BOOL newVal);
	VARIANT_BOOL GetthickBorder();
	void SetthickBorder(VARIANT_BOOL newVal);
    VARIANT_BOOL GetstretchButton();
    void SetstretchButton(VARIANT_BOOL newVal);
	VARIANT_BOOL GetbuttonBorder();
	void SetbuttonBorder(VARIANT_BOOL newVal);
	LONG GetbuttonBorderMargin();
	void SetbuttonBorderMargin(LONG newVal);
    LONG GetbuttonIconXOffset();
    void SetbuttonIconXOffset(LONG newVal);
    LONG GetbuttonIconYOffset();
    void SetbuttonIconYOffset(LONG newVal);
    VARIANT_BOOL GetouterBorder();
    void SetouterBorder(VARIANT_BOOL newVal);
    VARIANT_BOOL GetinnerBorder();
    void SetinnerBorder(VARIANT_BOOL newVal);
    LONG GetborderColorNormal();
    void SetborderColorNormal(LONG newVal);
    LONG GetborderColorHover();
    void SetborderColorHover(LONG newVal);
    LONG GetborderColorFocus();
    void SetborderColorFocus(LONG newVal);
    LONG GetborderColorDisabled();
    void SetborderColorDisabled(LONG newVal);
    LONG GetouterBorderColorNormal();
    void SetouterBorderColorNormal(LONG newVal);
    LONG GetouterBorderColorHover();
    void SetouterBorderColorHover(LONG newVal);
    LONG GetouterBorderColorFocus();
    void SetouterBorderColorFocus(LONG newVal);
    LONG GetouterBorderColorDisabled();
    void SetouterBorderColorDisabled(LONG newVal);
    LONG GetinnerBorderColorNormal();
    void SetinnerBorderColorNormal(LONG newVal);
    LONG GetinnerBorderColorHover();
    void SetinnerBorderColorHover(LONG newVal);
    LONG GetinnerBorderColorFocus();
    void SetinnerBorderColorFocus(LONG newVal);
    LONG GetinnerBorderColorDisabled();
    void SetinnerBorderColorDisabled(LONG newVal);
	BSTR GetbuttonImageNormal();
	void SetbuttonImageNormal(LPCTSTR newVal);
    BSTR GetbuttonImageHover();
    void SetbuttonImageHover(LPCTSTR newVal);
	BSTR GetbuttonImagePressed();
	void SetbuttonImagePressed(LPCTSTR newVal);
    BSTR GetbuttonImageDisabled();
    void SetbuttonImageDisabled(LPCTSTR newVal);
    BSTR GetbuttonIconNormal();
    void SetbuttonIconNormal(LPCTSTR newVal);
    BSTR GetbuttonIconHover();
    void SetbuttonIconHover(LPCTSTR newVal);
    BSTR GetbuttonIconPressed();
    void SetbuttonIconPressed(LPCTSTR newVal);
    BSTR GetbuttonIconDisabled();
    void SetbuttonIconDisabled(LPCTSTR newVal);

    VARIANT_BOOL GetfocusBorder();
    void SetfocusBorder(VARIANT_BOOL newVal);

public:
	static BOOL GetComboBoxInfoHelper(HWND hComboWnd, COMBOBOXINFO& info);
    LONG GetBorderHelper(LONG clrNormal, LONG clrHover, LONG clrFocus, LONG clrDisabled);
    CImageBase* GetButtonImageHelper(UINT uState, CImageBase* pNormal, CImageBase* pHover, CImageBase* pPressed, CImageBase* pDisabled);
	BOOL GetComboEditHwnd(HWND& hEdit);
	BOOL GetComboListHwnd(HWND& hList);
	BOOL GetComboRect(RECT& rect);
	BOOL GetEditRect(RECT& rect);
	BOOL GetButtonRect(RECT& rect);
	BOOL GetButtonState(DWORD& state);
    LONG GetCurSel();
    void SetCurSel(LONG iSel);

public:
	virtual BOOL UpdateSkinItemHwnd();
	virtual BOOL UpdateSkinItemProperty();
    virtual BOOL IsFocused();
    virtual BOOL IsHot();

    virtual LONG GetHeight();
    virtual void SetHeight(LONG newVal);

protected:
	virtual void DrawBackground(CDC* pDC, LPRECT lpInvalidRect = NULL, CRgn* pInvalidRgn = NULL, BOOL bChildSkin = FALSE);
	virtual void DrawForeground(CDC* pDC, LPRECT lpInvalidRect = NULL, CRgn* pInvalidRgn = NULL);
	virtual void DrawFrame(CDC* pDC, LPRECT lpInvalidRect = NULL, CRgn* pInvalidRgn = NULL);
	virtual void Draw(CDC* pDC, LPRECT lpInvalidRect = NULL, CRgn* pInvalidRgn = NULL);

	virtual BOOL OnNcPaint(UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* lpResult);
	virtual BOOL OnPaint(UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* lpResult);
	virtual BOOL OnCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* lpResult);
    virtual BOOL OnMouseEnter(UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* lpResult);
    virtual BOOL OnMouseLeave(UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* lpResult);

protected:
	HWND m_hEdit;
	HWND m_hList;
	CRect m_rcButton;
	LONG m_nMaxDropHeight;
	BOOL m_bHasBorder;
	BOOL m_bThickBorder;
    BOOL m_bStretchButton;
    BOOL m_bOuterBorder;
    BOOL m_bInnerBorder;
    LONG m_clrBorderColorNormal;
    LONG m_clrBorderColorHover;
    LONG m_clrBorderColorFocus;
    LONG m_clrBorderColorDisabled;
    LONG m_clrOuterBorderColorNormal;
    LONG m_clrOuterBorderColorHover;
    LONG m_clrOuterBorderColorFocus;
    LONG m_clrOuterBorderColorDisabled;
    LONG m_clrInnerBorderColorNormal;
    LONG m_clrInnerBorderColorHover;
    LONG m_clrInnerBorderColorFocus;
    LONG m_clrInnerBorderColorDisabled;
	BOOL m_bButtonBorder;
	LONG m_nButtonBorderMargin;
    BOOL m_bFocusBorder;
    CPoint m_ptButtonIconOffset;
    CImageBase* m_pButtonImageNormal;
	CString m_strButtonImageNormal;
    CImageBase* m_pButtonImageHover;
    CString m_strButtonImageHover;
	CImageBase* m_pButtonImagePressed;
	CString m_strButtonImagePressed;
    CImageBase* m_pButtonImageDisabled;
    CString m_strButtonImageDisabled;
    CImageBase* m_pButtonIconNormal;
    CString m_strButtonIconNormal;
    CImageBase* m_pButtonIconHover;
    CString m_strButtonIconHover;
    CImageBase* m_pButtonIconPressed;
    CString m_strButtonIconPressed;
    CImageBase* m_pButtonIconDisabled;
    CString m_strButtonIconDisabled;
};

