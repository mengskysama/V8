
#pragma once

#ifdef V8_JS_ENGINE_SUPPORT

#include "Skin_i.h"
#include "CmdTargetPtr.h"

class CXmlCmdTarget;

// CJSEngineV8 command target
class CJSEngineV8 : public CCmdTarget
{
	DECLARE_DYNAMIC(CJSEngineV8)
    DECLARE_INTERFACE_MAP()

public:
	CJSEngineV8();
	virtual ~CJSEngineV8();

public:
    BEGIN_DUAL_INTERFACE_PART(JSEngine, IJSEngine)
        STDMETHOD(InitNew)();
        STDMETHOD(Load)(BSTR bstrScript, BSTR bstrScriptName);
        STDMETHOD(Run)(VARIANT* pVarResult);
        STDMETHOD(Close)();
        STDMETHOD(GetState)(JSENGINE_STATE* pState);
        STDMETHOD(GetProperty)(BSTR propName, VARIANT* pPropValue);
        STDMETHOD(SetProperty)(BSTR propName, VARIANT* pPropValue);
        STDMETHOD(CallFunction)(BSTR bstrFuncName, VARIANT* pVarResult, DISPPARAMS* pDispParams);
    END_DUAL_INTERFACE_PART(JSEngine)

public:
    //ISkinJSEngine
    virtual HRESULT Initialize(CXmlCmdTarget* pSkinObject);
    virtual HRESULT InitNew();
    virtual HRESULT Load(BSTR bstrScript, BSTR bstrScriptName);
    virtual HRESULT Run(VARIANT* pVarResult);
    virtual HRESULT Close();
    virtual HRESULT GetState(JSENGINE_STATE* pState);
    virtual HRESULT GetProperty(BSTR propName, VARIANT* pPropValue);
    virtual HRESULT SetProperty(BSTR propName, VARIANT* pPropValue);
    virtual HRESULT CallFunction(BSTR bstrFuncName, VARIANT* pVarResult, DISPPARAMS* pDispParams);

protected:
	DECLARE_MESSAGE_MAP()

protected:
    v8::Persistent<v8::Context> m_jsContext;
    v8::Persistent<v8::Script> m_jsScript;
    CCmdTargetPtr<CXmlCmdTarget> m_pScriptGlobalObject;
};

#endif //V8_JS_ENGINE_SUPPORT