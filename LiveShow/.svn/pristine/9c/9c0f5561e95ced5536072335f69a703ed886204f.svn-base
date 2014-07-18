#include "StdAfx.h"
#include "CoreModule.h"
#include "ServerPacketProcess.h"
#include "protocol\ProtocolDefine.h"
#include "protocolInclude.h"
#include "BlacklistHelper.h"

#define PACKET_PROCESS(protocol,func) \
	case protocol: \
		func(srvHead, byteBuf); \
		break; \

ServerPacketProcess::ServerPacketProcess(CoreModule* pCoreModule)
:m_pCoreModule(pCoreModule)
{

}

ServerPacketProcess::~ServerPacketProcess(void)
{

}

void ServerPacketProcess::process(ServerHead& srvHead, ByteBuf& byteBuf)
{
	switch(srvHead.GetCmd())
	{
		PACKET_PROCESS(HALL_GET_SELFINFO_RSP, onGetSelfInfoRsp);	
		PACKET_PROCESS(HALL_SET_SELFINFO_RSP, onSetSelfInfoRsp);
		PACKET_PROCESS(HALL_SET_SELFINFO_NOTIFY, onSetSelfInfoNotify);
		PACKET_PROCESS(HALL_SRV_KICK_USER, onKicked);
		PACKET_PROCESS(HALL_SRV_NOTIFY_MESSAGE, onNotifyMessage);
		PACKET_PROCESS(HALL_SRV_NOTIFY_PASSWORD_SAFE, onNotifyPasswordSafe);
		PACKET_PROCESS(HALL_SRV_NOTIFY_USERINFOUPDATE, onMyInfoUpdateNotify);
		PACKET_PROCESS(HALL_SRV_NOTIFY_GIFT_STAR, onGiftStarNotify);
		PACKET_PROCESS(HALL_SRV_NOTIFY_MY_GOODS, onMyGoodsNotify);
		PACKET_PROCESS(ROOM_LOGIN_RSP, onRoomLoginRsp);
		PACKET_PROCESS(ROOM_LOGIN_NOTIFY, onRoomLoginNotify);
		PACKET_PROCESS(ROOM_LOGOUT_NOTIFY, onRoomLogoutNotify);
		PACKET_PROCESS(ROOM_GET_USERLIST_RSP, onGetUserListRsp);
		PACKET_PROCESS(ROOM_GET_WAITTINGLIST_RSP, onGetWaittingListRsp);
		PACKET_PROCESS(ROOM_SEND_MESSAGE_RSP, onSendMessageRsp);
		PACKET_PROCESS(ROOM_SEND_MESSAGE_NOTIFY, onSendMessageNotify);
		PACKET_PROCESS(ROOM_SEND_GIFT_RSP, onSendGiftRsp);
		PACKET_PROCESS(ROOM_SEND_GIFT_NOTIFY, onSendGiftNotify);
		PACKET_PROCESS(ROOM_SEND_LOUDSPEAKER_RSP, onSendLoudSpeakerRsp);
		PACKET_PROCESS(ROOM_SEND_LOUDSPEAKER_NOTIFY, onSendLoudSpeakerNotify);
		PACKET_PROCESS(ROOM_REQUEST_SHOW_RSP, onRequestShowRsp);
		PACKET_PROCESS(ROOM_REQUEST_SHOW_NOTIFY, onRequestShowNotify);
		PACKET_PROCESS(ROOM_EXIT_SHOW_NOTIFY, onExitShowNotify);
		PACKET_PROCESS(ROOM_OPERATE_VIDEO_AUDIO_RSP, onOperateAVRsp);
		PACKET_PROCESS(ROOM_OPERATE_VIDEO_AUDIO_NOTIFY, onOperateAVNotify);
		PACKET_PROCESS(ROOM_KICK_USER_RSP, onKickUserRsp);
		PACKET_PROCESS(ROOM_KICK_USER_NOTIFY, onKickUserNotify);
		PACKET_PROCESS(ROOM_ADD_BLACK_RSP, onAddBlackRsp);
		PACKET_PROCESS(ROOM_ADD_BLACK_NOTIFY, onAddBlackNotify);
		PACKET_PROCESS(ROOM_LOCK_IP_RSP, onLockIPRsp);
		PACKET_PROCESS(ROOM_LOCK_IP_NOTIFY, onLockIPNotify);
		PACKET_PROCESS(ROOM_SHUT_UP_RSP, onShutUpRsp);
		PACKET_PROCESS(ROOM_SHUT_UP_NOTIFY, onShutUpNotify);
		PACKET_PROCESS(ROOM_UNLOCK_SHUTUP_RSP, onUnlockShutUpRsp);
		PACKET_PROCESS(ROOM_UNLOCK_SHUTUP_NOTIFY, onUnLockShutUpNotify);
		PACKET_PROCESS(ROOM_PUSH_ON_SHOW_RSP, onPushOnShowRsp);
		PACKET_PROCESS(ROOM_PUSH_ON_SHOW_NOTIFY, onPushOnShowNotify);
		PACKET_PROCESS(ROOM_PULL_OFF_SHOW_RSP, onPullOffShowRsp);
		PACKET_PROCESS(ROOM_PULL_OFF_SHOW_NOTIFY, onPullOffShowNotify);
		PACKET_PROCESS(ROOM_ADD_TITLE_RSP, onAddTitleRsp);
		PACKET_PROCESS(ROOM_ADD_TITLE_NOTIFY, onAddTitleNotify);
		PACKET_PROCESS(ROOM_REMOVE_TITLE_RSP, onRemoveTitleRsp);
		PACKET_PROCESS(ROOM_REMOVE_TITLE_NOTIFY, onRemoveTitleNotify);
		PACKET_PROCESS(ROOM_OPEN_PRIVATE_RSP, onOpenPrivateRsp);
		PACKET_PROCESS(ROOM_REQUEST_P2P_RSP, onRequestP2PRsp);
		PACKET_PROCESS(ROOM_REQUEST_P2P_NOTIFY, onRequestP2PNotify);
		PACKET_PROCESS(ROOM_RESPONSE_P2P_RSP, onReponseP2PRsp);
		PACKET_PROCESS(ROOM_RESPONSE_P2P_NOTIFY, onResponseP2PNotify);
		PACKET_PROCESS(ROOM_REPORT_CLIENTINFO_NOTIFY, onReportClientInfoNotify);
		PACKET_PROCESS(ROOM_GET_CLIENTINFO_RSP, onGetClientInfoRsp);
		PACKET_PROCESS(ROOM_SET_ROOMINFO_RSP, onSetRoomInfoRsp);
		PACKET_PROCESS(ROOM_SET_ROOMINFO_NOTIFY, onSetRoomInfoNotify);
		PACKET_PROCESS(ROOM_GET_ROOMINFO_RSP, onGetRoomInfoRsp);
		PACKET_PROCESS(ROOM_SRV_USERINFO_CHANGE, onUserInfoChange);
		PACKET_PROCESS(ROOM_SRV_PRIVATE_SHOW_NOTIFY, onPrivateShowList);
		PACKET_PROCESS(ROOM_SRV_GIFT_OVER_TIME, onGiftOverTime);
		PACKET_PROCESS(ROOM_SRV_MONEY_UPDATE, onMoneyUpdate);
		PACKET_PROCESS(ROOM_SRV_OFFICIAL_NOTICE, onOfficialNotice);
		PACKET_PROCESS(ROOM_SRV_GIFT_PLUTUS_AWARD, onPlutusAward);
		PACKET_PROCESS(ROOM_SRV_ROOM_LOCKED, onRoomLocked);
		PACKET_PROCESS(ROOM_SET_MIC_STATUS_RSP, onSetMicStatusRsp);
		PACKET_PROCESS(ROOM_SET_MIC_STATUS_NOTIFY, onSetMicStatusNotify);
		PACKET_PROCESS(ROOM_SEND_FLYWORDS_RSP, onSendFlyWordsRsp);
		PACKET_PROCESS(ROOM_SEND_FLYWORDS_NOTIFY, onSendFlyWordsNotify);
		PACKET_PROCESS(ROOM_SRV_MEDIA_RESET_NOTIFY, onMediaServerResetNotify);
		PACKET_PROCESS(ROOM_SRV_RICH_STAR_NOTIFY, onRichStarNotify);
		PACKET_PROCESS(ROOM_ANSWER_PUSH_ON_SHOW_RSP, onPushOnShowAnswerRsp);
		PACKET_PROCESS(HALL_SRV_USER_TASK_STAT, onTaskNotify);
		PACKET_PROCESS(HALL_GET_TASK_REWARD_RSP, onGetTaskRewardRsp);
		PACKET_PROCESS(HALL_GET_FANS_RSP, onGetFansRsp);
		PACKET_PROCESS(ROOM_BUY_SOFA_RSP, onBuySofaRsp);
		PACKET_PROCESS(ROOM_BUY_SOFA_NOTIFY, onBuySofaNotify);
		PACKET_PROCESS(ROOM_SRV_CLEAR_SOFA_LIST, onClearSofaList);
		PACKET_PROCESS(ROOM_GET_SOFA_LIST_RSP, onGetSofaList);
		PACKET_PROCESS(ROOM_UPDATE_SONGLIST_RSP, onUpdateSongList);
		PACKET_PROCESS(ROOM_GET_SONGLIST_RSP, onGetSongList);
		PACKET_PROCESS(ROOM_ORDER_SONG_RSP, onOrderSongRsp);
		PACKET_PROCESS(ROOM_ORDER_SONG_NOTIFY, onOrderSongNotify);
		PACKET_PROCESS(ROOM_PROCESS_ORDER_RSP, onProcessOrderRsp);
		PACKET_PROCESS(ROOM_PROCESS_ORDER_NOTIFY, onProcessOrderNotify);
		PACKET_PROCESS(ROOM_GET_ORDERLIST_RSP, onGetOrderlistRsp);
		PACKET_PROCESS(ROOM_GET_OFFLINE_ACTOR_LIST_RSP, onGetOfflineActorRsp);
		PACKET_PROCESS(ROOM_MARK_SONG_RSP, onMarkSongRsp);
		PACKET_PROCESS(ROOM_MARK_SONG_NOTIFY, onMarkSongNotify);

	default:
		assert(0);
		break;
	}
}

void ServerPacketProcess::onGetSelfInfoRsp(ServerHead& srvHead, ByteBuf& byteBuf)
{
	GetSelfInfoRspPacket packet(byteBuf);
	packet.Unpack();
	
	CoreModule::GetCurInfo().bGetRichInfo = true;
	CoreModule::GetCurInfo().stRichInfo.strCity = packet.home_city;
	CoreModule::GetCurInfo().stRichInfo.strProvince = packet.home_province;
	CoreModule::GetCurInfo().stRichInfo.unYear = packet.bithday_year;
	CoreModule::GetCurInfo().stRichInfo.unMonth = packet.bithday_month;
	CoreModule::GetCurInfo().stRichInfo.unDay = packet.bithday_day;

	m_pCoreModule->GetModuleManager()->PushEvent(
		MakeEvent<MODULE_ID_CORE>()(EVENT_VALUE_CORE_GET_SELFINFO_RSP, 
		MODULE_ID_HALL, 0, 0, 0, NULL));
}

void ServerPacketProcess::onSetSelfInfoRsp(ServerHead& srvHead, ByteBuf& byteBuf)
{
	SetSelfInfoRspPacket packet(byteBuf);
	packet.Unpack();

	m_pCoreModule->GetModuleManager()->PushEvent(
		MakeEvent<MODULE_ID_CORE>()(EVENT_VALUE_CORE_SET_SELFINFO_RSP, 
		MODULE_ID_ROOM, packet.result, 0, 0, NULL));
}

void ServerPacketProcess::onSetSelfInfoNotify(ServerHead& srvHead, ByteBuf& byteBuf)
{
	core::SetSelfInfoEvent* pEvent = new core::SetSelfInfoEvent();

	SetSelfInfoNotifyPacket packet(byteBuf, *pEvent);
	packet.Unpack();

	std::map<ENM_InfoType, uint8>::iterator it = pEvent->uint8Map.find(INFOTYPE_VIP_LEVEL);
	if(it != pEvent->uint8Map.end())
	{
		CoreModule::GetCurInfo().enmVIPLevel = (ENM_VipLevel)it->second;
		m_pCoreModule->GetModuleManager()->PushMessage(
			MakeMessage<MODULE_ID_CORE>()(MESSAGE_VALUE_CORE_MY_VIPLEVEL_UPDATE));
	}

	it = pEvent->uint8Map.find(INFOTYPE_RICH_LEVEL);
	if (it != pEvent->uint8Map.end())
	{
		CoreModule::GetCurInfo().unRichLevel = it->second;
		m_pCoreModule->GetModuleManager()->PushMessage(
			MakeMessage<MODULE_ID_CORE>()(MESSAGE_VALUE_CORE_MY_RICHLEVEL_UPDATE));
	}

	//m_pCoreModule->GetModuleManager()->PushEvent(
	//	MakeEvent<MODULE_ID_CORE>()(EVENT_VALUE_CORE_SET_SELFINFO_NOTIFY, 
	//	MODULE_ID_HALL, 0, 0, 0, pEvent));

	//m_pCoreModule->GetModuleManager()->PushMessage(
	//	MakeMessage<MODULE_ID_CORE>()(MESSAGE_VALUE_CORE_MY_INFO_UPDATE));
}

void ServerPacketProcess::onKicked(ServerHead& srvHead, ByteBuf& byteBuf)
{
	core::BeKickedNotifyEvent* pEvent = new core::BeKickedNotifyEvent();

	BeKickedPacket packet(byteBuf, *pEvent);
	packet.Unpack();

	m_pCoreModule->AsynLogout();

	m_pCoreModule->GetModuleManager()->PushMessage(
		MakeMessage<MODULE_ID_CORE>()(MESSAGE_VALUE_CORE_BE_KICKED));

	m_pCoreModule->GetModuleManager()->PushEvent(
		MakeEvent<MODULE_ID_CORE>()(EVENT_VALUE_CORE_BE_KICKED_NOTIFY, 
		MODULE_ID_HALL, 0, 0, 0, pEvent));

	if(pEvent->result == core::BeKickedNotifyEvent::KICK_SEND_AD ||
		pEvent->result == core::BeKickedNotifyEvent::KICK_ACCOUNT_LOCK ||
		pEvent->result == core::BeKickedNotifyEvent::KICK_IP_LOCK ||
		pEvent->result == core::BeKickedNotifyEvent::KICK_MAC_LOCK)
	{
		if(CoreModule::GetCurInfo().enmVIPLevel < core::VIPLEVEL_SILVER)
		{
			BlacklistHelper::TickCount();
		}
	}
}

void ServerPacketProcess::onNotifyMessage(ServerHead& srvHead, ByteBuf& byteBuf)
{	
	core::NotifySystemMessageEvent* pEvent = new core::NotifySystemMessageEvent();

	SystemMessagePacket packet(byteBuf, *pEvent);
	packet.Unpack();

	m_pCoreModule->GetModuleManager()->PushEvent(
		MakeEvent<MODULE_ID_CORE>()(EVENT_VALUE_CORE_NOTIFY_SYSTEM_MESSAGE, 
		MODULE_ID_HALL, 0, 0, 0, pEvent));
}

void ServerPacketProcess::onNotifyPasswordSafe(ServerHead&, ByteBuf&)
{
	m_pCoreModule->GetModuleManager()->PushEvent(
		MakeEvent<MODULE_ID_CORE>()(EVENT_VALUE_CORE_NOTIFY_PASSWORD_SAFE, 
		MODULE_ID_HALL, 0, 0, 0, NULL));
}

void ServerPacketProcess::onMyInfoUpdateNotify(ServerHead& srvHead, ByteBuf& byteBuf)
{
	NotifyMyInfoUpdatePacket packet(byteBuf);
	packet.Unpack();

	CoreModule::GetCurInfo().bMan = packet.bMan;
	CoreModule::GetCurInfo().strName = packet.str179Name;

	m_pCoreModule->GetModuleManager()->PushEvent(
		MakeEvent<MODULE_ID_CORE>()(EVENT_VALUE_CORE_NOTIFY_MY_INFO_UPDATE, 
		MODULE_ID_HALL, 0, 0, 0, NULL));
}

void ServerPacketProcess::onGiftStarNotify(ServerHead& srvHead, ByteBuf& byteBuf)
{
	NotifyGiftStarPacket packet(byteBuf);
	packet.Unpack();

	m_pCoreModule->OnGetGiftStar(packet.mapUIN2GiftStar);

	m_pCoreModule->GetModuleManager()->PushMessage(
		MakeMessage<MODULE_ID_CORE>()(MESSAGE_VALUE_CORE_GIFT_STAR_INFO));
}

void ServerPacketProcess::onMyGoodsNotify(ServerHead& srvHead, ByteBuf& byteBuf)
{
	NotifyMyGoodsPacket packet(byteBuf);
	packet.Unpack();

	m_pCoreModule->OnGetMyGoodsChange(packet.bInit, packet.mapItem2Count);

	if(packet.bInit)
	{
		m_pCoreModule->GetModuleManager()->PushMessage(
			MakeMessage<MODULE_ID_CORE>()(MESSAGE_VALUE_CORE_MY_GODS));
	}
	else
	{
		m_pCoreModule->GetModuleManager()->PushMessage(
			MakeMessage<MODULE_ID_CORE>()(MESSAGE_VALUE_CORE_MY_GODS_UPDATE));
	}
}

void ServerPacketProcess::onRichStarNotify(ServerHead& srvHead, ByteBuf& byteBuf)
{
	core::NotifyRichStarEvent* pEvent = new core::NotifyRichStarEvent();

	NotifyRichStarPacket packet(byteBuf, *pEvent);
	packet.Unpack();

	pEvent->unRoomID = srvHead.GetRoomId();

	m_pCoreModule->GetModuleManager()->PushEvent(
		MakeEvent<MODULE_ID_CORE>()(EVENT_VALUE_CORE_NOTIFY_RICH_STAR, 
		MODULE_ID_ROOM, 0, 0, 0, pEvent));
}

void ServerPacketProcess::onRoomLoginRsp(ServerHead& srvHead, ByteBuf& byteBuf)
{
	core::EnterRoomRspEvent* pEvent = new core::EnterRoomRspEvent();

	EnterRoomRspPacket packet(byteBuf, *pEvent);
	packet.Unpack();

	pEvent->unRoomID = srvHead.GetRoomId();

	m_pCoreModule->GetModuleManager()->PushEvent(
		MakeEvent<MODULE_ID_CORE>()(EVENT_VALUE_CORE_ENTER_ROOM_RSP, 
		MODULE_ID_HALL, 0, 0, 0, pEvent));
}

void ServerPacketProcess::onRoomLoginNotify(ServerHead& srvHead, ByteBuf& byteBuf)
{
	core::EnterRoomNotifyEvent* pEvent = new core::EnterRoomNotifyEvent();

	EnterRoomNotifyPacket packet(byteBuf, *pEvent);
	packet.Unpack();

	pEvent->unRoomID = srvHead.GetRoomId();

	m_pCoreModule->GetModuleManager()->PushEvent(
		MakeEvent<MODULE_ID_CORE>()(EVENT_VALUE_CORE_ENTER_ROOM_NOTIFY, 
		MODULE_ID_ROOM, 0, 0, 0, pEvent));
}

void ServerPacketProcess::onRoomLogoutNotify(ServerHead& srvHead, ByteBuf& byteBuf)
{
	LeaveRoomNotifyPacket packet(byteBuf);
	packet.Unpack();

	m_pCoreModule->GetModuleManager()->PushEvent(
		MakeEvent<MODULE_ID_CORE>()(EVENT_VALUE_CORE_LEAVE_ROOM_NOTIFY, 
		MODULE_ID_ROOM, packet.uin, srvHead.GetRoomId(), 0, NULL));
}

void ServerPacketProcess::onGetUserListRsp(ServerHead& srvHead, ByteBuf& byteBuf)
{
	core::GetUserListRspEvent* pEvent = new core::GetUserListRspEvent();

	GetUserListRspPacket packet(byteBuf, *pEvent);
	packet.Unpack();

	pEvent->roomid = srvHead.GetRoomId();

	m_pCoreModule->GetModuleManager()->PushEvent(
		MakeEvent<MODULE_ID_CORE>()(EVENT_VALUE_CORE_GET_USERLIST_RSP, 
		MODULE_ID_ROOM, 0, 0, 0, pEvent));
}

void ServerPacketProcess::onGetWaittingListRsp(ServerHead& srvHead, ByteBuf& byteBuf)
{
	core::GetWaittinglistRspEvent* pEvent = new core::GetWaittinglistRspEvent();

	GetWaittingListRspPacket packet(byteBuf, *pEvent);
	packet.Unpack();

	pEvent->roomid = srvHead.GetRoomId();

	m_pCoreModule->GetModuleManager()->PushEvent(
		MakeEvent<MODULE_ID_CORE>()(EVENT_VALUE_CORE_GET_WAITTINGLIST_RSP, 
		MODULE_ID_ROOM, 0, 0, 0, pEvent));
}

void ServerPacketProcess::onSendMessageRsp(ServerHead& srvHead, ByteBuf& byteBuf)
{
	core::SendMessageRspEvent* pEvent = new core::SendMessageRspEvent();

	SendMessageRspPacket packet(byteBuf, *pEvent);
	packet.Unpack();

	pEvent->roomid = srvHead.GetRoomId();

	m_pCoreModule->GetModuleManager()->PushEvent(
		MakeEvent<MODULE_ID_CORE>()(EVENT_VALUE_CORE_SEND_MESSAGE_RSP, 
		MODULE_ID_ROOM, 0, 0, 0, pEvent));
}

void ServerPacketProcess::onSendMessageNotify(ServerHead& srvHead, ByteBuf& byteBuf)
{
	core::SendMessageNotifyEvent* pEvent = new core::SendMessageNotifyEvent();

	SendMessageNotifyPacket packet(byteBuf, *pEvent);
	packet.Unpack();

	pEvent->roomid = srvHead.GetRoomId();

	m_pCoreModule->GetModuleManager()->PushEvent(
		MakeEvent<MODULE_ID_CORE>()(EVENT_VALUE_CORE_SEND_MESSAGE_NOTIFY, 
		MODULE_ID_ROOM, 0, 0, 0, pEvent));
}

void ServerPacketProcess::onSendGiftRsp(ServerHead& srvHead, ByteBuf& byteBuf)
{
	core::SendGiftRspEvent* pEvent = new core::SendGiftRspEvent();

	SendGiftRspPacket packet(byteBuf, *pEvent, srvHead.GetSeq());
	packet.Unpack();

	pEvent->unRoomID = srvHead.GetRoomId();

	if (pEvent->enmResult == core::SENDGIFTRESULT_OK)
	{		
		CoreModule::GetCurInfo().unRichUpdateMoney = pEvent->unRichUpdateMoney;

		m_pCoreModule->GetModuleManager()->PushEvent(
			MakeEvent<MODULE_ID_CORE>()(EVENT_VALUE_CORE_NOTIFY_RICH_UPDATE_MONEY_CHANGE,
			MODULE_ID_HALL, 0, 0, 0, NULL));
	}

	m_pCoreModule->GetModuleManager()->PushEvent(
		MakeEvent<MODULE_ID_CORE>()(EVENT_VALUE_CORE_SEND_GIFT_RSP, 
		MODULE_ID_ROOM, 0, 0, 0, pEvent));
}

void ServerPacketProcess::onSendGiftNotify(ServerHead& srvHead, ByteBuf& byteBuf)
{
	core::SendGiftNotifyEvent* pEvent = new core::SendGiftNotifyEvent();

	SendGiftNotifyPacket packet(byteBuf, *pEvent);
	packet.Unpack();

	pEvent->unRoomID = srvHead.GetRoomId();

	m_pCoreModule->GetModuleManager()->PushEvent(
		MakeEvent<MODULE_ID_CORE>()(EVENT_VALUE_CORE_SEND_GIFT_NOTIFY, 
		MODULE_ID_ROOM, 0, 0, 0, pEvent));
}

void ServerPacketProcess::onSendLoudSpeakerRsp(ServerHead& srvHead, ByteBuf& byteBuf)
{
	core::SendLoudSpeakerRspEvent* pEvent = new core::SendLoudSpeakerRspEvent();

	SendLoudSpeakerRspPacket packet(byteBuf, *pEvent, srvHead.GetSeq());
	packet.Unpack();

	pEvent->unRoomID = srvHead.GetRoomId();

	if (pEvent->enmResult == core::SENDGIFTRESULT_OK)
	{		
		CoreModule::GetCurInfo().unRichUpdateMoney = pEvent->unRichUpdateMoney;

		m_pCoreModule->GetModuleManager()->PushEvent(
			MakeEvent<MODULE_ID_CORE>()(EVENT_VALUE_CORE_NOTIFY_RICH_UPDATE_MONEY_CHANGE,
			MODULE_ID_HALL, 0, 0, 0, NULL));
	}

	m_pCoreModule->GetModuleManager()->PushEvent(
		MakeEvent<MODULE_ID_CORE>()(EVENT_VALUE_CORE_SEND_LOUDSPEAKER_RSP, 
		MODULE_ID_ROOM, 0, 0, 0, pEvent));
}

void ServerPacketProcess::onSendLoudSpeakerNotify(ServerHead& srvHead, ByteBuf& byteBuf)
{
	core::SendLoudSpeakerNotifyEvent* pEvent = new core::SendLoudSpeakerNotifyEvent();

	SendLoudSpeakerNotifyPacket packet(byteBuf, *pEvent);
	packet.Unpack();

	pEvent->unRoomID = srvHead.GetRoomId();

	m_pCoreModule->GetModuleManager()->PushEvent(
		MakeEvent<MODULE_ID_CORE>()(EVENT_VALUE_CORE_SEND_LOUDSPEAKER_NOTIFY, 
		MODULE_ID_ROOM, 0, 0, 0, pEvent));
}

void ServerPacketProcess::onRequestShowRsp(ServerHead& srvHead, ByteBuf& byteBuf)
{
	core::RequestShowRspEvent* pEvent = new core::RequestShowRspEvent();

	RequestShowRspPacket packet(byteBuf, *pEvent);
	packet.Unpack();

	pEvent->unRoomID = srvHead.GetRoomId();

	m_pCoreModule->GetModuleManager()->PushEvent(
		MakeEvent<MODULE_ID_CORE>()(EVENT_VALUE_CORE_REQUEST_SHOW_RSP, 
		MODULE_ID_ROOM, 0, 0, 0, pEvent));
}

void ServerPacketProcess::onRequestShowNotify(ServerHead& srvHead, ByteBuf& byteBuf)
{
	core::RequestShowNotifyEvent* pEvent = new core::RequestShowNotifyEvent();

	RequestShowNotifyPacket packet(byteBuf, *pEvent);
	packet.Unpack();

	pEvent->unRoomID = srvHead.GetRoomId();

	m_pCoreModule->GetModuleManager()->PushEvent(
		MakeEvent<MODULE_ID_CORE>()(EVENT_VALUE_CORE_REQUEST_SHOW_NOTIFY, 
		MODULE_ID_ROOM, 0, 0, 0, pEvent));
}

void ServerPacketProcess::onExitShowNotify(ServerHead& srvHead, ByteBuf& byteBuf)
{
	ExitShowNotifyPacket packet(byteBuf);
	packet.Unpack();

	m_pCoreModule->GetModuleManager()->PushEvent(
		MakeEvent<MODULE_ID_CORE>()(EVENT_VALUE_CORE_EXIT_SHOW_NOTIFY, 
		MODULE_ID_ROOM, srvHead.GetRoomId(), packet.unSrcUIN, packet.type, NULL));
}

void ServerPacketProcess::onOperateAVRsp(ServerHead& srvHead, ByteBuf& byteBuf)
{
	core::OperateVideoAudioRspEvent* pEvent = new core::OperateVideoAudioRspEvent();

	OperateAVRspPacket packet(byteBuf, srvHead.GetSeq(), *pEvent);
	packet.Unpack();

	pEvent->unRoomID = srvHead.GetRoomId();

	m_pCoreModule->GetModuleManager()->PushEvent(
		MakeEvent<MODULE_ID_CORE>()(EVENT_VALUE_CORE_OPERATE_VIDEO_AUDIO_RSP, 
		MODULE_ID_ROOM, 0, 0, 0, pEvent));
}

void ServerPacketProcess::onOperateAVNotify(ServerHead& srvHead, ByteBuf& byteBuf)
{
	OperateAVNotifyPacket packet(byteBuf);
	packet.Unpack();

	m_pCoreModule->GetModuleManager()->PushEvent(
		MakeEvent<MODULE_ID_CORE>()(EVENT_VALUE_CORE_OPERATE_VIDEO_AUDIO_NOTIFY, 
		MODULE_ID_ROOM, srvHead.GetRoomId(), packet.enmOperateType, packet.srcUIN, NULL));
}

void ServerPacketProcess::onKickUserRsp(ServerHead& srvHead, ByteBuf& byteBuf)
{
	core::KickUserRspEvent* pEvent = new core::KickUserRspEvent();

	KickUserRspPacket packet(byteBuf, srvHead.GetSeq(), *pEvent);
	packet.Unpack();

	pEvent->unRoomID = srvHead.GetRoomId();

	m_pCoreModule->GetModuleManager()->PushEvent(
		MakeEvent<MODULE_ID_CORE>()(EVENT_VALUE_CORE_KICK_USER_RSP, 
		MODULE_ID_ROOM, 0, 0, 0, pEvent));
}

void ServerPacketProcess::onKickUserNotify(ServerHead& srvHead, ByteBuf& byteBuf)
{
	core::KickUserNotifyEvent* pEvent = new core::KickUserNotifyEvent();

	KickUserNotifyPacket packet(byteBuf, *pEvent);
	packet.Unpack();

	pEvent->unRoomID = srvHead.GetRoomId();

	m_pCoreModule->GetModuleManager()->PushEvent(
		MakeEvent<MODULE_ID_CORE>()(EVENT_VALUE_CORE_KICK_USER_NOTIFY, 
		MODULE_ID_ROOM, 0, 0, 0, pEvent));
}

void ServerPacketProcess::onAddBlackRsp(ServerHead& srvHead, ByteBuf& byteBuf)
{
	AddBlackRspPacket packet(byteBuf, srvHead.GetSeq());
	packet.Unpack();

	m_pCoreModule->GetModuleManager()->PushEvent(
		MakeEvent<MODULE_ID_CORE>()(EVENT_VALUE_CORE_ADD_BLACK_RSP, 
		MODULE_ID_ROOM, srvHead.GetRoomId(), packet.enmResult, packet.unDstUIN, NULL));
}

void ServerPacketProcess::onAddBlackNotify(ServerHead& srvHead, ByteBuf& byteBuf)
{
	AddBlackNotifyPacket packet(byteBuf);
	packet.Unpack();

	m_pCoreModule->GetModuleManager()->PushEvent(
		MakeEvent<MODULE_ID_CORE>()(EVENT_VALUE_CORE_ADD_BLACK_NOTIFY, 
		MODULE_ID_ROOM, srvHead.GetRoomId(), packet.srcUIN, packet.dstUIN, NULL));
}

void ServerPacketProcess::onLockIPRsp(ServerHead& srvHead, ByteBuf& byteBuf)
{
	LockIPRspPacket packet(byteBuf, srvHead.GetSeq());
	packet.Unpack();

	m_pCoreModule->GetModuleManager()->PushEvent(
		MakeEvent<MODULE_ID_CORE>()(EVENT_VALUE_CORE_LOCK_IP_RSP, 
		MODULE_ID_ROOM, srvHead.GetRoomId(), packet.enmResult, packet.unDstUIN, NULL));
}

void ServerPacketProcess::onLockIPNotify(ServerHead& srvHead, ByteBuf& byteBuf)
{
	core::LockIPNotifyEvent* pEvent = new core::LockIPNotifyEvent();

	LockIPNotifyPacket packet(byteBuf, *pEvent);
	packet.Unpack();

	pEvent->unRoomID = srvHead.GetRoomId();

	m_pCoreModule->GetModuleManager()->PushEvent(
		MakeEvent<MODULE_ID_CORE>()(EVENT_VALUE_CORE_LOCK_IP_NOTIFY, 
		MODULE_ID_ROOM, 0, 0, 0, pEvent));
}

void ServerPacketProcess::onShutUpRsp(ServerHead& srvHead, ByteBuf& byteBuf)
{
	ShutUpRspPacket packet(byteBuf, srvHead.GetSeq());
	packet.Unpack();

	m_pCoreModule->GetModuleManager()->PushEvent(
		MakeEvent<MODULE_ID_CORE>()(EVENT_VALUE_CORE_SHUT_UP_RSP, 
		MODULE_ID_ROOM, srvHead.GetRoomId(), packet.enmResult, packet.unDstUIN, NULL));
}

void ServerPacketProcess::onShutUpNotify(ServerHead& srvHead, ByteBuf& byteBuf)
{
	core::ShutUpNotifyEvent* pEvent = new core::ShutUpNotifyEvent() ;
	ShutUpNotifyPacket packet(byteBuf ,*pEvent);
	packet.Unpack();

	pEvent->unRoomID = srvHead.GetRoomId() ;

	m_pCoreModule->GetModuleManager()->PushEvent(
		MakeEvent<MODULE_ID_CORE>()(EVENT_VALUE_CORE_SHUT_UP_NOTIFY, 
		MODULE_ID_ROOM, 0 , 0 , 0 , pEvent ) );
}

void ServerPacketProcess::onUnlockShutUpRsp(ServerHead& srvHead, ByteBuf& byteBuf)
{
	UnShutUpRspPacket packet(byteBuf,srvHead.GetSeq());
	packet.Unpack();

	m_pCoreModule->GetModuleManager()->PushEvent(
		MakeEvent<MODULE_ID_CORE>()(EVENT_VALUE_CORE_UNLOCK_SHUTUP_RSP, 
		MODULE_ID_ROOM, srvHead.GetRoomId(), packet.enmResult, packet.unDstUIN, NULL));
}

void ServerPacketProcess::onUnLockShutUpNotify(ServerHead& srvHead, ByteBuf& byteBuf)
{
	UnShutUpNotifyPacket packet(byteBuf);
	packet.Unpack();

	m_pCoreModule->GetModuleManager()->PushEvent(
		MakeEvent<MODULE_ID_CORE>()(EVENT_VALUE_CORE_UNLOCK_SHUTUP_NOTIFY, 
		MODULE_ID_ROOM, srvHead.GetRoomId(), packet.srcUIN, packet.dstUIN, NULL));
}

void ServerPacketProcess::onPushOnShowRsp(ServerHead& srvHead, ByteBuf& byteBuf)
{
	core::PushOnShowRspEvent* pEvent = new core::PushOnShowRspEvent();

	PushOnShowRspPacket packet(byteBuf, srvHead.GetSeq(), *pEvent);
	packet.Unpack();

	pEvent->unRoomID = srvHead.GetRoomId();

	m_pCoreModule->GetModuleManager()->PushEvent(
		MakeEvent<MODULE_ID_CORE>()(EVENT_VALUE_CORE_PUSH_ON_SHOW_RSP, 
		MODULE_ID_ROOM, 0, 0, 0, pEvent));
}

void ServerPacketProcess::onPushOnShowNotify(ServerHead& srvHead, ByteBuf& byteBuf)
{
	core::PushOnShowNotifyEvent* pEvent = new core::PushOnShowNotifyEvent();

	PushOnShowNotifyPacket packet(byteBuf, *pEvent);
	packet.Unpack();

	pEvent->unRoomID = srvHead.GetRoomId();

	m_pCoreModule->GetModuleManager()->PushEvent(
		MakeEvent<MODULE_ID_CORE>()(EVENT_VALUE_CORE_PUSH_ON_SHOW_NOTIFY, 
		MODULE_ID_ROOM, 0, 0, 0, pEvent));
}

void ServerPacketProcess::onPullOffShowRsp(ServerHead& srvHead, ByteBuf& byteBuf)
{
	PullOffShowRspPacket packet(byteBuf, srvHead.GetSeq());
	packet.Unpack();

	m_pCoreModule->GetModuleManager()->PushEvent(
		MakeEvent<MODULE_ID_CORE>()(EVENT_VALUE_CORE_PULL_OFF_SHOW_RSP, 
		MODULE_ID_ROOM, srvHead.GetRoomId(), packet.enmResult, packet.unDstUIN, NULL));
}

void ServerPacketProcess::onPullOffShowNotify(ServerHead& srvHead, ByteBuf& byteBuf)
{
	PullOffShowNotifyPacket packet(byteBuf);
	packet.Unpack();

	m_pCoreModule->GetModuleManager()->PushEvent(
		MakeEvent<MODULE_ID_CORE>()(EVENT_VALUE_CORE_PULL_OFF_SHOW_NOTIFY, 
		MODULE_ID_ROOM, srvHead.GetRoomId(), packet.srcUIN, packet.dstUIN, NULL));
}

void ServerPacketProcess::onAddTitleRsp(ServerHead& srvHead, ByteBuf& byteBuf)
{
	core::AddTitleRspEvent* pEvent = new core::AddTitleRspEvent();

	AddTitleRspPacket packet(byteBuf, *pEvent, srvHead.GetSeq());
	packet.Unpack();

	pEvent->unRoomID = srvHead.GetRoomId();

	m_pCoreModule->GetModuleManager()->PushEvent(
		MakeEvent<MODULE_ID_CORE>()(EVENT_VALUE_CORE_ADD_TITLE_RSP, 
		MODULE_ID_ROOM, 0, 0, 0, pEvent));
}

void ServerPacketProcess::onAddTitleNotify(ServerHead& srvHead, ByteBuf& byteBuf)
{
	core::AddTitleNotifyEvent* pEvent = new core::AddTitleNotifyEvent();

	AddTitleNotifyPacket packet(byteBuf, *pEvent);
	packet.Unpack();

	pEvent->unRoomID = srvHead.GetRoomId();

	m_pCoreModule->GetModuleManager()->PushEvent(
		MakeEvent<MODULE_ID_CORE>()(EVENT_VALUE_CORE_ADD_TITLE_NOTIFY, 
		MODULE_ID_ROOM, 0, 0, 0, pEvent));
}

void ServerPacketProcess::onRemoveTitleRsp(ServerHead& srvHead, ByteBuf& byteBuf)
{
	core::RemoveTitleRspEvent* pEvent = new core::RemoveTitleRspEvent();

	RemoveTitleRspPacket packet(byteBuf, *pEvent, srvHead.GetSeq());
	packet.Unpack();

	pEvent->unRoomID = srvHead.GetRoomId();

	m_pCoreModule->GetModuleManager()->PushEvent(
		MakeEvent<MODULE_ID_CORE>()(EVENT_VALUE_CORE_REMOVE_TITLE_RSP, 
		MODULE_ID_ROOM, 0, 0, 0, pEvent));
}

void ServerPacketProcess::onRemoveTitleNotify(ServerHead& srvHead, ByteBuf& byteBuf)
{
	core::RemoveTitleNotifyEvent* pEvent = new core::RemoveTitleNotifyEvent();

	RemoveTitleNotifyPacket packet(byteBuf, *pEvent);
	packet.Unpack();

	pEvent->unRoomID = srvHead.GetRoomId();

	m_pCoreModule->GetModuleManager()->PushEvent(
		MakeEvent<MODULE_ID_CORE>()(EVENT_VALUE_CORE_REMOVE_TITLE_NOTIFY, 
		MODULE_ID_ROOM, 0, 0, 0, pEvent));
}

void ServerPacketProcess::onOpenPrivateRsp(ServerHead& srvHead, ByteBuf& byteBuf)
{
	OpenPrivateRspPacket packet(byteBuf, srvHead.GetSeq());
	packet.Unpack();

	m_pCoreModule->GetModuleManager()->PushEvent(
		MakeEvent<MODULE_ID_CORE>()(EVENT_VALUE_CORE_OPEN_PRIVATE_RSP, 
		MODULE_ID_ROOM, srvHead.GetRoomId(), packet.enmResult, packet.unDstUIN, NULL));
}

void ServerPacketProcess::onRequestP2PRsp(ServerHead& srvHead, ByteBuf& byteBuf)
{
	RequestP2PRspPacket packet(byteBuf, srvHead.GetSeq());
	packet.Unpack();

	m_pCoreModule->GetModuleManager()->PushEvent(
		MakeEvent<MODULE_ID_CORE>()(EVENT_VALUE_CORE_REQUEST_P2P_RSP, 
		MODULE_ID_ROOM, srvHead.GetRoomId(), packet.enmResult, packet.unDstUIN, NULL));
}

void ServerPacketProcess::onRequestP2PNotify(ServerHead& srvHead, ByteBuf& byteBuf)
{
	RequestP2PNotifyPacket packet(byteBuf);
	packet.Unpack();

	m_pCoreModule->GetModuleManager()->PushEvent(
		MakeEvent<MODULE_ID_CORE>()(EVENT_VALUE_CORE_REQUEST_P2P_NOTIFY, 
		MODULE_ID_ROOM, srvHead.GetRoomId(), packet.srcUIN, 0, NULL));
}

void ServerPacketProcess::onReponseP2PRsp(ServerHead& srvHead, ByteBuf& byteBuf)
{
	core::ResponseP2PRspEvent* pEvent = new core::ResponseP2PRspEvent();

	ResponseP2PRspPacket packet(byteBuf, srvHead.GetSeq(), *pEvent);
	packet.Unpack();

	pEvent->unRoomID = srvHead.GetRoomId();

	m_pCoreModule->GetModuleManager()->PushEvent(
		MakeEvent<MODULE_ID_CORE>()(EVENT_VALUE_CORE_RESPONSE_P2P_RSP, 
		MODULE_ID_ROOM, 0, 0, 0, pEvent));
}

void ServerPacketProcess::onResponseP2PNotify(ServerHead& srvHead, ByteBuf& byteBuf)
{
	core::ResponseP2PNotifyEvent* pEvent = new core::ResponseP2PNotifyEvent();

	ResponseP2PNotifyPacket packet(byteBuf, *pEvent);
	packet.Unpack();

	pEvent->unRoomID = srvHead.GetRoomId();

	m_pCoreModule->GetModuleManager()->PushEvent(
		MakeEvent<MODULE_ID_CORE>()(EVENT_VALUE_CORE_RESPONSE_P2P_NOTIFY, 
		MODULE_ID_ROOM, srvHead.GetRoomId(), 0, 0, pEvent));
}

void ServerPacketProcess::onReportClientInfoNotify(ServerHead& srvHead, ByteBuf& byteBuf)
{
	core::ReportClientInfoNotifyEvent* pEvent = new core::ReportClientInfoNotifyEvent();

	ReportClientInfoNotifyPacket packet(byteBuf, *pEvent);
	packet.Unpack();

	pEvent->unRoomID = srvHead.GetRoomId();

	m_pCoreModule->GetModuleManager()->PushEvent(
		MakeEvent<MODULE_ID_CORE>()(EVENT_VALUE_CORE_REPORT_CLIENTINFO_NOTIFY, 
		MODULE_ID_ROOM, 0, 0, 0, pEvent));
}

void ServerPacketProcess::onGetClientInfoRsp(ServerHead& srvHead, ByteBuf& byteBuf)
{
	core::QueryClientInfoRspEvent* pEvent = new core::QueryClientInfoRspEvent();

	QueryClientInfoRspPacket packet(byteBuf, *pEvent);
	packet.Unpack();

	pEvent->unRoomID = srvHead.GetRoomId();

	m_pCoreModule->GetModuleManager()->PushEvent(
		MakeEvent<MODULE_ID_CORE>()(EVENT_VALUE_CORE_QUERY_CLIENTINFO_RSP, 
		MODULE_ID_ROOM, 0, 0, 0, pEvent));
}

void ServerPacketProcess::onSetRoomInfoRsp(ServerHead& srvHead, ByteBuf& byteBuf)
{
	SetRoomInfoRspPacket packet(byteBuf);
	packet.Unpack();

	m_pCoreModule->GetModuleManager()->PushEvent(
		MakeEvent<MODULE_ID_CORE>()(EVENT_VALUE_CORE_SET_ROOMINFO_RSP, 
		MODULE_ID_ROOM, srvHead.GetRoomId(), packet.enmResult, 0, NULL));
}

void ServerPacketProcess::onSetRoomInfoNotify(ServerHead& srvHead, ByteBuf& byteBuf)
{
	core::SetRoomInfoNotifyEvent* pEvent = new core::SetRoomInfoNotifyEvent();

	SetRoomInfoNotifyPacket packet(byteBuf, *pEvent);
	packet.Unpack();

	pEvent->unRoomID = srvHead.GetRoomId();

	m_pCoreModule->GetModuleManager()->PushEvent(
		MakeEvent<MODULE_ID_CORE>()(EVENT_VALUE_CORE_SET_ROOMINFO_NOTIFY, 
		MODULE_ID_ROOM, 0, 0, 0, pEvent));
}

void ServerPacketProcess::onGetRoomInfoRsp(ServerHead& srvHead, ByteBuf& byteBuf)
{
	core::GetRoomInfoRspEvent* pEvent = new core::GetRoomInfoRspEvent();

	GetRoomInfoRspPacket packet(byteBuf, *pEvent);
	packet.Unpack();

	pEvent->unRoomID = srvHead.GetRoomId();

	m_pCoreModule->GetModuleManager()->PushEvent(
		MakeEvent<MODULE_ID_CORE>()(EVENT_VALUE_CORE_GET_ROOMINFO_RSP, 
		MODULE_ID_HALL, 0, 0, 0, pEvent));
}

void ServerPacketProcess::onUserInfoChange(ServerHead& srvHead, ByteBuf& byteBuf)
{
	core::NotifyUserInfoUpdateEvent* pEvent = new core::NotifyUserInfoUpdateEvent();

	UserInfoUpdatePacket packet(byteBuf, *pEvent);
	packet.Unpack();

	pEvent->unRoomID = srvHead.GetRoomId();

	m_pCoreModule->GetModuleManager()->PushEvent(
		MakeEvent<MODULE_ID_CORE>()(EVENT_VALUE_CORE_NOTIFY_ROOM_USER_INFO_UPDATE, 
		MODULE_ID_ROOM, 0, 0, 0, pEvent));
}

void ServerPacketProcess::onPrivateShowList(ServerHead& srvHead, ByteBuf& byteBuf)
{
	core::NotifyPrivateShowEvent* pEvent = new core::NotifyPrivateShowEvent();

	NotifyPrivateShowPacket packet(byteBuf, *pEvent);
	packet.Unpack();

	pEvent->unRoomID = srvHead.GetRoomId();

	m_pCoreModule->GetModuleManager()->PushEvent(
		MakeEvent<MODULE_ID_CORE>()(EVENT_VALUE_CORE_NOTIFY_PRIVATE_SHOW, 
		MODULE_ID_ROOM, 0, 0, 0, pEvent));
}

void ServerPacketProcess::onGiftOverTime(ServerHead& srvHead, ByteBuf& byteBuf)
{
	GiftOverTimePacket packet(byteBuf);
	packet.Unpack();

	for(std::vector<uint16>::iterator it = packet.vecGiftID.begin(); 
		it != packet.vecGiftID.end(); ++it)
	{
		m_pCoreModule->GetModuleManager()->PushEvent(
			MakeEvent<MODULE_ID_CORE>()(EVENT_VALUE_CORE_NOTIFY_GIFT_OVER_TIME, 
			MODULE_ID_ROOM, srvHead.GetRoomId(), packet.unUIN, *it, NULL));
	}	
}

void ServerPacketProcess::onMoneyUpdate(ServerHead& srvHead, ByteBuf& byteBuf)
{
	NotifyMoneyUpdatePacket packet(byteBuf);
	packet.Unpack();

	CurInfo& curInfo = CoreModule::GetCurInfo();
	curInfo.unLeftMoney = packet.unMoney;
	curInfo.unRichUpdateMoney = packet.unRichUpdateMoney;

	m_pCoreModule->GetModuleManager()->PushEvent(
		MakeEvent<MODULE_ID_CORE>()(EVENT_VALUE_CORE_NOTIFY_RICH_UPDATE_MONEY_CHANGE,
		MODULE_ID_HALL, 0, 0, 0, NULL));

	m_pCoreModule->GetModuleManager()->PushEvent(
		MakeEvent<MODULE_ID_CORE>()(EVENT_VALUE_CORE_NOTIFY_MONEY_UPDATE, 
		MODULE_ID_ROOM, packet.unMoney, 0, 0, NULL));
}

void ServerPacketProcess::onOfficialNotice(ServerHead& srvHead, ByteBuf& byteBuf)
{
	OfficialNoticePacket packet(byteBuf);
	packet.Unpack();

	NotifyOfficialNoticeEvent* pEvent = new core::NotifyOfficialNoticeEvent();
	pEvent->unRoomID = srvHead.GetRoomId();
	pEvent->strMessage = packet.strMessage;

	m_pCoreModule->GetModuleManager()->PushEvent(
		MakeEvent<MODULE_ID_CORE>()(EVENT_VALUE_CORE_NOTIFY_OFFICIAL_NOTICE, 
		MODULE_ID_ROOM, 0, 0, 0, pEvent));
}

void ServerPacketProcess::onPlutusAward(ServerHead& srvHead, ByteBuf& byteBuf)
{
	NotifyPlutusAwardEvent* pEvent = new core::NotifyPlutusAwardEvent();

	NotifyPlutusAwardPacket packet(byteBuf, *pEvent);
	packet.Unpack();
	
	pEvent->unRoomID = srvHead.GetRoomId();

	m_pCoreModule->GetModuleManager()->PushEvent(
		MakeEvent<MODULE_ID_CORE>()(EVENT_VALUE_CORE_NOTIFY_PLUTUS_AWARD, 
		MODULE_ID_ROOM, 0, 0, 0, pEvent));
}

void ServerPacketProcess::onRoomLocked(ServerHead& srvHead, ByteBuf& byteBuf)
{
	NotifyRoomLockedEvent* pEvent = new core::NotifyRoomLockedEvent();

	NotifyRoomLockedPacket packet(byteBuf, *pEvent);
	packet.Unpack();

	pEvent->unRoomID = srvHead.GetRoomId();

	m_pCoreModule->GetModuleManager()->PushEvent(
		MakeEvent<MODULE_ID_CORE>()(EVENT_VALUE_CORE_NOTIFY_ROOM_LOCKED, 
		MODULE_ID_ROOM, 0, 0, 0, pEvent));
}

void ServerPacketProcess::onSetMicStatusRsp(ServerHead& srvHead, ByteBuf& byteBuf)
{
	SetMicStatusRspPacket packet(byteBuf, srvHead.GetSeq());
	packet.Unpack();

	m_pCoreModule->GetModuleManager()->PushEvent(
		MakeEvent<MODULE_ID_CORE>()(EVENT_VALUE_CORE_MIC_SET_MIC_STATUS_RSP, 
		MODULE_ID_ROOM, srvHead.GetRoomId(), MAKELPARAM(packet.mic_index,packet.mic_status), packet.unResult, NULL));
}

void ServerPacketProcess::onSetMicStatusNotify(ServerHead& srvHead, ByteBuf& byteBuf)
{
	SetMicStatusNotifyPacket packet(byteBuf);
	packet.Unpack();

	m_pCoreModule->GetModuleManager()->PushEvent(
		MakeEvent<MODULE_ID_CORE>()(EVENT_VALUE_CORE_MIC_SET_MIC_STATUS_NOTIFY, 
		MODULE_ID_ROOM, srvHead.GetRoomId(), MAKELPARAM(packet.mic_index,packet.mic_status), packet.unSrcUIN, NULL));
}

void ServerPacketProcess::onSendFlyWordsRsp(ServerHead& srvHead, ByteBuf& byteBuf)
{
	SendFlyWordsRspEvent* pEvent = new core::SendFlyWordsRspEvent();

	SendFlyWordsRspPacket packet(byteBuf, *pEvent, srvHead.GetSeq());
	packet.Unpack();

	pEvent->unRoomID = srvHead.GetRoomId();

	if (pEvent->enmResult == core::SENDGIFTRESULT_OK)
	{		
		CoreModule::GetCurInfo().unRichUpdateMoney = pEvent->unRichUpdateMoney;

		m_pCoreModule->GetModuleManager()->PushEvent(
			MakeEvent<MODULE_ID_CORE>()(EVENT_VALUE_CORE_NOTIFY_RICH_UPDATE_MONEY_CHANGE,
			MODULE_ID_HALL, 0, 0, 0, NULL));
	}

	m_pCoreModule->GetModuleManager()->PushEvent(
		MakeEvent<MODULE_ID_CORE>()(EVENT_VALUE_CORE_SEND_FLYWORDS_RSP, 
		MODULE_ID_ROOM, 0, 0, 0, pEvent));
}

void ServerPacketProcess::onSendFlyWordsNotify(ServerHead& srvHead, ByteBuf& byteBuf)
{
	SendFlyWordsNotifyEvent* pEvent = new core::SendFlyWordsNotifyEvent();

	SendFlyWordsNotifyPacket packet(byteBuf, *pEvent);
	packet.Unpack();

	pEvent->unRoomID = srvHead.GetRoomId();

	m_pCoreModule->GetModuleManager()->PushEvent(
		MakeEvent<MODULE_ID_CORE>()(EVENT_VALUE_CORE_SEND_FLYWORDS_NOTIFY, 
		MODULE_ID_ROOM, 0, 0, 0, pEvent));
}

void ServerPacketProcess::onMediaServerResetNotify(ServerHead& srvHead, ByteBuf& byteBuf)
{
	NotifyMediaServerResetEvent* pEvent = new core::NotifyMediaServerResetEvent();

	NotifyMediaServerResetPacket packet(byteBuf, *pEvent);
	packet.Unpack();

	m_pCoreModule->GetModuleManager()->PushEvent(
		MakeEvent<MODULE_ID_CORE>()(EVENT_VALUE_CORE_NOTIFY_MEDIA_SERVER_RESET, 
		MODULE_ID_ROOM, 0, 0, 0, pEvent));
}

void ServerPacketProcess::onPushOnShowAnswerRsp(ServerHead& srvHead, ByteBuf& byteBuf)
{
	PushOnShowAnswerRspEvent* pEvent = new core::PushOnShowAnswerRspEvent();

	PushOnShowAnswerRspPacket packet(byteBuf, srvHead.GetSeq(), *pEvent);
	packet.Unpack();

	pEvent->unRoomID = srvHead.GetRoomId();

	m_pCoreModule->GetModuleManager()->PushEvent(
		MakeEvent<MODULE_ID_CORE>()(EVENT_VALUE_CORE_ANSWER_PUSH_ON_SHOW_RSP, 
		MODULE_ID_ROOM, 0, 0, 0, pEvent));
}

void ServerPacketProcess::onTaskNotify(ServerHead& srvHead, ByteBuf& byteBuf)
{
	TaskNotifyPacket packet(byteBuf);
	packet.Unpack();

	for(std::map<uint32, uint8>::iterator it = packet.mapID2Stat.begin();
		it != packet.mapID2Stat.end(); ++it)
	{
		if(it->first > TASK_SYSTEM_TASKNUM || it->first == 0)
		{
			assert(0);
			continue;
		}
		CoreModule::GetCurInfo().unarrTaskStat[it->first-1] = it->second;
	}

	m_pCoreModule->GetModuleManager()->PushEvent(
		MakeEvent<MODULE_ID_CORE>()(EVENT_VALUE_CORE_NOTIFY_TASK_INFO_UPDATE, 
		MODULE_ID_ROOM, 0, 0, 0, NULL));
}

void ServerPacketProcess::onGetTaskRewardRsp(ServerHead& srvHead, ByteBuf& byteBuf)
{
	GetTaskRewardRspPacket packet(byteBuf);
	packet.Unpack();
}

void ServerPacketProcess::onGetFansRsp(ServerHead& srvHead, ByteBuf& byteBuf)
{
	GetFansRspEvent* pEvent = new core::GetFansRspEvent();

	GetFansRspPacket packet(byteBuf, srvHead.GetSeq(), *pEvent);
	packet.Unpack();

	m_pCoreModule->GetModuleManager()->PushEvent(
		MakeEvent<MODULE_ID_CORE>()(EVENT_VALUE_CORE_GET_FANS_RSP, 
		MODULE_ID_ROOM, 0, 0, 0, pEvent));
}

void ServerPacketProcess::onBuySofaRsp(ServerHead& srvHead, ByteBuf& byteBuf)
{
	BuySofaRspEvent* pEvent = new core::BuySofaRspEvent();

	BuySofaRspPacket packet(byteBuf, srvHead.GetSeq(), *pEvent);
	packet.Unpack();

	pEvent->roomid = srvHead.GetRoomId();

	if (pEvent->enmResult == core::BuySofaRspEvent::BUYSOFARESULT_OK)
	{		
		CoreModule::GetCurInfo().unRichUpdateMoney = pEvent->nextMagnateLevelSpend;

		m_pCoreModule->GetModuleManager()->PushEvent(
			MakeEvent<MODULE_ID_CORE>()(EVENT_VALUE_CORE_NOTIFY_RICH_UPDATE_MONEY_CHANGE,
			MODULE_ID_HALL, 0, 0, 0, NULL));
	}

	m_pCoreModule->GetModuleManager()->PushEvent(
		MakeEvent<MODULE_ID_CORE>()(EVENT_VALUE_CORE_BUY_SOFA_RSP, 
		MODULE_ID_ROOM, 0, 0, 0, pEvent));
}

void ServerPacketProcess::onBuySofaNotify(ServerHead& srvHead, ByteBuf& byteBuf)
{
	BuySofaNotifyEvent* pEvent = new core::BuySofaNotifyEvent();

	BuySofaNotifyPacket packet(byteBuf, *pEvent);
	packet.Unpack();

	m_pCoreModule->GetModuleManager()->PushEvent(
		MakeEvent<MODULE_ID_CORE>()(EVENT_VALUE_CORE_BUY_SOFA_NOTIFY, 
		MODULE_ID_ROOM, 0, 0, 0, pEvent));
}

void ServerPacketProcess::onClearSofaList(ServerHead& srvHead, ByteBuf& byteBuf)
{
	ClearSofaListNotifyPacket packet(byteBuf);
	packet.Unpack();

	m_pCoreModule->GetModuleManager()->PushEvent(
		MakeEvent<MODULE_ID_CORE>()(EVENT_VALUE_CORE_NOTIFY_CLEAR_SOFA_LIST, 
		MODULE_ID_ROOM, packet.roomID, 0, 0, NULL));
}

void ServerPacketProcess::onGetSofaList(ServerHead& srvHead, ByteBuf& byteBuf)
{
	GetSofaListRspEvent* pEvent = new core::GetSofaListRspEvent();

	GetSofaListRspPacket packet(byteBuf, *pEvent);
	packet.Unpack();

	pEvent->roomID = srvHead.GetRoomId();

	m_pCoreModule->GetModuleManager()->PushEvent(
		MakeEvent<MODULE_ID_CORE>()(EVENT_VALUE_CORE_GET_SOFA_LIST_RSP, 
		MODULE_ID_ROOM, 0, 0, 0, pEvent));
}

void ServerPacketProcess::onUpdateSongList(ServerHead& srvHead, ByteBuf& byteBuf)
{
	UpdateSongListRspEvent* pEvent = new core::UpdateSongListRspEvent();

	UpdateSonglistRspPacket packet(byteBuf, *pEvent);
	packet.Unpack();

	pEvent->roomid = srvHead.GetRoomId();

	m_pCoreModule->GetModuleManager()->PushEvent(
		MakeEvent<MODULE_ID_CORE>()(EVENT_VALUE_UPDATE_SONGLIST_RSP, 
		MODULE_ID_ROOM, 0, 0, 0, pEvent));
}

void ServerPacketProcess::onGetSongList(ServerHead& srvHead, ByteBuf& byteBuf)
{
	GetSongListRspEvent* pEvent = new core::GetSongListRspEvent();

	GetSonglistRspPacket packet(byteBuf, *pEvent);
	packet.Unpack();

	pEvent->roomid = srvHead.GetRoomId();

	m_pCoreModule->GetModuleManager()->PushEvent(
		MakeEvent<MODULE_ID_CORE>()(EVENT_VALUE_GET_SONGLIST_RSP, 
		MODULE_ID_ROOM, 0, 0, 0, pEvent));
}

void ServerPacketProcess::onOrderSongRsp(ServerHead& srvHead, ByteBuf& byteBuf)
{
	OrderSongRspPacket packet(byteBuf, srvHead.GetSeq());
	packet.Unpack();

	m_pCoreModule->GetModuleManager()->PushEvent(
		MakeEvent<MODULE_ID_CORE>()(EVENT_VALUE_ORDER_SONG_RSP, 
		MODULE_ID_ROOM, srvHead.GetRoomId(), packet.result, packet.songerUin, NULL));
}

void ServerPacketProcess::onOrderSongNotify(ServerHead& srvHead, ByteBuf& byteBuf)
{
	OrderSongNotifyEvent* pEvent = new core::OrderSongNotifyEvent();

	OrderSongNotifyPacket packet(byteBuf, *pEvent);
	packet.Unpack();

	pEvent->roomid = srvHead.GetRoomId();

	m_pCoreModule->GetModuleManager()->PushEvent(
		MakeEvent<MODULE_ID_CORE>()(EVENT_VALUE_ORDER_SONG_NOTIFY, 
		MODULE_ID_ROOM, 0, 0, 0, pEvent));
}

void ServerPacketProcess::onProcessOrderRsp(ServerHead& srvHead, ByteBuf& byteBuf)
{
	ProcessOrderRspPacket packet(byteBuf);
	packet.Unpack();

	m_pCoreModule->GetModuleManager()->PushEvent(
		MakeEvent<MODULE_ID_CORE>()(EVENT_VALUE_PROCESS_ORDER_RSP, 
		MODULE_ID_ROOM, srvHead.GetRoomId(), packet.result, 0, NULL));
}

void ServerPacketProcess::onProcessOrderNotify(ServerHead& srvHead, ByteBuf& byteBuf)
{
	ProcessOrderNotifyEvent* pEvent = new core::ProcessOrderNotifyEvent();

	ProcessOrderNotifyPacket packet(byteBuf, *pEvent);
	packet.Unpack();

	pEvent->roomid = srvHead.GetRoomId();

	m_pCoreModule->GetModuleManager()->PushEvent(
		MakeEvent<MODULE_ID_CORE>()(EVENT_VALUE_PROCESS_ORDER_NOTIFY, 
		MODULE_ID_ROOM, 0, 0, 0, pEvent));
}

void ServerPacketProcess::onGetOrderlistRsp(ServerHead& srvHead, ByteBuf& byteBuf)
{
	GetOrderListRspEvent* pEvent = new core::GetOrderListRspEvent();

	GetOrderlistRspPacket packet(byteBuf, *pEvent);
	packet.Unpack();

	pEvent->roomid = srvHead.GetRoomId();

	m_pCoreModule->GetModuleManager()->PushEvent(
		MakeEvent<MODULE_ID_CORE>()(EVENT_VALUE_GET_ORDERLIST_RSP, 
		MODULE_ID_ROOM, 0, 0, 0, pEvent));
}

void ServerPacketProcess::onGetOfflineActorRsp(ServerHead& srvHead, ByteBuf& byteBuf)
{
	GetOfflineActorRspEvent* pEvent = new core::GetOfflineActorRspEvent();

	GetOfflineActorRspPacket packet(byteBuf, *pEvent);
	packet.Unpack();

	pEvent->roomID = srvHead.GetRoomId();

	m_pCoreModule->GetModuleManager()->PushEvent(
		MakeEvent<MODULE_ID_CORE>()(EVENT_VALUE_CORE_GET_OFFLINE_ACTOR_RSP, 
		MODULE_ID_ROOM, 0, 0, 0, pEvent));
}

void ServerPacketProcess::onMarkSongRsp(ServerHead& srvHead, ByteBuf& byteBuf)
{
	MarkSongRspPacket packet(byteBuf);
	packet.Unpack();

	m_pCoreModule->GetModuleManager()->PushEvent(
		MakeEvent<MODULE_ID_CORE>()(EVENT_VALUE_MARK_SONG_RSP, 
		MODULE_ID_ROOM, srvHead.GetRoomId(), packet.result, 0, NULL));
}

void ServerPacketProcess::onMarkSongNotify(ServerHead& srvHead, ByteBuf& byteBuf)
{
	MarkSongNotifyEvent* pEvent = new core::MarkSongNotifyEvent();

	MarkSongNotifyPacket packet(byteBuf, *pEvent);
	packet.Unpack();

	pEvent->roomid = srvHead.GetRoomId();

	m_pCoreModule->GetModuleManager()->PushEvent(
		MakeEvent<MODULE_ID_CORE>()(EVENT_VALUE_MARK_SONG_NOTIFY, 
		MODULE_ID_ROOM, 0, 0, 0, pEvent));
}