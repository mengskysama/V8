#pragma once
#include "ICodec.h"
#include "Global.h"

class FloatAACPlusDecodeCodec:public ICodec
{
public:
	FloatAACPlusDecodeCodec();
	~FloatAACPlusDecodeCodec();

	virtual bool Create(En_Codec_Mode en = CODEC_ENCODE);
	virtual void Destroy();
	virtual bool Encode(unsigned char* p,int len,unsigned char* outp,int&outl);
	virtual bool Decode(const unsigned char* p,int len,unsigned char* outp,int&outl);
	virtual bool SetMode(En_Codec_Param en = PARAM_ONE);

	virtual bool GetHeadBuf(unsigned char*& buf,int&len);
private:
	int	init(unsigned int sample_rate, unsigned int bitrate, unsigned int channels);

	int bitstream_fill_buffer(unsigned char* code, int code_size);
	int inter_leave_samples(float *time0, float *time1, short *time_out, int frame_size, int *channels);

	void*	m_hSbr;
	void*	m_hAac;

	unsigned int m_iSampleRate;
	unsigned int m_iBitRate;
	unsigned int m_iChannel;
	unsigned int frameCounter;  
	unsigned int* inBuffer; 

	void* m_BitBuf;
	void* m_hBitBuf;
	void* m_pStreamSBR;

	unsigned int keyflag;
	float* TimeDataFloat;
};
