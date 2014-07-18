#ifndef _AUDIOOUTPUT_H
#define _AUDIOOUTPUT_H

// AudioOutput depends on User being valid. This means it's important
// to removeBuffer from here BEFORE MainWindow gets any UserLeft
// messages. Any decendant user should feel free to remove unused
// AudioOutputUser objects; it's better to recreate them than
// having them use resources while unused.

#ifndef SPEAKER_FRONT_LEFT
#define SPEAKER_FRONT_LEFT              0x1
#define SPEAKER_FRONT_RIGHT             0x2
#define SPEAKER_FRONT_CENTER            0x4
#define SPEAKER_LOW_FREQUENCY           0x8
#define SPEAKER_BACK_LEFT               0x10
#define SPEAKER_BACK_RIGHT              0x20
#define SPEAKER_FRONT_LEFT_OF_CENTER    0x40
#define SPEAKER_FRONT_RIGHT_OF_CENTER   0x80
#define SPEAKER_BACK_CENTER             0x100
#define SPEAKER_SIDE_LEFT               0x200
#define SPEAKER_SIDE_RIGHT              0x400
#define SPEAKER_TOP_CENTER              0x800
#define SPEAKER_TOP_FRONT_LEFT          0x1000
#define SPEAKER_TOP_FRONT_CENTER        0x2000
#define SPEAKER_TOP_FRONT_RIGHT         0x4000
#define SPEAKER_TOP_BACK_LEFT           0x8000
#define SPEAKER_TOP_BACK_CENTER         0x10000
#define SPEAKER_TOP_BACK_RIGHT          0x20000
#endif

#include "Audio.h"
#include "Settings.h"
#include "Message.h"
#include "Mutex.h"
#include "Thread.h"
#include "packetdatastream.h"
#include "AudioOutputUser.h"

class AudioOutput;
class ClientUser;

typedef boost::shared_ptr<AudioOutput> AudioOutputPtr;

class AudioOutputRegistrar {

public:
	static std::map<wstring, AudioOutputRegistrar *> qmNew;
	static wstring current;
	static AudioOutputPtr newFromChoice(wstring choice = wstring());

	const wstring name;
	int priority;

	AudioOutputRegistrar(const wstring &n, int priority = 0);
	virtual ~AudioOutputRegistrar();
	virtual AudioOutput *create() = 0;
	virtual const std::vector<audioDevice> getDeviceChoices() = 0;
	virtual void setDeviceChoice(const QByteArray &, Settings &) = 0;
};

class AudioOutput : public BaseThread {
private:

private:
	float *fSpeakers;
	float *fSpeakerVolume;
//	bool *bSpeakerPositional;
protected:
	enum { SampleShort, SampleFloat } eSampleFormat;
	volatile bool bRunning;
	unsigned int iFrameSize;
	volatile unsigned int iMixerFreq;
	unsigned int iChannels;
	unsigned int iSampleSize;
	Mutex qrwlOutputs; //qmOutputs 网络线程 主线程
	std::map<const ClientUser *, AudioOutputUser *> qmOutputs;

	virtual void removeBuffer(AudioOutputUser *);
	void initializeMixer(const unsigned int *chanmasks, bool forceheadphone = false);
	bool mix(void *output, unsigned int nsamp);
public:
	void wipe();

	AudioOutput();
	~AudioOutput();
	void addFrameToBuffer(ClientUser *, const QByteArray &, unsigned int iSeq, MessageHandler::UDPMessageType type);
	void removeBuffer(const ClientUser *);
	void addUser(ClientUser *);
	void Entry() = 0;
	static float calcGain(float dotproduct, float distance);
	virtual void setVolume(int iVolume) = 0;
	virtual int  getVolume() = 0;
	virtual bool setPlayerMute(bool bMute) = 0;
	virtual bool getPlayerMute(bool& bMute) = 0;
public:
	int iLevel;
};

#else
class AudioInput;
#endif
