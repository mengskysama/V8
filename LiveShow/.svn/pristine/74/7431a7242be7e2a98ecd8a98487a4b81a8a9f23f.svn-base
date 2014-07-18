#pragma once

#ifdef MS_JS_ENGINE_SUPPORT

#include "ISkinJSEngine.h"

class CSkinBase;

class CSkinJSEngine : public ISkinJSEngine
{
public:
	CSkinJSEngine(void);

	//不允许在栈上创建
protected:
	virtual ~CSkinJSEngine(void);

private:
	//不允许复制
	explicit CSkinJSEngine(const CSkinJSEngine& other);
	CSkinJSEngine& operator = (const CSkinJSEngine& other);

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
	//JS引擎对象
	CComPtr<IActiveScript> m_pActiveScript;
	//JS引擎关联控制的CSkinBase对象
	CSkinBasePtr m_pSkinBase;
};

#endif //MS_JS_ENGINE_SUPPORT
