#include "stdafx.h"
#include "AVChannelMgr.h"
#include "TimersManager.h"
#include "RelayManger.h"

Mutex                                       AVChannelMgr::s_Mutex;//房间线程 timer s_AVChannelMap
std::map<UInt64, RCPtr<AVChannel> >         AVChannelMgr::s_AVChannelMap;
UInt32                                      AVChannelMgr::s_iTimerID = 0;

void   AVChannelMgr::Initialize()
{
	RelayManger::Initialize();
	AVChannelMaintainTimer* pTimer = new AVChannelMaintainTimer();
	TimersManager* pTimersManager   = TimersManager::getInstance();
	Assert(pTimersManager!=NULL);
	s_iTimerID= pTimersManager->registerTimerHandler(0, 100, pTimer);
	Assert(s_iTimerID>0);
}

void  AVChannelMgr::Destroy()
{
	{
		std::vector<RCPtr<AVChannel> > vChannels;
		GetAllAVChannel(vChannels);
		for (UInt32 i=0; i<vChannels.size(); ++i)
		{
			vChannels[i]->UserCanncel();
		}
	}

	::Sleep(100);

	if (s_iTimerID>0)
	{
		ErrTrace("AVChannelMgr::Destroy() ……  removeTimerHandler %d\n",GetTickCount());
		Assert(0);
		TimersManager* pTimersManager   = TimersManager::getInstance();
		pTimersManager->removeTimerHandler(s_iTimerID);
		s_iTimerID = 0;
	}
}

RCPtr<AVChannel>  AVChannelMgr::GetAVChannel(UInt32 iRoomId, UInt8 cLogicID)
{
	UInt64 lChannelID = iRoomId;
	lChannelID = (lChannelID<<32) + cLogicID;

	std::map<UInt64, RCPtr<AVChannel> >::iterator iter;
	MutexLocker locker(&s_Mutex);//房间线程 timer s_AVChannelMap
	iter = s_AVChannelMap.find(lChannelID);
	if (iter!=s_AVChannelMap.end())
	{
		return iter->second;
	}

	return RCPtr<AVChannel>(NULL);
}


void  AVChannelMgr::GetAllAVChannel(std::vector<RCPtr<AVChannel> >& vAVChannels)
{
	std::map<UInt64, RCPtr<AVChannel> >::iterator iter;
	MutexLocker locker(&s_Mutex);//房间线程 timer s_AVChannelMap
	for (iter=s_AVChannelMap.begin();iter!=s_AVChannelMap.end(); ++iter)
	{
		vAVChannels.push_back(iter->second);
	}
}

bool  AVChannelMgr::AddAVChannel(RCPtr<AVChannel>& pChannel)
{
	UInt64 lChannelID = pChannel->GetRoomId();
	lChannelID = (lChannelID<<32) + pChannel->GetLogicID();

	Trace("AVChannelMgr::AddAVChannel[DesUin = %d, cLogicID = %d] \n", pChannel->GetRoomId(), pChannel->GetLogicID());

	MutexLocker locker(&s_Mutex);//房间线程 timer s_AVChannelMap

	if (s_AVChannelMap.find(lChannelID)==s_AVChannelMap.end())
	{
		s_AVChannelMap.insert(avchannel_pair(lChannelID, pChannel));
		return true;
	}

	Assert(0);
	return false;
}

void  AVChannelMgr::Maintain()
{
	std::vector<RCPtr<AVChannel> > vAVChannels;
	GetAllAVChannel(vAVChannels);

	for (UInt32 i=0; i<vAVChannels.size(); ++i)
	{
		if (!vAVChannels[i]->Maintain())
		{
			DelAVChannel(vAVChannels[i]->GetRoomId(), vAVChannels[i]->GetLogicID());
		}
	}

}

void  AVChannelMgr::DelAVChannel(UInt32 iRoomId, UInt8 cLogicID)
{
	UInt64 lChannelID = iRoomId;
	lChannelID = (lChannelID<<32) + cLogicID;

	std::map<UInt64, RCPtr<AVChannel> >::iterator iter;
	MutexLocker locker(&s_Mutex);//房间线程 timer s_AVChannelMap
	iter = s_AVChannelMap.find(lChannelID);
	if (iter!=s_AVChannelMap.end())
	{
		s_AVChannelMap.erase(iter);
	}
}
