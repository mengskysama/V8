#ifndef _AUDIOOUTPUTUSER_H
#define _AUDIOOUTPUTUSER_H

#include <string>
#include "Common.h"

using namespace std;

class AudioOutputUser{
	friend class AudioOutput;
private:
	short* m_pfBuffer;
public:
	AudioOutputUser(const wstring name);
	virtual ~AudioOutputUser();
	short* getBuffer(){
		return m_pfBuffer;
	}
	void setBuffer(short* pBuffer){
		m_pfBuffer = pBuffer;
	}
	const wstring qsName;		
	virtual bool needSamples(unsigned int snum) = 0;
	short* test_a,*test_b,*test_c,*test_d,*test_e;
	UInt64 uiSession;
};

#endif
