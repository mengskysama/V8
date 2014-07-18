#pragma once

#include "XmlCmdTargetDefine.h"
#include "SkinBaseDefine.h"
#include "SkinItemDefine.h"

#include "Types.h"

#include "XmlCmdTarget.h"

class CSkinBase;

#ifdef MS_JS_ENGINE_SUPPORT

HRESULT InvokeScriptHelper(CSkinBase* pSkin, CComPtr<IDispatch> pDisp, DISPPARAMS* pParams, VARIANT* pVarResult);

HRESULT FireScriptHelper0(CSkinBase* pSkin, IDispatch* pDisp, VARIANT* pVarResult);

template<typename T0>
HRESULT FireScriptHelper1(CSkinBase* pSkin, IDispatch* pDisp, VARIANT* pVarResult, T0 param0)
{
	CComVariant vars[1];
	vars[0] = param0;
	DISPPARAMS dispParams = {vars, NULL, 1, 0};

	return InvokeScriptHelper(pSkin, pDisp, &dispParams, pVarResult);
};

template<typename T0, typename T1>
HRESULT FireScriptHelper2(CSkinBase* pSkin, IDispatch* pDisp, VARIANT* pVarResult, T0 param0, T1 param1)
{
	CComVariant vars[2];
	vars[1] = param0;
	vars[0] = param1;
	DISPPARAMS dispParams = {vars, NULL, 2, 0};

	return InvokeScriptHelper(pSkin, pDisp, &dispParams, pVarResult);
};

template<typename T0, typename T1, typename T2>
HRESULT FireScriptHelper3(CSkinBase* pSkin, IDispatch* pDisp, VARIANT* pVarResult, T0 param0, T1 param1, T2 param2)
{
	CComVariant vars[3];
	vars[2] = param0;
	vars[1] = param1;
	vars[0] = param2;
	DISPPARAMS dispParams = {vars, NULL, 3, 0};

	return InvokeScriptHelper(pSkin, pDisp, &dispParams, pVarResult);
};

template<typename T0, typename T1, typename T2, typename T3>
HRESULT FireScriptHelper4(CSkinBase* pSkin, IDispatch* pDisp, VARIANT* pVarResult, T0 param0, T1 param1, T2 param2, T3 param3)
{
	CComVariant vars[4];
	vars[3] = param0;
	vars[2] = param1;
	vars[1] = param2;
	vars[0] = param3;
	DISPPARAMS dispParams = {vars, NULL, 4, 0};

	return InvokeScriptHelper(pSkin, pDisp, &dispParams, pVarResult);
};

template<typename T0, typename T1, typename T2, typename T3, typename T4>
HRESULT FireScriptHelper5(CSkinBase* pSkin, IDispatch* pDisp, VARIANT* pVarResult, T0 param0, T1 param1, T2 param2, T3 param3, T4 param4)
{
	CComVariant vars[5];
	vars[4] = param0;
	vars[3] = param1;
	vars[2] = param2;
	vars[1] = param3;
	vars[0] = param4;
	DISPPARAMS dispParams = {vars, NULL, 5, 0};

	return InvokeScriptHelper(pSkin, pDisp, &dispParams, pVarResult);
};

#endif //MS_JS_ENGINE_SUPPORT


//////////////////////////////////////////////////////////////////////////
//V8支持
//////////////////////////////////////////////////////////////////////////
#ifdef V8_JS_ENGINE_SUPPORT

//执行脚本函数
v8::Handle<v8::Value> V8FireScriptHelper0(CSkinBase* pSkin, v8::Handle<v8::Object> recv, v8::Handle<v8::Function> func);

template<typename T0>
v8::Handle<v8::Value> V8FireScriptHelper1(CSkinBase* pSkin, v8::Handle<v8::Object> recv, v8::Handle<v8::Function> func, T0 param0)
{
    v8::HandleScope handleScope;

    v8::Handle<v8::Value> vals[1];
    vals[0] = ToValue(param0);

    v8::TryCatch tryCatch;

    v8::Handle<v8::Value> result = func->Call(recv, 1, vals);

    ASSERT(! result.IsEmpty());
    if (result.IsEmpty()) {
        CString strException = ValueToCString(tryCatch.Exception());
        TRACE("脚本异常：%s", strException);
        ::MessageBox((pSkin ? pSkin->GetSkinHwnd() : NULL), strException, _T("脚本错误"), MB_OK | MB_TOPMOST);
        return handleScope.Close(v8::Undefined());
    }

    return handleScope.Close(result);
};

template<typename T0, typename T1>
v8::Handle<v8::Value> V8FireScriptHelper2(CSkinBase* pSkin, v8::Handle<v8::Object> recv, v8::Handle<v8::Function> func, T0 param0, T1 param1)
{
    v8::HandleScope handleScope;

    v8::Handle<v8::Value> vals[2];
    vals[0] = ToValue(param0);
    vals[1] = ToValue(param1);

    v8::TryCatch tryCatch;

    v8::Handle<v8::Value> result = func->Call(recv, 2, vals);

    ASSERT(! result.IsEmpty());
    if (result.IsEmpty()) {
        CString strException = ValueToCString(tryCatch.Exception());
        TRACE("脚本异常：%s", strException);
        ::MessageBox((pSkin ? pSkin->GetSkinHwnd() : NULL), strException, _T("脚本错误"), MB_OK | MB_TOPMOST);
        return handleScope.Close(v8::Undefined());
    }

    return handleScope.Close(result);
};

template<typename T0, typename T1, typename T2>
v8::Handle<v8::Value> V8FireScriptHelper3(CSkinBase* pSkin, v8::Handle<v8::Object> recv, v8::Handle<v8::Function> func, T0 param0, T1 param1, T2 param2)
{
    v8::HandleScope handleScope;

    v8::Handle<v8::Value> vals[3];
    vals[0] = ToValue(param0);
    vals[1] = ToValue(param1);
    vals[2] = ToValue(param2);

    v8::TryCatch tryCatch;

    v8::Handle<v8::Value> result = func->Call(recv, 3, vals);

    ASSERT(! result.IsEmpty());
    if (result.IsEmpty()) {
        CString strException = ValueToCString(tryCatch.Exception());
        TRACE("脚本异常：%s", strException);
        ::MessageBox((pSkin ? pSkin->GetSkinHwnd() : NULL), strException, _T("脚本错误"), MB_OK | MB_TOPMOST);
        return handleScope.Close(v8::Undefined());
    }

    return handleScope.Close(result);
};

template<typename T0, typename T1, typename T2, typename T3>
v8::Handle<v8::Value> V8FireScriptHelper4(CSkinBase* pSkin, v8::Handle<v8::Object> recv, v8::Handle<v8::Function> func, T0 param0, T1 param1, T2 param2, T3 param3)
{
    v8::HandleScope handleScope;

    v8::Handle<v8::Value> vals[4];
    vals[0] = ToValue(param0);
    vals[1] = ToValue(param1);
    vals[2] = ToValue(param2);
    vals[3] = ToValue(param3);

    v8::TryCatch tryCatch;

    v8::Handle<v8::Value> result = func->Call(recv, 4, vals);

    ASSERT(! result.IsEmpty());
    if (result.IsEmpty()) {
        CString strException = ValueToCString(tryCatch.Exception());
        TRACE("脚本异常：%s", strException);
        ::MessageBox((pSkin ? pSkin->GetSkinHwnd() : NULL), strException, _T("脚本错误"), MB_OK | MB_TOPMOST);
        return handleScope.Close(v8::Undefined());
    }

    return handleScope.Close(result);
};

template<typename T0, typename T1, typename T2, typename T3, typename T4>
v8::Handle<v8::Value> V8FireScriptHelper5(CSkinBase* pSkin, v8::Handle<v8::Object> recv, v8::Handle<v8::Function> func, T0 param0, T1 param1, T2 param2, T3 param3, T4 param4)
{
    v8::HandleScope handleScope;

    v8::Handle<v8::Value> vals[5];
    vals[0] = ToValue(param0);
    vals[1] = ToValue(param1);
    vals[2] = ToValue(param2);
    vals[3] = ToValue(param3);
    vals[4] = ToValue(param4);

    v8::TryCatch tryCatch;

    v8::Handle<v8::Value> result = func->Call(recv, 5, vals);

    ASSERT(! result.IsEmpty());
    if (result.IsEmpty()) {
        CString strException = ValueToCString(tryCatch.Exception());
        TRACE("脚本异常：%s", strException);
        ::MessageBox((pSkin ? pSkin->GetSkinHwnd() : NULL), strException, _T("脚本错误"), MB_OK | MB_TOPMOST);
        return handleScope.Close(v8::Undefined());
    }

    return handleScope.Close(result);
};

#endif //V8_JS_ENGINE_SUPPORT
