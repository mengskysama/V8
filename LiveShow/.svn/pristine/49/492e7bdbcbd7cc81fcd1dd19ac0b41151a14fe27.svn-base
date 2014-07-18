#include "stdafx.h"
#include "SocketContext.h"
#include "AsyncCom.h"
#include "NetInterface.h"
#include "Event.h"
#include "ThreadsManager.h"
#include "SocketIDGenerater.h"
#include "TimeUtils.h"

using  namespace std;

SocketContext::SocketContext(SInt32 iFd,
							 SInt32 iUniqueID,
							 UInt32 iMaxPacketLen,
							 SocketHandler* pHandler,
							 UInt32 iReadBufferSize,
							 UInt32 iWriteBufferSize)
			:	m_iFd(iFd),
				m_iUniqueID(iUniqueID),
				m_iState(SOCKET_INITIALIZE),
				m_pSocketHandler(pHandler),
				m_iMaxPacketLen(iMaxPacketLen),
				m_iRequestedEvent(0),
				m_pReceiveBuffer(NULL),
				m_curRecvedPacket(iUniqueID),
				m_bKillExected(0)
{	
				m_pReceiveBuffer		=   new UInt8[m_iMaxPacketLen];
}

SocketContext::~SocketContext()
{
	            if(!m_bKillExected)
	            {
		            handleKill();
	            }

				if(m_pSocketHandler!=NULL)
				{
					delete m_pSocketHandler;
					m_pSocketHandler	=	NULL;
				}

				m_curRecvedPacket.setContentPtr(NULL, 0);

				if(m_pReceiveBuffer!=NULL)
				{
					delete[] m_pReceiveBuffer;
					m_pReceiveBuffer = NULL;
				}
				
}

//////////////////////////////////////////////////////////////////////////
//处理添加Socket的事件;
void		SocketContext::handleAdd()
{
		m_iState	= SOCKET_LISTEN;
}

//处理关闭Socket的事件;
void	SocketContext::handleKill()
{
		clearEvent(kAllEventMask);
		AsyncCom::clearEventRequest(m_iFd);	
		::closesocket(m_iFd);
		m_bKillExected = 1;
}

//////////////////////////////////////////////////////////////////////////
//接收该Socket上到达的数据包并且进行相应的处理;
void		SocketContext::handleInput()
{
		struct sockaddr_in addr;
		socklen_t addrlen = sizeof(addr);

		while (1)
		{
			// 接收UDP数据
			SInt32 iRecvLen = recvfrom(m_iFd, (char*)m_pReceiveBuffer, m_iMaxPacketLen, 0, (struct sockaddr *)&addr, &addrlen);
			if(iRecvLen>0)
			{
				//接收UDP数据成功;创建UDP包并且将UDP包放到Socket的RecvBuffer当中;
				m_curRecvedPacket.setContentPtr(m_pReceiveBuffer, iRecvLen);
				m_curRecvedPacket.setRemoteAddr(addr.sin_addr.s_addr, addr.sin_port);
				//Trace("收到数据包IP=%d  Port=%d \n",addr.sin_addr.s_addr, addr.sin_port);
				if(m_pSocketHandler!=NULL)
				{
					m_pSocketHandler->onPacketRcved(&m_curRecvedPacket);
				}			
			}
			else
			{
				break;
			}
		}
}

//关闭该Socket;
void	SocketContext::handleClosed()
{
	    clearEvent(kAllEventMask);

		if((m_pSocketHandler!=NULL)&&(m_iState!=SOCKET_CLOSED))
		{
			m_pSocketHandler->onClosed();
		}	
		setState(SOCKET_CLOSED);
}

//在Socket上发送UDP消息;
SInt32	SocketContext::sendUDPPacket(Packet* pPacket)
{
		Assert(pPacket!=NULL);
		SInt32			iRet			= -1;
		UInt32			iRemoteIP       = 0;
		UInt16			sRemotePort		= 0;
		pPacket->getRemoteAddr(iRemoteIP,sRemotePort);
		UInt8*          pPacketContent  =  pPacket->getPacketContent();
		UInt32          iPacketLen      =  pPacket->getPacketLen();

		struct sockaddr_in address;
		address.sin_family				= AF_INET; 
		address.sin_addr.s_addr			= iRemoteIP;
		address.sin_port				= sRemotePort;

		// 发送udp数据
		iRet	=	::sendto(m_iFd, (char*)pPacketContent, iPacketLen, 0, (struct sockaddr *)&address, sizeof(address));

		//Trace("SocketContext: sendUDPPacket[len=%d] [%s : %d], [result=%d] time = %I64d \n", iPacketLen, NetInterface::IPUInt32ToString(iRemoteIP).c_str(), ntohs(sRemotePort), iRet, GetTickCount());

		return  iRet;
}

//处理到达的Socket事件
void	SocketContext::processEvent(LPEvent pEvent)
{
		UInt32  iEventMask		= pEvent->m_iEventType;
		SInt32  iRetLen			= -1;

		if(iEventMask&kKillSocketEvent)
		{
			handleKill();
			return;	
		}
		if(iEventMask&kAddSocketEvent)
		{
			handleAdd();
		}
		switch(m_iState)
		{
			case SOCKET_INITIALIZE:
				 //所有Socket必须先调用Add函数;改变状态
				 Assert(0);			
				 break;
			case SOCKET_LISTEN:
				if(iEventMask&kSocketReadableEvent)
				{
					handleInput();
				}
				else if (iEventMask&kSocketSendPacketEvent)
				{
					iRetLen=sendUDPPacket((Packet*)pEvent->m_pCookie);
					if((iRetLen>0)&&(m_pSocketHandler!=NULL))
					{
						m_pSocketHandler->onDataSent(iRetLen);
						m_pSocketHandler->onPacketSent((Packet*)pEvent->m_pCookie);
					}
					((Packet*)pEvent->m_pCookie)->RemoveReference();
					pEvent->m_pCookie =NULL;
				}
				break;
			case SOCKET_CLOSED:
				if (iEventMask&kSocketSendPacketEvent)
				{
					m_pSocketHandler->onSendingFailed((Packet*)pEvent->m_pCookie);
					pEvent->m_pCookie = NULL;
				}
				break;
			default:
				Assert(0);
				break;
		}
		postProcessEvent();
}

//事件处理完毕之后的操作;
void	SocketContext::postProcessEvent()
{
	if (SOCKET_CLOSED!=m_iState)
	{
		m_iRequestedEvent |= kSocketReadableEvent;
		if(m_iRequestedEvent!=0)
		{
			Bool bRet = AsyncCom::requestEvent(m_iFd,m_iUniqueID, m_iRequestedEvent);
			if (bRet==0)
			{
				Assert(0);
				handleClosed();
			}
		}

		clearEvent(m_iRequestedEvent);
	}
}
