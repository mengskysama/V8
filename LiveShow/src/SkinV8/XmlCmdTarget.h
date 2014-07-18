
#pragma once

#include "XmlCmdTargetDefine.h"

#include <set>
#include <list>
#include <boost/dynamic_bitset/dynamic_bitset.hpp>

// CXmlCmdTarget 命令目标

class CXmlCmdTarget : public CCmdTarget
{
	DECLARE_DYNCREATE(CXmlCmdTarget)

public:
	CXmlCmdTarget();
	virtual ~CXmlCmdTarget();

	//初始化函数，必须在对象构造函数之后调用
	virtual void Initialize();

	virtual void OnFinalRelease();

protected:
	DECLARE_MESSAGE_MAP()
	DECLARE_SKIN_DISPATCH_MAP()
	DECLARE_SKIN_DEFAULT_VALUE_MAP()
	DECLARE_INTERFACE_MAP()

    //////////////////////////////////////////////////////////////////////////
    //Script支持
    //////////////////////////////////////////////////////////////////////////
#ifdef V8_JS_ENGINE_SUPPORT

public:
    //V8支持
    virtual BOOL AddObjectToSrcipt(v8::Handle<v8::Object> object);

#endif //V8_JS_ENGINE_SUPPORT

#ifdef MS_JS_ENGINE_SUPPORT

    //实现IActiveScriptSite接口
public:
    BEGIN_INTERFACE_PART(ActiveScriptSite, IActiveScriptSite)
        STDMETHOD(GetLCID)(LCID *plcid);
        STDMETHOD(GetItemInfo)(LPCOLESTR pstrName, DWORD dwReturnMask, IUnknown **ppiunkItem, ITypeInfo **ppti);
        STDMETHOD(GetDocVersionString)(BSTR *pszVersion);
        STDMETHOD(OnScriptTerminate)(const VARIANT *pvarResult, const EXCEPINFO *pexcepinfo);
        STDMETHOD(OnStateChange)(SCRIPTSTATE ssScriptState);
        STDMETHOD(OnScriptError)(IActiveScriptError *pscripterror);
        STDMETHOD(OnEnterScript)(void);
        STDMETHOD(OnLeaveScript)(void);	
    END_INTERFACE_PART(ActiveScriptSite)

protected:
    virtual HRESULT GetLCID(LCID *plcid);
    virtual HRESULT GetItemInfo(LPCOLESTR pstrName, DWORD dwReturnMask, IUnknown **ppiunkItem, ITypeInfo **ppti);
    virtual HRESULT GetDocVersionString(BSTR *pszVersion);
    virtual HRESULT OnScriptTerminate(const VARIANT *pvarResult, const EXCEPINFO *pexcepinfo);
    virtual HRESULT OnStateChange(SCRIPTSTATE ssScriptState);
    virtual HRESULT OnScriptError(IActiveScriptError *pscripterror);
    virtual HRESULT OnEnterScript(void);
    virtual HRESULT OnLeaveScript(void);

    //实现IActiveScriptSiteWindow接口
public:
    BEGIN_INTERFACE_PART(ActiveScriptSiteWindow, IActiveScriptSiteWindow)
        STDMETHOD(GetWindow)(HWND *phwnd);
        STDMETHOD(EnableModeless)(BOOL fEnable);
    END_INTERFACE_PART(ActiveScriptSiteWindow)

protected:
    virtual HRESULT GetWindow(HWND *phwnd);
    virtual HRESULT EnableModeless(BOOL fEnable);

    //向脚本暴露名字空间
public:
    virtual BOOL AddNameItemToSrcipt(CComPtr<IActiveScript> pScript);

#endif //MS_JS_ENGINE_SUPPORT

	//////////////////////////////////////////////////////////////////////////
	//接口查询
	//////////////////////////////////////////////////////////////////////////
public:
	virtual HRESULT GetInterface(IUnknown** ppiunk);

public:
	//////////////////////////////////////////////////////////////////////////
	//Dispatch Map 信息查询
	//////////////////////////////////////////////////////////////////////////
	BOOL GetDispIds(std::list<LONG>& listIds, BOOL bDispProp = TRUE);
	BOOL GetDispName(LONG dispId, CString& strName);
	BOOL GetDispType(LONG dispId, VARENUM& enumType);

	//////////////////////////////////////////////////////////////////////////
	//Dispatch Entry 查询
	//////////////////////////////////////////////////////////////////////////
protected:
	const AFX_DISPMAP_ENTRY* GetDispEntry(BSTR bstrPropName);
	const AFX_DISPMAP_ENTRY* GetDispEntry(LONG propId);

	//////////////////////////////////////////////////////////////////////////
	//Dispatch 调用
	//////////////////////////////////////////////////////////////////////////
public:
	//获得属性
	virtual HRESULT GetProperty(BSTR bstrPropName, VARIANT* pVarPropValue);
	virtual HRESULT GetProperty(LONG propId, VARIANT* pVarPropValue);
	//设置属性
	virtual HRESULT SetProperty(BSTR bstrPropName, VARIANT varPropValue);
	virtual HRESULT SetProperty(LONG propId, VARIANT varPropValue);
	//调用方法
	virtual HRESULT CallMethod(BSTR bstrFuncName, VARIANT* pVarResult, DISPPARAMS* pDispParams);
	virtual HRESULT CallMethod(LONG propId, VARIANT* pVarResult, DISPPARAMS* pDispParams);

    //////////////////////////////////////////////////////////////////////////
    //V8引擎支持
    //////////////////////////////////////////////////////////////////////////
#ifdef V8_JS_ENGINE_SUPPORT
    static v8::Handle<v8::Value> V8GetProperty(v8::Local<v8::String> property, const v8::AccessorInfo& info);
    static void V8SetProperty(v8::Local<v8::String> property, v8::Local<v8::Value> value, const v8::AccessorInfo& info);
    static v8::Handle<v8::Value> V8CallMethod(const v8::Arguments& args);
    static v8::Handle<v8::Value> WrapXmlCmdTarget(CXmlCmdTarget* target);
    static CXmlCmdTarget* UnwrapXmlCmdTarget(v8::Handle<v8::Value> object);
    virtual v8::Handle<v8::Value> V8InternalCallMethod(const CString& funcName, const v8::Arguments& args);
    v8::Handle<v8::Value> GetV8Object();

protected:
    v8::Persistent<v8::Value> m_v8Object;
#endif //V8_JS_ENGINE_SUPPORT

	//////////////////////////////////////////////////////////////////////////
	//从Xml载入属性
	//////////////////////////////////////////////////////////////////////////
public:
	virtual BOOL LoadPropertyFromXml(IXMLDOMNodePtr ptrNode);

	//////////////////////////////////////////////////////////////////////////
	//设置Dispatch标志位
	//////////////////////////////////////////////////////////////////////////
public:
	//修改标识
	BOOL SetModified(LONG dispid, BOOL bTrue);
	BOOL GetModified(LONG dispid, BOOL& bTrue);
	BOOL ClearModified();

private:
	boost::dynamic_bitset<> m_bitsetModified;	//用于标识属性是否被设置过
	BOOL m_bInitialized;
};
