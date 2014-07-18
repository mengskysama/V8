#ifndef __IOTHREAD_H__
#define __IOTHREAD_H__

#include "Common.h"
#include "Event.h"
#include "Thread.h"
#include "AsyncCom.h"

class AsyncCom;

class IOThread:public BaseThread
{
public:
	//////////////////////////////////////////////////////////////////////////
	//构造和析构函数
	IOThread(UInt32 iSocketNum):
		BaseThread(), 
		m_iTotalSocketEventNum(iSocketNum),
		m_pSocketIOEvents(NULL),
		m_bKilled(0)
	{
		m_pSocketIOEvents = new EventCOMM[iSocketNum];
	}
	virtual ~IOThread() 
	{
		if(m_pSocketIOEvents!=NULL)
		{
			delete [] m_pSocketIOEvents;
			m_pSocketIOEvents = NULL;
		}
	}

	//////////////////////////////////////////////////////////////////////////
	//功能描述：线程的主入口函数；
	//输入参数：无；
	//返回值：  无；
	virtual void Entry();

	//////////////////////////////////////////////////////////////////////////
	//功能描述: 停止IO线程
	//输入参数: 无
	//返回值:   无
	void		stopIOThread();

private:

	LPEvent			m_pSocketIOEvents;			//指向 Socket EventPool的指针;
	UInt32			m_iTotalSocketEventNum;		//Socket Events的总数目;

	Bool			m_bKilled;

	friend  class AsyncCom;
};
#endif //__IOTHREAD_H__