#pragma once
#include "coredefine.h"
#include "usermanager/useritem.h"
#include "GlobalRoomDefine.h"
using namespace core;

class RoomOption ;
class RoomBusiness ;
class CSofaItem ;
class HtmlBuild
{
public:	
	HtmlBuild( RoomBusiness* pRoomBusiness ) ;
	CString GetRequestP2PNotify( IUserItem* pSrcUserItem ) ;
	CString GetRequestPublicMicNotify( IUserItem * pSrcUserItem, uint8 unIndex );
	CString GetOpenPrivateRspNoPermission() ;
	CString GetOpenPrivateRspIsFull() ;
	CString GetPublicStamp( IUserItem* pSrcUserItem ,IUserItem* pDstUserItem , uint16 unStampID ) ;
	CString GetPrivateStamp( IUserItem* pSrcUserItem , uint16 unStampID ) ;
	CString GetPrivateStampRsp( IUserItem* pDstUserItem, uint16 unStampID );
	CString GetPrivateMicNotify( IUserItem* pSrcUserItem  ) ;
	CString GetPublicChangeVoice( IUserItem* pSrcUserItem ,IUserItem* pDstUserItem , uint16 unChangeVoiceID );
	CString GetPrivateChangeVoice( IUserItem* pSrcUserItem , uint16 unChangeVoiceID );
	CString GetPrivateChangeVoiceRsp( IUserItem* pDstUserItem, uint16 unChangeVoiceID );

	CString GetPrivateFireWork( IUserItem* pSrcUserItem ) ;
	CString GetPrivateSlavo( IUserItem* pSrcUserItem ) ;
	CString GetPublicFireWork( IUserItem* pSrcUserItem) ;
	CString GetPublicSlavo( IUserItem* pSrcUserItem ) ;
	CString GetGiftLucky( uint32 unSrcUIN ,uint32 src179ID ,CString& strSrcHead ,
		std::wstring strSrcName ,uint16 unGiftID , uint16	unLuckyTimes ,uint32 unLuckyMoney ) ;
	CString GetPrivateNormalGift( IUserItem* pSrcUserItem ,uint16 unGiftID , uint16 unCount ) ;

	CString GetPublicNormalGift( GiftInfo& giftinfo  , uint32 unGiftCount ) ;
	CString GetPublicSheChiGift( IUserItem* pSrcUserItem ,IUserItem* pDstUserItem , uint16 unGiftID , uint32 unCount ) ;

	CString GetChat(IUserItem* pSrcUserItem ,IUserItem* pDstUserItem ,
		CString& strText ,MessageFont& stFont , bool bPublic , bool bReplaceSelf , bool bAutoMsg = FALSE) ;	
	CString GetPrivateTips( const CString& strTitle , const CString& strValue , const CString& strColor=COLOR_NORMAL ) ;
	CString GetPublicTips( const CString& strTitle , const CString& strValue , const CString& strColor =COLOR_NORMAL ) ;
	CString GetOfficialNocice( CString& strValue ) ;
	CString GetNotice( const CString& strValue ) ;
	CString GetNotice(const CString& strValue , MessageFont& stFont) ;
	CString GetTempNotice( uint32 un179ID ,CString strSrcDecodeName ,const CString& strValue , MessageFont& stFont ) ;
	CString GetGiftStar(IUserItem* pUserItem, uint16 unGiftID);
	CString GetEnterRoomGetUserList_VIPLevelTop( IUserItem* pUserItem ) ;		//至尊皇冠
	CString GetEnterRoomGetUserList_VIPLevelDiamond(IUserItem* pUserItem);    //钻石皇冠
	CString GetEnterRoomGetUserList_VIPLevelBlackDiamond(IUserItem * pUserItem);
	CString GetEnterRoomGetUserList_Crest( IUserItem * pUserItem ) ;
	CString GetEnterRoomGetUserList_Blink( IUserItem * pUserItem ) ;
	CString GetEnterRoomGetUserList_LuckyStar( IUserItem* pUserItem ) ;
	CString GetEnterRoomGetUserList_Event(IUserItem * pUserItem, uint8 unIndex);
	CString GetEnterRoom( IUserItem * pUserItem ) ;
	CString GetLeaveRoom( IUserItem* pUserItem ) ;
	CString GetVipName( uint8 nVipLevel ) ;
	CString GetTitleName( uint16 nTitleType ) ;
	CString GetSpeaker(CString& strText, MessageFont& stFont, SpeakerInfo& stInfo);
	CString GetSpeakerLucky( CString& strSrcName ,CString& strSrcLuckyHead , uint32  src179ID , 
		uint16	unLuckyTimes , uint32 unLuckyMoney ,uint16 unGiftid ) ;
	CString GetSpeakerLucky(CString& strText, SpeakerInfo& stInfo, uint16 unGiftID);
	CString GetSpeakerPlutusAward(CString& strText, SpeakerInfo& stInfo, uint16 unGiftID);
	CString GetSpeakerSystem(CString& strText);
	CString GetSpeakerGame(CString& strText);
	CString GetSpeakerLuxury(SpeakerInfo& stSrcInfo, SpeakerInfo& stDstInfo, CString& strRoomName, uint16 unGiftID, uint16 unCount);
	CString GetWelecomeWords( IUserItem* pOwnerItem , CString& strText , MessageFont& stFont ) ; //房主在线时的欢迎辞
	CString GetActorAutoSpeakWords(IUserItem* pUserItem, CString& strText); //艺人自动发言词
	CString GetAgentAutoSpeakWords(IUserItem* pUserItem, CString& strText);//销售自动发言词
	CString OnNotifyPlutusAward( uint32 srcuin , uint32 un179ID , std::wstring& strDecodeName  , 
		std::wstring strRoomName ,uint16 nSelfTitleLevel , int nNumber , uint16 unGiftID ) ;			//财神到
	CString GetUserHead( IUserItem* pUserItem ) ;			//获取显示名字前的信息
	CString GetSpeakerFirework(CString& strText,SpeakerInfo& stInfo);
	CString GetSpeakerSalute(CString& strText, SpeakerInfo& stInfo);
	CString GetUserItemHeadImg( IUserItem* pUserItem , BOOL bStamp = TRUE ) ; //获得用户名前的章和冠图片
	CString GetUserItemRichLevelImg(IUserItem* pUserItem); //获得用户的富豪等级图标
	CString GetAutoWelcome( IUserItem* pUserItem , CString& strText ) ;
	CString GetVisistorRegistor( IUserItem* pUserItem , CString& strText ) ;

	CString GetGiftDateString( uint32 unTime ) ;
	CString GetPublicFlyText( IUserItem * pUserItem, CString strText);
	CString GetLuckyStarEnterRoom( IUserItem* pUserItem ) ;
	CString GetLuckyStarBirth() ; //超级幸运星诞生
	CString GetSofaUserHead(CSofaItem * pSofaItem);
	CString ChatUserItem( IUserItem* pUserItem ) ;
	CString ChatSofaUserItem(CSofaItem * pSofaItem);
	CString GetNameAndID(IUserItem * pUserItem);			//用户名+179ID，179ID大于6位不显示
	CString GetRoomMidInputDefaultText(CString& strDefault);
	CString GetMarkInvite(CString & strSongerName);
	CString GetMarkTips( const CString& strTitle , const CString& strValue , const CString& strColor=COLOR_NORMAL ) ;
private:
	CString ChatUser( uint32 nSelfUUID , IUserItem* pUserItem , bool bReplaceSelf  ) ;
	//CString ChatUserItem( IUserItem* pUserItem ) ;
	CString StampRightUserItem( IUserItem* pUserItem ) ;	
	CString PrivateUserItem( IUserItem* pUserItem , BOOL bStamp = TRUE ) ;
	CString ChatMessage( CString& strText , MessageFont& stFont , uint32 srcUin, bool bAutoMsg = FALSE) ;
	CString Img( CString strImgPath ) ;
	CString ItaLic( const CString& strValue ) ;
	CString UnderLine(const CString& strValue ) ;
	CString Bold(const CString& strValue ) ;
	CString Span(int nFontSize , const CString& strColor ,const CString& strValue ) ;
	CString Span(int nFontSize ,const CString& strValue ) ;
	CString Font(const CString& strFont , const CString& strColor , const CString& strValue ) ;
	CString Marquee(const CString& strValue ) ;
	CString SpeakerUserItem( CString& strSrcName , CString& strSrcHead , uint32 un179ID ,COLORREF cf = RGB(0,153,255) ) ;
	CString SpeakerUserItem( SpeakerInfo& stInfo, COLORREF cf = RGB(0,153,255) ) ;
	CString SpeakerMessage( CString& strText , MessageFont& stFont );
	CString SpeakerSystemMessage( CString& strText );
	CString SpeakerLuckyMessage( CString& strText );
	CString SystemMessageTitle();
	CString SpeakerFireworkTitle();
	CString SpeakerSaluteTitle();

	CString GetColorString( COLORREF cf ) ;			//颜色转换成字符串
	CString GetCurrentTimeString() ;				//得到时间

	CString GetFontText( MessageFont& stFont ,const CString& strText ) ; 
	CString GetLuckyTimesPngPath( uint16 unLuckyTimes ) ;
	RoomBusiness* m_pRoomBusiness ;
};