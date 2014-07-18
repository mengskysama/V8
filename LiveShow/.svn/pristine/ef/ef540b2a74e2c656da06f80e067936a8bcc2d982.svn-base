#pragma once
#include "Room.h"
#include "coredefine.h"

class CSendEventManager
{
public:
	CSendEventManager(void);
	~CSendEventManager(void);

	//管理事件
	static bool SendMessageOutEvent(uint32 unRoomID, uint32 unDstUin, bool bPill, std::wstring& strText, core::MessageFont& fontinfo, bool bAutoMsg = FALSE, bool bRejectAutoMsg = FALSE) ;
	static bool SendPushOnEvent(uint32 unRoomID, uint32 unDstUin, uint32 uIndex);
	static bool SendPushOffEvent(uint32 unRoomID, uint32 unDstUin);
	static bool SendKickOutEvent(uint32 unRoomID, uint32 unDstUin, std::wstring strReason, uint16 unTime);
	static bool SendKickOutAllGuestEvent(uint32 unRoomID);
	static bool SendAddBlackEvent(uint32 unRoomID, uint32 unDstUin);
	static bool SendLockIPEvent(uint32 unRoomID, uint32 unDstUin, std::wstring strReason, uint16 unTime);
	static bool SendShutUpEvent(uint32 unRoomID, uint32 unDstUin);
	static bool SendUnlockShutUpEvent(uint32 unRoomID, uint32 unDstUin);
	static bool SendAddTitleEvent(uint32 unRoomID, uint32 unDstUin, uint16 unTitleLevel);
	static bool SendRemoveTitleEvent(uint32 unRoomID, uint32 unDstUin, uint16 unTitleLevel);
	static bool SendCheckCPUandMemEvent(uint32 unRoomID, uint32 unDstUin);
	static bool SendSetMicStatus( uint32 unRoomID , int16 nIndex , int16 nStatus ) ;
	static bool SendFlyingWords(uint32 unRoomID, uint16 unGiftID, std::wstring strWords);
	//基本事件
	static bool SendRequestShow(uint32 unRoomID, uint32 unRequestShowType) ;		//请求排麦 
	static bool SendNormalGiftEvent(uint32 unRoomID, uint32 unDstUin, uint16 unGiftID, uint32 unCount, uint8 unDstType,wstring strGiftWords, bool bLuxury = false);
	static bool SendStampEvent(uint32 unRoomID, uint32 unDstUin, uint16 unStampID);
	static bool SendChangeVoiceEvent(uint32 unRoomID, uint32 unDstUin, uint16 unChangeVoiceID);
	static bool SendOpenPrivateEvent(uint32 unRoomID, uint32 unDstUin);
	static bool SendClosePrivateEvent(uint32 unRoomID, uint32 unDstUin);
	static bool SendRequestP2PEvent(uint32 unRoomID, uint32 unDstUin);
	static bool SendResponseP2PEvent(uint32 unRoomID, uint32 unDstUin, bool bAccept);
	static bool SendCheckNetSpeedEvent(uint32 unRoomID, uint32 unDstUin);
	static bool SendLeaveRoomEvent(uint32 unRoomID);
	static bool SendExitShow(uint32 unRoomID , int nType );
	static bool SendOpVideoAudioEvent(uint32 unRoomID, core::ENM_OperateAVType nType ,uint32 unDstUin);
	static bool SendSpeakerEvent(uint32 unRoomID, uint16 unSpeakerID, core::ENM_LoudSpeakerType nType, std::wstring strContent);
	static bool SendPushOnAnswerEvent(uint32 unRoomID, uint32 unDstUIN, uint32 unIndex, bool bAccept);
	static bool SendBuySofaEvent(uint32 unRoomID, uint8 unIndex, uint32 unPrice);

	//点歌
	static bool SendUpdateSongListEvent(uint32 unRoomID, std::vector<core::Song> vecSongList);
	static bool SendGetSongListEvent(uint32 unRoomID, uint32 unSongerUin);
	static bool SendOrderSongEvent(uint32 unRoomID, uint32 unSongerUin, std::wstring strSongName, std::wstring strAttachWords, uint16 unCount);
	static bool SendGetOrderListEvent(uint32 unRoomID);
	static bool SendProcessSongEvent(uint32 unRoomID, uint32 unOrderID, bool bAccept);
	//评价
	static bool SendMarkSongEvent(uint32 unRoomID, uint32 unOrderID, uint8 unMark);

	//大厅
	static bool SendEnterRoomRequest(uint32 unRoomID);
	static bool SendGuestRegisterRequest( HWND hParentHwnd );
};
