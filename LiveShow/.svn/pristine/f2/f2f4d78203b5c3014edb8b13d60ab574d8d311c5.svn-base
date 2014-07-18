#pragma once
#include "RC.h"
#include "BufferInPlace.h"
#include "BufferBaseRate.h"
#include "CommonPacket.h"
#include "BufferDefine.h"
#include <map>

using namespace std;
class CBufferOutputVideoTask;
class CBufferOutputVideoEngine:public RCObject
{
friend class CBufferOutputVideoTask;
public:
	CBufferOutputVideoEngine(UInt32 iRoomId,UInt32 iFromUin);
	~CBufferOutputVideoEngine();

	void	Initialize(ITransInPlace* pInPlace,IBufferBaseRate* pRate);
	void	UnInitialize();
	void	AddPacket(Packet* pData,UInt32 iSequence);
	void	ClearPackets();
	bool	IsCanRemove();
	void	Play();
protected:
	bool	GetFrameSeq(Packet* pDataPacket,UInt32& iFrameSeq,UInt32& iPieceCount);//获取帧Sequence
	bool	ParsePack(const UInt8* pBuf, UInt32 iLen,UInt32& iFrameSeq,UInt32& iPieceCount);//解析内部数据包获取帧Sequence
	void	ParseAllPack(Packet* pPacket,UInt32& iFrameSeq,bool& bSucc);//一个数据包内包含完整的一帧图像数据解析
	void	ParsePiecePack(Packet* pPacket,UInt32& iFrameSeq,UInt32& iPieceCount,bool& bSucc);//一个数据包只是一帧图像的一片数据解析
	bool	IsNormalPacket(UInt32 iFrameId,UInt32 iSequenceId); //收到的数据是否为过期无效数据判断
	void	AddFrameSeq(UInt32 iFrameId);//添加帧序号
	void	AddSequenceId(UInt32 iSequenceId);//添加队列序号
	void	UpdateTime(DWORD dwCurTime,UInt32 iFrameId);//更新收包计数
	float	CalcRecevRate(); //计算收包速率
	float	CalcRecevFrameRate();//计算收帧速率
	bool	IsCanCalcPlayRate(); //当前是否可以计算播放速率了
	void    PlayFrames();//播放帧
	void    PlayPackets();//播放包
	Packet* GetPlayPacket(UInt32 iSequenceId);//获取当前播放的数据包
	void    RemoveExpireFrame();//移除无效的帧
	void    CheckPacket();//数据校验、补偿
	bool    CanCalcAudioParam();//可以计算音频参数
	void    ResetStatus();//重置状态
	void    RequestCompenPacket(UInt32 iSequenceId,bool bA);
protected:
	static map<UInt64,CBufferOutputVideoEngine*> m_mapVideoOutput; //视频所有实例
	static Mutex m_static_mutex; //网络线程 播放线程 m_mapVideoOutput

	Mutex   m_mutex; //m_mapOutputVideoBuf buffer播放线程 m_mapFrameToSeq
	map<UInt32,StructVideoBufferPack> m_mapOutputVideoBuf;//视频缓存数据包
	typedef pair<UInt32,StructVideoBufferPack> pair_outputVB;
	map<UInt32,StructVideoFrameSeqToSeq> m_mapFrameToSeq; //iframeId -- isequenceId
	typedef pair<UInt32,StructVideoFrameSeqToSeq> pair_outputFB;
	map<UInt32,StructVideoRequestInfo> m_mapOutputVideoReqInf;//补偿包相关信息
	typedef pair<UInt32,StructVideoRequestInfo> pair_outputRI;
private:
	ITransInPlace* m_pInPlace;
	IBufferBaseRate* m_pRate;

	UInt32		m_iFromUin;				//被接收音频用户UIN
	UInt32		m_iRoomId;				//房间ID
	UInt32		m_iMinFrameId;			//最小帧ID
	UInt32		m_iMaxFrameId;			//最大帧ID
	UInt32		m_iPlayFrameId;			//当前播放的帧ID
	UInt32      m_iWillPlayFrameId;     //即将播放的帧ID
	UInt32		m_iMinSequenceId;		//最小序号ID
	UInt32		m_iMaxSequenceId;		//最大序号ID
	UInt32		m_iCheckSeqId;			//当前检查的序号ID
	UInt32		m_iCheckFrameId;		//当前检测的帧序号ID
	UInt32		m_iTotalPacketNum;		//计时开始总共收到的数据包总和
	UInt32		m_iLostPacketNum;       //计时开始总共丢失的数据包总和
	float		m_fLostRate;            //上一次计时丢包率
	DWORD		m_dwLastLostTime;		//上一次计算丢包率时间
	DWORD		m_lastRecvTime;         //上次接收数据包时间

	DWORD		m_dwFirstStartTime;	    //第一次计时
	DWORD		m_dwSecondStartTime;    //第二次计时
	DWORD		m_dwLostStartTime;      //计算丢包开始时间
	UInt32		m_iFirstStartNum;       //第一次计时开始到现在收到的数据包
	UInt32		m_iSecondStartNum;      //第二次计时开始到现在收到的数据包
	DWORD		m_dwLastRecvRateTime;   //上一次计算接收数据包速率的开始时间
	UInt32		m_iLastRecvRateInter;   //上一次计算的的间隔时间
	UInt32		m_iBufferSize;          //缓冲区大小
	UInt32      m_iAudioBufferSize;     //音频缓冲区大小  ms
	DWORD       m_dwLastRecvAudioTime;  //最近一次接收音频数据时间
	DWORD       m_dwStartCalcAudioParam;//开始获取音频缓冲大小及接收数据情况开始时间
	bool        m_bStartPlay;           //是否首次播放
	DWORD		m_dwLastCalcBufSizeTime; //上次计算缓冲区大小时间间隔

	UInt32		m_iFirstStartFrameSeq;    //第一计时开始帧序号
	UInt32		m_iFirstCurFrameSeq;	  //第一计时当前帧序号
	UInt32		m_iSecondStartFrameSeq;   //第二计时开始帧序号
	UInt32		m_iSecondCurFrameSeq;     //第二计时当前帧序号
	UInt32		m_iFirstStartFrameNum;    //第一计时帧数目
	UInt32		m_iSecondStartFrameNum;   //第二计时帧数目

	float		m_fRecvRate;			  //当前收包率
	float		m_fRecvFrameRate;         //当前接收帧速率
	DWORD		m_dwLastPlayRateTime;     //上次变更播放速率时间 
	DWORD		m_dwSendRate;			  //播放数据的速率 数据包之间时间间隔
	DWORD		m_dwLastPlayTime;		  //上一次播放媒体数据包时间 控制每播放一个数据包间隔时间
	DWORD		m_dwStartPlayTime;        //开始计时播放媒体时间
	UInt32		m_iStartPlayCount;        //开始播放媒体计数
	bool		m_bFilled;				  //是否缓冲足够的数据
};
