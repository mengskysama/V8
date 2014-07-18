/**
* ProxyTest.cpp
* 代理测试api实现
*
* @version 1.0 (2010-3-2)
* @author plushu
*
*/
#pragma once

#include "..\..\include\Net\ProxyTest.h"

namespace common
{
	namespace net
	{
		#define MAX_TEST_TASK_SMAE_TIME 20//同时存在的测试任务数

		CProxyTest::CProxyTest():m_circleBuffer(MAX_TEST_TASK_SMAE_TIME)
		{

		}
		
		CProxyTest::~CProxyTest()
		{

		}

		bool CProxyTest::Test(EnmProxyType enmProxyType, 
			const HWND& hWnd,
			const netlayer::IpPort& oAddr, 
			const netlayer::IpPort& oProxyAddr, 
			const std::string& strName,
			const std::string& strPass, 
			const std::string& strDomain)
		{
			netlayer::ConnectionLayer* pConnectionLayer = netlayer::GetConnectionLayer();

			if(pConnectionLayer == NULL)
			{
				return false;
			}
			
			unsigned int unConnectionID = 0;

			if(enmProxyType == PROXYTYPE_HTTP)
			{
				unConnectionID = pConnectionLayer->Create(netlayer::HTTPPROXY2, m_circleBuffer, oAddr, oProxyAddr, strName, strPass, strDomain);
			}
			else if(enmProxyType == PROXYTYPE_SOCK5)
			{
				unConnectionID = pConnectionLayer->Create(netlayer::SOCK5PROXY2, m_circleBuffer, oAddr, oProxyAddr, strName, strPass, strDomain);
			}
			else
			{
				return false;
			}

			if(unConnectionID > 0)
			{
				m_Connection2Hwnd[unConnectionID] = hWnd;
			}

			return unConnectionID > 0;
		}

		void CProxyTest::Trigger()
		{
			netlayer::MsgObj * msgObj;

			while(m_circleBuffer.Read(msgObj))
			{
				std::map<uint32, HWND>::iterator it = m_Connection2Hwnd.find(msgObj->uiID);
				
				if(it != m_Connection2Hwnd.end())
				{
					if(msgObj->enMsg == netlayer::COMM_CONNECT)
					{
						PostMessage(it->second, WM_PROXY_TEST, 1, 0);
					}
					else if(msgObj->enMsg == netlayer::COMM_CLOSE_NEED_PS)
					{
						PostMessage(it->second, WM_PROXY_TEST, 0, 0);
					}
					else
					{
						assert(0);
					}

					m_Connection2Hwnd.erase(it);
				}
				else
				{
					//assert(0);
				}

				netlayer::ConnectionLayer* pConnectionLayer = netlayer::GetConnectionLayer();

				if(pConnectionLayer != NULL)
				{
					pConnectionLayer->Delete(msgObj->uiID);
				}
			}
		}
	}
}