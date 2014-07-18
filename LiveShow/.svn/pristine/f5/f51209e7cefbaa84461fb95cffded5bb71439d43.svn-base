#pragma once
#include "CommonPacket.h"
#include "BufferDefine.h"
#include "Mutex.h"
#include <map>
using namespace std;


class CBufferInputVideoEngine
{
public:
	CBufferInputVideoEngine();
	~CBufferInputVideoEngine();
	
	void	Initialize();
	void	UnInitialize();

	void	AddPacket(Packet* pDataPacket,UInt32 iSequence);//添加视频数据包
	void	KeepBalance();//维持指定时间长度缓冲数据
	bool	GetPacket(Packet* &pDataPacket,UInt32 iSequence);//获取数据包  根据网络包序号
	void	ClearPackets();//退出清理数据包
	bool	GetEngineUsed();//获取对象是否在使用
	void	EchoVideoInputInfo();//音频信息
private:
	void	AddSequence(UInt32 iSequence);//添加数据包序号
	void	AddFrameSeq(UInt32 iFrameSeq);//添加的帧序号
	UInt32	GetPackCount();//获取数据包个数
	bool	GetFrameSeq(Packet* pDataPacket,UInt32& iFrameSeq);//获取帧Sequence
	bool	ParsePack(const UInt8* pBuf, UInt32 iLen,UInt32& iFrameSeq);//解析内部数据包获取帧Sequence
	void	ParseAllPack(Packet* pPacket,UInt32& iFrameSeq,bool& bSucc);//一个数据包内包含完整的一帧图像数据解析
	void	ParsePiecePack(Packet* pPacket,UInt32& iFrameSeq,bool& bSucc);//一个数据包只是一帧图像的一片数据解析
	void	RemoveFrame(UInt32 iFrameSeq);//移除一帧数据
	void	IncFrameSeq(UInt32& iFrameSeq);//帧数移动一帧
	void	RemoveTimeOut();//超时移除
private:
	UInt32	m_minSequence;//当前视频数据包最小编号
	UInt32	m_maxSequence;//当前视频数据包最大编号
	UInt32	m_minFrameSeq;//当前最小视频帧序号
	UInt32	m_maxFrameSeq;//当前最大视频帧序号
	DWORD	m_minFrameInterval;//最小帧时间戳
	DWORD	m_maxFrameInterval;//最大帧时间戳
	UInt32	m_keepFrameNum;//缓冲区保存帧数目
	Mutex	m_mutex;
	map<UInt32,StructVideoBufferPack> m_videoBufferMap;
	map<UInt32,StructVideoFrameSeqToSeq> m_frameToSeqMap;

};