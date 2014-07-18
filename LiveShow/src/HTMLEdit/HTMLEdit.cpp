/*
@file:		HtmlEdit.cpp
@purpose:	dllmain
@feature:	
@version:	1.0 2008.01.17 created by ken
*/
#include "stdafx.h"
#define _WTL_NO_AUTOMATIC_NAMESPACE

#ifdef GG_GROUP_SUPPORT


#include <atlbase.h>
//#include "WTL80\include\atlapp.h"
//WTL::CAppModule _Module;
CAtlWinModule _Module;

BOOL APIENTRY DllMain( HANDLE hModule, 
					  DWORD  ul_reason_for_call, 
					  LPVOID lpReserved
					  )
{

	//return _Module.DllMain(hModule,ul_reason_for_call,lpReserved,NULL,&LIBID_ATLLib);
// 	switch (ul_reason_for_call)
// 	{
// 	case DLL_PROCESS_ATTACH:
// 		{
// 			HRESULT hRes = ::CoInitialize(NULL);
// 			ATLASSERT(SUCCEEDED(hRes));
// 			//WTL::AtlInitCommonControls(ICC_COOL_CLASSES | ICC_BAR_CLASSES);	// add flags to support other controls
// 			hRes =_Module.Init(NULL, (HINSTANCE)hModule, &LIBID_ATLLib);
// 			_Module.DllMain()
// 			ATLASSERT(SUCCEEDED(hRes));
// 			DisableThreadLibraryCalls((HINSTANCE)hModule);//?????
// 			break;
// 		}
// 	case DLL_THREAD_ATTACH:
// 		{
// 			break;
// 		}
// 	case DLL_THREAD_DETACH:
// 		{
// 			break;
// 		}
// 	case DLL_PROCESS_DETACH:
// 		{
// 			_Module.Term();
// 			break;
// 		}
// 		
// 	}
	return TRUE;
}

#endif