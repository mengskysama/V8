#pragma once
#include "usermanager\UserManager.h"
#include "coredefine.h"
#include "globaluidefine.h"
#include "HtmlOutPutManager.h"
#include "HallDefine.h"
#include "sofa\SofaManager.h"
#include "ordersong\OrderSongManager.h"

struct IRoomBusinessSink 
{
	virtual void OnEnterRoom(IUserItem * pUserItem) = 0;									//用户进入房间
	virtual void OnEnterRoomGetUserList_LuckyStar(IUserItem* pUserItem ) = 0;				//幸运星
	virtual void OnEnterRoomGetUserList_VIPLevelTop( IUserItem* pUserItem ) = 0 ;			//至尊皇冠用户进场
	virtual void OnEnterRoomGetUserList_VIPLevelBlackDiamond(IUserItem * pUserItem) = 0;	//黑钻皇冠用户进场
	virtual void OnEnterRoomGetUserList_VIPLevelDiamond( IUserItem* pUserItem) = 0;			//钻石皇冠用户进场
	virtual void OnEnterRoomGetUserList_Blink(IUserItem* pUserItem ) = 0 ;					//闪亮登场	
	virtual void OnEnterRoomGetUserList_GiftStar( IUserItem* pUserItem ) = 0 ;				//礼物之星
	virtual void OnEnterRoomGetUserList_Crest(IUserItem * pUserItem) = 0 ;					//冠用户进场
	virtual void OnEnterRoomGetUserList_Event(IUserItem * pUserItem, uint8 unIndex) = 0;	//特殊活动进场
	virtual void OnKickUserRsp(uint32 unDstUIN) = 0;						//踢人响应
	virtual void OnKickUserNotify(uint32 unSrcUIN, uint32 unDstUIN) = 0;	//踢人通知
	virtual void OnAddBlackRsp(uint32 unDstUIN) = 0;						//拉黑响应
	virtual void OnAddBlackNotify(uint32 unSrcUIN, uint32 unDstUIN) = 0;	//拉黑通知
	virtual void OnLockIpRsp(uint32 unDstUIN) = 0;							//封IP响应
	virtual void OnLockIpNotify(uint32 unSrcUIN, uint32 unDstUIN) = 0;		//封IP通知
};

class RoomOption ;
class RoomBusiness
{
public:
	RoomBusiness(uint32 roomid, RoomOption * pRoomOption, UserManager * pUserManager, CSofaManager * pSofaManager, IRoomBusinessSink * pRoomBusinessSink);
	~RoomBusiness(void);

	void OnWelecomeWords(  RoomOption* pRoomOption ) ;

	UserManager * GetUserManager() { return m_pUserManager; }
	CSofaManager * GetSofaManager()	{ return m_pSofaManager; }
	COrderSongManager * GetOrderSongManager() { return m_pOrderSongManager; }
	void UpdateAllRoomMic( uint32 unMicVolume ) ;//更改麦克风音量
	uint32 GetRoomID(){ return m_roomid ;}
	RoomOption* GetRoomOption() ; 
public:
	//房间事件
	void ProcessGetUserListRsp( Event const& evt) ;
	void ProcessGetWaitingListRsp( Event const& evt) ;
	void ProcessEnterRoomNotify(Event const& evt) ;
	void ProcessLeaveRoomNotify(Event const& evt) ;
	void ProcessNotifyUserInfoUpdate(Event const& evt) ;
	void ProcessQueryClientInfoRsp(Event const& evt) ;
	void ProcessSetRoomInfoRsp(Event const& evt) ;
	void ProcessSetRoomInfoNotify(Event const& evt) ;
	void ProcessGetRoomInfoRsp(Event const& evt) ;
	void ProcessLockRoomNotify(Event const& evt) ;  //封停房间
	void ProcessGetSofaListRsp(Event const& evt) ;
	void ProcessBuySofaRsp(Event const& evt) ;
	void ProcessBuySofaNotify(Event const& evt) ;
	void ProcessNotifyClearSofaList(Event const& evt) ;

	//聊天
	void ProcessSendMessageRsp(Event const& evt) ;
	void ProcessSendMessageNotify(Event const& evt) ;

	//麦
	void ProcessRequestShowRsp(Event const& evt) ;
	void ProcessRequestShowNotify(Event const& evt) ;
	void ProcessExitShowNotify(Event const& evt) ;
	void ProcessOpenPrivateRsp(Event const& evt) ;
	void ProcessPushOnShowRsp(Event const& evt) ;					//邀请上麦
	void ProcessPushOnShowNotify(Event const& evt) ;
	void ProcessPullOffShowRsp(Event const& evt) ;					//邀请下麦
	void ProcessPullOffShowNotify(Event const& evt) ;
	void ProcessOpVideoAudioRsp( Event const& evt) ;				//打开、关闭音视频
	void ProcessOpVideoAudioNotify( Event const& evt) ;	
	void ProcessRequestP2PRsp(Event const& evt) ;
	void ProcessRequestP2PNotify(Event const& evt) ;
	void ProcessResponseP2PRsp(Event const& evt) ;
	void ProcessResponseP2PNotify(Event const& evt) ;
	void ProcessNotifyPrivateShow(Event const& evt) ;				//私麦列表
	void ProcessSetMicStatusRsp( Event const& evt ) ;				//限制使用此麦
	void ProcessSetMicStatusNotify( Event const& evt ) ;
	void ProcessAnswerPushOnShowRsp(Event const& evt);

	//管理
	void ProcessKickUserRsp(Event const& evt) ;
	void ProcessKickUserNotify(Event const& evt) ;
	void ProcessAddBlackRsp(Event const& evt) ;
	void ProcessAddBlackNotify(Event const& evt) ;
	void ProcessLockIpRsp(Event const& evt) ;		
	void ProcessLockIpNotify(Event const& evt) ;
	void ProcessShutUpRsp(Event const& evt) ;
	void ProcessShutUpNotify(Event const& evt) ;
	void ProcessUnLockShutUpRsp(Event const& evt) ; 
	void ProcessUnLockShutUpNotify(Event const& evt) ;
	void ProcessAddTitleRsp(Event const& evt) ;						//添加管理员
	void ProcessRemoveTitleRsp(Event const& evt) ;					//取消管理员
	void ProcessAddTitleNotify(Event const& evt) ;
	void ProcessRemoveTitleNotify(Event const& evt) ;

	//礼物
	void ProcessSendGiftRsp(Event const& evt) ;						//送礼
	void ProcessSendGiftNotify(Event const& evt) ;	
	void ProcessNotifyGiftOverTime(Event const& evt) ;				//礼物到期
	void ProcessNotifyPlutusAward(Event const& evt) ;				//财神到
	void ProcessSendFlyWordsRsp(Event const& evt) ;					//飞屏
	void ProcessSendFlyWordsNofity(Event const& evt) ;
	void ProcessNotifyMoneyUpdate(Event const& evt) ;				//钱币更新
	void ProcessNotifyOfficeNotice(Event const& evt) ;				//官方公告
	void ProcessNotifyRichStar(Event const& evt ) ;					//财富宝座

	//点歌
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

	//个人信息更新
	void ProcessMyInfoUpdate() ;
	void ProcessMyVipLevelUpdate();
	void ProcessMyRichLevelUpdate();

public:
	void OnGetUserList(std::vector<core::room_user_info> & userlist);
	void OnGetWaitingList(std::vector<uint32> & vecWaitinglist);

	void OnEnterRoomNotify(core::room_user_info & userinfo);
	void OnLeaveRoomNotify(uint32 UIN);
	void OnUserStatusChange(uint32 UIN, uint16 unStatus, bool bAdd);
	void OnUserTitleChange(uint32 unSrcUIN , uint32 unDstUIN , uint16 unTitle);
	void OnUserStampChange(uint32 unDstUIN, uint16 unStampID);
	void OnUpdateWaitingList(uint32 UIN, bool bAdd);  //进入 排麦
	void OnUserChangeVoice(uint32 unDstUIN, uint16 unChangeVoiceID);

	void OnOpVideoAudioOK( uint32 nOpAVType ,uint32 unUIN ) ;

	void OnSpeakerNotify(uint32 unSrcUIN, core::ENM_LoudSpeakerType enmType, std::wstring strMessage);

	void OnSendGiftRsp( SendGiftRspEvent * pInfo ) ;
	void OnSendGiftNotify( SendGiftNotifyEvent *pInfo ) ;
	void OnNotifyGiftOverTime( uint32 unDstUIN , uint32 unGiftID ) ;
	void OnGetRoomInfoRsp( ) ;					//房间公告


	void CollectRoom() ;  //收藏房间
	uint32 GetSearchSeqNo() ;
	void UserStampChange( uint32 unDstUIN, uint16 unStampID ) ;//印章变更

	CString GetRegisteUrl() ;
	CString GetUpdateVipUrl() ;
	CString GetRequestRoomUrl() ;
	CString GetFeedBackRoomUrl() ;
	CString GetPayUrl() ;
	CString GetLevelUpUrl();
	CString GetCopyRoomUrl();
	CString GetTeachAnnounceUrl();//获取论坛地址
	CString GetRoomPayUrl();
	CString GetServiceUrl();

	BOOL IsBlockChat( uint32 unUIN ) ;
	void UnBlockChat(uint32 unUIN ) ;
	void BlockChat( uint32 unUIN ) ;		

	void CreatePrivateMicDlgPos() ;
	void GetPrivateMicDlgPos( int& nPosX , int& nPosY , int& nIndex ) ;
	void ErasePrivateMicDlgPos( int nIndex ) ;

	void KickLeaveRoom(uint32 unRoomID, Hall::MsgBox179 msgbox179);

	uint32 GetCaiZiIntervalTime( core::ENM_VipLevel nVipLevel ) ;
private:
	uint16 GetUserStatusByIndex( uint8 unIndex ) ;	
private:
	uint32 m_roomid;
	RoomOption * m_pRoomOption;	
	UserManager * m_pUserManager;
	CSofaManager * m_pSofaManager;
	COrderSongManager * m_pOrderSongManager;
	IRoomBusinessSink * m_pRoomBusinessSink;
	uint32 m_nSeqNo ;
	std::vector<PrivateMicDlgPos> m_vPrivateMicDlgPos ;
};
