#pragma once

#include "MixerChannel.h"
#include "MixerDefine.h"

class CMixerManager
{
public:
	CMixerManager(void);
	~CMixerManager(void);

	//初始化  必须初始化才可以进行其他操作
	void Init(HWND hWnd);
	
	//获取当前总音量是否可用
	bool GetMixerControlTotalEnable();
	//获取伴音是否可用
	bool GetMixerControlWavoutEnable();
	//获取音量控制中的麦克风是否可用
	bool GetMixerControlOutmicEnable();
	//获取立体声混音是否可用
	bool GetMixerControlMixerEnable();
	//获取音频输入设备中音量大小
	bool GetMixerControlInmicEnable();
	//获取调整麦克风音量大小是否可用
	bool GetMixerControlWavInEnable();

	//音量控制 0 ~ 255 
	//设置总音量
	bool SetMixerControlTotalVolume(unsigned int nVolume);
	//获取总音量
	bool GetMixerControlTotalVolume(unsigned int& nVolume);
	//设置伴音
	bool SetMixerControlWavoutVolume(unsigned int nVolume);
	//获取伴音音量
	bool GetMixerControlWavoutVolume(unsigned int& nVolume);
	//设置音量控制中的麦克风
	bool SetMixerControlOutmicVolume(unsigned int nVolume);
	//获取音量控制中的麦克风音量
	bool GetMixerControlOutmicVolume(unsigned int& nVolume);

	//录音控制0~255
	//立体声混音
	bool SetMixerControlMixerVolume(unsigned int nVolume);
	//获取立体声混音音量
	bool GetMixerControlMixerVolume(unsigned int& nVolume);
	//录音控制中的麦克风
	bool SetMixerControlInmicVolume(unsigned int nVolume);
	//获取录音控制中的麦克风音量
	bool GetMixerControlInmicVolume(unsigned int& nVolume);

	//设置音频输入设备音量大小
	bool SetMixerControlWavInVolume(unsigned int nVolume);
	//设置音频输入设备音量大小
	bool GetMixerControlWavInVolume(unsigned int& nVolume);


	//静音设置
	//设置总音量静音
	bool SetMixerControlTotalMute(bool bEnable);    //主音量
	//获取总音量是否静音
	bool GetMixerControlTotalMute(bool& bEnable);
	//设置伴音静音
	bool SetMixerControlWavoutMute(bool bEnable);   //波形
	//获取伴音是否静音
	bool GetMixerControlWavoutMute(bool& bEnable);
	//设置音量控制中的麦克风的静音
	bool SetMixerControlOutmicMute(bool bEnable);   //输入监视器
	//获取音量控制中的麦克风的静音
	bool GetMixerControlOutmicMute(bool& bEnable);
	//以下情况分支持与不支持选择
	//立体声混音静音
	bool SetMixerControlMixerMute(bool bEnable);   //后线路输入
	//获取立体声混应是否静音
	bool GetMixerControlMixerMute(bool& bEnable);
	//设置录音控制中的麦克风静音
	bool SetMixerControlInmicMute(bool bEnable);   //后麦克风
	//获取录音控制中的麦克风是否静音
	bool GetMixerControlInmicMute(bool& bEnable);

	//判断控制器是 选择 还是 静音混音器 以此来判断系统支持哪种类型
	bool IsMixerControlSelect(bool& bSel);

	//设置唱歌
	bool SetMixerControlSingMode();
	//获取是否处于唱歌模式
	bool GetMixerControlSingMode(bool& bEnable);
	//设置与人聊天
	bool SetMixerControlChatMode();
	//获取是否与人聊天模式
	bool GetMixerControlChatMode(bool& bEnable);
	//在麦上放歌
	bool SetMixerControlPlayMode();
	//获取是否在麦上放歌模式
	bool GetMixerControlPlayMode(bool& bEnable);
	//获取当前所处的模式
	bool GetMixerControlMode(EnMixerControlMode& eMode);

	/**********************************************************
	一、声卡支持立体声混音情况 立体声混音为【选择、非选择】
	1.唱歌模式 听到背景声 也听到麦克风声音设置
	  立体声混音		选择     
	  波形				非静音
	  输出界面麦克风	非静音
    2.与人聊天 听不到背景声 可以听到麦克风声音
	  立体身混音        选择
	  波形              静音
	  输出界面麦克风    非静音
    3.在麦上放歌 听到背景声 听不到用户说话声音
	  立体声混音        选择
	  波形              非静音
	  输出界面麦克风    静音
	二声卡不支持立体声混音情况 立体声混音为【静音、非静音】
	1.唱歌模式 听到背景音乐 也听到麦克风声音设置
	  立体声混音        非静音
	  波形              非静音
	  输入监视器        非静音
    2.与人聊天模式 听不到背景声 可以听到麦克风用户说话声音
	  立体声混音        非静音
	  波形              静音
	  输入监视器        非静音
	3.在麦上放歌 听到背景音乐 听不到用户麦克风说话声音
	  立体声混音        非静音
	  波形              非静音
	  输入监视器        静音
	  ************************************************************/
protected:
	//获取混音控件类型
	void GetMixerLineParam(EnMixerControlType sType,StrucMixerParam& sMixerParam);
	//设置软件合成器静音
	bool SetMixerControlSoftMute(bool bEnable);
	//获取软件合成器是否静音
	bool GetMixerControlSoftMute(bool& bEnable);
	//设置前段输入静音
	bool SetMixerControlFrontMute(bool bEnable);
	//获取前段输入是否静音
	bool GetMixerControlFontMute(bool& bEnable);
	//设置立体声混音静音
	bool SetMixerControlTreeMute(bool bEnable);
	//获取立体声混音是否静音
	bool GetMixerControlTreeMute(bool& bEnable);
	//获取操作系统类型
	EnOsType GetOsType();
	//获取是否为vista Or Win7操作系统
	bool GetOsVistaOrWin7();

protected:
	CMixerChannel* m_pMixerChannel;
};