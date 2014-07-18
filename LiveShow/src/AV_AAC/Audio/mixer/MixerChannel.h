#pragma once
#include <MMSystem.h>
#include "Mixer.h"
#include "MixerDefine.h"
#include <string>

class CMixerChannel
{
public:
	CMixerChannel(void);
	~CMixerChannel(void);

public:
	//初始化混音通道
	void Init(HWND hWnd);
// 	//显示混音器线路数 nID为CCombox的ID
// 	void ShowMixerCaps(CComboBox* pMixerCombo);
// 	//显示指定线路上的控制器信息
// 	void ShowMixerLineControl(unsigned int nMixerLine,CComboBox* pMixerCombo);
	//获取groupId
	int GetMixerGroupId(unsigned int& groupId, const StrucMixerParam& sMixerParam);
	//设置混音器Blance Volume Mute
	int UpdateMixerControlState( unsigned int groupId,const strucChannelSet& sSet) ;
	//获取混音器Blance Volume Mute
	int GetMixerControlState( unsigned int groupId,strucChannelSet& sSet );
	//获取groupId 根据名字关键字
	int GetMixerGroupId(unsigned int& groupId,const std::wstring& str,const StrucMixerParam& sMixerParam);
	//设置混音器是否为选择选中状态
	int SetMixerControlSelState(unsigned int groupId,const strucChannelSet& sSet);
	//获取混音器是否选择选中状态
	int GetMixerControlSelState(unsigned int groupId,strucChannelSet& sSet);
	//判断指定的groupId是否为选择 TRUE 为选择
	bool IsMixerControlSelState(unsigned int groupId);
	//获取音频输入设备中选中的设备
	int GetMixerControlSelGroupId(unsigned int& groupId);

protected:
	//获取线路类型字符串
	/*
	#define MIXERLINE_TARGETTYPE_UNDEFINED      0
	#define MIXERLINE_TARGETTYPE_WAVEOUT        1
	#define MIXERLINE_TARGETTYPE_WAVEIN         2
	#define MIXERLINE_TARGETTYPE_MIDIOUT        3
	#define MIXERLINE_TARGETTYPE_MIDIIN         4
	#define MIXERLINE_TARGETTYPE_AUX            5
	*/
	void GetLineTargetTypeStr( DWORD dwType, std::string& strType ) ;
protected:
	HMIXER	mhMixer; //混音器句柄
	unsigned int mMixerCount;//混音器数目
	bool bMixerInit;//混音器是否初始化
	CMixer* mpCMixer ;//混音器
	HWND hMixerWnd;//当前混音器消息通知的窗口句柄

	unsigned int nextResourceID ;//资源ID标识号

	//存储声道 音量等数据
	struct strucChannelData {
		struct sChannelData stateData ;
		int nChannels ;
	};
	
	/*  mcomponentType
	#define MIXERLINE_COMPONENTTYPE_DST_FIRST       0x00000000L
	#define MIXERLINE_COMPONENTTYPE_DST_UNDEFINED   (MIXERLINE_COMPONENTTYPE_DST_FIRST + 0)
	#define MIXERLINE_COMPONENTTYPE_DST_DIGITAL     (MIXERLINE_COMPONENTTYPE_DST_FIRST + 1)
	#define MIXERLINE_COMPONENTTYPE_DST_LINE        (MIXERLINE_COMPONENTTYPE_DST_FIRST + 2)
	#define MIXERLINE_COMPONENTTYPE_DST_MONITOR     (MIXERLINE_COMPONENTTYPE_DST_FIRST + 3)
	#define MIXERLINE_COMPONENTTYPE_DST_SPEAKERS    (MIXERLINE_COMPONENTTYPE_DST_FIRST + 4)
	#define MIXERLINE_COMPONENTTYPE_DST_HEADPHONES  (MIXERLINE_COMPONENTTYPE_DST_FIRST + 5)
	#define MIXERLINE_COMPONENTTYPE_DST_TELEPHONE   (MIXERLINE_COMPONENTTYPE_DST_FIRST + 6)
	#define MIXERLINE_COMPONENTTYPE_DST_WAVEIN      (MIXERLINE_COMPONENTTYPE_DST_FIRST + 7)
	#define MIXERLINE_COMPONENTTYPE_DST_VOICEIN     (MIXERLINE_COMPONENTTYPE_DST_FIRST + 8)
	#define MIXERLINE_COMPONENTTYPE_DST_LAST        (MIXERLINE_COMPONENTTYPE_DST_FIRST + 8)

	#define MIXERLINE_COMPONENTTYPE_SRC_FIRST       0x00001000L
	#define MIXERLINE_COMPONENTTYPE_SRC_UNDEFINED   (MIXERLINE_COMPONENTTYPE_SRC_FIRST + 0)
	#define MIXERLINE_COMPONENTTYPE_SRC_DIGITAL     (MIXERLINE_COMPONENTTYPE_SRC_FIRST + 1)
	#define MIXERLINE_COMPONENTTYPE_SRC_LINE        (MIXERLINE_COMPONENTTYPE_SRC_FIRST + 2)
	#define MIXERLINE_COMPONENTTYPE_SRC_MICROPHONE  (MIXERLINE_COMPONENTTYPE_SRC_FIRST + 3)
	#define MIXERLINE_COMPONENTTYPE_SRC_SYNTHESIZER (MIXERLINE_COMPONENTTYPE_SRC_FIRST + 4)
	#define MIXERLINE_COMPONENTTYPE_SRC_COMPACTDISC (MIXERLINE_COMPONENTTYPE_SRC_FIRST + 5)
	#define MIXERLINE_COMPONENTTYPE_SRC_TELEPHONE   (MIXERLINE_COMPONENTTYPE_SRC_FIRST + 6)
	#define MIXERLINE_COMPONENTTYPE_SRC_PCSPEAKER   (MIXERLINE_COMPONENTTYPE_SRC_FIRST + 7)
	#define MIXERLINE_COMPONENTTYPE_SRC_WAVEOUT     (MIXERLINE_COMPONENTTYPE_SRC_FIRST + 8)
	#define MIXERLINE_COMPONENTTYPE_SRC_AUXILIARY   (MIXERLINE_COMPONENTTYPE_SRC_FIRST + 9)
	#define MIXERLINE_COMPONENTTYPE_SRC_ANALOG      (MIXERLINE_COMPONENTTYPE_SRC_FIRST + 10)
	#define MIXERLINE_COMPONENTTYPE_SRC_LAST        (MIXERLINE_COMPONENTTYPE_SRC_FIRST + 10)
	*/
	//声卡控制节点信息
	struct strucCtrlUT {
		DWORD dwMixerCtrlID ;
		DWORD   dwType;                 /* MIXERLINE_TARGETTYPE_xxxx */
		DWORD   dwDeviceID;             /* target device ID of device type */
		DWORD   cMultipleItems;
		DWORD   dwLineId;
		unsigned int groupIndex ;
		unsigned int mixerIndex;
		unsigned int lineIndex;
		unsigned int ascIndex;//顺序
		unsigned int desIndex;//倒序
		unsigned int mcomponentType;
		unsigned int connIndex;
		unsigned int controlIndex;
		enCtrlType	 ctrlType ;
		enMuteType   muteType;
		strucChannelData sChannelData;
		WCHAR szName[MIXER_LONG_NAME_CHARS];
	} ;
	
	//存储声卡控制节点信息
	strucCtrlUT* mpCtrlUT ;
	//当前声卡控制节点索引
	unsigned int nCtrlUTIndex ;
	//声卡控制节点数目
	unsigned int nCtrlUTCount ;
};
