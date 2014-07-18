#include "StdAfx.h"
#include "UIActiveXFrameWnd.h"
#include "UIActiveXBase.h"


STDMETHODIMP CActiveXFrameWnd::GetWindow(HWND* phwnd)
{	
	if( m_pOwner == NULL ) 
		return E_UNEXPECTED;
	*phwnd = m_pOwner->AxGetHWND();
	return S_OK;
}
