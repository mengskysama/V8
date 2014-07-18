#pragma once

#include "CommonPacket.h"
#include "AVPacketMacros.h"


#define VIDEO_PACKET_BODY_LEN                   13

class VideoPacket : public BinaryPacket
{
public:

	VideoPacket() : BinaryPacket() {}
	virtual ~VideoPacket(){}

	static  Packet*          makePacket(SInt32 iID, UInt8 cKey, UInt32 iSeqNum, UInt32 iKeySeqNum, UInt32 iLen, UInt8* pData);
	virtual Bool             parserPacket(Packet* pPacket);

	inline bool              isKeyFrame()           { return (m_cKey&KEY_VALUE)==KEY_VALUE;      }
	inline UInt8             getKeyFrame()          { return m_cKey;		 }
	inline UInt32            getSeqNum()            { return m_iSeqNum;      }
	inline UInt32            getKeySeqNum()         { return m_iKeySeqNum;   }
	inline UInt32            getDataLen()           { return m_iDataLen;     }
	inline UInt8*            getData()              { return m_pData;        }
 
private:

	UInt8                      m_cKey;             //是否为关键帧                1B
	UInt32                     m_iSeqNum;          //序列号                      4B
	UInt32                     m_iKeySeqNum;       //所依附的关键帧的序号        4B
	UInt32                     m_iDataLen;         //视频数据的长度              4B
	UInt8*                     m_pData;            //视频数据
};

#define VIDEO_PIECE_PACKET_BODY_LEN                   15

class VideoPiecePacket : public BinaryPacket
{
public:

	VideoPiecePacket() : BinaryPacket(), m_pData(NULL) {}
	virtual ~VideoPiecePacket() 
	{
		SAFE_DELETEA(m_pData);
	}

	static  Packet*             makePacket(SInt32 iID, UInt8 cKey,  UInt32 iSeqNum, UInt32 iKeySeqNum,
		                                   UInt8 cPieceSum, UInt8 cPieceSeqNum, UInt32 iLen, UInt8* pData);

	virtual Bool                parserPacket(Packet* pPacket);

	inline bool                 isKeyFrame()           { return (m_cKey&KEY_VALUE)==KEY_VALUE;      }
	inline UInt8                getKeyType()		   { return m_cKey;		    }
	inline UInt32               getSeqNum()            { return m_iSeqNum;      }
	inline UInt32               getKeySeqNum()         { return m_iKeySeqNum;   }
	inline UInt8                getPieceSum()          { return m_cPieceSum;    }
	inline UInt8                getPieceSeqNum()       { return m_cPieceSeqNum; }
	inline UInt32               getDataLen()           { return m_iDataLen;     }
	inline UInt8*               getData()              { return m_pData;        }
	
private:

	UInt8                       m_cKey;             //是否为关键桢               1B
	UInt32                      m_iSeqNum;          //序列号                     4B
	UInt32                      m_iKeySeqNum;       //所依附的关键桢的序号       4B
	UInt8                       m_cPieceSum;        //分片的总数                 1B
	UInt8                       m_cPieceSeqNum;     //第几个分片                 1B
	UInt32                      m_iDataLen;         //视频数据的长度             4B
	UInt8*                      m_pData;            //视频数据
};