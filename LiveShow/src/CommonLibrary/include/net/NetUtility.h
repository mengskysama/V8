/**
* NetUtility.h
* 网络相关的api
*
* @version 1.0 (2010-1-11)
* @author plushu
*
*/
#pragma once

#include "..\Common.h"
#include "winsock2.h"
#include <string>
#include "..\net\ProxyTest.h"

namespace common
{
	namespace net
	{
		/* 
		* 检查网络状态
		* @return 网络是否连接正常
		*/

		bool COMMON_INTERFACE CheckNetworkAlive();

		/*
		* 域名解析回调函数签名(HANDLE: AsyncGetHostByName返回的标示, int: 错误码(0表示成功) hostent* 无需被回调者删除)
		*/	

		typedef void (*GETHOSTBYNAME_CALLBACK)(HANDLE, int, hostent*, void*);

		/* 
		* 非阻塞式域名解析
		* @param [in] pHostName:将要解析的域名
		* @param [in] pCallBack:回调函数参数
		* @return 解析任务标示 
		*/

		HANDLE COMMON_INTERFACE AsyncGetHostByName(const char* pHostName, void* pContext, GETHOSTBYNAME_CALLBACK pCallBack);

		/* 
		* 检测ie代理设置
		* @param [out] ip:代理ip
		* @param [out] port:代理端口
		* @return 是否设置了代理
		*/

		bool COMMON_INTERFACE DetectIEProxySettings(std::wstring &ip, unsigned short& port);

		/* 
		* 获取代理测试的辅助对象
		* @return 代理测试的全局对象
		*/

		CProxyTest COMMON_INTERFACE & GetProxyTest();
	}
}