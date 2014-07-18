// SkinItemEdit.cpp : 实现文件
//

#include "stdafx.h"
#include "SkinItem.h"
#include "SkinEngine.h"
#include "SkinItemEdit.h"
#include "RenderEngine.h"
#include "ImageBase.h"


// CSkinItemEdit

IMPLEMENT_DYNCREATE(CSkinItemEdit, CSkinItem)
CSkinItemEdit::CSkinItemEdit()
{
	EnableAutomation();
	
	// 为了使应用程序在 OLE 自动化对象处于活动状态时保持
	//	运行，构造函数调用 AfxOleLockApp。
	m_bSystemEdit = TRUE;

	m_bHasBorder = TRUE;
	m_bThickBorder = FALSE;
    m_bOuterBorder = FALSE;
    m_bInnerBorder = FALSE;

	m_clrBorderColorNormal = RGB(165, 189, 223);//RGB(128, 128, 128);//RGB(143, 172, 219);
	m_clrBorderColorHover = RGB(165, 189, 223);//m_clrBorderColorNormal;
	m_clrBorderColorFocus = RGB(165, 189, 223);//m_clrBorderColorNormal;//RGB(0x61, 0x8E, 0xDA);
	m_clrBorderColorReadOnly = RGB(165, 189, 223);//m_clrBorderColorNormal;//RGB(0x61, 0x8E, 0xDA);
    m_clrBorderColorDisabled = RGB(128, 128, 128);
    m_clrOuterBorderColorNormal = m_clrBorderColorNormal;
    m_clrOuterBorderColorHover = m_clrBorderColorHover;
    m_clrOuterBorderColorFocus = m_clrBorderColorFocus;
    m_clrOuterBorderColorReadOnly = m_clrBorderColorReadOnly;
    m_clrOuterBorderColorDisabled = m_clrBorderColorDisabled;
    m_clrInnerBorderColorNormal = m_clrBorderColorNormal;
    m_clrInnerBorderColorHover = m_clrBorderColorHover;
    m_clrInnerBorderColorFocus = m_clrBorderColorFocus;
    m_clrInnerBorderColorReadOnly = m_clrBorderColorReadOnly;
    m_clrInnerBorderColorDisabled = m_clrBorderColorDisabled;

    m_clrBkColorReadOnly = RGB(233, 233, 233);
    m_clrTextColorReadOnly = -1;

    m_bFocusBorder = TRUE;
    m_bReadOnlyBorder = TRUE;

	AfxOleLockApp();
}

CSkinItemEdit::~CSkinItemEdit()
{
	// 为了在用 OLE 自动化创建所有对象后终止应用程序，
	// 	析构函数调用 AfxOleUnlockApp。
	
	AfxOleUnlockApp();
}


void CSkinItemEdit::OnFinalRelease()
{
	// 释放了对自动化对象的最后一个引用后，将调用
	// OnFinalRelease。基类将自动
	// 删除该对象。在调用该基类之前，请添加您的
	// 对象所需的附加清除代码。

	CCmdTarget::OnFinalRelease();
}


BEGIN_MESSAGE_MAP(CSkinItemEdit, CSkinItem)
END_MESSAGE_MAP()


BEGIN_SKIN_DISPATCH_MAP(CSkinItemEdit, CSkinItem)
	SKINBASE_DISP_PROPERTIES(CSkinItemEdit)
	SKINITEM_DISP_PROPERTIES(CSkinItemEdit)

	SKIN_DISP_PROPERTY(CSkinItemEdit, systemEdit, VT_BOOL)
	SKIN_DISP_PROPERTY(CSkinItemEdit, hasBorder, VT_BOOL)
	SKIN_DISP_PROPERTY(CSkinItemEdit, thickBorder, VT_BOOL)
    SKIN_DISP_PROPERTY(CSkinItemEdit, outerBorder, VT_BOOL)
    SKIN_DISP_PROPERTY(CSkinItemEdit, innerBorder, VT_BOOL)
	SKIN_DISP_PROPERTY(CSkinItemEdit, borderColorNormal, VT_I4)
	SKIN_DISP_PROPERTY(CSkinItemEdit, borderColorHover, VT_I4)
	SKIN_DISP_PROPERTY(CSkinItemEdit, borderColorFocus, VT_I4)
    SKIN_DISP_PROPERTY(CSkinItemEdit, borderColorReadOnly, VT_I4)
    SKIN_DISP_PROPERTY(CSkinItemEdit, borderColorDisabled, VT_I4)
    SKIN_DISP_PROPERTY(CSkinItemEdit, outerBorderColorNormal, VT_I4)
    SKIN_DISP_PROPERTY(CSkinItemEdit, outerBorderColorHover, VT_I4)
    SKIN_DISP_PROPERTY(CSkinItemEdit, outerBorderColorFocus, VT_I4)
    SKIN_DISP_PROPERTY(CSkinItemEdit, outerBorderColorReadOnly, VT_I4)
    SKIN_DISP_PROPERTY(CSkinItemEdit, outerBorderColorDisabled, VT_I4)
    SKIN_DISP_PROPERTY(CSkinItemEdit, innerBorderColorNormal, VT_I4)
    SKIN_DISP_PROPERTY(CSkinItemEdit, innerBorderColorHover, VT_I4)
    SKIN_DISP_PROPERTY(CSkinItemEdit, innerBorderColorFocus, VT_I4)
    SKIN_DISP_PROPERTY(CSkinItemEdit, innerBorderColorReadOnly, VT_I4)
    SKIN_DISP_PROPERTY(CSkinItemEdit, innerBorderColorDisabled, VT_I4)

    SKIN_DISP_PROPERTY(CSkinItemEdit, bkColorReadOnly, VT_I4)
    SKIN_DISP_PROPERTY(CSkinItemEdit, textColorReadOnly, VT_I4)
    SKIN_DISP_PROPERTY(CSkinItemEdit, focusBorder, VT_BOOL)
    SKIN_DISP_PROPERTY(CSkinItemEdit, readOnlyBorder, VT_BOOL)
END_SKIN_DISPATCH_MAP(CSkinItemEdit)

// 注意: 我们添加 IID_ISkinItemEdit 支持
//以支持来自 VBA 的类型安全绑定。此 IID 必须同附加到 .IDL 文件中的
//调度接口的 GUID 匹配。

// {B7AFB598-5718-40FE-BC7A-7EB3EBD7CC79}
static const IID IID_ISkinItemEdit =
{ 0xB7AFB598, 0x5718, 0x40FE, { 0xBC, 0x7A, 0x7E, 0xB3, 0xEB, 0xD7, 0xCC, 0x79 } };

BEGIN_INTERFACE_MAP(CSkinItemEdit, CSkinItem)
	INTERFACE_PART(CSkinItemEdit, IID_ISkinItemEdit, Dispatch)
END_INTERFACE_MAP()

// {ABAD6771-04A1-481F-A04F-03515C1900E2}
IMPLEMENT_OLECREATE_FLAGS(CSkinItemEdit, "Skin.SkinItemEdit", afxRegApartmentThreading, 0xabad6771, 0x4a1, 0x481f, 0xa0, 0x4f, 0x3, 0x51, 0x5c, 0x19, 0x0, 0xe2)


// CSkinItemEdit 消息处理程序
VARIANT_BOOL CSkinItemEdit::GetsystemEdit()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: 在此添加调度处理程序代码
	return m_bSystemEdit ? (VARIANT_TRUE) : (VARIANT_FALSE);
}

void CSkinItemEdit::SetsystemEdit(VARIANT_BOOL newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: 在此添加属性处理程序代码
	newVal = (newVal != VARIANT_FALSE);
	if (m_bSystemEdit == newVal)
		return;

	m_bSystemEdit = newVal;
	InvalidateRect();
}

VARIANT_BOOL CSkinItemEdit::GethasBorder()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

    // TODO: 在此添加调度处理程序代码
    BOOL bHasBorder = FALSE;
    if (CSkinItem::GetSkinItemLongHelper(GWL_STYLE, WS_BORDER, m_bHasBorder))
        return bHasBorder ? (VARIANT_TRUE) : (VARIANT_FALSE);

	return m_bHasBorder ? (VARIANT_TRUE) : (VARIANT_FALSE);
}

void CSkinItemEdit::SethasBorder(VARIANT_BOOL newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: 在此添加属性处理程序代码
	newVal = (newVal != VARIANT_FALSE);
	if (m_bHasBorder == newVal)
		return;

	m_bHasBorder = newVal;
    CSkinItem::SetSkinItemLongHelper(GWL_STYLE, WS_BORDER, m_bHasBorder);
	InvalidateRect();
}

VARIANT_BOOL CSkinItemEdit::GetthickBorder()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: 在此添加调度处理程序代码
	return m_bThickBorder ? (VARIANT_TRUE) : (VARIANT_FALSE);
}

void CSkinItemEdit::SetthickBorder(VARIANT_BOOL newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: 在此添加属性处理程序代码
	newVal = (newVal != VARIANT_FALSE);
	if (m_bThickBorder == newVal)
		return;

	m_bThickBorder = newVal;
	InvalidateRect();
}

VARIANT_BOOL CSkinItemEdit::GetouterBorder()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: 在此添加调度处理程序代码
	return m_bOuterBorder ? (VARIANT_TRUE) : (VARIANT_FALSE);
}

void CSkinItemEdit::SetouterBorder(VARIANT_BOOL newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: 在此添加属性处理程序代码
	newVal = (newVal != VARIANT_FALSE);
	if (m_bOuterBorder == newVal)
		return;

	m_bOuterBorder = newVal;
	InvalidateRect();
}

VARIANT_BOOL CSkinItemEdit::GetinnerBorder()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: 在此添加调度处理程序代码
	return m_bInnerBorder ? (VARIANT_TRUE) : (VARIANT_FALSE);
}

void CSkinItemEdit::SetinnerBorder(VARIANT_BOOL newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: 在此添加属性处理程序代码
	newVal = (newVal != VARIANT_FALSE);
	if (m_bInnerBorder == newVal)
		return;

	m_bInnerBorder = newVal;
	InvalidateRect();
}

LONG CSkinItemEdit::GetborderColorNormal(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: 在此添加调度处理程序代码
	return m_clrBorderColorNormal;
}

void CSkinItemEdit::SetborderColorNormal(LONG newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: 在此添加属性处理程序代码
	if (m_clrBorderColorNormal == newVal)
		return;

	m_clrBorderColorNormal = newVal;
	InvalidateRect();
}

LONG CSkinItemEdit::GetborderColorHover(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: 在此添加调度处理程序代码
	return m_clrBorderColorHover;
}

void CSkinItemEdit::SetborderColorHover(LONG newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: 在此添加属性处理程序代码
	if (m_clrBorderColorHover == newVal)
		return;

	m_clrBorderColorHover = newVal;
	InvalidateRect();
}

LONG CSkinItemEdit::GetborderColorFocus(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: 在此添加调度处理程序代码
	return m_clrBorderColorFocus;
}

void CSkinItemEdit::SetborderColorFocus(LONG newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: 在此添加属性处理程序代码
	if (m_clrBorderColorFocus == newVal)
		return;

	m_clrBorderColorFocus = newVal;
	InvalidateRect();
}

LONG CSkinItemEdit::GetborderColorReadOnly(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: 在此添加调度处理程序代码
	return m_clrBorderColorReadOnly;
}

void CSkinItemEdit::SetborderColorReadOnly(LONG newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: 在此添加属性处理程序代码
	if (m_clrBorderColorReadOnly == newVal)
		return;

	m_clrBorderColorReadOnly = newVal;
	InvalidateRect();
}

LONG CSkinItemEdit::GetborderColorDisabled(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: 在此添加调度处理程序代码
	return m_clrBorderColorDisabled;
}

void CSkinItemEdit::SetborderColorDisabled(LONG newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: 在此添加属性处理程序代码
	if (m_clrBorderColorDisabled == newVal)
		return;

	m_clrBorderColorDisabled = newVal;
	InvalidateRect();
}

LONG CSkinItemEdit::GetouterBorderColorNormal(void)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());

    // TODO: 在此添加调度处理程序代码
    return m_clrOuterBorderColorNormal;
}

void CSkinItemEdit::SetouterBorderColorNormal(LONG newVal)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());

    // TODO: 在此添加属性处理程序代码
    if (m_clrOuterBorderColorNormal == newVal)
        return;

    m_clrOuterBorderColorNormal = newVal;
    InvalidateRect();
}

LONG CSkinItemEdit::GetouterBorderColorHover(void)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());

    // TODO: 在此添加调度处理程序代码
    return m_clrOuterBorderColorHover;
}

void CSkinItemEdit::SetouterBorderColorHover(LONG newVal)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());

    // TODO: 在此添加属性处理程序代码
    if (m_clrOuterBorderColorHover == newVal)
        return;

    m_clrOuterBorderColorHover = newVal;
    InvalidateRect();
}

LONG CSkinItemEdit::GetouterBorderColorFocus(void)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());

    // TODO: 在此添加调度处理程序代码
    return m_clrOuterBorderColorFocus;
}

void CSkinItemEdit::SetouterBorderColorFocus(LONG newVal)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());

    // TODO: 在此添加属性处理程序代码
    if (m_clrOuterBorderColorFocus == newVal)
        return;

    m_clrOuterBorderColorFocus = newVal;
    InvalidateRect();
}

LONG CSkinItemEdit::GetouterBorderColorReadOnly(void)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());

    // TODO: 在此添加调度处理程序代码
    return m_clrOuterBorderColorReadOnly;
}

void CSkinItemEdit::SetouterBorderColorReadOnly(LONG newVal)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());

    // TODO: 在此添加属性处理程序代码
    if (m_clrOuterBorderColorReadOnly == newVal)
        return;

    m_clrOuterBorderColorReadOnly = newVal;
    InvalidateRect();
}

LONG CSkinItemEdit::GetouterBorderColorDisabled(void)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());

    // TODO: 在此添加调度处理程序代码
    return m_clrOuterBorderColorDisabled;
}

void CSkinItemEdit::SetouterBorderColorDisabled(LONG newVal)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());

    // TODO: 在此添加属性处理程序代码
    if (m_clrOuterBorderColorDisabled == newVal)
        return;

    m_clrOuterBorderColorDisabled = newVal;
    InvalidateRect();
}

LONG CSkinItemEdit::GetinnerBorderColorNormal(void)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());

    // TODO: 在此添加调度处理程序代码
    return m_clrInnerBorderColorNormal;
}

void CSkinItemEdit::SetinnerBorderColorNormal(LONG newVal)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());

    // TODO: 在此添加属性处理程序代码
    if (m_clrInnerBorderColorNormal == newVal)
        return;

    m_clrInnerBorderColorNormal = newVal;
    InvalidateRect();
}

LONG CSkinItemEdit::GetinnerBorderColorHover(void)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());

    // TODO: 在此添加调度处理程序代码
    return m_clrInnerBorderColorHover;
}

void CSkinItemEdit::SetinnerBorderColorHover(LONG newVal)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());

    // TODO: 在此添加属性处理程序代码
    if (m_clrInnerBorderColorHover == newVal)
        return;

    m_clrInnerBorderColorHover = newVal;
    InvalidateRect();
}

LONG CSkinItemEdit::GetinnerBorderColorFocus(void)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());

    // TODO: 在此添加调度处理程序代码
    return m_clrInnerBorderColorFocus;
}

void CSkinItemEdit::SetinnerBorderColorFocus(LONG newVal)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());

    // TODO: 在此添加属性处理程序代码
    if (m_clrInnerBorderColorFocus == newVal)
        return;

    m_clrInnerBorderColorFocus = newVal;
    InvalidateRect();
}

LONG CSkinItemEdit::GetinnerBorderColorReadOnly(void)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());

    // TODO: 在此添加调度处理程序代码
    return m_clrInnerBorderColorReadOnly;
}

void CSkinItemEdit::SetinnerBorderColorReadOnly(LONG newVal)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());

    // TODO: 在此添加属性处理程序代码
    if (m_clrInnerBorderColorReadOnly == newVal)
        return;

    m_clrInnerBorderColorReadOnly = newVal;
    InvalidateRect();
}

LONG CSkinItemEdit::GetinnerBorderColorDisabled(void)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());

    // TODO: 在此添加调度处理程序代码
    return m_clrInnerBorderColorDisabled;
}

void CSkinItemEdit::SetinnerBorderColorDisabled(LONG newVal)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());

    // TODO: 在此添加属性处理程序代码
    if (m_clrInnerBorderColorDisabled == newVal)
        return;

    m_clrInnerBorderColorDisabled = newVal;
    InvalidateRect();
}

LONG CSkinItemEdit::GetbkColorReadOnly(void)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());

    // TODO: 在此添加调度处理程序代码
    return m_clrBkColorReadOnly;
}

void CSkinItemEdit::SetbkColorReadOnly(LONG newVal)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());

    // TODO: 在此添加属性处理程序代码
    if (m_clrBkColorReadOnly == newVal)
        return;

    m_clrBkColorReadOnly = newVal;
    InvalidateRect();
}

LONG CSkinItemEdit::GettextColorReadOnly(void)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());

    // TODO: 在此添加调度处理程序代码
    return m_clrTextColorReadOnly;
}

void CSkinItemEdit::SettextColorReadOnly(LONG newVal)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());

    // TODO: 在此添加属性处理程序代码
    if (m_clrTextColorReadOnly == newVal)
        return;

    m_clrTextColorReadOnly = newVal;
    InvalidateRect();
}

VARIANT_BOOL CSkinItemEdit::GetfocusBorder()
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());

    // TODO: 在此添加调度处理程序代码
    return m_bFocusBorder ? (VARIANT_TRUE) : (VARIANT_FALSE);
}

void CSkinItemEdit::SetfocusBorder(VARIANT_BOOL newVal)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());

    // TODO: 在此添加属性处理程序代码
    newVal = (newVal != VARIANT_FALSE);
    if (m_bFocusBorder == newVal)
        return;

    m_bFocusBorder = newVal;
    InvalidateRect();
}

VARIANT_BOOL CSkinItemEdit::GetreadOnlyBorder()
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());

    // TODO: 在此添加调度处理程序代码
    return m_bReadOnlyBorder ? (VARIANT_TRUE) : (VARIANT_FALSE);
}

void CSkinItemEdit::SetreadOnlyBorder(VARIANT_BOOL newVal)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());

    // TODO: 在此添加属性处理程序代码
    newVal = (newVal != VARIANT_FALSE);
    if (m_bReadOnlyBorder == newVal)
        return;

    m_bReadOnlyBorder = newVal;
    InvalidateRect();
}

BOOL CSkinItemEdit::GetEditRect(RECT& rect)
{
	ASSERT(::IsWindow(m_hWnd));
	if (!::IsWindow(m_hWnd))
		return FALSE;

	if (! m_bSystemEdit) {
		CRect rcWnd = GetSkinAbsRect();
		rect = rcWnd;
		return TRUE;
	}

	CRect rcBorder;
	::GetClientRect(m_hWnd, &rcBorder);

	const DWORD dwStyle = ::GetWindowLong(m_hWnd, GWL_STYLE);
	const DWORD dwExStyle = ::GetWindowLong(m_hWnd, GWL_EXSTYLE);

	const LONG vertiScrollWidth = (LONG)::GetSystemMetrics(SM_CXVSCROLL);
	const LONG horizScrollHeight = (LONG)::GetSystemMetrics(SM_CYHSCROLL);
	LONG edgeWidth = (LONG)::GetSystemMetrics(SM_CYEDGE);
	LONG edgeHeight = (LONG)::GetSystemMetrics(SM_CXEDGE);

    if (dwExStyle & WS_EX_CLIENTEDGE) {
        //
    } else if (dwExStyle & WS_EX_STATICEDGE) {
        edgeWidth -= 1;
        edgeHeight -= 1;
    } else {
        edgeWidth -= 2;
        edgeHeight -= 2;
    }

	if (dwStyle & WS_VSCROLL) {
		if (dwExStyle & WS_EX_LEFTSCROLLBAR) {
			rcBorder.left -= vertiScrollWidth;
		} else {
			rcBorder.right += vertiScrollWidth;
		}
	}

	if (dwStyle & WS_HSCROLL) {
		rcBorder.bottom += horizScrollHeight;
	}
	rcBorder.InflateRect(edgeWidth, edgeHeight);
	
	rect = rcBorder;

	return TRUE;
}

BOOL CSkinItemEdit::IsReadOnly()
{
    if (! IsWindow(m_hWnd))
        return FALSE;

    BOOL bTrue = FALSE;
    if (! CSkinItem::GetSkinItemLongHelper(GWL_STYLE, ES_READONLY, bTrue))
        return FALSE;

    return bTrue;
}

LONG CSkinItemEdit::ChangeSaturation(LONG color, float fRatio)
{
	LONG r = (LONG)(GetRValue(color) * fRatio);
	if (r > 255)
		r = 255;
	LONG g = (LONG)(GetGValue(color) * fRatio);
	if (g > 255)
		g = 255;
	LONG b = (LONG)(GetBValue(color) * fRatio);
	if (b > 255)
		b = 255;
	return RGB(r, g, b);
}

void CSkinItemEdit::DrawBackground(CDC* pDC, LPRECT lpInvalidRect, CRgn* pInvalidRgn, BOOL bChildSkin)
{
	
}

void CSkinItemEdit::DrawForeground(CDC* pDC, LPRECT lpInvalidRect, CRgn* pInvalidRgn)
{
	
}

LONG CSkinItemEdit::GetBorderHelper(LONG clrNormal, LONG clrHover, LONG clrFocus, LONG clrReadOnly, LONG clrDisabled)
{
    LONG color = 0;

    if (IsReadOnly() && m_bReadOnlyBorder) {
        color = clrReadOnly;
    } else if (IsFocused() && m_bFocusBorder) {
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

void CSkinItemEdit::DrawFrame(CDC* pDC, LPRECT lpInvalidRect, CRgn* pInvalidRgn)
{
	if (! m_bHasBorder)
		return;

	CRect rcEdit;
	GetEditRect(rcEdit);

	CRect rcFrame = rcEdit;

	//外
	rcFrame.InflateRect(1, 1);
	if (m_bThickBorder && m_bOuterBorder) {
        LONG clrOuter = GetBorderHelper(
            m_clrOuterBorderColorNormal, 
            m_clrOuterBorderColorHover, 
            m_clrOuterBorderColorFocus, 
            m_clrOuterBorderColorReadOnly, 
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
            m_clrBorderColorReadOnly, 
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
            m_clrInnerBorderColorReadOnly, 
            m_clrInnerBorderColorDisabled);
        if (clrInnerBorder != -1)
            CRenderEngine::Render3DRect(*pDC, rcFrame, clrInnerBorder, clrInnerBorder);
	}
}

void CSkinItemEdit::Draw(CDC* pDC, LPRECT lpInvalidRect, CRgn* pInvalidRgn)
{
	
}

BOOL CSkinItemEdit::OnNcPaint(UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* lpResult)
{
	*lpResult = 0;

	CWnd* pWnd = CWnd::FromHandle(m_hWnd);
	CDC* pDC = NULL;
	if (m_bSystemEdit)
		pDC = pWnd->GetDC();
	else 
		pDC = pWnd->GetWindowDC();

	DrawFrame(pDC);

	pWnd->ReleaseDC(pDC);

	return TRUE;
}

BOOL CSkinItemEdit::OnPaint(UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* lpResult)
{
	*lpResult = CSkinItem::DefWndProc(uMsg, wParam, lParam);

	CWnd* pWnd = CWnd::FromHandle(m_hWnd);
	CDC* pDC = NULL;
	if (m_bSystemEdit)
		pDC = pWnd->GetDC();
	else 
		pDC = pWnd->GetWindowDC();

	DrawFrame(pDC);

	pWnd->ReleaseDC(pDC);

	return TRUE;
}

BOOL CSkinItemEdit::OnSetFocus(UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* lpResult)
{
	LRESULT ls;
	OnNcPaint(0, 0, 0, &ls);
	//::PostMessage(m_hWnd, WM_NCPAINT, 0, 0);

	return CSkinItem::OnSetFocus(uMsg, wParam, lParam, lpResult);
}

BOOL CSkinItemEdit::OnKillFocus(UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* lpResult)
{
	LRESULT ls;
	OnNcPaint(0, 0, 0, &ls);
	//::PostMessage(m_hWnd, WM_NCPAINT, 0, 0);

	HWND hParent = ::GetParent(m_hWnd);
	TCHAR className[256];
	::GetClassName(hParent, className, 255);

	//ComboBox
	if ( _tcscmp(_T("ComboBox"), className) == 0 ) {
		::SendMessage(hParent, WM_COMMAND, (EN_KILLFOCUS << 16 | ::GetDlgCtrlID(m_hWnd)), (LPARAM)m_hWnd);
	}

	return CSkinItem::OnKillFocus(uMsg, wParam, lParam, lpResult);
}

BOOL CSkinItemEdit::CtlColor(CDC* pDC, CWnd* pWnd, HBRUSH& hBrush)
{
    if (m_bSelfOnCtlColor)
        return FALSE;

    if (IsReadOnly() && !GetDisabled()) {
        hBrush = NULL;
        BOOL bCtlColor = FALSE;
        CRect skinRect = GetSkinAbsRect();
        m_brushBk.DeleteObject();
        if (m_clrBkColorReadOnly != -1){
            LONG transedColor = m_clrBkColorReadOnly;
            if (m_bColorTransform && m_bBkColorTransform)
                GetEngine()->TransformColor(transedColor, transedColor);
            pDC->SetBkColor(transedColor);
            m_brushBk.CreateSolidBrush(transedColor);
            bCtlColor = TRUE;
        }
        int textColor = GettextColorReadOnly();
        if (textColor == -1) {
            textColor = GettextColorNormal();
        }
        if (textColor != -1) {
            pDC->SetTextColor(textColor);
        }
        if (m_pbkImageNormal != NULL){
            m_pbkImageNormal->Draw(*pDC, &skinRect, NULL, GetImageParam());
        }
        if (bCtlColor) {
            hBrush = m_brushBk;
            return TRUE;
        }
        return CSkinItem::CtlColor(pDC, pWnd, hBrush);
    } else {
        return CSkinItem::CtlColor(pDC, pWnd, hBrush);
    }
}

BOOL CSkinItemEdit::OnMouseEnter(UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* lpResult)
{
    ::PostMessage(m_hWnd, WM_NCPAINT, 0, 0);

    HWND hParent = ::GetParent(m_hWnd);
    TCHAR className[256];
    ::GetClassName(hParent, className, 255);

    //ComboBox
    if ( _tcscmp(_T("ComboBox"), className) == 0 ) {
        ::SendMessage(hParent, WM_COMMAND, (WM_MOUSEENTER << 16 | ::GetDlgCtrlID(m_hWnd)), (LPARAM)m_hWnd);
    }

    return CSkinItem::OnMouseEnter(uMsg, wParam, lParam, lpResult);
}

BOOL CSkinItemEdit::OnMouseLeave(UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* lpResult)
{
    ::PostMessage(m_hWnd, WM_NCPAINT, 0, 0);

    HWND hParent = ::GetParent(m_hWnd);
    TCHAR className[256];
    ::GetClassName(hParent, className, 255);

    //ComboBox
    if ( _tcscmp(_T("ComboBox"), className) == 0 ) {
        ::SendMessage(hParent, WM_COMMAND, (WM_MOUSELEAVE << 16 | ::GetDlgCtrlID(m_hWnd)), (LPARAM)m_hWnd);
    }

    return CSkinItem::OnMouseLeave(uMsg, wParam, lParam, lpResult);
}
