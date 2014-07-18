/********************************************************************
	created:	
	created:	
	file base:	
	file ext:	h
	author:		Garret Lian
	
	purpose:	
*********************************************************************/
#ifndef __DIRECTUI_EnumUnknown_H__
#define __DIRECTUI_EnumUnknown_H__


//////////////////////////////////////////////////////////////////////////
class CActiveXEnum : public IEnumUnknown
{
public:
	CActiveXEnum(IUnknown* pUnk) :
	  m_pUnk(pUnk), m_dwRef(1), m_iPos(0) {
		  m_pUnk->AddRef();
	  }
	  
	  ~CActiveXEnum() {
		  m_pUnk->Release();
	  }

public:
	  STDMETHOD_(ULONG,AddRef)() {
		  return ++m_dwRef;
	  }
	  
	  STDMETHOD_(ULONG,Release)() {
		  LONG lRef = --m_dwRef;		  
		  if( lRef == 0 ) 
			  delete this;		  
		  return lRef;
	  }

	  STDMETHOD(QueryInterface)(REFIID riid, LPVOID *ppvObject) {
		  *ppvObject = NULL;		  
		  if( riid == IID_IUnknown ) 
			  *ppvObject = static_cast<IEnumUnknown*>(this);
		  else if( riid == IID_IEnumUnknown ) 
			  *ppvObject = static_cast<IEnumUnknown*>(this);		  
		  if( *ppvObject != NULL ) 
			  AddRef();
		  return *ppvObject == NULL ? E_NOINTERFACE : S_OK;
	  }
	  
	  STDMETHOD(Next)(ULONG celt, IUnknown **rgelt, ULONG *pceltFetched) {
		  if( pceltFetched != NULL )
			  *pceltFetched = 0;		  
		  if( ++m_iPos > 1 ) 
			  return S_FALSE;		  
		  *rgelt = m_pUnk;		  
		  (*rgelt)->AddRef();		  
		  if( pceltFetched != NULL ) 
			  *pceltFetched = 1;		  
		  return S_OK;
	  }
	  
	  STDMETHOD(Skip)(ULONG celt) {
		  m_iPos += celt;
		  return S_OK;
	  }
	  
	  STDMETHOD(Reset)(void) {
		  m_iPos = 0;
		  return S_OK;
	  }
	  
	  STDMETHOD(Clone)(IEnumUnknown **ppenum) {
		  return E_NOTIMPL;
	  }

protected:
	LONG		m_iPos;
	ULONG		m_dwRef;
	IUnknown	*m_pUnk;
};


#endif  //__DIRECTUI_EnumUnknown_H__