#pragma once
#ifdef _DEBUG
//#include <vld.h>
#endif
#include "CommonPacket.h"

#define BUFFER_OUTPUT_ONMIC_AUDIO_MIN_LIMIT		2500//500//300  //麦上用户之间音频缓冲毫秒上下限
#define BUFFER_OUTPUT_ONMIC_AUDIO_MAX_LIMIT		2500//800//500
#define BUFFER_OUTPUT_UNMIC_AUDIO_MIN_LIMIT		2500//1000//300  //麦下用户之间音视频缓冲毫秒上下限
#define BUFFER_OUTPUT_UNMIC_AUDIO_MAX_LIMIT		2500//3000
#define BUFFER_OUTPUT_ONMIC_VIDEO_MIN_LIMIT		2000//500  //麦上用户之间视频缓冲毫秒上下限
#define BUFFER_OUTPUT_ONMIC_VIDEO_MAX_LIMIT		2000//800
#define BUFFER_OUTPUT_UNMIC_VIDEO_MIN_LIMIT		2000//1000//500  //麦下用户之间视频缓冲毫秒上下限
#define BUFFER_OUTPUT_UNMIC_VIDEO_MAX_LIMIT		2000//3000 
#define BUFFER_OUTPUT_RATECALC_MIN_INTERVAL		5000//计算接收数据包速率的时间间隔 最小
#define BUFFER_OUTPUT_RATECALC_MAX_INTERVAL		5000//计算接收数据包速率的时间间隔 最大
#define BUFFER_OUTPUT_PLAYRATE_CAL_INTERVAL     1000  //播放速度计算间隔时间 --音频
#define BUFFER_OUTPUT_PLAYRATE_CAL_INTERVAS     300 //播放速度计算间隔时间 视频
// #define BUFFER_OUTPUT_PLAYRATE_ALLOW__SHAKE		1.5f //计算播放速度时，缓冲区中的数据个数允许抖动的值
#define BUFFER_OUTPUT_FRESHPACK_MS_INTERVAL		1000 //播放数据计时刷新时间间隔
// #define BUFFER_OUTPUT_FRESHPACK_AO_INTERVAL     2000 //播放数据音频刷新时间间隔
#define BUFFER_OUTPUT_FRAMEPLAY_MS_INTERVAL     1000 //播放数据帧时 计时刷新时间间隔
// #define BUFFER_OUTPUT_PLAYRATE_CHANGE_LIMIT		5    //播放数据抖动范围
// #define BUFFER_OUTPUT_PLAYAUDIO_INTER_LIMIT     2    //播放音频数据抖动范围
#define BUFFER_OUTPUT_BUFFER_SHAKE_INTERVAL     500  //Buffer变化的时间间隔
#define BUFFER_OUTPUT_BUFFER_SHAKE_LOSTRATE     1000*30 //计算丢包率间隔时间
#define BUFFER_OUTPUT_BUFFER_RECVRATE_SHAKE		1000  //计算收包速率的间隔时间
// #define BUFFER_OUTPUT_VIDEO_FRAMEPACK_INTER		50 //默认播放帧间数据间隔时间
#define BUFFER_OUTPUT_BUFFER_CALC_AUDIO_PARAM   100   //获取音频参数的时间间隔

#define BUFFER_OUTPUT_AUDIO_LOSTRATE_MINMUM     0.001f//音频丢包率上限 1/1000
#define BUFFER_OUTPUT_AUDIO_LOSTRATE_MIDDLE		0.050f//音频丢包率分界 5/100
#define BUFFER_OUTPUT_AUDIO_LOSTRATE_MAXMUM     0.200f//音频丢包率下限 2/10

#define BUFFER_OUTPUT_VIDEO_LOSTRATE_MINMUM     0.001f//视频丢包上限  1/1000
#define BUFFER_OUTPUT_VIDEO_LOSTRATE_MIDDLE     0.050f//视频丢包分界  3/100
#define BUFFER_OUTPUT_VIDEO_LOSTRATE_MAXMUM     0.200f//视频丢包分界  2/10
 
#define BUFFER_OUTPUT_LOST_COMPENSATION_MIN     280 //丢包补偿请求间隔时间下限
#define BUFFER_OUTPUT_LOST_COMPENSATION_MAX     800 //丢包补偿请求间隔时间上限

#define BUFFER_PERSEC_AUDIO_NUMBER          23.4375f/*17*/    //音频每秒接收数据包个数 供计算参考  //=========50
#define BUFFER_PERSEC_VIDEO_NUMBER			22    //视频每秒接收数据包个数 供计算参考
#define BUFFER_INPUT_AUDIO_LIMIT			3000  //采集到的音频数据缓存毫秒数
#define BUFFER_INPUT_VIDEO_LIMIT			3000  //采集到的视频数据缓存毫秒数
#define BUFFER_INPUT_AUDIO_TIME_OUT			3     //音频数据超时
#define BUFFER_INPUT_VIDEO_TIME_OUT         3	  //视频数据超时
#define BUFFER_PERSEC_VIDEO_FRAMES          10    //视频每秒帧数
#define BUFFER_PERSEC_VIDEO_SHAKE_MIN	    1     //视频缓冲帧 最小抖动
#define BUFFER_PERSEC_VIDEO_SHAKE_MAX       5	  //视频缓冲帧 最大抖动
#define BUFFER_PERSEC_VIDEO_SHAKE_LIMIT     1000  //视频缓冲帧 抖动
#define BUFFER_PERSEC_AUDIO_SEND_INTER		(1000.0f/BUFFER_PERSEC_AUDIO_NUMBER)//42.6668f/*(1000/BUFFER_PERSEC_AUDIO_NUMBER)*/    //发送数据包间隔时间  //=================50
#define BUFFER_PERSEC_AUDIO_SHAKE_LIMIT     4     //音频数据包个数抖动范围
#define BUFFER_PERSEC_AUDIO_SHAKE_CHECK     3000  //3s钟检测一次接收数据是否稳定

#define BUFFER_OUTPUT_AUDIO_PACKET_MAXLIMIT     BUFFER_PERSEC_AUDIO_NUMBER*30 //语音数据包 buffer中存包最大限制数
#define BUFFER_OUTPUT_VIDEO_PACKET_MAXLIMIT     BUFFER_PERSEC_VIDEO_NUMBER*30 //视频数据包 buffer中存包最大限制数
 
#define BUFFER_COMPENSATION_NUMBER          1     //请求补偿包发送个数
// #define BUFFER_COMPEN_REQUE_INTERVAL_MIN    200   //请求补偿数据包最小时间间隔
// #define BUFFER_COMPEN_REQUE_INTERVAL_MAX    500   //请求补偿数据包最大时间间隔
#define BUFFER_COMPEN_REQUE_TIMES           3     //请求补偿最多次数
#define BUFFER_VIDEO_PREVIEW_TIME           5*1000 //视频开始播放前5s钟正常速度播放，加快显示图像，然后再缓存2s来同步并补偿
#define BUFFER_MEDIA_EXPIRE_TIME			5*60*1000  //对象不使用失效时间
#define BUFFER_MEDIA_CHECK_TIME				5*60*1000  //检查对象时间

//音频数据包保存结构
struct StructAudioBufferPack
{
	UInt32	m_iSequence;
	UInt32  m_iFrameSeq;
	Packet* m_pPack;
	DWORD  m_time;
public:
	StructAudioBufferPack()
	{
		m_pPack = NULL;
		m_iSequence = 0;
		m_time=0;
		m_iFrameSeq = 0;
	}
	~StructAudioBufferPack(){}
};

//视频数据包保存结构
struct StructVideoBufferPack
{
	UInt32	m_iSequence;//数据包序号
	UInt32  m_iFrameSeq;//帧序号
	UInt32  m_iPieceCount;
	Packet* m_pPack;
	DWORD	m_time;
public:
	StructVideoBufferPack()
	{
		m_pPack = NULL;
		m_iSequence = 0;
		m_time=0;
		m_iFrameSeq = 0;
		m_iPieceCount=0;
	}
	~StructVideoBufferPack(){}
};

//保存每帧对应的序列号
struct StructVideoFrameSeqToSeq
{
	UInt32 m_minSequence;
	UInt32 m_maxSequence;
};

//保存补偿数据包
struct StructAudioRequestInfo 
{
	UInt32 m_iSequence; //补偿包序号
	DWORD  m_lastTime;  //上次补偿时间
	UInt32 m_iReqNum;	//已经补偿次数
public:
	StructAudioRequestInfo()
	{
		m_iSequence = 0;
		m_lastTime = 0;
		m_iReqNum = 0;
	}
	~StructAudioRequestInfo(){}
};

struct StructVideoRequestInfo 
{
	UInt32 m_iSequence; //补偿包序号
	DWORD  m_lastTime;  //上次补偿时间
	UInt32 m_iReqNum;	//已经补偿次数
public:
	StructVideoRequestInfo()
	{
		m_iSequence = 0;
		m_lastTime = 0;
		m_iReqNum = 0;
	}
	~StructVideoRequestInfo(){}
};


