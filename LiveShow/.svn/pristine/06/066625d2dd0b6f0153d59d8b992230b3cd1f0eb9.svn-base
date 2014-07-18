#pragma once
#include "CommonPacket.h"
#include "AllPacketsMacro.h"

/************************************************************************/
/* 向服务端请求补偿包命令                                               */
/************************************************************************/

#define RELAY_COMPEN_PACKET_BODY_LEN            16

class RelayCompenPacket : public BinaryPacket
{
public:
	RelayCompenPacket(void):BinaryPacket(),m_sDataLen(0),m_iFromUin(0),m_iRoomId(0),m_iSequence(0){}
	~RelayCompenPacket(void){}

	static  Packet*      makePacket(SInt32 iFd,UInt32 iRoomId,UInt32 iFromUin, UInt32 iSequenceId,bool bA = false);

	virtual Bool         parserPacket(Packet*);

	inline  UInt32       getFromUin()    { return m_iFromUin;           }
	inline  UInt32       getRoomId()     { return m_iRoomId;            }

private:
	UInt16 m_sCmd;
	UInt32 m_iFromUin;
	UInt32 m_iRoomId;
	UInt16 m_sDataLen;//不包含datalen本身长度 仅仅包体的长度
	UInt32 m_iSequence;
};
