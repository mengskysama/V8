#include "stdafx.h"
#include "AsyncCom.h"
#include "WorkThread.h"
#include "TimersManager.h"
#include "ThreadsManager.h"
#include "SocketIDGenerater.h"
#include "EventPool.h"
#include "MyAssert.h"

#include  <string>
#include  <vector>
using namespace std;

void	WorkThread::Entry()
{
	try
	{
		//Trace("WorkThread::Entry() ... begin \n");
		while(!IsStopRequested())
		{
			SetRunningState(true);
			//首先检测定时器
			SInt32 theTimeout = processTimer();

			//没有停止请求时;继续执行;
			LPEvent	 pCurEvent = NULL;

			//遍历处理命令队列中的所有事件
			if (getCommandEvent(pCurEvent, theTimeout))
			{
				if(pCurEvent!=NULL)
				{
					//成功获得命令事件
					processEvent(pCurEvent);

					//若为命令事件；需要删除其Buffer以释放资源;
					if (!(pCurEvent->m_iEventType&kSocketNetworkEvent))
					{
						EventPool::putLPEvent(pCurEvent);
					}
					pCurEvent =NULL;
				}
			}
		}

		Trace("WorkThread::Entry() ... end \n");
	}
	catch (...)
	{
		Trace("WorkThread::Entry() 意外退出？？？？？\n");
	}
}

void	WorkThread::processEvent(LPEvent pEvent)
{
	Assert(pEvent!=NULL);
	UInt32  iArrayIndex  = 0;

	if((pEvent->m_iEventType&kSocketEventMask)!=0)
	{
		//////////////////////////////////////////////////////////////////////////
		//事件为Socket事件;
		//计算出该Socket所在的下标索引;
		iArrayIndex=(pEvent->m_iUniqueID/ThreadsManager::s_iWorkThreadsNum)%m_iSocketsArraySize;

		if(pEvent->m_iEventType&kAddSocketEvent)
		{
			m_pSocketContexts[iArrayIndex] = (SocketContext*)pEvent->m_pCookie;
		}
		if(m_pSocketContexts[iArrayIndex]!=NULL)
		{
			//////////////////////////////////////////////////////////////////////////
			//调用SocketContext的事件处理函数对事件进行处理;
			m_pSocketContexts[iArrayIndex]->processEvent(pEvent);			
			if(pEvent->m_iEventType&kKillSocketEvent)
			{
				delete m_pSocketContexts[iArrayIndex];
				m_pSocketContexts[iArrayIndex]=NULL;
				SocketIDGenerater::clearUniqueID(pEvent->m_iUniqueID);
			}
		}
		else
		{
			//////////////////////////////////////////////////////////////////////////
			//该Socket已经被删除, 并且该事件为发送包的事件;需要对包进行清理;
			if(pEvent->m_iEventType&kSocketSendPacketEvent)
			{
				((Packet*)(pEvent->m_pCookie))->RemoveReference();
				pEvent->m_pCookie=NULL;
			}
		}
	}
	else if ((pEvent->m_iEventType&kTimerEventMask)!=0) 
	{
		Timer* pTimer = (Timer*)pEvent->m_pCookie;
		HeapElem* pHeapElem = pTimer->getHeapElem();
		if (pEvent->m_iEventType&kAddTimerEvent) 
		{
			//				pTimer->AddReference();
			pHeapElem->SetValue(TimeUtils::Milliseconds()+pTimer->getFirstExpireInterval());
			m_TimerHeap.Insert(pHeapElem);
		}
		else if (pEvent->m_iEventType&kKillTimerEvent)
		{
			if (m_TimerHeap.Remove(pHeapElem)!=NULL)
			{
				pTimer->RemoveReference();
			}
		}
	}
}

SInt32  WorkThread::processTimer()
{
	//Trace("WorkThread::processTimer() ... begin \n");
	SInt32 iWaitTime=10;
	while (1)
	{
		//Trace("WorkThread::processTimer() ... while \n");
		if (m_TimerHeap.PeekMin()!=NULL)
		{
			SInt64 lCurTime = TimeUtils::Milliseconds();
			if(m_TimerHeap.PeekMin()->GetValue() <= lCurTime)
			{
				//运行定时器
				HeapElem* pHeapElem = m_TimerHeap.ExtractMin();
				Timer* pTimer = (Timer*) pHeapElem->GetEnclosingObject();
				if (pTimer)
				{
					pTimer->Run();

					//重新加入时间堆中
					pHeapElem->SetValue(lCurTime+pTimer->getPeriodExpireInterval());
					m_TimerHeap.Insert(pHeapElem);
				}
			}
			else
			{
				iWaitTime = m_TimerHeap.PeekMin()->GetValue() - lCurTime;
				break;
			}
		}

		break;
	}
	//Trace("WorkThread::processTimer() ... end \n");
	return iWaitTime;
}

//////////////////////////////////////////////////////////////////////////
//功能描述:
//输入参数:
//返回值:
void	WorkThread::clearCommandEvents()
{
	LPEvent		 pEvent  = NULL;
	//////////////////////////////////////////////////////////////////////////
	//遍历处理命令队列中的所有事件，若队列中没有命令事件，则等待5ms;
	while (getCommandEvent(pEvent))
	{
		if(pEvent!=NULL)
		{
			//////////////////////////////////////////////////////////////////////////
			//若为命令事件；需要删除其Buffer以释放资源;
			if(pEvent->m_iEventType&kSocketSendPacketEvent)
			{
				((Packet*)pEvent->m_pCookie)->RemoveReference();
			}
			if((pEvent->m_iEventType&kAddSocketEvent) ) 
			{
				delete (SocketContext*)pEvent->m_pCookie;
			}	
			if(pEvent->m_iEventType&kKillSocketEvent)  
			{
				// 					UInt32 iArrayIndex=(pEvent->m_iUniqueID/ThreadsManager::s_iWorkThreadsNum)%m_iSocketsArraySize;
				// 					SAFE_DELETE(m_pSocketContexts[iArrayIndex]);
			}
			if(pEvent->m_iEventType&kTimerEventMask)
			{
				Timer* pTimer = (Timer*)pEvent->m_pCookie;
				if (pEvent->m_iEventType&kAddTimerEvent) 
				{
					//此时直接删除
					pTimer->RemoveReference();
				}
				else if (pEvent->m_iEventType&kKillTimerEvent)
				{
					//此时不需要处理，由clearTimerHeap释放
				}
			}

			pEvent->m_pCookie = NULL;
			EventPool::putLPEvent(pEvent);
			pEvent =NULL;
		}
	}			
}

void   WorkThread::clearTimerHeap()
{
	HeapElem* pHeapElem = NULL;
	while ((pHeapElem = m_TimerHeap.ExtractMin())!=NULL)
	{
		Timer* pTimer = (Timer*) pHeapElem->GetEnclosingObject();
		pTimer->RemoveReference();
	}
}