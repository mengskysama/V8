#pragma once
#include "51ktv.h"

//Event ID
enum
{
	EVENT_AV_CONN_START = EVENT_VALUE_AV_BEGIN,
	EVENT_AV_CONN_STOP  ,
	EVENT_AV_AUDIO_FAIL ,
	EVENT_AV_VIDEO_FAIL ,
	EVENT_AV_CONN_END   = EVENT_VALUE_AV_END 
};

//公麦Channel建立成功后，AVModel-->RoomModel
struct AV_Connect_Start_Event : public Event
{
	AV_Connect_Start_Event(param iRoomId, param iFromUin, param cLogicID)
	{
		eventValue = EVENT_AV_CONN_START;
		param0     = iRoomId;
		param1     = iFromUin;
		param2	   = cLogicID;
		srcMId     = MODULE_ID_AV;
		desMId     = MODULE_ID_ROOM; 
	}
};

//公麦Channel断开后，AVModel-->RoomModel
struct AV_Connect_Stop_Event : public Event
{
	AV_Connect_Stop_Event(param iRoomId,param iFromUin, param iReason)
	{
		eventValue = EVENT_AV_CONN_STOP;
		param0     = iRoomId;
		param1     = iFromUin;
		param2     = iReason;
		srcMId     = MODULE_ID_AV;
		desMId     = MODULE_ID_ROOM;
	}
};

//本地语音设备连接失败，AVModel-->RoomModel
struct AV_Audio_Fail_Event : public Event
{
	AV_Audio_Fail_Event(param iRoomId, param iReslut)
	{
		eventValue = EVENT_AV_AUDIO_FAIL;
		param0     = iRoomId;
		param1	   = iReslut;
		srcMId     = MODULE_ID_AV;
		desMId     = MODULE_ID_ROOM;
	}
};

//本地视频设备连接失败，AVModel-->RoomModel
struct AV_Video_Fail_Event : public Event
{
	AV_Video_Fail_Event(param iRoomId, param iReslut)
	{
		eventValue = EVENT_AV_VIDEO_FAIL;
		param0     = iRoomId;
		param1	   = iReslut;
		srcMId     = MODULE_ID_AV;
		desMId     = MODULE_ID_ROOM;
	}
};

//音频错误返回结果
enum
{
	kAudio_Err_Output = 0,//打开音频输出时候出错
	kAudio_Err_Input = 1  //打开音频输入设备时出错
};

//视频错误返回结果
enum
{
	kVideo_Err_Success = 0,
	kVideo_Err_NoExist  = 1,
	kVideo_Err_IsUsing  = 2
};

//摄像头状态信息
enum Enum_Camera_Status
{
	kVideo_Success = 0, //状态正常
	kVideo_NoCamer = 1, //没有摄像头
	kVideo_NoExist = 2, //摄像头不存在
	kVideo_IsUsing = 3, //被其他程序占用
	kVideo_Failed  = 4  //打开摄像头失败
};

//Service ID  用在有消息返回结果的处理
enum
{
	SERVICE_AV_RELAY_BEGIN = SERVICE_VALUE_AV_BEGIN ,
	SERVICE_AV_RELAY_REQ, //获取Relay库开始时间 
	SERVICE_AV_RELAY_CFG, //中转服务器IP配置 g_Setting为公共的全局变量，每次创建UDPChannel前，若是中转IP有变化重新调用该方法配置一下
	SERVICE_AV_RELAY_CONN,//创建UDP连接，开始连接中转服务器
	SERVICE_AV_VIDEO_CFG, //视频大小参数等设置，仅仅配置摄像头参数，不启动摄像头，当第一个房间使用摄像头时再启动
	SERVICE_AV_AUDIO_CFG, //音频设备配置，分输入输出,房间启动的时候不启动输入输出，用户上麦 或接受公麦用户音频数据时启动
	SERVICE_AV_END_VIDEO_CFG, //去除所有房间和该摄像头设备的关联，并关闭摄像头。
	SERVICE_AV_VIDEO_GET_CFG, //获取指定摄像头配置参数
	SERVICE_AV_KEY_SET_CFG,   //设置音视频Key
	SERVICE_AV_AUDIO_START,	  //用户上公麦、私麦、私聊等开启语音
	SERVICE_AV_VIDEO_START,   //用户上公麦、私麦、私聊等开启视频
	SERVICE_AV_AUDIO_STOP,	  //用户下公麦、私麦、私聊等关闭语音
	SERVICE_AV_VIDEO_STOP,	  //用户下公麦、私麦、私聊等关闭视频
	SERVICE_AV_VIDEO_VIRTUAL, //指定视频设备是否为虚拟视频设备
	SERVICE_AV_CLEAR_EXIT,    //用户关闭房间时，清理分配给房间的资源
	SERVICE_AV_AUDIO_OPP_START,//添加接收房间公麦、私麦、私聊语音 
	SERVICE_AV_VIDEO_OPP_START,//添加接收房间公麦、私麦、私聊视频
	SERVICE_AV_VIDEO_CHG_HANDLE,//更换对方窗口显示句柄，并修改解析出的图像大小
	SERVICE_AV_VIDEO_CHG_LOCALHDL,//更换本地窗口显示句柄
	SERVICE_AV_VIDEO_CHG_EFFACT,//更换本地图像显示效果，显示压缩图像解压效果 还是显示采集效果
	SERVICE_AV_VIDEO_PAINT_LOCAL,//绘制本地图像背景
	SERVICE_AV_VIDEO_PAINT_REMOTE,//绘制远程图像背景
	SERVICE_AV_AUDIO_OPP_STOP,//对方离开房间，下麦、停止语音聊天
	SERVICE_AV_VIDEO_OPP_STOP,//对方离开房间，下麦、停止视频聊天
	SERVICE_AV_AUDIO_LOCAL,   //本地房间语音暂停、恢复等操作
	SERVICE_AV_VIDEO_LOCAL,   //本地房间视频暂停、恢复等操作
	SERVICE_AV_AUDIO_REMOTE,  //暂停公麦、私麦、私聊远程用户的语音
	SERVICE_AV_VIDEO_REMOTE,  //暂停公麦、私麦、私聊远程用户的视频
	SERVICE_AV_PHOTOAB_LOCAL,   //本地摄像头是否可以拍照
	SERVICE_AV_PHOTOAB_REMOTE,  //远程摄像头是否可以拍照
	SERVICE_AV_FLUSH_VIDEO_DEV, //刷新视频设备 打开虚拟摄像头
	SERVICE_AV_CHANGE_RELAY_SVR,//变更中转服务器IP
	SERVICE_AV_PRESS_AUDIO_CFG,//配置音频压缩参数
	SERVICE_SET_SPEAKER_VOLUME, //设置语音音量
	SERVICE_GET_SPEAKER_VOLUME,  //获取语音音量
	SERVICE_SET_SPEAKER_ROOM_VOLUME,//设置房间扬声器音量
	SERVICE_GET_SPEAKER_ROOM_VOLUME,//获取房间扬声器音量
	SERVICE_SET_SYSPLAYER_VOLUME,//设置系统扬声器音量大小
	SERVICE_GET_SYSPLAYER_VOLUME,//获取系统扬声器音量大小
	SERVICE_SET_ROOM_ALL_VOLUME, //设置所有房间麦克风音量
	SERVICE_GET_ROOM_ALL_VOLUME, //获取所有房间麦克风音量
	SERVICE_SET_MIC_VOLUME,      //设置麦克风音量
	SERVICE_GET_MIC_VOLUME,		 //获取麦克风音量
	SERVICE_SET_MIC_MUTE,		 //设置麦克风静音
	SERVICE_SET_SOUND_MUTE,      //设置房间声音静音
	SERVICE_SET_PLAYER_MUTE,	 //设置系统播放设备静音
	SERVICE_GET_MIC_MUTE,        //获取麦克风是否静音
	SERVICE_GET_SOUND_MUTE,		 //获取房间音频是否静音
	SERVICE_GET_PLAYER_MUTE,     //获取系统扬声器是否静音
	SERVICE_SET_ROOM_ALL_MUTE,	 //设置所有房间是否静音
	SERVICE_GET_ROOM_ALL_MUTE,	 //获取所有房间是否静音
	SERVICE_SET_ROOM_MIC_MUTE,	 //设置房间麦克风静音
	SERVICE_GET_ROOM_MIC_MUTE,	 //获取房间麦克风静音
	SERVICE_CHK_SOUND_DEVICE,	 //检查声音设备
	SERVICE_GET_SOUND_LEVEL,	 //获取声音大小
	SERVICE_GET_SOUND_DEVICE_NUM,//获取音频设备个数
	SERVICE_SET_SOUND_DEVICE,    //设置音频设备
	SERVICE_GET_RECORD_DEVICE_NAME,//获取输入设备名称
	SERVICE_GET_PLAYOUT_DEVICE_NAME,//获取输出设备名称
	SERVICE_GET_AUDIO_DEVICE_LIST,//获取音频设备列表
	SERVICE_GET_VIDEO_DEVICE_LIST,//获取视频设备列表
	SERVICE_GET_VIDEO_DEVINFO_LIST,//获取视频设备详细信息
	SERVICE_SHOW_VIDEO_AJUST,	  //显示视频调整属性
	SERVICE_IS_START_ENGINE,      //指定房间自己是否上麦、私麦、私聊等
	SERVICE_IS_PHOTO_ABLE_LOCAL,  //本地自己是否可以拍照
	SERVICE_IS_PHOTO_ABLE_REMOTE, //接收对方视频是否可以拍照
	SERVICE_IS_START_AV_ENGINE,   //本房间是否启动了音视频
	SERVICE_IS_OPEN_AV_EFFACT,    //本地是否开启了对方接受视频效果
	SERVICE_IS_VIDEO_DEV_INIT,    //视频设备是否已经初始化
	SERVICE_AV_AUDIO_STOP_SAFE,   //安全关闭音频设备
	SERVICE_AV_VIDEO_START_TEST,  //开启视频测试
	SERVICE_AV_VIDEO_STOP_SAFE,   //安全关闭视频测试
	SERVICE_SET_MIC_GREATER,	   //设置麦克风加强
	SERVICE_GET_MIC_GREATER,	   //获取是否麦克风加强
	SERVICE_SET_VIEW_FFT_ALL,      //设置是否显示波形图  全部
	SERVICE_GET_VIEW_FFT_ALL,	   //获取是否显示波形图  全部
	SERVICE_SET_VIEW_FFT_ME,       //设置自己是否显示波形图
	SERVICE_GET_VIEW_FFT_ME,       //获取自己是否显示波形图
	SERVICE_SET_VIEW_FFT_WAV,      //设置显示声音波形图  只针对个人
	SERVICE_GET_VIEW_FFT_WAV,      //获取是否显示声音波图
	SERVICE_NET_STATE_RUN_RATE,    //获取当前房间网络丢包率
	SERVICE_AV_NET_STATUS_ALL,     //音视频网络状态
	SERVICE_MIXER_CONTROL_INIT,    //Mixer初始化
	SERVICE_MIXER_SET_TOTAL_VOLUME,//设置总音量
	SERVICE_MIXER_GET_TOTAL_VOLUME,//获取总音量
	SERVICE_MIXER_SET_WAVOUT_VOLUME,//设置伴音音量
	SERVICE_MIXER_GET_WAVOUT_VOLUME,//获取伴音音量
	SERVICE_MIXER_SET_OUTMIC_VOLUME,//设置音量控制中的麦克风音量
	SERVICE_MIXER_GET_OUTMIC_VOLUME,//获取音量控制中的麦克风音量
	SERVICE_MIXER_SET_MIXER_VOLUME,//设置立体声混音音量
	SERVICE_MIXER_GET_MIXER_VOLUME,//获取立体声混音音量
	SERVICE_MIXER_SET_INMIC_VOLUME,//设置录音控制中的麦克风音量
	SERVICE_MIXER_GET_INMIC_VOLUME,//获取录音控制中的麦克风音量
	SERVICE_MIXER_SET_WAVIN_VOLUME,//设置音频输入设备音量
	SERVICE_MIXER_GET_WAVIN_VOLUME,//获取音频输入设备音量
	SERVICE_MIXER_SET_TOTAL_MUTE,  //设置总音量静音
	SERVICE_MIXER_GET_TOTAL_MUTE,  //获取总音量静音
	SERVICE_MIXER_SET_WAVOUT_MUTE, //设置波形静音
	SERVICE_MIXER_GET_WAVOUT_MUTE, //获取波形是否静音
	SERVICE_MIXER_SET_OUTMIC_MUTE, //设置音量控制中的麦克风静音
	SERVICE_MIXER_GET_OUTMIC_MUTE, //获取音量控制中的麦克风静音
	SERVICE_MIXER_SET_MIXER_MUTE,  //设置立体声混音静音
	SERVICE_MIXER_GET_MIXER_MUTE,  //获取立体声混音静音
	SERVICE_MIXER_SET_INMIC_MUTE,  //设置录音控制的麦克风静音
	SERVICE_MIXER_GET_INMIC_MUTE,  //获取录音控制中的麦克风静音
	SERVICE_MIXER_SET_SING_MODE,   //设置唱歌模式
	SERVICE_MIXER_GET_SING_MODE,   //获取唱歌模式
	SERVICE_MIXER_SET_CHAT_MODE,   //设置与人聊天模式
	SERVICE_MIXER_GET_CHAT_MODE,   //获取是否处于与人聊天模式
	SERVICE_MIXER_SET_PLAY_MODE,   //设置在麦上放歌模式
	SERVICE_MIXER_GET_PLAY_MODE,   //获取在麦上放歌模式
	SERVICE_MIXER_GET_CURR_MODE,   //获取当前模式
	SERVICE_MIXER_GET_TOTAL_ENABLE,//获取当前总音量是否可用
	SERVICE_MIXER_GET_WAVOUT_ENABLE,//获取伴音是否可用
	SERVICE_MIXER_GET_OUTMIC_ENABLE,//获取音量控制中的麦克风是否可用
	SERVICE_MIXER_GET_MIXER_ENABLE,//获取立体声混音是否可用
	SERVICE_MIXER_GET_INMIC_ENABLE,//获取音频输入设备中音量大小是否可用
	SERVICE_MIXER_GET_WAVIN_ENABLE,//获取调整麦克风音量大小是否可用
	SERVICE_VIDEO_GET_CURR_STATUS, //获取摄像头当前状态
	SERVICE_VIDEO_SET_SHAP_ENABLE, //设置视频显示是否锐化
	SERVICE_AV_RELAY_END = SERVICE_AVLUE_AV_END
};

//消息ID
enum
{
	MSG_AV_RELAY_BEGIN = MESSAGE_VALUE_AV_BEGIN, 
};

//视频压缩模式
enum enuROOM_AV_VIDEO_MODE
{
	ROOM_VIDEO_CRF_MODE = 0x01, //恒定码率 10K
	ROOM_VIDEO_CQP_224  = 0x02, //恒定质量
	ROOM_VIDEO_CQP_232  = 0x03,
	ROOM_VIDEO_CQP_208  = 0x04,
	ROOM_VIDEO_ABR_MODE = 0x05,  //平均码率
	ROOM_VIDEO_CQP_216  = 0x06,
	ROOM_VIDEO_CRF_MAX  = 0x07,  //恒定码率 限定最大值 7 ~ 17K
	ROOM_VIDEO_CRF_MIN  = 0x08,  //恒定码率 限定 4 ~ 11K
	ROOM_VIDEO_CRF_OLD  = 0x09,
	ROOM_VIDEO_CQP_BIG  = 0x0A,
	ROOM_VIDEO_CQP_SMAL = 0x0B
};

//音频压缩模式
enum enuROOM_AV_AUDIO_MODE
{
	ROOM_AUDIO_CBR_MODE = 0x01,  //音频恒定码率 7K 复杂度默认
	ROOM_AUDIO_VBR_MODE = 0x02,  //音频变动码率 7K +1K  复杂度默认
	ROOM_AUDIO_CBR_SIXD = 0x03,  //音频恒定码率 6K 复杂度默认
	ROOM_AUDIO_CBR_EIGD = 0x04,  //音频恒定码率 8K 复杂度默认
	ROOM_AUDIO_CBR_NIND = 0x05,  //音频恒定码率 9k 复杂度默认
	ROOM_AUDIO_VBR_SIXD = 0x06,  //音频动态码率 6K 复杂度默认
	ROOM_AUDIO_VBR_EIGD = 0x07,  //音频动态码率 8K 复杂度默认
	ROOM_AUDIO_VBR_NIND = 0x08,  //音频动态码率 8K 复杂度默认
	ROOM_AUDIO_VBR_SIXC = 0x09,  //音频动态码率 6K 复杂度5
	ROOM_AUDIO_VBR_MODC = 0x0A,  //音频动态码率 7K 复杂度5
	ROOM_AUDIO_VBR_NINC = 0x0B   //音频动态码率 8K 复杂度5
};

enum EnMixerControlMode
{
	MIXER_CONTROL_SING = 1,//唱歌模式
	MIXER_CONTROL_CHAT,    //与人聊天
	MIXER_CONTROL_PLAY,    //在麦上放歌模式
	MIXER_CONTROL_OTHER    //其他模式
};

//视频尺寸类型选择
enum enuVIDEO_TYPE
{
	VIDEO_CENT_MIC		= 0x00,    //中间大视频麦
	VIDEO_EDGE_MIC		= 0x01,    //小麦
	VIDEO_PRIV_MIC      = 0x02     //私麦
};

//SERVICE_AV_RELAY_REQ
struct Service_AV_Req_Time
{
	int64 lRelaySymbolTime;   //自身Relay库启动时间
};

//Relay模块参数 SERVICE_AV_RELAY_CFG
struct Service_Relay_Config
{
	uint32		iMyUin;			 //自己的ID
	uint32      iRelayIP;        //中转服务器IP，网络字节序
	uint16      sRelayPort;      //中转服务器端口，主机字节序
	uint32      unP2PTimeout;    //p2p超时设置
};

//SERVICE_AV_VIDEO_CFG ,SERVICE_AV_END_VIDEO_CFG, SERVICE_AV_VIDEO_GET_CFG
struct Service_AV_Video_Cfg
{
	uint32      iWidth;          //320
	uint32      iHeight;         //240
	uint32      iQuality;        // 配置时，默认5
	int32       iDeviceID;       // 设备ID
	uint32      iVideoMode;      //视频压缩模式
};

//SERVICE_AV_AUDIO_CFG
struct Service_AV_Audio_Cfg
{
	uint32		iInputDev;
	uint32		iOutputDev;
};

//SERVICE_AV_AUDIO_START   
struct Service_AV_Audio_Start
{
	uint32      iRoomId;		  //开启语音房间编号
	uint8       cLogicID;         //逻辑ID
};

//SERVICE_AV_RELAY_CONN
struct Service_AV_Relay_Conn
{
	uint32      iRoomId;		  //创建UDP连接的房间编号
};

//SERVICE_AV_VIDEO_START  StartRoomVideo(UInt32 iRoomId,HWND localHwnd,int iDeviceID);
struct Service_AV_Video_Start
{
	uint32      iRoomId;		   //开启视频的房间的编号
	HWND        hWndLocal;         //自身的窗口句柄
	uint32      iDeviceId;		   //视频ID
	enuVIDEO_TYPE enType;
};

//SERVICE_AV_AUDIO_STOP,	  
struct Service_AV_Audio_Stop 
{
	uint32      iRoomId;
	uint8		cLogicID;
};

//SERVICE_AV_VIDEO_STOP,	  
struct Service_AV_Video_Stop
{
	uint32      iRoomId;
	HWND		hWndLocal;
	uint32      iDeviceID;
};

//SERVICE_AV_VIDEO_VIRTUAL
struct Service_Av_Video_Virtual
{
	uint32      iDeviceId;
	bool        bReturn;
};

//SERVICE_AV_AUDIO_LOCAL
struct Service_AV_Audio_Local
{
	uint32      iRoomId;
	bool		bPause; //true为暂停，false为恢复
};

//SERVICE_AV_VIDEO_LOCAL
struct Service_AV_Video_Local
{
	uint32		iRoomId;
	HWND		hWndLocal;
	bool		bPause;//true为暂停，false为恢复
};

//SERVICE_AV_AUDIO_REMOTE
struct Service_AV_Audio_Remote
{
	uint32		iRoomId;
	uint32		iFromUin;
	bool		bPause;//true为暂停，false为恢复
};

//SERVICE_AV_VIDEO_REMOTE
struct Service_AV_Video_Remote 
{
	uint32		iRoomId;
	uint32		iFromUin;
	HWND		hWndPartner;
	bool		bPause;//true为暂停，false为恢复
};

//SERVICE_AV_VIDEO_CHG_HANDLE
struct Service_AV_Video_Change_Handle
{
	uint32		iRoomId;
	uint32		iFromUin;
	uint32		iWidth;
	uint32		iHeight;
	HWND		hWndPartner;
	HWND		hOldWnd;
};

//SERVICE_AV_VIDEO_CHG_LOCALHDL
struct Service_AV_Video_Change_LocalHDL
{
	uint32		iRoomId;
	HWND		hNewWnd;
	HWND		hOldWnd;
};

//SERVICE_AV_VIDEO_CHG_EFFACT
struct Service_AV_Video_Chg_Effact
{
	uint32		iRoomId;
	HWND	    hWndLocal;
	bool        bEnable;
};

//SERVICE_AV_PHOTOAB_LOCAL
struct Service_AV_PhotoAble_Local
{
	uint32	   iRoomId;
	HWND	   hWndLocal;
	bool       bPhotoAble;
};

//SERVICE_AV_PHOTOAB_REMOTE
struct Service_AV_PhotoAble_Remote
{
	uint32	   iRoomId;
	uint32	   iFromUin;
	HWND	   hWndPartner;
	bool       bPhotoAble;
};

//SERVICE_SET_SPEAKER_VOLUME
struct Service_Set_Speaker_Volume
{
	uint32      iRoomId;
	uint32      iVolume;          //音量大小 - 房间声音 所有房间
};

//SERVICE_GET_SPEAKER_VOLUME
struct Service_Get_Speaker_Volume
{
	uint32      iRoomId;
	uint32      iVolume;          //音量大小 - 房间声音 所有房间
};

//SERVICE_SET_SPEAKER_ROOM_VOLUME,//设置房间扬声器音量
struct Service_Set_Speaker_Room_Volume
{
	uint32      iRoomId;
	uint32      iVolume;          //音量大小 - 房间声音
};
//SERVICE_GET_SPEAKER_ROOM_VOLUME,//获取房间扬声器音量
struct Service_Get_Speaker_Room_Volume
{
	uint32      iRoomId;
	uint32      iVolume;          //音量大小 - 房间声音
};

//SERVICE_SET_SYSPLAYER_VOLUME
struct Service_Set_SysPlayer_Volume
{
	uint32      iRoomId;
	uint32      iVolume;          //设置系统扬声器音量大小
};

//SERVICE_GET_SYSPLAYER_VOLUME
struct Service_Get_SysPlayer_Volume
{
	uint32      iRoomId;
	uint32      iVolume;          //获取系统扬声器音量大小
};

//SERVICE_SET_ROOM_ALL_VOLUME, //设置所有房间麦克风音量
struct Service_Set_Room_All_Volume
{
	uint32      iVolume; 
};
//SERVICE_GET_ROOM_ALL_VOLUME, //获取所有房间麦克风音量
struct Service_Get_Room_All_Volume
{
	uint32      iVolume; 
};

//SERVICE_SET_MIC_VOLUME
struct Service_Set_Mic_Volume
{
	uint32      iRoomId;
	uint32      iVolume;          //音量大小 - 麦克风
};

//SERVICE_GET_MIC_VOLUME
struct Service_Get_Mic_Volume
{
	uint32      iRoomId;
	uint32      iVolume;          //音量大小 - 麦克风
};

//SERVICE_SET_MIC_MUTE
struct Service_Set_Mic_Mute
{
	uint32      iRoomId;          
	bool        bMute;            //是否静音 true 静音  false 不静音
	bool		bResult;
};

//SERVICE_SET_SOUND_MUTE,      //设置房间声音静音
struct Service_Set_Sound_Mute
{
	uint32      iRoomId;          
	bool        bMute;            //是否静音 true 静音  false 不静音
	bool		bResult;
};

//SERVICE_SET_PLAYER_MUTE,	 //设置系统播放设备静音
struct Service_Set_Player_Mute
{
	uint32      iRoomId;          
	bool        bMute;            //是否静音 true 静音  false 不静音
	bool		bResult;
};

//SERVICE_GET_MIC_MUTE,        //获取麦克风是否静音
struct Service_Get_Mic_Mute
{
	uint32      iRoomId;          
	bool        bMute;            //是否静音 true 静音  false 不静音
	bool		bResult;
};

//SERVICE_GET_SOUND_MUTE,		 //获取房间音频是否静音
struct Service_Get_Sound_Mute
{
	uint32      iRoomId;          
	bool        bMute;            //是否静音 true 静音  false 不静音
	bool		bResult;
};

//SERVICE_GET_PLAYER_MUTE,     //获取系统扬声器是否静音
struct Service_Get_Player_Mute
{
	uint32      iRoomId;          
	bool        bMute;            //是否静音 true 静音  false 不静音
	bool		bResult;
};

//SERVICE_SET_ROOM_ALL_MUTE,	 //设置所有房间是否静音
struct Service_Set_Room_All_Mute
{
	bool bMute;
	bool bResult;
};

//SERVICE_GET_ROOM_ALL_MUTE,	 //获取所有房间是否静音
struct Service_Get_Room_All_Mute
{
	bool bMute;
	bool bResult;
};

//SERVICE_SET_ROOM_MIC_MUTE,	 //设置房间麦克风静音
struct Service_Set_Room_Mic_Mute
{
	bool bMute;
	bool bResult;
};
//SERVICE_GET_ROOM_MIC_MUTE,	 //获取房间麦克风静音
struct Service_Get_Room_Mic_Mute
{
	bool bMute;
	bool bResult;
};

//SERVICE_CHK_SOUND_DEVICE  //检查声音设备
struct Service_Check_Sound_Device
{
	bool        bOpen;
};

//SERVICE_GET_SOUND_LEVEL        //获取音量大小
struct Service_Get_Sound_Level
{
	uint32      iOutput;
	uint32      iInput;
};

//SERVICE_GET_SOUND_DEVICE_NUM //获取音频设备个数
struct Service_Get_Sound_Device
{
	int         iPlayout;      //扬声器
	int         iRecord;       //麦克风
};

//SERVICE_SET_SOUND_DEVICE //设置音频设备
struct Service_Set_Sound_Device
{
	int         iPlayout;      //扬声器
	int         iRecord;       //麦克风
};

//SERVICE_GET_RECORD_DEVICE_NAME //获取麦克风输入设备名称
#define DEVICE_NAME_LEN       128
struct Service_Get_Record_Device_Name
{
	int         iDeviceIndex;
	wchar_t     strDeviceName[DEVICE_NAME_LEN];
};

//SERVICE_GET_PLAYOUT_DEVICE_NAME //获取扬声器输出设备名称
struct Service_Get_PlayOut_Device_Name
{
	int         iDeviceIndex;
	wchar_t     strDeviceName[DEVICE_NAME_LEN];
};

//SERVICE_GET_AUDIO_DEVICE_LIST //获取音频设备列表
#define  MAX_AUDIO_DEVICE_NUM             10
struct Service_Get_Audio_Device_List
{
	int			iDeviceNum;
	wchar_t     wsDeviceName[MAX_AUDIO_DEVICE_NUM][DEVICE_NAME_LEN];
	bool		bInput;
};

//SERVICE_GET_VIDEO_DEVICE_LIST //获取视频设备列表
#define MAX_VIDEO_DEVICE_NUM              10
struct  Service_Get_Video_Device_List
{
	int         iDeviceNum;              //设备数量
	wchar_t     wsDeviceName[MAX_VIDEO_DEVICE_NUM][DEVICE_NAME_LEN];
};

//SERVICE_GET_VIDEO_DEVINFO_LIST获取视频列表详细信息
#define MAX_VIDEO_DEVICE_GUID_LEN 255
struct Service_Video_Info
{
	unsigned int iDeviceId;
	wchar_t     wsDeviceName[DEVICE_NAME_LEN];
	wchar_t     wsDeviceGUID[MAX_VIDEO_DEVICE_GUID_LEN];

};

struct Service_Get_Video_DevInfo_List
{
	int         iDeviceNum;    //设备数量
	Service_Video_Info  strucDeviceName[MAX_VIDEO_DEVICE_NUM];
};

// SERVICE_SHOW_VIDEO_AJUST,	  //显示视频调整属性
struct Service_Show_Video_Ajust
{
	HWND        hParentWnd;
	uint32      iDeviceID;
};

//SERVICE_IS_START_ENGINE
struct Service_Is_Start_Engine
{
	uint32      iRoomId;
	bool        bStart;
};

// SERVICE_AV_AUDIO_OPP_START,//添加接收房间公麦、私麦、私聊语音 
struct Service_Av_Audio_Opp_Start
{
	int64 lRelaySymbolTime;
	uint32 iRoomId;
	uint32 iFromUin;
	uint8  cLogicID;
};

// SERVICE_AV_VIDEO_OPP_START,//添加接收房间公麦、私麦、私聊视频
struct Service_Av_Video_Opp_Start
{
	int64 lRelaySymbolTime;
	uint32 iRoomId;
	uint32 iFromUin;
	uint8  cLogicID;
	HWND hWndPartner;
	uint32 iWidth;
	uint32 iHeight;
	uint32 iQuality;
	uint32 iVideoMode;
	enuVIDEO_TYPE enType;
};

// SERVICE_AV_AUDIO_OPP_STOP,//对方离开房间，下麦、停止语音聊天
struct Service_Av_Audio_Opp_Stop 
{
	uint32 iRoomId;
	uint32 iFromUin;
	uint8  cLogicID;
};
// SERVICE_AV_VIDEO_OPP_STOP,//对方离开房间，下麦、停止视频聊天
struct Service_Av_Video_Opp_Stop
{
	uint32 iRoomId;
	uint32 iFromUin;
	uint8  cLogicID;
	HWND hWndPartner;
};

//SERVICE_IS_PHOTO_ABLE_LOCAL
struct Service_Is_Photo_Able_Local
{
	uint32 iRoomId;
	HWND localHwnd;
	bool bAble;
};

//SERVICE_IS_PHOTO_ABLE_REMOTE
struct Service_Is_Photo_Able_Remote
{
	uint32 iRoomId;
	uint32 iFromUin;
	HWND partnerHwnd;
	bool bAble;
};

//SERVICE_IS_START_AV_ENGINE
struct Service_Is_Start_Av_Engine
{
	uint32 iRoomId;
	bool bAble;
};

//SERVICE_IS_OPEN_AV_EFFACT
struct Service_Is_Open_Av_Effact
{
	uint32 iRoomId;
	HWND hLocalWnd;
	bool bOpen;
};

//SERVICE_IS_VIDEO_DEV_INIT
struct Service_Is_Video_Dev_Init
{
	uint32 iDev;
	bool bInit;
};

//SERVICE_AV_CLEAR_EXIT
struct Service_Av_Clear_Exit
{
	uint32 iRoomId;
	bool bRet;
};

//SERVICE_AV_VIDEO_START_TEST,  //开启视频测试
struct Service_Av_Video_Start_Test
{
	uint32 iDeviceId;
	HWND hWnd;
	uint32 iResult;
	bool bRet;
};
//SERVICE_AV_VIDEO_STOP_SAFE,   //安全关闭视频测试
struct Service_Av_Video_Stop_Safe
{
	uint32 iDeviceId;
	HWND hWnd;
	bool bRet;
};

//SERVICE_SET_MIC_GREATER,	   //设置麦克风加强
struct Service_Set_Mic_Greater
{
	bool bEnable;
	bool bReturn;
};
//SERVICE_GET_MIC_GREATER,	   //获取是否麦克风加强
struct Service_Get_Mic_Greater
{
	bool bEnable;
	bool bReturn;
};
//SERVICE_SET_VIEW_FFT_ALL
struct  Service_Set_View_FFT_ALL
{
	bool bEnable;
};

//SERVICE_GET_VIEW_FFT_ALL
struct  Service_Get_View_FFT_ALL
{
	bool bEnable;
};

//SERVICE_SET_VIEW_FFT_ME
struct  Service_Set_View_FFT_ME
{
	bool bEnable;
};

//SERVICE_GET_VIEW_FFT_ME
struct  Service_Get_View_FFT_ME
{
	bool bEnable;
};

//SERVICE_SET_VIEW_FFT_WAV,      //设置显示声音波形图
struct Service_Set_View_FFT_Wav
{
	uint32 iRoomId;
	uint32 iFromUin;
	bool bEnable;
};

//SERVICE_GET_VIEW_FFT_WAV,      //获取是否显示声音波图
struct Service_Get_View_FFT_Wav
{
	uint32 iRoomId;
	uint32 iFromUin;
	bool bEnable;
};

//SERVICE_NET_STATE_RUN_RATE,   //获取当前房间网络丢包率
struct Service_Net_Start_Run_Rate
{
	float fRate;
};

//SERVICE_AV_NET_STATUS_ALL,     //音视频网络状态
struct Service_Av_Net_Status_All
{
	uint32 iAudioInTotalPacket; //上行的音频数据包总个数
	float  fAudioInLostRate; //上行音频数据包丢包率
	uint32 iVideoInTotalPacket; //上行视频数据包总个数
	float  fVideoInLostRate; //上行的视频数据丢包率
	uint32 iAudioOutTotalPacket;//下行音频数据包总个数
	float  fAudioOutLostRate;//下行音频数据丢包率
	uint32 iVideoOutTotalPacket;//下行视频数据包总个数
	float  fVideoOutLostRate;//下行视频数据丢包率
};

struct  Service_Av_Net_Status_All_Ex
{
	uint32 iRoomId;
	Service_Av_Net_Status_All sansa;
};

//SERVICE_MIXER_SET_TOTAL_VOLUME,//设置总音量
struct Service_Mixer_Set_Total_Volume
{
	uint32 iVolume;             //0~255
	bool bRet;
};

//SERVICE_MIXER_GET_TOTAL_VOLUME,//获取总音量
struct Service_Mixer_Get_Total_Volume
{
	uint32 iVolume;             //0~255
	bool bRet;
};

//SERVICE_MIXER_SET_WAVOUT_VOLUME,//设置伴音音量
struct Service_Mixer_Set_Wavout_Volume
{
	uint32 iVolume;             //0~255
	bool bRet;
};

//SERVICE_MIXER_GET_WAVOUT_VOLUME,//获取伴音音量
struct Service_Mixer_Get_Wavout_Volume
{
	uint32 iVolume;             //0~255
	bool bRet;
};

// SERVICE_MIXER_SET_OUTMIC_VOLUME,//设置音量控制中的麦克风音量
struct Service_Mixer_Set_Outmic_Volume
{
	uint32 iVolume;             //0~255
	bool bRet;
};
// SERVICE_MIXER_GET_OUTMIC_VOLUME,//获取音量控制中的麦克风音量
struct Service_Mixer_Get_Outmic_Volume
{
	uint32 iVolume;             //0~255
	bool bRet;
};

// SERVICE_MIXER_SET_MIXER_VOLUME,//设置立体声混音音量
struct Service_Mixer_Set_Mixer_Volume
{
	uint32 iVolume;             //0~255	
	bool bRet;
};

// SERVICE_MIXER_GET_MIXER_VOLUME,//获取立体声混音音量
struct Service_Mixer_Get_Mixer_Volume
{
	uint32 iVolume;             //0~255	
	bool bRet;
};
// SERVICE_MIXER_SET_INMIC_VOLUME,//设置录音控制中的麦克风音量
struct Service_Mixer_Set_Inmic_Volume
{
	uint32 iVolume;             //0~255	
	bool bRet;
};
// SERVICE_MIXER_GET_INMIC_VOLUME,//获取录音控制中的麦克风音量
struct Service_Mixer_Get_Inmic_Volume
{
	uint32 iVolume;             //0~255	
	bool bRet;
};
//SERVICE_MIXER_SET_WAVIN_VOLUME,//设置音频输入设备音量
struct Service_Mixer_Set_WavIn_Volume
{
	uint32 iVolume;             //0~255	
	bool bRet;
};
//SERVICE_MIXER_GET_WAVIN_VOLUME,//获取音频输入设备音量
struct Service_Mixer_Get_WavIn_Volume
{
	uint32 iVolume;             //0~255	
	bool bRet;
};
// SERVICE_MIXER_SET_TOTAL_MUTE,  //设置总音量静音
struct Service_Mixer_Set_Total_Mute
{
	bool bEnable;				//true 设置静音  false 不静音
	bool bRet;
};
// SERVICE_MIXER_GET_TOTAL_MUTE,  //获取总音量静音
struct Service_Mixer_Get_Total_Mute
{
	bool bEnable;				//true 设置静音  false 不静音
	bool bRet;
};
// SERVICE_MIXER_SET_WAVOUT_MUTE, //设置波形静音
struct Service_Mixer_Set_Wavout_Mute
{
	bool bEnable;				//true 设置静音  false 不静音
	bool bRet;
};
// SERVICE_MIXER_GET_WAVOUT_MUTE, //获取波形是否静音
struct Service_Mixer_Get_Wavout_Mute
{
	bool bEnable;				//true 设置静音  false 不静音
	bool bRet;
};
// SERVICE_MIXER_SET_OUTMIC_MUTE, //设置音量控制中的麦克风静音
struct Service_Mixer_Set_Outmic_Mute
{
	bool bEnable;				//true 设置静音  false 不静音
	bool bRet;
};
// SERVICE_MIXER_GET_OUTMIC_MUTE, //获取音量控制中的麦克风静音
struct Service_Mixer_Get_Outmic_Mute
{
	bool bEnable;				//true 设置静音  false 不静音
	bool bRet;
};
// SERVICE_MIXER_SET_MIXER_MUTE,  //设置立体声混音静音
struct Service_Mixer_Set_Mixer_Mute
{
	bool bEnable;				//true 设置静音  false 不静音
	bool bRet;
};
// SERVICE_MIXER_GET_MIXER_MUTE,  //获取立体声混音静音
struct Service_Mixer_Get_Mixer_Mute
{
	bool bEnable;				//true 设置静音  false 不静音
	bool bRet;
};
// SERVICE_MIXER_SET_INMIC_MUTE,  //设置录音控制的麦克风静音
struct Service_Mixer_Set_Inmic_Mute
{
	bool bEnable;				//true 设置静音  false 不静音
	bool bRet;
};
// SERVICE_MIXER_GET_INMIC_MUTE,  //获取录音控制中的麦克风静音
struct Service_Mixer_Get_Inmic_Mute
{
	bool bEnable;				//true 设置静音  false 不静音
	bool bRet;
};

//SERVICE_MIXER_CONTROL_INIT,    //Mixer初始化
struct Service_Mixer_Control_Init
{
	HWND hWnd;                   //接收Mixer消息的窗口句柄 没有填写NULL
};

//SERVICE_MIXER_SET_SING_MODE,   //设置唱歌模式
struct Service_Mixer_Set_Sing_Mode
{
	bool bEnable;
	bool bRet;
};
// SERVICE_MIXER_GET_SING_MODE,   //获取唱歌模式
struct Service_Mixer_Get_Sing_Mode
{
	bool bEnable;
	bool bRet;
};
// SERVICE_MIXER_SET_CHAT_MODE,   //设置与人聊天模式
struct Service_Mixer_Set_Chat_Mode
{
	bool bEnable;
	bool bRet;
};
// SERVICE_MIXER_GET_CHAT_MODE,   //获取是否处于与人聊天模式
struct Service_Mixer_Get_Chat_Mode
{
	bool bEnable;
	bool bRet;
};
// SERVICE_MIXER_SET_PLAY_MODE,   //设置在麦上放歌模式
struct Service_Mixer_Set_Play_Mode
{
	bool bEnable;
	bool bRet;
};
// SERVICE_MIXER_GET_PLAY_MODE,   //获取在麦上放歌模式
struct Service_Mixer_Get_Play_Mode
{
	bool bEnable;
	bool bRet;
};
// SERVICE_MIXER_GET_CURR_MODE,   //获取当前模式
struct Service_Mixer_Get_Curr_Mode
{
	uint32 iMode;
	bool bRet;
};

//SERVICE_AV_FLUSH_VIDEO_DEV, //刷新视频设备 打开虚拟摄像头
struct Service_Av_Flush_Video_Dev
{
	uint32 iDev;
	bool bRet;
};

//SERVICE_MIXER_GET_TOTAL_ENABLE,//获取当前总音量是否可用
struct Service_Mixer_Get_Total_Enable
{
	bool bEnable;
	bool bRet;
};

//SERVICE_MIXER_GET_WAVOUT_ENABLE,//获取伴音是否可用
struct Service_Mixer_Get_WavOut_Enable
{
	bool bEnable;
	bool bRet;
};
//SERVICE_MIXER_GET_OUTMIC_ENABLE,//获取音量控制中的麦克风是否可用
struct Service_Mixer_Get_OutMic_Enable
{
	bool bEnable;
	bool bRet;
};
//SERVICE_MIXER_GET_MIXER_ENABLE,//获取立体声混音是否可用
struct Service_Mixer_Get_Mixer_Enable
{
	bool bEnable;
	bool bRet;
};
//SERVICE_MIXER_GET_INMIC_ENABLE,//获取音频输入设备中音量大小是否可用
struct Service_Mixer_Get_InMic_Enable
{
	bool bEnable;
	bool bRet;
};
//SERVICE_MIXER_GET_WAVIN_ENABLE,//获取调整麦克风音量大小是否可用
struct Service_Mixer_Get_WavIn_Enable
{
	bool bEnable;
	bool bRet;
};

//SERVICE_AV_VIDEO_PAINT_LOCAL,//绘制本地图像背景
struct Service_Av_Video_Paint_Local
{
	uint32 iRoomId;
	HWND hWnd;
	bool bRet;
};

//SERVICE_AV_VIDEO_PAINT_REMOTE,//绘制远程图像背景
struct Service_Av_Video_Paint_Remote
{
	uint32 iRoomId;
	uint32 iFromUin;
	HWND hWnd;
	bool bRet;
};

//SERVICE_AV_CHANGE_RELAY_SVR
struct Service_Av_Change_Relay_Svr
{
	uint32 iRoomId;
	uint32 ip;
	uint16 sport;
	bool bret;
};

//SERVICE_AV_PRESS_AUDIO_CFG
struct Service_Av_Press_Audio_Cfg
{
	enuROOM_AV_AUDIO_MODE mode;
	bool bret;
};

//SERVICE_AV_KEY_SET_CFG
struct Service_Av_Key_Set_Cfg
{
	uint32 iKey;
	uint32 iRoomId;
	bool bret;
};

//获取摄像头当前状态SERVICE_VIDEO_GET_CURR_STATUS
struct Service_Video_Get_Curr_Status
{
	int iDevice;
	Enum_Camera_Status eStatus;
};

//设置视频显示是否锐化SERVICE_VIDEO_SET_SHAP_ENABLE
struct Service_Video_Set_Shapen_Enable
{
	bool bEnable;
};


#define PER_FRAME_OF_SAMEPLE  1
#define PER_PACKET_B_FRAME_LIMIT 1100
#define PER_PACKET_P_FRAME_LIMIT 850
