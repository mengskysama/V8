#include "StdAfx.h"
#include "TokenConnection.h"
#include "protocol\Token.h"
#include <boost/bind.hpp>
#include "utility\timers.h"
#include "InnerDefine.h"
#include "CoreModule.h"
#include "log.h"
#include "Core.h"

byte TokenConnection::m_sessionKey[17] = {0};
uint16 TokenConnection::m_tokenSize = 0;
byte TokenConnection::m_tokens[TOKEN_MAX_LEN] = {0};

TokenConnection::TokenConnection(CoreModule* pCoreModule,netlayer::MSG_BUF& circleBuffer)
:m_pCoreModule(pCoreModule),
m_pCircleBuffer(&circleBuffer),
m_enmState(enm_state_notbegin),
m_token_config(enm_server_type_179),
m_token_ConnectionID(-1),
m_Timer(-1),
m_TimerRetry(-1)
{
	m_pConnectionMgr = netlayer::GetConnectionLayer();
}

TokenConnection::~TokenConnection(void)
{

}

void TokenConnection::OnCycleTrigger()
{

}

netlayer::IpPort TokenConnection::GenIpPort()
{
	if(m_loginEvent.enmAccountType == core::ACCOUNTTYPE_VISITOR)
	{
		return netlayer::IpPort(m_token_config.NextIP(), m_token_config.GetPort(enm_server_type_visitor));
	}
	else
	{
		return netlayer::IpPort(m_token_config.NextIP(), m_token_config.GetPort(enm_server_type_179));
	}
}

void TokenConnection::StartLogin(LoginEvent& loginEvent)
{
	USES_CONVERSION;

	LOG_FILE_CACHE(L"TokenConnection::StartLogin()");

	m_loginEvent = loginEvent;

	m_enmState = enm_state_doing;

	Connect(true);

	m_Timer = common::utility::_Timers::instance().CreateTimer((uint32)TOKEN_TIMEOUT, 
		common::utility::_Timers::TimerCallback(
		boost::bind(&TokenConnection::OnLoginTimeout,this,boost::arg<1>(),boost::arg<2>())), 
		common::utility::_Timers::ENM_TIMER_TOKEN, 1);
}

void TokenConnection::OnReturn1stNetType(ENM_NET_TYPE netType)
{
	LOG_FILE_CACHE(L"TokenConnection::OnReturn1stNetType()");

	m_token_config.OnReturn1stNetType(netType);

	//有可能没有该type对应的token服务器
	Connect(true);
}

void TokenConnection::OnPingTestComplete(std::vector<ENM_NET_TYPE>& testResult)
{
	LOG_FILE_CACHE(L"TokenConnection::OnPingTestComplete()");

	m_token_config.OnPingTestComplete(testResult);

	if(testResult.empty() || m_token_IpPort.GetIp() == 0)
	{
		Connect(true);
	}
}

void TokenConnection::Connect(bool firstTime)
{
	USES_CONVERSION;

	if(m_enmState == enm_state_doing)
	{
		netlayer::IpPort ipPort = GenIpPort();

		if(ipPort.GetIp() != 0)
		{
			m_token_IpPort = ipPort;

			DeleteConnection();

			struct sockaddr_in addr;
			addr.sin_addr.s_addr = m_token_IpPort.GetIp();

			LOG_FILE_CACHE(L"TokenConnection::Connect(%s) -> Connect %s", 
				firstTime ? L"1st-connect":L"re-connect", A2T(inet_ntoa(addr.sin_addr)));

			m_token_ConnectionID = m_pConnectionMgr->Create(netlayer::TCPCLIENT,
				*m_pCircleBuffer, m_token_IpPort);

			if(firstTime)
			{
				m_TimerRetry = common::utility::_Timers::instance().CreateTimer((uint32)LOGIN_RETRY_CIRCLE, 
					common::utility::_Timers::TimerCallback(
					boost::bind(&TokenConnection::OnLoginRetryTimer,this,boost::arg<1>(),boost::arg<2>())), 
					common::utility::_Timers::ENM_TIMER_LOGIN_RETRY, TOKEN_TIMEOUT/LOGIN_RETRY_CIRCLE);
			}
		}
	}
}

void TokenConnection::Clear()
{
	m_enmState = enm_state_notbegin;
	DeleteConnection();
	ClearTimer();
}

void TokenConnection::DeleteConnection()
{
	if(m_token_ConnectionID != -1)
	{
		m_pConnectionMgr->Delete(m_token_ConnectionID);
		m_token_ConnectionID = -1;
	}
}

void TokenConnection::ClearTimer()
{
	if(m_TimerRetry != -1)
	{
		common::utility::_Timers::instance().DestroyTimer(m_TimerRetry);
		m_TimerRetry = -1;
	}

	if(m_Timer != -1)
	{
		common::utility::_Timers::instance().DestroyTimer(m_Timer);
		m_Timer = -1;
	}
}

void TokenConnection::Logout()
{
	Clear();
}

void TokenConnection::TokenError2Msg(LoginRspEvent* loginRsp)
{
	switch(TokenProtocol::errorCode)
	{
	case TokenProtocol::COMMON_ERROR://通用错误
		{
			loginRsp->enmLoginResult = LOGIN_ERROR_TOKEN_ERROR;
			loginRsp->strFailReason = L"账号认证失败";
			break;
		}
	case TokenProtocol::PASSWORD_WRONG://密码错误
		{
			loginRsp->enmLoginResult = LOGIN_ERROR_TOKEN_PASSWORD_ERROR;
			loginRsp->strFailReason = L"密码错误";
			break;
		}
	case TokenProtocol::ACCOUNT_NOEXIST://用户不存在
		{
			loginRsp->enmLoginResult = LOGIN_ERROR_TOKEN_ACCOUNT_ERROR;
			loginRsp->strFailReason = L"账号错误";
			break;
		}
	case TokenProtocol::ACCOUNT_FORBIDEN://用户被禁用
		{
			loginRsp->enmLoginResult = LOGIN_ERROR_TOKEN_ACCOUNT_FORBIDE;
			loginRsp->strFailReason = L"您的账号已被锁定，如有疑问，请联系客服";
			break;
		}
	case TokenProtocol::LOGIN_REFUSED://错误次数过多
		{
			loginRsp->enmLoginResult = LOGIN_ERROR_TOKEN_REFUSED;
			loginRsp->strFailReason = TokenProtocol::errorMsg2;
			break;
		}
	case TokenProtocol::NEED_VERIFY://登陆限制，需要验证码验证
		{
			loginRsp->enmLoginResult = LOGIN_ERROR_TOKEN_NEED_VERIFY;
			break;
		}

	case TokenProtocol::VERIFY_FORMAT_ERROR:
	case TokenProtocol::VERIFY_ERROR://验证码验证失败
		{
			loginRsp->enmLoginResult = LOGIN_ERROR_TOKEN_VERIFY_ERROR;
			break;
		}

	case TokenProtocol::VERIFY_TIMEOUT://验证码过期
		{
			loginRsp->enmLoginResult = LOGIN_ERROR_TOKEN_VERIFY_TIMEOUT;
			break;
		}

	case TokenProtocol::LOGIN_INVALID_EMAIL_PASS_SUCC:
	case TokenProtocol::LOGIN_INVALID_EMAIL_PASS_FAIL:
		{
			loginRsp->enmLoginResult = LOGIN_ERROR_TOKEN_EMAIL_NOT_ACTIVATE;
			loginRsp->strFailReason = L"邮箱未激活，请激活后再登录";
			break;				
		}

	case TokenProtocol::EVERYTHING_OK://客户端定义，使用
	default:
		{
			loginRsp->enmLoginResult = LOGIN_ERROR_TOKEN_OTHER_ERROR;			
			assert(0);
			break;
		}
	}
}

void TokenConnection::OnLoginSuccess()
{
	m_Timer = common::utility::_Timers::instance().CreateTimer((uint32)CYCLE_TOKEN_TIMEER, 
		common::utility::_Timers::TimerCallback(
		boost::bind(&TokenConnection::OnCycleTokenTimer,this,boost::arg<1>(),boost::arg<2>())), 
		common::utility::_Timers::ENM_TIMER_CYCLE_TOKEN);
}

void TokenConnection::OnReceiveTokenData(ByteBuf& byteBuf)
{
	ClearTimer();

	ByteBuf result(sizeof(m_tokens), m_tokens);
	ByteBuf sessionKey(16, m_sessionKey);
	byte tempLoginedAccount[ACCOUNT_MAX_LEN] = {0};
	ByteBuf loginedAccount(ACCOUNT_MAX_LEN, tempLoginedAccount);
	byte btInnerAccount[ACCOUNT_MAX_LEN] = {0};
	ByteBuf innerAccount(ACCOUNT_MAX_LEN, btInnerAccount);

	uint32 unInnerUIN = 0;//内部uin
	uint32 unOutUIN = 0;

	TokenProtocol::TokenRspError error = TokenProtocol::Decode179TokenRsp(unOutUIN,
		m_loginEvent.strPassword,byteBuf,result,sessionKey,unInnerUIN,innerAccount);

	if(error != TokenProtocol::TOKEN_RSP_SUCCESS)
	{
		LoginRspEvent* loginRsp = new LoginRspEvent();
		TokenError2Msg(loginRsp);

		m_pCoreModule->GetModuleManager()->PushEvent(
			MakeEvent<MODULE_ID_CORE>()(EVENT_VALUE_CORE_LOGIN_RSP, MODULE_ID_HALL, 0, 0, 0, loginRsp));

		m_enmState = enm_state_fail;
	}
	else
	{
		CurInfo& curInfo = CoreModule::GetCurInfo();
		
		curInfo.unUIN = unInnerUIN;
		
		std::string strInnerAccount = (char*)innerAccount.second;
		curInfo.str179EMail = common::utility::stringhelper::UTF8ToUnicode(strInnerAccount);

		m_tokenSize = result.first;
		curInfo.strToken = common::utility::stringhelper::AsciiToUnicode(
			common::utility::stringhelper::HexEncodeCopy((char*)m_tokens, m_tokenSize));

		m_enmState = enm_state_ok;
	}
}

void TokenConnection::OnReceiveCycleTokenData(ByteBuf& byteBuf)
{
	ByteBuf result(sizeof(m_tokens), m_tokens);
	byte temp_sessionKey[17];
	ByteBuf sessionKey(16, temp_sessionKey);
	byte tempLoginedAccount[ACCOUNT_MAX_LEN] = {0};
	ByteBuf loginedAccount(ACCOUNT_MAX_LEN, tempLoginedAccount);
	byte btInnerAccount[ACCOUNT_MAX_LEN] = {0};
	ByteBuf innerAccount(ACCOUNT_MAX_LEN, btInnerAccount);

	uint32 unInnerUIN = 0;//内部uin
	uint32 unOutUIN = 0;

	TokenProtocol::TokenRspError error = TokenProtocol::Decode179TokenRsp(unOutUIN, 
		m_loginEvent.strPassword,byteBuf,result,sessionKey,unInnerUIN,innerAccount);

	if(error != TokenProtocol::TOKEN_RSP_SUCCESS)
	{
		static bool bFirst = true;
		if(bFirst)
			assert(0);
		bFirst = false;
	}
	else
	{
		CurInfo& curInfo = CoreModule::GetCurInfo();

		m_tokenSize = result.first;
		curInfo.strToken = common::utility::stringhelper::AsciiToUnicode(
			common::utility::stringhelper::HexEncodeCopy((char*)m_tokens, m_tokenSize));
	}
}

void TokenConnection::OnTokenConnected()
{
	LOG_FILE_CACHE(L"TokenConnection::OnTokenConnected()");

	ByteBuf temp;

	if(m_enmState == enm_state_ok)
	{
		if(m_loginEvent.enmAccountType == core::ACCOUNTTYPE_VISITOR)
		{
			char szuin[20]={0};
			sprintf(szuin,"%d",CoreModule::GetCurInfo().unUIN);
			std::string account = szuin;
			temp = TokenProtocol::GenTokenReq(account, m_loginEvent.strPassword, true, true);
		}
		else
		{
			temp = TokenProtocol::GenTokenReq(
				common::utility::stringhelper::UnicodeToUTF8(m_loginEvent.strAccount), 
				m_loginEvent.strPassword, true, true);
		}		
	}
	else
	{
		temp = TokenProtocol::GenTokenReq(
			common::utility::stringhelper::UnicodeToUTF8(m_loginEvent.strAccount), 
			m_loginEvent.strPassword, false, true,
			m_loginEvent.bVerify, m_loginEvent.strVerifyKey, m_loginEvent.strVerifyCode);
	}			

	netlayer::Packet packet;
	packet.SetData(temp.second+3, temp.first-3, netlayer::Packet::THREE_BYTES_HEAD);
	m_pConnectionMgr->SendTo(m_token_ConnectionID, packet, m_token_IpPort);
}

void TokenConnection::OnReceive(netlayer::MsgObj* msgObj)
{
	switch(msgObj->enMsg)
	{
	case netlayer::COMM_DATA:
		{
			LOG_TRACE(L"Connection::OnReceiveToken:COMM_DATA");

			ByteBuf buf(msgObj->oPacket.GetDataSize(), (byte*)msgObj->oPacket.GetBufR());
			if(m_enmState == enm_state_ok)
			{
				OnReceiveCycleTokenData(buf);
			}
			else if(m_enmState == enm_state_doing)
			{
				OnReceiveTokenData(buf);
			}
			else
			{
				assert(0);
			}
			
			DeleteConnection();
			break;
		}
	case netlayer::COMM_CONNECT:
		{
			LOG_TRACE(L"Connection::OnReceiveToken:COMM_CONNECT");
			OnTokenConnected();
			break;
		}
	case netlayer::COMM_ACCEPT:
		{
			assert(0);
			LOG_ERROR(L"Connection::OnReceiveToken:COMM_ACCEPT");
			break;
		}
	case netlayer::COMM_CLOSE:
		{
			LOG_ERROR(L"Connection::OnReceiveToken:COMM_CLOSE");
			LOG_FILE_CACHE(L"Connection::OnReceiveToken:COMM_CLOSE");

			if(m_enmState == enm_state_doing)
			{
				ClearTimer();

				LoginRspEvent* loginRsp = new LoginRspEvent();
				loginRsp->enmLoginResult = LOGIN_ERROR_CLIENT_TOKEN_CONNECT_ERROR;
				loginRsp->strFailReason = L"登录验证服务器失败";
				m_pCoreModule->GetModuleManager()->PushEvent(
					MakeEvent<MODULE_ID_CORE>()(EVENT_VALUE_CORE_LOGIN_RSP, MODULE_ID_HALL, 0, 0, 0, loginRsp));

				m_enmState = enm_state_fail;
			}
			else
			{
				assert(0);
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

void TokenConnection::OnLoginTimeout(uintptr,bool)
{
	if(m_TimerRetry != -1)
	{
		common::utility::_Timers::instance().DestroyTimer(m_TimerRetry);
		m_TimerRetry = -1;
	}

	LOG_FILE_CACHE(L"TokenConnection::OnLoginTimeout()");

	LOG_FLUSH_CACHE;

	m_enmState = enm_state_fail;

	LoginRspEvent* loginRsp = new LoginRspEvent();
	loginRsp->enmLoginResult = LOGIN_ERROR_CLIENT_TOKEN_TIMEOUT_ERROR;
	loginRsp->strFailReason = L"登录验证服务器超时";
	m_pCoreModule->GetModuleManager()->PushEvent(
		MakeEvent<MODULE_ID_CORE>()(EVENT_VALUE_CORE_LOGIN_RSP, MODULE_ID_HALL, 0, 0, 0, loginRsp));
}

void TokenConnection::OnCycleTokenTimer(uintptr,bool)
{
	DeleteConnection();

	m_token_ConnectionID = m_pConnectionMgr->Create(netlayer::TCPCLIENT,
		*m_pCircleBuffer, m_token_IpPort);
}

void TokenConnection::OnLoginRetryTimer(uintptr,bool)
{
	DeleteConnection();

	Connect(false);
}