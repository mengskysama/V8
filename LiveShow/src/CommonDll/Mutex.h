#ifndef __MUTEX_H__
#define __MUTEX_H__

#include <stdlib.h>

#include "Common.h"
#include "MyAssert.h"

class Cond;

class COMMON_OUT_CLASS Mutex
{
public:

	Mutex();
	~Mutex();

	inline void Lock();
	inline void Unlock();


private:
	Mutex(const Mutex& right){};
	Mutex& operator = (const Mutex& right){ return *this; };

	CRITICAL_SECTION m_Mutex;

	DWORD m_nHolder;
	UInt32 m_nHolderCount;

	void RecursiveLock();
	void RecursiveUnlock();

	friend class Cond;
};

class COMMON_OUT_CLASS MutexLocker
{
public:

	MutexLocker(Mutex* inMutexP)	
		: m_pMutex(inMutexP)
	{
		if (m_pMutex != NULL)
			m_pMutex->Lock();
	}
	~MutexLocker()
	{
		if (m_pMutex != NULL)
			m_pMutex->Unlock();
	}

	void Lock()
	{
		if (m_pMutex != NULL)
			m_pMutex->Lock();
	}
	void Unlock()
	{
		if (m_pMutex != NULL)
			m_pMutex->Unlock();
	}

private:

	Mutex* m_pMutex;
};

void Mutex::Lock()
{
	this->RecursiveLock();
}

void Mutex::Unlock()
{
	this->RecursiveUnlock();
}

#endif //__MUTEX_H__
