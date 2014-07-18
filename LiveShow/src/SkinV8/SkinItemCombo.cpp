// SkinItemCombo.cpp : 实现文件
//

#include "stdafx.h"
#include "commctrl.h"
#include "SkinItemDialog.h"
#include "SkinItem.h"
#include "SkinItemEdit.h"
#include "SkinEngine.h"
#include "ImageBase.h"
#include "SkinItemCombo.h"
#include ".\skinitemcombo.h"
#include "RenderEngine.h"

#pragma warning (disable:4311)

#ifndef CB_GETCOMBOBOXINFO
#define CB_GETCOMBOBOXINFO          0x0164
#endif

// CSkinItemCombo

IMPLEMENT_DYNCREATE(CSkinItemCombo, CSkinItem)
CSkinItemCombo::CSkinItemCombo()
{
	EnableAutomation();
	// 为了使应用程序在 OLE 自动化对象处于活动状态时保持
	//	运行，构造函数调用 AfxOleLockApp。

	m_hEdit = NULL;
	m_hList = NULL;
	m_rcButton = CRect(0, 0, 0, 0);

	m_nMaxDropHeight = 200;

	m_bHasBorder = TRUE;

    m_bStretchButton = FALSE;
	m_bButtonBorder = FALSE;
	m_nButtonBorderMargin = 1;
    m_ptButtonIconOffset = CPoint(0, 0);

    m_bThickBorder = FALSE;
    m_bOuterBorder = FALSE;
    m_bInnerBorder = FALSE;

    m_bFocusBorder = TRUE;

    m_clrBorderColorNormal = RGB(165, 189, 223);//RGB(128, 128, 128);//RGB(143, 172, 219);
    m_clrBorderColorHover = RGB(165, 189, 223);//m_clrBorderColorNormal;
    m_clrBorderColorFocus = RGB(165, 189, 223);//m_clrBorderColorNormal;//RGB(0x61, 0x8E, 0xDA);
    m_clrBorderColorDisabled = RGB(128, 128, 128);
    m_clrOuterBorderColorNormal = m_clrBorderColorNormal;
    m_clrOuterBorderColorHover = m_clrBorderColorHover;
    m_clrOuterBorderColorFocus = m_clrBorderColorFocus;
    m_clrOuterBorderColorDisabled = m_clrBorderColorDisabled;
    m_clrInnerBorderColorNormal = m_clrBorderColorNormal;
    m_clrInnerBorderColorHover = m_clrBorderColorHover;
    m_clrInnerBorderColorFocus = m_clrBorderColorFocus;
    m_clrInnerBorderColorDisabled = m_clrBorderColorDisabled;

	m_pButtonImageNormal = NULL;
	m_strButtonImageNormal;
    m_pButtonImageHover = NULL;
    m_strButtonImageHover;
	m_pButtonImagePressed = NULL;
	m_strButtonImagePressed;
    m_pButtonImageDisabled= NULL;
    m_strButtonImageDisabled;
    m_pButtonIconNormal = NULL;
    m_strButtonIconNormal;
    m_pButtonIconHover = NULL;
    m_strButtonIconHover;
    m_pButtonIconPressed = NULL;
    m_strButtonIconPressed;
    m_pButtonIconDisabled = NULL;
    m_strButtonIconDisabled;

	AfxOleLockApp();
}

CSkinItemCombo::~CSkinItemCombo()
{
	// 为了在用 OLE 自动化创建所有对象后终止应用程序，
	// 	析构函数调用 AfxOleUnlockApp。
	
	AfxOleUnlockApp();
}


void CSkinItemCombo::OnFinalRelease()
{
	// 释放了对自动化对象的最后一个引用后，将调用
	// OnFinalRelease。基类将自动
	// 删除该对象。在调用该基类之前，请添加您的
	// 对象所需的附加清除代码。

	CCmdTarget::OnFinalRelease();
}


BEGIN_MESSAGE_MAP(CSkinItemCombo, CSkinItem)
END_MESSAGE_MAP()


BEGIN_SKIN_DISPATCH_MAP(CSkinItemCombo, CSkinItem)
	SKINBASE_DISP_PROPERTIES(CSkinItemCombo)
	SKINITEM_DISP_PROPERTIES(CSkinItemCombo)

	SKIN_DISP_PROPERTY(CSkinItemCombo, maxDropHeight, VT_I4)
    SKIN_DISP_PROPERTY(CSkinItemCombo, curSel, VT_I4)
	SKIN_DISP_PROPERTY(CSkinItemCombo, hasBorder, VT_BOOL)
	SKIN_DISP_PROPERTY(CSkinItemCombo, thickBorder, VT_BOOL)
    SKIN_DISP_PROPERTY(CSkinItemCombo, outerBorder, VT_BOOL)
    SKIN_DISP_PROPERTY(CSkinItemCombo, innerBorder, VT_BOOL)
    SKIN_DISP_PROPERTY(CSkinItemCombo, stretchButton, VT_BOOL)
	SKIN_DISP_PROPERTY(CSkinItemCombo, buttonBorder, VT_BOOL)
	SKIN_DISP_PROPERTY(CSkinItemCombo, buttonBorderMargin, VT_I4)
    SKIN_DISP_PROPERTY(CSkinItemCombo, buttonIconXOffset, VT_I4)
    SKIN_DISP_PROPERTY(CSkinItemCombo, buttonIconYOffset, VT_I4)
    SKIN_DISP_PROPERTY(CSkinItemCombo, borderColorNormal, VT_I4)
    SKIN_DISP_PROPERTY(CSkinItemCombo, borderColorHover, VT_I4)
    SKIN_DISP_PROPERTY(CSkinItemCombo, borderColorFocus, VT_I4)
    SKIN_DISP_PROPERTY(CSkinItemCombo, borderColorDisabled, VT_I4)
    SKIN_DISP_PROPERTY(CSkinItemCombo, outerBorderColorNormal, VT_I4)
    SKIN_DISP_PROPERTY(CSkinItemCombo, outerBorderColorHover, VT_I4)
    SKIN_DISP_PROPERTY(CSkinItemCombo, outerBorderColorFocus, VT_I4)
    SKIN_DISP_PROPERTY(CSkinItemCombo, outerBorderColorDisabled, VT_I4)
    SKIN_DISP_PROPERTY(CSkinItemCombo, innerBorderColorNormal, VT_I4)
    SKIN_DISP_PROPERTY(CSkinItemCombo, innerBorderColorHover, VT_I4)
    SKIN_DISP_PROPERTY(CSkinItemCombo, innerBorderColorFocus, VT_I4)
    SKIN_DISP_PROPERTY(CSkinItemCombo, innerBorderColorDisabled, VT_I4)

	SKIN_DISP_PROPERTY(CSkinItemCombo, buttonImageNormal, VT_BSTR)
    SKIN_DISP_PROPERTY(CSkinItemCombo, buttonImageHover, VT_BSTR)
	SKIN_DISP_PROPERTY(CSkinItemCombo, buttonImagePressed, VT_BSTR)
	SKIN_DISP_PROPERTY(CSkinItemCombo, buttonImageDisabled, VT_BSTR)
    SKIN_DISP_PROPERTY(CSkinItemCombo, buttonIconNormal, VT_BSTR)
    SKIN_DISP_PROPERTY(CSkinItemCombo, buttonIconHover, VT_BSTR)
    SKIN_DISP_PROPERTY(CSkinItemCombo, buttonIconPressed, VT_BSTR)
    SKIN_DISP_PROPERTY(CSkinItemCombo, buttonIconDisabled, VT_BSTR)
    SKIN_DISP_PROPERTY(CSkinItemCombo, focusBorder, VT_BOOL)

END_SKIN_DISPATCH_MAP(CSkinItemCombo)

// 注意: 我们添加 IID_ISkinItemCombo 支持
//以支持来自 VBA 的类型安全绑定。此 IID 必须同附加到 .IDL 文件中的
//调度接口的 GUID 匹配。

// {DC1E4C5A-3DE5-49E7-A9A2-B45160EF3C5C}
static const IID IID_ISkinItemCombo =
{ 0xDC1E4C5A, 0x3DE5, 0x49E7, { 0xA9, 0xA2, 0xB4, 0x51, 0x60, 0xEF, 0x3C, 0x5C } };

BEGIN_INTERFACE_MAP(CSkinItemCombo, CSkinItem)
	INTERFACE_PART(CSkinItemCombo, IID_ISkinItemCombo, Dispatch)
END_INTERFACE_MAP()

// {672B3FCD-6CA9-4045-9240-924C82240CEC}
IMPLEMENT_OLECREATE_FLAGS(CSkinItemCombo, "Skin.SkinItemCombo", afxRegApartmentThreading, 0x672b3fcd, 0x6ca9, 0x4045, 0x92, 0x40, 0x92, 0x4c, 0x82, 0x24, 0xc, 0xec)


// CSkinItemCombo 消息处理程序
LONG CSkinItemCombo::GetHeight()
{
    if (! ::IsWindow(m_hWnd)) {
        return CSkinItem::GetHeight();
    }

    CRect rcClient = CRect(0, 0, 0, 0);
    ::GetClientRect(m_hWnd, &rcClient);
    return rcClient.Height();
}

void CSkinItemCombo::SetHeight(LONG newVal)
{
    LONG oldHeight = GetHeight();
    if (newVal < 0 || newVal == oldHeight)
        return;

    //Do nothing
}

BOOL CSkinItemCombo::MoveWindow(LPCRECT lpRect, BOOL bRepaint)
{
	if (! ::IsWindow(m_hWnd))
		return FALSE;

	CRect rect;
	if (lpRect != NULL)
		rect = *lpRect;
	else
		rect = GetSkinRect();

	if (GetComctl32Version() <= 5) {
		rect.bottom += GetmaxDropHeight();
	} else {
		rect.bottom = rect.top + GetmaxDropHeight();
	}

    /*
    注意：改变ComboBox高度时会引发edit控件选中，因此设置的高度最好和ComboBox的一致
    */
    return MoveSkinItemHelper(rect, bRepaint);
}

LONG CSkinItemCombo::GetmaxDropHeight(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: 在此添加调度处理程序代码

	return m_nMaxDropHeight;
}

void CSkinItemCombo::SetmaxDropHeight(LONG newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: 在此添加属性处理程序代码
	if (newVal >= 0) {
		m_nMaxDropHeight = newVal;
		MoveWindow(NULL, FALSE);
	}
}

LONG CSkinItemCombo::GetcurSel()
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());

    // TODO: 在此添加调度处理程序代码
    return GetCurSel();
}

void CSkinItemCombo::SetcurSel(LONG newVal)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());

    // TODO: 在此添加调度处理程序代码
    SetCurSel(newVal);
}

VARIANT_BOOL CSkinItemCombo::GethasBorder()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: 在此添加调度处理程序代码
	return m_bHasBorder ? (VARIANT_TRUE) : (VARIANT_FALSE);
}

void CSkinItemCombo::SethasBorder(VARIANT_BOOL newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: 在此添加属性处理程序代码
	newVal = (newVal != VARIANT_FALSE);
	if (m_bHasBorder == newVal)
		return;

	m_bHasBorder = newVal;
	InvalidateRect();
}

VARIANT_BOOL CSkinItemCombo::GetthickBorder()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: 在此添加调度处理程序代码
	return m_bThickBorder ? (VARIANT_TRUE) : (VARIANT_FALSE);
}

void CSkinItemCombo::SetthickBorder(VARIANT_BOOL newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: 在此添加属性处理程序代码
	newVal = (newVal != VARIANT_FALSE);
	if (m_bThickBorder == newVal)
		return;

	m_bThickBorder = newVal;
	InvalidateRect();
}

VARIANT_BOOL CSkinItemCombo::GetstretchButton()
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());

    // TODO: 在此添加调度处理程序代码
    return m_bStretchButton ? (VARIANT_TRUE) : (VARIANT_FALSE);
}

void CSkinItemCombo::SetstretchButton(VARIANT_BOOL newVal)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());

    // TODO: 在此添加属性处理程序代码
    newVal = (newVal != VARIANT_FALSE);
    if (m_bStretchButton == newVal)
        return;

    m_bStretchButton = newVal;
    InvalidateRect();
}

VARIANT_BOOL CSkinItemCombo::GetbuttonBorder()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: 在此添加调度处理程序代码
	return m_bButtonBorder ? (VARIANT_TRUE) : (VARIANT_FALSE);
}

void CSkinItemCombo::SetbuttonBorder(VARIANT_BOOL newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: 在此添加属性处理程序代码
	newVal = (newVal != VARIANT_FALSE);
	if (m_bButtonBorder == newVal)
		return;

	m_bButtonBorder = newVal;
	InvalidateRect();
}

LONG CSkinItemCombo::GetbuttonBorderMargin(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: 在此添加调度处理程序代码
	return m_nButtonBorderMargin;
}

void CSkinItemCombo::SetbuttonBorderMargin(LONG newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: 在此添加属性处理程序代码
	if (m_nButtonBorderMargin == newVal)
		return;

	m_nButtonBorderMargin = newVal;
	InvalidateRect();
}

LONG CSkinItemCombo::GetbuttonIconXOffset(void)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());

    // TODO: 在此添加调度处理程序代码
    return m_ptButtonIconOffset.x;
}

void CSkinItemCombo::SetbuttonIconXOffset(LONG newVal)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());

    // TODO: 在此添加属性处理程序代码
    if (m_ptButtonIconOffset.x == newVal)
        return;

    m_ptButtonIconOffset.x = newVal;
    InvalidateRect();
}

LONG CSkinItemCombo::GetbuttonIconYOffset(void)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());

    // TODO: 在此添加调度处理程序代码
    return m_ptButtonIconOffset.y;
}

void CSkinItemCombo::SetbuttonIconYOffset(LONG newVal)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());

    // TODO: 在此添加属性处理程序代码
    if (m_ptButtonIconOffset.y == newVal)
        return;

    m_ptButtonIconOffset.y = newVal;
    InvalidateRect();
}

VARIANT_BOOL CSkinItemCombo::GetouterBorder()
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());

    // TODO: 在此添加调度处理程序代码
    return m_bOuterBorder ? (VARIANT_TRUE) : (VARIANT_FALSE);
}

void CSkinItemCombo::SetouterBorder(VARIANT_BOOL newVal)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());

    // TODO: 在此添加属性处理程序代码
    newVal = (newVal != VARIANT_FALSE);
    if (m_bOuterBorder == newVal)
        return;

    m_bOuterBorder = newVal;
    InvalidateRect();
}

VARIANT_BOOL CSkinItemCombo::GetinnerBorder()
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());

    // TODO: 在此添加调度处理程序代码
    return m_bInnerBorder ? (VARIANT_TRUE) : (VARIANT_FALSE);
}

void CSkinItemCombo::SetinnerBorder(VARIANT_BOOL newVal)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());

    // TODO: 在此添加属性处理程序代码
    newVal = (newVal != VARIANT_FALSE);
    if (m_bInnerBorder == newVal)
        return;

    m_bInnerBorder = newVal;
    InvalidateRect();
}

LONG CSkinItemCombo::GetborderColorNormal(void)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());

    // TODO: 在此添加调度处理程序代码
    return m_clrBorderColorNormal;
}

void CSkinItemCombo::SetborderColorNormal(LONG newVal)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());

    // TODO: 在此添加属性处理程序代码
    if (m_clrBorderColorNormal == newVal)
        return;

    m_clrBorderColorNormal = newVal;
    InvalidateRect();
}

LONG CSkinItemCombo::GetborderColorHover(void)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());

    // TODO: 在此添加调度处理程序代码
    return m_clrBorderColorHover;
}

void CSkinItemCombo::SetborderColorHover(LONG newVal)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());

    // TODO: 在此添加属性处理程序代码
    if (m_clrBorderColorHover == newVal)
        return;

    m_clrBorderColorHover = newVal;
    InvalidateRect();
}

LONG CSkinItemCombo::GetborderColorFocus(void)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());

    // TODO: 在此添加调度处理程序代码
    return m_clrBorderColorFocus;
}

void CSkinItemCombo::SetborderColorFocus(LONG newVal)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());

    // TODO: 在此添加属性处理程序代码
    if (m_clrBorderColorFocus == newVal)
        return;

    m_clrBorderColorFocus = newVal;
    InvalidateRect();
}

LONG CSkinItemCombo::GetborderColorDisabled(void)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());

    // TODO: 在此添加调度处理程序代码
    return m_clrBorderColorDisabled;
}

void CSkinItemCombo::SetborderColorDisabled(LONG newVal)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());

    // TODO: 在此添加属性处理程序代码
    if (m_clrBorderColorDisabled == newVal)
        return;

    m_clrBorderColorDisabled = newVal;
    InvalidateRect();
}

LONG CSkinItemCombo::GetouterBorderColorNormal(void)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());

    // TODO: 在此添加调度处理程序代码
    return m_clrOuterBorderColorNormal;
}

void CSkinItemCombo::SetouterBorderColorNormal(LONG newVal)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());

    // TODO: 在此添加属性处理程序代码
    if (m_clrOuterBorderColorNormal == newVal)
        return;

    m_clrOuterBorderColorNormal = newVal;
    InvalidateRect();
}

LONG CSkinItemCombo::GetouterBorderColorHover(void)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());

    // TODO: 在此添加调度处理程序代码
    return m_clrOuterBorderColorHover;
}

void CSkinItemCombo::SetouterBorderColorHover(LONG newVal)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());

    // TODO: 在此添加属性处理程序代码
    if (m_clrOuterBorderColorHover == newVal)
        return;

    m_clrOuterBorderColorHover = newVal;
    InvalidateRect();
}

LONG CSkinItemCombo::GetouterBorderColorFocus(void)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());

    // TODO: 在此添加调度处理程序代码
    return m_clrOuterBorderColorFocus;
}

void CSkinItemCombo::SetouterBorderColorFocus(LONG newVal)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());

    // TODO: 在此添加属性处理程序代码
    if (m_clrOuterBorderColorFocus == newVal)
        return;

    m_clrOuterBorderColorFocus = newVal;
    InvalidateRect();
}

LONG CSkinItemCombo::GetouterBorderColorDisabled(void)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());

    // TODO: 在此添加调度处理程序代码
    return m_clrOuterBorderColorDisabled;
}

void CSkinItemCombo::SetouterBorderColorDisabled(LONG newVal)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());

    // TODO: 在此添加属性处理程序代码
    if (m_clrOuterBorderColorDisabled == newVal)
        return;

    m_clrOuterBorderColorDisabled = newVal;
    InvalidateRect();
}

LONG CSkinItemCombo::GetinnerBorderColorNormal(void)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());

    // TODO: 在此添加调度处理程序代码
    return m_clrInnerBorderColorNormal;
}

void CSkinItemCombo::SetinnerBorderColorNormal(LONG newVal)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());

    // TODO: 在此添加属性处理程序代码
    if (m_clrInnerBorderColorNormal == newVal)
        return;

    m_clrInnerBorderColorNormal = newVal;
    InvalidateRect();
}

LONG CSkinItemCombo::GetinnerBorderColorHover(void)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());

    // TODO: 在此添加调度处理程序代码
    return m_clrInnerBorderColorHover;
}

void CSkinItemCombo::SetinnerBorderColorHover(LONG newVal)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());

    // TODO: 在此添加属性处理程序代码
    if (m_clrInnerBorderColorHover == newVal)
        return;

    m_clrInnerBorderColorHover = newVal;
    InvalidateRect();
}

LONG CSkinItemCombo::GetinnerBorderColorFocus(void)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());

    // TODO: 在此添加调度处理程序代码
    return m_clrInnerBorderColorFocus;
}

void CSkinItemCombo::SetinnerBorderColorFocus(LONG newVal)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());

    // TODO: 在此添加属性处理程序代码
    if (m_clrInnerBorderColorFocus == newVal)
        return;

    m_clrInnerBorderColorFocus = newVal;
    InvalidateRect();
}

LONG CSkinItemCombo::GetinnerBorderColorDisabled(void)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());

    // TODO: 在此添加调度处理程序代码
    return m_clrInnerBorderColorDisabled;
}

void CSkinItemCombo::SetinnerBorderColorDisabled(LONG newVal)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());

    // TODO: 在此添加属性处理程序代码
    if (m_clrInnerBorderColorDisabled == newVal)
        return;

    m_clrInnerBorderColorDisabled = newVal;
    InvalidateRect();
}

BSTR CSkinItemCombo::GetbuttonImageNormal()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: 在此添加调度处理程序代码
	return m_strButtonImageNormal.AllocSysString();
}

void CSkinItemCombo::SetbuttonImageNormal(LPCTSTR newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: 在此添加属性处理程序代码
    SetImageHelper(m_pButtonImageNormal, m_strButtonImageNormal, newVal);
}

BSTR CSkinItemCombo::GetbuttonImageHover()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: 在此添加调度处理程序代码
	return m_strButtonImageHover.AllocSysString();
}

void CSkinItemCombo::SetbuttonImageHover(LPCTSTR newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: 在此添加属性处理程序代码
    SetImageHelper(m_pButtonImageHover, m_strButtonImageHover, newVal);
}

BSTR CSkinItemCombo::GetbuttonImagePressed()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: 在此添加调度处理程序代码
	return m_strButtonImagePressed.AllocSysString();
}

void CSkinItemCombo::SetbuttonImagePressed(LPCTSTR newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: 在此添加属性处理程序代码
    SetImageHelper(m_pButtonImagePressed, m_strButtonImagePressed, newVal);
}

BSTR CSkinItemCombo::GetbuttonImageDisabled()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: 在此添加调度处理程序代码
	return m_strButtonImageDisabled.AllocSysString();
}

void CSkinItemCombo::SetbuttonImageDisabled(LPCTSTR newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: 在此添加属性处理程序代码
    SetImageHelper(m_pButtonImageDisabled, m_strButtonImageDisabled, newVal);
}

BSTR CSkinItemCombo::GetbuttonIconNormal()
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());

    // TODO: 在此添加调度处理程序代码
    return m_strButtonIconNormal.AllocSysString();
}

void CSkinItemCombo::SetbuttonIconNormal(LPCTSTR newVal)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());

    // TODO: 在此添加属性处理程序代码
    SetImageHelper(m_pButtonIconNormal, m_strButtonIconNormal, newVal);
}

BSTR CSkinItemCombo::GetbuttonIconHover()
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());

    // TODO: 在此添加调度处理程序代码
    return m_strButtonIconHover.AllocSysString();
}

void CSkinItemCombo::SetbuttonIconHover(LPCTSTR newVal)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());

    // TODO: 在此添加属性处理程序代码
    SetImageHelper(m_pButtonIconHover, m_strButtonIconHover, newVal);
}

BSTR CSkinItemCombo::GetbuttonIconPressed()
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());

    // TODO: 在此添加调度处理程序代码
    return m_strButtonIconPressed.AllocSysString();
}

void CSkinItemCombo::SetbuttonIconPressed(LPCTSTR newVal)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());

    // TODO: 在此添加属性处理程序代码
    SetImageHelper(m_pButtonIconPressed, m_strButtonIconPressed, newVal);
}

BSTR CSkinItemCombo::GetbuttonIconDisabled()
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());

    // TODO: 在此添加调度处理程序代码
    return m_strButtonIconDisabled.AllocSysString();
}

void CSkinItemCombo::SetbuttonIconDisabled(LPCTSTR newVal)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());

    // TODO: 在此添加属性处理程序代码
    SetImageHelper(m_pButtonIconDisabled, m_strButtonIconDisabled, newVal);
}

VARIANT_BOOL CSkinItemCombo::GetfocusBorder()
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());

    // TODO: 在此添加调度处理程序代码
    return m_bFocusBorder ? (VARIANT_TRUE) : (VARIANT_FALSE);
}

void CSkinItemCombo::SetfocusBorder(VARIANT_BOOL newVal)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());

    // TODO: 在此添加属性处理程序代码
    newVal = (newVal != VARIANT_FALSE);
    if (m_bFocusBorder == newVal)
        return;

    m_bFocusBorder = newVal;
    InvalidateRect();
}

LONG CSkinItemCombo::GetBorderHelper(LONG clrNormal, LONG clrHover, LONG clrFocus, LONG clrDisabled)
{
    LONG color = 0;

    if (IsFocused() && m_bFocusBorder) {
        color = clrFocus;
    } else if (IsHot()) {
        color = clrHover;
    } else {
        color = clrNormal;
    }

    if (GetDisabled())
        color = clrDisabled;

    if (color == -1)
        color = clrNormal;

    if (color == -1)
        return -1;

    if (GetColorTransform()) {
        GetEngine()->TransformColor(color, color);
    }

    return color;
}

CImageBase* CSkinItemCombo::GetButtonImageHelper(UINT uState, CImageBase* pNormal, CImageBase* pHover, CImageBase* pPressed, CImageBase* pDisabled)
{
    CImageBase* pImage = NULL;

    if (uState == STATE_SYSTEM_PRESSED) {
        pImage = pPressed;
    } else if (IsHot()) {
        pImage = pHover;
    } else if (IsFocused()) {
        pImage = pHover;
    } else if (uState == 0) {
        pImage = pNormal;
    }

    if (GetDisabled()) {
        pImage = pDisabled;
    }

    if (pImage == NULL) {
        pImage = pNormal;
    }

    return pImage;
}

BOOL CSkinItemCombo::GetComboBoxInfoHelper(HWND hComboWnd, COMBOBOXINFO& info)
{
	if (! ::IsWindow(hComboWnd))
		return FALSE;

	info.cbSize = sizeof(COMBOBOXINFO);
    return ::GetComboBoxInfo(hComboWnd, &info);
	//return (BOOL)::SendMessage(hComboWnd, CB_GETCOMBOBOXINFO, 0, (LPARAM)&info);
}

BOOL CSkinItemCombo::GetComboEditHwnd(HWND& hEdit)
{
	HWND hNext = ::GetWindow(m_hWnd, GW_CHILD);
	while (hNext != NULL) {
		TCHAR className[256];
		::GetClassName(hNext, className, 256);
		if ( _tcscmp(_T("Edit"), className) == 0 ) {
			hEdit = hNext;
			return TRUE;
		}
		hNext = ::GetWindow(hNext, GW_HWNDNEXT);
	}

	hEdit = NULL;
	return TRUE;
}

BOOL CSkinItemCombo::GetComboListHwnd(HWND& hList)
{
	COMBOBOXINFO comboInfo;
	if (! GetComboBoxInfoHelper(m_hWnd, comboInfo))
		return FALSE;

	hList = comboInfo.hwndList;
	return TRUE;
}

BOOL CSkinItemCombo::GetComboRect(RECT& rect)
{
	if (! ::IsWindow(m_hWnd))
		return FALSE;
	
	::GetClientRect(m_hWnd, &rect);
	return TRUE;
}

BOOL CSkinItemCombo::GetEditRect(RECT& rect)
{
	if (! ::IsWindow(m_hEdit))
		return FALSE;

	if (m_hWnd != ::GetParent(m_hEdit))
		return FALSE;

	CRect rcEdit;
	::GetWindowRect(m_hEdit, &rcEdit);

	::ScreenToClient(m_hWnd, &(rcEdit.TopLeft()));
	::ScreenToClient(m_hWnd, &(rcEdit.BottomRight()));

	rect = rcEdit;
	return TRUE;
}

BOOL CSkinItemCombo::GetButtonRect(RECT& rect)
{
	//COMBOBOXINFO comboInfo;
	//if (! GetComboBoxInfoHelper(m_hWnd, comboInfo))
	//	return FALSE;

	//rect = comboInfo.rcButton;
	//return TRUE;

	CRect rcCombo;
	::GetClientRect(m_hWnd, &rcCombo);

	rcCombo.left = rcCombo.right - ::GetSystemMetrics(SM_CXHTHUMB);

	rcCombo.DeflateRect(-1, 1);
	rcCombo.OffsetRect(-2, 0);
	rcCombo.left += 1;

	rect = rcCombo;

	return TRUE;
}

BOOL CSkinItemCombo::GetButtonState(DWORD& state)
{
	COMBOBOXINFO comboInfo;
	if (! GetComboBoxInfoHelper(m_hWnd, comboInfo))
		return FALSE;

	state = comboInfo.stateButton;
	return TRUE;
}

LONG CSkinItemCombo::GetCurSel()
{
    if (!IsWindow(m_hWnd))
        return -1;

    return (LONG)::SendMessage(m_hWnd, CB_GETCURSEL, 0, 0);
}

void CSkinItemCombo::SetCurSel(LONG iSel)
{
    if (!IsWindow(m_hWnd))
        return;

    ::SendMessage(m_hWnd, CB_SETCURSEL, iSel, 0);
}

BOOL CSkinItemCombo::UpdateSkinItemHwnd()
{
	if (! CSkinItem::UpdateSkinItemHwnd())
		return FALSE;

	m_hEdit = NULL;
    if (GetComboEditHwnd(m_hEdit) && ::IsWindow(m_hEdit) && m_hEdit != m_hWnd) {
	    CSkinItemEdit* pEditItem = (CSkinItemEdit*)GetEngine()->CreateSkinByClassName(_T("CSkinItemEdit"));
	    pEditItem->Setname(_T("EditItem"));
	    pEditItem->SethasBorder(VARIANT_FALSE);
        pEditItem->SetuseOriginPosition(VARIANT_TRUE);
        pEditItem->Attach(m_hEdit);
	    //pEditItem->Sethwnd((LONG)m_hEdit);
	    AddSubSkin(pEditItem);
    }

	m_hList = NULL;
    if (GetComboListHwnd(m_hList) && ::IsWindow(m_hList) && m_hList != m_hWnd) {
	    CSkinItem* pListItem = (CSkinItem*)GetEngine()->CreateSkinByClassName(_T("CSkinItem"));
	    pListItem->Setname(_T("ListItem"));
        pListItem->SetuseOriginPosition(VARIANT_TRUE);
        pListItem->Attach(m_hList);
	    //pListItem->Sethwnd((LONG)m_hList);
	    AddSubSkin(pListItem);
    }

	return TRUE;
}

BOOL CSkinItemCombo::UpdateSkinItemProperty()
{
	if (! CSkinItem::UpdateSkinItemProperty())
		return FALSE;

    CSkinItem* pEditItem = (CSkinItem*)GetSubSkin(_T("EditItem"));
    if (pEditItem != NULL) {
        pEditItem->UpdateSkinItemProperty();
    }

    CSkinItem* pListItem = (CSkinItem*)GetSubSkin(_T("ListItem"));
    if (pEditItem != NULL) {
        pEditItem->UpdateSkinItemProperty();
    }

	return TRUE;
}

BOOL CSkinItemCombo::IsFocused()
{
    if (CSkinItem::IsFocused())
        return TRUE;

    if (m_hEdit == NULL)
        return FALSE;

    return ::GetFocus() == m_hEdit;
}

BOOL CSkinItemCombo::IsHot()
{
    if (m_hEdit != NULL) {
        CSkinItem* pItem = CSkinItem::FromHandlePermanent(m_hEdit);
        if (pItem != NULL && pItem->IsHot())
            return TRUE;
    }

    return CSkinItem::IsHot();
}

void CSkinItemCombo::DrawBackground(CDC* pDC, LPRECT lpInvalidRect, CRgn* pInvalidRgn, BOOL bChildSkin)
{

}

void CSkinItemCombo::DrawForeground(CDC* pDC, LPRECT lpInvalidRect, CRgn* pInvalidRgn)
{
	CRect buttonRect;
	GetButtonRect(buttonRect);
	DWORD buttonState = 0;
	GetButtonState(buttonState);

	if (buttonState == STATE_SYSTEM_INVISIBLE)
		return;

	//填充背景色
	LONG bkColor = GetBorderHelper(m_clrBkColorNormal,
        m_clrBkColorHover,
        m_clrBkColorFocus,
        m_clrBkColorDisabled);
	if (bkColor != -1) {
		CRenderEngine::FillRect(*pDC, buttonRect, bkColor);
	}

    //绘制按钮边框
	if (m_bButtonBorder) {
		LONG color = GetBorderHelper(m_clrBorderColorNormal,
            m_clrBorderColorHover,
            m_clrBorderColorFocus,
            m_clrBorderColorDisabled);

        if (!m_bHasBorder || !m_bInnerBorder) {
            color = GetBorderHelper(m_clrBkColorNormal,
                m_clrBkColorHover,
                m_clrBkColorFocus,
                m_clrBkColorDisabled);
        }
		CRect borderRect = buttonRect;
		borderRect.DeflateRect(m_nButtonBorderMargin, m_nButtonBorderMargin);
		CRenderEngine::RenderRect(*pDC, borderRect, color);
	}

	//绘制按钮背景图片
	CImageBase* pImage = GetButtonImageHelper(buttonState,
        m_pButtonImageNormal, 
        m_pButtonImageHover, 
        m_pButtonImagePressed, 
        m_pButtonImageDisabled);
    CRect buttonDrawRect = buttonRect;
	if (pImage != NULL) {
        if (! m_bStretchButton) {
            buttonDrawRect = CRect(0, 0, pImage->Width(), pImage->Height());
            buttonDrawRect.OffsetRect(buttonRect.left, buttonRect.top);
            buttonDrawRect.OffsetRect((buttonRect.Width() - buttonDrawRect.Width()) / 2, 
                (buttonRect.Height() - buttonDrawRect.Height()) / 2);
        }
        DrawImageAlphaHelper(pImage, *pDC, &buttonDrawRect, NULL, m_bAlpha);
	}

    //绘制按钮图标
    CImageBase* pIcon = GetButtonImageHelper(buttonState,
        m_pButtonIconNormal, 
        m_pButtonIconHover, 
        m_pButtonIconPressed, 
        m_pButtonIconDisabled);
    if (pIcon != NULL) {
        CRect iconRect = CRect(0, 0, pIcon->Width(), pIcon->Height());
        iconRect.OffsetRect(buttonDrawRect.left, buttonDrawRect.top);
        iconRect.OffsetRect((buttonDrawRect.Width() - iconRect.Width()) / 2, 
            (buttonDrawRect.Height() - iconRect.Height()) / 2);
        iconRect.OffsetRect(m_ptButtonIconOffset);
        DrawImageAlphaHelper(pIcon, *pDC, &iconRect, NULL, m_bAlpha);
    }
}

void CSkinItemCombo::DrawFrame(CDC* pDC, LPRECT lpInvalidRect, CRgn* pInvalidRgn)
{
    if (! m_bHasBorder)
        return;

    CRect rcCombo;
	GetComboRect(rcCombo);

	CRect rcFrame = rcCombo;

    //外
    rcFrame.InflateRect(1, 1);
    if (m_bThickBorder && m_bOuterBorder) {
        LONG clrOuter = GetBorderHelper(
            m_clrOuterBorderColorNormal, 
            m_clrOuterBorderColorHover, 
            m_clrOuterBorderColorFocus, 
            m_clrOuterBorderColorDisabled);
        if (clrOuter != -1)
            CRenderEngine::Render3DRect(*pDC, rcFrame, clrOuter, clrOuter);
    }

    //中
    rcFrame.DeflateRect(1, 1);
    {
        LONG clrBorder = GetBorderHelper(
            m_clrBorderColorNormal, 
            m_clrBorderColorHover, 
            m_clrBorderColorFocus, 
            m_clrBorderColorDisabled);
        if (clrBorder != -1)
            CRenderEngine::Render3DRect(*pDC, rcFrame, clrBorder, clrBorder);
    }

    //内
    rcFrame.DeflateRect(1, 1);
    if (m_bThickBorder && m_bInnerBorder) {
        LONG clrInnerBorder = GetBorderHelper(
            m_clrInnerBorderColorNormal, 
            m_clrInnerBorderColorHover, 
            m_clrInnerBorderColorFocus, 
            m_clrInnerBorderColorDisabled);
        if (clrInnerBorder != -1)
            CRenderEngine::Render3DRect(*pDC, rcFrame, clrInnerBorder, clrInnerBorder);
    }
}

void CSkinItemCombo::Draw(CDC* pDC, LPRECT lpInvalidRect, CRgn* pInvalidRgn)
{
	DrawForeground(pDC);
    DrawFrame(pDC);
}

BOOL CSkinItemCombo::OnNcPaint(UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* lpResult)
{
	*lpResult = 0;

	CWnd* pWnd = CWnd::FromHandle(m_hWnd);

	CWindowDC dc(pWnd);

	DrawFrame(&dc);

	return TRUE;
}

BOOL CSkinItemCombo::OnPaint(UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* lpResult)
{
	*lpResult = CSkinItem::DefWndProc(uMsg, wParam, lParam);

	CWnd* pWnd = CWnd::FromHandle(m_hWnd);

	CWindowDC dc(pWnd);

	Draw(&dc);

	return TRUE;
}

BOOL CSkinItemCombo::OnCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* lpResult)
{
	if (HIWORD(wParam) == EN_SETFOCUS) {
		InvalidateRect();
	}
	if (HIWORD(wParam) == EN_KILLFOCUS) {
		InvalidateRect();
	}
    if (HIWORD(wParam) == WM_MOUSEENTER) {
        InvalidateRect();
        ::PostMessage(m_hWnd, WM_NCPAINT, 0, 0);
    }
    if (HIWORD(wParam) == WM_MOUSELEAVE) {
        InvalidateRect();
        ::PostMessage(m_hWnd, WM_NCPAINT, 0, 0);
    }

	return CSkinItem::OnCommand(uMsg, wParam, lParam, lpResult);
}

BOOL CSkinItemCombo::OnMouseEnter(UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* lpResult)
{
    ::PostMessage(m_hWnd, WM_NCPAINT, 0, 0);

    return CSkinItem::OnMouseEnter(uMsg, wParam, lParam, lpResult);
}

BOOL CSkinItemCombo::OnMouseLeave(UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* lpResult)
{
    ::PostMessage(m_hWnd, WM_NCPAINT, 0, 0);

    return CSkinItem::OnMouseLeave(uMsg, wParam, lParam, lpResult);
}
