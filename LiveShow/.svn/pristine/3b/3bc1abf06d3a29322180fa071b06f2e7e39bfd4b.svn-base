#include "stdafx.h"
// #include <stdio.h>
// #include <stdlib.h>
// #include <string.h>
// #include <errno.h>

#include "Thread.h"
#include "MyAssert.h"
#include "TimeUtils.h"

void* BaseThread::s_pMainThreadData = NULL;

DWORD BaseThread::s_nThreadStorageIndex = 0;

void BaseThread::Initialize()
{
	s_nThreadStorageIndex = ::TlsAlloc();
	Assert(s_nThreadStorageIndex >= 0);
}

void BaseThread::Destroy()
{
	::TlsFree(s_nThreadStorageIndex);
}

BaseThread::BaseThread()
     :m_dwThreadSeqNo(0),
	  m_bStopRequested(false),
	  m_bJoined(false),
	  m_pThreadData(NULL),
	  m_bThreadRunning(false),
	  m_nThreadID(NULL),
	  m_mutex()
{
}

BaseThread::~BaseThread()
{
	if (NULL != m_nThreadID)
	{
		::CloseHandle(m_nThreadID);
	}
}

void BaseThread::Start()
{
	Init();//init data
	unsigned int theId = 0; // We don't care about the identifier
	m_nThreadID = (HANDLE)_beginthreadex(NULL, 	// Inherit security
										0,		// Inherit stack size
										_Entry,	// Entry function
										(void*)this,	// Entry arg
										0,		// Begin executing immediately
										&theId);
	Assert(m_nThreadID != NULL);
}

void BaseThread::Init()
{
    m_dwThreadSeqNo = 0;
	m_bStopRequested = false;
	m_bJoined = false;
	m_pThreadData = NULL;
	m_bThreadRunning = false;
	if (NULL != m_nThreadID) 
		::CloseHandle(m_nThreadID);
	m_nThreadID = NULL;
}

void BaseThread::StopAndWaitForThread()
{
	Trace("BaseThread::StopAndWaitForThread() begin time = %I64d \n",GetTickCount());
	if (!IsRunningState())
	{
		return;
	}

	//SendStopRequest();
	this->m_bStopRequested = true;
	
	if (!m_bJoined)
		Join();
	Trace("BaseThread::StopAndWaitForThread() end time = %I64d \n",GetTickCount());
}

void BaseThread::Join()
{
	Trace("BaseThread::Join() begin time = %I64d \n",GetTickCount());
	// What we're trying to do is allow the thread we want to delete to complete
	// running. So we wait for it to stop.
	
	this->m_bThreadRunning = false;
	SwitchToThread();
	Assert(!m_bJoined);
	m_bJoined = true;
	DWORD dExitCode;
	if (GetExitCodeThread(m_nThreadID,&dExitCode))
	{
		if (dExitCode != STILL_ACTIVE)
		{
			return;
		}
	}

	Trace("BaseThread::Join() mid time = %I64d \n",GetTickCount());
	DWORD theErr = ::WaitForSingleObject(m_nThreadID, INFINITE);
	//DWORD theErr = ::WaitForSingleObject(m_nThreadID, 100);
	Assert(theErr == WAIT_OBJECT_0);
	Trace("BaseThread::Join() end time = %I64d \n",GetTickCount());
}

void BaseThread::Sleep(UInt32 inMsec)
{
	::Sleep(inMsec);
}

unsigned int WINAPI BaseThread::_Entry (LPVOID inBaseThread)
{
	BaseThread* theBaseThread = (BaseThread*)inBaseThread;

	BOOL theErr = ::TlsSetValue(s_nThreadStorageIndex, theBaseThread);
	Assert(theErr == TRUE);

	// Run the thread
	theBaseThread->Entry();
	return NULL;
}

BaseThread* BaseThread::GetCurrent()
{
	return (BaseThread *)::TlsGetValue(s_nThreadStorageIndex);
}

int BaseThread::GetErrno()
{
	int winErr = ::GetLastError();


	// Convert to a POSIX errorcode. The *major* assumption is that
	// the meaning of these codes is 1-1 and each Winsock, etc, etc
	// function is equivalent in errors to the POSIX standard. This is
	// a big assumption, but the server only checks for a small subset of
	// the real errors, on only a small number of functions, so this is probably ok.
	switch (winErr)
	{
		case ERROR_FILE_NOT_FOUND:
			return ENOENT;
		case ERROR_PATH_NOT_FOUND:
			return ENOENT;
		case WSAEINTR:
			return EINTR;
		case WSAENETRESET:
			return EPIPE;
		case WSAENOTCONN:
			return ENOTCONN;
		case WSAEWOULDBLOCK:
			return EAGAIN;
		case WSAECONNRESET:
			return EPIPE;
		case WSAEADDRINUSE:
			return EADDRINUSE;
		case WSAEMFILE:
			return EMFILE;
		case WSAEINPROGRESS:
			return EINPROGRESS;
		case WSAEADDRNOTAVAIL:
			return EADDRNOTAVAIL;
		case WSAECONNABORTED:
			return EPIPE;
		case 0:
			return 0;
		default:
			return ENOTCONN;
	}
}
