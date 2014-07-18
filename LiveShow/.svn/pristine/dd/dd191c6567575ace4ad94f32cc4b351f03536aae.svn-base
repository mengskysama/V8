#ifndef  __NETINTERFACE_H__
#define  __NETINTERFACE_H__

#include "Common.h"
#include "SocketHandler.h"
#include "Mutex.h"
#include "SocketContext.h"

#include <string>
#include <vector>

#pragma warning(disable: 4251)

#define  DEFAULT_MAX_SENDING_QUEUE_LEN		100000

class COMMON_OUT_CLASS  NetInterface
{
public:
	NetInterface(){}
	~NetInterface(){}
	
	//功能描述：注册套接字处理器函数集合,返回值为该连接的全局唯一ID号;
	//返回值:
	//       <0:	表示注册失败,
	//		>=0:	表示注册成功,返回该Socket的全局唯一ID号
	static SInt32 registerSocketHandler(UInt32				iLocalIp,							//Socket的本地IP地址;网络字节序；
										UInt16				sLocalPort, 						//Socket的本机端口号，网络字节序列；
										SocketHandler*		pHandler,							//Socket上的处理器;
										SInt32*             pISocketID = NULL,                  //传出系统分配的SokcetID
										UInt32				iMaxPacketLen = 4*1024,				//Socket上所传输的最大数据包的大小;
										UInt32              iReceiveBufferSize= 256*1024,        //Socket的接收缓存大小;
										UInt32              iWriteBufferSize  = 256*1024         //Socket的发送缓存大小;
										);						

	//功能描述: 关闭一个Socket;
	//输入参数: @参数1: iUniqueID, Socket的唯一ID号;
	static  void		unregisterSocket(SInt32 iUniqueID);

	//功能描述: 初始化网络接口;
	//返回值:   0; 否则,错误码；
	static  SInt32		initialize();

	//////////////////////////////////////////////////////////////////////////
	//功能描述: 释放网络接口的资源;
	//输入参数：无；
	//返回值:	无；
	static  void		destroy();

	//////////////////////////////////////////////////////////////////////////
	//
	/** 发送指定的UDP数据包,如果发送成功返回TRUE, 否则返回FALSE.*/
	//输入:iFD:本地UDP连接(Socket)的FD；iRemoteIP:远端的IP地址，网络字节序；sRemotePort:远端的端口号，网络字节序；packet:包对象;
	//输出:
	static  void		sendUDPPacket(SInt32 iTargetID, UInt32 iRemoteIP, UInt16 sRemotePort, Packet* packet);

	/************************************************************************/
	/* 工具函数                                                             */
	/************************************************************************/
	/**将IP地址字符串转换为32位的网络字节序地址*/
	static	UInt32		IPStringToUInt32(const std::string& strIp);

	/**将IP地址由32位的网络字节序地址转换为字符串*/
	static  std::string IPUInt32ToString(UInt32 iIp);

	static  void		getLocalIPAddresses(std::vector<std::string>& vIPAddress);

	/************************************************************************/
	/* 底层调用函数                                                         */
	/************************************************************************/	
	/**功能: 设置一个socket为非阻塞的 */
	//输入: iSockFd; 被设置为非阻塞的Socket 描述符;
	//输出: 无；
	static void			setNBlock(SInt32 iSockFd);

	//**功能: 设置一个Socket的接受缓冲区大小;
	//输入: iSockFd, 被设置的描述符; UInt32 iRecvBuffSize, Buffer的大小;
	static void			setRecvBufferSize(SInt32 iSockFd,UInt32 iRecvBuffSize);

	//**功能: 设置一个Socket的发送缓冲区大小；
	//输入: iSockFd, 被设置的描述符; UInt32 iRecvBuffSize, Buffer的大小;
	static void			setWriteBufferSize(SInt32 iSockFd,UInt32 iWriteBuffSize);

	//**功能: 设置一个Socket的Keep Alive属性;
	//输入: iSockFd; 被设置的Socket 描述符;
	//输出: 无；
	static void			setKeepAlive(SInt32 iSockFd);

	//**功能: 设置一个Socket的NoDelay属性;
	//输入: iSockFd; 被设置的Socket 描述符;
	//输出: 无；
	static void			setNoDelay(SInt32 iSockFd);

private:

	static  void        sendCommadEvent(SInt32 iTargetID, UInt32 iEventType, void* pData=NULL);

private:

	static  std::vector<std::string>     s_vLocalIPAddresses;
};
#endif //__NETINTERFACE_H__