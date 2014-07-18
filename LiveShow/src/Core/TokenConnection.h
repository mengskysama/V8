/**
* TokenConnection.h
* 负责登陆以及周期token
*
* @version 1.0 (2011-10-31)
* @author plushu
*
*/

#pragma once

#include "net\NetLayer.h"
#include "51ktv.h"
#include "InnerDefine.h"
#include "coredefine.h"
#include "IpPortConfig.h"
#include "net\NetSpeedDetector.h"
#include "NetTypePingTest.h"

class CoreModule;

class TokenConnection : public INetTypePingTestEvent
{
public:
	TokenConnection(CoreModule* pCoreModule, netlayer::MSG_BUF& circleBuffer);
	~TokenConnection(void);

	void OnReceive(netlayer::MsgObj * msgObj);
	void OnCycleTrigger();	
	void StartLogin(core::LoginEvent& loginEvent);
	void OnLoginSuccess();
	void Logout();

	virtual void OnReturn1stNetType(ENM_NET_TYPE);
	virtual void OnPingTestComplete(std::vector<ENM_NET_TYPE>&);
		
	int GetConnectionID()
	{
		return m_token_ConnectionID;
	}

	ENM_CONECTION_STATE GetConnectionState()
	{
		return m_enmState;
	}

	void SetConnectionState(ENM_CONECTION_STATE enmState)
	{
		m_enmState = enmState;
	}

	static byte * GetSessionKey()
	{
		return m_sessionKey;
	}

	static ByteBuf GetToken()
	{
		return ByteBuf(m_tokenSize, m_tokens);
	}

private:

	void Connect(bool firstTime);
	void OnTokenConnected();
	void OnReceiveTokenData(ByteBuf& byteBuf);
	void OnReceiveCycleTokenData(ByteBuf& byteBuf);
	void TokenError2Msg(core::LoginRspEvent* loginRsp);
	void Clear();
	void DeleteConnection();
	void OnLoginTimeout(uintptr,bool);
	void OnCycleTokenTimer(uintptr,bool);
	void OnLoginRetryTimer(uintptr,bool);
	void ClearTimer();
	netlayer::IpPort GenIpPort();

	netlayer::MSG_BUF* m_pCircleBuffer;
	CoreModule* m_pCoreModule;

	core::LoginEvent m_loginEvent;
	
	uint32 m_Timer;
	uint32 m_TimerRetry;
	int m_token_ConnectionID;
	netlayer::IpPort m_token_IpPort;
	IpPortConfig m_token_config;

	ENM_CONECTION_STATE m_enmState;
	netlayer::ConnectionLayer* m_pConnectionMgr;

	static uint16 m_tokenSize;
	static byte m_tokens[TOKEN_MAX_LEN];
	static byte m_sessionKey[17];//用于udp包加密解密
};
