#include "stdafx.h"
#include "VolController.h"
#include "MyAssert.h"
#include <stdlib.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#define ERR_INIT_MIXER             "获取混音器信息失败，请确认是否安装混音器设备..." 
#define ERR_NOT_INIT_MIXER         "还没有初始化混音器设备信息或获取失败，请先调用InitializeMixer()方法"
#define ERR_GET_CONTROL_DETAILS    "mixerGetControlDetails()函数出错"
#define ERR_EN_MUTE       "设置静音失败！"
#define ERR_DS_MUTE       "解除静音失败！"

BOOL   CVolController::m_mixerInited;        
HMIXER CVolController::m_hmx;                
DWORD  CVolController::m_dwLineID;           
UINT   CVolController::m_nChannelCount;      
DWORD  CVolController::m_dwVolumeControlID;    
DWORD  CVolController::m_dwMuteControlID;    
LONG   CVolController::m_lMinVlm;            
LONG   CVolController::m_lMaxVlm;               


CVolController::CVolController()
{
	m_mixerInited = FALSE;

	if(!InitializeMixer())
	{
		Trace(ERR_INIT_MIXER);
	}
}

CVolController::~CVolController()
{
}

// 对混音器进行操作前,进行与混音器设置有关的所有变量的初始化
BOOL CVolController::InitializeMixer()
{
	// 如果有，打开第一个混音器
	if (::mixerGetNumDevs() != 0)
	{
		// 打开mixer. 单声卡的deviceID为0
		if (mixerOpen(&m_hmx,
						0,
						NULL,
						NULL,
						MIXER_OBJECTF_MIXER | CALLBACK_WINDOW)
			!= MMSYSERR_NOERROR)
		{
			return FALSE;
		}
	}

	// Holds the mixer line data.
	MIXERLINE mxl;    
	
    ZeroMemory(&mxl,sizeof(mxl));
    mxl.cbStruct = sizeof(mxl);
	// 指出需要获取的通道，扬声器用MIXERLINE_COMPONENTTYPE_DST_SPEAKERS
    mxl.dwComponentType = MIXERLINE_COMPONENTTYPE_DST_SPEAKERS;

	if(mixerGetLineInfo((HMIXEROBJ)m_hmx, &mxl,
         MIXER_GETLINEINFOF_COMPONENTTYPE) != MMSYSERR_NOERROR)
	{
		return FALSE;
	}

	// 保存混音器MIXERLINE_COMPONENTTYPE_DST_SPEAKERS
	// 对应线路(MIXERLINE)的LineID和通道数量Channels
	m_dwLineID = mxl.dwLineID;
	m_nChannelCount = mxl.cChannels;

	// 初始化m_dwVolumeControlID，m_volMin，m_volMax
	// 和m_dwMuteControlID值用到的两个对象
	MIXERCONTROL mxc;            // Holds the mixer control data
    MIXERLINECONTROLS mxlc;      // Obtains the mixer control.

	// 初始化m_dwVolumeControlID，m_lMinVlm，m_lMaxVlm
    ZeroMemory(&mxc, sizeof(mxc));
    mxc.cbStruct = sizeof(mxc);
    ZeroMemory(&mxlc, sizeof(mxlc));
    mxlc.cbStruct = sizeof(mxlc);
    mxlc.dwLineID = m_dwLineID;
    mxlc.dwControlType = MIXERCONTROL_CONTROLTYPE_VOLUME;
    mxlc.cControls = 1;
    mxlc.cbmxctrl = sizeof(mxc);
    mxlc.pamxctrl = &mxc;
	if(mixerGetLineControls((HMIXEROBJ)m_hmx,&mxlc,
        MIXER_GETLINECONTROLSF_ONEBYTYPE) != MMSYSERR_NOERROR)
	{
		return FALSE;
	}
	m_dwVolumeControlID = mxc.dwControlID;
	m_lMinVlm = mxc.Bounds.lMinimum;
	m_lMaxVlm = mxc.Bounds.lMaximum;


	//<----------------------------->
	// 初始化m_dwMuteControlID
    ZeroMemory(&mxc, sizeof(mxc));
    mxc.cbStruct = sizeof(mxc);
    ZeroMemory(&mxlc, sizeof(mxlc));
    mxlc.cbStruct = sizeof(mxlc);
    mxlc.dwLineID = m_dwLineID;
    mxlc.dwControlType = MIXERCONTROL_CONTROLTYPE_MUTE;
    mxlc.cControls = 1;
    mxlc.cbmxctrl = sizeof(mxc);
    mxlc.pamxctrl = &mxc;
	if(mixerGetLineControls((HMIXEROBJ)m_hmx,&mxlc,
        MIXER_GETLINECONTROLSF_ONEBYTYPE) != MMSYSERR_NOERROR)
	{
		return FALSE;
	}
	m_dwMuteControlID = mxc.dwControlID;
	
	if(!m_mixerInited)
	{
		m_mixerInited = TRUE;
	}

	return TRUE;
}

// 获取混音器的最小值
LONG CVolController::GetVolumeRangMin()
{
	if(!m_mixerInited)
	{
		Trace(ERR_NOT_INIT_MIXER);
		return -1;
	}

	return m_lMinVlm;
}

// 获取混音器的最大值
LONG CVolController::GetVolumeRangMax()
{
	if(!m_mixerInited)
	{
		Trace(ERR_NOT_INIT_MIXER);
		return -1;
	}

	return m_lMaxVlm;
}

// 获取当前混音器的音量值
LONG CVolController::GetVolumeValue(MixerChannel destChannel)
{
	if(!m_mixerInited)
	{
		Trace(ERR_NOT_INIT_MIXER);
		return -1;
	}

	MIXERCONTROLDETAILS mxcd;  // Gets the control values.	
	MIXERCONTROLDETAILS_SIGNED volStruct; // Gets the control values.
	
	ZeroMemory(&mxcd, sizeof(mxcd));
	mxcd.cbStruct = sizeof(mxcd);
	mxcd.dwControlID = m_dwVolumeControlID;
	mxcd.cbDetails = sizeof(MIXERCONTROLDETAILS_SIGNED);
	mxcd.paDetails = &volStruct;
	mxcd.cChannels = 1;

	if(mixerGetControlDetails((HMIXEROBJ)m_hmx, &mxcd,
                               MIXER_GETCONTROLDETAILSF_VALUE) 
		== MMSYSERR_NOERROR)
	{
		// 获得当前音量值		
		return volStruct.lValue;
	}
	else
	{
		Trace(ERR_GET_CONTROL_DETAILS);
		return -1;
	}
}

// 把系统音量值设置为指定的值
BOOL CVolController::SetVolumeValue(LONG lVlmVal)
{
	if(m_lMaxVlm < lVlmVal || lVlmVal < m_lMinVlm)
	{
		return FALSE;
	}

	// 为了避免主音量设置改变已经存在的声道状态，在设置前保存两个声道信息
	BOOL leftChannelMute = CVolController::DetectSilence(LeftChannel);
	BOOL rightChannelMete = CVolController::DetectSilence(RightChannel);

	MIXERCONTROLDETAILS mxcd;  // Gets the control values.	
	MIXERCONTROLDETAILS_UNSIGNED twoChannels[2];
	
	ZeroMemory(&mxcd, sizeof(MIXERCONTROLDETAILS));
	mxcd.cbStruct = sizeof(MIXERCONTROLDETAILS);
	mxcd.dwControlID = m_dwVolumeControlID;
	mxcd.cChannels = 2;
	mxcd.cMultipleItems = 0;
	mxcd.paDetails = &twoChannels[0];
	mxcd.cbDetails = sizeof(MIXERCONTROLDETAILS_UNSIGNED);
	
	twoChannels[0].dwValue = lVlmVal;  // 保存为要设置的值
	twoChannels[1].dwValue = lVlmVal;
	
	if(leftChannelMute)
	{
		twoChannels[0].dwValue = 0;
	}
	if(rightChannelMete)
	{
		twoChannels[1].dwValue = 0;
	}
	if(leftChannelMute && rightChannelMete)
	{
		// 当两个声道都静音时，需要仍能调节音量
		twoChannels[0].dwValue = lVlmVal;
		twoChannels[1].dwValue = lVlmVal;
	}
	
	if(mixerSetControlDetails(
		reinterpret_cast<HMIXEROBJ>(m_hmx),
		&mxcd,
		MIXER_OBJECTF_HMIXER | MIXER_SETCONTROLDETAILSF_VALUE)
		!= MMSYSERR_NOERROR)
	{
		return FALSE;
	}	

	return TRUE;
}

// 设置声道静音
BOOL CVolController::SetChannelMute(MixerChannel destChannel, bool bIsMute)
{
	MIXERCONTROLDETAILS mxcd;  // Gets the control values.	
	MIXERCONTROLDETAILS_UNSIGNED twoChannels[2] = {0};
	
	ZeroMemory(&mxcd, sizeof(MIXERCONTROLDETAILS));
	mxcd.cbStruct = sizeof(MIXERCONTROLDETAILS);
	mxcd.dwControlID = m_dwVolumeControlID;
	mxcd.cChannels = 2;
	mxcd.cMultipleItems = 0;
	mxcd.paDetails = &twoChannels[0];
	mxcd.cbDetails = sizeof(MIXERCONTROLDETAILS_UNSIGNED);
	
	LONG channelMaxVlm = CVolController::GetVolumeValue(MainChannel);

	if(bIsMute)
	{
		switch(destChannel)
		{
		case LeftChannel:
			twoChannels[0].dwValue = 0;
			twoChannels[1].dwValue = CVolController::DetectSilence(RightChannel) ? 0 : channelMaxVlm;
			break;
		case RightChannel:
			twoChannels[0].dwValue = CVolController::DetectSilence(LeftChannel) ? 0 : channelMaxVlm;
			twoChannels[1].dwValue = 0;
			break;		
		}
	}
	else
	{
		switch(destChannel)
		{
		case LeftChannel:
			twoChannels[0].dwValue = channelMaxVlm;
			twoChannels[1].dwValue = CVolController::DetectSilence(RightChannel) ? 0 : channelMaxVlm;
			break;
		case RightChannel:
			twoChannels[0].dwValue = CVolController::DetectSilence(LeftChannel) ? 0 : channelMaxVlm;
			twoChannels[1].dwValue = channelMaxVlm;
			break;
		}
	}
	
	MMRESULT setResult = 
		mixerSetControlDetails(
		reinterpret_cast<HMIXEROBJ>(m_hmx),
		&mxcd,
		MIXER_OBJECTF_HMIXER | MIXER_SETCONTROLDETAILSF_VALUE);
	
	return setResult == MMSYSERR_NOERROR;
}

BOOL CVolController::GetChannelMute(MixerChannel destChannel, bool& bIsMute)
{
	MIXERCONTROLDETAILS mxcd;  // Gets the control values.	
	MIXERCONTROLDETAILS_UNSIGNED twoChannels[2] = {0};

	ZeroMemory(&mxcd, sizeof(MIXERCONTROLDETAILS));
	mxcd.cbStruct = sizeof(MIXERCONTROLDETAILS);
	mxcd.dwControlID = m_dwVolumeControlID;
	mxcd.cChannels = 2;
	mxcd.cMultipleItems = 0;
	mxcd.paDetails = &twoChannels[0];
	mxcd.cbDetails = sizeof(MIXERCONTROLDETAILS_UNSIGNED);


	MMRESULT setResult = 
		mixerGetControlDetails(
		reinterpret_cast<HMIXEROBJ>(m_hmx),
		&mxcd,
		MIXER_OBJECTF_HMIXER | MIXER_SETCONTROLDETAILSF_VALUE);
	

	switch(destChannel)
	{
	case LeftChannel:
		bIsMute = twoChannels[0].dwValue;
		break;
	case RightChannel:
		bIsMute = twoChannels[1].dwValue;
		break;
	case MainChannel:
		bIsMute = twoChannels[0].dwValue || twoChannels[1].dwValue;
		break;
	default:
		return false;
	}

	return setResult == MMSYSERR_NOERROR;
}

// 对混音器进行可用性设置
// bEnable为TRUE时，将混音器从不可用状态设置为可用状态(静音到非静音)，反之
void CVolController::EnableLine(BOOL bEnable)
{            
	MMRESULT mmResult;
	MIXERLINE lineDestination;
	memset(&lineDestination, 0, sizeof(MIXERLINE));
	lineDestination.cbStruct = sizeof(MIXERLINE);
	lineDestination.dwLineID = m_dwLineID; 
    mixerGetLineInfo((HMIXEROBJ)m_hmx, &lineDestination, MIXER_GETLINEINFOF_LINEID);

	int nControlCount = lineDestination.cControls;
	int nChannelCount = lineDestination.cChannels;

	MIXERLINECONTROLS LineControls;
	memset( &LineControls, 0, sizeof(MIXERLINECONTROLS) );
	MIXERCONTROL* aControls = (MIXERCONTROL*)malloc( nControlCount * sizeof(MIXERCONTROL) );
	
	memset( &aControls[0], 0, sizeof(nControlCount * sizeof(MIXERCONTROL)) );

	for ( int i = 0; i < nControlCount; i++ )    
	{
		aControls[i].cbStruct = sizeof(MIXERCONTROL);
	}

	LineControls.cbStruct = sizeof(MIXERLINECONTROLS); 
	LineControls.dwLineID = lineDestination.dwLineID;
	LineControls.cControls = nControlCount; 
	LineControls.cbmxctrl = sizeof(MIXERCONTROL);
	LineControls.pamxctrl = &aControls[0];
	mmResult = mixerGetLineControls( (HMIXEROBJ)m_hmx, &LineControls, MIXER_GETLINECONTROLSF_ALL );

	if ( mmResult == MMSYSERR_NOERROR )
	{
		for (int i = 0; i < nControlCount; i++ )
		{
			LONG lValue;
			BOOL bReadyToSet = FALSE;
			switch (aControls[i].dwControlType)
			{
			case MIXERCONTROL_CONTROLTYPE_MUTE:
				lValue = (BOOL)!bEnable;
				bReadyToSet = TRUE;
				break;
			case MIXERCONTROL_CONTROLTYPE_SINGLESELECT:
				lValue = (BOOL)bEnable;
				bReadyToSet = TRUE;
				break;
			case MIXERCONTROL_CONTROLTYPE_MUX:
				lValue = (BOOL)bEnable;
				bReadyToSet = TRUE;
				break;
			case MIXERCONTROL_CONTROLTYPE_MULTIPLESELECT:
				lValue = (BOOL)bEnable;
				bReadyToSet = TRUE;
				break;
			case MIXERCONTROL_CONTROLTYPE_MIXER:
				lValue = (BOOL)bEnable;
				bReadyToSet = TRUE;
				break;
			}
			if ( bReadyToSet )
			{
				MIXERCONTROLDETAILS_BOOLEAN* aDetails = NULL;
				int nMultipleItems = aControls[i].cMultipleItems;
				int nChannels = nChannelCount;
				
				MIXERCONTROLDETAILS ControlDetails;             
				memset( &ControlDetails, 0, sizeof(MIXERCONTROLDETAILS) );
				ControlDetails.cbStruct = sizeof(MIXERCONTROLDETAILS);
				ControlDetails.dwControlID = aControls[i].dwControlID;
				if ( aControls[i].fdwControl & MIXERCONTROL_CONTROLF_UNIFORM )
				{
					nChannels = 1;
				}
				if ( aControls[i].fdwControl & MIXERCONTROL_CONTROLF_MULTIPLE )
				{
					nMultipleItems = aControls[i].cMultipleItems;
					aDetails = (MIXERCONTROLDETAILS_BOOLEAN*)malloc(nMultipleItems*nChannels*sizeof(MIXERCONTROLDETAILS_BOOLEAN));
					if ( !aDetails )
					{
						continue;
					}
					for ( int nItem = 0; nItem < nMultipleItems; nItem++ )
					{
						for ( int nChannel = 0; nChannel < nChannels; nChannel++ )
						{
							aDetails[nItem+nChannel].fValue = lValue;
						}
					}
				} 
				else 
				{
					nMultipleItems = 0;
					aDetails = (MIXERCONTROLDETAILS_BOOLEAN*)malloc(nChannels*sizeof(MIXERCONTROLDETAILS_BOOLEAN));
					if ( !aDetails )
					{
						continue;
					}
					for ( int nChannel = 0; nChannel < nChannels; nChannel++ )
					{
						aDetails[nChannel].fValue = (LONG)lValue;
					}
				}
				ControlDetails.cChannels = nChannels;
				ControlDetails.cMultipleItems = nMultipleItems;
				ControlDetails.cbDetails = sizeof(MIXERCONTROLDETAILS_BOOLEAN);
				ControlDetails.paDetails = &aDetails[0];
				mmResult = mixerSetControlDetails((HMIXEROBJ)m_hmx, &ControlDetails, 0L);
				
				// 设置失败
				if (mmResult != MMSYSERR_NOERROR)
				{
					bEnable ? Trace(ERR_DS_MUTE) : Trace(ERR_EN_MUTE);
				}

				free( aDetails );
			}
		}
	}
	else
	{
		// 获取失败
		Trace(ERR_GET_CONTROL_DETAILS);
	}
	
	free( aControls );
}

// 静音时返回TRUE，反之
BOOL CVolController::DetectSilence(MixerChannel destChannel)
{
	MIXERCONTROLDETAILS mxcd;
	MIXERCONTROLDETAILS_BOOLEAN muteDetails;
	MIXERCONTROLDETAILS_UNSIGNED twoChannnels[2]; // 两个声道的值

	// Gets the control values.
	// 初始化MIXERCONTROLDETAILS结构体
    ZeroMemory(&mxcd, sizeof(MIXERCONTROLDETAILS));
    mxcd.cbStruct = sizeof(MIXERCONTROLDETAILS);
    mxcd.dwControlID = m_dwMuteControlID;
	mxcd.cbDetails = sizeof(MIXERCONTROLDETAILS_BOOLEAN);
    mxcd.paDetails = &muteDetails;
    mxcd.cChannels = 1;

	if(	mixerGetControlDetails((HMIXEROBJ)m_hmx, 
		                       &mxcd, 
							   MIXER_GETCONTROLDETAILSF_VALUE )
		!= MMSYSERR_NOERROR)
	{
		Trace(ERR_GET_CONTROL_DETAILS);
		return FALSE;
	}

	// 获取两个声道的信息
    ZeroMemory(&mxcd, sizeof(MIXERCONTROLDETAILS));
    mxcd.cbStruct = sizeof(MIXERCONTROLDETAILS);
    mxcd.dwControlID = m_dwVolumeControlID;
    mxcd.cChannels = 2;
	mxcd.cMultipleItems = 0;
    mxcd.paDetails = &twoChannnels[0];
	mxcd.cbDetails = sizeof(MIXERCONTROLDETAILS_UNSIGNED);
	if(	mixerGetControlDetails((HMIXEROBJ)m_hmx, 
		                       &mxcd, 
							   MIXER_GETCONTROLDETAILSF_VALUE)
		!= MMSYSERR_NOERROR)
	{
		Trace(ERR_GET_CONTROL_DETAILS);
		return FALSE;
	}

	switch(destChannel)
	{
	case MainChannel:
		return (BOOL) muteDetails.fValue;
	case LeftChannel:
		return twoChannnels[0].dwValue == 0;
	case RightChannel:
		return twoChannnels[1].dwValue == 0;
	}

	return (BOOL) muteDetails.fValue;
}
