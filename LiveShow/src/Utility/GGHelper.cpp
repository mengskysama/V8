#include "stdafx.h"
#include "winhttp.h"
#include <atlbase.h>  
#include "nb30.h"

#pragma comment(lib, "netapi32.lib")
#pragma comment(lib, "wininet.lib")
#pragma comment(lib, "Shell32")
#pragma comment(lib, "Iphlpapi.lib")

#include "tlhelp32.h"
#include "Shellapi.h"
#include "sddl.h"
#include "aclapi.h"
#include <Psapi.h>
#include <algorithm>
#include "boost\algorithm\string.hpp"
#include "GGHelper.h"
#include "utility/stringhelper.h"
#include "WMI_MACAddress.h"

using namespace common::utility;

void ShowErrorMessage(WCHAR* ErrorText)
{
	///测试代码，获取下错误信息
	WCHAR ErrorMsg[400];
	LPVOID lpMsgBuf;
	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER| //自动分配消息缓冲区
		FORMAT_MESSAGE_FROM_SYSTEM, //从系统获取信息
		NULL,
		GetLastError(), //获取错误信息标识
		MAKELANGID(LANG_NEUTRAL,SUBLANG_DEFAULT),//使用系统缺省语言
		// 本文转自 C Builder研究 - http://www.ccrun.com/article.asp?i=150&d=zh47o8
		(LPTSTR)&lpMsgBuf, //消息缓冲区
		0,
		NULL);
	swprintf_s(ErrorMsg, 400, L"%s, 由于以下错误而失败: %s", ErrorText, lpMsgBuf);
	AfxMessageBox(ErrorMsg, MB_ICONSTOP);//弹出错误提示框
	LocalFree(lpMsgBuf);
}

int HelperGetClipboardTextLength()
{
	int nRet = 0;	// char count

	if (OpenClipboard(NULL))
	{
		HGLOBAL hG = GetClipboardData(CF_UNICODETEXT);
		if (hG)
		{
			size_t sz = GlobalSize(hG);
			nRet = sz / 2 - 1;
		}

		CloseClipboard();
	}

	return nRet;
}

// nMax则是szBuffer缓冲区的大小
int HelperGetClipboardText(wchar_t* szBuffer, int nMax)
{
	// 传入的szBuffer为空
	if( szBuffer == NULL)
		return 0;

	int nRet = nMax;	// char copyed

	if (OpenClipboard(NULL))
	{
		HGLOBAL hG = GetClipboardData(CF_UNICODETEXT);
		if (hG)
		{
			wchar_t* lpsz = (wchar_t*)GlobalLock(hG);
			if (lpsz)
			{
				int nLenRaw = GlobalSize(hG) / 2 - 1;

				if (nLenRaw < nMax-1)
				{
					nRet = nLenRaw;
				}

				wcsncpy_s(szBuffer, nRet, lpsz,_TRUNCATE);
			}
			GlobalUnlock(hG);
			lpsz = NULL;	// invalidate
		}

		CloseClipboard();
	}

	return nRet;
}

bool HelperIsAnsiChar(wchar_t wc)
{
	return (char)wc == wc;
}

int HelperGetStringLengthInAnsi(const wchar_t* sz)
{
	int nRet = 0;

	const int len = wcslen(sz);
	for (int i = 0; i < len; i++)
	{
		nRet += HelperIsAnsiChar(sz[i]) ? 1 : 2;
	}

	return nRet;
}

int HelperGetSubstrInAnsi(const wchar_t* szSrc, wchar_t* sz, int nMaxInByte)
{
	int nRet = nMaxInByte;

	if (HelperGetStringLengthInAnsi(szSrc) <= nMaxInByte)
	{
		nRet = nMaxInByte;
		wcscpy(sz, szSrc);
	}
	else
	{
		const int srcLen = wcslen(szSrc);
		int i = 0;
		int count = 0;
		nRet = 0;
		for (i=0, count=0; i<nMaxInByte && count<nMaxInByte; count++) {
			i += (HelperIsAnsiChar(szSrc[count])) ? (1) : (2);
			if (i > nMaxInByte)
				break;
			nRet = i;
			sz[count] = szSrc[count];
		}
	}

	return nRet;
}

HRESULT CreateStreamOnMemory(IN void* pSrc, IN INT iSize, OUT LPSTREAM FAR* ppstm)
{
	*ppstm = NULL;

	if (iSize < 0)
		return E_FAIL;

	HGLOBAL hGlobal = ::GlobalAlloc(GMEM_FIXED | GMEM_ZEROINIT, iSize);
	if (hGlobal == NULL)
		return E_FAIL;

	void* pDst = ::GlobalLock(hGlobal);
	memcpy_s(pDst, iSize, pSrc, iSize);
	::GlobalUnlock(hGlobal);

	HRESULT hr = ::CreateStreamOnHGlobal(hGlobal, TRUE, ppstm);
	if (FAILED(hr)) {
		::GlobalFree(hGlobal);
		return hr;
	}

	return hr;
}

int	GetHttpReplyCode(char *szHttpReply)
{
	int n = 0;
	char* p = szHttpReply;
	char* pCode = NULL;
	char  chTemp = 0;
	while(*p)
	{
		if(*p==' ')
		{
			p ++;
			pCode = p;
			while(*p)
			{
				if(*p==' '|| *p=='\r' || *p=='\n')
				{
					chTemp = *p;

					*p = 0;

					n = atoi(pCode);
					*p = chTemp;
					break;
				}
				p ++;
			}
			//n = atoi(pCode);
			break;
		}
		p ++;
	}

	return n;		
}

HBITMAP CopyScreenToBitmap(LPRECT lpRect)
{
	// 屏幕和内存设备描述表
	HBITMAP hBitmap, hOldBitmap;

	// 位图句柄
	int nX, nY, nX2, nY2;

	// 选定区域坐标
	int nWidth, nHeight;

	// 确保选定区域不为空矩形
	if (IsRectEmpty(lpRect))
		return NULL;

	//为屏幕创建设备描述表
	HDC hScrDC = CreateDC(_T("DISPLAY"), NULL, NULL, NULL);

	if(!hScrDC)
	{
		return NULL;
	}

	//为屏幕设备描述表创建兼容的内存设备描述表
	HDC hMemDC = CreateCompatibleDC(hScrDC);

	// 获得选定区域坐标
	nX = lpRect->left;
	nY = lpRect->top;
	nX2 = lpRect->right;
	nY2 = lpRect->bottom;
	
	nWidth = nX2 - nX;
	nHeight = nY2 - nY;

	// 创建一个与屏幕设备描述表兼容的位图
	hBitmap = CreateCompatibleBitmap(hScrDC, nWidth, nHeight);

	// 把新位图选到内存设备描述表中
	hOldBitmap = (HBITMAP)SelectObject(hMemDC, hBitmap);
	
	// 把屏幕设备描述表拷贝到内存设备描述表中
	BitBlt(hMemDC, 0, 0, nWidth, nHeight, hScrDC, nX, nY, SRCCOPY);

	::DeleteObject(hBitmap);
	
	hBitmap = (HBITMAP)SelectObject(hMemDC, hOldBitmap);

	//清除 
	DeleteDC(hScrDC);
	DeleteDC(hMemDC);

	// 返回位图句柄
	return hBitmap;
}

bool HelperIsKeyDown(UINT uVirtKey)
{
	return GetKeyState(uVirtKey) < 0;
}

//////////////////////////////////////////////////////////////////////////

//检查是否是内部的IP地址
bool CheckInternalIP(const unsigned int ip_addr)
{
	//检查3类地址
	if ((ip_addr >= 0x0A000000 && ip_addr <= 0x0AFFFFFF ) ||
		(ip_addr >= 0xAC100000 && ip_addr <= 0xAC1FFFFF ) ||
		(ip_addr >= 0xC0A80000 && ip_addr <= 0xC0A8FFFF ) 
		)
	{
		return true;
	}

	return false;
}

bool Check_IP_ADDR_STRING(IP_ADDR_STRING*& pString)
{
	while(pString != NULL)
	{
		if(pString->IpAddress.String[0] != 0
			&& strcmp(pString->IpAddress.String, "0.0.0.0") != 0)
		{
			return true;
		}
		pString = pString->Next;
	}

	return false;
}

bool Check_IP_ADAPTER_INFO(IP_ADAPTER_INFO* piai, std::wstring& macAddr)
{
	IP_ADAPTER_INFO* piaiBegin = piai;

	//优先MIB_IF_TYPE_ETHERNET

	while(piai != NULL)
	{
		if(piai->Type == MIB_IF_TYPE_ETHERNET)
		{
			IP_ADDR_STRING* pString = &piai->IpAddressList;

			while(pString != NULL)
			{
				if(Check_IP_ADDR_STRING(pString))
				{
					for(int i=0;i<piai->AddressLength;i++)
					{
						wchar_t sz[8];
						wsprintf(sz, L"%02X", piai->Address[i]);
						macAddr.append(sz);
					}

					if(StrStrW(macAddr.c_str(), L"000000") == 0)
					{
						return true;
					}
					else
					{
						macAddr = L"";
						pString = pString->Next;
					}
				}				
			}
		}

		piai = piai->Next;
	}

	//提取失败时不限MIB_IF_TYPE_ETHERNET

	piai = piaiBegin;

	while(piai != NULL)
	{
		IP_ADDR_STRING* pString = &piai->IpAddressList;

		while(pString != NULL)
		{
			if(Check_IP_ADDR_STRING(pString))
			{
				for(int i=0;i<piai->AddressLength;i++)
				{
					wchar_t sz[8];
					wsprintf(sz, L"%02X", piai->Address[i]);
					macAddr.append(sz);
				}

				if(StrStrW(macAddr.c_str(), L"000000") == 0)
				{
					return true;
				}
				else
				{
					macAddr = L"";
					pString = pString->Next;
				}
			}				
		}

		piai = piai->Next;
	}

	return false;
}

bool NetbiosGetMACaddress(wchar_t* pszMac, int nInLen)
{
	typedef struct _ASTAT_
	{
		ADAPTER_STATUS adapt;
		NAME_BUFFER    NameBuff [30];
	}ASTAT, *PASTAT;

	ASTAT Adapter;

	NCB Ncb;
	UCHAR uRetCode;
	LANA_ENUM lenum;

	memset(&Ncb, 0, sizeof(Ncb));
	Ncb.ncb_command = NCBENUM;
	Ncb.ncb_buffer = (UCHAR *)&lenum;
	Ncb.ncb_length = sizeof(lenum);
	uRetCode = Netbios(&Ncb);

	for(int i = 0; i < lenum.length; i++)
	{
		memset(&Ncb, 0, sizeof(Ncb));
		Ncb.ncb_command = NCBRESET;
		Ncb.ncb_lana_num = lenum.lana[i];

		uRetCode = Netbios( &Ncb );

		memset( &Ncb, 0, sizeof (Ncb) );
		Ncb.ncb_command = NCBASTAT;
		Ncb.ncb_lana_num = lenum.lana[i];

		strcpy_s((char *)Ncb.ncb_callname,_countof(Ncb.ncb_callname), "* "); 

		Ncb.ncb_buffer = (PUCHAR)(char *) &Adapter;
		Ncb.ncb_length = sizeof(Adapter);

		uRetCode = Netbios( &Ncb );

		if(uRetCode == 0 )
		{	
			wsprintf(pszMac, L"%02X%02X%02X%02X%02X%02X",
				Adapter.adapt.adapter_address[0],
				Adapter.adapt.adapter_address[1],
				Adapter.adapt.adapter_address[2],
				Adapter.adapt.adapter_address[3],
				Adapter.adapt.adapter_address[4],
				Adapter.adapt.adapter_address[5]);
			
			if(StrStrW(pszMac, L"000000") == 0)
			{
				return true;
			}
		}
	}

	return false;
}

bool GetRealMacAddress(std::wstring& macAddr)
{
	_T_MACADDRESS st[10] = {0};

	int count = WMI_MacAddress(0, st, 10);

	wchar_t szMac[13] = {0};

	for(int i=0;i<count;i++)
	{
		wsprintf(szMac, L"%02X%02X%02X%02X%02X%02X",
			st[i].PermanentAddress[0],
			st[i].PermanentAddress[1],
			st[i].PermanentAddress[2],
			st[i].PermanentAddress[3],
			st[i].PermanentAddress[4],
			st[i].PermanentAddress[5]);

		if(StrStrW(szMac, L"000000") == 0)
		{
			macAddr = szMac;
			return true;
		}
	}

	return false;
}

bool GetMacAddress(std::wstring& macAddr)
{
	bool bHasIP = false;

	IP_ADAPTER_INFO stAdapterInfo[20];			// Allocate information for up to 20 NICs

	DWORD dwBufLen = sizeof(stAdapterInfo);		// Save the memory size of buffer

	DWORD dwStatus = GetAdaptersInfo(			// Call GetAdapterInfo
		stAdapterInfo,							// [out] buffer to receive data
		&dwBufLen);								// [in] size of receive data buffer

	if(dwStatus == ERROR_SUCCESS)               // Verify return value is valid, no buffer overflow
	{
		return Check_IP_ADAPTER_INFO(&stAdapterInfo[0], macAddr);
	}
	else if(dwStatus == ERROR_BUFFER_OVERFLOW)
	{
		IP_ADAPTER_INFO* piai = ( IP_ADAPTER_INFO* )HeapAlloc( GetProcessHeap( ), 0, dwBufLen );

		if( piai != NULL )
		{
			dwStatus = GetAdaptersInfo( piai, &dwBufLen );

			if(ERROR_SUCCESS == dwStatus)
			{
				bHasIP = Check_IP_ADAPTER_INFO(piai, macAddr);
			}

			HeapFree(GetProcessHeap(), 0, piai);
		}
	}

	if(!bHasIP)
	{
		wchar_t sz[100] = {0};
		NetbiosGetMACaddress(sz, 99);

		macAddr = sz;
	}

	return bHasIP;
}