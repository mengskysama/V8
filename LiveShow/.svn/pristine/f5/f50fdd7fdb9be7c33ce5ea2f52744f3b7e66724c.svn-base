#include "stdafx.h"
#include "RelayPacket.h"
#include "MyAssert.h"

Packet* RelayPacket::makePacket(SInt32 iFd, UInt32 iFromUin, UInt32 iRoomId,UInt16 sCmdType,UInt32 iSequence,UInt16 sDataLen)
{
	Packet* pPacket = new Packet(iFd, RELAY_PACKET_BODY_LEN+sDataLen);
	if (pPacket)
	{
		pPacket->writeUInt16(sCmdType);
		pPacket->writeUInt32(iFromUin);
		pPacket->writeUInt32(iRoomId);
		pPacket->writeUInt16(sDataLen);
		pPacket->writeUInt32(iSequence);
	}

	return pPacket;
}

Bool  RelayPacket::parserPacket(Packet *pPacket)
{
	Bool bRet = 0;
	if (pPacket)
	{
		bRet = pPacket->checkBytesLeft(RELAY_PACKET_BODY_LEN);
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

//RelayDataPacket
Packet* RelayDataPacket::makePacket(SInt32 iFd, UInt32 iFromUin, UInt32 iRoomId,UInt16 sCmdType,UInt32 iSequence,UInt16 sDataLen, UInt8* pData)
{
	Packet* pPacket = RelayPacket::makePacket(iFd, iFromUin, iRoomId, sCmdType, iSequence,RELAY_DATA_PACKET_BODY_LEN-RELAY_PACKET_BODY_LEN+sDataLen);
	if (pPacket)
	{
		pPacket->writeByteArray(pData, sDataLen);
	}

	return pPacket;
}

Packet*  RelayDataPacket::makePacket(SInt32 iFd, UInt16 sDataLen, UInt8* pData)
{
	Packet* pPacket = new Packet(iFd, sDataLen);
	if (pPacket)
	{
		pPacket->writeByteArray(pData, sDataLen);
	}
	return pPacket;
}

Bool RelayDataPacket::parserPacketToSelf(Packet *pPacket)
{
	Bool bRet = pPacket->checkBytesLeft(RELAY_DATA_PACKET_BODY_LEN);
	if (bRet)
	{
		m_sCmd		  =  pPacket->readUInt16();
		m_iFromUin    =  pPacket->readUInt32();
		m_iRoomId     =  pPacket->readUInt32();
		m_sDataLen    =  pPacket->readUInt16();
		m_iSequence  =   pPacket->readUInt32();
		if (m_sDataLen<=0)
		{
			m_pData = NULL;
			return bRet;
		}
		bRet = pPacket->checkBytesLeft(m_sDataLen);
		if (bRet)
		{
			m_pData = pPacket->getPaddingContentPtr();
		}
		else
		{
			Assert(0);
		}
	}
	else
	{
		Assert(0);
	}

	return bRet;
}

