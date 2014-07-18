/**
* Connection.h
* 登陆及链接保持
*
* @version 1.0 (2011-10-24)
* @author plushu
*
*/

#pragma once

#include "net\NetLayer.h"
#include "51ktv.h"
#include "InnerDefine.h"
#include "coredefine.h"
#include "TokenConnection.h"
#include "HallConnection.h"
#include "NetTypePingTest.h"

class CoreModule;

class Connection
{
public:
	Connection(CoreModule* pCoreModule);
	~Connection(void);

	void OnCycleTrigger();
	void StartLogin(core::LoginEvent& loginEvent);
	void Logout();
	void CleanCircleBuffer();
	void SendPacket(ClientPacket* ppacket);
	void OnLoginSuccess();

private:

	void Receive();	

	CoreModule* m_pCoreModule;

	netlayer::MSG_BUF	m_circleBuffer;
	TokenConnection		m_tokenConnection;
	HallConnection		m_hallConnection;
	NetTypePingTest		m_netTypePingTest;
};
