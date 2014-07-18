#include "StdAfx.h"
#include "NetTypePingTest.h"
#include "winsock2.h"
#include <boost/format.hpp>
#include <boost/algorithm/string.hpp>
#include <algorithm>
#include "InnerDefine.h"
#include "log.h"
#include "Utility\SystemHelper.h"
#include "Utility\XmlConfig.h"
#include "net\NetUtility.h"
#include "log.h"
#include "Core.h"

NetTypePingTest::NetTypePingTest(INetTypePingTestEvent* pEventToken, INetTypePingTestEvent* pEventHall)
:m_pNetTypePingTestEvent4Token(pEventToken)
,m_pNetTypePingTestEvent4Hall(pEventHall)
,m_bTested(false)
{
	m_netSpeedDetector.Initialize(this);
}

void NetTypePingTest::Initialize(std::map<ENM_NET_TYPE, uint32>& ips)
{
	USES_CONVERSION;

	for(std::map<ENM_NET_TYPE, uint32>::iterator it = ips.begin(); it != ips.end(); ++it)
	{
		struct sockaddr_in addr;
		addr.sin_addr.s_addr = it->second;

		m_type2IP[it->first] = A2T(inet_ntoa(addr.sin_addr));
	}

	if(m_type2IP.size() == 1)
	{
		m_bTested = true;

		ENM_NET_TYPE type = m_type2IP.begin()->first;

		m_testResult.push_back(type);

		m_pNetTypePingTestEvent4Token->OnReturn1stNetType(type);
		m_pNetTypePingTestEvent4Hall->OnReturn1stNetType(type);

		OnDetectTimeout();
	}
}

void NetTypePingTest::PingTest()
{
	m_bTested = true;

	LOG_FILE_CACHE(L"NetTypePingTest::Detect()");

	std::vector<std::wstring> vecIPArray;

	for(std::map<ENM_NET_TYPE, std::wstring>::iterator it = m_type2IP.begin();
		it != m_type2IP.end(); ++it)
	{
		vecIPArray.push_back(it->second);
	}

	m_netSpeedDetector.Detect(vecIPArray);
}

void NetTypePingTest::OnDetectReply(std::wstring& strIP, DWORD dwTimeout, int index)
{
	LOG_FILE_CACHE(L"NetTypePingTest::OnDetectReply()");

	for(std::map<ENM_NET_TYPE, std::wstring>::iterator it = m_type2IP.begin();
		it != m_type2IP.end(); ++it)
	{
		if(wcscmp(strIP.c_str(), it->second.c_str()) == 0)
		{
			if(m_testResult.size() == 0)
			{
				m_pNetTypePingTestEvent4Token->OnReturn1stNetType(it->first);
				m_pNetTypePingTestEvent4Hall->OnReturn1stNetType(it->first);
			}

			if(!IsNetTypeReturned(it->first))
			{
				m_testResult.push_back(it->first);
			}
			
			break;
		}
	}
}

bool NetTypePingTest::IsNetTypeReturned(ENM_NET_TYPE netType)
{
	for(std::vector<ENM_NET_TYPE>::iterator it = m_testResult.begin();
		it != m_testResult.end(); ++it)
	{
		if(netType == *it)
		{
			return true;
		}
	}

	return false;
}

void NetTypePingTest::OnDetectTimeout()
{
	LOG_FILE_CACHE(L"NetTypePingTest::OnDetectTimeout()");

	m_pNetTypePingTestEvent4Token->OnPingTestComplete(m_testResult);
	m_pNetTypePingTestEvent4Hall->OnPingTestComplete(m_testResult);
}
