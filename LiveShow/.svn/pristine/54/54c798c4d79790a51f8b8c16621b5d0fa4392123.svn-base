#pragma once

#include "Mutex.h"
#include "Timer.h"
#include "TimerHandler.h"

#include <vector>
#include <map>

#pragma warning(disable: 4251)

class COMMON_OUT_CLASS TimersManager 
{
public:

	static void initialize()
	{
		if (s_pTimersManager == NULL)
		{
			s_pTimersManager = new TimersManager();
		}
	}

	static void                 destroy()       {  
		SAFE_DELETE(s_pTimersManager); 
	}
	static TimersManager*       getInstance()   {  return s_pTimersManager;      }

	UInt32       	registerTimerHandler(SInt64 lFirstStartTime, SInt64 lPeriodStartTime, TimerHandler*);
	void	        removeTimerHandler(UInt32 iTID, bool bWait = false);

protected:

	TimersManager()	
	: m_iMaxTimersID(0),
	  m_timerMutexRW()
	{}

	virtual ~TimersManager()
	{
		//删除定时器列表中的所有定时器;
		Timer* pTimer = NULL;
		std::map<UInt32, Timer*>::iterator iter;
		MutexLocker locker(&m_timerMutexRW);
		for (iter=m_timerMap.begin(); iter!=m_timerMap.end(); iter++)
		{
			pTimer = iter->second;
			if (pTimer)
			{
				pTimer->RemoveReference();
			}
		}
		m_timerMap.clear();
	}

private:

	typedef std::pair<UInt32, Timer*> timer_pair;

	Mutex						     m_timerMutexRW;			//定时器列表的锁;
	UInt32						     m_iMaxTimersID;			//总的定时器最大的ID;
	std::map<UInt32, Timer*>	     m_timerMap;				//定时器表;

	static  TimersManager*		     s_pTimersManager;			//指向定时器管理单元单体的指针;  
};

