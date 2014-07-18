#include "StdAfx.h"
#include "IpPortConfig.h"
#include "winsock2.h"
#include <boost/format.hpp>
#include <boost/algorithm/string.hpp>
#include <algorithm>
#include "InnerDefine.h"
#include "Utility\SystemHelper.h"
#include "net\NetUtility.h"
#include "NetTypePingTest.h"
#include "log.h"
#include "Core.h"

IpPortConfig::IpPortConfig(ENM_SERVER_TYPE enmServerType)
:m_enmServerType(enmServerType)
,m_curIndex(0)
{
	static bool first = true;

	if(first)
	{
		first = false;
		srand(GetTickCount());
	}

	ReadXMl();
}

std::map<ENM_NET_TYPE, uint32> IpPortConfig::GetNetTypeIps()
{
	std::map<ENM_NET_TYPE, uint32> map;

	for(std::map<ENM_NET_TYPE, std::vector<uint32>>::iterator it = m_type2Ips.begin();
		it != m_type2Ips.end(); ++it)
	{
		std::vector<uint32>& vecips = it->second;
		map[it->first] =  vecips[rand()%vecips.size()];
	}

	return map;
}

void IpPortConfig::OnReturn1stNetType(ENM_NET_TYPE netType)
{
	if(m_type2Ips.find(netType) != m_type2Ips.end())
	{
		std::vector<uint32>& ipsVec = m_type2Ips[netType];

		for(int i=0;i<ipsVec.size();i++)
		{
			m_ipVec.push_back(ipsVec[i]);
		}
	}
}

bool IpPortConfig::IsNetTypeInResult(ENM_NET_TYPE netType, std::vector<ENM_NET_TYPE>& testResult)
{
	for(std::vector<ENM_NET_TYPE>::iterator it = testResult.begin();
		it != testResult.end(); ++it)
	{
		if(netType == *it)
		{
			return true;
		}
	}

	return false;
}

void IpPortConfig::OnPingTestComplete(std::vector<ENM_NET_TYPE>& testResult2)
{
	std::vector<ENM_NET_TYPE> testResult = testResult2;

	//测速未返回的类型
	std::vector<ENM_NET_TYPE> notReturnType;

	for(std::map<ENM_NET_TYPE, std::vector<uint32>>::iterator it = m_type2Ips.begin();
		it != m_type2Ips.end(); ++it)
	{
		if(!IsNetTypeInResult(it->first, testResult))
		{
			notReturnType.push_back(it->first);
		}
	}

	if(notReturnType.size() > 0)
	{
		//随机打乱顺序
		int beginIndex = rand()%notReturnType.size();
		for(int i=0;i<notReturnType.size();i++)
		{
			testResult.push_back(notReturnType[(beginIndex+i)%notReturnType.size()]);
		}
	}

	//////////////////////////////////////////////////////////////////////////

	m_ipVec.clear();

	int i=0;
	int count = testResult.size();

	while(true)
	{
		bool bOver = true;

		for(int j=0;j<count;j++)
		{
			if(m_type2Ips.find(testResult[j]) != m_type2Ips.end())
			{
				std::vector<uint32>& ips = m_type2Ips[testResult[j]];

				if(i<ips.size())
				{
					bOver = false;
					m_ipVec.push_back(ips[i]);
				}
			}
		}

		if(bOver)
		{
			break;
		}

		i++;
	}
}

void IpPortConfig::ReadXMl()
{
	std::wstring strXml = common::utility::systemhelper::Get179AppPath();
	strXml += CLIENT_COFIG_FILE;

	common::utility::XmlConfig xmlConfig;
	bool bsucc = xmlConfig.LoadFile(strXml);
	if(!bsucc)
	{
		assert(0);
		return;
	}

	ReadIP(xmlConfig, enm_net_type_cnc);
	ReadIP(xmlConfig, enm_net_type_ctc);

	ReadPort(xmlConfig);	
}

void IpPortConfig::ReadIP(common::utility::XmlConfig& xmlConfig, ENM_NET_TYPE enmNetType)
{
	USES_CONVERSION;

	std::wstring strConfigParent = m_enmServerType == enm_server_type_hall ? 
		CLIENT_COFIG_FILE_SERVER_HALL : CLIENT_COFIG_FILE_SERVER_TOKEN;

	std::wstring strIpTag = GetIpTag(enmNetType);

	common::utility::XmlConfig::ConfigResult result;
	bool bsucc = xmlConfig.LoadConfig(strConfigParent, strIpTag, false, result);
	if(!bsucc)
	{
		assert(0);
		return;
	}

	std::wstring& strIpTagRes = result[0][strIpTag];

	std::vector<std::wstring> vecIps = common::utility::stringhelper::Split_String_by_Delimiter(strIpTagRes, L',');

	std::vector<uint32> ipsVec;

	//随机打乱顺序
	int beginIndex = rand()%vecIps.size();
	for(int i=0;i<vecIps.size();i++)
	{
		ipsVec.push_back(inet_addr(T2A(vecIps[(beginIndex+i)%vecIps.size()].c_str())));
	}

	if(ipsVec.size() > 0)
	{
		m_type2Ips[enmNetType] = ipsVec;
	}
}

void IpPortConfig::ReadPort(common::utility::XmlConfig& xmlConfig)
{
	std::wstring strConfigParent = m_enmServerType == enm_server_type_hall ? 
		CLIENT_COFIG_FILE_SERVER_HALL : CLIENT_COFIG_FILE_SERVER_TOKEN;
	
	std::wstring strPortTag = L"";

	if(m_enmServerType == enm_server_type_hall)
	{
		strPortTag = CLIENT_COFIG_FILE_SERVER_HALL_PORT;
	}
	else
	{
		strPortTag = CLIENT_COFIG_FILE_SERVER_TOKEN_179_PORT;
		strPortTag += L",";
		strPortTag += CLIENT_COFIG_FILE_SERVER_TOKEN_VISITOR_PORT;
	}

	common::utility::XmlConfig::ConfigResult resultPort;
	bool bsucc = xmlConfig.LoadConfig(strConfigParent, strPortTag, false, resultPort);
	if(!bsucc)
	{
		assert(0);
		return;
	}

	if(m_enmServerType == enm_server_type_hall)
	{
		std::wstring& strPortRes = resultPort[0][CLIENT_COFIG_FILE_SERVER_HALL_PORT];
		m_type2Port[enm_server_type_hall] = _wtoi(strPortRes.c_str());
	}
	else
	{
		std::wstring& strPort179Res = resultPort[0][CLIENT_COFIG_FILE_SERVER_TOKEN_179_PORT];
		m_type2Port[enm_server_type_179] = _wtoi(strPort179Res.c_str());

		std::wstring& strPortVisitorRes = resultPort[0][CLIENT_COFIG_FILE_SERVER_TOKEN_VISITOR_PORT];
		m_type2Port[enm_server_type_visitor] = _wtoi(strPortVisitorRes.c_str());
	}
}

uint32 IpPortConfig::NextIP()
{
	if(m_ipVec.size() == 0)
	{
		return 0;
	}

	uint32 ip = m_ipVec[m_curIndex++];

	if(m_curIndex == m_ipVec.size())
	{
		m_curIndex = 0;
	}

	return ip;
}

uint16 IpPortConfig::GetPort(ENM_SERVER_TYPE enmServerType)
{
	if(m_type2Port.find(enmServerType) == m_type2Port.end())
	{
		return 0;
	}

	return m_type2Port[enmServerType];
}