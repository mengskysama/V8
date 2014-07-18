// SkinLibretto.cpp : 实现文件
//

#include "stdafx.h"
#include "ImageBase.h"
#include "SkinEngine.h"
#include "SkinItemDialog.h"
#include "SkinLibretto.h"
#include ".\skinlibretto.h"

#include "Types.h"

#include <vector>
#include <utility>

using namespace std;

#ifdef V8_JS_ENGINE_SUPPORT
using namespace v8;
#endif //V8_JS_ENGINE_SUPPORT

// CSkinLibretto

IMPLEMENT_DYNCREATE(CSkinLibretto, CSkinBase)

CSkinLibretto::CSkinLibretto()
{
	EnableAutomation();
	
	// 为了使应用程序在 OLE 自动化对象处于活动状态时保持
	//	运行，构造函数调用 AfxOleLockApp。
	m_bDisplay = true;

	//颜色
	m_textStyleNormal.Setcolor(RGB(28, 87, 130));
	m_textStyleHighlight.Setcolor(RGB(255, 0, 0));

	m_strBkImg = _T("");
	m_pBkImg = NULL;
	
	AfxOleLockApp();
}

CSkinLibretto::~CSkinLibretto()
{
	// 为了在用 OLE 自动化创建所有对象后终止应用程序，
	// 	析构函数调用 AfxOleUnlockApp。

	AfxOleUnlockApp();
}


void CSkinLibretto::OnFinalRelease()
{
	// 释放了对自动化对象的最后一个引用后，将调用
	// OnFinalRelease。基类将自动
	// 删除该对象。在调用该基类之前，请添加您的
	// 对象所需的附加清除代码。

	CCmdTarget::OnFinalRelease();
}


BEGIN_MESSAGE_MAP(CSkinLibretto, CSkinBase)
END_MESSAGE_MAP()


BEGIN_SKIN_DISPATCH_MAP(CSkinLibretto, CSkinBase)
	SKINBASE_DISP_PROPERTIES(CSkinLibretto)

	SKIN_DISP_PROPERTY(CSkinLibretto, color, VT_I4)
	SKIN_DISP_PROPERTY(CSkinLibretto, fontSize, VT_I4)
	SKIN_DISP_PROPERTY(CSkinLibretto, fontWeight, VT_I4)
	SKIN_DISP_PROPERTY(CSkinLibretto, italic, VT_BOOL)
	SKIN_DISP_PROPERTY(CSkinLibretto, underline, VT_BOOL)
	SKIN_DISP_PROPERTY(CSkinLibretto, strikeOut, VT_BOOL)
	SKIN_DISP_PROPERTY(CSkinLibretto, fontFamily, VT_BSTR)
	SKIN_DISP_PROPERTY(CSkinLibretto, hlFontSize, VT_I4)
	SKIN_DISP_PROPERTY(CSkinLibretto, hlFontWeight, VT_I4)
	SKIN_DISP_PROPERTY(CSkinLibretto, hlItalic, VT_BOOL)
	SKIN_DISP_PROPERTY(CSkinLibretto, hlUnderline, VT_BOOL)
	SKIN_DISP_PROPERTY(CSkinLibretto, hlStrikeOut, VT_BOOL)
	SKIN_DISP_PROPERTY(CSkinLibretto, hlFontFamily, VT_BSTR)
	SKIN_DISP_PROPERTY(CSkinLibretto, hlColor, VT_BSTR)
	SKIN_DISP_PROPERTY(CSkinLibretto, wordBreak, VT_BOOL)
	SKIN_DISP_PROPERTY(CSkinLibretto, singleLine, VT_BOOL)
	SKIN_DISP_PROPERTY(CSkinLibretto, horizAlign, VT_BSTR)
	SKIN_DISP_PROPERTY(CSkinLibretto, vertiAlign, VT_BSTR)
	SKIN_DISP_PROPERTY(CSkinLibretto, backgroundImg, VT_BSTR)

	SKIN_DISP_FUNCTION(CSkinLibretto, ClearLibretto, VT_EMPTY, VTS_NONE)
	SKIN_DISP_FUNCTION(CSkinLibretto, AppendLibretto, VT_EMPTY, VTS_BSTR)
	SKIN_DISP_FUNCTION(CSkinLibretto, SetCurLibretto, VT_EMPTY, VTS_I4)
	SKIN_DISP_FUNCTION(CSkinLibretto, EndDisplay, VT_EMPTY, VTS_NONE)

END_SKIN_DISPATCH_MAP(CSkinLibretto)

// 注意: 我们添加 IID_ISkinLibretto 支持
//以支持来自 VBA 的类型安全绑定。此 IID 必须同附加到 .IDL 文件中的
//调度接口的 GUID 匹配。

// {A931C754-9ECC-43F9-9407-72BB9D6B5CDE}
static const IID IID_ISkinLibretto =
{ 0xA931C754, 0x9ECC, 0x43F9, { 0x94, 0x7, 0x72, 0xBB, 0x9D, 0x6B, 0x5C, 0xDE } };

BEGIN_INTERFACE_MAP(CSkinLibretto, CSkinBase)
	INTERFACE_PART(CSkinLibretto, IID_ISkinLibretto, Dispatch)
END_INTERFACE_MAP()

// {189B75CF-483A-4D0E-BD51-8AB391206BC9}
IMPLEMENT_OLECREATE_FLAGS(CSkinLibretto, "Skin.SkinLibretto", afxRegApartmentThreading, 0x189b75cf, 0x483a, 0x4d0e, 0xbd, 0x51, 0x8a, 0xb3, 0x91, 0x20, 0x6b, 0xc9)


// CSkinLibretto 消息处理程序
BOOL CSkinLibretto::UpdateFont(HDC hDC)
{
	if (hDC == NULL)
		return FALSE;

	return m_textStyleNormal.UpdateFont(hDC);
}

BOOL CSkinLibretto::UpdateHlFont(HDC hDC)
{
	if (hDC == NULL)
		return FALSE;

	return m_textStyleHighlight.UpdateFont(hDC);
}

void CSkinLibretto::Draw(CDC* pDC, LPRECT lpInvalidRect, CRgn* pInvalidRgn)
{
	//绘制背景
	if (m_pBkImg != NULL){
		m_pBkImg->Draw(*pDC, &m_rcSkinRect, NULL, GetImageParam(), GetTransformedTransparentColor());
	}

	//不显示文字则返回
	if (! m_bDisplay)
		return;

	if (m_iCurLibre < 0 || m_iCurLibre >= (int)m_vecLibretto.size())
		return;

	//创建字体
	if (m_textStyleNormal.IsFontChanged() && ! UpdateFont(*pDC)){
		return;
	}
	if (m_textStyleHighlight.IsFontChanged() && ! UpdateHlFont(*pDC)){
		return;
	}
	
	//获得高度
	LONG width;
	LONG height = 0;
	LONG hlHeight = 0;
	if (! m_textStyleNormal.GetTextSize(*pDC, _T("test"), width, height) ||
		! m_textStyleHighlight.GetTextSize(*pDC, _T("test"), width, hlHeight))
		return;

	//绘制普通歌词
	const int lineNum = (m_rcSkinRect.Height() - hlHeight) / height;
	const int upLineNum = lineNum / 2;
	const int downLineNum = lineNum - upLineNum;

	CRect drawRect = m_rcSkinRect;
	drawRect.top += (upLineNum-1) * height;
	drawRect.bottom = drawRect.top + height;

	int count = 0;
	for (int i=m_iCurLibre-1; i>=0 && count<upLineNum; i--,count++){
		CRect rect = drawRect;
		m_textStyleNormal.DrawText(*pDC, m_vecLibretto[i].second, &rect);
		drawRect.top -= height;
		drawRect.bottom -= height;
	}

	drawRect = m_rcSkinRect;
	drawRect.top += upLineNum * height + hlHeight;
	drawRect.bottom = drawRect.top + height;
	count = 0;
	for (int i=m_iCurLibre+1; i<(int)m_vecLibretto.size() && count<downLineNum; i++,count++){
		CRect rect = drawRect;
		m_textStyleNormal.DrawText(*pDC, m_vecLibretto[i].second, &rect);
		drawRect.top += height;
		drawRect.bottom += height;
	}

	//绘制高亮歌词
	drawRect = m_rcSkinRect;
	drawRect.top += upLineNum * height;
	drawRect.bottom = drawRect.top + hlHeight;
	m_textStyleHighlight.DrawText(*pDC, m_vecLibretto[m_iCurLibre].second, &drawRect);
}

//////////////////////////////////////////////////////////////////////////
//设置字体的样式、大小
//////////////////////////////////////////////////////////////////////////
LONG CSkinLibretto::GetfontSize(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: 在此添加调度处理程序代码
	return m_textStyleNormal.GetfontSize();
}

void CSkinLibretto::SetfontSize(LONG newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: 在此添加调度处理程序代码
	m_textStyleNormal.SetfontSize(newVal);
}

LONG CSkinLibretto::GetfontWeight(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: 在此添加调度处理程序代码
	return m_textStyleNormal.GetfontWeight();
}

void CSkinLibretto::SetfontWeight(LONG newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: 在此添加调度处理程序代码
	m_textStyleNormal.SetfontWeight(newVal);
}

VARIANT_BOOL CSkinLibretto::Getitalic(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: 在此添加调度处理程序代码
	return m_textStyleNormal.Getitalic();
}

void CSkinLibretto::Setitalic(VARIANT_BOOL newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: 在此添加属性处理程序代码
	m_textStyleNormal.Setitalic(newVal);
}

VARIANT_BOOL CSkinLibretto::Getunderline(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: 在此添加调度处理程序代码
	return m_textStyleNormal.Getunderline();
}

void CSkinLibretto::Setunderline(VARIANT_BOOL newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: 在此添加属性处理程序代码
	m_textStyleNormal.Setunderline(newVal);
}

VARIANT_BOOL CSkinLibretto::GetstrikeOut(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: 在此添加调度处理程序代码
	return m_textStyleNormal.GetstrikeOut();
}

void CSkinLibretto::SetstrikeOut(VARIANT_BOOL newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: 在此添加属性处理程序代码
	m_textStyleNormal.SetstrikeOut(newVal);
}

BSTR CSkinLibretto::GetfontFamily(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: 在此添加调度处理程序代码
	return m_textStyleNormal.GetfontFamily();
}

void CSkinLibretto::SetfontFamily(LPCTSTR newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: 在此添加调度处理程序代码
	m_textStyleNormal.SetfontFamily(newVal);
}

//////////////////////////////////////////////////////////////////////////
//高亮歌词
//////////////////////////////////////////////////////////////////////////
LONG CSkinLibretto::GethlFontSize(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: 在此添加调度处理程序代码
	return m_textStyleHighlight.GetfontSize();
}

void CSkinLibretto::SethlFontSize(LONG newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: 在此添加调度处理程序代码
	m_textStyleHighlight.SetfontSize(newVal);
}

LONG CSkinLibretto::GethlFontWeight(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: 在此添加调度处理程序代码
	return m_textStyleHighlight.GetfontWeight();
}

void CSkinLibretto::SethlFontWeight(LONG newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: 在此添加调度处理程序代码
	m_textStyleHighlight.SetfontWeight(newVal);
}

VARIANT_BOOL CSkinLibretto::GethlItalic(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: 在此添加调度处理程序代码
	return m_textStyleHighlight.Getitalic();
}

void CSkinLibretto::SethlItalic(VARIANT_BOOL newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: 在此添加属性处理程序代码
	m_textStyleHighlight.Setitalic(newVal);
}

VARIANT_BOOL CSkinLibretto::GethlUnderline(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: 在此添加调度处理程序代码
	return m_textStyleHighlight.Getunderline();
}

void CSkinLibretto::SethlUnderline(VARIANT_BOOL newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: 在此添加属性处理程序代码
	m_textStyleHighlight.Setunderline(newVal);
}

VARIANT_BOOL CSkinLibretto::GethlStrikeOut(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: 在此添加调度处理程序代码
	return m_textStyleHighlight.GetstrikeOut();
}

void CSkinLibretto::SethlStrikeOut(VARIANT_BOOL newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: 在此添加属性处理程序代码
	m_textStyleHighlight.SetstrikeOut(newVal);
}

BSTR CSkinLibretto::GethlFontFamily(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: 在此添加调度处理程序代码
	return m_textStyleHighlight.GetfontFamily();
}

void CSkinLibretto::SethlFontFamily(LPCTSTR newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: 在此添加调度处理程序代码
	m_textStyleHighlight.SetfontFamily(newVal);
}

//////////////////////////////////////////////////////////////////////////
//设置颜色
//////////////////////////////////////////////////////////////////////////
LONG CSkinLibretto::Getcolor(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: 在此添加调度处理程序代码
	return m_textStyleNormal.Getcolor();
}

void CSkinLibretto::Setcolor(LONG newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: 在此添加属性处理程序代码
	m_textStyleNormal.Setcolor(newVal);
}

LONG CSkinLibretto::GethlColor(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: 在此添加调度处理程序代码
	return m_textStyleHighlight.Getcolor();
}

void CSkinLibretto::SethlColor(LONG newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: 在此添加属性处理程序代码
	m_textStyleHighlight.Setcolor(newVal);
}

//////////////////////////////////////////////////////////////////////////
//设置文字排布格式
//////////////////////////////////////////////////////////////////////////

VARIANT_BOOL CSkinLibretto::GetwordBreak(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: 在此添加调度处理程序代码
	return m_textStyleNormal.GetwordBreak();
}

void CSkinLibretto::SetwordBreak(VARIANT_BOOL newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: 在此添加属性处理程序代码
	m_textStyleNormal.SetwordBreak(newVal);
}

VARIANT_BOOL CSkinLibretto::GetsingleLine(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: 在此添加调度处理程序代码
	return m_textStyleNormal.GetsingleLine();
}

void CSkinLibretto::SetsingleLine(VARIANT_BOOL newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: 在此添加属性处理程序代码
	m_textStyleNormal.SetsingleLine(newVal);
}

BSTR CSkinLibretto::GethorizAlign(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: 在此添加调度处理程序代码
	return m_textStyleNormal.GethorizAlign();
}

void CSkinLibretto::SethorizAlign(LPCTSTR newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: 在此添加属性处理程序代码
	m_textStyleNormal.SethorizAlign(newVal);
}

BSTR CSkinLibretto::GetvertiAlign(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: 在此添加调度处理程序代码
	return m_textStyleNormal.GetvertiAlign();
}

void CSkinLibretto::SetvertiAlign(LPCTSTR newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: 在此添加属性处理程序代码
	m_textStyleNormal.SetvertiAlign(newVal);
}

//////////////////////////////////////////////////////////////////////////
//背景
//////////////////////////////////////////////////////////////////////////
BSTR CSkinLibretto::GetbackgroundImg(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: 在此添加调度处理程序代码
	return m_strBkImg.AllocSysString();
}

void CSkinLibretto::SetbackgroundImg(LPCTSTR newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: 在此添加属性处理程序代码
    SetImageHelper(m_pBkImg, m_strBkImg, newVal);
}

//////////////////////////////////////////////////////////////////////////
//Dispatch方法
//////////////////////////////////////////////////////////////////////////
void CSkinLibretto::ClearLibretto(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: 在此添加调度处理程序代码
	m_vecLibretto.clear();
	m_iCurLibre = -1;
	InvalidateRect();
}

void CSkinLibretto::AppendLibretto(LPCTSTR libre)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: 在此添加调度处理程序代码
	CString strLibre = libre;
	m_vecLibretto.push_back(pair<int, CString>(-1, strLibre));
}

void CSkinLibretto::SetCurLibretto(LONG index)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: 在此添加调度处理程序代码
	if (index < 0 || index >= (int)m_vecLibretto.size())
		return;
	m_bDisplay = true;
	m_iCurLibre = index;
	InvalidateRect(NULL, FALSE);
}

void CSkinLibretto::EndDisplay(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: 在此添加调度处理程序代码
	m_bDisplay = false;
	InvalidateRect();
}

#ifdef V8_JS_ENGINE_SUPPORT
Handle<Value> CSkinLibretto::V8ClearLibretto(const Arguments& args)
{
    HandleScope handleScope;

    ClearLibretto();

    Handle<Value> result = v8::Undefined();

    return handleScope.Close(result);
}

Handle<Value> CSkinLibretto::V8AppendLibretto(const Arguments& args)
{
    HandleScope handleScope;

    AppendLibretto(ValueToCString(Handle<String>::Cast(args[0])));

    Handle<Value> result = v8::Undefined();

    return handleScope.Close(result);
}

Handle<Value> CSkinLibretto::V8SetCurLibretto(const Arguments& args)
{
    HandleScope handleScope;

    SetCurLibretto(Handle<Integer>::Cast(args[0])->Int32Value());

    Handle<Value> result = v8::Undefined();

    return handleScope.Close(result);
}

Handle<Value> CSkinLibretto::V8EndDisplay(const Arguments& args)
{
    HandleScope handleScope;

    EndDisplay();

    Handle<Value> result = v8::Undefined();

    return handleScope.Close(result);
}

Handle<Value> CSkinLibretto::V8InternalCallMethod(const CString& funcName, const Arguments& args)
{
    if (funcName.CompareNoCase(_T("ClearLibretto")) == 0) {
        return V8ClearLibretto(args);
    } else if (funcName.CompareNoCase(_T("AppendLibretto")) == 0) {
        return V8AppendLibretto(args);
    } else if (funcName.CompareNoCase(_T("SetCurLibretto")) == 0) {
        return V8SetCurLibretto(args);
    } else if (funcName.CompareNoCase(_T("EndDisplay")) == 0) {
        return V8EndDisplay(args);
    } 

    return CSkinBase::V8InternalCallMethod(funcName, args);
}
#endif //V8_JS_ENGINE_SUPPORT
