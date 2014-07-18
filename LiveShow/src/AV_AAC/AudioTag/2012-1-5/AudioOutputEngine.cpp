#include "StdAfx.h"
#include "AudioOutputEngine.h"
#include "Global.h"
#include "AudioOutput.h"
#include "Settings.h"

AudioOutputEngine g_audioOutputEngine;

AudioOutputEngine::AudioOutputEngine(void)
{
	bInit = false;
	bStart = false;
}

AudioOutputEngine::~AudioOutputEngine(void)
{
	if (bInit)
	{
		Destory();
	}
}

void AudioOutputEngine::Init(AudioTransport *trans)
{
	if (!Global::g_global_struct)
	{
		Global::g_global_struct = new Global(trans);
		osInit();
		DeferInit::run_initializers();
	}
	bInit  = true;
}

void AudioOutputEngine::Destory()
{
	if (Global::g_global_struct != NULL)
	{
		delete Global::g_global_struct;
		Global::g_global_struct = NULL;
	}

	bInit = false;
}


void AudioOutputEngine::Stop()
{
	if (g_struct.ao)
	{
		g_struct.ao->StopAndWaitForThread();
	}
	bStart = false;
}

bool AudioOutputEngine::Start()
{
	if (!bInit)
	{
		return false;
	}
	if (!g_struct.ao)
	{
		g_struct.ao = AudioOutputRegistrar::newFromChoice(g_struct.s.strSysSelected);
	}
	//启动output线程
	if (g_struct.ao)
		g_struct.ao->Start();

	bStart = true;

	return true;
}

bool AudioOutputEngine::IsInit()
{
	return bInit;
}

bool AudioOutputEngine::IsStart()
{
	return bStart;
}

void AudioOutputEngine::SetAudioTest(bool bAudioTest)
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

void AudioOutputEngine::AddUserSession(UInt64 uin)
{
	ClientUser::add(uin);
}

void AudioOutputEngine::RemoveUserSession(UInt64 uin)
{
	ClientUser::remove(uin);
}

void AudioOutputEngine::PauseUserSession(UInt64 uin,bool bIsPause)
{
	ClientUser::setPause(uin,bIsPause);
}

void AudioOutputEngine::RemoveRoomSession(UInt32 iRoomId)
{
	ClientUser::clearRoom(iRoomId);
}

void AudioOutputEngine::RemoveRoomAllSession()
{
	ClientUser::clearRoom();
}

void AudioOutputEngine::OnRecvAudioData(UInt64 uin, unsigned char* data, unsigned int len)
{
	static int count=0;
	if(count%20==0)
	{
		//return;
	}

	PacketDataStream pds(data + 1, len-1);

	MessageHandler::UDPMessageType msgType = static_cast<MessageHandler::UDPMessageType>((data[0] >> 5) & 0x7);
	unsigned int msgFlags = data[0] & 0x1f;

	ClientUser *p = ClientUser::get(uin); //Uin为上麦所有用户
	switch (msgType) {
			case MessageHandler::UDPVoiceCELT:
			case MessageHandler::UDPVoiceSpeex:
				{	
					if (p) {
						unsigned int iSeq;
						pds >> iSeq;
						QByteArray qba;
						qba.reserve(pds.left() + 1);
						qba.push_back(static_cast<char>(msgFlags));
						QByteArray qbatmp = pds.dataBlock(pds.left());
						for (int i=0; i<qbatmp.size(); i++)
						{
							qba.push_back(qbatmp[i]);
						}
						g_struct.ao->addFrameToBuffer(p, qba, iSeq, msgType);
					}
					break;
				}
			default:
				break;
	}
}

std::vector<wstring> AudioOutputEngine::GetAudioOutputDevice()
{
	if (lstrOutputDevice.size() != 0)
	{
		return lstrOutputDevice;
	}
	if (AudioOutputRegistrar::qmNew && AudioOutputRegistrar::qmNew->find(g_struct.s.strSysSelected) != AudioOutputRegistrar::qmNew->end()) {
		AudioOutputRegistrar *aor = AudioOutputRegistrar::qmNew->find(g_struct.s.strSysSelected)->second;
		std::vector<audioDevice>  ql = aor->getDeviceChoices();
		for (std::vector<audioDevice>::iterator lit = ql.begin(); lit != ql.end(); lit++)
		{
			lstrOutputDevice.push_back(lit->first);
		}
	}
	return lstrOutputDevice;
}

void  AudioOutputEngine::SetAudioOutputDevice(int iOutputDevice)
{
	if (iOutputDevice != -1)
	{
		AudioOutputRegistrar *aor = AudioOutputRegistrar::qmNew->find(g_struct.s.strSysSelected)->second;
		std::vector<audioDevice>  ql = aor->getDeviceChoices();
		aor->setDeviceChoice(ql[iOutputDevice].second,g_struct.s);
	}
}

void  AudioOutputEngine::SetVolumeOutput(int iVolume)
{
	g_struct.s.fVolume = (float)iVolume / 255;
}

int AudioOutputEngine::GetVolumeOutput()
{
	return g_struct.s.fVolume * 255;
}

void AudioOutputEngine::SetSystemType(wstring strAudioSystem)
{
	g_struct.s.strSysSelected = strAudioSystem;
}

int AudioOutputEngine::GetOutputLevel()
{
	return g_struct.ao->iLevel;
}