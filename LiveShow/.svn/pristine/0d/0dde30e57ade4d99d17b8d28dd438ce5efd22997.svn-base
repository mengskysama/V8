#ifndef _SETTINGS_H
#define _SETTINGS_H

#include "stdafx.h"
#include "audio_pch.h"


struct Settings {
	enum AudioTransmit { Continous, VAD, PushToTalk };
	enum VADSource { Amplitude, SignalToNoise };
	enum LoopMode { None, Local, Server };

	AudioTransmit atTransmit;
	unsigned __int64 uiDoublePush;

	bool bTransmitPosition;
	bool bMute, bDeaf;
	bool bTTS;
	int iTTSVolume, iTTSThreshold;
	int iQuality;
	int iNoiseSuppress;
	unsigned int iIdleTime;
	VADSource vsVAD;
	float fVADmin, fVADmax;
	int iFramesPerPacket;
	wstring qsAudioInput, qsAudioOutput;
	float fVolume; //输出音量大小控制
	float fMicVolume;//输入麦克风音量控制
	float fOtherVolume;
	bool bAttenuateOthersOnTalk;
	bool bAttenuateOthers;
	int iOutputDelay;

	int iPortAudioInput, iPortAudioOutput;
	wstring qsASIOclass;
	std::list<void*> qlASIOmic;
	std::list<void*> qlASIOspeaker;
	wstring qsWASAPIInput, qsWASAPIOutput;
	QByteArray qbaDXInput, qbaDXOutput;

	bool bPositionalAudio;
	bool bPositionalHeadphone;
	float fAudioMinDistance, fAudioMaxDistance, fAudioMaxDistVolume, fAudioBloom;

	bool bFirstTime;

	bool bUsage;

	wstring qsUsername;
	wstring qsLastServer;

	wstring strSysSelected;


	// Nonsaved
	LoopMode lmLoopMode;
	float dPacketLoss;
	float dMaxPacketDelay;
	
	volatile bool bResetEncode;
	unsigned int iPrediction; //设置预测方式 0：帧内预测  1：短期帧预测  2：长期帧预测
	unsigned int with_cbr;//设置固定码率
	unsigned int with_vbr;//设置动态码率
	int icomplex;//复杂度

	bool doEcho() const;
	bool doPositionalAudio() const;

	Settings();
	void load();
	void save();
};

#else
struct Settings;
#endif
