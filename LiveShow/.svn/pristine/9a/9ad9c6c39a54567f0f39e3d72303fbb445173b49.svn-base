
#pragma once

#include "51ktv.h"
#include "imodule.h"
#include "coredefine.h"
#include "GlobalManager.h"
#include "RoomDefine.h"
#include "RoomParentEvent.h"
#include "SysSetting/SysSettingDlg.h"

class CRoomParentDlg;
typedef std::map<uint32, CRoomParentDlg*> MapRoomParentDlg;

core::CurInfo GetCurInfo();

class RoomAVLogic ;
class RoomModule : public IModule
{
public:
	RoomModule(void);
	~RoomModule(void);

	virtual void Load(IModuleManager *);
	virtual void Unload();
	virtual void ProcessEvent(Event const&);
	virtual void ProcessMessage(Message const&);
	virtual int32 Invoke(ServiceValue const serviceValue, param);
	void OpenRoomDlg(Room_Create_And_Show * stCreateAndShow);
	CRoomParentDlg* FindRoomParentDlg(uint32 roomid ) ;

	void OnRoomDlgClose(uint32 roomid);
	void UpdatePlutusAwardOrLuckMoney(  uint32 unMoney ) ;  //财神降临时，财神送钱时金币的变更或自己中奖时钱币的变更
	void UpdateAllRoomMoney(uint32 unMoney);
	void ReviseAllRoomMoney( uint32 lMoney ) ; //离开房间时，修正因财神引起的金币变更
	void UpdateAllRoomMic( uint32 unMicVolume ) ;//更改麦克风音量
	void UpdateAllRoomStatus(BOOL bOnLine);//更改在线离开状态
	void CloseMic() ;							//关闭话筒、麦克风
	void OpenMic() ;							//开启话筒、麦克风
	void SetAllRoomVideoCfg( int iDevice ) ;
	void UpdateAllRoomUserStampChange( uint32 unDstUIN, uint16 unStampID ) ;  //印章变更
	//void UpdateAllRoomAudioMode();
	IModuleManager * GetModuleManager(){ return m_pModuleManager ;} ;

	void KickLeaveRoom( uint32 roomid ) ;
	void AddRecentBrowseRoom(uint32 roomId);  //添加最近浏览房间

	void CloseAllRoom();

	bool IsSpeedUpMode()
	{
		return m_bSpeedUpMode;
	}
	bool IsEnteredTheRoom(uint32 unRoomID);
public:
	IModuleManager* m_pModuleManager;
private:
	int32 CreateAndShow(param st ) ;
	int32 GetAudioDeviceList(param st) ;
	int32 GetVideoDeviceList(param st ) ;
	int32 SavePhotoToBmp(param st ) ;
	int32 GetNetState(param st ) ;
	int32 GetRoomList(param st ) ;
	int32 CloseARoom(param st ) ;
	int32 ActiveARoom(param st ) ;
	int32 ClosedRegisterDlg(param st ) ;
	int32 ShowSysSetting(param st );
	int32 GetRichLevelInfo(param st);
	
	void CreateNetStateTimer(uint32 interval);
	void ClearNetStateTimer();
	void OnNetStateTimeOut(uintptr,bool);

	//定时检测cpu消耗
	void CreatePerformanceTimer(uint32 interval);
	void ClearPerformanceTimer();
	void OnPerformanceTimerOut(uintptr,bool);
	
	void NetStateReq(uint32&iRelayIP,struct Service_Av_Net_Status_All& sansa);  //发送音视频丢包率数据至服务器

	void OnCycleTrigger( Message const& msg );

	void ProcessLoginComplete( Message const& msg ) ;
	//清空房间
	void ProcessCleanRoom(Message const& msg ) ;

	//大厅声音
	void ProcessHallOpenSound(Message const& msg);				//大厅打开关闭声音

	//用户头像下载完毕
	void ProcessHallHeadImageUpdate(Message const& msg ) ;

	//大厅资源更新完毕
	void ProcessHallResUpdate(Message const& msg );

	//个人信息更新
	void ProcessMyInfoUpdate(Message const& msg ) ;
	void ProcessMyVipLevelUpdate(Message const& msg);
	void ProcessMyRichLevelUpdate(Message const& msg);

	//通用的Web访问接口返回响应
	void ProcessWebGetHttpResp(Event const&evt) ;	

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
	void ProcessNotifyClearSofaList(Event const& evt );
	void ProcessInitOffRoomAcotrRsp(Event const& evt); //离线主播列表返回响应函数

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
	void ProcessPushOnShowRsp(Event const& evt ) ;					//邀请上麦
	void ProcessPushOnShowNotify(Event const& evt ) ;
	void ProcessPullOffShowRsp(Event const& evt ) ;					//邀请下麦
	void ProcessPullOffShowNotify(Event const& evt ) ;
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
	void ProcessSendGiftRsp(Event const& evt ) ;				//送礼
	void ProcessSendGiftNotify(Event const& evt ) ;	
	void ProcessNotifyGiftOverTime(Event const& evt ) ;			//礼物到期
	void ProcessNotifyPlutusAward(Event const& evt ) ;			//财神到	
	void ProcessNotifyMoneyUpdate(Event const& evt ) ;			//钱币更新
	void ProcessNotifyOfficeNotice(Event const& evt ) ;			//官方公告
	void ProcessSendFlyWordsRsp(Event const& evt) ;				//飞屏响应
	void ProcessSendFlyWordsNotify(Event const& evt) ;			//飞屏通知
	void ProcessNotifyRichStar(Event const& evt ) ;				//财富宝座

	//个人设置
	void ProcessSetSelfInfoRsp(Event const& evt ) ;
	void ProcessGetFansRsp(Event const&evt);                       //粉丝排行

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

	//任务系统
	//DragonZ:V3.0去除房间右下角的任务
	//void ProcessTaskInfoNotify(Event const& evt);

	
private:
	RoomAVLogic *m_pRoomAvLogic ;
	MapRoomParentDlg m_mapRoomParentDlg;
	typedef void(RoomModule::*MessageFun)(Message const&) ;
	std::map<uint32,MessageFun> m_mMessageFun ;
	typedef void(RoomModule::*EventFun)(Event const& ) ;
	std::map<uint32,EventFun> m_mEventFun ;
	CRoomParentEvent m_RoomParentEvent ;
	CSysSettingDlg* m_pRoomSysSettingDlg;
	int32 m_NetStateTimer;
	int32 m_PerformanceTimer;
	bool m_bSpeedUpMode;//是否加速模式(cpu负载高，过滤礼物刷屏、跑马灯、flash)
	std::vector<uint32> m_vRoomIDList;
	std::map<uint32, bool> m_mapEnteredRoomID;//本次登录所进过的房间ID, 为true表示进过此ID的房间
};