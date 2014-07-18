// SkinItemSplitter.cpp : 实现文件
//

#include "stdafx.h"
#include "SkinItem.h"
#include "SkinItemSplitter.h"


// CSkinItemSplitter

IMPLEMENT_DYNCREATE(CSkinItemSplitter, CSkinItem)
CSkinItemSplitter::CSkinItemSplitter()
{
	EnableAutomation();
	
	// 为了使应用程序在 OLE 自动化对象处于活动状态时保持
	//	运行，构造函数调用 AfxOleLockApp。

	m_bTransparent = TRUE;

	AfxOleLockApp();
}

CSkinItemSplitter::~CSkinItemSplitter()
{
	// 为了在用 OLE 自动化创建所有对象后终止应用程序，
	// 	析构函数调用 AfxOleUnlockApp。
	
	AfxOleUnlockApp();
}


void CSkinItemSplitter::OnFinalRelease()
{
	// 释放了对自动化对象的最后一个引用后，将调用
	// OnFinalRelease。基类将自动
	// 删除该对象。在调用该基类之前，请添加您的
	// 对象所需的附加清除代码。

	CSkinItem::OnFinalRelease();
}


BEGIN_MESSAGE_MAP(CSkinItemSplitter, CSkinItem)
END_MESSAGE_MAP()


BEGIN_SKIN_DISPATCH_MAP(CSkinItemSplitter, CSkinItem)
	SKINBASE_DISP_PROPERTIES(CSkinItemSplitter)
	SKINITEM_DISP_PROPERTIES(CSkinItemSplitter)

END_SKIN_DISPATCH_MAP(CSkinItemSplitter)

// 注意: 我们添加 IID_ISkinItemSplitter 支持
//以支持来自 VBA 的类型安全绑定。此 IID 必须同附加到 .IDL 文件中的
//调度接口的 GUID 匹配。

// {A0BD99BE-1C40-46FB-AA24-652423F6DCF8}
static const IID IID_ISkinItemSplitter =
{ 0xA0BD99BE, 0x1C40, 0x46FB, { 0xAA, 0x24, 0x65, 0x24, 0x23, 0xF6, 0xDC, 0xF8 } };

BEGIN_INTERFACE_MAP(CSkinItemSplitter, CSkinItem)
	INTERFACE_PART(CSkinItemSplitter, IID_ISkinItemSplitter, Dispatch)
END_INTERFACE_MAP()

// {FA66128A-44E0-4192-A535-10C10DC26CA7}
IMPLEMENT_OLECREATE_FLAGS(CSkinItemSplitter, "Skin.SkinItemSplitter", afxRegApartmentThreading, 0xfa66128a, 0x44e0, 0x4192, 0xa5, 0x35, 0x10, 0xc1, 0xd, 0xc2, 0x6c, 0xa7)


// CSkinItemSplitter 消息处理程序
void CSkinItemSplitter::DrawBackground(CDC* pDC, LPRECT lpInvalidRect, CRgn* pInvalidRgn, BOOL bChildSkin)
{
	CSkinBase::DrawBackground(pDC, lpInvalidRect, pInvalidRgn);
}

void CSkinItemSplitter::DrawForeground(CDC* pDC, LPRECT lpInvalidRect, CRgn* pInvalidRgn)
{

}

void CSkinItemSplitter::DrawFrame(CDC* pDC, LPRECT lpInvalidRect, CRgn* pInvalidRgn)
{

}

void CSkinItemSplitter::Draw(CDC* pDC, LPRECT lpInvalidRect, CRgn* pInvalidRgn)
{
	DrawBackground(pDC, lpInvalidRect, pInvalidRgn);
	DrawForeground(pDC, lpInvalidRect, pInvalidRgn);
	DrawFrame(pDC, lpInvalidRect, pInvalidRgn);
}

BOOL CSkinItemSplitter::OnPaint(UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* lpResult)
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

    //复制dc到缓存dc
    memDC.BitBlt(0, 0, GetWidth(), GetHeight(), &dc, 0, 0, SRCCOPY);

	//在缓存上绘制
	Draw(&memDC, &rcInvalid, NULL);

	//复制缓存DC到显示DC
	dc.BitBlt(0, 0, GetWidth(), GetHeight(), &memDC, 0, 0, SRCCOPY);

	//恢复memDC
	memDC.SelectObject(pOldBitmap);
	memDC.DeleteDC();

	memBitmap.DeleteObject();

	return !GetselfOnPaint();
}
