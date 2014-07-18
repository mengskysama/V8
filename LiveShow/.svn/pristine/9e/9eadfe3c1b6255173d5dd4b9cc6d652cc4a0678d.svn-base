#include "StdAfx.h"
#include <string>
#include <iostream>
#include <fstream>
#include "MixerTools.h"
#include "MixerManager.h"

using namespace std;

CMixerTools m_MixerTools;

CMixerTools::CMixerTools(void)
{
	hInstance = NULL;
	bLoad = false;
	bSucc = false;
	funMixerControlMuteOrSelect = NULL;
	funMixerControlMuteOrSelect= NULL;
	funSetMixerControlMuteOrSelect= NULL;
	funSetMixerControlSingMode= NULL;
	funSetMixerControlChatMode= NULL;
	funSetMixerControlPlayMode= NULL;
	funGetMixerControlSingMode=NULL;
	funGetMixerControlChatMode=NULL;
	funGetMixerControlPlayMode=NULL;
	funGetMixerControlMode=NULL;
}

CMixerTools::~CMixerTools(void)
{
try
{
	if (hInstance)
	{
		FreeLibrary(hInstance);
	}
}
catch (...)
{
	OutputDebugStr(L"Audio Tools DLL is error!\n");
}
}

//加载DLL
void CMixerTools::LoadDll()
{
	return;
	if(bLoad) return;
	bLoad = true;
	wstring str = GetCurrPath();
	str.append(MIXERTOOLSNAME);
	fstream _file;
	_file.open(str.c_str(),ios::in);
	if(!_file)
	{
		bSucc = false;
		return;
	}
	try
	{
		hInstance=LoadLibrary(MIXERTOOLSNAME);
		if (!hInstance)
		{
			bSucc = false;
			return;
		}
		//指定控制器 选择 或 静音类型
		funMixerControlMuteOrSelect=(pfIsMixerControlMuteOrSelect)GetProcAddress(hInstance, "IsMixerControlMuteOrSelect");
		funSetMixerControlMuteOrSelect = (pfSetMixerControlMuteOrSelect)GetProcAddress(hInstance, "SetMixerControlMuteOrSelect");
		funSetMixerControlSingMode=(pfSetMixerControlSingMode)GetProcAddress(hInstance, "SetMixerControlSing");
		funSetMixerControlChatMode=(pfSetMixerControlSingMode)GetProcAddress(hInstance, "SetMixerControlChat");
		funSetMixerControlPlayMode=(pfSetMixerControlSingMode)GetProcAddress(hInstance, "SetMixerControlPlay");
		funGetMixerControlSingMode=(pfGetMixerControlSingMode)GetProcAddress(hInstance, "GetMixerControlSing");
		funGetMixerControlChatMode=(pfGetMixerControlSingMode)GetProcAddress(hInstance, "GetMixerControlChat");
		funGetMixerControlPlayMode=(pfGetMixerControlSingMode)GetProcAddress(hInstance, "GetMixerControlPlay");
		funGetMixerControlMode=(pfGetMixerControlMode)GetProcAddress(hInstance, "GetMixerControlMode");
		bSucc = true;
	}
	catch (...)
	{
		bSucc = false;
	}
}

bool CMixerTools::IsLoad()
{
// 	if (!bLoad) LoadDll();    用替代方法实现
// 	if (!bSucc) return false;
	return true;
}

//获取当前路径
wstring CMixerTools::GetCurrPath()
{
	WCHAR buf[5000] = {0}; 
	int nlen =  ::GetModuleFileName(NULL,buf,5000); 

	while(TRUE) 
	{ 
		if   (buf[nlen--]== '\\') 
			break; 
	} 
	++nlen;
	buf[++nlen] = '\0'; 
	wstring path = buf; 
	return path; 
}

//判断指定组件类型 选项是 选择 还是静音
bool CMixerTools::IsMixerControlMuteOrSelect(int componentType,int lineType,bool& bSel)
{
	if (IsLoad() && funMixerControlMuteOrSelect)
	{
		return (funMixerControlMuteOrSelect(componentType,lineType,&bSel) == 0);
	}
	else
	{
		return false;
	}
}

//设置指定的组件选项 选择 或 静音 选中 或 不选中
bool CMixerTools::SetMixerControlInmicMuteOrSelect(bool bSel)
{
	if (IsLoad() && funSetMixerControlMuteOrSelect)
	{
		return (funSetMixerControlMuteOrSelect(GetComponentType(MIXERLINE_COMPONENTTYPE_SRC_WAVEOUT),(int)MIXER_ALL,bSel) == 0);
	}
	else
	{
		return false;
	}
}

//当前系统音频是否支持混音设置  通过立体声混音这个选项是否是选择 来判断
bool CMixerTools::IsMixerControlSupportSelect(bool& bSel)
{
	return 	IsMixerControlMixerSelect(bSel);
}

//立体声混音类型的选项是 选择 还是 静音
bool CMixerTools::IsMixerControlMixerSelect(bool& bSel)
{
	if (IsLoad() && funMixerControlMuteOrSelect)
	{
		return (funMixerControlMuteOrSelect(GetComponentType(MIXERLINE_COMPONENTTYPE_SRC_WAVEOUT),(int)MIXER_ALL,&bSel) == 0);
	}
	else
	{
		return false;
	}
}

//设置立体声混音 选中 非选中状态
bool CMixerTools::SetMixerControlMixerMuteOrSelect(bool bSel)
{
	if (IsLoad() && funMixerControlMuteOrSelect)
	{
		return (funSetMixerControlMuteOrSelect(GetComponentType(MIXERLINE_COMPONENTTYPE_SRC_WAVEOUT),(int)MIXER_ALL,bSel) == 0);
	}
	else
	{
		return false;
	}
}

//后麦克风类型的选项是选择 还是 静音
bool CMixerTools::IsMixerControlInmicSelect(bool& bSel)
{
	if (IsLoad() && funMixerControlMuteOrSelect)
	{
		return (funMixerControlMuteOrSelect(GetComponentType(MIXERLINE_COMPONENTTYPE_SRC_MICROPHONE),(int)MIXER_IN,&bSel) == 0);
	}
	else
	{
		return false;
	}
}

//设置指定的组件选项 选择 或 静音 选中 或 不选中
bool CMixerTools::SetMixerControlMuteOrSelect(int componentType,int lineType,bool bSel)
{
	if (IsLoad() && funMixerControlMuteOrSelect)
	{
		return (funSetMixerControlMuteOrSelect(componentType,lineType,bSel) == 0);
	}
	else
	{
		return false;
	}
}

//获取控件类型 EnMixerComponentType 
int CMixerTools::GetComponentType(unsigned int nComponentType)
{
	EnMixerComponentType enType =(EnMixerComponentType)NULL;
	switch(nComponentType)
	{
	case MIXERLINE_COMPONENTTYPE_DST_SPEAKERS://音量控制
		{
			enType = ctDstSpeakers;
		}
	case MIXERLINE_COMPONENTTYPE_SRC_WAVEOUT://波形,立体声混音
		{
			enType = ctSrcWaveOut;
		}
		break;
	case MIXERLINE_COMPONENTTYPE_SRC_SYNTHESIZER://软件混合器
		{
			enType = ctSrcSynthesizer;
		}
		break;
	case MIXERLINE_COMPONENTTYPE_SRC_COMPACTDISC://CD
		{
			enType = ctSrcCD;
		}
		break;
	case MIXERLINE_COMPONENTTYPE_SRC_LINE://输入监视器,后线输入
		{
			enType = ctSrcLine;
		}
		break;
	case MIXERLINE_COMPONENTTYPE_DST_WAVEIN://录音控制
		{
			enType = ctDstWaveIn;
		}
		break;
	case MIXERLINE_COMPONENTTYPE_SRC_MICROPHONE://后麦克风 前输入
		{
			enType = ctSrcMicrophone;
		}
		break;
	case MIXERLINE_COMPONENTTYPE_DST_TELEPHONE:
		{
			enType = ctDstTelephone;
		}
		break;
	case MIXERLINE_COMPONENTTYPE_DST_UNDEFINED:
		{
			enType = ctDstUndefined;
		}
		break;
	case MIXERLINE_COMPONENTTYPE_DST_VOICEIN:
		{
			enType = ctDstVoiceIn;
		}
		break;
	case MIXERLINE_COMPONENTTYPE_SRC_ANALOG: //立体声混音
		{
			enType = ctSrcAnalog;
		}
		break;
	case MIXERLINE_COMPONENTTYPE_SRC_DIGITAL:
		{
			enType = ctSrcDigital;
		}
		break;
	case MIXERLINE_COMPONENTTYPE_SRC_PCSPEAKER:
		{
			enType = ctSrcPCSpeaker;
		}
		break;
	case MIXERLINE_COMPONENTTYPE_SRC_TELEPHONE:
		{
			enType = ctSrcTelephone;
		}
		break;
	case MIXERLINE_COMPONENTTYPE_SRC_UNDEFINED:
		{
			enType = ctSrcUndefined;
		}
		break;
	default:
		{
			enType = ctDstSpeakers;
		}
	}
	return enType;
}

//设置唱歌模式
bool CMixerTools::SetMixerControlSingMode()
{
	if (IsLoad() && funSetMixerControlSingMode)
	{
		return (funSetMixerControlSingMode()==0);
	}
	else
	{
		return false;
	}
}
//设置聊天模式
bool CMixerTools::SetMixerControlChatMode()
{
	if (IsLoad() && funSetMixerControlChatMode)
	{
		return (funSetMixerControlChatMode()==0);
	}
	else
	{
		return false;
	}
}
//设置放歌模式
bool CMixerTools::SetMixerControlPlayMode()
{
	if (IsLoad() && funSetMixerControlPlayMode)
	{
		return (funSetMixerControlPlayMode()==0);
	}
	else
	{
		return false;
	}
}

//获取是否唱歌模式
bool CMixerTools::GetMixerControlSingMode(bool& bEnable)
{
	if (IsLoad() && funGetMixerControlSingMode)
	{
		return (funGetMixerControlSingMode(&bEnable)==0);
	}
	else
	{
		return false;
	}
}

//获取是否聊天模式
bool CMixerTools::GetMixerControlChatMode(bool& bEnable)
{
	if (IsLoad() && funGetMixerControlChatMode)
	{
		return (funGetMixerControlChatMode(&bEnable)==0);
	}
	else
	{
		return false;
	}
}

//获取是否放歌模式
bool CMixerTools::GetMixerControlPlayMode(bool& bEnable)
{
	if (IsLoad() && funGetMixerControlPlayMode)
	{
		return (funGetMixerControlPlayMode(&bEnable)==0);
	}
	else
	{
		return false;
	}
}

//获取当前系统模式 0:唱歌模式 1聊天模式 2播放音乐模式 3其他模式
bool CMixerTools::GetMixerControlMode(int* pnMode)
{
	if (IsLoad() && funGetMixerControlMode)
	{
		return (funGetMixerControlMode(pnMode)==0);
	}
	else
	{
		return false;
	}
}
