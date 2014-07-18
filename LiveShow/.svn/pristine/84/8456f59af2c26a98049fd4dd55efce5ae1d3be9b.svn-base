#pragma once

#include "CommonPacket.h"
#include "AVPacketMacros.h"

#define  MEDIA_PACKET_BODY_LEN             4

class AudioPacket : BinaryPacket
{
public:

	AudioPacket() : BinaryPacket(), m_iDataLen(0), m_pData(NULL) {}

	virtual ~AudioPacket() {}

	static  Packet*                makePacket(SInt32 iID, UInt16 sAudioType, UInt32 iDataLen, UInt8* pData);
	virtual Bool                   parserPacket(Packet* pPacket);

	inline  UInt16                 getAudioType()  {  return getPacketType(); }
	inline  UInt32                 getDataLen()    {  return m_iDataLen;      }
	inline  UInt8*                 getData()       {  return m_pData;         }

private:

	UInt32                     m_iDataLen;
	UInt8*                     m_pData;
};