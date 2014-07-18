#include "stdafx.h"
#include "CondQueue.h"

Queue::Queue() 
:m_iLength(0)
{
	m_Sentinel.m_pNext = &m_Sentinel;
	m_Sentinel.m_pPrev = &m_Sentinel;
}

void Queue::enQueue(QueueElem* pElem)
{
	if (pElem->m_pQueue == this)
		return;
	pElem->m_pNext	= m_Sentinel.m_pNext;
	pElem->m_pPrev	= &m_Sentinel;
	pElem->m_pQueue = this;
	m_Sentinel.m_pNext->m_pPrev = pElem;
	m_Sentinel.m_pNext = pElem;
	m_iLength++;
}

QueueElem* Queue::deQueue()
{
	if (m_iLength > 0)
	{
		QueueElem* pElem = m_Sentinel.m_pPrev;
		pElem->m_pPrev->m_pNext = &m_Sentinel;
		m_Sentinel.m_pPrev = pElem->m_pPrev;
		pElem->m_pQueue = NULL;
		m_iLength--;
		return pElem;
	}
	else
		return NULL;
}

void Queue::remove(QueueElem* pElem)
{
	if (pElem->m_pQueue == this)
	{
		pElem->m_pNext->m_pPrev = pElem->m_pPrev;
		pElem->m_pPrev->m_pNext = pElem->m_pNext;
		pElem->m_pQueue = NULL;
		m_iLength--;
	}
}

void QueueIter::next()
{
	if (m_pCurrentElem == m_pQueue->getTail())
		m_pCurrentElem = NULL;
	else
		m_pCurrentElem = m_pCurrentElem->prev();
}

QueueElem* Queue_Blocking::deQueueBlocking(SInt32 iTimeoutInMilSecs)
{
	MutexLocker locker(&m_Mutex);

	if (m_Queue.getLength() == 0) 
	{	
		m_Cond.Wait(&m_Mutex, iTimeoutInMilSecs);
		return NULL;
	}

	QueueElem*	pRetVal = m_Queue.deQueue();
	return pRetVal;
}

QueueElem*    Queue_Blocking::deQueue()
{
	MutexLocker locker(&m_Mutex);
	QueueElem* pRetVal = m_Queue.deQueue(); 
	return pRetVal;
}


void Queue_Blocking::enQueue(QueueElem* pObj)
{
	{
		MutexLocker locker(&m_Mutex);
		m_Queue.enQueue(pObj);
	}
	m_Cond.Signal();
}
