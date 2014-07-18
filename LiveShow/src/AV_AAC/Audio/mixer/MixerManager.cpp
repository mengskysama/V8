#include "StdAfx.h"
#include "MixerManager.h"
#include "MixerTools.h"
#include "MixerOther.h"
#include "MyAssert.h"
#include <tchar.h>

CMixerManager::CMixerManager(void)
{
	m_pMixerChannel = NULL;
}

CMixerManager::~CMixerManager(void)
{
	if (m_pMixerChannel) delete m_pMixerChannel;
}

void CMixerManager::Init(HWND hWnd)
{
	if (!m_pMixerChannel) m_pMixerChannel = new CMixerChannel;
	m_pMixerChannel->Init(hWnd);
	m_MixerTools.LoadDll();
}

void CMixerManager::GetMixerLineParam(EnMixerControlType sType,StrucMixerParam& sMixerParam)
{
	switch(sType)
	{
	case MIXER_CONTROL_TOTAL_TYPE://总音量
		sMixerParam.dwLineType = MIXERLINE_TARGETTYPE_WAVEOUT;
		sMixerParam.nControlType = MIXERLINE_COMPONENTTYPE_DST_SPEAKERS;
		sMixerParam.enType = MIXER_OUT;
		break;
	case MIXER_CONTROL_WAVOUT_TYPE://伴奏 波形
		sMixerParam.dwLineType = MIXERLINE_TARGETTYPE_WAVEOUT;
		sMixerParam.nControlType = MIXERLINE_COMPONENTTYPE_SRC_WAVEOUT;
		sMixerParam.enType = MIXER_OUT;
		break;
	case MIXER_CONTROL_OUTMIC_TYPE1://麦克风 --输出  输入监视器
		sMixerParam.dwLineType = MIXERLINE_TARGETTYPE_UNDEFINED;
		sMixerParam.nControlType = MIXERLINE_COMPONENTTYPE_SRC_LINE;
		sMixerParam.enType = MIXER_OUT;
		break;
	case MIXER_CONTROL_OUTMIC_TYPE0://音量控制中的麦克风
		sMixerParam.dwLineType = MIXERLINE_TARGETTYPE_UNDEFINED;
		sMixerParam.nControlType = MIXERLINE_COMPONENTTYPE_SRC_MICROPHONE;
		sMixerParam.enType = MIXER_OUT;
		break;
	case MIXER_CONTROL_MIXER_TYPE://立体声混音  --输入  后线路输入
		sMixerParam.dwLineType = MIXERLINE_TARGETTYPE_UNDEFINED;
		sMixerParam.nControlType = MIXERLINE_COMPONENTTYPE_SRC_LINE;
		sMixerParam.enType = MIXER_IN;
		break;
	case MIXER_CONTROL_INMIC_TYPE://麦克风   --后麦克风
		sMixerParam.dwLineType = MIXERLINE_TARGETTYPE_WAVEIN;
		sMixerParam.nControlType = MIXERLINE_COMPONENTTYPE_SRC_MICROPHONE;
		sMixerParam.enType = MIXER_IN;
		break;
	case MIXER_CONTROL_SOFT_TYPE://软件合成器
		sMixerParam.dwLineType = MIXERLINE_TARGETTYPE_MIDIOUT;
		sMixerParam.nControlType = MIXERLINE_COMPONENTTYPE_SRC_SYNTHESIZER;
		sMixerParam.enType = MIXER_OUT;
		break;
	case MIXER_CONTROL_FRONT_TYPE://前段输入
		sMixerParam.dwLineType = MIXERLINE_TARGETTYPE_WAVEIN;
		sMixerParam.nControlType = MIXERLINE_COMPONENTTYPE_SRC_MICROPHONE;
		sMixerParam.enType = MIXER_IN;
		break;
	case MIXER_CONTROL_TREE_TYPE://立体声混音 --混音  Stereo  Mix
		sMixerParam.dwLineType = MIXERLINE_TARGETTYPE_WAVEOUT;
		sMixerParam.nControlType = MIXERLINE_COMPONENTTYPE_SRC_WAVEOUT;
		sMixerParam.enType = MIXER_OTHER;
		break;
	default:
		sMixerParam.dwLineType = -1;
		sMixerParam.nControlType = -1;
	}
}

//音量控制 0 ~ 255 
//设置总音量
bool CMixerManager::SetMixerControlTotalVolume(unsigned int nVolume)
{
	//Win7 Or Vista系统
	if (GetOsVistaOrWin7()) 
	{
		return g_mixerOther.SetMixerSpeakerVolume(nVolume);
	}
	
	//XP等系统
	if (!GetMixerControlTotalEnable()) 
		return false;
	unsigned int nTotalVolume = 0;
	unsigned int nGroupId = -1;
	StrucMixerParam sMixerParam;
	sMixerParam.enctrlType = Volume;
	GetMixerLineParam(MIXER_CONTROL_TOTAL_TYPE,sMixerParam); //获取总音量参数
	nTotalVolume = nVolume * 0xffff / 255;
	if (m_pMixerChannel->GetMixerGroupId(nGroupId,sMixerParam)>0)
	{
		strucChannelSet sSet;
		sSet.stateData.Left.dwValue = sSet.stateData.Right.dwValue = nTotalVolume;
		m_pMixerChannel->UpdateMixerControlState(nGroupId,sSet);
		return true;
	}
	return false;
}

//获取总音量
bool CMixerManager::GetMixerControlTotalVolume(unsigned int& nVolume)
{
	//Win7 Or Vista系统
	if (GetOsVistaOrWin7()) 
	{
		return g_mixerOther.GetMixerSpeakerVolume(nVolume);
	}

	if (!GetMixerControlTotalEnable()) 
		return false;
	unsigned int nTotalVolume = 0;
	unsigned int nGroupId = -1;
	StrucMixerParam sMixerParam;
	sMixerParam.enctrlType = Volume;
	GetMixerLineParam(MIXER_CONTROL_TOTAL_TYPE,sMixerParam); //获取总音量参数
	if(m_pMixerChannel->GetMixerGroupId(nGroupId,sMixerParam)>0)
	{
		strucChannelSet sSet;	
		m_pMixerChannel->GetMixerControlState(nGroupId,sSet);
		nTotalVolume = sSet.stateData.Left.dwValue;
		nVolume = nTotalVolume *  255/ 0xffff;
		return true;
	}
	return false;
}

//设置总音量静音
bool CMixerManager::SetMixerControlTotalMute(bool bEnable)
{
	//Win7 Or Vista系统
	if (GetOsVistaOrWin7()) 
	{
		return g_mixerOther.SetMixerSpeakerMute(bEnable);
	}

	//XP等系统
	if (!GetMixerControlTotalEnable()) 
		return false;
	unsigned int nGroupId = -1;
	StrucMixerParam sMixerParam;
	sMixerParam.enctrlType = Mute;
	GetMixerLineParam(MIXER_CONTROL_TOTAL_TYPE,sMixerParam); 
	if (m_pMixerChannel->GetMixerGroupId(nGroupId,sMixerParam)>0)
	{
		strucChannelSet sSet;
		sSet.bChecked = bEnable;
		m_pMixerChannel->UpdateMixerControlState(nGroupId,sSet);
		return true;
	}
	return false;
}

//获取当前总音量是否可用
bool CMixerManager::GetMixerControlTotalEnable()
{
	switch(GetOsType())
	{
	case OsWindowsUnKnown:
	case OsWindowsXP:
	case OsWindowsVista:
	case OsWindows7:
		{
			return true;
		}
		break;
	default:
		return false;
	}
}

//获取总音量是否静音
bool CMixerManager::GetMixerControlTotalMute(bool& bEnable)
{
	//Win7 Or Vista系统
	if (GetOsVistaOrWin7()) 
	{
		BOOL bEnabled = FALSE;
		bool bret= g_mixerOther.GetMixerSpeakerMute(bEnabled);
		bEnable = bEnabled;
		return bret;
	}

	//XP等系统
	if (!GetMixerControlTotalEnable()) 
		return false;
	unsigned int nTotalVolume = 0;
	unsigned int nGroupId = -1;
	StrucMixerParam sMixerParam;
	sMixerParam.enctrlType = Mute;
	GetMixerLineParam(MIXER_CONTROL_TOTAL_TYPE,sMixerParam); 
	if(m_pMixerChannel->GetMixerGroupId(nGroupId,sMixerParam)>0)
	{
		strucChannelSet sSet;	
		m_pMixerChannel->GetMixerControlState(nGroupId,sSet);
		bEnable = sSet.bChecked;
		return true;
	}
	return false;
}

//设置伴音
bool CMixerManager::SetMixerControlWavoutVolume(unsigned int nVolume)
{
	if (!GetMixerControlWavoutEnable())
		return false;
	unsigned int nTotalVolume = 0;
	unsigned int nGroupId = -1;
	StrucMixerParam sMixerParam;
	sMixerParam.enctrlType = Volume;
	GetMixerLineParam(MIXER_CONTROL_WAVOUT_TYPE,sMixerParam); //获取总音量参数
	nTotalVolume = nVolume * 0xffff / 255;
	if (m_pMixerChannel->GetMixerGroupId(nGroupId,sMixerParam)>0)
	{
		strucChannelSet sSet;
		sSet.stateData.Left.dwValue = sSet.stateData.Right.dwValue = nTotalVolume;
		m_pMixerChannel->UpdateMixerControlState(nGroupId,sSet);
		return true;
	}
	return false;
}

//获取伴音音量
bool CMixerManager::GetMixerControlWavoutVolume(unsigned int& nVolume)
{
	if (!GetMixerControlWavoutEnable())
		return false;
	unsigned int nTotalVolume = 0;
	unsigned int nGroupId = -1;
	StrucMixerParam sMixerParam;
	sMixerParam.enctrlType = Volume;
	GetMixerLineParam(MIXER_CONTROL_WAVOUT_TYPE,sMixerParam); //获取总音量参数
	if (m_pMixerChannel->GetMixerGroupId(nGroupId,sMixerParam)>0)
	{
		strucChannelSet sSet;	
		m_pMixerChannel->GetMixerControlState(nGroupId,sSet);
		nTotalVolume = sSet.stateData.Left.dwValue;
		nVolume = nTotalVolume *  255/ 0xffff;
		return true;
	}
	return false;
}


//设置伴音静音
bool CMixerManager::SetMixerControlWavoutMute(bool bEnable)
{
	if (!GetMixerControlWavoutEnable())
		return false;
	unsigned int nGroupId = -1;
	StrucMixerParam sMixerParam;
	sMixerParam.enctrlType = Mute;
	GetMixerLineParam(MIXER_CONTROL_WAVOUT_TYPE,sMixerParam); 
	if (m_pMixerChannel->GetMixerGroupId(nGroupId,sMixerParam)>0)
	{
		strucChannelSet sSet;
		sSet.bChecked = bEnable;
		m_pMixerChannel->UpdateMixerControlState(nGroupId,sSet);
		return true;
	}
	return false;
}

//获取伴音是否可用
bool CMixerManager::GetMixerControlWavoutEnable()
{
	switch(GetOsType())
	{
	case OsWindowsUnKnown:
	case OsWindowsXP:
		{
			return true;
		}
		break;
	case OsWindowsVista:
	case OsWindows7:
		{
			return false;
		}
		break;
	default:
		return false;
	}	
}

//获取伴音是否静音
bool CMixerManager::GetMixerControlWavoutMute(bool& bEnable)
{
	if (!GetMixerControlWavoutEnable())
		return false;
	unsigned int nTotalVolume = 0;
	unsigned int nGroupId = -1;
	StrucMixerParam sMixerParam;
	sMixerParam.enctrlType = Mute;
	GetMixerLineParam(MIXER_CONTROL_WAVOUT_TYPE,sMixerParam); 
	if(m_pMixerChannel->GetMixerGroupId(nGroupId,sMixerParam)>0)
	{
		strucChannelSet sSet;	
		m_pMixerChannel->GetMixerControlState(nGroupId,sSet);
		bEnable = sSet.bChecked;
		return true;
	}
	return false;
}


//设置音量控制中的麦克风
bool CMixerManager::SetMixerControlOutmicVolume(unsigned int nVolume)
{
	if (!GetMixerControlOutmicEnable())
		return false;
	unsigned int nTotalVolume = 0;
	unsigned int nGroupId = -1;
	StrucMixerParam sMixerParam;
	sMixerParam.enctrlType = Volume;
	sMixerParam.enType = MIXER_OUT;

	std::wstring str = _T("麦克风");
	std::wstring str1 = _T("Microphone");
	if (m_pMixerChannel->GetMixerGroupId(nGroupId,str,sMixerParam) == -1 && 
		m_pMixerChannel->GetMixerGroupId(nGroupId,str1,sMixerParam) == -1)//根据“麦克风”查找
	{
		GetMixerLineParam(MIXER_CONTROL_OUTMIC_TYPE0,sMixerParam); //线路名称查找
		if(m_pMixerChannel->GetMixerGroupId(nGroupId,sMixerParam) == -1)
		{
			GetMixerLineParam(MIXER_CONTROL_OUTMIC_TYPE1,sMixerParam); //线路输入查找
			if (m_pMixerChannel->GetMixerGroupId(nGroupId,sMixerParam) == -1)
			{
				return false;
			}
		}	
	}
	strucChannelSet sSet;
	nTotalVolume = nVolume * 0xffff / 255;
	sSet.stateData.Left.dwValue = sSet.stateData.Right.dwValue = nTotalVolume;
	m_pMixerChannel->UpdateMixerControlState(nGroupId,sSet);
	return true;
}

//获取音量控制中的麦克风是否可用
bool CMixerManager::GetMixerControlOutmicEnable()
{
	switch(GetOsType())
	{
	case OsWindowsUnKnown:
	case OsWindowsXP:
		{
			return true;
		}
		break;
	case OsWindowsVista:
	case OsWindows7:
		{
			return false;
		}
		break;
	default:
		return false;
	}	
}

//获取音量控制中的麦克风音量
bool CMixerManager::GetMixerControlOutmicVolume(unsigned int& nVolume)
{
	if (!GetMixerControlOutmicEnable())
		return false;
	unsigned int nTotalVolume = 0;
	unsigned int nGroupId = -1;
	StrucMixerParam sMixerParam;
	sMixerParam.enctrlType = Volume;
	sMixerParam.enType = MIXER_OUT;

	std::wstring str = _T("麦克风");
	std::wstring str1 = _T("Microphone");
	if (m_pMixerChannel->GetMixerGroupId(nGroupId,str,sMixerParam) == -1 && 
		m_pMixerChannel->GetMixerGroupId(nGroupId,str1,sMixerParam) == -1)//根据“麦克风”查找
	{
		GetMixerLineParam(MIXER_CONTROL_OUTMIC_TYPE0,sMixerParam); //线路名称查找
		if(m_pMixerChannel->GetMixerGroupId(nGroupId,sMixerParam) == -1)
		{
			GetMixerLineParam(MIXER_CONTROL_OUTMIC_TYPE1,sMixerParam); //线路输入查找
			if (m_pMixerChannel->GetMixerGroupId(nGroupId,sMixerParam) == -1)
			{
				return false;
			}
		}	
	}

	strucChannelSet sSet;	
	m_pMixerChannel->GetMixerControlState(nGroupId,sSet);
	nTotalVolume = sSet.stateData.Left.dwValue;
	nVolume = nTotalVolume *  255/  0xffff;
	return true;
}

//设置音量控制中的麦克风的静音
bool CMixerManager::SetMixerControlOutmicMute(bool bEnable)
{
	if (!GetMixerControlOutmicEnable())
		return false;
	unsigned int nGroupId = -1;
	StrucMixerParam sMixerParam;
	sMixerParam.enctrlType = Mute;
	sMixerParam.enType = MIXER_OUT;

	std::wstring str = _T("麦克风");
	std::wstring str1 = _T("Microphone");
	if (m_pMixerChannel->GetMixerGroupId(nGroupId,str,sMixerParam) == -1 && 
		m_pMixerChannel->GetMixerGroupId(nGroupId,str1,sMixerParam) == -1)//根据“麦克风”查找
	{
		GetMixerLineParam(MIXER_CONTROL_OUTMIC_TYPE0,sMixerParam); //线路名称查找
		if(m_pMixerChannel->GetMixerGroupId(nGroupId,sMixerParam) == -1)
		{
			GetMixerLineParam(MIXER_CONTROL_OUTMIC_TYPE1,sMixerParam); //线路输入查找
			if (m_pMixerChannel->GetMixerGroupId(nGroupId,sMixerParam) == -1)
			{
				return false;
			}
		}	
	}

	strucChannelSet sSet;
	sSet.bChecked = bEnable;
	m_pMixerChannel->UpdateMixerControlState(nGroupId,sSet);
	return true;
}

//获取音量控制中的麦克风的静音
bool CMixerManager::GetMixerControlOutmicMute(bool& bEnable)
{
	if (!GetMixerControlOutmicEnable())
		return false;
	unsigned int nTotalVolume = 0;
	unsigned int nGroupId = -1;
	StrucMixerParam sMixerParam;
	sMixerParam.enctrlType = Mute;
	sMixerParam.enType = MIXER_OUT;

	std::wstring str = _T("麦克风");
	std::wstring str1 = _T("Microphone");
	if (m_pMixerChannel->GetMixerGroupId(nGroupId,str,sMixerParam) == -1 && 
		m_pMixerChannel->GetMixerGroupId(nGroupId,str1,sMixerParam) == -1)//根据“麦克风”查找
	{
		GetMixerLineParam(MIXER_CONTROL_OUTMIC_TYPE0,sMixerParam); //线路名称查找
		if(m_pMixerChannel->GetMixerGroupId(nGroupId,sMixerParam) == -1)
		{
			GetMixerLineParam(MIXER_CONTROL_OUTMIC_TYPE1,sMixerParam); //线路输入查找
			if (m_pMixerChannel->GetMixerGroupId(nGroupId,sMixerParam) == -1)
			{
				return false;
			}
		}	
	}

	strucChannelSet sSet;	
	m_pMixerChannel->GetMixerControlState(nGroupId,sSet);
	bEnable = sSet.bChecked;
	return true;
}

//录音控制0~255
//立体声混音
bool CMixerManager::SetMixerControlMixerVolume(unsigned int nVolume)
{
	if (!GetMixerControlMixerEnable())
		return false;
	unsigned int nTotalVolume = 0;
	unsigned int nGroupId = -1;
	StrucMixerParam sMixerParam;
	sMixerParam.enctrlType = Volume;
	sMixerParam.enType = MIXER_OTHER;

	std::wstring str = _T("立体声");
	std::wstring str1 = _T("Stereo");
	if (m_pMixerChannel->GetMixerGroupId(nGroupId,str,sMixerParam) == -1 && 
		m_pMixerChannel->GetMixerGroupId(nGroupId,str1,sMixerParam) == -1)//根据“麦克风”查找
	{
		GetMixerLineParam(MIXER_CONTROL_MIXER_TYPE,sMixerParam); //获取总音量参数
		if (m_pMixerChannel->GetMixerGroupId(nGroupId,sMixerParam)==-1)
		{
			return false;
		}
	}

	strucChannelSet sSet;
	nTotalVolume = nVolume * 0xffff / 255;
	sSet.stateData.Left.dwValue = sSet.stateData.Right.dwValue = nTotalVolume;
	m_pMixerChannel->UpdateMixerControlState(nGroupId,sSet);
	return true;
}

//获取立体声混音音量
bool CMixerManager::GetMixerControlMixerVolume(unsigned int& nVolume)
{
	if (!GetMixerControlMixerEnable())
		return false;
	unsigned int nTotalVolume = 0;
	unsigned int nGroupId = -1;
	StrucMixerParam sMixerParam;
	sMixerParam.enctrlType = Volume;
	sMixerParam.enType = MIXER_OTHER;

	std::wstring str = _T("立体声");
	std::wstring str1 = _T("Stereo");
	if (m_pMixerChannel->GetMixerGroupId(nGroupId,str,sMixerParam) == -1 && 
		m_pMixerChannel->GetMixerGroupId(nGroupId,str1,sMixerParam) == -1)//根据“麦克风”查找
	{
		GetMixerLineParam(MIXER_CONTROL_MIXER_TYPE,sMixerParam); //获取总音量参数
		if (m_pMixerChannel->GetMixerGroupId(nGroupId,sMixerParam)==-1)
		{
			return false;
		}
	}
	strucChannelSet sSet;	
	m_pMixerChannel->GetMixerControlState(nGroupId,sSet);
	nTotalVolume = sSet.stateData.Left.dwValue;
	nVolume = nTotalVolume *  255/ 0xffff;
	return true;

}

//获取立体声混音是否可用
bool CMixerManager::GetMixerControlMixerEnable()
{
	switch(GetOsType())
	{
	case OsWindowsUnKnown:
	case OsWindowsXP:
		{
			return true;
		}
		break;
	case OsWindowsVista:
	case OsWindows7:
		{
			return false;
		}
		break;
	default:
		return false;
	}	
}

//立体声混音静音
bool CMixerManager::SetMixerControlMixerMute(bool bEnable)
{
	if (!GetMixerControlMixerEnable())
		return false;
	unsigned int nGroupId = -1;
	StrucMixerParam sMixerParam;
	sMixerParam.enctrlType = Volume;
	sMixerParam.enType = MIXER_OTHER;

	std::wstring str = _T("立体声");
	std::wstring str1 = _T("Stereo");
	if (m_pMixerChannel->GetMixerGroupId(nGroupId,str,sMixerParam) == -1 && 
		m_pMixerChannel->GetMixerGroupId(nGroupId,str1,sMixerParam) == -1)//根据“麦克风”查找
	{
		GetMixerLineParam(MIXER_CONTROL_MIXER_TYPE,sMixerParam); //获取总音量参数
		if (m_pMixerChannel->GetMixerGroupId(nGroupId,sMixerParam)==-1)
		{
			return false;
		}
	}
	
	if (m_pMixerChannel->IsMixerControlSelState(nGroupId))
	{
		strucChannelSet sSet;
		sSet.bChecked = bEnable;
		m_pMixerChannel->SetMixerControlSelState(nGroupId,sSet);
	}
	else
	{
		sMixerParam.enctrlType = Mute;
		if (m_pMixerChannel->GetMixerGroupId(nGroupId,str,sMixerParam) == -1 && 
			m_pMixerChannel->GetMixerGroupId(nGroupId,str1,sMixerParam) == -1)//根据“麦克风”查找
		{
			GetMixerLineParam(MIXER_CONTROL_MIXER_TYPE,sMixerParam); //获取总音量参数
			if (m_pMixerChannel->GetMixerGroupId(nGroupId,sMixerParam)==-1)
			{
				return false;
			}
		}

		strucChannelSet sSet;
		sSet.bChecked = bEnable;
		m_pMixerChannel->UpdateMixerControlState(nGroupId,sSet);
	}
	return true;
}

//获取立体声混应是否静音
bool CMixerManager::GetMixerControlMixerMute(bool& bEnable)
{
	if (!GetMixerControlMixerEnable())
		return false;
	unsigned int nGroupId = -1;
	StrucMixerParam sMixerParam;
	sMixerParam.enctrlType = Volume;
	sMixerParam.enType = MIXER_OTHER;

	std::wstring str = _T("立体声");
	std::wstring str1 = _T("Stereo");
	if (m_pMixerChannel->GetMixerGroupId(nGroupId,str,sMixerParam) == -1 && 
		m_pMixerChannel->GetMixerGroupId(nGroupId,str1,sMixerParam) == -1)//根据“麦克风”查找
	{
		GetMixerLineParam(MIXER_CONTROL_MIXER_TYPE,sMixerParam); //获取总音量参数
		if (m_pMixerChannel->GetMixerGroupId(nGroupId,sMixerParam)==-1)
		{
			return false;
		}
	}
	
	if (m_pMixerChannel->IsMixerControlSelState(nGroupId))
	{
		strucChannelSet sSet;
		m_pMixerChannel->GetMixerControlSelState(nGroupId,sSet);
		bEnable = sSet.bChecked;
	}
	else
	{
		sMixerParam.enctrlType = Mute;
		if (m_pMixerChannel->GetMixerGroupId(nGroupId,str,sMixerParam) == -1 && 
			m_pMixerChannel->GetMixerGroupId(nGroupId,str1,sMixerParam) == -1)//根据“麦克风”查找
		{
			GetMixerLineParam(MIXER_CONTROL_MIXER_TYPE,sMixerParam); //获取总音量参数
			if (m_pMixerChannel->GetMixerGroupId(nGroupId,sMixerParam)==-1)
			{
				return false;
			}
		}

		strucChannelSet sSet;	
		m_pMixerChannel->GetMixerControlState(nGroupId,sSet);
		bEnable = sSet.bChecked;
	}
	return true;
}

//判断控制器是 选择 还是 静音状态
bool CMixerManager::IsMixerControlSelect(bool& bSel)
{
	unsigned int nGroupId = -1;
	StrucMixerParam sMixerParam;
	sMixerParam.enctrlType = Volume;
	sMixerParam.enType = MIXER_OTHER;

	std::wstring str = _T("立体声");
	std::wstring str1 = _T("Stereo");
	if (m_pMixerChannel->GetMixerGroupId(nGroupId,str,sMixerParam) == -1 && 
		m_pMixerChannel->GetMixerGroupId(nGroupId,str1,sMixerParam) == -1)//根据“麦克风”查找
	{
		GetMixerLineParam(MIXER_CONTROL_MIXER_TYPE,sMixerParam); //获取总音量参数
		if (m_pMixerChannel->GetMixerGroupId(nGroupId,sMixerParam)==-1)
		{
			bSel = false;
			return false;
		}
	}

	bSel = m_pMixerChannel->IsMixerControlSelState(nGroupId);
	return true;
}



//设置后麦克风静音
bool CMixerManager::SetMixerControlInmicMute(bool bEnable)
{
	//Win7 Or Vista系统
	if (GetOsVistaOrWin7()) 
	{
		return g_mixerOther.SetMixerMicMute(bEnable);
	}

	if (!GetMixerControlInmicEnable())
		return false;
	unsigned int nGroupId = -1;
	StrucMixerParam sMixerParam;
	sMixerParam.enctrlType = Volume;
	GetMixerLineParam(MIXER_CONTROL_INMIC_TYPE,sMixerParam); 
	if (m_pMixerChannel->GetMixerGroupId(nGroupId,sMixerParam)==-1)
	{
		return false;
	}

	strucChannelSet sSet;
	sSet.bChecked = bEnable;
	if (m_pMixerChannel->IsMixerControlSelState(nGroupId))
	{
		m_pMixerChannel->SetMixerControlSelState(nGroupId,sSet);
	}
	else
	{
		m_pMixerChannel->UpdateMixerControlState(nGroupId,sSet);
	}
	return true;
}

//获取录音控制中的麦克风是否静音
bool CMixerManager::GetMixerControlInmicMute(bool& bEnable)
{
	//Win7 Or Vista系统
	if (GetOsVistaOrWin7()) 
	{
		BOOL bEnabled = FALSE;
		bool bRet = g_mixerOther.GetMixerMicMute(bEnabled);
		bEnable = bEnabled;
		return bRet;
	}

	if (!GetMixerControlInmicEnable())
		return false;
	unsigned int nTotalVolume = 0;
	unsigned int nGroupId = -1;
	StrucMixerParam sMixerParam;
	sMixerParam.enctrlType = Volume;
	GetMixerLineParam(MIXER_CONTROL_INMIC_TYPE,sMixerParam); 
	if (m_pMixerChannel->GetMixerGroupId(nGroupId,sMixerParam)==-1)
	{
		return false;
	}
	
	strucChannelSet sSet;	
	if (m_pMixerChannel->IsMixerControlSelState(nGroupId))
	{
		m_pMixerChannel->GetMixerControlSelState(nGroupId,sSet);
	}
	else
	{
		m_pMixerChannel->GetMixerControlState(nGroupId,sSet);
	}
	bEnable = sSet.bChecked;
	return true;
}

//录音控制中的麦克风
bool CMixerManager::SetMixerControlInmicVolume(unsigned int nVolume)
{
	//Win7 Or Vista系统
	if (GetOsVistaOrWin7()) 
	{
		return g_mixerOther.SetMixerMicVolume(nVolume);
	}

	if (!GetMixerControlInmicEnable())
		return false;
	unsigned int nTotalVolume = 0;
	unsigned int nGroupId = -1;
	StrucMixerParam sMixerParam;
	sMixerParam.enctrlType = Volume;
	GetMixerLineParam(MIXER_CONTROL_INMIC_TYPE,sMixerParam); //获取总音量参数
	nTotalVolume = nVolume * 65535 / 255;
	if (m_pMixerChannel->GetMixerGroupId(nGroupId,sMixerParam)>0)
	{
		strucChannelSet sSet;
		sSet.stateData.Left.dwValue = sSet.stateData.Right.dwValue = nTotalVolume;
		m_pMixerChannel->UpdateMixerControlState(nGroupId,sSet);
		return true;
	}
	return false;
}

//获取录音控制中的麦克风音量
bool CMixerManager::GetMixerControlInmicVolume(unsigned int& nVolume)
{
	//Win7 Or Vista系统
	if (GetOsVistaOrWin7()) 
	{
		return g_mixerOther.GetMixerMicVolume(nVolume);
	}

	if (!GetMixerControlInmicEnable())
		return false;
	unsigned int nTotalVolume = 0;
	unsigned int nGroupId = -1;
	StrucMixerParam sMixerParam;
	sMixerParam.enctrlType = Volume;
	GetMixerLineParam(MIXER_CONTROL_INMIC_TYPE,sMixerParam); //获取总音量参数
	if (m_pMixerChannel->GetMixerGroupId(nGroupId,sMixerParam)>0)
	{
		strucChannelSet sSet;	
		m_pMixerChannel->GetMixerControlState(nGroupId,sSet);
		nTotalVolume = sSet.stateData.Left.dwValue;
		nVolume = nTotalVolume *  255/ 65535;
		return true;
	}
	return false;
}

//获取音频输入设备中音量大小
bool CMixerManager::GetMixerControlInmicEnable()
{
	switch(GetOsType())
	{
	case OsWindowsUnKnown:
	case OsWindowsXP:
		{
			return true;
		}
		break;
	case OsWindowsVista:
	case OsWindows7:
		{
			return false;
		}
		break;
	default:
		return false;
	}	
}

//设置音频输入设备音量大小
bool CMixerManager::SetMixerControlWavInVolume(unsigned int nVolume)
{
	//Win7 Or Vista系统
	if (GetOsVistaOrWin7()) 
	{
		return g_mixerOther.SetMixerMicBoostVolume(nVolume);
	}
	if (!GetMixerControlWavInEnable())
		return false;
	unsigned int nGroupId = -1;
	unsigned int nTotalVolume = 0;
	if (m_pMixerChannel->GetMixerControlSelGroupId(nGroupId)!=-1)
	{
		strucChannelSet sSet;
		nTotalVolume = nVolume * 0xffff / 255;
		sSet.stateData.Left.dwValue = sSet.stateData.Right.dwValue = nTotalVolume;
		m_pMixerChannel->UpdateMixerControlState(nGroupId,sSet);
		return true;
	}
	return false;
}

//设置音频输入设备音量大小
bool CMixerManager::GetMixerControlWavInVolume(unsigned int& nVolume)
{
	//Win7 Or Vista系统
	if (GetOsVistaOrWin7()) 
	{
		return g_mixerOther.GetMixerMicBoostVolume(nVolume);
	}
	if (!GetMixerControlWavInEnable())
		return false;
	unsigned int nGroupId = -1;
	unsigned int nTotalVolume = 0;
	if (m_pMixerChannel->GetMixerControlSelGroupId(nGroupId)!=-1)
	{
		strucChannelSet sSet;	
		m_pMixerChannel->GetMixerControlState(nGroupId,sSet);
		nTotalVolume = sSet.stateData.Left.dwValue;
		nVolume = nTotalVolume *  255/ 0xffff;
		return true;
	}
	return false;
}

//获取调整麦克风音量大小是否可用
bool CMixerManager::GetMixerControlWavInEnable()
{
	switch(GetOsType())
	{
	case OsWindowsUnKnown:
	case OsWindowsXP:
	case OsWindowsVista:
	case OsWindows7:
		{
			return true;
		}
		break;
	default:
		return false;
	}
}

//设置软件合成器静音 //MIXERLINE_COMPONENTTYPE_SRC_SYNTHESIZER
bool CMixerManager::SetMixerControlSoftMute(bool bEnable)
{
	unsigned int nGroupId = -1;
	StrucMixerParam sMixerParam;
	sMixerParam.enctrlType = Mute;
	GetMixerLineParam(MIXER_CONTROL_SOFT_TYPE,sMixerParam); 
	if (m_pMixerChannel->GetMixerGroupId(nGroupId,sMixerParam)>0)
	{
		strucChannelSet sSet;
		sSet.bChecked = bEnable;
		m_pMixerChannel->UpdateMixerControlState(nGroupId,sSet);
		return true;
	}
	return false;
}

//获取软件合成器是否静音
bool CMixerManager::GetMixerControlSoftMute(bool& bEnable)
{
	unsigned int nTotalVolume = 0;
	unsigned int nGroupId = -1;
	StrucMixerParam sMixerParam;
	sMixerParam.enctrlType = Mute;
	GetMixerLineParam(MIXER_CONTROL_SOFT_TYPE,sMixerParam); 
	if(m_pMixerChannel->GetMixerGroupId(nGroupId,sMixerParam)>0)
	{
		strucChannelSet sSet;	
		m_pMixerChannel->GetMixerControlState(nGroupId,sSet);
		bEnable = sSet.bChecked;
		return true;
	}
	return false;
}

//设置前段输入静音
bool CMixerManager::SetMixerControlFrontMute(bool bEnable)
{
	unsigned int nGroupId = -1;
	StrucMixerParam sMixerParam;
	sMixerParam.enctrlType = Mute;
	GetMixerLineParam(MIXER_CONTROL_FRONT_TYPE,sMixerParam); 
	if (m_pMixerChannel->GetMixerGroupId(nGroupId,sMixerParam)>0)
	{
		strucChannelSet sSet;
		sSet.bChecked = bEnable;
		m_pMixerChannel->UpdateMixerControlState(nGroupId,sSet);
		return true;
	}
	return false;
}

//获取前段输入是否静音
bool CMixerManager::GetMixerControlFontMute(bool& bEnable)
{
	unsigned int nTotalVolume = 0;
	unsigned int nGroupId = -1;
	StrucMixerParam sMixerParam;
	sMixerParam.enctrlType = Mute;
	GetMixerLineParam(MIXER_CONTROL_FRONT_TYPE,sMixerParam); 
	if(m_pMixerChannel->GetMixerGroupId(nGroupId,sMixerParam)>0)
	{
		strucChannelSet sSet;	
		m_pMixerChannel->GetMixerControlState(nGroupId,sSet);
		bEnable = sSet.bChecked;
		return true;
	}
	return false;
}

//设置立体声混音静音
bool CMixerManager::SetMixerControlTreeMute(bool bEnable)
{
	unsigned int nGroupId = -1;
	StrucMixerParam sMixerParam;
	sMixerParam.enctrlType = Mute;
	GetMixerLineParam(MIXER_CONTROL_TREE_TYPE,sMixerParam); 
	if (m_pMixerChannel->GetMixerGroupId(nGroupId,sMixerParam)>0)
	{
		strucChannelSet sSet;
		sSet.bChecked = bEnable;
		m_pMixerChannel->UpdateMixerControlState(nGroupId,sSet);
		return true;
	}
	return false;
}

//获取立体声混音是否静音
bool CMixerManager::GetMixerControlTreeMute(bool& bEnable)
{
	unsigned int nTotalVolume = 0;
	unsigned int nGroupId = -1;
	StrucMixerParam sMixerParam;
	sMixerParam.enctrlType = Mute;
	GetMixerLineParam(MIXER_CONTROL_TREE_TYPE,sMixerParam); 
	if(m_pMixerChannel->GetMixerGroupId(nGroupId,sMixerParam)>0)
	{
		strucChannelSet sSet;	
		m_pMixerChannel->GetMixerControlState(nGroupId,sSet);
		bEnable = sSet.bChecked;
		return true;
	}
	return false;
}

//设置唱歌
bool CMixerManager::SetMixerControlSingMode()
{
	bool bSel = FALSE;
	if (IsMixerControlSelect(bSel))
	{
		if (bSel) //当前系统支持混音模式
		{
			//设置立体声混音选择
			if (!SetMixerControlMixerMute(TRUE))
			{
				return FALSE;
			}
			//设置波形 不静音
			if (!SetMixerControlWavoutMute(FALSE))
			{
				return FALSE;
			}
			//设置麦克风不静音
			if (!SetMixerControlOutmicMute(FALSE))
			{
				return FALSE;
			}

		}
		else
		{
			//设置立体声混音不静音
			if (!SetMixerControlMixerMute(FALSE))
			{
				return FALSE;
			}
			//设置波形 不静音
			if (!SetMixerControlWavoutMute(FALSE))
			{
				return FALSE;
			}
			//设置麦克风不静音
			if (!SetMixerControlOutmicMute(FALSE))
			{
				return FALSE;
			}
		}
		return true;
	}
	return false;
}

//获取是否处于唱歌模式
bool CMixerManager::GetMixerControlSingMode(bool& bEnable)
{
	bool bSel = FALSE;
	if (IsMixerControlSelect(bSel))
	{
		if (bSel) //支持混音模式
		{
			bool bEnabled = false;
			//获取立体声混音是否选择
			if (!GetMixerControlMixerMute(bEnabled))
			{
				return FALSE;
			}
			if(!bEnabled) //选择
			{
				bEnable = FALSE;
				return TRUE;
			}
			//设置波形 不静音
			if (!GetMixerControlWavoutMute(bEnabled))
			{
				return FALSE;
			}
			if(bEnabled)//不静音
			{
				bEnable = FALSE;
				return TRUE;
			}
			//设置麦克风不静音
			if (!GetMixerControlOutmicMute(bEnabled))
			{
				return FALSE;
			}
			if(bEnabled)//不静音
			{
				bEnable = FALSE;
				return TRUE;
			}
			bEnable = TRUE;

		}
		else //不支持混音模式
		{
			bool bEnabled = false;
			//获取立体声混音是否选择
			if (!GetMixerControlMixerMute(bEnabled))
			{
				return FALSE;
			}
			if(bEnabled) //不静音
			{
				bEnable = FALSE;
				return TRUE;
			}
			//设置波形 不静音
			if (!GetMixerControlWavoutMute(bEnabled))
			{
				return FALSE;
			}
			if(bEnabled)//不静音
			{
				bEnable = FALSE;
				return TRUE;
			}
			//设置麦克风不静音
			if (!GetMixerControlOutmicMute(bEnabled))
			{
				return FALSE;
			}
			if(bEnabled)//不静音
			{
				bEnable = FALSE;
				return TRUE;
			}
			bEnable = TRUE;
		}
		return TRUE;
	}
	return false;
}

//设置与人聊天
bool CMixerManager::SetMixerControlChatMode()
{
	bool bSel = FALSE;
	if (IsMixerControlSelect(bSel))
	{
		if (bSel) //当前系统支持混音模式
		{
			//设置立体声混音选择
			if (!SetMixerControlMixerMute(TRUE))
			{
				return FALSE;
			}
			//设置波形 静音
			if (!SetMixerControlWavoutMute(TRUE))
			{
				return FALSE;
			}
			//设置麦克风不静音
			if (!SetMixerControlOutmicMute(FALSE))
			{
				return FALSE;
			}

		}
		else
		{
			//设置立体声混音不静音
			if (!SetMixerControlMixerMute(FALSE))
			{
				return FALSE;
			}
			//设置波形 静音
			if (!SetMixerControlWavoutMute(TRUE))
			{
				return FALSE;
			}
			//设置麦克风不静音
			if (!SetMixerControlOutmicMute(FALSE))
			{
				return FALSE;
			}
		}
		return true;
	}
	return false;
}

//获取是否与人聊天模式
bool CMixerManager::GetMixerControlChatMode(bool& bEnable)
{
	bool bSel = FALSE;
	if (IsMixerControlSelect(bSel))
	{
		if (bSel) //支持混音模式
		{
			bool bEnabled = false;
			//获取立体声混音是否选择
			if (!GetMixerControlMixerMute(bEnabled))
			{
				return FALSE;
			}
			if(!bEnabled) //选择
			{
				bEnable = FALSE;
				return TRUE;
			}
			//设置波形 不静音
			if (!GetMixerControlWavoutMute(bEnabled))
			{
				return FALSE;
			}
			if(!bEnabled)//静音
			{
				bEnable = FALSE;
				return TRUE;
			}
			//设置麦克风不静音
			if (!GetMixerControlOutmicMute(bEnabled))
			{
				return FALSE;
			}
			if(bEnabled)//不静音
			{
				bEnable = FALSE;
				return TRUE;
			}
			bEnable = TRUE;

		}
		else //不支持混音模式
		{
			bool bEnabled = false;
			//获取立体声混音是否选择
			if (!GetMixerControlMixerMute(bEnabled))
			{
				return FALSE;
			}
			if(bEnabled) //不静音
			{
				bEnable = FALSE;
				return TRUE;
			}
			//设置波形 不静音
			if (!GetMixerControlWavoutMute(bEnabled))
			{
				return FALSE;
			}
			if(!bEnabled)//静音
			{
				bEnable = FALSE;
				return TRUE;
			}
			//设置麦克风不静音
			if (!GetMixerControlOutmicMute(bEnabled))
			{
				return FALSE;
			}
			if(bEnabled)//不静音
			{
				bEnable = FALSE;
				return TRUE;
			}
			bEnable = TRUE;
		}
		return TRUE;
	}
	return false;
}

//在麦上放歌
bool CMixerManager::SetMixerControlPlayMode()
{
	bool bSel = FALSE;
	if (IsMixerControlSelect(bSel))
	{
		if (bSel) //当前系统支持混音模式
		{
			//设置立体声混音选择
			if (!SetMixerControlMixerMute(TRUE))
			{
				return FALSE;
			}
			//设置波形 不静音
			if (!SetMixerControlWavoutMute(FALSE))
			{
				return FALSE;
			}
			//设置麦克风静音
			if (!SetMixerControlOutmicMute(TRUE))
			{
				return FALSE;
			}

		}
		else
		{
			//设置立体声混音不静音
			if (!SetMixerControlMixerMute(FALSE))
			{
				return FALSE;
			}
			//设置波形 不静音
			if (!SetMixerControlWavoutMute(FALSE))
			{
				return FALSE;
			}
			//设置麦克风不静音
			if (!SetMixerControlOutmicMute(TRUE))
			{
				return FALSE;
			}
		}
		return true;
	}
	return false;
}

//获取是否在麦上放歌模式
bool CMixerManager::GetMixerControlPlayMode(bool& bEnable)
{
	bool bSel = FALSE;
	if (IsMixerControlSelect(bSel))
	{
		if (bSel) //支持混音模式
		{
			bool bEnabled = false;
			//获取立体声混音是否选择
			if (!GetMixerControlMixerMute(bEnabled))
			{
				return FALSE;
			}
			if(!bEnabled) //选择
			{
				bEnable = FALSE;
				return TRUE;
			}
			//设置波形 不静音
			if (!GetMixerControlWavoutMute(bEnabled))
			{
				return FALSE;
			}
			if(bEnabled)//不静音
			{
				bEnable = FALSE;
				return TRUE;
			}
			//设置麦克风不静音
			if (!GetMixerControlOutmicMute(bEnabled))
			{
				return FALSE;
			}
			if(!bEnabled)//静音
			{
				bEnable = FALSE;
				return TRUE;
			}
			bEnable = TRUE;

		}
		else //不支持混音模式
		{
			bool bEnabled = false;
			//获取立体声混音是否选择
			if (!GetMixerControlMixerMute(bEnabled))
			{
				return FALSE;
			}
			if(bEnabled) //不静音
			{
				bEnable = FALSE;
				return TRUE;
			}
			//设置波形 不静音
			if (!GetMixerControlWavoutMute(bEnabled))
			{
				return FALSE;
			}
			if(bEnabled)//不静音
			{
				bEnable = FALSE;
				return TRUE;
			}
			//设置麦克风不静音
			if (!GetMixerControlOutmicMute(bEnabled))
			{
				return FALSE;
			}
			if(!bEnabled)//静音
			{
				bEnable = FALSE;
				return TRUE;
			}
			bEnable = TRUE;
		}
		return TRUE;
	}
	return false;
}

//获取当前所处的模式
bool CMixerManager::GetMixerControlMode(EnMixerControlMode& eMode)
{
	eMode = MIXER_CONTROL_OTHER;
	bool bEnable = FALSE;
	if (!GetMixerControlSingMode(bEnable))//判断是否唱歌模式
	{
		return false;
	}
	if (bEnable)
	{
		eMode = MIXER_CONTROL_SING;
		return true;
	}
	if (!GetMixerControlChatMode(bEnable))//判断是否聊天模式
	{
		return false;
	}
	if (bEnable)
	{
		eMode = MIXER_CONTROL_CHAT;
		return true;
	}
	if (!GetMixerControlPlayMode(bEnable))//判断是否放歌模式
	{
		return false;
	}
	if (bEnable)
	{
		eMode = MIXER_CONTROL_PLAY;
		return true;
	}
	return true;
}

//获取操作系统类型
EnOsType CMixerManager::GetOsType()
{
	OSVERSIONINFO osInfo;
	memset(&osInfo, 0, sizeof(OSVERSIONINFO));
	osInfo.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
	EnOsType eType = OsWindowsUnKnown;
	//获取版本类型
	if(::GetVersionEx(&osInfo))
	{
		Trace("CMixerManager::GetOsType  dwMajorVersion= %d, dwMinorVersion = %d, dwBuildNumber = %d \n", osInfo.dwMajorVersion, osInfo.dwMinorVersion, osInfo.dwBuildNumber);

		if(osInfo.dwMajorVersion <= 5)//XP or early
		{
			eType = OsWindowsXP;
		}
		else
		{
			if(osInfo.dwMinorVersion == 0)
			{
				eType = OsWindowsVista;
			}
			else
			{
				eType = OsWindows7;
			}
		}

	}
	return eType;
}

bool CMixerManager::GetOsVistaOrWin7()
{
	switch(GetOsType())
	{
	case OsWindowsVista:
	case OsWindows7:
		{
			return true;
		}
		break;
	default:
		return false;
	}
}