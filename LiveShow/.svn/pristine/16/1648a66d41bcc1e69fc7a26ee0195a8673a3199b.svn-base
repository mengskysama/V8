#pragma once
#include "CommonPacket.h"
#include "RC.h"
#include "BufferInputVideoEngine.h"
#include "BufferInputAudioEngine.h"

class CBufferInputEngine:public RCObject
{
public:
	CBufferInputEngine(UInt32 iRoomId);
	~CBufferInputEngine();

	void	Initialize();
	void	UnInitialize();

	void	AddInputPacket(Packet* pDataPacket,UInt32 iSequence,bool bA=false); //添加保存采集音频视频数据包
	bool	GetInputPacket(Packet* &pDataPacket,UInt32 iSequence,bool bA=false); //获取采集数据的指定数据包
	void	ClearInputPackets(bool bA=false);//清理输入数据包

	void	KeepBalance();//维持指定时间长度缓冲数据
	bool	GetEngineUsed();//获取当前对象是否在使用

	void	EchoAudioInputInfo();
	void	EchoVideoInputInfo();
protected:
private:
	UInt32 m_iRoomId;
	CBufferInputVideoEngine m_inputVideoEngine;
	CBufferInputAudioEngine m_inputAudioEngine;
};