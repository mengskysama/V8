#pragma once
#include "stdafx.h"
#include "Thread.h"
//#include "smallft.h"
#include "PacketDataStream.h"
#include "Mutex.h"
#include "Message.h"
#include "Audio.h"
#include "Global.h"
class AudioInput;
typedef boost::shared_ptr<AudioInput> AudioInputPtr;


class AudioInputRegistrar {
public:
	static std::map<wstring, AudioInputRegistrar *> *qmNew;
	static wstring current;
	static AudioInputPtr newFromChoice(wstring choice = wstring());

	const wstring name;
	int priority;

	AudioInputRegistrar(const wstring &n, int priority = 0);
	virtual ~AudioInputRegistrar();
	virtual AudioInput *create() = 0;
	virtual const std::vector<audioDevice> getDeviceChoices() = 0;
	virtual void setDeviceChoice(const QByteArray&, Settings &) = 0;
	virtual bool canEcho(const wstring &outputsys) const = 0;
};

class AudioInput : public BaseThread
{
protected:
	typedef enum { CodecCELT, CodecSpeex } CodecFormat;
	typedef enum { SampleShort, SampleFloat } SampleFormat;
	typedef void (*inMixerFunc)(float * RESTRICT, const void * RESTRICT, unsigned int, unsigned int);
private:
	SpeexResamplerState *srsMic, *srsEcho;

	Mutex qmEcho;
	unsigned int iJitterSeq;
	int iMinBuffered;

	unsigned int iMicFilled, iEchoFilled;
	inMixerFunc imfMic, imfEcho;
	inMixerFunc chooseMixer(const unsigned int nchan, SampleFormat sf);
protected:
	MessageHandler::UDPMessageType umtType;
	SampleFormat eMicFormat, eEchoFormat;

	unsigned int iSampleRate;
	unsigned int iMicChannels, iEchoChannels;
	unsigned int iMicFreq, iEchoFreq;
	unsigned int iMicLength, iEchoLength;
	unsigned int iMicSampleSize, iEchoSampleSize;
	unsigned int iEchoMCLength, iEchoFrameSize;

	bool bEchoMulti;
	int	iFrameSize;

	Mutex qmSpeex;
	SpeexPreprocessState *sppPreprocess;
	SpeexEchoState *sesEcho;

	CELTCodec *cCodec;
	CELTEncoder *ceEncoder;

	SpeexBits sbBits;
	void *esSpeex;

	int iAudioQuality;
	int iAudioFrames;

	//drft_lookup fftTable;

	short *psMic;					//采集到的音频数据
	short *psSpeaker;
	short *psClean;

	float *pfMicInput;
	float *pfEchoInput;
	float *pfOutput;

	void encodeAudioFrame();


	volatile bool bRunning;
	volatile bool bPreviousVoice;

	int iFrameCounter;
	int iSilentFrames;
	int iHoldFrames;

	std::deque<short*> qlEchoFrames;
	std::vector<QByteArray> qlFrames;
	void flushCheck(const QByteArray &, bool terminator);

	void initializeMixer();

	static bool preferCELT(int bitrate, int frames);
public:
	void addMic(const void *data, unsigned int nsamp);
	void addEcho(const void *data, unsigned int nsamp);
	bool bResetProcessor;

	int iBitrate;
	float dPeakMic, dPeakSpeaker, dPeakSignal, dMaxMic;
	float fSpeechProb;

	int iLevel;

	AudioInput();
	~AudioInput();
	void Entry() = 0;
	bool isTransmitting() const;

	BOOL SetMicrPhoneVolume(DWORD dwVolume);

	bool bEcho;
};
