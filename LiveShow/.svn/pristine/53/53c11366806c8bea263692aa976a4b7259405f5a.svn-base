#include "StdAfx.h"

#ifdef V8_JS_ENGINE_SUPPORT

#include "SkinBase.h"

#include "SkinJSEngineV8.h"
#include "Types.h"

#include <map>

using namespace std;

using namespace v8;

CSkinJSEngineV8::CSkinJSEngineV8(void)
{
    HandleScope handleScope;

    m_jsContext.Dispose();
    m_jsContext.Clear();

    m_jsContext = Context::New();
}

CSkinJSEngineV8::~CSkinJSEngineV8(void)
{
    HandleScope handleScope;

    m_jsContext.Dispose();
    m_jsContext.Clear();

    for (size_t i=0; i<m_vecJsScripts.size(); i++) {
        Persistent<Script>* pScript = m_vecJsScripts[i];
        pScript->Dispose();
        pScript->Clear();
        delete pScript;
    }
}

CSkinJSEngineV8::CSkinJSEngineV8(const CSkinJSEngineV8& other)
{
    ASSERT(0 && _T("CSkinJSEngineV8 拷贝构造不可用"));
}

CSkinJSEngineV8& CSkinJSEngineV8::operator = (const CSkinJSEngineV8& other)
{
    ASSERT(0 && _T("CSkinJSEngineV8 operator = 不可用"));
    return *this;
}

BOOL CSkinJSEngineV8::Create(CSkinBase* pBase)
{
    HandleScope handleScope;

    if (pBase == NULL)
        return FALSE;

    m_pSkinBase = pBase;

    return TRUE;
}

BOOL CSkinJSEngineV8::Load(const char* pJS, LPCTSTR fileName)
{
    HandleScope handleScope;

    Context::Scope contextScope(m_jsContext);

    CComBSTR bstrScript;
    bstrScript.Attach(W2BSTR((WCHAR*)(pJS+2)));

    m_strScript = bstrScript;

    TryCatch tryCatch;

    Handle<String> scriptString = CStringToString(m_strScript);
    Handle<String> scriptFileName = CStringToString(fileName);
    Handle<Script> script = Script::Compile(scriptString, scriptFileName);
    if (script.IsEmpty()) {
        CString strException = ValueToCString(tryCatch.Exception());
        TRACE("脚本编译错误");
        CString strError = fileName;
        strError += strException;
        ::MessageBox(NULL, strError, _T("脚本编译错误"), MB_OK | MB_TOPMOST);
        return FALSE;
    }

    Persistent<Script>* pScript = new Persistent<Script>;
    pScript->Dispose();
    pScript->Clear();
    *pScript = Persistent<Script>::New(script);
    m_vecJsScripts.push_back(pScript);

    return TRUE;
}

BOOL CSkinJSEngineV8::InitGlobalNamespace()
{
    HandleScope handleScope;

    Context::Scope contextScope(m_jsContext);

    Handle<Object> global = m_jsContext->Global();

    if (! m_pSkinBase->AddObjectToSrcipt(global))
        return FALSE;

    return TRUE;
}

BOOL CSkinJSEngineV8::Start()
{
    HandleScope handleScope;

    Context::Scope contextScope(m_jsContext);

    TryCatch tryCatch;

    for (size_t i=0; i<m_vecJsScripts.size(); i++) {
        Persistent<Script>* pScript = m_vecJsScripts[i];
        Handle<Value> result = (*pScript)->Run();
        if (result.IsEmpty()) {
            CString strException = ValueToCString(tryCatch.Exception());
            TRACE("脚本启动异常：%s", strException);
            ::MessageBox(NULL, strException, _T("脚本启动错误"), MB_OK | MB_TOPMOST);
            return FALSE;
        }
    }

    return TRUE;
}

BOOL CSkinJSEngineV8::Stop()
{
    return TRUE;
}

BOOL CSkinJSEngineV8::Close()
{
    return TRUE;
}

//获得引擎状态
SKINJSENGINE_STATE CSkinJSEngineV8::GetState() const
{
    return SKINJSENGINE_CLOSE;
}

HRESULT CSkinJSEngineV8::GetProperty(BSTR propName, VARIANT* pPropValue)
{
    HandleScope handleScope;

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

HRESULT CSkinJSEngineV8::CallFunction(BSTR funcName, VARIANT* pVarResult, DISPPARAMS* pDispParams)
{
    HandleScope handleScope;

    Context::Scope contextScope(m_jsContext);

    TryCatch tryCatch;

    Handle<Object> global = m_jsContext->Global();

    Handle<String> funcNameString = CStringToString(funcName);
    Handle<Value> funcValue = global->Get(funcNameString);

    if (funcValue.IsEmpty() || !funcValue->IsFunction()) {
        ASSERT(0 && "函数为空");
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
