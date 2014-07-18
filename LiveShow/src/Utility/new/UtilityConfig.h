#pragma once

//#define  SUPPORT_CHECKSUM_MD5

#ifdef UTILITY_EXPORTS
#define UTILITY_DLLEXPORT   __declspec( dllexport ) 
#else
#define UTILITY_DLLEXPORT   __declspec( dllimport ) 
#endif