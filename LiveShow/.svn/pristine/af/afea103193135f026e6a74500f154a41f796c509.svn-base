// SkinItemTab.cpp : 实现文件
//

#include "stdafx.h"
#include "ImageBase.h"
#include "SkinItemTab.h"
#include "RenderEngine.h"


// CSkinItemTab

IMPLEMENT_DYNCREATE(CSkinItemTab, CSkinItem)
CSkinItemTab::CSkinItemTab()
{
	EnableAutomation();
	
	// 为了使应用程序在 OLE 自动化对象处于活动状态时保持
	//	运行，构造函数调用 AfxOleLockApp。

	m_pNormalTabImage = NULL;
	m_pSelectedTabImage = NULL;
	m_textStyleTab.SethorizAlign(_T("center"));

	m_szItemSize = CSize(40, 24);
	
	AfxOleLockApp();
}

CSkinItemTab::~CSkinItemTab()
{
	// 为了在用 OLE 自动化创建所有对象后终止应用程序，
	// 	析构函数调用 AfxOleUnlockApp。
	
	AfxOleUnlockApp();
}


void CSkinItemTab::OnFinalRelease()
{
	// 释放了对自动化对象的最后一个引用后，将调用
	// OnFinalRelease。基类将自动
	// 删除该对象。在调用该基类之前，请添加您的
	// 对象所需的附加清除代码。

	CSkinItem::OnFinalRelease();
}


BEGIN_MESSAGE_MAP(CSkinItemTab, CSkinItem)
END_MESSAGE_MAP()


BEGIN_SKIN_DISPATCH_MAP(CSkinItemTab, CSkinItem)
	SKINBASE_DISP_PROPERTIES(CSkinItemTab)
	SKINITEM_DISP_PROPERTIES(CSkinItemTab)

	SKIN_DISP_PROPERTY(CSkinItemTab, tabTextColor, VT_I4)
	SKIN_DISP_PROPERTY(CSkinItemTab, normalTabImage, VT_BSTR)
	SKIN_DISP_PROPERTY(CSkinItemTab, selectedTabImage, VT_BSTR)
	SKIN_DISP_PROPERTY(CSkinItemTab, tabItemWidth, VT_I4)
	SKIN_DISP_PROPERTY(CSkinItemTab, tabItemHeight, VT_I4)

END_SKIN_DISPATCH_MAP(CSkinItemTab)

// 注意: 我们添加 IID_ISkinItemTab 支持
//以支持来自 VBA 的类型安全绑定。此 IID 必须同附加到 .IDL 文件中的
//调度接口的 GUID 匹配。

// {B547AA90-DB57-4205-B7BA-2D62AE026CC1}
static const IID IID_ISkinItemTab =
{ 0xB547AA90, 0xDB57, 0x4205, { 0xB7, 0xBA, 0x2D, 0x62, 0xAE, 0x2, 0x6C, 0xC1 } };

BEGIN_INTERFACE_MAP(CSkinItemTab, CSkinItem)
	INTERFACE_PART(CSkinItemTab, IID_ISkinItemTab, Dispatch)
END_INTERFACE_MAP()

// {F7DE59C4-5CF6-432B-A4D1-ECE537DF6615}
IMPLEMENT_OLECREATE_FLAGS(CSkinItemTab, "Skin.SkinItemTab", afxRegApartmentThreading, 0xf7de59c4, 0x5cf6, 0x432b, 0xa4, 0xd1, 0xec, 0xe5, 0x37, 0xdf, 0x66, 0x15)


// CSkinItemTab 消息处理程序
LONG CSkinItemTab::GetTabItemCount()
{
	if (! ::IsWindow(m_hWnd))
		return FALSE;

	return (LONG)::SendMessage(m_hWnd, TCM_GETITEMCOUNT, (WPARAM)0, (LPARAM)0);
}

LONG CSkinItemTab::GetTabItemCurSel()
{
	if (! ::IsWindow(m_hWnd))
		return FALSE;

	return (LONG)::SendMessage(m_hWnd, TCM_GETCURSEL, (WPARAM)0, (LPARAM)0);
}

BOOL CSkinItemTab::GetTabItemRect(LONG iItem, RECT& rect)
{
	if (! ::IsWindow(m_hWnd))
		return FALSE;

	return (BOOL)::SendMessage(m_hWnd, TCM_GETITEMRECT, (WPARAM)iItem, (LPARAM)(&rect));
}

BOOL CSkinItemTab::GetTabItemText(LONG iItem, CString& text)
{
	if (! ::IsWindow(m_hWnd))
		return FALSE;

	TCITEM tcm;
	memset(&tcm, 0, sizeof(tcm));
	tcm.mask = TCIF_TEXT;
	tcm.pszText = text.GetBuffer(1024);
	tcm.cchTextMax = 1023;

	BOOL bRet = (BOOL)::SendMessage(m_hWnd, TCM_GETITEM, (WPARAM)(iItem), (LPARAM)(&tcm));
	text.ReleaseBuffer();

	return bRet;
}

BOOL CSkinItemTab::SetTabItemSize(const SIZE& size)
{
	if (! ::IsWindow(m_hWnd))
		return FALSE;

	(DWORD)::SendMessage(m_hWnd, TCM_SETITEMSIZE, 0, MAKELPARAM(size.cx, size.cy));
	return TRUE;
}

BOOL CSkinItemTab::GetAllTabItemRgn(CRgn& rgn)
{
	rgn.CreateRectRgn(0, 0, 0, 0);
	LONG itemCount = GetTabItemCount();
	if (itemCount <= 0)
		return TRUE;
	CRect itemRect;
	for (int i=0; i<itemCount; i++) {
		if (GetTabItemRect(i, itemRect)) {
			CRgn itemRgn;
			itemRgn.CreateRectRgnIndirect(&itemRect);
			rgn.CombineRgn(&rgn, &itemRgn, RGN_OR);
			itemRgn.DeleteObject();
		}
	}
	return FALSE;
}

void CSkinItemTab::DrawBackground(CDC* pDC, LPRECT lpInvalidRect, CRgn* pInvalidRgn, BOOL bChildSkin)
{
	CRgn allItemRgn;
	GetAllTabItemRgn(allItemRgn);
	int iSaveDC = pDC->SaveDC();
	pDC->SelectClipRgn(&allItemRgn, RGN_DIFF);

	if (GetTransparent()) {
		DrawParentBackground(pDC, lpInvalidRect, pInvalidRgn);
	}

	CRect skinAbsRect = GetSkinAbsRect();
	if (GetbkColorNormal() != -1) {
		CRenderEngine::FillRect(*pDC, skinAbsRect, GetbkColorNormal());
	}

	if (m_pbkImageNormal != NULL) {
		if (GetColorTransform()) {
			m_pbkImageNormal->Draw(*pDC, &skinAbsRect, lpInvalidRect, 
				GetImageParam(), GetTransformedTransparentColor());
		} else {
			m_pbkImageNormal->Draw(*pDC, &skinAbsRect, lpInvalidRect, 
				NULL, GetTransparentColor());
		}
	}

	pDC->RestoreDC(iSaveDC);

	allItemRgn.DeleteObject();
}

void CSkinItemTab::DrawForeground(CDC* pDC, LPRECT lpInvalidRect, CRgn* pInvalidRgn)
{
	LONG itemCount = -1;
	itemCount = GetTabItemCount();
	if (itemCount <= 0)
		return;

	CRect invalidRect = GetSkinAbsRect();
	if (lpInvalidRect != 0) {
		invalidRect = lpInvalidRect;
	}

	const LONG itemCurSel = GetTabItemCurSel();

	CRect itemRect(0, 0, 0, 0);
	CString itemText;
	CRect itemTextArea;
	for (LONG i=0; i<itemCount; i++) {

		if (i == itemCurSel)
			continue;

		if (! GetTabItemRect(i, itemRect)) 
			continue;

		CRect tempRect;
		tempRect.IntersectRect(&invalidRect, &itemRect);
		if (tempRect.IsRectEmpty())
			continue;

		CImageBase* pImage = m_pNormalTabImage;
		if (pImage != NULL) {
			if (GetColorTransform()) {
				pImage->Draw(*pDC, &itemRect, NULL, 
					GetImageParam(), GetTransformedTransparentColor());
			} else {
				pImage->Draw(*pDC, &itemRect, NULL, 
					NULL, GetTransparentColor());
			}
		} else {
			CRenderEngine::FillRect(*pDC, itemRect, RGB(200, 200, 255));
		}

		if (! GetTabItemText(i, itemText))
			continue;

		m_textStyleTab.DrawText(*pDC, itemText, &itemRect, &itemTextArea);

	}

	if (GetTabItemRect(itemCurSel, itemRect)) {
		CRect tempRect;
		tempRect.IntersectRect(&invalidRect, &itemRect);
		if (! tempRect.IsRectEmpty()) {
			CImageBase* pImage = m_pSelectedTabImage;
			if (pImage == NULL)
				pImage = m_pNormalTabImage;
			if (pImage != NULL) {
				if (GetColorTransform()) {
					pImage->Draw(*pDC, &itemRect, NULL, 
						GetImageParam(), GetTransformedTransparentColor());
				} else {
					pImage->Draw(*pDC, &itemRect, NULL, 
						NULL, GetTransparentColor());
				}
			} else {
				CRenderEngine::FillRect(*pDC, itemRect, RGB(255, 255, 255));
			}
			if (GetTabItemText(itemCurSel, itemText)) {
				m_textStyleTab.DrawText(*pDC, itemText, &itemRect, &itemTextArea);
				if (IsFocused()) {
				//	pDC->DrawFocusRect(&itemTextArea);  //DragonZ:去掉点击后获得焦点有虚框的效果
				}
			}
		}
	}

}

void CSkinItemTab::DrawFrame(CDC* pDC, LPRECT lpInvalidRect, CRgn* pInvalidRgn)
{
	
}

void CSkinItemTab::Draw(CDC* pDC, LPRECT lpInvalidRect, CRgn* pInvalidRgn)
{
	DrawBackground(pDC, lpInvalidRect, pInvalidRgn);
	DrawForeground(pDC, lpInvalidRect, pInvalidRgn);
	DrawFrame(pDC, lpInvalidRect, pInvalidRgn);
}

BOOL CSkinItemTab::OnNcPaint(UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* lpResult)
{
	return FALSE;
}

BOOL CSkinItemTab::OnPaint(UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* lpResult)
{
	CWnd* pWnd = CWnd::FromHandle(m_hWnd);

	CPaintDC dc(pWnd);
	CRect invalidRect = dc.m_ps.rcPaint;

	Draw(&dc, &invalidRect);

	return TRUE;
}

LONG CSkinItemTab::GettabItemWidth()
{
	return m_szItemSize.cx;
}

void CSkinItemTab::SettabItemWidth(LONG newVal)
{
	if (newVal < 0 || newVal == m_szItemSize.cx)
		return;

	m_szItemSize.cx = newVal;
	SetTabItemSize(m_szItemSize);
	InvalidateRect();
}

LONG CSkinItemTab::GettabItemHeight()
{
	return m_szItemSize.cy;
}

void CSkinItemTab::SettabItemHeight(LONG newVal)
{
	if (newVal < 0 || newVal == m_szItemSize.cy)
		return;

	m_szItemSize.cy = newVal;
	SetTabItemSize(m_szItemSize);
	InvalidateRect();
}

LONG CSkinItemTab::GettabTextColor()
{
	return m_textStyleTab.Getcolor();
}

void CSkinItemTab::SettabTextColor(LONG newVal)
{
	m_textStyleTab.Setcolor(newVal);
}

BSTR CSkinItemTab::GetnormalTabImage()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: 在此添加调度处理程序代码
	return m_strNormalTabImage.AllocSysString();
}

void CSkinItemTab::SetnormalTabImage(LPCTSTR newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: 在此添加属性处理程序代码
	SetImageHelper(m_pNormalTabImage, m_strNormalTabImage, newVal);
	InvalidateRect();
}

BSTR CSkinItemTab::GetselectedTabImage()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: 在此添加调度处理程序代码
	return m_strSelectedTabImage.AllocSysString();
}

void CSkinItemTab::SetselectedTabImage(LPCTSTR newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: 在此添加属性处理程序代码
	SetImageHelper(m_pSelectedTabImage, m_strSelectedTabImage, newVal);
	InvalidateRect();
}
