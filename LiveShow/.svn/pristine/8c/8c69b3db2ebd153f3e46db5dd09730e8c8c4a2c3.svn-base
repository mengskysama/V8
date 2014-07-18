#include "StdAfx.h"
#include "AudioOutput.h"
#include "AudioInput.h"
#include "Global.h"
#include "Message.h"
#include "PacketDataStream.h"
#include "ClientUser.h"
#include "MyAssert.h"
#include "AudioOutputSpeech.h"

// Remember that we cannot use static member classes that are not pointers, as the constructor
// for AudioOutputRegistrar() might be called before they are initialized, as the constructor
// is called from global initialization.
// Hence, we allocate upon first call.

std::map<wstring, AudioOutputRegistrar *> *AudioOutputRegistrar::qmNew;
wstring AudioOutputRegistrar::current = wstring();

AudioOutputRegistrar::AudioOutputRegistrar(const wstring &n, int p) : name(n), priority(p) {
	if (! qmNew)
		qmNew = new std::map<wstring, AudioOutputRegistrar *>();
	(*qmNew)[name] = this;
}

AudioOutputRegistrar::~AudioOutputRegistrar() {
	delete (*qmNew)[name];
	qmNew->erase(qmNew->find(name));
}

AudioOutputPtr AudioOutputRegistrar::newFromChoice(wstring choice) {
	if (! qmNew)
		return AudioOutputPtr();

	if (!choice.empty() && qmNew->find(choice) != qmNew->end()) {
		g_struct.s.qsAudioOutput = choice;
		current = choice;
		return AudioOutputPtr(qmNew->find(choice)->second->create());
	}
	choice = g_struct.s.qsAudioOutput;
	if (qmNew->find(choice) != qmNew->end()) {
		current = choice;
		return AudioOutputPtr(qmNew->find(choice)->second->create());
	}

	AudioOutputRegistrar *r = NULL;

	std::map<wstring, AudioOutputRegistrar *>::iterator mit;
	for ( mit = qmNew->begin(); mit != qmNew->end(); mit++)
	{
		if (!r || (mit->second->priority > r->priority))
			r = mit->second;
	}
	if (r) {
		current = r->name;
		g_struct.s.qsAudioOutput = current;
		return AudioOutputPtr(r->create());
	}
	return AudioOutputPtr();
}

AudioOutput::AudioOutput() {
	iFrameSize = SAMPLE_RATE / 50;
	bRunning = false;

	iChannels = 0;
	fSpeakers = NULL;
	fSpeakerVolume = NULL;
	//bSpeakerPositional = NULL;

	iMixerFreq = 0;
	eSampleFormat = SampleFloat;
	iSampleSize = 0;
}

AudioOutput::~AudioOutput() {
	bRunning = false;
	wipe();

	if (fSpeakers)
		delete [] fSpeakers;
	if (fSpeakerVolume)
		delete [] fSpeakerVolume;
	//if (bSpeakerPositional)
	//	delete [] bSpeakerPositional;
}

// Here's the theory.
// We support sound "bloom"ing. That is, if sound comes directly from the left, if it is sufficiently
// close, we'll hear it full intensity from the left side, and "bloom" intensity from the right side.

float AudioOutput::calcGain(float dotproduct, float distance) {

	float dotfactor = (dotproduct + 1.0f) / 2.0f;
	float att;


	// No distance attenuation
	if (g_struct.s.fAudioMaxDistVolume > 0.99f) {
		att = min(1.0f, dotfactor + g_struct.s.fAudioBloom);
	} else if (distance < g_struct.s.fAudioMinDistance) {
		float bloomfac = g_struct.s.fAudioBloom * (1.0f - distance/g_struct.s.fAudioMinDistance);

		att = min(1.0f, bloomfac + dotfactor);
	} else {
		float datt;

		if (distance >= g_struct.s.fAudioMaxDistance) {
			datt = g_struct.s.fAudioMaxDistVolume;
		} else {
			float mvol = g_struct.s.fAudioMaxDistVolume;
			if (mvol < 0.01f)
				mvol = 0.01f;

			float drel = (distance-g_struct.s.fAudioMinDistance) / (g_struct.s.fAudioMaxDistance - g_struct.s.fAudioMinDistance);
			datt = powf(10.0f, log10f(mvol) * drel);
		}

		att = datt * dotfactor;
	}
	return att;
}

void AudioOutput::wipe() {
	std::map<const ClientUser *, AudioOutputUser *>::iterator mit;
	for (mit = qmOutputs.begin(); mit != qmOutputs.end();)
	{
		delete (AudioOutputUser *)mit->second;
		mit = qmOutputs.erase(mit);
	}

}


void AudioOutput::addFrameToBuffer(ClientUser *user, const QByteArray &qbaPacket, unsigned int iSeq, MessageHandler::UDPMessageType type) {
	if (iChannels == 0)
		return;
	//qrwlOutputs.lockForRead();
	MutexLocker l(&qrwlOutputs);
	AudioOutputSpeech *aop = NULL;
	if (qmOutputs.find(user) != qmOutputs.end())
	{
		aop = dynamic_cast<AudioOutputSpeech *>(qmOutputs.find(user)->second);
	}

	if (! aop || (aop->umtType != type)) {
		if (aop)
			removeBuffer(aop);

		while ((iMixerFreq == 0) && bRunning && !m_bStopRequested) 
		{
			Sleep(50);
			assert(0);
		}

		if (! iMixerFreq)
			return;

		aop = new AudioOutputSpeech(user, iMixerFreq, type);
		qmOutputs[user] = aop;
	}
	aop->addFrameToBuffer(qbaPacket, iSeq);
}

void AudioOutput::removeBuffer(const ClientUser *user) {
	if (qmOutputs.find(user) != qmOutputs.end())
	{
		removeBuffer(qmOutputs.find(user)->second);
	}
}

void AudioOutput::removeBuffer(AudioOutputUser *aop) {
	MutexLocker l(&qrwlOutputs);
	std::map<const ClientUser *, AudioOutputUser *>::iterator i;
	for (i=qmOutputs.begin(); i != qmOutputs.end(); ++i) {
		if (i->second == aop) {
			qmOutputs.erase(i);
			delete aop;
			break;
		}
	}
}

void AudioOutput::initializeMixer(const unsigned int *chanmasks, bool forceheadphone) {
	if (fSpeakers)
		delete fSpeakers;
	/*if (bSpeakerPositional)
		delete bSpeakerPositional;*/
	if (fSpeakerVolume)
		delete fSpeakerVolume;

	fSpeakers = new float[iChannels * 3];
	//bSpeakerPositional = new bool[iChannels];
	fSpeakerVolume = new float[iChannels];

	memset(fSpeakers, 0, sizeof(float) * iChannels * 3);
	//memset(bSpeakerPositional, 0, sizeof(bool) * iChannels);

	for (unsigned int i=0;i<iChannels;++i)
		fSpeakerVolume[i] = 1.0f;

	/*if (g_struct.s.bPositionalAudio && (iChannels > 1)) {
		for (unsigned int i=0;i<iChannels;i++) {
			float *s = &fSpeakers[3*i];
			bSpeakerPositional[i] = true;

			switch (chanmasks[i]) {
				case SPEAKER_FRONT_LEFT:
					s[0] = -0.5f;
					s[2] = 1.0f;
					break;
				case SPEAKER_FRONT_RIGHT:
					s[0] = 0.5f;
					s[2] = 1.0f;
					break;
				case SPEAKER_FRONT_CENTER:
					s[2] = 1.0f;
					break;
				case SPEAKER_LOW_FREQUENCY:
					break;
				case SPEAKER_BACK_LEFT:
					s[0] = -0.5f;
					s[2] = -1.0f;
					break;
				case SPEAKER_BACK_RIGHT:
					s[0] = 0.5f;
					s[2] = -1.0f;
					break;
				case SPEAKER_FRONT_LEFT_OF_CENTER:
					s[0] = -0.25;
					s[2] = 1.0f;
					break;
				case SPEAKER_FRONT_RIGHT_OF_CENTER:
					s[0] = 0.25;
					s[2] = 1.0f;
					break;
				case SPEAKER_BACK_CENTER:
					s[2] = -1.0f;
					break;
				case SPEAKER_SIDE_LEFT:
					s[0] = -1.0f;
					break;
				case SPEAKER_SIDE_RIGHT:
					s[0] = 1.0f;
					break;
				case SPEAKER_TOP_CENTER:
					s[1] = 1.0f;
					s[2] = 1.0f;
					break;
				case SPEAKER_TOP_FRONT_LEFT:
					s[0] = -0.5f;
					s[1] = 1.0f;
					s[2] = 1.0f;
					break;
				case SPEAKER_TOP_FRONT_CENTER:
					s[1] = 1.0f;
					s[2] = 1.0f;
					break;
				case SPEAKER_TOP_FRONT_RIGHT:
					s[0] = 0.5f;
					s[1] = 1.0f;
					s[2] = 1.0f;
					break;
				case SPEAKER_TOP_BACK_LEFT:
					s[0] = -0.5f;
					s[1] = 1.0f;
					s[2] = -1.0f;
					break;
				case SPEAKER_TOP_BACK_CENTER:
					s[1] = 1.0f;
					s[2] = -1.0f;
					break;
				case SPEAKER_TOP_BACK_RIGHT:
					s[0] = 0.5f;
					s[1] = 1.0f;
					s[2] = -1.0f;
					break;
				default:
					bSpeakerPositional[i] = false;
					fSpeakerVolume[i] = 0.0f;
					Trace("AudioOutput: Unknown speaker %d: %08x", i, chanmasks[i]);
					break;
			}
			if (g_struct.s.bPositionalHeadphone || forceheadphone) {
				s[1] = 0.0f;
				s[2] = 0.0f;
				if (s[0] == 0.0f)
					fSpeakerVolume[i] = 0.0f;
			}
		}
		for (unsigned int i=0;i<iChannels;i++) {
			float d = sqrtf(fSpeakers[3*i+0]*fSpeakers[3*i+0] + fSpeakers[3*i+1]*fSpeakers[3*i+1] + fSpeakers[3*i+2]*fSpeakers[3*i+2]);
			if (d > 0.0f) {
				fSpeakers[3*i+0] /= d;
				fSpeakers[3*i+1] /= d;
				fSpeakers[3*i+2] /= d;
			}
		}
	}*/
	iSampleSize = static_cast<int>(iChannels * ((eSampleFormat == SampleFloat) ? sizeof(float) : sizeof(short)));
	Trace("AudioOutput: Initialized %d channel %d hz mixer", iChannels, iMixerFreq);
}

bool AudioOutput::mix(void *outbuff, unsigned int nsamp) {

	AudioOutputUser *aop;
	std::list<AudioOutputUser *> qlMix;
	std::list<AudioOutputUser *> qlDel;

	if (g_struct.s.fVolume < 0.01)
		return false;

	const float mul = g_struct.s.fVolume;
	const unsigned int nchan = iChannels;

	//qrwlOutputs.lockForRead();
	MutexLocker l(&qrwlOutputs);
	std::map<const ClientUser *, AudioOutputUser *>::iterator mit;
	for (mit = qmOutputs.begin(); mit != qmOutputs.end(); mit ++)
	{
		if (! mit->second->needSamples(nsamp)) {
			qlDel.push_back(mit->second);
		} else {
			qlMix.push_back(mit->second);
		}
	}	

	if (! qlMix.empty()) {

		std::list<AudioOutputUser *>::iterator lit = qlMix.begin();
		const short * RESTRICT pfBuffer = (short *)(*lit)->pfBuffer;
		short * poutbuff = reinterpret_cast<short *>(outbuff);

		for (unsigned int i=0;i<nsamp;i++)
		{
			poutbuff[i] = pfBuffer[i];
		}

	/*	STACKVAR(float, speaker, iChannels*3);
		STACKVAR(float, svol, iChannels);

		STACKVAR(float, fOutput, iChannels * nsamp);
		float *output = (eSampleFormat == SampleFloat) ? reinterpret_cast<float *>(outbuff) : fOutput;
		bool validListener = false;

		memset(output, 0, sizeof(float) * nsamp * iChannels);

		for (unsigned int i=0;i<iChannels;++i)
			svol[i] = mul * fSpeakerVolume[i];


		std::list<AudioOutputUser *>::iterator lit;
		for (lit = qlMix.begin(); lit != qlMix.end(); lit++)
		{
			const float * RESTRICT pfBuffer = (*lit)->pfBuffer;

			for (unsigned int s=0;s<nchan;++s) {
				const float str = svol[s];
				float * RESTRICT o = output + s;
				for (unsigned int i=0;i<nsamp;++i)
					o[i*nchan] += pfBuffer[i] * str;
			}
		}

		// Clip
		float sum = 0;
		if (eSampleFormat == SampleFloat)
			for (unsigned int i=0;i<nsamp*iChannels;i++)
			{
				output[i] = output[i] < -1.0f ? -1.0f : (output[i] > 1.0f ? 1.0f : output[i]);
				sum += output[i] * output[i];
			}
			
		else
			for (unsigned int i=0;i<nsamp*iChannels;i++)
			{
				reinterpret_cast<short *>(outbuff)[i] = static_cast<short>(32768.f * (output[i] < -1.0f ? -1.0f : (output[i] > 1.0f ? 1.0f : output[i])));
				sum += output[i] * output[i];
			}
		
		iLevel = sqrtf(sum / (nsamp*iChannels)) * 9 / 32768.f;
*/
	}

	std::list<AudioOutputUser *>::iterator lit;
	for (lit = qlDel.begin(); lit != qlDel.end(); lit++)
	{
		removeBuffer(*lit);
	}

	return (! qlMix.empty());
}
