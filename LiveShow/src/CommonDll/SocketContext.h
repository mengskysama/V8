#ifndef  __SOCKETCONTEXT_H__
#define  __SOCKETCONTEXT_H__

#include "Common.h"
#include "Mutex.h"
#include "CommonPacket.h"
#include "SocketHandler.h"
#include "Event.h"

#include <string>
#include <deque>
#include <vector>

//////////////////////////////////////////////////////////////////////////
//Socket的状态;
enum
{
	SOCKET_INITIALIZE				= 0,
	SOCKET_LISTEN					= 1,
	SOCKET_CLOSED					= 2
};

typedef UInt32 SOCKET_STATE;

class SocketContext
{
public:
	SocketContext(SInt32 iFd,
				  SInt32 iUniqueID,
				  UInt32  iMaxPacketLen,
				  SocketHandler* pHandler,
				  UInt32 iReadBufferSize,
				  UInt32 iWriteBufferSize);

	~SocketContext();

	inline  void  setState(UInt32 iState)
	{
		m_iState = iState;
	}

	//处理到达的Socket事件
	void					processEvent(LPEvent pEvent);

private:

	//处理添加Socket的命令事件;
	void					handleAdd();

	//处理关闭Socket的命令事件;
    void					handleKill();


	//接收该Socket上到达的数据包并且进行相应的处理;
	void					handleInput();


	//关闭该Socket;
	void					handleClosed();

	//在Socket上发送UDP消息;
	SInt32					sendUDPPacket(Packet* packet);
	
	
	//事件处理完毕之后的操作;
	void					postProcessEvent();

	//清除所请求了的事件;
	inline   void  clearEvent(UInt32 iEventMask)
	{
		m_iRequestedEvent &= ~iEventMask;
	}


	SInt32					m_iFd;								//Socket的FD；
	SOCKET_STATE			m_iState;							//Socket的状态;

	UInt32					m_iMaxPacketLen;					//本Socket上的最大Packet长度,//单位:字节

	UInt8*					m_pReceiveBuffer;					//接收到的TCP包

	SocketHandler*			m_pSocketHandler;					//该Socket上的事件处理器;

	UInt32                  m_iRequestedEvent;					//本次所请求的Socket事件;

	SInt32					m_iUniqueID;                        //Socket的UniqueID值;

	Packet					m_curRecvedPacket;					//当前所接收到的数据包;

	Bool					m_bKillExected;						//指示Kill命令是否已经被执行过了;

};

#endif //__SOCKETCONTEXT_H__