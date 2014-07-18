#pragma once
#include "Common.h"
#include "MixerDefine.h"
#include <rpcsal.h>
#include <commctrl.h>
#include <mmdeviceapi.h>
#include <endpointvolume.h>
#include <devicetopology.h>
#include <Audioclient.h>

//Vista Win7 麦克风 扬声器 操作类
class CMixerOther
{
public:
	CMixerOther(void);
	~CMixerOther(void);
	
	//获取麦克风音量
	BOOL GetMixerMicVolume(UInt32& iVolume);
	//设置麦克风音量
	BOOL SetMixerMicVolume(UInt32 iVolume);
	//获取扬声器音量
	BOOL GetMixerSpeakerVolume(UInt32& iVolume);
	//设置扬声器音量
	BOOL SetMixerSpeakerVolume(UInt32 iVolume);\
	//获取麦克风是否静音
	BOOL GetMixerMicMute(BOOL& bMute);
	//设置麦克风静音
	BOOL SetMixerMicMute(BOOL bMute);
	//获取扬声器是否静音
	BOOL GetMixerSpeakerMute(BOOL& bMute);
	//设置扬声器是否静音
	BOOL SetMixerSpeakerMute(BOOL bMute);
	//设置麦克风加强值  0~255
	BOOL SetMixerMicBoostVolume(UInt32 iVolume);
	//获取麦克风加强值 0~255
	BOOL GetMixerMicBoostVolume(UInt32& iVolume);
	//设置麦克风加强 TRUE FALSE  设置加强增益为 0 OR MAX
	BOOL SetMixerMicBoostEnable(BOOL bEnable);
	//获取麦克风是否加强 增强Db == 0  没有加强 >0  加强
	BOOL GetMixerMicBoostEnable(BOOL& bEnable);

	//获取最大音量
	DWORD GetMaxVolume();
public:
	IMMDevice *m_pDevice;
private:
	EnOsType m_OsType; //操作系统类型
	IDeviceTopology *m_pDeviceTopology;
	IAudioEndpointVolume *m_pEndptVolume;
	IMMDeviceEnumerator *m_pEnumerator;
	IAudioVolumeLevel *m_pVolumeLevel;
	GUID m_Guid;
	float m_fMin,m_fMax,m_fStep;

	//获取操作系统类型
	EnOsType GetOsType();
	BOOL UnInitEndPoint();//卸载
	BOOL InitEndPoint(EnAudioType enType); //初始化
	BOOL SetCaptureDeviceSelect(IMMDevice *pEndptDev); //选择指定麦克风设备
	BOOL SetMicLevel(float fValue);
	BOOL GetMicLevel(float& fValue);
	BOOL GetMicLevelRange();
	HRESULT GetMicrophoneBoostLevel(IMMDevice *pEndptDev, IAudioVolumeLevel** ppVolumeLevel); //获取麦克风加强
};

extern CMixerOther g_mixerOther;