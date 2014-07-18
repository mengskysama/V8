#include "stdafx.h"
#include "FloatAACPlusEncodeCodec.h"
#include "Common.h"
extern "C"{
#include "sbr_main.h"
#include "aacenc.h"
#include "aac_ram.h"
#include "aac_rom.h"
};

#define CORE_DELAY   (1600)
#define INPUT_DELAY  ((CORE_DELAY)*2 +6*64-2048+1)
#define MAX_DS_FILTER_DELAY 16                         
#define CORE_INPUT_OFFSET_PS (0)
#define MAX_CHANNELS 2

FloatAACPlusEncodeCodec::FloatAACPlusEncodeCodec()
{
	m_hSbr = NULL;
	m_hAac = NULL;
	inputBuffer = NULL;
	outputBuffer = NULL;

	//默认参数
	m_iSampleRate = 48000;
	m_iBitRate = 32000;
	m_iChannel = 2;

	writeOffset = INPUT_DELAY*MAX_CHANNELS;
	envReadOffset = 0;
	coreWriteOffset = 0;

	ancDataBytes = new unsigned char[MAX_PAYLOAD_SIZE];
}

FloatAACPlusEncodeCodec::~FloatAACPlusEncodeCodec()
{
	Destroy();
}

bool FloatAACPlusEncodeCodec::Create(En_Codec_Mode en)
{
	return (init(48000,32000,2)==0);
}

void FloatAACPlusEncodeCodec::Destroy()
{
	if(m_hSbr != NULL)
	{
		EnvClose((HANDLE_SBR_ENCODER)m_hSbr);
		m_hSbr = NULL;
	}

	if(m_hAac != NULL)
	{
		AacEncClose((struct AAC_ENCODER *)m_hAac);
		m_hAac = NULL;
	}

	SAFE_DELETEA(inputBuffer);
	SAFE_DELETEA(outputBuffer);
	SAFE_DELETEA(ancDataBytes);
}

bool FloatAACPlusEncodeCodec::Encode(unsigned char* p,int len,unsigned char* outp,int&outl)
{
	if(m_hSbr == NULL || m_hAac == NULL)
		return false;

	unsigned int numAncDataBytes=0;
	short* inputStream = (short*)p;
	memset(ancDataBytes,0,MAX_PAYLOAD_SIZE);

	for(register int i=0; i<len/sizeof(short); ++ i) 
	{
		inputBuffer[i+writeOffset] = (float) inputStream[i];
	}

	EnvEncodeFrame ((HANDLE_SBR_ENCODER)m_hSbr,
		inputBuffer + envReadOffset,
		inputBuffer + coreWriteOffset,
		MAX_CHANNELS,
		&numAncDataBytes,
		ancDataBytes);

	if(useParametricStereo)
	{
		AacEncEncode((struct AAC_ENCODER *)m_hAac, 
			inputBuffer,
			1, 
			ancDataBytes,
			&numAncDataBytes,
			outputBuffer,
			&outl);
	}
	else
	{
		AacEncEncode((struct AAC_ENCODER *)m_hAac,
			inputBuffer+0,
			MAX_CHANNELS,
			ancDataBytes,
			&numAncDataBytes,
			outputBuffer,
			&outl);
	}

	memcpy(inputBuffer,inputBuffer+AACENC_BLOCKSIZE,CORE_INPUT_OFFSET_PS*sizeof(float));
	memcpy(outp, outputBuffer, outl);

	return true;
}

bool FloatAACPlusEncodeCodec::Decode(const unsigned char* p,int len,unsigned char* outp,int&outl)
{
	return true;
}

bool FloatAACPlusEncodeCodec::SetMode(En_Codec_Param en)
{
	return true;
}

bool FloatAACPlusEncodeCodec::GetHeadBuf(unsigned char*& buf,int&len)
{
	return true;
}

int FloatAACPlusEncodeCodec::init(unsigned int sample_rate, unsigned int bitrate, unsigned int channels)
{
	if(sample_rate < 44100 || bitrate < 24000 || channels < 2)
	{
		return -1;
	}

	m_iSampleRate = sample_rate;
	m_iBitRate = bitrate;
	m_iChannel = channels;
	useParametricStereo = 0;

	if ((channels == 2) && (bitrate > 16000) && (bitrate < 36000))
	{
		useParametricStereo = 1;
	}

	unsigned int sample_rate_AAC = m_iSampleRate;
	unsigned int channels_AAC = channels;
	unsigned int channels_SBR = channels;

	if (useParametricStereo)
	{
		channels_AAC = 1;
		channels_SBR = 2;
	}

	AACENC_CONFIG   config;

	AacInitDefaultConfig(&config);
	config.sampleRate = sample_rate;
	config.bitRate = bitrate;
	config.nChannelsIn = channels;
	config.nChannelsOut = channels_AAC;
	config.bandWidth = 0;

	//初始化SBR
	if(!IsSbrSettingAvail(bitrate, channels_AAC, sample_rate_AAC, &sample_rate_AAC)) 
	{
		return -2;
	}

	HANDLE_SBR_ENCODER hEnvEnc = NULL;
	sbrConfiguration sbrConfig;

	//ParametricStereo的位置
	envReadOffset   = 0;
	coreWriteOffset = 0;
	if(useParametricStereo)
	{
		envReadOffset = (MAX_DS_FILTER_DELAY + INPUT_DELAY)*MAX_CHANNELS;
		coreWriteOffset = CORE_INPUT_OFFSET_PS;
		writeOffset = envReadOffset;
	}

	//获得SBR初始化参数
	InitializeSbrDefaults(&sbrConfig);

	//使用ParametricStereo
	sbrConfig.usePs = useParametricStereo;

	AdjustSbrSettings(&sbrConfig,
		bitrate,
		channels_AAC,
		sample_rate_AAC,
		AACENC_TRANS_FAC,
		24000);

	//开辟编码器内存
	inputBuffer = new float[(AACENC_BLOCKSIZE*2 + MAX_DS_FILTER_DELAY + INPUT_DELAY)*MAX_CHANNELS];
	outputBuffer = new unsigned int[(6144/8)*MAX_CHANNELS/(sizeof(int))];

	EnvOpen(&hEnvEnc,
		inputBuffer + coreWriteOffset,
		&sbrConfig,
		&config.bandWidth);

	m_hSbr = (void *)hEnvEnc;

	//初始化AAC
	config.sampleRate = sample_rate_AAC;

	struct AAC_ENCODER *aacEnc = 0;
	if(AacEncOpen(&aacEnc, config) != 0)
	{
		AacEncClose(aacEnc);
		EnvClose(hEnvEnc);
		return -3;
	}

	m_hAac = (void *)aacEnc;

	return 0;
}