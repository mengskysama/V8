#ifndef __COND_H__
#define __COND_H__

#include "Mutex.h"
#include "MyAssert.h"

class COMMON_OUT_CLASS Cond
{
public:

	Cond();
	~Cond();

	inline void Signal();
	inline void Wait(Mutex* inMutex, SInt32 inTimeoutInMilSecs = 0);
	inline void Broadcast();
	inline void Reset();

private:

	HANDLE m_hCondition;
	UInt32 m_nWaitCount;
};

inline void Cond::Wait(Mutex* inMutex, SInt32 inTimeoutInMilSecs)
{
	DWORD theTimeout = INFINITE;
	if (inTimeoutInMilSecs > 0)
		theTimeout = inTimeoutInMilSecs;
	inMutex->Unlock();
	m_nWaitCount++;
	DWORD theErr = ::WaitForSingleObject(m_hCondition, theTimeout);
	m_nWaitCount--;
	Assert((theErr == WAIT_OBJECT_0) || (theErr == WAIT_TIMEOUT));
	inMutex->Lock();
}

inline void Cond::Signal()
{
	BOOL theErr = ::SetEvent(m_hCondition);
	Assert(theErr == TRUE);
}

inline void Cond::Broadcast()
{
	// There doesn't seem like any more elegant way to
	// implement Broadcast using events in Win32.
	// This will work, it may generate spurious wakeups,
	// but condition variables are allowed to generate
	// spurious wakeups
	UInt32 waitCount = m_nWaitCount;
	for (UInt32 x = 0; x < waitCount; x++)
	{
		BOOL theErr = ::SetEvent(m_hCondition);
		Assert(theErr == TRUE);
	}
}

inline void Cond::Reset()
{
	::ResetEvent(m_hCondition);
}

#endif //__COND_H__
