#pragma once

#include "SocketHandler.h"

class UdpSocketHandler : public SocketHandler
{
public:

	UdpSocketHandler(UInt32 iRoomId) : SocketHandler(), m_iRoomId(iRoomId){}
	~UdpSocketHandler(){}

	virtual void			onPacketRcved(Packet*);

private:
	void                    processAllRelay(Packet*);

private:
	UInt32                  m_iRoomId;
};

#define ALIVE_PACK_COUNT 3