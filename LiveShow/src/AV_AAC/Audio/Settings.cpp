#include "stdafx.h"
#include "Settings.h"
//#include "Log.h"
#include "Global.h"
#include "AudioInput.h"
//#include "Cert.h"
#include "AvDefine.h"


Settings::Settings() {
	atTransmit = VAD;
	bTransmitPosition = false;
	bMute = bDeaf = false;
	bTTS = true;
	iTTSVolume = 75;
	iTTSThreshold = 250;
	iQuality = BIT_RATE;
	fVolume = 1.0f;
	fMicVolume = 1.0f;
	fOtherVolume = 0.5f;
	bAttenuateOthersOnTalk = false;
	bAttenuateOthers = true;
	iFramesPerPacket = PER_FRAME_OF_SAMEPLE;//每个网络包包含几个frame
	iNoiseSuppress = -30;
	iIdleTime = 0;
	vsVAD = SignalToNoise;
	fVADmin = 0.80f;
	fVADmax = 0.98f;

	uiDoublePush = 0;

	bFirstTime = true;
	bUsage = true;

	iOutputDelay = 5;


	iPortAudioInput = -1; // default device
	iPortAudioOutput = -1; // default device

	bPositionalAudio = false;
	bPositionalHeadphone = false;
	fAudioMinDistance = 1.0f;
	fAudioMaxDistance = 15.0f;
	fAudioMaxDistVolume = 0.80f;
	fAudioBloom = 0.5f;

	strSysSelected = L"DirectSound";

	lmLoopMode = None;

	dPacketLoss = 0;
	dMaxPacketDelay = 0.0f;

	bResetEncode = false;
	iPrediction = 0;//默认帧内预测
	with_cbr = 1;//设置固定码率
	with_vbr = 0;//设置动态码率
	icomplex = -1;//复杂度
}

bool Settings::doEcho() const {
	
	if (AudioInputRegistrar::qmNew.size()>0) {
		AudioInputRegistrar *air = AudioInputRegistrar::qmNew.find(qsAudioInput)->second;
		if (air) {
			if (air->canEcho(qsAudioOutput))
				return true;
		}
	}
	return false;
}

bool Settings::doPositionalAudio() const {
	return bPositionalAudio;
}


