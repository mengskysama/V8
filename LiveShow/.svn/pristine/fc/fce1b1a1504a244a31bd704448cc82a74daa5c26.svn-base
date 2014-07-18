#pragma once
#include "51ktv.h"
#include "coredefine.h"

class CRoomParentDlg ;
class CRoomParentEvent
{
public:
	CRoomParentEvent() ;
	~CRoomParentEvent() ;

	void ProcessEvent( CRoomParentDlg* pParentDlg ,Event const&evt );

	//房间
	void ProcessGetUserListRsp( Event const& evt ) ;
	void ProcessGetWaitingListRsp( Event const& evt ) ;
	void ProcessEnterRoomNotify(Event const& evt ) ;
	void ProcessLeaveRoomNotify(Event const& evt ) ;
	void ProcessNotifyUserInfoUpdate(Event const& evt ) ;
	void ProcessQueryClientInfoRsp(Event const& evt ) ;
	void ProcessSetRoomInfoRsp(Event const& evt) ;
	void ProcessSetRoomInfoNotify(Event const& evt ) ;
	void ProcessGetRoomInfoRsp(Event const& evt ) ;
	void ProcessLockRoomNotify(Event const& evt ) ;  //封停房间
	void ProcessMediaServerResetNotify(Event const& evt ) ;//中转服务器IP地址变更
	void ProcessGetSofaListRsp(Event const& evt );
	void ProcessBuySofaRsp(Event const& evt );
	void ProcessBuySofaNotify(Event const& evt );
	void ProcessNotifyClearSofaList(Event const& evt);


	//聊天
	void ProcessSendMessageRsp(Event const& evt ) ;
	void ProcessSendMessageNotify(Event const& evt ) ;

	//喇叭
	void ProcessSendLoudSpeakerRsp(Event const& evt ) ;
	void ProcessSendLoudSpeakerNotify(Event const& evt ) ;

	//麦
	void ProcessRequestShowRsp(Event const& evt ) ;
	void ProcessRequestShowNotify(Event const& evt ) ;
	void ProcessExitShowNotify(Event const& evt) ;
	void ProcessOpenPrivateRsp(Event const& evt ) ;
	void ProcessPushOnShowRsp(Event const& evt ) ;			//邀请上麦
	void ProcessPushOnShowNotify(Event const& evt ) ;
	void ProcessPullOffShowRsp(Event const& evt ) ;			//邀请下麦
	void ProcessPuffOffShowNotify(Event const& evt ) ;
	void ProcessOpVideoAudioRsp( Event const& evt ) ;				//打开、关闭音视频
	void ProcessOpVideoAudioNotify( Event const& evt ) ;	
	void ProcessRequestP2PRsp(Event const& evt ) ;
	void ProcessRequestP2PNotify(Event const& evt ) ;
	void ProcessResponseP2PRsp(Event const& evt ) ;
	void ProcessResponseP2PNotify(Event const& evt ) ;
	void ProcessNotifyPrivateShow(Event const& evt ) ;				//私麦列表
	void ProcessSetMicStatusRsp( Event const& evt ) ;				//限制使用此麦
	void ProcessSetMicStatusNotify( Event const& evt ) ;
	void ProcessAnswerPushOnShowRsp(Event const& evt);

	//管理
	void ProcessKickUserRsp(Event const& evt ) ;
	void ProcessKickUserNotify(Event const& evt ) ;
	void ProcessAddBlackRsp(Event const& evt ) ;
	void ProcessAddBlackNotify(Event const& evt ) ;
	void ProcessLockIpRsp(Event const& evt ) ;		
	void ProcessLockIpNotify(Event const& evt ) ;
	void ProcessShutUpRsp(Event const& evt ) ;
	void ProcessShutUpNotify(Event const& evt ) ;
	void ProcessUnLockShutUpRsp(Event const& evt ) ; 
	void ProcessUnLockShutUpNotify(Event const&evt ) ;
	void ProcessAddTitleRsp(Event const& evt ) ;				//添加管理员
	void ProcessRemoveTitleRsp( Event const& evt ) ;			//取消管理员
	void ProcessAddTitleNotify( Event const& evt ) ;
	void ProcessRemoveTitleNotify( Event const& evt ) ;

	//礼物
	void ProcessSendGiftRsp(Event const& evt ) ;					//送礼
	void ProcessSendGiftNotify(Event const& evt ) ;	
	void ProcessNotifyGiftOverTime(Event const& evt ) ;			//礼物到期
	void ProcessNotifyPlutusAward(Event const& evt ) ;			//财神到
	void ProcessSendFlyWordsRsp(Event const& evt ) ;			//飞屏
	void ProcessSendFlyWordsNofity(Event const& evt ) ;
	void ProcessNotifyMoneyUpdate(Event const& evt ) ;			//钱币更新
	void ProcessNotifyOfficeNotice(Event const& evt ) ;			//官方公告
	void ProcessNotifyRichStar(Event const& evt ) ;				//财富宝座

	//点歌系统
	void ProcessUpdateSongListRsp(Event const& evt ) ;			//更新歌曲列表
	void ProcessGetSongListRsp(Event const& evt ) ;				//获得歌曲列表
	void ProcessOrderSongRsp(Event const& evt ) ;				//点歌响应
	void ProcessOrderSongNotify(Event const& evt ) ;			//点歌通知
	void ProcessTakeOrderRsp(Event const& evt ) ;				//处理点歌响应
	void ProcessTakeOrderNotify(Event const& evt ) ;			//处理点歌通知
	void ProcessGetOrderListRsp(Event const& evt ) ;			//获得已点歌曲列表
	//评价
	void ProcessMarkSongRsp(Event const& evt ) ;				//评价响应
	void ProcessMarkSongNotify(Event const& evt ) ;				//评价通知

	void OnSendNomalGift( core::ENM_GiftDstType enmDstType ,uint32 unSrcUIN ,uint32 unDstUIN , uint32 unSentCount ,
		uint16 unGiftID  , core::ENM_GiftType enmType, uint32 time ) ;
	void OnSendNomalGiftNotify( core::ENM_GiftDstType enmDstType ,uint32 unSrcUIN ,uint32 unDstUIN , uint32 unSentCount , 
		uint16 unGiftID  , core::ENM_GiftType enmType, uint32 time ) ;

private:
	CRoomParentDlg* m_pParentDlg ;
	typedef void(CRoomParentEvent::*EventFun)(Event const& ) ;
	std::map<uint32,EventFun> m_mEventFun ;
};