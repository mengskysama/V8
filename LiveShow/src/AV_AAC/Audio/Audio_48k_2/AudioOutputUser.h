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
	short *pfBuffer;
	virtual bool needSamples(unsigned int snum) = 0;
	short* test_a,*test_b,*test_c,*test_d,*test_e;
};

#endif
