#include "stdafx.h"
#include "AV.h"

AVModule * g_pAVModule = NULL;

IModule*  GetModule()
{
	if(g_pAVModule == NULL)
	{
		g_pAVModule = new AVModule();
	}

	return g_pAVModule;
}

void ReleaseModule()
{
	delete g_pAVModule ;
	g_pAVModule = NULL;
}



// BOOL APIENTRY DllMain( HMODULE hModule,
// 					  DWORD  ul_reason_for_call,
// 					  LPVOID lpReserved
// 					  )
// {
// 	if(DLL_PROCESS_ATTACH == ul_reason_for_call)
// 	{
// 		//Encode(hModule);
// 		//Decode(NULL);
// 		int i=0;
// 		i++;
// 	}
// 	else if(DLL_THREAD_ATTACH == ul_reason_for_call)
// 	{
// 		static int i=0;
// 		if(i == 7)
// 		{
// 			//_beginthread(Decode, 0, NULL);
// 			//Decode(NULL);
// 		}
// 		OutputDebugString(L"¹þ¹þ");
// 		i++;
// 
// 		//
// 	}
// 
// 	return TRUE;
// }