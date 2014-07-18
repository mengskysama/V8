#pragma once

#include "XmlCmdTarget.h"
#include "SkinBaseDefine.h"

#ifdef V8_JS_ENGINE_SUPPORT
#include "V8ContextFunction.h"
#endif //V8_JS_ENGINE_SUPPORT

#include <list>
#include <map>

class CImageTransformParam;
class CSkinEngine;
class CSkinItem;
class CImageBase;
class CSkinProxy;
class CSkinBase : public CXmlCmdTarget
{
	friend class CSkinEngine;
	friend class CSkinProxy;
	DECLARE_DYNCREATE(CSkinBase)
protected:
	DECLARE_MESSAGE_MAP()
	DECLARE_SKIN_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
	DECLARE_SKINBASE_PROPERTIES()

    //////////////////////////////////////////////////////////////////////////
    //Script支持
    //////////////////////////////////////////////////////////////////////////
#ifdef V8_JS_ENGINE_SUPPORT

    //V8支持
    BOOL AddObjectToSrcipt(v8::Handle<v8::Object> object);

#endif //V8_JS_ENGINE_SUPPORT

#ifdef MS_JS_ENGINE_SUPPORT

protected:
	HRESULT GetItemInfo(LPCOLESTR pstrName, DWORD dwReturnMask, IUnknown **ppiunkItem, ITypeInfo **ppti);
	HRESULT OnScriptError(IActiveScriptError *pscripterror);

protected:
	HRESULT GetWindow(HWND *phwnd);

	//向脚本暴露名字空间
public:
    BOOL AddNameItemToSrcipt(CComPtr<IActiveScript> pScript);

#endif //MS_JS_ENGINE_SUPPORT

public:
    //初始化与文件路径
    BOOL SetConfigFileName(LPCTSTR strConfigFileName);
    CString GetXmlFileName() const;
    CString GetScriptFileName() const;

    //XML配置
    BOOL CreateSubSkinFromXml(IXMLDOMElementPtr ptrDocumentRoot, BOOL bPreLoad = TRUE);
    BOOL LoadSubSkinPropFromXml(IXMLDOMElementPtr ptrDocumentRoot);
    BOOL PreLoadXml(LPCTSTR lpzPreLoad);
    static BOOL GetXmlRoot(const char* pzXml, BOOL bUnicode, IXMLDOMElement** ppXmlRoot);
    BOOL CreateFromXml();
    BOOL LoadPropFromXml();
    BOOL ParseXmlHelper(IXMLDOMElementPtr ptrDocumentRoot);
    BOOL LoadXML(const char* pzXml, BOOL bUnicode = TRUE);
    BOOL ParseXml();

    CString						   m_strConfigName;

protected:
	enum
	{
		SKINBASE_DISPIDS(CSkinBase),
	};

	//////////////////////////////////////////////////////////////////////////
	//构造与析构相关
	//////////////////////////////////////////////////////////////////////////
public:
    //构造函数
	CSkinBase();
    //析构函数
	virtual ~CSkinBase();
protected:
    //初始化函数，由SkinEngine调用，主要用于加载默认设置
    virtual void Initialize();
    //释放皮肤元素
	virtual DWORD ReleaseSkin();
    //最后的引用被释放时调用
	virtual void OnFinalRelease();

	//////////////////////////////////////////////////////////////////////////
	//获得皮肤配置文件的目录，配置文件的名字
	//////////////////////////////////////////////////////////////////////////
public:
	//获得皮肤路径
	CString GetSkinPath() const;

	//////////////////////////////////////////////////////////////////////////
	//关联窗口函数，因为CSkinProxy的代理模式，部分函数需要设为虚函数
	//////////////////////////////////////////////////////////////////////////
public:
	//代理窗口
	virtual CSkinProxy* GetSkinProxy();
	//父窗口
	virtual CSkinBase* GetSkinParent();
	//父窗口
	void SetSkinParent(CSkinBase* pSkinParent);
	//Owner窗口
	virtual CSkinBase* GetSkinOwner();
	//下一窗口
	virtual CSkinBase* GetSkinNext();
	//上一窗口
	virtual CSkinBase* GetSkinPrev();
	//祖先窗口
	CSkinBase* GetSkinAncestor();
private:
	//Owner窗口
	void SetSkinOwner(CSkinBase* pSkinOwner);
	//下一窗口
	void SetSkinNext(CSkinBase* pSkinNext);
	//上一窗口
	void SetSkinPrev(CSkinBase* pSkinPrev);

protected:
    CSkinBase* CreateSkinChild(LPCTSTR lpzClassName, LPCTSTR lpzChildName);

	//////////////////////////////////////////////////////////////////////////
	//窗口树访问函数，因为CSkinProxy的代理模式，部分函数需要设为虚函数
	//////////////////////////////////////////////////////////////////////////
public:
	//获得第一个子元素，bForward为方向
	virtual CSkinBase* GetFirstSubSkin(UINT uFilterBits = 0, UINT uFilterValues = 0, BOOL bForward = TRUE);
	//获得下一个的兄弟元素，bForward为方向
	virtual CSkinBase* GetNextSkin(UINT uFilterBits = 0, UINT uFilterValues = 0, BOOL bForward = TRUE);
	//遍历Skin树，bForward为方向
	static BOOL WalkSubSkinTree(CSkinBase* pRoot, CSkinBase*& pFind,
		UINT uFilterBits = 0, UINT uFilterValues = 0,
		UINT uNodeFileterBits = 0, UINT uNodeFilterValues = 0,
		BOOL bForward = TRUE);
	static BOOL WalkSkinTree(CSkinBase* pStart, CSkinBase*& pFind,
		UINT uFilterBits = 0, UINT uFilterValues = 0,
		UINT uNodeFileterBits = 0, UINT uNodeFilterValues = 0,
		BOOL bForward = TRUE);
	//获得下一个通过Tab键接收焦点的皮肤对象，bForward为方向
	CSkinBase* GetNextTabStopSkinInSkinTree(BOOL bForward = TRUE);

public:
	//获得皮肤对象所依附的窗口句柄，并获得窗口句柄与对象之间坐标的偏移，此偏移包含相对父窗口的偏移
	//如果是SkinItem对象，则为SkinItem的句柄
	HWND GetSkinHwnd(LPPOINT lpOffset = NULL);

	//获得与皮肤依附的窗口的坐标偏移
	BOOL GetSkinHwndOffset(LPPOINT lpOffset);

	//获得皮肤对象依附的窗口句柄所属的皮肤对象
	CSkinItem* GetSkinHwndItem(LPPOINT lpOffset = NULL);

	//皮肤所属的皮肤引擎
	CSkinEngine* GetEngine() const;

	//获得图像变换参数
	const CImageTransformParam* GetImageParam() const;

	//获得皮肤名字
	virtual CString GetSkinName();

    //获得皮肤类型
    virtual CString GetSkinClassName();

    //获得皮肤预加载配置
    virtual CString GetSkinPreLoad();

	//获得皮肤的类名
	virtual CString GetClassName();

	//获得皮肤文字
	virtual CString GetSkinText();

    //Size Delay
    virtual BOOL GetSizeDelayFlag();
    virtual void SetSizeDelayFlag(BOOL bTrue);
    virtual BOOL ApplyDelayedSize(BOOL bRepaint = TRUE);
    void SetSubSkinSizeDelayFlag(BOOL bTrue);
    void ApplySubSkinDelayedSize(BOOL bRepaint = TRUE);

	//坐标转换
	//skin坐标转屏幕坐标
	virtual BOOL SkinToScreen(LONG& x, LONG& y);
	virtual BOOL SkinToScreen(POINT& point);
	virtual BOOL SkinToScreen(RECT& rect);
	//屏幕坐标转skin坐标
	virtual BOOL ScreenToSkin(LONG& x, LONG& y);
	virtual BOOL ScreenToSkin(POINT& point);
	virtual BOOL ScreenToSkin(RECT& rect);
	//skin坐标转父窗口坐标
	virtual BOOL SkinToSkinParent(LONG& x, LONG& y);
	virtual BOOL SkinToSkinParent(POINT& point);
	virtual BOOL SkinToSkinParent(RECT& rect);
	//父窗口坐标skin坐标
	virtual BOOL SkinParentToSkin(LONG& x, LONG& y);
	virtual BOOL SkinParentToSkin(POINT& point);
	virtual BOOL SkinParentToSkin(RECT& rect);
	//skin坐标转skin依附的窗口坐标
	virtual BOOL SkinToSkinHwnd(LONG& x, LONG& y);
	virtual BOOL SkinToSkinHwnd(POINT& point);
	virtual BOOL SkinToSkinHwnd(RECT& rect);
	//skin依附的窗口坐标转skin坐标
	virtual BOOL SkinHwndToSkin(LONG& x, LONG& y);
	virtual BOOL SkinHwndToSkin(POINT& point);
	virtual BOOL SkinHwndToSkin(RECT& rect);

	//获得皮肤矩形
	virtual CRect GetSkinRect();	//带相对父窗口的偏移
	virtual CRect GetSkinAbsRect();	//不带相对父窗口的偏移

	//判断点是否在皮肤矩形内
	virtual BOOL IsPointInSkinRect(POINT pt);
	virtual BOOL IsPointInSkinRect(LONG x, LONG y);

	//获得皮肤区域的拷贝，总是返回有效句柄
	//注意：这两个函数返回的是皮肤区域的一个拷贝，使用后需释放
	virtual HRGN GetSkinRgn();	//带相对父窗口的偏移
	virtual HRGN GetSkinAbsRgn();	//不带相对父窗口的偏移

	//判断点是否在皮肤区域内
	virtual BOOL IsPointInSkinRgn(POINT pt);
	virtual BOOL IsPointInSkinRgn(LONG x, LONG y);

	//皮肤光标
	virtual HCURSOR GetSkinCursor(LPPOINT lpPoint);

	//获得变换后的透明色
	virtual LONG GetTransformedTransparentColor();

	//////////////////////////////////////////////////////////////////////////
	//Helper
	//////////////////////////////////////////////////////////////////////////
	BOOL SetImageHelper(CImageBase*& pImage, CString& strImage, LPCTSTR newVal, BOOL bSetSize = FALSE);
	HICON GetIconHelper(LPCTSTR iconFileName, LONG iconSize = -1);
	HCURSOR GetCursorHelper(LPCTSTR curFileName);
    BOOL DrawImageAlphaSideHelper(CImageBase* pImage, HDC hDC, LPCRECT lpDrawRect, LPRECT lpClipRect = NULL, BOOL bAlpha = FALSE, BOOL bSideStretch = TRUE, LONG sideWidth = 0, LONG sideHeight = 0);
    BOOL DrawImageAlphaHelper(CImageBase* pImage, HDC hDC, LPCRECT lpDrawRect, LPRECT lpClipRect = NULL, BOOL bAlpha = FALSE);
    BOOL DrawImageHelper(CImageBase* pImage, HDC hDC, LPCRECT lpDrawRect, LPRECT lpClipRect = NULL);
	DWORD GetComctl32Version();
    BOOL LoadSkinClassHelper(LPCTSTR lpzClassName, LPCTSTR lpzSkinClassName);

public:
	//////////////////////////////////////////////////////////////////////////
	//属性的Get虚函数
	//////////////////////////////////////////////////////////////////////////
	//皮肤元素的名字
	virtual BSTR GetName();
    //类型
    virtual BSTR GetSkinClass();
    //类型
    virtual BSTR GetPreLoad();
	//皮肤的文本
	virtual BSTR GetText();
	//鼠标光标
	virtual BSTR GetCursor();
    virtual BSTR GetSysCursor();
	//设置皮肤元素的位置
	virtual LONG GetLeft();
	virtual LONG GetTop();
	virtual LONG GetWidth();
	virtual LONG GetHeight();
    //SizeDelay
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
	//接收鼠标消息
	virtual BOOL GetMouseEvent();
	//透明
	virtual BOOL GetTransparent();
	//颜色变换
	virtual BOOL GetColorTransform();
	//颜色变换
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
    //
    virtual BOOL GetScriptHandleEvent();

protected:
	//////////////////////////////////////////////////////////////////////////
	//属性的Set虚函数
	//////////////////////////////////////////////////////////////////////////
	//皮肤元素的名字
	virtual void SetName(LPCTSTR newVal);
    //类型
    virtual void SetSkinClass(LPCTSTR newVal);
    //类型
    virtual void SetPreLoad(LPCTSTR newVal);
	//文本
	virtual void SetText(LPCTSTR newVal);
	//鼠标光标
	virtual void SetCursor(LPCTSTR newVal);
	virtual void SetSysCursor(LPCTSTR newVal);
	//设置皮肤元素的位置
	virtual void SetLeft(LONG newVal);
	virtual void SetTop(LONG newVal);
	virtual void SetWidth(LONG newVal);
	virtual void SetHeight(LONG newVal);
    //SizeDelay
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
    //接收鼠标消息
    virtual void SetMouseEvent(BOOL newVal);
	//透明
	virtual void SetTransparent(BOOL newVal);
	//颜色变换
	virtual void SetColorTransform(BOOL newVal);
	//颜色变换
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
    //
    virtual void SetScriptHandleEvent(BOOL newVal);

    //////////////////////////////////////////////////////////////////////////
    //
    //////////////////////////////////////////////////////////////////////////
protected:
    CXmlCmdTarget* InternalCreateLayout(LPCTSTR lpzLayoutClass, LPCTSTR lpzLayoutName);
    CXmlCmdTarget* InternalGetLayout(LPCTSTR lpzLayoutName);
    BOOL InternalDestroyLayout(LPCTSTR lpzLayoutName);

	//////////////////////////////////////////////////////////////////////////
	//Dispatch Method
	//////////////////////////////////////////////////////////////////////////
protected:
	//Debug
	void DebugBreak(void);
	//Parent and Child
	IUnknown* GetParent(void);
	IUnknown* GetChild(LPCTSTR strChildName);
    IUnknown* CreateChild(LPCTSTR lpzXmlName, LPCTSTR lpzChildName);
	//Event
	VARIANT_BOOL AttachEvent(LPCTSTR strFuncName, IDispatch* pFuncDisp);
	VARIANT_BOOL DetachEvent(LPCTSTR strFuncName);
	//Timer
	UINT_PTR SetTimer(UINT nIDEvent, UINT nEclapse);
	VARIANT_BOOL KillTimer(UINT nIDEvent);
	//
	IUnknown* GetActive(void);
	IUnknown* SetActive(void);
	IUnknown* GetFocus(void);
	IUnknown* SetFocus(void);
	IUnknown* GetCapture(void);
	IUnknown* SetCapture(void);
	VARIANT_BOOL ReleaseCapture(void);
    //Layout
    IUnknown* CreateLayout(LPCTSTR lpzLayoutClass, LPCTSTR lpzLayoutName);
    IUnknown* GetLayout(LPCTSTR lpzLayoutName);
    VARIANT_BOOL DestroyLayout(LPCTSTR lpzLayoutName);

	//Internal AttachEvent and DetachEvent
protected:
	virtual BOOL InternalAttachEvent(LPCTSTR strFuncName, IDispatch* pFuncDisp);
	virtual BOOL InternalDetachEvent(LPCTSTR strFuncName);

#ifdef V8_JS_ENGINE_SUPPORT

    //v8实现
protected:
    //Debug
    v8::Handle<v8::Value> V8DebugBreak(const v8::Arguments& args);
    //Parent and Child
    v8::Handle<v8::Value> V8GetParent(const v8::Arguments& args);
    v8::Handle<v8::Value> V8GetChild(const v8::Arguments& args);
    v8::Handle<v8::Value> V8CreateChild(const v8::Arguments& args);
    //Event
    v8::Handle<v8::Value> V8AttachEvent(const v8::Arguments& args);
    v8::Handle<v8::Value> V8DetachEvent(const v8::Arguments& args);
    //Timer
    v8::Handle<v8::Value> V8SetTimer(const v8::Arguments& args);
    v8::Handle<v8::Value> V8KillTimer(const v8::Arguments& args);
    //
    v8::Handle<v8::Value> V8GetActive(const v8::Arguments& args);
    v8::Handle<v8::Value> V8SetActive(const v8::Arguments& args);
    v8::Handle<v8::Value> V8GetFocus(const v8::Arguments& args);
    v8::Handle<v8::Value> V8SetFocus(const v8::Arguments& args);
    v8::Handle<v8::Value> V8GetCapture(const v8::Arguments& args);
    v8::Handle<v8::Value> V8SetCapture(const v8::Arguments& args);
    v8::Handle<v8::Value> V8ReleaseCapture(const v8::Arguments& args);
    //Layout
    v8::Handle<v8::Value> V8CreateLayout(const v8::Arguments& args);
    v8::Handle<v8::Value> V8GetLayout(const v8::Arguments& args);
    v8::Handle<v8::Value> V8DestroyLayout(const v8::Arguments& args);

protected:
    virtual v8::Handle<v8::Value> V8InternalCallMethod(const CString& funcName, const v8::Arguments& args);

#endif //V8_JS_ENGINE_SUPPORT

public:
	//////////////////////////////////////////////////////////////////////////
	//Message Handler
	//////////////////////////////////////////////////////////////////////////
	//for CSkinBase
	virtual BOOL OnSetText();
	virtual BOOL OnMouseMove(LONG x, LONG y, UINT flag);
	virtual BOOL OnLButtonDown(LONG x, LONG y, UINT flag);
	virtual BOOL OnLButtonUp(LONG x, LONG y, UINT flag);
	virtual BOOL OnLButtonDblClk(LONG x, LONG y, UINT flag);
	virtual BOOL OnRButtonDown(LONG x, LONG y, UINT flag);
	virtual BOOL OnRButtonUp(LONG x, LONG y, UINT flag);
	virtual BOOL OnRButtonDblClk(LONG x, LONG y, UINT flag);
	virtual BOOL OnMButtonDown(LONG x, LONG y, UINT flag);
	virtual BOOL OnMButtonUp(LONG x, LONG y, UINT flag);
	virtual BOOL OnMButtonDblClk(LONG x, LONG y, UINT flag);
	virtual BOOL OnMouseWheel(LONG x, LONG y, UINT flag);
	virtual BOOL OnMouseHover(LONG x, LONG y, UINT flag);
	virtual BOOL OnMouseEnter();
	virtual BOOL OnMouseLeave();
	virtual BOOL OnShowWindow(BOOL bShow);
	virtual BOOL OnMove(LONG x, LONG y);
    virtual BOOL OnEnterSizeMove(BOOL bSize);
    virtual BOOL OnExitSizeMove(BOOL bSize);
	virtual BOOL OnFrameSize(LONG x, LONG y, UINT flag);
	virtual BOOL OnSize(LONG x, LONG y, UINT flag);
	virtual BOOL OnEnable(BOOL bEnable);
	virtual BOOL OnSetFocus(CSkinBase* pOldFocus);
	virtual BOOL OnKillFocus(CSkinBase* pNewFocus);
	virtual BOOL OnActivate(CSkinBase* pOther, UINT flag);
	virtual BOOL OnCaptureChanged(CSkinBase* pNewCapture);
	virtual BOOL OnKeyDown(UINT VKCode, UINT flag);
	virtual BOOL OnKeyUp(UINT VKCode, UINT flag);
	virtual BOOL OnChar(UINT VKCode, UINT flag);
	virtual BOOL OnTimer(UINT nIDEvent);
	virtual BOOL OnClick(LONG iTab = 0);
	virtual BOOL OnSkinNotify(UINT nCode, VARIANT param);
	virtual BOOL OnInitDialog(CSkinBase* pFocus);
    virtual BOOL OnSetState(UINT uState);
    virtual BOOL OnSetCheck(UINT uCheck);
    virtual BOOL OnUpdateHwnd();

#ifdef MS_JS_ENGINE_SUPPORT

	//////////////////////////////////////////////////////////////////////////
	//Fire Script Event
	//////////////////////////////////////////////////////////////////////////
	//for CSkinBase
	BOOL Fire_OnSetText();
	BOOL Fire_OnMouseMove(LONG x, LONG y, UINT flag);
	BOOL Fire_OnLButtonDown(LONG x, LONG y, UINT flag);
	BOOL Fire_OnLButtonUp(LONG x, LONG y, UINT flag);
	BOOL Fire_OnLButtonDblClk(LONG x, LONG y, UINT flag);
	BOOL Fire_OnRButtonDown(LONG x, LONG y, UINT flag);
	BOOL Fire_OnRButtonUp(LONG x, LONG y, UINT flag);
	BOOL Fire_OnRButtonDblClk(LONG x, LONG y, UINT flag);
	BOOL Fire_OnMButtonDown(LONG x, LONG y, UINT flag);
	BOOL Fire_OnMButtonUp(LONG x, LONG y, UINT flag);
	BOOL Fire_OnMButtonDblClk(LONG x, LONG y, UINT flag);
	BOOL Fire_OnMouseWheel(LONG x, LONG y, UINT flag);
	BOOL Fire_OnMouseHover(LONG x, LONG y, UINT flag);
	BOOL Fire_OnMouseEnter();
	BOOL Fire_OnMouseLeave();
	BOOL Fire_OnShowWindow(BOOL bShow);
	BOOL Fire_OnMove(LONG x, LONG y);
    BOOL Fire_OnEnterSizeMove(BOOL bSize);
    BOOL Fire_OnExitSizeMove(BOOL bSize);
	BOOL Fire_OnFrameSize(LONG x, LONG y, UINT flag);
	BOOL Fire_OnSize(LONG x, LONG y, UINT flag);
	BOOL Fire_OnEnable(BOOL bEnable);
	BOOL Fire_OnSetFocus(CSkinBase* pOldFocus);
	BOOL Fire_OnKillFocus(CSkinBase* pNewFocus);
	BOOL Fire_OnActivate(CSkinBase* pOther, UINT flag);
	BOOL Fire_OnCaptureChanged(CSkinBase* pNewCapture);
	BOOL Fire_OnKeyDown(UINT VKCode, UINT flag);
	BOOL Fire_OnKeyUp(UINT VKCode, UINT flag);
	BOOL Fire_OnChar(UINT VKCode, UINT flag);
	BOOL Fire_OnTimer(UINT nIDEvent);
	BOOL Fire_OnClick(LONG iTab);
	BOOL Fire_OnSkinNotify(UINT nCode, VARIANT param);
	BOOL Fire_OnInitDialog(CSkinBase* pFocus);
    BOOL Fire_OnSetState(UINT uState);
    BOOL Fire_OnSetCheck(UINT uCheck);
    BOOL Fire_OnUpdateHwnd();

#endif //MS_JS_ENGINE_SUPPORT

	//////////////////////////////////////////////////////////////////////////
	//Script方法绑定
	//////////////////////////////////////////////////////////////////////////
protected:
    //Dispatch实现
	std::map<CString, IDispatchPtr> m_mapDispatch;
	IDispatchPtr GetFunction(LPCTSTR strDispName);
	BOOL SetFunction(LPCTSTR strDispName, IDispatchPtr ptrDisp);

#ifdef V8_JS_ENGINE_SUPPORT

public:
    //V8实现
    BOOL V8Fire_OnSetText();
    BOOL V8Fire_OnMouseMove(LONG x, LONG y, UINT flag);
    BOOL V8Fire_OnLButtonDown(LONG x, LONG y, UINT flag);
    BOOL V8Fire_OnLButtonUp(LONG x, LONG y, UINT flag);
    BOOL V8Fire_OnLButtonDblClk(LONG x, LONG y, UINT flag);
    BOOL V8Fire_OnRButtonDown(LONG x, LONG y, UINT flag);
    BOOL V8Fire_OnRButtonUp(LONG x, LONG y, UINT flag);
    BOOL V8Fire_OnRButtonDblClk(LONG x, LONG y, UINT flag);
    BOOL V8Fire_OnMButtonDown(LONG x, LONG y, UINT flag);
    BOOL V8Fire_OnMButtonUp(LONG x, LONG y, UINT flag);
    BOOL V8Fire_OnMButtonDblClk(LONG x, LONG y, UINT flag);
    BOOL V8Fire_OnMouseWheel(LONG x, LONG y, UINT flag);
    BOOL V8Fire_OnMouseHover(LONG x, LONG y, UINT flag);
    BOOL V8Fire_OnMouseEnter();
    BOOL V8Fire_OnMouseLeave();
    BOOL V8Fire_OnShowWindow(BOOL bShow);
    BOOL V8Fire_OnMove(LONG x, LONG y);
    BOOL V8Fire_OnEnterSizeMove(BOOL bSize);
    BOOL V8Fire_OnExitSizeMove(BOOL bSize);
	BOOL V8Fire_OnFrameSize(LONG x, LONG y, UINT flag);
    BOOL V8Fire_OnSize(LONG x, LONG y, UINT flag);
    BOOL V8Fire_OnEnable(BOOL bEnable);
    BOOL V8Fire_OnSetFocus(CSkinBase* pOldFocus);
    BOOL V8Fire_OnKillFocus(CSkinBase* pNewFocus);
    BOOL V8Fire_OnActivate(CSkinBase* pOther, UINT flag);
    BOOL V8Fire_OnCaptureChanged(CSkinBase* pNewCapture);
    BOOL V8Fire_OnKeyDown(UINT VKCode, UINT flag);
    BOOL V8Fire_OnKeyUp(UINT VKCode, UINT flag);
    BOOL V8Fire_OnChar(UINT VKCode, UINT flag);
    BOOL V8Fire_OnTimer(UINT nIDEvent);
    BOOL V8Fire_OnClick(LONG iTab);
    BOOL V8Fire_OnSkinNotify(UINT nCode, VARIANT param);
    BOOL V8Fire_OnInitDialog(CSkinBase* pFocus);
    BOOL V8Fire_OnSetState(UINT uState);
    BOOL V8Fire_OnSetCheck(UINT uCheck);
    BOOL V8Fire_OnUpdateHwnd();

protected:
    std::map<CString, CV8ContextFunction*> m_mapV8ContextFunction;
    CV8ContextFunction* V8GetFunction(LPCTSTR strDispName);
    BOOL V8SetFunction(LPCTSTR strDispName, v8::Handle<v8::Function> object);
    void V8ClearAllFunction();

    virtual BOOL V8InternalAttachEvent(v8::Handle<v8::String> strFuncName, v8::Handle<v8::Function> pFunc);
    virtual BOOL V8InternalDetachEvent(v8::Handle<v8::String> strFuncName);

#endif //V8_JS_ENGINE_SUPPORT

public:
	//绘制与刷新
	virtual void DrawParentBackground(CDC* pDC, LPRECT lpInvalidRect = NULL, CRgn* pInvalidRgn = NULL);
	virtual void DrawBackground(CDC* pDC, LPRECT lpInvalidRect = NULL, CRgn* pInvalidRgn = NULL, BOOL bChildSkin = FALSE);
	virtual void DrawForeground(CDC* pDC, LPRECT lpInvalidRect = NULL, CRgn* pInvalidRgn = NULL);
	virtual void DrawFrame(CDC* pDC, LPRECT lpInvalidRect = NULL, CRgn* pInvalidRgn = NULL);
	virtual void Draw(CDC* pDC, LPRECT lpInvalidRect = NULL, CRgn* pInvalidRgn = NULL);
	virtual BOOL InvalidateRect(LPCRECT pRect = NULL, BOOL bErase = TRUE);

	//区域更新
	virtual BOOL UpdateSkinRgn(LONG width, LONG height, BOOL bReDraw = FALSE);
	virtual BOOL MoveWindow(LPCRECT lpRect = NULL, BOOL bRepaint = FALSE);

protected:
	CSkinEngine* const	m_pEngine;		//皮肤所属的皮肤引擎指针，由所属的皮肤引擎绑定和解绑定
	CSkinBase*		m_pSkinParent;		//父窗口指针
	CSkinBase*		m_pSkinOwner;		//Owner窗口指针
	CSkinBase*		m_pSkinNext;		//下一兄弟窗口指针
	CSkinBase*		m_pSkinPrev;		//前一兄弟窗口指针
	CSkinProxy*		m_pSkinProxy;		//代理对象
	CString			m_strName;			//皮肤元素的名字，作为皮肤元素的唯一性ID，只能设置一次
    CString         m_strSkinClass;     //皮肤元素的类型，为属性的一个集合
    CString         m_strPreLoad;       //预加载的配置
	CString			m_strText;			//皮肤的文字
	HCURSOR			m_hCursor;			//鼠标光标
	HCURSOR			m_hSysCursor;		//系统光标
	CString			m_strCursor;		//光标文件名
	CString			m_strSysCursor;		//系统光标文件名
	CRect			m_rcSkinRect;		//皮肤元素的矩形区域，坐标为相对父皮肤对象
	HRGN			m_hSkinRgn;			//皮肤元素的区域
	LONG			m_nUpCornerRadius;	//区域上边角圆化半径
	LONG			m_nDownCornerRadius;//区域下边角圆化半径
	LONG			m_nBorderLineWidth;	//边线线宽
	LONG			m_clrBorderLineColor;//边线颜色
	LONG			m_clrTransparentColor;//透明色
	BOOL			m_bVisible;			//可见
	BOOL			m_bDisabled;		//可用
	BOOL			m_bTabStop;			//Tab Stop
	BOOL			m_bGroup;			//Group
	BOOL			m_bBackground;		//是否是背景元素
	BOOL			m_bMouseEvent;		//是否接收鼠标消息
	BOOL			m_bTransparent;		//是否是透明的，即由父窗口绘制背景
	BOOL			m_bColorTransform;	//是否颜色变换
    BOOL            m_bAlpha;           //打开alpha通道
    BOOL            m_bSizeDelay;       //延迟Size改变的生效
    BOOL            m_bSizeDelayFlag;   //延迟Size标识，由父窗口设置
    BOOL            m_bScriptHandleEvent;//脚本全权响应消息

    //////////////////////////////////////////////////////////////////////////
    //Layout
    //////////////////////////////////////////////////////////////////////////
protected:
    typedef std::vector<std::pair<CString, CXmlCmdTarget*>> LayoutArray;
    LayoutArray m_vecLayouts;

	//////////////////////////////////////////////////////////////////////////
	//Active Focus Capture 状态
	//////////////////////////////////////////////////////////////////////////
public:
	BOOL GetActiveSkin(CSkinBase** ppSkin = NULL);
	BOOL SetActiveSkin(CSkinBase** ppPrevSkin = NULL);
	BOOL GetFocusSkin(CSkinBase** ppSkin = NULL);
	BOOL SetFocusSkin(CSkinBase** ppPrevSkin = NULL);
	BOOL GetCaptureSkin(CSkinBase** ppSkin = NULL);
	BOOL SetCaptureSkin(CSkinBase** ppPrevSkin = NULL);
	BOOL ReleaseCaptureSkin();

	//////////////////////////////////////////////////////////////////////////
	//元素属性过滤
	//////////////////////////////////////////////////////////////////////////
public:
	enum SKIN_FILTER
	{
		FILTER_SKINITEM = 0x00000001,
		FILTER_VISIBLE = 0x00000002,
		FILTER_DISABLED = 0x00000004,
		FILTER_TABSTOP = 0x00000008,
		FILTER_GROUP = 0x00000010,
		FILTER_BACKGROUND = 0x00000020,
        FILTER_MOUSEEVENT = 0x00000040,
		FILTER_TRANSPARENT = 0x00000080,
	};

	//判断是否满足过滤属性
	virtual BOOL IsFiltered(UINT uFilterBits, UINT uFilterValues);

	//////////////////////////////////////////////////////////////////////////
	//子元素相关
	//////////////////////////////////////////////////////////////////////////
public:
	//添加、删除、判断、替换子元素
	BOOL AddSubSkin(CSkinBase* pSkin);
	BOOL RemoveSubSkin(CSkinBase* pSkin);
	BOOL IsSubSkin(CSkinBase* pSkin) const;
	BOOL ReplaceSubSkin(CSkinBase* pOldSkin, CSkinBase* pNewSkin);

public:
	//子元素属性与方法的调用函数
	HRESULT GetSubSkinProp(BSTR bstrSubSkinName, BSTR bstrPropName, VARIANT* pVarValue);
	HRESULT SetSubSkinProp(BSTR bstrSubSkinName, BSTR bstrPropName, VARIANT varValue);
	HRESULT CallSubSkinMethod(BSTR bstrSubSkinName, BSTR bstrFuncName, VARIANT* pVarResult, DISPPARAMS* pDispParams);
	HRESULT GetSubSkinPos(BSTR bstrSubSkinName, LONG* pLeft, LONG* pTop, LONG* pRight, LONG* pBottom);

	//是否是子窗口
	BOOL IsChildOf(CSkinBase* pParent);
	//是否是子孙窗口
	BOOL IsDescendantOf(CSkinBase* pAncestor);
	//是否是亲属窗口，即有共同的祖先窗口
	BOOL IsRelativeOf(CSkinBase* pRelative);

	//获得子元素
	CSkinBase* GetSubSkin(LPCTSTR strSubSkinName);
	CSkinBase* GetSubSkin(BSTR bstrSubSkinName);

	//获得子元素的区域和
	HRGN GetAllSubSkinRgn(UINT uFilterBits = (FILTER_SKINITEM | FILTER_VISIBLE),
		UINT uFilterValues = (FILTER_SKINITEM | FILTER_VISIBLE) );

	//获得点所在的且满足属性的子元素
	CSkinBase* GetPointInSubSkin(LONG x, LONG y, BOOL bReverse = TRUE, UINT uFilterBits = (FILTER_VISIBLE | FILTER_BACKGROUND | FILTER_MOUSEEVENT),
		UINT uFilterValues = (FILTER_VISIBLE | FILTER_MOUSEEVENT) );
	CSkinBase* GetPointInSubSkin(POINT pt, BOOL bReverse = TRUE, UINT uFilterBits = (FILTER_VISIBLE | FILTER_BACKGROUND | FILTER_MOUSEEVENT),
		UINT uFilterValues = (FILTER_VISIBLE | FILTER_MOUSEEVENT) );

protected:
	std::list<CSkinBase*>	m_listSkinBase;		//所有子元素

#ifdef _DEBUG
	//////////////////////////////////////////////////////////////////////////
	//静态成员，SkinBase计数
	//////////////////////////////////////////////////////////////////////////
protected:
	static int s_iSkinBaseCount;
#endif
};
