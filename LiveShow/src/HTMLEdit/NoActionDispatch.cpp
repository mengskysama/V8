#include "stdafx.h"
#include "NoActionDispatch.h"


HRESULT STDMETHODCALLTYPE CNoActionDispatch::QueryInterface(REFIID riid, void __RPC_FAR *__RPC_FAR *ppvObject)
{
	HRESULT hrRet = S_OK;

	// Standard QI() initialization - set *ppv to NULL.
	*ppvObject = NULL;

	// If the client is requesting an interface we support, set *ppv.
	if (IsEqualIID(riid, IID_IUnknown))
		*ppvObject = (IUnknown *) this;
	else if (IsEqualIID(riid, IID_IDispatch))
		*ppvObject = (IDispatch *) this;
	else
		// We don't support the interface the client is asking for.
		hrRet = E_NOINTERFACE;

	// If we're returning an interface pointer, AddRef() it.
	if (S_OK == hrRet)
		((IUnknown *) *ppvObject)->AddRef();

	return hrRet;
}

ULONG STDMETHODCALLTYPE CNoActionDispatch::AddRef(void)
{
	return ++m_uRefCount;
}

ULONG STDMETHODCALLTYPE CNoActionDispatch::Release(void)
{
	return --m_uRefCount;
}

HRESULT STDMETHODCALLTYPE CNoActionDispatch::GetTypeInfoCount(UINT *pctinfo)
{
	*pctinfo = 0;
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CNoActionDispatch::GetTypeInfo(UINT /*iTInfo*/, LCID /*lcid*/, ITypeInfo ** /*ppTInfo*/)
{
	return DISP_E_BADINDEX;
}

HRESULT STDMETHODCALLTYPE CNoActionDispatch::GetIDsOfNames(REFIID /*riid*/, LPOLESTR * /*rgszNames*/, UINT /*cNames*/, LCID /*lcid*/, DISPID * /*rgDispId*/)
{
	return DISP_E_UNKNOWNNAME;
}

HRESULT STDMETHODCALLTYPE CNoActionDispatch::Invoke(DISPID dispIdMember, REFIID riid, LCID lcid, WORD wFlags, DISPPARAMS * pDispParams, VARIANT *pVarResult, EXCEPINFO * pExcepInfo, UINT * puArgErr)
{
	//	If we were installed it means we should disable
	//	dragging. So set the return value to false
	pVarResult->vt = VT_BOOL;
	pVarResult->boolVal = false;
	return S_FALSE;
}