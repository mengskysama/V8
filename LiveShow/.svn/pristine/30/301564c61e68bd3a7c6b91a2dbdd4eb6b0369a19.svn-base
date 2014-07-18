#pragma once
#include "AudioTransport.h"
#include "audio_pch.h"
#include "AvDefine.h"


class AudioInputEngine
{
public:
	AudioInputEngine(void);
	~AudioInputEngine(void);

	static AudioInputEngine g_audioInputEngine;

	void Init(AudioTransport *trans);

	void Destory();

	bool Start();

	void Stop();

	bool IsInit();

	bool IsStart();

	//设备选择
	std::vector<wstring> GetAudioInputDevice();

	void SetAudioDevice(int iInputDevice);

	void SetVolumeInput( int iVolume);
	int  GetVolumeInput();

	//音量大小
	int GetInputLevel();

	void SetAudioTest(bool bAudioTest);

	bool SetMicInputMute(bool bMute);
	bool GetMicInputMute(bool& bMute);

	void SetMicRoomAllVolume(int iVolume);
	int  GetMicRoomAllVolume();
	
	//设置音频编码参数
	bool SetMode(enuROOM_AV_AUDIO_MODE mode);
private:
	bool bInit;
	bool bStart;

	std::vector<wstring> lstrInputDevice;
};

extern AudioInputEngine g_audioInputEngine;
