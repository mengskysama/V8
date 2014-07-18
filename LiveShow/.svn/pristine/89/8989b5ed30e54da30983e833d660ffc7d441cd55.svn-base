#include "stdafx.h"
#include "AudioPacket.h"

Packet*  AudioPacket::makePacket(SInt32 iID, UInt16 sAudioType, UInt32 iDataLen,  UInt8* pData)
{
	Packet*  pPacket = BinaryPacket::makePacket(iID, AV_VERSION, sAudioType, iDataLen+MEDIA_PACKET_BODY_LEN);
	if (pPacket)
	{
		pPacket->writeUInt32(iDataLen);
		pPacket->writeByteArray(pData, iDataLen);
	}

	return pPacket;
}

Bool  AudioPacket::parserPacket(Packet* pPacket)
{
	Bool bRet = BinaryPacket::parserPacket(pPacket);
	if (!bRet) return bRet;

	bRet = pPacket->checkBytesLeft(MEDIA_PACKET_BODY_LEN);
	if (bRet)
	{
		m_iDataLen = pPacket->readUInt32();
		bRet = pPacket->checkBytesLeft(m_iDataLen);
		if (bRet)
		{
			m_pData = pPacket->getPaddingContentPtr();
		}
	}

	return bRet;
}