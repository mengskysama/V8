#include "stdafx.h"
#include <math.h>
#include "BufferDefine.h"
#include "BufferOutputVideoRateAgc.h"


//获取当前速率计算更新时间间隔 根据不同BUFFER长度适当调整
DWORD CBufferOutputVideoRateAgc::GetRateTimeInterval(
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
		if (GetTickCount() - dwLastRecvTime < BUFFER_OUTPUT_BUFFER_RECVRATE_SHAKE)
		{
			return iLastRecvRate;
		}
		else
		{
			dwLastRecvTime = GetTickCount();
		}

		//缓冲区大小范围
		unsigned int iBufMinInter = iBufMinInterval;
		unsigned int iBufMaxInter = iBufMaxInterval;
		bool bOnMic = GetMicStatus();
		if (!bOnMic) //不在麦上
		{
			iBufMinInter = iBufMinInterval1;
			iBufMaxInter = iBufMaxInterval1;	
		}

		unsigned int iondisf =  iBufMaxInter - iBufMinInter;
		if (iBufSize<iBufMinInter || iBufSize>iBufMaxInter || iondisf==0)
		{
			//Assert(0);
			return iLastRecvRate;
		}

		unsigned int iDelayTime = (float)(iBufSize- iBufMinInter) * (BUFFER_OUTPUT_RATECALC_MAX_INTERVAL -	\
			BUFFER_OUTPUT_RATECALC_MIN_INTERVAL) / iondisf +BUFFER_OUTPUT_RATECALC_MIN_INTERVAL;
		iDelayTime = iDelayTime - iDelayTime%10;

		return iDelayTime;
}

//返回请求补偿包下次请求的时间间隔
DWORD CBufferOutputVideoRateAgc::CalcRequestCompenInterval(
	UInt32 iBufferSize,			//缓冲区总大小
	UInt32 iBufferDataSize,		//当前缓冲区数据个数
	float  fRecvRate			//收到数据包的速率
	)
{
		DWORD dwInter = BUFFER_OUTPUT_LOST_COMPENSATION_MIN;
		if (iBufferSize < BUFFER_OUTPUT_LOST_COMPENSATION_MIN * 3){
			return dwInter;
		}

		float fBufferDataSize = iBufferDataSize * 1000 / BUFFER_PERSEC_VIDEO_NUMBER;
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

//计算Buffer大小 根据网络丢包率 来动态调整BUFFER大小  丢包越少BUFFER越小 丢包越多BUFFER越大，为补偿提供相对充足的时间
UInt32 CBufferOutputVideoRateAgc::CalcBufferSize(
							  UInt32 iBufMinInterval,		//缓冲区最小缓冲时间 - 麦上 最小
							  UInt32 iBufMaxInterval,		//缓冲区最大缓冲时间 - 麦上 最大
							  UInt32 iBufMinInterval1,		//缓冲区最小缓冲时间 - 麦下 最小
							  UInt32 iBufMaxInterval1,		//缓冲区最大缓冲时间 - 麦下 最大
							  UInt32 iAverageRate,			//平均速率 每秒接收到的数据包个数
							  float  fLostRate,				//当前丢包率
							  DWORD& dwLastTime,			//上一次计算缓冲区时间
							  UInt32 iCurBufSize,			//当前缓冲区大小
							  UInt32 iRefenceBufSize,		//参照buf大小，用于音视频同步
							  DWORD  dwRefRecvTime			//参照最近一次接收数据时间
							  )
{
		if (GetTickCount() - dwLastTime < BUFFER_OUTPUT_BUFFER_SHAKE_INTERVAL){
			return iCurBufSize;
		}else{
			dwLastTime = GetTickCount();
		}
		//开始计算
		unsigned int iBufferSizeTemp = iCurBufSize;
		unsigned int iBufMinInter = iBufMinInterval;
		unsigned int iBufMaxInter = iBufMaxInterval;
		unsigned int iBufMidInter = iBufMinInterval + (iBufMaxInterval - iBufMinInterval) / 2;
		bool bOnMic = GetMicStatus();
		if (!bOnMic){
			iBufMinInter = iBufMinInterval1;
			iBufMaxInter = iBufMaxInterval1;	
			iBufMidInter = iBufMaxInterval;
		}
		float fx = 0.0f;
		unsigned int iondisf =  iBufMidInter - iBufMinInter;
		unsigned int iondisb =  iBufMaxInter - iBufMidInter;
		float flostf = BUFFER_OUTPUT_VIDEO_LOSTRATE_MIDDLE - BUFFER_OUTPUT_VIDEO_LOSTRATE_MINMUM;
		float flostb = BUFFER_OUTPUT_VIDEO_LOSTRATE_MAXMUM - BUFFER_OUTPUT_VIDEO_LOSTRATE_MIDDLE;
		if (iondisf>0 && iondisb>0){
			//按照丢包率计算缓冲区大小
			if ( 0.0f == fLostRate){
				iBufferSizeTemp = iBufMinInterval1;
			}else 
			if (fLostRate<BUFFER_OUTPUT_VIDEO_LOSTRATE_MINMUM){
				iBufferSizeTemp = iBufMinInterval1;
			}else 
			if (fLostRate >=BUFFER_OUTPUT_VIDEO_LOSTRATE_MINMUM && 
				fLostRate < BUFFER_OUTPUT_VIDEO_LOSTRATE_MIDDLE){ // 丢包率在 1/1000 ~ 5/100之间的计算
					fx = fLostRate/flostf>1.0f?1.0f:fLostRate/flostf;
					iBufferSizeTemp = (float)iondisf * sqrtf( 1 - (1-fx)*(1-fx))+iBufMinInter;//buf和丢包率的关系 y=sqrtf(1-(1-x)*(1-x)),x为丢包率，y为递增缓冲
			}else 
			if (fLostRate>=BUFFER_OUTPUT_VIDEO_LOSTRATE_MIDDLE && 
				fLostRate < BUFFER_OUTPUT_VIDEO_LOSTRATE_MAXMUM){// 丢包率在 5/100 ~ 3/10之间的计算
					fx = fLostRate/flostf>1.0f?1.0f:fLostRate/flostf;
					iBufferSizeTemp = (float)iondisb * sqrtf(1 - (1-fx)*(1-fx))+ iBufMidInter;
			}else 
			if (fLostRate >= BUFFER_OUTPUT_VIDEO_LOSTRATE_MAXMUM){
				iBufferSizeTemp = iBufMaxInter;
			}
			iBufferSizeTemp = iBufferSizeTemp - iBufferSizeTemp%10 + (iBufferSizeTemp%10>0?10:0);//以10为单位
			//与当前缓冲区大小关系
			if (iBufferSizeTemp < iCurBufSize-100){
				UInt32 iDeta = (iCurBufSize - iBufferSizeTemp) / 4;
				iDeta = iDeta < 100 ? 100:iDeta;
				iBufferSizeTemp = iCurBufSize-iDeta;
			}else 
			if (iBufferSizeTemp > iCurBufSize+100){
				UInt32 iDeta = (iBufferSizeTemp - iCurBufSize) / 4;
				iDeta = iDeta < 100 ? 100:iDeta;
				iBufferSizeTemp = iCurBufSize+iDeta;
			}
			//缓冲区变化范围
			if (iBufferSizeTemp <iBufMinInter){
				iBufferSizeTemp = iBufMinInter;
			}else 
			if (iBufferSizeTemp>iBufMaxInter){
				iBufferSizeTemp = iBufMaxInter;
			}
		}
		//比较参照buf大小
		if (GetTickCount() - dwRefRecvTime<500){
			if (iRefenceBufSize>1000){
				iBufferSizeTemp = iRefenceBufSize;
				iBufferSizeTemp += -854; //500ms 音频和视频矫正  音频缓冲延迟500ms,视频双向预测延迟近500ms 相互抵消了
			}
		}
		
		return iBufferSizeTemp;
}

//计算播放数据包之间的时间间隔  数据包的间隔时间
DWORD CBufferOutputVideoRateAgc::CalcSendRate(
							UInt32 iBufSize,			//缓冲区大小   ---时间段  当前缓冲区时间大小
							UInt32 iBufDataSize,        //当前缓冲区中数据个数 ---数据包个数
							UInt32 iBufFrameSize,       //当前帧的实际范围 --数据包帧范围
							float  fRecvRate,			//接收数据的速率 ---收包速率
							UInt32 iLastRate			//老的发包速率 --上次计算得到的发包速率
							)
{
		//当前缓冲区中的帧数是否在指定的范围内
		UInt32 iShake = BUFFER_PERSEC_VIDEO_SHAKE_MIN;
		float fNormalFrameSize = (float)iBufSize * BUFFER_PERSEC_VIDEO_FRAMES / 1000;  //缓冲区缓冲的帧数
		UInt32 iBufferDataTime = iBufFrameSize*100;  //iBufDataSize * 1000 / BUFFER_PERSEC_VIDEO_NUMBER;
		UInt32 iSendFrameRate = 1000 / BUFFER_PERSEC_VIDEO_FRAMES;

		//正常帧的抖动范围内，帧率保持不变
		if(iBufFrameSize <=fNormalFrameSize+iShake && iBufFrameSize>=fNormalFrameSize){
			return  iSendFrameRate;
		}

		//缓冲区抖动范围大小
		iShake = ceil((float)iBufSize*5 / BUFFER_PERSEC_VIDEO_SHAKE_LIMIT);
		iShake = iShake > BUFFER_PERSEC_VIDEO_SHAKE_MAX*2?BUFFER_PERSEC_VIDEO_SHAKE_MAX*2:iShake;

		if (iBufferDataTime <= iBufSize / 4)
		{
			iSendFrameRate += 50;
		}
		else if (iBufferDataTime>iBufSize / 4 && iBufferDataTime<=iBufSize / 2)
		{
			iSendFrameRate += 50;
		}
		else if (iBufferDataTime>iBufSize / 2 && iBufferDataTime<=iBufSize*3/ 4)
		{
			iSendFrameRate += 30;
		}
		else if (iBufferDataTime>iBufSize*3/ 4 && iBufferDataTime<=iBufSize-50)
		{
			iSendFrameRate +=5;
		}
		else if (iBufferDataTime>iBufSize-100 && iBufferDataTime<=iBufSize+100)
		{
			iSendFrameRate +=0;
		}
		else if (iBufferDataTime>iBufSize+100 && iBufferDataTime<=iBufSize*5/4)
		{
			iSendFrameRate -=5;
		}
		else if (iBufferDataTime>iBufSize*5/4 && iBufferDataTime<=iBufSize*6/4)
		{
			iSendFrameRate -=10;
		}
		else if (iBufferDataTime>iBufSize*6/4 && iBufferDataTime<=iBufSize*7/4)
		{
			iSendFrameRate -=30;
		}
		else if (iBufferDataTime>iBufSize*7/4 && iBufferDataTime<=iBufSize*8/4)
		{
			iSendFrameRate -=40;
		}
		else
		{
			iSendFrameRate -=50;
		}

		if (iSendFrameRate - iLastRate > iShake && iSendFrameRate > iLastRate)
		{
			iSendFrameRate = iLastRate+iShake;
		}
		if (iLastRate - iSendFrameRate < iShake && iSendFrameRate < iLastRate)
		{
			iSendFrameRate = iLastRate-iShake;
		}

		return iSendFrameRate;
}