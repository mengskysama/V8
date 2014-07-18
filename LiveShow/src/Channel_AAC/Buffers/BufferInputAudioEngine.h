#pragma once
#include "BufferDefine.h"
#include "CommonPacket.h"
#include "Mutex.h"
#include <map>
using namespace std;

class CBufferInputAudioEngine
{
public:
	CBufferInputAudioEngine();
	~CBufferInputAudioEngine();

	void	Initialize();
	void	UnInitialize();

	void	AddPacket(Packet* pDataPacket,UInt32 iSequence); //添加网络语音数据包
	bool	GetPacket(Packet* &pDataPacket,UInt32 iSequence); //获取网络语音数据包
	void	ClearPackets();//清理所有的网络数据包
	void	KeepBalance();//维持指定时间长度缓冲数据
	bool	GetEngineUsed();//获取当前对象是否在使用
	void	EchoAudioInputInfo();//音频信息
protected:
	void	AddSequence(UInt32 iSequence);//添加网络数据包序列号
	void	AddFrameSeq(UInt32 iFrameSeq);//添加数据帧序列
	bool	GetFrameSeq(Packet* pDataPacket,UInt32& iFrameSeq);//获取音频数据帧序列号
	bool	ParsePacket(const UInt8* pBuf, UInt32 iLen,UInt32& iFrameSeq);//解析音频数据包
	void	RemoveFrame(UInt32 iFrameSeq);//移除一帧数据
	void	IncFrameSeq(UInt32& iFrameSeq);//帧编号递增
	void	RemoveTimeOut();//超时移除数据
	bool	FindSeqFromFrameSeq(UInt32 iFrameSeq,UInt32& iSequence);//通过帧序列号 找对应的网络序列号
	void	RemoveSequence(UInt32 iSequenceId); //移除超过指定长度的数据包
	void	IncSequence(UInt32& iSequenceId); //移动序号
	bool	FindSequence(UInt32& iSequence);//找到网络序列号
private:
	map<UInt32,StructAudioBufferPack> m_audioBufferMap;//sequence to packet map
	map<UInt32,UInt32> m_FrameToSeqMap;//frameseq to sequence map
	Mutex	m_Mutex;
	UInt32	m_minSequence; //最小网络包序列号
	UInt32	m_maxSequence; //最大网络报序列号
	UInt32	m_minFrameSeq; //最小语音数据帧序列号
	UInt32	m_maxFrameSeq; //最大语音数据帧序列号
	UInt32	m_keepFrameNum;//保持每秒帧数
	DWORD	m_minFrameInterval;//最小帧时间戳
	DWORD	m_maxFrameInterval;//最大帧时间戳
	DWORD	m_minSequeInterval;//最小序号时间戳
	DWORD	m_maxSequeInterval;//最大序号时间戳
};
