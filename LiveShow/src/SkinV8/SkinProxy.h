#pragma once

#include "SkinBase.h"

// CSkinProxy

class CSkinProxy : public CSkinBase
{
	DECLARE_DYNCREATE(CSkinProxy)

public:
	CSkinProxy();
	virtual ~CSkinProxy();

	virtual void OnFinalRelease();

protected:
	DECLARE_MESSAGE_MAP()
	DECLARE_OLECREATE(CSkinProxy)
	DECLARE_SKIN_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()

protected:
	enum
	{
		SKINBASE_DISPIDS(CSkinProxy),

		SKIN_DISPID(CSkinProxy, SetClient),
        SKIN_DISPID(CSkinProxy, GetClient),
	};

public:
	//////////////////////////////////////////////////////////////////////////
	//属性的Get虚函数
	//////////////////////////////////////////////////////////////////////////
	////皮肤元素的名字
	//virtual BSTR GetName();
	//皮肤的文本
	virtual BSTR GetText();
	//鼠标光标
	virtual BSTR GetCursor();
	//设置皮肤元素的位置
	virtual LONG GetLeft();
	virtual LONG GetTop();
	virtual LONG GetWidth();
	virtual LONG GetHeight();
    virtual BOOL GetSizeDelay();
	//可见性
	virtual BOOL GetVisible();
	//可用性
	virtual BOOL GetDisabled();
	//Tab Stop
	virtual BOOL GetTabStop();
	//Group
	virtual BOOL GetGroup();
	//背景属性
	virtual BOOL GetBackground();
	//透明
	virtual BOOL GetTransparent();
	//颜色变换
	virtual BOOL GetColorTransform();
    //alpha通道
    virtual BOOL GetAlpha();
	//上边角半径
	virtual LONG GetUpCornerRadius();
	//下边角半径
	virtual LONG GetDownCornerRadius();
	//边线宽
	virtual LONG GetBorderLineWidth();
	//边线颜色
	virtual LONG GetBorderLineColor();
	//透明色
	virtual LONG GetTransparentColor();

protected:
	//////////////////////////////////////////////////////////////////////////
	//属性的Set虚函数
	//////////////////////////////////////////////////////////////////////////
	////皮肤元素的名字
	//virtual void SetName(LPCTSTR newVal);
	//文本
	virtual void SetText(LPCTSTR newVal);
	//鼠标光标
	virtual void SetCursor(LPCTSTR newVal);
	//设置皮肤元素的位置
	virtual void SetLeft(LONG newVal);
	virtual void SetTop(LONG newVal);
	virtual void SetWidth(LONG newVal);
	virtual void SetHeight(LONG newVal);
    virtual void SetSizeDelay(BOOL newVal);
	//可见性
	virtual void SetVisible(BOOL newVal);
	//可用性
	virtual void SetDisabled(BOOL newVal);
	//Tab Stop
	virtual void SetTabStop(BOOL newVal);
	//Group
	virtual void SetGroup(BOOL newVal);
	//背景属性
	virtual void SetBackground(BOOL newVal);
	//透明
	virtual void SetTransparent(BOOL newVal);
	//颜色变换
	virtual void SetColorTransform(BOOL newVal);
    //alpha通道
    virtual void SetAlpha(BOOL newVal);
	//上边角半径
	virtual void SetUpCornerRadius(LONG newVal);
	//下边角半径
	virtual void SetDownCornerRadius(LONG newVal);
	//边线宽
	virtual void SetBorderLineWidth(LONG newVal);
	//边线颜色
	virtual void SetBorderLineColor(LONG newVal);
	//透明色
	virtual void SetTransparentColor(LONG newVal);

    //Size Delay
    BOOL GetSizeDelayFlag();
    virtual void SetSizeDelayFlag(BOOL bTrue);
    virtual BOOL ApplyDelayedSize(BOOL bRepaint = TRUE);

public:
	//父窗口
	virtual CSkinBase* GetSkinParent();
	//Owner窗口
	virtual CSkinBase* GetSkinOwner();
	//下一窗口
	virtual CSkinBase* GetSkinNext();
	//上一窗口
	virtual CSkinBase* GetSkinPrev();
	//获得第一个子元素，bForward为方向
	virtual CSkinBase* GetFirstSubSkin(UINT uFilterBits = 0, UINT uFilterValues = 0, BOOL bForward = TRUE);
	//获得下一个的兄弟元素，bForward为方向
	virtual CSkinBase* GetNextSkin(UINT uFilterBits = 0, UINT uFilterValues = 0, BOOL bForward = TRUE);
	//属性过滤
	virtual BOOL IsFiltered(UINT uFilterBits, UINT uFilterValues);

public:
	//解除代理
	BOOL RemoveClient();

protected:

	BOOL UpdateClientProperty(CSkinBase* pClient);

	//////////////////////////////////////////////////////////////////////////
	//Dispatch Method
	//////////////////////////////////////////////////////////////////////////
	VARIANT_BOOL SetClient(LONG newVal);
    IUnknown* GetClient(void);

#ifdef V8_JS_ENGINE_SUPPORT
    //v8实现
    v8::Handle<v8::Value> V8SetClient(const v8::Arguments& args);
    v8::Handle<v8::Value> V8GetClient(const v8::Arguments& args);
    virtual v8::Handle<v8::Value> V8InternalCallMethod(const CString& funcName, const v8::Arguments& args);
#endif //V8_JS_ENGINE_SUPPORT

protected:
	CSkinBase* m_pSkinClient;
};


