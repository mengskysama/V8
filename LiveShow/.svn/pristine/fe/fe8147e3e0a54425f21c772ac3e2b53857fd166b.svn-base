#pragma once

#include "Skin_i.h"

#ifdef UTILITY_EXPORTS
#define UTILITY_API __declspec(dllexport)
#else
#define UTILITY_API __declspec(dllimport)
#endif

//create skin engine
BOOL UTILITY_API CreateSkinEngine(ISkinEngine** pEngine, HANDLE* pModuleHandle = NULL);

//get skin engine
BOOL UTILITY_API GetSkinEngine(ISkinEngine** ppEngine);
