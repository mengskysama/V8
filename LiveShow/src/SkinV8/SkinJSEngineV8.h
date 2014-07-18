#pragma once

#ifdef V8_JS_ENGINE_SUPPORT

#include "ISkinJSEngine.h"

#include <map>
#include <vector>

class CSkinBase;

class CSkinJSEngineV8 : public ISkinJSEngine
{
public:
    CSkinJSEngineV8(void);

    //不允许在栈上创建
protected:
    virtual ~CSkinJSEngineV8(void);

private:
    //不允许复制
    explicit CSkinJSEngineV8(const CSkinJSEngineV8& other);
    CSkinJSEngineV8& operator = (const CSkinJSEngineV8& other);

public:
    //创建
    virtual BOOL Create(CSkinBase* pBase);
    //加载
    virtual BOOL Load(const char* pJS, LPCTSTR fileName);
    //初始化全局名字空间
    virtual BOOL InitGlobalNamespace();
    //启动
    virtual BOOL Start();
    //停止
    virtual BOOL Stop();
    //关闭
    virtual BOOL Close();
    //获得引擎状态
    virtual SKINJSENGINE_STATE GetState() const;
    //获得JS变量值
    virtual HRESULT GetProperty(BSTR propName, VARIANT* pPropValue);
    //调用JS函数
    virtual HRESULT CallFunction(BSTR funcName, VARIANT* pVarResult, DISPPARAMS* pDispParams);

private:
    //JS引擎运行上下文
    v8::Persistent<v8::Context> m_jsContext;
    std::vector<v8::Persistent<v8::Script>*> m_vecJsScripts;
    //JS脚本代码
    CString m_strScript;
    //关联的CSkinBase对象
    CSkinBasePtr m_pSkinBase;
};

#endif //V8_JS_ENGINE_SUPPORT
