#pragma  once

#include "AVChannel.h"
#include "VideoPacket.h"

#include <map>

typedef struct piece_set
{
	SInt64    lCreateTime;
	UInt32    iDataSize;
	UInt8     cTotal;
	UInt32    iKeySeqNum;
	UInt8     cKeyFrame;
	std::map<UInt8, VideoPiecePacket*>  piecesMap;

	piece_set():lCreateTime(0), iDataSize(0), cTotal(0), iKeySeqNum(0), cKeyFrame(0){}
	~piece_set()
	{
		std::map<UInt8, VideoPiecePacket*>::iterator iter=piecesMap.begin();
		for (; iter!=piecesMap.end(); iter++)
		{
			delete iter->second;
		}
		piecesMap.clear();
	}
} SPieceSet;


class VideoChannel : public AVChannel
{
public:

	VideoChannel(UInt32 iRoomId,UInt32 iDesUin, UInt8 cLogicID, HWND partnerHwnd,
		UInt32 iWidth, UInt32 iHeight, UInt32 iQuality);
	~VideoChannel();

private:

	virtual void              StartSelfAV()                             ;
	virtual void              StopSelfAV()                                ;
	virtual void              onReceive(const UInt8* pBuf, UInt32 iLen,unsigned int iSeq = 0) ;
	virtual void              onTimer();

	void                      processVideoPacket(Packet*);
	void                      processVideoPiecePacket(Packet*);

	bool                      addPiecePacket(VideoPiecePacket*);
	void                      checkPieceSet(UInt32 iSeqNum);
	void                      checkExpireSet();

private:
	HWND                              m_partnerHwnd;           //对方视频窗口句柄
	UInt32                            m_iWidth;                //对方的视频宽度
	UInt32                            m_iHeight;               //对方的视频高度
	UInt32                            m_iQuality;              //对方的编码质量

	Mutex							  m_mutex; //m_PieceSetMap 网络线程
	std::map<UInt32, SPieceSet*>      m_PieceSetMap;
};