#pragma once

enum SKINJSENGINE_STATE
{
    SKINJSENGINE_CLOSE,
    SKINJSENGINE_NOTLOAD,
    SKINJSENGINE_LOAD,
    SKINJSENGINE_START,
    SKINJSENGINE_STOP,
};

class CSkinBase;

class ISkinJSEngine
{
public:
    ISkinJSEngine(void);

    //不允许在栈上创建
protected:
    virtual ~ISkinJSEngine(void);

    //显式的析构函数
public:
    void Destroy();

private:
    //不允许复制
    explicit ISkinJSEngine(const ISkinJSEngine& other);
    ISkinJSEngine& operator = (const ISkinJSEngine& other);

public:
    //创建
    virtual BOOL Create(CSkinBase* pBase) = 0;
    //加载
    virtual BOOL Load(const char* pJS, LPCTSTR fileName = NULL) = 0;
    //初始化全局名字空间
    virtual BOOL InitGlobalNamespace() = 0;
    //启动
    virtual BOOL Start() = 0;
    //停止
    virtual BOOL Stop() = 0;
    //关闭
    virtual BOOL Close() = 0;
    //获得引擎状态
    virtual SKINJSENGINE_STATE GetState() const = 0;
    //获得JS变量值
    virtual HRESULT GetProperty(BSTR propName, VARIANT* pPropValue);
    //调用JS函数
    virtual HRESULT CallFunction(BSTR funcName, VARIANT* pVarResult, DISPPARAMS* pDispParams);
};
