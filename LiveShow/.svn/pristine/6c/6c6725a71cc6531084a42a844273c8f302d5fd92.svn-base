#pragma once

#include <afxhtml.h>
#include <comdef.h>
#include <string>
#include "DocHostSite.h"
#include "../ExportMacro.h"
#include <mshtmdid.h>//定义了HTMLDocumentEvents2接口的方法DISPID


#define WM_LOADJSREADY					WM_USER+6513
#define WM_CLOSEDIALOG					WM_USER+6514
#define WM_MISSION_COMPLETED			WM_USER+6515
#define WM_MISSION_HYPERLINK_ONCLICK	WM_USER+6516    
#define WM_MISSION_SHOW_MESSAGEBOX		WM_USER+6517
#define WM_MISSION_HYPERLINK_ENTERROOM	WM_USER+6518
#define WM_MISSION_HYPERLINK_OPENURL	WM_USER+6519
#define WM_MISSION_HYPERLINK_INLINEURL	WM_USER+6520
#define WM_MISSION_HYPERLINK_LOGIN		WM_USER+6521
#define WM_MISSION_HYPERLINK_SYSSETTING WM_USER+6522
#define WM_MISSION_NAVIGATECOMPLETE2    WM_USER+6523
#define WM_MISSION_HTML_ERROR           WM_USER+6524
#define WM_MISSION_HYPERLINK_LOGINENTERROOM WM_USER+6525
#define WM_MISSION_HYPERLINK_MOUSEMOVE  WM_USER+6526
#define WM_MISSION_HYPERLINK_MOUSEOUT  WM_USER+6527
#define WM_MISSION_HYPERLINK_ONCLICK_MOUSEUP	WM_USER+6528


class CHtmlViewDesigner : public IHTMLEditDesigner
{
public:
	CHtmlViewDesigner();

	BOOL			Attach(IHTMLDocument2 *pDoc);  //附加要载取鼠标事件的页面
	void			Detach();

	virtual HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, void __RPC_FAR *__RPC_FAR *ppvObject);
	virtual ULONG	STDMETHODCALLTYPE AddRef(void);
	virtual ULONG	STDMETHODCALLTYPE Release(void);

	virtual HRESULT STDMETHODCALLTYPE PreHandleEvent(DISPID inEvtDispId, IHTMLEventObj *pIEventObj);
	virtual HRESULT STDMETHODCALLTYPE PostHandleEvent(DISPID inEvtDispId, IHTMLEventObj *pIEventObj);
	virtual HRESULT STDMETHODCALLTYPE TranslateAccelerator(DISPID inEvtDispId, IHTMLEventObj *pIEventObj);
	virtual HRESULT STDMETHODCALLTYPE PostEditorEventNotify(DISPID inEvtDispId, IHTMLEventObj *pIEventObj);

	void SetRespondHWND(HWND pParentHWND);
	void SetNeedEvent(bool bNeedOnClick=false,bool bNeedMouseMove=false );

protected:
	CComPtr<IHTMLEditServices> m_pServices;
	HWND m_pParentHWND;
	bool m_needOnClickEvent;//是否需要点击事件
	bool m_needMouseMove;  //是否需要鼠标移动事件
};

class COMMON_INTERFACE CMLHtmlView : public CHtmlView
{
	DECLARE_DYNCREATE(CMLHtmlView)
	DECLARE_DISPATCH_MAP()
	DECLARE_MESSAGE_MAP()
    DECLARE_EVENTSINK_MAP()

public:
	CMLHtmlView(BOOL IsView = TRUE);
	virtual ~CMLHtmlView();

public:
	enum CONTEXT_MENU_MODE		// 上下文菜单
	{
		NoContextMenu,			// 无菜单
		DefaultMenu,			// 默认菜单
		TextSelectionOnly,		// 仅文本选择菜单
		CustomMenu				// 自定义菜单
	};

protected:
	CONTEXT_MENU_MODE m_ContextMenuMode;			// 上下文菜单模式
	CString m_DefaultMsgBoxTitle;					// 默认MessageBox标题
	DWORD	m_HostInfoFlag;							// 浏览器的一些设置如是否有滚动条等
	BOOL	m_IsView;								// 是否是视图

	CComPtr<IHTMLDocument2>	m_spDoc;
	CHtmlViewDesigner m_ViewDesigner; 
	CWnd* m_pParent;

    std::wstring m_strURL;
	bool m_needOnClickEvent;//是否需要点击事件
	bool m_needChangeMessageBox;//是否需要拦截MessageBox
	bool m_needMouseMove;  //是否需要鼠标移动事件

    LPDISPATCH m_lpDisp;//用于判断网页加载是否真正完毕

public:
	// 设置上下文菜单模式
	//void SetContextMenuMode(CONTEXT_MENU_MODE MenuMode);
	// 取上下文菜单模式
	//CONTEXT_MENU_MODE GetContextMenuMode();
	// 设置默认MessageBox标题
	//void SetDefaultMsgBoxTitle(CString strTitle);
	// 更改浏览器的一些设置如是否有滚动条等
	void SetHostInfoFlag(DWORD dwFlag);

public:
	// 当在一个窗体上创建时（当作窗体的一个控件）可以指定一个控件ID按指定的控件大小位置
	BOOL CreateFromStatic(UINT nID, CWnd* pParent);
	BOOL Create(CRect rect, CWnd* pParent, UINT nID);
	//bNeedOnClick:是否需要点击事件 bNeedChangeMessageBox:是否需要拦截messagebox   bNeedMouseMove:是否需要拦截鼠标移动事件
    void SetUrl(std::wstring& url, bool bNeedOnClick=false, bool bNeedChangeMessageBox=false,bool bNeedMouseMove=false);
    void UpdateURL();

public:

#if (_WIN32_IE >= 0x0501) // IE 5.5 and higher
	// IDocHostUIHandler2 overrideables
	virtual HRESULT OnGetOverrideKeyPath(LPOLESTR * pchKey, DWORD dw);

#endif
	virtual BOOL CreateControlSite(COleControlContainer* pContainer,COleControlSite** ppSite, UINT nID, REFCLSID clsid);
	// MesseBox
	virtual HRESULT OnShowMessage(HWND hwnd,LPOLESTR lpstrText,LPOLESTR lpstrCaption,DWORD dwType,LPOLESTR lpstrHelpFile,DWORD dwHelpContext,LRESULT * plResult);
	// 定制宿主信息
	virtual HRESULT OnGetHostInfo(DOCHOSTUIINFO * pInfo);
	// 上下文菜单
	//virtual HRESULT OnShowContextMenu(DWORD dwID, LPPOINT ppt,LPUNKNOWN pcmdtReserved, LPDISPATCH pdispReserved);
	// 显示定制上下文菜单
	//virtual HRESULT OnShowCustomContextMenu(LPPOINT ppt, LPUNKNOWN pcmdtReserved,LPDISPATCH pdispReserved);
	// 设置脚本扩展
	virtual HRESULT OnGetExternal(LPDISPATCH *lppDispatch);
	// 获取自定义拖放对象
	//virtual HRESULT OnGetDropTarget(LPDROPTARGET pDropTarget,LPDROPTARGET* ppDropTarget);
    virtual void OnNavigateError(LPCTSTR lpszURL, LPCTSTR lpszFrame, DWORD dwError, BOOL *pbCancel);

protected:
	const CString GetSystemErrorMessage(DWORD dwError);
	CString GetNextToken(CString& strSrc, const CString strDelim,BOOL bTrim = FALSE, BOOL bFindOneOf = TRUE);
	
	BOOL SetScriptDocument();
	const CString GetLastError() const;
	BOOL GetJScript(CComPtr<IDispatch>& spDisp);
	BOOL GetJScripts(CComPtr<IHTMLElementCollection>& spColl);
	CString ScanJScript(CString& strAText, CStringArray& args);

    //Invoke_*** web调用JavaScript，客户端得到回调
	BOOL Invoke_EnterRoom(int roomID);//进房
	BOOL Invoke_EnterRoom2(int roomID);//隐身进房
	BOOL Invoke_LoginEnterRoom(LPCTSTR lpszURL);//登录进房
	BOOL Invoke_OpenURL(LPCTSTR lpszURL);//带token打开链接  (默认浏览器)
	BOOL Invoke_InLineURL(LPCTSTR lpszURL); //带token打开内嵌链接  (大厅右侧内嵌页面)
	BOOL Invoke_Login(LPCTSTR lpszURL);  //注册对话框中直接登录
	BOOL Invoke_SysSetting();  //页面中有修改设置对话框里的内容

public:
	BOOL IsJScriptOK();
	BOOL CallJScript(const CString strFunc,_variant_t* pVarResult = NULL);
	BOOL CallJScript(const CString strFunc,const CString strArg1,_variant_t* pVarResult = NULL);
	BOOL CallJScript(const CString strFunc,const CString strArg1,const CString strArg2,_variant_t* pVarResult = NULL);
	BOOL CallJScript(const CString strFunc,const CString strArg1,const CString strArg2,const CString strArg3,_variant_t* pVarResult = NULL);
	BOOL CallJScript(const CString strFunc,const CStringArray& paramArray,_variant_t* pVarResult = NULL);

protected:
	virtual void PostNcDestroy();
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void DocumentComplete(LPDISPATCH pDisp, VARIANT* URL);
    virtual void NavigateComplete2(LPDISPATCH pDisp, VARIANT* URL);

public:
	// 修改网页元素的内容
	BOOL PutElementHtml(CString ElemID,CString Html);
	// 取表单元素的值
	BOOL GetElementValue(CString ElemID,CString& Value);
	// 设置元素的值
	BOOL PutElementValue(CString ElemID,CString Value);
	// 给元素设置焦点
	void ElementSetFocus(CString EleName);
	IHTMLStyle* GetElementStyle(CString ElemID);
	HRESULT GetElement(LPCTSTR szElementId, IDispatch **ppdisp, BOOL *pbCollection = NULL);
	HRESULT GetElement(LPCTSTR szElementId, IHTMLElement **pphtmlElement);
	HRESULT GetElementInterface(LPCTSTR szElementId, REFIID riid, void **ppvObj);
	CString GetValue(CComPtr<IHTMLElement> & pEvtObj,CString key) ;
	void GetScrollSize(int &iWidth, int &iHeight);

public:
	afx_msg void OnDestroy();
	afx_msg int OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
};




