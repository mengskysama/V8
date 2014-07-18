#include "X51Sync.h"
#include "assert.h"


//===================================================================
// CX51SyncObject
//===================================================================

CX51SyncObject::CX51SyncObject( LPCTSTR pstrName)
{
	m_hObject = NULL;
	if( pstrName == NULL )
		m_strName = "";
	else
		m_strName = (char*)pstrName;
}

CX51SyncObject::~CX51SyncObject()
{
	if (m_hObject != NULL)
	{
		::CloseHandle(m_hObject);
		m_hObject = NULL;
	}
}

BOOL CX51SyncObject::Lock(DWORD dwTimeout)
{
	DWORD dwRet = ::WaitForSingleObject(m_hObject, dwTimeout);
	if (dwRet == WAIT_OBJECT_0 || dwRet == WAIT_ABANDONED)
		return TRUE;
	else
		return FALSE;
}


//===================================================================
// CX51Mutex
//===================================================================

CX51Mutex::CX51Mutex(BOOL bInitiallyOwn, LPCTSTR pstrName,
			   LPSECURITY_ATTRIBUTES lpsaAttribute /* = NULL */)
			   : CX51SyncObject(pstrName)
{
	m_hObject = ::CreateMutex(lpsaAttribute, bInitiallyOwn, pstrName);
}

CX51Mutex::~CX51Mutex()
{
}

BOOL CX51Mutex::Unlock()
{
	return ::ReleaseMutex(m_hObject);
}


//===================================================================
// CX51Semaphore
//===================================================================

CX51Semaphore::CX51Semaphore(LONG lInitialCount, LONG lMaxCount,
					   LPCTSTR pstrName, LPSECURITY_ATTRIBUTES lpsaAttributes)
					   :  CX51SyncObject(pstrName)
{
	assert(lMaxCount > 0);
	assert(lInitialCount <= lMaxCount);

	m_hObject = ::CreateSemaphore(lpsaAttributes, lInitialCount, lMaxCount,
		pstrName);
}

CX51Semaphore::~CX51Semaphore()
{
}

BOOL CX51Semaphore::Unlock()
{
	return Unlock(1, NULL);
}

BOOL CX51Semaphore::Unlock(LONG lCount, LPLONG lpPrevCount /* =NULL */)
{
	return ::ReleaseSemaphore(m_hObject, lCount, lpPrevCount);
}


//===================================================================
// CX51Event
//===================================================================

CX51Event::CX51Event(BOOL bInitiallyOwn, BOOL bManualReset, LPCTSTR pstrName,
			   LPSECURITY_ATTRIBUTES lpsaAttribute)
			   : CX51SyncObject(pstrName)
{
	m_hObject = ::CreateEvent(lpsaAttribute, bManualReset,
		bInitiallyOwn, pstrName);
}

CX51Event::~CX51Event()
{
}

BOOL CX51Event::Unlock()
{
	return TRUE;
}

BOOL CX51Event::SetEvent()
{
	assert( m_hObject != NULL );
	return ::SetEvent( m_hObject );
}

BOOL CX51Event::PulseEvent()
{
	assert( m_hObject != NULL );
	return ::PulseEvent( m_hObject );
}

BOOL CX51Event::ResetEvent()
{
	assert( m_hObject != NULL );
	return ::ResetEvent( m_hObject );
}

//===================================================================
// CX51CriticalSection
//===================================================================
CX51CriticalSection::CX51CriticalSection() : CX51SyncObject(NULL)
{ ::InitializeCriticalSection(&m_sect); }
CX51CriticalSection::operator CRITICAL_SECTION*()
{ return (CRITICAL_SECTION*) &m_sect; }
CX51CriticalSection::~CX51CriticalSection()
{ ::DeleteCriticalSection(&m_sect); }
BOOL CX51CriticalSection::Lock()
{ ::EnterCriticalSection(&m_sect); return TRUE; }
BOOL CX51CriticalSection::Lock(DWORD /* dwTimeout */)
{ return Lock(); }
BOOL CX51CriticalSection::Unlock()
{ ::LeaveCriticalSection(&m_sect); return TRUE; }