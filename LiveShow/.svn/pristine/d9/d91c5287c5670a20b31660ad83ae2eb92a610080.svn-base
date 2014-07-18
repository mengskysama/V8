#pragma once
#include "MixerDefine.h"
#include <string>

using namespace std;

class CMixerTools
{
public:
	CMixerTools(void);
	~CMixerTools(void);

public:
	//加载DLL文件
	void LoadDll();
	//当前系统音频是否支持混音设置  通过立体声混音这个选项是否是选择 来判断
	bool IsMixerControlSupportSelect(bool& bSel);
	//立体声混音类型的选项是 选择 还是 静音
	bool IsMixerControlMixerSelect(bool& bSel);
	//后麦克风类型的选项是选择 还是 静音
	bool IsMixerControlInmicSelect(bool& bSel);
	//设置后麦克风 无论是选择 或 静音 选中或非选中
	bool SetMixerControlInmicMuteOrSelect(bool bSel);
	//设置立体声混音 选中 非选中状态
	bool SetMixerControlMixerMuteOrSelect(bool bSel);
	//设置唱歌模式
	bool SetMixerControlSingMode();
	//设置聊天模式
	bool SetMixerControlChatMode();
	//设置放歌模式
	bool SetMixerControlPlayMode();
	//获取是否唱歌模式
	bool GetMixerControlSingMode(bool& bEnable);
	//获取是否聊天模式
	bool GetMixerControlChatMode(bool& bEnable);
	//获取是否放歌模式
	bool GetMixerControlPlayMode(bool& bEnable);
	//获取当前系统模式 0:唱歌模式 1聊天模式 2播放音乐模式 3其他模式
	bool GetMixerControlMode(int* pnMode);
private:
	//获取当前文件路劲
	wstring GetCurrPath();
	//判断指定组件类型 选项是 选择 还是静音
	bool IsMixerControlMuteOrSelect(int componentType,int lineType,bool& bSel);
	//获取控件类型 EnMixerComponentType 
	int GetComponentType(unsigned int nComponentType);
	//设置指定的组件选项 选择 或 静音 选中 或 不选中
	bool SetMixerControlMuteOrSelect(int componentType,int lineType,bool bSel);
	//判断当前操作必要条件是否满足
	bool IsLoad();
private:
	bool bLoad;//是否加载DLL
	bool bSucc;//加载DLL是否成功
	HINSTANCE hInstance;
	pfIsMixerControlMuteOrSelect funMixerControlMuteOrSelect;
	pfSetMixerControlMuteOrSelect funSetMixerControlMuteOrSelect;
	pfSetMixerControlSingMode funSetMixerControlSingMode;
	pfSetMixerControlSingMode funSetMixerControlChatMode;
	pfSetMixerControlSingMode funSetMixerControlPlayMode;
	pfGetMixerControlSingMode funGetMixerControlSingMode;
	pfGetMixerControlSingMode funGetMixerControlChatMode;
	pfGetMixerControlSingMode funGetMixerControlPlayMode;
	pfGetMixerControlMode     funGetMixerControlMode;
};

extern CMixerTools m_MixerTools;