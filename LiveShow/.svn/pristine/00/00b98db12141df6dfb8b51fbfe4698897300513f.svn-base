#include "stdafx.h"
#include "BufferInputVideoEngine.h"
#include "RelayPacket.h"
#include "Packets/AVPacketMacros.h"
#include "Packets/VideoPacket.h"

CBufferInputVideoEngine::CBufferInputVideoEngine()
{
		m_minSequence = 0;
		m_maxSequence = 0;
		m_minFrameSeq = 0;
		m_maxFrameSeq = 0;
		m_minFrameInterval = 0;
		m_maxFrameInterval = 0;
		m_keepFrameNum =BUFFER_PERSEC_VIDEO_FRAMES*BUFFER_INPUT_VIDEO_LIMIT /1000;
		m_frameToSeqMap.clear();
		BufferTrace("CBufferInputEngine::CBufferInputVideoEngine() begin\n");
}

CBufferInputVideoEngine::~CBufferInputVideoEngine()
{
		BufferTrace("CBufferInputVideoEngine::~CBufferInputVideoEngine() begin\n");
		ClearPackets();
}

void	CBufferInputVideoEngine::Initialize()
{
		BufferTrace("CBufferInputVideoEngine::Initialize() begin\n");
}

void	CBufferInputVideoEngine::UnInitialize()
{
		BufferTrace("CBufferInputVideoEngine::UnInitialize() begin\n");
		ClearPackets();
}

bool	CBufferInputVideoEngine::GetEngineUsed()//获取对象是否在使用
{
		BufferTrace("CBufferInputVideoEngine::GetEngineUsed() begin\n");
		return !(m_minSequence == 0 && m_maxSequence==0);
}

void	CBufferInputVideoEngine::AddPacket(Packet* pDataPacket,UInt32 iSequence)
{
		BufferTrace("CBufferInputVideoEngine::AddPacket() begin\n");
// 		static unsigned int s = 0;
// 		if (s++ % 10 == 0)
// 		{
// 			pDataPacket->RemoveReference();
// 			return;
// 		}

		if (m_videoBufferMap.size() >BUFFER_OUTPUT_VIDEO_PACKET_MAXLIMIT)
		{
			pDataPacket->RemoveReference();
			Assert(0);
			ErrTrace("CBufferInputVideoEngine::AddPacket() 缓冲区长度超过限制.m_minSequence=%d,m_maxSequence=%d.....\n",m_minSequence,m_maxSequence);
			return;
		}


		UInt32 iFrameSeq = 0;//对保存的数据包拆包，获取帧序号
		if (!GetFrameSeq(pDataPacket,iFrameSeq))
		{
			Assert(0);
			ErrTrace("CBufferInputVideoEngine::AddPacket() 拆包失败......\n");
			return;
		}

		MutexLocker locker(&m_mutex);
		StructVideoBufferPack sPack;
		sPack.m_pPack = pDataPacket;
		sPack.m_iSequence = iSequence;
		sPack.m_time = GetTickCount();
		sPack.m_iFrameSeq = iFrameSeq;
		m_videoBufferMap[iSequence] = sPack; //保存的包为发送到网络上的数据包
		AddSequence(iSequence);
		AddFrameSeq(iFrameSeq);
		
		map<UInt32,StructVideoFrameSeqToSeq>::iterator iter;
		if ((iter=m_frameToSeqMap.find(iFrameSeq))!=m_frameToSeqMap.end())
		{
			if (iSequence>iter->second.m_maxSequence)
			{
				iter->second.m_maxSequence = iSequence;
			}
		}
		else
		{
			StructVideoFrameSeqToSeq struSeq;
			struSeq.m_minSequence = iSequence;
			struSeq.m_maxSequence = iSequence;
			m_frameToSeqMap[iFrameSeq] = struSeq;
		}
}

void	CBufferInputVideoEngine::ClearPackets()
{
		BufferTrace("CBufferInputVideoEngine::ClearPackets() begin\n");
		MutexLocker locker(&m_mutex);
		map<UInt32,StructVideoBufferPack>::iterator iter=m_videoBufferMap.begin();
		while (iter!=m_videoBufferMap.end())
		{
			iter->second.m_pPack->RemoveReference();
			iter=m_videoBufferMap.erase(iter);
		}
		m_minSequence = 0;
		m_maxSequence = 0;
		m_minFrameSeq = 0;
		m_maxFrameSeq = 0;
		m_minFrameInterval = 0;
		m_maxFrameInterval = 0;
		m_frameToSeqMap.clear();
}

void	CBufferInputVideoEngine::AddSequence(UInt32 iSequence)
{
		BufferTrace("CBufferInputVideoEngine::AddSequence() begin\n");
		if (iSequence<m_minSequence || m_minSequence<=0)
		{
			if (m_minFrameInterval==0)
				m_minFrameInterval = GetTickCount();
			m_minSequence = iSequence;
		}

		m_maxSequence = iSequence;
		m_maxFrameInterval = GetTickCount();
}

void	CBufferInputVideoEngine::AddFrameSeq(UInt32 iFrameSeq)
{
		BufferTrace("CBufferInputVideoEngine::AddFrameSeq() begin\n");
		if (iFrameSeq<m_minFrameSeq || m_minFrameSeq==0)
		{
			m_minFrameSeq = iFrameSeq;
		}
		m_maxFrameSeq = iFrameSeq;
}

UInt32	CBufferInputVideoEngine::GetPackCount()
{
		BufferTrace("CBufferInputVideoEngine::GetPackCount() begin\n");
		return (UInt32)m_videoBufferMap.size();
}

bool	CBufferInputVideoEngine::ParsePack(const UInt8* pBuf, UInt32 iLen,UInt32& iFrameSeq)
{
		BufferTrace("CBufferInputVideoEngine::ParsePack() begin\n");
		Packet tempPacket(-1);
		tempPacket.setContentPtr(const_cast<UInt8*>(pBuf), iLen);
		
		bool bSucc = false;
		UInt16  sPacketType = ntohs(*(UInt16*)(&pBuf[2]));
		switch (sPacketType)
		{
		case VIDEO_PACKET_TYPE:
			ParseAllPack(&tempPacket,iFrameSeq,bSucc);
			break;
		case VIDEO_PIECE_PACKET_TYPE:
			ParsePiecePack(&tempPacket,iFrameSeq,bSucc);
			break;
		default:
			Assert(0);
			break;
		}

		tempPacket.setContentPtr(NULL, 0);
		return bSucc;
}

void	CBufferInputVideoEngine::ParseAllPack(Packet* pPacket,UInt32& iFrameSeq,bool& bSucc)
{
		BufferTrace("CBufferInputVideoEngine::ParseAllPack() begin\n");
		VideoPacket videoPacket;
		if (!videoPacket.parserPacket(pPacket))
		{
			Assert(0);
			ErrTrace("");
			bSucc = false;
			return;
		}

		iFrameSeq = videoPacket.getSeqNum();
		bSucc = true;
		return;
}

void	CBufferInputVideoEngine::ParsePiecePack(Packet* pPacket,UInt32& iFrameSeq,bool& bSucc)
{
		BufferTrace("CBufferInputVideoEngine::ParsePiecePack() begin\n");
		VideoPiecePacket PiecePacket;
		if (!PiecePacket.parserPacket(pPacket))
		{
			Assert(0);
			bSucc = false;
			return;
		}

		iFrameSeq = PiecePacket.getSeqNum();
		bSucc = true;
		return;
}

bool	CBufferInputVideoEngine::GetFrameSeq(Packet* pDataPacket,UInt32& iFrameSeq)
{
		BufferTrace("CBufferInputVideoEngine::GetFrameSeq() begin\n");
		RelayDataPacket dataPacket;
		if (!dataPacket.parserPacketToSelf(pDataPacket))
		{
			Assert(0);
			return false;
		}
		
		return ParsePack(dataPacket.getData(),dataPacket.getDataLen(),iFrameSeq);
}

bool	CBufferInputVideoEngine::GetPacket(Packet* &pDataPacket,UInt32 iSequence)
{
		BufferTrace("CBufferInputVideoEngine::GetPacket() begin\n");
		MutexLocker locker(&m_mutex);
		map<UInt32,StructVideoBufferPack>::iterator iter=m_videoBufferMap.find(iSequence);
		if (iter!=m_videoBufferMap.end())
		{
			pDataPacket = iter->second.m_pPack;
			pDataPacket->AddReference();
			return true;
		}
		return false;
}

void	CBufferInputVideoEngine::KeepBalance()
{
		BufferTrace("CBufferInputVideoEngine::KeepBalance() 外部 m_minSequence=%d,m_maxSequence=%d,m_keepFrameNum=%d,m_videoBufferMap.size()=%d,m_frameToSeqMap.size()=%d\n",m_minSequence,m_maxSequence,m_keepFrameNum,m_videoBufferMap.size(),m_frameToSeqMap.size());
		MutexLocker locker(&m_mutex);
		if ((m_maxFrameSeq - m_minFrameSeq)>m_keepFrameNum)
		{
			int i = 0;
			while ((m_maxFrameSeq - m_minFrameSeq)>m_keepFrameNum && i++ <10)
			{
				DWORD dwStart = GetTickCount();
				RemoveFrame(m_minFrameSeq);
				IncFrameSeq(m_minFrameSeq);
				BufferTrace("CBufferInputVideoEngine::KeepBalance() 内部 m_minSequence=%d,m_maxSequence=%d,m_keepFrameNum=%d,m_videoBufferMap.size()=%d,m_frameToSeqMap.size()=%d,m_minFrameSeq=%d,m_maxFrameSeq=%d\n",m_minSequence,m_maxSequence,m_keepFrameNum,m_videoBufferMap.size(),m_frameToSeqMap.size(),m_minFrameSeq,m_maxFrameSeq);
				SwitchToThread();
			}
		}

		RemoveTimeOut();
}

void	CBufferInputVideoEngine::RemoveFrame(UInt32 iFrameSeq)
{
		BufferTrace("CBufferInputVideoEngine::RemoveFrame() begin\n");
		map<UInt32,StructVideoFrameSeqToSeq>::iterator iter;
		if ((iter = m_frameToSeqMap.find(iFrameSeq))!=m_frameToSeqMap.end())
		{
			StructVideoFrameSeqToSeq strucSeq = iter->second;
			if (strucSeq.m_minSequence>0 && strucSeq.m_maxSequence>strucSeq.m_minSequence)
			{
				UInt32 iCurSeq = m_minSequence;
				do 
				{
					BufferTrace("CBufferInputVideoEngine::RemoveFrame() while\n");
					map<UInt32,StructVideoBufferPack>::iterator iter1=m_videoBufferMap.find(iCurSeq);
					if (iter1!=m_videoBufferMap.end())
					{
						AssertVS(iter1->second.m_pPack->GetReference()==1,iter1->second.m_pPack->GetReference(),"iter1->second.m_pPack->GetReference()==1");
						iter1->second.m_pPack->RemoveReference();
						m_videoBufferMap.erase(iter1);
					}

				} while (++iCurSeq<=strucSeq.m_maxSequence);

				m_minSequence = strucSeq.m_maxSequence+1;
			}
			m_frameToSeqMap.erase(iter);
		}
		else
		{
			AssertVS(0,iFrameSeq,"CBufferInputVideoEngine::RemoveFrame 数据帧不存在");
		}
}

void	CBufferInputVideoEngine::IncFrameSeq(UInt32& iFrameSeq)
{
		BufferTrace("CBufferInputVideoEngine::IncFrameSeq() while\n");
		map<UInt32,StructVideoFrameSeqToSeq>::iterator iter;
		if ((iter = m_frameToSeqMap.find(++iFrameSeq))!=m_frameToSeqMap.end())
		{
			map<UInt32,StructVideoBufferPack>::iterator iter1=m_videoBufferMap.find(iter->second.m_minSequence);
			if (iter1!=m_videoBufferMap.end())
			{
				m_minFrameInterval = iter1->second.m_time;
			}
			else
			{
				AssertVS(0,iFrameSeq,"CBufferInputVideoEngine::IncFrameSeq 数据sequenceid不存在");
			}
		}
		else
		{
			AssertVS(0,iFrameSeq,"CBufferInputVideoEngine::IncFrameSeq 数据帧不存在");
		}
}

void	CBufferInputVideoEngine::RemoveTimeOut()
{
		BufferTrace("CBufferInputAudioEngine::RemoveTimeOut()  m_minFrameInterval=%d,m_maxFrameInterval=%d,interval=%d,minus=%d,limit=%d\n",m_minFrameInterval,m_maxFrameInterval,m_maxFrameInterval- m_minFrameInterval,GetTickCount() - m_maxFrameInterval,BUFFER_INPUT_VIDEO_LIMIT*BUFFER_INPUT_VIDEO_TIME_OUT);
		if (m_minFrameInterval!=0 && (GetTickCount() - m_maxFrameInterval)>BUFFER_INPUT_VIDEO_LIMIT*BUFFER_INPUT_VIDEO_TIME_OUT)
		{
			ClearPackets();
		}
}

void	CBufferInputVideoEngine::EchoVideoInputInfo()
{
		LostTrace("CBufferInputVideoEngine::EchoVideoInputInfo MinSequenceId=%d,MaxSequenceId=%d \n",m_minSequence,m_maxSequence);
}