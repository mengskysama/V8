#include "stdafx.h"
#include "BufferInputAudioEngine.h"
#include "RelayPacket.h"
#include "Packets/AudioPacket.h"
#include "BufferDefine.h"

CBufferInputAudioEngine::CBufferInputAudioEngine()
{
		m_minSequence = 0;
		m_maxSequence = 0;
		m_minFrameSeq = 0;
		m_maxFrameSeq = 0;
		m_keepFrameNum = BUFFER_PERSEC_AUDIO_NUMBER*BUFFER_INPUT_AUDIO_LIMIT / 1000; //缓冲区维持的音频的帧数
		m_minFrameInterval = 0;
		m_maxFrameInterval = 0;
		m_minSequeInterval = 0;
		m_maxSequeInterval = 0;
}

CBufferInputAudioEngine::~CBufferInputAudioEngine()
{
	BufferTrace("CBufferInputAudioEngine::~CBufferInputAudioEngine() \n");
	ClearPackets();
}

void	CBufferInputAudioEngine::Initialize()
{
		BufferTrace("CBufferInputAudioEngine::CBufferInputAudioEngine() \n");
}

void	CBufferInputAudioEngine::UnInitialize()
{
		BufferTrace("CBufferInputAudioEngine::UnInitialize() \n");
		ClearPackets();
}

void	CBufferInputAudioEngine::AddPacket(Packet* pDataPacket,UInt32 iSequence)
{
		BufferTrace("CBufferInputAudioEngine::AddPacket() \n");

		if (m_audioBufferMap.size() >BUFFER_OUTPUT_AUDIO_PACKET_MAXLIMIT)
		{
			pDataPacket->RemoveReference();
			ErrTrace("CBufferInputAudioEngine::AddPacket() 缓冲区长度超过限制.m_minSequence=%d,m_maxSequence=%d.....\n",m_minSequence,m_maxSequence);
			return;
		}

		UInt32 iFrameSeq = 0;//对保存的数据包拆包，获取帧序号
		if (!GetFrameSeq(pDataPacket,iFrameSeq))
		{
			Assert(0);
			return;
		}
		
		MutexLocker locker(&m_Mutex);
		StructAudioBufferPack sPack;
		sPack.m_iSequence = iSequence;
		sPack.m_pPack = pDataPacket;
		sPack.m_time = GetTickCount();
		sPack.m_iFrameSeq = iFrameSeq;
		m_audioBufferMap[iSequence] = sPack;
		AddSequence(iSequence);
		AddFrameSeq(iFrameSeq);
		
		if (m_FrameToSeqMap.find(iFrameSeq)==m_FrameToSeqMap.end())
		{
			m_FrameToSeqMap[iFrameSeq] = iSequence; //只映射最小的ID
		}
}

bool	CBufferInputAudioEngine::GetPacket(Packet* &pDataPacket,UInt32 iSequence)
{
		BufferTrace("CBufferInputAudioEngine::~GetPacket() \n");
		MutexLocker locker(&m_Mutex);
		map<UInt32,StructAudioBufferPack>::iterator iter = m_audioBufferMap.find(iSequence);
		if (iter!=m_audioBufferMap.end())
		{
			pDataPacket=iter->second.m_pPack;
			pDataPacket->AddReference();
			return true;
		}
		return false;
}

void	CBufferInputAudioEngine::AddSequence(UInt32 iSequence)
{
		BufferTrace("CBufferInputAudioEngine::AddSequence() \n");
		if (iSequence<m_minSequence || m_minSequence<=1)
		{
			m_minSequence = iSequence;
			if (0 == m_minSequeInterval)
				m_minSequeInterval = GetTickCount();
		}
		m_maxSequence = iSequence;
		m_maxSequeInterval = GetTickCount();
}

void	CBufferInputAudioEngine::AddFrameSeq(UInt32 iFrameSeq)
{
		BufferTrace("CBufferInputAudioEngine::AddFrameSeq() \n");
		if (iFrameSeq<m_minFrameSeq || m_minFrameSeq==0)
		{
			m_minFrameSeq = iFrameSeq;
			if (0==m_minFrameInterval)
				m_minFrameInterval = GetTickCount();
		}
		m_maxFrameSeq = iFrameSeq;
		m_maxFrameInterval = GetTickCount();
}

bool	CBufferInputAudioEngine::GetFrameSeq(Packet* pDataPacket,UInt32& iFrameSeq)
{
		BufferTrace("CBufferInputAudioEngine::GetFrameSeq() \n");
		RelayDataPacket dataPacket;
		if (!dataPacket.parserPacketToSelf(pDataPacket))
		{
			Assert(0);
			return false;
		}
		return ParsePacket(dataPacket.getData(),dataPacket.getDataLen(),iFrameSeq);
}

bool	CBufferInputAudioEngine::ParsePacket(const UInt8* pBuf, UInt32 iLen,UInt32& iFrameSeq)
{
		BufferTrace("CBufferInputAudioEngine::ParsePacket() \n");
		Packet tempPacket(-1);
		tempPacket.setContentPtr(const_cast<UInt8*>(pBuf), iLen);

		AudioPacket audioPacket;
		if (!audioPacket.parserPacket(&tempPacket))
		{
			Assert(0);
			return false;
		}

		unsigned char* data = audioPacket.getData();
		unsigned int iSeq = (unsigned int)*(data+1);
		iFrameSeq = iSeq;
		tempPacket.setContentPtr(NULL, 0);
		return true;
}

void	CBufferInputAudioEngine::ClearPackets()//清理所有的网络数据包
{
		BufferTrace("CBufferInputAudioEngine::ClearPackets() \n");
		MutexLocker locker(&m_Mutex);
		map<UInt32,StructAudioBufferPack>::iterator iter = m_audioBufferMap.begin();
		while (iter!=m_audioBufferMap.end())
		{
			iter->second.m_pPack->RemoveReference();
			iter = m_audioBufferMap.erase(iter);
		}
		m_minSequence = 0;
		m_maxSequence = 0;
		m_minFrameSeq = 0;
		m_maxFrameSeq = 0;
		m_minFrameInterval = 0;
		m_maxFrameInterval = 0;
		m_minSequeInterval = 0;
		m_maxSequeInterval = 0;
		m_FrameToSeqMap.clear();
}

void	CBufferInputAudioEngine::KeepBalance()//维持指定时间长度缓冲数据
{
		BufferTrace("CBufferInputAudioEngine::KeepBalance() 外部 m_minSequence=%d,m_maxSequence=%d,m_keepFrameNum=%d,m_audioBufferMap.size()=%d,m_FrameToSeqMap.size()=%d\n",m_minSequence,m_maxSequence,m_keepFrameNum,m_audioBufferMap.size(),m_FrameToSeqMap.size());
		MutexLocker locker(&m_Mutex);
		if ((m_maxSequence - m_minSequence)>m_keepFrameNum)
		{
			int i = 0;
			while ((m_maxSequence - m_minSequence)>m_keepFrameNum && i++ < 20)
			{
				DWORD dwStart = 0;
				RemoveSequence(m_minSequence);
				IncSequence(m_minSequence);
				BufferTrace("CBufferInputAudioEngine::KeepBalance() 循环 m_minSequence=%d,m_maxSequence=%d,m_keepFrameNum=%d,m_audioBufferMap.size()=%d,m_FrameToSeqMap.size()=%d\n",m_minSequence,m_maxSequence,m_keepFrameNum,m_audioBufferMap.size(),m_FrameToSeqMap.size());
				SwitchToThread();
			}
		}
		RemoveTimeOut();
}

bool	CBufferInputAudioEngine::GetEngineUsed()
{
		return !(m_minSequence==0 && m_maxSequence==0);
}

void	CBufferInputAudioEngine::RemoveTimeOut()
{
		BufferTrace("CBufferInputAudioEngine::RemoveTimeOut()  m_minSequeInterval=%d,m_maxSequeInterval=%d,interval=%d,minus=%d,limit=%d\n",m_minSequeInterval,m_maxSequeInterval,m_maxSequeInterval- m_minFrameInterval,GetTickCount() - m_maxSequeInterval,BUFFER_INPUT_AUDIO_LIMIT*BUFFER_INPUT_AUDIO_TIME_OUT);
		if (m_minFrameInterval!=0 && GetTickCount() - m_maxSequeInterval>BUFFER_INPUT_AUDIO_LIMIT*BUFFER_INPUT_AUDIO_TIME_OUT)
		{
			ClearPackets();
		}
}

bool	CBufferInputAudioEngine::FindSeqFromFrameSeq(UInt32 iFrameSeq,UInt32& iSequence)
{
		BufferTrace("CBufferInputAudioEngine::FindSeqFromFrameSeq() \n");
		map<UInt32,UInt32>::iterator iter;
		if ((iter=m_FrameToSeqMap.find(iFrameSeq))!=m_FrameToSeqMap.end())
		{
			iSequence = iter->second;
			return true;
		}
		return false;
}

void	CBufferInputAudioEngine::IncFrameSeq(UInt32& iFrameSeq)
{
		BufferTrace("CBufferInputAudioEngine::IncFrameSeq() \n");
		UInt32 iSequence = 0;
		while (!FindSeqFromFrameSeq(++iFrameSeq,iSequence) && iFrameSeq<m_maxFrameSeq){}
		m_minSequence = iSequence;
		if (m_minSequence!=0)
		{
			map<UInt32,StructAudioBufferPack>::iterator iter;
			if ((iter=m_audioBufferMap.find(m_minSequence))!=m_audioBufferMap.end())
			{
				m_minFrameInterval = iter->second.m_time;
				m_minSequeInterval = iter->second.m_time;
			}
			else
			{
				AssertVS(0,0,"CBufferInputVideoEngine::IncFrameSeq 数据sequenceid不存在");
			}
		}
		else
		{
			AssertVS(0,0,"CBufferInputAudioEngine::IncFrameSeq 数据帧不存在");
		}
}

void	CBufferInputAudioEngine::RemoveFrame(UInt32 iFrameSeq)
{
		BufferTrace("CBufferInputAudioEngine::RemoveFrame() \n");
		UInt32 iSequence = 0;
		if (FindSeqFromFrameSeq(iFrameSeq,iSequence))
		{
			map<UInt32,StructAudioBufferPack>::iterator iter = m_audioBufferMap.find(iSequence);
			if (iter!=m_audioBufferMap.end())
			{
				iter->second.m_pPack->RemoveReference();
				m_audioBufferMap.erase(iter);
		
				Assert(m_minFrameSeq == iFrameSeq);
				Assert(m_minSequence == iSequence);
			}
		}
		else
		{
			Assert(0);
		}
}

void	CBufferInputAudioEngine::RemoveSequence(UInt32 iSequenceId) //移除超过指定长度的数据包
{
		BufferTrace("CBufferInputAudioEngine::RemoveSequence() \n");
		map<UInt32,StructAudioBufferPack>::iterator iter = m_audioBufferMap.find(iSequenceId);
		if (iter!=m_audioBufferMap.end())
		{
			iter->second.m_pPack->RemoveReference();
			m_audioBufferMap.erase(iter);

			Assert(m_minSequence == iSequenceId);
		}
}

void	CBufferInputAudioEngine::IncSequence(UInt32& iSequenceId) //移动序号
{
		BufferTrace("CBufferInputAudioEngine::IncSequence() begin\n");
		while (!FindSequence(++iSequenceId) && iSequenceId<m_maxSequence){
			BufferTrace("CBufferInputAudioEngine::IncSequence() while loop\n");
		}
		if (m_minSequence!=0)
		{
			map<UInt32,StructAudioBufferPack>::iterator iter;
			if ((iter=m_audioBufferMap.find(iSequenceId))!=m_audioBufferMap.end())
			{
				m_minFrameInterval = iter->second.m_time;
				m_minSequeInterval = iter->second.m_time;
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
}

bool	CBufferInputAudioEngine::FindSequence(UInt32& iSequence)//找到网络序列号
{
		BufferTrace("CBufferInputAudioEngine::FindSequence() begin\n");
		map<UInt32,StructAudioBufferPack>::iterator iter;
		if ((iter=m_audioBufferMap.find(iSequence))!=m_audioBufferMap.end())
		{
			if(m_minFrameSeq != iter->second.m_iFrameSeq)
			{
				UInt32 iFrameSeq = m_minFrameSeq;
				while(iFrameSeq<iter->second.m_iFrameSeq)
				{
					map<UInt32,UInt32>::iterator iter;
					if ((iter=m_FrameToSeqMap.find(iFrameSeq++))!=m_FrameToSeqMap.end())
					{
						m_FrameToSeqMap.erase(iter);
					}
				}

				m_minFrameSeq = iter->second.m_iFrameSeq;
			}
			return true;
		}
		return false;
}

void	CBufferInputAudioEngine::EchoAudioInputInfo()//视频信息
{
		LostTrace("CBufferInputAudioEngine::EchoAudioInputInfo MinSequenceId=%d,MaxSequenceId=%d \n",m_minSequence,m_maxSequence);
}