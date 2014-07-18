#include "stdafx.h"
#include "VideoPacket.h"

Packet*  VideoPacket::makePacket(SInt32 iID, UInt8 cKey, UInt32 iSeqNum, UInt32 iKeySeqNum, UInt32 iLen, UInt8* pData)
{
	Packet *pPacket = BinaryPacket::makePacket(iID, AV_VERSION, VIDEO_PACKET_TYPE, VIDEO_PACKET_BODY_LEN+iLen);
	if (pPacket)
	{
		pPacket->writeUInt8(cKey);
		pPacket->writeUInt32(iSeqNum);
		pPacket->writeUInt32(iKeySeqNum);
		pPacket->writeUInt32(iLen);
		pPacket->writeByteArray(pData, iLen);
	}

	return pPacket;
}

Bool  VideoPacket::parserPacket(Packet* pPacket)
{
	Bool bRet = BinaryPacket::parserPacket(pPacket);
	if (!bRet) return bRet;

	bRet = pPacket->checkBytesLeft(VIDEO_PACKET_BODY_LEN);
	if (bRet)
	{
		m_cKey    = pPacket->readUInt8();
		m_iSeqNum = pPacket->readUInt32();
		m_iKeySeqNum = pPacket->readUInt32();
		m_iDataLen   = pPacket->readUInt32();
		
		bRet = pPacket->checkBytesLeft(m_iDataLen);
		if (bRet)
		{
			m_pData = pPacket->getPaddingContentPtr();
		}
	}

	return bRet;
}

Packet*  VideoPiecePacket::makePacket(SInt32 iID, UInt8 cKey, UInt32 iSeqNum, UInt32 iKeySeqNum, 
									  UInt8 cPieceSum, UInt8 cPieceSeqNum, UInt32 iLen, UInt8 *pData)
{
	Packet* pPacket = BinaryPacket::makePacket(iID, AV_VERSION, VIDEO_PIECE_PACKET_TYPE, VIDEO_PIECE_PACKET_BODY_LEN+iLen);
	if (pPacket)
	{
		pPacket->writeUInt8(cKey);
		pPacket->writeUInt32(iSeqNum);
		pPacket->writeUInt32(iKeySeqNum);
		pPacket->writeUInt8(cPieceSum);
		pPacket->writeUInt8(cPieceSeqNum);
		pPacket->writeUInt32(iLen);
		pPacket->writeByteArray(pData, iLen);
	}

	return pPacket;
}

Bool  VideoPiecePacket::parserPacket(Packet* pPacket)
{
	Bool bRet = BinaryPacket::parserPacket(pPacket);
	if (!bRet) return bRet;

	bRet = pPacket->checkBytesLeft(VIDEO_PIECE_PACKET_BODY_LEN);
	if (bRet)
	{
		m_cKey    = pPacket->readUInt8();
		m_iSeqNum = pPacket->readUInt32();
		m_iKeySeqNum = pPacket->readUInt32();
		m_cPieceSum  = pPacket->readUInt8();
		m_cPieceSeqNum = pPacket->readUInt8();
		m_iDataLen   = pPacket->readUInt32();

		bRet = pPacket->checkBytesLeft(m_iDataLen);
		if (bRet)
		{
			m_pData = new UInt8[m_iDataLen];
			::memcpy(m_pData, pPacket->getPaddingContentPtr(), m_iDataLen);
		}
	}

	return bRet;
}