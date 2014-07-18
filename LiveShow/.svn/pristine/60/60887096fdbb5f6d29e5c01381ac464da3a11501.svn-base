#pragma once
#include <string>

#ifdef _SUPPORT_COMMON_EXPORT
#define COMMON_INTERFACE _declspec(dllexport)
#else 
#define COMMON_INTERFACE _declspec(dllimport)
#endif

#define COMMON_API extern "C" __declspec(dllexport)

COMMON_API void install(int channel, wchar_t* sid);

COMMON_INTERFACE bool get_macverify(int channel, std::wstring& mac, std::wstring& verify);

COMMON_API bool get_hardware_info(std::wstring& hardware);
