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
	nRcvTotalPacket = 0;
	nRcvLostPacket = 0;
	fLostRate = 0.0f;
	iLastTimeTick = 0;
	iTimeInterval = AUDIO_LOST_PACKET_TIME_INTERVAL;
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

		DeferInit::run_destroyers();
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

void AudioOutputEngine::OnRecvAudioData(UInt64 uin, unsigned char* data, unsigned int len,unsigned int iSeq)
{
	if (ClientUser::isPause(uin)) 
	{
		byte pData[1]={0};
		g_struct.PrepareWavDatas(uin,pData,1);
		return;
	}		

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 	PacketDataStream pds(data + 1, len-1);
// 	MessageHandler::UDPMessageType msgType = static_cast<MessageHandler::UDPMessageType>((data[0] >> 5) & 0x7);
// 	unsigned int msgFlags = data[0] & 0x1f;
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
	
	PacketDataStream pds(data,len);
	MessageHandler::UDPMessageType msgType = MessageHandler::UDPVoiceAACPlus/*MessageHandler::UDPVoiceCELT*/;
	
	ClientUser* p = ClientUser::get(uin); //Uin为上麦所有用户

	if(p == NULL)
	{
		Assert(0);
		return;
	}
	
	switch (msgType)
	{
	case MessageHandler::UDPVoiceAACPlus:
	case MessageHandler::UDPVoiceCELT:
		{
			pds.next8();//该字节无用
			CalcLostRate(p,iSeq);

			QByteArray qba;
			pds.dataBlock(pds.left(), qba);

			g_struct.ao->addFrameToBuffer(p, qba, iSeq, msgType);
		}
		break;
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
	if (/*AudioOutputRegistrar::qmNew && */AudioOutputRegistrar::qmNew.find(g_struct.s.strSysSelected) != AudioOutputRegistrar::qmNew.end()) {
		AudioOutputRegistrar *aor = AudioOutputRegistrar::qmNew.find(g_struct.s.strSysSelected)->second;
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
		int iDevice = iOutputDevice;
		AudioOutputRegistrar *aor = AudioOutputRegistrar::qmNew.find(g_struct.s.strSysSelected)->second;
		std::vector<audioDevice>  ql = aor->getDeviceChoices();
		if (ql.size() == 0)
		{
			return;
		}
		else if (iDevice >= ql.size())
		{
			iDevice = 0;
		}
		aor->setDeviceChoice(ql[iDevice].second,g_struct.s);
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
	if (!g_struct.ao)
	{
		g_struct.ao = AudioOutputRegistrar::newFromChoice(g_struct.s.strSysSelected);
	}
	return g_struct.ao->iLevel>255?255:g_struct.ao->iLevel;
}


//设置输出系统音量
void AudioOutputEngine::SetSysVolume(int iVolume)
{
	g_struct.ao->setVolume(iVolume);
}

//获取系统输出音量
int AudioOutputEngine::GetSysvolume()
{
	return g_struct.ao->getVolume();
}

bool AudioOutputEngine::SetSysPlayerMute(bool bMute)
{
	return g_struct.ao->setPlayerMute(bMute);
}

bool AudioOutputEngine::GetSysPlayerMute(bool& bMute)
{
	return g_struct.ao->getPlayerMute(bMute);
}

void AudioOutputEngine::CalcLostRate(ClientUser *p,unsigned int iSeqence)
{
	UInt64 iCurTick = ::GetTickCount();
	if (iCurTick - iLastTimeTick < 0)
	{
		iLastTimeTick = 0;
	}
	if (iCurTick - iLastTimeTick > iTimeInterval)
	{
		if (nRcvTotalPacket + nRcvLostPacket == 0)
		{
			fLostRate = 0;
		}
		else
		{
			fLostRate = (float)nRcvLostPacket / (nRcvTotalPacket + nRcvLostPacket);
		}
		
		nRcvLostPacket = 0;
		nRcvTotalPacket = 0;
		iLastTimeTick = iCurTick;
		//Trace("语音丢包率为：：：：：：：：：：：：：：：：：：：：：：：：：：：：：：：%f \n",fLostRate);
	}
	
	if (iSeqence - p->uiLastActiveSeq > 1)
	{
		nRcvLostPacket  += (iSeqence - p->uiLastActiveSeq < 10 ? iSeqence - p->uiLastActiveSeq : 1);
		p->uiLostPacket += (iSeqence - p->uiLastActiveSeq < 10 ? iSeqence - p->uiLastActiveSeq : 1);
	}
	else
	{
		++nRcvTotalPacket;
		++p->uiTotalPacket;
	}
	p->uiLastActiveSeq = iSeqence;
}

float AudioOutputEngine::GetLostPacketRate()
{
	return this->fLostRate;
}
