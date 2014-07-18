#ifndef __EVENT_H__
#define __EVENT_H__

#include "Common.h"
#include "CondQueue.h"

enum
{
	kSocketNetworkEvent         =  0x00001000,      //Socket上的网络事件            

	kSocketReadableEvent		=  0x00000001,		//Socket可读事件;				//Cookie不带信息;
	kSocketWriteableEvent		=  0x00000002,		//Socket可写事件;				//Cookie不带信息;
	kSocketClosedEvent          =  0x00000004,		//Socket已经关闭事件;			//Cookie不带信息;

	kSocketSendPacketEvent      =  0x00000020,      //Socket发送某个包命令事件;		//Cookie中为指向一个数据包的指针;
	 
	kAddSocketEvent             =  0x00000100,		//添加一个Socket的命令事件;     //Cookie中附带为SocketContext的指针;  
	kKillSocketEvent            =  0x00000200,      //关闭一个Socket的命令事件;	    //Cookie不带信息;

	kSocketEventMask            =  0x0000ffff,

	kAddTimerEvent              =  0x00010000,      //添加定时器
	kKillTimerEvent             =  0x00020000,      //删除定时器

	kTimerEventMask             =  0x000f0000,      //定时器事件掩码

	kAllEventMask               =  0xffffffff
};

typedef struct __Event
{
	UInt32		m_iEventType;						//Event的类型;
	void*       m_pCookie;							//事件的附带信息;
	SInt32      m_iUniqueID;						//事件所属实体的唯一ID号;
													//其中包含的实体有FileContext的唯一ID号，
													//以及SocketContext唯一ID号等;
	QueueElem   m_QueueElem;
	__Event():
		m_QueueElem(this),
		m_iEventType(0),
		m_pCookie(NULL),
		m_iUniqueID(-1)
		{}
}EventCOMM, *LPEvent;

#endif //__EVENT_H__