#pragma once
#include "RC.h"
#include "Common.h"
#include "CommonPacket.h"
#include "BufferInPlace.h"
#include "BufferBaseRate.h"
#include "BufferOutputAudioEngine.h"
#include "BufferOutputVideoEngine.h"
#include <map>

using namespace std;

class CBufferOutputEngine:public RCObject,public ITransInPlace
{
public:
	CBufferOutputEngine(UInt32 iRoomId);
	~CBufferOutputEngine();
	
	void Initialize();
	void UnInitialize();

	void AddOutputPacket(UInt32 iFromUin,Packet* pDataPacket,UInt32 iSequence,bool bA);
	void ClearOutputPackets(UInt32 iFromUin,bool bA=false);

	void SetRoomOnMic(bool bEnable); 
	void ClearExpireUser();
	unsigned int GetBufferSize(unsigned int uin,En_Buffer_Type en);
	void SetBufferSize(unsigned int uin,En_Buffer_Type en,unsigned int bufsize);
	virtual void SetRecvTime(unsigned int uin,En_Buffer_Type en,DWORD dwtime);
	virtual DWORD GetRecvTime(unsigned int uin,En_Buffer_Type en);
protected:
	RCPtr<CBufferOutputAudioEngine> GetBufferOutputEngineA(UInt32 iFromUin);
	RCPtr<CBufferOutputVideoEngine> GetBufferOutputEngineV(UInt32 iFromUin);
private:
	Mutex m_mutexA;	//播放线程 socket线程 m_mapOutputAudioEngine
	Mutex m_mutexV; //播放线程 socket线程 m_mapOutputVideoEngine
 	UInt32 m_iRoomId;
	IBufferBaseRate* m_pIBaseAudioRate; //当前房间语音速率控制用户逻辑计算类
	IBufferBaseRate* m_pIBaseVideoRate; //当前房间视频速率控制用户逻辑计算类
	std::map<unsigned int,unsigned int> m_mapAudioBufferSize;//存放音频缓冲区大小 fromuin <--> buffersize
	std::map<unsigned int,unsigned int> m_mapVideoBufferSize;//存放视频缓冲区大小 fromuin <--> buffersize
	std::map<unsigned int,DWORD> m_mapAudioBufferRecvTime;//存放音频接收数据开始时间 fromuin <--> recvtime
	std::map<unsigned int,DWORD> m_mapVideoBufferRecvTime;//存放视频接收数据开始时间 fromuin <--> recvtime
	map<UInt32,RCPtr<CBufferOutputAudioEngine>> m_mapOutputAudioEngine;// FromUin -- OutputAudioEngine
	map<UInt32,RCPtr<CBufferOutputVideoEngine>> m_mapOutputVideoEngine;// FromUin -- OutputVideoEngine
	typedef pair<UInt32,RCPtr<CBufferOutputAudioEngine>> pair_outputA;
	typedef pair<UInt32,RCPtr<CBufferOutputVideoEngine>> pair_outputV;
};