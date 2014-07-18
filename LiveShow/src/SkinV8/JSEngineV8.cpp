// JSEngineV8.cpp : implementation file
//

#include "stdafx.h"
#include "JSEngineV8.h"
#include "XmlCmdTarget.h"
#include "Types.h"

#ifdef V8_JS_ENGINE_SUPPORT

using namespace v8;

// CJSEngineV8

IMPLEMENT_DYNAMIC(CJSEngineV8, CCmdTarget)
DELEGATE_DUAL_INTERFACE(CJSEngineV8, JSEngine)


CJSEngineV8::CJSEngineV8()
{
    HandleScope handleScope;

    m_jsContext.Dispose();
    m_jsContext.Clear();
    m_jsScript.Dispose();
    m_jsScript.Clear();
}

CJSEngineV8::~CJSEngineV8()
{
    HandleScope handleScope;

    m_jsContext.Dispose();
    m_jsContext.Clear();
    m_jsScript.Dispose();
    m_jsScript.Clear();
}

HRESULT CJSEngineV8::Initialize(CXmlCmdTarget* pObject)
{
    HandleScope handleScope;

    if (! m_jsContext.IsEmpty())
        return S_FALSE;

    m_pScriptGlobalObject = pObject;
    return S_OK;
}


BEGIN_MESSAGE_MAP(CJSEngineV8, CCmdTarget)
END_MESSAGE_MAP()

// Note: we add support for IID_ISkinEngine to support typesafe binding
//  from VBA.  This IID must match the GUID that is attached to the 
//  dispinterface in the .IDL file.

// {7F0536A8-ED3F-43b8-816C-137A695185B6}
__declspec(selectany) const IID IID_IJSEngine =
{ 0x7F0536A8, 0xED3F, 0x43b8, { 0x81, 0x6C, 0x13, 0x7A, 0x69, 0x51, 0x85, 0xB6 } };

BEGIN_INTERFACE_MAP(CJSEngineV8, CCmdTarget)
    INTERFACE_PART(CJSEngineV8, IID_IJSEngine, JSEngine)
END_INTERFACE_MAP()


// CJSEngineV8 message handlers
HRESULT CJSEngineV8::XJSEngine::InitNew()
{
    METHOD_PROLOGUE(CJSEngineV8, JSEngine);

    return pThis->InitNew();
}

HRESULT CJSEngineV8::XJSEngine::Load(BSTR bstrScript, BSTR bstrScriptName)
{
    METHOD_PROLOGUE(CJSEngineV8, JSEngine);

    return pThis->Load(bstrScript, bstrScriptName);
}

HRESULT CJSEngineV8::XJSEngine::Run(VARIANT* pVarResult)
{
    METHOD_PROLOGUE(CJSEngineV8, JSEngine);

    return pThis->Run(pVarResult);
}

HRESULT CJSEngineV8::XJSEngine::Close()
{
    METHOD_PROLOGUE(CJSEngineV8, JSEngine);

    return pThis->Close();
}

HRESULT CJSEngineV8::XJSEngine::GetState(JSENGINE_STATE* pState)
{
    METHOD_PROLOGUE(CJSEngineV8, JSEngine);

    return pThis->GetState(pState);
}

HRESULT CJSEngineV8::XJSEngine::GetProperty(BSTR propName, VARIANT* pPropValue)
{
    METHOD_PROLOGUE(CJSEngineV8, JSEngine);

    return pThis->GetProperty(propName, pPropValue);
}

HRESULT CJSEngineV8::XJSEngine::SetProperty(BSTR propName, VARIANT* pPropValue)
{
    METHOD_PROLOGUE(CJSEngineV8, JSEngine);

    return pThis->SetProperty(propName, pPropValue);
}

HRESULT CJSEngineV8::XJSEngine::CallFunction(BSTR bstrFuncName, VARIANT* pVarResult, DISPPARAMS* pDispParams)
{
    METHOD_PROLOGUE(CJSEngineV8, JSEngine);

    return pThis->CallFunction(bstrFuncName, pVarResult, pDispParams);
}

HRESULT CJSEngineV8::InitNew()
{
    HandleScope handleScope;

    if (! m_jsContext.IsEmpty())
        return S_FALSE;

    m_jsContext = Context::New();

    if (m_pScriptGlobalObject == NULL)
        return S_OK;

    Handle<Object> global = m_jsContext->Global();

    BOOL bSuc = m_pScriptGlobalObject->AddObjectToSrcipt(global);
    ASSERT(bSuc);

    return S_OK;
}

HRESULT CJSEngineV8::Load(BSTR bstrScript, BSTR bstrScriptName)
{
    HandleScope handleScope;

    if (m_jsContext.IsEmpty())
        return S_FALSE;

    Context::Scope contextScope(m_jsContext);

    TryCatch tryCatch;

    Handle<String> scriptString = CStringToString(bstrScript);
    Handle<String> scriptFileName = CStringToString(bstrScriptName);
    Handle<Script> script = Script::Compile(scriptString, scriptFileName);
    if (script.IsEmpty()) {
        CString strException = ValueToCString(tryCatch.Exception());
        ASSERT(0 && "½Å±¾±àÒë´íÎó");
        CString strError = strException;
        return S_FALSE;
    }

    script->Run();

    return S_OK;
}

HRESULT CJSEngineV8::Run(VARIANT* pVarResult)
{
    HandleScope handleScope;

    if (m_jsContext.IsEmpty())
        return S_FALSE;

    return S_OK;
}

HRESULT CJSEngineV8::Close()
{
    HandleScope handleScope;

    m_jsContext.Dispose();
    m_jsContext.Clear();
    m_jsScript.Dispose();
    m_jsScript.Clear();

    m_pScriptGlobalObject = NULL;
    return S_OK;
}

HRESULT CJSEngineV8::GetState(JSENGINE_STATE* pState)
{
    HandleScope handleScope;

    if (m_jsContext.IsEmpty())
        return S_FALSE;

    return S_OK;
}

HRESULT CJSEngineV8::GetProperty(BSTR propName, VARIANT* pPropValue)
{
    HandleScope handleScope;

    if (m_jsContext.IsEmpty())
        return S_FALSE;

    Context::Scope contextScope(m_jsContext);

    TryCatch tryCatch;

    Handle<Object> global = m_jsContext->Global();

    Handle<String> propNameString = CStringToString(propName);
    Handle<Value> propValue = global->Get(propNameString);

    if (propValue->IsFunction()) {
        return S_FALSE;
    }

    if (pPropValue != NULL) {
        CComVariant varResult = ValueToVariant(propValue);
        ::VariantClear(pPropValue);
        ::VariantCopy(pPropValue, &varResult);
    }

    return S_OK;
}

HRESULT CJSEngineV8::SetProperty(BSTR propName, VARIANT* pPropValue)
{
    HandleScope handleScope;

    if (m_jsContext.IsEmpty())
        return S_FALSE;

    Context::Scope contextScope(m_jsContext);

    TryCatch tryCatch;

    Handle<Object> global = m_jsContext->Global();

    Handle<String> propNameString = CStringToString(propName);
    CComVariant varValue = *pPropValue;
    Handle<Value> propValue = VariantToValue(varValue);
    bool bSuc = global->Set(propNameString, propValue);

    return bSuc ? (S_OK) : (S_FALSE);
}

HRESULT CJSEngineV8::CallFunction(BSTR bstrFuncName, VARIANT* pVarResult, DISPPARAMS* pDispParams)
{
    HandleScope handleScope;

    if (m_jsContext.IsEmpty())
        return S_FALSE;

    Context::Scope contextScope(m_jsContext);

    TryCatch tryCatch;

    Handle<Object> global = m_jsContext->Global();

    Handle<String> funcName = CStringToString(bstrFuncName);
    Handle<Value> funcValue = global->Get(funcName);

    if (funcValue.IsEmpty() || !funcValue->IsFunction()) {
        ASSERT(0 && "º¯ÊýÎª¿Õ");
        return S_FALSE;
    }

    Handle<Function> func = Handle<Function>::Cast(funcValue);

    const UINT argNum = (pDispParams != NULL) ? (pDispParams->cArgs) : (0);
    Handle<Value> *vals = new Handle<Value>[argNum];
    for (UINT i=0; i<argNum; i++) {
        vals[i] = ToValue(pDispParams->rgvarg[argNum-i-1]);
    }

    Handle<Value> result = func->Call(global, argNum, vals);

    delete [] vals;

    if (pVarResult != NULL) {
        CComVariant varResult = ValueToVariant(result);
        ::VariantClear(pVarResult);
        ::VariantCopy(pVarResult, &varResult);
    }

    return S_OK;
}

#endif //V8_JS_ENGINE_SUPPORT