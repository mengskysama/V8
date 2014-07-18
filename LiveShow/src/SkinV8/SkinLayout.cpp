// SkinLayout.cpp : implementation file
//

#include "stdafx.h"
#include "Skin.h"
#include "SkinLayout.h"
#include "SkinBase.h"
#include "SkinHelper.h"


// CSkinLayout

IMPLEMENT_DYNCREATE(CSkinLayout, CXmlCmdTarget)


CSkinLayout::CSkinLayout()
{
	EnableAutomation();
	
	// To keep the application running as long as an OLE automation 
	//	object is active, the constructor calls AfxOleLockApp.
    m_nLeftMargin = 0;
    m_nTopMargin = 0;
    m_nRightMargin = 0;
    m_nBottomMargin = 0;
    m_pHost = NULL;

	AfxOleLockApp();
}

CSkinLayout::~CSkinLayout()
{
	// To terminate the application when all objects created with
	// 	with OLE automation, the destructor calls AfxOleUnlockApp.
	
	AfxOleUnlockApp();
}


void CSkinLayout::OnFinalRelease()
{
	// When the last reference for an automation object is released
	// OnFinalRelease is called.  The base class will automatically
	// deletes the object.  Add additional cleanup required for your
	// object before calling the base class.

	CCmdTarget::OnFinalRelease();
}


BEGIN_MESSAGE_MAP(CSkinLayout, CXmlCmdTarget)
END_MESSAGE_MAP()


BEGIN_SKIN_DISPATCH_MAP(CSkinLayout, CXmlCmdTarget)
    SKIN_DISP_PROPERTY(CSkinLayout, leftMargin, VT_I4)
    SKIN_DISP_PROPERTY(CSkinLayout, topMargin, VT_I4)
    SKIN_DISP_PROPERTY(CSkinLayout, rightMargin, VT_I4)
    SKIN_DISP_PROPERTY(CSkinLayout, bottomMargin, VT_I4)

    SKIN_DISP_FUNCTION(CSkinLayout, Dock, VT_BOOL, VTS_BSTR VTS_I4 VTS_I4 VTS_I4 VTS_I4 VTS_I4 VTS_I4 VTS_I4)
    SKIN_DISP_FUNCTION(CSkinLayout, Undock, VT_BOOL, VTS_BSTR)
    SKIN_DISP_FUNCTION(CSkinLayout, UndockAll, VT_BOOL, VTS_NONE)
    SKIN_DISP_FUNCTION(CSkinLayout, Layout, VT_BOOL, VTS_NONE)
END_SKIN_DISPATCH_MAP(CSkinLayout)

// Note: we add support for IID_ISkinLayout to support typesafe binding
//  from VBA.  This IID must match the GUID that is attached to the 
//  dispinterface in the .IDL file.

// {B181E76B-B65F-49FE-ACF2-5ED0499C5873}
static const IID IID_ISkinLayout =
{ 0xB181E76B, 0xB65F, 0x49FE, { 0xAC, 0xF2, 0x5E, 0xD0, 0x49, 0x9C, 0x58, 0x73 } };

BEGIN_INTERFACE_MAP(CSkinLayout, CXmlCmdTarget)
	INTERFACE_PART(CSkinLayout, IID_ISkinLayout, Dispatch)
END_INTERFACE_MAP()

// {5D8D1854-0B77-4276-92C5-3B72E45C5EF4}
IMPLEMENT_OLECREATE_FLAGS(CSkinLayout, "Skin.SkinLayout", afxRegApartmentThreading, 0x5d8d1854, 0xb77, 0x4276, 0x92, 0xc5, 0x3b, 0x72, 0xe4, 0x5c, 0x5e, 0xf4)


// CSkinLayout message handlers
LONG CSkinLayout::GetleftMargin()
{
    return m_nLeftMargin;
}

void CSkinLayout::SetleftMargin(LONG newVal)
{
    m_nLeftMargin = newVal;
}

LONG CSkinLayout::GettopMargin()
{
    return m_nTopMargin;
}

void CSkinLayout::SettopMargin(LONG newVal)
{
    m_nTopMargin = newVal;
}

LONG CSkinLayout::GetrightMargin()
{
    return m_nRightMargin;
}

void CSkinLayout::SetrightMargin(LONG newVal)
{
    m_nRightMargin = newVal;
}

LONG CSkinLayout::GetbottomMargin()
{
    return m_nBottomMargin;
}

void CSkinLayout::SetbottomMargin(LONG newVal)
{
    m_nBottomMargin = newVal;
}

void CSkinLayout::SetHost(CSkinBase* pHost)
{
    m_pHost = pHost;
}

VARIANT_BOOL CSkinLayout::Dock(LPCTSTR lpzName, LONG nMode, LONG nInitialX, LONG nInitialY, LONG nLeftMargin, LONG nTopMargin, LONG nRightMargin, LONG nBottomMargin)
{
    ChildInfo childInfo;
    childInfo.strChildName = lpzName;
    childInfo.nInitialX = nInitialX;
    childInfo.nInitialY = nInitialY;
    childInfo.dockInfo.nAlignment = nMode;
    childInfo.dockInfo.nLeftMargin = nLeftMargin;
    childInfo.dockInfo.nTopMargin = nTopMargin;
    childInfo.dockInfo.nRightMargin = nRightMargin;
    childInfo.dockInfo.nBottomMargin = nBottomMargin;

    m_vecChildInfo.push_back(childInfo);

    return VARIANT_TRUE;
}

VARIANT_BOOL CSkinLayout::Undock(LPCTSTR lpzName)
{
    ChildArray::iterator iter = m_vecChildInfo.begin(),
        last = m_vecChildInfo.end();
    bool bFinded = false;
    for (; iter!=last; iter++) {
        if (iter->strChildName.CompareNoCase(lpzName) == 0) {
            m_vecChildInfo.erase(iter);
            bFinded = true;
            break;
        }
    }

    return bFinded ? (VARIANT_TRUE) : (VARIANT_FALSE);
}

VARIANT_BOOL CSkinLayout::Layout()
{
    if (m_pHost == NULL)
        return VARIANT_FALSE;

    CRect rcClient = m_pHost->GetSkinAbsRect();

    CRect rcRemain(rcClient);
    rcRemain.DeflateRect(m_nLeftMargin, m_nTopMargin, m_nRightMargin, m_nBottomMargin);

    ChildArray::iterator iter = m_vecChildInfo.begin();
    for (; iter!=m_vecChildInfo.end(); iter++) {
        ChildInfo& childInfo = *iter;
        DockInfo& dockInfo = childInfo.dockInfo;

        CSkinBase* pChild = m_pHost->GetSubSkin(childInfo.strChildName);
        ASSERT(pChild != NULL);
        if (pChild == NULL)
            continue;

        CRect rcChild(rcRemain);
        switch (dockInfo.nAlignment)
        {
        case LEFT:
            rcChild.right = min(rcChild.left + childInfo.nInitialX, rcRemain.right);
            rcRemain.left = rcChild.right;
            break;

        case TOP:
            rcChild.left = min(rcChild.right - childInfo.nInitialX, rcRemain.left);
            rcRemain.right = rcChild.left;
            break;

        case RIGHT:
            rcChild.bottom = min(rcChild.top + childInfo.nInitialY, rcRemain.bottom);
            rcRemain.top = rcChild.bottom;
            break;

        case BOTTOM:
            rcChild.top = max(rcChild.bottom - childInfo.nInitialY, rcRemain.top);
            rcRemain.bottom = rcChild.top;
            break;

        case FILL:
            rcChild = rcRemain;
            rcRemain.SetRectEmpty();
            break;

        default:
            continue;
        }

        rcChild.DeflateRect(dockInfo.nLeftMargin, dockInfo.nTopMargin, dockInfo.nRightMargin, dockInfo.nBottomMargin);

        pChild->Setleft(rcChild.left);
        pChild->Settop(rcChild.top);
        pChild->Setwidth(rcChild.Width());
        pChild->Setheight(rcChild.Height());
    }

    return VARIANT_TRUE;
}

VARIANT_BOOL CSkinLayout::UndockAll()
{
    m_vecChildInfo.clear();

    return VARIANT_TRUE;
}

#ifdef V8_JS_ENGINE_SUPPORT

v8::Handle<v8::Value> CSkinLayout::V8Dock(const v8::Arguments& args)
{
    v8::HandleScope handleScope;

    v8::Handle<v8::Value> object = v8::Null();

    CString strName = ValueToCString(v8::Handle<v8::String>::Cast(args[0]));
    UINT iMode = v8::Handle<v8::Integer>::Cast(args[1])->Uint32Value();
    UINT iInitialX = v8::Handle<v8::Integer>::Cast(args[2])->Uint32Value();
    UINT iInitialY = v8::Handle<v8::Integer>::Cast(args[3])->Uint32Value();
    UINT iLeftMargin = v8::Handle<v8::Integer>::Cast(args[4])->Uint32Value();
    UINT iTopMargin = v8::Handle<v8::Integer>::Cast(args[5])->Uint32Value();
    UINT iRightMargin = v8::Handle<v8::Integer>::Cast(args[6])->Uint32Value();
    UINT iBottomMargin = v8::Handle<v8::Integer>::Cast(args[6])->Uint32Value();
    v8::Handle<v8::Value> ret = Dock(strName, iMode, iInitialX, iInitialY, iLeftMargin, iTopMargin, iRightMargin, iBottomMargin) 
         ? (v8::Boolean::New(true)) : (v8::Boolean::New(false));

    return handleScope.Close(ret);
}
 
v8::Handle<v8::Value> CSkinLayout::V8Undock(const v8::Arguments& args)
{
    v8::HandleScope handleScope;

    CString strName = ValueToCString(v8::Handle<v8::String>::Cast(args[0]));
    v8::Handle<v8::Value> ret = Undock(strName) 
        ? (v8::Boolean::New(true)) : (v8::Boolean::New(false));

    return handleScope.Close(ret);
}

v8::Handle<v8::Value> CSkinLayout::V8Layout(const v8::Arguments& args)
{
    v8::HandleScope handleScope;

    v8::Handle<v8::Value> ret = Layout() 
        ? (v8::Boolean::New(true)) : (v8::Boolean::New(false));

    return handleScope.Close(ret);
}

v8::Handle<v8::Value> CSkinLayout::V8UndockAll(const v8::Arguments& args)
{
    v8::HandleScope handleScope;

    v8::Handle<v8::Value> ret = UndockAll() 
        ? (v8::Boolean::New(true)) : (v8::Boolean::New(false));

    return handleScope.Close(ret);
}

v8::Handle<v8::Value> CSkinLayout::V8InternalCallMethod(const CString& funcName, const v8::Arguments& args)
{
    if (funcName.CompareNoCase(_T("Dock")) == 0) {
        return V8Dock(args);
    } else if (funcName.CompareNoCase(_T("Undock")) == 0) {
        return V8Undock(args);
    } else if (funcName.CompareNoCase(_T("Layout")) == 0) {
        return V8Layout(args);
    } else if (funcName.CompareNoCase(_T("UndockAll")) == 0) {
        return V8UndockAll(args);
    }

    return CXmlCmdTarget::V8InternalCallMethod(funcName, args);
}

#endif //V8_JS_ENGINE_SUPPORT
