/**
* UdpConnectionImpl.cpp
* 
*
* @version 1.0 (06 ,04, 2008)
* @author reinhardt ken
*
*/

#pragma warning(disable : 4786)
#include "UdpConnectionImpl.h"
#include "ConnectionLayerImpl.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
namespace netlayer
{

UdpConnectionImpl::UdpConnectionImpl(ConnectionLayerImpl& oCommMgr)
: ConnectionBase(oCommMgr), m_oSock(INVALID_SOCKET)
{
}

UdpConnectionImpl::~UdpConnectionImpl()
{
	Close();
}

//创建
bool UdpConnectionImpl::Create(const IpPort& oAddr)
{
	if (m_oSock != INVALID_SOCKET)
	{
		Close();
	}
	m_oSock = nethelper::CreateUdpSocket(oAddr.GetIp(), oAddr.GetPort(), true, false);
	if (m_oSock != INVALID_SOCKET)
	{
		AddToRead(m_oSock, this);
		AddToWrite(m_oSock, this);
		return true;
	}
	return false;
}

//关闭
bool UdpConnectionImpl::Close()
{
	RemoveFromRead(m_oSock, this);
	nethelper::CloseSocket(m_oSock);
	m_oSock = INVALID_SOCKET;
	return true;
}

//发送数据
bool UdpConnectionImpl::SendTo(const IpPort& oAddr, Packet& oPacket)
{
	if (m_oSock != INVALID_SOCKET)
	{
		unsigned int uiBytes = oPacket.GetUdpSize();
		oPacket.ToNetOrder();
		return uiBytes == nethelper::SendTo(m_oSock, 
									oAddr.GetIp(), 
									oAddr.GetPort(), 
									oPacket.GetBufR()+Packet::TWO_BYTES_HEAD, 
									uiBytes);
	}
	else
	{
		return false;
	}
}

//有数据过来
void UdpConnectionImpl::OnRead(SOCKET oSock)
{
	int iReadBytes;
	while (true)
	{
		MsgObj * pMsg = AllocMsgObj();
		iReadBytes = nethelper::RecvFrom(m_oSock, 
										pMsg->oPacket.GetBufW(), 
										pMsg->oPacket.GetBufSize(), 
										m_oAddr);
		pMsg->oPacket.SetFlag(0x00) ;
		pMsg->oPacket.SetDataSize(htons(iReadBytes+2));
		if (iReadBytes > 0)
		{
			pMsg->oPacket.ToHostOrder();
			pMsg->oIpPort.SetIp(m_oAddr.sin_addr.S_un.S_addr);
			
			pMsg->oIpPort.SetPort(m_oAddr.sin_port);
			pMsg->enMsg = COMM_DATA;
			pMsg->bTcp = false;
			Notify(pMsg);
		}
		else
		{
			break;
		}
	}
}

void UdpConnectionImpl::OnWrite(SOCKET oSock)
{
	RemoveFromWrite(m_oSock, this);
	Notify(COMM_CONNECT, UDP_COMM);
}


int UdpConnectionImpl::GetSockName()
{
	return m_oSock;
#if 0
	int len = sizeof(struct sockaddr_in);
	struct sockaddr_in localAddr;
	localAddr.sin_family = AF_INET;
	if( ::getsockname(m_oSock, (sockaddr *)&localAddr, &len) != 0 )
	{
		//int re=re=WSAGetLastError();
		return 0;
	}

	return localAddr.sin_addr.s_addr;
#endif
}

}; //end namespace netlayer
