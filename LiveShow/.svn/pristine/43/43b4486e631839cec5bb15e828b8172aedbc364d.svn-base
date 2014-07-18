/**
* HallConnection.h
* ¸ºÔð´óÌüµÇÂ½
*
* @version 1.0 (2011-11-30)
* @author plushu
*
*/

#pragma once

#include "net\NetLayer.h"
#include "51ktv.h"
#include "InnerDefine.h"
#include "coredefine.h"
#include "IpPortConfig.h"
#include "protocol\ProtocolBase.h"
#include "ServerPacketProcess.h"
#include "protocol\Login.h"
#include "NetTypePingTest.h"

class CoreModule;

class HallConnection : public INetTypePingTestEvent
{
public:
	HallConnection(CoreModule* pCoreModule, netlayer::MSG_BUF& circleBuffer);
	~HallConnection(void);

	void OnReceive(netlayer::MsgObj * msgObj);
	void OnCycleTrigger();	
	void StartLogin();
	void Logout();
	void SendPacket(ClientPacket* ppacket);
	
	std::map<ENM_NET_TYPE, uint32> GetNetTypeIps()
	{
		return m_hall_config.GetNetTypeIps();
	}

	virtual void OnReturn1stNetType(ENM_NET_TYPE);
	virtual void OnPingTestComplete(std::vector<ENM_NET_TYPE>&);

	int GetConnectionID()
	{
		return m_hall_ConnectionID;
	}

	ENM_CONECTION_STATE GetConnectionState()
	{
		return m_enmState;
	}

	void SetConnectionState(ENM_CONECTION_STATE enmState)
	{
		m_enmState = enmState;
	}

private:
	
	void OnHallConnected();
	void OnReceiveHallData(ByteBuf& byteBuf);	
	void onLoginRsp(ServerHead& srvHead, ByteBuf& byteBuf);
	void onLoginSuccess(ServerHead& srvHead, LoginRspPacket& packet);
	void Clear();
	void DeleteConnection();
	void OnLoginTimeout(uintptr,bool);
	void OnHearbeatTimeout(uintptr,bool);
	void OnLoginRetryTimer(uintptr,bool);
	void ClearLoginTimer();
	void ClearHeartbeatTimer();

	netlayer::MSG_BUF* m_pCircleBuffer;
	CoreModule* m_pCoreModule;
	
	uint32 m_LoginTimer;
	uint32 m_TimerRetry;
	uint32 m_HeartbeatTimer;
	int m_hall_ConnectionID;
	netlayer::IpPort m_hall_IpPort;
	IpPortConfig m_hall_config;

	ServerPacketProcess m_severPacketProcess;

	ENM_CONECTION_STATE m_enmState;
	netlayer::ConnectionLayer* m_pConnectionMgr;
};
