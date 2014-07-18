#include "stdafx.h"
#include "EventPool.h"

Mutex                   EventPool::m_Mutex;
std::deque<LPEvent>     EventPool::m_LPEventPool;

#define  MAX_EVENTNUM_IN_POOL             1024*10

LPEvent  EventPool::getLPEvent()
{
	LPEvent pEvent =NULL;
	{
		MutexLocker locker(&m_Mutex);
		if (!m_LPEventPool.empty())
		{
			pEvent = m_LPEventPool.front();
			m_LPEventPool.pop_front();
		}
	}
	
	if (NULL==pEvent)
	{
		pEvent = new EventCOMM();
	}
	
	return pEvent;
}

void  EventPool::putLPEvent(LPEvent pEvent)
{
	if (NULL==pEvent)
	{
		return;
	}

	{
		MutexLocker locker(&m_Mutex);
		if (m_LPEventPool.size()<MAX_EVENTNUM_IN_POOL)
		{
			m_LPEventPool.push_back(pEvent);
			return;
		}
	}

	SAFE_DELETE(pEvent);
}

void  EventPool::destroy()
{
	LPEvent pEvent = NULL;
	MutexLocker locker(&m_Mutex);
	while (!m_LPEventPool.empty())
	{
		pEvent = m_LPEventPool.front();
		SAFE_DELETE(pEvent);
		
		m_LPEventPool.pop_front();
	}
}