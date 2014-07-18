#include "stdafx.h"
#include "WMI_MACAddress.h"
#include <comutil.h>
#include <Wbemidl.h>
#include <tchar.h>
#include <strsafe.h>
#include <algorithm>
#include <ntddndis.h>
#include <atlconv.h>
#include <objbase.h>
#include <winioctl.h>

#pragma comment (lib, "comsuppw.lib")
#pragma comment (lib, "wbemuuid.lib")

typedef struct _T_WQL_QUERY
{
	CHAR*	szSelect;		// SELECT语句
	WCHAR*	szProperty;		// 属性字段
} T_WQL_QUERY;

// WQL查询语句
const T_WQL_QUERY szWQLQuery[] = {
	// 包含USB网卡
	"SELECT * FROM Win32_NetworkAdapter WHERE (MACAddress IS NOT NULL) AND (NOT (PNPDeviceID LIKE 'ROOT%'))",
	L"PNPDeviceID",

	// 不包含USB网卡
	"SELECT * FROM Win32_NetworkAdapter WHERE (MACAddress IS NOT NULL) AND (NOT (PNPDeviceID LIKE 'ROOT%')) AND (NOT (PNPDeviceID LIKE 'USB%'))",
	L"PNPDeviceID"
};

static BOOL WMI_DoWithPNPDeviceID( const TCHAR *PNPDeviceID, T_MACADDRESS *pMacAddress, INT iIndex )
{
	TCHAR	DevicePath[MAX_PATH];
	HANDLE	hDeviceFile;	
	BOOL	isOK = FALSE;

	// 生成设备路径名
	StringCchCopy( DevicePath, MAX_PATH, TEXT("\\\\.\\") );
	StringCchCat( DevicePath, MAX_PATH, PNPDeviceID );
	StringCchCat( DevicePath, MAX_PATH, TEXT("#{ad498944-762f-11d0-8dcb-00c04fc3358c}") );

	// 将“PNPDeviceID”中的“\”替换成“#”，以获得真正的设备路径名
	std::replace( DevicePath + 4, DevicePath + 4 + _tcslen(PNPDeviceID), TEXT('\\'), TEXT('#') ); 

	// 获取设备句柄
	hDeviceFile = CreateFile( DevicePath,
		0,
		FILE_SHARE_READ | FILE_SHARE_WRITE,
		NULL,
		OPEN_EXISTING,
		0,
		NULL);

	if( hDeviceFile != INVALID_HANDLE_VALUE )
	{
		ULONG	dwID;
		BYTE	ucData[8];
		DWORD	dwByteRet;		

		// 获取当前MAC地址
		dwID = OID_802_3_CURRENT_ADDRESS;
		isOK = DeviceIoControl( hDeviceFile, IOCTL_NDIS_QUERY_GLOBAL_STATS, &dwID, sizeof(dwID), ucData, sizeof(ucData), &dwByteRet, NULL );
		if( isOK )
		{
			memcpy( pMacAddress[iIndex].MACAddress, ucData, dwByteRet );

			// 获取原生MAC地址
			dwID = OID_802_3_PERMANENT_ADDRESS;
			isOK = DeviceIoControl( hDeviceFile, IOCTL_NDIS_QUERY_GLOBAL_STATS, &dwID, sizeof(dwID), ucData, sizeof(ucData), &dwByteRet, NULL );
			if( isOK )
			{
				memcpy( pMacAddress[iIndex].PermanentAddress, ucData, dwByteRet );
			}
		}

		CloseHandle( hDeviceFile );
	}

	return isOK;
}

INT WMI_MacAddress( INT iQueryType, T_MACADDRESS *pMacAddress, INT iSize )
{
	HRESULT hres;
	INT	iTotal = 0;
	
	// 判断查询类型是否支持
	if( (iQueryType < 0) || (iQueryType >= sizeof(szWQLQuery)/sizeof(T_WQL_QUERY)) )
	{
		return -1;	// 查询类型不支持
	}

	/* plushu

	// 初始化COM
    hres = CoInitializeEx( NULL, COINIT_MULTITHREADED ); 
    if( FAILED(hres) )
    {
        return -2;
    }

    // 设置COM的安全认证级别
	hres = CoInitializeSecurity( 
		NULL, 
		-1, 
		NULL, 
		NULL, 
		RPC_C_AUTHN_LEVEL_DEFAULT, 
		RPC_C_IMP_LEVEL_IMPERSONATE,
        NULL,
        EOAC_NONE,
        NULL
		);
	if( FAILED(hres) )
    {
     //plushu   CoUninitialize();
        return -2;
    }*/
    
	// 获得WMI连接COM接口
    IWbemLocator *pLoc = NULL;
    hres = CoCreateInstance( 
		CLSID_WbemLocator,             
        NULL, 
        CLSCTX_INPROC_SERVER, 
        IID_IWbemLocator,
		reinterpret_cast<LPVOID*>(&pLoc)
		); 
    if( FAILED(hres) )
    {
	//plushu	CoUninitialize();
        return -2;
    }

    // 通过连接接口连接WMI的内核对象名"ROOT\\CIMV2"
	IWbemServices *pSvc = NULL;
	hres = pLoc->ConnectServer(
         _bstr_t( L"ROOT\\CIMV2" ),
         NULL,
         NULL,
         NULL,
         0,
         NULL,
         NULL,
         &pSvc
		 );    
    if( FAILED(hres) )
    {
		pLoc->Release(); 
     //plushu   CoUninitialize();
        return -2;
    }

	// 设置请求代理的安全级别
    hres = CoSetProxyBlanket(
		pSvc,
		RPC_C_AUTHN_WINNT,
		RPC_C_AUTHZ_NONE,
		NULL,
		RPC_C_AUTHN_LEVEL_CALL,
		RPC_C_IMP_LEVEL_IMPERSONATE,
		NULL,
		EOAC_NONE
		);
	if( FAILED(hres) )
    {
        pSvc->Release();
        pLoc->Release();     
  //plushu      CoUninitialize();
        return -2;
    }

    // 通过请求代理来向WMI发送请求
    IEnumWbemClassObject *pEnumerator = NULL;
    hres = pSvc->ExecQuery(
		bstr_t("WQL"), 
		bstr_t( szWQLQuery[iQueryType].szSelect ),
        WBEM_FLAG_FORWARD_ONLY | WBEM_FLAG_RETURN_IMMEDIATELY, 
        NULL,
        &pEnumerator
		);
	if( FAILED(hres) )
    {
        pSvc->Release();
        pLoc->Release();
  //plushu      CoUninitialize();
        return -3;
    }

    // 循环枚举所有的结果对象  
    while( pEnumerator )
    {
		IWbemClassObject *pclsObj = NULL;
		ULONG uReturn = 0;

		if( (pMacAddress != NULL) && (iTotal >= iSize) )
		{
			break;
		}

        pEnumerator->Next(
			WBEM_INFINITE,
			1, 
            &pclsObj,
			&uReturn
			);

        if( uReturn == 0 )
        {
            break;
        }

		VARIANT	vtProperty;
		TCHAR szProperty[128];
			
		// 获取网卡设备标识符
		VariantInit( &vtProperty );	
		pclsObj->Get( szWQLQuery[iQueryType].szProperty, 0, &vtProperty, NULL, NULL );
		StringCchCopy( szProperty, sizeof(szProperty)/sizeof(TCHAR), W2T(vtProperty.bstrVal) );
		VariantClear( &vtProperty );

		if( pMacAddress != NULL )
		{	// 通过设备标识符获取原生MAC地址和当前MAC地址
			if( WMI_DoWithPNPDeviceID( szProperty, pMacAddress, iTotal ) )
			{
				iTotal++;
			}
		}
		else
		{
			iTotal++;
		}

		pclsObj->Release();
    } // End While

    // 释放资源
	pEnumerator->Release();
    pSvc->Release();
    pLoc->Release();    
 //plushu   CoUninitialize();

    return iTotal;
}
