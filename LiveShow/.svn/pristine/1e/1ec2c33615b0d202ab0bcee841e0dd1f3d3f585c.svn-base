/**
* ConnectionBase.cpp
* 
*
* @version 1.0 (06 ,04, 2008)
* @author reinhardt ken
*
*/
#pragma  once
#pragma warning(disable : 4786)
#include "ConnectionBase.h"
#include "NetHelper.h"
#include "InnerUtility.h"
#include "ConnectionLayerImpl.h"

namespace netlayer
{

ConnectionBase::ConnectionBase(ConnectionLayerImpl& connectionMgr) 
: m_pConnectionMgr(&connectionMgr)
{
}

ConnectionBase::~ConnectionBase() 
{
}

bool ConnectionBase::Create(const IpPort& oAddr) 
{
	return false;
}

bool ConnectionBase::Create(const IpPort& oAddr, 
					   const IpPort& oProxyAddr, 
					   const InnerUtility::String& strName, 
					   const InnerUtility::String& strPass,
					   const InnerUtility::String& strDomain) 
{
	return false;
}

bool ConnectionBase::Close() 
{
	return false;
}

bool ConnectionBase::SendTo(const IpPort& oAddr, Packet& oPacket) 
{
	return false;
}

void ConnectionBase::OnRead(SOCKET oSock) 
{
}

void ConnectionBase::OnWrite(SOCKET oSock) 
{
}

void ConnectionBase::OnExcept(SOCKET oSock) 
{
}

bool ConnectionBase::Notify(MsgObj * pMsg)
{
	pMsg->uiID = reinterpret_cast<unsigned int>(this);
	if (m_pConnectionMgr->Notify(pMsg, 0))
	{
		m_pConnectionMgr->CommitMsgObj(pMsg->uiID);
		return true;
	}
	return false;
}

bool ConnectionBase::Notify(COMM_MSG enMsg, TYPE enType, ConnectionBase * pComm)
{
	MsgObj * pMsg = AllocMsgObj();
	pMsg->enMsg = enMsg;
	pMsg->uiID = reinterpret_cast<unsigned int>(this);
	pMsg->bTcp = enType == TCP_COMM;
	pMsg->oIpPort.SetIp(0);
	pMsg->oIpPort.SetPort(0);
	if (m_pConnectionMgr->Notify(pMsg, pComm))
	{
		m_pConnectionMgr->CommitMsgObj(pMsg->uiID);
		return true;
	}
	return false;
}

bool ConnectionBase::AddToRead(SOCKET oSock, ConnectionBase * pComm)
{
	return m_pConnectionMgr->AddToRead(oSock, pComm);
}

bool ConnectionBase::AddToWrite(SOCKET oSock, ConnectionBase * pComm)
{
	return m_pConnectionMgr->AddToWrite(oSock, pComm);
}

bool ConnectionBase::AddToExcept(SOCKET oSock, ConnectionBase * pComm)
{
	return m_pConnectionMgr->AddToExcept(oSock, pComm);
}

bool ConnectionBase::RemoveFromRead(SOCKET oSock, ConnectionBase * pComm)
{
	return m_pConnectionMgr->RemoveFromRead(oSock, pComm);
}

bool ConnectionBase::RemoveFromWrite(SOCKET oSock, ConnectionBase * pComm)
{
	return m_pConnectionMgr->RemoveFromWrite(oSock, pComm);
}

bool ConnectionBase::RemoveFromExcept(SOCKET oSock, ConnectionBase * pComm)
{
	return m_pConnectionMgr->RemoveFromExcept(oSock, pComm);
}

//获取临时的BUF
MsgObj * ConnectionBase::AllocMsgObj()
{
	return m_pConnectionMgr->AllocMsgObj(reinterpret_cast<unsigned int>(this));
}

int ConnectionBase::GetSockName()
{
	return 0;
}

}; //end namespace netlayer
