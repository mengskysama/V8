#pragma once
#include "Common.h"

//码率控制基类
class IBufferBaseRate
{
public:
	IBufferBaseRate():m_bOnMic(false){}

	//设置当前房间是否在麦上
	virtual void SetMicStatus(bool bEnable)
	{ m_bOnMic = bEnable;}
	
	//获取是否在麦上状态
	virtual bool GetMicStatus(){ return m_bOnMic; }

	//获取当前速率计算更新时间间隔 根据不同BUFFER长度适当调整
	virtual DWORD GetRateTimeInterval(
		UInt32 iBufMinInterval,		//缓冲区最小缓冲时间 - 麦上 最小
		UInt32 iBufMaxInterval,		//缓冲区最大缓冲时间 - 麦上 最大
		UInt32 iBufMinInterval1,	//缓冲区最小缓冲时间 - 麦下 最小
		UInt32 iBufMaxInterval1,	//缓冲区最大缓冲时间 - 麦下 最大
		UInt32 iBufSize,			//当前缓冲区大小
		UInt32 iLastRecvRate,       //上次计算的间隔时间
		DWORD& dwLastRecvTime       //上次计算收包速率的时间
		) = 0;

	//返回请求补偿包下次请求的时间间隔
	virtual DWORD CalcRequestCompenInterval(
		UInt32 iBufferSize,			//缓冲区总大小
		UInt32 iBufferDataSize,		//当前缓冲区数据个数
		float  fRecvRate			//收到数据包的速率
		) = 0;

	//计算Buffer大小 根据网络丢包率 来动态调整BUFFER大小  丢包越少BUFFER越小 丢包越多BUFFER越大，为补偿提供相对充足的时间
	virtual UInt32 CalcBufferSize(
		UInt32 iBufMinInterval,		//缓冲区最小缓冲时间 - 麦上 最小
		UInt32 iBufMaxInterval,		//缓冲区最大缓冲时间 - 麦上 最大
		UInt32 iBufMinInterval1,	//缓冲区最小缓冲时间 - 麦下 最小
		UInt32 iBufMaxInterval1,	//缓冲区最大缓冲时间 - 麦下 最大
		UInt32 iAverageRate,		//平均速率 每秒接收到的数据包个数
		float  fLostRate,			//当前丢包率
		DWORD& dwLastTime,          //上一次计算缓冲区时间
		UInt32 iCurBufSize,         //当前缓冲区大小
		UInt32 iRefenceBufSize,     //参照buf大小，用于音视频同步
		DWORD  dwRefRecvTime        //参照最近一次接收数据时间
		) =0;

	//计算播放数据包之间的时间间隔  数据包的间隔时间
	virtual DWORD CalcSendRate(
		UInt32 iBufSize,			//缓冲区大小
		UInt32 iBufDataSize,        //当前缓冲区中数据大小
		UInt32 iBufFrameSize,       //当前帧的实际范围
		float  fRecvRate,			//接收数据的速率
		UInt32 iLastRate			//老的发包速率
		)=0;
protected:
	bool m_bOnMic; //是否在麦上 默认不在麦上 TRUE:在麦上  FALSE:不在麦上
};