// Core.cpp : 定义 DLL 应用程序的入口点。
//

#include "stdafx.h"
#include "Core.h"

#ifdef _MANAGED
#pragma managed(push, off)
#endif

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
					 )
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
    return TRUE;
}

#ifdef _MANAGED
#pragma managed(pop)
#endif

CoreModule* g_CoreModule = NULL;
INSTANCE_LOG(L"CoreModule")

IModule*  GetModule()
{
	if(g_CoreModule == NULL)
	{
		g_CoreModule = new CoreModule();
	}

	return g_CoreModule;
}

void ReleaseModule()
{
	delete g_CoreModule ;
	g_CoreModule = NULL;
}