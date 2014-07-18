#include "stdafx.h"

#define _WIN32_WINNT 0x0400 //the current version is less than 0x0400

#include "Mutex.h"
#include <stdlib.h>
#include <string.h>
#include "Thread.h"

Mutex::Mutex()
{
	::InitializeCriticalSection(&m_Mutex);
	m_nHolder = 0;
	m_nHolderCount = 0;
}


Mutex::~Mutex()
{
	::DeleteCriticalSection(&m_Mutex);
}

void Mutex::RecursiveLock()
{
	// We already have this mutex. Just refcount and return
	if (/*BaseThread::GetCurrentThreadID()*/::GetCurrentThreadId() == m_nHolder)
	{
		m_nHolderCount++;
		return;
	}

	::EnterCriticalSection(&m_Mutex);

	Assert(m_nHolder == 0);
	m_nHolder = BaseThread::GetCurrentThreadID();
	m_nHolderCount++;
	Assert(m_nHolderCount == 1);
}

void Mutex::RecursiveUnlock()
{
	if (BaseThread::GetCurrentThreadID() != m_nHolder)
		return;

	Assert(m_nHolderCount > 0);
	m_nHolderCount--;
	if (m_nHolderCount == 0)
	{
		m_nHolder = 0;

		::LeaveCriticalSection(&m_Mutex);
	}
}
