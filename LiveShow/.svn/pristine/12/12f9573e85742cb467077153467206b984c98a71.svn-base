/**
* ipport.h
* 
*
* @version 1.0 (06 ,04, 2008)
* @author reinhardt ken
*
*/
#pragma  once

#include <string>
#include <WinSock2.h>

namespace netlayer {
#pragma pack(1)
class IpPort {
public:
	IpPort(unsigned int uiIP = 0, unsigned short usPort = 0)
		: m_uiIP(uiIP), m_usPort(usPort) {
	};

	IpPort(const std::string& strIP, unsigned short usPort) 
		: m_usPort(usPort), m_uiIP(0),m_strIP(strIP) {
// #ifdef WIN32
// 		hostent * hp = gethostbyname(strIP.c_str());
// 		if (hp) {
// 			m_uiIP =  *(unsigned int *)hp->h_addr;
// 		}
// #else
// 		m_uiIP = inet_addr(strIP.c_str());
// #endif
	}

	bool operator == (const IpPort& v) const {
		return m_uiIP == v.m_uiIP && m_usPort == v.m_usPort
			&&m_strIP==v.m_strIP;
	}

	bool operator != (const IpPort& v) const {
		return !operator == (v);
	}

	bool operator < (const IpPort& v) const {
		return m_uiIP < v.m_uiIP 
			|| (m_uiIP == v.m_uiIP && m_usPort < v.m_usPort);
	}

	inline unsigned int GetIp() const {
		return m_uiIP;
	}

	std::string GetIpStr() const
	{
		return m_strIP;
	}

	inline unsigned short GetPort() const {
		return htons(m_usPort);
	}
	
	//  [11/6/2008 ken] 设置了数字格式的，就把string格式的清空
	inline void SetIp(unsigned int uiIp) {
		m_uiIP = uiIp;
		m_strIP="";
	}

	//  [11/6/2008 ken] 设置了string格式的，就把数字格式的清空
	inline void SetIp(std::string const & ip)
	{
// #ifdef WIN32
// 		hostent * hp = gethostbyname(ip.c_str());
// 		if (hp) {
// 			m_uiIP =  *(unsigned int *)hp->h_addr;
// 		}
// #else
// 		m_uiIP = inet_addr(ip.c_str());
// #endif
		m_uiIP=0;
		m_strIP=ip;
	}
	
	inline void SetPort(unsigned short usPort) {
		m_usPort = usPort;
	}

private:
	unsigned int m_uiIP;
	unsigned short m_usPort;
	std::string m_strIP;//  [11/6/2008 ken] 与m_uiIP不同时使用
}; //end class IpPort
#pragma pack()
};
