#include "51ktv.h"
#include "imodule.h"
#include "AVModule.h"
#include "AvDefine.h"

#ifdef AV_EXPORTS
#define AV_DLLEXPORT   __declspec( dllexport ) 
#else
#define AV_DLLEXPORT   __declspec( dllimport ) 
#endif

extern AVModule* g_pAVModule;

extern "C" 
{
	AV_DLLEXPORT IModule *  GetModule();
	AV_DLLEXPORT void  ReleaseModule();
}
