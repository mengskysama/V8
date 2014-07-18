#pragma once

#include <map>
#include <vector>

#include "AVChannel.h"
class AVChannelMgr
{
public:
	static void Initialize();
	static void Destroy();
	static RCPtr<AVChannel> GetAVChannel(UInt32 iRoomId, UInt8 cLogicID);
	static bool AddAVChannel(RCPtr<AVChannel>& pChannel);
	static void Maintain();
private:
	static void DelAVChannel(UInt32 iRoomId, UInt8 cLogicID);
	static void GetAllAVChannel(std::vector<RCPtr<AVChannel> >& vAVChannels);

	static Mutex                                       s_Mutex; //房间线程 timer s_AVChannelMap
	static std::map<UInt64, RCPtr<AVChannel> >         s_AVChannelMap;

	static UInt32                                      s_iTimerID;

	typedef std::pair<UInt64, RCPtr<AVChannel> >       avchannel_pair;
};

#include "TimerHandler.h"

class AVChannelMaintainTimer : public TimerHandler
{
	virtual  void onPeriodExpired()
	{
		AVChannelMgr::Maintain();
	}
};



