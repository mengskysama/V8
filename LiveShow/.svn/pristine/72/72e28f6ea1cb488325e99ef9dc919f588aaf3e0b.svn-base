
#pragma once

#include "SkinBase.h"

// CSkinRoundImage √¸¡Óƒø±Í
class CSkinItemDialog;
class CImageBase;
class CSkinRoundImage : public CSkinBase
{
	friend class CSkinEngine;
	DECLARE_DYNCREATE(CSkinRoundImage)

protected:
	CSkinRoundImage();
	virtual ~CSkinRoundImage();

	virtual void OnFinalRelease();

protected:
	DECLARE_MESSAGE_MAP()
	DECLARE_OLECREATE(CSkinRoundImage)
	DECLARE_SKIN_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()


protected:
	enum 
	{
		SKINBASE_DISPIDS(CSkinRoundImage),

		SKIN_DISPID(CSkinRoundImage, topLeftImage),
		SKIN_DISPID(CSkinRoundImage, topMidImage),
		SKIN_DISPID(CSkinRoundImage, topRightImage),
		SKIN_DISPID(CSkinRoundImage, midLeftImage),
		SKIN_DISPID(CSkinRoundImage, midMidImage),
		SKIN_DISPID(CSkinRoundImage, midRightImage),
		SKIN_DISPID(CSkinRoundImage, bottomLeftImage),
		SKIN_DISPID(CSkinRoundImage, bottomMidImage),
		SKIN_DISPID(CSkinRoundImage, bottomRightImage),
        SKIN_DISPID(CSkinRoundImage, sideAlpha),
        SKIN_DISPID(CSkinRoundImage, centerAlpha),
        SKIN_DISPID(CSkinRoundImage, sideStretch),

	};

public:
	// Ù–‘
	BSTR GettopLeftImage();
	void SettopLeftImage(LPCTSTR newVal);
	BSTR GettopMidImage();
	void SettopMidImage(LPCTSTR newVal);
	BSTR GettopRightImage();
	void SettopRightImage(LPCTSTR newVal);
	BSTR GetmidLeftImage();
	void SetmidLeftImage(LPCTSTR newVal);
	BSTR GetmidMidImage();
	void SetmidMidImage(LPCTSTR newVal);
	BSTR GetmidRightImage();
	void SetmidRightImage(LPCTSTR newVal);
	BSTR GetbottomLeftImage();
	void SetbottomLeftImage(LPCTSTR newVal);
	BSTR GetbottomMidImage();
	void SetbottomMidImage(LPCTSTR newVal);
	BSTR GetbottomRightImage();
	void SetbottomRightImage(LPCTSTR newVal);

    VARIANT_BOOL GetsideAlpha();
    void SetsideAlpha(VARIANT_BOOL newVal);
    VARIANT_BOOL GetcenterAlpha();
    void SetcenterAlpha(VARIANT_BOOL newVal);
    VARIANT_BOOL GetsideStretch();
    void SetsideStretch(VARIANT_BOOL newVal);

public:
	virtual BOOL OnMouseEnter();
	virtual BOOL OnMouseLeave();
	virtual BOOL OnLButtonDown(LONG x, LONG y, UINT flag);
	virtual BOOL OnLButtonUp(LONG x, LONG y, UINT flag);

public:
	void Draw(CDC* pDC, LPRECT lpInvalidRect = NULL, CRgn* pInvalidRgn = NULL);

protected:
	CImageBase* m_pTopLeftImage;
	CImageBase* m_pTopMidImage;
	CImageBase* m_pTopRightImage;
	CImageBase* m_pMidLeftImage;
	CImageBase* m_pMidMidImage;
	CImageBase* m_pMidRightImage;
	CImageBase* m_pBottomLeftImage;
	CImageBase* m_pBottomMidImage;
	CImageBase* m_pBottomRightImage;
	CString m_strTopLeftImage;
	CString m_strTopMidImage;
	CString m_strTopRightImage;
	CString m_strMidLeftImage;
	CString m_strMidMidImage;
	CString m_strMidRightImage;
	CString m_strBottomLeftImage;
	CString m_strBottomMidImage;
	CString m_strBottomRightImage;

    BOOL m_bSideAlpha;
    BOOL m_bCenterAlpha;
    BOOL m_bSideStretch;

	LONG m_nTopWidth;
	LONG m_nTopHeight;
	LONG m_nBottomWidth;
	LONG m_nBottomHeight;
};
