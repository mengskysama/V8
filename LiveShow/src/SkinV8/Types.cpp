#include "StdAfx.h"
#include "Types.h"

BOOL FormatBstrVariantToDecimal(CComVariant& var)
{
    if (var.vt != VT_BSTR)
        return FALSE;
    COLE2CT tVar(var.bstrVal);
    CString strVar = tVar;
    const int len = strVar.GetLength();
    if (len <= 0)
        return FALSE;
    if (strVar.GetAt(0) == _T('#')) {
        int value = 0;
        int base = 1;
        for (int i=len-1; i>=1; i--){
            if ( strVar.GetAt(i) >= _T('0') && strVar.GetAt(i) <= _T('9') ){
                value += (strVar.GetAt(i) - _T('0')) * base;
                base *= 16;
            }else if ( strVar.GetAt(i) >= _T('a') && strVar.GetAt(i) <= _T('f') ){
                value += (strVar.GetAt(i) - _T('a') + 10) * base;
                base *= 16;
            }else if ( strVar.GetAt(i) >= _T('A') && strVar.GetAt(i) <= _T('F') ){
                value += (strVar.GetAt(i) - _T('A') + 10) * base;
                base *= 16;
            }else{
                return FALSE;
            }
        }
        strVar.Format(_T("%d"), value);
        var = strVar;
        return TRUE;
    } else if (len > 1 && 
        strVar.GetAt(0) == _T('0') &&
        strVar.GetAt(1) == _T('x')) {
            int value = 0;
            int base = 1;
            for (int i=len-1; i>=2; i--){
                if ( strVar.GetAt(i) >= _T('0') && strVar.GetAt(i) <= _T('9') ){
                    value += (strVar.GetAt(i) - _T('0')) * base;
                    base *= 16;
                }else if ( strVar.GetAt(i) >= _T('a') && strVar.GetAt(i) <= _T('f') ){
                    value += (strVar.GetAt(i) - _T('a') + 10) * base;
                    base *= 16;
                }else if ( strVar.GetAt(i) >= _T('A') && strVar.GetAt(i) <= _T('F') ){
                    value += (strVar.GetAt(i) - _T('A') + 10) * base;
                    base *= 16;
                }else{
                    return FALSE;
                }
            }
            strVar.Format(_T("%d"), value);
            var = strVar;
            return TRUE;
    }

    return FALSE;
}

#ifdef MS_JS_ENGINE_SUPPORT

bool VariantToBool(VARIANT& var)
{
    CComVariant comVar(var);

    if (comVar.ChangeType(VT_BOOL) == S_OK)
        return ( comVar.boolVal != (VARIANT_FALSE) );

    if (comVar.ChangeType(VT_I4) == S_OK)
        return ( comVar.intVal != 0 );

    return false;
}

#endif //MS_JS_ENGINE_SUPPORT

#ifdef V8_JS_ENGINE_SUPPORT
using namespace v8;

bool ValueToBool(Handle<Value> value)
{
    HandleScope handleScope;

    if (value.IsEmpty())
        return false;

    if (value->IsUndefined())
        return false;

    if (value->IsNull())
        return false;

    if (value->IsBoolean())
        return value->BooleanValue();

    if (value->IsTrue())
        return true;

    if (value->IsFalse())
        return false;

    if (value->IsInt32())
        return value->Int32Value() != 0;

    if (value->IsNumber())
        return value->NumberValue() != 0.0;

    if (value->IsString())
        return value->BooleanValue();

    return false;
}

Handle<Value> ToValue(int t)
{
    HandleScope handleScope;

    return handleScope.Close(Integer::New(t));
}

Handle<Value> ToValue(unsigned int t)
{
    HandleScope handleScope;

    return handleScope.Close(Integer::New(t));
}

Handle<Value> ToValue(long t)
{
    HandleScope handleScope;

    return handleScope.Close(Integer::New(t));
}

Handle<Value> ToValue(unsigned long t)
{
    HandleScope handleScope;

    return handleScope.Close(Integer::New(t));
}

Handle<Value> ToValue(__int64 t)
{
    HandleScope handleScope;

    return handleScope.Close(Number::New((double)t));
}

Handle<Value> ToValue(unsigned __int64 t)
{
    HandleScope handleScope;

    return handleScope.Close(Number::New((double)t));
}

Handle<Value> ToValue(float t)
{
    HandleScope handleScope;

    return handleScope.Close(Number::New(t));
}

Handle<Value> ToValue(double t)
{
    HandleScope handleScope;

    return handleScope.Close(Number::New(t));
}

Handle<Value> ToValue(bool t)
{
    HandleScope handleScope;

    return handleScope.Close(Boolean::New(t));
}

Handle<Value> ToValue(LPCTSTR t)
{
    HandleScope handleScope;

#ifdef UNICODE
    return handleScope.Close(String::New((uint16_t*)t));
#else
    return handleScope.Close(String::New((char*)t));
#endif
}

Handle<Value> ToValue(CXmlCmdTarget* t)
{
    HandleScope handleScope;

    Handle<Value> object = v8::Null();
    if (t != NULL)
        object = t->GetV8Object();

    return handleScope.Close(object);
}

Handle<Value> ToValue(CSkinBase* t)
{
    HandleScope handleScope;

    Handle<Value> object = ToValue((CXmlCmdTarget*)t);

    return handleScope.Close(object);
}

Handle<Value> ToValue(VARIANT var)
{
    HandleScope handleScope;

    return handleScope.Close(VariantToValue(var));
}

Handle<Value> ToValue(Handle<Value> t)
{
    HandleScope handleScope;

    return handleScope.Close(t);
}

Handle<Value> VariantToValue(const CComVariant& var)
{
    HandleScope handleScope;

    Handle<Value> val = v8::Undefined();

    switch (var.vt)
    {
    case VT_EMPTY:
        val = v8::Undefined();
        break;

    case VT_NULL:
        val = v8::Null();
        break;

    //有符号
    case VT_I1:
        val = Integer::New(var.cVal);
        val = val->ToInt32();
        break;
    case VT_I2:
        val = Integer::New(var.iVal);
        val = val->ToInt32();
        break;
    case VT_I4:
    case VT_INT:
        val = Integer::New(var.intVal);
        val = val->ToInt32();
        break;
    case VT_I8:
        val = Number::New((double)var.llVal);
        break;

    //无符号
    case VT_UI1:
        val = Integer::New(var.bVal);
        val = val->ToUint32();
        break;
    case VT_UI2:
        val = Integer::New(var.uiVal);
        val = val->ToUint32();
        break;
    case VT_UI4:
    case VT_UINT:
        val = Integer::New(var.uintVal);
        val = val->ToUint32();
        break;
    case VT_UI8:
        val = Integer::New((uint32_t)var.ullVal);
        break;

    //实数
    case VT_R4:
        val = Number::New((double)var.fltVal);
        break;
    case VT_R8:
        val = Number::New((double)var.dblVal);
        break;

    //字串
    case VT_LPSTR:
        val = String::New(var.pcVal);
        break;
    case VT_LPWSTR:
        val = String::New(var.puiVal);
        break;
    case VT_BSTR:
        val = String::New((uint16_t*)var.bstrVal);
        break;

    //bool
    case VT_BOOL:
        val = Boolean::New((var.boolVal) ? true : false);
        break;

    default:
        ASSERT(0 && _T("不支持的类型！"));
        break;
    }

    return handleScope.Close(val);
}

VARENUM ValueType(Handle<Value> value)
{
    if (value.IsEmpty())
        return VT_EMPTY;

    if (value->IsUndefined())
        return VT_EMPTY;

    if (value->IsNull())
        return VT_NULL;

    if (value->IsTrue())
        return VT_BOOL;

    if (value->IsFalse())
        return VT_BOOL;

    if (value->IsString())
        return VT_BSTR;

    if (value->IsBoolean())
        return VT_BOOL;

    if (value->IsInt32())
        return VT_I4;

    if (value->IsNumber())
        return VT_I4;

    if (value->IsExternal())
        return VT_UNKNOWN;

    if (value->IsFunction())
        return VT_DISPATCH;

    if (value->IsObject())
        return VT_DISPATCH;

    if (value->IsDate())
        return VT_DATE;

    if (value->IsArray())
        return VT_ARRAY;

    return VT_EMPTY;
}

CComVariant ValueToVariant(Handle<Value> value)
{
    CComVariant var;

    if (value.IsEmpty()) {
        var.ChangeType(VT_EMPTY);
        return var;
    }

    if (value->IsUndefined()) {
        var.ChangeType(VT_EMPTY);
        return var;
    }

    if (value->IsNull()) {
        var.ChangeType(VT_NULL);
        return var;
    }

    if (value->IsTrue()) {
        var.ChangeType(VT_BOOL);
        var.boolVal = VARIANT_TRUE;
        return var;
    }

    if (value->IsFalse()) {
        var.ChangeType(VT_BOOL);
        var.boolVal = VARIANT_FALSE;
        return var;
    }

    if (value->IsString()) {
        var.ChangeType(VT_BSTR);
        var = ValueToCString(value);
        return var;
    }

    if (value->IsBoolean()) {
        var.ChangeType(VT_BOOL);
        var.boolVal = value->BooleanValue() ? (VARIANT_TRUE) : (VARIANT_FALSE);
        return var;
    }

    if (value->IsInt32()) {
        var.ChangeType(VT_I4);
        var.intVal = value->Int32Value();
        return var;
    }

    if (value->IsNumber()) {
        var.ChangeType(VT_I4);
        var.intVal = value->Int32Value();
    }

    if (value->IsExternal()) {
        var.ChangeType(VT_UNKNOWN);
        return var;
    }

    if (value->IsFunction()) {
        var.ChangeType(VT_DISPATCH);
        return var;
    }

    if (value->IsObject()) {
        var.ChangeType(VT_DISPATCH);
        return var;
    }

    if (value->IsDate()) {
        var.ChangeType(VT_DATE);
        return var;
    }

    if (value->IsArray()) {
        var.ChangeType(VT_ARRAY);
        return var;
    }

    return var;
}

CString ValueToCString(Handle<Value> value)
{
#ifdef UNICODE
    String::Value val(value);
    return CString((WCHAR*)(*val));
#else
    String::AsciiValue val(value);
    return CString(*val);
#endif
}

Local<String> CStringToString(LPCTSTR str)
{
    HandleScope handleScope;

#ifdef UNICODE
    if (str == NULL)
        return handleScope.Close(String::New((uint16_t*)(L""), -1));
    else 
        return handleScope.Close(String::New((uint16_t*)str, (int)_tcslen(str)));
#else
    if (str == NULL)
        return handleScope.Close(String::New(""));
    else 
        return handleScope.Close(String::New((const char*)str, (int)_tcslen(str)));
#endif
}

#endif //V8_JS_ENGINE_SUPPORT
