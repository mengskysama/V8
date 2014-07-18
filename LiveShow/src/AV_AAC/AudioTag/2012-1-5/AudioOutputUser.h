#ifndef _AUDIOOUTPUTUSER_H
#define _AUDIOOUTPUTUSER_H

#include <string>

using namespace std;

class AudioOutputUser{
	friend class AudioOutput;
protected:
	unsigned int iBufferSize;
	void resizeBuffer(unsigned int newsize);
public:
	AudioOutputUser(const wstring name);
	~AudioOutputUser();
	const wstring qsName;
	float *pfBuffer;
	float *pfVolume;
	virtual bool needSamples(unsigned int snum) = 0;
};

#endif
