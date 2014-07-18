#pragma  once

#include "51ktv.h"
#include <algorithm>
#include "imodulemanager.h"
#include "Net\ipport.h"
#include "net\NetSpeedDetector.h"
#include "Utility\StringHelper.h"
#include "InnerDefine.h"

class INetTypePingTestEvent
{
public:
	//首次返回(用于立即发起连接)
	virtual void OnReturn1stNetType(ENM_NET_TYPE) = 0;

	//测试3s超时到，按到达顺序排序
	virtual void OnPingTestComplete(std::vector<ENM_NET_TYPE>&) = 0;
};

class NetTypePingTest : public common::net::INetSpeedDetectorEvent
{
public:

	NetTypePingTest(INetTypePingTestEvent* pEventToken, INetTypePingTestEvent* pEventHall);
	
	//初始化待测速样本ip
	void Initialize(std::map<ENM_NET_TYPE, uint32>& ips);

	bool IsPingTested()
	{
		return m_bTested;
	}

	void PingTest();

private:

	void OnDetectReply(std::wstring& strIP, DWORD dwTimeout, int index);
	void OnDetectTimeout();

	//该类型是否返回过
	bool IsNetTypeReturned(ENM_NET_TYPE);
	
	bool m_bTested;
	std::map<ENM_NET_TYPE, std::wstring> m_type2IP;
	INetTypePingTestEvent* m_pNetTypePingTestEvent4Token;
	INetTypePingTestEvent* m_pNetTypePingTestEvent4Hall;
	std::vector<ENM_NET_TYPE> m_testResult;
	common::net::CNetSpeedDetector m_netSpeedDetector;
};