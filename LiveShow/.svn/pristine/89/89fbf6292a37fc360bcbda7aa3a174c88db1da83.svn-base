#include "StdAfx.h"

#include "ComInstanceCreator.h"

HRESULT CoCreateInstanceFromFile(LPCTSTR pszPathName, REFCLSID rclsid, REFIID riid, LPVOID* ppv, HANDLE* pModuleHandle)
{
	typedef HRESULT (STDAPICALLTYPE* PDLLGETCLASSOBJECT)(REFCLSID rclsid, REFIID riid, LPVOID* ppv);
	HRESULT hr = S_OK;
	HINSTANCE hLibrary = NULL;
	PDLLGETCLASSOBJECT pDllGetClassObject = NULL;
	IClassFactory* pFactory = NULL;

	hLibrary = ::LoadLibrary(pszPathName);
	DWORD err = GetLastError();
	if (hLibrary == NULL)
		return E_FAIL;

    if (pModuleHandle != NULL)
        *pModuleHandle = hLibrary;

	pDllGetClassObject = PDLLGETCLASSOBJECT(::GetProcAddress(hLibrary, "DllGetClassObject"));
	if (pDllGetClassObject == NULL)
		return E_FAIL;

	hr = pDllGetClassObject(rclsid, IID_IClassFactory, (void**)&pFactory);
	if (FAILED(hr) || pFactory == NULL)
		return E_FAIL;

	hr = pFactory->CreateInstance(NULL, riid, ppv);
	pFactory->Release();

	return hr;
}
