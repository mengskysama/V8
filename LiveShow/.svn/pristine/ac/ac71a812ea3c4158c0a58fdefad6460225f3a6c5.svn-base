#include "stdafx.h"
#include "FloatAACPlusDecodeCodec.h"

#define SAMPLES_PER_FRAME 1024
#define INPUT_BUF_SIZE (6144*2/8)   

#include "aacdecoder.h"

FloatAACPlusDecodeCodec::FloatAACPlusDecodeCodec()
{
	m_hSbr = NULL;
	m_hAac = NULL;

	m_iSampleRate = 48000;
	m_iBitRate = 32000;
	m_iChannel = 2;
	frameCounter = 0;

	inBuffer = new unsigned int[INPUT_BUF_SIZE/(sizeof(int))];
	memset(inBuffer,0,INPUT_BUF_SIZE);

	m_pStreamSBR = new SBRBITSTREAM();

	m_BitBuf = new BIT_BUF();
	m_hBitBuf = NULL;
	keyflag = 0;
	TimeDataFloat = NULL;
}

FloatAACPlusDecodeCodec::~FloatAACPlusDecodeCodec()
{
	Destroy();
}

bool FloatAACPlusDecodeCodec::Create(En_Codec_Mode en)
{
	return (init(48000,32000,2)==0);
}

void FloatAACPlusDecodeCodec::Destroy()
{
	closeSBR(keyflag);
	CAacDecoderClose(keyflag);

	m_hSbr = NULL;
	m_hAac = NULL;

	SAFE_DELETEA(inBuffer);
	SAFE_DELETE(m_pStreamSBR);
	SAFE_DELETE(m_BitBuf);
}

bool FloatAACPlusDecodeCodec::Encode(unsigned char* p,int len,unsigned char* outp,int&outl)
{
	return true;
}

bool FloatAACPlusDecodeCodec::Decode(const unsigned char* p,int len,unsigned char* outp,int&outl)
{
	if(m_hAac == NULL)
	{
		return 0;
	}

	((SBRBITSTREAM*)m_pStreamSBR)->NrElements = 0;
	if(bitstream_fill_buffer((unsigned char *)p, len) != 0)
		return 0;

	int frame_size = 0;
	int sample_rate = 0;
	int channels = 0;
	char channel_mode = 0;

	int error_status = CAacDecoder_DecodeFrame(keyflag,(AACDECODER)m_hAac, &frame_size, &sample_rate, &channels, &channel_mode, 1);
	if(error_status != 0)
	{
		return 0;
	}

	int bDownSample = 0;

	//构建SBR对象
	if (m_hSbr == NULL && ((SBRBITSTREAM*)m_pStreamSBR)->NrElements) 
	{ 
		int lpFilter = 0;
		SBRDECODER sbrDecoderInfo = openSBR(keyflag,sample_rate,frame_size, bDownSample, lpFilter);
		if(sbrDecoderInfo != NULL)
		{
			m_hSbr = (void *)sbrDecoderInfo;
		}
	}

	m_iChannel = channels;
	m_iSampleRate = sample_rate;

	//SBR信号复原
	if (applySBR(keyflag,(SBRDECODER)m_hSbr, (SBRBITSTREAM*)m_pStreamSBR, TimeDataFloat, &channels, 1, bDownSample, 0) != SBRDEC_OK)
	{	
		m_hSbr = 0;
	}

	if (!bDownSample){
		frame_size = frame_size*2;
		sample_rate *= 2;
	}

	//数据转换
	for (int i = 0; i < frame_size * channels; i++) 
	{
		if (TimeDataFloat[i] < -32768.0) {        
			TimeDataFloat[i] = -32768.0;
		} else {
			if (TimeDataFloat[i] > 32767.0) {          
				TimeDataFloat[i] = 32767.0;
			}
		}
	}

	inter_leave_samples(&TimeDataFloat[0], &TimeDataFloat[frame_size], (short*)outp, frame_size, &channels);
	if (frameCounter>0) 
		outl =frame_size * channels * 2;
	else outl = 0;
	frameCounter++;

	return true;
}

bool FloatAACPlusDecodeCodec::SetMode(En_Codec_Param en)
{
	return true;
}

bool FloatAACPlusDecodeCodec::GetHeadBuf(unsigned char*& buf,int&len)
{
	return true;
}

int FloatAACPlusDecodeCodec::init(unsigned int sample_rate, unsigned int bitrate, unsigned int channels)
{
	m_iSampleRate = sample_rate;
	m_iBitRate = bitrate;
	m_iChannel = channels;

	m_hBitBuf = CreateBitBuffer((BIT_BUF*)m_BitBuf,(unsigned char*)inBuffer, INPUT_BUF_SIZE);

	AACDECODER aacDecoderInfo = NULL;
	aacDecoderInfo = CAacDecoderOpen(keyflag,(HANDLE_BIT_BUF)m_hBitBuf,  (SBRBITSTREAM*)m_pStreamSBR, TimeDataFloat);
	if(aacDecoderInfo == NULL)
	{
		return -2;
	}

	memset(TimeDataFloat,0,4*SAMPLES_PER_FRAME*sizeof(float));

	int error_status = CAacDecoderInit(aacDecoderInfo, m_iSampleRate/2, m_iBitRate);
	if(error_status)
	{
		return -3;
	}

	m_hAac = (void *)aacDecoderInfo;
	return 0;
}

int FloatAACPlusDecodeCodec::bitstream_fill_buffer(unsigned char* code, int code_size)
{
	for(int i = 0; i<code_size; i++){
// 		char buf[512] = {0};
// 		sprintf(buf,"cntBits = %d\n",((HANDLE_BIT_BUF)m_hBitBuf)->cntBits);
// 		OutputDebugStringA(buf);
		WriteBits((HANDLE_BIT_BUF)m_hBitBuf, code[i], 8);
	}
	return 0;
}

int FloatAACPlusDecodeCodec::inter_leave_samples(float *time0, float *time1, short *time_out, int frame_size, int *channels)
{
	int i;
	for(i=0; i<frame_size; i++)
	{
		*time_out++ = (short) *time0++;

		if(*channels == 2) {
			*time_out++ = (short) *time1++;
		}else{
			*time_out = *(time_out-1);
			*time_out++;
		}
	}
	*channels = 2;
	return 0;
}

