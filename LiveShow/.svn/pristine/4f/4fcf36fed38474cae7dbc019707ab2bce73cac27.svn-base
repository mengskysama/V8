#include "stdafx.h"
#include "Room.h"

RoomModule * g_RoomModule = NULL;

IModule*  GetModule()
{
	if(g_RoomModule == NULL)
	{
		g_RoomModule = new RoomModule();
	}

	return g_RoomModule;
}

void ReleaseModule()
{
	delete g_RoomModule ;
	g_RoomModule = NULL;
}