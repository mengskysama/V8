#include "StdAfx.h"
#include "PersonalConfig.h"
#include "..\Room.h"
#include "..\..\CommonLibrary/include/utility/ConfigManager.h"
#include "..\..\CommonLibrary\include\utility\StringHelper.h"

CPersonalConfig* CPersonalConfig::s_pPersonalConfig = NULL;

CPersonalConfig::CPersonalConfig(void)
:m_unVideoIndex(0),
m_unRoomSpeakerVolume(255),	//默认音量大小
m_unRoomMicVolume(255)		//默认麦克风大小
{
	m_bUseVirtualCam = true;
	m_enmP2PPermission = P2P_PERMISSION_ABOVE_VIP ;
	m_bAutoScrollSpeaker = m_bPublicAutoScroll = m_bPrivateAutoScroll = true;
}

CPersonalConfig::~CPersonalConfig(void)
{
}

void CPersonalConfig::Init( uint32 unCustomSetting )
{
	m_unCustomSetting =	unCustomSetting;
	m_unPreCustomSetting = unCustomSetting;
	if (m_unCustomSetting == 0)
	{
		SetToDefault();
	}

	m_bBlockPublicMsg = ((unCustomSetting & core::SELFSETTING_MESSAGE_NO_PUBLIC) != 0);
	m_bBlockPrivateMsg = ((unCustomSetting & core::SELFSETTING_MESSAGE_NO_PRIVATE) != 0);
	m_bAutoReply = ((unCustomSetting & core::SELFSETTING_MESSAGE_AUTO_REPLY) != 0);
	m_bBlockSpeakerSound = ((unCustomSetting & core::SELFSETTING_MESSAGE_NO_LOUDSPEAKER_AUDIO) != 0);
	m_bBlockGiftMsg = ((unCustomSetting & core::SELFSETTING_MESSAGE_NO_GIFT) != 0);
	m_bBlockUserInOutMsg = ((unCustomSetting & core::SELFSETTING_MESSAGE_NO_ENTER_LEAVE) == 0);
	m_bAutoScrollSpeaker = ((unCustomSetting & core::SELFSETTING_MESSAGE_LOUDSPEAKER_SCROLL) == 0);
	m_bBlockLuckySound = ((unCustomSetting & core::SELFSETTING_MESSAGE_NO_LUCKY_AUDIO) != 0);

	switch (unCustomSetting & core::SELFSETTING_P2PSHOW_MASK)
	{
	case core::SELFSETTING_P2PSHOW_VIP:
		{
			m_enmP2PPermission = P2P_PERMISSION_ABOVE_VIP;
		}
		break;
	case core::SELFSETTING_P2PSHOW_KING:
		{
			m_enmP2PPermission = P2P_PERMISSION_ABOVE_CROWN;
		}
		break;
	case core::SELFSETTING_P2PSHOW_CLOSE:
		{
			m_enmP2PPermission = P2P_PERMISSION_REFUSE_ANYONE;
		}
		break;
	default:
		break;
	}

	UserInfo_PersonalSetting stSetting;
	CConfigManager::Instance()->GetUserInfo_PersonalSetting(GetCurInfo().unUIN, stSetting);

	m_enmOnMicPurpose = (EnMixerControlMode)stSetting.unOnMicPurpose;
	m_unGeneralVolume = stSetting.unGeneralVolume;
	m_unAccompanyVolume = stSetting.unAccompanyVolume;
	m_unMicVolume = stSetting.unMicVolume;
	m_bStrengthenMic = stSetting.bStrengthenMic;
	m_unRecordMixVolume = stSetting.unRecordMixVolume;
	m_unRecordMicVolume = stSetting.unRecordMicVolume;
	m_strUseVideoDevice = stSetting.strUseVideoDevice;
	m_strUseAudioDevice = stSetting.strUseAudioDevice;
	m_bAutoVirtualVideo = stSetting.bAutoVirtualVideo;
	m_unRoomSpeakerVolume = stSetting.unRoomSpeakerVolume;
	m_unRoomMicVolume = stSetting.unRoomMicVolume;
	
	m_TempMessageFont =stSetting.TempMessageFont ;
	m_ChatMessageFont = stSetting.ChatMessageFont;

}

void CPersonalConfig::SaveToSever()
{
	uint32 unTemp = 0x0000;
	if (m_bBlockPublicMsg)
	{
		unTemp |= core::SELFSETTING_MESSAGE_NO_PUBLIC;
	}
	if (m_bBlockPrivateMsg)
	{
		unTemp |= core::SELFSETTING_MESSAGE_NO_PRIVATE;
	}
	if (m_bAutoReply)
	{
		unTemp |= core::SELFSETTING_MESSAGE_AUTO_REPLY;
	}
	if (m_bBlockSpeakerSound)
	{
		unTemp |= core::SELFSETTING_MESSAGE_NO_LOUDSPEAKER_AUDIO;
	}
	if (m_bBlockGiftMsg)
	{
		unTemp |= core::SELFSETTING_MESSAGE_NO_GIFT;
	}
	if (!m_bBlockUserInOutMsg)
	{
		unTemp |= core::SELFSETTING_MESSAGE_NO_ENTER_LEAVE;
	}
	if (!m_bAutoScrollSpeaker)
	{
		unTemp |= core::SELFSETTING_MESSAGE_LOUDSPEAKER_SCROLL;
	}
	if (m_bBlockLuckySound)
	{
		unTemp |= core::SELFSETTING_MESSAGE_NO_LUCKY_AUDIO;
	}

	switch (m_enmP2PPermission)
	{
	case P2P_PERMISSION_ABOVE_VIP:
		{
			unTemp |= core::SELFSETTING_P2PSHOW_VIP;
		}
		break;
	case P2P_PERMISSION_ABOVE_CROWN:
		{
			unTemp |= core::SELFSETTING_P2PSHOW_KING;
		}
		break;
	case P2P_PERMISSION_REFUSE_ANYONE:
		{
			unTemp |= core::SELFSETTING_P2PSHOW_CLOSE;
		}
		break;
	default:
		break;
	}

	if (unTemp != m_unCustomSetting)
	{
		core::SetSelfInfoEvent * pEvent = new core::SetSelfInfoEvent();
		pEvent->uint32Map[core::INFOTYPE_SELF_SETTING] = unTemp;
		m_unPreCustomSetting = unTemp;

		g_RoomModule->m_pModuleManager->PushEvent(MakeEvent<MODULE_ID_ROOM>()(core::EVENT_VALUE_CORE_SET_SELFINFO, MODULE_ID_CORE, 0, 0, 0, pEvent));
	}
}

void CPersonalConfig::SetToDefault()
{
	m_enmOnMicPurpose = MIXER_CONTROL_SING;
	m_bAutoVirtualVideo = true;
	m_bBlockPublicMsg = false;
	m_bBlockPrivateMsg = false;
	m_bAutoScrollSpeaker = m_bPublicAutoScroll = m_bPrivateAutoScroll = true;
	m_bAutoReply = false;
	m_bBlockSpeakerSound = false;
	m_bBlockGiftMsg = false;
	m_bBlockUserInOutMsg = true;
	m_bBlockLuckySound = false;
	m_enmP2PPermission = P2P_PERMISSION_ABOVE_VIP;

	m_unCustomSetting = 0x0000;
	m_unPreCustomSetting = 0x0000;
}

CPersonalConfig* CPersonalConfig::GetInstance()
{
	if (s_pPersonalConfig == NULL)
	{
		s_pPersonalConfig = new CPersonalConfig();
	}

	return s_pPersonalConfig;
}

void CPersonalConfig::Save()
{
	SaveToSever();
	SaveToLocal();
}

void CPersonalConfig::SetTempMessageFont( core::MessageFont& messagefont )
{ 
	m_TempMessageFont = messagefont ; 
	SaveToLocal();
}

void CPersonalConfig::SetChatMessageFont(core::MessageFont& messageFont)
{
	m_ChatMessageFont = messageFont;
	SaveToLocal();
}

void CPersonalConfig::SaveToLocal()
{
	UserInfo_PersonalSetting stSetting;

	stSetting.unOnMicPurpose = m_enmOnMicPurpose;
	stSetting.unGeneralVolume = m_unGeneralVolume;
	stSetting.unAccompanyVolume = m_unAccompanyVolume;
	stSetting.unMicVolume = m_unMicVolume;
	stSetting.bStrengthenMic = m_bStrengthenMic;
	stSetting.unRecordMixVolume = m_unRecordMixVolume;
	stSetting.unRecordMicVolume = m_unRecordMicVolume;
	stSetting.strUseVideoDevice.Format(_T("%s"),m_strUseVideoDevice.c_str()) ;
	stSetting.strUseAudioDevice.Format(_T("%s"),m_strUseAudioDevice.c_str()) ;
	stSetting.bAutoVirtualVideo = m_bAutoVirtualVideo;
	stSetting.unRoomSpeakerVolume = m_unRoomSpeakerVolume;
	stSetting.unRoomMicVolume = m_unRoomMicVolume;
	stSetting.TempMessageFont = m_TempMessageFont ;
	stSetting.ChatMessageFont = m_ChatMessageFont;
	CConfigManager::Instance()->SetUserInfo_PersonalSetting(GetCurInfo().unUIN, stSetting);
}

void CPersonalConfig::OnSetSelfInfoOK()
{
	m_unCustomSetting = m_unPreCustomSetting;
}

void CPersonalConfig::DeleteInstance()
{
	if (s_pPersonalConfig)
	{
		delete s_pPersonalConfig;
		s_pPersonalConfig = NULL;
	}
}