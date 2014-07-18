#pragma once
#include "AudioTransport.h"
#include "audio_pch.h"
#include "Common.h"

#define AUDIO_LOST_PACKET_TIME_INTERVAL  30*1000

class ClientUser;

class AudioOutputEngine
{
public:
	AudioOutputEngine(void);
	~AudioOutputEngine(void);

	static AudioOutputEngine g_audioOutputEngine;
	void Init(AudioTransport *trans);
	void Destory();
	bool Start();
	void Stop();
	bool IsInit();
	bool IsStart();
	//音频测试
	void SetAudioTest(bool bAudioTest);
	//增加语音对象
	void AddUserSession(UInt64 uin);
	//删除语音对象
	void RemoveUserSession(UInt64 uin);
	//移除该房间语音对象
	void RemoveRoomSession(UInt32 iRoomId);
	//移除所有房间语音对象
	void RemoveRoomAllSession();
	//暂停语音对象
	void PauseUserSession(UInt64,bool);

	//处理接收到的语音数据
	void OnRecvAudioData(UInt64 uin, unsigned char* data, unsigned int len,unsigned int iSeq);
	std::vector<wstring> GetAudioOutputDevice();
	void SetAudioOutputDevice(int iOutputDevice);

	//音量调节 所有房间
	void SetVolumeOutput(int iVolume);
	int GetVolumeOutput();

	//系统选择
	void SetSystemType(wstring strAudioSystem);

	int GetOutputLevel();

	//设置输出系统音量
	void SetSysVolume(int iVolume);
	
	//获取系统输出音量
	int GetSysvolume();

	bool SetSysPlayerMute(bool bMute);
	bool GetSysPlayerMute(bool& bMute);

	float GetLostPacketRate();
private:
	bool bInit;
	bool bStart;

	int nRcvTotalPacket; //收到的包
	int nRcvLostPacket;  //丢掉的包
	float fLostRate;     //丢包率
	UInt64 iTimeInterval; //计算丢包的时间间隔
	UInt64 iLastTimeTick; //上次计算时间

	std::vector<wstring> lstrOutputDevice;

	void CalcLostRate(ClientUser *p,unsigned int iSeqence);
};

extern AudioOutputEngine g_audioOutputEngine;
