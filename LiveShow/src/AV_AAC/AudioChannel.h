#pragma once

#include "AVChannel.h"

class AudioChannel : public AVChannel
{
public:

	AudioChannel(UInt32 iRoomId, UInt32 iDesUin, UInt8 cLogicID);
	~AudioChannel();

private:

	virtual void              StartSelfAV()                               ;
	virtual void              StopSelfAV()                                ;
	virtual void              onReceive(const UInt8* pBuf, UInt32 iLen, unsigned int iSeq) ;

};