/* ----------------------------------------------------------
文件名称：WMI_MACAddress.h

作者：秦建辉

MSN：splashcn@msn.com

版本历史：
	V1.1	2010年05月23日
			代码优化。

	V1.0	2010年05月12日
			完成正式版本。

功能描述：
	结合WMI和DeviceIoControl获取网卡原生MAC地址和当前MAC地址

接口函数：
	WMI_MacAddress
 ------------------------------------------------------------ */
#pragma once

#include <windows.h>

#define MACADDRESS_BYTELEN		6	// MAC地址字节长度

typedef struct _T_MACADDRESS {
	BYTE	PermanentAddress[MACADDRESS_BYTELEN];	// 原生MAC地址
	BYTE	MACAddress[MACADDRESS_BYTELEN];			// 当前MAC地址
} T_MACADDRESS;

//-------------------导出函数-------------
#ifdef __cplusplus
extern "C"{
#endif

/*
功能：结合WMI和DeviceIoControl获取网卡原生MAC地址和当前MAC地址
入口参数：
	iQueryType：需要获取的网卡类型
			0：包括USB网卡
			1：不包括USB网卡
	pMacAddress：存储网卡MAC地址
	uSize：可存储的最大网卡数目
返回值：
	 -1：不支持的设备属性值
	 -2：WMI连接失败
	 -3：不正确的WQL查询语句
	>=0：获取的网卡数目	
*/
INT WMI_MacAddress( INT iQueryType, T_MACADDRESS *pMacAddress, INT iSize );

#ifdef __cplusplus
}
#endif
