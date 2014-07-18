#pragma once
#include "HtmlBuild.h"
#include "GlobalRoomDefine.h"
#include "roomsetting/RoomOption.h"

class HtmlEditImpl ;
class RoomOption ;
class SpeakerMsg;

class CRoomParentDlg ;
class CSofaItem;
class HtmlOutPutManager
{
public:
	HtmlOutPutManager( CRoomParentDlg* pParentDlg ) ;
	~HtmlOutPutManager() ;
	
	void OnEnterRoomGetUserList_VIPLevelTop( IUserItem* pUserIterm ) ;
	void OnEnterRoomGetUserList_VIPLevelBlackDiamond( IUserItem* pUserIterm ) ;
	void OnEnterRoomGetUserList_VIPLevelDiamond(IUserItem* pUserItem);
	void OnEnterRoomGetUserList_GiftStar( IUserItem* pUserIterm ) ;
	void OnEnterRoomGetUserList_Crest( IUserItem* pUserItem ) ;
	void OnEnterRoomGetUserList_Blink( IUserItem* pUserItem ) ;
	void OnEnterRoomGetUserList_LuckyStar( IUserItem* pUserItem ) ;
	void OnEnterRoomGetUserList_Event(IUserItem * pUserItem, uint8 unIndex);
	void OnEnterRoom( IUserItem * pUserItem ) ;
	void OnLeaveRoom( uint32 srcuin ) ;

	void OnSendMsgSuccess( SendMessageEvent& message ) ;

	void OnSendPublicMsgRsp( uint32 srcuin , uint32 dstuin , CString& strText , MessageFont& stFont ) ;
	void OnPublicMsgNotify( uint32 srcuin , uint32 dstuin , CString& strText , MessageFont& stFont  ) ;						//公屏

	void OnPrivateMsg( uint32 srcuin , uint32 dstuin , CString& strText , MessageFont& stFont , bool bNotify , bool bAutoMsg = FALSE) ;						//私聊

	void OnTempNoticeMsg(uint32 srcuin ,CString& strText , MessageFont& stFont) ;					//临时公告

	void OnWelecomeWords( RoomOption* pRoomOption ) ;	//欢迎辞
	void OnActorAutoSpeak(IUserItem* pUserItem); //艺人自动发言
	void OnAgentAutoSpeak(IUserItem* pUserItem);//销售自动发言
	void OnShowGiftStar(IUserItem* pUserItem);
	void OnNoticeMsg( CString& strText ) ;//房间公告,strText内含字体信息
	void OnNoticeMsg( CString& strText , MessageFont& stFont) ;						//房间公告
	void OnOfficialNocice( CString& strText ) ;
	void OnSendMsg_NoPublic() ; //不能公聊
	void OnSendMsg_NoCaiZi() ;	//不能发送彩字
	void OnUserTitleChange( uint32 srcuin ,uint32 dstuin ,uint16 unTitle ) ;		//管理员称号变更
	void OnShutUpNotifyEvent( uint32 srcuin , uint32 dstuin , uint32 unLeftTime ) ;						//禁言
	void OnSendMsgRsp_Fast() ;					//发送消息太快
	void OnSendMsgRsp_PublicForbid() ;			//公聊被禁止
	void OnSendMsgRsp_CaiZiForbid() ;			//彩条被禁止
	void OnSendMsgRsp_UsrForbid( uint32 unLeftTime ) ;				//用户被禁言
	void OnSendMsgRsp_NoPermission() ;			//无权限
	void OnSendMsgRsp_PublicVip() ;
	void OnSendMsgRsp_PublicReg() ;
	void OnNotEnoughGold() ;					//金币不足
	void OnNotRepeat() ;						//禁止重复发言
	void OnMsgEmpty() ;							//聊天信息为空	
	void OnNotCaiZiTime( core::ENM_VipLevel nVipLevel , uint32 nLeftTime ) ;						//彩字发送间隔未满10秒
	void OnSendMsgLimitedTime(uint32 unLeftTime);
	void OnCopyRoomUrl();

	void OnUserStampChangeRsp( uint32 unDstUIN, uint16 unStampID) ;
	void OnUserStampChangeNotify( uint32 unSrcUIN , uint32 unDstUIN, uint16 unStampID ) ;		//盖章
	void OnRequestShow_WaitingList( bool freeshow_opened ) ;
	void OnRequestShow_Failed( core::ENM_RequestShowResult nResult ) ;  //请求排麦失败
	void OnRequestShow_Already_OnShow() ;
	void OnRequestShow_Public( uint8 unIndex ) ;   //自己排上公麦
	void OnRequestShow_PublicNotify( uint32 unSrcUIN ) ;
	void OnRequestShow_Private( ) ;				//自己排上私麦
	void OnRequestShow_PrivateNotify( uint32 srcuin ) ; 
	void OnRequestShow_InOtherRoom() ;
	void OnRequestShow_WaitButOnMic( uint32 srcuin ) ;
	void OnRequestShow_WaitingOnMic() ;
	void OnRequestShow_Success() ;
	void OnExitShowSelf( IUserItem *pUserItem  ) ;
	void OnExitShowNotify( uint32 unSrcUIN , int nType ) ;
	void OnExitP2P( uint32 unUIN  ) ;
	void OnKickOutRsp( uint32 unDstUIN ) ;
	void OnKickOutNotify( uint32 srcuin ,uint32 unDstUIN ) ;
	void OnAddBlackNotify( uint32 unSrcUIN , uint32 unDstUIN ) ;
	void OnLockIpNotify( uint32 unSrcUIN , uint32 unDstUIN ) ;
	void OnSpeakerMsg( core::ENM_LoudSpeakerType enmType , CString& strText , MessageFont& stFont , SpeakerInfo& stInfo);
	void OnSpeakerLuckyMsg(CString& strText, SpeakerInfo& stInfo, uint16 unGiftID);
	void OnSpeakerPlutusAward(CString& strText, SpeakerInfo& stInfo, uint16 unGiftID); //财神
	void OnSpeakerSystemMsg(CString& strText);
	void OnSpeakerGameMsg(CString& strText);
	void OnSpeakerLuxuryMsg(SpeakerInfo& stSrcInfo, SpeakerInfo& stDstInfo, CString& strRoomName, uint16 unGiftID, uint16 unCount);
	void OnUserChangeVoiceRsp( uint32 unDstUIN, uint16 unChangeVoiceID) ;
	void OnUserChangeVoiceNotify( uint32 unSrcUIN , uint32 unDstUIN, uint16 unChangeVoiceID ) ;		//变声卡

	void OnOpenPrivateRsp_NotOpen() ;			//地方没有私麦打开
	void OnOpenPrivateRsp_NotPermission() ;		//无权限	
	void OnOpenPrivateRsp_ISFull() ;			//当前打开的私麦已达上限
	void OnOpenPrivateRsp_AlreadyOpened() ;		//已经打开过了
	void OnOpenPrivateRsp_IsSelf();				//是自己
	void OnPushOnShowRsp_Success( uint32 dstuin ) ;//邀请上麦成功
	void OnPushOnShowRsp_Failed( uint32 dstuin ) ;			   //邀请上麦
	void OnPushOnShowRsp_MicLocked() ;
	void OnPushOnShowRsp_OnMic();
	void OnPushOnShowRsp_Mic_HasPlayer();
	void OnPushOnShowRsp_NeedAnswer();			//邀请发出，需别人同意
	void OnPushOnShowRsp_Refuse();				//自己拒绝公麦邀请
	void OnPushOnShowRsp_Refuse( uint32 unDstUIN ); //别人拒绝公麦邀请
	void OnPullOffShowRsp_Success( uint32 unDstUIN ) ;
	void OnPullOffShowRsp_Failed() ;
	void OnPushOnShowNotify( uint32 srcuin , uint32 dstuin , uint8 unIndex ) ;
	void OnPuffOffShowNotify( uint32 unSrcUIN ,uint32 unDstUIN ) ;
	void OnSetMicStatusRsp( int nIndex , int nStatus  ) ;
	void OnSetMicStatusNotify( int nIndex , int nStatus , uint32 srcuin ) ;

	void OnRequestP2PRspOK( uint32 dstuin ) ; 
	void OnRequestP2PNotify(  uint32 unSrcUIN ) ;
	void OnRequestP2PRspFailed( uint32 nResult , uint32 dstuin ) ;
	void OnResponseP2PRspFailed( uint32 nResult , uint32 dstuin , ENM_ResponseP2PType enmType ) ;
	void OnResponseP2PRspAccept( uint32 dstuin ) ;		//自己接受
	void OnResponseP2PRspRefuse( uint32 dstuin ) ;		//自己拒绝
	void OnResponseP2PNotifyAccept( uint32 srcuin ) ;		//对方接受
	void OnResponseP2PNotifyRefuse( uint32 srcuin ) ;		//对方拒绝
	void OnRequestP2PFailed_NotPermission() ;
	void OnKickUserNotPermission( uint32 unDstUIN ) ;
	void OnKickUserRsp_Success( uint32 dstuin , bool bKickAllVisitor ) ;	
	void OnAddBlackRsp_Success( uint32 dstuin ) ;
	void OnLockIpRsp_Success( uint32 dstuin ) ;
	void OnShutUpRsp_Success( uint32 dstuin ) ;
	void OnUnLockShutUpRsp_Success( uint32 dstuin ) ;
	void OnAddTitleRsp_Success( core::ENM_TitleLevel nTitleLevel , uint32 dstuin ) ;
	void OnAddTitleRsp_IsFull( core::ENM_TitleLevel nTitleLevel , uint32 dstuin ) ;
	void OnAddTitleRsp_IsAdmin_InOther( core::ENM_TitleLevel nTitleLevel , uint32 dstuin ) ;
	void OnRemoveTitleRsp_Success( core::ENM_TitleLevel nTitleLevel , uint32 dstuin ) ;
	void OnAddTitleNotify( uint32 unSrcUIN ,uint32 unDstUIN ,core::ENM_TitleLevel nTitleLevel , uint32 unRoomID ) ;
	void OnRemoveTitleNotify( uint32 unSrcUIN ,uint32 unDstUIN , core::ENM_TitleLevel nTitleLevel ,  uint32 unRoomID ) ;
	void OnOperateNotPermission() ;			//拉黑禁言、邀麦等无权限
	void OnTopVipClickBlink();
	void OnUnLockShutUpNotify( uint32 unDstUIN ) ;				//解除禁言
	void OnRequestPushOnShowNotify(uint32 unSrcUIN, uint8 unIndex);

	void OnSendGift_NotExist() ;
	void OnSendGift_CanNotSend( uint16 unGiftID ) ;
	void OnSendGift_CanNotSendToVisitor( core::ENM_GiftType enmType ) ;
	void OnSendGift_CanNotSendToManage( ) ;
	void OnPublicGiftTip( uint32 srcuin , uint16 unGiftID , core::ENM_GiftDstType enmDstType ) ;
	void OnPaodaoSheChiGift(core::SendGiftNotifyEvent*) ;//奢侈道具礼物
	void OnLinkEnterGift(uint16 unGiftID) ;			//购买闪亮登场类道具成功
	void OnPaodaoSheChiGift( uint32 srcuin , uint32 dstuin ,std::wstring strGiftWord ,uint16 unGiftID , uint16 unCount ,uint32 unTime ) ;
	void OnNotifyPlutusAward( uint32 srcuin , uint32 un179ID , std::wstring& strName ,
		std::wstring strRoomName ,std::vector<uint16>& vecMoneys  , uint16 unGiftID ) ; //财神到

	void OnPublicChatChange( ENM_RoomPublicChat enmType , uint32 srcuin ) ;
	void OnColorTextChange( bool bColorText , uint32 srcuin ) ;
	void OnFreeShowChange( bool bFreeShow , uint32 srcuin ) ;			//自由排麦
	void OnSignatureChange( std::wstring& strSignature ) ;				//房间主题

	void SetPublicSheChiGift( uint32 srcuin ,uint32 dstuin ,uint16 unGiftID ,uint32 unCount  ) ;
	void SetPublicNormalGift( GiftInfo& giftinfo, bool reallyOutput ) ;//reallyOutput:是否实际输出
	void SetPrivateNormalGift( uint32 srcuin , uint32 dstuin ,uint16 unGiftID , uint16 unCount ) ;	
	void SetPrivateFireWork( uint32 srcuin  ) ;
	void SetPrivateSlavo( uint32 srcuin ) ;
	void InsertPrivateTips(const CString& strTitle , const CString& strValue , const CString& strColor ) ;

	bool ProcessSendMsgText( CString& strText , bool bFilter = true ) ; //返回是否禁发
	void ProcessRecvMsgText( CString& strText , bool bFilter = true ) ; //
	void ProcessRecvNoticeText( CString& strText , bool bFilter = true ) ;//公告的非法词过滤

	//canCheat:本次输出是否可以作弊(1/2概率性输出),返回是否可以追加输出一次(如果本次真的作弊)
	bool OutPutGift( GiftInfo& giftinfo, bool canCheat ) ;

	void OutPutLucky( LuckyInfo& luckyinfo ) ;
	void PlutusAward( int nPlutesIndex , int nPlutusMoney ) ;

	void OnPrivateShowOff( IUserItem* pUserItem ) ;			//该用户已经下私麦
	void OnModifyRoomName();
	CString GetUserHead( IUserItem* pUserItem ) ;
	CString GetUserItemHeadImg( IUserItem* pUsrItem ) ;//获得用户名前的章和冠图片
	CString GetNameAndID(IUserItem * pUserItem);
	void OnBlockChatToMe( std::wstring strName , uint32 un179ID );
	void OnUnBlockChatToMe( std::wstring strName , uint32 un179ID );

	void AutoWelcome( IUserItem* pUserItem ) ;
	void VisitorRegister( IUserItem* pUserItem ) ;
	void OnFlyTextNotify( uint32 unSrcUIN, std::wstring strText );

	CString GetSofaUserHead(CSofaItem * pSofaItem);
	void OnSofaPublicNotify(IUserItem * pUserItem, bool bSameUser = false);
	void OnSofaBeKicked(IUserItem * pUserItem);		//沙发被踢
	void OnSofaNewUser(IUserItem * pNewUserItem, IUserItem * pOldUserItem, bool bEmpty = false);		//用户上沙发
	void OnSofaNewUser(IUserItem * pNewUserItem, CSofaItem * pSofaItem);
	void OnSofaMyUpdate(IUserItem * pUserItem);		//自己更新沙发

	void OnModifyFont(); //非荣耀VIP以上不能修改字体

	void InsertRoomMidInputDefaultText(CString& strText);

	void OnMarkNotify(CString& strSongerName);			//提示点歌者去评价
	void OnMarkSongNotify(CString& strOrderName, CString& strSongName, CString& strMark);//提示主播点歌评价结果

private:	
	bool IsPublicMic( uint16 unStatus ) ;
	CString MicChineseIndex( int nIndex ) ;
	void EmotionCodeToPath( CString& strText ) ;

	HtmlBuild m_HtmlBuild ;
	CRoomParentDlg* m_pParentDlg ;
};