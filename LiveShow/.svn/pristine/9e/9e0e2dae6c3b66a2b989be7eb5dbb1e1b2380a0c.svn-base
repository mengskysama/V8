#pragma once
#include <MMSystem.h>
#include "AvDefine.h"
//操作系统类型
enum EnOsType
{
	OsWindowsUnKnown = 0,
	OsWindowsXP ,
	OsWindowsVista,
	OsWindows7
};

//支持的混音器类型
enum EnMixerControlType
{	
	MIXER_CONTROL_TOTAL_TYPE = 0,//总音量
	MIXER_CONTROL_WAVOUT_TYPE,//伴奏
	MIXER_CONTROL_OUTMIC_TYPE0,//音量控制中的麦克风
	MIXER_CONTROL_OUTMIC_TYPE1,
	MIXER_CONTROL_MIXER_TYPE,//录音立体声混音
	MIXER_CONTROL_INMIC_TYPE,//录音控制中的麦克风 --对应立体声混音
	MIXER_CONTROL_SOFT_TYPE,//软件合成器 --后线路输入
	MIXER_CONTROL_FRONT_TYPE,//前段输入  --麦克风
	MIXER_CONTROL_TREE_TYPE //立体声混音
};

// enum EnMixerControlMode
// {
// 	MIXER_CONTROL_SING = 0,//唱歌模式
// 	MIXER_CONTROL_CHAT,    //与人聊天
// 	MIXER_CONTROL_PLAY,    //在麦上放歌模式
// 	MIXER_CONTROL_OTHER    //其他模式
// };

//混音器类型
enum EnMixerComponentType
{
	ctDstDigital = 0,
	ctDstHeadPhones,
	ctDstLine,
	ctDstMonitor,
	ctDstSpeakers,
	ctDstTelephone,
	ctDstUndefined,
	ctDstVoiceIn,
	ctDstWaveIn,
	ctSrcAnalog,
	ctSrcAux,
	ctSrcCD,
	ctSrcDigital,
	ctSrcLine,
	ctSrcMicrophone,
	ctSrcPCSpeaker,
	ctSrcSynthesizer,
	ctSrcTelephone,
	ctSrcUndefined,
	ctSrcWaveOut
};

//设置左右声道数据
struct sChannelData {
	MIXERCONTROLDETAILS_UNSIGNED Left ;
	MIXERCONTROLDETAILS_UNSIGNED Right ;
} ;

//设置混音器传入数据
struct strucChannelSet{
	struct sChannelData stateData ;
	bool  bChecked;
};

//线路类型
enum EnMixerLineType
{
	MIXER_OUT = 0,	 //线路输出
	MIXER_IN,    //线路输入
	MIXER_OTHER,  //其他 不输入线路输入 输出的
	MIXER_ALL
};

enum enCtrlType {
	Balance = 0, //标识声道平衡
	Volume, //音量
	Mute //静音
} ;

enum enMuteType
{
	kMute = 0,
	kSelec = 1
};

//线路参数
struct StrucMixerParam 
{
	unsigned nControlType;
	DWORD dwLineType;
	EnMixerLineType enType;
	enCtrlType enctrlType;
};

#ifdef _DEBUG
#define MIXERTOOLSNAME L"AudioTools.dll"
#else
#define MIXERTOOLSNAME L"AudioTools.dll"
#endif

//指定控件类型是选择 还是 静音
typedef int(*pfIsMixerControlMuteOrSelect)(int cnType,int liType,bool* bSel);
typedef int(*pfSetMixerControlMuteOrSelect)(int cnType,int liType,bool bSel);
typedef int(*pfSetMixerControlSingMode)(void);
typedef int(*pfGetMixerControlSingMode)(bool* pbEnable);
typedef int(*pfGetMixerControlMode)(int* pnMode);

#define EXIT_ON_ERROR(hres)  \
	if (FAILED(hres))  { goto Exit; }
#define SAFE_RELEASE(punk)  \
	if ((punk) != NULL) \
{ (punk)->Release(); (punk) = NULL; }
#define MAX_VOLUME  255

enum EnAudioType
{
	AudioRender = 0,
	AudioCapture
};