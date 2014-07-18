#include "StdAfx.h"
#include "AudioInput.h"
#include "MyAssert.h"


std::map<wstring, AudioInputRegistrar *> *AudioInputRegistrar::qmNew;	//音频采集
wstring AudioInputRegistrar::current = wstring();

AudioInputRegistrar::AudioInputRegistrar(const wstring &n, int p) : name(n), priority(p) {
	if (! qmNew)
		qmNew = new std::map<wstring, AudioInputRegistrar *>();
	qmNew->insert(make_pair(name,this));
	(*qmNew)[name] = this;
}

AudioInputRegistrar::~AudioInputRegistrar() {
	if (qmNew->find(name) != qmNew->end())
	{
		qmNew->erase(qmNew->find(name));
	}
	if(qmNew->size() == 0)
	{
		delete qmNew;
		qmNew = NULL;
	}
}

//选择一个新的采集器 directsound or wasapi
AudioInputPtr AudioInputRegistrar::newFromChoice(wstring choice) {
	if (! qmNew)
		return AudioInputPtr();

	if (!choice.empty() && qmNew->find(choice) != qmNew->end()) {
		g_struct.s.qsAudioInput = choice;
		current = choice;
		AudioInputRegistrar * air = qmNew->find(current)->second;
		
		return AudioInputPtr(air->create());
	}
	choice = g_struct.s.qsAudioInput;
	if (qmNew->find(choice) != qmNew->end()) {
		current = choice;
		return AudioInputPtr(qmNew->find(choice)->second->create());
	}

	AudioInputRegistrar *r = NULL;
	std::map<wstring, AudioInputRegistrar *>::iterator mit;
	for (mit = qmNew->begin(); mit != qmNew->end(); mit++)
	{
		if (!r || (mit->second->priority > r->priority))
			r = mit->second;
	}
	if (r) {
		current = r->name;
		g_struct.s.qsAudioInput = current;
		return AudioInputPtr(r->create());
	}
	return AudioInputPtr();
}

//初始化编解码器
AudioInput::AudioInput() {
	bEcho = true;
	m_de_cdDecoder = NULL;
	iLevel = 0;
	//celt or speex
	iAudioFrames = g_struct.s.iFramesPerPacket;
	iAudioQuality = g_struct.s.iQuality;
	if (preferCELT(iAudioQuality, iAudioFrames))
		umtType = MessageHandler::UDPVoiceCELT;
	else
		umtType = MessageHandler::UDPVoiceSpeex;

	cCodec = NULL;
	ceEncoder = NULL;

	if (umtType == MessageHandler::UDPVoiceCELT) {
		iSampleRate = SAMPLE_RATE;
		iFrameSize = SAMPLE_RATE / 50;

		esSpeex = NULL;
		Trace("AudioInput: %d bits/s, %d hz, %d sample CELT", iAudioQuality, iSampleRate, iFrameSize);
	}
	else {
		assert(0);
	}
	iEchoFreq = iMicFreq = iSampleRate;

	//mumble_drft_init(&fftTable, iFrameSize);

	iFrameCounter = 0;
	iSilentFrames = 0;
	iHoldFrames = 0;

	bResetProcessor = true;

	bEchoMulti = false;

	sppPreprocess = NULL;
	sesEcho = NULL;
	srsMic = srsEcho = NULL;
	iJitterSeq = 0;
	iMinBuffered = 1000;

	psMic = new short[iFrameSize*2];//2个声道
	psClean = new short[iFrameSize*2];
	psSpeaker = NULL;

	iEchoChannels = iMicChannels = 0;
	iEchoFilled = iMicFilled = 0;
	eMicFormat = eEchoFormat = SampleFloat;
	iMicSampleSize = iEchoSampleSize = 0;

	bPreviousVoice = false;

	pfMicInput = pfEchoInput = pfOutput = NULL;

	iBitrate = 0;
	dPeakMic = dPeakSignal = dPeakSpeaker = 0.0;

	bRunning = false;

	WAVEFORMATEX  wfx;

	ZeroMemory(&wfx, sizeof(wfx));
	wfx.wFormatTag = WAVE_FORMAT_PCM;
	wfx.nChannels = 2;
	wfx.nSamplesPerSec = iSampleRate;
	wfx.nBlockAlign = 4;
	wfx.nAvgBytesPerSec = wfx.nSamplesPerSec * wfx.nBlockAlign;
	wfx.wBitsPerSample = 16;

	if (FAILED(g_pWaveFile.Open(L"c:\\mywave.wav", &wfx, WAVEFILE_WRITE)))
	{
		g_pWaveFile.Close();
	}
}

AudioInput::~AudioInput() {

	g_pWaveFile.Close();

	bRunning = false;

	if (ceEncoder) {
		cCodec->encoder_destroy(ceEncoder);
	} else if (esSpeex) {
		speex_bits_destroy(&sbBits);
		speex_encoder_destroy(esSpeex);
	}

//	mumble_drft_clear(&fftTable);

	std::deque<short*>::iterator dit;
	for (dit = qlEchoFrames.begin(); dit != qlEchoFrames.end();)
	{
		delete [] *dit;
		dit = qlEchoFrames.erase(dit);
	}

	if (sppPreprocess)
		speex_preprocess_state_destroy(sppPreprocess);
	if (sesEcho)
		speex_echo_state_destroy(sesEcho);

	if (srsMic)
		speex_resampler_destroy(srsMic);
	if (srsEcho)
		speex_resampler_destroy(srsEcho);

	delete [] psMic;
	delete [] psClean;
	if (psSpeaker)
		delete [] psSpeaker;

	if (pfMicInput)
		delete [] pfMicInput;
	if (pfEchoInput)
		delete [] pfEchoInput;
	if (pfOutput)
		delete [] pfOutput;
}

bool AudioInput::isTransmitting() const {
	return bPreviousVoice;
};

#define IN_MIXER_FLOAT(channels) \
	static void inMixerFloat##channels ( float * RESTRICT buffer, const void * RESTRICT ipt, unsigned int nsamp, unsigned int N) { \
	const float * RESTRICT input = reinterpret_cast<const float *>(ipt); \
	register const float m = 1.0f / static_cast<float>(channels); \
	for(unsigned int i=0;i<nsamp;++i) {\
	register float v= 0.0f; \
	for(unsigned int j=0;j<channels;++j) \
	v += input[i*channels+j]; \
	buffer[i] = v * m; \
	} \
}

#define IN_MIXER_SHORT(channels) \
	static void inMixerShort##channels ( float * RESTRICT buffer, const void * RESTRICT ipt, unsigned int nsamp, unsigned int N) { \
	const short * RESTRICT input = reinterpret_cast<const short *>(ipt); \
	register const float m = 1.0f / (32768.f * static_cast<float>(channels)); \
	for(unsigned int i=0;i<nsamp;++i) {\
	register float v= 0.0f; \
	for(unsigned int j=0;j<channels;++j) \
	v += static_cast<float>(input[i*channels+j]); \
	buffer[i] = v * m; \
	} \
}

IN_MIXER_FLOAT(1)
IN_MIXER_FLOAT(2)
IN_MIXER_FLOAT(3)
IN_MIXER_FLOAT(4)
IN_MIXER_FLOAT(5)
IN_MIXER_FLOAT(6)
IN_MIXER_FLOAT(7)
IN_MIXER_FLOAT(8)
IN_MIXER_FLOAT(N)

IN_MIXER_SHORT(1)
IN_MIXER_SHORT(2)
IN_MIXER_SHORT(3)
IN_MIXER_SHORT(4)
IN_MIXER_SHORT(5)
IN_MIXER_SHORT(6)
IN_MIXER_SHORT(7)
IN_MIXER_SHORT(8)
IN_MIXER_SHORT(N)

AudioInput::inMixerFunc AudioInput::chooseMixer(const unsigned int nchan, SampleFormat sf) {
	inMixerFunc r = NULL;
	if (sf == SampleFloat) {
		switch (nchan) {
			case 1:
				r = inMixerFloat1;
				break;
			case 2:
				r = inMixerFloat2;
				break;
			case 3:
				r = inMixerFloat3;
				break;
			case 4:
				r = inMixerFloat4;
				break;
			case 5:
				r = inMixerFloat5;
				break;
			case 6:
				r = inMixerFloat6;
				break;
			case 7:
				r = inMixerFloat7;
				break;
			case 8:
				r = inMixerFloat8;
				break;
			default:
				r = inMixerFloatN;
				break;
		}
	} else {
		switch (nchan) {
			case 1:
				r = inMixerShort1;
				break;
			case 2:
				r = inMixerShort2;
				break;
			case 3:
				r = inMixerShort3;
				break;
			case 4:
				r = inMixerShort4;
				break;
			case 5:
				r = inMixerShort5;
				break;
			case 6:
				r = inMixerShort6;
				break;
			case 7:
				r = inMixerShort7;
				break;
			case 8:
				r = inMixerShort8;
				break;
			default:
				r = inMixerShortN;
				break;
		}
	}
	return r;
}

void AudioInput::initializeMixer() {
	int err;

	if (srsMic)
		speex_resampler_destroy(srsMic);
	if (srsEcho)
		speex_resampler_destroy(srsEcho);
	if (pfMicInput)
		delete [] pfMicInput;
	if (pfEchoInput)
		delete [] pfEchoInput;
	if (pfOutput)
		delete [] pfOutput;

	if (iMicFreq != iSampleRate)
		srsMic = speex_resampler_init(1, iMicFreq, iSampleRate, 3, &err);

	iMicLength = (iFrameSize * iMicFreq) / iSampleRate;

	pfMicInput = new float[iMicLength];
	pfOutput = new float[iFrameSize * max(1U,iEchoChannels)];

	if (iEchoChannels > 0) {
		bEchoMulti = g_struct.s.bEchoMulti;
		if (iEchoFreq != iSampleRate)
			srsEcho = speex_resampler_init(bEchoMulti ? iEchoChannels : 1, iEchoFreq, iSampleRate, 3, &err);
		iEchoLength = (iFrameSize * iEchoFreq) / iSampleRate;
		iEchoMCLength = bEchoMulti ? iEchoLength * iEchoChannels : iEchoLength;
		iEchoFrameSize = bEchoMulti ? iFrameSize * iEchoChannels : iFrameSize;
		pfEchoInput = new float[iEchoMCLength];
	} else {
		srsEcho = NULL;
		pfEchoInput = NULL;
	}

	imfMic = chooseMixer(iMicChannels, eMicFormat);
	imfEcho = chooseMixer(iEchoChannels, eEchoFormat);

	iMicSampleSize = static_cast<int>(iMicChannels * ((eMicFormat == SampleFloat) ? sizeof(float) : sizeof(short)));
	iEchoSampleSize = static_cast<int>(iEchoChannels * ((eEchoFormat == SampleFloat) ? sizeof(float) : sizeof(short)));

	bResetProcessor = true;

	Trace("AudioInput: Initialized mixer for %d channel %d hz mic and %d channel %d hz echo", iMicChannels, iMicFreq, iEchoChannels, iEchoFreq);
}

void AudioInput::addMic(const void *data, unsigned int nsamp) {
	while (nsamp > 0) {
		unsigned int left = min(nsamp, iMicLength - iMicFilled);

		imfMic(pfMicInput + iMicFilled, data, left, iMicChannels);

		iMicFilled += left;
		nsamp -= left;

		if (nsamp > 0) {
			if (eMicFormat == SampleFloat)
				data = reinterpret_cast<const float *>(data) + left * iMicChannels;
			else
				data = reinterpret_cast<const short *>(data) + left * iMicChannels;
		}

		if (iMicFilled == iMicLength) {
			iMicFilled = 0;

			float *ptr = srsMic ? pfOutput : pfMicInput;
			if (srsMic) {
				spx_uint32_t inlen = iMicLength;
				spx_uint32_t outlen = iFrameSize;
				speex_resampler_process_float(srsMic, 0, pfMicInput, &inlen, pfOutput, &outlen);
			}
			const float mul = 32768.f;
			for (int j=0;j<iFrameSize;++j)
				psMic[j] = static_cast<short>(ptr[j] * mul);


			if (iEchoChannels > 0) {
				short *echo = NULL;

				//获取回音数据
				{
					MutexLocker l(&qmEcho);

					if (qlEchoFrames.empty()) {
						iJitterSeq = 0;
						iMinBuffered = 1000;
					} else {
						iMinBuffered = min(iMinBuffered, qlEchoFrames.size());

						if ((iJitterSeq > 100) && (iMinBuffered > 1)) {
							iJitterSeq = 0;
							iMinBuffered = 1000;
							delete [] qlEchoFrames.front();
							qlEchoFrames.pop_front();
						}
						echo = qlEchoFrames.front();
						qlEchoFrames.pop_front();
					}
				}

				if (echo) {
					if (psSpeaker)
						delete [] psSpeaker;
					psSpeaker = echo;
				}
			}
			encodeAudioFrame();
		}
	}
}

//存储回音数据
void AudioInput::addEcho(const void *data, unsigned int nsamp) {
	while (nsamp > 0) {
		unsigned int left = min(nsamp, iEchoLength - iEchoFilled);

		if (bEchoMulti) {
			const unsigned int samples = left * iEchoChannels;
			if (eEchoFormat == SampleFloat)
				for (unsigned int i=0;i<samples;++i)
					pfEchoInput[i] = reinterpret_cast<const float *>(data)[i];
			else
				for (unsigned int i=0;i<samples;++i)
					pfEchoInput[i] = static_cast<float>(reinterpret_cast<const short *>(data)[i]) * (1.0f / 32768.f);
		} else {
			imfEcho(pfEchoInput + iEchoFilled, data, left, iEchoChannels);
		}

		iEchoFilled += left;
		nsamp -= left;

		if (nsamp > 0) {
			if (eEchoFormat == SampleFloat)
				data = reinterpret_cast<const float *>(data) + left * iEchoChannels;
			else
				data = reinterpret_cast<const short *>(data) + left * iEchoChannels;
		}

		if (iEchoFilled == iEchoLength) {
			iEchoFilled = 0;

			float *ptr = srsEcho ? pfOutput : pfEchoInput;
			if (srsEcho) {
				spx_uint32_t inlen = iEchoLength;
				spx_uint32_t outlen = iFrameSize;
				speex_resampler_process_interleaved_float(srsEcho, pfEchoInput, &inlen, pfOutput, &outlen);
			}

			short *outbuff = new short[iEchoFrameSize];

			const float mul = 32768.f;
			for (unsigned int j=0;j<iEchoFrameSize;++j)
				outbuff[j] = static_cast<short>(ptr[j] * mul);

			iJitterSeq = min(iJitterSeq+1,10000U);

			MutexLocker l(&qmEcho);
			qlEchoFrames.push_back(outbuff);
		}
	}
}

bool AudioInput::preferCELT(int bitrate, int frames) {
	return ((bitrate >= 32000) || (frames == 1));
}

void AudioInput::encodeAudioFrame() {
	int iArg;
	//ClientUser *p=ClientUser::get(g.uiSession);
	int i;
	float sum;
	short max;

	short *psSource;

	iFrameCounter++;

	if (! bRunning) {
		return;
	}

	MutexLocker l(&qmSpeex);

	bResetProcessor = false;

	if (bResetProcessor) {
		if (sppPreprocess)
			speex_preprocess_state_destroy(sppPreprocess);
		if (sesEcho)
			speex_echo_state_destroy(sesEcho);

		sppPreprocess = speex_preprocess_state_init(iFrameSize, iSampleRate);

		iArg = 1;
		speex_preprocess_ctl(sppPreprocess, SPEEX_PREPROCESS_SET_VAD, &iArg);
		//speex_preprocess_ctl(sppPreprocess, SPEEX_PREPROCESS_SET_AGC, &iArg);
		speex_preprocess_ctl(sppPreprocess, SPEEX_PREPROCESS_SET_DENOISE, &iArg);
		speex_preprocess_ctl(sppPreprocess, SPEEX_PREPROCESS_SET_DEREVERB, &iArg);

		iArg = 30000;
		speex_preprocess_ctl(sppPreprocess, SPEEX_PREPROCESS_SET_AGC_TARGET, &iArg);

		float v = 30000.0f / static_cast<float>(g_struct.s.iMinLoudness);
		iArg = (floorf(20.0f * log10f(v)));
		speex_preprocess_ctl(sppPreprocess, SPEEX_PREPROCESS_SET_AGC_MAX_GAIN, &iArg);

		iArg = g_struct.s.iNoiseSuppress;
		speex_preprocess_ctl(sppPreprocess, SPEEX_PREPROCESS_SET_NOISE_SUPPRESS, &iArg);

		if (iEchoChannels > 0) {
			sesEcho = speex_echo_state_init_mc(iFrameSize, iFrameSize*10, 1, bEchoMulti ? iEchoChannels : 1);
			iArg = iSampleRate;
			speex_echo_ctl(sesEcho, SPEEX_ECHO_SET_SAMPLING_RATE, &iArg);
			speex_preprocess_ctl(sppPreprocess, SPEEX_PREPROCESS_SET_ECHO_STATE, sesEcho);

			Trace("AudioInput: ECHO CANCELLER ACTIVE");
		} else {
			sesEcho = NULL;
		}

		bResetProcessor = false;
	}

	int iIsSpeech=1;
	psSource = psMic;
/*
	//回音消除和音质处理
	if (bEcho && sesEcho && psSpeaker)
	{
		speex_echo_cancellation(sesEcho, psMic, psSpeaker, psClean);
		iIsSpeech=speex_preprocess_run(sppPreprocess, psClean);
		psSource = psClean;
	} 
	else {
		iIsSpeech=speex_preprocess_run(sppPreprocess, psMic);
		psSource = psMic;
	}*/

	/*sum=1.0f;
	for (i=0;i<iFrameSize;i++)
		sum += static_cast<float>(psSource[i] * psSource[i]);
	float micLevel = sqrtf(sum / static_cast<float>(iFrameSize));
	dPeakSignal=20.0f*log10f(micLevel / 32768.0f);
	if (dPeakSignal < -96.0f)
		dPeakSignal = -96.0f;

	spx_int32_t prob = 0;
	speex_preprocess_ctl(sppPreprocess, SPEEX_PREPROCESS_GET_PROB, &prob);
	fSpeechProb = static_cast<float>(prob) / 100.0f;

	float level = (g_struct.s.vsVAD == Settings::SignalToNoise) ? fSpeechProb : (1.0f + dPeakMic / 96.0f);

	if (level > g_struct.s.fVADmax)
		iIsSpeech = 1;
	else if (level > g_struct.s.fVADmin && bPreviousVoice)
		iIsSpeech = 1;
	else
		iIsSpeech = 0;

	if (! iIsSpeech) {
		iHoldFrames++;
		if (iHoldFrames < g_struct.s.iVoiceHold)
			iIsSpeech=1;
	} else {
		iHoldFrames = 0;
	}*/

	//tIdle.restart();
	/*
	int r = celt_encoder_ctl(ceEncoder, CELT_SET_POST_MDCT_CALLBACK(celtBack, NULL));
	qWarning() << "Set Callback" << r;
	*/

	//编码 speex或者CELT
	unsigned char buffer[512];
	int len;

	if (umtType == MessageHandler::UDPVoiceCELT) {
		if (cCodec == NULL)
		{
			cCodec = CELTCodec::instance();
			ceEncoder = cCodec->encoderCreate();
		}
		else if (cCodec && ! bPreviousVoice) {
			cCodec->encoder_ctl(ceEncoder, CELT_RESET_STATE);
		}

		cCodec->encoder_ctl(ceEncoder, CELT_SET_PREDICTION(0));

		cCodec->encoder_ctl(ceEncoder,CELT_SET_BITRATE(iAudioQuality));
		len = cCodec->encode(ceEncoder, psSource, SAMPLE_RATE / 50, buffer, 512);
		iBitrate = len * 50 * 8;
		
		/*////////////////////////////////////////////////////////////////////////

		if (m_de_cdDecoder == NULL) {
			m_de_cdDecoder = cCodec->decoderCreate();
		}
		
		celt_int16 fout2[2560]={0};

		if (cCodec)
		{
			int len3 = cCodec->decode(m_de_cdDecoder, buffer, len, fout2, SAMPLE_RATE / 50);
			len3++;

			UINT dwDataWrote;
			if( FAILED(g_pWaveFile.Write( SAMPLE_RATE / 50*2*2, (BYTE*)fout2, 
				&dwDataWrote ) ))
			{
				int a=0;
				a++;
			}
			else
			{
				OutputDebugString(L"plushuwav g_pWaveFile.Write 3");				
			}
		}

		///////////////////////////////////////////////////////////////////////*/
	} 
	else {
		assert(0);
	}

	QByteArray qba;
	for(int i=0; i<len; i++)
	{
		qba.push_back(buffer[i]);
	}

	flushCheck(qba, false);

	if (! iIsSpeech)
		iBitrate = 0;

	bPreviousVoice = iIsSpeech;
}

void AudioInput::flushCheck(const QByteArray &frame, bool terminator) {
	qlFrames.push_back(frame);
	if (! terminator && qlFrames.size() < iAudioFrames)
		return;

	int flags = g_struct.iTarget;
	if (terminator)
		flags = g_struct.iPrevTarget;

	flags |= (umtType << 5);

	char data[1024];
	data[0] = static_cast<unsigned char>(flags);

	PacketDataStream pds(data + 1, 1023);
	pds << iFrameCounter - qlFrames.size();

	if (terminator)
		qlFrames.push_back(QByteArray()); 

	for (int i=0;i<qlFrames.size(); ++i) {
		const QByteArray &qba = qlFrames.at(i);
		unsigned char head = static_cast<unsigned char>(qba.size());		
		pds.append(head);
		pds.append((char*)(&qba[0]), qba.size());
	}

	//test
	//g_struct.s.lmLoopMode = Settings::Local;

	if (g_struct.s.lmLoopMode == Settings::Local)
	{
		//如果是本地预览声音，则添加到本地预览声音缓存
		QByteArray qba;
		for(int i=0; i<pds.size()+1; i++)
		{
			qba.push_back(data[i]);
		}
		LoopUser::lpLoopy.addFrame(qba);
	}
		
	else if (g_struct.trans)
	{
		//获得到数据，准备发送                123+1
		g_struct.trans->SendAudioData(data,pds.size() + 1); 
	}

	qlFrames.clear();
}

BOOL AudioInput::SetMicrPhoneVolume(DWORD dwVolume)
{
	BOOL bResult = FALSE;
	try
	{
		int mixerNum = (int)mixerGetNumDevs();
		for(int i = 0; i < mixerNum; i ++)
		{
			DWORD dwSource = 0;
			HMIXER m_hMixer = NULL;   
			MMRESULT mr = NULL;
			//打开Mixer设备   
			if(mixerOpen(&m_hMixer, i, 0, 0, MIXER_OBJECTF_MIXER) != MMSYSERR_NOERROR)
				continue; 

			MIXERLINE MixerLine;   
			//结构体的大小   
			MixerLine.cbStruct = sizeof(MixerLine);   
			//录制设备总线   
			MixerLine.dwComponentType   =   MIXERLINE_COMPONENTTYPE_DST_WAVEIN;   
			//得到录制总线中的连接数   
			if(mixerGetLineInfo((HMIXEROBJ)m_hMixer, &MixerLine,
				MIXER_OBJECTF_HMIXER | MIXER_GETLINEINFOF_COMPONENTTYPE) != MMSYSERR_NOERROR)
				continue; 

			//将连接数保存   
			DWORD dwConnections = MixerLine.cConnections;   
			//准备获取麦克风设备的ID   
			DWORD dwLineID = 0;   
			for(DWORD i = 0; i < dwConnections; i++ )   
			{   
				//枚举每一个设备，当Source的ID等于当前的迭代记数   
				MixerLine.dwSource = i;   
				//根据SourceID获得连接的信息   
				if(mixerGetLineInfo((HMIXEROBJ)m_hMixer, &MixerLine,   
					MIXER_OBJECTF_HMIXER | MIXER_GETLINEINFOF_SOURCE) != MMSYSERR_NOERROR) 
					//判断函数执行错误   
					break;  

				//如果当前设备类型是麦克风，则跳出循环。   
				if(MixerLine.dwComponentType == MIXERLINE_COMPONENTTYPE_SRC_MICROPHONE)   
				{   
					dwLineID = MixerLine.dwLineID;
					dwSource = MixerLine.dwSource;
					break;   
				}   
			}

			//如果没有找到，返回失败。   
			if(dwLineID == 0)   
				continue;   

			//get line id of destination line   
			MIXERLINE lineinfo_Dest;   
			::ZeroMemory(&lineinfo_Dest, sizeof(MIXERLINE));   
			lineinfo_Dest.cbStruct = sizeof(MIXERLINE);   
			lineinfo_Dest.dwComponentType = MIXERLINE_COMPONENTTYPE_DST_WAVEIN;   
			if(::mixerGetLineInfo(   
				(HMIXEROBJ)m_hMixer,   
				&lineinfo_Dest,   
				MIXER_OBJECTF_HMIXER   |     
				MIXER_GETLINEINFOF_COMPONENTTYPE) != MMSYSERR_NOERROR)
				continue;  

			//get id of specified connector   
			MIXERLINE lineinfo_Connector;   
			::ZeroMemory(&lineinfo_Connector, sizeof(MIXERLINE));  

			lineinfo_Connector.cbStruct = sizeof(MIXERLINE);   
			lineinfo_Connector.dwDestination = lineinfo_Dest.dwLineID;   
			lineinfo_Connector.dwSource = dwSource;   
			if(::mixerGetLineInfo(   
				(HMIXEROBJ)m_hMixer,   
				&lineinfo_Connector,     
				MIXER_GETLINEINFOF_SOURCE) != MMSYSERR_NOERROR)
				continue;  

			MIXERCONTROL mxc;   
			MIXERLINECONTROLS mxlc;   
			mxlc.cbStruct = sizeof(MIXERLINECONTROLS);   
			mxlc.dwLineID = lineinfo_Connector.dwLineID;   
			mxlc.dwControlType = MIXERCONTROL_CONTROLTYPE_VOLUME;   
			mxlc.cControls = 1;   
			mxlc.cbmxctrl = sizeof(MIXERCONTROL);   
			mxlc.pamxctrl = &mxc;   
			if(::mixerGetLineControls(   
				reinterpret_cast<HMIXEROBJ>(m_hMixer),   
				&mxlc,   
				MIXER_OBJECTF_HMIXER|MIXER_GETLINECONTROLSF_ONEBYTYPE)!= MMSYSERR_NOERROR)
				continue;  

			//get volume value range now   
			MIXERCONTROLDETAILS_UNSIGNED mxcdVolume;   
			MIXERCONTROLDETAILS mxcd;   
			mxcd.cbStruct = sizeof(MIXERCONTROLDETAILS);   
			mxcd.dwControlID = mxc.dwControlID;   
			mxcd.cChannels = 1;   
			mxcd.cMultipleItems = 0;   
			mxcd.cbDetails = sizeof(MIXERCONTROLDETAILS_UNSIGNED);   
			mxcd.paDetails = &mxcdVolume;   
			if(mixerGetControlDetails(   
				reinterpret_cast<HMIXEROBJ>(m_hMixer),   
				&mxcd,   
				MIXER_GETCONTROLDETAILSF_VALUE)!= MMSYSERR_NOERROR)
				continue;  

			//set volume   
			MIXERCONTROLDETAILS_UNSIGNED mxcdVolume_Set = { mxc.Bounds.dwMaximum * dwVolume / 255 };   
			MIXERCONTROLDETAILS mxcd_Set;   
			mxcd_Set.cbStruct = sizeof(MIXERCONTROLDETAILS);   
			mxcd_Set.dwControlID = mxc.dwControlID;   
			mxcd_Set.cChannels = 1;   
			mxcd_Set.cMultipleItems = 0;   
			mxcd_Set.cbDetails = sizeof(MIXERCONTROLDETAILS_UNSIGNED);   
			mxcd_Set.paDetails = &mxcdVolume_Set;   
			if(mixerSetControlDetails(reinterpret_cast<HMIXEROBJ>(m_hMixer),   
				&mxcd_Set,   
				MIXER_OBJECTF_HMIXER | MIXER_SETCONTROLDETAILSF_VALUE) != MMSYSERR_NOERROR)
				continue;   

			::mixerClose(m_hMixer);
			m_hMixer = NULL;
			bResult = TRUE;;
		}
	}
	catch(...){ return FALSE; }
	return bResult;
}

