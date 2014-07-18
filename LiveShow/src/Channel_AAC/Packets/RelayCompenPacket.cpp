#include "stdafx.h"
#include "RelayCompenPacket.h"
#include "MyAssert.h"
#include "RelayPacket.h"

Packet* RelayCompenPacket::makePacket(SInt32 iFd,UInt32 iRoomId,UInt32 iFromUin, UInt32 iSequenceId,bool bA)
{
	Packet* pPacket = new Packet(iFd, RELAY_COMPEN_PACKET_BODY_LEN);
	if (pPacket)
	{
		pPacket->writeUInt16(bA?AV_CMD_GET_AUDIO:AV_CMD_GET_VIDEO);
		pPacket->writeUInt32(iFromUin);
		pPacket->writeUInt32(iRoomId);
		pPacket->writeUInt16(0);
		pPacket->writeUInt32(iSequenceId);
	}

	return pPacket;
}


Bool  RelayCompenPacket::parserPacket(Packet* pPacket)
{
	Bool bRet = 0;
	if (pPacket)
	{
		bRet = pPacket->checkBytesLeft(RELAY_COMPEN_PACKET_BODY_LEN);
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
