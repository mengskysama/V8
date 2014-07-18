/********************************************************************
created:	
created:	
file base:	
file ext:	h
author:		Garret Lian

purpose:	
*********************************************************************/
#ifndef __DIRECTUI_OleInPlaceFrame_H__
#define __DIRECTUI_OleInPlaceFrame_H__


class IActivXOwner;


//////////////////////////////////////////////////////////////////////////
class CActiveXFrameWnd : public IOleInPlaceFrame
{
public:
	CActiveXFrameWnd(IActivXOwner* pOwner) : 
	  m_dwRef(1), m_pOwner(pOwner), m_pActiveObject(NULL) { }

	  ~CActiveXFrameWnd() {
		  if( m_pActiveObject != NULL )
			  m_pActiveObject->Release();
	  }

public:
	  // IUnknown
	  STDMETHOD_(ULONG,AddRef)() {
		  return ++m_dwRef;
	  }

	  STDMETHOD_(ULONG,Release)() {
		  ULONG lRef = --m_dwRef;
		  if( lRef == 0 ) 
			  delete this;
		  return lRef;
	  }

	  STDMETHOD(QueryInterface)(REFIID riid, LPVOID *ppvObject)  {
		  *ppvObject = NULL;
		  if( riid == IID_IUnknown ) 
			  *ppvObject = static_cast<IOleInPlaceFrame*>(this);
		  else if( riid == IID_IOleWindow ) 
			  *ppvObject = static_cast<IOleWindow*>(this);
		  else if( riid == IID_IOleInPlaceFrame ) 
			  *ppvObject = static_cast<IOleInPlaceFrame*>(this);
		  else if( riid == IID_IOleInPlaceUIWindow ) 
			  *ppvObject = static_cast<IOleInPlaceUIWindow*>(this);
		  if( *ppvObject != NULL ) 
			  AddRef();
		  return *ppvObject == NULL ? E_NOINTERFACE : S_OK;
	  }  

	  // IOleInPlaceFrameWindow
	  STDMETHOD(InsertMenus)(HMENU /*hmenuShared*/, LPOLEMENUGROUPWIDTHS /*lpMenuWidths*/) {
		  return S_OK;
	  }

	  STDMETHOD(SetMenu)(HMENU /*hmenuShared*/, HOLEMENU /*holemenu*/, HWND /*hwndActiveObject*/) {
		  return S_OK;
	  }

	  STDMETHOD(RemoveMenus)(HMENU /*hmenuShared*/) {
		  return S_OK;
	  }

	  STDMETHOD(SetStatusText)(LPCOLESTR /*pszStatusText*/) {
		  return S_OK;
	  }

	  STDMETHOD(EnableModeless)(BOOL /*fEnable*/) {
		  return S_OK;
	  }

	  STDMETHOD(TranslateAccelerator)(LPMSG /*lpMsg*/, WORD /*wID*/) {
		  return S_FALSE;
	  }

	  // IOleWindow
	  STDMETHOD(GetWindow)(HWND* phwnd);

	  STDMETHOD(ContextSensitiveHelp)(BOOL /*fEnterMode*/) {
		  return S_OK;
	  }

	  // IOleInPlaceUIWindow
	  STDMETHOD(GetBorder)(LPRECT /*lprectBorder*/) {
		  return S_OK;
	  }

	  STDMETHOD(RequestBorderSpace)(LPCBORDERWIDTHS /*pborderwidths*/) {
		  return INPLACE_E_NOTOOLSPACE;
	  }

	  STDMETHOD(SetBorderSpace)(LPCBORDERWIDTHS /*pborderwidths*/) {
		  return S_OK;
	  }

	  STDMETHOD(SetActiveObject)(IOleInPlaceActiveObject* pActiveObject, LPCOLESTR /*pszObjName*/) {
		  if( m_pActiveObject != NULL )
			  m_pActiveObject->Release();
		  m_pActiveObject = pActiveObject;
		  if( m_pActiveObject != NULL )
			  m_pActiveObject->AddRef();
		  return S_OK;
	  }

protected:
	ULONG m_dwRef;
	IActivXOwner *m_pOwner;
	IOleInPlaceActiveObject* m_pActiveObject;
};


#endif  //__DIRECTUI_OleInPlaceFrame_H__