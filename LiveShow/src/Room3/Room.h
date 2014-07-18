#include "51ktv.h"
#include "imodule.h"
#include "RoomModule.h"

extern "C" 
{
	__declspec( dllexport ) IModule *  GetModule();
	__declspec( dllexport ) void  ReleaseModule();
}

extern RoomModule* g_RoomModule;