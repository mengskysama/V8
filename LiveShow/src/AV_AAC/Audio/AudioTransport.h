#pragma once

class AudioTransport
{
public:
	
	~AudioTransport(void){}

protected:
	AudioTransport()
	{

	}

public:
	virtual void SendAudioData(const void* data,int len,bool bRTP = true) = 0;
};
