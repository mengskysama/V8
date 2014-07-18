#include "StdAfx.h"
#include "AudioInputEngine.h"
#include "Global.h"
#include "AudioInput.h"
#include "Settings.h"

AudioInputEngine g_audioInputEngine;

AudioInputEngine::AudioInputEngine(void)
{
	bInit = false;
	bStart = false;
}

AudioInputEngine::~AudioInputEngine(void)
{
	if (bInit)
	{
		Destory();
	}
}

void AudioInputEngine::Init(AudioTransport *trans)
{
	if (!Global::g_global_struct)
	{
		Global::g_global_struct = new Global(trans);
		osInit();
		DeferInit::run_initializers();
	}
	bInit  = true;
}

void AudioInputEngine::Destory()
{
	if (Global::g_global_struct != NULL)
	{
		delete Global::g_global_struct;
		Global::g_global_struct = NULL;
	}

	bInit = false;
}

void AudioInputEngine::Stop()
{
	if (g_struct.ai)
	{
		g_struct.ai->StopAndWaitForThread();
	}
	bStart = false;
}

bool AudioInputEngine::Start()
{
	if (!bInit)
	{
		return false;
	}
	if (!g_struct.ai)
	{
		g_struct.ai = AudioInputRegistrar::newFromChoice(g_struct.s.strSysSelected);
	}
	
	if (g_struct.ai)
	{
		g_struct.ai->Start();
	}

	bStart = true;

	return true;
}

bool AudioInputEngine::IsInit()
{
	return bInit;
}

bool AudioInputEngine::IsStart()
{
	return bStart;
}


std::vector<wstring> AudioInputEngine::GetAudioInputDevice()
{
	if (lstrInputDevice.size() != 0)
	{
		return lstrInputDevice;
	}

	if (AudioInputRegistrar::qmNew && AudioInputRegistrar::qmNew->find(g_struct.s.strSysSelected) != AudioInputRegistrar::qmNew->end()) {
		AudioInputRegistrar *air = AudioInputRegistrar::qmNew->find(g_struct.s.strSysSelected)->second;
		std::vector<audioDevice>  ql = air->getDeviceChoices();

		for (std::vector<audioDevice>::iterator lit = ql.begin(); lit != ql.end(); lit++)
		{
			lstrInputDevice.push_back(lit->first);
		}
	}
	return lstrInputDevice;

}

void  AudioInputEngine::SetAudioDevice(int iInputDevice)
{
	if (iInputDevice != -1)
	{
		AudioInputRegistrar *air = AudioInputRegistrar::qmNew->find(g_struct.s.strSysSelected)->second;
		std::vector<audioDevice>  ql = air->getDeviceChoices();
		air->setDeviceChoice(ql[iInputDevice].second,g_struct.s);
	}
}

void AudioInputEngine::SetVolumeInput(int iVolume)
{
	g_struct.ai->SetMicrPhoneVolume(iVolume);
}

int AudioInputEngine::GetInputLevel()
{
	return g_struct.ai->iLevel;
}

void AudioInputEngine::SetAudioTest(bool bAudioTest)
{
	if (bAudioTest)
	{
		//设置为回环测试
		g_struct.s.lmLoopMode = Settings::LoopMode::Local;
	}
	else
	{
		g_struct.s.lmLoopMode = Settings::LoopMode::None;
	}
}
