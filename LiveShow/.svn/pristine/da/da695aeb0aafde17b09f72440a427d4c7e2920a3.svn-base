/**
* UdpConnectionImpl.h
* 
*
* @version 1.0 (06 ,04, 2008)
* @author reinhardt ken
*
*/
#pragma  once

#include "ConnectionBase.h"
#include "..\..\include\Net\ConnectionCommon.h"

namespace netlayer
{
class ConnectionMgr;
class UdpConnectionImpl : public ConnectionBase  
{
public:
	UdpConnectionImpl(ConnectionLayerImpl& oCommMgr);
	virtual ~UdpConnectionImpl();

	virtual bool Create(const IpPort& oAddr);
	virtual bool Close();
	virtual bool SendTo(const IpPort& oAddr, Packet& oPacket);
	virtual void OnRead(SOCKET oSock);
	virtual void OnWrite(SOCKET oSock);
	virtual int GetSockName();

private:
	SOCKET m_oSock;
	sockaddr_in m_oAddr;
}; //end class UdpConnectionImpl
}; 