#pragma once
#include "coredefine.h"
class IUserItem
{
public:
	IUserItem(void);
	~IUserItem(void);

	virtual uint32 GetUserUIN() { return m_UserInfo.unUIN; }
	virtual uint32 GetUser179ID() { return m_UserInfo.un179id; } 
	virtual std::wstring GetDecodeUserName() { return m_strDecodeName ; }
	virtual std::wstring GetUserName() { return m_UserInfo.strName; }
	virtual uint8 GetUserVipLevel() { return m_UserInfo.unVipLevel; }
	virtual uint8 GetUserUsrLevel() { return m_UserInfo.unUsrLevel; }
	virtual uint8 GetUserRichLevel() { return m_UserInfo.unRichLevel; }
	virtual uint16 GetUserTitleLevel() { return m_UserInfo.unTitleLevel; }
	//virtual std::wstring GetUserJiaZu() { return m_UserInfo.strJiaZu; }
	virtual uint16 GetUserStatus() { return m_UserInfo.unStatus; }
	virtual uint16 GetUserStampID() { return m_unStampID; }
	virtual bool IsWaiting() { return m_bWaiting; }
	virtual void SetWaiting(bool bWaiting) { m_bWaiting = bWaiting; }
	//virtual uint32 GetUserRoomID() { return m_roomid; }
	virtual core::room_user_info * GetUserInfo() { return &m_UserInfo; }
	//virtual void SetUserStatus(uint16 unStatus) { m_UserInfo.unStatus |= unStatus; }
	virtual void AddUserStatus(uint16 unStatus) { m_UserInfo.unStatus |= unStatus; }
	virtual void RemoveUserStatus(uint16 unStatus) { m_UserInfo.unStatus &= ~unStatus; }
	virtual void SetUserSatus(uint16 unStatus) { m_UserInfo.unStatus = unStatus; }
	virtual void SetUserTitle(uint16 unTitle) { m_UserInfo.unTitleLevel = unTitle; }
	virtual void SetUserVipLevel(uint8 unVipLevel) { m_UserInfo.unVipLevel = unVipLevel; }
	virtual void SetUserRichLevel(uint8 unRichLevel) { m_UserInfo.unRichLevel = unRichLevel; }
	virtual void SetUserStamp(uint16 unStampID) { m_unStampID = unStampID; }
	virtual void SetUserInfo(core::room_user_info & userinfo);
	//virtual bool IsShowJiaZu() { return m_UserInfo.bShowJiaZu; }
	virtual bool IsPublicOne() { return ((m_UserInfo.unStatus & core::USERSTATUSTYPE_PUBLIC_1) != 0); }
	virtual bool IsPublicTwo() { return ((m_UserInfo.unStatus & core::USERSTATUSTYPE_PUBLIC_2) != 0); }
	virtual bool IsPublicThree() { return ((m_UserInfo.unStatus & core::USERSTATUSTYPE_PUBLIC_3) != 0); }
	virtual bool IsPrivate() { return ((m_UserInfo.unStatus & core::USERSTATUSTYPE_PRIVATE) != 0); }
	virtual bool IsP2P() { return ((m_UserInfo.unStatus & core::USERSTATUSTYPE_P2P) != 0); }
	virtual bool IsForbidden() { return ((m_UserInfo.unStatus & core::USERSTATUSTYPE_FORBIDDEN) != 0); }
	virtual bool IsVideo() { return ((m_UserInfo.unStatus & core::USERSTATUSTYPE_VIDEO) != 0); }
	virtual bool IsAudio() { return ((m_UserInfo.unStatus & core::USERSTATUSTYPE_AUDIO) != 0); }
	virtual bool IsMale() { return m_UserInfo.bMan; }
	virtual bool IsBlinkEnter() { return m_unBlinkEnterID != 0; }
	virtual uint16 GetBlinkEnterID() { return m_unBlinkEnterID; }
	virtual uint32 GetListIconIndex(uint32 unColumn) { return m_unListIconIndex[unColumn]; }
	virtual void SetListIconIndex(uint32 unColumn, uint32 unIndex) { m_unListIconIndex[unColumn] = unIndex; }
	virtual void AddGiftItemID(uint16 unGiftID);
	virtual void DelGiftItemID(uint16 unGiftID);
	virtual bool IsBlockChatToMe() { return m_bBlockChatToMe; }
	virtual void SetBlockChatToMe(bool bBlock) { m_bBlockChatToMe = bBlock; }
	virtual bool IsSinger() { return ((m_UserInfo.unBusiness & core::BUSINESSTYPE_SONGER) != 0); }
	virtual bool IsDancer() { return ((m_UserInfo.unBusiness & core::BUSINESSTYPE_DANCER) != 0); }
	virtual bool IsHost() { return ((m_UserInfo.unBusiness & core::BUSINESSTYPE_HOST) != 0); }
	virtual bool IsOperator() { return ((m_UserInfo.unBusiness & core::BUSINESSTYPE_OPERATOR) != 0); }
	virtual bool IsAgent() { return ((m_UserInfo.unBusiness & core::BUSINESSTYPE_AGENT) != 0); }
	virtual bool IsPatrolMan() { return ((m_UserInfo.unBusiness & core::BUSINESSTYPE_PATROLMAN) != 0); }
	virtual bool IsAdCleaner() { return ((m_UserInfo.unBusiness & core::BUSINESSTYPE_AD_CLEANER) != 0); }
	virtual bool IsHaveCam() { return m_UserInfo.stClientInfo.haveCamera(); }
	virtual int GetPublicMicIndex();
	virtual bool IsActor() ;		//是否是艺人
	virtual bool IsOfficial();		//是否是官方人员
	virtual uint16 GetUserChangeVoiceID() { return m_unChangeVoiceID; }
	virtual void SetUserChangeVoice(uint16 unChangeVoiceID) { m_unChangeVoiceID = unChangeVoiceID; }
	virtual bool IsChangeVoice() { return m_unChangeVoiceID != 0; }
	virtual bool IsHideLogin() { return m_UserInfo.stClientInfo.isHideLogin(); }
	virtual bool IsLuckyStar() { return ((m_UserInfo.unBusiness & core::BUSINESSTYPE_LUCKY_STAR) != 0); }
	virtual std::vector<core::GiftStarInfo> GetGiftStarInfo();
	virtual uint8 GetWealthyIndex() { return m_unWealthyIndex; }
protected:
	bool m_bWaiting;
	core::room_user_info m_UserInfo;
	uint32 m_unListIconIndex[5];
	//bool m_bBlinkEnter;
	uint16 m_unBlinkEnterID;
	uint16 m_unStampID;
	bool m_bBlockChatToMe;
	uint16 m_unChangeVoiceID;
	std::wstring m_strDecodeName ;			//插入HTML代码中的用户昵称
			
	uint8 m_unWealthyIndex;					//是否是万元户
	//uint32 m_roomid;
};
