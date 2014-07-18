#pragma once
#include "Common.h"
#include "RC.h"
#include "BufferInPlace.h"
#include "BufferBaseRate.h"
#include "BufferDefine.h"
#include <map>

using namespace std;

class CBufferOutputAudioTask;
class CBufferOutputAudioEngine:public RCObject
{
friend class CBufferOutputAudioTask;
friend class CBufferOutputEngine;
public:
	CBufferOutputAudioEngine(UInt32 iRoomId,UInt32 iFromUin);
	~CBufferOutputAudioEngine();

	void Initialize(ITransInPlace* pInPlace,IBufferBaseRate* pRate);
	void UnInitialize();

	void AddPacket(Packet* pData,UInt32 iSequence);
	void ClearPackets();
	bool IsCanRemove();
	void Play();
protected:
	void AddFrameSeq(UInt32 iFrameId); //添加帧序号 保存当前BUFFER中最大 最小数据帧
	void AddSequenceId(UInt32 iSequenceId); //添加队列序号 保存当前BUFFER中 最大 最小序列号
	void UpdateTime(DWORD dwCurTime);//更新收包计数
	void PlayPackets();//播放数据包
	void CheckPacket(); //第三个包检查第一个数据包完整 
	void RequestCompenPacket(UInt32 iSequenceId,bool bA); //请求补偿数据包
	void RefreshStatus();//更新当前音频缓冲长度状态
	void ResetStatus();//重置缓冲区状态
	bool IsNormalPacket(UInt32 iFrameId,UInt32 iSequenceId); //收到的数据是否为过期无效数据判断
	bool IsCanCalcRecvRate();//是否可以计算收包速率
	float CalcRecevRate();//计算收包速率
	Packet* GetPlayPacket(UInt32 iSequenceId);//获取当前播放的数据包
	double CanCalcPlayRate();//计算播放速率
	
	map<UInt32,StructAudioBufferPack> m_mapOutputAudioBuf;//音频缓存数据包
	typedef pair<UInt32,StructAudioBufferPack> pair_outputAB;
	map<UInt32,StructAudioRequestInfo> m_mapOutputAudioReqInf;//补偿包相关信息
	typedef pair<UInt32,StructAudioRequestInfo> pair_outputRI;

	static map<UInt64,CBufferOutputAudioEngine*> m_mapAudioOutput; //音频所有实例
	static Mutex m_static_mutex;//网络线程 播放线程 m_mapAudioOutput
private:
	ITransInPlace* m_pInPlace;
	IBufferBaseRate* m_pRate;

	Mutex	m_mutex;				 //播放线程 网络线程 m_mapOutputAudioBuf
	float	m_fLostRate;             //上一次计时丢包率
	DWORD	m_dwLastLostTime;		 //上一次计算丢包率时间
	DWORD   m_lastRecvTime;			 //最近一次接收数据时间
	UInt32	m_iTotalPacketNum;		 //计时开始总共收到的数据包总和
	UInt32	m_iLostPacketNum;        //计时开始总共丢失的数据包总和
	
	DWORD	m_dwLastRecvRateTime;    //上一次计算接收数据包速率的开始时间
	DWORD	m_dwFirstStartTime;		 //第一次计时
	DWORD	m_dwSecondStartTime;     //第二次计时
	UInt32	m_iBufferSize;           //缓冲区大小
	UInt32	m_iFirstStartNum;        //第一次计时开始到现在收到的数据包
	UInt32	m_iSecondStartNum;       //第二次计时开始到现在收到的数据包
	UInt32  m_iLastRecvRateInter;	 //上一次计算的的间隔时间

	UInt32	m_iFromUin;				//被接收音频用户UIN
	UInt32	m_iRoomId;				//房间ID
	UInt32	m_iMinFrameId;			//最小帧ID
	UInt32	m_iMaxFrameId;			//最大帧ID
	UInt32	m_iMinSequenceId;		//最小序号ID
	UInt32	m_iMaxSequenceId;		//最大序号ID
	UInt32	m_iCheckSeqId;			//当前检查的序号ID
	bool    m_bFilled;              //是否缓冲足够的数据
	float   m_fRecvRate;			//收包速率
	DWORD   m_dwLastCalcRecvRate;   //上次计算收包速率时间
	DWORD   m_fLastPlayTime;        //上次播放数据包的时间
	UInt32  m_iStartPlayCount;      //开始计时起播放的数据包数
	double  m_fSendRate;			//每次发送数据包间隔时间
	bool    m_bAjustSendRate;       //是否开始调整播放速率

	bool    m_bShakeKeepRate;       //抖动检测保持一定时间的稳定速率
	DWORD   m_dwShakeCheckStart;

};
