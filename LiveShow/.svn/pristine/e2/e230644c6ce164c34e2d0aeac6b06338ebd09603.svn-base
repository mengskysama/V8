#include "stdafx.h"
#include "Common.h"
#include "Event.h"
#include "IOThread.h"
#include "AsyncCom.h"
#include "ThreadsManager.h"

#include <vector>

void IOThread::Entry()
{
	AsyncCom::initialize();

	//监听并且分派所有IO事件;
	std::vector<LPEvent> vEvents;
	SInt32		theRetValue		= EINTR;
	UInt32		iUniqueID		= 0;
	UInt32		iThreadIndex	= 0;

	while (!IsStopRequested())
	{
		theRetValue		= EINTR;

		theRetValue	=	AsyncCom::waitEvents(vEvents);

		if(theRetValue==EINTR) continue;
		
		iUniqueID	= 0;
		iThreadIndex= 0;

		//将所有的I/O线程收到的事件添加到工作线程
		for(UInt32 iEventCount=0; iEventCount<(UInt32)vEvents.size();iEventCount++)
		{
			iUniqueID		= vEvents[iEventCount]->m_iUniqueID;
			Assert(iUniqueID>=0);
			
			iThreadIndex	= iUniqueID%ThreadsManager::s_iWorkThreadsNum;
			ThreadsManager::s_pWorkThreads[iThreadIndex]->addCommandEvent(vEvents[iEventCount]);
		}

		vEvents.clear();
	}
	
	m_bKilled =1;

	AsyncCom::destroy();
}


//////////////////////////////////////////////////////////////////////////
//功能描述: 停止IO线程
//输入参数: 无
//返回值:   无
void		IOThread::stopIOThread()
{
//			{
//				MutexLocker locker(&m_mutex);
//				m_bStopRequested = 1;
//			}

			SendStopRequest();

			if(!m_bKilled)
			{
				::PostMessage(AsyncCom::getMsgWindow(),WM_QUIT,0,0);
				DWORD theErr = ::WaitForSingleObject(m_nThreadID, INFINITE);
				Assert(theErr==WAIT_OBJECT_0);
			}
}