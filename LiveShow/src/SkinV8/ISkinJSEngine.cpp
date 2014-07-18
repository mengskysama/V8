#include "StdAfx.h"
#include "ISkinJSEngine.h"

ISkinJSEngine::ISkinJSEngine(void)
{

}

ISkinJSEngine::~ISkinJSEngine(void)
{

}

void ISkinJSEngine::Destroy()
{
    delete this;
}

ISkinJSEngine::ISkinJSEngine(const ISkinJSEngine& other)
{
    ASSERT(0 && _T("ISkinJSEngine 拷贝构造不可用"));
}

ISkinJSEngine& ISkinJSEngine::operator = (const ISkinJSEngine& other)
{
    ASSERT(0 && _T("ISkinJSEngine operator = 不可用"));
    return *this;
}

HRESULT ISkinJSEngine::GetProperty(BSTR propName, VARIANT* pPropValue)
{
    return S_FALSE;
}

HRESULT ISkinJSEngine::CallFunction(BSTR funcName, VARIANT* pVarResult, DISPPARAMS* pDispParams)
{
    return S_FALSE;
}
