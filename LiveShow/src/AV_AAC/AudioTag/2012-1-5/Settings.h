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

	bool bExpert;


	bool bTransmitPosition;
	bool bMute, bDeaf;
	bool bTTS;
	int iTTSVolume, iTTSThreshold;
	int iQuality, iMinLoudness, iVoiceHold, iJitterBufferSize;
	int iNoiseSuppress;
	unsigned int iIdleTime;
	VADSource vsVAD;
	float fVADmin, fVADmax;
	int iFramesPerPacket;
	wstring qsAudioInput, qsAudioOutput;
	float fVolume;
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

	bool bEcho;
	bool bEchoMulti;
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

	bool doEcho() const;
	bool doPositionalAudio() const;

	Settings();
	void load();
	void save();
};

#else
struct Settings;
#endif
