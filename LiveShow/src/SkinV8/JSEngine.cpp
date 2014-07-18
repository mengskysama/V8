// JSEngineV8.cpp : implementation file
//

#include "stdafx.h"
#include "JSEngine.h"
#include "XmlCmdTarget.h"

#ifdef MS_JS_ENGINE_SUPPORT

// CJSEngine

IMPLEMENT_DYNAMIC(CJSEngine, CCmdTarget)
DELEGATE_DUAL_INTERFACE(CJSEngine, JSEngine)


CJSEngine::CJSEngine()
{

}

CJSEngine::~CJSEngine()
{
    Close();
}

HRESULT CJSEngine::Initialize(CXmlCmdTarget* pObject)
{
    if (m_pActiveScript != NULL)
        return S_FALSE;

    m_pScriptGlobalObject = pObject;
    return S_OK;
}


BEGIN_MESSAGE_MAP(CJSEngine, CCmdTarget)
END_MESSAGE_MAP()

// Note: we add support for IID_ISkinEngine to support typesafe binding
//  from VBA.  This IID must match the GUID that is attached to the 
//  dispinterface in the .IDL file.

// {7F0536A8-ED3F-43b8-816C-137A695185B6}
__declspec(selectany) const IID IID_IJSEngine =
{ 0x7F0536A8, 0xED3F, 0x43b8, { 0x81, 0x6C, 0x13, 0x7A, 0x69, 0x51, 0x85, 0xB6 } };

BEGIN_INTERFACE_MAP(CJSEngine, CCmdTarget)
    INTERFACE_PART(CJSEngine, IID_IJSEngine, JSEngine)
END_INTERFACE_MAP()


// CJSEngine message handlers
HRESULT CJSEngine::XJSEngine::InitNew()
{
    METHOD_PROLOGUE(CJSEngine, JSEngine);

    return pThis->InitNew();
}

HRESULT CJSEngine::XJSEngine::Load(BSTR bstrScript, BSTR bstrScriptName)
{
    METHOD_PROLOGUE(CJSEngine, JSEngine);

    return pThis->Load(bstrScript, bstrScriptName);
}

HRESULT CJSEngine::XJSEngine::Run(VARIANT* pVarResult)
{
    METHOD_PROLOGUE(CJSEngine, JSEngine);

    return pThis->Run(pVarResult);
}

HRESULT CJSEngine::XJSEngine::Close()
{
    METHOD_PROLOGUE(CJSEngine, JSEngine);

    return pThis->Close();
}

HRESULT CJSEngine::XJSEngine::GetState(JSENGINE_STATE* pState)
{
    METHOD_PROLOGUE(CJSEngine, JSEngine);

    return pThis->GetState(pState);
}

HRESULT CJSEngine::XJSEngine::GetProperty(BSTR propName, VARIANT* pPropValue)
{
    METHOD_PROLOGUE(CJSEngine, JSEngine);

    return pThis->GetProperty(propName, pPropValue);
}

HRESULT CJSEngine::XJSEngine::SetProperty(BSTR propName, VARIANT* pPropValue)
{
    METHOD_PROLOGUE(CJSEngine, JSEngine);

    return pThis->SetProperty(propName, pPropValue);
}

HRESULT CJSEngine::XJSEngine::CallFunction(BSTR bstrFuncName, VARIANT* pVarResult, DISPPARAMS* pDispParams)
{
    METHOD_PROLOGUE(CJSEngine, JSEngine);

    return pThis->CallFunction(bstrFuncName, pVarResult, pDispParams);
}

HRESULT CJSEngine::InitNew()
{
    if (m_pActiveScript != NULL)
        return S_FALSE;

    CComPtr<IActiveScript> activeScript;

    HRESULT hr = S_OK;

    //创建脚本运行环境
    hr = ::CoCreateInstance(CLSID_JAVAScript, NULL, CLSCTX_INPROC, 
        IID_IActiveScript, (void**)&activeScript);
    if (FAILED(hr)){
        TRACE(("创建脚本运行环境失败\n"));
        return S_FALSE;
    }

    if (m_pScriptGlobalObject == NULL)
        m_pScriptGlobalObject = CCmdTargetPtr<CXmlCmdTarget>(new CXmlCmdTarget);

    //创建脚本Site
    CComPtr<IActiveScriptSite> pSite;
    GUID guid = IID_IActiveScriptSite;
    hr = m_pScriptGlobalObject->ExternalQueryInterface(&guid,(void**)&pSite);
    if (FAILED(hr) || pSite == NULL) {
        TRACE(("创建脚本Site失败\n"));
        return S_FALSE;
    }

    //设置Site
    hr = activeScript->SetScriptSite(pSite);
    if(FAILED(hr)) {
        TRACE(("设置脚本Site失败\n"));
        return S_FALSE;
    }

    BOOL bSuc = m_pScriptGlobalObject->AddNameItemToSrcipt(activeScript);
    ASSERT(bSuc);

    m_pActiveScript = activeScript;
    return S_OK;
}

HRESULT CJSEngine::Load(BSTR bstrScript, BSTR bstrScriptName)
{
    if (m_pActiveScript == NULL)
        return S_FALSE;

    HRESULT hr = S_OK;

    //获得脚本解析器
    CComPtr<IActiveScriptParse> srciptParser;
    hr = m_pActiveScript->QueryInterface(IID_IActiveScriptParse, (void**)&srciptParser);
    if (FAILED(hr)) {
        TRACE(("创建脚本解析器失败\n"));
        return S_FALSE;
    }

    //初始化脚本解析器
    hr = srciptParser->InitNew();
    if (FAILED(hr)) {
        TRACE(("初始化脚本解析器失败\n"));
        return S_FALSE;
    }

    //解析脚本
    hr = srciptParser->ParseScriptText(bstrScript, NULL, NULL, 
        NULL, 0, 0, SCRIPTTEXT_ISVISIBLE, NULL, NULL);
    if (FAILED(hr)) {
        TRACE(("脚本解析失败\n"));
        return S_FALSE;
    }

    return hr;
}

HRESULT CJSEngine::Run(VARIANT* pVarResult)
{
    if (m_pActiveScript == NULL)
        return S_FALSE;

    HRESULT hr = S_OK;

    //启动脚本
    hr = m_pActiveScript->SetScriptState(SCRIPTSTATE_CONNECTED);
    if (FAILED(hr)) {
        TRACE(("脚本启动失败\n"));
        return S_FALSE;
    }

    return hr;
}

HRESULT CJSEngine::Close()
{
    if (m_pActiveScript == NULL)
        return S_FALSE;

    m_pActiveScript->Close();
    m_pActiveScript = NULL;

    m_pScriptGlobalObject = NULL;

    return S_OK;
}

HRESULT CJSEngine::GetState(JSENGINE_STATE* pState)
{
    if (m_pActiveScript == NULL)
        return S_FALSE;

    return S_OK;
}

HRESULT CJSEngine::GetProperty(BSTR propName, VARIANT* pPropValue)
{
    if (m_pActiveScript == NULL)
        return S_FALSE;

    HRESULT hr = S_OK;

    //获得全局对象
    CComPtr<IDispatch> dispGlobal;
    hr = m_pActiveScript->GetScriptDispatch(NULL, &dispGlobal);
    if (FAILED(hr) || dispGlobal == NULL) {
        TRACE(("脚本变量获取失败\n"));
        return S_FALSE;
    }

    //查询
    DISPID funcID = -1;
    OLECHAR *rgszNames[] = {propName};
    hr = dispGlobal->GetIDsOfNames(IID_NULL, rgszNames, 1, 0, &funcID);
    if (FAILED(hr)) {
        TRACE(("脚本变量获取失败\n"));
        return S_FALSE;
    }

    //获得脚本变量
    hr = dispGlobal->Invoke(funcID, IID_NULL, 0, DISPATCH_PROPERTYGET, NULL, pPropValue, NULL, NULL);
    if (FAILED(hr)) {
        TRACE(("获得脚本变量失败\n"));
        return S_FALSE;
    }

    return hr;
}

HRESULT CJSEngine::SetProperty(BSTR propName, VARIANT* pPropValue)
{
    return S_FALSE;
}

HRESULT CJSEngine::CallFunction(BSTR bstrFuncName, VARIANT* pVarResult, DISPPARAMS* pDispParams)
{
    if (m_pActiveScript == NULL)
        return S_FALSE;

    HRESULT hr = S_OK;

    //获得全局对象
    CComPtr<IDispatch> dispGlobal;
    hr = m_pActiveScript->GetScriptDispatch(NULL, &dispGlobal);
    if (FAILED(hr) || dispGlobal == NULL) {
        TRACE(("脚本函数获取失败\n"));
        return S_FALSE;
    }

    //查询函数
    DISPID funcID = -1;
    OLECHAR *rgszNames[] = {bstrFuncName};
    hr = dispGlobal->GetIDsOfNames(IID_NULL, rgszNames, 1, 0, &funcID);
    if (FAILED(hr)) {
        TRACE(("脚本函数获取失败\n"));
        return S_FALSE;
    }

    //调用函数
    DISPPARAMS dispParams = {NULL, 0, 0, 0};
    if (pDispParams == NULL)
        pDispParams = &dispParams;
    hr = dispGlobal->Invoke(funcID, IID_NULL, 0, DISPATCH_METHOD, pDispParams, pVarResult, NULL, NULL);
    if (FAILED(hr)) {
        TRACE(("脚本函数执行失败\n"));
        return S_FALSE;
    }

    return hr;
}

#endif //MS_JS_ENGINE_SUPPORT