#pragma once
#include "coredefine.h"
#include "AvDefine.h"


enum ENM_P2PPermission
{
	P2P_PERMISSION_REFUSE_ANYONE = 0,
	P2P_PERMISSION_ABOVE_VIP = 1,
	P2P_PERMISSION_ABOVE_CROWN = 2,
};

class CPersonalConfig
{
public:
	CPersonalConfig(void);
	~CPersonalConfig(void);

	static CPersonalConfig* GetInstance();
	static void DeleteInstance();

	void Init(uint32 unCustomSetting);
	void SetToDefault();
	void Save();

	EnMixerControlMode GetOnMicPurpose() { return m_enmOnMicPurpose; }
	uint32 GetGeneralVolume() { return m_unGeneralVolume; }
	uint32 GetAccompanyVolume() { return m_unAccompanyVolume; }
	uint32 GetMicVolume() { return m_unMicVolume; }
	bool GetIsStrengthenMic() { return m_bStrengthenMic; }
	uint32 GetRecordMixVolume() { return m_unRecordMixVolume; }
	uint32 GetRecordMicVolume() { return m_unRecordMicVolume; }
	std::wstring GetUseVideoDevice() { return m_strUseVideoDevice; }
	uint32 GetUseVideoDeviceIndex() { return m_unVideoIndex; }
	std::wstring GetUseAudioDevice() { return m_strUseAudioDevice; }
	bool GetIsAutoVirtualVideo() { return m_bAutoVirtualVideo; }
	bool GetIsBlockPublicMsg() { return m_bBlockPublicMsg; }
	bool GetIsBlockPrivateMsg() { return m_bBlockPrivateMsg; }
	bool GetPrivateAutoScroll( ) { return m_bPrivateAutoScroll; }
	bool GetPublicAutoScroll( ) { return m_bPublicAutoScroll; }
	bool GetIsBlockLuckySound() { return m_bBlockLuckySound; }

	bool GetIsAutoReply() { return m_bAutoReply; }
	std::wstring GetAutoReplyWords() { return m_strAutoReply; }
	bool GetIsBlockSpeakerSound() { return m_bBlockSpeakerSound; }
	bool GetIsBlockGiftMsg() { return m_bBlockGiftMsg; }
	bool GetIsAutoScrollSpeaker() { return m_bAutoScrollSpeaker; }
	bool GetIsBlockUserInOutMsg() { return m_bBlockUserInOutMsg; }
	ENM_P2PPermission GetP2PPermission() { return m_enmP2PPermission; }
	uint32 GetRoomSpeakerVolume() { return m_unRoomSpeakerVolume; }
	uint32 GetRoomMicVolume() { return m_unRoomMicVolume; }
	bool GetIsUseVirtualCam() { return m_bUseVirtualCam; }

	void SetOnMicPurpose(EnMixerControlMode enmOnMicPurpose) { m_enmOnMicPurpose = enmOnMicPurpose; }
	void SetGeneralVolume(uint32 unGeneralVolume) { m_unGeneralVolume = unGeneralVolume; }
	void SetAccompanyVolume(uint32 unAccompanyVolume) { m_unAccompanyVolume = unAccompanyVolume; }
	void SetMicVolume(uint32 unMicVolume) { m_unMicVolume = unMicVolume; }
	void SetIsStrengthenMic(bool bStrengthenMic) { m_bStrengthenMic = bStrengthenMic; }
	void SetRecordMixVolume(uint32 unRecordMixVolume) { m_unRecordMixVolume = unRecordMixVolume; }
	void SetRecordMicVolume(uint32 unRecordMicVolume) { m_unRecordMicVolume = unRecordMicVolume; }
	void SetUseVideoDevice(std::wstring strUseVideoDevice) { m_strUseVideoDevice = strUseVideoDevice; }
	void SetUseVideoDeviceIndex(uint32 unIndex) { m_unVideoIndex = unIndex; }
	void SetUseAudioDevice(std::wstring strUseAudioDevice) { m_strUseAudioDevice = strUseAudioDevice; }
	void SetIsAutoVirtualVideo(bool bAutoVirtualVideo) { m_bAutoVirtualVideo = bAutoVirtualVideo; }
	void SetIsBlockPublicMsg(bool bBlockPublicMsg) { m_bBlockPublicMsg = bBlockPublicMsg; }
	void SetIsBlockPrivateMsg(bool bBlockPrivateMsg) { m_bBlockPrivateMsg = bBlockPrivateMsg; }
	void SetPrivateAutoScroll( bool bPrivateAutoScroll ) { m_bPrivateAutoScroll = bPrivateAutoScroll; }
	void SetPublicAutoScroll( bool bPublicAutoScroll ) { m_bPublicAutoScroll = bPublicAutoScroll; }
	void SetIsBlockLuckySound(bool bBlockLuckySound) { m_bBlockLuckySound = bBlockLuckySound; }
	void SetIsAutoReply(bool bAutoReply) { m_bAutoReply = bAutoReply; }
	void SetAutoReplyWords(std::wstring strAutoReply) { m_strAutoReply = strAutoReply; }
	void SetIsBlockSpeakerSound(bool bBlockSpeakerSound) { m_bBlockSpeakerSound = bBlockSpeakerSound; }
	void SetIsBlockGiftMsg(bool bBlockGiftMsg) { m_bBlockGiftMsg = bBlockGiftMsg; }
	void SetIsAutoScrollSpeaker(bool bAutoScrollSpeaker) { m_bAutoScrollSpeaker = bAutoScrollSpeaker; }
	void SetIsBlockUserInOutMsg(bool bBlockUserInOutMsg) { m_bBlockUserInOutMsg = bBlockUserInOutMsg; }
	void SetP2PPermission(ENM_P2PPermission enmP2PPermission) { m_enmP2PPermission = enmP2PPermission; }
	void SetRoomSpeakerVolume(uint32 unSpeakerVolume) { m_unRoomSpeakerVolume = unSpeakerVolume; }
	void SetRoomMicVolume(uint32 unMicVolume) { m_unRoomMicVolume = unMicVolume; }
	void SetIsUseVirtualCam(bool bUseVirtual) { m_bUseVirtualCam = bUseVirtual; }

	void OnSetSelfInfoOK();

	core::MessageFont GetTempMessageFont(){ return m_TempMessageFont ;} 
	void SetTempMessageFont( core::MessageFont& messagefont ) ;
	core::MessageFont GetChatMessageFont(){ return m_ChatMessageFont; }
	void SetChatMessageFont( core::MessageFont& messageFont);
public:
	void SaveToSever();
	void SaveToLocal();

protected:
	uint32 m_unCustomSetting;
	uint32 m_unPreCustomSetting;
	//音频设置
	EnMixerControlMode m_enmOnMicPurpose;
	uint32 m_unGeneralVolume;
	uint32 m_unAccompanyVolume;
	uint32 m_unMicVolume;
	bool m_bStrengthenMic;
	uint32 m_unRecordMixVolume;
	uint32 m_unRecordMicVolume;

	//视频设置
	std::wstring m_strUseVideoDevice;
	std::wstring m_strUseAudioDevice;
	bool m_bAutoVirtualVideo;
	uint32 m_unVideoIndex;
	bool m_bUseVirtualCam;

	//消息设置
	bool m_bBlockPublicMsg;
	bool m_bBlockPrivateMsg;
	bool m_bAutoScroll;
	bool m_bPrivateAutoScroll ;
	bool m_bPublicAutoScroll ;
	bool m_bAutoReply;
	std::wstring m_strAutoReply;
	bool m_bBlockSpeakerSound;
	bool m_bBlockGiftMsg;
	bool m_bAutoScrollSpeaker;
	bool m_bBlockUserInOutMsg;
	bool m_bBlockLuckySound;

	//房间音频设置
	uint32 m_unRoomSpeakerVolume;
	uint32 m_unRoomMicVolume;

	core::MessageFont m_TempMessageFont ;	//临时公告字体信息
	core::MessageFont m_ChatMessageFont;    //聊天字体信息
	//一对一私聊设置
	ENM_P2PPermission m_enmP2PPermission;

	static CPersonalConfig* s_pPersonalConfig;
};
