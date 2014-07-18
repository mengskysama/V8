/**
* ProxyTest.h
* 代理测试api
*
* @version 1.0 (2010-3-2)
* @author plushu
*
*/
#pragma once

#include "..\Common.h"
#include "..\net\ConnectionCommon.h"
#include "..\net\NetLayer.h"
#include "winsock2.h"
#include <string>
#include <map>

namespace common
{
	namespace net
	{
		enum EnmProxyType
		{
			PROXYTYPE_NULL,
			PROXYTYPE_HTTP,  // 使用HTTP代理
			PROXYTYPE_SOCK5, // 使用SOCK5代理
		};

		#define	WM_PROXY_TEST (WM_USER+0x2003)

		class COMMON_INTERFACE CProxyTest
		{
		public:
			CProxyTest();
			~CProxyTest();

			/* 
			* 由使用者定时调用(以获得主线程调用机会)
			*/

			void Trigger();

			/*
			* 提交测试代理服务器的任务
			* @param [in] enmProxyType:代理类型			
			* @param [in] oAddr:目的地址
			* @param [in] oProxyAddr:代理地址
			* @param [in] strName:代理用户名
			* @param [in] strPass:代理密码
			* @param [in] strDomain:代理域
			* @return 返回测试任务是否提交成功
			*/

			bool Test(EnmProxyType enmProxyType, 
					const HWND& hWnd,
					const netlayer::IpPort& oAddr, 
					const netlayer::IpPort& oProxyAddr, 
					const std::string& strName = "", 
					const std::string& strPass = "", 
					const std::string& strDomain = "");

		private:

			netlayer::MSG_BUF m_circleBuffer;
			std::map<uint32, HWND> m_Connection2Hwnd;
		};
	}
}