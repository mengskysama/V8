
#pragma once

#include "XmlCmdTarget.h"
#include "ILayout.h"
#include "ILayoutItem.h"

class CSkinBase;

// CSkinLayout command target

class CSkinLayout : 
    public CXmlCmdTarget,
    public ILayout
{
	DECLARE_DYNCREATE(CSkinLayout)

public:
	CSkinLayout();
	virtual ~CSkinLayout();

	virtual void OnFinalRelease();


protected:
    DECLARE_MESSAGE_MAP()
    DECLARE_OLECREATE(CSkinLayout)
    DECLARE_SKIN_DISPATCH_MAP()
    DECLARE_INTERFACE_MAP()

protected:
    enum 
    {
        SKIN_DISPID(CSkinLayout, leftMargin),
        SKIN_DISPID(CSkinLayout, topMargin),
        SKIN_DISPID(CSkinLayout, rightMargin),
        SKIN_DISPID(CSkinLayout, bottomMargin),

        SKIN_DISPID(CSkinLayout, Dock),
        SKIN_DISPID(CSkinLayout, Undock),
        SKIN_DISPID(CSkinLayout, UndockAll),
        SKIN_DISPID(CSkinLayout, Layout),
    };

    LONG GetleftMargin();
    void SetleftMargin(LONG newVal);
    LONG GettopMargin();
    void SettopMargin(LONG newVal);
    LONG GetrightMargin();
    void SetrightMargin(LONG newVal);
    LONG GetbottomMargin();
    void SetbottomMargin(LONG newVal);

public:
    void SetHost(CSkinBase* pHost);

protected:
    typedef struct tagDockInfo
    {
        LONG nLeftMargin;
        LONG nTopMargin;
        LONG nRightMargin;
        LONG nBottomMargin;
        LONG nAlignment;
    } DockInfo;

    typedef struct tagChildInfo
    {
        CString strChildName;
        LONG nInitialX;
        LONG nInitialY;
        DockInfo dockInfo;
    } ChildInfo;

    typedef std::vector<ChildInfo> ChildArray;

public:
    VARIANT_BOOL Dock(LPCTSTR lpzName, LONG iMode, LONG iInitialX, LONG iInitialY, LONG iLeftMargin, LONG iTopMargin, LONG iRightMargin, LONG iBottomMargin);
    VARIANT_BOOL Undock(LPCTSTR lpzName);
    VARIANT_BOOL Layout();
    VARIANT_BOOL UndockAll();

#ifdef V8_JS_ENGINE_SUPPORT

    //v8й╣ож
protected:
    v8::Handle<v8::Value> V8Dock(const v8::Arguments& args);
    v8::Handle<v8::Value> V8Undock(const v8::Arguments& args);
    v8::Handle<v8::Value> V8Layout(const v8::Arguments& args);
    v8::Handle<v8::Value> V8UndockAll(const v8::Arguments& args);

protected:
    virtual v8::Handle<v8::Value> V8InternalCallMethod(const CString& funcName, const v8::Arguments& args);

#endif //V8_JS_ENGINE_SUPPORT

protected:
    LONG m_nLeftMargin;
    LONG m_nTopMargin;
    LONG m_nRightMargin;
    LONG m_nBottomMargin;
    CSkinBase* m_pHost;
    ChildArray m_vecChildInfo;
};
