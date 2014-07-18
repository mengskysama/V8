#pragma once
#ifdef _DEBUG
//#include <vld.h>
#endif
#include "AllPacketsMacro.h"
#include "CommonPacket.h"

#define KEEP_LIVE_ACK   0x0001

enum
{
	AV_CMD_KEEPALIVE = 0x01, //保持包
	AV_CMD_AUDIO     = 0x02, //音频数据包
	AV_CMD_VIDEO     = 0x03, //视频数据包
	AV_CMD_DESTROY   = 0x04,  //终止连接
	AV_CMD_GET_AUDIO = 0x05,  //获取音频丢包补偿包
	AV_CMD_GET_VIDEO = 0x06,  //获取视频丢包补偿包
	AV_CMD_ALIVE     = 0x07   //心跳维持包
};

#define RELAY_PACKET_BODY_LEN      16       


class RelayPacket
{
public:
	RelayPacket(void):m_sDataLen(0),m_iFromUin(0),m_iRoomId(0),m_iSequence(0){}
	~RelayPacket(void){}

	static  Packet*      makePacket(SInt32 iFd, UInt32 iFromUin, UInt32 iRoomId,UInt16 sCmdType,UInt32 iSequence,UInt16 sDataLen=0);

	virtual Bool         parserPacket(Packet*);
	inline  UInt16       getCmdType()	 { return m_sCmd;				}
	inline  UInt32       getFromUin()    { return m_iFromUin;           }
	inline  UInt32       getRoomId()     { return m_iRoomId;            }
	inline  UInt16		 getDataLen()	 { return m_sDataLen;			}
	inline  UInt32       getSequence()   { return m_iSequence;          }
protected:
	UInt16 m_sCmd;
	UInt32 m_iFromUin;
	UInt32 m_iRoomId;
	UInt16 m_sDataLen;//不包含datalen本身长度 仅仅包体的长度
	UInt32 m_iSequence;
};

#define RELAY_DATA_PACKET_BODY_LEN           16

class RelayDataPacket : public RelayPacket
{
public:

	RelayDataPacket() : RelayPacket() {}

	virtual ~RelayDataPacket() {}

	static  Packet*  makePacket(SInt32 iFd, UInt32 iFromUin, UInt32 iRoomId,UInt16 sCmdType,UInt32 iSequence,UInt16 sDataLen=0, UInt8* pData=NULL);
	static  Packet*  makePacket(SInt32 iFd, UInt16 sDataLen=0, UInt8* pData=NULL);

	virtual Bool     parserPacketToSelf(Packet* pPacket);
	
	inline  UInt8*   getData()          {   return m_pData;     }

private:
	UInt8* m_pData;
};
