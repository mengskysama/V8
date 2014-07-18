#include "StdAfx.h"
#include "AudioOutput.h"
#include "AudioInput.h"
#include "Global.h"
#include "Message.h"
#include "PacketDataStream.h"
#include "ClientUser.h"
#include "MyAssert.h"
#include "AudioOutputSpeech.h"
#include "DirectSound.h"

// Remember that we cannot use static member classes that are not pointers, as the constructor
// for AudioOutputRegistrar() might be called before they are initialized, as the constructor
// is called from global initialization.
// Hence, we allocate upon first call.

std::map<wstring, AudioOutputRegistrar *> AudioOutputRegistrar::qmNew;
wstring AudioOutputRegistrar::current = wstring();

AudioOutputRegistrar::AudioOutputRegistrar(const wstring &n, int p) : name(n), priority(p) {
	qmNew[name] = this;
}

AudioOutputRegistrar::~AudioOutputRegistrar() {
	qmNew.erase(qmNew.find(name));
}

AudioOutputPtr AudioOutputRegistrar::newFromChoice(wstring choice) {
	if (qmNew.size()==0)
		return AudioOutputPtr();

	if (!choice.empty() && qmNew.find(choice) != qmNew.end()) {
		g_struct.s.qsAudioOutput = choice;
		current = choice;
		return AudioOutputPtr(qmNew.find(choice)->second->create());
	}
	choice = g_struct.s.qsAudioOutput;
	if (qmNew.find(choice) != qmNew.end()) {
		current = choice;
		return AudioOutputPtr(qmNew.find(choice)->second->create());
	}

	AudioOutputRegistrar *r = NULL;

	std::map<wstring, AudioOutputRegistrar *>::iterator mit;
	for ( mit = qmNew.begin(); mit != qmNew.end(); mit++)
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
	iFrameSize = SAMPLE_RATE / SAMPLE_DIV;
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
	MutexLocker l(&qrwlOutputs);  //qmOutputs 网络线程 主线程
	AudioOutputSpeech *aop = NULL;
	if (qmOutputs.find(user) != qmOutputs.end())
	{
		aop = dynamic_cast<AudioOutputSpeech *>(qmOutputs.find(user)->second);
	}else{
		if ((UInt32)user->uiSession==0)
		{
			aop = new AudioOutputSpeech(user, iMixerFreq, type);
			qmOutputs[user] = aop;
		}else{
			AssertVS(0,(UInt32)user->uiSession,"AudioOutput::addFrameToBuffer user not exits!");
			return;
		}
	}

	// 	if (! aop || (aop->umtType != type)) {
	// 		if (aop)
	// 			removeBuffer(aop);
	// 
	// 		while ((iMixerFreq == 0) && bRunning && !m_bStopRequested) 
	// 		{
	// 			Sleep(50);
	// 			Assert(0);
	// 		}
	// 
	// 		if (! iMixerFreq)
	// 			return;
	// 
	// 		aop = new AudioOutputSpeech(user, iMixerFreq, type);
	// 		qmOutputs[user] = aop;
	// 	}
	aop->addFrameToBuffer(qbaPacket, iSeq);
}

void AudioOutput::addUser(ClientUser *p)
{
	MutexLocker l(&qrwlOutputs); //qmOutputs 网络线程 主线程
	AudioOutputSpeech *aop = NULL;
	if (qmOutputs.find(p) != qmOutputs.end())
	{
		aop = dynamic_cast<AudioOutputSpeech *>(qmOutputs.find(p)->second);
	}else{
		aop = new AudioOutputSpeech(p, iMixerFreq, MessageHandler::UDPVoiceAACPlus/*MessageHandler::UDPVoiceCELT*/);
		qmOutputs[p] = aop;
	}
}

void AudioOutput::removeBuffer(const ClientUser *user) {
	MutexLocker l(&qrwlOutputs); //qmOutputs 网络线程 主线程
	if (qmOutputs.find(user) != qmOutputs.end())
	{
		removeBuffer(qmOutputs.find(user)->second);
	}
}

void AudioOutput::removeBuffer(AudioOutputUser *aop) {
	MutexLocker l(&qrwlOutputs); //qmOutputs 网络线程 主线程
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

#define VOLUMEMAX 0x7FFF
#define VOLUMEMIN (0 - 0x7FFF)
void muti_pair_mixer(const short* _psrc1, short* _psrc2, short* _pdst, int _size, float fvol)
{
	double f = 1.0;
	short *pdst = _pdst;

	__int64 dst = 0;
	double value = 0;
	for(register int i = 0; i < _size; i++)
	{
		dst = 2 * (*_psrc1*fvol + *_psrc2) / 4;
		value = f * dst;

		if(abs(value) >= VOLUMEMAX)
		{
			f = VOLUMEMAX  / (double)dst;
			if(value > 0)
				*_pdst = (short)VOLUMEMAX;
			else if(value < 0)
				*_pdst = (short)VOLUMEMIN;
		}
		else
		{
			f = (1.0  - f) / 32 + f;
			*_pdst = (short)value;
		}

		_psrc1 ++;
		_psrc2 ++;
		_pdst ++;
	} 
}

float align_right_mixer_ex(std::vector<AudioOutputUser *>& vecAll,short* dst,int size,float f)
{
	float fm = f;
	short* pdest = dst;
	static __int64 mdst[3840];
	static unsigned int maxboundary = 0;
	std::vector<AudioOutputUser *>::iterator iter;
	for (unsigned int iloop = 0;iloop<size;iloop++)
	{
		mdst[iloop] = 0;
		for (iter=vecAll.begin();iter!=vecAll.end();iter++)
		{
			const short * psBuffer = (short*)((*iter)->getBuffer());
			const float fo = g_struct.GetRoomSpeakerVolume((*iter)->uiSession>>32);
			mdst[iloop]+=(int)psBuffer[iloop]*fo;
		}
		if (_abs64(mdst[iloop])>maxboundary)
		{
			maxboundary = _abs64(mdst[iloop]);
		}
	}

	fm = (1.0  - fm) / 32 + fm;

	if (maxboundary*fm>VOLUMEMAX)
	{
		fm = 1.0f*VOLUMEMAX / maxboundary;
	}

	for (unsigned int jloop=0;jloop<size;jloop++)
	{
		__int64 value = mdst[jloop]*fm;
		if (value>VOLUMEMAX)
		{
			*pdest = (short)VOLUMEMAX;
		}
		else if (value<VOLUMEMIN)
		{
			*pdest = (short)VOLUMEMIN;
		}
		else
		{
			*pdest = (short)value;
		}
		pdest++;
	}
	return fm;
}

float align_right_mixer(std::vector<AudioOutputUser *>& _src, short* _pdst, int _size, float fvalue)
{
	float fold = fvalue;
	float fnew = 1.0f;
	float fmid = 1.0f;
	short* psSource = NULL;
	short* psDest = _pdst;
	int max = 0;
	int suma = 0;
	int sumd = 0;

	static int dst_arr[3840];
	static int dst_abs_arr[3840];
	static float dst_rig[100]; //最多100路混音
		
	//计算每个房间音量大小
	for (register int i = 0; i< _src.size();i++)
	{
		dst_rig[i] = g_struct.GetRoomSpeakerVolume(_src[i]->uiSession>>32);
	}

	//计算波形最大值
	for(register int i = 0; i < _size; i++)
	{
		suma = 0;
		sumd = 0;
		for(int j = 0; j < _src.size(); ++j)
		{
			psSource = (short *)(_src[j]->getBuffer());
			psSource[i]>0?(suma+=psSource[i]*dst_rig[j]):(sumd+=psSource[i]*dst_rig[j]);
		}

		//掩盖效应
		if (sumd < 0 && suma>0 && abs(suma+sumd)<5)
		{
			if (abs(sumd)>suma)
			{
				if (suma<VOLUMEMAX/4)
				{
					dst_arr[i] = sumd;
				}
				else if (suma<VOLUMEMAX*3/4)
				{
					dst_arr[i] = sumd+suma*3/4;
				}
				else
				{
					dst_arr[i] = sumd+suma/2;
				}
			}
			else
			{
				if (sumd<VOLUMEMAX/4)
				{
					dst_arr[i] = suma;
				}
				else if (suma<VOLUMEMAX*3/4)
				{
					dst_arr[i] = suma+sumd*3/4;
				}
				else
				{
					dst_arr[i] = suma+sumd/2;
				}
			}
		}
		else
		{
			dst_arr[i] = sumd+suma;
		}
		
		dst_abs_arr[i] = abs(dst_arr[i]);
		if(dst_abs_arr[i] > max)
			max = dst_abs_arr[i];
	}

	//计算递减因子
	if(max > VOLUMEMAX)
	{
		fnew = 1.0 * VOLUMEMAX / (max*0.85);
	}

	//动态调整
	if (fnew>fold)
	{
		fnew = (1.0  - fold) / 16 + fold;
	}

	//扭曲因子
	fmid = (1.0  - fnew) / 8 + fnew;
	for(register int i = 0; i < _size; i++)
	{
		if (dst_abs_arr[i]<VOLUMEMAX/4)
		{
			*_pdst = (short)dst_arr[i];
			_pdst ++;
			continue;
		}

		if (dst_abs_arr[i]<VOLUMEMAX*3/4)
		{
			*_pdst = (short)(dst_arr[i]*fmid);
			_pdst ++;
			continue;
		}

		suma = fnew * dst_arr[i];
		if (suma>=VOLUMEMAX)
		{
			*_pdst = (short)(VOLUMEMAX);
		}
		else if (suma<=VOLUMEMIN)
		{
			*_pdst = (short)(VOLUMEMIN);
		}
		else
		{
			*_pdst = (short)suma;
		}
		_pdst ++;
	}
	return fnew;
}

bool AudioOutput::mix(void *outbuff, unsigned int nsamp) {

	AudioOutputUser *aop;
	std::vector<AudioOutputUser *> qlMix;
	//std::vector<AudioOutputUser *> qlDel;

	if (g_struct.s.fVolume < 0.01)
		return false;

	const float mul = g_struct.s.fVolume * VOLUMEMIXGAIN;
	const unsigned int nchan = iChannels;

	//qrwlOutputs.lockForRead();
	MutexLocker l(&qrwlOutputs); //qmOutputs 网络线程 主线程
	std::map<const ClientUser *, AudioOutputUser *>::iterator mit;
	for (mit = qmOutputs.begin(); mit != qmOutputs.end(); mit ++)
	{
		SwitchToThread();
		if (! mit->second->needSamples(nsamp)) {
			//qlDel.push_back(mit->second);
		} else {
			qlMix.push_back(mit->second);
			
			//添加声音波形
			ClientUser *p = const_cast<ClientUser *>(mit->first);
			UInt64 lui = p->uiSession;
			mit->second->uiSession = lui;
			g_struct.PrepareWavDatas(lui,(byte*)(mit->second->getBuffer()),nsamp*sizeof(short));
		}
	}	

	if (! qlMix.empty()) {

		std::vector<AudioOutputUser *>::iterator lit = qlMix.begin();
		//const short * RESTRICT pfBuffer = (short *)(*lit)->pfBuffer;
		short * poutbuff = reinterpret_cast<short *>(outbuff);
// 		float muO = g_struct.GetRoomSpeakerVolume((*lit)->uiSession>>32);
// 		
// 		for (unsigned int i=0;i<nsamp;i++)
// 		{
// 			poutbuff[i] = pfBuffer[i]*muO;
// 		}
// 
// 		STACKVAR(float, svol, iChannels);
// 		STACKVAR(float, fOutput, iChannels * nsamp);
// 
// 		float *output = (eSampleFormat == SampleFloat) ? reinterpret_cast<float *>(outbuff) : fOutput;
// 		bool validListener = false;
// 
// 		memset(output, 0, sizeof(float) * nsamp * iChannels);
// 
// 		for (unsigned int i=0;i<iChannels;++i)
// 			svol[i] = mul * fSpeakerVolume[i];
// 		lit = qlMix.begin();

// 		while (++lit != qlMix.end())
// 		{
// 			const short * RESTRICT psBuffer = (short*)((*lit)->pfBuffer);
// 			muO = g_struct.GetRoomSpeakerVolume((*lit)->uiSession>>32);
// 			muti_pair_mixer(psBuffer,poutbuff,poutbuff,nsamp,muO);
// 		}
		static float f = 1.0f;
		f = align_right_mixer(qlMix,poutbuff,nsamp,f);

		//std::list<AudioOutputUser *>::iterator lit;
// 		for (lit = qlDel.begin(); lit != qlDel.end(); lit++)
// 		{
// 			removeBuffer(*lit);
// 		}
	}



	return (! qlMix.empty());
}


