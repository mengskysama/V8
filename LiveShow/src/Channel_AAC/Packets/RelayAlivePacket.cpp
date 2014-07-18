#include "StdAfx.h"
#include "RelayAlivePacket.h"
#include "MyAssert.h"

Packet* RelayAlivePacket::makePacket(SInt32 iFd, UInt32 iFromUin, UInt32 iRoomId,UInt32 ikey)
{
	Packet* pPacket = new Packet(iFd, RELAY_ALIVE_PACKET_BODY_LEN);
	if (pPacket)
	{
		pPacket->writeUInt16(AV_CMD_KEEPALIVE);
		pPacket->writeUInt32(iFromUin);
		pPacket->writeUInt32(iRoomId);
		pPacket->writeUInt16(0);
		pPacket->writeUInt32(ikey);
	}

	return pPacket;
}


Bool  RelayAlivePacket::parserPacket(Packet* pPacket)
{
	Bool bRet = 0;
	if (pPacket)
	{
		bRet = pPacket->checkBytesLeft(RELAY_ALIVE_PACKET_BODY_LEN);
		if (bRet)
		{
			m_sCmd		  =  pPacket->readUInt16();
			m_iFromUin    =  pPacket->readUInt32();
			m_iRoomId     =  pPacket->readUInt32();
			m_sDataLen    =  pPacket->readUInt16();
			m_iSequence   =  pPacket->readUInt32();
		}
		else
		{
			Assert(0);
		}
	}

	return bRet;
}
