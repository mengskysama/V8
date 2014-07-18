#include "stdafx.h"
#include "AVManager.h"
#include "AVEngine.h"
#include "AVChannelMgr.h"
#include "AudioChannel.h"
#include "VideoChannel.h"
#include "VideoInputEngine.h"
#include "VideoOutputEngine.h"
#include "AVModule.h"
#include "AudioInputEngine.h"
#include "AudioOutputEngine.h"
#include "Global.h"
#include "AudioInput.h"
#include "AudioOutput.h"
#include "AV.h"
#include "coredefine.h"
#include "RelayManger.h"
#include "TimeUtils.h"
#include "MyAssert.h"

AVManager::AVManager(void)
{
	Trace("AVManager::AVManager(void): 初始化音视频库...  time = %I64d \n",GetTickCount());
	g_AVEngine.InitAVEngine();//初始化音频视频
	AVChannelMgr::Initialize();
}

AVManager::~AVManager(void)
{
	Trace("AVManager::~AVManager(void): 销毁音视频库...  time = %I64d \n",GetTickCount());

	std::map<UInt32, RCPtr<VideoInputEngine> >::iterator iter;
	for (iter=VideoInputEngine::m_VInputEngineMap.begin();iter!=VideoInputEngine::m_VInputEngineMap.end();++iter)
	{
		Trace("AVManager析构~摄像头[DeviceId=%d]停止,准备销毁.  time = %I64d \n",iter->first,GetTickCount());
		iter->second->StopCapture(true);
	}

	ClearAllRoomVideo();
	ClearAllRoomAudio();

	AVChannelMgr::Destroy();
	g_AVEngine.DestroyAVEngine();
}

bool  AVManager::ConfigVideoParam(UInt32 iWidth, UInt32 iHeight, UInt32 iQuality, int iDeviceID,unsigned int iVideoMode)
{
	Trace("AVManager::ConfigVideoParam...  time = %I64d \n",GetTickCount());
	unsigned int iResult;
	std::map<UInt32, RCPtr<VideoInputEngine> >::iterator iter;
	if((iter=VideoInputEngine::m_VInputEngineMap.find(iDeviceID))==VideoInputEngine::m_VInputEngineMap.end())
	{
		Trace("AVManager::ConfigVideoParam iDeviceID=%d...  time = %I64d \n",iDeviceID,GetTickCount());
		RCPtr<VideoInputEngine> pVInEngine = VideoInputEngine::Create(iDeviceID);
		pVInEngine->Init(iWidth,iHeight,iQuality,iDeviceID/*,iVideoMode*/);
	}
	return true;
}

bool AVManager::StartRoomVideo(UInt32 iRoomId,HWND localHwnd,int iDeviceID,enuVIDEO_TYPE enType)
{
	Trace("AVManager::StartRoomVideo...  time = %I64d \n",GetTickCount());
	std::map<UInt32, RCPtr<VideoInputEngine> >::iterator iter;
	if((iter=VideoInputEngine::m_VInputEngineMap.find(iDeviceID))!=VideoInputEngine::m_VInputEngineMap.end())
	{
		Trace("AVManager::StartRoomVideo.iDeviceID=%d..  time = %I64d \n",iDeviceID,GetTickCount());
		iter->second->AppendRoom(iDeviceID,iRoomId);
		iter->second->SetNetStatus(true);
		g_AVEngine.AddVideoLogicChannel(iRoomId,localHwnd,enType);
		return true;
	}
	return false;
}

void AVManager::CloseRoomVideo(UInt32 iRoomId,HWND localHwnd,int iDeviceID)
{
	Trace("AVManager::CloseRoomVideo begin  time = %I64d \n",GetTickCount());
	std::map<UInt32, RCPtr<VideoInputEngine> >::iterator iter;
	if((iter=VideoInputEngine::m_VInputEngineMap.find(iDeviceID))!=VideoInputEngine::m_VInputEngineMap.end())
	{
		g_AVEngine.DelVideoLogicChannel(iRoomId,localHwnd);
		iter->second->SetNetStatus(false);
		iter->second->RemoveAppendRoom(iRoomId);
	}
	//摄像头没有房间使用，释放掉

	if (!VideoInputEngine::IsDeviceUsed(iDeviceID))
	{
		if(VideoInputEngine::m_VInputEngineMap.find(iDeviceID)!=VideoInputEngine::m_VInputEngineMap.end() && !VideoInputEngine::m_VInputEngineMap.find(iDeviceID)->second->IsExistsUsed())
		{
			VideoInputEngine::m_VInputEngineMap.find(iDeviceID)->second->StopCapture();
			VideoInputEngine::m_VInputEngineMap.erase(iDeviceID);
		}
	}
	Trace("AVManager::CloseRoomVideo end  time = %I64d \n",GetTickCount());
}

bool AVManager::StartRoomAudio(UInt32 iRoomId, const UInt8& cLogicID)
{
	Trace("AVManager::StartRoomAudio  time = %I64d \n",GetTickCount());
	g_AVEngine.AddAudioLogicChannel(iRoomId,cLogicID);
	return true;
}

bool AVManager::CloseRoomAudio(UInt32 iRoomId, const UInt8& cLogicID)
{
	Trace("AVManager::CloseRoomAudio begin  time = %I64d \n",GetTickCount());
	g_AVEngine.DelAudioLogicChannel(iRoomId,cLogicID);
	Trace("AVManager::CloseRoomAudio end  time = %I64d \n",GetTickCount());
	return true;
}

bool AVManager::ClearRoomAudio(UInt32 iRoomId)
{
	Trace("AVManager::ClearRoomAudio  time = %I64d \n",GetTickCount());
	g_AVEngine.ClearAudioLogicChannel(iRoomId);
	g_AVEngine.ClearAudioLogicOutput(iRoomId);
	g_AVEngine.ClearRoomSoundMute(iRoomId);
	return true;
}

bool AVManager::ClearAllRoomAudio()
{
	Trace("AVManager::ClearAllRoomAudio  time = %I64d \n",GetTickCount());
	g_AVEngine.ClearAudioLogicChannelAll();
	g_AVEngine.ClearAudioLogicOutputAll();
	return true;
}

bool AVManager::ClearRoomVideo(UInt32 iRoomId)
{
	Trace("AVManager::ClearRoomVideo  time = %I64d \n",GetTickCount());
	g_AVEngine.DelVideoLogicChannelRoom(iRoomId);
	g_AVEngine.ClearVideoInputAppendRoom(iRoomId);
	g_AVEngine.ClearVideoOutputRoom(iRoomId);
	return true;
}

bool AVManager::ClearAllRoomVideo()
{
	Trace("AVManager::ClearAllRoomVideo  time = %I64d \n",GetTickCount());

	std::list<UInt32> listRoom;
	std::map<UInt32, RCPtr<VideoInputEngine> >::iterator iter;
	for(iter=VideoInputEngine::m_VideoInputEngineRoomMap.begin();iter!=VideoInputEngine::m_VideoInputEngineRoomMap.end();++iter)
	{
		listRoom.push_back(iter->first);
	}

	std::list<UInt32> ::iterator iter1;
	for (iter1=listRoom.begin();iter1!=listRoom.end();iter1++)
	{
		g_AVEngine.DelVideoLogicChannelRoom(*iter1);
		g_AVEngine.ClearVideoInputAppendRoom(*iter1);

	}

	g_AVEngine.ClearVideoInputAppendRoomAll();
	g_AVEngine.ClearVideoOutputRoomAll();
	return true;
}

bool AVManager::IsVideoDevInit(UInt32 iDeviceId)
{
	Trace("AVManager::IsVideoDevInit  time = %I64d \n",GetTickCount());
	return g_AVEngine.IsVideoDevInit(iDeviceId);
}

bool AVManager::SetAudioDevice(UInt32 iInputDev,UInt32 iOutputDev)
{
	Trace("AVManager::SetAudioDevice iInputDev=%d,iOutputDev=%d  time = %I64d \n",iInputDev,iOutputDev,GetTickCount());
	g_AVEngine.SetAudioDevice(iInputDev,iOutputDev);
	return true;
}

bool AVManager::StartRoomConn(UInt32 iRoomId)
{
	Trace("AVManager::StartRoomConn  time = %I64d \n",GetTickCount());
	RelayManger* pRelayMgr = RelayManger::Instance();
	AssertVS(pRelayMgr!=NULL,iRoomId,"pRelayMgr==NULL");
	return pRelayMgr->RegisterUDPChannel(iRoomId);
}

bool AVManager::CloseRoomConn(UInt32 iRoomId)
{
	Trace("AVManager::CloseRoomConn  time = %I64d \n",GetTickCount());
	RelayManger* pRelayMgr = RelayManger::Instance();
	AssertVS(pRelayMgr!=NULL,iRoomId,"pRelayMgr==NULL");
	pRelayMgr->UnregisterUDPChannel(iRoomId);
	return true;
}

bool AVManager::ChangeRelayServer(UInt32 iRoomId,UInt32 iRelayIP,UInt16 sRelayPort)
{
	Trace("AVManager::SetRelayServer  time = %I64d \n",GetTickCount());
	RelayManger* pRelayMgr = RelayManger::Instance();
	AssertVS(pRelayMgr!=NULL,iRoomId,"pRelayMgr==NULL");
	pRelayMgr->ChangeRelayServer(iRoomId,iRelayIP,sRelayPort);
	return true;
}

bool AVManager::ChangePartnerHwnd(UInt32 iRoomId,UInt32 iDesUin, HWND hPartnerWnd, UInt32 iWidth, UInt32 iHeight, HWND hOldWnd)
{
	Trace("AVManager::ChangePartnerHwnd iRoomId=%d,iDesUin=%d  time = %I64d \n",iRoomId,iDesUin,GetTickCount());
	return g_AVEngine.ChangePartnerHwnd(iRoomId,iDesUin,hPartnerWnd,iWidth,iHeight,hOldWnd);
}

bool AVManager::ChangeLocalHwnd(UInt32 iRoomId, HWND hLocalNewWnd, HWND hOldWnd)
{
	Trace("AVManager::ChangeLocalHwnd iRoomId=%d time = %I64d \n",iRoomId,GetTickCount());
	return g_AVEngine.ChangeLocalHwnd(iRoomId,hLocalNewWnd,hOldWnd);
}

bool AVManager::PaintLocal(UInt32 iRoomId,HWND hWnd)
{
	Trace("AVManager::PaintLocal iRoomId=%d time = %I64d \n",iRoomId,GetTickCount());
	return g_AVEngine.PaintLocal(iRoomId,hWnd);
}

bool AVManager::PaintRemote(UInt32 iRoomId,UInt32 iFromUin,HWND hWnd)
{
	Trace("AVManager::PaintRemote iRoomId=%d time = %I64d \n",iRoomId,GetTickCount());
	return g_AVEngine.PaintRemote(iRoomId,iFromUin,hWnd);
}

bool AVManager::ChangeLocalEffact(UInt32 iRoomId,HWND localHwnd,bool bEnable)
{
	Trace("AVManager::SetVideoDecoderEffect iRoomId=%d  time = %I64d \n",iRoomId,GetTickCount());
	return g_AVEngine.SetVideoDecoderEffect(iRoomId,localHwnd,bEnable);
}

bool AVManager::IsLocalEffact(UInt32 iRoomId,HWND localHwnd)
{
	Trace("AVManager::IsLocalEffact iRoomId=%d  time = %I64d \n",iRoomId,GetTickCount());
	return g_AVEngine.IsVideoDecoderEffact(iRoomId,localHwnd);
}

void  AVManager::FinishVideoParam(int iDeviceID)
{
	Trace("AVManager::FinishVideoParam  time = %I64d \n",GetTickCount());
	std::map<UInt32, RCPtr<VideoInputEngine> >::iterator iter;
	if((iter=VideoInputEngine::m_VInputEngineMap.find(iDeviceID))!=VideoInputEngine::m_VInputEngineMap.end())
	{
		iter->second->StopCapture();
		iter->second->RemoveAppendDevice(iDeviceID);
		Trace("AVManager::FinishVideoParam StopCapture  time = %I64d \n",GetTickCount());
	}
}

void  AVManager::GetVideoParam(const UInt32 &iDeviceID, UInt32 &iWidth, UInt32 &iHeight, UInt32 &iQuality)
{
	Trace("AVManager::GetVideoParam  time = %I64d \n",GetTickCount());
	std::map<UInt32, RCPtr<VideoInputEngine> >::iterator iter;
	if((iter=VideoInputEngine::m_VInputEngineMap.find(iDeviceID))!=VideoInputEngine::m_VInputEngineMap.end())
	{
		iWidth = iter->second->GetWidth();
		iHeight = iter->second->GetHeight();
		iQuality = iter->second->GetQuality();
	}
}

bool  AVManager::RecvRoomAudioOpp(__int64 lSymbolTime, UInt32 iRoomId, UInt32 iFromUin, UInt8& cLogicID)
{
	Trace("AVManager::RecvRoomAudioOpp  time = %I64d \n",GetTickCount());     ////////////////////////////////////?????查
	RCPtr<AVChannel>  pChannel(new AudioChannel(iRoomId,iFromUin, cLogicID));

	if (!pChannel->Start(lSymbolTime))
	{
		Assert(0);
		return false;
	}

	if (!AVChannelMgr::AddAVChannel(pChannel))
	{
		Assert(0);
		return false;
	}

	cLogicID = pChannel->GetLogicID();
	g_AVEngine.AddAudioLogicOutput(iRoomId,iFromUin);

	Trace("开始接收房间音频[iRoomId = %d, iDesUin = %d, cLogicID = %d] time = %I64d\n", iRoomId, iFromUin, cLogicID,GetTickCount());
	return true;
}

void AVManager::CloseRoomAudioOpp(UInt32 iRoomId, UInt32 iFromUin, UInt8& cLogicID)
{
	Trace("停止音频[iRoomId = %d, cLogicID = %d, iFromUin=%d] time = %I64d\n", iRoomId, cLogicID, iFromUin,GetTickCount());
	RCPtr<AVChannel> pChannel = AVChannelMgr::GetAVChannel(iRoomId, cLogicID);
	if (pChannel)
	{
		pChannel->UserCanncel();
	}

	g_AVEngine.DelAudioLogicOutput(iRoomId,iFromUin);

}

bool AVManager::RecvRoomVideoOpp(__int64 lSymbolTime, UInt32 iRoomId, UInt32 iFromUin, UInt8& cLogicID,HWND partnerHwnd, UInt32 iWidth, UInt32 iHeight, UInt32 iQuality,UInt32 iVideoMode,enuVIDEO_TYPE enType/*=VIDEO_PRIV_MIC*/)
{
	Trace("AVManager::RecvRoomVideoOpp  time = %I64d \n",GetTickCount());

	RCPtr<AVChannel> pChannel(new VideoChannel(iRoomId, iFromUin, cLogicID, partnerHwnd, iWidth, iHeight, iQuality));

	if (!pChannel->Start(lSymbolTime))
	{
		Assert(0);
		return false;
	}

	if (!AVChannelMgr::AddAVChannel(pChannel))
	{
		Assert(0);
		return false;
	}

	cLogicID = pChannel->GetLogicID();

	Trace("开始接收房间视频[iRoomId = %d, iDesUin = %d, cLogicID = %d] time = %I64d \n", iFromUin, cLogicID,GetTickCount());

	g_AVEngine.AddVideoPartnerHandle(iRoomId,iFromUin,partnerHwnd,iWidth, iHeight, iQuality,iVideoMode,enType);

	return true;
}

void AVManager::CloseRoomVideoOpp(UInt32 iRoomId, UInt32 iFromUin, UInt8& cLogicID,HWND partnerHwnd)
{
	Trace("停止视频[iRoomId = %d, cLogicID = %d, iFromUin=%d] time = %I64d\n", iRoomId, cLogicID, iFromUin,GetTickCount());
	RCPtr<AVChannel> pChannel = AVChannelMgr::GetAVChannel(iRoomId, cLogicID);
	if (pChannel)
	{
		pChannel->UserCanncel();
	}

	RCPtr<VideoOutputEngine> pVOutEngine = VideoOutputEngine::FindOutputEngine(iRoomId);
	if(pVOutEngine)
	{
		pVOutEngine->DelPartnerHwnd(iFromUin,partnerHwnd);
	}
}

void  AVManager::PauseLocalVideo(UInt32 iRoomId, HWND selfHwnd)
{
	Trace("AVManager::PauseLocalVideo iRoomId=%d time = %I64d \n",iRoomId,GetTickCount());
	if (NULL!=selfHwnd)
	{
		std::map<UInt32, RCPtr<VideoInputEngine> >::iterator iter;
		if((iter=VideoInputEngine::m_VideoInputEngineRoomMap.find(iRoomId))!=VideoInputEngine::m_VideoInputEngineRoomMap.end())
		{
			iter->second->PauseLocalHwnd(selfHwnd);
		}
	}
	else
	{
		Assert(0);
	}
}

void  AVManager::ContinueLocalVideo(UInt32 iRoomId, HWND selfHwnd)
{
	Trace("AVManager::ContinueLocalVideo iRoomId=%d time = %I64d \n",iRoomId,GetTickCount());
	if (NULL!=selfHwnd)
	{
		std::map<UInt32, RCPtr<VideoInputEngine> >::iterator iter;
		if((iter=VideoInputEngine::m_VideoInputEngineRoomMap.find(iRoomId))!=VideoInputEngine::m_VideoInputEngineRoomMap.end())
		{
			iter->second->ContinueLocalHwnd(selfHwnd);
		}
	}
	else
	{
		Assert(0);
	}
}

void AVManager::PauseLocalAudio(UInt32 iRoomId)
{
	Trace("AVManager::PauseLocalAudio iRoomId=%d time = %I64d \n",iRoomId,GetTickCount());
	g_AVEngine.PauseRoomAudio(iRoomId);
}

void AVManager::ContinueLocalAudio(UInt32 iRoomId)
{
	Trace("AVManager::ContinueLocalAudio iRoomId=%d time = %I64d \n",iRoomId,GetTickCount());
	g_AVEngine.ContinueRoomAudio(iRoomId);
}


void  AVManager::PauseRemoteVideo(UInt32 iRoomId,UInt32 iFromUin, HWND partnerHwnd)
{
	Trace("AVManager::PauseRemoteVideo iRoomId=%d iFromUin=%d time = %I64d \n",iRoomId,iFromUin,GetTickCount());
	if (NULL!=partnerHwnd)
	{
		RCPtr<VideoOutputEngine> pVOutEngine = VideoOutputEngine::FindOutputEngine(iRoomId);
		if(pVOutEngine)
		{
			pVOutEngine->PausePartnerHwnd(iFromUin,partnerHwnd);
		}
	}
	else
	{
		Assert(0);
	}
}

void  AVManager::ContinueRemoteVideo(UInt32 iRoomId,UInt32 iFromUin, HWND partnerHwnd)
{
	Trace("AVManager::ContinueRemoteVideo iRoomId=%d iFromUin=%d time = %I64d \n",iRoomId,iFromUin,GetTickCount());
	if (NULL!=partnerHwnd)
	{
		RCPtr<VideoOutputEngine> pVOutEngine = VideoOutputEngine::FindOutputEngine(iRoomId);
		if(pVOutEngine)
		{
			pVOutEngine->ContinuePartnerHwnd(iFromUin,partnerHwnd);
		}
	}
	else
	{
		Assert(0);
	}
}

void AVManager::PauseRemoteAudio(UInt32 iRoomId, UInt32 iFromUin)
{
	Trace("AVManager::PauseRemoteAudio iRoomId=%d iFromUin=%d time = %I64d \n",iRoomId,iFromUin,GetTickCount());
	g_AVEngine.PauseRemoteAudio(iRoomId,iFromUin);
}

void AVManager::ContinueRemoteAudio(UInt32 iRoomId, UInt32 iFromUin)
{
	Trace("AVManager::ContinueRemoteAudio iRoomId=%d iFromUin=%d time = %I64d \n",iRoomId,iFromUin,GetTickCount());
	g_AVEngine.ContinueRemoteAudio(iRoomId,iFromUin);
}

bool AVManager::IsPhotoableLocalHwnd(UInt32 iRoomId, HWND localHwnd)
{
	Trace("AVManager::IsPhotoableLocalHwnd iRoomId=%d  time = %I64d \n",iRoomId,GetTickCount());
	RCPtr<VideoInputEngine> pVideoInputEngine =VideoInputEngine::FindAppendRoom(iRoomId); 
	if (pVideoInputEngine)
	{
		return pVideoInputEngine->IsPhotoableLocalHwnd(localHwnd);
	}
	return false;
}

bool AVManager::IsPhotoableRemoteHwnd(UInt32 iRoomId, UInt32 iFromUin, HWND partnerHwnd)
{
	Trace("AVManager::IsPhotoableRemoteHwnd iRoomId=%d iFromUin=%d time = %I64d \n",iRoomId,iFromUin,GetTickCount());
	RCPtr<VideoOutputEngine> pVideoOutputEngine = VideoOutputEngine::FindOutputEngine(iRoomId);
	if (pVideoOutputEngine)
	{
		return pVideoOutputEngine->IsPhotoablePartnerHwnd(iFromUin, partnerHwnd);
	}
	return false;
}

bool  AVManager::IsStartAVEngine(UInt32 iRoomId)
{
	Trace("AVManager::IsStartAVEngine iRoomId=%d time = %I64d \n",iRoomId,GetTickCount());
	return g_AVEngine.IsStartAVInputEngine(iRoomId);
}

bool AVManager::IsVideoVirtualDevice(int iDevId)
{
	Trace("AVManager::IsVideoVirtualDevice iDevId=%d time = %I64d \n",iDevId,GetTickCount());
	return g_AVEngine.IsVideoVirtualDevice(iDevId);
}

void  AVManager::SetSpeakerVolume(UInt32 iRoomId,UInt32 iVolume)
{
	g_AVEngine.SetSpeakVolume(iRoomId,iVolume);
}

UInt32  AVManager::GetSpeakerVolume()
{
	return g_audioOutputEngine.GetVolumeOutput();
}

//获取指定房间音量
UInt32 AVManager::GetSpeakerRoomVolume(UInt32 iRoomId)
{
	return g_AVEngine.GetSpeakerRoomVolume(iRoomId);
}

//设置指定房间音量
void AVManager::SetSpeakerRoomVolume(UInt32 iRoomId,UInt32 iVolume)
{
	g_AVEngine.SetSpeakerRoomVolume(iRoomId,iVolume);
}

void AVManager::SetSysPlayerVolume(UInt32 iRoomId,UInt32 iVolume)
{
	g_AVEngine.SetSysPlayerVolume(iRoomId,iVolume);
}

UInt32 AVManager::GetSysPlayerVolume()
{
	return g_audioOutputEngine.GetSysvolume();
}

void AVManager::SetMicRoomAllVolume(UInt32 iVolume)
{
	g_AVEngine.SetMicRoomAllVolume(iVolume);
}

UInt32 AVManager::GetMicRoomAllVolume()
{
	return g_AVEngine.GetMicRoomAllVolume();
}

void  AVManager::SetMicVolume(UInt32 iRoomId, UInt32 iVolume)
{
	return g_AVEngine.SetMicVolume(iRoomId,iVolume);
}

UInt32  AVManager::GetMicVolume()
{
	return g_audioInputEngine.GetVolumeInput();
}

void  AVManager::CheckSoundDevice(bool bOpen)
{
	g_AVEngine.TestAudio(bOpen);
}

void  AVManager::GetSoundLevel(UInt32 &iOutput, UInt32 &iInput)
{
	iOutput = g_audioOutputEngine.GetOutputLevel();
	iInput = g_audioInputEngine.GetInputLevel();
}

bool  AVManager::SetSoundDevices(int iRecordIndex, int iPlayIndex)
{
	g_audioInputEngine.SetAudioDevice(iRecordIndex);
	g_audioOutputEngine.SetAudioOutputDevice(iPlayIndex);
	return true;
}

bool AVManager::SetMicInputMute(UInt32 iRoomId, bool bMute)
{
	return g_audioInputEngine.SetMicInputMute(bMute);
}

bool AVManager::GetMicInputMute(UInt32 iRoomId, bool& bMute)
{
	return g_audioInputEngine.GetMicInputMute(bMute);
}

bool AVManager::SetRoomSoundMute(UInt32 iRoomId, bool bMute)
{
	return g_AVEngine.SetRoomSoundMute(iRoomId,bMute);
}

bool AVManager::GetRoomSoundMute(UInt32 iRoomId, bool& bMute)
{
	return g_AVEngine.GetRoomSoundMute(iRoomId,bMute);
}


bool AVManager::SetAllRoomSoundMute(bool bMute)
{
	return g_AVEngine.SetAllRoomSoundMute(bMute);
}

bool AVManager::GetAllRoomSoundMute(bool& bMute)
{
	return g_AVEngine.GetAllRoomSoundMute(bMute);
}

bool AVManager::GetAllRoomMicMute(bool& bMute)
{
	return g_AVEngine.GetAllRoomMicMute(bMute);
}

bool AVManager::SetAllRoomMicMute(bool bMute)
{
	return g_AVEngine.SetAllRoomMicMute(bMute);
}

bool AVManager::SetSysPlayerMute(UInt32 iRoomId, bool bMute)
{
	return g_audioOutputEngine.SetSysPlayerMute(bMute);
}

bool AVManager::GetSysPlayerMute(UInt32 iRoomId, bool& bMute)
{
	return g_audioOutputEngine.GetSysPlayerMute(bMute);
}

#include <xstring>
#include <vector>

void  AVManager::GetAudioDeviceList(int &iDeviceNum, wchar_t pDeviceName[][DEVICE_NAME_LEN], bool bInput)
{
	std::vector<wstring>	vdevice;
	if (bInput)
	{
		vdevice = g_audioInputEngine.GetAudioInputDevice();
	}
	else
	{
		vdevice = g_audioOutputEngine.GetAudioOutputDevice();
	}

	iDeviceNum = vdevice.size();

	if (iDeviceNum > MAX_AUDIO_DEVICE_NUM)
	{
		iDeviceNum = MAX_AUDIO_DEVICE_NUM;
	}
	wstring strDevice;
	for (int i=0; i<iDeviceNum; i++)
	{
		strDevice =  vdevice[i];
		::wcsncpy_s(pDeviceName[i], DEVICE_NAME_LEN,strDevice.c_str(),min(strDevice.length(),DEVICE_NAME_LEN-1));
		if (strDevice.length()>=DEVICE_NAME_LEN)
		{
			pDeviceName[i][DEVICE_NAME_LEN-1] = L'\0';
		}
	}
}

void  AVManager::GetVideoDeviceList(int &iDeviceNum, wchar_t pDeviceName[][DEVICE_NAME_LEN])
{
	std::vector<std::wstring> vDeviceName;
	VideoInputEngine::EnumVideoDevices(vDeviceName);

	iDeviceNum = (int)vDeviceName.size();
	if (0!=iDeviceNum)
	{
		if (iDeviceNum>MAX_VIDEO_DEVICE_NUM) iDeviceNum=MAX_VIDEO_DEVICE_NUM;

		for (int i=0; i<iDeviceNum; ++i)
		{
			::wcsncpy_s(&pDeviceName[i][0], DEVICE_NAME_LEN, vDeviceName[i].c_str(), min(vDeviceName[i].length(),DEVICE_NAME_LEN-1));
			if (vDeviceName[i].length()>=DEVICE_NAME_LEN)
			{
				pDeviceName[i][DEVICE_NAME_LEN-1] = L'\0';
			}
		}
	}
}

void AVManager::GetVideoDeviceInfoList(int &iDeviceNum,Service_Video_Info struDeviceInfo[])
{
	VideoInputEngine::EnumVideoDeviceInfo(iDeviceNum,struDeviceInfo);
}

bool  AVManager::ShowVideoAdjustDlg(HWND hParWnd, UInt32 iDeviceID)
{
	return VideoInputEngine::ShowVideoAdjustDlg(hParWnd, iDeviceID);
}

void  AVManager::OperateVideoEngine()
{
	if (g_AVEngine.m_bStopVideoEngine)
	{
		Trace("AVManager::OperateVideoEngine()...关闭没有使用的视频设备\n");
		RCPtr<VideoInputEngine> pVideoInputEngine =VideoInputEngine::FindAppendRoom(g_AVEngine.m_iStopRoomId); 
		if (pVideoInputEngine && 
			!VideoInputEngine::IsDeviceUsed(pVideoInputEngine->GetDeviceID()) &&
			!pVideoInputEngine->IsExistsUsed())
		{
			pVideoInputEngine->StopCapture();
		}
		g_AVEngine.m_bStopVideoEngine = false;
		g_AVEngine.m_iStopRoomId=0;
	}
}

bool  AVManager::GetNumOfSoundDevices(int &iPlayNum, int &iRecordNum)
{
	iPlayNum = g_audioOutputEngine.GetAudioOutputDevice().size();
	iRecordNum = g_audioInputEngine.GetAudioInputDevice().size();
	return true;
}

void AVManager::SafeAudioInputStop()
{
	g_AVEngine.SafeAudioInputStop();
}

bool  AVManager::GetRecordDeviceName(int iIndex, wchar_t *pDeviceName, int iNameLen)
{
	if (iIndex >=  g_audioInputEngine.GetAudioInputDevice().size())
	{
		pDeviceName = NULL;
		iNameLen = 0;
		return false;
	}

	wstring strDeviceName = g_audioInputEngine.GetAudioInputDevice()[iIndex];
	wcsncpy_s(pDeviceName,iNameLen,strDeviceName.c_str(),min(strDeviceName.length(),iNameLen-1));
	return true;
}

bool  AVManager::GetPlayoutDeviceName(int iIndex, wchar_t *pDeviceName, int iNameLen)
{
	if (iIndex >=  g_audioOutputEngine.GetAudioOutputDevice().size())
	{
		pDeviceName = NULL;
		iNameLen = 0;
		return false;
	}
	wstring strDeviceName = g_audioOutputEngine.GetAudioOutputDevice()[iIndex];
	wcsncpy_s(pDeviceName,iNameLen,strDeviceName.c_str(),min(strDeviceName.length(),iNameLen-1));
	return true;
}

bool AVManager::StartVideoTest(int iDeviceId,HWND hWnd,UInt32& iResult)
{
	return g_AVEngine.StartVideoTest(iDeviceId,hWnd,iResult);
}

bool AVManager::CloseVideoTest(int iDeviceId,HWND hWnd)
{
	return g_AVEngine.CloseVideoTest(iDeviceId,hWnd);
}

float AVManager::GetNatLostRate()
{
	return g_AVEngine.GetNatLostRate();
}

bool AVManager::GetMicGreater(bool& bEnable)
{
	return g_AVEngine.GetMicGreater(bEnable);
}

bool AVManager::SetMicGreater(bool bEnable)
{
	return g_AVEngine.SetMicGreater(bEnable);
}

bool AVManager::GetFFTView(UInt32 iRoomId,UInt32 iFromUin)
{
	return g_AVEngine.GetShowFFTWav(iRoomId,iFromUin);
}

void AVManager::SetFFTView(UInt32 iRoomId,UInt32 iFromUin,bool bEnable)
{
	g_AVEngine.SetShowFFTWav(iRoomId,iFromUin,bEnable);
}

void AVManager::SetFFTALL(bool bEnable)
{
	g_AVEngine.SetFFTALL(bEnable);
}

void AVManager::GetFFTALL(bool& bEnable)
{
	g_AVEngine.GetFFTALL(bEnable);
}

void AVManager::SetFFTME(bool bEnable)
{
	g_AVEngine.SetFFTME(bEnable);
}

void AVManager::GetFFTME(bool& bEnable)
{
	g_AVEngine.GetFFTME(bEnable);
}

bool AVManager::flushVideoDev(unsigned int iDev)
{
	return g_AVEngine.flushVideoDev(iDev);
}
bool AVManager::SetAudioMode(enuROOM_AV_AUDIO_MODE mode)
{
	return g_AVEngine.SetMode(mode);
}

bool AVManager::SetAvKey(unsigned int iRoomId,unsigned int iKey)
{
	RelayManger* pRelayManager = RelayManger::Instance();
	if (pRelayManager){
		pRelayManager->SetAVKey(iRoomId,iKey);
		return true;
	}else{
		AssertVS(0,iRoomId,"AVManager::SetAvKey fail pRelayManager=NULL");
		return false;
	}
}

Enum_Camera_Status AVManager::GetCamerStatus(int iDevice)
{
	return g_AVEngine.GetCamerStatus(iDevice);
}

bool AVManager::GetAVNetStatus(Service_Av_Net_Status_All_Ex& sa)
{
	return g_AVEngine.GetAVNetStatus(sa.iRoomId,sa.sansa);
}

bool AVManager::SetVideoSharpen(bool bEnable)
{
	g_struct.bEnableSharpen = bEnable;
	return true;
}