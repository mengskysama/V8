#pragma once
#include "ICodec.h"

class FloatAACPlusEncodeCodec:public ICodec
{
public:
	FloatAACPlusEncodeCodec();
	~FloatAACPlusEncodeCodec();

	virtual bool Create(En_Codec_Mode en = CODEC_ENCODE);
	virtual void Destroy();
	virtual bool Encode(unsigned char* p,int len,unsigned char* outp,int&outl);
	virtual bool Decode(const unsigned char* p,int len,unsigned char* outp,int&outl);
	virtual bool SetMode(En_Codec_Param en = PARAM_ONE);

	virtual bool GetHeadBuf(unsigned char*& buf,int&len);

private:
	int	init(unsigned int sample_rate, unsigned int bitrate, unsigned int channels);

	void*	m_hSbr;
	void*	m_hAac;

	unsigned int  m_iSampleRate;
	unsigned int  m_iBitRate;
	unsigned int  m_iChannel;

	int		writeOffset;
	int		envReadOffset;
	int		coreWriteOffset;
	int		useParametricStereo;

	unsigned char* ancDataBytes;
	unsigned int* outputBuffer;
	float*	inputBuffer;
};
