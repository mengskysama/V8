#pragma once
#include "CommonPacket.h"

/*
对外提供统一管理接口
*/
class CBufferManager
{
public:
	CBufferManager();
	~CBufferManager();

	void	Initialize();//初始化
	void	UnInitialize();//卸载资源
	void    DestroyInstance();//销毁播放线程
	
	//		input control
	void	AddInputPacket(UInt32 iRoomId,Packet* pDataPacket,UInt32 iSequence,bool bA=false); //添加保存采集音频视频数据包
	bool	GetInputPacket(UInt32 iRoomId,Packet* &pDataPacket,UInt32 iSequence,bool bA=false); //获取采集数据的指定数据包
	void	ClearInputPackets(UInt32 iRoomId,bool bA=false);//清理指定房间输入数据包

	//		output control
	void	AddOutputPacket(UInt32 iRoomId,UInt32 iFromUin,Packet* pDataPacket,UInt32 iSequence,bool bA=false);//添加收到的音频视频数据包
	void	ClearOutputPackets(UInt32 iRoomId,UInt32 iFromUin,bool bA=false);//清理指定房间用户的输出数据包
	void	SetRoomOnMic(UInt32 iRoomId,bool bEnable);//设置指定房间是否为上麦状态
	
	//		打印当前缓冲区序号
	void	EchoAudioInputInfo(UInt32 iRoomId);
	void	EchoVideoInputInfo(UInt32 iRoomId);

	bool    IsRelease(){ return m_isRelease; }
	void    SetRelease(bool b){ m_isRelease = b;}
private:
	volatile bool m_isRelease;
};

extern CBufferManager g_bufferManager;