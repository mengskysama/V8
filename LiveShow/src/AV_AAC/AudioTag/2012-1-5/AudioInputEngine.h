#pragma once
#include "AudioTransport.h"
#include "audio_pch.h"


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

	//音量大小
	int GetInputLevel();

	int GetOutputLevel();

	void SetAudioTest(bool bAudioTest);
private:
	bool bInit;
	bool bStart;

	std::vector<wstring> lstrInputDevice;
};

extern AudioInputEngine g_audioInputEngine;
