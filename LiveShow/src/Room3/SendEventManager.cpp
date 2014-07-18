#include "StdAfx.h"
#include "SendEventManager.h"
#include "HallDefine.h"

CSendEventManager::CSendEventManager(void)
{
}

CSendEventManager::~CSendEventManager(void)
{
}

bool CSendEventManager::SendMessageOutEvent( uint32 unRoomID, uint32 unDstUin , bool bPill , std::wstring& strText ,core::MessageFont& fontinfo, bool bAutoMsg, bool bRejectAutoMsg) 
{
	core::SendMessageEvent *pSendMsgEvent = new core::SendMessageEvent ;
	pSendMsgEvent->roomid = unRoomID ;	
	pSendMsgEvent->dstuin =  unDstUin ;
	pSendMsgEvent->stFont = fontinfo ;
	pSendMsgEvent->message =  strText ;
	pSendMsgEvent->bIsAutoMsg = bAutoMsg;
	pSendMsgEvent->bRejectAutoMsg = bRejectAutoMsg;
	if ( !bPill )
	{
		pSendMsgEvent->enmType = core::SENDMESSAGETYPE_PUBLIC ; //¹«ÁÄ
	}
	else
	{
		pSendMsgEvent->enmType = core::SENDMESSAGETYPE_PRIVATE ; //Ë½ÁÄ
	}
	return g_RoomModule->m_pModuleManager->PushEvent( MakeEvent<MODULE_ID_ROOM>()(core::EVENT_VALUE_CORE_SEND_MESSAGE ,MODULE_ID_CORE,0,0,0,pSendMsgEvent) ) ;
}

bool CSendEventManager::SendPushOnEvent( uint32 unRoomID, uint32 unDstUin, uint32 uIndex )
{
	return g_RoomModule->m_pModuleManager->PushEvent(MakeEvent<MODULE_ID_ROOM>()(core::EVENT_VALUE_CORE_PUSH_ON_SHOW, MODULE_ID_CORE, unRoomID, unDstUin, uIndex));
}

bool CSendEventManager::SendPushOffEvent( uint32 unRoomID, uint32 unDstUin )
{
	return g_RoomModule->m_pModuleManager->PushEvent(MakeEvent<MODULE_ID_ROOM>()(core::EVENT_VALUE_CORE_PULL_OFF_SHOW, MODULE_ID_CORE, unRoomID, unDstUin));
}

bool CSendEventManager::SendKickOutEvent( uint32 unRoomID, uint32 unDstUin, std::wstring strReason, uint16 unTime )
{
	core::KickUserEvent * pKickUserEvent = new core::KickUserEvent();
	pKickUserEvent->strReason = strReason;
	pKickUserEvent->unRoomID = unRoomID;
	pKickUserEvent->unTime = unTime;
	pKickUserEvent->bKickAllVisitor = false;
	pKickUserEvent->unDstUIN = unDstUin;
	return g_RoomModule->m_pModuleManager->PushEvent(MakeEvent<MODULE_ID_ROOM>()(core::EVENT_VALUE_CORE_KICK_USER, MODULE_ID_CORE, 0, 0, 0, pKickUserEvent));
}

bool CSendEventManager::SendKickOutAllGuestEvent( uint32 unRoomID )
{
	core::KickUserEvent * pKickUserEvent = new core::KickUserEvent();
	pKickUserEvent->unRoomID = unRoomID;
	//ASSERT(m_pUserManager != NULL);
	pKickUserEvent->bKickAllVisitor = true;
	return g_RoomModule->m_pModuleManager->PushEvent(MakeEvent<MODULE_ID_ROOM>()(core::EVENT_VALUE_CORE_KICK_USER, MODULE_ID_CORE, 0, 0, 0, pKickUserEvent));
}

bool CSendEventManager::SendAddBlackEvent( uint32 unRoomID, uint32 unDstUin )
{
	return g_RoomModule->m_pModuleManager->PushEvent(MakeEvent<MODULE_ID_ROOM>()(core::EVENT_VALUE_CORE_ADD_BLACK, MODULE_ID_CORE, unRoomID, unDstUin));
}

bool CSendEventManager::SendLockIPEvent( uint32 unRoomID, uint32 unDstUin, std::wstring strReason, uint16 unTime )
{
	core::LockIPEvent * pLockIPEvent = new core::LockIPEvent();
	pLockIPEvent->strReason = strReason;
	pLockIPEvent->unRoomID = unRoomID;
	pLockIPEvent->unTime = unTime/60;
	pLockIPEvent->unDstUIN = unDstUin;
	return g_RoomModule->m_pModuleManager->PushEvent(MakeEvent<MODULE_ID_ROOM>()(core::EVENT_VALUE_CORE_LOCK_IP, MODULE_ID_CORE, 0, 0, 0, pLockIPEvent));
}

bool CSendEventManager::SendShutUpEvent( uint32 unRoomID, uint32 unDstUin )
{
	return g_RoomModule->m_pModuleManager->PushEvent(MakeEvent<MODULE_ID_ROOM>()(core::EVENT_VALUE_CORE_SHUT_UP, MODULE_ID_CORE, unRoomID, unDstUin));
}

bool CSendEventManager::SendUnlockShutUpEvent( uint32 unRoomID, uint32 unDstUin )
{
	return g_RoomModule->m_pModuleManager->PushEvent(MakeEvent<MODULE_ID_ROOM>()(core::EVENT_VALUE_CORE_UNLOCK_SHUTUP, MODULE_ID_CORE, unRoomID, unDstUin));
}

bool CSendEventManager::SendAddTitleEvent( uint32 unRoomID, uint32 unDstUin, uint16 unTitleLevel )
{
	return g_RoomModule->m_pModuleManager->PushEvent(MakeEvent<MODULE_ID_ROOM>()(core::EVENT_VALUE_CORE_ADD_TITLE, MODULE_ID_CORE, unRoomID, unTitleLevel, unDstUin));
}

bool CSendEventManager::SendRemoveTitleEvent( uint32 unRoomID, uint32 unDstUin, uint16 unTitleLevel )
{
	return g_RoomModule->m_pModuleManager->PushEvent(MakeEvent<MODULE_ID_ROOM>()(core::EVENT_VALUE_CORE_REMOVE_TITLE, MODULE_ID_CORE, unRoomID, unTitleLevel, unDstUin));
}


bool CSendEventManager::SendCheckCPUandMemEvent( uint32 unRoomID, uint32 unDstUin )
{
	core::QueryClientInfoEvent * pEvent = new core::QueryClientInfoEvent();
	pEvent->unRoomID = unRoomID;
	pEvent->unDstUIN = unDstUin;
	pEvent->bHardwareInfo = true;
	pEvent->bNetInfo = false;
	pEvent->bVideoInfo = false;
	return g_RoomModule->m_pModuleManager->PushEvent(MakeEvent<MODULE_ID_ROOM>()(core::EVENT_VALUE_CORE_QUERY_CLIENTINFO, MODULE_ID_CORE, 0, 0, 0, pEvent));
}

bool CSendEventManager::SendSetMicStatus( uint32 unRoomID , int16 nIndex , int16 nStatus ) 
{
	uint32 unParam = nIndex ;
	unParam<<= 16 ;
	unParam |= nStatus ;
	return g_RoomModule->m_pModuleManager->PushEvent( MakeEvent<MODULE_ID_ROOM>()(core::EVENT_VALUE_CORE_MIC_SET_MIC_STATUS,MODULE_ID_CORE,unRoomID, unParam)) ;
}

bool CSendEventManager::SendRequestShow( uint32 unRoomID, uint32 unRequestShowType ) 
{ //ÇëÇóÅÅÂó
	return g_RoomModule->m_pModuleManager->PushEvent(MakeEvent<MODULE_ID_ROOM>()(core::EVENT_VALUE_CORE_REQUEST_SHOW,MODULE_ID_CORE, unRoomID, unRequestShowType)) ;
}

bool CSendEventManager::SendNormalGiftEvent( uint32 unRoomID, uint32 unDstUin, uint16 unGiftID, uint32 unCount, uint8 unDstType,wstring strGiftWords, bool bLuxury )
{
	core::SendGiftEvent * pSendGiftEvent = new core::SendGiftEvent();
	if (bLuxury)
	{
		pSendGiftEvent->enmType = core::GIFTTYPE_SHECHI;
	}
	else
	{
		pSendGiftEvent->enmType = core::GIFTTYPE_NORMAL;
	}
	pSendGiftEvent->unRoomID = unRoomID;
	pSendGiftEvent->unDstUIN = unDstUin;
	pSendGiftEvent->unGiftID = unGiftID;
	pSendGiftEvent->unCount = unCount;
	pSendGiftEvent->strAttachMessage = strGiftWords ;
	pSendGiftEvent->enmDstType = (core::ENM_GiftDstType)unDstType;
	return g_RoomModule->m_pModuleManager->PushEvent(MakeEvent<MODULE_ID_ROOM>()(core::EVENT_VALUE_CORE_SEND_GIFT, MODULE_ID_CORE, 0, 0, 0, pSendGiftEvent));
}

bool CSendEventManager::SendStampEvent( uint32 unRoomID, uint32 unDstUin, uint16 unStampID )
{
	core::SendGiftEvent * pSendGiftEvent = new core::SendGiftEvent();
	pSendGiftEvent->enmType = core::GIFTTYPE_STAMP;
	pSendGiftEvent->unGiftID = unStampID;
	pSendGiftEvent->unDstUIN = unDstUin;
	pSendGiftEvent->unRoomID = unRoomID;
	pSendGiftEvent->enmDstType = core::GIFTDSTTYPE_SINGLE;
	pSendGiftEvent->unCount = 1;
	return g_RoomModule->m_pModuleManager->PushEvent(MakeEvent<MODULE_ID_ROOM>()(core::EVENT_VALUE_CORE_SEND_GIFT, MODULE_ID_CORE, 0, 0, 0, pSendGiftEvent));
}

bool CSendEventManager::SendOpenPrivateEvent( uint32 unRoomID, uint32 unDstUin )
{
	return g_RoomModule->m_pModuleManager->PushEvent(MakeEvent<MODULE_ID_ROOM>()(core::EVENT_VALUE_CORE_OPEN_PRIVATE, MODULE_ID_CORE, unRoomID, unDstUin));
}

bool CSendEventManager::SendClosePrivateEvent( uint32 unRoomID, uint32 unDstUin )
{
	return g_RoomModule->m_pModuleManager->PushEvent(MakeEvent<MODULE_ID_ROOM>()(core::EVENT_VALUE_CORE_CLOSE_PRIVATE, MODULE_ID_CORE, unRoomID, unDstUin));
}

bool CSendEventManager::SendRequestP2PEvent( uint32 unRoomID, uint32 unDstUin )
{
	return g_RoomModule->m_pModuleManager->PushEvent(MakeEvent<MODULE_ID_ROOM>()(core::EVENT_VALUE_CORE_REQUEST_P2P, MODULE_ID_CORE, unRoomID, unDstUin));
}

bool CSendEventManager::SendResponseP2PEvent( uint32 unRoomID, uint32 unDstUin, bool bAccept )
{
	return g_RoomModule->m_pModuleManager->PushEvent(MakeEvent<MODULE_ID_ROOM>()(core::EVENT_VALUE_CORE_RESPONSE_P2P, MODULE_ID_CORE, unRoomID, (bAccept ? core::RESPONSEP2PTYPE_ACCEPT : core::RESPONSEP2PTYPE_REFUSE), unDstUin));
}


bool CSendEventManager::SendCheckNetSpeedEvent( uint32 unRoomID, uint32 unDstUin )
{
	core::QueryClientInfoEvent * pEvent = new core::QueryClientInfoEvent();
	pEvent->unRoomID = unRoomID;
	pEvent->unDstUIN = unDstUin;
	pEvent->bHardwareInfo = false;
	pEvent->bNetInfo = true;
	pEvent->bVideoInfo = false;
	return g_RoomModule->m_pModuleManager->PushEvent(MakeEvent<MODULE_ID_ROOM>()(core::EVENT_VALUE_CORE_QUERY_CLIENTINFO, MODULE_ID_CORE, 0, 0, 0, pEvent));
}

bool CSendEventManager::SendLeaveRoomEvent( uint32 unRoomID )
{	
	return g_RoomModule->m_pModuleManager->PushEvent(MakeEvent<MODULE_ID_ROOM>()(core::EVENT_VALUE_CORE_LEAVE_ROOM, MODULE_ID_CORE, unRoomID));
}

bool CSendEventManager::SendExitShow( uint32 unRoomID , int nType ) 
{
	return g_RoomModule->m_pModuleManager->PushEvent(MakeEvent<MODULE_ID_ROOM>()(core::EVENT_VALUE_CORE_EXIT_SHOW,MODULE_ID_CORE,unRoomID , nType )) ;
}

bool CSendEventManager::SendOpVideoAudioEvent( uint32 unRoomID, core::ENM_OperateAVType nType ,uint32 unDstUin ) 
{
	return g_RoomModule->m_pModuleManager->PushEvent(MakeEvent<MODULE_ID_ROOM>()(core::EVENT_VALUE_CORE_OPERATE_VIDEO_AUDIO,MODULE_ID_CORE,unRoomID ,nType,unDstUin)) ;
}

bool CSendEventManager::SendSpeakerEvent( uint32 unRoomID, uint16 unSpeakerID, core::ENM_LoudSpeakerType nType, std::wstring strContent )
{
	core::SendLoudSpeakerEvent * pEvent = new core::SendLoudSpeakerEvent();
	pEvent->enmType = nType;
	pEvent->unRoomID = unRoomID;
	pEvent->unGiftID = unSpeakerID;
	pEvent->strMessage = strContent;
	return g_RoomModule->m_pModuleManager->PushEvent(MakeEvent<MODULE_ID_ROOM>()(core::EVENT_VALUE_CORE_SEND_LOUDSPEAKER, MODULE_ID_CORE, 0, 0, 0, pEvent));
}

bool CSendEventManager::SendEnterRoomRequest( uint32 unRoomID )
{
	return g_RoomModule->m_pModuleManager->PushEvent(MakeEvent<MODULE_ID_ROOM>()(Hall::EVENT_VALUE_HALL_ENTER_ROOM_REQUEST, MODULE_ID_HALL, unRoomID));
}

bool CSendEventManager::SendChangeVoiceEvent( uint32 unRoomID, uint32 unDstUin, uint16 unChangeVoiceID )
{
	core::SendGiftEvent * pSendGiftEvent = new core::SendGiftEvent();
	pSendGiftEvent->enmType = core::GIFTTYPE_CHANGEVOICE;
	pSendGiftEvent->unGiftID = unChangeVoiceID;
	pSendGiftEvent->unDstUIN = unDstUin;
	pSendGiftEvent->unRoomID = unRoomID;
	pSendGiftEvent->enmDstType = core::GIFTDSTTYPE_SINGLE;
	pSendGiftEvent->unCount = 1;
	return g_RoomModule->m_pModuleManager->PushEvent(MakeEvent<MODULE_ID_ROOM>()(core::EVENT_VALUE_CORE_SEND_GIFT, MODULE_ID_CORE, 0, 0, 0, pSendGiftEvent));
}

bool CSendEventManager::SendFlyingWords( uint32 unRoomID, uint16 unGiftID, std::wstring strWords )
{
	core::SendFlyWordsEvent * pSendFlyWordsEvent = new core::SendFlyWordsEvent();
	pSendFlyWordsEvent->unRoomID = unRoomID;
	pSendFlyWordsEvent->unGiftID = unGiftID;
	pSendFlyWordsEvent->strMessage = strWords;
	return g_RoomModule->m_pModuleManager->PushEvent(MakeEvent<MODULE_ID_ROOM>()(core::EVENT_VALUE_CORE_SEND_FLYWORDS, MODULE_ID_CORE, 0, 0, 0, pSendFlyWordsEvent));
}

bool CSendEventManager::SendGuestRegisterRequest( HWND hParentHwnd )
{
	RoomShowRegister* pRoomShowRegister = new RoomShowRegister() ;
	pRoomShowRegister->m_parentHwnd = hParentHwnd ;
	return g_RoomModule->m_pModuleManager->PushEvent(MakeEvent<MODULE_ID_ROOM>()(Room::EVENT_VALUE_ROOM_SHOW_REGISTER, MODULE_ID_HALL,0,0,0,pRoomShowRegister));
}

bool CSendEventManager::SendPushOnAnswerEvent( uint32 unRoomID, uint32 unDstUIN, uint32 unIndex, bool bAccept )
{
	core::PushOnShowAnswerEvent * pPushOnShowAnswerEvent = new core::PushOnShowAnswerEvent();
	pPushOnShowAnswerEvent->unRoomID = unRoomID;
	pPushOnShowAnswerEvent->unDstUIN = unDstUIN;
	pPushOnShowAnswerEvent->unIndex = unIndex;
	pPushOnShowAnswerEvent->enmType = bAccept ? core::ANSWERPUSHONSHOW_ACCEPT : core::ANSWERPUSHONSHOW_REFUSE;
	return g_RoomModule->m_pModuleManager->PushEvent(MakeEvent<MODULE_ID_ROOM>()(core::EVENT_VALUE_CORE_ANSWER_PUSH_ON_SHOW, MODULE_ID_CORE, 0, 0, 0, pPushOnShowAnswerEvent));
}

bool CSendEventManager::SendBuySofaEvent( uint32 unRoomID, uint8 unIndex, uint32 unPrice )
{
	return g_RoomModule->m_pModuleManager->PushEvent(MakeEvent<MODULE_ID_ROOM>()(core::EVENT_VALUE_CORE_BUY_SOFA, MODULE_ID_CORE, unRoomID, unIndex, unPrice)) ;
}

bool CSendEventManager::SendUpdateSongListEvent( uint32 unRoomID, std::vector<core::Song> vecSongList )
{
	core::UpdateSongListEvent * pUpdateSongListEvent = new core::UpdateSongListEvent();
	pUpdateSongListEvent->roomid = unRoomID;
	pUpdateSongListEvent->vecSongList = vecSongList;
	return g_RoomModule->m_pModuleManager->PushEvent(MakeEvent<MODULE_ID_ROOM>()(core::EVENT_VALUE_UPDATE_SONGLIST, MODULE_ID_CORE, 0, 0, 0, pUpdateSongListEvent));
}

bool CSendEventManager::SendGetSongListEvent( uint32 unRoomID, uint32 unSongerUin )
{
	return g_RoomModule->m_pModuleManager->PushEvent(MakeEvent<MODULE_ID_ROOM>()(core::EVENT_VALUE_GET_SONGLIST, MODULE_ID_CORE, unRoomID, unSongerUin)) ;
}

bool CSendEventManager::SendOrderSongEvent( uint32 unRoomID, uint32 unSongerUin, std::wstring strSongName, std::wstring strAttachWords, uint16 unCount )
{
	core::OrderSongEvent * pOrderSongEvent = new core::OrderSongEvent();
	pOrderSongEvent->roomid = unRoomID;
	pOrderSongEvent->songerUin = unSongerUin;
	pOrderSongEvent->songName = strSongName;
	pOrderSongEvent->attachWords = strAttachWords;
	pOrderSongEvent->count = unCount;
	return g_RoomModule->m_pModuleManager->PushEvent(MakeEvent<MODULE_ID_ROOM>()(core::EVENT_VALUE_ORDER_SONG, MODULE_ID_CORE, 0, 0, 0, pOrderSongEvent));
}

bool CSendEventManager::SendGetOrderListEvent( uint32 unRoomID )
{
	return g_RoomModule->m_pModuleManager->PushEvent(MakeEvent<MODULE_ID_ROOM>()(core::EVENT_VALUE_GET_ORDERLIST, MODULE_ID_CORE, unRoomID)) ;
}

bool CSendEventManager::SendProcessSongEvent( uint32 unRoomID, uint32 unOrderID, bool bAccept )
{
	uint32 unProcess = (bAccept ? 1 : 2);
	return g_RoomModule->m_pModuleManager->PushEvent(MakeEvent<MODULE_ID_ROOM>()(core::EVENT_VALUE_PROCESS_ORDER, MODULE_ID_CORE, unRoomID, unOrderID, unProcess));
}

bool CSendEventManager::SendMarkSongEvent( uint32 unRoomID, uint32 unOrderID, uint8 unMark )
{
	return g_RoomModule->m_pModuleManager->PushEvent(MakeEvent<MODULE_ID_ROOM>()(core::EVENT_VALUE_MARK_SONG, MODULE_ID_CORE, unRoomID, unOrderID, unMark));
}