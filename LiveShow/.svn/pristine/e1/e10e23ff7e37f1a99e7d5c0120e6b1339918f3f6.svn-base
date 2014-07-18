#include "StdAfx.h"
#include "AV.h"
#include "AVModule.h"
#include "AVManager.h"
#include "RelayManger.h"
#include "MyAssert.h"
#include "TimeUtils.h"
#include "AVChannelMgr.h"

const wchar_t* const  g_AVModuleName = L"AVModule";

void AVModule::Load(IModuleManager * pModuleManager)
{
	m_pModuleManager = pModuleManager;
	RelayManger::Initialize(pModuleManager);
}

void AVModule::Unload()
{
	ErrTrace("AVModule::Unload()\n");
	RelayManger::DestroyBufferInstance();
	AVChannelMgr::Destroy();
	RelayManger::DestroyInstance();

	UInt32 iLen  = 0;
	UInt32 iType = 0;
	void*  pMsg  = NULL;
	while(m_CircleBuffer.pop_front(pMsg, iLen, iType))
	{
		delete (Event*)pMsg; 
		pMsg = NULL;
		iType= 0;
		iLen = 0;
	}
}

void AVModule::ProcessEvent(Event const& evt)
{
	switch(evt.eventValue)
	{
		// 	case Room::EVENT_VALUE_TEST:
		// 		{
		// 			OutputDebugString(L"\n%%%%%%%%%%%%%%%%%    来自Hall模块的问候!   %%%%%%%%%%%%%%%  time = %I64d \n",TimeUtils::Milliseconds());
		// 		}
		// 		break;
	case 1: break;
	default:break;
	}
}

void AVModule::ProcessMessage(Message const& msg)
{
	switch (msg.messageValue)
	{
	case MESSAGE_VALUE_SYS_CYCLE_TRIGGER:
		{
			//处理循环buffer中的Event
			processCircleBuffer();

			m_avManager.OperateVideoEngine();
		}
		break;
	default:
		break;
	}
}

int32 AVModule::Invoke(ServiceValue const serviceValue, param st)
{
	switch(serviceValue)
	{
	case SERVICE_AV_RELAY_REQ: //获取Relay库运行的开始事件  
		{
			Trace("AVModule::Invoke - SERVICE_AV_RELAY_REQ  time = %I64d \n",GetTickCount());
			Service_AV_Req_Time* pMsg = reinterpret_cast<Service_AV_Req_Time*>(st);
			RelayManger* pP2PMgr = RelayManger::Instance();
			pMsg->lRelaySymbolTime = RelayManger::GetLibSymbolTime();
		}
		break;
	case SERVICE_AV_RELAY_CFG://中转服务器IP配置
		{
			Trace("AVModule::Invoke - SERVICE_AV_RELAY_CFG  time = %I64d \n",GetTickCount());
			Service_Relay_Config * pMsg = reinterpret_cast<Service_Relay_Config*>(st);
			RelayManger::Config(pMsg->iMyUin, pMsg->iRelayIP,pMsg->sRelayPort,pMsg->unP2PTimeout);
		}
		break;
	case SERVICE_AV_RELAY_CONN://创建房间UDP连接，开始监听
		{
			Trace("AVModule::Invoke - SERVICE_AV_RELAY_CONN  time = %I64d \n",GetTickCount());
			Service_AV_Relay_Conn * pMsg = reinterpret_cast<Service_AV_Relay_Conn*>(st);
			m_avManager.StartRoomConn(pMsg->iRoomId);
		}
		break;
	case SERVICE_AV_VIDEO_CFG: //视频大小参数等设置，仅仅配置摄像头参数，不启动摄像头，当第一个房间使用摄像头时再启动
		{
			Trace("AVModule::Invoke - SERVICE_AV_VIDEO_CFG  time = %I64d \n",GetTickCount());
			Service_AV_Video_Cfg *pMsg = reinterpret_cast<Service_AV_Video_Cfg*>(st);
			bool bRet = m_avManager.ConfigVideoParam(pMsg->iWidth, pMsg->iHeight, pMsg->iQuality, pMsg->iDeviceID,pMsg->iVideoMode);
			if (!bRet)
			{
				Trace("视频摄像头参数配置失败......SERVICE_AV_VIDEO_CFG  time = %I64d \n",GetTickCount());
				Assert(0);
				return -1;
			}
		}
		break;
	case SERVICE_AV_AUDIO_CFG://配置音频设备参数
		{
			Trace("AVModule::Invoke - SERVICE_AV_AUDIO_CFG  time = %I64d \n",GetTickCount());
			Service_AV_Audio_Cfg *pMsg = reinterpret_cast<Service_AV_Audio_Cfg*>(st);
			bool bRet = m_avManager.SetAudioDevice(pMsg->iInputDev,pMsg->iOutputDev);
			if (!bRet)
			{
				Trace("音频设备参数配置失败......SERVICE_AV_AUDIO_CFG  time = %I64d \n",GetTickCount());
				Assert(0);
				return -1;
			}
		}
		break;
	case SERVICE_AV_END_VIDEO_CFG://去除所有房间和该摄像头设备的关联，并关闭摄像头。
		{
			Trace("AVModule::Invoke - SERVICE_AV_END_VIDEO_CFG  time = %I64d \n",GetTickCount());
			Service_AV_Video_Cfg* pMsg = reinterpret_cast<Service_AV_Video_Cfg*>(st);
			m_avManager.FinishVideoParam(pMsg->iDeviceID);
		}
		break;
	case SERVICE_AV_VIDEO_GET_CFG: //获取指定摄像头配置参数
		{
			Trace("AVModule::Invoke - SERVICE_AV_VIDEO_GET_CFG  time = %I64d \n",GetTickCount());
			Service_AV_Video_Cfg* pMsg = reinterpret_cast<Service_AV_Video_Cfg*>(st);
			m_avManager.GetVideoParam(pMsg->iDeviceID, pMsg->iWidth, pMsg->iHeight, pMsg->iQuality);
		}
		break;
	case SERVICE_AV_AUDIO_START:	  //用户上公麦、私麦、私聊等开启语音
		{
			Trace("AVModule::Invoke - SERVICE_AV_AUDIO_START  time = %I64d \n",GetTickCount());
			Service_AV_Audio_Start* pMsg = reinterpret_cast<Service_AV_Audio_Start*>(st);
			if (!m_avManager.StartRoomAudio(pMsg->iRoomId, pMsg->cLogicID))
			{
				Trace("启动房间[RoomId=%d]音频失败......SERVICE_AV_AUDIO_START\n",pMsg->iRoomId);
				Assert(0);
				return -1;
			}
		}
		break;
	case SERVICE_AV_VIDEO_START:   //用户上公麦、私麦、私聊等开启视频
		{
			Trace("AVModule::Invoke - SERVICE_AV_VIDEO_START  time = %I64d \n",GetTickCount());
			Service_AV_Video_Start* pMsg =  reinterpret_cast<Service_AV_Video_Start*>(st);
			if (!m_avManager.StartRoomVideo(pMsg->iRoomId,pMsg->hWndLocal,pMsg->iDeviceId,pMsg->enType))
			{
				Trace("启动房间[RoomId=%d]视频失败......SERVICE_AV_VIDEO_START\n",pMsg->iRoomId);
				return -1;
			}
		}
		break;
	case SERVICE_AV_AUDIO_STOP: //用户下公麦、私麦、私聊等关闭语音
		{
			Trace("AVModule::Invoke - SERVICE_AV_AUDIO_STOP  time = %I64d \n",GetTickCount());
			Service_AV_Audio_Stop* pMsg =  reinterpret_cast<Service_AV_Audio_Stop*>(st);
			m_avManager.CloseRoomAudio(pMsg->iRoomId, pMsg->cLogicID);
		}
		break;
	case SERVICE_AV_VIDEO_STOP: //用户下公麦、私麦、私聊等关闭视频 
		{
			Trace("AVModule::Invoke - SERVICE_AV_VIDEO_STOP  time = %I64d \n",GetTickCount());
			Service_AV_Video_Stop* pMsg =  reinterpret_cast<Service_AV_Video_Stop*>(st);
			m_avManager.CloseRoomVideo(pMsg->iRoomId, pMsg->hWndLocal,pMsg->iDeviceID);
		}
		break;
	case SERVICE_AV_AUDIO_LOCAL: //本地房间语音暂停、恢复等操作
		{
			Trace("AVModule::Invoke - SERVICE_AV_AUDIO_LOCAL  time = %I64d \n",GetTickCount());
			Service_AV_Audio_Local* pMsg =  reinterpret_cast<Service_AV_Audio_Local*>(st);
			if (pMsg->bPause)
				m_avManager.PauseLocalAudio(pMsg->iRoomId);
			else
				m_avManager.ContinueLocalAudio(pMsg->iRoomId);
		}
		break;
	case SERVICE_AV_VIDEO_LOCAL://本房间视频暂停、恢复等操作
		{
			Trace("AVModule::Invoke - SERVICE_AV_VIDEO_LOCAL  time = %I64d \n",GetTickCount());
			Service_AV_Video_Local* pMsg =  reinterpret_cast<Service_AV_Video_Local*>(st);
			if (pMsg->bPause)
				m_avManager.PauseLocalVideo(pMsg->iRoomId,pMsg->hWndLocal);
			else
				m_avManager.ContinueLocalVideo(pMsg->iRoomId,pMsg->hWndLocal);
		}
		break;
	case SERVICE_AV_AUDIO_REMOTE://暂停公麦、私麦、私聊远程用户的语音
		{
			Trace("AVModule::Invoke - SERVICE_AV_AUDIO_REMOTE  time = %I64d \n",GetTickCount());
			Service_AV_Audio_Remote* pMsg =  reinterpret_cast<Service_AV_Audio_Remote*>(st);
			if (pMsg->bPause)
				m_avManager.PauseRemoteAudio(pMsg->iRoomId,pMsg->iFromUin);
			else
				m_avManager.ContinueRemoteAudio(pMsg->iRoomId,pMsg->iFromUin);
		}	
		break;
	case SERVICE_AV_VIDEO_REMOTE://暂停公麦、私麦、私聊远程用户视频
		{
			Trace("AVModule::Invoke - SERVICE_AV_VIDEO_REMOTE  time = %I64d \n",GetTickCount());
			Service_AV_Video_Remote* pMsg =  reinterpret_cast<Service_AV_Video_Remote*>(st);
			if (pMsg->bPause)
				m_avManager.PauseRemoteVideo(pMsg->iRoomId,pMsg->iFromUin,pMsg->hWndPartner);
			else
				m_avManager.ContinueRemoteVideo(pMsg->iRoomId,pMsg->iFromUin,pMsg->hWndPartner);
		}
		break;
	case SERVICE_AV_PHOTOAB_LOCAL://本地摄像头是否可以拍照
		{
			Trace("AVModule::Invoke - SERVICE_AV_PHOTOAB_LOCAL  time = %I64d \n",GetTickCount());
			Service_AV_PhotoAble_Local* pMsg = reinterpret_cast<Service_AV_PhotoAble_Local*>(st);
			pMsg->bPhotoAble = m_avManager.IsPhotoableLocalHwnd(pMsg->iRoomId,pMsg->hWndLocal);
		}
		break;
	case SERVICE_AV_PHOTOAB_REMOTE://远程摄像头是否可以拍照
		{
			Trace("AVModule::Invoke - SERVICE_AV_PHOTOAB_REMOTE  time = %I64d \n",GetTickCount());
			Service_AV_PhotoAble_Remote* pMsg = reinterpret_cast<Service_AV_PhotoAble_Remote*>(st);
			pMsg->bPhotoAble = m_avManager.IsPhotoableRemoteHwnd(pMsg->iRoomId,pMsg->iFromUin,pMsg->hWndPartner);
		}
		break;
	case SERVICE_SET_SPEAKER_VOLUME://设置房间音量
		{
			Trace("AVModule::Invoke - SERVICE_SET_SPEAKER_VOLUME  time = %I64d \n",GetTickCount());
			Service_Set_Speaker_Volume* pMsg = reinterpret_cast<Service_Set_Speaker_Volume*>(st);
			m_avManager.SetSpeakerVolume(pMsg->iRoomId,pMsg->iVolume);
		}
		break;
	case SERVICE_GET_SPEAKER_VOLUME://获取房间音量
		{
			Trace("AVModule::Invoke - SERVICE_GET_SPEAKER_VOLUME  time = %I64d \n",GetTickCount());
			Service_Get_Speaker_Volume* pMsg = reinterpret_cast<Service_Get_Speaker_Volume*>(st);
			pMsg->iVolume = m_avManager.GetSpeakerVolume();
		}
		break;
	case SERVICE_SET_SPEAKER_ROOM_VOLUME://设置房间扬声器音量
		{
			Trace("AVModule::Invoke - SERVICE_SET_SPEAKER_ROOM_VOLUME  time = %I64d \n",GetTickCount());
			Service_Set_Speaker_Room_Volume* pMsg = reinterpret_cast<Service_Set_Speaker_Room_Volume*>(st);
			m_avManager.SetSpeakerRoomVolume(pMsg->iRoomId,pMsg->iVolume);
		}
		break;
	case SERVICE_GET_SPEAKER_ROOM_VOLUME://获取房间扬声器音量
		{
			Trace("AVModule::Invoke - SERVICE_GET_SPEAKER_ROOM_VOLUME  time = %I64d \n",GetTickCount());
			Service_Get_Speaker_Room_Volume* pMsg = reinterpret_cast<Service_Get_Speaker_Room_Volume*>(st);
			pMsg->iVolume = m_avManager.GetSpeakerRoomVolume(pMsg->iRoomId);
		}
		break;
	case SERVICE_SET_SYSPLAYER_VOLUME://设置系统扬声器音量
		{
			Trace("AVModule::Invoke - SERVICE_SET_SYSPLAYER_VOLUME  time = %I64d \n",GetTickCount());
			Service_Set_SysPlayer_Volume* pMsg = reinterpret_cast<Service_Set_SysPlayer_Volume*>(st);
			m_avManager.SetSysPlayerVolume(pMsg->iRoomId,pMsg->iVolume);
		}
		break;
	case SERVICE_GET_SYSPLAYER_VOLUME://获取系统扬声器音量
		{
			Trace("AVModule::Invoke - SERVICE_GET_SYSPLAYER_VOLUME  time = %I64d \n",GetTickCount());
			Service_Get_SysPlayer_Volume* pMsg = reinterpret_cast<Service_Get_SysPlayer_Volume*>(st);
			pMsg->iVolume = m_avManager.GetSysPlayerVolume();
		}
		break;
	case SERVICE_IS_VIDEO_DEV_INIT://视频设备是否已经初始化
		{
			Trace("AVModule::Invoke - SERVICE_IS_VIDEO_DEV_INIT  time = %I64d \n",GetTickCount());
			Service_Is_Video_Dev_Init* pMsg = reinterpret_cast<Service_Is_Video_Dev_Init*>(st);
			pMsg->bInit = m_avManager.IsVideoDevInit(pMsg->iDev);
		}
		break;
	case SERVICE_SET_MIC_VOLUME://设置麦克风音量
		{
			Trace("AVModule::Invoke - SERVICE_SET_MIC_VOLUME  time = %I64d \n",GetTickCount());
			Service_Set_Mic_Volume* pMsg = reinterpret_cast<Service_Set_Mic_Volume*>(st);
			m_avManager.SetMicVolume(pMsg->iRoomId, pMsg->iVolume);
		}
		break;
	case SERVICE_GET_MIC_VOLUME://获取麦克风音量 
		{
			Trace("AVModule::Invoke - SERVICE_GET_MIC_VOLUME  time = %I64d \n",GetTickCount());
			Service_Get_Mic_Volume* pMsg = reinterpret_cast<Service_Get_Mic_Volume*>(st);
			pMsg->iVolume = m_avManager.GetMicVolume();
		} 
		break;
	case SERVICE_CHK_SOUND_DEVICE://检查音频设备
		{
			Trace("AVModule::Invoke - SERVICE_CHK_SOUND_DEVICE  time = %I64d \n",GetTickCount());
			Service_Check_Sound_Device* pMsg = reinterpret_cast<Service_Check_Sound_Device*>(st);
			m_avManager.CheckSoundDevice(pMsg->bOpen);
		}
		break;
	case SERVICE_GET_SOUND_LEVEL://获取声音大小
		{
			Trace("AVModule::Invoke - SERVICE_GET_SOUND_LEVEL  time = %I64d \n",GetTickCount());
			Service_Get_Sound_Level* pMsg = reinterpret_cast<Service_Get_Sound_Level*>(st);
			m_avManager.GetSoundLevel(pMsg->iOutput, pMsg->iInput);
		}
		break;
	case SERVICE_GET_SOUND_DEVICE_NUM: //获取声音输入输出设备的数量
		{
			Trace("AVModule::Invoke - SERVICE_GET_SOUND_DEVICE_NUM  time = %I64d \n",GetTickCount());
			Service_Get_Sound_Device* pMsg =  reinterpret_cast<Service_Get_Sound_Device*>(st);
			if (!m_avManager.GetNumOfSoundDevices(pMsg->iPlayout, pMsg->iRecord))
			{
				return -1;
			}
		}
		break;
	case SERVICE_SET_SOUND_DEVICE://设置选择的声音设备
		{
			Trace("AVModule::Invoke - SERVICE_SET_SOUND_DEVICE  time = %I64d \n",GetTickCount());
			Service_Set_Sound_Device* pMsg = reinterpret_cast<Service_Set_Sound_Device*>(st);
			if (!m_avManager.SetSoundDevices(pMsg->iRecord, pMsg->iPlayout))
			{
				return -1;
			}
		}
		break;
	case SERVICE_GET_RECORD_DEVICE_NAME://获取输入设备名称
		{	
			Trace("AVModule::Invoke - SERVICE_GET_RECORD_DEVICE_NAME  time = %I64d \n",GetTickCount());
			Service_Get_Record_Device_Name* pMsg = reinterpret_cast<Service_Get_Record_Device_Name*>(st);
			if (!m_avManager.GetRecordDeviceName(pMsg->iDeviceIndex, &(pMsg->strDeviceName[0]), DEVICE_NAME_LEN))
			{
				return -1;
			}
		}
		break;
	case SERVICE_GET_PLAYOUT_DEVICE_NAME://获取扬声器设备名称
		{
			Trace("AVModule::Invoke - SERVICE_GET_PLAYOUT_DEVICE_NAME  time = %I64d \n",GetTickCount());
			Service_Get_PlayOut_Device_Name* pMsg = reinterpret_cast<Service_Get_PlayOut_Device_Name*>(st);
			if (!m_avManager.GetPlayoutDeviceName(pMsg->iDeviceIndex, &(pMsg->strDeviceName[0]), DEVICE_NAME_LEN))
			{
				return -1;
			}
		}
		break;
	case SERVICE_GET_AUDIO_DEVICE_LIST: //获取音频设备列表
		{
			Trace("AVModule::Invoke - SERVICE_GET_AUDIO_DEVICE_LIST  time = %I64d \n",GetTickCount());
			Service_Get_Audio_Device_List* pMsg = reinterpret_cast<Service_Get_Audio_Device_List*>(st);
			m_avManager.GetAudioDeviceList(pMsg->iDeviceNum, pMsg->wsDeviceName,pMsg->bInput);
		}
		break;
	case SERVICE_GET_VIDEO_DEVICE_LIST://获取视频设备列表
		{
			Trace("AVModule::Invoke - SERVICE_GET_VIDEO_DEVICE_LIST  time = %I64d \n",GetTickCount());
			Service_Get_Video_Device_List* pMsg = reinterpret_cast<Service_Get_Video_Device_List*>(st);
			m_avManager.GetVideoDeviceList(pMsg->iDeviceNum, pMsg->wsDeviceName);
		}
		break;
	case SERVICE_GET_VIDEO_DEVINFO_LIST://获取视频信息列表
		{
			Trace("AVModule::Invoke - SERVICE_GET_VIDEO_DEVINFO_LIST  time = %I64d \n",GetTickCount());
			Service_Get_Video_DevInfo_List* pMsg = reinterpret_cast<Service_Get_Video_DevInfo_List*>(st);
			m_avManager.GetVideoDeviceInfoList(pMsg->iDeviceNum, pMsg->strucDeviceName);
		}
		break;
	case SERVICE_SHOW_VIDEO_AJUST://显示视频调整对话框
		{
			Trace("AVModule::Invoke - SERVICE_SHOW_VIDEO_AJUST  time = %I64d \n",GetTickCount());
			Service_Show_Video_Ajust* pMsg = reinterpret_cast<Service_Show_Video_Ajust*>(st);
			if (!m_avManager.ShowVideoAdjustDlg(pMsg->hParentWnd, pMsg->iDeviceID))
			{
				return -1;
			}
		}
		break;
	case SERVICE_IS_START_ENGINE://指定房间自己是否上麦、私麦、私聊等
		{
			Trace("AVModule::Invoke - SERVICE_IS_START_ENGINE  time = %I64d \n",GetTickCount());
			Service_Is_Start_Engine* pMsg = reinterpret_cast<Service_Is_Start_Engine*>(st);
			pMsg->bStart = m_avManager.IsStartAVEngine(pMsg->iRoomId);
		}
		break;
	case SERVICE_AV_AUDIO_OPP_START://添加接收房间公麦、私麦、私聊语音 
		{
			Trace("AVModule::Invoke - SERVICE_AV_AUDIO_OPP_START  time = %I64d \n",GetTickCount());
			Service_Av_Audio_Opp_Start *pMsg =  reinterpret_cast<Service_Av_Audio_Opp_Start*>(st);
			m_avManager.RecvRoomAudioOpp(pMsg->lRelaySymbolTime,pMsg->iRoomId,pMsg->iFromUin,pMsg->cLogicID);
		}
		break;
	case SERVICE_AV_VIDEO_OPP_START://添加接收房间公麦、私麦、私聊视频
		{
			Trace("AVModule::Invoke - SERVICE_AV_VIDEO_OPP_START  time = %I64d \n",GetTickCount());
			Service_Av_Video_Opp_Start *pMsg =  reinterpret_cast<Service_Av_Video_Opp_Start*>(st);
			m_avManager.RecvRoomVideoOpp(pMsg->lRelaySymbolTime,pMsg->iRoomId,pMsg->iFromUin,pMsg->cLogicID,pMsg->hWndPartner,pMsg->iWidth,pMsg->iHeight,pMsg->iQuality,pMsg->iVideoMode,pMsg->enType);
		}
		break;
	case SERVICE_AV_VIDEO_CHG_HANDLE://更改对方显示窗口，及输出图像大小
		{
			Trace("AVModule::Invoke - SERVICE_AV_VIDEO_CHG_HANDLE  time = %I64d \n",GetTickCount());
			Service_AV_Video_Change_Handle *pMsg =  reinterpret_cast<Service_AV_Video_Change_Handle*>(st);
			m_avManager.ChangePartnerHwnd(pMsg->iRoomId,pMsg->iFromUin,pMsg->hWndPartner,pMsg->iWidth,pMsg->iHeight,pMsg->hOldWnd);
		}
		break;
	case SERVICE_AV_VIDEO_CHG_LOCALHDL://更改本地自己显示的窗口的句柄
		{
			Trace("AVModule::Invoke - SERVICE_AV_VIDEO_CHG_LOCALHDL  time = %I64d \n",GetTickCount());
			Service_AV_Video_Change_LocalHDL *pMsg =  reinterpret_cast<Service_AV_Video_Change_LocalHDL*>(st);
			m_avManager.ChangeLocalHwnd(pMsg->iRoomId,pMsg->hNewWnd,pMsg->hOldWnd);
		}	
		break;
	case SERVICE_AV_VIDEO_CHG_EFFACT://更改本地视频图像效果
		{
			Trace("AVModule::Invoke - SERVICE_AV_VIDEO_CHG_EFFACT  time = %I64d \n",GetTickCount());
			Service_AV_Video_Chg_Effact *pMsg =  reinterpret_cast<Service_AV_Video_Chg_Effact*>(st);
			m_avManager.ChangeLocalEffact(pMsg->iRoomId,pMsg->hWndLocal,pMsg->bEnable);
		}
		break;
	case SERVICE_IS_OPEN_AV_EFFACT://是否开启了远程显示图像效果
		{
			Trace("AVModule::Invoke - SERVICE_IS_OPEN_AV_EFFACT  time = %I64d \n",GetTickCount());
			Service_Is_Open_Av_Effact *pMsg =  reinterpret_cast<Service_Is_Open_Av_Effact*>(st);
			pMsg->bOpen = m_avManager.IsLocalEffact(pMsg->iRoomId,pMsg->hLocalWnd);
		}
		break;
	case SERVICE_AV_AUDIO_OPP_STOP://对方离开房间，下麦、停止语音聊天
		{
			Trace("AVModule::Invoke - SERVICE_AV_AUDIO_OPP_STOP  time = %I64d \n",GetTickCount());
			Service_Av_Audio_Opp_Stop *pMsg =  reinterpret_cast<Service_Av_Audio_Opp_Stop*>(st);
			m_avManager.CloseRoomAudioOpp(pMsg->iRoomId,pMsg->iFromUin,pMsg->cLogicID);
		}
		break;
	case SERVICE_AV_VIDEO_OPP_STOP://对方离开房间，下麦、停止视频聊天
		{
			Trace("AVModule::Invoke - SERVICE_AV_VIDEO_OPP_STOP  time = %I64d \n",GetTickCount());
			Service_Av_Video_Opp_Stop *pMsg =  reinterpret_cast<Service_Av_Video_Opp_Stop*>(st);
			m_avManager.CloseRoomVideoOpp(pMsg->iRoomId,pMsg->iFromUin,pMsg->cLogicID,pMsg->hWndPartner);
		}
		break;	
	case SERVICE_IS_START_AV_ENGINE://房间是否开启音视频
		{
			Trace("AVModule::Invoke - SERVICE_IS_START_AV_ENGINE  time = %I64d \n",GetTickCount());
			Service_Is_Start_Av_Engine *pMsg =  reinterpret_cast<Service_Is_Start_Av_Engine*>(st);
			pMsg->bAble = m_avManager.IsStartAVEngine(pMsg->iRoomId);
		}
		break;
	case SERVICE_IS_PHOTO_ABLE_LOCAL://本地摄像头是否可以拍照
		{
			Trace("AVModule::Invoke - SERVICE_IS_PHOTO_ABLE_LOCAL  time = %I64d \n",GetTickCount());
			Service_Is_Photo_Able_Local *pMsg =  reinterpret_cast<Service_Is_Photo_Able_Local*>(st);
			pMsg->bAble = m_avManager.IsPhotoableLocalHwnd(pMsg->iRoomId,pMsg->localHwnd);
		}
		break;
	case SERVICE_IS_PHOTO_ABLE_REMOTE://接收到的上麦用户是否可以拍照
		{
			Trace("AVModule::Invoke - SERVICE_IS_PHOTO_ABLE_REMOTE  time = %I64d \n",GetTickCount());
			Service_Is_Photo_Able_Remote *pMsg =  reinterpret_cast<Service_Is_Photo_Able_Remote*>(st);
			pMsg->bAble = m_avManager.IsPhotoableRemoteHwnd(pMsg->iRoomId,pMsg->iFromUin,pMsg->partnerHwnd);
		}
		break;
	case SERVICE_AV_VIDEO_VIRTUAL://判断指定设备是否为虚拟视频设备
		{
			Trace("AVModule::Invoke - SERVICE_AV_VIDEO_VIRTUAL  time = %I64d \n",GetTickCount());
			Service_Av_Video_Virtual *pMsg =  reinterpret_cast<Service_Av_Video_Virtual*>(st);
			pMsg->bReturn = m_avManager.IsVideoVirtualDevice(pMsg->iDeviceId);
		}
		break;
	case SERVICE_AV_CLEAR_EXIT://房间退出时，清理分配给房间的资源
		{
			Trace("AVModule::Invoke - SERVICE_AV_CLEAR_EXIT  time = %I64d \n",GetTickCount());
			Service_Av_Clear_Exit *pMsg =  reinterpret_cast<Service_Av_Clear_Exit*>(st);
			bool bRet3 = m_avManager.CloseRoomConn(pMsg->iRoomId);
			bool bRet1 = m_avManager.ClearRoomAudio(pMsg->iRoomId);
			bool bRet2 = m_avManager.ClearRoomVideo(pMsg->iRoomId);
			pMsg->bRet = bRet1 & bRet2 & bRet3;
		}
		break;
	case SERVICE_AV_AUDIO_STOP_SAFE://安全关闭音频
		{
			Trace("AVModule::Invoke - SERVICE_AV_AUDIO_STOP_SAFE  time = %I64d \n",GetTickCount());
			m_avManager.SafeAudioInputStop();
		}
		break;
	case SERVICE_AV_VIDEO_START_TEST://开启测试摄像头
		{
			Trace("AVModule::Invoke - SERVICE_AV_VIDEO_START_TEST  time = %I64d \n",GetTickCount());
			Service_Av_Video_Start_Test *pMsg =  reinterpret_cast<Service_Av_Video_Start_Test*>(st);
			pMsg->bRet = m_avManager.StartVideoTest(pMsg->iDeviceId,pMsg->hWnd,pMsg->iResult);

		}
		break;
	case SERVICE_AV_VIDEO_STOP_SAFE://安全关闭测试摄像头
		{
			Trace("AVModule::Invoke - SERVICE_AV_VIDEO_STOP_SAFE  time = %I64d \n",GetTickCount());
			Service_Av_Video_Stop_Safe *pMsg =  reinterpret_cast<Service_Av_Video_Stop_Safe*>(st);
			pMsg->bRet = m_avManager.CloseVideoTest(pMsg->iDeviceId,pMsg->hWnd);
		}
		break;
	case SERVICE_SET_MIC_MUTE://设置麦克风是否静音 
		{
			Trace("AVModule::Invoke - SERVICE_SET_MIC_MUTE  time = %I64d \n",GetTickCount());
			Service_Set_Mic_Mute* pMsg = reinterpret_cast<Service_Set_Mic_Mute*>(st);
			pMsg->bResult = m_avManager.SetMicInputMute(pMsg->iRoomId, pMsg->bMute);
		}
		break;
	case SERVICE_GET_MIC_MUTE://获取麦克风是否静音
		{
			Trace("AVModule::Invoke - SERVICE_GET_MIC_MUTE  time = %I64d \n",GetTickCount());
			Service_Get_Mic_Mute* pMsg = reinterpret_cast<Service_Get_Mic_Mute*>(st);
			pMsg->bResult = m_avManager.GetMicInputMute(pMsg->iRoomId,pMsg->bMute);
		}
		break;
	case SERVICE_SET_SOUND_MUTE://设置房间声音是否静音
		{
			Trace("AVModule::Invoke - SERVICE_SET_SOUND_MUTE  time = %I64d \n",GetTickCount());
			Service_Set_Sound_Mute* pMsg = reinterpret_cast<Service_Set_Sound_Mute*>(st);
			pMsg->bResult = m_avManager.SetRoomSoundMute(pMsg->iRoomId, pMsg->bMute);
		}
		break;
	case SERVICE_GET_SOUND_MUTE://获取房间声音是否静音
		{
			Trace("AVModule::Invoke - SERVICE_GET_SOUND_MUTE  time = %I64d \n",GetTickCount());
			Service_Get_Sound_Mute* pMsg = reinterpret_cast<Service_Get_Sound_Mute*>(st);
			pMsg->bResult = m_avManager.GetRoomSoundMute(pMsg->iRoomId,pMsg->bMute);
		}
		break;
	case SERVICE_SET_PLAYER_MUTE://设置系统扬声器是否静音
		{
			Trace("AVModule::Invoke - SERVICE_SET_PLAYER_MUTE  time = %I64d \n",GetTickCount());
			Service_Set_Player_Mute* pMsg = reinterpret_cast<Service_Set_Player_Mute*>(st);
			pMsg->bResult = m_avManager.SetSysPlayerMute(pMsg->iRoomId, pMsg->bMute);
		}
		break;
	case SERVICE_GET_PLAYER_MUTE://获取系统扬声器是否静音
		{
			Trace("AVModule::Invoke - SERVICE_GET_PLAYER_MUTE  time = %I64d \n",GetTickCount());
			Service_Get_Player_Mute* pMsg = reinterpret_cast<Service_Get_Player_Mute*>(st);
			pMsg->bResult = m_avManager.GetSysPlayerMute(pMsg->iRoomId,pMsg->bMute);
		}
		break;
	case SERVICE_SET_ROOM_ALL_MUTE://设置所有房间是否静音
		{
			Trace("AVModule::Invoke - SERVICE_SET_ROOM_ALL_MUTE  time = %I64d \n",GetTickCount());
			Service_Set_Room_All_Mute* pMsg = reinterpret_cast<Service_Set_Room_All_Mute*>(st);
			pMsg->bResult = m_avManager.SetAllRoomSoundMute(pMsg->bMute);
		}
		break;
	case SERVICE_GET_ROOM_ALL_MUTE://获取所有房间是否静音
		{
			Trace("AVModule::Invoke - SERVICE_GET_ROOM_ALL_MUTE  time = %I64d \n",GetTickCount());
			Service_Get_Room_All_Mute* pMsg = reinterpret_cast<Service_Get_Room_All_Mute*>(st);
			pMsg->bResult = m_avManager.GetAllRoomSoundMute(pMsg->bMute);
		}
		break;
	case SERVICE_SET_ROOM_ALL_VOLUME://设置所有房间麦克风音量
		{
			Trace("AVModule::Invoke - SERVICE_SET_ROOM_ALL_VOLUME  time = %I64d \n",GetTickCount());
			Service_Set_Room_All_Volume* pMsg = reinterpret_cast<Service_Set_Room_All_Volume*>(st);
			m_avManager.SetMicRoomAllVolume(pMsg->iVolume);
		}
		break;
	case SERVICE_GET_ROOM_ALL_VOLUME://获取所有房间麦克风音量
		{
			Trace("AVModule::Invoke - SERVICE_GET_ROOM_ALL_VOLUME  time = %I64d \n",GetTickCount());
			Service_Get_Room_All_Volume* pMsg = reinterpret_cast<Service_Get_Room_All_Volume*>(st);
			pMsg->iVolume = m_avManager.GetMicRoomAllVolume();
		}
		break;
	case SERVICE_SET_MIC_GREATER://设置麦克风增强
		{
			Trace("AVModule::Invoke - SERVICE_SET_MIC_GREATER  time = %I64d \n",GetTickCount());
			Service_Set_Mic_Greater* pMsg = reinterpret_cast<Service_Set_Mic_Greater*>(st);
			pMsg->bReturn = m_avManager.SetMicGreater(pMsg->bEnable);

		}
		break;
	case SERVICE_GET_MIC_GREATER://获取麦克风是否加强
		{
			Trace("AVModule::Invoke - SERVICE_GET_MIC_GREATER  time = %I64d \n",GetTickCount());
			Service_Get_Mic_Greater* pMsg = reinterpret_cast<Service_Get_Mic_Greater*>(st);
			pMsg->bReturn = m_avManager.GetMicGreater(pMsg->bEnable);
		}
		break;
	case SERVICE_SET_VIEW_FFT_WAV://设置显示语音波形图
		{
			Trace("AVModule::Invoke - SERVICE_SET_VIEW_FFT_WAV  time = %I64d \n",GetTickCount());
			Service_Set_View_FFT_Wav* pMsg = reinterpret_cast<Service_Set_View_FFT_Wav*>(st);
			m_avManager.SetFFTView(pMsg->iRoomId,pMsg->iFromUin,pMsg->bEnable);
		}
		break;
	case SERVICE_SET_VIEW_FFT_ALL://设置显示语音波形图  全局设置
		{
			Trace("AVModule::Invoke - SERVICE_SET_VIEW_FFT_ALL  time = %I64d \n",GetTickCount());
			Service_Set_View_FFT_ALL* pMsg = reinterpret_cast<Service_Set_View_FFT_ALL*>(st);
			m_avManager.SetFFTALL(pMsg->bEnable);
		}
		break;
	case SERVICE_GET_VIEW_FFT_ALL://获取全局是否显示波形图
		{
			Trace("AVModule::Invoke - SERVICE_GET_VIEW_FFT_ALL  time = %I64d \n",GetTickCount());
			Service_Get_View_FFT_ALL* pMsg = reinterpret_cast<Service_Get_View_FFT_ALL*>(st);
			m_avManager.GetFFTALL(pMsg->bEnable);
		}
		break;
	case SERVICE_SET_VIEW_FFT_ME://设置自己是否显示波形图
		{
			Trace("AVModule::Invoke - SERVICE_SET_VIEW_FFT_ME  time = %I64d \n",GetTickCount());
			Service_Set_View_FFT_ME* pMsg = reinterpret_cast<Service_Set_View_FFT_ME*>(st);
			m_avManager.SetFFTME(pMsg->bEnable);
		}
		break;
	case SERVICE_GET_VIEW_FFT_ME://获取自己是否显示波形图
		{
			Trace("AVModule::Invoke - SERVICE_GET_VIEW_FFT_ME  time = %I64d \n",GetTickCount());
			Service_Get_View_FFT_ME* pMsg = reinterpret_cast<Service_Get_View_FFT_ME*>(st);
			m_avManager.GetFFTME(pMsg->bEnable);
		}
		break;
	case SERVICE_GET_VIEW_FFT_WAV://获取是否显示波形图
		{
			Trace("AVModule::Invoke - SERVICE_GET_VIEW_FFT_WAV  time = %I64d \n",GetTickCount());
			Service_Get_View_FFT_Wav* pMsg = reinterpret_cast<Service_Get_View_FFT_Wav*>(st);
			pMsg->bEnable = m_avManager.GetFFTView(pMsg->iRoomId,pMsg->iFromUin);
		}
		break;
	case SERVICE_NET_STATE_RUN_RATE://获取用户网络丢包率
		{
			Trace("AVModule::Invoke - SERVICE_NET_STATE_RUN_RATE  time = %I64d \n",GetTickCount());
			Service_Net_Start_Run_Rate* pMsg = reinterpret_cast<Service_Net_Start_Run_Rate*>(st);
			pMsg->fRate = m_avManager.GetNatLostRate();
		}
		break;
	case SERVICE_MIXER_SET_TOTAL_VOLUME://设置总音量
		{
			Trace("AVModule::Invoke - SERVICE_MIXER_SET_TOTAL_VOLUME  time = %I64d \n",GetTickCount());
			Service_Mixer_Set_Total_Volume* pMsg = reinterpret_cast<Service_Mixer_Set_Total_Volume*>(st);
			pMsg->bRet = m_mixerManager.SetMixerControlTotalVolume(pMsg->iVolume);
		}
		break;
	case SERVICE_MIXER_GET_TOTAL_VOLUME://获取总音量
		{
			Trace("AVModule::Invoke - SERVICE_MIXER_GET_TOTAL_VOLUME  time = %I64d \n",GetTickCount());
			Service_Mixer_Get_Total_Volume* pMsg = reinterpret_cast<Service_Mixer_Get_Total_Volume*>(st);
			pMsg->bRet = m_mixerManager.GetMixerControlTotalVolume(pMsg->iVolume);
		}
		break;
	case SERVICE_MIXER_SET_WAVOUT_VOLUME://设置伴音音量
		{
			Trace("AVModule::Invoke - SERVICE_MIXER_SET_WAVOUT_VOLUME  time = %I64d \n",GetTickCount());
			Service_Mixer_Set_Wavout_Volume* pMsg = reinterpret_cast<Service_Mixer_Set_Wavout_Volume*>(st);
			pMsg->bRet = m_mixerManager.SetMixerControlWavoutVolume(pMsg->iVolume);
		}
		break;
	case SERVICE_MIXER_GET_WAVOUT_VOLUME://获取伴音音量
		{
			Trace("AVModule::Invoke - SERVICE_MIXER_GET_WAVOUT_VOLUME  time = %I64d \n",GetTickCount());
			Service_Mixer_Get_Wavout_Volume* pMsg = reinterpret_cast<Service_Mixer_Get_Wavout_Volume*>(st);
			pMsg->bRet = m_mixerManager.GetMixerControlWavoutVolume(pMsg->iVolume);
		}
		break;
	case SERVICE_MIXER_SET_OUTMIC_VOLUME://设置音量控制中的麦克风音量
		{
			Trace("AVModule::Invoke - SERVICE_MIXER_SET_OUTMIC_VOLUME  time = %I64d \n",GetTickCount());
			Service_Mixer_Set_Outmic_Volume* pMsg = reinterpret_cast<Service_Mixer_Set_Outmic_Volume*>(st);
			pMsg->bRet = m_mixerManager.SetMixerControlOutmicVolume(pMsg->iVolume);
		}
		break;
	case SERVICE_MIXER_GET_OUTMIC_VOLUME://获取音量控制中的麦克风音量
		{
			Trace("AVModule::Invoke - SERVICE_MIXER_GET_OUTMIC_VOLUME  time = %I64d \n",GetTickCount());
			Service_Mixer_Get_Outmic_Volume* pMsg = reinterpret_cast<Service_Mixer_Get_Outmic_Volume*>(st);
			pMsg->bRet = m_mixerManager.GetMixerControlOutmicVolume(pMsg->iVolume);
		}
		break;
	case SERVICE_MIXER_SET_MIXER_VOLUME://设置立体声混音音量
		{
			Trace("AVModule::Invoke - SERVICE_MIXER_SET_MIXER_VOLUME  time = %I64d \n",GetTickCount());
			Service_Mixer_Set_Mixer_Volume* pMsg = reinterpret_cast<Service_Mixer_Set_Mixer_Volume*>(st);
			pMsg->bRet = m_mixerManager.SetMixerControlMixerVolume(pMsg->iVolume);
		}
		break;
	case SERVICE_MIXER_GET_MIXER_VOLUME://获取立体声混音音量
		{
			Trace("AVModule::Invoke - SERVICE_MIXER_GET_MIXER_VOLUME  time = %I64d \n",GetTickCount());
			Service_Mixer_Get_Mixer_Volume* pMsg = reinterpret_cast<Service_Mixer_Get_Mixer_Volume*>(st);
			pMsg->bRet = m_mixerManager.GetMixerControlMixerVolume(pMsg->iVolume);
		}
		break;
	case SERVICE_MIXER_SET_INMIC_VOLUME://设置录音控制中的麦克风音量
		{
			Trace("AVModule::Invoke - SERVICE_MIXER_SET_INMIC_VOLUME  time = %I64d \n",GetTickCount());
			Service_Mixer_Set_Inmic_Volume* pMsg = reinterpret_cast<Service_Mixer_Set_Inmic_Volume*>(st);
			pMsg->bRet = m_mixerManager.SetMixerControlInmicVolume(pMsg->iVolume);
		}
		break;
	case SERVICE_MIXER_GET_INMIC_VOLUME://获取录音控制中的麦克风音量
		{
			Trace("AVModule::Invoke - SERVICE_MIXER_GET_INMIC_VOLUME  time = %I64d \n",GetTickCount());
			Service_Mixer_Set_Inmic_Volume* pMsg = reinterpret_cast<Service_Mixer_Set_Inmic_Volume*>(st);
			pMsg->bRet = m_mixerManager.GetMixerControlInmicVolume(pMsg->iVolume);
		}
		break;
	case SERVICE_MIXER_SET_TOTAL_MUTE://设置总音量静音
		{
			Trace("AVModule::Invoke - SERVICE_MIXER_SET_TOTAL_MUTE  time = %I64d \n",GetTickCount());
			Service_Mixer_Set_Total_Mute* pMsg = reinterpret_cast<Service_Mixer_Set_Total_Mute*>(st);
			pMsg->bRet = m_mixerManager.SetMixerControlTotalMute(pMsg->bEnable);
		}
		break;
	case SERVICE_MIXER_GET_TOTAL_MUTE: //获取总音量静音
		{
			Trace("AVModule::Invoke - SERVICE_MIXER_GET_TOTAL_MUTE  time = %I64d \n",GetTickCount());
			Service_Mixer_Get_Total_Mute* pMsg = reinterpret_cast<Service_Mixer_Get_Total_Mute*>(st);
			pMsg->bRet = m_mixerManager.GetMixerControlTotalMute(pMsg->bEnable);
		}
		break;
	case SERVICE_MIXER_SET_WAVOUT_MUTE: //设置波形静音
		{
			Trace("AVModule::Invoke - SERVICE_MIXER_SET_WAVOUT_MUTE  time = %I64d \n",GetTickCount());
			Service_Mixer_Set_Wavout_Mute* pMsg = reinterpret_cast<Service_Mixer_Set_Wavout_Mute*>(st);
			pMsg->bRet = m_mixerManager.SetMixerControlWavoutMute(pMsg->bEnable);
		}
		break;
	case SERVICE_MIXER_GET_WAVOUT_MUTE://获取波形是否静音
		{
			Trace("AVModule::Invoke - SERVICE_MIXER_GET_WAVOUT_MUTE  time = %I64d \n",GetTickCount());
			Service_Mixer_Get_Wavout_Mute* pMsg = reinterpret_cast<Service_Mixer_Get_Wavout_Mute*>(st);
			pMsg->bRet = m_mixerManager.GetMixerControlWavoutMute(pMsg->bEnable);
		}
		break;
	case SERVICE_MIXER_SET_OUTMIC_MUTE://设置音量控制中的麦克风静音
		{
			Trace("AVModule::Invoke - SERVICE_MIXER_SET_OUTMIC_MUTE");
			Service_Mixer_Set_Outmic_Mute* pMsg = reinterpret_cast<Service_Mixer_Set_Outmic_Mute*>(st);
			pMsg->bRet = m_mixerManager.SetMixerControlOutmicMute(pMsg->bEnable);
		}
		break;
	case SERVICE_MIXER_GET_OUTMIC_MUTE://获取音量控制中的麦克风静音
		{
			Trace("AVModule::Invoke - SERVICE_MIXER_GET_OUTMIC_MUTE  time = %I64d \n",GetTickCount());
			Service_Mixer_Get_Outmic_Mute* pMsg = reinterpret_cast<Service_Mixer_Get_Outmic_Mute*>(st);
			pMsg->bRet = m_mixerManager.GetMixerControlOutmicMute(pMsg->bEnable);
		}
		break;
	case SERVICE_MIXER_SET_MIXER_MUTE://设置立体声混音静音
		{
			Trace("AVModule::Invoke - SERVICE_MIXER_SET_MIXER_MUTE  time = %I64d \n",GetTickCount());
			Service_Mixer_Set_Mixer_Mute* pMsg = reinterpret_cast<Service_Mixer_Set_Mixer_Mute*>(st);
			pMsg->bRet = m_mixerManager.SetMixerControlMixerMute(pMsg->bEnable);
		}
		break;
	case SERVICE_MIXER_GET_MIXER_MUTE://获取立体声混音静音
		{
			Trace("AVModule::Invoke - SERVICE_MIXER_GET_MIXER_MUTE  time = %I64d \n",GetTickCount());
			Service_Mixer_Get_Mixer_Mute* pMsg = reinterpret_cast<Service_Mixer_Get_Mixer_Mute*>(st);
			pMsg->bRet = m_mixerManager.GetMixerControlMixerMute(pMsg->bEnable);
		}
		break;
	case SERVICE_MIXER_SET_INMIC_MUTE://设置录音控制的麦克风静音
		{
			Trace("AVModule::Invoke - SERVICE_MIXER_SET_INMIC_MUTE  time = %I64d \n",GetTickCount());
			Service_Mixer_Set_Inmic_Mute* pMsg = reinterpret_cast<Service_Mixer_Set_Inmic_Mute*>(st);
			pMsg->bRet = m_mixerManager.SetMixerControlInmicMute(pMsg->bEnable);
		}
		break;
	case SERVICE_MIXER_GET_INMIC_MUTE://获取录音控制中的麦克风静音
		{
			Trace("AVModule::Invoke - SERVICE_MIXER_GET_INMIC_MUTE  time = %I64d \n",GetTickCount());
			Service_Mixer_Get_Inmic_Mute* pMsg = reinterpret_cast<Service_Mixer_Get_Inmic_Mute*>(st);
			pMsg->bRet = m_mixerManager.GetMixerControlInmicMute(pMsg->bEnable);
		}
		break;
	case SERVICE_MIXER_CONTROL_INIT://Mixer初始化
		{
			Trace("AVModule::Invoke - SERVICE_MIXER_CONTROL_INIT  time = %I64d \n",GetTickCount());
			Service_Mixer_Control_Init* pMsg = reinterpret_cast<Service_Mixer_Control_Init*>(st);
			m_mixerManager.Init(pMsg->hWnd);
		}
		break;
	case SERVICE_MIXER_SET_SING_MODE://设置唱歌模式
		{
			Trace("AVModule::Invoke - SERVICE_MIXER_SET_SING_MODE  time = %I64d \n",GetTickCount());
			Service_Mixer_Set_Sing_Mode* pMsg = reinterpret_cast<Service_Mixer_Set_Sing_Mode*>(st);
			pMsg->bRet = m_mixerManager.SetMixerControlSingMode();
		}
		break;
	case SERVICE_MIXER_GET_SING_MODE://获取唱歌模式
		{
			Trace("AVModule::Invoke - SERVICE_MIXER_GET_SING_MODE  time = %I64d \n",GetTickCount());
			Service_Mixer_Set_Sing_Mode* pMsg = reinterpret_cast<Service_Mixer_Set_Sing_Mode*>(st);
			pMsg->bRet = m_mixerManager.GetMixerControlSingMode(pMsg->bEnable);
		}
		break;
	case SERVICE_MIXER_SET_CHAT_MODE://设置与人聊天模式
		{
			Trace("AVModule::Invoke - SERVICE_MIXER_SET_CHAT_MODE  time = %I64d \n",GetTickCount());
			Service_Mixer_Set_Chat_Mode* pMsg = reinterpret_cast<Service_Mixer_Set_Chat_Mode*>(st);
			pMsg->bRet = m_mixerManager.SetMixerControlChatMode();
		}
		break;
	case SERVICE_MIXER_GET_CHAT_MODE://获取是否处于与人聊天模式
		{
			Trace("AVModule::Invoke - SERVICE_MIXER_GET_CHAT_MODE  time = %I64d \n",GetTickCount());
			Service_Mixer_Get_Chat_Mode* pMsg = reinterpret_cast<Service_Mixer_Get_Chat_Mode*>(st);
			pMsg->bRet = m_mixerManager.GetMixerControlChatMode(pMsg->bEnable);
		}
		break;
	case SERVICE_MIXER_SET_PLAY_MODE://设置在麦上放歌模式
		{
			Trace("AVModule::Invoke - SERVICE_MIXER_SET_PLAY_MODE  time = %I64d \n",GetTickCount());
			Service_Mixer_Set_Play_Mode* pMsg = reinterpret_cast<Service_Mixer_Set_Play_Mode*>(st);
			pMsg->bRet = m_mixerManager.SetMixerControlPlayMode();
		}
		break;
	case SERVICE_MIXER_GET_PLAY_MODE://获取在麦上放歌模式
		{
			Trace("AVModule::Invoke - SERVICE_MIXER_GET_PLAY_MODE  time = %I64d \n",GetTickCount());
			Service_Mixer_Get_Play_Mode* pMsg = reinterpret_cast<Service_Mixer_Get_Play_Mode*>(st);
			pMsg->bRet = m_mixerManager.GetMixerControlPlayMode(pMsg->bEnable);
		}
		break;
	case SERVICE_MIXER_GET_CURR_MODE://获取当前模式
		{
			Trace("AVModule::Invoke - SERVICE_MIXER_GET_CURR_MODE  time = %I64d \n",GetTickCount());
			Service_Mixer_Get_Curr_Mode* pMsg = reinterpret_cast<Service_Mixer_Get_Curr_Mode*>(st);
			EnMixerControlMode eMode;
			pMsg->bRet = m_mixerManager.GetMixerControlMode(eMode);
			pMsg->iMode = (unsigned int)eMode;
		}
		break;
	case SERVICE_MIXER_SET_WAVIN_VOLUME://设置音频输入设备音量
		{
			Trace("AVModule::Invoke - SERVICE_MIXER_SET_WAVIN_VOLUME  time = %I64d \n",GetTickCount());
			Service_Mixer_Set_WavIn_Volume* pMsg = reinterpret_cast<Service_Mixer_Set_WavIn_Volume*>(st);
			pMsg->bRet = m_mixerManager.SetMixerControlWavInVolume(pMsg->iVolume);
		}
		break;
	case SERVICE_MIXER_GET_WAVIN_VOLUME://获取音频输入设备音量
		{
			Trace("AVModule::Invoke - SERVICE_MIXER_GET_WAVIN_VOLUME  time = %I64d \n",GetTickCount());
			Service_Mixer_Get_WavIn_Volume* pMsg = reinterpret_cast<Service_Mixer_Get_WavIn_Volume*>(st);
			pMsg->bRet = m_mixerManager.GetMixerControlWavInVolume(pMsg->iVolume);
		}
		break;	
	case SERVICE_MIXER_GET_TOTAL_ENABLE://获取当前总音量是否可用
		{
			Trace("AVModule::Invoke - SERVICE_MIXER_GET_TOTAL_ENABLE  time = %I64d \n",GetTickCount());
			Service_Mixer_Get_Total_Enable* pMsg = reinterpret_cast<Service_Mixer_Get_Total_Enable*>(st);
			pMsg->bEnable = m_mixerManager.GetMixerControlTotalEnable();
		}
		break;
	case SERVICE_MIXER_GET_WAVOUT_ENABLE://获取伴音是否可用
		{
			Trace("AVModule::Invoke - SERVICE_MIXER_GET_WAVOUT_ENABLE  time = %I64d \n",GetTickCount());
			Service_Mixer_Get_WavOut_Enable* pMsg = reinterpret_cast<Service_Mixer_Get_WavOut_Enable*>(st);
			pMsg->bEnable = m_mixerManager.GetMixerControlWavoutEnable();
		}
		break;
	case SERVICE_MIXER_GET_OUTMIC_ENABLE://获取音量控制中的麦克风是否可用
		{
			Trace("AVModule::Invoke - SERVICE_MIXER_GET_OUTMIC_ENABLE  time = %I64d \n",GetTickCount());
			Service_Mixer_Get_OutMic_Enable* pMsg = reinterpret_cast<Service_Mixer_Get_OutMic_Enable*>(st);
			pMsg->bEnable = m_mixerManager.GetMixerControlOutmicEnable();
		}
		break;
	case SERVICE_MIXER_GET_MIXER_ENABLE://获取立体声混音是否可用
		{
			Trace("AVModule::Invoke - SERVICE_MIXER_GET_MIXER_ENABLE  time = %I64d \n",GetTickCount());
			Service_Mixer_Get_Mixer_Enable* pMsg = reinterpret_cast<Service_Mixer_Get_Mixer_Enable*>(st);
			pMsg->bEnable = m_mixerManager.GetMixerControlMixerEnable();
		}
		break;
	case SERVICE_MIXER_GET_INMIC_ENABLE://获取音频输入设备中音量大小是否可用
		{
			Trace("AVModule::Invoke - SERVICE_MIXER_GET_INMIC_ENABLE  time = %I64d \n",GetTickCount());
			Service_Mixer_Get_InMic_Enable* pMsg = reinterpret_cast<Service_Mixer_Get_InMic_Enable*>(st);
			pMsg->bEnable = m_mixerManager.GetMixerControlInmicEnable();
		}
		break;
	case SERVICE_MIXER_GET_WAVIN_ENABLE://获取调整麦克风音量大小是否可用
		{
			Trace("AVModule::Invoke - SERVICE_MIXER_GET_WAVIN_ENABLE  time = %I64d \n",GetTickCount());
			Service_Mixer_Get_WavIn_Enable* pMsg = reinterpret_cast<Service_Mixer_Get_WavIn_Enable*>(st);
			pMsg->bEnable = m_mixerManager.GetMixerControlWavInEnable();
		}
		break;
	case SERVICE_AV_FLUSH_VIDEO_DEV://刷新摄像头设备  用于打开虚拟摄像头
		{
			Trace("AVModule::Invoke - SERVICE_AV_FLUSH_VIDEO_DEV  time = %I64d \n",GetTickCount());
			Service_Av_Flush_Video_Dev* pMsg = reinterpret_cast<Service_Av_Flush_Video_Dev*>(st);
			pMsg->bRet = m_avManager.flushVideoDev(pMsg->iDev);
		}
		break;
	case SERVICE_AV_CHANGE_RELAY_SVR://变更中转服务器IP
		{
			Trace("AVModule::Invoke - SERVICE_AV_CHANGE_RELAY_SVR  time = %I64d \n",GetTickCount());
			Service_Av_Change_Relay_Svr* pMsg = reinterpret_cast<Service_Av_Change_Relay_Svr*>(st);
			pMsg->bret = m_avManager.ChangeRelayServer(pMsg->iRoomId,pMsg->ip,pMsg->sport);
		}
		break;

	case SERVICE_SET_ROOM_MIC_MUTE://设置房间麦克风是否静音 所有房间
		{
			Trace("AVModule::Invoke - SERVICE_SET_ROOM_MIC_MUTE  time = %I64d \n",GetTickCount());
			Service_Set_Room_Mic_Mute* pMsg = reinterpret_cast<Service_Set_Room_Mic_Mute*>(st);
			pMsg->bResult = m_avManager.SetAllRoomMicMute(pMsg->bMute);
		}
		break;
	case SERVICE_AV_NET_STATUS_ALL://获取房间音视频丢包率、数据包个数等
		{
			Trace("AVModule::Invoke - SERVICE_AV_NET_STATUS_ALL  time = %I64d \n",GetTickCount());
			Service_Av_Net_Status_All_Ex* pMsg = reinterpret_cast<Service_Av_Net_Status_All_Ex*>(st);
			m_avManager.GetAVNetStatus(*pMsg);
		}
		break;
	case SERVICE_GET_ROOM_MIC_MUTE://获取房间麦克风静音
		{
			Trace("AVModule::Invoke - SERVICE_GET_ROOM_MIC_MUTE  time = %I64d \n",GetTickCount());
			Service_Get_Room_Mic_Mute* pMsg = reinterpret_cast<Service_Get_Room_Mic_Mute*>(st);
			pMsg->bResult = m_avManager.GetAllRoomMicMute(pMsg->bMute);
		}
		break;
	case SERVICE_AV_VIDEO_PAINT_LOCAL://绘制本地图像背景
		{
			Trace("AVModule::Invoke - SERVICE_AV_VIDEO_PAINT_LOCAL  time = %I64d \n",GetTickCount());
			Service_Av_Video_Paint_Local* pMsg = reinterpret_cast<Service_Av_Video_Paint_Local*>(st);
			pMsg->bRet = m_avManager.PaintLocal(pMsg->iRoomId,pMsg->hWnd);
		}
		break;
	case SERVICE_AV_VIDEO_PAINT_REMOTE://绘制远程图像背景
		{
			Trace("AVModule::Invoke - SERVICE_AV_VIDEO_PAINT_REMOTE  time = %I64d \n",GetTickCount());
			Service_Av_Video_Paint_Remote* pMsg = reinterpret_cast<Service_Av_Video_Paint_Remote*>(st);
			pMsg->bRet = m_avManager.PaintRemote(pMsg->iRoomId,pMsg->iFromUin,pMsg->hWnd);
		}
		break;
	case SERVICE_AV_PRESS_AUDIO_CFG://音频参数设置
		{
			Trace("AVModule::Invoke - SERVICE_AV_PRESS_AUDIO_CFG  time = %I64d \n",GetTickCount());
			Service_Av_Press_Audio_Cfg* pMsg = reinterpret_cast<Service_Av_Press_Audio_Cfg*>(st);
			pMsg->bret = m_avManager.SetAudioMode(pMsg->mode);
		}
		break;
	case SERVICE_AV_KEY_SET_CFG://设置音视频key
		{
			Trace("AVModule::Invoke - SERVICE_AV_KEY_SET_CFG  time = %I64d \n",GetTickCount());
			Service_Av_Key_Set_Cfg* pMsg = reinterpret_cast<Service_Av_Key_Set_Cfg*>(st);
			pMsg->bret = m_avManager.SetAvKey(pMsg->iRoomId,pMsg->iKey);
		}
		break;
	case SERVICE_VIDEO_GET_CURR_STATUS://获取摄像头当前状态
		{
			Trace("AVModule::Invoke - SERVICE_VIDEO_GET_CURR_STATUS  time = %I64d \n",GetTickCount());
			Service_Video_Get_Curr_Status* pMsg = reinterpret_cast<Service_Video_Get_Curr_Status*>(st);
			pMsg->eStatus = m_avManager.GetCamerStatus(pMsg->iDevice);
		}
		break;
	case SERVICE_VIDEO_SET_SHAP_ENABLE://设置视频显示是否锐化
		{
			Trace("AVModule::Invoke - SERVICE_VIDEO_SET_SHAP_ENABLE  time = %I64d \n",GetTickCount());
			Service_Video_Set_Shapen_Enable* pMsg = reinterpret_cast<Service_Video_Set_Shapen_Enable*>(st);
			m_avManager.SetVideoSharpen(pMsg->bEnable);
		}
		break;
	default:
		Assert(0);
		break;
	}
	return 0;
}

bool  AVModule::sendEvent(void* pEvent, uint32 iType)
{
	if (!m_CircleBuffer.push_back(pEvent, sizeof(Event), iType))
	{
		Assert(0);
		return false;
	}

	return true;
}

wchar_t const * const  AVModule::GetModuleName()
{
	return g_AVModuleName;
}

//Model内通知上层状态消息
void  AVModule::processCircleBuffer()
{
	UInt32 iLen  = 0;
	UInt32 iType = 0;
	void*  pMsg  = NULL;
	while(m_CircleBuffer.pop_front(pMsg, iLen, iType))
	{
		switch (iType)
		{
		case EVENT_AV_CONN_START:
		case EVENT_AV_CONN_STOP:
		case EVENT_AV_AUDIO_FAIL:
		case EVENT_AV_VIDEO_FAIL:
			Assert(sizeof(Event)==iLen);
			m_pModuleManager->PushEvent(*static_cast<Event*>(pMsg));
			break;
		default:
			Assert(0);
			break;
		}

		delete (Event*)pMsg; 
		pMsg = NULL;
		iType= 0;
		iLen = 0;
	}
}



