#ifndef _AUDIOOUTPUTSPEECH_H
#define _AUDIOOUTPUTSPEECH_H

#include "Audio.h"
#include "Message.h"
#include "AudioOutputUser.h"
#include "ICodec.h"
#include "IFactory.h"
#include "Cyclebuffer.h"

#define ACC_ENCODE_INPUT_SIZE 8192

#define CACHE_SAMPLE_BUFFER_SIZE 18 //缓存采样buffer大小
#define MIN_CACHE_SAMPLE_COUNT 5
#define MAX_CACHE_SAMPLE_COUNT (CACHE_SAMPLE_BUFFER_SIZE-MIN_CACHE_SAMPLE_COUNT)

class AudioOutputSpeech : public AudioOutputUser 
{
	friend class AudioOutput;

protected:

	MessageHandler::UDPMessageType umtType;

	CELTCodec* m_cCodec;
	CELTDecoder* m_cdDecoder;
	ICodec* m_pCodec;
	IFactory* m_pFactory;
	unsigned char m_pBufTemp[ACC_ENCODE_INPUT_SIZE];

	Mutex m_cacheMutex;
	volatile UINT m_putIndex;
	volatile UINT m_getIndex;
	short m_cacheSample[MAX_CACHE_SAMPLE_COUNT][AUDIO_CHANNEL*SAMPLE_RATE/SAMPLE_DIV];
	
	CycleBuffer<ACC_ENCODE_INPUT_SIZE*10> m_cache;
public:
	ClientUser* p;

	virtual bool needSamples(unsigned int snum);

	void addFrameToBuffer(const QByteArray &, unsigned int iBaseSeq);
	AudioOutputSpeech(ClientUser *, unsigned int freq, MessageHandler::UDPMessageType type);
	~AudioOutputSpeech();
};

#endif