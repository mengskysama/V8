#ifndef _DIRECTSOUND_H
#define _DIRECTSOUND_H

#include "AudioInput.h"
#include "AudioOutput.h"
#include <dsound.h>
#include <ks.h>
#include <ksmedia.h>
#define DIRECTSOUND_VERSION 0x1000

class DXAudioOutput : public AudioOutput {
public:
	DXAudioOutput();
	~DXAudioOutput();
	void Entry();
	void setVolume(int iVolume);

public:
	LPDIRECTSOUNDBUFFER8 pDSBOutput;
	Mutex qmDuplexCreate;
};

class DXAudioInput : public AudioInput {
public:
	DXAudioInput();
	~DXAudioInput();
	void Entry();

public:
	LPDIRECTSOUNDCAPTUREBUFFER8 pDSCaptureBuffer;

	Mutex qmDuplexCreate;
	


};

#else
class DXAudioOutput;
class DXAudioInput;
#endif
