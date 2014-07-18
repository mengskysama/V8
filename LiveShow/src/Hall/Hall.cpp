#include "stdafx.h"
#include "Hall.h"

HallModule * g_HallModule = NULL;

IModule*  GetModule()
{
	if(g_HallModule == NULL)
	{
		g_HallModule = new HallModule();
	}

	return g_HallModule;
}

void ReleaseModule()
{
	delete g_HallModule ;
	g_HallModule = NULL;
}

