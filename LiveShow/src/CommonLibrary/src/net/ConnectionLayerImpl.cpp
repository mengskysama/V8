/**
* ConnectionLayerImpl.cpp
* 
*
* @version 1.0 (06 ,04, 2008)
* @author reinhardt ken
*
*/
#pragma  once
#pragma warning(disable : 4786)
#pragma warning(disable : 4355)
#include "UdpConnectionImpl.h"
#include "HttpProxy.h"
#include "Sock5Proxy.h"
#include "ConnectionLayerImpl.h"
#include <cassert>
#include "TcpConnectionImpl.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

#pragma comment(lib,"Ws2_32.lib")

namespace netlayer
{
ConnectionLayerImpl::ConnectionLayerImpl()
: m_oThread(THREAD_FUNC(*this, MADDR(ConnectionLayerImpl, Process))),
	m_bNeedStop(false),
	m_bChangeEvents(false),
	m_iMaxFd(0)
{
	FD_ZERO(&m_oReadSet);
	FD_ZERO(&m_oWriteSet);
	FD_ZERO(&m_oExceptSet);

	WORD wVersionRequested;
	WSADATA wsaData;
	wVersionRequested =MAKEWORD(2,2);
	WSAStartup( wVersionRequested, &wsaData);

	m_oThread.Start(0);
}

ConnectionLayerImpl::~ConnectionLayerImpl()
{
	m_bNeedStop = true;
	m_oThread.Stop();

	WSACleanup();

}

bool ConnectionLayerImpl::AddToRead(SOCKET oSock, ConnectionBase * pComm)
{
	assert(pComm);
	SCOPED_LOCK oLock(m_oLockEvents);
	m_oReadEvents[oSock] = pComm;
	m_bChangeEvents = true;
	return true;
}

bool ConnectionLayerImpl::AddToWrite(SOCKET oSock, ConnectionBase * pComm)
{
	assert(pComm);
	SCOPED_LOCK oLock(m_oLockEvents);
	m_oWriteEvents[oSock] = pComm;
	m_bChangeEvents = true;
	return true;
}

bool ConnectionLayerImpl::AddToExcept(SOCKET oSock, ConnectionBase * pComm)
{
	assert(pComm);
	SCOPED_LOCK oLock(m_oLockEvents);
	m_oExceptEvents[oSock] = pComm;
	m_bChangeEvents = true;
	return true;
}

bool ConnectionLayerImpl::RemoveFromRead(SOCKET oSock, ConnectionBase * pComm)
{
	SCOPED_LOCK oLock(m_oLockEvents);
	m_oReadEvents.erase(oSock);
	m_bChangeEvents = true;
	return true;
}

bool ConnectionLayerImpl::RemoveFromWrite(SOCKET oSock, ConnectionBase * pComm)
{
	SCOPED_LOCK oLock(m_oLockEvents);
	m_oWriteEvents.erase(oSock);
	m_bChangeEvents = true;
	return true;
}

bool ConnectionLayerImpl::RemoveFromExcept(SOCKET oSock, ConnectionBase * pComm)
{
	SCOPED_LOCK oLock(m_oLockEvents);
	m_oExceptEvents.erase(oSock);
	m_bChangeEvents = true;
	return true;
}

int ConnectionLayerImpl::SetSelectSet(fd_set& oRead, fd_set& oWrite, fd_set& oExcept)
{
	if (!m_bChangeEvents) 
	{
		oRead = m_oReadSet;
		oWrite = m_oWriteSet;
		oExcept = m_oExceptSet;
		return m_oReadEvents.size() + m_oWriteEvents.size() + m_oWriteEvents.size();
	}
	SCOPED_LOCK oLock(m_oLockEvents);
	m_iMaxFd = 0;
	m_bChangeEvents = false;
	FD_ZERO(&oRead);
	MAP_EVENTS::iterator it = m_oReadEvents.begin();
	MAP_EVENTS::iterator itEnd = m_oReadEvents.end();
	for (; it != itEnd; ++it)
	{
		if (m_iMaxFd < it->first) m_iMaxFd = it->first;
		FD_SET(it->first, &oRead);
	}
	FD_ZERO(&oWrite);
	itEnd = m_oWriteEvents.end();
	for (it = m_oWriteEvents.begin(); it != itEnd; ++it)
	{
		if (m_iMaxFd < it->first) m_iMaxFd = it->first;
		FD_SET(it->first, &oWrite);
	}
	FD_ZERO(&oExcept);
	itEnd = m_oWriteEvents.end();
	for (it = m_oWriteEvents.begin(); it != itEnd; ++it)
	{
		if (m_iMaxFd < it->first) m_iMaxFd = it->first;
		FD_SET(it->first, &oWrite);
	}
	m_oReadSet = oRead;
	m_oWriteSet = oWrite;
	m_oExceptSet = oExcept;
	return m_oReadEvents.size() + m_oWriteEvents.size() + m_oWriteEvents.size();
}

//触发通讯模块
void ConnectionLayerImpl::Trigger(fd_set& oRead, 
							   fd_set& oWrite, 
							   fd_set& oExcept)
{
	m_oTmpReadPtrs.clear();
	m_oTmpWritePtrs.clear();
	m_oTmpExceptPtrs.clear();
	{
		SCOPED_LOCK oLock(m_oLockEvents);
		MAP_EVENTS::iterator it = m_oReadEvents.begin();
		MAP_EVENTS::iterator itEnd = m_oReadEvents.end();
		for (; it != itEnd; ++it)
		{
			if (FD_ISSET(it->first, &oRead))
			{
				m_oTmpReadPtrs.push_back(PTRSOCKET(it->second, it->first));
			}
		}
		itEnd = m_oWriteEvents.end();
		for (it = m_oWriteEvents.begin(); it != itEnd; ++it)
		{
			if (FD_ISSET(it->first, &oWrite))
			{
				m_oTmpWritePtrs.push_back(PTRSOCKET(it->second, it->first));
			}
		}
		itEnd = m_oExceptEvents.end();
		for (it = m_oExceptEvents.begin(); it != itEnd; ++it)
		{
			if (FD_ISSET(it->first, &oExcept))
			{
				m_oTmpExceptPtrs.push_back(PTRSOCKET(it->second, it->first));
			}
		}
	}
	SCOPED_LOCK oLockComm(m_oLockComms);
	COMMPTRS::iterator it = m_oTmpReadPtrs.begin();
	COMMPTRS::iterator itEnd = m_oTmpReadPtrs.end();
	MAP_COMMS::iterator itComm;
	for (; it != itEnd; ++it)
	{
		itComm = m_oComms.find(reinterpret_cast<unsigned int>(it->first));
		if (itComm != m_oComms.end())
		{
			//_LOG(F()<<boost::wformat(L"受到数据（it->second,%d）")%itComm->second.get()<<OVER);
			itComm->second->OnRead(it->second);
		}
	}
	itEnd = m_oTmpWritePtrs.end();
	for (it = m_oTmpWritePtrs.begin(); it != itEnd; ++it)
	{
		itComm = m_oComms.find(reinterpret_cast<unsigned int>(it->first));
		if (itComm != m_oComms.end())
		{
			itComm->second->OnWrite(it->second);
		}
	}
	itEnd = m_oTmpExceptPtrs.end();
	for (it = m_oTmpExceptPtrs.begin(); it != itEnd; ++it)
	{
		itComm = m_oComms.find(reinterpret_cast<unsigned int>(it->first));
		if (itComm != m_oComms.end())
		{
			itComm->second->OnExcept(it->second);
		}
	}
}

//线程函数
void ConnectionLayerImpl::Process(void *)
{
	fd_set oRead;
	fd_set oWrite;
	fd_set oExcept;
	timeval oTime = {0, 10000}; //10ms
	
	while (!m_bNeedStop)
	{
		if (SetSelectSet(oRead, oWrite, oExcept) > 0)
		{
			oTime.tv_sec = 0;
			oTime.tv_usec = 10000;
			int iCount = select(m_iMaxFd, &oRead, &oWrite, &oExcept, &oTime);
			if (iCount > 0) //有事件触发
			{
				Trigger(oRead, oWrite, oExcept);
			}
		}
		else
		{
			Sleep(1);
		}
	}
}

//创建一个通讯实例
unsigned int ConnectionLayerImpl::Create(COMM_TYPE enType,
								MSG_BUF& oBuf,
								const IpPort& oAddr, 
								const IpPort& oProxyAddr, 
								const std::string& strName, 
								const std::string& strPass,
								const std::string& strDomain)
{
	SPCOMM spComm;
	switch (enType)
	{
	case UDP:
		spComm.reset(new UdpConnectionImpl(*this));
		if (!spComm->Create(oAddr))
		{
			spComm.reset(0);
		}
		break;
	case TCPCLIENT:
		spComm.reset(new TcpConnectionImpl(*this, Packet::THREE_BYTES_HEAD));
		if (!spComm->Create(oAddr))
		{
			spComm.reset(0);
		}
		break;
	case TCPCLIENT2:
		spComm.reset(new TcpConnectionImpl(*this, Packet::TWO_BYTES_HEAD));
		if (!spComm->Create(oAddr))
		{
			spComm.reset(0);
		}
		break;
	case TCPCLIENT4:
		spComm.reset(new TcpConnectionImpl(*this, Packet::FOUR_BYTES_HEAD));
		if (!spComm->Create(oAddr))
		{
			spComm.reset(0);
		}
		break;
	case TCPSERVER:
// 		spComm.reset(new TcpServer(*this));
// 		if (!spComm->Create(oAddr))
// 		{
// 			spComm.reset(0);
// 		}
		break;

	case HTTPPROXY:
		spComm.reset(new HttpProxy<TcpConnectionImpl>(*this, Packet::THREE_BYTES_HEAD));
		if (!spComm->Create(oAddr, oProxyAddr, strName, strPass, strDomain))
		{
			spComm.reset(0);
		}
		break;
	case HTTPPROXY2:
		spComm.reset(new HttpProxy<TcpConnectionImpl>(*this, Packet::TWO_BYTES_HEAD));
		if (!spComm->Create(oAddr, oProxyAddr, strName, strPass, strDomain))
		{
			spComm.reset(0);
		}
		break;
	case HTTPPROXY4:
		spComm.reset(new HttpProxy<TcpConnectionImpl>(*this, Packet::FOUR_BYTES_HEAD));
		if (!spComm->Create(oAddr, oProxyAddr, strName, strPass, strDomain))
		{
			spComm.reset(0);
		}
		break;
	case SOCK5PROXY:
		spComm.reset(new Sock5Proxy<TcpConnectionImpl>(*this, Packet::THREE_BYTES_HEAD));
		if (!spComm->Create(oAddr, oProxyAddr, strName, strPass))
		{
			spComm.reset(0);
		}
		break;
	case SOCK5PROXY2:
		spComm.reset(new Sock5Proxy<TcpConnectionImpl>(*this, Packet::TWO_BYTES_HEAD));
		if (!spComm->Create(oAddr, oProxyAddr, strName, strPass))
		{
			spComm.reset(0);
		}
		break;
	case SOCK5PROXY4:
		spComm.reset(new Sock5Proxy<TcpConnectionImpl>(*this, Packet::FOUR_BYTES_HEAD));
		if (!spComm->Create(oAddr, oProxyAddr, strName, strPass))
		{
			spComm.reset(0);
		}
		break;
	}
	unsigned int uiID = reinterpret_cast<unsigned int>(spComm.get());
	if (uiID != 0)
	{
		SCOPED_LOCK oLock(m_oLockComms);
		m_oComms.insert(MAP_COMMS::value_type(uiID, spComm));

		m_mapCycleBuf[uiID] = &oBuf;
		if(m_mapMsgCahce.find(uiID) != m_mapMsgCahce.end())
		{
			assert(0);
		}

		m_mapMsgCahce.insert(std::map<int, MSG_CACHE>::value_type(uiID, MSG_CACHE()));
		m_mapMsgCahce[uiID].resize(oBuf.Size() + 2);
	}
	return uiID;
}

//删除一个Connection实例
void ConnectionLayerImpl::Delete(unsigned int uiID)
{
	SCOPED_LOCK oLock(m_oLockComms);
	m_oComms.erase(uiID);
	m_mapCycleBuf.erase(uiID);
	m_mapMsgCahce.erase(uiID);	
}

//发送数据
bool ConnectionLayerImpl::SendTo(unsigned int uiID, Packet& oPacket, const IpPort& oAddr)
{
	SCOPED_LOCK oLock(m_oLockComms);
	MAP_COMMS::iterator it = m_oComms.find(uiID);
	if (it != m_oComms.end())
	{
		return it->second->SendTo(oAddr, oPacket);
	}
	return false;
}

//获取临时BUF
MsgObj * ConnectionLayerImpl::AllocMsgObj(int nID)
{
	std::map<int, MSG_CACHE>::iterator it = m_mapMsgCahce.find(nID);

	if(it != m_mapMsgCahce.end())
	{
		return it->second.Get();
	}

	return NULL;
}

//提交临时BUF
void ConnectionLayerImpl::CommitMsgObj(int nID)
{
	std::map<int, MSG_CACHE>::iterator it = m_mapMsgCahce.find(nID);

	if(it != m_mapMsgCahce.end())
	{
		++it->second;
	}
}

//回调消息
bool ConnectionLayerImpl::Notify(MsgObj * pMsg, ConnectionBase * pComm)
{
	bool bRet = false;
	if (pMsg->enMsg == COMM_ACCEPT && pComm)
	{
		SPCOMM spComm(pComm);
		pMsg->uiID = reinterpret_cast<unsigned int>(pComm);
		//所有的notify都是在OnRead, OnWrite, OnExcept中触发，所以也在m_oLockComms保护下
		m_oComms.insert(MAP_COMMS::value_type(pMsg->uiID, spComm));
		if(m_mapCycleBuf.find(pMsg->uiID) != m_mapCycleBuf.end() && m_mapCycleBuf[pMsg->uiID]->Write(pMsg))
		{
			bRet = true;
		}
		else
		{
			m_oComms.erase(pMsg->uiID);
			bRet = false;
		}
	}
	else
	{
		if(m_mapCycleBuf.find(pMsg->uiID) != m_mapCycleBuf.end())
		{
			bRet = m_mapCycleBuf[pMsg->uiID]->Write(pMsg);
		}		
	}
	if (!bRet) //应用层来不及处理
	{
		Sleep(1);
	}
	return bRet;
}


int  ConnectionLayerImpl::GetSockName(unsigned int uiID)
{
	SCOPED_LOCK oLock(m_oLockComms);
	MAP_COMMS::iterator it = m_oComms.find(uiID);
	if (it != m_oComms.end())
	{
		return it->second->GetSockName();
	}
	return 0;
}

}; //end namespace netlayer
