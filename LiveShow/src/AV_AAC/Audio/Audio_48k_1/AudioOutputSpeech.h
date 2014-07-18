#ifndef _AUDIOOUTPUTSPEECH_H
#define _AUDIOOUTPUTSPEECH_H

#include "Audio.h"
#include "Message.h"
#include "AudioOutputUser.h"

class AudioOutputSpeech : public AudioOutputUser {
	friend class AudioOutput;

protected:
	MessageHandler::UDPMessageType umtType;
	unsigned int iBufferOffset;
	unsigned int iBufferFilled;
	unsigned int iOutputSize;
	unsigned int iLastConsume;
	unsigned int iFrameSize;
	bool bLastAlive;
	bool bHasTerminator;

	float *fFadeIn;
	float *fFadeOut;

	SpeexResamplerState *srs;

	Mutex qmJitter;
	JitterBuffer *jbJitter;
	int iMissCount;

	CELTCodec *cCodec;
	CELTDecoder *cdDecoder;

	SpeexBits sbBits;
	void *dsSpeex;

	std::list<QByteArray> qlFrames;

	unsigned char ucFlags;
public:
	int iMissedFrames;
	ClientUser *p;

	virtual bool needSamples(unsigned int snum);

	void addFrameToBuffer(const QByteArray &, unsigned int iBaseSeq);
	AudioOutputSpeech(ClientUser *, unsigned int freq, MessageHandler::UDPMessageType type);
	~AudioOutputSpeech();
};

#endif