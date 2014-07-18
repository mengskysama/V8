//SkinItemDialog.cpp : implementation file
//

#include "stdafx.h"
#include "ImageBase.h"
#include "SkinBase.h"
#include "SkinProxy.h"
#include "SkinItem.h"
#include "SkinItemCombo.h"
#include "SkinButton.h"
#include "SkinImage.h"
#include "SkinProgress.h"
#include "SkinText.h"
#include "SkinEngine.h"
#include "SkinItemDialog.h"

#include <list>
#include <map>
#include <set>
#include <algorithm>

#include "RenderEngine.h"

using namespace std;

#ifdef V8_JS_ENGINE_SUPPORT
using namespace v8;
#endif //V8_JS_ENGINE_SUPPORT

// CSkinItemDialog

IMPLEMENT_DYNCREATE(CSkinItemDialog, CSkinItem)

CSkinItemDialog::CSkinItemDialog()
{
	EnableAutomation();

	m_nTitleHeight = 0;
	m_bTitleInactive = FALSE;
	m_bNcActivate = FALSE;
	m_pTitleMaskImage = NULL;

	m_nSizeBorder = 4;
	m_nSizeCorner = 10;

    m_clrBkColorDisabled = -1;

	m_nBorderWidth = 0;
	m_clrBorderColor = -1;
	m_strBorderHorizImage = _T("");
	m_strBorderVertiImage = _T("");
	m_pBorderHorizImage = NULL;
	m_pBorderVertiImage = NULL;

	m_bSkin = FALSE;

	m_nMinWidth = -1;
	m_nMaxWidth = -1;
	m_nMinHeight = -1;
	m_nMaxHeight = -1;

    m_bRemoveNC = TRUE;
	m_bResizable = FALSE;
    m_bMovable = TRUE;
	m_bMinimizable = FALSE;
	m_bMaximizable = FALSE;
    m_uBkHitTest = HTCLIENT;

	m_bInitCenterWindow = TRUE;

    m_clrBkColorNormal = -1;

	m_bSelfOnPaint = FALSE;
	m_bSelfOnSize = TRUE;

	m_bUseDrawBuffer = TRUE;

    m_bEraseBk = FALSE;
    m_bEraseBkOnce = FALSE;
    m_lEraseBkCount = 0;

    m_bSelfOnEraseBkgnd = FALSE;
}

CSkinItemDialog::~CSkinItemDialog()
{

}

void CSkinItemDialog::OnFinalRelease()
{
	// When the last reference for an automation object is released
	// OnFinalRelease is called.  The base class will automatically
	// deletes the object.  Add additional cleanup required for your
	// object before calling the base class.

	CCmdTarget::OnFinalRelease();
}

//////////////////////////////////////////////////////////////////////////
//消息和Dispatch的映射表
//////////////////////////////////////////////////////////////////////////
BEGIN_MESSAGE_MAP(CSkinItemDialog, CSkinItem)
END_MESSAGE_MAP()

BEGIN_SKIN_DISPATCH_MAP(CSkinItemDialog, CSkinItem)
	SKINBASE_DISP_PROPERTIES(CSkinItemDialog)
	SKINITEM_DISP_PROPERTIES(CSkinItemDialog)

	SKIN_DISP_PROPERTY(CSkinItemDialog, resizable, VT_BOOL)
	SKIN_DISP_PROPERTY(CSkinItemDialog, movable, VT_BOOL)
	SKIN_DISP_PROPERTY(CSkinItemDialog, minimizable, VT_BOOL)
	SKIN_DISP_PROPERTY(CSkinItemDialog, maximizable, VT_BOOL)
	SKIN_DISP_PROPERTY(CSkinItemDialog, initCenterWindow, VT_BOOL)
	SKIN_DISP_PROPERTY(CSkinItemDialog, minWidth, VT_I4)
	SKIN_DISP_PROPERTY(CSkinItemDialog, minHeight, VT_I4)
	SKIN_DISP_PROPERTY(CSkinItemDialog, maxWidth, VT_I4)
	SKIN_DISP_PROPERTY(CSkinItemDialog, maxHeight, VT_I4)
	SKIN_DISP_PROPERTY(CSkinItemDialog, titleHeight, VT_I4)
	SKIN_DISP_PROPERTY(CSkinItemDialog, max, VT_BOOL)
	SKIN_DISP_PROPERTY(CSkinItemDialog, sizeBorder, VT_I4)
	SKIN_DISP_PROPERTY(CSkinItemDialog, sizeCorner, VT_I4)
	SKIN_DISP_PROPERTY(CSkinItemDialog, border, VT_I4)
	SKIN_DISP_PROPERTY(CSkinItemDialog, titleInactive, VT_BOOL)
    SKIN_DISP_PROPERTY(CSkinItemDialog, titleMaskImage, VT_BSTR)
    SKIN_DISP_PROPERTY(CSkinItemDialog, removeNC, VT_BOOL)
    SKIN_DISP_PROPERTY(CSkinItemDialog, bkHitTest, VT_BSTR)
    SKIN_DISP_PROPERTY(CSkinItemDialog, sysTickCount, VT_UI4)
    SKIN_DISP_PROPERTY(CSkinItemDialog, eraseBk, VT_BOOL)
    SKIN_DISP_PROPERTY(CSkinItemDialog, eraseBkOnce, VT_BOOL)
    SKIN_DISP_FUNCTION(CSkinItemDialog, BeginPaint, VT_EMPTY, VTS_NONE )
	SKIN_DISP_FUNCTION(CSkinItemDialog, EndPaint, VT_EMPTY, VTS_NONE )

END_SKIN_DISPATCH_MAP(CSkinItemDialog)

// Note: we add support for IID_ISkinItemDialog to support typesafe binding
//  from VBA.  This IID must match the GUID that is attached to the 
//  dispinterface in the .IDL file.

// {FDC27E47-ADE9-42BE-8CE9-19DDE0831D37}
static const IID IID_ISkinItemDialog =
{ 0xFDC27E47, 0xADE9, 0x42BE, { 0x8C, 0xE9, 0x19, 0xDD, 0xE0, 0x83, 0x1D, 0x37 } };

BEGIN_INTERFACE_MAP(CSkinItemDialog, CSkinItem)
	INTERFACE_PART(CSkinItemDialog, IID_ISkinItemDialog, Dispatch)
END_INTERFACE_MAP()

//////////////////////////////////////////////////////////////////////////
//Dispatch方法
//////////////////////////////////////////////////////////////////////////
void CSkinItemDialog::BeginPaint()
{

}

void CSkinItemDialog::EndPaint()
{

}

#ifdef V8_JS_ENGINE_SUPPORT
//v8实现
Handle<Value> CSkinItemDialog::V8BeginPaint(const Arguments& args)
{
    HandleScope handleScope;

    Handle<Value> result = v8::Undefined();

    return handleScope.Close(result);
}

Handle<Value> CSkinItemDialog::V8EndPaint(const Arguments& args)
{
    HandleScope handleScope;

    Handle<Value> result = v8::Undefined();

    return handleScope.Close(result);
}

Handle<Value> CSkinItemDialog::V8InternalCallMethod(const CString& funcName, const Arguments& args)
{
    if (funcName.CompareNoCase(_T("BeginPaint")) == 0) {
        return V8BeginPaint(args);
    } else if (funcName.CompareNoCase(_T("EndPaint")) == 0) {
        return V8EndPaint(args);
    } 

    return CSkinItem::V8InternalCallMethod(funcName, args);
}
#endif //V8_JS_ENGINE_SUPPORT

//////////////////////////////////////////////////////////////////////////
//Dispatch属性
//////////////////////////////////////////////////////////////////////////
void CSkinItemDialog::SetName(LPCTSTR newVal)
{
	//验证没有命名过
	ASSERT(m_strName.IsEmpty());
	if (! m_strName.IsEmpty())
		return;

	//验证新名字是否为空
	CString strName = newVal;
	ASSERT(! strName.IsEmpty());
	if (strName.IsEmpty())
		return;

	m_strName = newVal;	//命名区分大小写
}

VARIANT_BOOL CSkinItemDialog::Getmax(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	return (::IsZoomed(m_hWnd)) ? (VARIANT_TRUE) : (VARIANT_FALSE);
}

void CSkinItemDialog::Setmax(VARIANT_BOOL newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	if (newVal != VARIANT_FALSE){
		::ShowWindow(m_hWnd, SW_MAXIMIZE);
	}else {
		::ShowWindow(m_hWnd, SW_RESTORE);
	}
}

LONG CSkinItemDialog::GettitleHeight(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	return m_nTitleHeight;
}

void CSkinItemDialog::SettitleHeight(long newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	m_nTitleHeight = newVal;
}

VARIANT_BOOL CSkinItemDialog::GettitleInactive(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	return (m_bTitleInactive) ? (VARIANT_TRUE) : (VARIANT_FALSE);
}

void CSkinItemDialog::SettitleInactive(VARIANT_BOOL newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	newVal = (newVal != VARIANT_FALSE);

	if (m_bTitleInactive == newVal)
		return;

	m_bTitleInactive = newVal;
	InvalidateRect();
}

BSTR CSkinItemDialog::GettitleMaskImage(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	return m_strTitleMaskImage.AllocSysString();
}

void CSkinItemDialog::SettitleMaskImage(LPCTSTR newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
    SetImageHelper(m_pTitleMaskImage, m_strTitleMaskImage, newVal);
}

LONG CSkinItemDialog::GetmaxHeight(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	return m_nMaxHeight;
}

void CSkinItemDialog::SetmaxHeight(long newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	m_nMaxHeight = newVal;
}

LONG CSkinItemDialog::GetmaxWidth(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	return m_nMaxWidth;
}

void CSkinItemDialog::SetmaxWidth(long newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	m_nMaxWidth = newVal;
}

LONG CSkinItemDialog::GetminHeight(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	return m_nMinHeight;
}

void CSkinItemDialog::SetminHeight(long newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	m_nMinHeight = newVal;
}

LONG CSkinItemDialog::GetminWidth(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	return m_nMinWidth;
}

void CSkinItemDialog::SetminWidth(long newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	m_nMinWidth = newVal;
}

LONG CSkinItemDialog::GetsizeBorder(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	return m_nSizeBorder;
}

void CSkinItemDialog::SetsizeBorder(LONG newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	if (m_nSizeBorder == newVal)
		return;

	m_nSizeBorder = newVal;
}

LONG CSkinItemDialog::GetsizeCorner(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	return m_nSizeCorner;
}

void CSkinItemDialog::SetsizeCorner(LONG newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	if (m_nSizeCorner == newVal)
		return;

	m_nSizeCorner = newVal;
}

LONG CSkinItemDialog::Getborder(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	return m_nBorderWidth;
}

void CSkinItemDialog::Setborder(LONG newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	if (m_nBorderWidth == newVal)
		return;

	m_nBorderWidth = newVal;
	InvalidateRect();
}

LONG CSkinItemDialog::GetborderColor(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	return m_clrBorderColor;
}

void CSkinItemDialog::SetborderColor(LONG newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	if (m_clrBorderColor == newVal)
		return;

	m_clrBorderColor = newVal;
	InvalidateRect();
}

BSTR CSkinItemDialog::GetborderHorizImage(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	return m_strBorderHorizImage.AllocSysString();
}

void CSkinItemDialog::SetborderHorizImage(LPCTSTR newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
    SetImageHelper(m_pBorderHorizImage, m_strBorderHorizImage, newVal);
}

BSTR CSkinItemDialog::GetborderVertiImage(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	return m_strBorderVertiImage.AllocSysString();
}

void CSkinItemDialog::SetborderVertiImage(LPCTSTR newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
    SetImageHelper(m_pBorderVertiImage, m_strBorderVertiImage, newVal);
}

VARIANT_BOOL CSkinItemDialog::Getresizable(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	
	return m_bResizable ? (VARIANT_TRUE) : (VARIANT_FALSE);
}

void CSkinItemDialog::Setresizable(VARIANT_BOOL newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	newVal = (newVal != VARIANT_FALSE);

	if (m_bResizable == newVal)
		return;

	m_bResizable = newVal;
	if (! ::IsWindow(m_hWnd))
		return;

	SetSkinItemLongHelper(GWL_STYLE, WS_SIZEBOX, m_bResizable);
}

VARIANT_BOOL CSkinItemDialog::Getmovable(void)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());

    return m_bMovable ? (VARIANT_TRUE) : (VARIANT_FALSE);
}

void CSkinItemDialog::Setmovable(VARIANT_BOOL newVal)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());

    newVal = (newVal != VARIANT_FALSE);

    if (m_bMovable == newVal)
        return;

    m_bMovable = newVal;
}

VARIANT_BOOL CSkinItemDialog::Getminimizable(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	return m_bMinimizable ? (VARIANT_TRUE) : (VARIANT_FALSE);
}

void CSkinItemDialog::Setminimizable(VARIANT_BOOL newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	newVal = (newVal != VARIANT_FALSE);

	if (m_bMinimizable == newVal)
		return;

	m_bMinimizable = newVal;
	if (! ::IsWindow(m_hWnd))
		return;

	SetSkinItemLongHelper(GWL_STYLE, WS_MINIMIZEBOX, m_bMinimizable);
}

VARIANT_BOOL CSkinItemDialog::Getmaximizable(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	return m_bMaximizable ? (VARIANT_TRUE) : (VARIANT_FALSE);
}

void CSkinItemDialog::Setmaximizable(VARIANT_BOOL newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	newVal = (newVal != VARIANT_FALSE);

	if (m_bMaximizable == newVal)
		return;

	m_bMaximizable = newVal;
	if (! ::IsWindow(m_hWnd))
		return;

	SetSkinItemLongHelper(GWL_STYLE, WS_MAXIMIZEBOX, m_bMaximizable);
}

VARIANT_BOOL CSkinItemDialog::GetinitCenterWindow(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	return m_bInitCenterWindow ? (VARIANT_TRUE) : (VARIANT_FALSE);
}

void CSkinItemDialog::SetinitCenterWindow(VARIANT_BOOL newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	newVal = (newVal != VARIANT_FALSE);
	if (m_bInitCenterWindow == newVal)
		return;

	m_bInitCenterWindow = newVal;
}

VARIANT_BOOL CSkinItemDialog::GetremoveNC(void)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());

    return m_bRemoveNC ? (VARIANT_TRUE) : (VARIANT_FALSE);
}

void CSkinItemDialog::SetremoveNC(VARIANT_BOOL newVal)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());

    newVal = (newVal != VARIANT_FALSE);
    if (m_bRemoveNC == newVal)
        return;

    m_bRemoveNC = newVal;
}

BSTR CSkinItemDialog::GetbkHitTest(void)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());
    CString str;
    switch (m_uBkHitTest)
    {
    case HTCLIENT:
        str = _T("client");
        break;
    case HTCAPTION:
        str = _T("caption");
        break;
    default:
        str = _T("client");
        break;
    }

    return str.AllocSysString();
}

void CSkinItemDialog::SetbkHitTest(LPCTSTR newVal)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());
    CString str = newVal;
    if (str.CompareNoCase(_T("client")) == 0) {
        m_uBkHitTest = HTCLIENT;
    } else if (str.CompareNoCase(_T("caption")) == 0) {
        m_uBkHitTest = HTCAPTION;
    } else {
        m_uBkHitTest = HTCLIENT;
    }
}

ULONG CSkinItemDialog::GetsysTickCount()
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());

    return ::GetTickCount();
}

void CSkinItemDialog::SetsysTickCount(ULONG time)
{
    
}

VARIANT_BOOL CSkinItemDialog::GeteraseBk(void)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());

    return m_bEraseBk ? (VARIANT_TRUE) : (VARIANT_FALSE);
}

void CSkinItemDialog::SeteraseBk(VARIANT_BOOL newVal)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());

    newVal = (newVal != VARIANT_FALSE);
    if (m_bEraseBk == newVal)
        return;

    m_bEraseBk = newVal;
}

VARIANT_BOOL CSkinItemDialog::GeteraseBkOnce(void)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());

    return m_bEraseBkOnce ? (VARIANT_TRUE) : (VARIANT_FALSE);
}

void CSkinItemDialog::SeteraseBkOnce(VARIANT_BOOL newVal)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());

    newVal = (newVal != VARIANT_FALSE);
    if (m_bEraseBkOnce == newVal)
        return;

    m_bEraseBkOnce = newVal;
    //计数清零
    if (m_bEraseBkOnce == TRUE)
        m_lEraseBkCount = 0;
}

BOOL CSkinItemDialog::AttachHwnd(HWND hWnd)
{
	//验证是否是窗口
	ASSERT(::IsWindow(hWnd));
	if (!::IsWindow(hWnd))
		return FALSE;

#ifdef CUSTOM_SCROLLBAR_SUPPORT
    if (m_bSkinScrollBar) {
        HWND scrollBarHwnd = hWnd;
        if (m_lSkinScrollBarChild != -1) {
            LONG generation = m_lSkinScrollBarChild;
            while (generation > 0) {
                scrollBarHwnd = ::GetWindow(scrollBarHwnd, GW_CHILD);
                generation --;
            }
        } else {
            HWND hChild = scrollBarHwnd;
            while (hChild) {
                hChild = ::GetWindow(hChild, GW_CHILD);
                if (hChild)
                    scrollBarHwnd = hChild;
            }
        }
        InitializeCoolSB(scrollBarHwnd);
        CoolSB_SetStyle(scrollBarHwnd, SB_VERT, CSBS_HOTTRACKED);
        CoolSB_SetStyle(scrollBarHwnd, SB_HORZ, CSBS_HOTTRACKED);
        CoolSB_SetSize(scrollBarHwnd, SB_VERT, m_lSkinScrollBarHeight, m_lSkinScrollBarWidth);
        CoolSB_SetSize(scrollBarHwnd, SB_HORZ, m_lSkinScrollBarHeight, m_lSkinScrollBarWidth);
        CoolSB_SetMinThumbSize(scrollBarHwnd, SB_VERT, 20);
        CoolSB_SetMinThumbSize(scrollBarHwnd, SB_HORZ, 20);
    }
#endif

	//附着窗口句柄
	if(! Attach(hWnd))
		return FALSE;


#pragma warning (push)
#pragma warning (disable:4311)
    LONG lBrush = (LONG)((HBRUSH)::GetStockObject(WHITE_BRUSH));
#pragma warning (pop)
    ::SetClassLong(hWnd, GCL_HBRBACKGROUND, lBrush);

    if (m_bRemoveNC) {
	    //去除Caption属性
	    SetSkinItemLongHelper(GWL_STYLE, WS_CAPTION, FALSE);
    }

	//更新皮肤属性
    UpdateSkinItemProperty();

	return TRUE;
}

BOOL CSkinItemDialog::OnDupSkinItem(HWND hWnd, CSkinItem* pBase)
{
	if (pBase == NULL)
		return FALSE;

	//查找窗口句柄重复的皮肤元素
	list<CSkinBase*>::iterator iter = m_listSkinBase.begin(),
		last = m_listSkinBase.end();
	CSkinBase* pTempBase = NULL;
	for (; iter!=last; iter++){
		pTempBase = (*iter);
        if (pTempBase == NULL
            || !pTempBase->IsKindOf(RUNTIME_CLASS(CSkinItem)))
            continue;
		if ( ((CSkinItem*)pTempBase) == pBase)
			break;
	}
	ASSERT(iter != m_listSkinBase.end());
	if (iter == m_listSkinBase.end())
		return FALSE;

	//获得先附着此窗口句柄的皮肤元素
	CSkinItem* pItem = CSkinItem::FromHandlePermanent(hWnd);
	ASSERT(pItem != NULL);
	if (! pItem->IsKindOf(RUNTIME_CLASS(CSkinItemDialog)))
		return FALSE;

	//复制部分皮肤属性
	CSkinItemDialog* pWindow = (CSkinItemDialog*)pItem;

    //设置窗口位置
	//pWindow->Setleft(pBase->Getleft());
	//pWindow->Settop(pBase->Gettop());
	//pWindow->Setwidth(pBase->Getwidth());
	//pWindow->Setheight(pBase->Getheight());
    CRect wndRect;
    wndRect.left = pBase->Getleft();
    wndRect.top = pBase->Gettop();
    wndRect.right = wndRect.left + pBase->Getwidth();
    wndRect.bottom = wndRect.top + pBase->Getheight();
    pWindow->MoveWindow(&wndRect, FALSE);

	pWindow->Setvisible(pBase->Getvisible());
	pWindow->Setdisabled(pBase->Getdisabled());
	BSTR bstrName = pBase->Getname();
	CString strName(bstrName);
	::SysFreeString(bstrName);
	pWindow->Setname(strName);

	//替换窗口句柄重复的皮肤元素
	ReplaceSubSkin(pBase, pWindow);

    if (! GetEngine()->CloseSkinJSEngine(this)) {
        USES_CONVERSION;
        TRACE(("去掉窗口句柄重复的皮肤元素时，关闭脚本引擎出错:%s，重复的皮肤元素名:%s\n"), 
            T2A(GetScriptFileName()), T2A(pBase->GetSkinName()));
        ASSERT(0 && _T("去掉窗口句柄重复的皮肤元素时，关闭脚本引擎出错"));
        return FALSE;
    }

    if (! GetEngine()->CreateSkinJSEngine(this)) {
        USES_CONVERSION;
        TRACE(("去掉窗口句柄重复的皮肤元素时，重启动脚本出错:%s，重复的皮肤元素名:%s\n"), 
            T2A(GetScriptFileName()), T2A(pBase->GetSkinName()));
        ASSERT(0 && _T("去掉窗口句柄重复的皮肤元素时，重启动脚本出错"));
        return FALSE;
    };

	//删除原来元素
	GetEngine()->DestroySkin(pBase);

	return TRUE;
}

BOOL CSkinItemDialog::GetTitleRect(RECT& rect)
{
	memset(&rect, 0, sizeof(rect));
	if (m_nTitleHeight > 0) {
		rect.left = 0;
		rect.top = 0;
		rect.right = GetWidth();
		rect.bottom = m_nTitleHeight;
	}
	return TRUE;
}

//更新皮肤属性，在皮肤新附着窗口时调用
BOOL CSkinItemDialog::UpdateSkinItemProperty()
{
	if (! CSkinItem::UpdateSkinItemProperty())
		return FALSE;

	//更新属性
	SetSkinItemLongHelper(GWL_STYLE, WS_SIZEBOX, m_bResizable);

	return TRUE;
}

void CSkinItemDialog::DrawBackground(CDC* pDC, LPRECT lpInvalidRect, CRgn* pInvalidRgn, BOOL bChildSkin)
{
	//保存DC
	int iAllSavedDC = pDC->SaveDC();

	//绘制父窗口背景
	if (GetTransparent()) {
		DrawParentBackground(pDC, lpInvalidRect, pInvalidRgn);
	}

	//设置剪切区域
	CRect paintRect;	//绘制矩形
	if (lpInvalidRect != NULL){
		paintRect = *lpInvalidRect;
	} else {
		paintRect = GetSkinAbsRect();
	}
	CRgn clipRgn;
	clipRgn.CreateRectRgnIndirect(&paintRect);

	if (pInvalidRgn != NULL) {
		clipRgn.CombineRgn(&clipRgn, pInvalidRgn, RGN_AND);
	}

	//填充背景
	if (GetbkColorNormal() != -1){
		CBrush brush;
		brush.CreateSolidBrush(GetbkColorNormal());
		pDC->FillRgn(&clipRgn, &brush);
	}

	if (! bChildSkin) {

		//所有可见的背景元素
		CRgn allVisiBackRgn;
		allVisiBackRgn.Attach(GetAllSubSkinRgn(FILTER_VISIBLE | FILTER_BACKGROUND, FILTER_VISIBLE | FILTER_BACKGROUND));

		//测试区域
		//CBrush brush1;
		//brush1.CreateSolidBrush(RGB(255, 0, 0));
		//pDC->FillRgn(&allVisiBackRgn, &brush1);

		//所有可见的非背景元素
		CRgn allInvisiBackRgn;
		allInvisiBackRgn.Attach(GetAllSubSkinRgn(FILTER_VISIBLE | FILTER_SKINITEM, FILTER_VISIBLE | FILTER_SKINITEM));

		//测试区域
		//CBrush brush2;
		//brush2.CreateSolidBrush(RGB(0, 255, 0));
		//pDC->FillRgn(&allInvisiBackRgn, &brush2);

		CRgn visiBackRgn;
		visiBackRgn.CreateRectRgn(0, 0, 0, 0);
		visiBackRgn.CombineRgn(&allVisiBackRgn, &allInvisiBackRgn, RGN_DIFF);

		//测试区域
		//CBrush brush3;
		//brush3.CreateSolidBrush(RGB(0, 0, 255));
		//pDC->FillRgn(&visiBackRgn, &brush3);

		//设置剪切区域
		pDC->SelectClipRgn(&visiBackRgn, RGN_AND);
	}

	//绘制非SkinItem的皮肤元素
	list<CSkinBase*>::const_iterator iter = m_listSkinBase.begin(), last = m_listSkinBase.end();
	for (; iter!=last; iter++) {

		CSkinBase* pBase = *iter;

		//不绘制不可见元素
		if (! pBase->GetVisible())
			continue;

		//不绘制非背景元素
		if (! pBase->GetBackground())
			continue;

		//皮肤矩形区域求交
		CRect skinRect = pBase->GetSkinRect();
		CRect rect;
		rect.IntersectRect(&paintRect, &skinRect);

		if (rect.IsRectEmpty() || 
			pBase->IsFiltered(FILTER_SKINITEM, FILTER_SKINITEM))
			continue;

		//绘制skin
		int iSkinSavedDC = pDC->SaveDC();
		CRgn skinRgn;
		skinRgn.Attach(pBase->GetSkinRgn());
		if (! bChildSkin)
			pDC->SelectClipRgn(&skinRgn, RGN_AND);
		pBase->Draw(pDC);
		pDC->RestoreDC(iSkinSavedDC);
	}

	pDC->RestoreDC(iAllSavedDC);
}

void CSkinItemDialog::DrawForeground(CDC* pDC, LPRECT lpInvalidRect, CRgn* pInvalidRgn)
{
	//保存DC
	int iAllSavedDC = pDC->SaveDC();

	//设置剪切区域
	CRect paintRect;	//绘制矩形
	if (lpInvalidRect != NULL){
		paintRect = *lpInvalidRect;
	} else {
		paintRect = GetSkinAbsRect();
	}
	CRgn clipRgn;
	clipRgn.CreateRectRgnIndirect(&paintRect);

	if (pInvalidRgn != NULL) {
		clipRgn.CombineRgn(&clipRgn, pInvalidRgn, RGN_AND);
	}

	//绘制非SkinItem的皮肤元素
	list<CSkinBase*>::const_iterator iter = m_listSkinBase.begin(), last = m_listSkinBase.end();
	for (; iter!=last; iter++) {

		CSkinBase* pBase = *iter;

		if (pBase->IsFiltered(FILTER_SKINITEM, 
			FILTER_SKINITEM))
			continue;

		//不绘制不可见元素
		if (! pBase->GetVisible())
			continue;

		//不绘制背景元素
		if (pBase->GetBackground())
			continue;

		//皮肤矩形区域求交
		CRect skinRect = pBase->GetSkinRect();
		CRect rect;
		rect.IntersectRect(&paintRect, &skinRect);

		if (rect.IsRectEmpty())
			continue;

		//绘制skin
		int iSkinSavedDC = pDC->SaveDC();
		CRgn skinRgn;
		skinRgn.Attach(pBase->GetSkinRgn());
		pDC->SelectClipRgn(&skinRgn, RGN_AND);
		pBase->Draw(pDC);
		pDC->RestoreDC(iSkinSavedDC);
	}

	pDC->RestoreDC(iAllSavedDC);
}

void CSkinItemDialog::DrawFrame(CDC* pDC, LPRECT lpInvalidRect, CRgn* pInvalidRgn)
{
	//保存DC
	int iAllSavedDC = pDC->SaveDC();

	//绘制边框
	if (m_nBorderWidth > 0){
		const int wndWidth = m_rcSkinRect.Width();
		const int wndHeight = m_rcSkinRect.Height();
		CRect fillLeftRect(0, m_nTitleHeight, m_nBorderWidth, wndHeight);
		CRect fillRightRect(wndWidth - m_nBorderWidth, m_nTitleHeight, wndWidth, wndHeight);
		CRect fillDownRect(0, wndHeight - m_nBorderWidth, wndWidth, wndHeight);

		//颜色填充
		if (m_clrBorderColor != -1){
			//左、右
			if (m_pBorderVertiImage == NULL){
				CRenderEngine::FillRect(*pDC, fillLeftRect, m_clrBorderColor);
				CRenderEngine::FillRect(*pDC, fillRightRect, m_clrBorderColor);
			}
			//下边
			if (m_pBorderHorizImage == NULL){
				CRenderEngine::FillRect(*pDC, fillDownRect, m_clrBorderColor);
			}
		}

		//图片填充
		if (m_pBorderVertiImage != NULL){
			m_pBorderVertiImage->Draw(*pDC, &fillLeftRect, NULL, GetImageParam(), GetTransformedTransparentColor());
			m_pBorderVertiImage->Draw(*pDC, &fillRightRect, NULL, GetImageParam(), GetTransformedTransparentColor());
		}
		if (m_pBorderHorizImage != NULL){
			m_pBorderHorizImage->Draw(*pDC, &fillDownRect, NULL, GetImageParam(), GetTransformedTransparentColor());
		}
	}

	//绘制区域边框
	if (m_nBorderLineWidth > 0 && m_clrBorderLineColor != -1){
        LONG borderColor = m_clrBorderLineColor;
        if (GetColorTransform())
            GetEngine()->TransformColor(borderColor, &borderColor);
		CBrush brush;
		brush.CreateSolidBrush(borderColor);
		CRgn rgn;
		rgn.Attach(GetSkinAbsRgn());
		pDC->FrameRgn(&rgn, &brush, m_nBorderLineWidth, m_nBorderLineWidth);
	}

	pDC->RestoreDC(iAllSavedDC);
}

void CSkinItemDialog::Draw(CDC* pDC, LPRECT lpInvalidRect, CRgn* pInvalidRgn)
{
	//保存DC
	int iAllSavedDC = pDC->SaveDC();

	//绘制背景
	DrawBackground(pDC, lpInvalidRect, pInvalidRgn);

	//绘制前景
	DrawForeground(pDC, lpInvalidRect, pInvalidRgn);

	//绘制外框
	DrawFrame(pDC, lpInvalidRect, pInvalidRgn);

	//恢复DC
	pDC->RestoreDC(iAllSavedDC);
}

BOOL CSkinItemDialog::InvalidateRect(LPCRECT lpRects, BOOL bErase)
{
	if (! ::IsWindow(m_hWnd))
		return FALSE;

	m_bUseDrawBuffer = TRUE;

	return ::InvalidateRect(m_hWnd, lpRects, bErase);
}

//////////////////////////////////////////////////////////////////////////
//消息处理
//////////////////////////////////////////////////////////////////////////
BOOL CSkinItemDialog::OnPaint(UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* lpResult)
{
	*lpResult = 0;

	ASSERT(::IsWindow(m_hWnd));
	if (! ::IsWindow(m_hWnd) || ::IsIconic(m_hWnd))
		return FALSE;

	//准备CWnd
	CWnd* pWnd = CWnd::FromHandle(m_hWnd);
	if (pWnd == NULL)
		return TRUE;

	//获得invalid区域
	CRgn invalidRgn;
	if(!invalidRgn.CreateRectRgn(0, 0, 0, 0))
	{
		assert(0);
		return TRUE;
	}

	int ret = pWnd->GetUpdateRgn(&invalidRgn);
	ASSERT(ret != ERROR);

	//准备DC
	CPaintDC dc(pWnd);

	//获得相关矩形
	CRect rcInvalid = dc.m_ps.rcPaint;

	int iSaveDC = dc.SaveDC();

	//所有Item元素
	CRgn allSkinItemRgn;
	allSkinItemRgn.Attach(GetAllSubSkinRgn(FILTER_VISIBLE | FILTER_SKINITEM, FILTER_VISIBLE | FILTER_SKINITEM));

	if(allSkinItemRgn.GetSafeHandle() == NULL)
	{
		assert(0);
	}

	CRgn clipRgn;
	clipRgn.CreateRectRgn(0, 0, 0, 0);
	clipRgn.CombineRgn(&invalidRgn, &allSkinItemRgn, RGN_DIFF);

	CRect clipRect;
	clipRgn.GetRgnBox(&clipRect);

	if (GetclipAllChild()) {
		HRGN hAllChildRgn = NULL;
		if (CSkinItem::GetAllChildRgnHelper(m_hWnd, hAllChildRgn)
			&& hAllChildRgn != NULL) {
			CRgn allChildRgn;
			allChildRgn.Attach(hAllChildRgn);
			clipRgn.CombineRgn(&clipRgn, &allChildRgn, RGN_DIFF);
		}
	}

	dc.SelectClipRgn(&clipRgn);

	if (! m_bUseDrawBuffer) {	//不使用双缓存
		//绘制
		Draw(&dc, &clipRect, &clipRgn);

        //向窗口发送绘制消息
        ::SendMessage(m_hWnd, WM_SKIN_PAINT, (WPARAM)((HDC)dc), NULL);
	} else {
		//建立缓存DC
		CDC memDC;
		memDC.CreateCompatibleDC(&dc);
		CBitmap memBitmap;
		memBitmap.CreateCompatibleBitmap(&dc, GetWidth(), GetHeight());
		CBitmap* pOldBitmap = memDC.SelectObject(&memBitmap);

        //复制dc到缓存dc
        memDC.BitBlt(0, 0, GetWidth(), GetHeight(), &dc, 0, 0, SRCCOPY);

        //设置剪切区域
		memDC.SelectClipRgn(&clipRgn);

		//在缓存上绘制
		Draw(&memDC, &clipRect, &clipRgn);

        //向窗口发送绘制消息
		::SendMessage(m_hWnd, WM_SKIN_PAINT, (WPARAM)((HDC)memDC), NULL);

		//复制缓存DC到显示DC
		dc.BitBlt(0, 0, GetWidth(), GetHeight(), &memDC, 0, 0, SRCCOPY);

		//恢复memDC
		memDC.SelectObject(pOldBitmap);

		memBitmap.DeleteObject();
	}

	//绘制title状态
	if (! m_bNcActivate
		&& m_bTitleInactive) {
		CRect titleRect;
		GetTitleRect(titleRect);
		if (m_pTitleMaskImage != NULL) {
			m_pTitleMaskImage->Draw(dc.GetSafeHdc(), &titleRect, NULL, GetImageParam(), -1, TRUE);
		}
	}

	dc.RestoreDC(iSaveDC);

	m_bUseDrawBuffer = TRUE;

	return !GetselfOnPaint();
}

BOOL CSkinItemDialog::OnSize(UINT uMsg, WPARAM wParam,LPARAM lParam, LRESULT* lpResult)
{
	ASSERT(::IsWindow(m_hWnd));
	if (! IsWindow(m_hWnd))
		return FALSE;

	LONG width = LOWORD(lParam);
	LONG height = HIWORD(lParam);
    UINT flag = (UINT)wParam;

	if (! ::IsIconic(m_hWnd)){
        //设置SizeDelay标识
        SetSubSkinSizeDelayFlag(TRUE);

		//触发脚本OnFrameSize函数
        CSkinBase::OnFrameSize(width, height, flag);
        //触发脚本OnSize函数
		CSkinBase::OnSize(width, height, flag);

        //应用DelayedSize
        ApplySubSkinDelayedSize(FALSE);

        //清除SizeDelay标识
        SetSubSkinSizeDelayFlag(FALSE);
	}

	//设置Size标志
	m_bUseDrawBuffer = TRUE;

    return CSkinItem::OnSize(uMsg, wParam, lParam, lpResult);
}

BOOL CSkinItemDialog::OnActivate(UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* lpResult)
{
	return CSkinItem::OnActivate(uMsg, wParam, lParam, lpResult);
}

BOOL CSkinItemDialog::OnSkinNotify(UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* lpResult)
{
	*lpResult = 0;

	UINT code = (UINT)wParam;
	CComVariant varParam;
	if(lParam != NULL){
		varParam = *(VARIANT*)lParam;
	}

	CSkinBase::OnSkinNotify(code, varParam);

	return FALSE;
}

BOOL CSkinItemDialog::OnNcHitTest(UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* lpResult)
{
    if (!m_bRemoveNC)
        return FALSE;

	//调用默认的，有些窗口需要响应WM_NcHitTest
	DefWndProc(uMsg, wParam, lParam);

	LRESULT& lResult = *lpResult;

	//获得相关坐标
	LONG x = GETXPARAM(lParam);
	LONG y = GETYPARAM(lParam);

	ScreenToSkin(x, y);

	CRect wndRect = GetSkinAbsRect();
	ASSERT(y >= wndRect.top);

	//计算点到四边的距离
	LONG top = y - wndRect.top;
	LONG left = x - wndRect.left;
	LONG bottom = wndRect.bottom - y;
	LONG right = wndRect.right - x;

	//默认为客户区
	lResult = HTCLIENT;

	//小于标题栏高度
	if (top < GettitleHeight()){
		lResult = HTCAPTION;
	}

    if (m_uBkHitTest == HTCAPTION) {
        CSkinBase* pSkin = GetPointInSubSkin(x, y);
        if (pSkin == NULL) {
            lResult = HTCAPTION;
        }
    }

	if (! Getresizable() ||
		GetsizeBorder() <= 0 ||
		::IsZoomed(m_hWnd))
		return TRUE;

	LONG cornerWidth = GetsizeCorner();
	LONG borderWidth = GetsizeBorder();

	//丄左、上右、下左、下右
	if(left < cornerWidth && top < cornerWidth){
		lResult = HTTOPLEFT;
		return TRUE;
	}

	if(right < cornerWidth && top < cornerWidth){
		lResult = HTTOPRIGHT;
		return TRUE;
	}

	if(left < cornerWidth && bottom < cornerWidth){
		lResult = HTBOTTOMLEFT;
		return TRUE;
	}

	if(right < cornerWidth && bottom < cornerWidth){
		lResult = HTBOTTOMRIGHT;
		return TRUE;
	}

	//上、下、左、右
	if(top <= borderWidth) {
		lResult = HTTOP;
	}

	if(bottom <= borderWidth) {
		lResult = HTBOTTOM;
	}

	if(left <= borderWidth){
		lResult = HTLEFT;
	}

	if(right <= borderWidth){
		lResult = HTRIGHT;
	}

	return TRUE;
}

BOOL CSkinItemDialog::OnNcLButtonDown(UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* lpResult)
{
    if (!m_bRemoveNC)
        return FALSE;

    *lpResult = 0;

    LONG x = GETXPARAM(lParam);
    LONG y = GETYPARAM(lParam);
    UINT flag = (UINT)wParam;

    //转为窗口坐标
    ScreenToSkin(x, y);

    OnNcLButtonDown(x, y, flag);

    return FALSE;
}

BOOL CSkinItemDialog::OnNcLButtonDblClk(UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* lpResult)
{
    if (!m_bRemoveNC)
        return FALSE;

	*lpResult = 0;

	LONG x = GETXPARAM(lParam);
	LONG y = GETYPARAM(lParam);
	UINT flag = (UINT)wParam;

	//转为窗口坐标
	ScreenToSkin(x, y);

	CSkinBase* pSkinCapture = NULL;
	if (GetSubSkinCapture(&pSkinCapture) && 
		pSkinCapture != NULL) {
			pSkinCapture->OnLButtonDblClk(x, y, flag);
			return TRUE;
	} else {
		CSkinBase* pCurMouseInSkin = GetPointInSubSkin(x, y);
		if (pCurMouseInSkin != NULL) {
			pCurMouseInSkin->OnLButtonDblClk(x, y, flag);
			return TRUE;
		}
	}

	return FALSE;
}

BOOL CSkinItemDialog::OnNcLButtonUp(UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* lpResult)
{
    if (!m_bRemoveNC)
        return FALSE;

    *lpResult = 0;

    LONG x = GETXPARAM(lParam);
    LONG y = GETYPARAM(lParam);
    UINT flag = (UINT)wParam;

    //转为窗口坐标
    ScreenToSkin(x, y);

    CSkinBase* pSkinCapture = NULL;
    if (GetSubSkinCapture(&pSkinCapture) && 
        pSkinCapture != NULL) {
            pSkinCapture->OnLButtonUp(x, y, flag);
            return TRUE;
    } else {
        CSkinBase* pCurMouseInSkin = GetPointInSubSkin(x, y);
        if (pCurMouseInSkin != NULL) {
            pCurMouseInSkin->OnLButtonUp(x, y, flag);
            return TRUE;
        }
    }

    return FALSE;
}

BOOL CSkinItemDialog::OnNcRButtonDown(UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* lpResult)
{
    if (!m_bRemoveNC)
        return FALSE;

    *lpResult = 0;

    LONG x = GETXPARAM(lParam);
    LONG y = GETYPARAM(lParam);
    UINT flag = (UINT)wParam;

    //转为窗口坐标
    ScreenToSkin(x, y);

    //OnNcRButtonDown(x, y, flag);

    return FALSE;
}

BOOL CSkinItemDialog::OnNcRButtonDblClk(UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* lpResult)
{
    if (!m_bRemoveNC)
        return FALSE;

    *lpResult = 0;

    LONG x = GETXPARAM(lParam);
    LONG y = GETYPARAM(lParam);
    UINT flag = (UINT)wParam;

    //转为窗口坐标
    ScreenToSkin(x, y);

    CSkinBase* pSkinCapture = NULL;
    if (GetSubSkinCapture(&pSkinCapture) && 
        pSkinCapture != NULL) {
            pSkinCapture->OnRButtonDblClk(x, y, flag);
            return TRUE;
    } else {
        CSkinBase* pCurMouseInSkin = GetPointInSubSkin(x, y);
        if (pCurMouseInSkin != NULL) {
            pCurMouseInSkin->OnRButtonDblClk(x, y, flag);
            return TRUE;
        }
    }

    return FALSE;
}

BOOL CSkinItemDialog::OnNcRButtonUp(UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* lpResult)
{
    if (!m_bRemoveNC)
        return FALSE;

    *lpResult = 0;

    LONG x = GETXPARAM(lParam);
    LONG y = GETYPARAM(lParam);
    UINT flag = (UINT)wParam;

    //转为窗口坐标
    ScreenToSkin(x, y);

    CSkinBase* pSkinCapture = NULL;
    if (GetSubSkinCapture(&pSkinCapture) && 
        pSkinCapture != NULL) {
            pSkinCapture->OnRButtonUp(x, y, flag);
            return TRUE;
    } else {
        CSkinBase* pCurMouseInSkin = GetPointInSubSkin(x, y);
        if (pCurMouseInSkin != NULL) {
            pCurMouseInSkin->OnRButtonUp(x, y, flag);
            return TRUE;
        }
    }

    return FALSE;
}

BOOL CSkinItemDialog::OnNcMouseMove(UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* lpResult)
{
    if (!m_bRemoveNC)
        return FALSE;

	*lpResult = 0;

	LONG x = GETXPARAM(lParam);
	LONG y = GETYPARAM(lParam);
	UINT flag = (UINT)wParam;

	//转为窗口坐标
	ScreenToSkin(x, y);

	OnNcMouseMove(x, y, flag);

	return FALSE;
}

BOOL CSkinItemDialog::OnNcMouseLeave(UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* lpResult)
{
    if (!m_bRemoveNC)
        return FALSE;

    *lpResult = 0;

	LONG x = GETXPARAM(lParam);
	LONG y = GETYPARAM(lParam);
	UINT flag = (UINT)wParam;

    //CPoint ptCursor(0, 0);
    //::GetCursorPos(&ptCursor);
    //x = ptCursor.x;
    //y = ptCursor.y;
	//转为窗口坐标
	ScreenToSkin(x, y);

	OnNcMouseMove(x, y, flag);

	if (m_pPrevMouseInSkin != NULL) {
		m_pPrevMouseInSkin->OnMouseLeave();
		m_pPrevMouseInSkin = NULL;
	}

	OnNcMouseLeave();
	return FALSE;
}

BOOL CSkinItemDialog::OnNcPaint(UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* lpResult)
{
    if (!m_bRemoveNC)
        return FALSE;

    //if (IsInStack(WM_SYNCPAINT)) {
    //    if (wParam != 1) {
    //        HRGN hInvalidRgn = ::CreateRectRgnIndirect(CRect(0, 0, 0, 0));
    //        ::CombineRgn(hInvalidRgn, (HRGN)wParam, NULL, RGN_COPY);
    //        HDC hDC = ::GetDCEx(m_hWnd, (HRGN)wParam, DCX_WINDOW | DCX_INTERSECTRGN);
    //        CRenderEngine::FillRgn(hDC, (HRGN)hInvalidRgn, RGB(255, 255, 255));
    //        ::ReleaseDC(m_hWnd, hDC);
    //        ::DeleteObject(hInvalidRgn);
    //    } else {
    //        HDC hDC = ::GetDCEx(m_hWnd, NULL, DCX_WINDOW);
    //        CRect wndRect = GetSkinAbsRect();
    //        CRenderEngine::FillRect(hDC, wndRect, RGB(255, 255, 255));
    //        ::ReleaseDC(m_hWnd, hDC);
    //    } 
    //}

    *lpResult = 0;
	return TRUE;
}

BOOL CSkinItemDialog::OnNcActivate(UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* lpResult)
{
    if (!m_bRemoveNC)
        return FALSE;

	*lpResult = 1;

	m_bNcActivate = (BOOL)wParam;

	CRect rect;
	GetTitleRect(rect);
	//InvalidateRect(&rect, FALSE);

	return TRUE;
}

BOOL CSkinItemDialog::OnNcCalcSize(UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* lpResult)
{
    if (!m_bRemoveNC)
        return FALSE;

	*lpResult = 0;
	return TRUE;
}

BOOL CSkinItemDialog::OnLButtonDown(UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* lpResult)
{
	*lpResult = 0;

	LONG x = GETXPARAM(lParam);
	LONG y = GETYPARAM(lParam);
	UINT flag = (UINT)wParam;

	OnLButtonDown(x, y, flag);

	return FALSE;
}

BOOL CSkinItemDialog::OnLButtonUp(UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* lpResult)
{
	*lpResult = 0;

	LONG x = GETXPARAM(lParam);
	LONG y = GETYPARAM(lParam);
	UINT flag = (UINT)wParam;

	OnLButtonUp(x, y, flag);

	return FALSE;
}

BOOL CSkinItemDialog::OnLButtonDblClk(UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* lpResult)
{
    *lpResult = 0;

    LONG x = GETXPARAM(lParam);
    LONG y = GETYPARAM(lParam);
    UINT flag = (UINT)wParam;

    OnLButtonDblClk(x, y, flag);

    return FALSE;
}

BOOL CSkinItemDialog::OnRButtonDown(UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* lpResult)
{
    *lpResult = 0;

    LONG x = GETXPARAM(lParam);
    LONG y = GETYPARAM(lParam);
    UINT flag = (UINT)wParam;

    OnRButtonDown(x, y, flag);

    return FALSE;
}

BOOL CSkinItemDialog::OnRButtonUp(UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* lpResult)
{
    *lpResult = 0;

    LONG x = GETXPARAM(lParam);
    LONG y = GETYPARAM(lParam);
    UINT flag = (UINT)wParam;

    OnRButtonUp(x, y, flag);

    return FALSE;
}

BOOL CSkinItemDialog::OnRButtonDblClk(UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* lpResult)
{
    *lpResult = 0;

    LONG x = GETXPARAM(lParam);
    LONG y = GETYPARAM(lParam);
    UINT flag = (UINT)wParam;

    OnRButtonDblClk(x, y, flag);

    return FALSE;
}

BOOL CSkinItemDialog::OnMouseMove(UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* lpResult)
{
	*lpResult = 0;

	TRACKMOUSEEVENT tme;
	tme.cbSize = sizeof(tme);
	tme.dwFlags  = TME_HOVER | TME_LEAVE;
	tme.hwndTrack = m_hWnd;
	tme.dwHoverTime = HOVER_DEFAULT;
	::_TrackMouseEvent(&tme);

	LONG x = GETXPARAM(lParam);
	LONG y = GETYPARAM(lParam);
	UINT flag = (UINT)wParam;

	OnMouseMove(x, y, flag);

	return FALSE;
}

BOOL CSkinItemDialog::OnMouseHover(UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* lpResult)
{
    *lpResult = 0;

    LONG x = GETXPARAM(lParam);
    LONG y = GETYPARAM(lParam);
    UINT flag = (UINT)wParam;
    OnMouseHover(x, y, flag);

    OnMouseHover(x, y, flag);
    return FALSE;
}

BOOL CSkinItemDialog::OnMouseEnter(UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* lpResult)
{
    *lpResult = 0;

    if (m_pPrevMouseInSkin != NULL) {
        m_pPrevMouseInSkin->OnMouseEnter();
        m_pPrevMouseInSkin = NULL;
    }

    OnMouseEnter();
    return FALSE;
}

BOOL CSkinItemDialog::OnMouseLeave(UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* lpResult)
{
	*lpResult = 0;

	if (m_pPrevMouseInSkin != NULL) {
		m_pPrevMouseInSkin->OnMouseLeave();
		m_pPrevMouseInSkin = NULL;
	}

	OnMouseLeave();
	return FALSE;
}

BOOL CSkinItemDialog::OnCaptureChanged(UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* lpResult)
{
	*lpResult = 0;

	HWND hWndNewCapture = (HWND)lParam;
	CSkinItem* pItemNewCapture = CSkinItem::FromHandlePermanent(hWndNewCapture);

	//
	m_pSkinCapture = NULL;

	return FALSE;
}

BOOL CSkinItemDialog::OnGetMinMaxInfo(UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* lpResult)
{
	*lpResult = 0;

	LPMINMAXINFO pInfo = (LPMINMAXINFO)lParam;

	CRect workArea;
	::SystemParametersInfo(SPI_GETWORKAREA, 0, &workArea, 0);
	const long workAreaWidth = workArea.Width();
	const long workAreaHeight = workArea.Height();
	pInfo->ptMaxSize.x = workAreaWidth;
	pInfo->ptMaxSize.y = workAreaHeight;

	if (Getmaximizable()){
		pInfo->ptMaxPosition.x = 0;
		pInfo->ptMaxPosition.y = 0;
	}else{
		pInfo->ptMaxPosition.x = workArea.left;
		pInfo->ptMaxPosition.y = workArea.top;
	}

	if (m_nMaxWidth > 0){
		pInfo->ptMaxTrackSize.x = min(m_nMaxWidth, workAreaWidth);
	}
	if (m_nMaxHeight > 0){
		pInfo->ptMaxTrackSize.y = min(m_nMaxHeight, workAreaHeight);
	}

	if (m_nMinWidth >= 0){
		pInfo->ptMinTrackSize.x = max(0, m_nMinWidth);
	}
	if (m_nMinHeight >= 0){
		pInfo->ptMinTrackSize.y = max(0, m_nMinHeight);
	}

	return TRUE;
}

BOOL CSkinItemDialog::OnSetFocus(UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* lpResult)
{
	*lpResult = 0;

    if (m_bSelfOnSetFocus)
        return FALSE;

	if (m_pSkinFocus != NULL) {
		BOOL bRet = CSkinItem::OnSetFocus(uMsg, wParam, lParam, lpResult);
		return TRUE;
	}
	CSkinBase* pFind = NULL;
	if (CSkinBase::WalkSubSkinTree(this, pFind, 
		FILTER_TABSTOP | FILTER_VISIBLE, FILTER_TABSTOP | FILTER_VISIBLE,
        FILTER_VISIBLE, FILTER_VISIBLE)) {
		pFind->SetFocusSkin();
		return TRUE;
	}
	return FALSE;
}

BOOL CSkinItemDialog::OnSetCursor(UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* lpResult)
{
	*lpResult = 0;

	//获得鼠标坐标
	CPoint pt;
	LPPOINT lpPoint = NULL;
	if (GetCursorPos(&pt)) {
		::ScreenToClient(m_hWnd, &pt);
		lpPoint = &pt;
	}

	HCURSOR hCursor = NULL;
	CSkinBase* pSkinCapture = NULL;
	if (GetSubSkinCapture(&pSkinCapture) && 
		pSkinCapture != NULL) {
		hCursor = pSkinCapture->GetSkinCursor(lpPoint);
	} else if (m_pPrevMouseInSkin != NULL) {
		hCursor = m_pPrevMouseInSkin->GetSkinCursor(lpPoint);
	} else {
		return FALSE;
	}

	if (hCursor == NULL)
		return FALSE;
	
	::SetCursor(hCursor);
	*lpResult = 1;
	return TRUE;
}

BOOL CSkinItemDialog::OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* lpResult)
{
	//先调用默认的对话框初始化
	LRESULT &lr = *lpResult;
	lr = S_OK;
	lr = DefWndProc(uMsg, wParam, lParam);

	if (GetinitCenterWindow()) {
		BOOL bChild;
		if (GetSkinItemLongHelper(GWL_STYLE, WS_CHILD, bChild)
			&& bChild) {
			CWnd::FromHandle(m_hWnd)->CenterWindow(NULL);
		}
	}

	m_bNcActivate = (::GetActiveWindow() == m_hWnd);

	//调用皮肤的初始化函数
	list<CSkinBase*>::const_iterator iter = m_listSkinBase.begin(),
		last = m_listSkinBase.end();
	for (; iter!=last; iter++){
		CSkinBase* pBase = (*iter);
		if ( ! pBase->IsFiltered(FILTER_SKINITEM, FILTER_SKINITEM) 
			||  pBase->IsKindOf(RUNTIME_CLASS(CSkinProxy)) )
			continue;
		CSkinItem* pItem = (CSkinItem*)(*iter);
		if (pItem != NULL)
			pItem->UpdateSkinItem();
	}

	CSkinBase::OnInitDialog(NULL);
	return TRUE;
}

BOOL CSkinItemDialog::OnCtlColor(UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* lpResult)
{
	HBRUSH hBrush = NULL;

	const HWND hWnd = (HWND)lParam;
	const HDC hDC = (HDC)wParam;
	CSkinItem* pItem = CSkinItem::FromHandlePermanent(hWnd);
	if (pItem == NULL)
		return FALSE;

	BOOL bCtlColor = FALSE;
	//进行皮肤设置
	if (pItem != NULL){

		//
		CDC tempDC;
		CWnd tempWnd;
		tempDC.m_hDC = hDC;
		tempWnd.m_hWnd = hWnd;
		CWnd* pWnd = &tempWnd;

		//皮肤设置
		HBRUSH itemBrush = NULL;
		bCtlColor = pItem->CtlColor(&tempDC, pWnd, itemBrush);
		if (bCtlColor) {
			hBrush = itemBrush;
		}

		//分离临时对象
		tempDC.m_hDC = NULL;
		tempWnd.m_hWnd = NULL;
	}
	*lpResult = (LRESULT)hBrush;
	return bCtlColor;
}

BOOL CSkinItemDialog::OnEraseBkgnd(UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* lpResult)
{
	*lpResult = 0;
	ASSERT(::IsWindow(m_hWnd));
	if (! ::IsWindow(m_hWnd))
		return FALSE;

    if (GetselfOnEraseBkgnd())
        return FALSE;

    //if (IsInStack(WM_SYNCPAINT)) {
    //    HRGN hInvalidRgn = ::CreateRectRgnIndirect(CRect(0, 0, 0, 0));
    //    ::GetUpdateRgn(m_hWnd, hInvalidRgn, FALSE);
    //    HRGN hCopyRgn = ::CreateRectRgnIndirect(CRect(0, 0, 0, 0));
    //    ::CombineRgn(hCopyRgn, hInvalidRgn, NULL, RGN_COPY);
    //    HDC hDC = ::GetDCEx(m_hWnd, (HRGN)hCopyRgn, DCX_INTERSECTRGN);
    //    CRenderEngine::FillRgn(hDC, (HRGN)hInvalidRgn, RGB(255, 255, 255));
    //    ::ReleaseDC(m_hWnd, hDC);
    //    ::DeleteObject(hInvalidRgn);
    //}

    CSkinItem::OnEraseBkgnd(uMsg, wParam, lParam, lpResult);

    *lpResult = TRUE;

    if (! m_bEraseBk)
        return TRUE;

    if (m_bEraseBkOnce && m_lEraseBkCount >= 1)
        return TRUE;

    if (   !IsInStack(m_hWnd, WM_PAINT) //不由OnPaint调用
        && !IsInUpdateRgn() //不由UpdateRgn调用
        && !IsInStack(m_hWnd, WM_ACTIVATE) //不由OnActivate调用
        && !IsInStack(m_hWnd, WM_SYNCPAINT) //不由OnSyncPaint调用
        && !IsInStack(m_hWnd, WM_SYSCOMMAND) //不由OnSysCommand调用
        && !IsInStack(WM_SYSCOMMAND) //不在OnSysCommand的调用栈中
        && !IsInStack(WM_SIZE) //不再OnSize的调用栈中
        && !IsInStack(WM_CLOSE) //不再OnClose的调用栈中
        ) {

        m_lEraseBkCount ++;

        //DC
        HDC hDC = (HDC)wParam;
        CDC* pDC = CDC::FromHandle(hDC);

        //保存DC
        int iSaveDC = pDC->SaveDC();

        //清空所有ClipRgn
        pDC->SelectClipRgn(NULL);

        //建立缓存DC
        const LONG width = GetWidth();
        const LONG height = GetHeight();
        CDC memDC;
        memDC.CreateCompatibleDC(pDC);
        CBitmap memBitmap;
        memBitmap.CreateCompatibleBitmap(pDC, width, height);
        CBitmap* pOldBitmap = memDC.SelectObject(&memBitmap);

        //复制显示DC到缓存DC
        memDC.BitBlt(0, 0, width, height, pDC, 0, 0, SRCCOPY);

        DrawBackground(&memDC, NULL, NULL, TRUE);

        //复制缓存DC到显示DC
        pDC->BitBlt(0, 0, width, height, &memDC, 0, 0, SRCCOPY);

		//向窗口发送背景绘制消息
		::SendMessage(m_hWnd, WM_SKIN_ERASEBKGND, (WPARAM)((HDC)memDC), NULL);

        //恢复memDC
        memDC.SelectObject(pOldBitmap);

        //恢复DC
        pDC->RestoreDC(iSaveDC);

		memBitmap.DeleteObject();
    }

    return TRUE;
}

BOOL CSkinItemDialog::OnSysCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* lpResult)
{
    *lpResult = 0;

    UINT nID = (UINT)wParam;
    UINT scCode = nID & 0xFFF0;
    UINT htCode = nID & 0x000F;

    if (! Getmovable()) {
        if (scCode == SC_MOVE)
            return TRUE;
    }

    BOOL bSize = (scCode == SC_MAXIMIZE || scCode == SC_RESTORE);
    if (bSize) {
        CSkinBase::OnEnterSizeMove(TRUE);
    }

    *lpResult = DefWndProc(uMsg, wParam, lParam);

    if (bSize) {
        CSkinBase::OnExitSizeMove(TRUE);
    }

    return TRUE;
}

BOOL CSkinItemDialog::OnNcMouseMove(LONG x, LONG y, UINT flag)
{
    CSkinBase* pCurMouseInSkin = GetPointInSubSkin(x, y);
    CSkinBase* pSkinCapture = NULL;
    if (GetSubSkinCapture(&pSkinCapture) && 
        pSkinCapture != NULL) {
        pSkinCapture->OnMouseMove(x, y, flag);
        m_pPrevMouseInSkin = pCurMouseInSkin;
        return TRUE;
    }

    if (m_pPrevMouseInSkin != pCurMouseInSkin) {
        if (m_pPrevMouseInSkin != NULL)
            m_pPrevMouseInSkin->OnMouseLeave();
        if (pCurMouseInSkin != NULL)
            pCurMouseInSkin->OnMouseEnter();
        m_pPrevMouseInSkin = pCurMouseInSkin;
    }
    if (pCurMouseInSkin != NULL) {
        pCurMouseInSkin->OnMouseMove(x, y, flag);
        return TRUE;
    } 

    return CSkinBase::OnMouseMove(x, y, flag);
}

BOOL CSkinItemDialog::OnNcLButtonDown(LONG x, LONG y, UINT flag)
{
    CSkinBase* pSkinCapture = NULL;
    if (GetSubSkinCapture(&pSkinCapture) && 
        pSkinCapture != NULL) {
        pSkinCapture->OnLButtonDown(x, y, flag);
        return TRUE;
    }

    CSkinBase* pCurMouseInSkin = GetPointInSubSkin(x, y);
    if (pCurMouseInSkin != NULL) {
        pCurMouseInSkin->OnLButtonDown(x, y, flag);
        return TRUE;
    }

    return CSkinBase::OnLButtonDown(x, y, flag);
}

BOOL CSkinItemDialog::OnNcLButtonDblClk(LONG x, LONG y, UINT flag)
{
    CSkinBase* pSkinCapture = NULL;
    if (GetSubSkinCapture(&pSkinCapture) && 
        pSkinCapture != NULL) {
        pSkinCapture->OnLButtonDblClk(x, y, flag);
        return TRUE;
    }

    CSkinBase* pCurMouseInSkin = GetPointInSubSkin(x, y);
    if (pCurMouseInSkin != NULL) {
        pCurMouseInSkin->OnLButtonDblClk(x, y, flag);
        return TRUE;
    }

    return CSkinBase::OnLButtonDblClk(x, y, flag);
}

BOOL CSkinItemDialog::OnNcMouseLeave()
{
    return CSkinBase::OnMouseLeave();
}

BOOL CSkinItemDialog::OnMouseMove(LONG x, LONG y, UINT flag)
{
	CSkinBase* pCurMouseInSkin = GetPointInSubSkin(x, y);
	CSkinBase* pSkinCapture = NULL;
	if (GetSubSkinCapture(&pSkinCapture) && 
		pSkinCapture != NULL) {
		pSkinCapture->OnMouseMove(x, y, flag);
        m_pPrevMouseInSkin = pCurMouseInSkin;
		return TRUE;
	}

    //背景拖动支持
    HWND hAncestor = ::GetAncestor(m_hWnd, GA_ROOT);
    SHORT lbState = ::GetAsyncKeyState(VK_LBUTTON);
    BOOL bLButtonDown = (lbState & 0x8001) != 0;
    if (m_bDragging && bLButtonDown) {
        CPoint curPoint(x, y);
        if (m_ptOldDragPoint != curPoint) {
            SkinToScreen(curPoint);
            CRect ancestorRect;
            ::GetWindowRect(hAncestor, &ancestorRect);
            ::OffsetRect(&ancestorRect, -ancestorRect.left, -ancestorRect.top);
            ::OffsetRect(&ancestorRect, curPoint.x, curPoint.y);
            ::OffsetRect(&ancestorRect, -m_ptDragPoint.x, -m_ptDragPoint.y);
            ::SendMessage(hAncestor, WM_MOVING, 0, (LPARAM)(&ancestorRect));
            ::SetWindowPos(hAncestor, NULL, ancestorRect.left, ancestorRect.top, ancestorRect.Width(), ancestorRect.Height(), 0);
        }
        m_ptOldDragPoint = curPoint;
    }

	if (m_pPrevMouseInSkin != pCurMouseInSkin) {
		if (m_pPrevMouseInSkin != NULL)
			m_pPrevMouseInSkin->OnMouseLeave();
		if (pCurMouseInSkin != NULL)
			pCurMouseInSkin->OnMouseEnter();
		m_pPrevMouseInSkin = pCurMouseInSkin;
	}
	if (pCurMouseInSkin != NULL) {
		pCurMouseInSkin->OnMouseMove(x, y, flag);
		return TRUE;
    } 

	return CSkinBase::OnMouseMove(x, y, flag);
}

BOOL CSkinItemDialog::OnLButtonDown(LONG x, LONG y, UINT flag)
{
	CSkinBase* pSkinCapture = NULL;
	if (GetSubSkinCapture(&pSkinCapture) && 
		pSkinCapture != NULL) {
		pSkinCapture->OnLButtonDown(x, y, flag);
		return TRUE;
	}

	CSkinBase* pCurMouseInSkin = GetPointInSubSkin(x, y);
	if (pCurMouseInSkin != NULL) {
		pCurMouseInSkin->OnLButtonDown(x, y, flag);
		return TRUE;
    }

    //背景拖动支持
    HWND hAncestor = ::GetAncestor(m_hWnd, GA_ROOT);
    if (m_bBkDrag && !::IsZoomed(hAncestor) && Getmovable()) {
        ::SetCapture(m_hWnd);
        m_bDragging = TRUE;
        m_ptDragPoint = CPoint(x, y);
        m_ptOldDragPoint = m_ptDragPoint;
        SkinToScreen(m_ptDragPoint);
        ::ScreenToClient(hAncestor, &m_ptDragPoint);
    }

	return CSkinBase::OnLButtonDown(x, y, flag);
}

BOOL CSkinItemDialog::OnLButtonUp(LONG x, LONG y, UINT flag)
{
	CSkinBase* pSkinCapture = NULL;
	if (GetSubSkinCapture(&pSkinCapture) && 
		pSkinCapture != NULL) {
		pSkinCapture->OnLButtonUp(x, y, flag);
		return TRUE;
	}

    //背景拖动支持
    HWND hAncestor = ::GetAncestor(m_hWnd, GA_ROOT);
    if (m_bDragging) {
        if (::GetCapture() == m_hWnd) {
            ::ReleaseCapture();
        }
        m_bDragging = FALSE;
        m_ptDragPoint = CPoint(-1, -1);
    }

	CSkinBase* pCurMouseInSkin = GetPointInSubSkin(x, y);
	if (pCurMouseInSkin != NULL) {
		pCurMouseInSkin->OnLButtonUp(x, y, flag);
		return TRUE;
    } 

	return CSkinBase::OnLButtonUp(x, y, flag);
}

BOOL CSkinItemDialog::OnLButtonDblClk(LONG x, LONG y, UINT flag)
{
	CSkinBase* pSkinCapture = NULL;
	if (GetSubSkinCapture(&pSkinCapture) && 
		pSkinCapture != NULL) {
		pSkinCapture->OnLButtonDblClk(x, y, flag);
		return TRUE;
	}

	CSkinBase* pCurMouseInSkin = GetPointInSubSkin(x, y);
	if (pCurMouseInSkin != NULL) {
		pCurMouseInSkin->OnLButtonDblClk(x, y, flag);
		return TRUE;
	}

	return CSkinBase::OnLButtonDblClk(x, y, flag);
}

BOOL CSkinItemDialog::OnRButtonDown(LONG x, LONG y, UINT flag)
{
	CSkinBase* pSkinCapture = NULL;
	if (GetSubSkinCapture(&pSkinCapture) && 
		pSkinCapture != NULL) {
		pSkinCapture->OnRButtonDown(x, y, flag);
		return TRUE;
	}

	CSkinBase* pCurMouseInSkin = GetPointInSubSkin(x, y);
	if (pCurMouseInSkin != NULL) {
		pCurMouseInSkin->OnRButtonDown(x, y, flag);
		return TRUE;
	}

	return CSkinBase::OnRButtonDown(x, y, flag);
}

BOOL CSkinItemDialog::OnRButtonUp(LONG x, LONG y, UINT flag)
{
	CSkinBase* pSkinCapture = NULL;
	if (GetSubSkinCapture(&pSkinCapture) && 
		pSkinCapture != NULL) {
		pSkinCapture->OnRButtonUp(x, y, flag);
		return TRUE;
	}

	CSkinBase* pCurMouseInSkin = GetPointInSubSkin(x, y);
	if (pCurMouseInSkin != NULL) {
		pCurMouseInSkin->OnRButtonUp(x, y, flag);
		return TRUE;
	}

	return CSkinBase::OnRButtonUp(x, y, flag);
}

BOOL CSkinItemDialog::OnRButtonDblClk(LONG x, LONG y, UINT flag)
{
	CSkinBase* pSkinCapture = NULL;
	if (GetSubSkinCapture(&pSkinCapture) && 
		pSkinCapture != NULL) {
		pSkinCapture->OnRButtonDblClk(x, y, flag);
		return TRUE;
	}

    CSkinBase* pCurMouseInSkin = GetPointInSubSkin(x, y);
	if (pCurMouseInSkin != NULL) {
		pCurMouseInSkin->OnRButtonDblClk(x, y, flag);
		return TRUE;
	}

	return CSkinBase::OnRButtonDblClk(x, y, flag);
}

BOOL CSkinItemDialog::OnMButtonDown(LONG x, LONG y, UINT flag)
{
	CSkinBase* pSkinCapture = NULL;
	if (GetSubSkinCapture(&pSkinCapture) && 
		pSkinCapture != NULL) {
		pSkinCapture->OnMButtonDown(x, y, flag);
		return TRUE;
	}

	CSkinBase* pCurMouseInSkin = GetPointInSubSkin(x, y);
	if (pCurMouseInSkin != NULL) {
		pCurMouseInSkin->OnMButtonDown(x, y, flag);
		return TRUE;
	}

	return CSkinBase::OnMButtonDown(x, y, flag);
}

BOOL CSkinItemDialog::OnMButtonUp(LONG x, LONG y, UINT flag)
{
	CSkinBase* pSkinCapture = NULL;
	if (GetSubSkinCapture(&pSkinCapture) && 
		pSkinCapture != NULL) {
		pSkinCapture->OnMButtonUp(x, y, flag);
		return TRUE;
	}

	CSkinBase* pCurMouseInSkin = GetPointInSubSkin(x, y);
	if (pCurMouseInSkin != NULL) {
		pCurMouseInSkin->OnMButtonUp(x, y, flag);
		return TRUE;
	}

	return CSkinBase::OnMButtonUp(x, y, flag);
}

BOOL CSkinItemDialog::OnMButtonDblClk(LONG x, LONG y, UINT flag)
{
	CSkinBase* pSkinCapture = NULL;
	if (GetSubSkinCapture(&pSkinCapture) && 
		pSkinCapture != NULL) {
		pSkinCapture->OnMButtonDblClk(x, y, flag);
		return TRUE;
	}

	CSkinBase* pCurMouseInSkin = GetPointInSubSkin(x, y);
	if (pCurMouseInSkin != NULL) {
		pCurMouseInSkin->OnMButtonDblClk(x, y, flag);
		return TRUE;
	}

	return CSkinBase::OnMButtonDblClk(x, y, flag);
}

BOOL CSkinItemDialog::OnMouseWheel(LONG x, LONG y, UINT flag)
{
	CSkinBase* pSkinCapture = NULL;
	if (GetSubSkinCapture(&pSkinCapture) && 
		pSkinCapture != NULL) {
		pSkinCapture->OnMouseWheel(x, y, flag);
		return TRUE;
	}

	CSkinBase* pCurMouseInSkin = GetPointInSubSkin(x, y);
	if (pCurMouseInSkin != NULL) {
		pCurMouseInSkin->OnMouseWheel(x, y, flag);
		return TRUE;
	}

	return CSkinBase::OnMouseWheel(x, y, flag);
}

BOOL CSkinItemDialog::OnMouseHover(LONG x, LONG y, UINT flag)
{
	CSkinBase* pSkinCapture = NULL;
	if (GetSubSkinCapture(&pSkinCapture) && 
		pSkinCapture != NULL) {
		pSkinCapture->OnMouseHover(x, y, flag);
		return TRUE;
	}

	CSkinBase* pCurMouseInSkin = GetPointInSubSkin(x, y);
	if (pCurMouseInSkin != NULL) {
		pCurMouseInSkin->OnMouseHover(x, y, flag);
		return TRUE;
	}

	return CSkinBase::OnMouseHover(x, y, flag);
}

BOOL CSkinItemDialog::OnMouseEnter()
{
	return CSkinBase::OnMouseEnter();
}

BOOL CSkinItemDialog::OnMouseLeave()
{
	return CSkinBase::OnMouseLeave();
}
