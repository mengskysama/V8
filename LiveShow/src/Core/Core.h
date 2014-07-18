#include "51ktv.h"
#include "imodule.h"
#include "CoreModule.h"
#include "log.h"

extern "C" 
{
	__declspec( dllexport ) IModule *  GetModule();
	__declspec( dllexport ) void  ReleaseModule();
}

extern CoreModule* g_CoreModule;
EXTERN_LOG()