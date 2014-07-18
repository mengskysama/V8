#pragma once

#include "TimerHandler.h"
#include "TimeUtils.h"
#include "Heap.h"

#include "RC.h"

class TimersManager;

class Timer : public RCObject
{
public:

	Timer(UInt32 iTimeID, SInt64 lFirstExpireTime, SInt64 lPeriodExpireTime, TimerHandler* pHandler)	
		: m_iTimerID(iTimeID),
	      m_iRunedTimes(0),
		  m_pHandler(pHandler),
		  m_lFirstExpireInterval(lFirstExpireTime),
		  m_lPeriodExpireInterval(lPeriodExpireTime),
		  m_HeapElem(this)
	{
	}

	~Timer()
	{
		if(m_pHandler!=NULL)
		{
			delete m_pHandler;
			m_pHandler	=	NULL;
		}
	}

	inline void Run()
	{
		if(m_pHandler!=NULL)
		{
			if (0==m_iRunedTimes)
			{
				m_pHandler->onFirstExpired();
			}
			else
			{
				m_pHandler->onPeriodExpired();
			}
		}

		m_iRunedTimes++;
	}

	inline  UInt32            getTimerID()               {  return m_iTimerID;             }
	inline  HeapElem*         getHeapElem()              {  return &m_HeapElem;            }
	inline  SInt64            getFirstExpireInterval()   {  return m_lFirstExpireInterval; }
	inline  SInt64            getPeriodExpireInterval()  {  return m_lPeriodExpireInterval;}

private:

	UInt32          m_iTimerID;                 //定时的ID
	
	UInt32			m_iRunedTimes;				//已经运行了的次数;
	SInt64			m_lFirstExpireInterval;		//第一次启动的时间间隔;
	SInt64			m_lPeriodExpireInterval;	//周期性启动的时间间隔;

	TimerHandler*	m_pHandler;					//定时器处理器句柄;

	HeapElem        m_HeapElem;                 //堆元素

	friend   class TimersManager;
};
