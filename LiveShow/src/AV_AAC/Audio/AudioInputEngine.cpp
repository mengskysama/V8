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

		DeferInit::run_destroyers();
	}

	bInit = false;
}

void AudioInputEngine::Stop()
{
	Trace("AudioInputEngine::Stop() begin\n");
	if (g_struct.ai)
	{
		g_struct.ai->StopAndWaitForThread();
	}
	bStart = false;
	Trace("AudioInputEngine::Stop() end\n");
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

	if (/*AudioInputRegistrar::qmNew && */AudioInputRegistrar::qmNew.find(g_struct.s.strSysSelected) != AudioInputRegistrar::qmNew.end()) {
		AudioInputRegistrar *air = AudioInputRegistrar::qmNew.find(g_struct.s.strSysSelected)->second;
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
		//可能音频设备不存在导致崩溃
		int iDevice = iInputDevice;
		AudioInputRegistrar *air = AudioInputRegistrar::qmNew.find(g_struct.s.strSysSelected)->second;
		std::vector<audioDevice>  ql = air->getDeviceChoices();
		if (ql.size() == 0)
		{
			return;
		}
		else if (iDevice >=  ql.size())
		{
			iDevice = 0;
		}
		air->setDeviceChoice(ql[iDevice].second,g_struct.s);
	}
}

void AudioInputEngine::SetVolumeInput(int iVolume)
{
	if (!g_struct.ai)
	{
		g_struct.ai = AudioInputRegistrar::newFromChoice(g_struct.s.strSysSelected);
	}
	g_struct.ai->SetMicrPhoneVolume(iVolume);
}

int AudioInputEngine::GetVolumeInput()
{
	if (!g_struct.ai)
	{
		g_struct.ai = AudioInputRegistrar::newFromChoice(g_struct.s.strSysSelected);
	}
	return g_struct.ai->GetMicrPhoneVolume();
}

int AudioInputEngine::GetInputLevel()
{
	if (!g_struct.ai)
	{
		g_struct.ai = AudioInputRegistrar::newFromChoice(g_struct.s.strSysSelected);
	}
	return g_struct.ai->iLevel>255?255:g_struct.ai->iLevel;
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


bool AudioInputEngine::SetMicInputMute(bool bMute)
{
	if (!g_struct.ai)
	{
		g_struct.ai = AudioInputRegistrar::newFromChoice(g_struct.s.strSysSelected);
	}	
	return g_struct.ai->SetMicInputMute(bMute);
}

bool AudioInputEngine::GetMicInputMute(bool& bMute)
{
	if (!g_struct.ai)
	{
		g_struct.ai = AudioInputRegistrar::newFromChoice(g_struct.s.strSysSelected);
	}
	return g_struct.ai->GetMicInputMute(bMute);
}

void AudioInputEngine::SetMicRoomAllVolume(int iVolume)
{
	g_struct.s.fMicVolume = (float)iVolume / 255;
}

int  AudioInputEngine::GetMicRoomAllVolume()
{
	return g_struct.s.fMicVolume * 255;
}

//设置音频编码参数
bool AudioInputEngine::SetMode(enuROOM_AV_AUDIO_MODE mode)
{
	switch (mode)
	{
	case ROOM_AUDIO_CBR_MODE://音频恒定码率 7K 复杂度默认
		{
			g_struct.s.bResetEncode = true; //重置编码器参数
			g_struct.s.iQuality = BIT_RATE; //56000  56kbps
			g_struct.s.icomplex = -1;//默认复杂度
			g_struct.s.with_cbr = 1;//设置固定码率
			g_struct.s.with_vbr = 0;//设置动态码率
		}
		break;
	case ROOM_AUDIO_VBR_MODE://音频变动码率 7K +1K  复杂度默认
		{
			g_struct.s.bResetEncode = true; //重置编码器参数
			g_struct.s.iQuality = BIT_RATE; //56000  56kbps
			g_struct.s.icomplex = -1;//默认复杂度
			g_struct.s.with_cbr = 0;//设置固定码率
			g_struct.s.with_vbr = 1;//设置动态码率
		}
		break;
	case ROOM_AUDIO_CBR_SIXD://音频恒定码率 6K 复杂度默认
		{
			g_struct.s.bResetEncode = true; //重置编码器参数
			g_struct.s.iQuality = BIT_RATE; //56000  56kbps
			g_struct.s.icomplex = -1;//默认复杂度
			g_struct.s.with_cbr = 0;//设置固定码率
			g_struct.s.with_vbr = 1;//设置动态码率
		}
		break;
	case ROOM_AUDIO_CBR_EIGD://音频恒定码率 8K 复杂度默认
		{
			g_struct.s.bResetEncode = true; //重置编码器参数
			g_struct.s.iQuality = 64000; //64kbps
			g_struct.s.icomplex = -1;//默认复杂度
			g_struct.s.with_cbr = 1;//设置固定码率
			g_struct.s.with_vbr = 0;//设置动态码率
		}
		break;
	case ROOM_AUDIO_CBR_NIND://音频恒定码率 9k 复杂度默认
		{
			g_struct.s.bResetEncode = true; //重置编码器参数
			g_struct.s.iQuality = 72000; //输出码率
			g_struct.s.icomplex = -1;//默认复杂度
			g_struct.s.with_cbr = 1;//设置固定码率
			g_struct.s.with_vbr = 0;//设置动态码率
		}
		break;
	case ROOM_AUDIO_VBR_SIXD://音频动态码率 6K 复杂度默认
		{
			g_struct.s.bResetEncode = true; //重置编码器参数
			g_struct.s.iQuality = 48000; //输出码率
			g_struct.s.icomplex = -1;//默认复杂度
			g_struct.s.with_cbr = 0;//设置固定码率
			g_struct.s.with_vbr = 1;//设置动态码率
		}
		break;
	case ROOM_AUDIO_VBR_EIGD://音频动态码率 8K 复杂度默认
		{
			g_struct.s.bResetEncode = true; //重置编码器参数
			g_struct.s.iQuality = 64000; //输出码率
			g_struct.s.icomplex = -1;//默认复杂度
			g_struct.s.with_cbr = 0;//设置固定码率
			g_struct.s.with_vbr = 1;//设置动态码率
		}
		break;
	case ROOM_AUDIO_VBR_NIND://音频动态码率 9K 复杂度默认
		{
			g_struct.s.bResetEncode = true; //重置编码器参数
			g_struct.s.iQuality = 72000; //输出码率
			g_struct.s.icomplex = -1;//默认复杂度
			g_struct.s.with_cbr = 0;//设置固定码率
			g_struct.s.with_vbr = 1;//设置动态码率
		}
		break;
	case ROOM_AUDIO_VBR_SIXC://音频动态码率 6K 复杂度 5
		{
			g_struct.s.bResetEncode = true; //重置编码器参数
			g_struct.s.iQuality = 48000; //输出码率
			g_struct.s.icomplex = 5;//默认复杂度
			g_struct.s.with_cbr = 0;//设置固定码率
			g_struct.s.with_vbr = 1;//设置动态码率
		}
		break;
	case ROOM_AUDIO_VBR_MODC://音频动态码率 7K 复杂度5
		{
			g_struct.s.bResetEncode = true; //重置编码器参数
			g_struct.s.iQuality = 56000; //输出码率
			g_struct.s.icomplex = 5;//默认复杂度
			g_struct.s.with_cbr = 0;//设置固定码率
			g_struct.s.with_vbr = 1;//设置动态码率
		}
		break;
	case ROOM_AUDIO_VBR_NINC://音频动态码率 8K 复杂度5
		{
			g_struct.s.bResetEncode = true; //重置编码器参数
			g_struct.s.iQuality = 64000; //输出码率
			g_struct.s.icomplex = 5;//默认复杂度
			g_struct.s.with_cbr = 0;//设置固定码率
			g_struct.s.with_vbr = 1;//设置动态码率
		}
		break;
	}
	return true;
}
