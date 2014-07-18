#include "StdAfx.h"
#include "Connection.h"
#include "protocol\Login.h"
#include "protocol\Token.h"
#include "utility\timers.h"
#include <boost/bind.hpp>
#include "InnerDefine.h"
#include "CoreModule.h"
#include "log.h"
#include "Core.h"

Connection::Connection(CoreModule* pCoreModule)
:m_pCoreModule(pCoreModule),
m_tokenConnection(pCoreModule, m_circleBuffer),
m_hallConnection(pCoreModule, m_circleBuffer),
m_netTypePingTest(&m_tokenConnection, &m_hallConnection)
{
	netlayer::GetConnectionLayer();

	m_netTypePingTest.Initialize(m_hallConnection.GetNetTypeIps());
}

Connection::~Connection(void)
{
	netlayer::DestroyConnectionLayer();
}

void Connection::OnCycleTrigger()
{
	Receive();
}

void Connection::StartLogin(LoginEvent& loginEvent)
{
	bool bStateOK = (m_tokenConnection.GetConnectionState() == enm_state_notbegin ||
		m_tokenConnection.GetConnectionState() == enm_state_fail ||
		m_hallConnection.GetConnectionState() == enm_state_notbegin ||
		m_hallConnection.GetConnectionState() == enm_state_fail);

	assert(bStateOK);

	if(!bStateOK)
	{
		LoginRspEvent* loginRsp = new LoginRspEvent();
		loginRsp->enmLoginResult = LOGIN_ERROR_CLIENT_STATE_ERROR;
		loginRsp->strFailReason = L"ÇëÎðÖØ¸´µÇÂ½¡£";
		m_pCoreModule->GetModuleManager()->PushEvent(
			MakeEvent<MODULE_ID_CORE>()(EVENT_VALUE_CORE_LOGIN_RSP, MODULE_ID_HALL, 0, 0, 0, loginRsp));
		return;
	}

	CoreModule::GetCurInfo().enmAccountType = loginEvent.enmAccountType;
	CoreModule::GetCurInfo().strLoginAccount = loginEvent.strAccount;
	
	m_tokenConnection.SetConnectionState(enm_state_notbegin);
	m_hallConnection.SetConnectionState(enm_state_notbegin);

	if(!m_netTypePingTest.IsPingTested())
	{
		LOG_FILE_CACHE(L"Connection::StartLogin() PingTest()");
		m_netTypePingTest.PingTest();
	}
	else
	{
		LOG_FILE_CACHE(L"Connection::StartLogin()");
	}

	m_tokenConnection.StartLogin(loginEvent);
}

void Connection::CleanCircleBuffer()
{
	netlayer::MsgObj * msgObj;

	while(m_circleBuffer.Read(msgObj))
	{
		if(msgObj->uiID == m_hallConnection.GetConnectionID())
		{
			//log
		}
		else if(msgObj->uiID == m_tokenConnection.GetConnectionID())
		{
			//log
		}
		else 
		{
			assert(false);
		}
	}
}

void Connection::Logout()
{
	CleanCircleBuffer();

	m_tokenConnection.Logout();
	m_hallConnection.Logout();

	ClientPacket::Init();
	CoreModule::GetCurInfo().Init();	
}

void Connection::SendPacket(ClientPacket* ppacket)
{
	m_hallConnection.SendPacket(ppacket);
}

void Connection::OnLoginSuccess()
{
	m_tokenConnection.OnLoginSuccess();
}

void Connection::Receive()
{
	netlayer::MsgObj * msgObj = NULL;

	while(m_circleBuffer.Read(msgObj))
	{
		if(msgObj == NULL)
		{
			assert(false);
			continue;
		}

		if(msgObj->uiID == m_hallConnection.GetConnectionID())
		{
			m_hallConnection.OnReceive(msgObj);
		}
		else if(msgObj->uiID == m_tokenConnection.GetConnectionID())
		{
			m_tokenConnection.OnReceive(msgObj);
			
			if(m_tokenConnection.GetConnectionState() == enm_state_ok && 
				m_hallConnection.GetConnectionState() == enm_state_notbegin)
			{
				m_hallConnection.StartLogin();
			}
		}
		else 
		{
			assert(false);
		}

		msgObj = NULL;
	}
}
