#include "StdAfx.h"
#include "AudioOutputUser.h"
#include "MyAssert.h"

AudioOutputUser::AudioOutputUser(const wstring name) : qsName(name) {
	iBufferSize = 0;
	pfBuffer = NULL;
}

AudioOutputUser::~AudioOutputUser() {
	if (pfBuffer)
		delete [] pfBuffer;
}

void AudioOutputUser::resizeBuffer(unsigned int newsize) {
	if (newsize > iBufferSize) {
		short *n = new short[newsize];
		if (pfBuffer) {
			memcpy(n, pfBuffer, sizeof(short) * iBufferSize);
			delete [] pfBuffer;
		}
		pfBuffer = n;
		iBufferSize = newsize;
	}
}