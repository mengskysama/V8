#pragma once

//#include "Common.h"
#include "Mutex.h"

#include <map>
#include <set>
#include <vector>
#include <algorithm>
#include "AudioTransport.h"
#include "VideoInputEngine.h"
#include "Mixer/MixerManager.h"

#define   MAX_VIDEO_PACKET_LEN                 (548-VIDEO_PIECE_PACKET_BODY_LEN-4)

#define  AUDIODSOUND

class AVEngine : public AudioTransport
{
public:

	AVEngine();
	~AVEngine();
	
	bool                          InitAVEngine();
	bool						  InitVideoEngine(UInt32 iDeviceId); //初始化指定的摄像头设备，用在房间选取了不同的摄像头设备时候调用。

	void                          DestroyAVEngine();
	void                          DestroyAVInputEngine(UInt32 iRoomId);
	void                          DestroyAVOutputEngine(UInt32 iRoomId);

	bool                          IsStartAVEngine();
	bool                          IsStartAVInputEngine(SInt32 iRoomId);
	bool						  IsVideoDevInit(UInt32 iDeviceId);
	bool                          flushVideoDev(UInt32 iDeviceId);
	bool						  PaintLocal(UInt32 iRoomId,HWND hWnd);
	bool						  PaintRemote(UInt32 iRoomId,UInt32 iFromUin,HWND hWnd);

	bool						  StartVideoTest(int iDeviceId,HWND hWnd,UInt32& iResult);
	bool						  CloseVideoTest(int iDeviceId,HWND hWnd);

	void                          AddAudioLogicChannel(UInt32 iRoomId, UInt8 cLogicID);
	void                          DelAudioLogicChannel(UInt32 iRoomId, UInt8 cLogicID);
	void						  SetAudioDevice(UInt32 iInputDev,UInt32 iOutputDev);
	void						  AddAudioLogicOutput(UInt32 iRoomId,UInt32 iFromUin);
	void						  DelAudioLogicOutput(UInt32 iRoomId,UInt32 iFromUin);
	void						  SafeAudioInputStop();
	Enum_Camera_Status			  GetCamerStatus(int iDevice);
	bool						  GetAVNetStatus(UInt32 iRoomId,Service_Av_Net_Status_All& sansa);

	void						  ClearAudioLogicChannel(UInt32 iRoomId);
	void						  ClearAudioLogicChannelAll();
	void						  ClearAudioLogicOutput(UInt32 iRoomId);
	void						  ClearAudioLogicOutputAll();

	void						  PauseRoomAudio(UInt32 iRoomId);
	void						  ContinueRoomAudio(UInt32 iRoomId);
	bool						  IsRoomAudioPause(UInt32 iRoomId);

	void					      PauseRemoteAudio(UInt32 iRoomId,UINT32 iFromUin);
	void						  ContinueRemoteAudio(UInt32 iRoomId,UINT32 iFromUin);

	void                          AddVideoLogicChannel(UInt32 iRoomId, HWND localHwnd,enuVIDEO_TYPE enType);
	void                          DelVideoLogicChannel(UInt32 iRoomId, HWND localHwnd);
	bool						  IsVideoDecoderEffact(UInt32 iRoomId, HWND localHwnd);
	bool						  SetVideoDecoderEffect(UInt32 iRoomId, HWND localHwnd,bool bEnable);
	void						  DelVideoLogicChannelRoom(UInt32 iRoomId);
	void						  DelVideoLogicChannelAll();
	void						  ClearVideoOutputRoom(UInt32 iRoomId);
	void						  ClearVideoOutputRoomAll();
	void						  ClearVideoInputAppendRoom(UInt32 iRoomId);
	void						  ClearVideoInputAppendRoomAll();
	
	void						  AddVideoPartnerHandle(UInt32 iRoomId,UInt32 iDestUin,HWND partnerHwnd, UInt32 iWidth, UInt32 iHeight, SInt32 iQuality,UInt32 iVideoMode,enuVIDEO_TYPE enType);
	bool						  ChangePartnerHwnd(UInt32 iRoomId,UInt32 iDesUin, HWND hPartnerWnd, UInt32 iWidth, UInt32 iHeight, HWND hOldWnd);
	bool						  ChangeLocalHwnd(UInt32 iRoomId, HWND hWnd, HWND hOldWnd);
	void						  DelVideoPartnerHandle(UInt32 iRoomId,UInt32 iDestUin, HWND partnerHwnd);
	void						  PrepareWavData(UInt32 iRoomId,UInt32 iFromUin,byte* pData,UInt32 iLen);

	void                          SendAudioData(const void* data,int len,bool bRTP = false);		
	void                          RecvAudioData(UInt32 iRoomId, UInt32 iDesUin, const void* data, unsigned int  len,unsigned int iSeq,bool bRTP = false);

	void                          SendVideoData(UInt8* pData, UInt32 iDataLen, unsigned char cKey, UInt32 iSeqNum, UInt32 iKeySeqNum, std::vector<HWND>& vLocalHwnds);
	void                          RecvVideoData(UInt32 iRoomId, UInt32 iDesUin, UInt8* pData, UInt32 iDataLen, UInt8 cKey, UInt32 iSeqNum, UInt32 iKeySeqNum);

	bool						  IsAtLeastOneChannelReady(std::vector<HWND>& vLocalHwnds);			//至少一个通道准备好

	void						  TestAudio(bool bTest);

	void						  SetSpeakVolume(UInt32 iRoomId, int iVolume);
	void						  SetSpeakerRoomVolume(UInt32 iRoomId,UInt32 iVolume);
	UINT32                        GetSpeakerRoomVolume(UInt32 iRoomId);
	void						  SetMicVolume(UInt32 iRoomId, int iVolume);
	void						  SetSysPlayerVolume(UInt32 iRoomId, int iVolume);
	bool                          SetRoomSoundMute(UInt32 iRoomId, bool bMute);
	bool						  GetRoomSoundMute(UInt32 iRoomId,bool& bMute);
	bool						  ClearRoomSoundMute(UInt32 iRoomId);
	
	bool						  SetAllRoomSoundMute(bool bMute);
	bool						  GetAllRoomSoundMute(bool& bMute);

	bool						  GetAllRoomMicMute(bool& bMute);
	bool						  SetAllRoomMicMute(bool bMute);

	void						  SetMicRoomAllVolume(UInt32 iVolume);
	UInt32						  GetMicRoomAllVolume();

	float						  GetNatLostRate();
	bool						  GetShowFFTWav(UInt32 iRoomId,UInt32 iFromUin);
	void						  SetShowFFTWav(UInt32 iRoomId,UInt32 iFromUin,bool bEnable);
	void						  SetFFTALL(bool bEnable);
	void						  GetFFTALL(bool& bEnable);
	void						  SetFFTME(bool& bEnable);
	void						  GetFFTME(bool& bEnable);
	bool						  SetMicGreater(bool bEnable);
	bool						  GetMicGreater(bool& bEnable);
	bool						  IsVideoVirtualDevice(int iDevId);
	
	//设置音频编码参数
	bool						  SetMode(enuROOM_AV_AUDIO_MODE mode);
	
private:

	void                          GetAllVideoUinChannel(std::vector<HWND>& vLocalHwnds, std::vector<UInt32>& vAllRoomId);
private:
	struct ST_AUDIO_PROPERTY 
	{
		static std::vector<UInt8> m_sVecLogicChannel; //保存公共的LogicId 若为空的时候，即没有房间在使用该隐僻设备，所有房间公用一个Vec
		bool bPaused; //本房间的语音是否暂停对外发送  采用不发送数据方式 为以后扩展多房间可以同时语音，一个房间语音暂停，其他房间还可以继续发送语音数据包 互不影响
	
		ST_AUDIO_PROPERTY()
			:bPaused(true)  //默认上麦声音暂停
		{
		}

		ST_AUDIO_PROPERTY(UInt8 u8LogicChannel)
			:bPaused(true)  //默认上麦声音暂停
		{
			std::vector<UInt8>::iterator iter;
			if ( (iter=find(m_sVecLogicChannel.begin(),m_sVecLogicChannel.end(),u8LogicChannel)) == m_sVecLogicChannel.end())
			{
				m_sVecLogicChannel.push_back(u8LogicChannel);
			}
		}
		void RemoveLogicChannel(UInt8 u8LogicChannel)
		{
			std::vector<UInt8>::iterator iter;
			for (iter=m_sVecLogicChannel.begin();iter!=m_sVecLogicChannel.end();)
			{
				if (u8LogicChannel == *iter)
				{
					iter = m_sVecLogicChannel.erase(iter);
				}
				else
				{
					++iter;
				}
			}
		}
		void RemoveAllLogicChannel()
		{
			m_sVecLogicChannel.clear();
		}
		void AddLogicChannel(UInt8 u8LogicChannel)
		{
			std::vector<UInt8>::iterator iter;
			if ( (iter=find(m_sVecLogicChannel.begin(),m_sVecLogicChannel.end(),u8LogicChannel)) == m_sVecLogicChannel.end())
			{
				m_sVecLogicChannel.push_back(u8LogicChannel);
			}
		}
		bool IsLogicEmpty(){return m_sVecLogicChannel.empty();};
		void PauseAudio(){bPaused = true;}
		void ContinueAudio(){bPaused = false;}
		bool IsPaused(){return bPaused;}
	};
	Mutex										m_AudioMutex;//m_allAudioLogicChannel 房间线程
	std::map<UInt32,ST_AUDIO_PROPERTY>			m_allAudioLogicChannel;

	Mutex                                        m_VideoMutex; //房间线程 buffer线程  m_mapLocalHwndtoChannel  VideoOutputEngine::m_VOutputEngineMap
	std::map<HWND, UInt32 >						 m_mapLocalHwndtoChannel;
	
	std::map<UInt32,bool>						 m_mapRoomMux; //设置单个房间静音
	bool										 m_bRoomMux; //所有房间是否静音
public:
	bool                                         m_bStopVideoEngine;
	UInt32										 m_iStopRoomId;
};

extern AVEngine           g_AVEngine;



