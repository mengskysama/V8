#include "StdAfx.h"
#include "AudioOutputUser.h"
#include "MyAssert.h"

AudioOutputUser::AudioOutputUser(const wstring name) : qsName(name) {
	m_pfBuffer = NULL;
}

AudioOutputUser::~AudioOutputUser() {
}