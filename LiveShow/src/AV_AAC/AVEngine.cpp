#include "stdafx.h"
#include "AVEngine.h"
#include "VideoInputEngine.h"
#include "VideoOutputEngine.h"
#include "RelayManger.h"
#include "AudioPacket.h"
#include "VideoPacket.h"
#include "AVModule.h"
#include "Global.h"
#include "AudioInputEngine.h"
#include "AudioOutputEngine.h"
#include "AV.h"
#include "ILogicListener.h"
#include "ChannelManager.h"
#include "ClientUser.h"
#include "VideoRateControl.h"
#include "Mixer/MixerManager.h"

AVEngine       g_AVEngine;
std::vector<UInt8> AVEngine::ST_AUDIO_PROPERTY::m_sVecLogicChannel;

UInt64 MakeUserSession(UInt32 iRoomId,UInt32 iFromUin)
{
	UInt64 ret = iRoomId;
	return (ret<<32)+iFromUin;
}

int PrepareData(UInt64 luin,byte* pData,unsigned int iLen)
{
	UInt64 lui = luin;
	UInt32 iRoomId = lui>>32;
	UInt32 iFromUin = (lui<<32)>>32;
	g_AVEngine.PrepareWavData(iRoomId,iFromUin,pData,iLen);
	return 0;
}

bool GetRoomAudioStatus(unsigned int iRoomId)
{	
	return g_AVEngine.IsRoomAudioPause(iRoomId);
}

AVEngine::AVEngine()
 :m_bStopVideoEngine(false),
  m_iStopRoomId(0),
  m_bRoomMux(false)
{
	Trace("AVEngine::AVEngine()初始化...  time = %I64d \n",GetTickCount());
}

AVEngine::~AVEngine()
{
	Trace("AVEngine::~AVEngine()销毁...  time = %I64d \n",GetTickCount());
}

bool  AVEngine::InitAVEngine()
{
	Trace("AVEngine::InitAVEngine()...  time = %I64d \n",GetTickCount());
	RCPtr<VideoInputEngine> pVInEngine = VideoInputEngine::Create();
	if (!pVInEngine->IsInit())
	{
		pVInEngine->Init();
	}
	g_audioInputEngine.Init(this);
	g_audioOutputEngine.Init(this); 
	
	return true;
}

bool AVEngine::InitVideoEngine(UInt32 iDeviceId)
{
	Trace("AVEngine::InitVideoEngine().iDeviceId=%d..  time = %I64d \n",iDeviceId,GetTickCount());
	RCPtr<VideoInputEngine> pVInEngine = VideoInputEngine::Create(iDeviceId);
	if (!pVInEngine->IsInit())
	{
		pVInEngine->Init(320,240,26,iDeviceId);
	}
	return true;
}

void  AVEngine::DestroyAVEngine()
{
	Trace("AVEngine::DestroyAVEngine()  time = %I64d \n",GetTickCount());
	std::map<UInt32, RCPtr<VideoInputEngine> >::iterator iter;
	for( iter=VideoInputEngine::m_VInputEngineMap.begin(); iter!=VideoInputEngine::m_VInputEngineMap.end();++iter)
	{
		iter->second->Destroy();
	}
	Trace("AVEngine::DestroyAVInputEngine() m_VInputEngineMap.size()=%d time = %I64d \n",VideoInputEngine::m_VInputEngineMap.size(),GetTickCount());
	std::map<UInt32, RCPtr<VideoOutputEngine> > ::iterator itera;
	for (itera=VideoOutputEngine::m_VOutputEngineMap.begin();itera!=VideoOutputEngine::m_VOutputEngineMap.end();++itera)
	{
		itera->second->Destroy();
	}
	Trace("AVEngine::DestroyAVInputEngine() m_VOutputEngineMap.size()=%d time = %I64d \n",VideoOutputEngine::m_VOutputEngineMap.size(),GetTickCount());
}

void   AVEngine::DestroyAVInputEngine(UInt32 iRoomId) //================================================haosl 销毁的问题
{
	Trace("AVEngine::DestroyAVInputEngine()  time = %I64d \n",GetTickCount());
	std::map<UInt32, RCPtr<VideoInputEngine> >::iterator iter;
	if( (iter=VideoInputEngine::m_VInputEngineMap.find(iRoomId)) != VideoInputEngine::m_VInputEngineMap.end())
	{
		iter->second->Destroy();
	}
	Trace("AVEngine::DestroyAVInputEngine() m_VInputEngineMap.size()=%d time = %I64d \n",VideoInputEngine::m_VInputEngineMap.size(),GetTickCount());
}

void   AVEngine::DestroyAVOutputEngine(UInt32 iRoomId)
{
	Trace("AVEngine::DestroyAVOutputEngine() iRoomId=%d time = %I64d \n",iRoomId,GetTickCount());
	std::map<UInt32, RCPtr<VideoOutputEngine> > ::iterator iter;
	if ( (iter=VideoOutputEngine::m_VOutputEngineMap.find(iRoomId))!=VideoOutputEngine::m_VOutputEngineMap.end())
	{
		iter->second->Destroy();
	}
	Trace("AVEngine::DestroyAVOutputEngine() m_VOutputEngineMap.size()=%d time = %I64d \n",VideoOutputEngine::m_VOutputEngineMap.size(),GetTickCount());
}

//判断语音设备是否开启状态，只要有一个房间在使用音频 视频设备就算开启
bool AVEngine::IsStartAVEngine()
{
	Trace("AVEngine::IsStartAVEngine()  time = %I64d \n",GetTickCount());
	if (g_audioInputEngine.IsStart())
	{
		return true;
	}
	std::map<UInt32, RCPtr<VideoInputEngine> >::iterator iter;
	for( iter=VideoInputEngine::m_VInputEngineMap.begin(); iter!=VideoInputEngine::m_VInputEngineMap.end();++iter)
	{
		if (iter->second->IsStart())
		{
			return true;
		}
	}
	return false;
}

bool AVEngine::IsStartAVInputEngine(SInt32 iRoomId)
{
	Trace("AVEngine::IsStartAVInputEngine() iRoomId=%d time = %I64d \n",iRoomId,GetTickCount());
	if (m_allAudioLogicChannel.end()!=m_allAudioLogicChannel.find(iRoomId))
	{
		return true;
	}
	std::map<UInt32, RCPtr<VideoInputEngine> >::iterator iter;
	if( (iter=VideoInputEngine::m_VInputEngineMap.find(iRoomId)) != VideoInputEngine::m_VInputEngineMap.end())
	{
		return iter->second->IsStart();
	}
	return false;
}

bool AVEngine::IsVideoDevInit(UInt32 iDeviceId)
{
	Trace("AVEngine::IsVideoDevInit() iDeviceId=%d time = %I64d \n",iDeviceId,GetTickCount());
	return VideoInputEngine::FindInputEngine(iDeviceId);
}

bool AVEngine::flushVideoDev(UInt32 iDeviceId)
{
	Trace("AVEngine::flushVideoDev() iDeviceId=%d time = %I64d \n",iDeviceId,GetTickCount());
	UInt32 iResult;
	RCPtr<VideoInputEngine> pVInput(NULL);
	if (pVInput = VideoInputEngine::FindInputEngine(iDeviceId))
	{
		if (!pVInput->IsStart())
		{
			pVInput->StartCapture(iResult);
			pVInput->StopCapture(FALSE);
		}
	}
	else
	{
		InitVideoEngine(iDeviceId);
		pVInput = VideoInputEngine::FindInputEngine(iDeviceId);
		pVInput->StartCapture(iResult);
		pVInput->StopCapture(FALSE);
	}
	return true;
}

void  AVEngine::AddAudioLogicChannel(UInt32 iRoomId, UInt8 cLogicID)
{	
	Trace("AVEngine::AddAudioLogicChannel() iRoomId=%d time = %I64d \n",iRoomId,GetTickCount());
	MutexLocker locker(&m_AudioMutex);//m_allAudioLogicChannel 房间线程
	if (m_allAudioLogicChannel.end()==m_allAudioLogicChannel.find(iRoomId)) //m_allAudioLogicChannel 为发送语音服务  区分每个房间有多少逻辑语音Channel
	{	
		struct ST_AUDIO_PROPERTY stAudioProperty(cLogicID);
		m_allAudioLogicChannel[iRoomId]=stAudioProperty;
	}
	else
	{
		m_allAudioLogicChannel[iRoomId].AddLogicChannel(cLogicID);
	}

	if (!g_audioInputEngine.IsStart())
	{
		g_audioInputEngine.Start();
	}

	if (!g_audioOutputEngine.IsStart())
	{
		g_audioOutputEngine.Start();
	}
}

void AVEngine::PauseRoomAudio(UInt32 iRoomId)
{	
	Trace("AVEngine::PauseRoomAudio() iRoomId=%d time = %I64d \n",iRoomId,GetTickCount());
	MutexLocker locker(&m_AudioMutex);//m_allAudioLogicChannel 房间线程
	if (m_allAudioLogicChannel.end()!=m_allAudioLogicChannel.find(iRoomId)) 
		m_allAudioLogicChannel.find(iRoomId)->second.PauseAudio();
}

bool AVEngine::IsRoomAudioPause(UInt32 iRoomId)
{
	//Trace("AVEngine::IsRoomAudioPause() iRoomId=%d time = %I64d \n",iRoomId,GetTickCount());
	MutexLocker locker(&m_AudioMutex);//m_allAudioLogicChannel 房间线程
	if (m_allAudioLogicChannel.end()!=m_allAudioLogicChannel.find(iRoomId)) 
		return m_allAudioLogicChannel.find(iRoomId)->second.IsPaused();
	return true;
}

void AVEngine::ContinueRoomAudio(UInt32 iRoomId)
{
	Trace("AVEngine::ContinueRoomAudio() iRoomId=%d time = %I64d \n",iRoomId,GetTickCount());
	MutexLocker locker(&m_AudioMutex);//m_allAudioLogicChannel 房间线程
	if (m_allAudioLogicChannel.end()!=m_allAudioLogicChannel.find(iRoomId)) 
		m_allAudioLogicChannel.find(iRoomId)->second.ContinueAudio();
}

void AVEngine::PauseRemoteAudio(UInt32 iRoomId,UINT32 iFromUin)
{
	Trace("AVEngine::PauseRemoteAudio() iRoomId=%d iFromUin=%d time = %I64d \n",iRoomId,iFromUin,GetTickCount());
	g_audioOutputEngine.PauseUserSession(MakeUserSession(iRoomId,iFromUin),true);
}

void AVEngine::ContinueRemoteAudio(UInt32 iRoomId,UINT32 iFromUin)
{
	Trace("AVEngine::PauseRemoteAudio() iRoomId=%d iFromUin=%d time = %I64d \n",iRoomId,iFromUin,GetTickCount());
	g_audioOutputEngine.PauseUserSession(MakeUserSession(iRoomId,iFromUin),false);
}

void  AVEngine::DelAudioLogicChannel(UInt32 iRoomId, UInt8 cLogicID)
{
	Trace("AVEngine::DelAudioLogicChannel() begin iRoomId=%d time = %I64d \n",iRoomId,GetTickCount());
	{
		MutexLocker locker(&m_AudioMutex);//m_allAudioLogicChannel 房间线程
		std::map<UInt32, ST_AUDIO_PROPERTY>::iterator iter = m_allAudioLogicChannel.find(iRoomId);
		if (iter != m_allAudioLogicChannel.end())
		{
			iter->second.RemoveLogicChannel(cLogicID);
			if (iter->second.IsLogicEmpty())
			{
				m_allAudioLogicChannel.erase(iter);
			}
		}
	}
	if (m_allAudioLogicChannel.empty()) //没有房间在使用语音设备
	{
		g_audioInputEngine.Stop();
	}
	Trace("AVEngine::DelAudioLogicChannel() end iRoomId=%d time = %I64d \n",iRoomId,GetTickCount());
}

void AVEngine::SafeAudioInputStop()
{
	Trace("AVEngine::SafeAudioInputStop() time = %I64d \n",GetTickCount());
	MutexLocker locker(&m_AudioMutex);//m_allAudioLogicChannel 房间线程
	if (m_allAudioLogicChannel.empty()) //没有房间在使用语音设备
	{
		g_audioInputEngine.Stop();
	}
	if (ClientUser::c_qmUsers.empty())
	{
		g_audioOutputEngine.Stop();
	}
}

void AVEngine::ClearAudioLogicChannel(UInt32 iRoomId)
{
	Trace("AVEngine::ClearAudioLogicChannel() iRoomId=%d time = %I64d \n",iRoomId,GetTickCount());
	MutexLocker locker(&m_AudioMutex);//m_allAudioLogicChannel 房间线程
	std::map<UInt32, ST_AUDIO_PROPERTY>::iterator iter = m_allAudioLogicChannel.find(iRoomId);
	if (iter != m_allAudioLogicChannel.end())
	{
		iter->second.RemoveAllLogicChannel();
		m_allAudioLogicChannel.erase(iter);
	}	
	if (m_allAudioLogicChannel.empty()) //没有房间在使用语音设备
	{
		g_audioInputEngine.Stop();
	}
}

void AVEngine::ClearAudioLogicChannelAll()
{
	Trace("AVEngine::ClearAudioLogicChannelAll() time = %I64d \n",GetTickCount());
	MutexLocker locker(&m_AudioMutex);//m_allAudioLogicChannel 房间线程
	std::map<UInt32, ST_AUDIO_PROPERTY>::iterator iter;
	for (iter=m_allAudioLogicChannel.begin();iter!=m_allAudioLogicChannel.end();)
	{
		iter->second.RemoveAllLogicChannel();
		iter = m_allAudioLogicChannel.erase(iter);
	}
	m_allAudioLogicChannel.clear();
	g_audioInputEngine.Stop();
	Trace("AVEngine::ClearAudioLogicChannelAll() m_allAudioLogicChannel.size()=%d time = %I64d \n",m_allAudioLogicChannel.size(),GetTickCount());
}

void AVEngine::ClearAudioLogicOutput(UInt32 iRoomId)
{
	Trace("AVEngine::ClearAudioLogicOutput() time = %I64d \n",GetTickCount());
	g_audioOutputEngine.RemoveRoomSession(iRoomId);
}

void AVEngine::ClearAudioLogicOutputAll()
{
	Trace("AVEngine::ClearAudioLogicOutputAll() time = %I64d \n",GetTickCount());
	g_audioOutputEngine.RemoveRoomAllSession();
}

void AVEngine::AddAudioLogicOutput(UInt32 iRoomId,UInt32 iFromUin)
{
	Trace("AVEngine::AddAudioLogicOutput() time = %I64d \n",GetTickCount());
	if (!g_audioOutputEngine.IsStart())
	{
		g_audioOutputEngine.Start();
	}
	g_audioOutputEngine.AddUserSession(MakeUserSession(iRoomId,iFromUin));
}

void AVEngine::DelAudioLogicOutput(UInt32 iRoomId,UInt32 iFromUin)
{
	Trace("AVEngine::DelAudioLogicOutput() time = %I64d \n",GetTickCount());
	g_audioOutputEngine.RemoveUserSession(MakeUserSession(iRoomId,iFromUin));
}

void AVEngine::SetAudioDevice(UInt32 iInputDev,UInt32 iOutputDev)
{
	Trace("AVEngine::SetAudioDevice() iInputDev=%d iOutputDev=%d time = %I64d \n",iInputDev,iOutputDev,GetTickCount());
	g_audioInputEngine.SetAudioDevice(iInputDev);
	g_audioOutputEngine.SetAudioOutputDevice(iOutputDev);
}


//添加公麦或私麦显示摄像头窗口句柄  其他人
void AVEngine::AddVideoPartnerHandle(UInt32 iRoomId,UInt32 iDestUin,HWND partnerHwnd, UInt32 iWidth, UInt32 iHeight, SInt32 iQuality,UInt32 iVideoMode,enuVIDEO_TYPE enType)
{
	Trace("AVEngine::AddVideoPartnerHandle() iRoomId=%d iDestUin=%d time = %I64d \n",iRoomId,iDestUin,GetTickCount());
	if (NULL!=partnerHwnd)
	{
		g_struct.Init(partnerHwnd);
		g_struct.SetPrepareWavDataFun(PrepareData);
		g_struct.SetGetRoomStatusFun(GetRoomAudioStatus);
		RCPtr<VideoOutputEngine> pVOutEngine = VideoOutputEngine::Create(iRoomId);
		if(pVOutEngine)
		{
			pVOutEngine->AddPartnerHwnd(iDestUin,partnerHwnd,iWidth, iHeight, iQuality,iVideoMode,enType);
		}
	}
}

//更换显示窗口句柄
bool AVEngine::ChangePartnerHwnd(UInt32 iRoomId,UInt32 iDesUin, HWND hPartnerWnd, UInt32 iWidth, UInt32 iHeight, HWND hOldWnd)
{
	Trace("AVEngine::ChangePartnerHwnd() iRoomId=%d iDesUin=%d time = %I64d \n",iRoomId,iDesUin,GetTickCount());
	if (NULL!=hPartnerWnd)
	{
		RCPtr<VideoOutputEngine> pVideoOutputEngine = VideoOutputEngine::FindOutputEngine(iRoomId);
		if (pVideoOutputEngine)
		{
			return pVideoOutputEngine->ChangePartnerHwnd(iDesUin,hPartnerWnd,iWidth,iHeight,hOldWnd);
		}
	}
	return false;
}

//删除公麦或私麦显示摄像头窗口句柄  其他人
void AVEngine::DelVideoPartnerHandle(UInt32 iRoomId,UInt32 iDestUin, HWND partnerHwnd)
{
	Trace("AVEngine::DelVideoPartnerHandle() iRoomId=%d iDesUin=%d time = %I64d \n",iRoomId,iDestUin,GetTickCount());
	if (NULL!=partnerHwnd)
	{
		RCPtr<VideoOutputEngine> pVideoOutputEngine = VideoOutputEngine::FindOutputEngine(iRoomId);
		if (pVideoOutputEngine)
		{
			pVideoOutputEngine->DelPartnerHwnd(iDestUin, partnerHwnd);
		}
	}
}

void AVEngine::PrepareWavData(UInt32 iRoomId,UInt32 iFromUin,byte* pData,UInt32 iLen)
{
	Trace("AVEngine::PrepareWavData() iRoomId=%d iDesUin=%d time = %I64d \n",iRoomId,iFromUin,GetTickCount());
	RCPtr<VideoOutputEngine> pVideoOutputEngine = VideoOutputEngine::FindOutputEngine(iRoomId);
	if (pVideoOutputEngine)
	{
		pVideoOutputEngine->PrepareWav(iFromUin, pData,iLen);
	}
}

bool AVEngine::PaintLocal(UInt32 iRoomId,HWND hWnd)
{
	Trace("AVEngine::PaintLocal() iRoomId=%d time = %I64d \n",iRoomId,GetTickCount());
	std::map<UInt32,RCPtr<VideoInputEngine>>::iterator iter = VideoInputEngine::m_VideoInputEngineRoomMap.find(iRoomId);
	if (iter!=VideoInputEngine::m_VideoInputEngineRoomMap.end())
	{
		return iter->second->PaintLocal(hWnd);
	}
	else
	{
		return false;
	}
}

bool AVEngine::PaintRemote(UInt32 iRoomId,UInt32 iFromUin,HWND hWnd)
{
	Trace("AVEngine::PaintRemote() iRoomId=%d iDesUin=%d time = %I64d \n",iRoomId,iFromUin,GetTickCount());
	if (NULL!=hWnd)
	{
		RCPtr<VideoOutputEngine> pVideoOutputEngine = VideoOutputEngine::FindOutputEngine(iRoomId);
		if (pVideoOutputEngine)
		{
			return pVideoOutputEngine->PaintRemote(iFromUin, hWnd);
		}
	}
	return false;
}

//添加本地显示句柄 及启动房间摄像头
void  AVEngine::AddVideoLogicChannel(UInt32 iRoomId, HWND localHwnd,enuVIDEO_TYPE enType)
{
	Trace("AVEngine::AddVideoLogicChannel() iRoomId=%d time = %I64d \n",iRoomId,GetTickCount());
	g_struct.Init(localHwnd);
	g_struct.SetPrepareWavDataFun(PrepareData);
	g_struct.SetGetRoomStatusFun(GetRoomAudioStatus);
	//必须在主线程中启动capture，否则切换输入法僵死
	bool bStartCapture =false;
	bool bAdd = false;
	bool bFind = false;
	{
		MutexLocker locker(&m_VideoMutex); //房间线程 buffer线程  m_mapLocalHwndtoChannel  VideoOutputEngine::m_VOutputEngineMap

		if (NULL!=localHwnd)   //m_mapLocalHwndtoChannel 为发送视频对象服务  本地Handle和RoomId映射  发送时候将取本地
		{
			std::map<HWND,UInt32>::iterator iter; 
			for (iter=m_mapLocalHwndtoChannel.begin();iter!=m_mapLocalHwndtoChannel.end();++iter)
			{
				if (iRoomId == iter->second )
				{
					bFind = true;
					break;
				}
			}
			if (!bFind)
			{
				bStartCapture = true;  //仅仅启动房间的摄像头  房间开启的。
			}

			iter = m_mapLocalHwndtoChannel.find(localHwnd);
			if (iter==m_mapLocalHwndtoChannel.end())
			{
				bAdd = true;
			}
			m_mapLocalHwndtoChannel[localHwnd] = iRoomId;
		}
	}


	{
		RCPtr<VideoInputEngine> pVideoInputEngine = VideoInputEngine::FindAppendRoom(iRoomId);
		if (pVideoInputEngine)
		{
			pVideoInputEngine->SetType(enType);
		}
	}

	if (bAdd)
	{
		RCPtr<VideoInputEngine> pVideoInputEngine = VideoInputEngine::FindAppendRoom(iRoomId);
		if (pVideoInputEngine)
		{
			pVideoInputEngine->AddLocalHwnd(localHwnd,iRoomId,TRUE);
		}
	}

	if (bStartCapture)
	{
		UInt32 iResult = kResult_Success;
		RCPtr<VideoInputEngine> pVideoInputEngine = VideoInputEngine::FindAppendRoom(iRoomId);
		if (pVideoInputEngine && !pVideoInputEngine->IsStart())
		{
			if (!pVideoInputEngine->StartCapture(iResult))
			{
				//启动视频设备失败  和上层次交互
				Assert(0);
				AV_Video_Fail_Event* pVideoFailEvent = new AV_Video_Fail_Event(iRoomId, iResult);
				if (!g_pAVModule->sendEvent(static_cast<void*>(pVideoFailEvent), EVENT_AV_VIDEO_FAIL))
				{
					Assert(0);
					delete pVideoFailEvent;
				}
			}
		}
	}
}

bool AVEngine::SetVideoDecoderEffect(UInt32 iRoomId, HWND localHwnd,bool bEnable)
{
	Trace("AVEngine::SetVideoDecoderEffect() iRoomId=%d time = %I64d \n",iRoomId,GetTickCount());
	RCPtr<VideoInputEngine> pVideoInputEngine = VideoInputEngine::FindAppendRoom(iRoomId);
	if (pVideoInputEngine)
	{
		return pVideoInputEngine->SetVideoDecoder(localHwnd,bEnable);
	}
	return false;
}

bool AVEngine::IsVideoDecoderEffact(UInt32 iRoomId, HWND localHwnd)
{
	Trace("AVEngine::IsVideoDecoderEffact() iRoomId=%d time = %I64d \n",iRoomId,GetTickCount());
	RCPtr<VideoInputEngine> pVideoInputEngine = VideoInputEngine::FindAppendRoom(iRoomId);
	if (pVideoInputEngine)
	{
		return pVideoInputEngine->IsVideoDecoder(localHwnd);
	}
	return false;
}

bool AVEngine::ChangeLocalHwnd(UInt32 iRoomId, HWND hWnd, HWND hOldWnd)
{
	Trace("AVEngine::ChangeLocalHwnd() iRoomId=%d time = %I64d \n",iRoomId,GetTickCount());
	if (NULL!=hWnd)
	{
		MutexLocker locker(&m_VideoMutex); //房间线程 buffer线程  m_mapLocalHwndtoChannel  VideoOutputEngine::m_VOutputEngineMap
		std::map<HWND,UInt32>::iterator iter; 
		iter = m_mapLocalHwndtoChannel.find(hOldWnd);
		if (iter!=m_mapLocalHwndtoChannel.end())
		{
			m_mapLocalHwndtoChannel.erase(iter);
		}
		m_mapLocalHwndtoChannel[hWnd] = iRoomId;
	}
	
	RCPtr<VideoInputEngine> pVideoInputEngine = VideoInputEngine::FindAppendRoom(iRoomId);
	if (pVideoInputEngine)
	{
		pVideoInputEngine->ChangeLocalHwnd(hWnd,hOldWnd);
		return true;
	}
	return false;
}

//开启测试摄像头
bool AVEngine::StartVideoTest(int iDeviceId,HWND hWnd,UInt32& iResult)
{	
	ErrTrace("AVEngine::StartVideoTest() 本地视频测试打开设备iDeviceId=%d time = %I64d \n",iDeviceId,GetTickCount());
	MutexLocker locker(&m_VideoMutex); //房间线程 buffer线程  m_mapLocalHwndtoChannel  VideoOutputEngine::m_VOutputEngineMap
	RCPtr<VideoInputEngine> pVInput(NULL);
	if (pVInput = VideoInputEngine::FindInputEngine(iDeviceId))
	{
		pVInput->AddLocalHwnd(hWnd,0,FALSE); //测试不开启波形图预览
		pVInput->SetVideoDecoder(hWnd,FALSE);
		return pVInput->StartCapture(iResult);
	}
	Trace("AVEngine::StartVideoTest() end iDeviceId=%d time = %I64d \n",iDeviceId,GetTickCount());
	return false;
}

//关闭测试开启的摄像头
bool AVEngine::CloseVideoTest(int iDeviceId,HWND hWnd)
{
	Trace("AVEngine::CloseVideoTest() begin iDeviceId=%d time = %I64d \n",iDeviceId,GetTickCount());
	MutexLocker locker(&m_VideoMutex); //房间线程 buffer线程  m_mapLocalHwndtoChannel  VideoOutputEngine::m_VOutputEngineMap
	RCPtr<VideoInputEngine> pVInput(NULL);
	if (pVInput = VideoInputEngine::FindInputEngine(iDeviceId))
	{
		pVInput->DelLocalHwnd(hWnd);
		if (!VideoInputEngine::IsDeviceUsed(iDeviceId) && !pVInput->IsExistsUsed())
		{
			return pVInput->StopCapture();
		}
	}
	Trace("AVEngine::CloseVideoTest() end iDeviceId=%d time = %I64d \n",iDeviceId,GetTickCount());
	return false;
}

//删除本地显示摄像头 下麦
void  AVEngine::DelVideoLogicChannel(UInt32 iRoomId, HWND localHwnd) 
{
	Trace("AVEngine::DelVideoLogicChannel() begin iRoomId=%d time = %I64d \n",iRoomId,GetTickCount());
	MutexLocker locker(&m_VideoMutex); //房间线程 buffer线程  m_mapLocalHwndtoChannel  VideoOutputEngine::m_VOutputEngineMap
	bool bStopCapture =false;
	bool bFind = false;
	{
		CocTrace("AVEngine::DelVideoLogicChannel iRoomId=%d\n",iRoomId);
		if (NULL!=localHwnd)
		{
			CocTrace("AVEngine::DelVideoLogicChannel iRoomId=%d,m_mapLocalHwndtoChannel.size()=%d\n",iRoomId,m_mapLocalHwndtoChannel.size());
			std::map<HWND, UInt32 >::iterator iterHWND;
			iterHWND = m_mapLocalHwndtoChannel.find(localHwnd);
			if (iterHWND!=m_mapLocalHwndtoChannel.end())
			{
				m_mapLocalHwndtoChannel.erase(iterHWND);
			}
			
			CocTrace("AVEngine::DelVideoLogicChannel iRoomId=%d,m_mapLocalHwndtoChannel.size()=%d\n",iRoomId,m_mapLocalHwndtoChannel.size());
			std::map<HWND,UInt32>::iterator iter;
			for (iter=m_mapLocalHwndtoChannel.begin();iter!=m_mapLocalHwndtoChannel.end();++iter)
			{
				if (iRoomId == iter->second )
				{
					bFind = true;
					break;
				}
			}
			
			RCPtr<VideoInputEngine> pVideoInputEngine = VideoInputEngine::FindAppendRoom(iRoomId);
			if (pVideoInputEngine)
			{
				if (!bFind && !VideoInputEngine::IsDeviceUsed(pVideoInputEngine->GetDeviceID(),iRoomId) && !pVideoInputEngine->IsOtherUsed(localHwnd))
				{
					bStopCapture = true;//此时停止视频采集
					CocTrace("AVEngine::DelVideoLogicChannel iRoomId=%d,此时停止视频采集\n",iRoomId);
				}

			}
		}
	}
	{
		RCPtr<VideoInputEngine> pVideoInputEngine = VideoInputEngine::FindAppendRoom(iRoomId);
		if (pVideoInputEngine)
			pVideoInputEngine->DelLocalHwnd(localHwnd);
	}

	if (bStopCapture)
	{
		//由主线程来stop
		RCPtr<VideoInputEngine> pVideoInputEngine = VideoInputEngine::FindAppendRoom(iRoomId);
		if (pVideoInputEngine)
		{
			pVideoInputEngine->StopCapture();
			m_bStopVideoEngine = true;
			m_iStopRoomId=iRoomId;
			CocTrace("AVEngine::DelVideoLogicChannel iRoomId=%d,Stop\n",iRoomId);
		}
	}
	Trace("AVEngine::DelVideoLogicChannel() end iRoomId=%d time = %I64d \n",iRoomId,GetTickCount());
}

void AVEngine::DelVideoLogicChannelRoom(UInt32 iRoomId)
{
	Trace("AVEngine::DelVideoLogicChannelRoom() begin iRoomId=%d time = %I64d \n",iRoomId,GetTickCount());
	bool bStopCapture =false;
	bool bRemove =false;
	{
		MutexLocker locker(&m_VideoMutex); //房间线程 buffer线程  m_mapLocalHwndtoChannel  VideoOutputEngine::m_VOutputEngineMap
		std::map<HWND,UInt32>::iterator iter;
		for (iter=m_mapLocalHwndtoChannel.begin();iter!=m_mapLocalHwndtoChannel.end();)
		{
			if (iRoomId == iter->second )
			{
				RCPtr<VideoInputEngine> pVideoInputEngine = VideoInputEngine::FindAppendRoom(iRoomId);
				if (pVideoInputEngine)
					pVideoInputEngine->DelLocalHwnd(iter->first);
				iter = m_mapLocalHwndtoChannel.erase(iter);
			}
			else
			{
				++iter;
			}
		}

		RCPtr<VideoInputEngine> pVideoInputEngine = VideoInputEngine::FindAppendRoom(iRoomId);
		if (pVideoInputEngine)
		{
			if (!VideoInputEngine::IsDeviceUsed(pVideoInputEngine->GetDeviceID(),iRoomId))
			{
				bStopCapture = true;//此时停止视频采集
			}

		}
	}

	if (bStopCapture)
	{
		//由主线程来stop
		RCPtr<VideoInputEngine> pVideoInputEngine = VideoInputEngine::FindAppendRoom(iRoomId);
		if (pVideoInputEngine)
		{
			pVideoInputEngine->StopCapture();
			m_bStopVideoEngine = true;
			m_iStopRoomId=iRoomId;
		}
	}
	Trace("AVEngine::DelVideoLogicChannelRoom() end iRoomId=%d time = %I64d \n",iRoomId,GetTickCount());
}

void AVEngine::DelVideoLogicChannelAll()
{
	Trace("AVEngine::DelVideoLogicChannelAll() time = %I64d \n",GetTickCount());
	VideoInputEngine::RemoveAllAppendRoom();
	VideoInputEngine::RemoveAllInputEngine();
}

void AVEngine::ClearVideoOutputRoom(UInt32 iRoomId)
{
	Trace("AVEngine::ClearVideoOutputRoom() time = %I64d \n",GetTickCount());
	MutexLocker locker(&m_VideoMutex); //房间线程 buffer线程  m_mapLocalHwndtoChannel  VideoOutputEngine::m_VOutputEngineMap
	VideoOutputEngine::RemoveOutputEngine(iRoomId);
}

void AVEngine::ClearVideoOutputRoomAll()
{
	Trace("AVEngine::ClearVideoOutputRoomAll() time = %I64d \n",GetTickCount());
	MutexLocker locker(&m_VideoMutex); //房间线程 buffer线程  m_mapLocalHwndtoChannel  VideoOutputEngine::m_VOutputEngineMap
	VideoOutputEngine::RemoveAllOutputEngine();
}

void AVEngine::ClearVideoInputAppendRoom(UInt32 iRoomId)
{
	Trace("AVEngine::ClearVideoInputAppendRoom() time = %I64d \n",GetTickCount());
	VideoInputEngine::RemoveAppendRoom(iRoomId);
}

void AVEngine::ClearVideoInputAppendRoomAll()
{
	Trace("AVEngine::ClearVideoInputAppendRoomAll() time = %I64d \n",GetTickCount());
	VideoInputEngine::RemoveAllInputEngine();
}

void  AVEngine::SendAudioData(const void *data, int len,bool bRTP)
{
#ifdef SELF_TEST_NO_AUDIO
	return;
#endif
#ifdef _DEBUG
	OutputDebugStr(L"SendAudioData\r\n");
#endif

	UInt32 iRoomId = 0;
	MutexLocker locker(&m_AudioMutex);//m_allAudioLogicChannel 房间线程
	RelayManger* pRelayMgr = RelayManger::Instance();
	std::map<UInt32, ST_AUDIO_PROPERTY>::iterator iter = m_allAudioLogicChannel.find(iRoomId);
    //向多个房间发送音频数据
	for (iter = m_allAudioLogicChannel.begin();iter!=m_allAudioLogicChannel.end();iter++)
	{
		if (iter->second.IsPaused()) continue; //判断房间语音是否暂停，暂停将不发送
		iRoomId  = iter->first;
		Packet *pPacket = AudioPacket::makePacket(-1,AUDIO_RTP_PACKET_TYPE,len, reinterpret_cast<UInt8*>(const_cast<void*>(data)));
		if (!pPacket)
		{
			Trace("AVEngine::SendAudioData: 申请内存失败[len=%d] time = %I64d\n", len,GetTickCount());
			return;
		}
		pPacket->AddReference();
		pRelayMgr->SendData(iRoomId, pPacket->getPacketContent(), pPacket->getPacketLen(),true);
		pPacket->RemoveReference();
	}
#ifdef _DEBUG
	OutputDebugStr(L"SendAudioData--end!\r\n");
#endif
}

void  AVEngine::RecvAudioData(UInt32 iRoomId,UInt32 iDesUin, const void *data, unsigned int length,unsigned int iSeq,bool bRTP)
{
#ifdef _DEBUG
	//OutputDebugStr(L"RecvAudioData\r\n");
#endif
	MutexLocker locker(&m_AudioMutex);//m_allAudioLogicChannel 房间线程
	if (m_bRoomMux) return ;
	std::map<UInt32,bool>::iterator iter;
	if ((iter=m_mapRoomMux.find(iRoomId))!=m_mapRoomMux.end() && iter->second) return;
	g_audioOutputEngine.OnRecvAudioData(MakeUserSession(iRoomId,iDesUin),(unsigned char*)data,length,iSeq);
#ifdef _DEBUG
	//OutputDebugStr(L"RecvAudioData---end!\r\n");
#endif
}

void  AVEngine::SendVideoData(UInt8* pData, UInt32 iDataLen, unsigned char cKey, UInt32 iSeqNum, UInt32 iKeySeqNum, std::vector<HWND>& vLocalHwnds)
{
	//向所有的视频用户发送数据
	std::vector<UInt32> vAllRooms;
	GetAllVideoUinChannel(vLocalHwnds, vAllRooms);
	if (vAllRooms.empty())
	{
		Assert(0);
		return;
	}

	RelayManger* pRelayMgr = RelayManger::Instance();

	if (iDataLen>MAX_VIDEO_PACKET_LEN)
	{
		//视频数据过大，需要拆包
		UInt8 cPieceSeqNum = 0;
		UInt8 cPieceSum = iDataLen/MAX_VIDEO_PACKET_LEN;
		if (iDataLen%MAX_VIDEO_PACKET_LEN !=0)
		{
			++cPieceSum;
		}

		UInt32 iLeaveDataLen = iDataLen;
		UInt32 iSendLen = 0;
		
		do 
		{
			iSendLen = iLeaveDataLen>MAX_VIDEO_PACKET_LEN ? MAX_VIDEO_PACKET_LEN : iLeaveDataLen;

			++cPieceSeqNum;

			Packet* pPacket = VideoPiecePacket::makePacket(-1, cKey, iSeqNum, iKeySeqNum, cPieceSum, cPieceSeqNum, iSendLen, pData+(iDataLen-iLeaveDataLen));
			if (!pPacket)
			{
				Trace("AVEngine::SendVideoData: 申请内存失败[key=%d, Seq=%d,cPieceSum=%d,iSendLen=%d],DataLen=%d] time = %I64d\n", (cKey & KEY_VALUE)==KEY_VALUE?1:0, iSeqNum,cPieceSeqNum,iSendLen,iDataLen,GetTickCount());
				return;
			}
			pPacket->AddReference();
			static unsigned int s_video = 0;
			s_video++;
			for (UInt32 i=0; i<vAllRooms.size() ; ++i)
			{
				if (s_video%20==0)
				{
					Trace("AVEngine::SendVideoData: 发送视频分片[key=%d, Seq=%d, piece=%d, Len=%d]给[Room=%d ] time = %I64d\n", (cKey & KEY_VALUE)==KEY_VALUE?1:0, iSeqNum, cPieceSeqNum, iSendLen,vAllRooms[i],GetTickCount());
				}
				pRelayMgr->SendData(vAllRooms[i], pPacket->getPacketContent(), pPacket->getPacketLen(),false);
			}

			pPacket->RemoveReference();
			
			iLeaveDataLen -= iSendLen;
		} 
		while(iLeaveDataLen>0);		

		Assert(cPieceSeqNum==cPieceSum);
	}
	else
	{
		Packet* pPacket = VideoPacket::makePacket(-1, cKey, iSeqNum, iKeySeqNum, iDataLen, pData);
		if (!pPacket)
		{
			Trace("AVEngine::SendVideoData: 申请内存失败[key=%d, Seq=%d],DataLen=%d] time = %I64d\n", (cKey & KEY_VALUE)==KEY_VALUE, iSeqNum,iDataLen,GetTickCount());
			return;
		}
		pPacket->AddReference();

		for (UInt32 i=0; i<vAllRooms.size(); ++i)
		{

			//Trace("AVEngine::SendVideoData: 发送视频数据[key=%d, Seq=%d] \n", bKey?1:0, iSeqNum);

			pRelayMgr->SendData(vAllRooms[i], pPacket->getPacketContent(), pPacket->getPacketLen(),false);
		}

		pPacket->RemoveReference();
	}
}

void  AVEngine::RecvVideoData(UInt32 iRoomId, UInt32 iDesUin, UInt8* pData, UInt32 iDataLen, UInt8 cKey, UInt32 iSeqNum, UInt32 iKeySeqNum)
{
	MutexLocker locker(&m_VideoMutex); //房间线程 buffer线程  m_mapLocalHwndtoChannel  VideoOutputEngine::m_VOutputEngineMap
	RCPtr<VideoOutputEngine> pVOutEngine = VideoOutputEngine::FindOutputEngine(iRoomId);
	if(pVOutEngine)
	{
		BAD_MEM_READ(pData,iDataLen,"AVEngine::RecvVideoData")
			
		pVOutEngine->DecodeVideoData(iDesUin, pData, iDataLen, cKey, iSeqNum, iKeySeqNum);
	}
}

void  AVEngine::GetAllVideoUinChannel(std::vector<HWND>& vLocalHwnds, std::vector<UInt32>& vAllRoomId)
{
	//Trace("AVEngine::GetAllVideoUinChannel() time = %I64d \n",GetTickCount());
	//MutexLocker locker(&m_VideoMutex); 死锁 偶尔 退出时候
	for (UInt32 i=0; i<vLocalHwnds.size(); ++i)
	{
		std::map<HWND, UInt32 >::iterator iterHWND;
		iterHWND = m_mapLocalHwndtoChannel.find(vLocalHwnds[i]);
		if (iterHWND!=m_mapLocalHwndtoChannel.end())
		{
			vAllRoomId.push_back(iterHWND->second);
		}
	}
}

bool  AVEngine::IsAtLeastOneChannelReady(std::vector<HWND>& vLocalHwnds)
{
	Trace("AVEngine::IsAtLeastOneChannelReady() time = %I64d \n",GetTickCount());
	std::vector<UInt32> vAllRooms;
	GetAllVideoUinChannel(vLocalHwnds, vAllRooms);

	for (UInt32 i=0; i<vAllRooms.size(); ++i)
	{
		//Trace("AVEngine::SendVideoData: 发送视频数据[key=%d, Seq=%d]给[Uin=%d LogicID=%d] \n", bKey?1:0, iSeqNum, iDesUin, cLogicID);

		RelayManger* pRelayMgr = RelayManger::Instance();
		unsigned short sStatus = pRelayMgr->QueryChannelStatus(vAllRooms[i]);
		if (sStatus == CHANNEL_SUCCESS)
		{
			return true;
		}
	}
	return false;
}


void AVEngine::TestAudio(bool bTest)
{
	Trace("AVEngine::TestAudio() time = %I64d \n",GetTickCount());
	g_audioInputEngine.SetAudioTest(bTest);
	if (!bTest)
	{
		return;
	}
	if (!g_audioInputEngine.IsStart())
	{
		g_audioInputEngine.Start();
	}
	else
	{
		g_audioInputEngine.Stop();
		g_audioInputEngine.Start();
	}
	if (g_audioOutputEngine.IsStart())
	{
		g_audioOutputEngine.Stop();
		g_audioOutputEngine.Start();
	}
	else
	{
		g_audioOutputEngine.Start();
	}
	
}

void AVEngine::SetSpeakVolume(UInt32 iRoomId, int iVolume)
{
	Trace("AVEngine::SetSpeakVolume() time = %I64d \n",GetTickCount());
	std::map<UInt32, ST_AUDIO_PROPERTY>::iterator iter = m_allAudioLogicChannel.find(iRoomId);
	if (iter != m_allAudioLogicChannel.end())
	{
		g_audioOutputEngine.SetVolumeOutput(iVolume);
	}
}

void AVEngine::SetSpeakerRoomVolume(UInt32 iRoomId,UInt32 iVolume)
{
	g_struct.SetRoomSpeakerVolume(iRoomId,(float)iVolume/255);
}

UINT32 AVEngine::GetSpeakerRoomVolume(UInt32 iRoomId)
{
	return g_struct.GetRoomSpeakerVolume(iRoomId)*0xff;
}

void AVEngine::SetMicVolume(UInt32 iRoomId,int iVolume)
{
	MutexLocker locker(&m_AudioMutex);//m_allAudioLogicChannel 房间线程
	std::map<UInt32, ST_AUDIO_PROPERTY>::iterator iter = m_allAudioLogicChannel.find(iRoomId);
	if (iter != m_allAudioLogicChannel.end())
	{
		g_audioInputEngine.SetVolumeInput(iVolume);
	}
}

void AVEngine::SetSysPlayerVolume(UInt32 iRoomId, int iVolume)
{
	MutexLocker locker(&m_AudioMutex);//m_allAudioLogicChannel 房间线程
	std::map<UInt32, ST_AUDIO_PROPERTY>::iterator iter = m_allAudioLogicChannel.find(iRoomId);
	if (iter != m_allAudioLogicChannel.end())
	{
		g_audioOutputEngine.SetSysVolume(iVolume);
	}
}

bool AVEngine::SetRoomSoundMute(UInt32 iRoomId, bool bMute)
{
	MutexLocker locker(&m_AudioMutex);//m_allAudioLogicChannel 房间线程
	m_mapRoomMux[iRoomId] = bMute;
	return true;
}

bool AVEngine::GetRoomSoundMute(UInt32 iRoomId,bool& bMute)
{
	MutexLocker locker(&m_AudioMutex);//m_allAudioLogicChannel 房间线程
	bMute = false;
	std::map<UInt32,bool>::iterator iter;
	if ((iter=m_mapRoomMux.find(iRoomId))!=m_mapRoomMux.end())
	{
		bMute = iter->second;
	}
	return true;
}

bool AVEngine::ClearRoomSoundMute(UInt32 iRoomId)
{
	MutexLocker locker(&m_AudioMutex);//m_allAudioLogicChannel 房间线程
	std::map<UInt32,bool>::iterator iter;
	if ((iter=m_mapRoomMux.find(iRoomId))!=m_mapRoomMux.end())
	{
		m_mapRoomMux.erase(iter);
	}
	return true;
}

bool AVEngine::SetAllRoomSoundMute(bool bMute)
{	
	this->m_bRoomMux = bMute;
	return true;
}

bool AVEngine::GetAllRoomSoundMute(bool& bMute)
{
	bMute = this->m_bRoomMux;
	return true;
}

bool AVEngine::GetAllRoomMicMute(bool& bMute)
{
	bMute = g_struct.GetMicMute();
	return true;
}

bool AVEngine::SetAllRoomMicMute(bool bMute)
{
	g_struct.SetMicMute(bMute);
	return true;
}

void	AVEngine::SetMicRoomAllVolume(UInt32 iVolume)
{
	g_audioInputEngine.SetMicRoomAllVolume(iVolume);
}

UInt32	AVEngine::GetMicRoomAllVolume()
{
	return g_audioInputEngine.GetMicRoomAllVolume();
}

float AVEngine::GetNatLostRate()
{
	float fRate = 0.0f;
	std::map<UInt32, RCPtr<VideoOutputEngine> > ::iterator iter;
	for (iter=VideoOutputEngine::m_VOutputEngineMap.begin();iter!=VideoOutputEngine::m_VOutputEngineMap.end();++iter)
	{
		fRate += iter->second->GetLostPacketRate();
	}

	return (fRate + g_audioOutputEngine.GetLostPacketRate()) / (VideoOutputEngine::m_VOutputEngineMap.size() + 1);
}

bool AVEngine::GetShowFFTWav(UInt32 iRoomId,UInt32 iFromUin)
{
	std::map<UInt32, RCPtr<VideoOutputEngine> > ::iterator iter = VideoOutputEngine::m_VOutputEngineMap.find(iRoomId);
	if (iter!=VideoOutputEngine::m_VOutputEngineMap.end())
	{
		return iter->second->GetShowFFTWav(iFromUin);
	}
	return true;
}

void AVEngine::SetShowFFTWav(UInt32 iRoomId,UInt32 iFromUin,bool bEnable)
{
	std::map<UInt32, RCPtr<VideoOutputEngine> > ::iterator iter = VideoOutputEngine::m_VOutputEngineMap.find(iRoomId);
	if (iter!=VideoOutputEngine::m_VOutputEngineMap.end())
	{
		return iter->second->SetShowFFTWav(iFromUin,bEnable);
	}
}

void AVEngine::SetFFTALL(bool bEnable)
{
	VideoOutputEngine::m_bViewFFTALL = bEnable;
}

void AVEngine::GetFFTALL(bool& bEnable)
{
	bEnable = VideoOutputEngine::m_bViewFFTALL;
}

void AVEngine::SetFFTME(bool& bEnable)
{
	VideoInputEngine::m_bViewFFTME = bEnable;
}

void AVEngine::GetFFTME(bool& bEnable)
{
	bEnable = VideoInputEngine::m_bViewFFTME;
}

bool AVEngine::SetMicGreater(bool bEnable)
{
	return true;
}

bool AVEngine::GetMicGreater(bool& bEnable)
{
	return true;
}

bool AVEngine::IsVideoVirtualDevice(int iDevId)
{
	return VideoInputEngine::IsVideoVirtualDevice(iDevId);
}

bool AVEngine::SetMode(enuROOM_AV_AUDIO_MODE mode)
{
	return g_audioInputEngine.SetMode(mode);
}

Enum_Camera_Status AVEngine::GetCamerStatus(int iDevice)
{
	RCPtr<VideoInputEngine> pVInput(NULL);
	if (pVInput = VideoInputEngine::FindInputEngine(iDevice))
	{

		return pVInput->GetCamerStatus();
	}
	Assert(0);
	return kVideo_NoExist;
}

bool AVEngine::GetAVNetStatus(UInt32 iRoomId,Service_Av_Net_Status_All& sansa)
{
	ZeroMemory(&sansa,sizeof(Service_Av_Net_Status_All));
	{ //音频、视频 下行网络状态
		std::map<UInt32, RCPtr<VideoOutputEngine> > ::iterator iter;
		iter = VideoOutputEngine::m_VOutputEngineMap.find(iRoomId);
		if (iter!=VideoOutputEngine::m_VOutputEngineMap.end())
		{
			sansa.fVideoOutLostRate = iter->second->GetLostPacketRate();
			sansa.iVideoOutTotalPacket = iter->second->GetTotalPackets();
		}

		float fLostRates = 0.0f;
		UInt32 iTotalPacket = 0;
		UInt32 iLostPacket = 0;
		ClientUser::getAudioPackStatus(iRoomId,iTotalPacket,iLostPacket);
		if (iTotalPacket+iLostPacket>0)
		{
			fLostRates = (float)iLostPacket / (iTotalPacket+iLostPacket);
		}
		sansa.fAudioOutLostRate = fLostRates;
		sansa.iAudioOutTotalPacket = iTotalPacket;
	}
	
	//本地上行网络状态
	{
		UInt32 iAudioTotalPacket = 0;
		UInt32 iVideoTotalPacket = 0;
		UInt32 iLostAudioPacket = 0;
		UInt32 iLostVideoPacket = 0;
		//指定房间用户上麦
		if (VideoInputEngine::m_VideoInputEngineRoomMap.find(iRoomId)!=VideoInputEngine::m_VideoInputEngineRoomMap.end())
		{
			RelayManger* pRelayMgr = RelayManger::Instance();

			pRelayMgr->GetAVNetSendStatus(iRoomId,iAudioTotalPacket,iLostAudioPacket, iVideoTotalPacket, iLostVideoPacket);
		}
		
		sansa.iVideoInTotalPacket = iVideoTotalPacket;
		sansa.iAudioInTotalPacket = iAudioTotalPacket;
		
		if (iAudioTotalPacket+iLostAudioPacket>0)
		{
			sansa.fAudioInLostRate = (float)iLostAudioPacket / (iAudioTotalPacket+iLostAudioPacket);
		}
		
		if (iVideoTotalPacket+iLostVideoPacket>0)
		{
			sansa.fVideoInLostRate = (float)iLostVideoPacket / (iVideoTotalPacket+iLostVideoPacket);
		}
	}
	
	return true;
}