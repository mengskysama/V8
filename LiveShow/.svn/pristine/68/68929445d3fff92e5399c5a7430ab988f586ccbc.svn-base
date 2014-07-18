#include "stdafx.h"
#include "RC.h"

RCObject::~RCObject()
{
	m_iRefCount = 0;
}

void  RCObject::AddReference()
{
	MutexLocker locker(&m_RCMutex);
	++m_iRefCount;
}

unsigned int RCObject::GetReference()
{
	return m_iRefCount;
}

void RCObject::SetReference(unsigned int iref)
{
	m_iRefCount = iref;
}

void  RCObject::RemoveReference()
{
	{
		MutexLocker locker(&m_RCMutex);
		Assert(m_iRefCount!=0);
		--m_iRefCount;
		if (0!=m_iRefCount) 
		{
			if (1==m_iRefCount)
			{
				m_Cond.Signal(); 
			}
			return;
		}
	}

	delete this;
}

bool  RCObject::isShared()  
{ 
	MutexLocker locker(&m_RCMutex); 
	return m_iRefCount>1;
}

void  RCObject::WaitUnshared()
{
	MutexLocker locker(&m_RCMutex);
	if (m_iRefCount>1)
	{
		m_Cond.Reset();
		m_Cond.Wait(&m_RCMutex,INFINITE);
	}
}