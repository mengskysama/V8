/**
* NetUtility.cpp
* 网络相关的api实现
*
* @version 1.0 (2010-1-11)
* @author plushu
*
*/

#include "..\..\include\Net\NetUtility.h"
#include "Iphlpapi.h"
#include "tchar.h"
#include "..\..\..\..\include\winhttp.h"
#include "..\..\include\utility\SystemHelper.h"

namespace common
{
namespace net
{
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

	bool Check_IP_ADDR_STRING(IP_ADDR_STRING* pString ,unsigned int & ip)
	{
		while(pString != NULL)
		{
			if(pString->IpAddress.String[0] != 0
				&& strcmp(pString->IpAddress.String, "0.0.0.0") != 0)
			{
				ip= inet_addr((pString->IpAddress.String) ); 
				ip=htonl(ip);
				return true;
			}

			pString = pString->Next;
		}

		return false;
	}

	bool Check_IP_ADAPTER_INFO(IP_ADAPTER_INFO* piai)
	{
		while(piai != NULL)
		{
			unsigned int uip;
			if(Check_IP_ADDR_STRING(&piai->IpAddressList,uip))
			{
				if(CheckInternalIP(uip) == false && 
					common::utility::systemhelper::IsWindowsVersionEqualOrLater(common::utility::systemhelper::OS_VISTA))
					return true;//如果是外网地址，同时是vista，不在检查网关。
				else if(Check_IP_ADDR_STRING(&piai->GatewayList,uip))
					return true;
			}

			piai = piai->Next;
		}

		return false;
	}

	bool IsIPAvailable()
	{
		bool bHasIP = false;

		IP_ADAPTER_INFO stAdapterInfo[20];			// Allocate information for up to 20 NICs

		DWORD dwBufLen = sizeof(stAdapterInfo);		// Save the memory size of buffer

		DWORD dwStatus = GetAdaptersInfo(			// Call GetAdapterInfo
			stAdapterInfo,							// [out] buffer to receive data
			&dwBufLen);								// [in] size of receive data buffer

		if(dwStatus == ERROR_SUCCESS)               // Verify return value is valid, no buffer overflow
		{
			return Check_IP_ADAPTER_INFO(&stAdapterInfo[0]);
		}
		else if(dwStatus == ERROR_BUFFER_OVERFLOW)
		{
			IP_ADAPTER_INFO* piai = ( IP_ADAPTER_INFO* )HeapAlloc( GetProcessHeap( ), 0, dwBufLen );

			if( piai != NULL )
			{
				dwStatus = GetAdaptersInfo( piai, &dwBufLen );

				if(ERROR_SUCCESS == dwStatus)
				{
					bHasIP = Check_IP_ADAPTER_INFO(piai);
				}

				HeapFree(GetProcessHeap(), 0, piai);
			}
		}

		return bHasIP;
	}

	bool CheckNetworkAlive()
	{
		bool bRet = false;

		DWORD dwTableSize = 0;
		PMIB_IFTABLE lpTable = NULL;

		// Get struct size
		if (ERROR_INSUFFICIENT_BUFFER != GetIfTable(NULL, &dwTableSize, FALSE))
		{
			return bRet;
		}

		// Allocate memory
		lpTable = (PMIB_IFTABLE) new char[dwTableSize];
		if (NULL == lpTable)
		{
			return bRet;
		}

		// Get Info
		if (ERROR_SUCCESS != GetIfTable(lpTable, &dwTableSize, FALSE))
		{
			delete [] lpTable;
			return bRet;
		}

		for (DWORD i=0; i<lpTable->dwNumEntries; ++i)
		{
			if(( lpTable->table[i].dwType == MIB_IF_TYPE_ETHERNET || 
				lpTable->table[i].dwType == MIB_IF_TYPE_PPP) && 
				( lpTable->table[i].dwOperStatus == MIB_IF_OPER_STATUS_CONNECTED ||
				lpTable->table[i].dwOperStatus ==  MIB_IF_OPER_STATUS_OPERATIONAL))
			{
				if(IsIPAvailable())
				{
					bRet = true;
					break;
				}
			}
		}

		if (NULL != lpTable)
		{
			delete [] lpTable;
		}

		return bRet;
	}

	//////////////////////////////////////////////////////////////////////////

	#define GETHOSTBYNAME_MSG (WM_USER+2010)
	#define GETHOSTBYNAME_WND_CLASSNAME _T("AsyncGetHostByName Helper Window")

	struct AsyncGetHostByNameData
	{
		GETHOSTBYNAME_CALLBACK pCallBack;
		void* pContext;
		char cHostent[MAXGETHOSTSTRUCT];
	};

	static LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
	{
		if(message == GETHOSTBYNAME_MSG)
		{
			LONG lData = GetWindowLong(hWnd, GWL_USERDATA);

			wchar_t sz[100]={0};

			if(lData > 0)
			{
				AsyncGetHostByNameData* pData = (AsyncGetHostByNameData*)lData;

				int nErrorCode = WSAGETASYNCERROR(lParam);

				hostent* pHostent = NULL;

				if(nErrorCode == 0)
				{
					pHostent = (hostent*)pData->cHostent;
				}

				(*pData->pCallBack)((HANDLE)wParam, nErrorCode, pHostent, pData->pContext);
				
				wsprintf(sz, L"%s_%d", GETHOSTBYNAME_WND_CLASSNAME, (int)pData->pContext);

				delete pData;
			}

			DestroyWindow(hWnd);

			UnregisterClass(sz, GetModuleHandle(0));

			return 0;
		}

		return DefWindowProc(hWnd, message, wParam, lParam);
	}

	HANDLE AsyncGetHostByName(const char* pHostName, void* pContext, GETHOSTBYNAME_CALLBACK pCallBack)
	{
		WNDCLASSEX wndclass;
		wndclass.cbSize		   = sizeof wndclass;
		wndclass.style		   = 0;
		wndclass.lpfnWndProc   = WindowProc;
		wndclass.cbClsExtra	   = 0;
		wndclass.cbWndExtra    = 0;
		wndclass.hInstance     = GetModuleHandle(0);
		wndclass.hIcon		   = 0;
		wndclass.hCursor	   = 0;
		wndclass.hbrBackground = 0;
		wndclass.lpszMenuName  = 0;
		wchar_t sz[100]={0};
		wsprintf(sz, L"%s_%d", GETHOSTBYNAME_WND_CLASSNAME, (int)pContext);
		wndclass.lpszClassName = sz;
		wndclass.hIconSm       = 0;

		if(RegisterClassEx(&wndclass))
		{
			AsyncGetHostByNameData* pData = new AsyncGetHostByNameData;
			memset(&pData->cHostent, 0, sizeof(pData->cHostent));
			pData->pCallBack = pCallBack;
			pData->pContext = pContext;
			HWND hWnd = CreateWindow(sz, sz, 0, 0, 0, 0, 0, 0, 0, 0, GetModuleHandle(0));
			SetWindowLong(hWnd, GWL_USERDATA, (LONG)pData);

			return WSAAsyncGetHostByName(hWnd, GETHOSTBYNAME_MSG, pHostName, pData->cHostent, MAXGETHOSTSTRUCT);
		}

		return 0;
	}

	bool DetectIEProxySettings(std::wstring &ip, unsigned short& port)
	{
		WINHTTP_CURRENT_USER_IE_PROXY_CONFIG ieProxyConfig;
		::ZeroMemory(&ieProxyConfig, sizeof(ieProxyConfig));

		if (::WinHttpGetIEProxyConfigForCurrentUser(&ieProxyConfig))
		{
			if (ieProxyConfig.lpszProxy != NULL)
			{
				std::wstring temp;
				temp = ieProxyConfig.lpszProxy;
				wchar_t tempC=':';
				std::size_t pos =temp.find(tempC);
				ip=temp.substr(0,pos);
				std::wstring wStrPort=temp.substr(pos+1,temp.size());
				port=_wtoi(wStrPort.c_str());

				::GlobalFree(ieProxyConfig.lpszProxy);
				::GlobalFree(ieProxyConfig.lpszProxyBypass);

				return true;
			}
		}

		return false;
	}

	CProxyTest& GetProxyTest()
	{
		static CProxyTest s_ProxyTest;
		return s_ProxyTest;
	}
}
}