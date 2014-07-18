#pragma once
#include "51ktvtype.h"
#include <vector>
#include "..\RoomModule.h"
#include <map>
#include "..\..\CommonDll\RC.h"
#include "avdefine.h"
#include "..\..\CommonDll\Mutex.h"

using namespace std;

//用户房间语音视频状态
enum enuROOM_AV_STATE
{
	ROOM_AV_NONE = 0, //没有上麦、私聊等
	ROOM_AV_AUDIO, //私聊 仅语音
	ROOM_AV_AV //上麦 开视频和语音
};

//被接受逻辑连接状态 对方
enum enuROOM_AV_USER_STATE
{
	ROOM_AV_OPP_NONE = 0, 
	ROOM_AV_OPP_AUDIO,
	ROOM_AV_OPP_VIDEO
};

//保存被接收视频语音方数据
struct strucROOMAVUSER:public RCObject
{
	uint32 iFromUin;
	uint8 cLogicId;
	HWND hWndPartner;
	enuROOM_AV_USER_STATE eState;
};

#define MAX_CONNECTING_TIME              10000
#define RECV_VIDEO_WIDTH				 640
#define RECV_VIDEO_HIEGHT				 420

class RoomAVLogic
{
public:
	RoomAVLogic(uint32 iRoomId,uint32 iSelfUin,RoomModule &module):
			m_iRoomId(iRoomId),
			m_iSelfUin(iSelfUin),
			m_module(module),
			m_bIsStart(false),
			m_nRelayStartTime(0),
			m_iWidth(0),
			m_iHeight(0),
			m_iQuality(0),
			m_iRelayIP(0),
			m_sRelayPort(0),
			m_iDevice(0),
			m_cLogicId(0),
			m_rState(ROOM_AV_NONE),
			m_eVideoMode(ROOM_VIDEO_CRF_MODE)
	  {}
	~RoomAVLogic(void){}
	
	void ConnectRelayServer(uint32 iRelayIP,uint32 iRelayPort); //初始化连接中转服务器参数，不连接，仅配置参数
	void SetVideoCfg(int iDevice,int iHeight,int iWidth,int iQuality,enuROOM_AV_VIDEO_MODE eMode = /*ROOM_VIDEO_CRF_MAX*/ROOM_VIDEO_CRF_MODE); //设置视频采集属性
	void SetAudioCfg(int iInputDev,int iOutputDev);//设置音频设备输入设备 输出设备
	void SetKeyCfg(uint32 iKey);//设置音视频传输Key
    bool FlushVideoDev(int iDev);//刷新视频设备 用于打开虚拟摄像头程序
	bool ChangeRelayServer(uint32 iRelayIp,uint16 sRelayPort);//变更中转服务器IP

	void UpSelf(HWND hWndLocal,enuVIDEO_TYPE enVideoType = VIDEO_PRIV_MIC);//用户自己上麦 开启本地的视频和语音 本房间内
	void DownSelf();//用户自己下麦
	void UpSelfVoice();//开启语音 仅开启语音，不开启视频
	void DownSelfVoice();//关闭语音
	bool SetAudioMode(enuROOM_AV_AUDIO_MODE mode);//设置音频编码参数
	Enum_Camera_Status GetCameraStatus(int iDevice);//获取摄像头状态
	
	void RecvAV(uint32 iFromUin,HWND hWndPartner,UInt32 iWidth = 320, UInt32 iHeight=240,enuVIDEO_TYPE enVideoType = VIDEO_PRIV_MIC,enuROOM_AV_VIDEO_MODE eMode = /*ROOM_VIDEO_CRF_MAX*/ROOM_VIDEO_CRF_MODE);//接收公麦、私麦、私聊视频+语音
	void RecvAVChg(uint32 iFromUin,HWND hPartnerWnd, UInt32 iWidth, UInt32 iHeight, HWND hOldWnd);//更改接收用户视频显示窗口及图像输出大小
	void LocalAVChg(HWND hNewWnd,HWND hOldWnd);//更改本地视频显示窗口
	void LocalAVEffact(HWND hLocalWnd,bool bEnable);//是否显示本地视频为发送数据解压后的效果 bEnable=TRUE 是
	bool LocalPaintBk(HWND hLocalWnd);//绘制本地最后一帧图像
	bool RemotePaintBk(uint32 iFromUin,HWND hPartnerWnd);//绘制远程用户最后一帧数据

	void RemoveAV(uint32 iFromUin);//移除公麦、私麦、私聊用户视频+语音
	void RecvAudio(uint32 iFromUin);//接收私聊语音
	void RemoveAudio(uint32 iFromUin);//移除私聊语音

	void ClearRoomAV();//房间退出时，清理房间开启的视频，麦克风等信息 以及UDP连接信息

	void PauseLocalAudio(bool bPause); //暂停、恢复本地语音
	void PauseRemoteAudio(uint32 iFromUin,bool bPause); //暂停、恢复远程用户语音
	void PauseLocalVideo(bool bPause);//暂停、恢复本地视频
	void PauseRemoteVideo(uint32 iFromUin,bool bPause); //暂停、恢复远程用户视频
	
	bool IsPhotoableLocalHwnd(HWND localHwnd);//是否可以拍照
	bool IsPhotoableRemoteHwnd(UInt32 iFromUin, HWND partnerHwnd);
	bool IsVideoVirtualDevice(UInt32 iDevId);//指定设备是否为虚拟视频设备
	
	bool IsStartAVEngine(); //该房间是否正在音视频
	bool IsLocalAVEffact(HWND localHwnd); //本地是否开启了显示解压数据效果
	
	//数据的音量
	void   SetSpeakerVolume(UInt32 iVolume);//设置播放音量   -所有房间
	UInt32 GetSpeakerVolume();//获取播放音量
	void   SetSpeakerRoomVolume(UInt32 iVolume);//设置单个房间音量   -单个房间
	UInt32 GetSpeakerRoomVolume();//获取单个房间音量

	void   SetMicRoomAllVolume(UInt32 iVolume); //设置本软件麦克风声音大小  所有房间
	UInt32 GetMicRoomAllVolume();//获取本软件麦克风声音大小  所有房间

	bool   SetSondMute(bool bMute);//设置房间静音  仅当前房间静音
	bool   GetSoundMute(bool& bMute);//获取房间是否静音
	bool   GetAllSoundMute(bool& bMute);//获取所有房间是否静音
	bool   SetAllSoundMute(bool bMute);//设置所有房间是否静音
	bool   GetAllMicMute(bool& bMute);//获取所有房间麦克风静音
	bool   SetAllMicMute(bool bMute);//设置所有房间麦克风静音
	
	//系统音量
	void   SetSysPlayerVolume(UInt32 iVolume);//设置系统声音大小
	UInt32 GetSysPlayerVolume();//获取系统声音大小
	void   SetMicVolume(UInt32 iVolume);//设置麦克风音量
	UInt32 GetMicVolume();//获取麦克风音量

	bool   SetMicMute(bool bMute);//设置麦克风静音
	bool   GetMicMute(bool& bMute);//获取麦克风是否静音
	bool   SetPlayerMute(bool bMute);//设置系统扬声器静音
	bool   GetPlayerMute(bool& bMute);//获取系统扬声器是否静音

	void   GetSoundLevel(UInt32 &iOutput, UInt32 &iInput);//获取麦克风 扬声器音强
	bool   SetMicGreater(bool bEnable);//设置麦克风加强
	bool   GetMicGreater(bool& bEnable);//获取麦克风是否加强
	void   SetViewFFTALL(bool bEnable);//设置所有是否显示波形图
	void   SetViewFFTME(bool bEnable);//设置自己是否显示波形图
	void   SetViewFFTWav(UInt32 iFromUin,bool bEnable);//显示接收到的用户的波形图
	bool   GetViewFFTWav(UInt32 iFromUin);//是否显示波形图
	void   GetNetState(float& fRate);//fRate 0 ~ 1 当前房间网络丢包率
	bool   GetNetState(uint32&iRelayIP,struct Service_Av_Net_Status_All& sansa); //获取音视频发包收包丢包率等状态

	void   GetRecordDeviceName(int iIndex, wchar_t *pDeviceName,int iNameLen);//获取音频输入设备名称
	void   GetPlayoutDeviceName(int iIndex, wchar_t *pDeviceName, int iNameLen);//获取音频输出设备

	void   GetAudioDeviceList(int &iDeviceNum,wchar_t wsDeviceName[][DEVICE_NAME_LEN], bool bInput);//获取音频输入输出设备列表
	void   GetVideoDeviceList(int &iDeviceNum,wchar_t wsDeviceName[][DEVICE_NAME_LEN]);//获取视频设备列表
	void   GetVideoDeviceInfoList(int&iDeviceNum,Service_Video_Info struDeviceInfo[]);//获取视频设备信息列表
	void   GetNumOfSoundDevices(int &iPlayNum, int &iRecordNum);//获取音频输入输出设备数量

	bool TestSetAudioDevice(int iInputDevice, int iOutputDevice);//设置音频输入、输出设备 语音测试时候用
	bool TestCheckSoundDevice(bool bOpen);//设置音频测试  首先TestSetAudioDevice设置音频设备 再调用该函数 
	bool TestSetDefaultAudioDevice();//设置回默认音频设备  若是用户没有变更音频设备，退出测试窗口时调用该函数，恢复为默认配置
	void TestSafeCloseInputDevice(); //关闭音频输入设备，若是该设备没有房间使用关闭，否者不关闭 音频测试时，选择其他设备前需调用一次

	bool ShowVideoAjustDlg(HWND hParentWnd,uint32 iDeviceId);//显示视频属性调整对话框
	void TestSetVideoDevice(int iDeviceId);//设置视频设备
	bool TestStartVideoDevice(int iDeviceId,HWND hWnd,UInt32 &iResult);//启动设置的视频设备
	bool TestStopVideoDevice(int iDeviceId,HWND hWnd);
	void SavePhotoToBmp(HWND hWnd, CString strPath);//保存拍照为BMP到指定的路径 BMP格式
	void SetEnableShapen(bool bEnable); //设置用户显示的图像是否锐化后显示  节省CPU
	
	//初始化混音器
	void InitMixerControl(HWND hWnd);
	//设置中的语音设置0~255
	bool SetMixerControlTotalVolume(unsigned int nVolume);//设置总音量
	bool GetMixerControlTotalVolume(unsigned int& nVolume);//获取总音量
	bool SetMixerControlWavoutVolume(unsigned int nVolume);//设置伴音
	bool GetMixerControlWavoutVolume(unsigned int& nVolume);//获取伴音音量
	bool SetMixerControlOutmicVolume(unsigned int nVolume);//设置音量控制中的麦克风
	bool GetMixerControlOutmicVolume(unsigned int& nVolume);//获取音量控制中的麦克风音量
	//录音控制0~255
	bool SetMixerControlMixerVolume(unsigned int nVolume);//立体声混音
	bool GetMixerControlMixerVolume(unsigned int& nVolume);//获取立体声混音音量
	bool SetMixerControlInmicVolume(unsigned int nVolume);//录音控制中的麦克风
	bool GetMixerControlInmicVolume(unsigned int& nVolume);//获取录音控制中的麦克风音量
	bool SetMixerControlWavInVolume(unsigned int nVolume);//设置音频输入设备音量
	bool GetMixerControlWavInVolume(unsigned int& nVolume);//获取音频输入设备音量
	//静音设置
	bool SetMixerControlTotalMute(bool bEnable);    //主音量//设置总音量静音
	bool GetMixerControlTotalMute(bool& bEnable);//获取总音量是否静音
	bool SetMixerControlWavoutMute(bool bEnable);   //波形//设置伴音静音
	bool GetMixerControlWavoutMute(bool& bEnable);//获取伴音是否静音
	bool SetMixerControlOutmicMute(bool bEnable);   //输入监视器//设置音量控制中的麦克风的静音
	bool GetMixerControlOutmicMute(bool& bEnable);//获取音量控制中的麦克风的静音
	bool SetMixerControlMixerMute(bool bEnable);   //后线路输入//立体声混音静音  若是选择 则为选中 或 不选中
	bool GetMixerControlMixerMute(bool& bEnable);//获取立体声混应是否静音
	bool SetMixerControlInmicMute(bool bEnable);   //后麦克风//设置录音控制中的麦克风静音
	bool GetMixerControlInmicMute(bool& bEnable);//获取录音控制中的麦克风是否静音
	//设置模式
	bool SetMixerControlSingMode(bool bEnable);//设置唱歌模式
	bool GetMixerControlSingMode(bool& bEnable);//获取是否唱歌模式
	bool SetMixerControlChatMode(bool bEnable);//设置与人聊天模式
	bool GetMixerControlChatMode(bool& bEnable);//获取是否与人聊天模式
	bool SetMixerControlPlayMode(bool bEnable);//设置在麦上放歌模式
	bool GetMixerControlPlayMode(bool& bEnable);//获取是否在麦上唱歌模式
	bool GetMixerControlMode(EnMixerControlMode& enMode);//获取系统当前模式
	bool SetMixerControlMode(EnMixerControlMode enMode);//设置系统当前模式
	//获取控制器当前系统是否可用
	bool GetMixerControlTotalEnable();//获取当前总音量是否可用
	bool GetMixerControlWavoutEnable();//获取伴音是否可用
	bool GetMixerControlOutmicEnable();//获取音量控制中的麦克风是否可用
	bool GetMixerControlMixerEnable();//获取立体声混音是否可用
	bool GetMixerControlInmicEnable();//获取音频输入设备中音量大小
	bool GetMixerControlWavInEnable();//获取调整麦克风音量大小是否可用
private:
	void AddUserMap(uint32 iFromUin,uint8 cLogicId,enuROOM_AV_USER_STATE eState,HWND hWndPartner=NULL);
	RCPtr<strucROOMAVUSER> FindUserMap(uint32 iFromUin,enuROOM_AV_USER_STATE eState);
	void RemoveUserMap(uint8 cLogicId);
	bool IsVideoDeviceInit(int iDevice);//视频设备是否初始化

	uin m_iRoomId;
	uin m_iSelfUin;
	int64 m_nRelayStartTime;
	uin m_iRelayIP;//中转服务器IP
	UInt16 m_sRelayPort;//中转服务器端口
	uin m_iSelfIP;//客户端公网IP

	bool m_bIsStart;//是否已经开始连接

	int m_iWidth;//视频采集宽度
	int m_iHeight;//视频采集高度
	int m_iQuality;//视频采集质量
	int m_iDevice; //摄像头设备序列 视频设备

	int m_iInputDev;//音频输入设备
	int m_iOutputDev;//音频输出设备
	uin m_iKey; //音视频key

	HWND m_hWndLocal; //自己本地窗口的句柄
	uint8 m_cLogicId; //连接中转费服务器的LogicId
	Mutex m_mutex; //房间线程 
	
	enuROOM_AV_STATE m_rState;
	enuROOM_AV_VIDEO_MODE m_eVideoMode;//视频压缩模式

	RoomModule &m_module;

	std::map<uint8,RCPtr<strucROOMAVUSER> > m_userMap;
};
