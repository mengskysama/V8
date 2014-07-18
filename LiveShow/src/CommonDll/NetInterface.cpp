#include "stdafx.h"
#include "NetInterface.h"
#include "ThreadsManager.h"
#include "SocketIDGenerater.h"
#include "EventPool.h"

using namespace std;

vector<string>    NetInterface::s_vLocalIPAddresses;

SInt32	NetInterface::initialize()
{
		WSADATA wsaData;
		::WSAStartup(MAKEWORD(2, 2), &wsaData);

		UInt32  iNumIPAddrs			= 0;
		UInt32	iCurrentIndex		= 0;
		string  strIPAddress		= "";
		struct  sockaddr_in* pAddr  = NULL; 
		SInt32	iTempSocket			= ::socket(AF_INET, SOCK_DGRAM, 0);
		if (iTempSocket == -1)
		{
			Assert(0);
			return EOPENFAILED;
		}

		static const UInt32 kMaxAddrBufferSize = 2048;
		char				pInBuffer[kMaxAddrBufferSize];
		char				pOutBuffer[kMaxAddrBufferSize];
		UInt32				iReturnedSize	=	0;

		//
		// Use the WSAIoctl function call to get a list of IP addresses
		SInt32 theErr = ::WSAIoctl(	iTempSocket, 
									SIO_GET_INTERFACE_LIST, 
									pInBuffer, 
									kMaxAddrBufferSize,
									pOutBuffer,
									kMaxAddrBufferSize,
									(LPDWORD)&iReturnedSize,
									NULL,
									NULL);
		if (theErr != 0)
		{
			Assert(0);
			::closesocket(iTempSocket);
			return EIOFAILED;
		}

		LPINTERFACE_INFO pAddrList = (LPINTERFACE_INFO)&pOutBuffer[0];
		iNumIPAddrs = iReturnedSize / sizeof(INTERFACE_INFO);

		for (;iCurrentIndex < iNumIPAddrs; iCurrentIndex++)
		{
			strIPAddress= "";
			pAddr = (struct sockaddr_in*)&pAddrList[iCurrentIndex].iiAddress;
			strIPAddress  += ::inet_ntoa(pAddr->sin_addr);

			//store the IP addr
			if(strIPAddress!="127.0.0.1")
			{
				s_vLocalIPAddresses.push_back(strIPAddress);
			}		
		}
		::closesocket(iTempSocket);
		return ENOERR;
}

void	NetInterface::destroy()
{
	    s_vLocalIPAddresses.clear();

		//清除EventPool
		EventPool::destroy();

		::WSACleanup();
}

SInt32	NetInterface::registerSocketHandler(UInt32				iLocalIp,							//Socket的本地IP地址;网络字节序;
											UInt16				sLocalPort, 						//Socket的本机端口号，网络字节序列;
											SocketHandler*		pHandler,							//Socket上的处理器;
											SInt32*             pISocketID,                         //传出系统分配的SocketID
											UInt32				iMaxPacketLen,						//Socket上所传输的最大数据包的大小;
											UInt32              iReceiveBufferSize,					//Socket的接收缓存大小;
											UInt32              iWriteBufferSize					//Socket的发送缓存大小;
											)
{
	int re;
		if(pHandler==NULL)
		{
			return EPARAMATERS;
		}
		//创建一个Socket描述符;
		SInt32 iFd = (SInt32)socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
		if (iFd < 0)
		{
			return EOPENFAILED;
		}

		setNBlock(iFd);

		struct sockaddr_in servaddr;
		servaddr.sin_family = AF_INET;
		servaddr.sin_port = sLocalPort;
		servaddr.sin_addr.s_addr = iLocalIp;
		if (bind(iFd, (struct sockaddr *)(&servaddr), sizeof(struct sockaddr)) == -1)
		{
			re= WSAGetLastError();
			closesocket(iFd);
			return EBINDFAILED;
		}
		
		//传出系统分配的Socket ID
		if (pISocketID!=NULL)
		{
			*pISocketID = iFd;
		}

		//设置接收缓冲区和发送缓冲区的大小;
		setRecvBufferSize(iFd, iReceiveBufferSize);
		setWriteBufferSize(iFd,iWriteBufferSize);
		
		//给新的SocketContext分配一个ID; 并且根据分配的ID将SocketContext放入到指定的线程;
		SInt32	iUniqueID		= SocketIDGenerater::getNewID();
		pHandler->setSocketID(iUniqueID);
		SocketContext* pNewSocketContext	= new SocketContext(iFd,
																iUniqueID,
																iMaxPacketLen,
																pHandler,
																iReceiveBufferSize,
																iWriteBufferSize);

		sendCommadEvent(iUniqueID, kAddSocketEvent, (void*)pNewSocketContext);
		
		return iUniqueID;
}

//////////////////////////////////////////////////////////////////////////
//
void NetInterface::unregisterSocket(SInt32 iTargetID)
{
	if (iTargetID < 0 )
	{
		Assert(0);
		return;
	}

	sendCommadEvent(iTargetID, kKillSocketEvent);
}

//////////////////////////////////////////////////////////////////////////
//
/** 发送指定的UDP数据包,如果发送成功返回TRUE, 否则返回FALSE.*/
//输入:iFD:本地UDP连接(Socket)的FD；iRemoteIP:远端的IP地址，网络字节序；sRemotePort:远端的端口号，网络字节序；packet:包对象;
//输出:
void	NetInterface::sendUDPPacket(SInt32 iTargetID, UInt32 iRemoteIP, UInt16 sRemotePort, Packet* pPacket)
{
	if(pPacket==NULL)
	{
		Assert(0);
		return;
	}

	//Trace("NetInterface: sendUDPPacket [%s : %d] \n", NetInterface::IPUInt32ToString(iRemoteIP).c_str(), ntohs(sRemotePort));
	pPacket->setRemoteAddr(iRemoteIP,sRemotePort);
	sendCommadEvent(iTargetID, kSocketSendPacketEvent, (void*)pPacket);
}

/************************************************************************/
/* 工具函数                                                             */
/************************************************************************/
/**将IP地址字符串转换为32位的网络字节序地址*/
UInt32 NetInterface::IPStringToUInt32(const std::string& strIp)
{
		return inet_addr(strIp.c_str());
}

/**将IP地址由32位的网络字节序地址转换为字符串*/
std::string NetInterface::IPUInt32ToString(UInt32 iIp)
{
	string strResult = "";
	struct sockaddr_in addr;
	addr.sin_addr.s_addr = iIp;
	strResult = inet_ntoa(addr.sin_addr);
	return strResult;
}

void NetInterface::getLocalIPAddresses(std::vector<std::string>& vIPAddress)
{
	 vIPAddress = s_vLocalIPAddresses;
}

/************************************************************************/
/* 底层调用函数                                                         */
/************************************************************************/	
/**功能: 设置一个socket为非阻塞的 */
//输入: iSockFd; 被设置为非阻塞的Socket 描述符;
//输出: 无；
void	NetInterface::setNBlock(SInt32 iSockFd)
{
	u_long one = 1;
	int err = ::ioctlsocket(iSockFd, FIONBIO, &one);
}

//**功能: 设置一个Socket的接受缓冲区大小;
//输入: iSockFd, 被设置的描述符; UInt32 iRecvBuffSize, Buffer的大小;
void	NetInterface::setRecvBufferSize(SInt32 iSockFd,UInt32 iRecvBuffSize)
{
	int bufSize = iRecvBuffSize;
	int err = ::setsockopt(iSockFd, SOL_SOCKET, SO_RCVBUF, (char*)&bufSize, sizeof(int));
}

//**功能: 设置一个Socket的发送缓冲区大小；
//输入: iSockFd, 被设置的描述符; UInt32 iRecvBuffSize, Buffer的大小;
void	NetInterface::setWriteBufferSize(SInt32 iSockFd,UInt32 iWriteBuffSize)
{
	int bufSize = iWriteBuffSize;
	int err = ::setsockopt(iSockFd, SOL_SOCKET, SO_SNDBUF, (char*)&bufSize, sizeof(int));
}

//**功能: 设置一个Socket的Keep Alive属性;
//输入: iSockFd; 被设置的Socket 描述符;
//输出: 无；
void	NetInterface::setKeepAlive(SInt32 iSockFd)
{
	int one = 1;
	int err = ::setsockopt(iSockFd, SOL_SOCKET, SO_KEEPALIVE, (char*)&one, sizeof(int));
}

//**功能: 设置一个Socket的NoDelay属性;
//输入: iSockFd; 被设置的Socket 描述符;
//输出: 无；
void	NetInterface::setNoDelay(SInt32 iSockFd)
{
	int one = 1;
	int err = ::setsockopt(iSockFd, IPPROTO_TCP, TCP_NODELAY, (char*)&one, sizeof(int));
	Assert(err == 0);
}

void  NetInterface::sendCommadEvent(SInt32 iTargetID, UInt32 iEventType, void* pData)
{
	LPEvent pEvent			= EventPool::getLPEvent();
	pEvent->m_iUniqueID		= iTargetID;
	pEvent->m_iEventType	= iEventType;
	pEvent->m_pCookie		= pData;

	UInt32  iThreadIndex    = iTargetID% ThreadsManager::s_iWorkThreadsNum;
	ThreadsManager::s_pWorkThreads[iThreadIndex]->addCommandEvent(pEvent);
}
