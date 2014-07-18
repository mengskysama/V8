#pragma once


#ifdef _SUPPORT_COMMON_EXPORT
#define COMMON_INTERFACE _declspec(dllexport)
#else 
#define COMMON_INTERFACE _declspec(dllimport)
#endif


