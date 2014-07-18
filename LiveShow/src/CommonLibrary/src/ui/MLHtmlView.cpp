#include "stdafx.h"
#include "../../include/ui/MLHtmlView.h"
#include <ExDispid.h>
#include "resource.h"
#include "assert.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


CHtmlViewDesigner::CHtmlViewDesigner()
{
	m_pServices = (IHTMLEditServices *) NULL;
	m_needOnClickEvent = false;
	m_needMouseMove = false;
}

void CHtmlViewDesigner::SetRespondHWND(HWND pParentHWND)
{
	m_pParentHWND = pParentHWND;
}

void CHtmlViewDesigner::SetNeedEvent(bool bNeedOnClick,bool bNeedMouseMove )
{
	m_needOnClickEvent = bNeedOnClick;
	m_needMouseMove = bNeedMouseMove;

}

BOOL CHtmlViewDesigner::Attach(IHTMLDocument2 *pDoc)
{
	Detach();

	CComPtr<IServiceProvider> pTemp;

	if (pDoc ==	(IHTMLDocument2	*) NULL)
		return FALSE;

	pDoc->QueryInterface(IID_IServiceProvider, (void **) &pTemp);

	if (pTemp != (IServiceProvider *) NULL)
	{
		pTemp->QueryService(SID_SHTMLEditServices, IID_IHTMLEditServices, (void	**)	&m_pServices);

		if (m_pServices	!= (IHTMLEditServices *) NULL)
		{
			HRESULT result = m_pServices->AddDesigner(this);
			return result==S_OK;
		}
	}

	return FALSE;
}

void CHtmlViewDesigner::Detach()
{
	if (m_pServices != (IHTMLEditServices *) NULL)
	{
		m_pServices->RemoveDesigner(this);
		m_pServices = NULL;
	}
}

HRESULT STDMETHODCALLTYPE CHtmlViewDesigner::PreHandleEvent(DISPID inEvtDispId, IHTMLEventObj *pIEventObj)
{
	USES_CONVERSION;
	CComPtr<IHTMLElement>  pSel;
	BSTR b = BSTR(NULL);

	switch(inEvtDispId)
	{
	case DISPID_HTMLELEMENTEVENTS2_ONMOUSEDOWN:  //载取页面的鼠标点击事件 mousedown
		{
			if(!m_needOnClickEvent)
			{
				break;
			}

			if (pIEventObj != (IHTMLEventObj *) NULL)
			{
				pIEventObj->get_srcElement(&pSel);	

				//	We've got our source element, get it's inner text
				if (pSel != (IHTMLElement *) NULL)
				{
					pSel->get_tagName(&b);

					if (_tcsicmp(L"A", (b)) == 0 || _tcsicmp(L"a", (b)) == 0)
					{
						TRACE(_T("web MouseDown\n"));
						::SendMessage(m_pParentHWND,WM_MISSION_HYPERLINK_ONCLICK, 0, (LPARAM)pIEventObj);

						return S_OK;
					}
				}
			}
			break;
		}

	case DISPID_HTMLELEMENTEVENTS2_ONMOUSEUP:  //载取页面的鼠标点击事件
		{
			if(!m_needOnClickEvent)
			{
				break;
			}

			if (pIEventObj != (IHTMLEventObj *) NULL)
			{
				pIEventObj->get_srcElement(&pSel);	

				//	We've got our source element, get it's inner text
				if (pSel != (IHTMLElement *) NULL)
				{
					pSel->get_tagName(&b);

					if (_tcsicmp(L"A", (b)) == 0 || _tcsicmp(L"a", (b)) == 0)
					{
						TRACE(_T("web MouseUp\n"));
						::SendMessage(m_pParentHWND,WM_MISSION_HYPERLINK_ONCLICK_MOUSEUP, 0, (LPARAM)pIEventObj);

						return S_FALSE;
					}
				}
			}
			break;
		}


	case DISPID_HTMLELEMENTEVENTS2_ONMOUSEMOVE:  //载取页面的鼠标移动事件  OnMouseMove
		{
			if(m_needMouseMove)
			{
				CPoint point ;
				GetCursorPos( &point ) ;
				::SendMessage(m_pParentHWND,WM_MISSION_HYPERLINK_MOUSEMOVE, 0, (LPARAM)&point);
				break;
			}
			break;
		}
	
	case DISPID_HTMLELEMENTEVENTS2_ONMOUSEOUT:  //载取页面的鼠标移动事件  OnMouseOut
		{
			if(m_needMouseMove)
			{
				CPoint point ;
				GetCursorPos( &point ) ;
				::SendMessage(m_pParentHWND,WM_MISSION_HYPERLINK_MOUSEOUT, 0, (LPARAM)&point);
				break;
			}
			break;
		}
		
	default:
		break;
	}

	return S_FALSE;
}


HRESULT STDMETHODCALLTYPE CHtmlViewDesigner::QueryInterface(REFIID riid, void __RPC_FAR *__RPC_FAR *ppvObject)
{
	HRESULT hrRet = S_OK;

	// Standard QI() initialization - set *ppv to NULL.
	*ppvObject = NULL;

	// If the client is requesting an interface we support, set *ppvObject.
	if (IsEqualIID(riid, IID_IUnknown))
		*ppvObject = (IUnknown *) this;
	else if (IsEqualIID(riid, IID_IHTMLEditDesigner))
		*ppvObject = (IHTMLEditDesigner *) this;
	else
		// We don't support the interface the client is asking for.
		hrRet = E_NOINTERFACE;

	// If we're returning an interface pointer, AddRef() it.
	if (S_OK == hrRet)
		((IUnknown *) *ppvObject)->AddRef();

	return hrRet;
}

ULONG STDMETHODCALLTYPE CHtmlViewDesigner::AddRef(void)
{
	return 0;
}

ULONG STDMETHODCALLTYPE CHtmlViewDesigner::Release(void)
{
	return 0;
}

HRESULT STDMETHODCALLTYPE CHtmlViewDesigner::PostHandleEvent(DISPID inEvtDispId, IHTMLEventObj * pIEventObj)
{
	return S_FALSE;
}

HRESULT STDMETHODCALLTYPE CHtmlViewDesigner::TranslateAccelerator(DISPID /*inEvtDispId*/, IHTMLEventObj * /*pIEventObj*/)
{
	return S_FALSE;
}

HRESULT STDMETHODCALLTYPE CHtmlViewDesigner::PostEditorEventNotify(DISPID inEvtDispId, IHTMLEventObj * pIEventObj)
{
	return S_FALSE;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

IMPLEMENT_DYNCREATE(CMLHtmlView, CHtmlView)

	BEGIN_DISPATCH_MAP(CMLHtmlView, CHtmlView)
		DISP_FUNCTION(CMLHtmlView, "invoke_enterroom", Invoke_EnterRoom, VT_BOOL, VTS_I4)
		DISP_FUNCTION(CMLHtmlView, "invoke_enterroom2", Invoke_EnterRoom2, VT_BOOL, VTS_I4)
		DISP_FUNCTION(CMLHtmlView, "invoke_loginenterroom", Invoke_LoginEnterRoom, VT_BOOL, VTS_BSTR)
		DISP_FUNCTION(CMLHtmlView, "invoke_openurl", Invoke_OpenURL, VT_BOOL, VTS_BSTR)
		DISP_FUNCTION(CMLHtmlView, "invoke_inlineurl", Invoke_InLineURL, VT_BOOL, VTS_BSTR)
		DISP_FUNCTION(CMLHtmlView, "invoke_login", Invoke_Login, VT_BOOL, VTS_BSTR)
		DISP_FUNCTION(CMLHtmlView, "invoke_syssetting", Invoke_SysSetting, VT_BOOL, VTS_NONE)
	END_DISPATCH_MAP()

	BEGIN_EVENTSINK_MAP(CMLHtmlView, CHtmlView)
		//ON_EVENT_REFLECT(CMLHtmlView, DISPID_PROGRESSCHANGE, OnProgressChange, VTS_I4 VTS_I4)
		//ON_EVENT(CMLHtmlView, IDC_BROWSER, DISPID_NAVIGATEERROR, OnNavigateError, VTS_DISPATCH VTS_PVARIANT VTS_PVARIANT VTS_PVARIANT VTS_PBOOL)
		//ON_EVENT(CMLHtmlView, IDC_BROWSER, DISPID_DOCUMENTCOMPLETE, OnDocumentComplete, VTS_DISPATCH VTS_PVARIANT)
	END_EVENTSINK_MAP()

	BEGIN_MESSAGE_MAP(CMLHtmlView, CHtmlView)
		ON_WM_DESTROY()
		ON_WM_MOUSEACTIVATE()
		ON_WM_SIZE()
		ON_WM_CREATE()
	END_MESSAGE_MAP()

	CMLHtmlView::CMLHtmlView(BOOL IsView)
{
	m_ContextMenuMode=DefaultMenu;
	m_DefaultMsgBoxTitle="LHP HTMLVIEW";
	m_HostInfoFlag = DOCHOSTUIFLAG_THEME | DOCHOSTUIFLAG_NO3DBORDER | DOCHOSTUIFLAG_NO3DOUTERBORDER;
	m_IsView = IsView;
	m_spDoc  = NULL;
	m_pParent= NULL;
    m_lpDisp = NULL;

    m_strURL = L"about:blank";
	m_needOnClickEvent = false;
	m_needMouseMove = false;

	EnableAutomation();
}

CMLHtmlView::~CMLHtmlView()
{
	m_ViewDesigner.Detach();
}

//进房
//html中调用方法    onclick="window.external.invoke_enterroom(1111);"
BOOL CMLHtmlView::Invoke_EnterRoom(int roomID)
{
	OutputDebugStringA("plushu Invoke_EnterRoom");
	return S_OK == m_pParent->SendMessage(WM_MISSION_HYPERLINK_ENTERROOM, roomID, 0);
}

//隐身登陆
BOOL CMLHtmlView::Invoke_EnterRoom2(int roomID)
{
	OutputDebugStringA("plushu Invoke_EnterRoom2");
	return S_OK == m_pParent->SendMessage(WM_MISSION_HYPERLINK_ENTERROOM, roomID, 1);
}

//登录进房
//html中调用方法    onclick="window.external.invoke_loginenterroom('100001|dxgss@126.com|1235678');"
BOOL CMLHtmlView::Invoke_LoginEnterRoom(LPCTSTR lpszURL)
{
	OutputDebugStringA("plushu Invoke_LoginEnterRoom");
	return S_OK == m_pParent->SendMessage(WM_MISSION_HYPERLINK_LOGINENTERROOM, (WPARAM)lpszURL, 0);
}

//带token打开链接  (默认浏览器)
//html中调用方法    o nclick="window.external.invoke_openurl('http://www.51.com');"
BOOL CMLHtmlView::Invoke_OpenURL(LPCTSTR lpszURL)
{
	OutputDebugStringA("plushu Invoke_OpenURL");
	return S_OK == m_pParent->SendMessage(WM_MISSION_HYPERLINK_OPENURL, (WPARAM)lpszURL, 0);
}

//带token打开内嵌链接  (大厅右侧内嵌页面)
//html中调用方法    onclick="window.external.invoke_inlineurl('http://www.51.com');"
BOOL CMLHtmlView::Invoke_InLineURL(LPCTSTR lpszURL)
{
	OutputDebugStringA("plushu Invoke_InLineURL");
	return S_OK == m_pParent->SendMessage(WM_MISSION_HYPERLINK_INLINEURL, (WPARAM)lpszURL, 0);
}

//注册对话框中直接登录(注册邮箱已被占用时会出现)
BOOL CMLHtmlView::Invoke_Login(LPCTSTR lpszURL)
{
	OutputDebugStringA("plushu Invoke_Login");
	return S_OK == m_pParent->SendMessage(WM_MISSION_HYPERLINK_LOGIN, (WPARAM)lpszURL, 0);
}

//页面中有修改设置对话框里的内容
BOOL CMLHtmlView::Invoke_SysSetting()
{
	OutputDebugStringA("plushu Invoke_SysSetting");
	return S_OK == m_pParent->SendMessage(WM_MISSION_HYPERLINK_SYSSETTING, 0, 0);
}

// void CMLHtmlView::SetContextMenuMode(CONTEXT_MENU_MODE MenuMode)
// {
// 	m_ContextMenuMode=MenuMode;
// }
// 
// CMLHtmlView::CONTEXT_MENU_MODE CMLHtmlView::GetContextMenuMode()
// {
// 	return m_ContextMenuMode;
// }
// 
// void CMLHtmlView::SetDefaultMsgBoxTitle(CString strTitle)
// {
// 	m_DefaultMsgBoxTitle=strTitle;
// }

void CMLHtmlView::SetHostInfoFlag(DWORD dwFlag)
{
	m_HostInfoFlag = dwFlag;
}

BOOL CMLHtmlView::CreateControlSite(COleControlContainer * pContainer, COleControlSite ** ppSite, UINT /*nID*/, REFCLSID /*clsid*/)
{
	*ppSite = new CDocHostSite(pContainer, this);// 创建自己的控制站点实例

	return (*ppSite) ? TRUE : FALSE;
}

HRESULT CMLHtmlView::OnShowMessage(HWND hwnd,LPOLESTR lpstrText,LPOLESTR lpstrCaption,DWORD dwType,LPOLESTR lpstrHelpFile,DWORD dwHelpContext,LRESULT * plResult)
{
	if(!m_needChangeMessageBox)
		return S_FALSE;

	CString strTitle = lpstrCaption;
	if(m_pParent != NULL /*&& strTitle.CompareNoCase(L"Windows Internet Explorer") == 0*/)
	{
		m_pParent->SendMessage(WM_MISSION_SHOW_MESSAGEBOX, WPARAM(lpstrText), 0);
		return S_OK;
	}

	return S_FALSE;	
}

HRESULT CMLHtmlView::OnGetHostInfo(DOCHOSTUIINFO * pInfo)
{
	pInfo->cbSize = sizeof(DOCHOSTUIINFO);
	pInfo->dwFlags = m_HostInfoFlag;
	pInfo->dwDoubleClick = DOCHOSTUIDBLCLK_DEFAULT;

	return S_OK;
}

#if (_WIN32_IE >= 0x0501) // IE 5.5 and higher

HRESULT CMLHtmlView::OnGetOverrideKeyPath(LPOLESTR *, DWORD)
{
	return E_NOTIMPL;
}

#endif

HRESULT CMLHtmlView::OnGetExternal(LPDISPATCH *lppDispatch)
{
	*lppDispatch = GetIDispatch(TRUE);
	return S_OK;
}

void CMLHtmlView::ElementSetFocus(CString ElemID)
{
	IHTMLDocument2* pDoc = NULL;
	IHTMLElementCollection* pAllElem = NULL;
	IHTMLElement* pElem = NULL;
	IHTMLControlElement* pControl=NULL;
	HRESULT hr = S_FALSE;

	pDoc = (IHTMLDocument2*)GetHtmlDocument();
	if(pDoc!=NULL)
	{
		hr = pDoc->get_all(&pAllElem);
		if(SUCCEEDED(hr) && pAllElem!=NULL)
		{
			_variant_t varID(ElemID);
			_variant_t varIdx(0);
			hr = pAllElem->item(varID, varIdx, (LPDISPATCH*)&pElem);
			if(SUCCEEDED(hr) && pElem != NULL)
			{
				pElem->QueryInterface(&pControl);
				if(pControl!=NULL)
				{
					pControl->focus();
					pControl->Release();
				}
				pElem->Release();
			} // if(SUCCEEDED(hr) && pElem != NULL)
			pAllElem->Release();
		}//if(pAllElem!=NULL)
		pDoc->Release();
	}//if(pDoc!=NULL)
}

IHTMLStyle* CMLHtmlView::GetElementStyle(CString ElemID)
{
	IHTMLDocument2* pDoc = NULL;
	IHTMLElementCollection* pElemColl = NULL;
	IDispatch* pElemDisp = NULL;
	IHTMLElement* pElem=NULL;
	IHTMLStyle* pStyle = NULL;

	BOOL Result=FALSE;

	pDoc = (IHTMLDocument2*)GetHtmlDocument();
	if(pDoc!=NULL)
	{
		pDoc->get_all(&pElemColl);
		if(pElemColl!=NULL)
		{
			_variant_t varID(ElemID);
			_variant_t varIdx(0);
			HRESULT hr = pElemColl->item(varID, varIdx, &pElemDisp);
			if(SUCCEEDED(hr))
			{
				if(pElemDisp!=NULL)
				{
					hr = pElemDisp->QueryInterface(IID_IHTMLElement,(void**)&pElem);
					if(SUCCEEDED(hr))
					{
						hr = pElem->get_style(&pStyle);
						if(SUCCEEDED(hr))
							Result=TRUE;
						pElem->Release();
					}
					pElemDisp->Release();
				}
			}
		}//if(pAllElem!=NULL)
		pDoc->Release();
	}//if(pDoc!=NULL)

	return pStyle;
}

BOOL CMLHtmlView::GetElementValue(CString ElemID,CString& Value)
{
	Value.Empty();

	IHTMLDocument2* pDoc = NULL;
	IHTMLElementCollection* pAllElem = NULL;
	IDispatch* pElemDisp = NULL;
	IHTMLElement* pElem = NULL;

	IHTMLInputElement* pInput=NULL;
	IHTMLSelectElement* pSelect=NULL;
	IHTMLTextAreaElement* pTextArea=NULL;


	BOOL Result = FALSE;
	HRESULT hr = S_FALSE;

	pDoc = (IHTMLDocument2*)GetHtmlDocument();
	if(pDoc != NULL)
	{
		hr = pDoc->get_all(&pAllElem);
		if(SUCCEEDED(hr) && pAllElem != NULL)
		{
			_variant_t varID(ElemID);
			_variant_t varIdx(0);
			hr = pAllElem->item(varID, varIdx, &pElemDisp);
			if(SUCCEEDED(hr) && pElemDisp != NULL)
			{
				hr = pElemDisp->QueryInterface(IID_IHTMLElement,(void**)&pElem);
				if(SUCCEEDED(hr) && pElem != NULL)
				{
					BSTR bstrTagName;
					hr = pElem->get_tagName(&bstrTagName);
					if(SUCCEEDED(hr))
					{
						CString strTagName(bstrTagName);
						SysFreeString(bstrTagName);
						strTagName.MakeUpper();
						if(strTagName == "INPUT")
						{
							hr = pElem->QueryInterface(&pInput);
							if(SUCCEEDED(hr) && pInput!=NULL)
							{
								BSTR bstrType;
								hr = pInput->get_type(&bstrType);
								if(SUCCEEDED(hr))
								{
									CString strType(bstrType);
									strType.MakeUpper();
									SysFreeString(bstrType);
									if(strType == "TEXT" || strType == "HIDDEN")
									{
										BSTR bstrValue;
										hr = pInput->get_value(&bstrValue);
										if(SUCCEEDED(hr))
										{
											CString strValue(bstrValue);
											SysFreeString(bstrValue);
											Value = strValue;
											Result = TRUE;
										}
									} // if(strType == "TEXT" || strType == "HIDDEN")
									else if(strType=="CHECKBOX" || strType == "RADIO")
									{
										VARIANT_BOOL Check;
										hr = pInput->get_checked(&Check);
										if(SUCCEEDED(hr))
										{
											if(Check == VARIANT_TRUE)
												Value = "1";
											else
												Value = "0";
											Result = TRUE;
										}
									} // else if(strType=="CHECKBOX" || strType == "RADIO")
								} // if(SUCCEEDED(hr))
								pInput->Release();
							} // if(SUCCEEDED(hr) && pInput!=NULL)
						} // if(strTagName == "INPUT")
						else if(strTagName == "SELECT")
						{
							hr = pElem->QueryInterface(&pSelect);
							if(SUCCEEDED(hr) && pSelect!=NULL)
							{
								long index;
								hr = pSelect->get_selectedIndex(&index);
								if(SUCCEEDED(hr))
								{
									Value.Format(_T("%d"),index);
									Result = TRUE;
								}
								pSelect->Release();
							} // if(SUCCEEDED(hr) && pSelect!=NULL)
						} // else if(strTagName == "SELECT")
						else if(strTagName == "TEXTAREA")
						{
							hr = pElem->QueryInterface(&pTextArea);
							if(SUCCEEDED(hr) && pTextArea != NULL)
							{
								BSTR bstrValue;
								hr = pTextArea->get_value(&bstrValue);
								if(SUCCEEDED(hr))
								{
									CString strValue(bstrValue);
									SysFreeString(bstrValue);
									Value = strValue;
									Result = TRUE;
								}
								pTextArea->Release();
							}
						} // else if(strTagName == "TEXTAREA")
					}
					pElem->Release();
				}
			} // if(SUCCEEDED(hr) && pElem != NULL)
			pAllElem->Release();
		} // if(SUCCEEDED(hr) && pAllElem != NULL)
		pDoc->Release();
	} // if(pDoc != NULL)

	return Result;
}

BOOL CMLHtmlView::PutElementValue(CString ElemID,CString Value)
{
	IHTMLDocument2* pDoc = NULL;
	IHTMLElementCollection* pAllElem = NULL;
	IDispatch* pElemDisp = NULL;
	IHTMLElement* pElem = NULL;

	IHTMLInputElement* pInput=NULL;
	IHTMLSelectElement* pSelect=NULL;
	IHTMLTextAreaElement* pTextArea=NULL;


	BOOL Result = FALSE;
	HRESULT hr = S_FALSE;

	pDoc = (IHTMLDocument2*)GetHtmlDocument();
	if(pDoc != NULL)
	{
		hr = pDoc->get_all(&pAllElem);
		if(SUCCEEDED(hr) && pAllElem != NULL)
		{
			_variant_t varID(ElemID);
			_variant_t varIdx(0);
			hr = pAllElem->item(varID, varIdx, &pElemDisp);
			if(SUCCEEDED(hr) && pElemDisp != NULL)
			{
				hr = pElemDisp->QueryInterface(IID_IHTMLElement,(void**)&pElem);
				if(SUCCEEDED(hr) && pElem != NULL)
				{
					BSTR bstrTagName;
					hr = pElem->get_tagName(&bstrTagName);
					if(SUCCEEDED(hr))
					{
						CString strTagName(bstrTagName);
						SysFreeString(bstrTagName);
						strTagName.MakeUpper();
						if(strTagName == "INPUT")
						{
							hr = pElem->QueryInterface(&pInput);
							if(SUCCEEDED(hr) && pInput!=NULL)
							{
								BSTR bstrType;
								hr = pInput->get_type(&bstrType);
								if(SUCCEEDED(hr))
								{
									CString strType(bstrType);
									strType.MakeUpper();
									SysFreeString(bstrType);
									if(strType == "TEXT" || strType == "HIDDEN")
									{
										BSTR bstrValue;
										bstrValue = Value.AllocSysString();
										hr = pInput->put_value(bstrValue);
										SysFreeString(bstrValue);
										if(SUCCEEDED(hr))
											Result = TRUE;
									} // if(strType == "TEXT" || strType == "HIDDEN")
									else if(strType=="CHECKBOX" || strType == "RADIO")
									{
										VARIANT_BOOL Check;
										if(Value == "1")
											Check = VARIANT_TRUE;
										else
											Check = VARIANT_FALSE;

										hr = pInput->put_checked(Check);
										if(SUCCEEDED(hr))
											Result = TRUE;
									} // else if(strType=="CHECKBOX" || strType == "RADIO")
								} // if(SUCCEEDED(hr))
								pInput->Release();
							} // if(SUCCEEDED(hr) && pInput!=NULL)
						} // if(strTagName == "INPUT")
						else if(strTagName == "SELECT")
						{
							hr = pElem->QueryInterface(&pSelect);
							if(SUCCEEDED(hr) && pSelect!=NULL)
							{
								long index;
								index = _wtoi((LPCTSTR)Value);
								hr = pSelect->put_selectedIndex(index);
								if(SUCCEEDED(hr))
									Result = TRUE;
								pSelect->Release();
							} // if(SUCCEEDED(hr) && pSelect!=NULL)
						} // else if(strTagName == "SELECT")
						else if(strTagName == "TEXTAREA")
						{
							hr = pElem->QueryInterface(&pTextArea);
							if(SUCCEEDED(hr) && pTextArea != NULL)
							{
								BSTR bstrValue;
								bstrValue = Value.AllocSysString();
								hr = pTextArea->put_value(bstrValue);
								SysFreeString(bstrValue);
								if(SUCCEEDED(hr))
									Result = TRUE;
								pTextArea->Release();
							}
						} // else if(strTagName == "TEXTAREA")
					}
					pElem->Release();
				}
			} // if(SUCCEEDED(hr) && pElem != NULL)
			pAllElem->Release();
		} // if(SUCCEEDED(hr) && pAllElem != NULL)
		pDoc->Release();
	} // if(pDoc != NULL)

	return Result;
}

// 修改网页元素的内容
BOOL CMLHtmlView::PutElementHtml(CString ElemID,CString Html)
{
	IHTMLDocument2* pDoc = NULL;
	IHTMLElementCollection* pElemColl = NULL;
	IDispatch* pElemDisp = NULL;
	IHTMLElement* pElem=NULL;

	BOOL Result=FALSE;

	pDoc = (IHTMLDocument2*)GetHtmlDocument();
	if(pDoc!=NULL)
	{
		pDoc->get_all(&pElemColl);
		if(pElemColl!=NULL)
		{
			_variant_t varID(ElemID);
			_variant_t varIdx(0);
			HRESULT hr = pElemColl->item(varID, varIdx, &pElemDisp);
			if(SUCCEEDED(hr))
			{
				if(pElemDisp!=NULL)
				{
					hr = pElemDisp->QueryInterface(IID_IHTMLElement,(void**)&pElem);
					if(SUCCEEDED(hr))
					{
						BSTR bstrHtml=Html.AllocSysString();
						pElem->put_innerHTML(bstrHtml);
						pElem->Release();
						::SysFreeString(bstrHtml);
						Result=TRUE;
					}
					pElemDisp->Release();
				}
			}
		}//if(pAllElem!=NULL)
		pDoc->Release();
	}//if(pDoc!=NULL)
	return Result;
}

BOOL CMLHtmlView::PreCreateWindow(CREATESTRUCT& cs)
{
	// 消除窗口闪烁
	cs.style |= WS_CLIPSIBLINGS;
	if(m_IsView)
		cs.dwExStyle |= WS_EX_CLIENTEDGE;

	return CWnd::PreCreateWindow(cs);
}

BOOL CMLHtmlView::CreateFromStatic(UINT nID, CWnd* pParent)
{
    m_pParent     = pParent;
	m_ViewDesigner.SetRespondHWND(pParent->m_hWnd);
    CWnd* pStatic = pParent->GetDlgItem(nID);
    if(pStatic == NULL )
        return FALSE;

    CRect rc;
    pStatic->GetWindowRect(&rc);
    pParent->ScreenToClient(&rc);
    pStatic->DestroyWindow();

    if(!CHtmlView::Create(NULL, NULL, (WS_CHILD | WS_VISIBLE ), rc, pParent, nID, NULL))
        return FALSE;
    m_IsView = FALSE;

    //去掉边框3D风格
    ModifyStyleEx(WS_EX_CLIENTEDGE, 0);

    OnInitialUpdate();

    return TRUE;
}

void CMLHtmlView::SetUrl(std::wstring& url, bool bNeedOnClick, bool bNeedChangeMessageBox,bool bNeedMouseMove)
{
	m_strURL = url;
	m_needOnClickEvent = bNeedOnClick;
	m_needChangeMessageBox = bNeedChangeMessageBox;
	m_needMouseMove = bNeedMouseMove;

	m_ViewDesigner.SetNeedEvent(bNeedOnClick,bNeedMouseMove);
}

void CMLHtmlView::UpdateURL()
{
     Navigate2(m_strURL.c_str(),NULL,NULL);    
}

BOOL CMLHtmlView::Create(CRect rect, CWnd* pParent, UINT nID)
{
	if(!CHtmlView::Create(NULL, NULL, (WS_CHILD | WS_VISIBLE ), rect, pParent, nID, NULL))
		return FALSE;

	m_IsView = FALSE;

    //去掉边框3D风格
    ModifyStyleEx(WS_EX_CLIENTEDGE, 0);

    OnInitialUpdate();
   
	return TRUE;
}

int CMLHtmlView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CHtmlView::OnCreate(lpCreateStruct) == -1)
		return -1;

	//注册拖放功能
	//m_DropTarget.Register(this);
	return 0;
}

void CMLHtmlView::OnDestroy()
{
	if(m_IsView)
		CHtmlView::OnDestroy();
	else
	{
		if (m_pBrowserApp)
		{
			m_pBrowserApp.Release();
			m_pBrowserApp = NULL;
		}
		CWnd::OnDestroy();
	}
}

int CMLHtmlView::OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message)
{
	if(m_IsView)
		return CHtmlView::OnMouseActivate(pDesktopWnd, nHitTest, message);
	else
		return CWnd::OnMouseActivate(pDesktopWnd, nHitTest, message);
}

void CMLHtmlView::PostNcDestroy()
{
	if(m_IsView)
		CHtmlView::PostNcDestroy();
}

void CMLHtmlView::OnSize(UINT nType, int cx, int cy)
{
	CFormView::OnSize(nType, cx, cy);

	if (::IsWindow(m_wndBrowser.m_hWnd)) 
	{ 
		CRect rect; 
		GetClientRect(rect); 
		::AdjustWindowRectEx(rect, GetStyle(), FALSE, WS_EX_CLIENTEDGE); 
		m_wndBrowser.SetWindowPos(NULL, rect.left, rect.top, rect.Width(), rect.Height(), SWP_NOACTIVATE | SWP_NOZORDER); 
	} 
}

const CString CMLHtmlView::GetSystemErrorMessage(DWORD dwError)
{
	CString strError;
	LPTSTR lpBuffer;

	if(!FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		NULL,  dwError,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_SYS_DEFAULT),
		(LPTSTR) &lpBuffer, 0, NULL))
	{
		strError = "FormatMessage Native Error" ;
	}
	else
	{
		strError = lpBuffer;
		LocalFree(lpBuffer);
	}
	return strError;
}

CString CMLHtmlView::GetNextToken(CString& strSrc, const CString strDelim,BOOL bTrim, BOOL bFindOneOf)
{
	CString strToken;
	int idx = bFindOneOf? strSrc.FindOneOf(strDelim) : strSrc.Find(strDelim);
	if(idx != -1)
	{
		strToken  = strSrc.Left(idx);
		strSrc = strSrc.Right(strSrc.GetLength() - (idx + 1) );
	}
	else
	{
		strToken = strSrc;
		strSrc.Empty();
	}
	if(bTrim)
	{
		strToken.TrimLeft();
		strToken.TrimRight();
	}
	return strToken;
}

BOOL CMLHtmlView::IsJScriptOK()
{
	return (m_spDoc != NULL); 
}

BOOL CMLHtmlView::GetJScript(CComPtr<IDispatch>& spDisp)
{
	if( m_spDoc==NULL )
		return FALSE;

	HRESULT hr = m_spDoc->get_Script(&spDisp);
	ATLASSERT(SUCCEEDED(hr));
	return SUCCEEDED(hr);
}

BOOL CMLHtmlView::GetJScripts(CComPtr<IHTMLElementCollection>& spColl)
{
	if( m_spDoc==NULL )
		return FALSE;

	HRESULT hr = m_spDoc->get_scripts(&spColl);
	ATLASSERT(SUCCEEDED(hr));
	return SUCCEEDED(hr);
}

BOOL CMLHtmlView::CallJScript(const CString strFunc,_variant_t* pVarResult)
{
	CStringArray paramArray;
	return CallJScript(strFunc,paramArray,pVarResult);
}

BOOL CMLHtmlView::CallJScript(const CString strFunc,const CString strArg1,_variant_t* pVarResult)
{
	CStringArray paramArray;
	paramArray.Add(strArg1);
	return CallJScript(strFunc,paramArray,pVarResult);
}

BOOL CMLHtmlView::CallJScript(const CString strFunc,const CString strArg1,const CString strArg2,_variant_t* pVarResult)
{
    CStringArray paramArray;
    paramArray.Add(strArg1);
    paramArray.Add(strArg2);
    return CallJScript(strFunc,paramArray,pVarResult);
}

BOOL CMLHtmlView::CallJScript(const CString strFunc,const CString strArg1,const CString strArg2,const CString strArg3,_variant_t* pVarResult)
{
	CStringArray paramArray;
	paramArray.Add(strArg1);
	paramArray.Add(strArg2);
	paramArray.Add(strArg3);
	return CallJScript(strFunc,paramArray,pVarResult);
}

BOOL CMLHtmlView::CallJScript(const CString strFunc, const CStringArray& paramArray,_variant_t* pVarResult)
{
	CComPtr<IDispatch> spScript;
	if(!GetJScript(spScript))
	{
		OutputDebugString(_T("函数GetJScrip调用失败！"));
		return FALSE;
	}
	CComBSTR bstrFunc(strFunc);
	DISPID dispid = NULL;
	HRESULT hr = spScript->GetIDsOfNames(IID_NULL,&bstrFunc,1,
		LOCALE_SYSTEM_DEFAULT,&dispid);
	if(FAILED(hr))
	{
		OutputDebugString(GetSystemErrorMessage(hr));
		return FALSE;
	}

	INT_PTR arraySize = paramArray.GetSize();

	DISPPARAMS dispparams;
	memset(&dispparams, 0, sizeof dispparams);
	dispparams.cArgs = (UINT)arraySize;
	dispparams.rgvarg = new VARIANT[dispparams.cArgs];

	for( int i = 0; i < arraySize; i++)
	{
		CComBSTR bstr = paramArray.GetAt(arraySize - 1 - i); // back reading
		bstr.CopyTo(&dispparams.rgvarg[i].bstrVal);
		dispparams.rgvarg[i].vt = VT_BSTR;
	}
	dispparams.cNamedArgs = 0;

	EXCEPINFO excepInfo;
	memset(&excepInfo, 0, sizeof excepInfo);
	_variant_t vaResult;
	UINT nArgErr = (UINT)-1;  // initialize to invalid arg

	hr = spScript->Invoke(dispid,IID_NULL,0,
		DISPATCH_METHOD,&dispparams,&vaResult,&excepInfo,&nArgErr);

	for( int j = 0; j < arraySize; j++)
		::SysFreeString(dispparams.rgvarg[j].bstrVal);
	delete [] dispparams.rgvarg;
	if(FAILED(hr))
	{
		//MessageBox(GetSystemErrorMessage(hr));
		return FALSE;
	}

	if(pVarResult)
	{
		*pVarResult = vaResult;
	}
	return TRUE;
}

CString CMLHtmlView::ScanJScript(CString& strAText, CStringArray& args)
{
	args.RemoveAll();
	CString strDelim(" \n\r\t"),strSrc(strAText);
	BOOL bFound = FALSE;
	while(!strSrc.IsEmpty())
	{
		CString strStart = GetNextToken(strSrc,strDelim);
		if(strStart == "function")
		{
			bFound = TRUE;
			break;
		}
		if(strStart == "/*")
		{
			// Skip comments
			while(!strSrc.IsEmpty())
			{
				CString strStop = GetNextToken(strSrc,strDelim);
				if(strStop == "*/")
				{
					break;
				}
			}
		}
	}

	if(!bFound)
		return _T("");

	CString strFunc = GetNextToken(strSrc,_T("("),TRUE);
	CString strArgs = GetNextToken(strSrc,_T(")"),TRUE);

	// Parse arguments
	CString strArg;
	while(!(strArg = GetNextToken(strArgs,_T(","))).IsEmpty())
		args.Add(strArg);

	strAText= strSrc;
	return strFunc;
}

BOOL CMLHtmlView::SetScriptDocument()
{
	LPDISPATCH pDisp = GetHtmlDocument();
	if(pDisp==NULL)
	{
		ASSERT(FALSE);
		return FALSE;
	}

	m_spDoc = NULL;

	CComPtr<IDispatch> spDisp;
    spDisp.Attach(pDisp);

	HRESULT hr = spDisp->QueryInterface(IID_IHTMLDocument2,(void**)&m_spDoc);
	if(FAILED(hr))
	{
		MessageBox(_T("Failed to get HTML document COM object"));
		return FALSE;
	}
	return TRUE;
}

void CMLHtmlView::DocumentComplete(LPDISPATCH pDisp, VARIANT* URL)
{
    if(m_lpDisp && (m_lpDisp == pDisp))//判断网页是否真正加载完毕,用于多Frame网页
    {
        SetScriptDocument();

        GetParent()->PostMessage(WM_MISSION_COMPLETED,NULL,NULL);  //completed
        m_lpDisp=NULL;//Reset glpDisp		
		
    }
}

void CMLHtmlView::NavigateComplete2(LPDISPATCH pDisp, VARIANT* URL)
{
    //if(!m_lpDisp) by plushu 2012-3-20
    {
        m_lpDisp = pDisp;

		if(m_needOnClickEvent || m_needMouseMove)
		{
			IHTMLDocument2 *pDoc2 = (IHTMLDocument2*)GetHtmlDocument();
			if(pDoc2 != NULL)
			{
				m_ViewDesigner.Attach(pDoc2);
			}
		}

		::SendMessage(m_pParent->GetSafeHwnd(),WM_MISSION_NAVIGATECOMPLETE2, 0, 0);

    }
}

void CMLHtmlView::OnNavigateError(LPCTSTR lpszURL, LPCTSTR lpszFrame, DWORD dwError, BOOL *pbCancel)
{
    if ((dwError>=400 && dwError <= 505) || dwError == INET_E_RESOURCE_NOT_FOUND)
    {
        ::SendMessage(GetParent()->GetSafeHwnd(), WM_LOADJSREADY, 0, false);
    }
	else
	{
		::SendMessage(GetParent()->GetSafeHwnd(), WM_MISSION_HTML_ERROR, 0, false);
	}
}

HRESULT CMLHtmlView::GetElement(LPCTSTR szElementId, IDispatch **ppdisp, 
	BOOL *pbCollection /*= NULL*/)
{
	CComPtr<IHTMLElementCollection> sphtmlAll;
	CComPtr<IHTMLElementCollection> sphtmlColl;
	CComPtr<IDispatch> spdispElem;
	CComVariant varName;
	CComVariant varIndex;
	HRESULT hr = S_OK;
	CComPtr<IHTMLDocument2> sphtmlDoc;
	USES_CONVERSION;

	*ppdisp = NULL;

	if (pbCollection)
		*pbCollection = FALSE;

	sphtmlDoc = (IHTMLDocument2*)GetHtmlDocument();
	if (sphtmlDoc == NULL)
		return hr;

	varName.vt = VT_BSTR;
	varName.bstrVal = T2BSTR(szElementId);
	if (!varName.bstrVal)
	{
		hr = E_OUTOFMEMORY;
		goto __Error;
	}

	hr = sphtmlDoc->get_all(&sphtmlAll);
	if (sphtmlAll == NULL)
		goto __Error;
	hr = sphtmlAll->item(varName, varIndex, &spdispElem);
	if (spdispElem == NULL)
	{
		hr = E_NOINTERFACE;
		goto __Error;
	}

	spdispElem->QueryInterface(__uuidof(IHTMLElementCollection), (void **) &sphtmlColl);
	if (sphtmlColl)
	{
		if (pbCollection)
			*pbCollection = TRUE;
#ifdef _DEBUG
		else
		{
			TRACE(traceHtml, 0, "Warning: duplicate IDs or NAMEs.\n");
			ATLASSERT(FALSE);
		}
#endif

	}
__Error:
	if (SUCCEEDED(hr))
	{
		*ppdisp = spdispElem;
		if (spdispElem)
			(*ppdisp)->AddRef();
	}
	return hr;
}

HRESULT CMLHtmlView::GetElement(LPCTSTR szElementId, IHTMLElement **pphtmlElement)
{
	return GetElementInterface(szElementId, __uuidof(IHTMLElement), (void **) pphtmlElement);
}

HRESULT CMLHtmlView::GetElementInterface(LPCTSTR szElementId, REFIID riid, void **ppvObj)
{
	HRESULT hr = E_NOINTERFACE;
	*ppvObj = NULL;
	CComPtr<IDispatch> spdispElem;

	hr = GetElement(szElementId, &spdispElem);

	if (spdispElem)
		hr = spdispElem->QueryInterface(riid, ppvObj);
	return hr;
}

CString CMLHtmlView::GetValue(CComPtr<IHTMLElement> & pEvtObj,CString key) 
{
	if(pEvtObj)
	{
		CComVariant valuetemp;
		CComBSTR valuestr;
		HRESULT HR = pEvtObj->getAttribute(CComBSTR(key),0,&valuetemp) ;
		if ( HR == S_OK && valuetemp.vt != VT_NULL )
		{
			valuetemp.CopyTo(&valuestr);
			valuetemp.Clear() ;
			CString  re(valuestr);
			return re;
		}
		valuetemp.Clear() ;
	}

	return L"";
}

void CMLHtmlView::GetScrollSize(int &iWidth, int &iHeight)
{
	long lBodyWidth = 0;
	long lBodyHeight = 0;

	if(m_spDoc != NULL)
	{
		CComPtr<IHTMLElement> pBody;
		m_spDoc->get_body(&pBody);
		ASSERT(pBody != NULL);
		if (pBody == NULL)
			return ;

		CComQIPtr<IHTMLElement2> pElement = pBody;
		ASSERT(pElement != NULL);
		if (pElement == NULL)
			return ;

		//pElement->get_scrollWidth(&lBodyWidth);		
		iWidth = lBodyWidth;

		pElement->get_scrollHeight(&lBodyHeight);
		iHeight = lBodyHeight;		

	}
	
}