#pragma once
#include "51ktv.h"
#include "coredefine.h"

struct BaseRoomInfo 
{
	std::wstring strRoomName;
	std::wstring strRoomTheme;
	std::wstring strOwnerName;
	uint32 unOwner179ID;
	bool bOwnerMan;
	//std::wstring strRoomNotice;
	//std::wstring strRoomWelcome;

	BaseRoomInfo()
	{
		strRoomName = L"";
		strRoomTheme = L"";
		strOwnerName = L"";
		unOwner179ID = 0;
		bOwnerMan = true;
		//strRoomNotice = L"";
		//strRoomWelcome = L"";
	}
};

enum ENM_RoomPublicChat
{
	PUBLICCHAT_ABOVE_REGISTER	= 0,	//注册用户可用
	PUBLICCHAT_ABOVE_VIP		= 1	,	//vip用户可用
};

struct RoomOptionCode
{
	bool bOpen;
	ENM_RoomPublicChat enmPublicChat;
	bool bColorText;
	bool bUserInOut;
	bool bFreeShowStatus;
	bool bFreeShowForManager;
	bool bFreeShowEditTime;
	bool bPrivateShowEnable;
	bool bPrivateShowStatus;
	bool bAutoWelcome;
};

#define DEFAULT_WELCOME_WORDS	L"@%nick@欢迎您来到%room/掌声//掌声//掌声/"

class RoomOption
{
public:
	RoomOption(uint32 unRoomID);
	~RoomOption(void);

	void LoadRoomOption(std::map<core::ENM_RoomInfoType, std::wstring> & mapKey2String, std::map<core::ENM_RoomInfoType, uint8> & mapKey2Uint8, std::map<core::ENM_RoomInfoType, uint32> & mapKey2Uint32);
	void OnConfirmRoomSetting(std::wstring strName, std::wstring strSignature, bool bOpen, std::wstring strPassword, ENM_RoomPublicChat enmPublicType, bool bColorText, bool bUserInOut);
	void OnConfirmMicSetting(uint8 unFreeShowTime, bool bFreeShowStatus, bool bFreeShowForManager, bool bFreeShowEditTime, bool bPrivateShowStatus);
	void OnConfirmUserManager(/*bool bAutoWelcome, */std::wstring strWelcomeWords, core::MessageFont stFont, bool bDefaultWelcome);
	void OnSaveRoomNotice(std::vector<std::wstring> & vecNoticeList);
	void OnSetInfoOK();

	void SetBaseRoomInfo(BaseRoomInfo & stRoomInfo);

	std::wstring GetRoomName() { return m_strName; }
	std::wstring GetRoomSignature() { return m_strSignature; }
	bool IsOpen() { return m_bOpen; }
	std::wstring GetRoomPassword() { return m_strPassword; }
	bool CanPublicChat() { return m_bPublicChat; }
	ENM_RoomPublicChat GetPublicChatType() { return m_enmPublicChat; }
	bool IsShowUserInOut() { return m_bUserInOut; }
	bool CanColorText() { return m_bColorText; }
	bool CanFreeShow() { return m_bFreeShowStatus; }
	uint8 GetFreeShowTime() { return m_unFreeShowTime; }
	bool IsFreeShowForManager() { return m_bFreeShowForManager; }
	bool CanEditFreeShowTime() { return m_bFreeShowEditTime; }
	bool CanPrivateShow() { return m_bPrivateShowStatus; }
	bool IsPrivateEnable() { return m_bPrivateShowEnable; }//只能获取状态，不能设置
	bool IsAutoWelcome() { return m_bAutoWelcome; }
	std::wstring GetWelcomeWords() { return m_strWelcomeWords; }
	core::MessageFont GetWelcomeFontInfo() { return m_stWelcomeFont; }
	std::vector<std::wstring> GetRoomNotice() { return m_vecRoomNocice; }  //房间公告
	std::wstring GetOfficialNocice() { return m_strOfficialNotice; }
	std::wstring GetOwnerName() { return m_strOwnerName; }
	bool IsOwnerMale() { return m_bOwnerMale; }
	uint32 GetOwner179ID() { return m_unOwner179ID; }
	std::wstring GetDefaulFontName(){return m_strDefaulFontName;}

	void SetRoomName(std::wstring strName) { m_strName = strName; }
	void SetRoomSignature(std::wstring strSignature) { m_strSignature = strSignature; }
	void SetOpen(bool bOpen) { m_bOpen = bOpen; }
	void SetRoomPassword(std::wstring strPassword) { m_strPassword = strPassword; }
	void SetPublicChat(bool bPublicChat) { m_bPublicChat = bPublicChat; }
	void SetPublicChatType(ENM_RoomPublicChat enmType) { m_enmPublicChat = enmType; }
	void SetUserInOut(bool bUserInOut) { m_bUserInOut = bUserInOut; }
	void SetColorText(bool bColorText) { m_bColorText = bColorText; }
	void SetFreeShow(bool bFreeShow) { m_bFreeShowStatus = bFreeShow; }
	void SetFreeShowTime(uint8 unTime) { m_unFreeShowTime = unTime; }
	void SetFreeShowForManager(bool bFreeShowForManager) { m_bFreeShowForManager = bFreeShowForManager; }
	void SetEditFreeShowTime(bool bFreeShowEditTime) { m_bFreeShowEditTime = bFreeShowEditTime; }
	void SetPrivateShow(bool bPrivateShow) { m_bPrivateShowStatus = bPrivateShow; }
	void SetAutoWelcome(bool bAutoWelcome) { m_bAutoWelcome = bAutoWelcome; }
	void SetWelcomeWords(std::wstring strWelcomeWords) { m_strWelcomeWords = strWelcomeWords; }
	void SetOfficialNocice(std::wstring strNotice) { m_strOfficialNotice = strNotice; }
	void SetOwnerName(std::wstring strOwnerName) { m_strOwnerName = strOwnerName; }
	void SetOwnerMale(bool bOwnerMale) { m_bOwnerMale = bOwnerMale; }
	void SetOwner179ID(uint32 unOwner179ID) { m_unOwner179ID = unOwner179ID; }

	bool IsPublicChatChange() { return m_bPublicChatChange; }
	bool IsColorTextChange() { return m_bColorTextChange; }
	bool IsFreeShowChange() { return m_bFreeShowChange; }			//自由排麦是否变更
	bool IsSignatureChange() { return m_bSignatureChange; }			//房间主题是否变更
	bool IsRoomNameChange() { return m_bNameChange; }

	void SetPublicMicStatus( uint16 nIndex , uint8 unStatus ) ;
	uint8 GetPublicMicStatus( uint16 nIndex ) ;
public:
	bool EncodeWelcomeWords(std::wstring & strWelcomeWords, core::MessageFont stFont);
	bool DecodeWelcomeWords(std::wstring & strWelcomeWords, core::MessageFont & stFont);

private:
	void EncodeRoomNotice(std::wstring & strNotice);
	void DecodeRoomNotice(std::wstring strNotice);

	void DecodeRoomOption(uint32 unKey);
	uint32 EncodeRoomOption(RoomOptionCode stRoomOption);

	bool CompareFont(core::MessageFont stFont1, core::MessageFont stFont2);
	void SetDefaultOption(RoomOptionCode & stRoomOption);

private:
	uint32 m_unOptionCode;
	uint32 m_uRoomID;
	std::wstring m_strName;
	std::wstring m_strSignature;
	bool m_bOpen;
	std::wstring m_strPassword;
	bool m_bPublicChat;
	ENM_RoomPublicChat m_enmPublicChat;
	bool m_bColorText;
	bool m_bUserInOut;
	bool m_bFreeShowStatus;
	uint8 m_unFreeShowTime;
	uint8 m_unMicFirstStatus ;			//一号麦的状态 0 open
	uint8 m_unMicSecondStatus ;
	uint8 m_unMicThirdStatus ;
	bool m_bFreeShowForManager;
	bool m_bFreeShowEditTime;
	bool m_bPrivateShowStatus;
	bool m_bPrivateShowEnable;
	bool m_bAutoWelcome;
	std::wstring m_strWelcomeWords;
	core::MessageFont m_stWelcomeFont;
	std::vector<std::wstring> m_vecRoomNocice;
	std::wstring m_strOfficialNotice;
	std::wstring m_strOwnerName;
	bool m_bOwnerMale;
	uint32 m_unOwner179ID;
	std::wstring m_strPreSetName;			//设置后记录一下，收到Rsp后再保存
	std::wstring m_strPreSetSignature;
	std::wstring m_strPreSetPassword;
	uint32 m_unPreSetOptionCode;			//设置后记录一下，收到Rsp后再保存
	uint8 m_unPreSetFreeShowTime;
	std::wstring m_strDefaulFontName;

	bool m_bPublicChatChange;
	bool m_bColorTextChange;
	bool m_bFreeShowChange;
	bool m_bSignatureChange;
	bool m_bNameChange;

	bool m_bInited;
};
