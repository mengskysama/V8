#include "stdafx.h"
#include "BufferEngine.h"
#include "BufferOutputTask.h"
#include "BufferOutputAudioEngine.h"
#include "BufferOutputVideoEngine.h"

CBufferOutputAudioTask::CBufferOutputAudioTask()
{
	BufferTrace("CBufferOutputAudioTask::CBufferOutputAudioTask() begin\n");
}

CBufferOutputAudioTask::~CBufferOutputAudioTask()
{
	BufferTrace("CBufferOutputAudioTask::~CBufferOutputAudioTask() begin\n");
}

void CBufferOutputAudioTask::Entry()
{
	ErrTrace("CBufferOutputAudioTask::Entry() ½øÈë thread_id=%d¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª\n",GetCurrentThreadID());
	SetRunningState(true);
	map<UInt64,CBufferOutputAudioEngine*>::iterator iter;
	CBufferOutputAudioEngine* pAudioEngine = NULL;
	while (!IsStopRequested() && IsRunningState())
	{
		DWORD startTime = GetTickCount();
		{
			MutexLocker locker(&CBufferOutputAudioEngine::m_static_mutex);//ÍøÂçÏß³Ì ²¥·ÅÏß³Ì m_mapAudioOutput
			for (iter = CBufferOutputAudioEngine::m_mapAudioOutput.begin();
				iter!=CBufferOutputAudioEngine::m_mapAudioOutput.end();iter++){
				pAudioEngine = iter->second;
				if (pAudioEngine){
					pAudioEngine->Play();
				}
				SwitchToThread();
			}
		}
		
		DWORD sMinus = GetTickCount()-startTime;
		SwitchToThread();

		if (sMinus <5){
			SleepEx(5-sMinus,FALSE);
		}
	}

	ErrTrace("CBufferOutputAudioTask::Entry() ÍË³ö thread_id=%d¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª\n",GetCurrentThreadID());
}

CBufferOutputVideoTask::CBufferOutputVideoTask()
{
	BufferTrace("CBufferOutputVideoTask::CBufferOutputVideoTask() begin\n");
}

CBufferOutputVideoTask::~CBufferOutputVideoTask()
{
	BufferTrace("CBufferOutputVideoTask::~CBufferOutputVideoTask() begin\n");
}

void CBufferOutputVideoTask::Entry()
{
	ErrTrace("CBufferOutputVideoTask::Entry() ½øÈë thread_id=%d¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª\n",GetCurrentThreadID());
	SetRunningState(true);
	DWORD dwStartTime = GetTickCount();
	map<UInt64,CBufferOutputVideoEngine*>::iterator iter;
	CBufferOutputVideoEngine* pVideoEngine = NULL;
	while (!IsStopRequested() && IsRunningState())
	{
		DWORD startTime = GetTickCount();
		{
			MutexLocker locker(&CBufferOutputVideoEngine::m_static_mutex);//ÍøÂçÏß³Ì ²¥·ÅÏß³Ì m_mapVideoOutput
			for (iter = CBufferOutputVideoEngine::m_mapVideoOutput.begin();
				iter!=CBufferOutputVideoEngine::m_mapVideoOutput.end();iter++){
					pVideoEngine = iter->second;
					if (pVideoEngine){
						pVideoEngine->Play();
					}
					SwitchToThread();
			}
		}
		
		 //ÒÆ³ý²»ÔÚÊ¹ÓÃÓÃ»§¶ÔÏó 5 minute ¼ì²âÒ»´Î
		if (GetTickCount()-dwStartTime>BUFFER_MEDIA_CHECK_TIME){
			dwStartTime = GetTickCount();
			g_bufferEngine.ClearExpireUser();
		}

		DWORD sMinus = GetTickCount()-startTime;
		SwitchToThread();

		if (sMinus <5){
			SleepEx(5-sMinus,FALSE);
		}
	}

	ErrTrace("CBufferOutputVideoTask::Entry() ÍË³ö thread_id=%d¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª\n",GetCurrentThreadID());
}
