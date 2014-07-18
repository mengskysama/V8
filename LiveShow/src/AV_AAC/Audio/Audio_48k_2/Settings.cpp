#include "stdafx.h"
#include "Settings.h"
//#include "Log.h"
#include "Global.h"
#include "AudioInput.h"
//#include "Cert.h"


Settings::Settings() {
	atTransmit = VAD;
	bTransmitPosition = false;
	bMute = bDeaf = false;
	bTTS = true;
	iTTSVolume = 75;
	iTTSThreshold = 250;
	iQuality = BIT_RATE;
	fVolume = 1.0f;
	fOtherVolume = 0.5f;
	bAttenuateOthersOnTalk = false;
	bAttenuateOthers = true;
	iMinLoudness = 1000;
	iVoiceHold = 50;
	iJitterBufferSize = 1;
	iFramesPerPacket = 2;
	iNoiseSuppress = -30;
	iIdleTime = 0;
	vsVAD = SignalToNoise;
	fVADmin = 0.80f;
	fVADmax = 0.98f;


	uiDoublePush = 0;
	bExpert = false;

	bFirstTime = true;
	bUsage = true;

	iOutputDelay = 5;


	bEcho = false;
	bEchoMulti = true;

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
}

bool Settings::doEcho() const {
	/*if (! bEcho)
		return false;*/

	if (AudioInputRegistrar::qmNew) {
		AudioInputRegistrar *air = AudioInputRegistrar::qmNew->find(qsAudioInput)->second;
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


