#pragma  once

#include "51ktv.h"
#include <algorithm>
#include "imodulemanager.h"
#include "Net\ipport.h"
#include "Utility\XmlConfig.h"
#include "net\NetSpeedDetector.h"
#include "Utility\StringHelper.h"
#include "InnerDefine.h"

class IpPortConfig
{
public:

	IpPortConfig(ENM_SERVER_TYPE enmServerType);

	//获取抽样ip
	std::map<ENM_NET_TYPE, uint32> GetNetTypeIps();

	void OnReturn1stNetType(ENM_NET_TYPE);
	void OnPingTestComplete(std::vector<ENM_NET_TYPE>&);
	
	uint32 NextIP();
	uint16 GetPort(ENM_SERVER_TYPE enmServerType);

private:

	std::wstring GetIpTag(ENM_NET_TYPE enmNetType)
	{
		switch(enmNetType)
		{
		case enm_net_type_cnc:
			return CLIENT_COFIG_FILE_SERVER_CNC_IP;
			break;
		case enm_net_type_ctc:
			return CLIENT_COFIG_FILE_SERVER_CTC_IP;
			break;
		default:
			assert(0);
			return L"";
			break;
		}
	}

	void ReadXMl();
	void ReadIP(common::utility::XmlConfig& xmlConfig, ENM_NET_TYPE enmNetType);//->m_type2Ips
	void ReadPort(common::utility::XmlConfig& xmlConfig);//->m_type2Port
	bool IsNetTypeInResult(ENM_NET_TYPE netType, std::vector<ENM_NET_TYPE>& testResult);

	ENM_SERVER_TYPE		m_enmServerType;
	std::vector<uint32> m_ipVec;//尝试ip列表
	int					m_curIndex;
	std::map<ENM_SERVER_TYPE, uint16> m_type2Port;
	std::map<ENM_NET_TYPE, std::vector<uint32>> m_type2Ips;
};