#include "stdafx.h"
#include "TimersManager.h"
#include "TimeUtils.h"
#include "EventPool.h"
#include "ThreadsManager.h"

TimersManager* TimersManager::s_pTimersManager = NULL;

UInt32 TimersManager::registerTimerHandler(SInt64 lFirstStartTime, SInt64 lPeriodStartTime, TimerHandler* pHandler)
{
	UInt32  iThreadIndex = 0;
	Timer*  pTimer = NULL;

	{
		MutexLocker		locker(&m_timerMutexRW);
		do 
		{
			m_iMaxTimersID++;
			if (0==m_iMaxTimersID) m_iMaxTimersID=1;
		} 
		while(m_timerMap.find(m_iMaxTimersID)!=m_timerMap.end());

		iThreadIndex = m_iMaxTimersID%ThreadsManager::s_iWorkThreadsNum;
		pTimer = new Timer(m_iMaxTimersID, lFirstStartTime, lPeriodStartTime, pHandler);
		Assert(NULL!=pTimer);
		
		pTimer->AddReference();
		m_timerMap.insert(timer_pair(m_iMaxTimersID, pTimer));
	}

	//防止在工作线程还没有添加定时器时就删除
	pTimer->AddReference();

	LPEvent pEvent = EventPool::getLPEvent();
	pEvent->m_iEventType = kAddTimerEvent;
	pEvent->m_pCookie    = (void*)pTimer;
	ThreadsManager::s_pWorkThreads[iThreadIndex]->addCommandEvent(pEvent);

	return pTimer->getTimerID();
}

void TimersManager::removeTimerHandler(UInt32 iTID, bool bWait)
{
	Timer*  pTimer = NULL;

	{
		MutexLocker			locker(&m_timerMutexRW);
		std::map<UInt32, Timer*>::iterator timerIter = m_timerMap.find(iTID);
		if(timerIter!=m_timerMap.end())
		{
			pTimer	=	timerIter->second;
			m_timerMap.erase(timerIter);
		}
	}

	if (!pTimer) 
	{
		//定时器已经不存在
		Assert(0);
		return;
	}

	Assert(pTimer->isShared());
	LPEvent pEvent = EventPool::getLPEvent();
	pEvent->m_iEventType = kKillTimerEvent;
	pEvent->m_pCookie    = (void*)pTimer;

	UInt32  iThreadIndex = iTID%ThreadsManager::s_iWorkThreadsNum;
	ThreadsManager::s_pWorkThreads[iThreadIndex]->addCommandEvent(pEvent);

	if (bWait)
	{
		pTimer->WaitUnshared();
	}
	
	pTimer->RemoveReference();
}