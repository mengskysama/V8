
#pragma once


// CSkinItemEdit ÃüÁîÄ¿±ê

class CSkinItemEdit : public CSkinItem
{
	DECLARE_DYNCREATE(CSkinItemEdit)

public:
	CSkinItemEdit();
	virtual ~CSkinItemEdit();

	virtual void OnFinalRelease();

protected:
	DECLARE_MESSAGE_MAP()
	DECLARE_OLECREATE(CSkinItemEdit)
	DECLARE_SKIN_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
protected:
	enum
	{
		SKINBASE_DISPIDS(CSkinItemEdit),
		SKINITEM_DISPIDS(CSkinItemEdit),

		SKIN_DISPID(CSkinItemEdit, systemEdit),
		SKIN_DISPID(CSkinItemEdit, hasBorder),
		SKIN_DISPID(CSkinItemEdit, thickBorder),
        SKIN_DISPID(CSkinItemEdit, outerBorder),
        SKIN_DISPID(CSkinItemEdit, innerBorder),
		SKIN_DISPID(CSkinItemEdit, borderColorNormal),
		SKIN_DISPID(CSkinItemEdit, borderColorHover),
		SKIN_DISPID(CSkinItemEdit, borderColorFocus),
        SKIN_DISPID(CSkinItemEdit, borderColorReadOnly),
        SKIN_DISPID(CSkinItemEdit, borderColorDisabled),
        SKIN_DISPID(CSkinItemEdit, outerBorderColorNormal),
        SKIN_DISPID(CSkinItemEdit, outerBorderColorHover),
        SKIN_DISPID(CSkinItemEdit, outerBorderColorFocus),
        SKIN_DISPID(CSkinItemEdit, outerBorderColorReadOnly),
        SKIN_DISPID(CSkinItemEdit, outerBorderColorDisabled),
        SKIN_DISPID(CSkinItemEdit, innerBorderColorNormal),
        SKIN_DISPID(CSkinItemEdit, innerBorderColorHover),
        SKIN_DISPID(CSkinItemEdit, innerBorderColorFocus),
        SKIN_DISPID(CSkinItemEdit, innerBorderColorReadOnly),
        SKIN_DISPID(CSkinItemEdit, innerBorderColorDisabled),

        SKIN_DISPID(CSkinItemEdit, bkColorReadOnly),
        SKIN_DISPID(CSkinItemEdit, textColorReadOnly),
        SKIN_DISPID(CSkinItemEdit, focusBorder),
        SKIN_DISPID(CSkinItemEdit, readOnlyBorder),
	};
public:
	VARIANT_BOOL GetsystemEdit();
	void SetsystemEdit(VARIANT_BOOL newVal);
	VARIANT_BOOL GethasBorder();
	void SethasBorder(VARIANT_BOOL newVal);
	VARIANT_BOOL GetthickBorder();
	void SetthickBorder(VARIANT_BOOL newVal);
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
    LONG GetborderColorReadOnly();
    void SetborderColorReadOnly(LONG newVal);
    LONG GetborderColorDisabled();
    void SetborderColorDisabled(LONG newVal);
    LONG GetouterBorderColorNormal();
    void SetouterBorderColorNormal(LONG newVal);
    LONG GetouterBorderColorHover();
    void SetouterBorderColorHover(LONG newVal);
    LONG GetouterBorderColorFocus();
    void SetouterBorderColorFocus(LONG newVal);
    LONG GetouterBorderColorReadOnly();
    void SetouterBorderColorReadOnly(LONG newVal);
    LONG GetouterBorderColorDisabled();
    void SetouterBorderColorDisabled(LONG newVal);
    LONG GetinnerBorderColorNormal();
    void SetinnerBorderColorNormal(LONG newVal);
    LONG GetinnerBorderColorHover();
    void SetinnerBorderColorHover(LONG newVal);
    LONG GetinnerBorderColorFocus();
    void SetinnerBorderColorFocus(LONG newVal);
    LONG GetinnerBorderColorReadOnly();
    void SetinnerBorderColorReadOnly(LONG newVal);
    LONG GetinnerBorderColorDisabled();
    void SetinnerBorderColorDisabled(LONG newVal);

    LONG GetbkColorReadOnly();
    void SetbkColorReadOnly(LONG newVal);
    LONG GettextColorReadOnly();
    void SettextColorReadOnly(LONG newVal);

    VARIANT_BOOL GetfocusBorder();
    void SetfocusBorder(VARIANT_BOOL newVal);
    VARIANT_BOOL GetreadOnlyBorder();
    void SetreadOnlyBorder(VARIANT_BOOL newVal);

public:
	static LONG ChangeSaturation(LONG color, float fRatio);
    LONG GetBorderHelper(LONG clrNormal, LONG clrHover, LONG clrFocus, LONG clrReadOnly, LONG clrDisabled);

protected:
	BOOL GetEditRect(RECT& rect);
    BOOL IsReadOnly();

protected:
	virtual void DrawBackground(CDC* pDC, LPRECT lpInvalidRect = NULL, CRgn* pInvalidRgn = NULL, BOOL bChildSkin = FALSE);
	virtual void DrawForeground(CDC* pDC, LPRECT lpInvalidRect = NULL, CRgn* pInvalidRgn = NULL);
	virtual void DrawFrame(CDC* pDC, LPRECT lpInvalidRect = NULL, CRgn* pInvalidRgn = NULL);
	virtual void Draw(CDC* pDC, LPRECT lpInvalidRect = NULL, CRgn* pInvalidRgn = NULL);
    virtual BOOL CtlColor(CDC* pDC, CWnd* pWnd, HBRUSH& hBrush);
	virtual BOOL OnNcPaint(UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* lpResult);
	virtual BOOL OnPaint(UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* lpResult);
	virtual BOOL OnSetFocus(UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* lpResult);
	virtual BOOL OnKillFocus(UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* lpResult);
    virtual BOOL OnMouseEnter(UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* lpResult);
    virtual BOOL OnMouseLeave(UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* lpResult);

protected:
	BOOL m_bSystemEdit;
	BOOL m_bHasBorder;
	BOOL m_bThickBorder;
    BOOL m_bOuterBorder;
    BOOL m_bInnerBorder;
	LONG m_clrBorderColorNormal;
	LONG m_clrBorderColorHover;
	LONG m_clrBorderColorFocus;
    LONG m_clrBorderColorReadOnly;
    LONG m_clrBorderColorDisabled;
    LONG m_clrOuterBorderColorNormal;
    LONG m_clrOuterBorderColorHover;
    LONG m_clrOuterBorderColorFocus;
    LONG m_clrOuterBorderColorReadOnly;
    LONG m_clrOuterBorderColorDisabled;
    LONG m_clrInnerBorderColorNormal;
    LONG m_clrInnerBorderColorHover;
    LONG m_clrInnerBorderColorFocus;
    LONG m_clrInnerBorderColorReadOnly;
    LONG m_clrInnerBorderColorDisabled;

    LONG m_clrBkColorReadOnly;
    LONG m_clrTextColorReadOnly;
    BOOL m_bFocusBorder;
    BOOL m_bReadOnlyBorder;
};
