/*
@file:		GGHelper.h
@purpose:	GG的帮助函数,宏等
@version:	1.0 (2008.6.20)
@auther:	feller
*/

#pragma once

#include <xstring>
#include <vector>
#include <map>
#include <objidl.h>
#include "..\src\CommonLibrary\thirdlibrary\CxImage\include\ximage.h"

#define _WIN32_WINNT 0x0500
#include <Iphlpapi.h>

#ifdef UTILITY_EXPORTS
#define UTILITY_API __declspec(dllexport)
#else
#define UTILITY_API __declspec(dllimport)
#endif

/** 内存占用优化 
下面为清理内存时最少保留的内存和其它几个级别的内存数值。
*/
#define MINI_MEM_SIZE	8*1024*1024
#define MEDIUM_MEM_SIZE 12*1024*1024

/**显示系统错误提示信息
@param:ErrorText附加的提醒信息
@return void.
*/
UTILITY_API void ShowErrorMessage(WCHAR* ErrorText = L"");

UTILITY_API int HelperGetClipboardTextLength();

// nMax则是szBuffer缓冲区的大小
UTILITY_API int HelperGetClipboardText(wchar_t* szBuffer, int nMax);

UTILITY_API bool HelperIsAnsiChar(wchar_t wc);

UTILITY_API int HelperGetStringLengthInAnsi(const wchar_t* sz);

UTILITY_API int HelperGetSubstrInAnsi(const wchar_t* szSrc, wchar_t* sz, int nMaxInByte);

/// 从一块内存创建一个stream对象
UTILITY_API HRESULT CreateStreamOnMemory(IN void* pSrc, IN INT iSize, OUT LPSTREAM FAR* ppstm);

UTILITY_API int	GetHttpReplyCode(char *szHttpReply);

UTILITY_API HBITMAP CopyScreenToBitmap(LPRECT lpRect);

//DragonZ:USE FOR Account ComboBox
UTILITY_API bool HelperIsKeyDown(UINT uVirtKey);

UTILITY_API bool GetMacAddress(std::wstring& macAddr);

UTILITY_API bool GetRealMacAddress(std::wstring& macAddr);
