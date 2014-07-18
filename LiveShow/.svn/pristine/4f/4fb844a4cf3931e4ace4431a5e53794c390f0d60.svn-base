#include "stdafx.h"
#include "BufferEngine.h"
#include "TimersManager.h"
#include "BufferOutputTask.h"

CBufferEngine g_bufferEngine;
Mutex CBufferEngine::m_InputMutex;

CBufferEngine::CBufferEngine() 
:pThreadVideo(NULL),
pThreadAudio(NULL),
IsThreadStartVideo(false),
IsThreadStartAudio(false),
IsThreadDestroy(false)
{
}

CBufferEngine::~CBufferEngine()
{
}

void	CBufferEngine::Initialize() 
{
		IsThreadDestroy = false;
		InitOutputTimer();
}

void    CBufferEngine::DestroyInstance()
{
		ErrTrace("CBufferEngine::DestroyInstance() begin\n");
		IsThreadDestroy = true;
		SwitchToThread();
		RemoveInputTimer();
		RemoveOutputTimer();
		ErrTrace("CBufferEngine::DestroyInstance() 停止所有输入，输出线程\n");
}

void	CBufferEngine::UnInitialize()
{
		BufferTrace("CBufferEngine::UnInitialize() ~!\n");
		IsThreadDestroy = true;
		RemoveInputTimer();
		RemoveOutputTimer();
		UInitInputEngine();
		UInitOutputEngine();
}

void	CBufferEngine::AddInputPacket(UInt32 iRoomId,Packet* pDataPacket,UInt32 iSequence,bool bA) 
{
		BufferTrace("CBufferEngine::AddInputPacket() begin\n");
		MutexLocker locker(&m_InputMutex);
		InitInputTimer(); 
		RCPtr<CBufferInputEngine> pBufferInputEngine = GetBufferInputEngine(iRoomId);
		if (pBufferInputEngine)
			pBufferInputEngine->AddInputPacket(pDataPacket,iSequence,bA);
		else
		{
			pDataPacket->RemoveReference();
			AssertVS(0,iRoomId,"CBufferEngine::AddInputPacket InputEngine不存在!\n");
		}
}

bool	CBufferEngine::GetInputPacket(UInt32 iRoomId,Packet* &pDataPacket,UInt32 iSequence,bool bA) 
{
		BufferTrace("CBufferEngine::GetInputPacket() begin\n");
		MutexLocker locker(&m_InputMutex);
		RCPtr<CBufferInputEngine> pBufferInputEngine = GetBufferInputEngine(iRoomId);
		if (pBufferInputEngine)
		{
			return pBufferInputEngine->GetInputPacket(pDataPacket,iSequence,bA);
		}
		else
		{
			AssertVS(0,iRoomId,"CBufferEngine::GetInputPacket InputEngine不存在!\n");
			return false;
		}
}

void	CBufferEngine::ClearInputPackets(UInt32 iRoomId,bool bA)
{
		BufferTrace("CBufferEngine::ClearInputPackets() begin\n");
		MutexLocker locker(&m_InputMutex);
		RCPtr<CBufferInputEngine> pBufferInputEngine = GetBufferInputEngine(iRoomId);
		if (pBufferInputEngine)
		{
			pBufferInputEngine->ClearInputPackets(bA);
		}
		else
		{
			AssertVS(0,iRoomId,"CBufferEngine::GetInputPacket InputEngine不存在!\n");
			return;
		}	
}

void    CBufferEngine::ClearInputPackets()
{
		BufferTrace("CBufferEngine::ClearInputPackets() begin\n");
		MutexLocker locker(&m_InputMutex);
		map<UInt32,RCPtr<CBufferInputEngine>>::iterator iter;
		for (iter=m_inputEngineMap.begin();iter!=m_inputEngineMap.end();iter++)
		{
			iter->second->ClearInputPackets(true);
			iter->second->ClearInputPackets(false);
		}
}

RCPtr<CBufferInputEngine>	CBufferEngine::GetBufferInputEngine(UInt32 iRoomId)
{
		BufferTrace("CBufferEngine::GetBufferInputEngine() begin\n");
		MutexLocker locker(&m_InputMutex);
		map<UInt32,RCPtr<CBufferInputEngine>>::iterator iter;
		if ((iter=m_inputEngineMap.find(iRoomId))!=m_inputEngineMap.end())
		{
			return iter->second;
		}
		else
		{
			Trace("创建roomId=%d CBufferInputEngine,time=%d\n",iRoomId,GetTickCount());
			RCPtr<CBufferInputEngine> pBufferInputEngine(new CBufferInputEngine(iRoomId));
			pBufferInputEngine->Initialize();
			m_inputEngineMap.insert(input_pair(iRoomId,pBufferInputEngine));
			return pBufferInputEngine;
		}
}

void	CBufferEngine::InitInputTimer()
{
		BufferTrace("CBufferEngine::InitInputTimer() begin\n");
		if (0==m_iInputTimerID)
		{
			BufferTrace("CBufferEngine::InitInputTimer() time=%d\n",GetTickCount());
			TimerInputWatch* pWatchTimer = new TimerInputWatch();
			TimersManager* pTimersManager   = TimersManager::getInstance();
			Assert(pTimersManager!=NULL);
			m_iInputTimerID= pTimersManager->registerTimerHandler(0, TIMER_INPUT_DATA_INTERVAL, pWatchTimer);
			Assert(m_iInputTimerID>0);
		}
}

void	CBufferEngine::RemoveInputTimer()
{
		BufferTrace("CBufferEngine::RemoveInputTimer() time=%d\n",GetTickCount());
		if (0!=m_iInputTimerID)
		{
			BufferTrace("CBufferEngine::RemoveInputTimer() removeTimerHandler \n");
			AssertVS(0,m_iInputTimerID,"CBufferEngine::RemoveInputTimer() 移除 timer.");
			TimersManager* pTimersManager   = TimersManager::getInstance();
			pTimersManager->removeTimerHandler(m_iInputTimerID, false);
			m_iInputTimerID = 0;
		}
}

void	CBufferEngine::EchoAudioInputInfo(UInt32 iRoomId)
{
		MutexLocker locker(&m_InputMutex);
		map<UInt32,RCPtr<CBufferInputEngine>>::iterator iter = m_inputEngineMap.find(iRoomId);
		if (iter!=m_inputEngineMap.end())
		{
			iter->second->EchoAudioInputInfo();
		}
		else
		{
			LostTrace("[RoomId:%d] CBufferEngine::EchoAudioInputInfo 房间Buffer不存在\n",iRoomId);
		}
}

void	CBufferEngine::EchoVideoInputInfo(UInt32 iRoomId)
{
		MutexLocker locker(&m_InputMutex);
		map<UInt32,RCPtr<CBufferInputEngine>>::iterator iter = m_inputEngineMap.find(iRoomId);
		if (iter!=m_inputEngineMap.end())
		{
			iter->second->EchoVideoInputInfo();
		}
		else
		{
			LostTrace("[RoomId:%d] CBufferEngine::EchoVideoInputInfo 房间Buffer不存在\n",iRoomId);
		}
}

void	CBufferEngine::UInitInputEngine()
{
		BufferTrace("CBufferEngine::UInitInputEngine() time=%d\n",GetTickCount());
		MutexLocker locker(&m_InputMutex);
		map<UInt32,RCPtr<CBufferInputEngine>>::iterator iter = m_inputEngineMap.begin();
		while (iter!=m_inputEngineMap.end())
		{
			iter->second->UnInitialize();
			iter = m_inputEngineMap.erase(iter);
		}
}

RCPtr<CBufferOutputEngine>	CBufferEngine::GetBufferOutputEngine(UInt32 iRoomId)
{
		//BufferTrace("CBufferEngine::GetBufferOutputEngine() begin\n");
		map<UInt32,RCPtr<CBufferOutputEngine>>::iterator iter;
		if ((iter=m_outputEngineMap.find(iRoomId))!=m_outputEngineMap.end())
		{
			return iter->second;
		}
		else
		{
			RCPtr<CBufferOutputEngine> pBufferoutputEngine(new CBufferOutputEngine(iRoomId));
			pBufferoutputEngine->Initialize();
			m_outputEngineMap.insert(output_pair(iRoomId,pBufferoutputEngine));
			return pBufferoutputEngine;
		}
}

void	CBufferEngine::AddOutputPacket(UInt32 iRoomId,UInt32 iFromUin,Packet* pDataPacket,UInt32 iSequence,bool bA)
{
		//BufferTrace("CBufferEngine::AddOutputPacket() begin\n");
		InitOutputTimer(); 
		if (bA)
		{
			RCPtr<CBufferOutputEngine> pBufferOutput = GetBufferOutputEngine(iRoomId);
			if (pBufferOutput)
			{
				pBufferOutput->AddOutputPacket(iFromUin,pDataPacket,iSequence,bA);
			}
			else
			{
				pDataPacket->RemoveReference();
				AssertVS(0,iRoomId,"CBufferEngine::AddOutputPacket 没有找到OutputEngine实例!");
			}
		}
		else
		{
			RCPtr<CBufferOutputEngine> pBufferOutput = GetBufferOutputEngine(iRoomId);
			if (pBufferOutput)
			{
				pBufferOutput->AddOutputPacket(iFromUin,pDataPacket,iSequence,bA);
			}
			else
			{
				pDataPacket->RemoveReference();
				AssertVS(0,iRoomId,"CBufferEngine::AddOutputPacket 没有找到OutputEngine实例!");
			}
		}
}

void	CBufferEngine::ClearOutputPackets(UInt32 iRoomId,UInt32 iFromUin,bool bA)
{
		BufferTrace("CBufferEngine::ClearOutputPackets() begin\n");
		RCPtr<CBufferOutputEngine> pBufferOutputEngine = GetBufferOutputEngine(iRoomId);
		if (pBufferOutputEngine)
		{
			pBufferOutputEngine->ClearOutputPackets(iFromUin,bA);
		}
		else
		{
			AssertVS(0,iRoomId,"CBufferEngine::ClearOutputPackets 没有找到OutputEngine实例!");
			return;
		}
}

void	CBufferEngine::SetRoomOnMic(UInt32 iRoomId,bool bEnable)
{
		BufferTrace("CBufferEngine::SetRoomOnMic() begin\n");
		RCPtr<CBufferOutputEngine> pBufferOutputEngine = GetBufferOutputEngine(iRoomId);
		if (pBufferOutputEngine)
		{
			pBufferOutputEngine->SetRoomOnMic(bEnable);
		}
		else
		{
			AssertVS(0,iRoomId,"CBufferEngine::SetRoomOnMic 实例不存在!");
		}
}

void	CBufferEngine::UInitOutputEngine()
{
		BufferTrace("CBufferEngine::UInitOutputEngine() time=%d\n",GetTickCount());
		map<UInt32,RCPtr<CBufferOutputEngine>>::iterator iter = m_outputEngineMap.begin();
		while (iter!=m_outputEngineMap.end()){
			iter->second->UnInitialize();
			iter = m_outputEngineMap.erase(iter);
		}

}

void	CBufferEngine::InitOutputTimer()
{
		//BufferTrace("CBufferEngine::InitOutputTimer() begin\n");
		if (IsThreadDestroy) return ;

		if (!IsThreadStartVideo){
			BufferTrace("CBufferEngine::InitOutputTimer() IsThreadStartVideo time=%d\n",GetTickCount());
			IsThreadStartVideo = TRUE;
			pThreadVideo = new CBufferOutputVideoTask();
			pThreadVideo->Start();
		}

		if (!IsThreadStartAudio){
			BufferTrace("CBufferEngine::InitOutputTimer() IsThreadStartAudio time=%d\n",GetTickCount());
			IsThreadStartAudio = TRUE;
			pThreadAudio = new CBufferOutputAudioTask();
			pThreadAudio->Start();
		}
}

void	CBufferEngine::RemoveOutputTimer()
{
		if (IsThreadStartVideo){
			IsThreadStartVideo = FALSE;
			pThreadVideo->SendStopRequest();
			pThreadVideo->StopAndWaitForThread();
			delete pThreadVideo;
			pThreadVideo = NULL;
		}

		if (IsThreadStartAudio){
			IsThreadStartAudio = FALSE;
			pThreadAudio->SendStopRequest();
			pThreadAudio->StopAndWaitForThread();
			delete pThreadAudio;
			pThreadAudio = NULL;
		}
}

void	CBufferEngine::PlayOutputAudio()
{
		
}

void	CBufferEngine::PlayOutputVideo()
{

}

void	CBufferEngine::ClearExpireUser()
{
	map<UInt32,RCPtr<CBufferOutputEngine>>::iterator iter;
	for (iter=m_outputEngineMap.begin();iter!=m_outputEngineMap.end();iter++)
	{
		iter->second->ClearExpireUser();
	}
}
