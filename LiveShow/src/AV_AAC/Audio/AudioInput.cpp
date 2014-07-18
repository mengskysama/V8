#include "StdAfx.h"
#include "AudioInput.h"
#include "MyAssert.h"
#include "AvDefine.h"
#include "./AACPlusEncode/FactoryImpl.h"

std::map<wstring, AudioInputRegistrar *> AudioInputRegistrar::qmNew;	//音频采集
wstring AudioInputRegistrar::current = wstring();

AudioInputRegistrar::AudioInputRegistrar(const wstring &n, int p) : name(n), priority(p) {
	qmNew.insert(make_pair(name,this));
	qmNew[name] = this;
}

AudioInputRegistrar::~AudioInputRegistrar() {
	if (qmNew.find(name) != qmNew.end())
	{
		qmNew.erase(qmNew.find(name));
	}
}

//选择一个新的采集器 directsound or wasapi
AudioInputPtr AudioInputRegistrar::newFromChoice(wstring choice) {
	if ( qmNew.size()==0)
		return AudioInputPtr();

	if (!choice.empty() && qmNew.find(choice) != qmNew.end()) {
		g_struct.s.qsAudioInput = choice;
		current = choice;
		AudioInputRegistrar * air = qmNew.find(current)->second;
		
		return AudioInputPtr(air->create());
	}
	choice = g_struct.s.qsAudioInput;
	if (qmNew.find(choice) != qmNew.end()) {
		current = choice;
		return AudioInputPtr(qmNew.find(choice)->second->create());
	}

	AudioInputRegistrar *r = NULL;
	std::map<wstring, AudioInputRegistrar *>::iterator mit;
	for (mit = qmNew.begin(); mit != qmNew.end(); mit++)
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

	m_de_cdDecoder = NULL;
	iLevel = 0;
	iAudioQuality = g_struct.s.iQuality;
	umtType = MessageHandler::UDPVoiceAACPlus;
	m_cCodec = NULL;
	m_ceEncoder = NULL;

	m_pCodec = NULL;
	m_pFactory = NULL;

	if (umtType == MessageHandler::UDPVoiceCELT) {
		iSampleRate = SAMPLE_RATE;
		iFrameSize = SAMPLE_RATE / SAMPLE_DIV;
		Trace("AudioInput: %d bits/s, %d hz, %d sample CELT", iAudioQuality, iSampleRate, iFrameSize);
	}
	else if (umtType == MessageHandler::UDPVoiceAACPlus)
	{
		iSampleRate = SAMPLE_RATE;
		iFrameSize = SAMPLE_RATE / SAMPLE_DIV;
		memset(m_pBufTemp,0,sizeof(m_pBufTemp));
		memset(m_pBufEncode,0,sizeof(m_pBufEncode));
	}
	else {
		Assert(0);
	}

	cFrameCounter = 0;
	
	psMic = new short[iFrameSize*2];//2个声道
	
	bPreviousVoice = false;

	bRunning = false;

	WAVEFORMATEX  wfx;

	ZeroMemory(&wfx, sizeof(wfx));
	wfx.wFormatTag = WAVE_FORMAT_PCM;
	wfx.nChannels = 2;
	wfx.nSamplesPerSec = iSampleRate;
	wfx.nBlockAlign = 4;
	wfx.nAvgBytesPerSec = wfx.nSamplesPerSec * wfx.nBlockAlign;
	wfx.wBitsPerSample = 16;

// 	if (FAILED(g_pWaveFile.Open(L"c:\\mywave100.wav", &wfx, WAVEFILE_WRITE)))
// 	{
// 		g_pWaveFile.Close();
// 	}
}

AudioInput::~AudioInput() {

//	g_pWaveFile.Close();

	bRunning = false;
	if (umtType == MessageHandler::UDPVoiceCELT) {
		if (m_ceEncoder) {
			m_cCodec->encoder_destroy(m_ceEncoder);
		}

		if (m_de_cdDecoder) {
			m_cCodec->decoder_destroy(m_de_cdDecoder);
		}
	}
	else if (umtType == MessageHandler::UDPVoiceAACPlus)
	{
		SAFE_DELETE(m_pCodec);
	}

	delete [] psMic;
	
	CELTCodec::deleteInstance();
}

void AudioInput::encodeAudioFrame() {

	if (! bRunning) {
		return;
	}

	int iIsSpeech=1;
	short *psSource = psMic;

	//编码 speex或者CELT
	unsigned char buffer[512];
	int len;
	
	if (umtType == MessageHandler::UDPVoiceCELT) 
	{
		bool bNew = false;
		if (m_cCodec == NULL)
		{
			m_cCodec = CELTCodec::instance();
			m_ceEncoder = m_cCodec->encoderCreate();
			bNew = true;
		}
		else if (m_cCodec && ! bPreviousVoice) {
			m_cCodec->encoder_ctl(m_ceEncoder, CELT_RESET_STATE);
			bNew = true;
		}
		else if (g_struct.s.bResetEncode)
		{
			m_cCodec->encoder_ctl(m_ceEncoder, CELT_RESET_STATE);
			bNew = true;
			g_struct.s.bResetEncode = false;
		}

		if (!m_cCodec)
			return;

		if (bNew)
		{
			if (m_cCodec->encoder_ctl(m_ceEncoder, CELT_SET_PREDICTION(g_struct.s.iPrediction))!=CELT_OK)
			{
				Assert("prediction request failed!\n");
				return;
			}

			if (m_cCodec->encoder_ctl(m_ceEncoder,CELT_SET_BITRATE(iAudioQuality))!=CELT_OK)
			{
				Assert("bitrate request failed!\n");
				return;
			}

			if(g_struct.s.with_cbr)
			{
				if (m_cCodec->encoder_ctl(m_ceEncoder,CELT_SET_VBR(0))!=CELT_OK)
				{
					Trace("cbr request failed!\n");
					return;
				}
			}
			else if (g_struct.s.with_vbr)
			{
				if (m_cCodec->encoder_ctl(m_ceEncoder,CELT_SET_VBR(1))!=CELT_OK)
				{
					Trace("vbr request failed!\n");
					return;
				}
			}

			if (g_struct.s.icomplex != -1)
			{
				if (m_cCodec->encoder_ctl(m_ceEncoder,CELT_SET_COMPLEXITY(g_struct.s.icomplex))!=CELT_OK)
				{
					Trace("complexity request failed!\n");
					return;
				}
			}
		}
		len = m_cCodec->encode(m_ceEncoder, psSource, SAMPLE_RATE / SAMPLE_DIV, buffer, 255); //255为输出压缩音频数据最大长度

		int iBitrate = len * 50 * 8;
		
		if (m_de_cdDecoder == NULL) {
			m_de_cdDecoder = m_cCodec->decoderCreate();
		}

	} 
	else if (umtType == MessageHandler::UDPVoiceAACPlus)
	{
		if (NULL == m_pCodec)
		{
			m_pFactory = GetFloatEncodeFactory();
			m_pCodec = m_pFactory->RegisterCodec();
			if (!m_pCodec->Create(CODEC_ENCODE))
			{
				ErrTrace("m_pCodec->Create Failed!\n");
			}
		}
		
		m_cache.Write((unsigned char*)psSource,iFrameSize*4);
		//筹足8192个数据
 		if (m_cache.Size()<ACC_ENCODE_INPUT_SIZE) return;
 		m_cache.Read(m_pBufEncode,ACC_ENCODE_INPUT_SIZE);
		
		if (!(m_pCodec && m_pCodec->Encode(m_pBufEncode,ACC_ENCODE_INPUT_SIZE,buffer,len)))
		{
			ErrTrace("m_pCodec->Encode 编码失败....\n");
		}

		int iBitrate = len * 21 * 8;
	}
	else {
		Assert(0);
	}

	static QByteArray qba(512);
	memcpy(&qba[2], buffer, len);
	*((short*)&qba[0]) = (short)len;
	
	flushCheck(qba, false);

	bPreviousVoice = iIsSpeech;
}

void AudioInput::flushCheck(const QByteArray &frame, bool terminator) {

	qlFrames.push_back(frame);
	if (umtType == MessageHandler::UDPVoiceCELT &&! terminator && qlFrames.size() < g_struct.s.iFramesPerPacket)
		return;
	
	cFrameCounter++;

	char data[512] = {0};
	PacketDataStream pds(data, 512);
	unsigned char cflag = (unsigned char)(cFrameCounter - qlFrames.size());
	pds.append8(cflag);

	if (terminator)
		for (int i=0;i<PER_FRAME_OF_SAMEPLE;i++)
		{
			qlFrames.push_back(QByteArray()); 
		}

	for (int i=0;i<qlFrames.size(); ++i) { 
		const QByteArray &qba = qlFrames.at(i);
		if (*((short*)&qba[0]) == 0) continue;
		short head = static_cast<short>(*((short*)&qba[0]));		
		pds.append16(htons(head));
		pds.append((char*)(&qba[2]), head);

		if(!pds.isValid())
		{
			LostTrace("音频包错误 .....\n");
			Assert(0);
#ifndef IGNOR_INT_3
			_asm int 3;
#endif
		}
	}

	if (g_struct.s.lmLoopMode == Settings::Local)
	{
		//如果是本地预览声音，则添加到本地预览声音缓存
		QByteArray qba;
		for(int i=0; i<pds.size(); i++)
		{
			qba.push_back(data[i]);
		}
		LoopUser::lpLoopy.addFrame(qba);
	}
	else if (g_struct.trans)
	{
		//获得到数据，准备发送                123+1
		g_struct.trans->SendAudioData(data,pds.size()); 
	}

	qlFrames.clear();
}

