#include "StdAfx.h"
#include "HallConnection.h"
#include <boost/bind.hpp>
#include "utility\timers.h"
#include "InnerDefine.h"
#include "CoreModule.h"
#include "RoomDefine.h"
#include "log.h"
#include "Core.h"
#include "crypt\Crypt.h"
#include "protocol\Heartbeat.h"

HallConnection::HallConnection(CoreModule* pCoreModule,netlayer::MSG_BUF& circleBuffer)
:m_pCoreModule(pCoreModule),
m_severPacketProcess(pCoreModule),
m_pCircleBuffer(&circleBuffer),
m_enmState(enm_state_notbegin),
m_hall_config(enm_server_type_hall),
m_hall_ConnectionID(-1),
m_LoginTimer(-1),
m_TimerRetry(-1),
m_HeartbeatTimer(-1)
{
	m_pConnectionMgr = netlayer::GetConnectionLayer();
}

HallConnection::~HallConnection(void)
{

}

void HallConnection::OnCycleTrigger()
{

}

void HallConnection::StartLogin()
{
	USES_CONVERSION;

	LOG_FILE_CACHE(L"HallConnection::StartLogin()");

	m_hall_IpPort = netlayer::IpPort(m_hall_config.NextIP(), m_hall_config.GetPort(enm_server_type_hall));

	m_enmState = enm_state_doing;

	DeleteConnection();

	if(m_hall_IpPort.GetIp() != 0)
	{
		struct sockaddr_in addr;
		addr.sin_addr.s_addr = m_hall_IpPort.GetIp();

		LOG_FILE_CACHE(L"HallConnection::StartLogin() -> Connect %s", A2T(inet_ntoa(addr.sin_addr)));

		m_hall_ConnectionID = m_pConnectionMgr->Create(netlayer::TCPCLIENT2,
			*m_pCircleBuffer, m_hall_IpPort);

		m_TimerRetry = common::utility::_Timers::instance().CreateTimer((uint32)LOGIN_RETRY_CIRCLE, 
			common::utility::_Timers::TimerCallback(
			boost::bind(&HallConnection::OnLoginRetryTimer,this,boost::arg<1>(),boost::arg<2>())), 
			common::utility::_Timers::ENM_TIMER_LOGIN_RETRY, HALL_LOGIN_TIMEOUT/LOGIN_RETRY_CIRCLE);
	}
	else
	{
		assert(0);
	}

	m_LoginTimer = common::utility::_Timers::instance().CreateTimer((uint32)HALL_LOGIN_TIMEOUT, 
		common::utility::_Timers::TimerCallback(
		boost::bind(&HallConnection::OnLoginTimeout,this,boost::arg<1>(),boost::arg<2>())), 
		common::utility::_Timers::ENM_TIMER_HALL_LOGIN, 1);
}

void HallConnection::OnReturn1stNetType(ENM_NET_TYPE netType)
{
	LOG_FILE_CACHE(L"HallConnection::OnReturn1stNetType()");

	m_hall_config.OnReturn1stNetType(netType);
}

void HallConnection::OnPingTestComplete(std::vector<ENM_NET_TYPE>& testResult)
{
	LOG_FILE_CACHE(L"HallConnection::OnPingTestComplete()");

	m_hall_config.OnPingTestComplete(testResult);
}

void HallConnection::OnReceiveHallData(ByteBuf& byteBuf)
{
	ServerHead srvHead;

	srvHead.Construct(byteBuf);
	srvHead.Unpack();

	m_pCoreModule->GetModuleManager()->CallService(SERVICE_VALUE_SYS_STAT_NETCMD, srvHead.GetCmd());

	if(srvHead.GetCmd() == HALL_LOGIN_RSP)
	{
		onLoginRsp(srvHead, ByteBuf(byteBuf.first-sizeof(SERVER_HEADER),
			const_cast<byte*>(byteBuf.second+sizeof(SERVER_HEADER))));
	}
	else
	{
		m_severPacketProcess.process(srvHead, ByteBuf(byteBuf.first-sizeof(SERVER_HEADER),
			const_cast<byte*>(byteBuf.second+sizeof(SERVER_HEADER))));
	}
}

void HallConnection::SendPacket(ClientPacket* ppacket)
{
	if(m_hall_ConnectionID == -1)
	{
		return;
	}

	if(m_enmState != enm_state_ok && ppacket->GetCmd() != HALL_LOGIN_REQ)
	{
		assert(0);
	}

	m_pCoreModule->GetModuleManager()->CallService(SERVICE_VALUE_SYS_STAT_NETCMD, ppacket->GetCmd());

	ppacket->Pack();

	netlayer::Packet packet;
	packet.SetData(ppacket->Data(), ppacket->Size(), netlayer::Packet::TWO_BYTES_HEAD);

	m_pConnectionMgr->SendTo(m_hall_ConnectionID, packet, m_hall_IpPort);
}

void HallConnection::OnHallConnected()
{
	SendPacket(&LoginPacket());
}

void HallConnection::OnReceive(netlayer::MsgObj* msgObj)
{
	switch(msgObj->enMsg)
	{
	case netlayer::COMM_DATA:
		{
			ByteBuf buf(msgObj->oPacket.GetDataSize(), (byte*)msgObj->oPacket.GetBufR());
			OnReceiveHallData(buf);
			break;
		}
	case netlayer::COMM_CONNECT:
		{
			LOG_TRACE(L"Connection::OnReceiveHall:COMM_CONNECT");
			OnHallConnected();
			break;
		}
	case netlayer::COMM_ACCEPT:
		{
			assert(0);
			LOG_ERROR(L"Connection::OnReceiveHall:COMM_ACCEPT");
			break;
		}
	case netlayer::COMM_ERROR:
	case netlayer::COMM_CLOSE:
		{
			LOG_FILE_CACHE(L"Connection::OnReceiveHall:COMM_CLOSE");

			if(m_enmState == enm_state_doing)
			{
				m_enmState = enm_state_fail;

				ClearLoginTimer();

				LoginRspEvent* loginRsp = new LoginRspEvent();
				loginRsp->enmLoginResult = LOGIN_ERROR_CLIENT_HALL_CONNECT_ERROR;
				loginRsp->strFailReason = L"登录大厅服务器失败";
				m_pCoreModule->GetModuleManager()->PushEvent(
					MakeEvent<MODULE_ID_CORE>()(EVENT_VALUE_CORE_LOGIN_RSP, MODULE_ID_HALL, 0, 0, 0, loginRsp));
			}
			else if(m_enmState == enm_state_ok)
			{
				m_enmState = enm_state_fail;
				ClearHeartbeatTimer();
				m_pCoreModule->OnConnectionClose();
			}

			break;
		}
	case netlayer::COMM_CLOSE_NEED_PS:
		{
			assert(0);
			break;
		}
	default:
		assert(0);
	}
}

void HallConnection::onLoginSuccess(ServerHead& srvHead, LoginRspPacket& packet)
{
	CurInfo& curInfo = CoreModule::GetCurInfo();

	curInfo.unUIN = srvHead.GetUIN();
	curInfo.bLogined = true;
	curInfo.bMan = packet.bMan;
	curInfo.un179id = packet._179id;
	curInfo.strName = packet._179name;
	curInfo.enmVIPLevel = packet.vip_level;
	curInfo.unUserLevel = packet.usr_level;
	curInfo.unOnlineTime = packet.online_time;
	curInfo.unLeftTime = packet.left_time;
	curInfo.unLeftMoney = packet.leftMoney;
	curInfo.unSystemTime = packet.system_time_low;
	curInfo.unSelfSetting = packet.unSelfSetting;
	curInfo.bUseDianxin = packet.bDianxin;
	curInfo.bRmbUser = packet.bRmbUser;
	curInfo.bPasswordSafe = packet.bPasswordSafe;
	curInfo.unRichLevel = packet.rich_level;
	curInfo.unRichUpdateMoney = packet.rich_update_money;
	curInfo.bFirstLogin = packet.lastlogin_time_high==0 && packet.lastlogin_time_low==0;
	curInfo.unBusiness = packet.unBusiness;

	if(CoreModule::GetCurInfo().enmAccountType == ACCOUNTTYPE_VISITOR)
	{
		curInfo.un179id = curInfo.unUIN;

		wchar_t sz[100];
		wsprintf(sz, L"游客%04d", curInfo.un179id%10000);
		CoreModule::GetCurInfo().strName = sz;
	}

	m_pCoreModule->OnLoginSuccess();

	m_HeartbeatTimer = common::utility::_Timers::instance().CreateTimer((uint32)HALL_HEARTBEAT_TIMEOUT, 
		common::utility::_Timers::TimerCallback(
		boost::bind(&HallConnection::OnHearbeatTimeout,this,boost::arg<1>(),boost::arg<2>())), 
		common::utility::_Timers::ENM_TIMER_HALL_HEARTBEAT);
}

void HallConnection::onLoginRsp(ServerHead& srvHead, ByteBuf& byteBuf)
{
	LOG_CLEAR_CACHE;

	ClearLoginTimer();

	core::LoginRspEvent* pEvent = new core::LoginRspEvent();

	LoginRspPacket packet(byteBuf, *pEvent);
	packet.Unpack();

	if(pEvent->enmLoginResult == core::LOGIN_ERROR_SUCCESS)
	{
		m_enmState = enm_state_ok;
		onLoginSuccess(srvHead, packet);
	}
	else
	{
		m_enmState = enm_state_fail;
	}

	m_pCoreModule->GetModuleManager()->PushEvent(
		MakeEvent<MODULE_ID_CORE>()(EVENT_VALUE_CORE_LOGIN_RSP, 
		MODULE_ID_HALL, 0, 0, 0, pEvent));

	m_pCoreModule->GetModuleManager()->PushMessage(
		MakeMessage<MODULE_ID_CORE>()(MESSAGE_VALUE_CORE_LOGIN_COMPLETE, pEvent->enmLoginResult, 0, 0, 0));	
}

void HallConnection::OnLoginTimeout(uintptr,bool)
{
	LOG_FILE_CACHE(L"HallConnection::OnLoginTimeout()");

	LOG_FLUSH_CACHE;

	if(m_TimerRetry != -1)
	{
		common::utility::_Timers::instance().DestroyTimer(m_TimerRetry);
		m_TimerRetry = -1;
	}

	m_enmState = enm_state_fail;

	LoginRspEvent* loginRsp = new LoginRspEvent();
	loginRsp->enmLoginResult = LOGIN_ERROR_CLIENT_HALL_TIMEOUT_ERROR;
	loginRsp->strFailReason = L"登录大厅服务器超时";
	m_pCoreModule->GetModuleManager()->PushEvent(
		MakeEvent<MODULE_ID_CORE>()(EVENT_VALUE_CORE_LOGIN_RSP, MODULE_ID_HALL, 0, 0, 0, loginRsp));
}

void HallConnection::OnHearbeatTimeout(uintptr,bool)
{
	Room_Get_Room_List roomList;
	m_pCoreModule->GetModuleManager()->CallService(Room::SERVICE_ROOM_GET_ROOM_LIST, (param)&roomList);

	SendPacket(&HeartbeatPacket(roomList.vecRoomList));
}

void HallConnection::Logout()
{
	SendPacket(&LogoutPacket());
	Clear();
}

void HallConnection::Clear()
{
	m_enmState = enm_state_notbegin;
	DeleteConnection();
	ClearLoginTimer();
	ClearHeartbeatTimer();
}

void HallConnection::DeleteConnection()
{
	if(m_hall_ConnectionID != -1)
	{
		m_pConnectionMgr->Delete(m_hall_ConnectionID);
		m_hall_ConnectionID = -1;
	}
}

void HallConnection::ClearLoginTimer()
{
	if(m_TimerRetry != -1)
	{
		common::utility::_Timers::instance().DestroyTimer(m_TimerRetry);
		m_TimerRetry = -1;
	}

	if(m_LoginTimer != -1)
	{
		common::utility::_Timers::instance().DestroyTimer(m_LoginTimer);
		m_LoginTimer = -1;
	}
}

void HallConnection::ClearHeartbeatTimer()
{
	if(m_HeartbeatTimer != -1)
	{
		common::utility::_Timers::instance().DestroyTimer(m_HeartbeatTimer);
		m_HeartbeatTimer = -1;
	}
}

void HallConnection::OnLoginRetryTimer(uintptr,bool)
{
	USES_CONVERSION;

	LOG_FILE_CACHE(L"HallConnection::OnLoginRetryTimer()");

	DeleteConnection();
	
	m_hall_IpPort = netlayer::IpPort(m_hall_config.NextIP(), m_hall_config.GetPort(enm_server_type_hall));

	if(m_hall_IpPort.GetIp() != 0)
	{
		struct sockaddr_in addr;
		addr.sin_addr.s_addr = m_hall_IpPort.GetIp();

		LOG_FILE_CACHE(L"HallConnection::OnLoginRetryTimer() -> Connect %s", A2T(inet_ntoa(addr.sin_addr)));

		m_hall_ConnectionID = m_pConnectionMgr->Create(netlayer::TCPCLIENT2,
			*m_pCircleBuffer, m_hall_IpPort);
	}
}