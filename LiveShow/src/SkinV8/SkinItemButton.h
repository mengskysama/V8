
#pragma once


// CSkinItemButton 命令目标

class CSkinItemButton : public CSkinItem
{
	friend class CSkinEngine;
	DECLARE_DYNCREATE(CSkinItemButton)

protected:
	CSkinItemButton();
	virtual ~CSkinItemButton();

	virtual void OnFinalRelease();

protected:
	DECLARE_MESSAGE_MAP()
	DECLARE_OLECREATE(CSkinItemButton)
	DECLARE_SKIN_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
protected:
	enum 
	{
		SKINBASE_DISPIDS(CSkinItemButton),
		SKINITEM_DISPIDS(CSkinItemButton),

		SKIN_DISPID(CSkinItemButton, buttonStyle),
		SKIN_DISPID(CSkinItemButton, textLeftMargin),
        SKIN_DISPID(CSkinItemButton, textColorTransform),
		SKIN_DISPID(CSkinItemButton, checked),

		SKIN_DISPID(CSkinItemButton, normalImage0),
		SKIN_DISPID(CSkinItemButton, normalImage1),
		SKIN_DISPID(CSkinItemButton, normalImage2),
		SKIN_DISPID(CSkinItemButton, normalImage3),
		SKIN_DISPID(CSkinItemButton, normalImage4),
		SKIN_DISPID(CSkinItemButton, normalImage5),
		SKIN_DISPID(CSkinItemButton, normalImage6),
        SKIN_DISPID(CSkinItemButton, normalImage7),

		SKIN_DISPID(CSkinItemButton, checkImage0),
		SKIN_DISPID(CSkinItemButton, checkImage1),
		SKIN_DISPID(CSkinItemButton, checkImage2),
		SKIN_DISPID(CSkinItemButton, checkImage3),
		SKIN_DISPID(CSkinItemButton, checkImage4),
		SKIN_DISPID(CSkinItemButton, checkImage5),
		SKIN_DISPID(CSkinItemButton, checkImage6),
        SKIN_DISPID(CSkinItemButton, checkImage7),

		SKIN_DISPID(CSkinItemButton, radioImage0),
		SKIN_DISPID(CSkinItemButton, radioImage1),
		SKIN_DISPID(CSkinItemButton, radioImage2),
		SKIN_DISPID(CSkinItemButton, radioImage3),
		SKIN_DISPID(CSkinItemButton, radioImage4),
		SKIN_DISPID(CSkinItemButton, radioImage5),
		SKIN_DISPID(CSkinItemButton, radioImage6),
        SKIN_DISPID(CSkinItemButton, radioImage7),

	};

protected:
	enum SKINITEM_BUTTON_STYLE
	{
		BUTTON_STYLE_NORMAL,
		BUTTON_STYLE_CHECK,
		BUTTON_STYLE_RADIO,
	};
protected:
	BOOL GetCheck(UINT& uCheck);
	BOOL SetCheck(UINT uCheck);
	BOOL GetState(UINT& uState);
	BOOL SetState(UINT uState);
	BOOL GetTextMargin(LPRECT pMargin);
	BOOL SetTextMargin(LPRECT pMargin);
protected:
	BSTR GetbuttonStyle();
	void SetbuttonStyle(LPCTSTR newVal);
	LONG GettextLeftMargin();
	void SettextLeftMargin(LONG newVal);
	VARIANT_BOOL Getchecked();
	void Setchecked(VARIANT_BOOL newVal);
    VARIANT_BOOL GettextColorTransform();
    void SettextColorTransform(VARIANT_BOOL newVal);

	//////////////////////////////////////////////////////////////////////////
	//normal
	//////////////////////////////////////////////////////////////////////////
	BSTR GetnormalImage0();
	void SetnormalImage0(LPCTSTR newVal);
	BSTR GetnormalImage1();
	void SetnormalImage1(LPCTSTR newVal);
	BSTR GetnormalImage2();
	void SetnormalImage2(LPCTSTR newVal);
	BSTR GetnormalImage3();
	void SetnormalImage3(LPCTSTR newVal);
	BSTR GetnormalImage4();
	void SetnormalImage4(LPCTSTR newVal);
	BSTR GetnormalImage5();
	void SetnormalImage5(LPCTSTR newVal);
	BSTR GetnormalImage6();
	void SetnormalImage6(LPCTSTR newVal);
    BSTR GetnormalImage7();
    void SetnormalImage7(LPCTSTR newVal);
	//////////////////////////////////////////////////////////////////////////
	//check 
	//////////////////////////////////////////////////////////////////////////
	BSTR GetcheckImage0();
	void SetcheckImage0(LPCTSTR newVal);
	BSTR GetcheckImage1();
	void SetcheckImage1(LPCTSTR newVal);
	BSTR GetcheckImage2();
	void SetcheckImage2(LPCTSTR newVal);
	BSTR GetcheckImage3();
	void SetcheckImage3(LPCTSTR newVal);
	BSTR GetcheckImage4();
	void SetcheckImage4(LPCTSTR newVal);
	BSTR GetcheckImage5();
	void SetcheckImage5(LPCTSTR newVal);
	BSTR GetcheckImage6();
	void SetcheckImage6(LPCTSTR newVal);
    BSTR GetcheckImage7();
    void SetcheckImage7(LPCTSTR newVal);
	//////////////////////////////////////////////////////////////////////////
	//radio 
	//////////////////////////////////////////////////////////////////////////
	BSTR GetradioImage0();
	void SetradioImage0(LPCTSTR newVal);
	BSTR GetradioImage1();
	void SetradioImage1(LPCTSTR newVal);
	BSTR GetradioImage2();
	void SetradioImage2(LPCTSTR newVal);
	BSTR GetradioImage3();
	void SetradioImage3(LPCTSTR newVal);
	BSTR GetradioImage4();
	void SetradioImage4(LPCTSTR newVal);
	BSTR GetradioImage5();
	void SetradioImage5(LPCTSTR newVal);
	BSTR GetradioImage6();
	void SetradioImage6(LPCTSTR newVal);
    BSTR GetradioImage7();
    void SetradioImage7(LPCTSTR newVal);

protected:
	virtual void DrawBackground(CDC* pDC, LPRECT lpInvalidRect = NULL, CRgn* pInvalidRgn = NULL, BOOL bChildSkin = FALSE);
	virtual void DrawForeground(CDC* pDC, LPRECT lpInvalidRect = NULL, CRgn* pInvalidRgn = NULL);
	virtual void DrawFrame(CDC* pDC, LPRECT lpInvalidRect = NULL, CRgn* pInvalidRgn = NULL);
	virtual void Draw(CDC* pDC, LPRECT lpInvalidRect = NULL, CRgn* pInvalidRgn = NULL);
	//更新皮肤属性，在皮肤新附着窗口时调用
	virtual BOOL OnPaint(UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* lpResult);
	virtual BOOL OnEnable(UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* lpResult);
	virtual BOOL OnSetText(UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* lpResult);
	virtual BOOL OnBMSetState(UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* lpResult);
	virtual BOOL OnBMSetCheck(UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* lpResult);

protected:
	CImageBase* m_pNormalImages[8];
	CImageBase* m_pCheckImages[8];
	CImageBase* m_pRadioImages[8];
	SKINITEM_BUTTON_STYLE m_enButtonStyle;
	LONG m_nTextLeftMargin;
    BOOL m_bTextColorTransform;
};
