#ifndef _DIRECTSOUND_H
#define _DIRECTSOUND_H

#include "AudioInput.h"
#include "AudioOutput.h"
#include <dsound.h>
#include <ks.h>
#include <ksmedia.h>
#include "VolController.h"
#define DIRECTSOUND_VERSION 0x1000

#define VOLUMEMIXGAIN 1.0f

class DXAudioOutput : public AudioOutput {
public:
	DXAudioOutput();
	~DXAudioOutput();
	void Entry();
	void setVolume(int iVolume);
	int  getVolume();

	bool setPlayerMute(bool bMute);
	bool getPlayerMute(bool& bMute);
public:
	LPDIRECTSOUNDBUFFER8 pDSBOutput;
	Mutex qmDuplexCreate;
private:
	CVolController m_volumectr;
};

class DXAudioInput : public AudioInput {
public:
	DXAudioInput();
	~DXAudioInput();
	void Entry();

public:
	LPDIRECTSOUNDCAPTUREBUFFER8 pDSCaptureBuffer;	

	bool SetMicrPhoneVolume(DWORD dwVolume);
	int  GetMicrPhoneVolume();

	bool SetMicInputMute(bool bMute);
	bool GetMicInputMute(bool& bMute);
};

#else
class DXAudioOutput;
class DXAudioInput;
#endif
