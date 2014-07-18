#include "StdAfx.h"

#ifdef MS_JS_ENGINE_SUPPORT

#include "SkinBase.h"
#include "SkinJSEngine.h"

CSkinJSEngine::CSkinJSEngine(void)
{

}

CSkinJSEngine::~CSkinJSEngine(void)
{

}

CSkinJSEngine::CSkinJSEngine(const CSkinJSEngine& other)
{
	ASSERT(0 && _T("CSkinJSEngine 拷贝构造不可用"));
}

CSkinJSEngine& CSkinJSEngine::operator = (const CSkinJSEngine& other)
{
	ASSERT(0 && _T("CSkinJSEngine operator = 不可用"));
	return *this;
}

BOOL CSkinJSEngine::Create(CSkinBase* pBase)
{
	if (pBase == NULL)
		return FALSE;

	if (m_pActiveScript != NULL)
		return FALSE;

	HRESULT hr = S_FALSE;

	//创建脚本Site
	CComPtr<IActiveScriptSite> pSite;
	GUID guid = IID_IActiveScriptSite;
	hr = pBase->ExternalQueryInterface(&guid,(void**)&pSite);
	if (FAILED(hr) || pSite == NULL){
		TRACE(("创建脚本Site失败\n"));
		return FALSE;
	}

	//创建脚本运行环境
	hr = ::CoCreateInstance(CLSID_JAVAScript, NULL, CLSCTX_INPROC, 
		IID_IActiveScript, (void**)&m_pActiveScript);
	if (FAILED(hr)){
		TRACE(("创建脚本运行环境失败\n"));
		return FALSE;
	}

	//设置脚本Site
	hr = m_pActiveScript->SetScriptSite(pSite);
	if(FAILED(hr)){
		TRACE(("设置脚本Site失败\n"));
		return FALSE;
	}

    CComQIPtr<IActiveScriptParse> scriptParser = m_pActiveScript;
    if (scriptParser == NULL)
        return FALSE;

    //初始化脚本解析器
    hr = scriptParser->InitNew();
    if (FAILED(hr)) {
        TRACE(("初始化脚本解析器失败\n"));
        return FALSE;
    }

    m_pSkinBase = pBase;

	return TRUE;
}

BOOL CSkinJSEngine::Load(const char* pJS, LPCTSTR fileName)
{
	SKINJSENGINE_STATE state = GetState();
	if (state != SKINJSENGINE_NOTLOAD && state != SKINJSENGINE_LOAD)
		return FALSE;

    HRESULT hr = S_FALSE;

	//获得脚本解析器
	CComQIPtr<IActiveScriptParse> scriptParser = m_pActiveScript;
	if (scriptParser == NULL) {
		TRACE(("创建脚本解析器失败\n"));
		return FALSE;
	}

	CComBSTR bstrScript;
	bstrScript.Attach(W2BSTR((WCHAR*)(pJS+2)));

	//解析脚本
	try{
		hr = scriptParser->ParseScriptText(bstrScript, NULL, NULL, 
			NULL, 0, 0, SCRIPTTEXT_ISVISIBLE, NULL, NULL);
	} catch(...) {
		ASSERT(0 && _T("脚本解析异常"));
		return FALSE;
	}

	if (FAILED(hr))
		return FALSE;

	return TRUE;
}

BOOL CSkinJSEngine::InitGlobalNamespace()
{
    if (! m_pSkinBase->AddNameItemToSrcipt(m_pActiveScript))
        return FALSE;

    return TRUE;
}

BOOL CSkinJSEngine::Start()
{
	SKINJSENGINE_STATE state = GetState();
	if (state != SKINJSENGINE_START 
		&& state != SKINJSENGINE_LOAD)
		return FALSE;

	HRESULT hr = m_pActiveScript->SetScriptState(SCRIPTSTATE_CONNECTED);
	return (hr == S_OK);
}

BOOL CSkinJSEngine::Stop()
{
	SKINJSENGINE_STATE state = GetState();
	if (state != SKINJSENGINE_START)
		return FALSE;

	HRESULT hr = m_pActiveScript->SetScriptState(SCRIPTSTATE_DISCONNECTED);
	return (hr == S_OK);
}

BOOL CSkinJSEngine::Close()
{
	if (m_pActiveScript) {
		m_pActiveScript->Close();
		m_pActiveScript = NULL;
	}
	return TRUE;
}

//获得引擎状态
SKINJSENGINE_STATE CSkinJSEngine::GetState() const
{
	if (m_pActiveScript == NULL)
		return SKINJSENGINE_CLOSE;

	HRESULT hr = S_OK;
	SCRIPTSTATE state = SCRIPTSTATE_CLOSED;
	m_pActiveScript->GetScriptState(&state);
	switch (state)
	{
	case SCRIPTSTATE_UNINITIALIZED:
		return SKINJSENGINE_NOTLOAD;
	case SCRIPTSTATE_INITIALIZED:
		return SKINJSENGINE_LOAD;
	case SCRIPTSTATE_STARTED:
		return SKINJSENGINE_LOAD;
	case SCRIPTSTATE_CONNECTED:
		return SKINJSENGINE_START;
	case SCRIPTSTATE_DISCONNECTED:
		return SKINJSENGINE_STOP;
	case SCRIPTSTATE_CLOSED:
		return SKINJSENGINE_CLOSE;
	default:
		return SKINJSENGINE_CLOSE;
	}

	ASSERT(0 && _T("CSkinJSEngine::GetState() 错误！"));
	return SKINJSENGINE_CLOSE;
}

HRESULT CSkinJSEngine::GetProperty(BSTR propName, VARIANT* pPropValue)
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

HRESULT CSkinJSEngine::CallFunction(BSTR funcName, VARIANT* pVarResult, DISPPARAMS* pDispParams)
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
    OLECHAR *rgszNames[] = {funcName};
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
