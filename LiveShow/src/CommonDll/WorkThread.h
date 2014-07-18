#ifndef __WORKTHREAD_H__
#define __WORKTHREAD_H__

#include	"Common.h"
#include	"CondQueue.h"
#include	"Event.h"
#include	"Thread.h"
#include	"Mutex.h"
#include	"SocketContext.h"
#include    "Heap.h"

class ThreadsManager;

class WorkThread:public BaseThread
{
public:
	//////////////////////////////////////////////////////////////////////////
	//构造和析构函数：
	WorkThread(UInt32 iSocketArraySize): 
	  BaseThread(), 
	  m_EventQueue(),
	  m_iSocketsArraySize(iSocketArraySize),
	  m_iTotalSocketNum(0)
	{
		m_pSocketContexts = new SocketContext*[m_iSocketsArraySize];
		::memset(m_pSocketContexts,0,sizeof(SocketContext*)*m_iSocketsArraySize);
	}

   	virtual ~WorkThread()
	{
		clearTimerHeap();
		clearCommandEvents();

		if(m_pSocketContexts!=NULL)
		{
			for (int i = 0 ; i < m_iSocketsArraySize ; i++)
			{
				SAFE_DELETE(m_pSocketContexts[i]);
			}
			delete []m_pSocketContexts;
			m_pSocketContexts = NULL;
		}
	}
	
	//////////////////////////////////////////////////////////////////////////
	//功能描述：线程的主入口函数
	//输入参数：无；
	//返回值：  无；
	virtual void Entry();

	//////////////////////////////////////////////////////////////////////////
	//功能描述: 向该线程添加一个命令事件;
	//输入参数: @参数1: pEvent; 被添加的命令事件;
	//返回值: 0: 表示成功; <0; 表示失败的原因码;

	inline  void addCommandEvent(LPEvent pEvent)
	{
		return m_EventQueue.enQueue(&(pEvent->m_QueueElem));
	}

private:
	//////////////////////////////////////////////////////////////////////////
	//功能描述：处理该线程上的事件；
	//输入参数：@参数1: pEvent; 指向被处理的事件的指针;
	//返回值： 无;
	void			processEvent(LPEvent pEvent);

	SInt32          processTimer();

	//////////////////////////////////////////////////////////////////////////
	//功能描述：获取本线程上收到的命令事件; 该函数阻塞等待5ms;
	//输入参数：@参数1: pEvent; 接受到的命令事件;
	//返回值：1: 有命令事件到达; 0: 表示没有命令事件到达;
	inline Bool		getCommandEvent(LPEvent& pEvent, SInt32 iTimeOut=1)
	{
		QueueElem* pCurElem = m_EventQueue.deQueueBlocking(iTimeOut);
		if(pCurElem!=NULL)
		{
			pEvent	=(LPEvent)(pCurElem->getEnclosingObject());
			return 1;
		}
		return 0;
	}

	//////////////////////////////////////////////////////////////////////////
	//功能描述:  清除所有的命令事件;
	//输入参数:
	//返回值:
	void         clearCommandEvents();

	//清除所有的定时器
	void            clearTimerHeap();

	//////////////////////////////////////////////////////////////////////////
	//成员变量；
	Queue_Blocking    m_EventQueue;				//该线程上的命令事件队列;	  

	Heap              m_TimerHeap;              //定时器堆

	//////////////////////////////////////////////////////////////////////////
	//SocketContext相关属性；
	SocketContext**   m_pSocketContexts;		//该线程处理的SocketContext列表；
	UInt32            m_iTotalSocketNum;		//本线程上正在处理的SocketContext的总数;
	SInt32            m_iSocketsArraySize;		//Socket数组的大小;

	friend class	  ThreadsManager;
};

#endif //__WORKTHREAD_H__