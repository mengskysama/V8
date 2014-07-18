#include "StdAfx.h"
#include "AudioOutputUser.h"
#include "MyAssert.h"

AudioOutputUser::AudioOutputUser(const wstring name) : qsName(name) {
	iBufferSize = 0;
	pfBuffer = NULL;
	pfVolume = NULL;
}

AudioOutputUser::~AudioOutputUser() {
	if (pfBuffer)
		delete [] pfBuffer;
	if (pfVolume)
		delete [] pfVolume;
}

void AudioOutputUser::resizeBuffer(unsigned int newsize) {
	if (newsize > iBufferSize) {
		float *n = new float[newsize];
		if (pfBuffer) {
			memcpy(n, pfBuffer, sizeof(float) * iBufferSize);
			delete [] pfBuffer;
		}
		pfBuffer = n;
		iBufferSize = newsize;
	}
}