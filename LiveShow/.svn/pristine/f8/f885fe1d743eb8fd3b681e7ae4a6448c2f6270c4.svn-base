/**
* TwoByteTcpConnectionImpl.h
* 
*
* @version 1.0 (06 ,04, 2008)
* @author reinhardt ken
*
*/
#pragma  once

#include "ConnectionBase.h"
#define SECURITY_WIN32
#include <security.h>

namespace netlayer
{
	class ConnectionLayerImpl;

	class TcpConnectionImpl : public ConnectionBase  
	{
	public:
		TcpConnectionImpl(ConnectionLayerImpl& oCommMgr, Packet::HEADTYPE enmRecvHeadType, SOCKET oSock = INVALID_SOCKET);
		virtual ~TcpConnectionImpl();

		virtual bool Create(const IpPort& oAddr);
		virtual bool Close();
		virtual bool SendTo(const IpPort& oAddr, Packet& oPacket);
		virtual void OnRead(SOCKET oSock);
		virtual void OnExcept(SOCKET oSock);
		virtual void OnWrite(SOCKET oSock);
		int GetSockName();

	protected:
		int RecvAll(InnerUtility::BUF& oBuf, bool bTcp = true);

		void OnTwoByteRead(SOCKET oSock);
		void OnThreeByteRead(SOCKET oSock);
		void OnFourByteRead(SOCKET oSock);

	protected:
		enum STATE {
			CLOSED,
			CONNECTING,
			CONNECTED,
			HTTPAUTH, //HTTP认证
			HTTPNTLMAUTH1, //HTTP认证 step1(ntlm)
			HTTPNTLMAUTH2, //HTTP认证 step2(ntlm)
			SOCK4AUTH, //sock4认证
			SOCK5VERSION, // sock5发送版本
			SOCK5AUTH, //将要进行sock5认证
			SOCK5AUTHSENT, //sock5认证发送完毕
			SOCK5AUTHOK, //sock5认证完成
			SOCK5IP,//发送ip等信息
			SOCK5SETCONTYPE, //sock5设置连接类型
		};


	protected:
		Packet::HEADTYPE m_enmRecvHeadType;//接受tcp的包头
		SOCKET m_oSock;
		STATE m_enState;
		InnerUtility::BUF m_oSendData; //发送缓冲区
		InnerUtility::BUF m_oRecvData; //接受缓冲区
	};

};