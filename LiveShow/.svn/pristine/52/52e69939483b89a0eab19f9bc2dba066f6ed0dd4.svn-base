/********************************************************************
created:	
created:	
file base:	
file ext:	h
author:		Garret Lian

  purpose:	
*********************************************************************/
#ifndef __DIRECTUI_FLASH_CTRL_H__
#define __DIRECTUI_FLASH_CTRL_H__


#include "UIActiveXBase.h"
#include "..\..\AV_aac\\Video\\dx\\ddraw.h"
#include "flash9f.tlh"
#include <vector>


class IUIFlashEventNotify;

//////////////////////////////////////////////////////////////////////////

class CUIFlashCtrl : 
	public CActiveXCtrl,
	public ShockwaveFlashObjects::_IShockwaveFlashEvents,
	public ShockwaveFlashObjects::IServiceProvider
{
	typedef CActiveXCtrl PARENT;
public:
	CUIFlashCtrl(IUIFlashEventNotify* en = 0);
	virtual ~CUIFlashCtrl();
	
public:
	// IUnknown
	STDMETHOD_(ULONG,AddRef)();
	STDMETHOD_(ULONG,Release)();
	STDMETHOD(QueryInterface)(REFIID riid, LPVOID *ppvObject);

	//IDispatch
	STDMETHOD(GetTypeInfoCount)( UINT *pctinfo) { 
		return E_NOTIMPL; 
	}

	STDMETHOD(GetTypeInfo)( UINT iTInfo, 
		LCID lcid, 
		ITypeInfo **ppTInfo) { 
		return E_NOTIMPL; 
	}

	STDMETHOD(GetIDsOfNames)( REFIID riid, 
		LPOLESTR *rgszNames,
		UINT cNames, 
		LCID lcid, 
		DISPID *rgDispId) { 
		return E_NOTIMPL; 
	}

	STDMETHOD(Invoke)( DISPID dispIdMember, 
		REFIID riid, 
		LCID lcid, 
		WORD wFlags, 
		DISPPARAMS *pDispParams,
		VARIANT *pVarResult, 
		EXCEPINFO *pExcepInfo, 
		UINT *puArgErr);

	//_IShockwaveFlashEvents
	STDMETHOD(OnReadyStateChange) (long newState );
	STDMETHOD(OnProgress) (long percentDone );
	STDMETHOD(FSCommand) (_bstr_t command, _bstr_t args );
	STDMETHOD(FlashCall) (_bstr_t request );

	//IServiceProvider
	STDMETHOD(raw_RemoteQueryService) (	
		GUID * guidService,	
		GUID * riid, 
		IUnknown **ppvObject );

public:
	virtual bool CreateControl( IActivXOwner* pOwner, CLSID clsid, const RECT& rcPos );
	virtual void ReleaseControl();
		
	void Play(LPCTSTR movie);
	void CallFunction(CString func, std::vector<CString> vecParams);
	long GetVersion();

protected:
	ShockwaveFlashObjects::IShockwaveFlash *m_pFlash;
	DWORD m_dwConnPointID;
	LPDIRECTDRAW m_pDD;
	IUIFlashEventNotify *m_pEventNotify;
	BOOL m_bFixTransparency;

};


//////////////////////////////////////////////////////////////////////////
class CUITransparentFlashCtrl : public CUIFlashCtrl
{
public:
	CUITransparentFlashCtrl(IUIFlashEventNotify* en = 0);
	~CUITransparentFlashCtrl();

public:
	virtual void DoPaint(HDC hDC, const RECT& rcPaint);

protected:
	BOOL m_bTransparent;
	//BOOL m_bFixTransparency;

	RECT m_rcBounds;

	int m_iBPP;

	HDC m_hdcBack;
	HBITMAP m_bmpBack;
	BYTE *m_lpBitsOnly;

	HDC m_hdcBackW;
	HBITMAP m_bmpBackW;
	BYTE *m_lpBitsOnlyW;	

	HBITMAP	m_bmptemp;
	HBITMAP m_bmptempW;
};


//////////////////////////////////////////////////////////////////////////
class IUIFlashEventNotify
{
public:
	virtual void OnReadyStateChange(long newState ) = 0;
	virtual void OnProgress(long percentDone ) = 0;
	virtual void FSCommand(const CString& command, const CString& args ) = 0;
	virtual void FlashCall(const CString& request ) = 0;
};


#endif  //__DIRECTUI_FLASH_CTRL_H__