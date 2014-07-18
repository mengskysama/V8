#ifndef __CONDQUEUE_H__
#define __CONDQUEUE_H__


#include "Mutex.h"
#include "Cond.h"
#include "Thread.h"

class Queue;

class QueueElem {
public:
	QueueElem(void* pEnclosingObject = NULL) : 
	  m_pNext(NULL), 
	  m_pPrev(NULL), 
	  m_pQueue(NULL),
	  m_pEnclosingObject(pEnclosingObject) {}
		
	 virtual ~QueueElem() { }

	inline  Bool	isMember(const Queue& queue) 
	{ 
		return (&queue == m_pQueue); 
	}

	inline  Bool	isMemberOfAnyQueue()     
	{ 
		return m_pQueue != NULL; 
	}

	inline void*	getEnclosingObject() 
	{ 
		return m_pEnclosingObject; 
	}

	inline void		setEnclosingObject(void* pObj) 
	{ 
		m_pEnclosingObject = pObj; 
	}

	QueueElem*		next()		{ return m_pNext; }
	QueueElem*		prev()		{ return m_pPrev; }

	Queue*			inQueue()	{ return m_pQueue; }
	
	inline void		remove();
private:

	QueueElem*		 m_pNext;
	QueueElem*		 m_pPrev;
	Queue *			 m_pQueue;
	void*			 m_pEnclosingObject;

	friend class	 Queue;
};

class Queue 
{
public:
	Queue();
	~Queue() {}

	void            enQueue(QueueElem* pObject);

	QueueElem*		deQueue();

	QueueElem*		getHead() 
	{ 
		if (m_iLength > 0) 
		{
            return m_Sentinel.m_pPrev;
		}
		return NULL; 
	}

	QueueElem*    getTail() 
	{ 
		if (m_iLength > 0) 
			return m_Sentinel.m_pNext; 
		return NULL; 
	}

	inline	UInt32  getLength() { return m_iLength; }

	void            remove(QueueElem* object);


protected:

	QueueElem		m_Sentinel;
	UInt32          m_iLength;
};

class QueueIter
{
public:
	QueueIter(Queue* pQueue) : 
	  m_pQueue(pQueue),
	  m_pCurrentElem(pQueue->getHead()) 
	{}

	QueueIter(Queue* pQueue, QueueElem* pStartElem) : 
	  m_pQueue(pQueue)
	{
		if ( pStartElem )
		{  
			m_pCurrentElem = pStartElem;
		}
		else
			m_pCurrentElem = NULL;
	}
	~QueueIter() {}

	inline    void  reset() 
	{ 
		m_pCurrentElem = m_pQueue->getHead(); 
	}

	inline    QueueElem* getCurrent() 
	{ 
		return m_pCurrentElem; 
	}

	void      next();

	inline	  Bool    isDone() 
	{ 
		return m_pCurrentElem == NULL; 
	}

private:

	Queue*        m_pQueue;
	QueueElem*    m_pCurrentElem;
};

class Queue_Blocking
{
public:
	Queue_Blocking() {}
	~Queue_Blocking() {}

	QueueElem*    deQueueBlocking(SInt32 iTimeoutInMilSecs);
	QueueElem*    deQueue();
	void          enQueue(QueueElem* pObj);

	Cond*         getCond()   { return &m_Cond; }
	Queue*        getQueue()  { return &m_Queue; }

private:

	Cond              m_Cond;
	Mutex             m_Mutex;
	Queue             m_Queue;
};

void QueueElem::remove()
{
	if (m_pQueue != NULL)
		m_pQueue->remove(this);
}

#endif //__CONDQUEUE_H__