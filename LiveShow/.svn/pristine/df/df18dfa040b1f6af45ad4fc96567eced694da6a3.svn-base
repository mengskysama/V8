/**
* ConnectionBase.h
* 
*
* @version 1.0 (06 ,04, 2008)
* @author reinhardt ken
*
*/
#pragma  once

#include "NetHelper.h"
#include "InnerUtility.h"
#include "..\..\include\Net\ConnectionCommon.h"

namespace netlayer
{
class ConnectionLayerImpl;

class ConnectionBase  
{
protected:
	enum TYPE { UDP_COMM, TCP_COMM };

public:
	ConnectionBase(ConnectionLayerImpl& oCommMgr);
	virtual ~ConnectionBase();
	virtual bool Create(const IpPort& oAddr);
	virtual bool Create(const IpPort& oAddr, const IpPort& oProxyAddr, 
						const InnerUtility::String& strName, 
						const InnerUtility::String& strPass,
						const InnerUtility::String& strDomain = "");
	virtual bool Close();
	virtual bool SendTo(const IpPort& oAddr, Packet& oPacket);
	virtual void OnRead(SOCKET oSock);
	virtual void OnWrite(SOCKET oSock);
	virtual void OnExcept(SOCKET oSock);
	virtual int GetSockName();

protected:
	//回调消息
	bool Notify(MsgObj * pMsg);
	bool Notify(COMM_MSG enMsg, TYPE enType, ConnectionBase * pComm = 0);
	//设置select set
	bool AddToRead(SOCKET oSock, ConnectionBase * pComm);
	bool AddToWrite(SOCKET oSock, ConnectionBase * pComm);
	bool AddToExcept(SOCKET oSock, ConnectionBase * pComm);
	bool RemoveFromRead(SOCKET oSock, ConnectionBase * pComm);
	bool RemoveFromWrite(SOCKET oSock, ConnectionBase * pComm);
	bool RemoveFromExcept(SOCKET oSock, ConnectionBase * pComm);
	//获取临时的BUF
	MsgObj * AllocMsgObj();

protected:
	ConnectionLayerImpl * m_pConnectionMgr;
}; //end class ConnectionBase

}; 