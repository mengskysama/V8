#pragma once
#include "AudioTransport.h"
#include "audio_pch.h"
#include "Common.h"


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
	void OnRecvAudioData(UInt64 uin, unsigned char* data, unsigned int len);

	std::vector<wstring> GetAudioOutputDevice();

	void SetAudioOutputDevice(int iOutputDevice);

	//音量调节
	void SetVolumeOutput(int iVolume);

	int GetVolumeOutput();

	//系统选择
	void SetSystemType(wstring strAudioSystem);

	int GetOutputLevel();

	
private:
	bool bInit;
	bool bStart;

	std::vector<wstring> lstrOutputDevice;
};

extern AudioOutputEngine g_audioOutputEngine;
