/**
* TwoByteTcpConnectionImpl.cpp
* 
*
* @version 1.0 (06 ,04, 2008)
* @author reinhardt ken
*
*/
#pragma  once
#pragma warning(disable : 4786)
#include "TcpConnectionImpl.h"
#include "ConnectionLayerImpl.h"

#define SOCKET_BUFFER_LEN 64 * 1024

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
namespace netlayer
{

	TcpConnectionImpl::TcpConnectionImpl(ConnectionLayerImpl& oCommMgr, Packet::HEADTYPE enmRecvHeadType, SOCKET oSock)
		: ConnectionBase(oCommMgr), m_oSock(oSock), m_enState(CLOSED), m_enmRecvHeadType(enmRecvHeadType)
	{
		if (oSock != INVALID_SOCKET)
		{
			nethelper::SetNonBlock(oSock);
			nethelper::SetSocketBuf(oSock, SOCKET_BUFFER_LEN);
			m_enState = CONNECTED;
			AddToRead(m_oSock, this);
		}
	}

	TcpConnectionImpl::~TcpConnectionImpl()
	{
		Close();
	}

	bool TcpConnectionImpl::Create(const IpPort& oAddr)
	{
		m_oSock = nethelper::CreateSocket(false) ;
		if (nethelper::SetNonBlock(m_oSock))
		{
			sockaddr_in oDstAdr = nethelper::IpPortToAddr(oAddr.GetIp(), oAddr.GetPort()) ;
			connect(m_oSock, reinterpret_cast<const struct sockaddr *>(&oDstAdr), sizeof(oDstAdr));
			//int err=WSAGetLastError();
			m_enState = CONNECTING;
			AddToWrite(m_oSock, this);
			AddToExcept(m_oSock, this);
			return true;
		}
		else
		{
			nethelper::CloseSocket(m_oSock);
			m_oSock = INVALID_SOCKET;
			return false;
		}
	}

	bool TcpConnectionImpl::Close()
	{
		RemoveFromRead(m_oSock, this);
		RemoveFromWrite(m_oSock, this);
		RemoveFromExcept(m_oSock, this);
		nethelper::CloseSocket(m_oSock);
		m_oSock = INVALID_SOCKET;
		m_oRecvData.resize(0);
		m_oSendData.resize(0);
		m_enState = CLOSED;
		return true;
	}

	bool TcpConnectionImpl::SendTo(const IpPort& oAddr, Packet& oPacket)
	{
		if (CONNECTED == m_enState && m_oSendData.size() == 0)
		{
			int iBytes = oPacket.GetDataSize() /*+ sizeof(m_oPacket.oHeader)*/;
			oPacket.ToNetOrder();
			int iSendBytes = nethelper::Send(m_oSock, oPacket.GetBufR(), iBytes);
			if (iSendBytes == iBytes)
			{
				return true;
			}
			else if (iSendBytes < iBytes && iSendBytes > 0) 
			{
				m_oSendData.insert(m_oSendData.end(), 
					(unsigned char *)oPacket.GetBufR() + iSendBytes,
					(unsigned char *)oPacket.GetBufR() + iBytes);
				AddToWrite(m_oSock, this);
				return true;
			}
		}
		return false;
	}

	void TcpConnectionImpl::OnTwoByteRead(SOCKET oSock)
	{
		Header * pHeader = 0;
		RecvAll(m_oRecvData);
		bool bNeedFindNext = false;
		while (m_oRecvData.size() >= Packet::TWO_BYTES_HEAD)
		{
			bNeedFindNext = true;
			pHeader = reinterpret_cast<Header *>(reinterpret_cast<char *>(&m_oRecvData[0])-1);
			pHeader->ToHostOrder();
			if (pHeader->usShortSize > Packet::TWO_BYTES_HEAD)
			{
				if (m_oRecvData.size() >= pHeader->usShortSize)
				{
					MsgObj * pMsg = AllocMsgObj();
					pMsg->enMsg = COMM_DATA;
					pMsg->bTcp = true;
					pMsg->oPacket.resize(pHeader->usShortSize-Packet::TWO_BYTES_HEAD);
					pMsg->oPacket.SetDataSize(pHeader->usShortSize);
					memcpy(pMsg->oPacket.GetBufW(), (char*)pHeader+sizeof(pHeader->ucFlag)+sizeof(pHeader->usShortSize), pHeader->usShortSize-Packet::TWO_BYTES_HEAD);
					Notify(pMsg);
					m_oRecvData.erase(m_oRecvData.begin(), m_oRecvData.begin() + pHeader->usShortSize /*+ sizeof(Header)*/);
					bNeedFindNext = false;
				}
				else
				{
					// 2008 10 23
					//bug 1549 添加如下语句。。。。低级啊
					pHeader->ToHostOrder();
					break;
				}
			}
			if (bNeedFindNext) //因为不是文本协议，查找下一个没有任何意义，应该断开连接
			{
				MsgObj * pMsg = AllocMsgObj();
				pMsg->enMsg = COMM_ERROR;
				pMsg->bTcp = true;
				Notify(pMsg);
				Close();//不加死锁。。。
				// 			InnerUtility::BUF::iterator it = std::find(m_oRecvData.begin(), m_oRecvData.end(), COMM_FLAG);
				// 			m_oRecvData.erase(m_oRecvData.begin(), it);
			}
		}
	}

	void TcpConnectionImpl::OnThreeByteRead(SOCKET oSock)
	{
		Header * pHeader = 0;
		RecvAll(m_oRecvData);
		bool bNeedFindNext = false;
		while (m_oRecvData.size() >= Packet::THREE_BYTES_HEAD)
		{
			bNeedFindNext = true;
			pHeader = reinterpret_cast<Header*>(&m_oRecvData[0]);
			pHeader->ToHostOrder();
			if (COMM_FLAG == pHeader->ucFlag && pHeader->usShortSize > 2)
			{
				if (m_oRecvData.size() >= pHeader->usShortSize)
				{
					MsgObj * pMsg = AllocMsgObj();
					pMsg->enMsg = COMM_DATA;
					pMsg->bTcp = true;
					pMsg->oPacket.resize(pHeader->usShortSize);
					pMsg->oPacket.SetFlag(pHeader->ucFlag);
					pMsg->oPacket.SetDataSize(pHeader->usShortSize);
					memcpy(pMsg->oPacket.GetBufW(), (char*)pHeader, pHeader->usShortSize);
					Notify(pMsg);
					m_oRecvData.erase(m_oRecvData.begin(), m_oRecvData.begin() + pHeader->usShortSize /*+ sizeof(Header)*/);
					bNeedFindNext = false;
				}
				else
				{
					// 2008 10 23
					//bug 1549 添加如下语句。。。。低级啊
					pHeader->ToHostOrder();
					break;
				}
			}
			if (bNeedFindNext) //因为不是文本协议，查找下一个没有任何意义，应该断开连接
			{
				MsgObj * pMsg = AllocMsgObj();
				pMsg->enMsg = COMM_ERROR;
				pMsg->bTcp = true;
				Notify(pMsg);
				Close();//不加死锁。。。

				// 			InnerUtility::BUF::iterator it = std::find(m_oRecvData.begin(), m_oRecvData.end(), COMM_FLAG);
				// 			m_oRecvData.erase(m_oRecvData.begin(), it);
			}
		}
	}

	void TcpConnectionImpl::OnFourByteRead(SOCKET oSock)
	{
		Header * pHeader = 0;
		RecvAll(m_oRecvData);
		bool bNeedFindNext = false;
		while (m_oRecvData.size() >= Packet::FOUR_BYTES_HEAD)
		{
			bNeedFindNext = true;
			pHeader = reinterpret_cast<Header*>(&m_oRecvData[0]-1);
			pHeader->ToIntHostOrder();
			
			if (m_oRecvData.size() >= pHeader->unIntSize)
			{
				wchar_t sz2[120] = {0};
				wsprintf(sz2, L"plushu size:%d", pHeader->unIntSize);
				//OutputDebugString(sz2);

				MsgObj * pMsg = AllocMsgObj();
				pMsg->enMsg = COMM_DATA;
				pMsg->bTcp = true;
				pMsg->oPacket.resize(pHeader->unIntSize);
				pMsg->oPacket.SetFlag(pHeader->ucFlag);
				pMsg->oPacket.SetIntDataSize(pHeader->unIntSize);
				memcpy(pMsg->oPacket.GetBufW(), (char*)pHeader+sizeof(Header), pHeader->unIntSize);
				Notify(pMsg);
				m_oRecvData.erase(m_oRecvData.begin(), m_oRecvData.begin() + pHeader->unIntSize /*+ sizeof(Header)*/);
				bNeedFindNext = false;
			}
			else
			{
				// 2008 10 23
				//bug 1549 添加如下语句。。。。低级啊
				pHeader->ToIntHostOrder();
				break;
			}
		
			if (bNeedFindNext) //因为不是文本协议，查找下一个没有任何意义，应该断开连接
			{
				MsgObj * pMsg = AllocMsgObj();
				pMsg->enMsg = COMM_ERROR;
				pMsg->bTcp = true;
				Notify(pMsg);
				Close();//不加死锁。。。

				// 			InnerUtility::BUF::iterator it = std::find(m_oRecvData.begin(), m_oRecvData.end(), COMM_FLAG);
				// 			m_oRecvData.erase(m_oRecvData.begin(), it);
			}
		}
	}

	void TcpConnectionImpl::OnRead(SOCKET oSock)
	{
		if(m_enmRecvHeadType == Packet::TWO_BYTES_HEAD)
		{
			OnTwoByteRead(oSock);
		}
		else if(m_enmRecvHeadType == Packet::THREE_BYTES_HEAD)
		{
			OnThreeByteRead(oSock);
		}
		else if(m_enmRecvHeadType == Packet::FOUR_BYTES_HEAD)
		{
			OnFourByteRead(oSock);
		}
		else
		{
			assert(0);
		}
	}

	void TcpConnectionImpl::OnExcept(SOCKET oSock)
	{
		RemoveFromExcept(m_oSock, this);
		if (CONNECTED == m_enState)
		{
			Notify(COMM_CLOSE, TCP_COMM);
			m_enState = CLOSED;
		}
	}

	void TcpConnectionImpl::OnWrite(SOCKET oSock)
	{
		if (CONNECTED == m_enState)
		{
			if (m_oSendData.size())
			{
				int iBytes = nethelper::Send(m_oSock, &m_oSendData[0], m_oSendData.size());
				if (iBytes == m_oSendData.size())
				{
					RemoveFromWrite(m_oSock, this);
					m_oSendData.resize(0);
				}
				else if (iBytes > 0 && iBytes < m_oSendData.size())
				{
					m_oSendData.erase(m_oSendData.begin(), m_oSendData.begin() + iBytes);
				}
			}
			else
			{
				RemoveFromWrite(m_oSock, this);
			}
		}
		else if (CONNECTING == m_enState)
		{
			m_enState = CONNECTED;
			RemoveFromWrite(m_oSock, this);
			AddToRead(m_oSock, this);
			Notify(COMM_CONNECT, TCP_COMM);
		}
	}

	//读取socket上所有数据到oBuf中，追加在oBuf后面
	int TcpConnectionImpl::RecvAll(InnerUtility::BUF& oBuf, bool bTcp)
	{
		int iOldSize = 0;
		int iReadBytes = 0;
		while (true)
		{
			iOldSize = oBuf.size();
			oBuf.resize(iOldSize + SOCKET_BUFFER_LEN);
			iReadBytes = nethelper::Recv(m_oSock, reinterpret_cast<char *>(&oBuf[iOldSize]), SOCKET_BUFFER_LEN);
			if (iReadBytes > 0 && iReadBytes != SOCKET_BUFFER_LEN)
			{
				oBuf.erase(oBuf.begin() + iOldSize + iReadBytes, oBuf.end());
				break;
			}
			else if (iReadBytes == 0)
			{
				if (CONNECTED == m_enState)
				{
					Notify(COMM_CLOSE, bTcp ? TCP_COMM : UDP_COMM);
				}
				Close();
				break;
			}
			else if (iReadBytes == SOCKET_ERROR)
			{
				oBuf.erase(oBuf.begin() + iOldSize, oBuf.end());

				if (WSAEWOULDBLOCK != WSAGetLastError())
				{
					if (CONNECTED == m_enState)
					{
						Notify(COMM_CLOSE, bTcp ? TCP_COMM : UDP_COMM);
					}
					Close();
				}
				break;
			}
		}
		return oBuf.size();
	}

	int TcpConnectionImpl::GetSockName()
	{
		int len = sizeof(struct sockaddr_in);
		struct sockaddr_in localAddr;
		localAddr.sin_family = AF_INET;
		if( ::getsockname(m_oSock, (sockaddr *)&localAddr, &len) != 0 )
		{
			//int re=re=WSAGetLastError();
			return 0;
		}

		return localAddr.sin_addr.s_addr;
	}

}; //end namespace netlayer
