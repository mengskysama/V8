// stdafx.cpp : source file that includes just the standard includes
// Skin.pch will be the pre-compiled header
// stdafx.obj will contain the pre-compiled type information

#include "stdafx.h"

HRESULT CreateStreamOnMemory(IN void* pSrc, IN INT iSize, OUT LPSTREAM FAR* ppstm)
{
	*ppstm = NULL;

	if (iSize < 0)
		return E_FAIL;

	HGLOBAL hGlobal = ::GlobalAlloc(GMEM_FIXED | GMEM_ZEROINIT, iSize);
	if (hGlobal == NULL)
		return E_FAIL;

	void* pDst = ::GlobalLock(hGlobal);
	_macro_memcpy(pDst, iSize, pSrc, iSize);
	::GlobalUnlock(hGlobal);

	HRESULT hr = ::CreateStreamOnHGlobal(hGlobal, TRUE, ppstm);
	if (FAILED(hr)) {
		::GlobalFree(hGlobal);
		return hr;
	}

	return hr;
}
