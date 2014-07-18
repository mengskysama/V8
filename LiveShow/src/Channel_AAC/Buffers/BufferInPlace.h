#pragma once

enum En_Buffer_Type
{
	bfAudio = 0x01,
	bfVideo = 0x02,
	bfOther = 0x03
};

class ITransInPlace
{
public:
	virtual unsigned int GetBufferSize(unsigned int uin,En_Buffer_Type en) = 0;
	virtual void SetBufferSize(unsigned int uin,En_Buffer_Type en,unsigned int bufsize)=0;
	virtual void SetRecvTime(unsigned int uin,En_Buffer_Type en,DWORD dwtime)=0;
	virtual DWORD GetRecvTime(unsigned int uin,En_Buffer_Type en)=0;
};