#pragma once
#include "stdafx.h"
#include "Thread.h"
//#include "smallft.h"
#include "PacketDataStream.h"
#include "Mutex.h"
#include "Message.h"
#include "Audio.h"
#include "Global.h"
#include "dsutil.h"
#include "ICodec.h"
#include "IFactory.h"
#include "CycleBuffer.h"

#define ACC_ENCODE_INPUT_SIZE 8192

class AudioInput;
typedef boost::shared_ptr<AudioInput> AudioInputPtr;


class AudioInputRegistrar {
public:
	static std::map<wstring, AudioInputRegistrar *> qmNew;
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
	MessageHandler::UDPMessageType umtType;

	unsigned int iSampleRate;

	int	iFrameSize;

	CELTCodec* m_cCodec;
	CELTEncoder* m_ceEncoder;
	ICodec * m_pCodec;
	IFactory* m_pFactory;
	unsigned char m_pBufTemp[ACC_ENCODE_INPUT_SIZE*3];
	unsigned char m_pBufEncode[ACC_ENCODE_INPUT_SIZE];
	CycleBuffer<ACC_ENCODE_INPUT_SIZE*3> m_cache;

	//////////////////////////////////////////////////////////////////////////

	CELTDecoder* m_de_cdDecoder;	
	CWaveFile g_pWaveFile;

	//////////////////////////////////////////////////////////////////////////

	int iAudioQuality;

	short *psMic;					//采集到的音频数据
	
	void encodeAudioFrame();

	volatile bool bRunning;
	volatile bool bPreviousVoice;

	unsigned char cFrameCounter;

	std::vector<QByteArray> qlFrames;
	void flushCheck(const QByteArray &, bool terminator);

public:	

	int iLevel;

	AudioInput();
	~AudioInput();
	void Entry() = 0;

	virtual bool SetMicrPhoneVolume(DWORD dwVolume) = 0;
	virtual int  GetMicrPhoneVolume() = 0;

	virtual bool SetMicInputMute(bool bMute) = 0;
	virtual bool GetMicInputMute(bool& bMute) = 0;	
};
