#include "stdafx.h"
#include "BufferDefine.h"
#include "BufferOutputAudioRateAgc.h"

//获取当前速率计算更新时间间隔 根据不同BUFFER长度适当调整
DWORD CBufferOutputAudioRateAgc::GetRateTimeInterval(
								  UInt32 iBufMinInterval,		//缓冲区最小缓冲时间 - 麦上 最小
								  UInt32 iBufMaxInterval,		//缓冲区最大缓冲时间 - 麦上 最大
								  UInt32 iBufMinInterval1,		//缓冲区最小缓冲时间 - 麦下 最小
								  UInt32 iBufMaxInterval1,		//缓冲区最大缓冲时间 - 麦下 最大
								  UInt32 iBufSize,				//当前缓冲区大小
								  UInt32 iLastRecvRate,			//上次计算的间隔时间
								  DWORD& dwLastRecvTime			//上次计算收包速率的时间
								  )
{
	if (0 == dwLastRecvTime){
		dwLastRecvTime = GetTickCount();
		return BUFFER_OUTPUT_RATECALC_MIN_INTERVAL;
	}
	if (GetTickCount() - dwLastRecvTime < BUFFER_OUTPUT_BUFFER_RECVRATE_SHAKE){
		return iLastRecvRate;
	}else{
		dwLastRecvTime = GetTickCount();
	}

	//缓冲区大小范围
	unsigned int iBufMinInter = iBufMinInterval;
	unsigned int iBufMaxInter = iBufMaxInterval;
	bool bOnMic = GetMicStatus();
	if (!bOnMic){
		iBufMinInter = iBufMinInterval1;
		iBufMaxInter = iBufMaxInterval1;	
	}

	unsigned int iondisf =  iBufMaxInter - iBufMinInter;
	if (iBufSize<iBufMinInter || iBufSize>iBufMaxInter){
		return iLastRecvRate;
	}

	if (0==iondisf){
		return BUFFER_OUTPUT_RATECALC_MIN_INTERVAL;
	}

	unsigned int iDelayTime = (float)(iBufSize- iBufMinInter) * (BUFFER_OUTPUT_RATECALC_MAX_INTERVAL -	\
		BUFFER_OUTPUT_RATECALC_MIN_INTERVAL) / iondisf +BUFFER_OUTPUT_RATECALC_MIN_INTERVAL;
	iDelayTime = iDelayTime - iDelayTime%10;
	return iDelayTime;
}


//返回请求补偿包下次请求的时间间隔
DWORD CBufferOutputAudioRateAgc::CalcRequestCompenInterval(
										UInt32 iBufferSize,			//缓冲区总大小
										UInt32 iBufferDataSize,		//当前缓冲区数据个数
										float  fRecvRate			//收到数据包的速率
										)
{
	DWORD dwInter = BUFFER_OUTPUT_LOST_COMPENSATION_MIN;
	if (iBufferSize < BUFFER_OUTPUT_LOST_COMPENSATION_MIN * 3){
		return dwInter;
	}

	float fBufferDataSize = iBufferDataSize * 1000 / BUFFER_PERSEC_AUDIO_NUMBER;
	if (fBufferDataSize < BUFFER_OUTPUT_LOST_COMPENSATION_MIN * 3){
		return dwInter;
	}

	float flimit = fBufferDataSize>iBufferSize?iBufferSize:fBufferDataSize;
	dwInter = (flimit - BUFFER_OUTPUT_LOST_COMPENSATION_MIN * 3) / 3 + 
		BUFFER_OUTPUT_LOST_COMPENSATION_MIN;
	dwInter = dwInter > BUFFER_OUTPUT_LOST_COMPENSATION_MAX?BUFFER_OUTPUT_LOST_COMPENSATION_MAX:dwInter;
	dwInter = dwInter - dwInter%10;
	return dwInter;
}