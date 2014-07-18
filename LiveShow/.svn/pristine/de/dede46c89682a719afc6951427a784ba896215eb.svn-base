// LHP 2004-4-29

#include "stdafx.h"
#include "MyOleDropTarget.h"


IMPLEMENT_DYNAMIC(CMyOleDropTarget, COleDropTarget)

BEGIN_MESSAGE_MAP(CMyOleDropTarget, COleDropTarget)
END_MESSAGE_MAP()

BEGIN_INTERFACE_MAP(CMyOleDropTarget, COleDropTarget)
	INTERFACE_PART(CMyOleDropTarget, IID_IDropTarget, MyDropTarget)
END_INTERFACE_MAP()

CMyOleDropTarget::CMyOleDropTarget()
{
	m_pIEDropTarget = NULL;
}

CMyOleDropTarget::~CMyOleDropTarget()
{
	ReleaseIEDropTarget();
}

void CMyOleDropTarget::SetIEDropTarget(LPDROPTARGET pDropTarget)
{
	if(m_pIEDropTarget)
		ReleaseIEDropTarget();
	m_pIEDropTarget = pDropTarget;
	m_pIEDropTarget->AddRef();
}

void CMyOleDropTarget::ReleaseIEDropTarget()
{
	if(!m_pIEDropTarget)
		return;
	m_pIEDropTarget->Release();
	m_pIEDropTarget = NULL;
}

STDMETHODIMP_(ULONG) CMyOleDropTarget::XMyDropTarget::AddRef()
{
	METHOD_PROLOGUE_EX_(CMyOleDropTarget, MyDropTarget)
	return pThis->ExternalAddRef();
}

STDMETHODIMP_(ULONG) CMyOleDropTarget::XMyDropTarget::Release()
{
	METHOD_PROLOGUE_EX_(CMyOleDropTarget, MyDropTarget)
	return pThis->ExternalRelease();
}

STDMETHODIMP CMyOleDropTarget::XMyDropTarget::QueryInterface(
	REFIID iid, LPVOID* ppvObj)
{
	METHOD_PROLOGUE_EX_(CMyOleDropTarget, MyDropTarget)
	return pThis->ExternalQueryInterface(&iid, ppvObj);
}

STDMETHODIMP CMyOleDropTarget::XMyDropTarget::DragEnter(THIS_ LPDATAOBJECT lpDataObject,
	DWORD dwKeyState, POINTL pt, LPDWORD pdwEffect)
{
	METHOD_PROLOGUE_EX(CMyOleDropTarget, MyDropTarget)
	ASSERT_VALID(pThis);

//	if(pThis->m_pIEDropTarget)
//	{
//		//pThis->m_pIEDropTarget->DragEnter(lpDataObject, dwKeyState, pt, pdwEffect);
//	}
	return pThis->m_xDropTarget.DragEnter(lpDataObject, dwKeyState, pt, pdwEffect);
}

STDMETHODIMP CMyOleDropTarget::XMyDropTarget::DragOver(THIS_ DWORD dwKeyState,
	POINTL pt, LPDWORD pdwEffect)
{
	METHOD_PROLOGUE_EX(CMyOleDropTarget, MyDropTarget)
	ASSERT_VALID(pThis);

//	if(pThis->m_pIEDropTarget)
//	{
//		pThis->m_pIEDropTarget->DragOver(dwKeyState, pt, pdwEffect);
//	}
	return pThis->m_xDropTarget.DragOver(dwKeyState, pt, pdwEffect);
}

STDMETHODIMP CMyOleDropTarget::XMyDropTarget::DragLeave(THIS)
{
	METHOD_PROLOGUE_EX(CMyOleDropTarget, MyDropTarget)
	ASSERT_VALID(pThis);


//	if(pThis->m_pIEDropTarget)
//	{
//		pThis->m_pIEDropTarget->DragLeave();
//	}
	return pThis->m_xDropTarget.DragLeave();
}

STDMETHODIMP CMyOleDropTarget::XMyDropTarget::Drop(THIS_ LPDATAOBJECT lpDataObject,
	DWORD dwKeyState, POINTL pt, LPDWORD pdwEffect)
{
	METHOD_PROLOGUE_EX(CMyOleDropTarget, MyDropTarget)
	ASSERT_VALID(pThis);

//	if(pThis->m_pIEDropTarget)
//	{
//		pThis->m_pIEDropTarget->Drop(lpDataObject, dwKeyState, pt, pdwEffect);
//	}
	return pThis->m_xDropTarget.Drop(lpDataObject, dwKeyState, pt, pdwEffect);
}