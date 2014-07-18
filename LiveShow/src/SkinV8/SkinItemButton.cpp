// SkinItemButton.cpp : 实现文件
//

#include "stdafx.h"
#include "commctrl.h"
#include "SkinItem.h"
#include "SkinEngine.h"
#include "ImageBase.h"
#include "SkinItemButton.h"

// Button control messages
#ifndef BCM_FIRST
#define BCM_FIRST               0x1600
#endif
#ifndef BCM_SETTEXTMARGIN
#define BCM_SETTEXTMARGIN       (BCM_FIRST + 0x0004)
#endif
#ifndef BCM_GETTEXTMARGIN
#define BCM_GETTEXTMARGIN       (BCM_FIRST + 0x0005)
#endif
#ifndef BST_HOT
#define BST_HOT            0x0200
#endif

// CSkinItemButton

IMPLEMENT_DYNCREATE(CSkinItemButton, CSkinItem)
CSkinItemButton::CSkinItemButton()
{
	EnableAutomation();
	
	// 为了使应用程序在 OLE 自动化对象处于活动状态时保持
	//	运行，构造函数调用 AfxOleLockApp。

	m_bTransparent = TRUE;

	m_buseOriginPosition = TRUE;

	m_enButtonStyle = BUTTON_STYLE_NORMAL;

	m_nTextLeftMargin = 4;

    m_bTextColorTransform = FALSE;

	for (int i=0; i<8; i++) {
		m_pNormalImages[i] = NULL;
		m_pCheckImages[i] = NULL;
		m_pRadioImages[i] = NULL;
	}

	AfxOleLockApp();
}

CSkinItemButton::~CSkinItemButton()
{
	// 为了在用 OLE 自动化创建所有对象后终止应用程序，
	// 	析构函数调用 AfxOleUnlockApp。
	
	AfxOleUnlockApp();
}


void CSkinItemButton::OnFinalRelease()
{
	// 释放了对自动化对象的最后一个引用后，将调用
	// OnFinalRelease。基类将自动
	// 删除该对象。在调用该基类之前，请添加您的
	// 对象所需的附加清除代码。

	CCmdTarget::OnFinalRelease();
}

BEGIN_MESSAGE_MAP(CSkinItemButton, CSkinItem)
END_MESSAGE_MAP()


BEGIN_SKIN_DISPATCH_MAP(CSkinItemButton, CSkinItem)
	SKINBASE_DISP_PROPERTIES(CSkinItemButton)
	SKINITEM_DISP_PROPERTIES(CSkinItemButton)

	SKIN_DISP_PROPERTY(CSkinItemButton, buttonStyle, VT_BSTR)
	SKIN_DISP_PROPERTY(CSkinItemButton, textLeftMargin, VT_I4)
    SKIN_DISP_PROPERTY(CSkinItemButton, textColorTransform, VT_BOOL)
	SKIN_DISP_PROPERTY(CSkinItemButton, checked, VT_BOOL)

	SKIN_DISP_PROPERTY(CSkinItemButton, normalImage0, VT_BSTR)
	SKIN_DISP_PROPERTY(CSkinItemButton, normalImage1, VT_BSTR)
	SKIN_DISP_PROPERTY(CSkinItemButton, normalImage2, VT_BSTR)
	SKIN_DISP_PROPERTY(CSkinItemButton, normalImage3, VT_BSTR)
	SKIN_DISP_PROPERTY(CSkinItemButton, normalImage4, VT_BSTR)
	SKIN_DISP_PROPERTY(CSkinItemButton, normalImage5, VT_BSTR)
	SKIN_DISP_PROPERTY(CSkinItemButton, normalImage6, VT_BSTR)
    SKIN_DISP_PROPERTY(CSkinItemButton, normalImage7, VT_BSTR)

	SKIN_DISP_PROPERTY(CSkinItemButton, checkImage0, VT_BSTR)
	SKIN_DISP_PROPERTY(CSkinItemButton, checkImage1, VT_BSTR)
	SKIN_DISP_PROPERTY(CSkinItemButton, checkImage2, VT_BSTR)
	SKIN_DISP_PROPERTY(CSkinItemButton, checkImage3, VT_BSTR)
	SKIN_DISP_PROPERTY(CSkinItemButton, checkImage4, VT_BSTR)
	SKIN_DISP_PROPERTY(CSkinItemButton, checkImage5, VT_BSTR)
	SKIN_DISP_PROPERTY(CSkinItemButton, checkImage6, VT_BSTR)
    SKIN_DISP_PROPERTY(CSkinItemButton, checkImage7, VT_BSTR)

	SKIN_DISP_PROPERTY(CSkinItemButton, radioImage0, VT_BSTR)
	SKIN_DISP_PROPERTY(CSkinItemButton, radioImage1, VT_BSTR)
	SKIN_DISP_PROPERTY(CSkinItemButton, radioImage2, VT_BSTR)
	SKIN_DISP_PROPERTY(CSkinItemButton, radioImage3, VT_BSTR)
	SKIN_DISP_PROPERTY(CSkinItemButton, radioImage4, VT_BSTR)
	SKIN_DISP_PROPERTY(CSkinItemButton, radioImage5, VT_BSTR)
	SKIN_DISP_PROPERTY(CSkinItemButton, radioImage6, VT_BSTR)
    SKIN_DISP_PROPERTY(CSkinItemButton, radioImage7, VT_BSTR)

	SKIN_DISP_PROPERTY_ALIAS(CSkinItemButton, style,  buttonStyle, VT_BSTR)
	SKIN_DISP_PROPERTY_ALIAS(CSkinItemButton, leftMargin,  textLeftMargin, VT_I4)

END_SKIN_DISPATCH_MAP(CSkinItemButton)

// 注意: 我们添加 IID_ISkinItemButton 支持
//以支持来自 VBA 的类型安全绑定。此 IID 必须同附加到 .IDL 文件中的
//调度接口的 GUID 匹配。

// {B6B69964-591D-45C3-8FB9-8C753F550EF5}
static const IID IID_ISkinItemButton =
{ 0xB6B69964, 0x591D, 0x45C3, { 0x8F, 0xB9, 0x8C, 0x75, 0x3F, 0x55, 0xE, 0xF5 } };

BEGIN_INTERFACE_MAP(CSkinItemButton, CSkinItem)
	INTERFACE_PART(CSkinItemButton, IID_ISkinItemButton, Dispatch)
END_INTERFACE_MAP()

// {A702D6BF-4296-42BC-B9BA-529D2A07C925}
IMPLEMENT_OLECREATE_FLAGS(CSkinItemButton, "Skin.SkinItemButton", afxRegApartmentThreading, 0xa702d6bf, 0x4296, 0x42bc, 0xb9, 0xba, 0x52, 0x9d, 0x2a, 0x7, 0xc9, 0x25)


// CSkinItemButton 消息处理程序
BOOL CSkinItemButton::GetCheck(UINT& uCheck)
{
	ASSERT(::IsWindow(m_hWnd));
	if (!::IsWindow(m_hWnd))
		return FALSE;

	uCheck = (UINT)::SendMessage(m_hWnd, BM_GETCHECK, 0, 0);
	return TRUE;
}

BOOL CSkinItemButton::SetCheck(UINT uCheck)
{
	ASSERT(::IsWindow(m_hWnd));
	if (!::IsWindow(m_hWnd))
		return FALSE;

	::SendMessage(m_hWnd, BM_SETCHECK, uCheck, 0);
	return TRUE;
}

BOOL CSkinItemButton::GetState(UINT& uState)
{
	ASSERT(::IsWindow(m_hWnd));
	if (!::IsWindow(m_hWnd))
		return FALSE;

	uState = (UINT)::SendMessage(m_hWnd, BM_GETSTATE, 0, 0);
	return TRUE;
}

BOOL CSkinItemButton::SetState(UINT uState)
{
	ASSERT(::IsWindow(m_hWnd));
	if (!::IsWindow(m_hWnd))
		return FALSE;

	::SendMessage(m_hWnd, BM_SETSTATE, uState, 0);
	return TRUE;
}

BOOL CSkinItemButton::GetTextMargin(LPRECT pMargin)
{
	ASSERT(::IsWindow(m_hWnd));
	if (!::IsWindow(m_hWnd))
		return FALSE;

	return (BOOL)::SendMessage(m_hWnd, BCM_GETTEXTMARGIN, 0, (LPARAM)pMargin);
}

BOOL CSkinItemButton::SetTextMargin(LPRECT pMargin)
{
	ASSERT(::IsWindow(m_hWnd));
	if (!::IsWindow(m_hWnd))
		return FALSE;

	return (BOOL)::SendMessage(m_hWnd, BCM_SETTEXTMARGIN, 0, (LPARAM)pMargin);
}

void CSkinItemButton::DrawBackground(CDC* pDC, LPRECT lpInvalidRect, CRgn* pInvalidRgn, BOOL bChildSkin)
{
	CSkinBase::DrawBackground(pDC, lpInvalidRect, pInvalidRgn);
}

void CSkinItemButton::DrawForeground(CDC* pDC, LPRECT lpInvalidRect, CRgn* pInvalidRgn)
{
	if (m_textStyleNormal.IsFontChanged()) {
		m_textStyleNormal.UpdateFont(*pDC);
	}

	BOOL bRet = FALSE;

	CRect textMargin(0, 0, 0, 0);
	bRet = GetTextMargin(&textMargin);

	UINT uState = 0;
	BOOL bGetState = GetState(uState);

	//根据状态计算要显示的图标下标
	BOOL bCheck = FALSE;
	BOOL bBushed = FALSE;
	BOOL bHot = FALSE;
	BOOL bFocus = FALSE;
	if (bGetState) {
		bCheck = (BST_CHECKED&uState) != 0;
		bBushed = (BST_PUSHED&uState) != 0;
		bHot = (BST_HOT&uState) != 0;
		bFocus = (BST_FOCUS&uState) != 0;
	}
    BOOL bDisabled = GetDisabled();
    int imgIndex = 0;
    if (! bCheck) {
        imgIndex = 0;
    } else {
        imgIndex = 4;
    }
    if (bDisabled) {
        imgIndex += 3;
    } else if (bHot && !bBushed) {
        imgIndex += 1;
    } else if (bHot && bBushed) {
        imgIndex += 2;
    } else {
        imgIndex += 0;
    }

	//获得要绘制的按钮图像
	CImageBase* pDrawImage = NULL;
	if (m_enButtonStyle == BUTTON_STYLE_NORMAL) {
		pDrawImage = m_pNormalImages[imgIndex];
		if (pDrawImage == NULL)
			pDrawImage = m_pNormalImages[0];
	} else if (m_enButtonStyle == BUTTON_STYLE_CHECK) {
		pDrawImage = m_pCheckImages[imgIndex];
		if (pDrawImage == NULL)
			pDrawImage = m_pCheckImages[0];
	} else if (m_enButtonStyle == BUTTON_STYLE_RADIO) {
		pDrawImage = m_pRadioImages[imgIndex];
		if (pDrawImage == NULL)
			pDrawImage = m_pRadioImages[0];
	}

	//获得绝对矩形
	CRect skinRect = GetSkinAbsRect();

	//计算绘制Check框
	LONG width = 0;
	LONG height = 0;
	if (m_enButtonStyle != BUTTON_STYLE_NORMAL) {
		CRect checkRect(0, 0, width, height);
		if (pDrawImage != NULL) {
			width = pDrawImage->Width();
			height = pDrawImage->Height();
			checkRect = CRect(0, 0, width, height);
			checkRect.OffsetRect(0, (skinRect.Height() - checkRect.Height()) / 2);
            DrawImageAlphaHelper(pDrawImage, *pDC, &checkRect, NULL, m_bAlpha);
		}
	} else {
        DrawImageAlphaHelper(pDrawImage, *pDC, &skinRect, NULL, m_bAlpha);
	}

	//计算绘制文字
	CRect textRect = skinRect;
	if (m_enButtonStyle != BUTTON_STYLE_NORMAL)
		textRect.left += width + m_nTextLeftMargin;

	CRect realTextRect;
    LONG textColor = m_textStyleNormal.Getcolor();
    if (bHot) {
        textColor = m_textStyleHover.Getcolor();
    } else if (GetDisabled()) {
        textColor = m_textStyleDisabled.Getcolor();
    }
    if (GetColorTransform() && m_bTextColorTransform)
        GetEngine()->TransformColor(textColor, textColor);

	m_textStyleNormal.DrawTextEx(*pDC, m_strText, &textRect, textColor, &realTextRect);

	if (bFocus && !m_strText.IsEmpty()) {
		realTextRect.InflateRect(1, 1);
		//pDC->DrawFocusRect(&realTextRect);  //DragonZ:去掉点击后获得焦点有虚框的效果
	}
}

void CSkinItemButton::DrawFrame(CDC* pDC, LPRECT lpInvalidRect, CRgn* pInvalidRgn)
{

}

void CSkinItemButton::Draw(CDC* pDC, LPRECT lpInvalidRect, CRgn* pInvalidRgn)
{
	DrawBackground(pDC, lpInvalidRect, pInvalidRgn);
	DrawForeground(pDC, lpInvalidRect, pInvalidRgn);
	DrawFrame(pDC, lpInvalidRect, pInvalidRgn);
}

BOOL CSkinItemButton::OnBMSetState(UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* lpResult)
{
	*lpResult = DefWndProc(uMsg, wParam, lParam);

    CSkinBase::OnSetState((UINT)wParam);

	InvalidateRect();

	return TRUE;
}

BOOL CSkinItemButton::OnBMSetCheck(UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* lpResult)
{
	*lpResult = DefWndProc(uMsg, wParam, lParam);

    CSkinBase::OnSetCheck((UINT)wParam);

	InvalidateRect();

	return TRUE;
}

BOOL CSkinItemButton::OnPaint(UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* lpResult)
{
	*lpResult = 0;

	ASSERT(::IsWindow(m_hWnd));
	if (! ::IsWindow(m_hWnd) || ::IsIconic(m_hWnd))
		return FALSE;

	//准备CWnd
	CWnd* pWnd = CWnd::FromHandle(m_hWnd);
	if (pWnd == NULL)
		return TRUE;

	//准备DC
	CPaintDC dc(pWnd);

	//获得相关矩形
	CRect rcInvalid = dc.m_ps.rcPaint;

	//双缓存
	CDC memDC;
	if(!memDC.CreateCompatibleDC(&dc))
	{
		assert(0);
		return FALSE;
	}
	CBitmap memBitmap;
	if(!memBitmap.CreateCompatibleBitmap(&dc, GetWidth(), GetHeight()))
	{
		assert(0);
		return FALSE;
	}

	CBitmap* pOldBitmap = memDC.SelectObject(&memBitmap);

	//在缓存上绘制
	Draw(&memDC, &rcInvalid, NULL);

	//复制缓存DC到显示DC
	dc.BitBlt(0, 0, GetWidth(), GetHeight(), &memDC, 0, 0, SRCCOPY);

	//恢复memDC
	memDC.SelectObject(pOldBitmap);

	memBitmap.DeleteObject();

	return !GetselfOnPaint();
}

BOOL CSkinItemButton::OnEnable(UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* lpResult)
{
	*lpResult = 0;
	//*lpResult = DefWndProc(uMsg, wParam, lParam);
	InvalidateRect();
	CSkinItem::OnEnable(uMsg, wParam, lParam, lpResult);
	return TRUE;
}

BOOL CSkinItemButton::OnSetText(UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* lpResult)
{
	BOOL bRet = CSkinItem::OnSetText(uMsg, wParam, lParam, lpResult);
	if (*lpResult == TRUE) {
		InvalidateRect();
	}
	return bRet;
}

BSTR CSkinItemButton::GetbuttonStyle(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: 在此添加调度处理程序代码
	CString strResult;
	switch (m_enButtonStyle)
	{
	case BUTTON_STYLE_NORMAL:
		strResult = _T("normal");
		break;
	case BUTTON_STYLE_CHECK:
		strResult = _T("normal");
		break;
	case BUTTON_STYLE_RADIO:
		strResult = _T("normal");
		break;
	default:
		break;
	}
	return strResult.AllocSysString();
}

void CSkinItemButton::SetbuttonStyle(LPCTSTR newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: 在此添加属性处理程序代码
	CString strStyle = newVal;
	SKINITEM_BUTTON_STYLE newStyle = m_enButtonStyle;
	if (strStyle.CompareNoCase(_T("normal")) == 0) {
		newStyle = BUTTON_STYLE_NORMAL;
	} else if (strStyle.CompareNoCase(_T("check")) == 0) {
		newStyle = BUTTON_STYLE_CHECK;
	} else if (strStyle.CompareNoCase(_T("radio")) == 0) {
		newStyle = BUTTON_STYLE_RADIO;
	} 
	if (newStyle == m_enButtonStyle)
		return;
	m_enButtonStyle = newStyle;
	InvalidateRect();
}

LONG CSkinItemButton::GettextLeftMargin()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: 在此添加属性处理程序代码
	return m_nTextLeftMargin;
}

void CSkinItemButton::SettextLeftMargin(LONG newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: 在此添加调度处理程序代码
	if (m_nTextLeftMargin == newVal)
		return;

	m_nTextLeftMargin = newVal;
	InvalidateRect();
}

VARIANT_BOOL CSkinItemButton::Getchecked()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: 在此添加属性处理程序代码
	UINT uCheck = 0;
	if (! GetCheck(uCheck))
		return VARIANT_FALSE;

	return (uCheck != 0) ? (VARIANT_TRUE) : (VARIANT_FALSE);
}

void CSkinItemButton::Setchecked(VARIANT_BOOL newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: 在此添加属性处理程序代码
	UINT uCheck = (newVal != VARIANT_FALSE);
	SetCheck(uCheck);
}

VARIANT_BOOL CSkinItemButton::GettextColorTransform()
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());

    // TODO: 在此添加属性处理程序代码
    return m_bTextColorTransform ? (VARIANT_TRUE) : (VARIANT_FALSE);
}

void CSkinItemButton::SettextColorTransform(VARIANT_BOOL newVal)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());

    // TODO: 在此添加属性处理程序代码
    newVal = (newVal != VARIANT_FALSE);
    if (m_bTextColorTransform == newVal)
        return;

    m_bTextColorTransform = newVal;
    InvalidateRect();
}

//////////////////////////////////////////////////////////////////////////
//normal
//////////////////////////////////////////////////////////////////////////
BSTR CSkinItemButton::GetnormalImage0()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: 在此添加调度处理程序代码
	CString str;
	return str.AllocSysString();
}

void CSkinItemButton::SetnormalImage0(LPCTSTR newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: 在此添加属性处理程序代码
	CString str;
	SetImageHelper(m_pNormalImages[0], str, newVal, FALSE);
}

BSTR CSkinItemButton::GetnormalImage1()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: 在此添加调度处理程序代码
	CString str;
	return str.AllocSysString();
}

void CSkinItemButton::SetnormalImage1(LPCTSTR newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: 在此添加属性处理程序代码
	CString str;
	SetImageHelper(m_pNormalImages[1], str, newVal, FALSE);
}

BSTR CSkinItemButton::GetnormalImage2()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: 在此添加调度处理程序代码
	CString str;
	return str.AllocSysString();
}

void CSkinItemButton::SetnormalImage2(LPCTSTR newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: 在此添加属性处理程序代码
	CString str;
	SetImageHelper(m_pNormalImages[2], str, newVal, FALSE);
}

BSTR CSkinItemButton::GetnormalImage3()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: 在此添加调度处理程序代码
	CString str;
	return str.AllocSysString();
}

void CSkinItemButton::SetnormalImage3(LPCTSTR newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: 在此添加属性处理程序代码
	CString str;
	SetImageHelper(m_pNormalImages[3], str, newVal, FALSE);
}

BSTR CSkinItemButton::GetnormalImage4()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: 在此添加调度处理程序代码
	CString str;
	return str.AllocSysString();
}

void CSkinItemButton::SetnormalImage4(LPCTSTR newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: 在此添加属性处理程序代码
	CString str;
	SetImageHelper(m_pNormalImages[4], str, newVal, FALSE);
}

BSTR CSkinItemButton::GetnormalImage5()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: 在此添加调度处理程序代码
	CString str;
	return str.AllocSysString();
}

void CSkinItemButton::SetnormalImage5(LPCTSTR newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: 在此添加属性处理程序代码
	CString str;
	SetImageHelper(m_pNormalImages[5], str, newVal, FALSE);
}

BSTR CSkinItemButton::GetnormalImage6()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: 在此添加调度处理程序代码
	CString str;
	return str.AllocSysString();
}

void CSkinItemButton::SetnormalImage6(LPCTSTR newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: 在此添加属性处理程序代码
	CString str;
	SetImageHelper(m_pNormalImages[6], str, newVal, FALSE);
}

BSTR CSkinItemButton::GetnormalImage7()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: 在此添加调度处理程序代码
	CString str;
	return str.AllocSysString();
}

void CSkinItemButton::SetnormalImage7(LPCTSTR newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: 在此添加属性处理程序代码
	CString str;
	SetImageHelper(m_pNormalImages[7], str, newVal, FALSE);
}

//////////////////////////////////////////////////////////////////////////
//check 
//////////////////////////////////////////////////////////////////////////
BSTR CSkinItemButton::GetcheckImage0()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: 在此添加调度处理程序代码
	CString str;
	return str.AllocSysString();
}

void CSkinItemButton::SetcheckImage0(LPCTSTR newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: 在此添加属性处理程序代码
	CString str;
	SetImageHelper(m_pCheckImages[0], str, newVal, FALSE);
}

BSTR CSkinItemButton::GetcheckImage1()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: 在此添加调度处理程序代码
	CString str;
	return str.AllocSysString();
}

void CSkinItemButton::SetcheckImage1(LPCTSTR newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: 在此添加属性处理程序代码
	CString str;
	SetImageHelper(m_pCheckImages[1], str, newVal, FALSE);
}

BSTR CSkinItemButton::GetcheckImage2()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: 在此添加调度处理程序代码
	CString str;
	return str.AllocSysString();
}

void CSkinItemButton::SetcheckImage2(LPCTSTR newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: 在此添加属性处理程序代码
	CString str;
	SetImageHelper(m_pCheckImages[2], str, newVal, FALSE);
}

BSTR CSkinItemButton::GetcheckImage3()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: 在此添加调度处理程序代码
	CString str;
	return str.AllocSysString();
}

void CSkinItemButton::SetcheckImage3(LPCTSTR newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: 在此添加属性处理程序代码
	CString str;
	SetImageHelper(m_pCheckImages[3], str, newVal, FALSE);
}

BSTR CSkinItemButton::GetcheckImage4()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: 在此添加调度处理程序代码
	CString str;
	return str.AllocSysString();
}

void CSkinItemButton::SetcheckImage4(LPCTSTR newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: 在此添加属性处理程序代码
	CString str;
	SetImageHelper(m_pCheckImages[4], str, newVal, FALSE);
}

BSTR CSkinItemButton::GetcheckImage5()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: 在此添加调度处理程序代码
	CString str;
	return str.AllocSysString();
}

void CSkinItemButton::SetcheckImage5(LPCTSTR newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: 在此添加属性处理程序代码
	CString str;
	SetImageHelper(m_pCheckImages[5], str, newVal, FALSE);
}

BSTR CSkinItemButton::GetcheckImage6()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: 在此添加调度处理程序代码
	CString str;
	return str.AllocSysString();
}

void CSkinItemButton::SetcheckImage6(LPCTSTR newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: 在此添加属性处理程序代码
	CString str;
	SetImageHelper(m_pCheckImages[6], str, newVal, FALSE);
}

BSTR CSkinItemButton::GetcheckImage7()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: 在此添加调度处理程序代码
	CString str;
	return str.AllocSysString();
}

void CSkinItemButton::SetcheckImage7(LPCTSTR newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: 在此添加属性处理程序代码
	CString str;
	SetImageHelper(m_pCheckImages[7], str, newVal, FALSE);
}

//////////////////////////////////////////////////////////////////////////
//radio 
//////////////////////////////////////////////////////////////////////////
BSTR CSkinItemButton::GetradioImage0()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: 在此添加调度处理程序代码
	CString str;
	return str.AllocSysString();
}

void CSkinItemButton::SetradioImage0(LPCTSTR newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: 在此添加属性处理程序代码
	CString str;
	SetImageHelper(m_pRadioImages[0], str, newVal, FALSE);
}

BSTR CSkinItemButton::GetradioImage1()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: 在此添加调度处理程序代码
	CString str;
	return str.AllocSysString();
}

void CSkinItemButton::SetradioImage1(LPCTSTR newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: 在此添加属性处理程序代码
	CString str;
	SetImageHelper(m_pRadioImages[1], str, newVal, FALSE);
}

BSTR CSkinItemButton::GetradioImage2()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: 在此添加调度处理程序代码
	CString str;
	return str.AllocSysString();
}

void CSkinItemButton::SetradioImage2(LPCTSTR newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: 在此添加属性处理程序代码
	CString str;
	SetImageHelper(m_pRadioImages[2], str, newVal, FALSE);
}

BSTR CSkinItemButton::GetradioImage3()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: 在此添加调度处理程序代码
	CString str;
	return str.AllocSysString();
}

void CSkinItemButton::SetradioImage3(LPCTSTR newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: 在此添加属性处理程序代码
	CString str;
	SetImageHelper(m_pRadioImages[3], str, newVal, FALSE);
}

BSTR CSkinItemButton::GetradioImage4()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: 在此添加调度处理程序代码
	CString str;
	return str.AllocSysString();
}

void CSkinItemButton::SetradioImage4(LPCTSTR newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: 在此添加属性处理程序代码
	CString str;
	SetImageHelper(m_pRadioImages[4], str, newVal, FALSE);
}

BSTR CSkinItemButton::GetradioImage5()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: 在此添加调度处理程序代码
	CString str;
	return str.AllocSysString();
}

void CSkinItemButton::SetradioImage5(LPCTSTR newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: 在此添加属性处理程序代码
	CString str;
	SetImageHelper(m_pRadioImages[5], str, newVal, FALSE);
}

BSTR CSkinItemButton::GetradioImage6()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: 在此添加调度处理程序代码
	CString str;
	return str.AllocSysString();
}

void CSkinItemButton::SetradioImage6(LPCTSTR newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: 在此添加属性处理程序代码
	CString str;
	SetImageHelper(m_pRadioImages[6], str, newVal, FALSE);
}

BSTR CSkinItemButton::GetradioImage7()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: 在此添加调度处理程序代码
	CString str;
	return str.AllocSysString();
}

void CSkinItemButton::SetradioImage7(LPCTSTR newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: 在此添加属性处理程序代码
	CString str;
	SetImageHelper(m_pRadioImages[7], str, newVal, FALSE);
}
