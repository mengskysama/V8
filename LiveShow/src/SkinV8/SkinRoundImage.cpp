// SkinImage.cpp : implementation file
//

#include "stdafx.h"
#include "SkinEngine.h"
#include "ImageBase.h"

#include "SkinRoundImage.h"

// CSkinRoundImage

IMPLEMENT_DYNCREATE(CSkinRoundImage, CSkinBase)

CSkinRoundImage::CSkinRoundImage()
{
	EnableAutomation();

	m_pTopLeftImage = NULL;
	m_pTopMidImage = NULL;
	m_pTopRightImage = NULL;
	m_pMidLeftImage = NULL;
	m_pMidMidImage = NULL;
	m_pMidRightImage = NULL;
	m_pBottomLeftImage = NULL;
	m_pBottomMidImage = NULL;
	m_pBottomRightImage = NULL;

    m_bSideAlpha = TRUE;
    m_bCenterAlpha = FALSE;
    m_bSideStretch = FALSE;

	m_nTopWidth = 0;
	m_nTopHeight = 0;
	m_nBottomWidth = 0;
	m_nBottomHeight = 0;

	//默认为背景
	m_bBackground = TRUE;
}

CSkinRoundImage::~CSkinRoundImage()
{

}

void CSkinRoundImage::OnFinalRelease()
{
	// When the last reference for an automation object is released
	// OnFinalRelease is called.  The base class will automatically
	// deletes the object.  Add additional cleanup required for your
	// object before calling the base class.

	CCmdTarget::OnFinalRelease();
}


BEGIN_MESSAGE_MAP(CSkinRoundImage, CSkinBase)
END_MESSAGE_MAP()


BEGIN_SKIN_DISPATCH_MAP(CSkinRoundImage, CSkinBase)
	SKINBASE_DISP_PROPERTIES(CSkinRoundImage)

	SKIN_DISP_PROPERTY(CSkinRoundImage, topLeftImage, VT_BSTR)
	SKIN_DISP_PROPERTY(CSkinRoundImage, topMidImage, VT_BSTR)
	SKIN_DISP_PROPERTY(CSkinRoundImage, topRightImage, VT_BSTR)
	SKIN_DISP_PROPERTY(CSkinRoundImage, midLeftImage, VT_BSTR)
	SKIN_DISP_PROPERTY(CSkinRoundImage, midMidImage, VT_BSTR)
	SKIN_DISP_PROPERTY(CSkinRoundImage, midRightImage, VT_BSTR)
	SKIN_DISP_PROPERTY(CSkinRoundImage, bottomLeftImage, VT_BSTR)
	SKIN_DISP_PROPERTY(CSkinRoundImage, bottomMidImage, VT_BSTR)
	SKIN_DISP_PROPERTY(CSkinRoundImage, bottomRightImage, VT_BSTR)
    SKIN_DISP_PROPERTY(CSkinRoundImage, sideAlpha, VT_BOOL)
    SKIN_DISP_PROPERTY(CSkinRoundImage, centerAlpha, VT_BOOL)
    SKIN_DISP_PROPERTY(CSkinRoundImage, sideStretch, VT_BOOL)

    SKIN_DISP_PROPERTY_ALIAS(CSkinRoundImage, topImage, topMidImage, VT_BSTR)
    SKIN_DISP_PROPERTY_ALIAS(CSkinRoundImage, bottomImage, bottomMidImage, VT_BSTR)
    SKIN_DISP_PROPERTY_ALIAS(CSkinRoundImage, leftImage, midLeftImage, VT_BSTR)
    SKIN_DISP_PROPERTY_ALIAS(CSkinRoundImage, midImage, midMidImage, VT_BSTR)
    SKIN_DISP_PROPERTY_ALIAS(CSkinRoundImage, rightImage, midRightImage, VT_BSTR)

END_SKIN_DISPATCH_MAP(CSkinRoundImage)

// 注意: 我们添加 IID_ISkinRoundImage 支持
//以支持来自 VBA 的类型安全绑定。此 IID 必须同附加到 .IDL 文件中的
//调度接口的 GUID 匹配。

// {CC73E25D-DA05-4C0A-899D-ABCA0259C5D5}
static const IID IID_ISkinRoundImage =
{ 0xCC73E25D, 0xDA05, 0x4C0A, { 0x89, 0x9D, 0xAB, 0xCA, 0x2, 0x59, 0xC5, 0xD5 } };

BEGIN_INTERFACE_MAP(CSkinRoundImage, CSkinBase)
	INTERFACE_PART(CSkinRoundImage, IID_ISkinRoundImage, Dispatch)
END_INTERFACE_MAP()

// {6BD2ACC9-02F6-46DD-90D1-0BF38B9E2CD8}
IMPLEMENT_OLECREATE_FLAGS(CSkinRoundImage, "Skin.SkinRoundImage", afxRegApartmentThreading, 0x6bd2acc9, 0x2f6, 0x46dd, 0x90, 0xd1, 0xb, 0xf3, 0x8b, 0x9e, 0x2c, 0xd8)

// CSkinRoundImage message handlers
BOOL CSkinRoundImage::OnMouseEnter()
{
	return CSkinBase::OnMouseEnter();
}

BOOL CSkinRoundImage::OnMouseLeave()
{
	return CSkinBase::OnMouseLeave();
}

BOOL CSkinRoundImage::OnLButtonDown(LONG x, LONG y, UINT flag)
{
	if (IsPointInSkinRgn(x, y)) {
		SetCaptureSkin();
	} else {
		ReleaseCaptureSkin();
	}

	return CSkinBase::OnLButtonDown(x, y, flag);
}

BOOL CSkinRoundImage::OnLButtonUp(LONG x, LONG y, UINT flag)
{
	ReleaseCaptureSkin();
	if (IsPointInSkinRgn(x, y)) {
		OnClick();
	} else {
		OnMouseLeave();
	}
	return CSkinBase::OnLButtonUp(x, y, flag);
}

void CSkinRoundImage::Draw(CDC* pDC, LPRECT lpInvalidRect, CRgn* pInvalidRgn)
{
	CImageBase* pImage = NULL;
	//////////////////////////////////////////////////////////////////////////
	//上
	//////////////////////////////////////////////////////////////////////////
	if (m_pTopLeftImage != NULL) {
		CRect drawRect = GetSkinRect();
		drawRect.left = drawRect.left;
		drawRect.right = drawRect.left + m_nTopWidth;
		drawRect.top = drawRect.top;
		drawRect.bottom = drawRect.top + m_nTopHeight;
        DrawImageAlphaHelper(m_pTopLeftImage, *pDC, &drawRect, NULL, m_bAlpha && m_bSideAlpha);
	}

	pImage = m_pTopMidImage;
	if (pImage == NULL)
		pImage = m_pMidMidImage;
	if (pImage != NULL) {
		CRect drawRect = GetSkinRect();
		drawRect.left = drawRect.left + m_nTopWidth;
		drawRect.right = drawRect.right - m_nTopWidth;
		drawRect.top = drawRect.top;
        LONG height = (m_bSideStretch || m_pTopMidImage == NULL) ? (m_nTopHeight) : (m_pTopMidImage->Height());
		drawRect.bottom = drawRect.top + height;
        DrawImageAlphaHelper(pImage, *pDC, &drawRect, NULL, m_bAlpha && m_bSideAlpha);
	}

	if (m_pTopRightImage != NULL) {
		CRect drawRect = GetSkinRect();
		drawRect.left = drawRect.right - m_nTopWidth;
		drawRect.right = drawRect.right;
		drawRect.top = drawRect.top;
		drawRect.bottom = drawRect.top + m_nTopHeight;
        DrawImageAlphaHelper(m_pTopRightImage, *pDC, &drawRect, NULL, m_bAlpha && m_bSideAlpha);
	}

	//////////////////////////////////////////////////////////////////////////
	//中
	//////////////////////////////////////////////////////////////////////////
	LONG width = m_nTopWidth;
	if (width == 0)
		width = m_nBottomWidth;

    LONG midLeftWidth = (m_bSideStretch || m_pMidLeftImage == NULL) ? (width) : (m_pMidLeftImage->Width());
    LONG midRightWidth = (m_bSideStretch || m_pMidRightImage == NULL) ? (width) : (m_pMidRightImage->Width());

	pImage = m_pMidLeftImage;
	if (pImage == NULL)
		pImage = m_pMidMidImage;
	if (pImage != NULL) {
		CRect drawRect = GetSkinRect();
		drawRect.left = drawRect.left;
        LONG leftWidth = midLeftWidth;
		drawRect.right = drawRect.left + leftWidth;
		drawRect.top = drawRect.top + m_nTopHeight;
		drawRect.bottom = drawRect.bottom - m_nBottomHeight;
        DrawImageAlphaHelper(pImage, *pDC, &drawRect, NULL, m_bAlpha && m_bSideAlpha);
	}

	if (m_pMidMidImage != NULL) {
		CRect drawRect = GetSkinRect();
		drawRect.left = drawRect.left + midLeftWidth;
		drawRect.right = drawRect.right - midRightWidth;
		drawRect.top = drawRect.top + m_nTopHeight;
		drawRect.bottom = drawRect.bottom - m_nBottomHeight;
        DrawImageAlphaHelper(m_pMidMidImage, *pDC, &drawRect, NULL, m_bAlpha && m_bCenterAlpha);
	}

	pImage = m_pMidRightImage;
	if (pImage == NULL)
		pImage = m_pMidMidImage;
	if (pImage != NULL) {
		CRect drawRect = GetSkinRect();
		drawRect.left = drawRect.right - midRightWidth;
		drawRect.right = drawRect.right;
		drawRect.top = drawRect.top + m_nTopHeight;
		drawRect.bottom = drawRect.bottom - m_nBottomHeight;
        DrawImageAlphaHelper(pImage, *pDC, &drawRect, NULL, m_bAlpha && m_bSideAlpha);
	}

	//////////////////////////////////////////////////////////////////////////
	//下
	//////////////////////////////////////////////////////////////////////////
	if (m_pBottomLeftImage != NULL) {
		CRect drawRect = GetSkinRect();
		drawRect.left = drawRect.left;
		drawRect.right = drawRect.left + m_nBottomWidth;
		drawRect.top = drawRect.bottom - m_nBottomHeight;
		drawRect.bottom = drawRect.bottom;
        DrawImageAlphaHelper(m_pBottomLeftImage, *pDC, &drawRect, NULL, m_bAlpha && m_bSideAlpha);
	}

	pImage = m_pBottomMidImage;
	if (pImage == NULL)
		pImage = m_pMidMidImage;
	if (pImage != NULL) {
		CRect drawRect = GetSkinRect();
		drawRect.left = drawRect.left + m_nBottomWidth;
		drawRect.right = drawRect.right - m_nBottomWidth;
        LONG height = (m_bSideStretch || m_pBottomMidImage == NULL) ? (m_nBottomHeight) : (m_pBottomMidImage->Height());
		drawRect.top = drawRect.bottom - height;
		drawRect.bottom = drawRect.bottom;
        DrawImageAlphaHelper(pImage, *pDC, &drawRect, NULL, m_bAlpha && m_bSideAlpha);
	}

	if (m_pBottomRightImage != NULL) {
		CRect drawRect = GetSkinRect();
		drawRect.left = drawRect.right - m_nBottomWidth;
		drawRect.right = drawRect.right;
		drawRect.top = drawRect.bottom - m_nBottomHeight;
		drawRect.bottom = drawRect.bottom;
        DrawImageAlphaHelper(m_pBottomRightImage, *pDC, &drawRect, NULL, m_bAlpha && m_bSideAlpha);
	}
}

BSTR CSkinRoundImage::GettopLeftImage()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: 在此添加调度处理程序代码
	return m_strTopLeftImage.AllocSysString();
}

void CSkinRoundImage::SettopLeftImage(LPCTSTR newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: 在此添加属性处理程序代码
	if (SetImageHelper(m_pTopLeftImage, m_strTopLeftImage, newVal, FALSE)) {
		m_nTopWidth = m_pTopLeftImage->Width();
		m_nTopHeight = m_pTopLeftImage->Height();
	}
}

BSTR CSkinRoundImage::GettopMidImage()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: 在此添加调度处理程序代码
	return m_strTopMidImage.AllocSysString();
}

void CSkinRoundImage::SettopMidImage(LPCTSTR newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: 在此添加属性处理程序代码
	if (SetImageHelper(m_pTopMidImage, m_strTopMidImage, newVal, FALSE)) {
		if (m_nTopHeight == 0) {
			m_nTopHeight = m_pTopMidImage->Height();
		}
	}
}

BSTR CSkinRoundImage::GettopRightImage()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: 在此添加调度处理程序代码
	return m_strTopRightImage.AllocSysString();
}

void CSkinRoundImage::SettopRightImage(LPCTSTR newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: 在此添加属性处理程序代码
	if (SetImageHelper(m_pTopRightImage, m_strTopRightImage, newVal, FALSE)) {
		if (m_nTopWidth == 0) {
			m_nTopWidth = m_pTopRightImage->Width();
		}
		if (m_nTopHeight == 0) {
			m_nTopHeight = m_pTopRightImage->Height();
		}
	}
}

BSTR CSkinRoundImage::GetmidLeftImage()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: 在此添加调度处理程序代码
	return m_strMidLeftImage.AllocSysString();
}

void CSkinRoundImage::SetmidLeftImage(LPCTSTR newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: 在此添加属性处理程序代码
	SetImageHelper(m_pMidLeftImage, m_strMidLeftImage, newVal, FALSE);
}

BSTR CSkinRoundImage::GetmidMidImage()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: 在此添加调度处理程序代码
	return m_strMidMidImage.AllocSysString();
}

void CSkinRoundImage::SetmidMidImage(LPCTSTR newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: 在此添加属性处理程序代码
	SetImageHelper(m_pMidMidImage, m_strMidMidImage, newVal, FALSE);
}

BSTR CSkinRoundImage::GetmidRightImage()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: 在此添加调度处理程序代码
	return m_strMidRightImage.AllocSysString();
}

void CSkinRoundImage::SetmidRightImage(LPCTSTR newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: 在此添加属性处理程序代码
	SetImageHelper(m_pMidRightImage, m_strMidRightImage, newVal, FALSE);
}

BSTR CSkinRoundImage::GetbottomLeftImage()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: 在此添加调度处理程序代码
	return m_strBottomLeftImage.AllocSysString();
}

void CSkinRoundImage::SetbottomLeftImage(LPCTSTR newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: 在此添加属性处理程序代码
	if (SetImageHelper(m_pBottomLeftImage, m_strBottomLeftImage, newVal, FALSE)) {
		m_nBottomWidth = m_pBottomLeftImage->Width();
		m_nBottomHeight = m_pBottomLeftImage->Height();
	}
}

BSTR CSkinRoundImage::GetbottomMidImage()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: 在此添加调度处理程序代码
	return m_strBottomMidImage.AllocSysString();
}

void CSkinRoundImage::SetbottomMidImage(LPCTSTR newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: 在此添加属性处理程序代码
	if (SetImageHelper(m_pBottomMidImage, m_strBottomMidImage, newVal, FALSE)) {
		if (m_nBottomHeight == 0) {
			m_nBottomHeight = m_pBottomMidImage->Height();
		}
	}
}

BSTR CSkinRoundImage::GetbottomRightImage()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: 在此添加调度处理程序代码
	return m_strBottomRightImage.AllocSysString();
}

void CSkinRoundImage::SetbottomRightImage(LPCTSTR newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: 在此添加属性处理程序代码
	if (SetImageHelper(m_pBottomRightImage, m_strBottomRightImage, newVal, FALSE)) {
		if (m_nBottomWidth == 0) {
			m_nBottomWidth = m_pBottomRightImage->Width();
		}
		if (m_nBottomHeight == 0) {
			m_nBottomHeight = m_pBottomRightImage->Height();
		}
	}
}

VARIANT_BOOL CSkinRoundImage::GetsideAlpha()
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());

    // TODO: 在此添加调度处理程序代码
    return m_bSideAlpha ? (VARIANT_TRUE) : (VARIANT_FALSE);
}

void CSkinRoundImage::SetsideAlpha(VARIANT_BOOL newVal)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());

    // TODO: 在此添加调度处理程序代码
    newVal = (newVal != VARIANT_FALSE);
    if (newVal == m_bSideAlpha)
        return;
    m_bSideAlpha = newVal;
    InvalidateRect(NULL);
}

VARIANT_BOOL CSkinRoundImage::GetcenterAlpha()
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());

    // TODO: 在此添加调度处理程序代码
    return m_bCenterAlpha ? (VARIANT_TRUE) : (VARIANT_FALSE);
}

void CSkinRoundImage::SetcenterAlpha(VARIANT_BOOL newVal)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());

    // TODO: 在此添加调度处理程序代码
    newVal = (newVal != VARIANT_FALSE);
    if (newVal == m_bCenterAlpha)
        return;
    m_bCenterAlpha = newVal;
    InvalidateRect(NULL);
}

VARIANT_BOOL CSkinRoundImage::GetsideStretch()
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());

    // TODO: 在此添加调度处理程序代码
    return m_bSideStretch ? (VARIANT_TRUE) : (VARIANT_FALSE);
}

void CSkinRoundImage::SetsideStretch(VARIANT_BOOL newVal)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());

    // TODO: 在此添加调度处理程序代码
    newVal = (newVal != VARIANT_FALSE);
    if (newVal == m_bSideStretch)
        return;
    m_bSideStretch = newVal;
    InvalidateRect(NULL);
}

