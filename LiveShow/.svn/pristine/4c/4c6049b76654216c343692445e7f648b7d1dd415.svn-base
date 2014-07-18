#pragma once

#include "TimerHandler.h"
#include "ChannelManager.h"

class ChannelMaintainTimer : public TimerHandler
{
public:

	ChannelMaintainTimer():TimerHandler(){}

	virtual ~ChannelMaintainTimer(){}

	virtual  void onPeriodExpired()
	{
		RCPtr<ChannelManager> pInstance = ChannelManager::Instance();
		if (pInstance)
		{
			pInstance->ProcessCmdPacket();
			pInstance->Maintain();
		}
	}
};