/**
* ServerPacketProcess.h
* 来自服务器的协议处理
*
* @version 1.0 (2011-12-21)
* @author plushu
*
*/

#pragma once

#include "51ktv.h"
#include "coredefine.h"
#include "protocol\ProtocolBase.h"

class CoreModule;

class ServerPacketProcess
{
public:
	ServerPacketProcess(CoreModule* pCoreModule);
	~ServerPacketProcess(void);

	void process(ServerHead& srvHead, ByteBuf& byteBuf);

private:
	void onGetSelfInfoRsp(ServerHead&, ByteBuf&);
	void onSetSelfInfoRsp(ServerHead&, ByteBuf&);
	void onSetSelfInfoNotify(ServerHead&, ByteBuf&);
	void onRoomLoginRsp(ServerHead&, ByteBuf&);
	void onKicked(ServerHead&, ByteBuf&);
	void onNotifyMessage(ServerHead&, ByteBuf&);
	void onNotifyPasswordSafe(ServerHead&, ByteBuf&);
	void onRoomLoginNotify(ServerHead&, ByteBuf&);
	void onRoomLogoutNotify(ServerHead&, ByteBuf&);
	void onGetUserListRsp(ServerHead&, ByteBuf&);
	void onGetWaittingListRsp(ServerHead&, ByteBuf&);
	void onSendMessageRsp(ServerHead&, ByteBuf&);
	void onSendMessageNotify(ServerHead&, ByteBuf&);
	void onSendGiftRsp(ServerHead&, ByteBuf&);
	void onSendGiftNotify(ServerHead&, ByteBuf&);
	void onSendLoudSpeakerRsp(ServerHead&, ByteBuf&);
	void onSendLoudSpeakerNotify(ServerHead&, ByteBuf&);
	void onRequestShowRsp(ServerHead&, ByteBuf&);
	void onRequestShowNotify(ServerHead&, ByteBuf&);
	void onExitShowNotify(ServerHead&, ByteBuf&);
	void onOperateAVRsp(ServerHead&, ByteBuf&);
	void onOperateAVNotify(ServerHead&, ByteBuf&);
	void onKickUserRsp(ServerHead&, ByteBuf&);
	void onKickUserNotify(ServerHead&, ByteBuf&);
	void onAddBlackRsp(ServerHead&, ByteBuf&);
	void onAddBlackNotify(ServerHead&, ByteBuf&);
	void onLockIPRsp(ServerHead&, ByteBuf&);
	void onLockIPNotify(ServerHead&, ByteBuf&);
	void onShutUpRsp(ServerHead&, ByteBuf&);
	void onShutUpNotify(ServerHead&, ByteBuf&);
	void onUnlockShutUpRsp(ServerHead&, ByteBuf&);
	void onUnLockShutUpNotify(ServerHead&, ByteBuf&);
	void onPushOnShowRsp(ServerHead&, ByteBuf&);
	void onPushOnShowNotify(ServerHead&, ByteBuf&);
	void onPullOffShowRsp(ServerHead&, ByteBuf&);
	void onPullOffShowNotify(ServerHead&, ByteBuf&);
	void onAddTitleRsp(ServerHead&, ByteBuf&);
	void onAddTitleNotify(ServerHead&, ByteBuf&);
	void onRemoveTitleRsp(ServerHead&, ByteBuf&);
	void onRemoveTitleNotify(ServerHead&, ByteBuf&);
	void onOpenPrivateRsp(ServerHead&, ByteBuf&);
	void onRequestP2PRsp(ServerHead&, ByteBuf&);
	void onRequestP2PNotify(ServerHead&, ByteBuf&);
	void onReponseP2PRsp(ServerHead&, ByteBuf&);
	void onResponseP2PNotify(ServerHead&, ByteBuf&);
	void onReportClientInfoNotify(ServerHead&, ByteBuf&);
	void onGetClientInfoRsp(ServerHead&, ByteBuf&);
	void onSetRoomInfoRsp(ServerHead&, ByteBuf&);
	void onSetRoomInfoNotify(ServerHead&, ByteBuf&);
	void onGetRoomInfoRsp(ServerHead&, ByteBuf&);
	void onUserInfoChange(ServerHead&, ByteBuf&);
	void onPrivateShowList(ServerHead&, ByteBuf&);
	void onGiftOverTime(ServerHead&, ByteBuf&);
	void onMoneyUpdate(ServerHead&, ByteBuf&);
	void onOfficialNotice(ServerHead&, ByteBuf&);
	void onPlutusAward(ServerHead&, ByteBuf&);
	void onRoomLocked(ServerHead&, ByteBuf&);
	void onSetMicStatusRsp(ServerHead&, ByteBuf&);
	void onSetMicStatusNotify(ServerHead&, ByteBuf&);
	void onSendFlyWordsRsp(ServerHead&, ByteBuf&);
	void onSendFlyWordsNotify(ServerHead&, ByteBuf&);
	void onMediaServerResetNotify(ServerHead&, ByteBuf&);
	void onMyInfoUpdateNotify(ServerHead&, ByteBuf&);
	void onGiftStarNotify(ServerHead&, ByteBuf&);
	void onMyGoodsNotify(ServerHead&, ByteBuf&);
	void onRichStarNotify(ServerHead&, ByteBuf&);
	void onPushOnShowAnswerRsp(ServerHead&, ByteBuf&);
	void onTaskNotify(ServerHead&, ByteBuf&);
	void onGetTaskRewardRsp(ServerHead&, ByteBuf&);
	void onGetFansRsp(ServerHead&, ByteBuf&);
	void onBuySofaRsp(ServerHead&, ByteBuf&);
	void onBuySofaNotify(ServerHead&, ByteBuf&);
	void onClearSofaList(ServerHead&, ByteBuf&);
	void onGetSofaList(ServerHead&, ByteBuf&);
	void onUpdateSongList(ServerHead&, ByteBuf&);
	void onGetSongList(ServerHead&, ByteBuf&);
	void onOrderSongRsp(ServerHead&, ByteBuf&);
	void onOrderSongNotify(ServerHead&, ByteBuf&);
	void onProcessOrderRsp(ServerHead&, ByteBuf&);
	void onProcessOrderNotify(ServerHead&, ByteBuf&);
	void onGetOrderlistRsp(ServerHead&, ByteBuf&);
	void onGetOfflineActorRsp(ServerHead&, ByteBuf&);
	void onMarkSongRsp(ServerHead&, ByteBuf&);
	void onMarkSongNotify(ServerHead&, ByteBuf&);

private:
	CoreModule* m_pCoreModule;	
};
