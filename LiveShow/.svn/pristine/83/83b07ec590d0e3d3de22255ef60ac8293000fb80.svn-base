// SkinItemProgress.cpp : 实现文件
//

#include "stdafx.h"
#include "SkinItem.h"
#include "SkinEngine.h"
#include "ImageBase.h"
#include "SkinItemProgress.h"


// CSkinItemProgress

IMPLEMENT_DYNCREATE(CSkinItemProgress, CSkinItem)
CSkinItemProgress::CSkinItemProgress()
{
	EnableAutomation();
	
	// 为了使应用程序在 OLE 自动化对象处于活动状态时保持
	//	运行，构造函数调用 AfxOleLockApp。

    SKINITEM_PROGRESS_IMAGE_LIST(CSkinItem, CLEAR_IMAGE_PROPERTY_VAR);

	AfxOleLockApp();
}

CSkinItemProgress::~CSkinItemProgress()
{
	// 为了在用 OLE 自动化创建所有对象后终止应用程序，
	// 	析构函数调用 AfxOleUnlockApp。
	
	AfxOleUnlockApp();
}


void CSkinItemProgress::OnFinalRelease()
{
	// 释放了对自动化对象的最后一个引用后，将调用
	// OnFinalRelease。基类将自动
	// 删除该对象。在调用该基类之前，请添加您的
	// 对象所需的附加清除代码。

	CSkinItem::OnFinalRelease();
}

BEGIN_MESSAGE_MAP(CSkinItemProgress, CSkinItem)
END_MESSAGE_MAP()


BEGIN_SKIN_DISPATCH_MAP(CSkinItemProgress, CSkinItem)
	SKINBASE_DISP_PROPERTIES(CSkinItemProgress)
	SKINITEM_DISP_PROPERTIES(CSkinItemProgress)
    SKINITEM_PROGRESS_IMAGE_LIST(CSkinItemProgress, SKIN_DISP_PROPERTY_BSTR)

END_SKIN_DISPATCH_MAP(CSkinItemProgress)

// 注意: 我们添加 IID_ISkinItemProgress 支持
//以支持来自 VBA 的类型安全绑定。此 IID 必须同附加到 .IDL 文件中的
//调度接口的 GUID 匹配。

// {19F76DC7-2B29-4ABE-B3CE-B11D58D3E624}
static const IID IID_ISkinItemProgress =
{ 0x19F76DC7, 0x2B29, 0x4ABE, { 0xB3, 0xCE, 0xB1, 0x1D, 0x58, 0xD3, 0xE6, 0x24 } };

BEGIN_INTERFACE_MAP(CSkinItemProgress, CSkinItem)
	INTERFACE_PART(CSkinItemProgress, IID_ISkinItemProgress, Dispatch)
END_INTERFACE_MAP()

// {7638541C-297A-4051-BCCE-91FD5A8C5F0A}
IMPLEMENT_OLECREATE_FLAGS(CSkinItemProgress, "Skin.SkinItemProgress", afxRegApartmentThreading, 0x7638541c, 0x297a, 0x4051, 0xbc, 0xce, 0x91, 0xfd, 0x5a, 0x8c, 0x5f, 0xa)


// CSkinItemProgress 消息处理程序
BOOL CSkinItemProgress::GetPos(LONG& pos)
{
	ASSERT(::IsWindow(m_hWnd));
	if (!::IsWindow(m_hWnd))
		return FALSE;

	pos = (UINT)::SendMessage(m_hWnd, PBM_GETPOS, 0, 0);
	return TRUE;
}

BOOL CSkinItemProgress::SetPos(LONG pos)
{
	ASSERT(::IsWindow(m_hWnd));
	if (!::IsWindow(m_hWnd))
		return FALSE;

	::SendMessage(m_hWnd, PBM_SETPOS, 0, (LPARAM)pos);
	return TRUE;
}

BOOL CSkinItemProgress::GetRange(LONG& minValue, LONG& maxValue)
{
	ASSERT(::IsWindow(m_hWnd));
	if (!::IsWindow(m_hWnd))
		return FALSE;

	PBRANGE range;
	::SendMessage(m_hWnd, PBM_GETRANGE, 0, (LPARAM)(&range));
	minValue = range.iLow;
	maxValue = range.iHigh;
	return TRUE;
}

BOOL CSkinItemProgress::SetRange(LONG minValue, LONG maxValue)
{
	ASSERT(::IsWindow(m_hWnd));
	if (!::IsWindow(m_hWnd))
		return FALSE;

	::SendMessage(m_hWnd, PBM_SETRANGE, minValue, (LPARAM)(maxValue));
	return TRUE;
}

void CSkinItemProgress::DrawBackground(CDC* pDC, LPRECT lpInvalidRect, CRgn* pInvalidRgn, BOOL bChildSkin)
{
	CSkinBase::DrawBackground(pDC, lpInvalidRect, pInvalidRgn);

	CRect skinRect = GetSkinAbsRect();
	LONG midLeft = 0;
	LONG midWidth = skinRect.Width();

	if (m_pbackLeftImage != NULL) {
		CRect leftDrawRect = CRect(0, 0, m_pbackLeftImage->Width(), skinRect.Height());
		m_pbackLeftImage->Draw(*pDC, &leftDrawRect, NULL, GetImageParam(), GetTransformedTransparentColor());
		midLeft += leftDrawRect.left + leftDrawRect.Width();
		midWidth -= leftDrawRect.Width();
	}

	if (m_pbackRightImage != NULL) {
		CRect rightDrawRect = CRect((skinRect.Width() - m_pbackRightImage->Width()), 0, skinRect.Width(), skinRect.Height());
		m_pbackRightImage->Draw(*pDC, &rightDrawRect, NULL, GetImageParam(), GetTransformedTransparentColor());
		midWidth -= rightDrawRect.Width();
	}

	if (m_pbackMidImage != NULL) {
		CRect midDrawRect = CRect(midLeft, 0, (midLeft + midWidth), skinRect.Height());
		m_pbackMidImage->Draw(*pDC, &midDrawRect, NULL, GetImageParam(), GetTransformedTransparentColor());
	}
}

void CSkinItemProgress::DrawForeground(CDC* pDC, LPRECT lpInvalidRect, CRgn* pInvalidRgn)
{
	LONG pos = 0;
	LONG minValue = 0;
	LONG maxValue = 0;
	GetPos(pos);
	GetRange(minValue, maxValue);

	CRect skinRect = GetSkinAbsRect();
	LONG left = 0;
	LONG width = skinRect.Width();

	LONG unitWidth = 0;
	if (m_pprogressImage != NULL) {
		unitWidth = m_pprogressImage->Width();
	}

	if (unitWidth <= 0 || m_pprogressImage == NULL)
		return;

	if (m_pbackLeftImage != NULL) {
		left += m_pbackLeftImage->Width();
		width -= m_pbackLeftImage->Width();
	}

	if (m_pbackRightImage != NULL) {
		width -= m_pbackRightImage->Width();
	}

	LONG unitNum = ( width * (pos - minValue) ) / ( (maxValue - minValue) * unitWidth );
	CRect drawRect = CRect(0, 0, m_pprogressImage->Width(), m_pprogressImage->Height());
	LONG yOffset = (skinRect.Height() - drawRect.Height()) / 2;
	drawRect.OffsetRect(left, yOffset);
	for (int i=0; i<unitNum; i++) {
		m_pprogressImage->Draw(*pDC, &drawRect, NULL, GetImageParam(), GetTransformedTransparentColor());
		drawRect.OffsetRect(unitWidth, 0);
	}
}

void CSkinItemProgress::DrawFrame(CDC* pDC, LPRECT lpInvalidRect, CRgn* pInvalidRgn)
{
}

void CSkinItemProgress::Draw(CDC* pDC, LPRECT lpInvalidRect, CRgn* pInvalidRgn)
{
	DrawBackground(pDC, lpInvalidRect, pInvalidRgn);
	DrawForeground(pDC, lpInvalidRect, pInvalidRgn);
	DrawFrame(pDC, lpInvalidRect, pInvalidRgn);
}

//更新皮肤属性，在皮肤新附着窗口时调用
BOOL CSkinItemProgress::OnPaint(UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* lpResult)
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
	memDC.CreateCompatibleDC(&dc);
	CBitmap memBitmap;
	memBitmap.CreateCompatibleBitmap(&dc, GetWidth(), GetHeight());
	CBitmap* pOldBitmap = memDC.SelectObject(&memBitmap);

	//在缓存上绘制
	Draw(&memDC, &rcInvalid, NULL);

	//复制缓存DC到显示DC
	dc.BitBlt(0, 0, GetWidth(), GetHeight(), &memDC, 0, 0, SRCCOPY);

	//恢复memDC
	memDC.SelectObject(pOldBitmap);

	memBitmap.DeleteObject();

	return TRUE;
}

SKINITEM_PROGRESS_IMAGE_LIST(CSkinItemProgress, IMPLEMENT_IMAGE_PROPERTY);

