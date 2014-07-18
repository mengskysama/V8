#include "stdafx.h"
#include "Setting.h"
#include "UdpChannel.h"
#include "RelayCompenPacket.h"
#include "BufferOutputAudioEngine.h"
#include "AvDefine.h"
#include "Packets/AudioPacket.h"

map<UInt64,CBufferOutputAudioEngine*> CBufferOutputAudioEngine::m_mapAudioOutput;
Mutex CBufferOutputAudioEngine::m_static_mutex;//网络线程 播放线程 m_mapAudioOutput

CBufferOutputAudioEngine::CBufferOutputAudioEngine(UInt32 iRoomId,UInt32 iFromUin)
:m_pInPlace(NULL),
m_pRate(NULL),
m_iFromUin(iFromUin),
m_iRoomId(iRoomId),
m_lastRecvTime(GetTickCount()),
m_iMinFrameId(0),
m_iMaxFrameId(0),
m_iMinSequenceId(0),
m_iMaxSequenceId(0),
m_iCheckSeqId(0),
m_dwLastLostTime(GetTickCount()),
m_iTotalPacketNum(0),
m_iLostPacketNum(0),
m_fLostRate(0.0f),
m_iLastRecvRateInter(BUFFER_OUTPUT_RATECALC_MIN_INTERVAL),
m_iBufferSize(BUFFER_OUTPUT_ONMIC_AUDIO_MAX_LIMIT),
m_dwLastRecvRateTime(0),
m_dwFirstStartTime(0),
m_dwSecondStartTime(0),
m_iFirstStartNum(0),
m_iSecondStartNum(0),
m_bFilled(false),
m_fRecvRate(BUFFER_PERSEC_AUDIO_NUMBER),
m_dwLastCalcRecvRate(GetTickCount()),
m_fSendRate(BUFFER_PERSEC_AUDIO_SEND_INTER),
m_fLastPlayTime(GetTickCount()),
m_iStartPlayCount(0),
m_bAjustSendRate(false),
m_dwShakeCheckStart(GetTickCount()),
m_bShakeKeepRate(false)
{
		BufferTrace("CBufferOutputAudioEngine::CBufferOutputAudioEngine() \n");
		MutexLocker m_locker(&m_static_mutex);//网络线程 播放线程 m_mapAudioOutput
		m_mapAudioOutput.insert(make_pair(MakeSession(m_iRoomId,m_iFromUin),this));
}

CBufferOutputAudioEngine::~CBufferOutputAudioEngine()
{
		BufferTrace("CBufferOutputAudioEngine::~CBufferOutputAudioEngine() \n");
		MutexLocker m_locker(&m_static_mutex);//网络线程 播放线程 m_mapAudioOutput
		map<UInt64,CBufferOutputAudioEngine*>::iterator iter = m_mapAudioOutput.find(MakeSession(m_iRoomId,m_iFromUin));
		if (iter!=m_mapAudioOutput.end()){
			m_mapAudioOutput.erase(iter);
		}
		m_pInPlace = NULL;
		m_pRate = NULL;
		ClearPackets();
}

void CBufferOutputAudioEngine::AddPacket(Packet* pData,UInt32 iSequence)
{
		//BufferTrace("CBufferOutputAudioEngine::AddPacket() begin\n");
		unsigned int iFrameSeq = iSequence;
		Assert(pData->GetReference());
		if (::IsBadWritePtr(pData->getPacketContent(),pData->getPacketLen()) != 0){
			Assert(0);
		}

		Assert(pData->GetReference());
		
		MutexLocker locker(&m_mutex);//播放线程 网络线程 m_mapOutputAudioBuf
		map<UInt32,StructAudioBufferPack>::iterator iter;
		if ((iter=m_mapOutputAudioBuf.find(iSequence))!=m_mapOutputAudioBuf.end() || 
			!IsNormalPacket(iFrameSeq,iSequence) || 
			m_mapOutputAudioBuf.size()>BUFFER_OUTPUT_AUDIO_PACKET_MAXLIMIT){
			LostTrace("CBufferOutputAudioEngine收到无效数据包.............................数据包缓冲超限=%s,iSequence=%d,time=%d [minSequenceId:%d] [maxSequenceId:%d] [iMinFrameId:%d] [iMaxFrameId:%d]\n",
				m_mapOutputAudioBuf.size()>BUFFER_OUTPUT_AUDIO_PACKET_MAXLIMIT?"TRUE":"FALSE",iSequence,GetTickCount(),m_iMinSequenceId,m_iMaxSequenceId,m_iMinFrameId,m_iMaxFrameId);
			pData->RemoveReference();//数据包已经存在 不处理
			return;
		}else{
			m_lastRecvTime = GetTickCount();

			StructAudioBufferPack sabp;
			sabp.m_iSequence = iSequence;
			sabp.m_pPack = pData;
			sabp.m_time = GetTickCount();
			sabp.m_iFrameSeq = iFrameSeq;
			m_mapOutputAudioBuf[iSequence] = sabp;

			AddFrameSeq(iFrameSeq);
			AddSequenceId(iSequence);
			UpdateTime(GetTickCount());

			Assert(pData->GetReference());
		}
}

void CBufferOutputAudioEngine::Initialize(ITransInPlace* pInPlace,IBufferBaseRate* pRate)
{
		m_pInPlace = pInPlace;
		m_pRate = pRate;
}

void CBufferOutputAudioEngine::UnInitialize()
{
		BufferTrace("CBufferOutputAudioEngine::UnInitialize() \n");
		
		MutexLocker m_locker(&m_static_mutex);//网络线程 播放线程 m_mapAudioOutput
		map<UInt64,CBufferOutputAudioEngine*>::iterator iter = m_mapAudioOutput.find(MakeSession(m_iRoomId,m_iFromUin));
		if (iter!=m_mapAudioOutput.end()){
			m_mapAudioOutput.erase(iter);
		}
		m_pInPlace = NULL;
		m_pRate = NULL;
		ClearPackets();
}

void CBufferOutputAudioEngine::ClearPackets()
{
		BufferTrace("CBufferOutputAudioEngine::ClearPackets() begin\n");
		MutexLocker locker(&m_mutex);//播放线程 网络线程 m_mapOutputAudioBuf
		map<UInt32,StructAudioBufferPack>::iterator iter = m_mapOutputAudioBuf.begin();
		while (iter!=m_mapOutputAudioBuf.end()){
			BufferTrace("CBufferOutputAudioEngine::ClearPackets() while\n");
			iter->second.m_pPack->RemoveReference();
			iter = m_mapOutputAudioBuf.erase(iter);
		}
}

bool CBufferOutputAudioEngine::IsNormalPacket(UInt32 iFrameId,UInt32 iSequenceId)
{
		//BufferTrace("CBufferOutputAudioEngine::IsNormalPacket() begin\n");
		if (!m_bFilled && iSequenceId<m_iMinSequenceId && m_iMinSequenceId-iSequenceId<40){
			return true;
		}
		if (iSequenceId < m_iMinSequenceId && m_iMinSequenceId!=0 ){
			return false;
		}
		return true;
}

void CBufferOutputAudioEngine::AddFrameSeq(UInt32 iFrameId)
{
		//BufferTrace("CBufferOutputAudioEngine::AddFrameSeq() begin\n");
		if (0 == m_iMinFrameId){
			m_iMinFrameId = iFrameId;
		}
		if (iFrameId>m_iMaxFrameId){
			m_iMaxFrameId = iFrameId;
		}
		else if (!m_bFilled && iFrameId<m_iMinFrameId && m_iMinFrameId-iFrameId<40) //防止开始的时候的乱序
		{
			m_iMinFrameId = iFrameId;
		}
}

void CBufferOutputAudioEngine::AddSequenceId(UInt32 iSequenceId)
{
		//BufferTrace("CBufferOutputAudioEngine::AddSequenceId() begin\n");
		DWORD dwTime = GetTickCount();
		{
			if (dwTime - m_dwLastLostTime> BUFFER_OUTPUT_BUFFER_SHAKE_LOSTRATE){
				m_dwLastLostTime=GetTickCount();
				if (m_iLostPacketNum+m_iTotalPacketNum!=0)
					m_fLostRate = (float)m_iLostPacketNum /(m_iLostPacketNum+m_iTotalPacketNum);
				//BufferTrace("CBufferOutputAudioEngine::AddSequenceId 计算丢包率 -------  m_fLostRate=%f\n",m_fLostRate);
			}

			if (iSequenceId!=m_iMaxSequenceId+1 && m_iMaxSequenceId!=0)
				m_iLostPacketNum  += (iSequenceId - m_iMaxSequenceId-1<10?iSequenceId - m_iMaxSequenceId-1:1) ;
			m_iTotalPacketNum++;
		}

		if (0 == m_iMinSequenceId){
			m_iMinSequenceId = iSequenceId;
		}
		if (iSequenceId>m_iMaxSequenceId){
			m_iMaxSequenceId = iSequenceId;
		}
		else if (!m_bFilled && iSequenceId<m_iMinSequenceId && m_iMinSequenceId-iSequenceId<40)//防止开始的时候的乱序
		{
			m_iMinSequenceId = iSequenceId;
		}
}

void CBufferOutputAudioEngine::UpdateTime(DWORD dwCurTime){
		//BufferTrace("CBufferOutputAudioEngine::UpdateTime() begin\n");
		if (m_pRate)
		{
			//获取 计算速率单位时间间隔
			m_iLastRecvRateInter = m_pRate->GetRateTimeInterval(
				BUFFER_OUTPUT_ONMIC_AUDIO_MIN_LIMIT, //缓冲区最小缓冲时间 - 麦上 最小
				BUFFER_OUTPUT_ONMIC_AUDIO_MAX_LIMIT, //缓冲区最大缓冲时间 - 麦上 最大
				BUFFER_OUTPUT_UNMIC_AUDIO_MIN_LIMIT, //缓冲区最小缓冲时间 - 麦下 最小
				BUFFER_OUTPUT_UNMIC_AUDIO_MAX_LIMIT, //缓冲区最大缓冲时间 - 麦下 最大
				m_iBufferSize,
				m_iLastRecvRateInter,
				m_dwLastRecvRateTime
				);

			//BufferTrace("CBufferOutputAudioEngine 计算接收速率 时间间隔..m_iLastRecvRateInter=%d\n",m_iLastRecvRateInter);
			DWORD dwCurTimeMinus = GetTickCount();
			if (dwCurTimeMinus - m_dwFirstStartTime > m_iLastRecvRateInter){
				m_dwFirstStartTime = dwCurTimeMinus;
				m_iFirstStartNum = 0;
			}

			if (dwCurTimeMinus - m_dwFirstStartTime > m_iLastRecvRateInter / 2 && 
				dwCurTimeMinus - m_dwSecondStartTime > m_iLastRecvRateInter){
				m_dwSecondStartTime = dwCurTimeMinus;
				m_iSecondStartNum = 0;
			}

			m_iFirstStartNum++;
			m_iSecondStartNum++;
			//BufferTrace("CBufferOutputAudioEngine::UpdateTime dwInterval=%d,m_dwFirstStartTime=%d,m_iFirstStartNum=%d,m_dwSecondStartTime=%d,m_iSecondStartNum=%d\n",0,
			//	m_dwFirstStartTime,m_iFirstStartNum,m_dwSecondStartTime,m_iSecondStartNum);
		}
		else
		{
			AssertVS(0,m_iRoomId,"CBufferOutputAudioEngine::UpdateTime m_pRate=NULL");
		}
}

float CBufferOutputAudioEngine::CalcRecevRate()
{
		//BufferTrace("CBufferOutputVideoEngine::CalcRecevRate() begin %d\n",GetTickCount());
		DWORD dwCurTimeMinus = GetTickCount();
		if (dwCurTimeMinus - m_dwFirstStartTime > dwCurTimeMinus - m_dwSecondStartTime)
		{
			return (float)m_iFirstStartNum * 1000 / (dwCurTimeMinus - m_dwFirstStartTime);
		}
		else
		{
			return (float)m_iSecondStartNum * 1000 / (dwCurTimeMinus - m_dwSecondStartTime);
		}
}

bool CBufferOutputAudioEngine::IsCanRemove()
{
		return (GetTickCount() - m_lastRecvTime> BUFFER_MEDIA_EXPIRE_TIME)?true:false;
}

void CBufferOutputAudioEngine::Play()
{
		//BufferTrace("CBufferOutputAudioEngine::Play()....\n");
		if (!m_bFilled && m_mapOutputAudioBuf.size() < (1.0f*BUFFER_PERSEC_AUDIO_NUMBER*BUFFER_OUTPUT_ONMIC_AUDIO_MAX_LIMIT)/1000){
			return ;
		}

		if (!m_bFilled) {
			m_fLastPlayTime = GetTickCount();
			m_iStartPlayCount = 0;
			m_bFilled = true;
		}
		if (m_mapOutputAudioBuf.size() < 1 || (GetTickCount() - m_lastRecvTime) > 1000*10){//10s钟没有收到数据 超时 刷新
			MutexLocker locker(&m_mutex);//播放线程 网络线程 m_mapOutputAudioBuf
			ResetStatus();
			return;
		}

		IsCanCalcRecvRate()?m_fRecvRate = CalcRecevRate():NULL;
		m_fSendRate = CanCalcPlayRate();

		//ErrTrace("CBufferOutputAudioEngine::Play() m_mapOutputAudioBuf.size()=%d,seq=%d,m_fRecvRate=%f,m_fSendRate=%f\n",m_mapOutputAudioBuf.size(),m_iMaxSequenceId - m_iMinSequenceId+1,m_fRecvRate,m_fSendRate);

		unsigned int iLimit = 0;
		DWORD dwCurrTime = GetTickCount();
		while (m_fLastPlayTime < dwCurrTime && ++iLimit<5)
		{
			m_fLastPlayTime += m_fSendRate;
			m_iStartPlayCount++;
			PlayPackets();
			SwitchToThread();
		}
		CheckPacket();
		RefreshStatus();
}

double CBufferOutputAudioEngine::CanCalcPlayRate()
{
	if (GetTickCount() - m_dwShakeCheckStart > BUFFER_PERSEC_AUDIO_SHAKE_CHECK) //一定间隔时间抖动检测
	{
		unsigned int iNormalRate = BUFFER_PERSEC_AUDIO_NUMBER;
		m_bShakeKeepRate = (m_fRecvRate<=iNormalRate-1 || m_fRecvRate>=iNormalRate+2)?true:false;
		m_dwShakeCheckStart = GetTickCount();
	}

	if (m_bShakeKeepRate) //抖动稳定速率发送数据
		return BUFFER_PERSEC_AUDIO_SEND_INTER;
	
	const DWORD dwBufferLen = (1.0f*BUFFER_PERSEC_AUDIO_NUMBER*BUFFER_OUTPUT_ONMIC_AUDIO_MAX_LIMIT)/1000; //正常接收
	const DWORD dwCurrLen = m_mapOutputAudioBuf.size(); //当前缓冲区中的数据长度个数

	if (dwBufferLen == dwCurrLen || dwBufferLen == dwCurrLen-1 || dwBufferLen == dwCurrLen-2) 
	{
		m_bAjustSendRate = false;
		return BUFFER_PERSEC_AUDIO_SEND_INTER;
	}
	
	if(dwCurrLen<=dwBufferLen-BUFFER_PERSEC_AUDIO_SHAKE_LIMIT) //小于最小临界，发送数据速度过快
	{
		return BUFFER_PERSEC_AUDIO_SEND_INTER + 0.7f;
		m_bAjustSendRate = true;
	}
	else
	if (dwCurrLen >= dwBufferLen+BUFFER_PERSEC_AUDIO_SHAKE_LIMIT) //缓冲区中数据过多，发送数据过慢
	{
		return BUFFER_PERSEC_AUDIO_SEND_INTER - 0.5f;
		m_bAjustSendRate = true;
	}

	return m_fSendRate;
}

void CBufferOutputAudioEngine::ResetStatus()//重置缓冲区状态
{
		m_bFilled = false;
// 		m_lastRecvTime = GetTickCount();
//		m_dwSendRate = BUFFER_PERSEC_AUDIO_SEND_INTER;
//  		if (m_iMinSequenceId>m_iMaxSequenceId){
// 			m_iCheckSeqId = 0;
// 			m_iMinFrameId = 0;
// 			m_iMaxFrameId = 0;
// 			m_iMinSequenceId = 0;
// 			m_iMaxSequenceId = 0;
// 		}
		{
			ClearPackets();
			m_mapOutputAudioReqInf.clear();
			m_iCheckSeqId = 0;
			m_iMinFrameId = 0;
			m_iMaxFrameId = 0;
			m_iMinSequenceId = 0;
			m_iMaxSequenceId = 0;
		}
		m_dwLastLostTime = GetTickCount();
		m_iTotalPacketNum = 0;
		m_iLostPacketNum = 0;
		m_fLostRate = 0.0f;
		m_iLastRecvRateInter = BUFFER_OUTPUT_RATECALC_MIN_INTERVAL;
		m_iBufferSize = BUFFER_OUTPUT_ONMIC_AUDIO_MAX_LIMIT;
		m_dwLastRecvRateTime = 0;
		m_dwFirstStartTime = 0;
		m_dwSecondStartTime = 0;
		m_iFirstStartNum = 0;
		m_iSecondStartNum = 0;
		m_fRecvRate = BUFFER_PERSEC_AUDIO_NUMBER;
		m_dwLastCalcRecvRate = GetTickCount();
		m_fLastPlayTime = GetTickCount();
		m_iStartPlayCount = 0;
}


void CBufferOutputAudioEngine::RefreshStatus()
{
		if (m_pInPlace){
			DWORD dwTime = m_mapOutputAudioBuf.size()*m_fSendRate;
			m_pInPlace->SetBufferSize(m_iFromUin,bfAudio,dwTime);
			m_pInPlace->SetRecvTime(m_iFromUin,bfAudio,m_lastRecvTime);
		}else{
			ErrTrace("CBufferOutputAudioEngine::RefreshStatus()....m_pInPlace=NULL.\n");
		}
}

bool CBufferOutputAudioEngine::IsCanCalcRecvRate()
{
		if (GetTickCount()-m_dwLastCalcRecvRate > 200){
			m_dwLastCalcRecvRate = GetTickCount();
			return true;
		}
		return false;
}

void CBufferOutputAudioEngine::PlayPackets()
{
		//BufferTrace("CBufferOutputAudioEngine m_mapOutputAudioBuf.size()=%d,m_iMinSequenceId=%d,m_iMaxSequenceId=%d\n", 
		//	m_mapOutputAudioBuf.size(),m_iMinSequenceId,m_iMaxSequenceId);
		Packet* pPacket = NULL;
		MutexLocker locker(&m_mutex);//播放线程 网络线程 m_mapOutputAudioBuf
		if(m_iMinSequenceId<=m_iMaxSequenceId && m_iMaxSequenceId!=0){
			pPacket = GetPlayPacket(m_iMinSequenceId++);//播放数据包，即使丢包也算一次
		}
		if (!pPacket)
		{
			UInt8 cTemp; //数据包不存在 发送空包。。。。。cygnus
			for (int i=0;i<PER_FRAME_OF_SAMEPLE;i++)
			{
				pPacket = AudioPacket::makePacket(-1,AUDIO_RTP_PACKET_TYPE,0, &cTemp);
				pPacket->AddReference();
				Assert(pPacket->GetReference());
				try{
					BAD_MEM_WRITE_EX(pPacket->getPacketContent(),pPacket->getPacketLen(),"CBufferOutputAudioEngine::PlayPackets() pPacket mem") ;
					UdpChannel::PlayAudioPacket(m_iRoomId,m_iFromUin,pPacket,m_iMinSequenceId);
				}catch (...){
					ErrTrace("CBufferOutputAudioEngine::PlayPackets() UdpChannel::PlayAudioPacket Error!");
				}
				pPacket->RemoveReference();
				ErrTrace("数据包不存在 发送空包.....m_iMinSequenceId=%d\n",m_iMinSequenceId);
			}
			return;
		}
		Assert(pPacket->GetReference());
		try{
			BAD_MEM_WRITE_EX(pPacket->getPacketContent(),pPacket->getPacketLen(),"CBufferOutputAudioEngine::PlayPackets() pPacket mem") ;
			UdpChannel::PlayAudioPacket(m_iRoomId,m_iFromUin,pPacket,m_iMinSequenceId);
		}catch (...){
			ErrTrace("CBufferOutputAudioEngine::PlayPackets() UdpChannel::PlayAudioPacket Error!");
		}
		pPacket->RemoveReference();
}

Packet* CBufferOutputAudioEngine::GetPlayPacket(UInt32 iSequenceId)
{
		map<UInt32,StructAudioBufferPack>::iterator iter=m_mapOutputAudioBuf.find(iSequenceId);
		if (iter!=m_mapOutputAudioBuf.end())
		{
			Packet* pPacket = iter->second.m_pPack;
			m_mapOutputAudioBuf.erase(iter);
			return pPacket;
		}
		else
		{
			LostTrace("[RoomId:%12ld] [FromUin:%12ld] [Audio-SequenceId:%12ld] [time:%12ld] [-----Play-----] [minSequenceId:%d] [maxSequenceId:%d] [LostPacket:%d] [TotalPacket:%d] [LostRate:%f] %s\n",
				m_iRoomId,m_iFromUin,iSequenceId,GetTickCount(),m_iMinSequenceId,m_iMaxSequenceId,m_iLostPacketNum,m_iTotalPacketNum,(float)m_iLostPacketNum/(m_iLostPacketNum+m_iTotalPacketNum),"音频数据包丢失");
			return NULL;
		}
}

void CBufferOutputAudioEngine::RequestCompenPacket(UInt32 iSequenceId,bool bA)
{
		for (int i=0;i<BUFFER_COMPENSATION_NUMBER;i++){
			Packet* pPacket = RelayCompenPacket::makePacket(-1,m_iRoomId,m_iFromUin,iSequenceId,bA);
			pPacket->AddReference();
			UdpChannel::RequestCompenPacket(m_iRoomId,m_iFromUin,pPacket);
		}
}

void CBufferOutputAudioEngine::CheckPacket()
{
		if (m_iCheckSeqId < m_iMinSequenceId) 
			m_iCheckSeqId = m_iMinSequenceId;
		if (m_iCheckSeqId>m_iMaxSequenceId) return;	

		unsigned int iLimit = 0;
		unsigned int iMaxSeq = m_iMaxSequenceId-m_iMinSequenceId>BUFFER_PERSEC_AUDIO_NUMBER?m_iMaxSequenceId-5:m_iMaxSequenceId;
		for (UInt32 i = m_iCheckSeqId; i<=iMaxSeq && iLimit++<3; ){
			if (m_mapOutputAudioBuf.find(i)==m_mapOutputAudioBuf.end()){
				RequestCompenPacket(i,TRUE); //请求补偿指定序号音频数据包
				StructAudioRequestInfo sari;
				sari.m_iReqNum = 1;
				sari.m_iSequence = i;
				sari.m_lastTime = GetTickCount();
				m_mapOutputAudioReqInf[i] = sari;
				SwitchToThread();
				LostTrace("[RoomId:%12ld] [FromUin:%12ld] [Audio-SequenceId:%12ld] [time:%12ld] [-----Request-----] [minSequenceId:%d] [maxSequenceId:%d] %s\n",
					m_iRoomId,m_iFromUin,i,GetTickCount(),m_iMinSequenceId,m_iMaxSequenceId,"首次请求补偿音频数据包");
			}
			m_iCheckSeqId = ++i;
		}
		
		DWORD dwInteval = 0;//请求补偿数据包时间间隔
		if (m_pRate){
			dwInteval = m_pRate->CalcRequestCompenInterval(
				m_iBufferSize,(UInt32)m_mapOutputAudioBuf.size(),
				m_fRecvRate);
		}else{
			AssertVS(0,0,"m_pRate->CalcRequestCompenInterval  m_pRate=NULL");
		}
		map<UInt32,StructAudioRequestInfo>::iterator iter=m_mapOutputAudioReqInf.begin();
		while (iter!=m_mapOutputAudioReqInf.end()){
			//超过指定次数
			if ((GetTickCount() - iter->second.m_lastTime > dwInteval) && (iter->second.m_iReqNum++>=BUFFER_COMPEN_REQUE_TIMES || iter->first<m_iMinSequenceId)){
				if (m_mapOutputAudioBuf.find(iter->first)==m_mapOutputAudioBuf.end()){
					LostTrace("[RoomId:%12ld] [FromUin:%12ld] [Audio-SequenceId:%12ld] [time:%12ld] [-----Delete-----] [请求次数%d] [minSequenceId:%d] [maxSequenceId:%d] [LostPacket:%d] [TotalPacket:%d] [LostRate:%f] %s\n",
						m_iRoomId,m_iFromUin,iter->first,GetTickCount(),iter->second.m_iReqNum,m_iMinSequenceId,m_iMaxSequenceId,m_iLostPacketNum,m_iTotalPacketNum,(float)m_iLostPacketNum/(m_iLostPacketNum+m_iTotalPacketNum),"移除无效数据包");
				}
				iter = m_mapOutputAudioReqInf.erase(iter);
				SwitchToThread();
				continue;
			}
			//补偿成功
			if (m_mapOutputAudioBuf.find(iter->first)!=m_mapOutputAudioBuf.end()){
				LostTrace("[RoomId:%12ld] [FromUin:%12ld] [Audio-SequenceId:%12ld] [time:%12ld] [-----Success-----] [请求次数%d] [minSequenceId:%d] [maxSequenceId:%d] %s\n",
					m_iRoomId,m_iFromUin,iter->first,GetTickCount(),iter->second.m_iReqNum,m_iMinSequenceId,m_iMaxSequenceId,"数据包已经补偿成功");
				iter = m_mapOutputAudioReqInf.erase(iter);
				SwitchToThread();
				continue;
			}
			//重新请求
			//BufferTrace("****Cur time=%d,last time=%d,now interval=%d,dwInteval=%d\n",GetTickCount(),iter->second.m_lastTime,GetTickCount() - iter->second.m_lastTime,dwInteval);
			if (GetTickCount() - iter->second.m_lastTime > dwInteval){
				LostTrace("[RoomId:%12ld] [FromUin:%12ld] [Audio-SequenceId:%12ld] [time:%12ld] [-----Re Request-----] [请求次数%d] [minSequenceId:%d] [maxSequenceId:%d] %s\n",
					m_iRoomId,m_iFromUin,iter->first,GetTickCount(),iter->second.m_iReqNum,m_iMinSequenceId,m_iMaxSequenceId,"请求补偿数据包");
				iter->second.m_lastTime = GetTickCount();
				RequestCompenPacket(iter->first,TRUE);
				SwitchToThread();
			}
			iter++;
		}
}