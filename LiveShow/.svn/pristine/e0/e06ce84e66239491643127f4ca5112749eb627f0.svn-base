#include "stdafx.h"
#include "Cond.h"
#include "Thread.h"


Cond::Cond()
{
	m_hCondition = ::CreateEvent(NULL, FALSE, FALSE, NULL);
	m_nWaitCount = 0;
	Assert(m_hCondition != NULL);
}

Cond::~Cond()
{
	BOOL theErr = ::CloseHandle(m_hCondition);
	Assert(theErr == TRUE);
}
