// SkinGroup.cpp : 实现文件
//

#include "stdafx.h"
#include "SkinBase.h"
#include "SkinEngine.h"
#include "RenderEngine.h"
#include "SkinGroup.h"


// CSkinGroup

IMPLEMENT_DYNCREATE(CSkinGroup, CSkinBase)
CSkinGroup::CSkinGroup()
{
	EnableAutomation();
	
	// 为了使应用程序在 OLE 自动化对象处于活动状态时保持
	//	运行，构造函数调用 AfxOleLockApp。

	m_clrBoxLineColor = RGB(0, 0, 0);
	m_nBoxMargin = 6;
	m_nBoxRadius = 0;
    m_nTextMargin = 10;
	m_textStyle.SethorizAlign(_T("center"));
    m_bBackground = TRUE;

	AfxOleLockApp();
}

CSkinGroup::~CSkinGroup()
{
	// 为了在用 OLE 自动化创建所有对象后终止应用程序，
	// 	析构函数调用 AfxOleUnlockApp。
	
	AfxOleUnlockApp();
}


void CSkinGroup::OnFinalRelease()
{
	// 释放了对自动化对象的最后一个引用后，将调用
	// OnFinalRelease。基类将自动
	// 删除该对象。在调用该基类之前，请添加您的
	// 对象所需的附加清除代码。

	CSkinBase::OnFinalRelease();
}


BEGIN_MESSAGE_MAP(CSkinGroup, CSkinBase)
END_MESSAGE_MAP()


BEGIN_SKIN_DISPATCH_MAP(CSkinGroup, CSkinBase)
	SKINBASE_DISP_PROPERTIES(CSkinGroup)

	SKIN_DISP_PROPERTY(CSkinGroup, textColor, VT_I4)
	SKIN_DISP_PROPERTY(CSkinGroup, boxLineColor, VT_I4)
	SKIN_DISP_PROPERTY(CSkinGroup, boxMargin, VT_I4)
	SKIN_DISP_PROPERTY(CSkinGroup, boxRadius, VT_I4)
    SKIN_DISP_PROPERTY(CSkinGroup, textMargin, VT_I4)

END_SKIN_DISPATCH_MAP(CSkinGroup)

// 注意: 我们添加 IID_ISkinGroup 支持
//以支持来自 VBA 的类型安全绑定。此 IID 必须同附加到 .IDL 文件中的
//调度接口的 GUID 匹配。

// {EFA20886-FD4A-445F-A162-25E4DD84703A}
static const IID IID_ISkinGroup =
{ 0xEFA20886, 0xFD4A, 0x445F, { 0xA1, 0x62, 0x25, 0xE4, 0xDD, 0x84, 0x70, 0x3A } };

BEGIN_INTERFACE_MAP(CSkinGroup, CSkinBase)
	INTERFACE_PART(CSkinGroup, IID_ISkinGroup, Dispatch)
END_INTERFACE_MAP()

// {EC6EE506-FF95-438D-B955-97625F385099}
IMPLEMENT_OLECREATE_FLAGS(CSkinGroup, "Skin.SkinGroup", afxRegApartmentThreading, 0xec6ee506, 0xff95, 0x438d, 0xb9, 0x55, 0x97, 0x62, 0x5f, 0x38, 0x50, 0x99)


// CSkinGroup 消息处理程序
void CSkinGroup::Draw(CDC* pDC, LPRECT lpInvalidRect, CRgn* pInvalidRgn)
{
	CRect skinRect = GetSkinRect();

	CSize textSize;
	m_textStyle.GetTextSize(*pDC, m_strText, textSize);
	CRect textRect(0, 0, textSize.cx, textSize.cy);
	textRect.OffsetRect(skinRect.left, skinRect.top);
	textRect.OffsetRect((m_nBoxMargin + m_nTextMargin), (m_nBoxMargin - textRect.Height() / 2));
	textRect.InflateRect(4, 0);

	int iSaveDC = pDC->SaveDC();

	//绘制box
	CRgn textRgn;
	textRgn.CreateRectRgnIndirect(&textRect);
	pDC->SelectClipRgn(&textRgn, RGN_DIFF);		//剪切文字区域

	skinRect.DeflateRect(m_nBoxMargin, m_nBoxMargin);
	CRgn boxRgn;
	boxRgn.Attach(CRenderEngine::CreateRountRectRgn(skinRect.left, skinRect.top, skinRect.right + 1, skinRect.bottom + 1, m_nBoxRadius));
	CBrush boxBrush;
	LONG boxColor = m_clrBoxLineColor;
	if (GetColorTransform()) {
		GetEngine()->TransformColor(boxColor, boxColor);
	}
	boxBrush.CreateSolidBrush(boxColor);
	pDC->FrameRgn(&boxRgn, &boxBrush, 1, 1);

	//绘制文字
	pDC->SelectClipRgn(&textRgn, RGN_OR);
	m_textStyle.DrawText(*pDC, m_strText, &textRect);

	pDC->RestoreDC(iSaveDC);
}

LONG CSkinGroup::GettextColor()
{
	return m_textStyle.Getcolor();
}

void CSkinGroup::SettextColor(LONG newVal)
{
	m_textStyle.Setcolor(newVal);
}

LONG CSkinGroup::GetboxLineColor()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	return m_clrBoxLineColor;
}

void CSkinGroup::SetboxLineColor(LONG newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	if (m_clrBoxLineColor == newVal)
		return;

	m_clrBoxLineColor = newVal;
	InvalidateRect();
}

LONG CSkinGroup::GetboxMargin()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	return m_nBoxMargin;
}

void CSkinGroup::SetboxMargin(LONG newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	if (m_nBoxMargin == newVal)
		return;

	m_nBoxMargin = newVal;
	InvalidateRect();
}

LONG CSkinGroup::GetboxRadius()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	return m_nBoxRadius;
}

void CSkinGroup::SetboxRadius(LONG newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	if (m_nBoxRadius == newVal)
		return;

	m_nBoxRadius = newVal;
	InvalidateRect();
}

LONG CSkinGroup::GettextMargin()
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());
    return m_nTextMargin;
}

void CSkinGroup::SettextMargin(LONG newVal)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());
    if (m_nTextMargin == newVal)
        return;

    m_nTextMargin = newVal;
    InvalidateRect();
}
