/********************************************************************
	created:	
	created:
	file base:
	file ext:	cpp
	author:		Garret Lian
	
	purpose:	
*********************************************************************/
#include "StdAfx.h"
#include "UIFlashCtrl.h"

#define UITRACE(...) 


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CUIFlashCtrl::CUIFlashCtrl(IUIFlashEventNotify* en /*= 0*/) :
m_pEventNotify(en),
m_pFlash(0),
m_dwConnPointID(0),
m_pDD(0),
m_bFixTransparency(FALSE)
{

} 

CUIFlashCtrl::~CUIFlashCtrl()
{
	if(m_pDD)
		m_pDD->Release();
}


//////////////////////////////////////////////////////////////////////////
//IUnknown
STDMETHODIMP_(ULONG) CUIFlashCtrl::AddRef()
{
	return PARENT::AddRef();
}

STDMETHODIMP_(ULONG) CUIFlashCtrl::Release()
{
	return PARENT::Release();
}

STDMETHODIMP CUIFlashCtrl::QueryInterface(REFIID riid, void ** ppvObject)
{
	*ppvObject = 0;

	PARENT::QueryInterface(riid, ppvObject);

	if(*ppvObject == NULL) {
		if(IsEqualGUID (riid, IID_IDispatch))
			*ppvObject = static_cast<ShockwaveFlashObjects::_IShockwaveFlashEvents *>(this);
		else if (IsEqualGUID(riid, ShockwaveFlashObjects::DIID__IShockwaveFlashEvents))
			*ppvObject = (void*)static_cast<ShockwaveFlashObjects::_IShockwaveFlashEvents *>(this);
		else if (IsEqualGUID(riid, ShockwaveFlashObjects::IID_IServiceProvider))
			*ppvObject = (void*)static_cast<ShockwaveFlashObjects::IServiceProvider *>(this);

		if(*ppvObject != NULL)
			AddRef();
	}

	return *ppvObject == NULL ? E_NOINTERFACE : S_OK;
}



//////////////////////////////////////////////////////////////////////////
//IDispatch
STDMETHODIMP CUIFlashCtrl::Invoke(DISPID dispIdMember, 
								  REFIID  riid, 
								  LCID lcid,
								  WORD wFlags, 
								  DISPPARAMS FAR* pDispParams,  
								  VARIANT FAR* pVarResult, 
								  EXCEPINFO FAR* pExcepInfo,
								  unsigned int FAR*  puArgErr)
{
	switch(dispIdMember) 
	{ 
	case DISPID_READYSTATECHANGE: 
		if(pDispParams->cArgs == 1)
			return OnReadyStateChange(pDispParams->rgvarg[0].intVal);
		break; 
	case 0x7A6://Progress		 
		if(pDispParams->cArgs == 1)
			return OnProgress(pDispParams->rgvarg[0].intVal);
		break; 
	case 0x96://FSCommand
		if(pDispParams->cArgs == 2)
			return FSCommand(pDispParams->rgvarg[1].bstrVal, pDispParams->rgvarg[0].bstrVal);
		break; 
	case 0xC5:
		if(pDispParams->cArgs == 1)
			return FlashCall(pDispParams->rgvarg[0].bstrVal);
		break;
	default:
		{
			UITRACE("Invoke%d\n", dispIdMember);
			return DISP_E_MEMBERNOTFOUND; 
		}
	} 

	return S_OK;
}



//////////////////////////////////////////////////////////////////////////
STDMETHODIMP CUIFlashCtrl::OnReadyStateChange( long newState )
{
	UITRACE("state:%d\n", newState);
	if(m_pEventNotify)
		m_pEventNotify->OnReadyStateChange(newState);
	return S_OK;
}

STDMETHODIMP CUIFlashCtrl::OnProgress( long percentDone )
{
	UITRACE("percent:%d\n", percentDone);
	if(m_pEventNotify)
		m_pEventNotify->OnProgress(percentDone);
	return S_OK;	
}

STDMETHODIMP CUIFlashCtrl::FSCommand( _bstr_t command, _bstr_t args )
{
	CString str((const char*)command);
	CString str1((const char*)args);
	UITRACE("FSCommand, command:%s, agrs %s\n", (LPCTSTR)str, (LPCTSTR)str1);
	if(m_pEventNotify)
		m_pEventNotify->FSCommand(str, str1);
	return S_OK;	
}

STDMETHODIMP CUIFlashCtrl::FlashCall(_bstr_t request )
{
	CString str((const char*)request);
	UITRACE("FlashCall, reques: %s\n", (LPCTSTR)str);
	if(m_pEventNotify)
		m_pEventNotify->FlashCall(str);
	return E_NOTIMPL;
}



//////////////////////////////////////////////////////////////////////////
STDMETHODIMP CUIFlashCtrl::raw_RemoteQueryService( GUID * guidService,
												 GUID * riid, 
												 IUnknown **ppvObject )
{
	typedef HRESULT (WINAPI* FDirectDrawCreate)( GUID FAR *lpGUID, LPDIRECTDRAW FAR *lplpDD, IUnknown FAR *pUnkOuter );

	if (IsEqualGUID(*guidService, IID_IDirectDraw)) {
		if(m_pDD == NULL) {
			HMODULE handle = LoadLibrary(L"Ddraw.dll");

			FDirectDrawCreate directDrawCreate = (FDirectDrawCreate)GetProcAddress(handle, 
				"DirectDrawCreate");

			if (directDrawCreate(NULL, &m_pDD, NULL) != DD_OK) {
				return E_NOINTERFACE;
			}
		} 
		if(m_pDD)
			return m_pDD->QueryInterface(*riid, (void**)ppvObject);
	}

	return E_NOINTERFACE;
}


//////////////////////////////////////////////////////////////////////////
bool CUIFlashCtrl::CreateControl(IActivXOwner* pOwner, CLSID clsid, const RECT& rcPos )
{
	IOleControl* pOleControl = NULL;
	HRESULT hr = ::CoCreateInstance(clsid, NULL, CLSCTX_ALL, IID_IOleControl, (LPVOID*) &pOleControl);
	if( FAILED(hr) )
	{
		OutputDebugString(L"CUIFlashCtrl::CreateControl CoCreateInstance error");
		return false;
	}

	pOleControl->QueryInterface(IID_IOleObject, (LPVOID*) &m_pObject);
	pOleControl->Release();
	if( m_pObject == NULL ) 
	{
		OutputDebugString(L"CUIFlashCtrl::CreateControl QueryInterface IID_IOleObject error");
		return false;
	}

	m_pOwner = pOwner;

	// More control creation stuff
	DWORD dwMiscStatus = 0;
	m_pObject->GetMiscStatus(DVASPECT_CONTENT, &dwMiscStatus);
	IOleClientSite* pOleClientSite = NULL;
	this->QueryInterface(IID_IOleClientSite, (LPVOID*) &pOleClientSite);
	CUIAutoPtr<IOleClientSite> RefOleClientSite = pOleClientSite;

	// Initialize control
	if( (dwMiscStatus & OLEMISC_SETCLIENTSITEFIRST) != 0 ) 
		m_pObject->SetClientSite(pOleClientSite);

	IPersistStreamInit* pPersistStreamInit = NULL;
	m_pObject->QueryInterface(IID_IPersistStreamInit, (LPVOID*) &pPersistStreamInit);

	if( pPersistStreamInit != NULL ) {
		hr = pPersistStreamInit->InitNew();
		pPersistStreamInit->Release();
	}

	if( FAILED(hr) ) {
		m_pObject->Release();
		m_pObject = NULL;		
		OutputDebugString(L"CUIFlashCtrl::CreateControl QueryInterface InitNew error");
		return false;
	}

	if( (dwMiscStatus & OLEMISC_SETCLIENTSITEFIRST) == 0 )
		m_pObject->SetClientSite(pOleClientSite);

	// Grab the view...
	hr = m_pObject->QueryInterface(IID_IViewObjectEx, (LPVOID*) &m_pViewObject);	
	if( FAILED(hr) )
		hr = m_pObject->QueryInterface(IID_IViewObject2, (LPVOID*) &m_pViewObject);	
	if( FAILED(hr) ) 
		hr = m_pObject->QueryInterface(IID_IViewObject, (LPVOID*) &m_pViewObject);

	//m_pViewObject->SetAdvise(DVASPECT_CONTENT, 0, static_cast<IAdviseSink*>(this));

	//SetPos(rcPos);

	// Activate and done...
	m_pObject->SetHostNames(OLESTR("UIFlashActiveX"), NULL);

	GetControl(__uuidof(ShockwaveFlashObjects::IShockwaveFlash), (LPVOID*)&m_pFlash);

	if(m_pFlash) {
		long lVersion = m_pFlash->FlashVersion();
		if ((lVersion & 0x00FF0000) == 0x00080000)
		{
			m_bFixTransparency = TRUE;
		}
		else
		{
			m_bFixTransparency = FALSE;
		}
		m_pFlash->PutWMode(L"transparent");
		m_pFlash->PutScale(L"showAll");
		m_pFlash->PutBackgroundColor(0x00000000);
		m_pFlash->PutEmbedMovie(FALSE);
	}

	if( (dwMiscStatus & OLEMISC_INVISIBLEATRUNTIME) == 0 ) {
		hr = m_pObject->DoVerb(OLEIVERB_INPLACEACTIVATE, 
			NULL, 
			static_cast<IOleClientSite*>(this), 
			0, \
			m_pOwner->AxGetHWND(), 
			&rcPos);

		::RedrawWindow(m_pOwner->AxGetHWND(), &rcPos, NULL, \
			RDW_INVALIDATE | RDW_UPDATENOW | RDW_ERASE | RDW_INTERNALPAINT | RDW_FRAME);
	} 

	IObjectWithSite* pSite = NULL;
	m_pObject->QueryInterface(IID_IObjectWithSite, (LPVOID*) &pSite); 	
	if( pSite != NULL )	{
		pSite->SetSite(static_cast<IOleClientSite*>(this));
		pSite->Release();
	}	

	CActiveXCtrl::Advise(this->m_pObject, 
		ShockwaveFlashObjects::DIID__IShockwaveFlashEvents, 
		static_cast<IDispatch*>(this), 
		&m_dwConnPointID );

	return m_pFlash!=NULL;
}

void CUIFlashCtrl::ReleaseControl()
{
	CActiveXCtrl::UnAdvise(this->m_pObject, 
		ShockwaveFlashObjects::DIID__IShockwaveFlashEvents, 
		m_dwConnPointID);
	if(m_pFlash != NULL)
		m_pFlash->Release();
	PARENT::ReleaseControl();
}

void CUIFlashCtrl::Play( LPCTSTR movie )
{
	if(m_pFlash) {
		if(SUCCEEDED(m_pFlash->LoadMovie(0, movie) ) )
		{
			HRESULT hr = m_pFlash->Play();
			//m_pFlash->CallFunction(L"<invoke name=\"loadAndPlayVideo\" returntype=\"xml\"><arguments><string>E:\\flash\\test.flv</string></arguments></invoke>");
			//m_pFlash->CallFunction(L"<invoke name=\"setParams\" returntype=\"xml\"><arguments><string>Â½ÎÄ»Ôsss001</string></arguments></invoke>");
			if (!SUCCEEDED(hr))
			{
				CString strTemp;
				strTemp.Format(L"CUIFlashCtrl::CUIFlashCtrl::Play Play error hr = %d", hr);
				OutputDebugString(strTemp);
			}
		}
		else
		{
			OutputDebugString(L"CUIFlashCtrl::CUIFlashCtrl::Play LoadMovie error");
		}
	}
}

void CUIFlashCtrl::CallFunction( CString func, std::vector<CString> vecParams )
{
	if (m_pFlash)
	{
		CString strTemp;
		strTemp.Format(L"<invoke name=\"%s\" returntype=\"xml\"><arguments>", func.GetBuffer(0));
		for (std::vector<CString>::iterator it = vecParams.begin(); it != vecParams.end(); it++)
		{
			CString strParam;
			CString strContent = *it;
			strContent.Replace(_T("<"), _T("&lt;"));
			strContent.Replace(_T(">"), _T("&gt;"));
			strParam.Format(L"<string>%s</string>", strContent.GetBuffer(0));
			strTemp += strParam;
		}
		strTemp += L"</arguments></invoke>";

		m_pFlash->CallFunction(strTemp.GetBuffer(0));
	}
}

long CUIFlashCtrl::GetVersion()
{
	if (m_pFlash)
	{
		return m_pFlash->FlashVersion();
	}

	return 0;
}

//////////////////////////////////////////////////////////////////////////
CUITransparentFlashCtrl::CUITransparentFlashCtrl(IUIFlashEventNotify* en /*= 0*/) :
CUIFlashCtrl(en),
m_bTransparent(true),
m_iBPP(0),
m_hdcBack(0),
m_bmpBack(0),
m_lpBitsOnly(0),
m_hdcBackW(0),
m_bmpBackW(0),
m_lpBitsOnlyW(0),
m_bmptemp(0),
m_bmptempW(0)
{
	m_rcBounds.left = m_rcBounds.top = m_rcBounds.right = m_rcBounds.bottom = 0;
}

CUITransparentFlashCtrl::~CUITransparentFlashCtrl()
{
	if (m_bmpBackW)
	{
		::SelectObject(m_hdcBack, m_bmptempW);
		::DeleteObject(m_bmpBackW);
		m_bmptempW = NULL;
	}

	if (m_hdcBackW)
	{
		::DeleteDC(m_hdcBackW);
		m_hdcBackW = NULL;
	}		

	if (m_bmpBack)
	{
		::SelectObject(m_hdcBack, m_bmptemp);
		::DeleteObject(m_bmpBack);
		m_bmpBack = NULL;
	}

	if (m_hdcBack)
	{
		::DeleteDC(m_hdcBack);
		m_hdcBack = NULL;
	}

}

void CUITransparentFlashCtrl::DoPaint( HDC hDC, const RECT& rcPaint )
{
	HRESULT hr;
	RECT r;

	if (!m_bTransparent)
	{
		RECT rTotal;
		::GetClientRect(m_hWndHost, &rTotal);

		if (!hDC)
		{
			hDC = ::GetDC(m_hWndHost);
			hr = m_pViewObject->Draw(DVASPECT_CONTENT, -1, NULL, NULL, NULL, \
				hDC, (RECTL*) &rTotal, (RECTL*) &rTotal, NULL, NULL); 
			::ReleaseDC(m_hWndHost, hDC);
		}
		else
		{
			hr = m_pViewObject->Draw(DVASPECT_CONTENT, -1, NULL, NULL, NULL, \
				hDC, (RECTL*) &rTotal, (RECTL*) &rTotal, NULL, NULL); 
		}
		return;
	}

	::GetWindowRect(m_hWndHost, &r);

	if (!m_hdcBack || !EqualRect(&r, &m_rcBounds))
	{
		if (m_bmpBackW)
		{
			::SelectObject(m_hdcBack, m_bmptempW);
			::DeleteObject(m_bmpBackW);
			m_bmptempW = NULL;
		}

		if (m_hdcBackW)
		{
			::DeleteDC(m_hdcBackW);
			m_hdcBackW = NULL;
		}		

		if (m_bmpBack)
		{
			::SelectObject(m_hdcBack, m_bmptemp);
			::DeleteObject(m_bmpBack);
			m_bmpBack = NULL;
		}

		if (m_hdcBack)
		{
			::DeleteDC(m_hdcBack);
			m_hdcBack = NULL;
		}

		m_rcBounds = r;

		HDC hdc = ::GetDC(m_hWndHost);

		BITMAPINFOHEADER bih = {0};
		bih.biSize = sizeof(BITMAPINFOHEADER);
		bih.biBitCount = 32;
		bih.biCompression = BI_RGB;
		bih.biPlanes = 1;
		bih.biWidth = r.right - r.left;
		bih.biHeight = -(r.bottom - r.top);

		m_hdcBack = CreateCompatibleDC(hdc);
		m_bmpBack = CreateDIBSection(hdc, (BITMAPINFO *)&bih, DIB_RGB_COLORS, (void **)&m_lpBitsOnly, NULL, 0x0);

		m_bmptemp = (HBITMAP)SelectObject(m_hdcBack, m_bmpBack);

		if (m_bFixTransparency)
		{
			m_hdcBackW = CreateCompatibleDC(hdc);
			m_bmpBackW = CreateDIBSection(hdc, (BITMAPINFO *)&bih, DIB_RGB_COLORS, (void **)&m_lpBitsOnlyW, NULL, 0x0);
			m_bmptempW = (HBITMAP)SelectObject(m_hdcBackW, m_bmpBackW);
		}

		::ReleaseDC(m_hWndHost, hdc);

		if (m_iBPP == 0)
			m_iBPP = GetDeviceCaps(m_hdcBack, BITSPIXEL);
	}

	POINT p = {r.left, r.top};
	POINT p2 = {0, 0};
	SIZE sz = {r.right-r.left, r.bottom-r.top};

	RECT rTotal;
	::GetClientRect(m_hWndHost, &rTotal);
	RECTL rcBounds = {rTotal.left, rTotal.top, rTotal.right, rTotal.bottom};
	BYTE *dst = m_lpBitsOnly, *dstW;

	if(m_lpBitsOnly == NULL)
	{
		return;
	}

	if (m_iBPP == 32)
	{
		if (!m_bFixTransparency) //if flash player version is other than 8, do usual painting
		{
			memset(m_lpBitsOnly, 0, sz.cx * sz.cy * 4);

			hr = m_pViewObject->Draw(DVASPECT_TRANSPARENT, -1, NULL, NULL, NULL, \
				m_hdcBack, (RECTL*) &rTotal, (RECTL*) &rTotal, NULL, NULL);
		}
		else //if player version is 8, we need to fix flash player 8 control transparency bug
		{
			memset(m_lpBitsOnly, 0, sz.cx * sz.cy * 4);
			memset(m_lpBitsOnlyW, 255, sz.cx * sz.cy * 4);

			hr = m_pViewObject->Draw(DVASPECT_TRANSPARENT, -1, NULL, NULL, NULL, \
				m_hdcBack, (RECTL*) &rTotal, (RECTL*) &rTotal, NULL, NULL); 
			hr = m_pViewObject->Draw(DVASPECT_TRANSPARENT, -1, NULL, NULL, NULL, \
				m_hdcBackW, (RECTL*) &rTotal, (RECTL*) &rTotal, NULL, NULL); 

			dst = m_lpBitsOnly;
			dstW = m_lpBitsOnlyW;

			BYTE r, g, b, a, rw, gw, bw, aw, alpha_r, alpha_g, alpha_b, alpha;

			for (int y = 0; y < sz.cy; y++)
			{
				for (int x = 0; x < sz.cx; x++)
				{
					//the idea is that we draw the same data onto black and white DC's
					//and then calculate per pixel alpha based on difference, produced by alpha blending
					r = *dst++;
					g = *dst++;
					b = *dst++;
					a = *dst++;
					rw = *dstW++;
					gw = *dstW++;
					bw = *dstW++;
					aw = *dstW++;
					alpha_r = rw-r;
					alpha_g = gw-g;
					alpha_b = bw-b;
					//division by 3 is for accuracy and can be replaced by
					//alpha = alpha_g; for example
					alpha = (alpha_r + alpha_g + alpha_b) / 3;
					*(dst - 1) = 255 - alpha;
					//this algorithm should be optimized for MMX to achieve best performance
				}
			} 
		}
	}
	else //in 8/16/24 bit screen depth UpdateLayeredWindow produces wrong results - we use underlaying DC to paint to
	{
		HDC hdcParent = ::GetWindowDC(m_hWndHost);

		BOOL bRet = BitBlt(m_hdcBack, 0, 0, rTotal.right, rTotal.bottom, hdcParent, 0, 0, SRCCOPY);
		::ReleaseDC(m_hWndHost, hdcParent);

		hr = m_pViewObject->Draw(DVASPECT_TRANSPARENT, -1, NULL, NULL, NULL, \
			m_hdcBack, (RECTL*) &rTotal, (RECTL*) &rTotal, NULL, NULL); 
		dst = m_lpBitsOnly;
	}

	BLENDFUNCTION bf;
	bf.BlendOp = AC_SRC_OVER;
	bf.AlphaFormat = AC_SRC_ALPHA;
	bf.BlendFlags = 0;
	bf.SourceConstantAlpha = 255;
	BOOL bRet = UpdateLayeredWindow(m_hWndHost, NULL, &p, &sz, m_hdcBack, &p2, 0, &bf, m_iBPP == 32 ? ULW_ALPHA : ULW_OPAQUE);

	ReleaseDC(hDC);
}