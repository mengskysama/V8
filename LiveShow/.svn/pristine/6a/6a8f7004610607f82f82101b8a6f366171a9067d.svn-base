#include "StdAfx.h"

#include "SkinUtil.h"
#include "Skin_i.c"
#include "ComInstanceCreator.h"
#include "DynamicLibraryHolder.h"

#pragma warning(disable : 4311)

LRESULT NotifySkinHelper(HWND hWnd, WPARAM wParam, LONG num)
{
	ASSERT(::IsWindow(hWnd));
	if (! ::IsWindow(hWnd))
		return 0;

	COleVariant var(num);
	return ::SendMessage(hWnd, WM_IM_NOTIFY, wParam, (LPARAM)&var);
}

LRESULT NotifySkinHelper(HWND hWnd, WPARAM wParam, HWND h)
{
	return NotifySkinHelper(hWnd, wParam, (LONG)h);
}

LRESULT NotifySkinHelper(HWND hWnd, WPARAM wParam, CString str)
{
	ASSERT(::IsWindow(hWnd));
	if (! ::IsWindow(hWnd))
		return 0;

	COleVariant var(str);
	return ::SendMessage(hWnd, WM_IM_NOTIFY, wParam, (LPARAM)&var);
}

BOOL CreateSkinEngine(ISkinEngine** pEngine, HANDLE* pModuleHandle)
{
	*pEngine = NULL;
	ISkinEngine* pE = NULL;

#ifdef _DEBUG
	HRESULT hr = ::CoCreateInstanceFromFile(TEXT("skind.dll"), CLSID_SkinEngine, IID_ISkinEngine, (void**)&pE, pModuleHandle);
#else
	HRESULT hr = ::CoCreateInstanceFromFile(TEXT("skin.dll"), CLSID_SkinEngine, IID_ISkinEngine, (void**)&pE, pModuleHandle);
#endif // _DEBUG

	if (FAILED(hr) || pE == NULL){
		pE = NULL;
		return FALSE;
	}

	CString strSkinPath;
	::GetModuleFileName(NULL, 
		strSkinPath.GetBuffer(MAX_PATH), MAX_PATH);
	strSkinPath.ReleaseBuffer();
	int pos = strSkinPath.ReverseFind('\\');
	strSkinPath = strSkinPath.Left(pos+1);
	strSkinPath += "skin";
	CComBSTR bstrSkinPath;
	bstrSkinPath.Attach(strSkinPath.AllocSysString());
	pE->SetSkinPath((BSTR)bstrSkinPath);

	*pEngine = pE;

	return TRUE;
}

BOOL GetSkinEngine(ISkinEngine** ppEngine)
{
	static CComPtr<ISkinEngine> s_SkinEngine;
    static CDynamicLibraryHolder s_DllHolder;
	if (s_SkinEngine == NULL) {
        HANDLE hModuleHandle = NULL;
		::CreateSkinEngine(&s_SkinEngine, &hModuleHandle);
        s_DllHolder.Attach((HMODULE)hModuleHandle);
	}

	(*ppEngine) = s_SkinEngine;
	if ( (*ppEngine) != NULL ){
		(*ppEngine)->AddRef();
		return TRUE;
	}
	return FALSE;
}
