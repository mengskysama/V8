#pragma once

#include "Common.h"
#include "AvDefine.h"

#ifdef CLIENT_TEST
	#ifdef AV_EXPORTS
		#define AV_ENGINE_API __declspec(dllexport)
	#else
		#define AV_ENGINE_API __declspec(dllimport)
	#endif
#else
	#define AV_ENGINE_API                        
#endif

class AV_ENGINE_API AVManager
{
public:
	AVManager(void);
	~AVManager(void);

	//配置采集视频参数 不启动摄像头 直到第一个房间使用摄像头时才采集
	bool                          ConfigVideoParam(unsigned int iWidth, unsigned int iHeight, unsigned int iQuality, int iDeviceID,unsigned int iVideoMode);
	//去除所有房间和该摄像头设备的关联，并关闭摄像头。
	void                          FinishVideoParam(int iDeviceID);
	//房间添加本地显示窗口句柄，若是摄像头没有开启，则开启摄像头开始采集
	bool						  StartRoomVideo(UInt32 iRoomId,HWND localHwnd,int iDeviceID,enuVIDEO_TYPE enType = VIDEO_PRIV_MIC);
	//房间移除本地显示窗口句柄，若是摄像头只有本房间在用，本房间退出后，将直接关闭摄像头采集
	void						  CloseRoomVideo(UInt32 iRoomId,HWND localHwnd,int iDeviceID);
	//开启房间自己的语音，公麦、私麦、私聊语音开始发送数据准备 
	bool                          StartRoomAudio(UInt32 iRoomId, const UInt8& cLogicID = 0);
	//关闭房间自己的语音，公麦、私麦、私聊语音关闭，若是没有其他房间依赖音频设备，将关闭音频采集    语音设备只有一个 摄像头设备可能有多个。
	bool						  CloseRoomAudio(UInt32 iRoomId, const UInt8& cLogicID = 0);
	//设置音频输入、输出设备的	
	bool						  SetAudioDevice(UInt32 iInputDev,UInt32 iOutputDev);			  
	//和中转服务器建立连接 心跳包保持连通
	bool						  StartRoomConn(UInt32 iRoomId);//房间开始开启UDP连接，开始监听
	//房间退出时，关闭房间UDP连接
	bool						  CloseRoomConn(UInt32 iRoomId);//房间退出，关闭UDP连接
	//更改用户显示窗口
	bool						  ChangePartnerHwnd(UInt32 iRoomId,UInt32 iDesUin, HWND hPartnerWnd, UInt32 iWidth, UInt32 iHeight, HWND hOldWnd);
	//更改本地自己显示的窗口
	bool						  ChangeLocalHwnd(UInt32 iRoomId, HWND hLocalNewWnd, HWND hOldWnd);
	//设置本地视频显示为发送数据解码后的效果
	bool						  ChangeLocalEffact(UInt32 iRoomId,HWND localHwnd,bool bEnable);
	//是否设置了本地显示远程图像效果
	bool						  IsLocalEffact(UInt32 iRoomId,HWND localHwnd);
	//关闭房间所有语音， 用户房间退出时，清理所有的本房间建立的语音逻辑通道等
	bool						  ClearRoomAudio(UInt32 iRoomId);
	//软件退出时清理所有房间建立的逻辑通道及关闭语音
	bool						  ClearAllRoomAudio();
	//关闭房间所有视频
	bool						  ClearRoomVideo(UInt32 iRoomId);
	//关闭所有房间的摄像头
	bool						  ClearAllRoomVideo();
	//摄像头设备是否已经初始化
	bool						  IsVideoDevInit(UInt32 iDeviceId);
	//绘制本地背景图像
	bool						  PaintLocal(UInt32 iRoomId,HWND hWnd);
	//绘制远程背景图像
	bool						  PaintRemote(UInt32 iRoomId,UInt32 iFromUin,HWND hWnd);
	//获取摄像头状态
	Enum_Camera_Status			  GetCamerStatus(int iDevice);
	//获取房间网络状态
	bool						  GetAVNetStatus(Service_Av_Net_Status_All_Ex& sa);

	//iDeviceID 需传入  获取指定摄像头设备的参数
	void                          GetVideoParam(const UInt32 &iDeviceID, UInt32 &iWidth, UInt32 &iHeight, UInt32 &iQuality);
	//添加接收房间公麦、私麦、私聊语音 
	bool                          RecvRoomAudioOpp(__int64 lSymbolTime, UInt32 iRoomId, UInt32 iFromUin, UInt8& cLogicID);
	//添加接收房间公麦、私麦、私聊视频
	bool                          RecvRoomVideoOpp(__int64 lSymbolTime, UInt32 iRoomId, UInt32 iFromUin, UInt8& cLogicID,HWND partnerHwnd, UInt32 iWidth, UInt32 iHeight, UInt32 iQuality,UInt32 iVideoMode,enuVIDEO_TYPE enType=VIDEO_PRIV_MIC);
	//对方离开房间，下麦、停止语音聊天
	void						  CloseRoomAudioOpp(UInt32 iRoomId, UInt32 iFromUin, UInt8& cLogicID);
	//对方离开房间，下麦、停止视频聊天
	void						  CloseRoomVideoOpp(UInt32 iRoomId, UInt32 iFromUin, UInt8& cLogicID,HWND partnerHwnd);

	//暂停/继续本地视频 暂停时不继续发数据包 对方将接收不到数据
	void                          PauseLocalVideo(UInt32 iRoomId,HWND selfHwnd);
	void                          ContinueLocalVideo(UInt32 iRoomId,HWND selfHwnd);

	//暂停/继续对方视频，继续接收但是不显示出来
	void                          PauseRemoteVideo(UInt32 iRoomId, UInt32 iFromUin, HWND partnerHwnd);
	void                          ContinueRemoteVideo(UInt32 iRoomId, UInt32 iFromUin, HWND partnerHwnd);
	
	//暂停/继续本地语音
	void                          PauseLocalAudio(UInt32 iRoomId);
	void                          ContinueLocalAudio(UInt32 iRoomId);

	//暂停/继续对方语音 
	void						  PauseRemoteAudio(UInt32 iRoomId, UInt32 iFromUin);
	void						  ContinueRemoteAudio(UInt32 iRoomId, UInt32 iFromUin);

	//是否可以拍照
	bool                          IsPhotoableLocalHwnd(UInt32 iRoomId, HWND localHwnd);
	bool                          IsPhotoableRemoteHwnd(UInt32 iRoomId, UInt32 iFromUin, HWND partnerHwnd);

	//指定房间是否正在音视频
	bool                          IsStartAVEngine(UInt32 iRoomId);

	//判断指定设备是否为虚拟视频设备
	bool						  IsVideoVirtualDevice(int iDevId);
	
	//设置房间媒体服务器IP Port
	bool						  ChangeRelayServer(UInt32 iRoomId,UInt32 iRelayIP,UInt16 sRelayPort);

	//------------------------------------设置软音频 输入\输出
	//音量控制 设置扬声器音量
	void                          SetSpeakerVolume(UInt32 iRoomId,UInt32 iVolume);
	//获取音频输出音量
 	UInt32						  GetSpeakerVolume();
	//获取指定房间音量
	UInt32						  GetSpeakerRoomVolume(UInt32 iRoomId);
	//设置指定房间音量
	void                          SetSpeakerRoomVolume(UInt32 iRoomId,UInt32 iVolume);
	//设置房间麦克风音量  所有房间
	void						  SetMicRoomAllVolume(UInt32 iVolume);
	//获取房间麦克风音量  所有房间
	UInt32						  GetMicRoomAllVolume();
	//设置房间是否静音
	bool						  SetRoomSoundMute(UInt32 iRoomId, bool bMute);
	//获取房间是否静音
	bool						  GetRoomSoundMute(UInt32 iRoomId, bool& bMute);
	//设置所有房间是否静音
	bool						  SetAllRoomSoundMute(bool bMute);
	//获取所有房间是否静音
	bool						  GetAllRoomSoundMute(bool& bMute);
	//获取所有房间麦克风是否静音
	bool						  GetAllRoomMicMute(bool& bMute);
	//设置所有房间麦克风是否静音
	bool						  SetAllRoomMicMute(bool bMute);
	//------------------------------------设置系统音频 输入\输出
	//设置系统音频输出设备音量
	void						  SetSysPlayerVolume(UInt32 iRoomId,UInt32 iVolume);
	//获取系统音频输出设备音量
	UInt32						  GetSysPlayerVolume();
	//设置麦克风音量
	void                          SetMicVolume(UInt32 iRoomId, UInt32 iVolume);
	//获取麦克风音量
	UInt32						  GetMicVolume();
	// 设置静音 
	bool						  SetMicInputMute(UInt32 iRoomId, bool bMute);
	//获取麦克风是否静音
	bool						  GetMicInputMute(UInt32 iRoomId, bool& bMute);
	//设置系统扬声器是否静音
	bool						  SetSysPlayerMute(UInt32 iRoomId, bool bMute);
	//获取系统扬声器是否静音
	bool						  GetSysPlayerMute(UInt32 iRoomId, bool& bMute);


	//设置音频测试
 	void                          CheckSoundDevice(bool bOpen); 
	//获取扬声器 麦克风音强
	void                          GetSoundLevel(UInt32 &iOutput, UInt32 &iInput);
	//设置音频输入设备  播放设备
	bool                          SetSoundDevices(int iRecordIndex, int iPlayIndex);
	//获取音频输入设备名称
	bool                          GetRecordDeviceName(int iIndex, wchar_t *pDeviceName, int iNameLen);
	//获取音频播放设备名称
	bool						  GetPlayoutDeviceName(int iIndex, wchar_t *pDeviceName, int iNameLen);
	#define DEVICE_NAME_LEN                   128
	#define MAX_VIDEO_DEVICE_NUM               10
	//获取所有音频设备
	void						  GetAudioDeviceList(int &iDeviceNum, wchar_t pDeviceName[][DEVICE_NAME_LEN],bool bInput);
	//获取所有的视频设备
	void                          GetVideoDeviceList(int &iDeviceNum, wchar_t pDeviceName[][DEVICE_NAME_LEN]);
	//获取所有视频设备详细信息
	void						  GetVideoDeviceInfoList(int &iDeviceNum,Service_Video_Info struDeviceInfo[]);
	//显示视频属性窗口
	bool                          ShowVideoAdjustDlg(HWND hParWnd, unsigned int iDeviceID);
	//关闭所有没有使用的摄像头设备
	void                          OperateVideoEngine();
	//获取音频输入 输出设备的数量
	bool                          GetNumOfSoundDevices(int &iPlayNum, int &iRecordNum);
	//安全关闭音频  关闭测试开启的音频设备
	void						  SafeAudioInputStop();
	//开启视频测试设备
	bool						  StartVideoTest(int iDeviceId,HWND hWnd,UInt32& iResult);
	//关闭视频测试设备
	bool						  CloseVideoTest(int iDeviceId,HWND hWnd);
	//获取网络丢包率
	float						  GetNatLostRate();
	//获取是否开启麦克风加强
	bool						  GetMicGreater(bool& bEnable);
	//设置麦克风加强
	bool						  SetMicGreater(bool bEnable);
	//是否设置了显示波形
	bool						  GetFFTView(UInt32 iRoomId,UInt32 iFromUin);
	//设置显示波形图
	void						  SetFFTView(UInt32 iRoomId,UInt32 iFromUin,bool bEnable);
	//设置是否显示波形图 全局
	void						  SetFFTALL(bool bEnable);
	//获取是否显示波形图  全局
	void						  GetFFTALL(bool& bEnable);
	//设置自己是否显示波形图
	void						  SetFFTME(bool bEnable);
	//获取自己是否显示波形图
	void						  GetFFTME(bool& bEnable);

	//刷新视频设备 仅仅打开再关闭
	bool                          flushVideoDev(unsigned int iDev);
	//设置音频编码参数
	bool						  SetAudioMode(enuROOM_AV_AUDIO_MODE mode);
	//设置音视频key
	bool						  SetAvKey(unsigned int iRoomId,unsigned int iKey);
	//设置视频显示是否锐化
	bool						  SetVideoSharpen(bool bEnable);
};
