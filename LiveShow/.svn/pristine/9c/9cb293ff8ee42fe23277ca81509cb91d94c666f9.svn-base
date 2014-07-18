#include "stdafx.h"
#include "BufferOutputEngine.h"
#include "BufferOutputAudioRateAgc.h"
#include "BufferOutputVideoRateAgc.h"
#include "BufferOutputAudioEngine.h"

CBufferOutputEngine::CBufferOutputEngine(UInt32 iRoomId)
:m_iRoomId(iRoomId),
m_pIBaseAudioRate(NULL),
m_pIBaseVideoRate(NULL)
{
}

CBufferOutputEngine::~CBufferOutputEngine()
{
}

void	CBufferOutputEngine::Initialize()
{
	BufferTrace("CBufferOutputEngine::Initialize() begin\n");
	m_pIBaseAudioRate = new CBufferOutputAudioRateAgc();
	m_pIBaseVideoRate = new CBufferOutputVideoRateAgc();
	m_mapAudioBufferSize.clear();
	m_mapVideoBufferSize.clear();
}

void	CBufferOutputEngine::UnInitialize()
{
	{
		MutexLocker locker(&m_mutexA);//播放线程 socket线程 m_mapOutputAudioEngine
		map<UInt32,RCPtr<CBufferOutputAudioEngine>>::iterator iter = m_mapOutputAudioEngine.begin();
		while (iter!=m_mapOutputAudioEngine.end())
		{
			BufferTrace("CBufferOutputEngine::UnInitialize() while1\n");
			iter->second->UnInitialize();
			iter++;
		}
	}
	{
		MutexLocker locker(&m_mutexV);//播放线程 socket线程 m_mapOutputVideoEngine
		map<UInt32,RCPtr<CBufferOutputVideoEngine>>::iterator iter1 = m_mapOutputVideoEngine.begin();
		while (iter1!=m_mapOutputVideoEngine.end())
		{
			BufferTrace("CBufferOutputEngine::UnInitialize() while2\n");
			iter1->second->UnInitialize();
			iter1++;
		}
	}
	SAFE_DELETE(m_pIBaseAudioRate)
	SAFE_DELETE(m_pIBaseVideoRate)
	m_mapAudioBufferSize.clear();
	m_mapVideoBufferSize.clear();
}

void CBufferOutputEngine::SetBufferSize(unsigned int uin,En_Buffer_Type en,unsigned int bufsize)
{
	switch(en)
	{
	case bfAudio:
		m_mapAudioBufferSize[uin] = bufsize;
		break;
	case bfVideo:
		m_mapVideoBufferSize[uin] = bufsize;
		break;
	default:
		AssertVS(0,0,"CBufferOutputEngine::SetBufferSize default 无效类型");
	}
}

unsigned int CBufferOutputEngine::GetBufferSize(unsigned int uin,En_Buffer_Type en)
{
	unsigned int ret = 0;
	switch(en)
	{
	case bfAudio:
		{
			std::map<unsigned int,unsigned int>::iterator iter = m_mapAudioBufferSize.find(uin);
			if (iter!=m_mapAudioBufferSize.end()){
				ret = iter->second;
			}else{
				//AssertVS(0,0,"CBufferOutputEngine::GetBufferSize bfAudio NULL");
			}
		}
		break;
	case bfVideo:
		{
			std::map<unsigned int,unsigned int>::iterator iter = m_mapVideoBufferSize.find(uin);
			if (iter!=m_mapVideoBufferSize.end()){
				ret = iter->second;
			}else{
				//AssertVS(0,0,"CBufferOutputEngine::GetBufferSize bfVideo NULL");
			}
		}
		break;
	case bfOther:
		BufferTrace("CBufferOutputEngine::GetBufferSize bfOther 不支持类型");
		break;
	default:
		BufferTrace("CBufferOutputEngine::GetBufferSize default 不支持类型");
	}
	return ret;
}

void CBufferOutputEngine::SetRecvTime(unsigned int uin,En_Buffer_Type en,DWORD dwtime)
{
	switch(en)
	{
	case bfAudio:
		m_mapAudioBufferRecvTime[uin] = dwtime;
		break;
	case bfVideo:
		m_mapVideoBufferRecvTime[uin] = dwtime;
		break;
	default:
		AssertVS(0,0,"CBufferOutputEngine::SetRecvTime default 无效类型");
	}
}

DWORD CBufferOutputEngine::GetRecvTime(unsigned int uin,En_Buffer_Type en)
{
	DWORD ret = 0;
	switch(en)
	{
	case bfAudio:
		{
			std::map<unsigned int,DWORD>::iterator iter = m_mapAudioBufferRecvTime.find(uin);
			if (iter!=m_mapAudioBufferRecvTime.end()){
				ret = iter->second;
			}else{
				//AssertVS(0,0,"CBufferOutputEngine::GetRecvTime bfAudio NULL");
			}
		}
		break;
	case bfVideo:
		{
			std::map<unsigned int,DWORD>::iterator iter = m_mapVideoBufferRecvTime.find(uin);
			if (iter!=m_mapVideoBufferRecvTime.end()){
				ret = iter->second;
			}else{
				//AssertVS(0,0,"CBufferOutputEngine::GetRecvTime bfVideo NULL");
			}
		}
		break;
	case bfOther:
		BufferTrace("CBufferOutputEngine::GetRecvTime bfOther 不支持类型");
		break;
	default:
		BufferTrace("CBufferOutputEngine::GetRecvTime default 不支持类型");
	}
	return ret;
}

RCPtr<CBufferOutputAudioEngine>	CBufferOutputEngine::GetBufferOutputEngineA(UInt32 iFromUin)
{
	//BufferTrace("CBufferOutputEngine::GetBufferOutputEngineA() begin\n");
	map<UInt32,RCPtr<CBufferOutputAudioEngine>>::iterator iter;
	if ((iter=m_mapOutputAudioEngine.find(iFromUin))!=m_mapOutputAudioEngine.end()){
		return iter->second;
	}else{
		BufferTrace("创建iFromUin=%d CBufferOutputAudioEngine,time=%d\n",iFromUin,GetTickCount());
		RCPtr<CBufferOutputAudioEngine> pBufferOutputAudioEngine(new CBufferOutputAudioEngine(m_iRoomId,iFromUin));
		pBufferOutputAudioEngine->Initialize(this,m_pIBaseAudioRate);
		m_mapOutputAudioEngine.insert(pair_outputA(iFromUin,pBufferOutputAudioEngine));
		return pBufferOutputAudioEngine;
	}
}

RCPtr<CBufferOutputVideoEngine>	CBufferOutputEngine::GetBufferOutputEngineV(UInt32 iFromUin)
{
	//BufferTrace("CBufferOutputEngine::GetBufferOutputEngineV() begin\n");
	map<UInt32,RCPtr<CBufferOutputVideoEngine>>::iterator iter;
	if ((iter=m_mapOutputVideoEngine.find(iFromUin))!=m_mapOutputVideoEngine.end()){
		return iter->second;
	}else{
		Trace("创建iFromUin=%d CBufferOutputVideoEngine,time=%d\n",iFromUin,GetTickCount());
		RCPtr<CBufferOutputVideoEngine> pBufferOutputVideoEngine(new CBufferOutputVideoEngine(m_iRoomId,iFromUin));
		pBufferOutputVideoEngine->Initialize(this,m_pIBaseVideoRate);
		m_mapOutputVideoEngine.insert(pair_outputV(iFromUin,pBufferOutputVideoEngine));
		return pBufferOutputVideoEngine;
	}
}

void	CBufferOutputEngine::AddOutputPacket(UInt32 iFromUin,Packet* pDataPacket,UInt32 iSequence,bool bA)
{
	//BufferTrace("CBufferOutputEngine::AddOutputPacket() begin\n");
	if (bA){
		MutexLocker locker(&m_mutexA);//播放线程 socket线程 m_mapOutputAudioEngine
		RCPtr<CBufferOutputAudioEngine> pBufferOutputAudioEngine = GetBufferOutputEngineA(iFromUin);
		if (pBufferOutputAudioEngine){
			pBufferOutputAudioEngine->AddPacket(pDataPacket,iSequence);
		}else{
			pDataPacket->RemoveReference();
			AssertVS(0,iFromUin,"CBufferOutputEngine::AddOutputPacket 实例不存在");
		}
	}else{
		MutexLocker locker(&m_mutexV);//播放线程 socket线程 m_mapOutputVideoEngine
		RCPtr<CBufferOutputVideoEngine> pBufferOutputVideoEngine = GetBufferOutputEngineV(iFromUin);
		if (pBufferOutputVideoEngine){
			pBufferOutputVideoEngine->AddPacket(pDataPacket,iSequence);
		}else{
			pDataPacket->RemoveReference();
			AssertVS(0,iFromUin,"CBufferOutputEngine::AddOutputPacket 实例不存在");
		}
	}	
}

void CBufferOutputEngine::ClearOutputPackets(UInt32 iFromUin,bool bA)
{
	BufferTrace("CBufferOutputEngine::ClearOutputPackets() begin\n");
	if (bA){
		MutexLocker locker(&m_mutexA);//播放线程 socket线程 m_mapOutputAudioEngine
		map<UInt32,RCPtr<CBufferOutputAudioEngine>>::iterator iter;
		if ((iter=m_mapOutputAudioEngine.find(iFromUin))!=m_mapOutputAudioEngine.end()){
			iter->second->ClearPackets();
		}else{
			AssertVS(0,iFromUin,"CBufferOutputEngine::AddOutputPacket 实例不存在");
		}
	}else{
		MutexLocker locker(&m_mutexV);//播放线程 socket线程 m_mapOutputVideoEngine
		map<UInt32,RCPtr<CBufferOutputVideoEngine>>::iterator iter;
		if ((iter=m_mapOutputVideoEngine.find(iFromUin))!=m_mapOutputVideoEngine.end()){
			iter->second->ClearPackets();
		}else{
			AssertVS(0,iFromUin,"CBufferOutputEngine::AddOutputPacket 实例不存在");
		}
	}
}

void CBufferOutputEngine::SetRoomOnMic(bool bEnable)
{
	m_pIBaseAudioRate->SetMicStatus(bEnable);
	m_pIBaseVideoRate->SetMicStatus(bEnable);
}

void CBufferOutputEngine::ClearExpireUser()
{
	//清理音频无效对象
	{
		MutexLocker locker(&m_mutexA);//播放线程 socket线程 m_mapOutputAudioEngine
		map<UInt32,RCPtr<CBufferOutputAudioEngine>>::iterator iter;
		for (iter=m_mapOutputAudioEngine.begin();iter!=m_mapOutputAudioEngine.end();)
		{
			if (iter->second->IsCanRemove()){
				ErrTrace("删除对象CBufferOutputAudioEngine uin=%d。\n",iter->first);
				MutexLocker locker(&CBufferOutputAudioEngine::m_static_mutex);//网络线程 播放线程 m_mapAudioOutput
				iter = m_mapOutputAudioEngine.erase(iter);
				SwitchToThread();
				continue;
			}
			iter++;
		}
	}
	//清理无效视频对象
	{
		MutexLocker locker(&m_mutexV); //播放线程 socket线程 m_mapOutputVideoEngine
		map<UInt32,RCPtr<CBufferOutputVideoEngine>>::iterator iter;
		for (iter=m_mapOutputVideoEngine.begin();iter!=m_mapOutputVideoEngine.end();){
			if (iter->second->IsCanRemove()){
				ErrTrace("删除对象CBufferOutputVideoEngine uin=%d。\n",iter->first);
				iter = m_mapOutputVideoEngine.erase(iter);
				SwitchToThread();
				continue;
			}
			iter++;
		}
	}
}