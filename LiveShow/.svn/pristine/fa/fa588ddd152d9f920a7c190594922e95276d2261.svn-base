// SkinProxy.cpp : 实现文件
//

#include "stdafx.h"
#include "SkinEngine.h"
#include "SkinItem.h"
#include "SkinProxy.h"

#pragma warning (disable:4312)

#ifdef V8_JS_ENGINE_SUPPORT
using namespace v8;
#endif //V8_JS_ENGINE_SUPPORT

// CSkinProxy

IMPLEMENT_DYNCREATE(CSkinProxy, CSkinBase)
CSkinProxy::CSkinProxy()
:m_pSkinClient(NULL)
{
	EnableAutomation();
	
	// 为了使应用程序在 OLE 自动化对象处于活动状态时保持
	//	运行，构造函数调用 AfxOleLockApp。
	
	AfxOleLockApp();
}

CSkinProxy::~CSkinProxy()
{
	// 为了在用 OLE 自动化创建所有对象后终止应用程序，
	// 	析构函数调用 AfxOleUnlockApp。

	RemoveClient();
	
	AfxOleUnlockApp();
}

void CSkinProxy::OnFinalRelease()
{
	// 释放了对自动化对象的最后一个引用后，将调用
	// OnFinalRelease。基类将自动
	// 删除该对象。在调用该基类之前，请添加您的
	// 对象所需的附加清除代码。

	CSkinBase::OnFinalRelease();
}


BEGIN_MESSAGE_MAP(CSkinProxy, CSkinBase)
END_MESSAGE_MAP()


BEGIN_SKIN_DISPATCH_MAP(CSkinProxy, CSkinBase)
	SKINBASE_DISP_PROPERTIES(CSkinProxy)

	SKIN_DISP_FUNCTION(CSkinProxy, SetClient, VT_BOOL, VTS_I4)
    SKIN_DISP_FUNCTION(CSkinProxy, GetClient, VT_UNKNOWN, VTS_NONE)
END_SKIN_DISPATCH_MAP(CSkinProxy)

// 注意: 我们添加 IID_ISkinProxy 支持
//以支持来自 VBA 的类型安全绑定。此 IID 必须同附加到 .IDL 文件中的
//调度接口的 GUID 匹配。

// {371ABD97-6091-4FDB-A087-69FBC71AD861}
static const IID IID_ISkinProxy =
{ 0x371ABD97, 0x6091, 0x4FDB, { 0xA0, 0x87, 0x69, 0xFB, 0xC7, 0x1A, 0xD8, 0x61 } };

BEGIN_INTERFACE_MAP(CSkinProxy, CSkinBase)
	INTERFACE_PART(CSkinProxy, IID_ISkinProxy, Dispatch)
END_INTERFACE_MAP()

// {087C294B-0695-433A-973A-C3EAD50A560D}
IMPLEMENT_OLECREATE(CSkinProxy, "Skin.SkinProxy", 0x87c294b, 0x695, 0x433a, 0x97, 0x3a, 0xc3, 0xea, 0xd5, 0xa, 0x56, 0xd)


// CSkinProxy 消息处理程序

//皮肤的文本
BSTR CSkinProxy::GetText()
{
	if (m_pSkinClient != NULL) {
		return m_pSkinClient->GetText();
	}
	return CSkinBase::GetText();
}

//鼠标光标
BSTR CSkinProxy::GetCursor()
{
	if (m_pSkinClient != NULL) {
		return m_pSkinClient->GetCursor();
	}
	return CSkinBase::GetCursor();
}

//设置皮肤元素的位置
LONG CSkinProxy::GetLeft()
{
	if (m_pSkinClient != NULL) {
		return m_pSkinClient->GetLeft();
	}
	return CSkinBase::GetLeft();
}

LONG CSkinProxy::GetTop()
{
	if (m_pSkinClient != NULL) {
		return m_pSkinClient->GetTop();
	}
	return CSkinBase::GetTop();
}

LONG CSkinProxy::GetWidth()
{
	if (m_pSkinClient != NULL) {
		return m_pSkinClient->GetWidth();
	}
	return CSkinBase::GetWidth();
}

LONG CSkinProxy::GetHeight()
{
	if (m_pSkinClient != NULL) {
		return m_pSkinClient->GetHeight();
	} 
	return CSkinBase::GetHeight();
}

BOOL CSkinProxy::GetSizeDelay()
{
    if (m_pSkinClient != NULL) {
        return m_pSkinClient->GetSizeDelay();
    }
    return CSkinBase::GetSizeDelay();
}

//可见性
BOOL CSkinProxy::GetVisible()
{
	if (m_pSkinClient != NULL) {
		return m_pSkinClient->GetVisible();
	}
	return CSkinBase::GetVisible();
}

//可用性
BOOL CSkinProxy::GetDisabled()
{
	if (m_pSkinClient != NULL) {
		return m_pSkinClient->GetDisabled();
	}
	return CSkinBase::GetDisabled();
}

//Tab Stop
BOOL CSkinProxy::GetTabStop()
{
	if (m_pSkinClient != NULL) {
		return m_pSkinClient->GetTabStop();
	}
	return CSkinBase::GetTabStop();
}

//Group
BOOL CSkinProxy::GetGroup()
{
	if (m_pSkinClient != NULL) {
		return m_pSkinClient->GetGroup();
	}
	return CSkinBase::GetGroup();
}

//背景属性
BOOL CSkinProxy::GetBackground()
{
	if (m_pSkinClient != NULL) {
		return m_pSkinClient->GetBackground();
	}
	return CSkinBase::GetBackground();
}

//透明
BOOL CSkinProxy::GetTransparent()
{
	if (m_pSkinClient != NULL) {
		return m_pSkinClient->GetTransparent();
	}
	return CSkinBase::GetTransparent();
}

//颜色变换
BOOL CSkinProxy::GetColorTransform()
{
	if (m_pSkinClient != NULL) {
		return m_pSkinClient->GetColorTransform();
	}
	return CSkinBase::GetColorTransform();
}

//颜色变换
BOOL CSkinProxy::GetAlpha()
{
	if (m_pSkinClient != NULL) {
		return m_pSkinClient->GetAlpha();
	}
	return CSkinBase::GetAlpha();
}

//上边角半径
LONG CSkinProxy::GetUpCornerRadius()
{
	if (m_pSkinClient != NULL) {
		return m_pSkinClient->GetUpCornerRadius();
	}
	return CSkinBase::GetUpCornerRadius();
}

//下边角半径
LONG CSkinProxy::GetDownCornerRadius()
{
	if (m_pSkinClient != NULL) {
		return m_pSkinClient->GetDownCornerRadius();
	}
	return CSkinBase::GetDownCornerRadius();
}

//边线宽
LONG CSkinProxy::GetBorderLineWidth()
{
	if (m_pSkinClient != NULL) {
		return m_pSkinClient->GetBorderLineWidth();
	}
	return CSkinBase::GetBorderLineWidth();
}

//边线颜色
LONG CSkinProxy::GetBorderLineColor()
{
	if (m_pSkinClient != NULL) {
		return m_pSkinClient->GetBorderLineColor();
	}
	return CSkinBase::GetBorderLineColor();
}

//透明色
LONG CSkinProxy::GetTransparentColor()
{
	if (m_pSkinClient != NULL) {
		return m_pSkinClient->GetTransparentColor();
	}
	return CSkinBase::GetTransparentColor();
}

//文本
void CSkinProxy::SetText(LPCTSTR newVal)
{
	CSkinBase::SetText(newVal);
	if (m_pSkinClient != NULL) {
		m_pSkinClient->SetText(newVal);
	}
}

//鼠标光标
void CSkinProxy::SetCursor(LPCTSTR newVal)
{
	CSkinBase::SetCursor(newVal);
	if (m_pSkinClient != NULL) {
		m_pSkinClient->SetCursor(newVal);
	}
}

//设置皮肤元素的位置
void CSkinProxy::SetLeft(LONG newVal)
{
    m_rcSkinRect.OffsetRect((newVal - m_rcSkinRect.left), 0);

	if (m_pSkinClient != NULL) {
		m_pSkinClient->SetLeft(newVal);
	}
}

void CSkinProxy::SetTop(LONG newVal)
{
    m_rcSkinRect.OffsetRect(0, (newVal - m_rcSkinRect.top));

    if (m_pSkinClient != NULL) {
		m_pSkinClient->SetTop(newVal);
	}
}

void CSkinProxy::SetWidth(LONG newVal)
{
    m_rcSkinRect.right = m_rcSkinRect.left + newVal;

	if (m_pSkinClient != NULL) {
		m_pSkinClient->SetWidth(newVal);
	}
}

void CSkinProxy::SetHeight(LONG newVal)
{
    m_rcSkinRect.bottom = m_rcSkinRect.top + newVal;

	if (m_pSkinClient != NULL) {
		m_pSkinClient->SetHeight(newVal);
	}
}

void CSkinProxy::SetSizeDelay(BOOL newVal)
{
    CSkinBase::SetSizeDelay(newVal);
    if (m_pSkinClient != NULL) {
        m_pSkinClient->SetSizeDelay(newVal);
    }
}

//可见性
void CSkinProxy::SetVisible(BOOL newVal)
{
	CSkinBase::SetVisible(newVal);
	if (m_pSkinClient != NULL) {
		m_pSkinClient->SetVisible(newVal);
	}
}

//可用性
void CSkinProxy::SetDisabled(BOOL newVal)
{
	CSkinBase::SetDisabled(newVal);
	if (m_pSkinClient != NULL) {
		m_pSkinClient->SetDisabled(newVal);
	}
}

//Tab Stop
void CSkinProxy::SetTabStop(BOOL newVal)
{
	CSkinBase::SetTabStop(newVal);
	if (m_pSkinClient != NULL) {
		m_pSkinClient->SetTabStop(newVal);
	}
}

//Group
void CSkinProxy::SetGroup(BOOL newVal)
{
	CSkinBase::SetGroup(newVal);
	if (m_pSkinClient != NULL) {
		m_pSkinClient->SetGroup(newVal);
	}
}

//背景属性
void CSkinProxy::SetBackground(BOOL newVal)
{
	CSkinBase::SetBackground(newVal);
	if (m_pSkinClient != NULL) {
		m_pSkinClient->SetBackground(newVal);
	}
}

//透明
void CSkinProxy::SetTransparent(BOOL newVal)
{
	CSkinBase::SetTransparent(newVal);
	if (m_pSkinClient != NULL) {
		m_pSkinClient->SetTransparent(newVal);
	}
}

//颜色变换
void CSkinProxy::SetColorTransform(BOOL newVal)
{
	CSkinBase::SetColorTransform(newVal);
	if (m_pSkinClient != NULL) {
		m_pSkinClient->SetColorTransform(newVal);
	}
}

//颜色变换
void CSkinProxy::SetAlpha(BOOL newVal)
{
	CSkinBase::SetAlpha(newVal);
	if (m_pSkinClient != NULL) {
		m_pSkinClient->SetAlpha(newVal);
	}
}

//上边角半径
void CSkinProxy::SetUpCornerRadius(LONG newVal)
{
	CSkinBase::SetUpCornerRadius(newVal);
	if (m_pSkinClient != NULL) {
		m_pSkinClient->SetUpCornerRadius(newVal);
	}
}

//下边角半径
void CSkinProxy::SetDownCornerRadius(LONG newVal)
{
	CSkinBase::SetDownCornerRadius(newVal);
	if (m_pSkinClient != NULL) {
		m_pSkinClient->SetDownCornerRadius(newVal);
	}
}

//边线宽
void CSkinProxy::SetBorderLineWidth(LONG newVal)
{
	CSkinBase::SetBorderLineWidth(newVal);
	if (m_pSkinClient != NULL) {
		m_pSkinClient->SetBorderLineWidth(newVal);
	}
}

//边线颜色
void CSkinProxy::SetBorderLineColor(LONG newVal)
{
	CSkinBase::SetBorderLineColor(newVal);
	if (m_pSkinClient != NULL) {
		m_pSkinClient->SetBorderLineColor(newVal);
	}
}

//透明色
void CSkinProxy::SetTransparentColor(LONG newVal)
{
	CSkinBase::SetTransparentColor(newVal);
	if (m_pSkinClient != NULL) {
		m_pSkinClient->SetTransparentColor(newVal);
	}
}


BOOL CSkinProxy::GetSizeDelayFlag()
{
    if (m_pSkinClient != NULL) {
        return m_pSkinClient->GetSizeDelayFlag();
    }
    return CSkinBase::GetSizeDelayFlag();
}

void CSkinProxy::SetSizeDelayFlag(BOOL bTrue)
{
    CSkinBase::SetSizeDelayFlag(bTrue);
    if (m_pSkinClient != NULL) {
        m_pSkinClient->SetSizeDelayFlag(bTrue);
    }
}

BOOL CSkinProxy::ApplyDelayedSize(BOOL bRepaint)
{
    CSkinBase::ApplyDelayedSize(bRepaint);
    if (m_pSkinClient != NULL) {
        return m_pSkinClient->ApplyDelayedSize(bRepaint);
    }
    return FALSE;
}

//父窗口
CSkinBase* CSkinProxy::GetSkinParent()
{
	//if (m_pSkinClient != NULL) {
	//	return m_pSkinClient->GetSkinParent();
	//} 
	return CSkinBase::GetSkinParent();
}

//Owner窗口
CSkinBase* CSkinProxy::GetSkinOwner()
{
	if (m_pSkinClient != NULL) {
		return m_pSkinClient->GetSkinOwner();
	} 
	return NULL;
}

//下一窗口
CSkinBase* CSkinProxy::GetSkinNext()
{
	return CSkinBase::GetSkinNext();
}

//上一窗口
CSkinBase* CSkinProxy::GetSkinPrev()
{
	return CSkinBase::GetSkinPrev();
}

//获得第一个子元素，bForward为方向
CSkinBase* CSkinProxy::GetFirstSubSkin(UINT uFilterBits, UINT uFilterValues, BOOL bForward)
{
	if (m_pSkinClient != NULL) {
		return m_pSkinClient->GetFirstSubSkin(uFilterBits, uFilterValues, bForward);
	} 
	return NULL;
}

//获得下一个的兄弟元素，bForward为方向
CSkinBase* CSkinProxy::GetNextSkin(UINT uFilterBits, UINT uFilterValues, BOOL bForward)
{
	return CSkinBase::GetNextSkin(uFilterBits, uFilterValues, bForward);
}

BOOL CSkinProxy::IsFiltered(UINT uFilterBits, UINT uFilterValues)
{
	if (m_pSkinClient != NULL) {
		return m_pSkinClient->IsFiltered(uFilterBits, uFilterValues);
	}
	return CSkinBase::IsFiltered(uFilterBits, uFilterValues);
}

BOOL CSkinProxy::UpdateClientProperty(CSkinBase* pClient)
{
	if (pClient == NULL)
		return FALSE;

	if (m_pSkinClient != NULL)
		return FALSE;

	pClient->SetLeft(this->GetLeft());
	pClient->SetTop(this->GetTop());
	pClient->SetWidth(this->GetWidth());
	pClient->SetHeight(this->GetHeight());
    pClient->SetSizeDelay(this->GetSizeDelay());
	pClient->SetVisible(this->GetVisible());
	pClient->SetDisabled(this->GetDisabled());

	return TRUE;
}

BOOL CSkinProxy::RemoveClient()
{
	if (m_pSkinClient == NULL)
		return FALSE;

	if ( !GetEngine()->IsSkinDestroyed(m_pSkinClient) )
		m_pSkinClient->m_pSkinProxy = NULL;
	m_pSkinClient = NULL;

	return TRUE;
}

VARIANT_BOOL CSkinProxy::SetClient(LONG newVal)
{
	HWND hWnd = (HWND)newVal;
	CSkinItem* pItem = CSkinItem::FromHandlePermanent(hWnd);
	if (pItem == NULL) {
		return VARIANT_FALSE;
	}

	UpdateClientProperty(pItem);

	m_pSkinClient = pItem;
	pItem->m_pSkinProxy = this;
	return VARIANT_TRUE;
}

IUnknown* CSkinProxy::GetClient(void)
{
    if (m_pSkinClient == NULL)
        return NULL;

    IUnknown* pUnknown = NULL;
    m_pSkinClient->GetInterface(&pUnknown);
    return pUnknown;
}

#ifdef V8_JS_ENGINE_SUPPORT
Handle<Value> CSkinProxy::V8SetClient(const Arguments& args)
{
    HandleScope handleScope;

    LONG hWnd = args[0]->Int32Value();

    Handle<Value> ret = SetClient(hWnd) ? (Boolean::New(true)) : (Boolean::New(false));

    return handleScope.Close(ret);
}

Handle<Value> CSkinProxy::V8GetClient(const Arguments& args)
{
    HandleScope handleScope;

    Handle<Value> object = v8::Null();
    if (m_pSkinClient != NULL)
        object = m_pSkinClient->GetV8Object();

    return handleScope.Close(object);
}

Handle<Value> CSkinProxy::V8InternalCallMethod(const CString& funcName, const Arguments& args)
{
    if (funcName.CompareNoCase(_T("SetClient")) == 0) {
        return V8SetClient(args);
    } else if (funcName.CompareNoCase(_T("GetClient")) == 0) {
        return V8GetClient(args);
    } 

    return CSkinBase::V8InternalCallMethod(funcName, args);
}
#endif //V8_JS_ENGINE_SUPPORT
