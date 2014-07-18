#include "stdafx.h"
#include "VideoChannel.h"
#include "AVEngine.h"
#include "TimeUtils.h"

#define  PIECE_SET_EXPIRE_TIME             3000

VideoChannel::VideoChannel(UInt32 iRoomId,UInt32 iDesUin, UInt8 cLogicID, HWND partnerHwnd,
						   UInt32 iWidth, UInt32 iHeight, UInt32 iQuality)
						   :AVChannel(iRoomId, iDesUin, cLogicID,false),
						   m_partnerHwnd(partnerHwnd),
						   m_iWidth(iWidth),
						   m_iHeight(iHeight),
						   m_iQuality(iQuality)
{
}

VideoChannel::~VideoChannel()
{
	MutexLocker locker(&m_mutex);//m_PieceSetMap 网络线程
	std::map<UInt32, SPieceSet*>::iterator iter=m_PieceSetMap.begin();
	for (; iter!=m_PieceSetMap.end(); iter++)
	{
		delete iter->second;
	}
	m_PieceSetMap.clear();
}

void  VideoChannel::StartSelfAV() //接收视频不应该干预发送视频
{
	//g_AVEngine.AddVideoLogicChannel(m_iRoomId, m_selfHwnd);
}

void  VideoChannel::StopSelfAV()
{
	//g_AVEngine.DelVideoLogicChannel(m_iRoomId, m_selfHwnd);
}

void  VideoChannel::onTimer()
{
	checkExpireSet();
}

void  VideoChannel::onReceive(const UInt8* pBuf, UInt32 iLen,unsigned int iSeq)
{
	if (m_bUserCancel) 
	{
		ErrTrace("VideoChannel::onReceive 已经取消操作....正在销毁...\n");
		return;
	}
		
	Packet tempPacket(-1);
	tempPacket.setContentPtr(const_cast<UInt8*>(pBuf), iLen);

	UInt16  sPacketType = ntohs(*(UInt16*)(&pBuf[2]));
	switch (sPacketType)
	{
	case VIDEO_PACKET_TYPE:
		processVideoPacket(&tempPacket);
		break;
	case VIDEO_PIECE_PACKET_TYPE:
		processVideoPiecePacket(&tempPacket);
		break;
	default:
		Assert(0);
		break;
	}

	tempPacket.setContentPtr(NULL, 0);
}

void  VideoChannel::processVideoPacket(Packet* pPacket)
{
	VideoPacket videoPacket;
	if (!videoPacket.parserPacket(pPacket))
	{
		Assert(0);
		return;
	}

 	Trace("VideoChannel:接收到完整视频帧[关键帧=%d, iSeqNum=%d, KeySeqNum=%d, Len=%d] time = %I64d\n",videoPacket.isKeyFrame()?1:0,videoPacket.getSeqNum(),videoPacket.getKeySeqNum(),videoPacket.getDataLen(),GetTickCount());
	g_AVEngine.RecvVideoData(m_iRoomId,m_iFromUin, videoPacket.getData(), videoPacket.getDataLen(), videoPacket.getKeyFrame(), videoPacket.getSeqNum(), videoPacket.getKeySeqNum());
}

void  VideoChannel::processVideoPiecePacket(Packet* pPacket)
{
	VideoPiecePacket* pPiecePacket = new VideoPiecePacket;
	if (!pPiecePacket->parserPacket(pPacket))
	{
		Assert(0);
		return;
	}

	if (addPiecePacket(pPiecePacket))
	{
		checkPieceSet(pPiecePacket->getSeqNum());
	}
}

bool  VideoChannel::addPiecePacket(VideoPiecePacket* pPiecePacket)
{
	UInt32 iSeqNum      = pPiecePacket->getSeqNum();
	UInt8  cPieceSeqNum = pPiecePacket->getPieceSeqNum();

	std::map<UInt32, SPieceSet*>::iterator setIter;
	setIter = m_PieceSetMap.find(iSeqNum);
	if (setIter==m_PieceSetMap.end())
	{
		//构建PieceSet
		SPieceSet* pPieceSet = new SPieceSet();

		pPieceSet->cTotal                  = pPiecePacket->getPieceSum();
		pPieceSet->iDataSize               = pPiecePacket->getDataLen();
		pPieceSet->lCreateTime             = TimeUtils::Milliseconds();
		pPieceSet->iKeySeqNum              = pPiecePacket->getKeySeqNum();
		pPieceSet->cKeyFrame               = pPiecePacket->getKeyType();
		pPieceSet->piecesMap[cPieceSeqNum] = pPiecePacket;

		MutexLocker locker(&m_mutex);//m_PieceSetMap 网络线程
		m_PieceSetMap[iSeqNum] = pPieceSet;

		return false;           
	}
	else
	{
		SPieceSet* pPieceSet = setIter->second;

		Assert(pPieceSet->cTotal     == pPiecePacket->getPieceSum());
		Assert(pPieceSet->iKeySeqNum == pPiecePacket->getKeySeqNum());
		Assert(pPieceSet->cKeyFrame  == pPiecePacket->getKeyType());

		std::map<UInt8, VideoPiecePacket*>::iterator pieceIter;
		pieceIter = pPieceSet->piecesMap.find(cPieceSeqNum);
		if (pieceIter==pPieceSet->piecesMap.end())
		{
			pPieceSet->piecesMap[cPieceSeqNum] = pPiecePacket;
			pPieceSet->iDataSize += pPiecePacket->getDataLen();
		}
		else
		{
			//已经存在
			Assert(0);
			delete pPiecePacket;
			pPiecePacket = NULL;
			return false;
		}
	}

	return true;
}

void  VideoChannel::checkPieceSet(UInt32 iSeqNum)
{
	MutexLocker locker(&m_mutex);//m_PieceSetMap 网络线程
	if(m_PieceSetMap.find(iSeqNum) == m_PieceSetMap.end()) return;
	SPieceSet* pPieceSet = m_PieceSetMap[iSeqNum];
	if (pPieceSet->piecesMap.size()==pPieceSet->cTotal)
	{
		//该所有的分片已经接受完
		UInt32 iDataLen = 0;
		UInt8* pVideoData = new UInt8[pPieceSet->iDataSize];
		bool   bDataVaild = true;

		for (UInt8 iPieceSeqNum=1; iPieceSeqNum<=pPieceSet->cTotal; ++iPieceSeqNum)
		{
			std::map<UInt8, VideoPiecePacket*>::iterator pieceIter = pPieceSet->piecesMap.find(iPieceSeqNum);
			if (pieceIter!=pPieceSet->piecesMap.end())
			{
				VideoPiecePacket* pTempPacket = pieceIter->second;
				if (pTempPacket)
				{
					//ErrTrace("当前组装的视频数据[关键帧=%d, iSeqNum=%d, iSubSeqNum=%d, KeySeqNum=%d, DataLen=%d] time = %I64d\n", (pTempPacket->isKeyFrame()?1:0), pTempPacket->getSeqNum(), pTempPacket->getPieceSeqNum(), pTempPacket->getKeySeqNum(),pTempPacket->getDataLen(),GetTickCount());
					if(iDataLen+pTempPacket->getDataLen() > pPieceSet->iDataSize)
					{
						Assert(0);
#ifndef IGNOR_INT_3
						_asm int 3;
#endif
					}
					::memcpy(pVideoData+iDataLen, pTempPacket->getData(), pTempPacket->getDataLen());
					iDataLen += pTempPacket->getDataLen();

					pTempPacket = NULL;
				}
			}
			else
			{
				Assert(0);
				ErrTrace("当前组装的视频数据出错****************************************************************************************** time = %I64d\n",GetTickCount());
				bDataVaild = false;
				break;
			}
		}

		if (bDataVaild && iDataLen==pPieceSet->iDataSize)
		{
			Trace("当视频数据包[关键帧=%d, iSeqNum=%d, KeySeqNum=%d, DataLen=%d] time = %I64d\n", ((pPieceSet->cKeyFrame&KEY_VALUE)==KEY_VALUE?1:0), iSeqNum, pPieceSet->iKeySeqNum,iDataLen,GetTickCount());
			g_AVEngine.RecvVideoData(m_iRoomId,m_iFromUin, pVideoData, iDataLen, pPieceSet->cKeyFrame, iSeqNum, pPieceSet->iKeySeqNum);
		}
		else
		{
			Assert(0);
#ifndef IGNOR_INT_3
			_asm int 3;
#endif
		}

		delete[] pVideoData;
		pVideoData = NULL;

		//清除存放Piece的Set
		delete pPieceSet;
		pPieceSet = NULL;
		m_PieceSetMap.erase(m_PieceSetMap.find(iSeqNum));
	}
}

void  VideoChannel::checkExpireSet()
{
	MutexLocker locker(&m_mutex);//m_PieceSetMap 网络线程
	SInt64 lCurTime = TimeUtils::Milliseconds();
	std::map<UInt32, SPieceSet*>::iterator iter=m_PieceSetMap.begin();
	for(; iter!=m_PieceSetMap.end();)
	{
		SPieceSet* pPieceSet = iter->second;
		if (lCurTime - pPieceSet->lCreateTime > PIECE_SET_EXPIRE_TIME )
		{
			delete pPieceSet;
			pPieceSet = NULL;
			m_PieceSetMap.erase(iter++);
		}
		else
		{
			++iter;
		}
	}
}