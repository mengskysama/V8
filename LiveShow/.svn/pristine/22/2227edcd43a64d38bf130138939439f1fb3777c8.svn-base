#pragma once

#include "XmlCmdTarget.h"

class CSkinBase;

//将表示数字的BSTR型的VARIANT转换为十进制，转换后仍然为BSTR，如"0x12"->"18"
BOOL FormatBstrVariantToDecimal(CComVariant& var);

#ifdef MS_JS_ENGINE_SUPPORT

bool VariantToBool(VARIANT& var);

#endif //MS_JS_ENGINE_SUPPORT


#ifdef V8_JS_ENGINE_SUPPORT

bool ValueToBool(v8::Handle<v8::Value> value);

//将参数转换为v8的Value
v8::Handle<v8::Value> ToValue(int t);

v8::Handle<v8::Value> ToValue(unsigned int t);

v8::Handle<v8::Value> ToValue(long t);

v8::Handle<v8::Value> ToValue(unsigned long t);

v8::Handle<v8::Value> ToValue(__int64 t);

v8::Handle<v8::Value> ToValue(unsigned __int64 t);

v8::Handle<v8::Value> ToValue(float t);

v8::Handle<v8::Value> ToValue(double t);

v8::Handle<v8::Value> ToValue(bool t);

v8::Handle<v8::Value> ToValue(LPCTSTR t);

v8::Handle<v8::Value> ToValue(CXmlCmdTarget* t);

v8::Handle<v8::Value> ToValue(CSkinBase* t);

v8::Handle<v8::Value> ToValue(VARIANT var);

v8::Handle<v8::Value> ToValue(v8::Handle<v8::Value> t);

//模板
template<typename T>
v8::Handle<v8::Value> ToValue(T t)
{
    ASSERT(0 && _T("Unsupported ToValue"));
    return v8::Undefined();
}

v8::Handle<v8::Value> VariantToValue(const CComVariant& var);

CComVariant ValueToVariant(v8::Handle<v8::Value> value);

VARENUM ValueType(v8::Handle<v8::Value> value);

CString ValueToCString(v8::Handle<v8::Value> value);

v8::Local<v8::String> CStringToString(LPCTSTR str);

#endif //V8_JS_ENGINE_SUPPORT
