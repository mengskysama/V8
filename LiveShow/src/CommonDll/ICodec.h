#pragma once

enum En_Codec_Mode
{
	CODEC_ENCODE = 0x01,
	CODEC_DECODE = 0x02
};

enum En_Codec_Param
{
	PARAM_ONE = 0x01,
	PARAM_TWO = 0x02
};

class ICodec
{
public:
	ICodec(){}
	virtual ~ICodec(){}
	virtual bool Create(En_Codec_Mode en = CODEC_ENCODE) = 0;
	virtual void Destroy() = 0;
	virtual bool Encode(unsigned char* p,int len,unsigned char* outp,int&outl) = 0;
	virtual bool Decode(const unsigned char* p,int len,unsigned char* outp,int&outl) = 0;
	virtual bool SetMode(En_Codec_Param en = PARAM_ONE) = 0;
	virtual bool GetHeadBuf(unsigned char*& buf,int&len) {return true;};
};