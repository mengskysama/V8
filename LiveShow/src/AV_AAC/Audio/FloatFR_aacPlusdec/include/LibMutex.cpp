#include "LibMutex.h"

LibMutex::LibMutex()
{
	m_mutex = ::CreateMutex(NULL, FALSE, NULL);
}


LibMutex::~LibMutex()
{
	::CloseHandle(m_mutex);
}


void LibMutex::Lock() const
{
	DWORD d = WaitForSingleObject(m_mutex, INFINITE);
}


void LibMutex::Unlock() const
{
	::ReleaseMutex(m_mutex);
}


LibMutexLocker::LibMutexLocker(LibMutex* pMutex)
		:m_pMutex(pMutex)
{
	if (m_pMutex)
	{
		m_pMutex->Lock();
	}
}

LibMutexLocker::~LibMutexLocker()
{
	if (m_pMutex)
	{
		m_pMutex->Unlock();
	}
}

void LibMutexLocker::Lock()
{
	if (m_pMutex)
	{
		m_pMutex->Lock();
	}
}

void LibMutexLocker::Unlock()
{
	if (m_pMutex)
	{
		m_pMutex->Unlock();
	}
}





