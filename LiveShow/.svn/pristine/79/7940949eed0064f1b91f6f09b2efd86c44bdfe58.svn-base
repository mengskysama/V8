#include "stdafx.h"
#include "HtmlControlSite.h"


#ifdef _DEBUG
#define SWITCH_STATE
#endif


BEGIN_INTERFACE_MAP(HtmlControlSite, COleControlSite)
	INTERFACE_PART(HtmlControlSite, IID_IDocHostUIHandler, DocHostUIHandler)
END_INTERFACE_MAP()

HtmlControlSite::HtmlControlSite(COleControlContainer* pContainer)
: COleControlSite(pContainer)
{
	AFX_MODULE_STATE_m=NULL;
}

HtmlControlSite::~HtmlControlSite()
{
	ZeroMemory((void*)&AFX_MODULE_THREAD_STATE_m,sizeof(AFX_MODULE_THREAD_STATE));
	ZeroMemory((void*)&backup_mts,sizeof(AFX_MODULE_THREAD_STATE));
}

inline CHtmlView* HtmlControlSite::GetView() const
{
	return STATIC_DOWNCAST(CHtmlView, m_pCtrlCont->m_pWnd);
}


STDMETHODIMP HtmlControlSite::XDocHostUIHandler::GetExternal(LPDISPATCH *lppDispatch)
{
	METHOD_PROLOGUE_EX_(HtmlControlSite, DocHostUIHandler)
#ifdef SWITCH_STATE
	BeginStateSwitch(pThis);
#endif
	
	CHtmlView* pView = pThis->GetView();
	ASSERT_VALID(pView);
	HRESULT re;
	if (pView == NULL)
		re=E_FAIL;
	else
		re= pView->OnGetExternal(lppDispatch);
	
#ifdef SWITCH_STATE
	EndStateSwitch(pThis);
#endif
	return re;
}


STDMETHODIMP HtmlControlSite::XDocHostUIHandler::ShowContextMenu(
	DWORD dwID, LPPOINT ppt, LPUNKNOWN pcmdtReserved, LPDISPATCH pdispReserved)
{
	METHOD_PROLOGUE_EX_(HtmlControlSite, DocHostUIHandler)
#ifdef SWITCH_STATE
	BeginStateSwitch(pThis);
#endif
	CHtmlView* pView = pThis->GetView();
	ASSERT_VALID(pView);
	HRESULT re;
		if (pView == NULL)
			re=E_FAIL;
		else
			re= pView->OnShowContextMenu(dwID, ppt, pcmdtReserved, pdispReserved);

#ifdef SWITCH_STATE
	EndStateSwitch(pThis);
#endif
	return re;
}


STDMETHODIMP HtmlControlSite::XDocHostUIHandler::GetHostInfo(
	DOCHOSTUIINFO *pInfo)
{
	METHOD_PROLOGUE_EX_(HtmlControlSite, DocHostUIHandler)
#ifdef SWITCH_STATE
	BeginStateSwitch(pThis);
#endif
	CHtmlView* pView = pThis->GetView();
	ASSERT_VALID(pView);
	HRESULT re;
		if (pView == NULL)
			re=E_FAIL;
		else
			re= pView->OnGetHostInfo(pInfo);

#ifdef SWITCH_STATE
	EndStateSwitch(pThis);
#endif
	return re;
}


STDMETHODIMP HtmlControlSite::XDocHostUIHandler::ShowUI(
	DWORD dwID, LPOLEINPLACEACTIVEOBJECT pActiveObject,
	LPOLECOMMANDTARGET pCommandTarget, LPOLEINPLACEFRAME pFrame,
	LPOLEINPLACEUIWINDOW pDoc)
{
	METHOD_PROLOGUE_EX_(HtmlControlSite, DocHostUIHandler)
#ifdef SWITCH_STATE
	BeginStateSwitch(pThis);
#endif
	CHtmlView* pView = pThis->GetView();
	ASSERT_VALID(pView);
	HRESULT re;
		if (pView == NULL)
			re=E_FAIL;
		else
			 re= pView->OnShowUI(dwID, pActiveObject, pCommandTarget, pFrame, pDoc);

#ifdef SWITCH_STATE
	EndStateSwitch(pThis);
#endif
	return re;
}


STDMETHODIMP HtmlControlSite::XDocHostUIHandler::HideUI(void)
{
	METHOD_PROLOGUE_EX_(HtmlControlSite, DocHostUIHandler)
#ifdef SWITCH_STATE
	BeginStateSwitch(pThis);
#endif
		CHtmlView* pView = pThis->GetView();
	ASSERT_VALID(pView);
	HRESULT re;
		if (pView == NULL)
			re=E_FAIL;
		else
			re= pView->OnHideUI();

#ifdef SWITCH_STATE
	EndStateSwitch(pThis);
#endif
	return re;
}


STDMETHODIMP HtmlControlSite::XDocHostUIHandler::UpdateUI(void)
{
	METHOD_PROLOGUE_EX_(HtmlControlSite, DocHostUIHandler)
#ifdef SWITCH_STATE
	BeginStateSwitch(pThis);
#endif
		

	CHtmlView* pView = pThis->GetView();
	ASSERT_VALID(pView);
	HRESULT re;
		if (pView == NULL)
			re=E_FAIL;
		else
			re=pView->OnUpdateUI();

#ifdef SWITCH_STATE
	EndStateSwitch(pThis);
#endif
	return re;
}


STDMETHODIMP HtmlControlSite::XDocHostUIHandler::EnableModeless(BOOL fEnable)
{
	METHOD_PROLOGUE_EX_(HtmlControlSite, DocHostUIHandler)
#ifdef SWITCH_STATE
	BeginStateSwitch(pThis);
#endif
	CHtmlView* pView = pThis->GetView();
	ASSERT_VALID(pView);
	HRESULT re;
		if (pView == NULL)
			re=E_FAIL;
		else
			re=pView->OnEnableModeless(fEnable);

#ifdef SWITCH_STATE
	EndStateSwitch(pThis);
#endif
	return re;
}


STDMETHODIMP HtmlControlSite::XDocHostUIHandler::OnDocWindowActivate(BOOL fActivate)
{
	METHOD_PROLOGUE_EX_(HtmlControlSite, DocHostUIHandler)
#ifdef SWITCH_STATE
	BeginStateSwitch(pThis);
#endif
	CHtmlView* pView = pThis->GetView();
	ASSERT_VALID(pView);
	HRESULT re;
		if (pView == NULL)
			re=E_FAIL;
		else
			re=pView->OnDocWindowActivate(fActivate);

#ifdef SWITCH_STATE
	EndStateSwitch(pThis);
#endif
	return re;
}


STDMETHODIMP HtmlControlSite::XDocHostUIHandler::OnFrameWindowActivate(
	BOOL fActivate)
{
	METHOD_PROLOGUE_EX_(HtmlControlSite, DocHostUIHandler)
#ifdef SWITCH_STATE
	BeginStateSwitch(pThis);
#endif
	CHtmlView* pView = pThis->GetView();
	ASSERT_VALID(pView);
	HRESULT re;
		if (pView == NULL)
			re=E_FAIL;
		else
			re=pView->OnFrameWindowActivate(fActivate);

#ifdef SWITCH_STATE
	EndStateSwitch(pThis);
#endif
	return re;
}


STDMETHODIMP HtmlControlSite::XDocHostUIHandler::ResizeBorder(
	LPCRECT prcBorder, LPOLEINPLACEUIWINDOW pUIWindow, BOOL fFrameWindow)
{
	METHOD_PROLOGUE_EX_(HtmlControlSite, DocHostUIHandler)
#ifdef SWITCH_STATE
	BeginStateSwitch(pThis);
#endif
	CHtmlView* pView = pThis->GetView();
	ASSERT_VALID(pView);
	HRESULT re;
		if (pView == NULL)
			re=E_FAIL;
		else
			re=pView->OnResizeBorder(prcBorder, pUIWindow, fFrameWindow);

#ifdef SWITCH_STATE
	EndStateSwitch(pThis);
#endif
	return re;
}

STDMETHODIMP HtmlControlSite::XDocHostUIHandler::TranslateAccelerator(
	LPMSG lpMsg, const GUID* pguidCmdGroup, DWORD nCmdID)
{
	METHOD_PROLOGUE_EX_(HtmlControlSite, DocHostUIHandler)
#ifdef SWITCH_STATE
	BeginStateSwitch(pThis);
#endif
	CHtmlView* pView = pThis->GetView();
	ASSERT_VALID(pView);
	HRESULT re;
		if (pView == NULL)
			re=E_FAIL;
		else
			re=pView->OnTranslateAccelerator(lpMsg, pguidCmdGroup, nCmdID);

#ifdef SWITCH_STATE
	EndStateSwitch(pThis);
#endif
	return re;
}


STDMETHODIMP HtmlControlSite::XDocHostUIHandler::GetOptionKeyPath(
	LPOLESTR* pchKey, DWORD dwReserved)
{
	METHOD_PROLOGUE_EX_(HtmlControlSite, DocHostUIHandler)
#ifdef SWITCH_STATE
	BeginStateSwitch(pThis);
#endif
	CHtmlView* pView = pThis->GetView();
	ASSERT_VALID(pView);
	HRESULT re;
		if (pView == NULL)
			re=E_FAIL;
		else
			re=pView->OnGetOptionKeyPath(pchKey, dwReserved);

#ifdef SWITCH_STATE
	EndStateSwitch(pThis);
#endif
	return re;
}


STDMETHODIMP HtmlControlSite::XDocHostUIHandler::GetDropTarget(
	LPDROPTARGET pDropTarget, LPDROPTARGET* ppDropTarget)
{
	METHOD_PROLOGUE_EX_(HtmlControlSite, DocHostUIHandler)
#ifdef SWITCH_STATE
	BeginStateSwitch(pThis);
#endif
	CHtmlView* pView = pThis->GetView();
	ASSERT_VALID(pView);
	HRESULT re;
		if (pView == NULL)
			re=E_FAIL;
		else
			re=pView->OnGetDropTarget(pDropTarget, ppDropTarget);

#ifdef SWITCH_STATE
	EndStateSwitch(pThis);
#endif
	return re;
}


STDMETHODIMP HtmlControlSite::XDocHostUIHandler::TranslateUrl(
	DWORD dwTranslate, OLECHAR* pchURLIn, OLECHAR** ppchURLOut)
{
	METHOD_PROLOGUE_EX_(HtmlControlSite, DocHostUIHandler)
#ifdef SWITCH_STATE
	BeginStateSwitch(pThis);
#endif
	CHtmlView* pView = pThis->GetView();
	ASSERT_VALID(pView);
	HRESULT re;
		if (pView == NULL)
			re=E_FAIL;
		else
			re=pView->OnTranslateUrl(dwTranslate, pchURLIn, ppchURLOut);

#ifdef SWITCH_STATE
	EndStateSwitch(pThis);
#endif
	return re;
}


STDMETHODIMP HtmlControlSite::XDocHostUIHandler::FilterDataObject(
	LPDATAOBJECT pDataObject, LPDATAOBJECT* ppDataObject)
{
	METHOD_PROLOGUE_EX_(HtmlControlSite, DocHostUIHandler)
#ifdef SWITCH_STATE
	BeginStateSwitch(pThis);
#endif
	CHtmlView* pView = pThis->GetView();
	ASSERT_VALID(pView);
	HRESULT re;
		if (pView == NULL)
			re=E_FAIL;
		else
			re=pView->OnFilterDataObject(pDataObject, ppDataObject);

#ifdef SWITCH_STATE
	EndStateSwitch(pThis);
#endif
	return re;
}


STDMETHODIMP_(ULONG) HtmlControlSite::XDocHostUIHandler::AddRef()
{
	METHOD_PROLOGUE_EX_(HtmlControlSite, DocHostUIHandler)
		return pThis->ExternalAddRef();
}

STDMETHODIMP_(ULONG) HtmlControlSite::XDocHostUIHandler::Release()
{
	METHOD_PROLOGUE_EX_(HtmlControlSite, DocHostUIHandler)
		return pThis->ExternalRelease();
}

STDMETHODIMP HtmlControlSite::XDocHostUIHandler::QueryInterface(
	REFIID iid, LPVOID far* ppvObj)     
{
	METHOD_PROLOGUE_EX_(HtmlControlSite, DocHostUIHandler)
		return pThis->ExternalQueryInterface(&iid, ppvObj);
}



void HtmlControlSite::XDocHostUIHandler::BeginStateSwitch(HtmlControlSite *pThis)
{
	if(pThis->AFX_MODULE_STATE_m==NULL)
	{
		pThis->AFX_MODULE_STATE_m=AfxGetModuleState();
		pThis->mts=AfxGetModuleThreadState();
		memcpy(&pThis->AFX_MODULE_THREAD_STATE_m,pThis->mts,sizeof(AFX_MODULE_THREAD_STATE));
	}

	pThis->mts=AfxGetModuleThreadState();
	memcpy(&pThis->backup_mts,pThis->mts,sizeof(AFX_MODULE_THREAD_STATE));
	memcpy(pThis->mts,(AFX_MODULE_THREAD_STATE*)&pThis->AFX_MODULE_THREAD_STATE_m,sizeof(AFX_MODULE_THREAD_STATE));
}

void HtmlControlSite::XDocHostUIHandler::EndStateSwitch(HtmlControlSite *pThis)
{
	memcpy(pThis->mts,&pThis->backup_mts,sizeof(AFX_MODULE_THREAD_STATE));
}