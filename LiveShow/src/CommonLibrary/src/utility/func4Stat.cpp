#include "stdafx.h"
#include "utility\func4Stat.h"
#include "GGHelper.h"
#include "utility\Md5.h"
#include "utility\StringHelper.h"
#include <winsock2.h>

#include "shlwapi.h"
#pragma comment(lib, "shlwapi.lib")

#include <WinInet.h>
#pragma comment(lib, "wininet.lib")

#include <setupapi.h>
#pragma comment(lib, "setupapi.lib")

#define STATISTIC_TIMEOUT 2000 

static void CALLBACK InternetCallback(HINTERNET hInternet,
									  DWORD dwContext,
									  DWORD dwInternetStatus,
									  LPVOID lpvStatusInformation,
									  DWORD dwStatusInformationLength)
{
	if (dwInternetStatus == INTERNET_STATUS_REQUEST_COMPLETE)
	{
		HANDLE hEvent = *(HANDLE*)dwContext;

		SetEvent(hEvent);
	}
}

void silentopenurl(wchar_t* url)
{
	HINTERNET handle1 = InternetOpen(L"179", INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, INTERNET_FLAG_ASYNC);

	if(handle1 == 0)
	{
		return;
	}

	HANDLE hEvent = CreateEvent(NULL, TRUE, FALSE, L"179Statistic");

	InternetSetStatusCallback(handle1, InternetCallback);

	HINTERNET handle2 = InternetOpenUrl(handle1, url, NULL, 0, NULL, (DWORD)(&hEvent));

	DWORD dwWaitResult = WaitForSingleObject(hEvent, STATISTIC_TIMEOUT);

	InternetCloseHandle(handle2);

	InternetCloseHandle(handle1);

	CloseHandle(hEvent);

	//休眠一段时间以等待Wininet线程回调Callback时退出
	Sleep(100);
}

std::wstring GetVerifyNum(int channel, std::wstring& macAddr)
{
	USES_CONVERSION;

	char sz[100] = {0};
	sprintf(sz, "%d%s", channel, T2A(macAddr.substr(0, 8).c_str()));

	std::string input = sz;
	std::string md5 = common::utility::md5::GetStringMd5(input);
	std::string result;
	for(int i=0;i<md5.size();i++)
	{
		if(md5[i] >= '0' && md5[i] <= '9')
			result.push_back(md5[i]);
	}
	for(int i=0;i<6;i++)
		result.push_back('0');
	result = result.substr(0, 6);

	std::wstring result2 = A2T(result.c_str());
	return result2;
}

bool get_macverify(int channel, std::wstring& macAddr, std::wstring& verify)
{
	if(!GetMacAddress(macAddr))
	{
		return false;
	}

	verify = GetVerifyNum(channel, macAddr);

	return true;
}

void install(int channel, wchar_t* sid)
{
	std::wstring macAddr, verify;

	if(!get_macverify(channel, macAddr, verify))
	{
		return;
	}

	wchar_t sz[100]={0};

	CString url;
	url.Format( L"http://interface.179.com/user/statisticsInstallation?pageid=%d&channel=%d&stat=%u&mac=%s&verify=%s&sid=%s" , 
		4,channel,4294967295,macAddr.c_str(),verify.c_str(),sid);

	silentopenurl(url.GetBuffer());
}

void get_cpu_info(std::string& cpuinfo)
{
	BYTE szSystemInfo[4096]={0}; // 在程序执行完毕后，此处存储取得的系统特征码
	UINT uSystemInfoLen = 0; // 在程序执行完毕后，此处存储取得的系统特征码的长度

	// CPU ID
	{
		BOOL bException = FALSE;
		BYTE szCpu[16] = {0};
		UINT uCpuID = 0U;
		__try
		{
			_asm
			{
				mov eax, 0
				cpuid
				mov dword ptr szCpu[0], ebx
				mov dword ptr szCpu[4], edx
				mov dword ptr szCpu[8], ecx
				mov eax,1
				cpuid
				mov uCpuID, edx
			}
		}
		__except( EXCEPTION_EXECUTE_HANDLER )
		{
			bException = TRUE;
		}

		if( !bException )
		{
			CopyMemory( szSystemInfo + uSystemInfoLen, &uCpuID, sizeof( UINT ) );
			uSystemInfoLen += sizeof( UINT );
			uCpuID = strlen( ( char* )szCpu );
			CopyMemory( szSystemInfo + uSystemInfoLen, szCpu, uCpuID );
			uSystemInfoLen += uCpuID;
		}
	}

	char strCPU[100]={0};
	for(int i=0;i<uSystemInfoLen;i++)
	{
		char sz[3]={0};
		sprintf(sz, "%02X", szSystemInfo[i]);
		strcat(strCPU, sz);
	}

	cpuinfo = strCPU;
}

void get_local_ip(std::string& ip)
{
	char name[255];
	PHOSTENT hostinfo;
	if(gethostname(name, sizeof(name)) == 0)
	{
		if((hostinfo = gethostbyname(name)) != NULL)
		{
			char* pip = inet_ntoa(*(struct in_addr*)*hostinfo->h_addr_list);
			ip = pip;
		}
	}
}

bool get_hardware_info(std::wstring& hardware)
{
	USES_CONVERSION;

	GUID usbGuid = {0xa5dcbf10L,0x6530,0x11d2,0x90,0x1f,0x00,0xc0,0x4f,0xb9,0x51,0xed};

	DWORD dwFlag = DIGCF_DEVICEINTERFACE|DIGCF_PRESENT;
	HDEVINFO hDevInfo = SetupDiGetClassDevs(&usbGuid, NULL, NULL, dwFlag);
	if(INVALID_HANDLE_VALUE == hDevInfo)
	{
		return GetMacAddress(hardware);
	}

	SP_DEVINFO_DATA sDevInfoData;
	sDevInfoData.cbSize = sizeof(SP_DEVINFO_DATA);

	CString strTotal;

	DWORD nSize = 0;
	TCHAR szDIS[1024];

	for(int i=0; SetupDiEnumDeviceInfo(hDevInfo, i, &sDevInfoData); i++)
	{
		nSize = 0;

		if(!SetupDiGetDeviceInstanceId(hDevInfo, &sDevInfoData, szDIS, sizeof(szDIS), &nSize))
		{
			break;
		}

		CString strDIS(szDIS);
		strTotal += strDIS;
	}

	if(strTotal.GetLength() == 0)
	{
		return GetMacAddress(hardware);
	}

	wchar_t szFilePath[MAX_PATH] ;
	GetWindowsDirectory(szFilePath, MAX_PATH);

	__int64 i64TotalBytes1, i64TotalBytes2, i64Temp;
	CString strDir;
	strDir.Format( _T("%c:\\"), szFilePath[0] ) ;
	if (!GetDiskFreeSpaceEx( strDir,(PULARGE_INTEGER)&i64Temp , 
		(PULARGE_INTEGER)&i64TotalBytes1 , (PULARGE_INTEGER)&i64Temp ) )
	{
		return GetMacAddress(hardware);
	}

	CString str;
	str.Format(L"%I64d", i64TotalBytes1);

	strTotal += str;

	std::string szTotal = T2A(strTotal.GetBuffer());

	std::string cpuinfo;
	get_cpu_info(cpuinfo);

	std::string ipinfo;
	get_local_ip(ipinfo);

	szTotal += cpuinfo;
	szTotal += ipinfo;

	std::string md5 = common::utility::md5::GetStringMd5(szTotal);

	md5 = md5.substr(0, 12);

	hardware = A2T(md5.c_str());

	common::utility::stringhelper::MakeUpper(hardware);

	return true;
}