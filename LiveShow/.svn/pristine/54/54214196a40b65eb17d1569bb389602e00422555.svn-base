#pragma once

#include "51ktv.h"

namespace core
{
	enum
	{
		EVENT_VALUE_CORE_MODULE_BEGIN = EVENT_VALUE_CORE_BEGIN,		
		
		EVENT_VALUE_CORE_LOGIN,
		EVENT_VALUE_CORE_LOGIN_RSP,

		EVENT_VALUE_CORE_LOGOUT,//没有额外参数

		EVENT_VALUE_CORE_GET_SELFINFO,//没有额外参数
		EVENT_VALUE_CORE_GET_SELFINFO_RSP,//没有额外参数

		EVENT_VALUE_CORE_SET_SELFINFO,//暂时没用到
		EVENT_VALUE_CORE_SET_SELFINFO_RSP,//param0:ENM_SetSelfInfoResult 暂时没用到
		EVENT_VALUE_CORE_SET_SELFINFO_NOTIFY,//暂时没用到

		EVENT_VALUE_CORE_BE_KICKED_NOTIFY,
		EVENT_VALUE_CORE_DISCONNECTED_NOTIFY,

		EVENT_VALUE_CORE_ENTER_ROOM,
		EVENT_VALUE_CORE_ENTER_ROOM_RSP,
		EVENT_VALUE_CORE_ENTER_ROOM_NOTIFY,

		EVENT_VALUE_CORE_LEAVE_ROOM,//param0:roomid
		EVENT_VALUE_CORE_LEAVE_ROOM_NOTIFY,//param0:uin param1:roomid

		EVENT_VALUE_CORE_GET_USERLIST,//param0:roomid
		EVENT_VALUE_CORE_GET_USERLIST_RSP,

		EVENT_VALUE_CORE_GET_WAITTINGLIST,//param0:roomid
		EVENT_VALUE_CORE_GET_WAITTINGLIST_RSP,

		EVENT_VALUE_CORE_SEND_MESSAGE,
		EVENT_VALUE_CORE_SEND_MESSAGE_RSP,
		EVENT_VALUE_CORE_SEND_MESSAGE_NOTIFY,

		EVENT_VALUE_CORE_SEND_GIFT,
		EVENT_VALUE_CORE_SEND_GIFT_RSP,
		EVENT_VALUE_CORE_SEND_GIFT_NOTIFY,

		EVENT_VALUE_CORE_SEND_LOUDSPEAKER,
		EVENT_VALUE_CORE_SEND_LOUDSPEAKER_RSP,
		EVENT_VALUE_CORE_SEND_LOUDSPEAKER_NOTIFY,

		EVENT_VALUE_CORE_SEND_FLYWORDS,
		EVENT_VALUE_CORE_SEND_FLYWORDS_RSP,
		EVENT_VALUE_CORE_SEND_FLYWORDS_NOTIFY,

		EVENT_VALUE_CORE_REQUEST_SHOW,//param0:roomid param1:ENM_RequestShowType
		EVENT_VALUE_CORE_REQUEST_SHOW_RSP,
		EVENT_VALUE_CORE_REQUEST_SHOW_NOTIFY,

		EVENT_VALUE_CORE_EXIT_SHOW,//param0:roomid param1:type(0:1v1 1:其他)
		EVENT_VALUE_CORE_EXIT_SHOW_NOTIFY,//param0:roomid param1:srcuin param2:type(0:1v1 1:其他)

		EVENT_VALUE_CORE_OPERATE_VIDEO_AUDIO,//param0:roomid param1:ENM_OperateAVType param2:dstuin
		EVENT_VALUE_CORE_OPERATE_VIDEO_AUDIO_RSP,
		EVENT_VALUE_CORE_OPERATE_VIDEO_AUDIO_NOTIFY,//param0:roomid param1:ENM_OperateAVType param2:srcuin

		EVENT_VALUE_CORE_KICK_USER,
		EVENT_VALUE_CORE_KICK_USER_RSP,
		EVENT_VALUE_CORE_KICK_USER_NOTIFY,	

		EVENT_VALUE_CORE_ADD_BLACK,//param0:roomid param1:dstuin
		EVENT_VALUE_CORE_ADD_BLACK_RSP,//param0:roomid param1:ENM_KickResult param2:dstuin
		EVENT_VALUE_CORE_ADD_BLACK_NOTIFY,//param0:roomid param1:srcuin param2:dstuin

		EVENT_VALUE_CORE_LOCK_IP,
		EVENT_VALUE_CORE_LOCK_IP_RSP,//param0:roomid param1:ENM_KickResult param2:dstuin
		EVENT_VALUE_CORE_LOCK_IP_NOTIFY,

		EVENT_VALUE_CORE_SHUT_UP,//param0:roomid param1:dstuin
		EVENT_VALUE_CORE_SHUT_UP_RSP,//param0:roomid param1:ENM_KickResult param2:dstuin
		EVENT_VALUE_CORE_SHUT_UP_NOTIFY,

		EVENT_VALUE_CORE_UNLOCK_SHUTUP,//param0:roomid param1:dstuin
		EVENT_VALUE_CORE_UNLOCK_SHUTUP_RSP,//param0:roomid param1:ENM_KickResult param2:dstuin
		EVENT_VALUE_CORE_UNLOCK_SHUTUP_NOTIFY,//param0:roomid param1:srcuin param2:dstuin

		EVENT_VALUE_CORE_PUSH_ON_SHOW,//param0:roomid param1:dstuin param2:index
		EVENT_VALUE_CORE_PUSH_ON_SHOW_RSP,
		EVENT_VALUE_CORE_PUSH_ON_SHOW_NOTIFY,

		EVENT_VALUE_CORE_PULL_OFF_SHOW,//param0:roomid param1:dstuin
		EVENT_VALUE_CORE_PULL_OFF_SHOW_RSP,//param0:roomid param1:ENM_PullOffResult param2:dstuin
		EVENT_VALUE_CORE_PULL_OFF_SHOW_NOTIFY,//param0:roomid param1:srcuin param2:dstuin

		EVENT_VALUE_CORE_ADD_TITLE,//param0:roomid param1:ENM_TitleLevel param2:dstuin
		EVENT_VALUE_CORE_ADD_TITLE_RSP,
		EVENT_VALUE_CORE_ADD_TITLE_NOTIFY,

		EVENT_VALUE_CORE_REMOVE_TITLE,//param0:roomid param1:ENM_TitleLevel param2:dstuin
		EVENT_VALUE_CORE_REMOVE_TITLE_RSP,
		EVENT_VALUE_CORE_REMOVE_TITLE_NOTIFY,

		EVENT_VALUE_CORE_OPEN_PRIVATE,//param0:roomid param1:dstuin
		EVENT_VALUE_CORE_OPEN_PRIVATE_RSP,//param0:roomid param1:ENM_OpenPrivateResult param2:dstuin

		EVENT_VALUE_CORE_CLOSE_PRIVATE,//param0:roomid param1:dstuin

		EVENT_VALUE_CORE_REQUEST_P2P,//param0:roomid param1:dstuin
		EVENT_VALUE_CORE_REQUEST_P2P_RSP,//param0:roomid param1:ENM_ConnectP2PResult param2:dstuin
		EVENT_VALUE_CORE_REQUEST_P2P_NOTIFY,//param0:roomid param1:srcuin

		EVENT_VALUE_CORE_RESPONSE_P2P,//param0:roomid param1:ENM_ResponseP2PType param2:dstuin
		EVENT_VALUE_CORE_RESPONSE_P2P_RSP,
		EVENT_VALUE_CORE_RESPONSE_P2P_NOTIFY,

		EVENT_VALUE_CORE_REPORT_CLIENTINFO,
		EVENT_VALUE_CORE_REPORT_CLIENTINFO_NOTIFY,

		EVENT_VALUE_CORE_QUERY_CLIENTINFO,
		EVENT_VALUE_CORE_QUERY_CLIENTINFO_RSP,

		EVENT_VALUE_CORE_SET_ROOMINFO,
		EVENT_VALUE_CORE_SET_ROOMINFO_RSP,//param0:roomid param1:ENM_SetRoomInfoResult
		EVENT_VALUE_CORE_SET_ROOMINFO_NOTIFY,

		EVENT_VALUE_CORE_GET_ROOMINFO,//param0:roomid
		EVENT_VALUE_CORE_GET_ROOMINFO_RSP,

		EVENT_VALUE_CORE_MIC_SET_MIC_STATUS,//param0:roomid param1: (high16:mic_index(1,2,3) low16:status(0:open 1:close))
		EVENT_VALUE_CORE_MIC_SET_MIC_STATUS_RSP,//param0:roomid param1: (high16:mic_index(1,2,3) low16:status(0:open 1:close)) param2:ENM_SetMicStatusResult
		EVENT_VALUE_CORE_MIC_SET_MIC_STATUS_NOTIFY,//param0:roomid param1: (high16:mic_index(1,2,3) low16:status(0:open 1:close)) param2:srcuin

		EVENT_VALUE_CORE_ANSWER_PUSH_ON_SHOW,
		EVENT_VALUE_CORE_ANSWER_PUSH_ON_SHOW_RSP,

		EVENT_VALUE_CORE_REPORT_COLLECT_INFO,

		EVENT_VALUE_CORE_GET_TASK_REWARD,//获取任务奖励 param0:taskID

		EVENT_VALUE_CORE_GET_FANS,//param0:uin
		EVENT_VALUE_CORE_GET_FANS_RSP,

		EVENT_VALUE_CORE_GET_SOFA_LIST,//param0:roomid
		EVENT_VALUE_CORE_GET_SOFA_LIST_RSP,

		EVENT_VALUE_CORE_BUY_SOFA,//param0:roomid param1:sofaindex(1开始) param2:price(单位沙发)
		EVENT_VALUE_CORE_BUY_SOFA_RSP,
		EVENT_VALUE_CORE_BUY_SOFA_NOTIFY,

		//////////////////////////////////////////////////////////////////////////
		//点歌系统

		EVENT_VALUE_UPDATE_SONGLIST,
		EVENT_VALUE_UPDATE_SONGLIST_RSP,

		EVENT_VALUE_GET_SONGLIST,//param0:roomid param1:songerUin
		EVENT_VALUE_GET_SONGLIST_RSP,

		EVENT_VALUE_ORDER_SONG,
		EVENT_VALUE_ORDER_SONG_RSP,//param0:roomid param1:ENM_OrderSongResult param2:songeruin
		EVENT_VALUE_ORDER_SONG_NOTIFY,

		EVENT_VALUE_PROCESS_ORDER,//param0:roomid param1:orderId param2:1(接受) 2(拒绝)
		EVENT_VALUE_PROCESS_ORDER_RSP,//param0:roomid param1:result 0(成功) 1(失败) 2(15秒)
		EVENT_VALUE_PROCESS_ORDER_NOTIFY,

		EVENT_VALUE_GET_ORDERLIST,//param0:roomid
		EVENT_VALUE_GET_ORDERLIST_RSP,

		EVENT_VALUE_MARK_SONG,//param0:roomid param1:orderId param2:1(好评) 2(差评) 0(弃评)
		EVENT_VALUE_MARK_SONG_RSP,//param0:roomid param1:result
		EVENT_VALUE_MARK_SONG_NOTIFY,

		//////////////////////////////////////////////////////////////////////////
		//开宝箱

		EVENT_VALUE_CORE_OPEN_BOX_REQ,//param0:roomid param1:isfree
		EVENT_VALUE_CORE_OPEN_BOX_RSP,

		EVENT_VALUE_CORE_REWARD_BOX_KEY_NOTIFY,

		//////////////////////////////////////////////////////////////////////////
		//离线主播列表

		EVENT_VALUE_CORE_GET_OFFLINE_ACTOR_REQ,//param0:roomid
		EVENT_VALUE_CORE_GET_OFFLINE_ACTOR_RSP,

		//////////////////////////////////////////////////////////////////////////

		EVENT_VALUE_CORE_NOTIFY_ROOM_USER_INFO_UPDATE,
		EVENT_VALUE_CORE_NOTIFY_GIFT_RESOURCE_UPDATE,//礼物资源有更新
		EVENT_VALUE_CORE_NOTIFY_SYSTEM_MESSAGE,//大厅通知消息
		EVENT_VALUE_CORE_NOTIFY_GIFT_OVER_TIME,//礼物到期 param0:roomid param1:dstuin param2:giftid
		EVENT_VALUE_CORE_NOTIFY_PRIVATE_SHOW,
		EVENT_VALUE_CORE_NOTIFY_PLUTUS_AWARD,//财神到
		EVENT_VALUE_CORE_NOTIFY_MONEY_UPDATE,//钱币更新 param0:money
		EVENT_VALUE_CORE_NOTIFY_OFFICIAL_NOTICE,//官方公告
		EVENT_VALUE_CORE_NOTIFY_ROOM_LOCKED,//房间被封
		EVENT_VALUE_CORE_NOTIFY_PASSWORD_SAFE,//用户设置了密保
		EVENT_VALUE_CORE_NOTIFY_MY_INFO_UPDATE,//我的信息变更(昵称，性别)
		EVENT_VALUE_CORE_NOTIFY_MEDIA_SERVER_RESET,//媒体中转服务器变更
		EVENT_VALUE_CORE_NOTIFY_RICH_STAR,//财富宝座变更
		EVENT_VALUE_CORE_NOTIFY_RICH_UPDATE_MONEY_CHANGE,//富豪升级钱币更新
		EVENT_VALUE_CORE_NOTIFY_TASK_INFO_UPDATE,//任务系统更新
		EVENT_VALUE_CORE_NOTIFY_CLEAR_SOFA_LIST,//清空沙发列表 param0:roomid

		EVENT_VALUE_CORE_ASYN_LOGOUT,//(在m_circleBuffer.Read()的调用栈中)异步Logout
		EVENT_VALUE_CORE_RELOGIN,//自动重连

		EVENT_VALUE_CORE_MODULE_END,
	};

	enum
	{
		MESSAGE_VALUE_CORE_MODULE_BEGIN = MESSAGE_VALUE_CORE_BEGIN,

		MESSAGE_VALUE_CORE_PRE_LOGIN,//准备要登陆了
		MESSAGE_VALUE_CORE_BE_KICKED,//大厅被踢
		MESSAGE_VALUE_CORE_DISCONNECTED,//断线了
		MESSAGE_VALUE_CORE_LOGIN_COMPLETE,//param0:ENM_LoginErrorCode
		MESSAGE_VALUE_CORE_RICHINFO_COMPLETE,//获取自己的丰富信息完毕
		MESSAGE_VALUE_CORE_ISP_CHANGE,//ISP变更 param0: 0:电信 1:网通
		MESSAGE_VALUE_CORE_GIFT_STAR_INFO,//获取到礼物之星信息
		MESSAGE_VALUE_CORE_MY_GODS,//获取到我的物品
		MESSAGE_VALUE_CORE_MY_GODS_UPDATE,//我的物品变更(暂时不带内容)
		MESSAGE_VALUE_CORE_MY_INFO_UPDATE,//我的信息变更
		MESSAGE_VALUE_CORE_MY_VIPLEVEL_UPDATE,//vip等级变更
		MESSAGE_VALUE_CORE_MY_RICHLEVEL_UPDATE,//富豪等级变更

		MESSAGE_VALUE_CORE_MODULE_END,
	};

	enum
	{
		SERVICE_VALUE_CORE_MODULE_BEGIN = SERVICE_VALUE_CORE_BEGIN,

		SERVICE_VALUE_CORE_GET_CUR_INFO, //param: &CurInfo
		SERVICE_VALUE_CORE_SWITCH_ISP,//切换isp(param: 1:电信 0:网通)
		SERVICE_VALUE_CORE_UPDATE_MONEY,//param: int 正表示加钱，负表示减钱
		SERVICE_VALUE_CORE_GET_GIFT_STAR_INFO,//param: &GetGiftStarInfo
		SERVICE_VALUE_CORE_GET_MY_GOODS,//param: &GetMyGoods

		SERVICE_VALUE_CORE_END,
	};

	enum ENM_AccountType
	{
		ACCOUNTTYPE_VISITOR	=	0x00,
		ACCOUNTTYPE_179		=	0x01,
		ACCOUNTTYPE_QQ		=	0x02,
	};

	struct RichInfo
	{
		uint16 unYear;
		uint8 unMonth;
		uint8 unDay;
		std::wstring strProvince;
		std::wstring strCity;
	};

	enum ENM_VipLevel
	{
		VIPLEVEL_NONE			=	0x00,	//未注册
		VIPLEVEL_REGISTER		=	0x01,	//已注册
		VIPLEVEL_VIP			=	0x02,	//vip
		VIPLEVEL_HONOR			=	0x04,	//荣耀vip
		VIPLEVEL_SILVER			=	0x08,	//银冠
		VIPLEVEL_GOLDEN			=	0x10,	//金冠
		VIPLEVEL_CRYSTAL		=	0x20,	//水晶皇冠
		VIPLEVEL_DIAMOND		=	0x40,	//钻石皇冠
		VIPLEVEL_BLACKDIAMOND	=	0x60,	//黑钻皇冠
		VIPLEVEL_TOP			=	0x80,	//至尊皇冠
	};

	enum ENM_BusinessType
	{
		BUSINESSTYPE_SONGER		=	0x0001,	//人气歌手
		BUSINESSTYPE_DANCER		=	0x0002,	//热舞宝贝
		BUSINESSTYPE_HOST		=	0x0004,	//闪亮主播
		BUSINESSTYPE_OPERATOR	=	0x0008,	//线上运营
		BUSINESSTYPE_AGENT		=	0x0010,	//官方代理
		BUSINESSTYPE_PATROLMAN	=	0x0020,	//巡管
		BUSINESSTYPE_AD_CLEANER	=	0x0040,	//广告处理
		BUSINESSTYPE_LUCKY_STAR	=	0x0080,	//超级幸运星
	};

	enum ENM_TitleLevel
	{
		TITLELEVEL_NONE				=	0x00,
		TITLELEVEL_TEMP_MANAGER		=	0x01,
		TITLELEVEL_MANAGER			=	0x02,
		TITLELEVEL_SECOND_OWNER		=	0x04,
		TITLELEVEL_OWNER			=	0x08,
		TITLELEVEL_SUPER_MANAGER	=	0x10,
	};

	enum ENM_InfoType
	{
		INFOTYPE_NAME				=	0x00,	//string
		INFOTYPE_179ID				=	0x01,	//uint32
		INFOTYPE_GENDER				=	0x02,	//uint8
		INFOTYPE_VIP_LEVEL			=	0x03,	//uint8
		INFOTYPE_USR_LEVEL			=	0x04,	//uint8
		INFOTYPE_ONLINE_TIME		=	0x05,	//uint16
		INFOTYPE_LEFT_TIME			=	0x06,	//uint16
		INFOTYPE_BIRTHDAY_YEAR		=	0x07,	//uint16
		INFOTYPE_BIRTHDAY_MONTH		=	0x08,	//uint8
		INFOTYPE_BIRTHDAY_DAY		=	0x09,	//uint8
		INFOTYPE_HOME_PROVINCE		=	0x0a,	//string
		INFOTYPE_HOME_CITY			=	0x0b,	//string
		INFOTYPE_SELF_SETTING		=	0x0c,	//uint32
		INFOTYPE_RICH_LEVEL			=	0x0d,	//uint8

		//room相关字段(for notify)
		USERINFO_ROOM_TITLELEVEL	=	0x20,	//uint16（部分或全部走单独的通知，***任命***为临管）
		USERINFO_ROOM_JIAZU			=	0x21,	//string
		USERINFO_ROOM_CLIENTINFO	=	0x22,	//uint16
		USERINFO_ROOM_STATUS		=	0x23,	//uint16（部分或全部走单独的通知，***取消***公麦？）
	};

	enum ENM_SelfSetting
	{
		SELFSETTING_MESSAGE_NO_PUBLIC				=	0x0001,//屏蔽公聊(0:不屏蔽)
		SELFSETTING_MESSAGE_NO_PRIVATE				=	0x0002,//屏蔽私聊(0:不屏蔽)
		SELFSETTING_MESSAGE_AUTO_SCROLL				=	0x0004,//消息自动滚屏(0:滚屏)
		SELFSETTING_MESSAGE_AUTO_REPLY				=	0x0008,//消息自动回复(0:不回复)
		SELFSETTING_MESSAGE_NO_LOUDSPEAKER_AUDIO	=	0x0010,//屏蔽喇叭声音(0:不屏蔽)
		SELFSETTING_MESSAGE_NO_GIFT					=	0x0020,//屏蔽礼物消息(0:不屏蔽)
		SELFSETTING_MESSAGE_NO_ENTER_LEAVE			=	0x0040,//屏蔽用户进出消息(0:屏蔽)		

		SELFSETTING_P2PSHOW_MASK					=	0x0180,//1对1视频设置mask
		SELFSETTING_P2PSHOW_VIP						=	0x0180,//1对1视频设置->vip以上
		SELFSETTING_P2PSHOW_KING					=	0x0100,//1对1视频设置->皇冠
		SELFSETTING_P2PSHOW_CLOSE					=	0x0080,//1对1视频设置->拒绝任何人

		SELFSETTING_MESSAGE_LOUDSPEAKER_SCROLL		=	0x0200,//喇叭自动滚屏(0:滚屏)
		SELFSETTING_MESSAGE_NO_LUCKY_AUDIO			=	0x0400,//屏蔽中奖声音(0:不屏蔽)

		//从这里开始加新的
	};

	enum ENM_TaskData
	{
		TASK_SYSTEM_TASKNUM			=	 9,	  //任务个数

		TASKSTATTYPE_UNOPEN			= 0x00,   //未开启
		TASKSTATTYPE_UNFINISH		= 0x01,   //未完成
		TASKSTATTYPE_CANNOT_REWARD 	= 0x02,   //已完成，不可领取状态
		TASKSTATTYPE_CAN_REWARD		= 0x03,   //已完成，可领取奖励状态
		TASKSTATTYPE_ISREWARD		= 0x04,   //已完成，已领取奖励状态
		TASKSTATTYPE_ERROR   	 	= 0x10,   //错误类型
	};
	
	struct CurInfo
	{
		uint32 unUIN;		
		uint32 un179id;
		std::wstring str179EMail;		
		bool bMan;
		std::wstring strName;
		std::wstring strToken;
		bool bLogined;
		ENM_AccountType enmAccountType;		//登陆的账号类型
		std::wstring strLoginAccount;		//登陆时填的账号(179id、179账号（邮箱）)

		ENM_VipLevel enmVIPLevel;
		uint8 unUserLevel;
		uint16 unOnlineTime;			//在线时间
		uint16 unLeftTime;				//升级剩余时间
		uint32 unLeftMoney;				//剩余币
		uint32 unSelfSetting;			//个性设置(ENM_SelfSetting)
		uint32 unSystemTime;			//系统时间
		uint32 unBusiness;				//ENM_BusinessType

		bool bFirstLogin;//是否首次登陆
		bool bRmbUser;//是否充值过
		bool bPasswordSafe;//是否设置过密保		

		uint8 unRichLevel;			//富豪等级
		uint32 unRichUpdateMoney;	//富豪升级所需币

		uint8 unarrTaskStat[TASK_SYSTEM_TASKNUM];//9个任务状态ENM_TaskStatType

		bool bGetRichInfo;
		RichInfo stRichInfo;

		bool bUseDianxin;//当前是否使用电信服务器
		bool bInBlacklist;//是否在黑名单中(禁止发言)

		CurInfo():unUIN(0),bLogined(false),bGetRichInfo(false),enmVIPLevel(VIPLEVEL_NONE),
			bUseDianxin(true),bInBlacklist(false)
		{
			memset(unarrTaskStat, 0, sizeof(unarrTaskStat));
		}

		void Init()
		{
			unUIN = un179id = 0;
			bLogined = bGetRichInfo = bInBlacklist = false;
			bUseDianxin = true;
			memset(unarrTaskStat, 0, sizeof(unarrTaskStat));
		}
	};
	

	//////////////////////////////////////////////////////////////////////////

	struct LoginEvent : public ExtraInfo
	{
		//EVENT_VALUE_CORE_LOGIN

		ENM_AccountType enmAccountType;//登陆的账号类型
		std::wstring strAccount;
		std::string strPassword;//密码md5
		bool bVerify;
		std::string strVerifyKey;
		std::wstring strVerifyCode;

		LoginEvent():bVerify(false)
		{
		}
	};

	/**
	* 登录返回值
	*/
	enum ENM_LoginErrorCode
	{
		LOGIN_ERROR_SUCCESS,					//登陆成功

		LOGIN_ERROR_CLIENT_GET_TOKEN_HOST_ERROR,//token域名解析失败
		LOGIN_ERROR_CLIENT_GET_HALL_HOST_ERROR, //hall域名解析失败
		LOGIN_ERROR_CLIENT_STATE_ERROR,			//状态错误，请勿重复登陆(客户端bug)
		LOGIN_ERROR_CLIENT_TOKEN_CONNECT_ERROR,	//token连接失败
		LOGIN_ERROR_CLIENT_TOKEN_TIMEOUT_ERROR,	//token登录超时
		LOGIN_ERROR_CLIENT_HALL_CONNECT_ERROR,	//hall连接失败
		LOGIN_ERROR_CLIENT_HALL_TIMEOUT_ERROR,	//hall登录超时
		LOGIN_ERROR_CLIENT_NET_TYPE_ERROR,		//获取网络类型出错(测速未返回数据)

		LOGIN_ERROR_TOKEN_ERROR,				//账号认证失败
		LOGIN_ERROR_TOKEN_PASSWORD_ERROR,		//密码错误
		LOGIN_ERROR_TOKEN_ACCOUNT_ERROR,		//账号错误
		LOGIN_ERROR_TOKEN_ACCOUNT_FORBIDE,		//您的账号已被锁定,如有疑问,请联系客服
		LOGIN_ERROR_TOKEN_REFUSED,			    //(依据token服务器返回)
		LOGIN_ERROR_TOKEN_NEED_VERIFY,		    //需要验证码验证
		LOGIN_ERROR_TOKEN_VERIFY_ERROR,			//验证码验证失败
		LOGIN_ERROR_TOKEN_VERIFY_TIMEOUT,	    //验证码过期
		LOGIN_ERROR_TOKEN_EMAIL_NOT_ACTIVATE,   //邮箱未激活，请激活后再登录
		LOGIN_ERROR_TOKEN_OTHER_ERROR,			//其他错误

		LOGIN_ERROR_179_ERROR_BEGIN		= 	1000,
		LOGIN_ERROR_179_ACCOUNT_LOCK	= 	LOGIN_ERROR_179_ERROR_BEGIN+0x01,	//账号被锁定
		LOGIN_ERROR_179_IP_LOCK			= 	LOGIN_ERROR_179_ERROR_BEGIN+0x02,	//ip被锁定
		LOGIN_ERROR_179_MAC_LOCK		= 	LOGIN_ERROR_179_ERROR_BEGIN+0x03,	//mac被锁定
		LOGIN_ERROR_179_TIMEOUT			= 	LOGIN_ERROR_179_ERROR_BEGIN+0x04,	//登陆超时
		LOGIN_ERROR_179_VERSION_ERROR	= 	LOGIN_ERROR_179_ERROR_BEGIN+0x05,	//版本不匹配
		LOGIN_ERROR_179_OTHER			= 	LOGIN_ERROR_179_ERROR_BEGIN+0x10,	
		LOGIN_ERROR_179_MAINTAN			= 	LOGIN_ERROR_179_ERROR_BEGIN+0x11,	//服务器在维护
		LOGIN_ERROR_179_UNKNOWN			= 	LOGIN_ERROR_179_ERROR_BEGIN+0xff,
	};

	struct LoginRspEvent : public ExtraInfo
	{
		//EVENT_VALUE_CORE_LOGIN_RSP

		ENM_LoginErrorCode enmLoginResult;
		std::wstring strFailReason;
		uint16 lockDays;//被封天数
		std::wstring strUnlockDate;//解封日期
		uint32 latestversion;
		uint32 lastlogin_time_high,lastlogin_time_low;
	};

	//////////////////////////////////////////////////////////////////////////

	struct SetSelfInfoEvent : public ExtraInfo
	{
		//EVENT_VALUE_CORE_SET_SELFINFO
		//EVENT_VALUE_CORE_SET_SELFINFO_NOTIFY

		std::map<ENM_InfoType, uint8>			uint8Map;
		std::map<ENM_InfoType, uint16>			uint16Map;
		std::map<ENM_InfoType, uint32>			uint32Map;
		std::map<ENM_InfoType, std::wstring>	stringMap;
	};

	//EVENT_VALUE_CORE_SET_SELFINFO_RSP

	enum ENM_SetSelfInfoResult
	{
		SETSELFINFO_OK				=	0x00,	//成功
		SETSELFINFO_OK_NOT_ALLOWED	=	0x01,	//有字段不允许修改
		SETSELFINFO_OK_UNKNOWN		=	0xff,
	};

	//////////////////////////////////////////////////////////////////////////

	struct BeKickedNotifyEvent : public ExtraInfo
	{
		//EVENT_VALUE_CORE_BE_KICKED_NOTIFY

		enum ENM_KICK_RESULT
		{
			KICK_RE_LOGIN		= 0x00,	//账号重登陆
			KICK_SEND_AD		= 0x01,	//房间发广告
			KICK_ACCOUNT_LOCK	= 0x10,	//账号被锁定 +lockDays+strUnlockDate
			KICK_IP_LOCK		= 0x11,	//ip被锁定 +lockDays+strUnlockDate
			KICK_MAC_LOCK		= 0x12,	//mac被锁定 +lockDays+strUnlockDate
			KICK_OTHER			= 0xff, //其他
		};

		ENM_KICK_RESULT result;
		std::wstring strReason;
		uint16 lockDays;
		std::wstring strUnlockDate;//解封日期
	};

	//////////////////////////////////////////////////////////////////////////

	enum ENM_UserStatusType
	{
		USERSTATUSTYPE_PUBLIC_1		=	0x01,	//1号公麦
		USERSTATUSTYPE_PUBLIC_2		=	0x02,	//2号公麦
		USERSTATUSTYPE_PUBLIC_3		=	0x04,	//3号公麦
		USERSTATUSTYPE_PRIVATE		=	0x08,	//私麦
		USERSTATUSTYPE_P2P			=	0x10,	//一对一视频
		USERSTATUSTYPE_FORBIDDEN	=	0x20,	//被禁言
		USERSTATUSTYPE_VIDEO		=	0x40,	//视频打开
		USERSTATUSTYPE_AUDIO		=	0x80,	//音频打开
	};

	struct ClientInfo
	{
		//								 最低位	
		//  f e d c b a 9 8 7 6 5 4 3 2 1 0
		//	* * * * * * * * * * * * * * * *
		//								| |  -> 客户端类型
		//                            | -> 是否有摄像头
		//                          | -> 是否隐身登陆


	public:
		ClientInfo():data(0){}

		ClientInfo(bool haveCamera)
		{
			data = 0;
			if(haveCamera)
			{
				data |= 0x04;
			}
		}

		void setData(uint16 data)
		{
			this->data = data;
		}

		operator uint16()
		{
			return data;
		}

		void setHideLogin()
		{
			data |= 0x08;
		}

		bool isHideLogin()
		{
			return (data & 0x08) != 0;
		}

		bool haveCamera()
		{
			return (data & 0x04) != 0;
		}

	private: 
		uint16 data;
	};

	struct room_user_info
	{
		uint32 unUIN;
		bool bMan;
		uint32 un179id;
		std::wstring strName;
		uint8 unVipLevel;
		uint8 unUsrLevel;
		uint8 unRichLevel;
		uint16 unTitleLevel;
		uint32 unBusiness; //ENM_BusinessType
		ClientInfo stClientInfo;
		uint16 unStatus;	//ENM_UserStatusType
		std::vector<uint16> vecItemID;
	};
	
	//////////////////////////////////////////////////////////////////////////

	struct EnterRoomEvent : public ExtraInfo
	{
		//EVENT_VALUE_CORE_ENTER_ROOM

		uint32 roomid;
		ClientInfo stClientInfo;
		std::wstring password;
	};

	struct EnterRoomRspEvent : public ExtraInfo
	{
		//EVENT_VALUE_CORE_ENTER_ROOM_RSP

		enum ENM_EnterRoomResult
		{
			ENTERROOM_ERROR_OK					=	0x00,
			ENTERROOM_ERROR_NOT_EXIST			=	0x01,
			ENTERROOM_ERROR_ROOM_FULL			=	0x02,
			ENTERROOM_ERROR_BLACKLIST			=	0x03,	//在黑名单中
			ENTERROOM_ERROR_KICKED				=	0x04,	//被踢，后接unKickTimeLeft
			ENTERROOM_ERROR_NEED_PASSWORD		=	0x05,
			ENTERROOM_ERROR_PASSWORD_WRONG		=	0x06,
			ENTERROOM_ERROR_MAX_ROOM_NUM		=	0x07,	//超过最大进房数，后接unMaxRoomNum
			ENTERROOM_ERROR_ALREADY_IN_ROOM		= 	0x08,	//已经在房间里
			ENTERROOM_ERROR_TIMEOUT				=	0x09,	//超时
			ENTERROOM_ERROR_OTHER				=	0x0a,	
			ENTERROOM_ERROR_IP_LOCKED			=	0x0b,	//ip被封，后接unKickTimeLeft
			ENTERROOM_ERROR_CLOSED				=	0x0c,	//房间已关闭
			ENTERROOM_ERROR_ROOM_LOCKED			=	0x0d,	//房间被封，后接unLockDaysLeft
			ENTERROOM_ERROR_ROOM_CREATE_FAIL	=	0x0e,
			ENTERROOM_ERROR_SESSION_CREATE_FAIL =	0x10,
			ENTERROOM_ERROR_DECODE_CONNUIN_FAIL =	0x11,
			ENTERROOM_ERROR_GETITEM_TIMEOUT		=	0x12,
			ENTERROOM_ERROR_GETITEM_ERROR		=	0x13,
			ENTERROOM_ERROR_OPNE_ROOM_TOOBIG	=	0x14,	////进入房间达到服务器最大数
			ENTERROOM_ERROR_UNKNOWN				=	0xff,
		};

		uint32 unRoomID;
		ENM_EnterRoomResult enmResult;
		std::wstring strFailReason;
		uint16 unKickTimeLeft;//被踢或被封ip,剩余时间(分钟)
		uint16 unLockDaysLeft;//房间被封,剩余时间(天数)
		uint16 unMaxRoomNum;//当前身份最多进房数
		std::wstring strRoomName;
		std::wstring strSignature;
		uint32 relayIP_dianxin;
		uint16 relayPort_dianxin;
		uint32 relayIP_wangtong;
		uint16 relayPort_wangtong;
		std::wstring strOwnerName;
		uint32 unOwner179ID;
		bool bOwnerMan;
		uint32 unAVKey;
		std::vector<room_user_info> vecPubMic;//公麦主播信息
		bool bCollected;//房间是否收藏
	};

	struct EnterRoomNotifyEvent : public ExtraInfo
	{
		//EVENT_VALUE_CORE_ENTER_ROOM_NOTIFY

		uint32 unRoomID;
		room_user_info stUserInfo;
	};

	//////////////////////////////////////////////////////////////////////////

	struct GetUserListRspEvent : public ExtraInfo
	{
		//EVENT_VALUE_CORE_GET_USERLIST_RSP

		bool bcomplete;
		uint32 roomid;
		std::vector<room_user_info> vecUsrList;
	};

	//////////////////////////////////////////////////////////////////////////

	struct GetWaittinglistRspEvent : public ExtraInfo
	{
		//EVENT_VALUE_CORE_GET_WAITTINGLIST_RSP

		uint32 roomid;
		std::vector<uint32> vecWaittinglist;
	};

	//////////////////////////////////////////////////////////////////////////

	enum ENM_FontStyleType
	{
		FONTSTYLETYPE_NONE		=	0x00,
		FONTSTYLETYPE_BOLD		=	0x01,	//粗体
		FONTSTYLETYPE_UNDERLINE	=	0x02,	//下划线
		FONTSTYLETYPE_ITALICS	=	0x04, 	//斜体
		FONTSTYLETYPE_COLORTEXT	=	0x08,	//彩条
	};

	enum ENM_SendMessageType
	{
		SENDMESSAGETYPE_PUBLIC		=	0x00,	//公屏，dstuin为0表示所有人
		SENDMESSAGETYPE_PRIVATE		=	0x01,	//悄悄话
		SENDMESSAGETYPE_TEMP_NOTICE	=	0x02,	//临时公告
		SENDMESSAGETYPE_NOTICE		=	0x03,	//公告
	};

	enum ENM_SendMessageResult
	{
		SENDMESSAGERESULT_OK				=	0x00,
		SENDMESSAGERESULT_TOO_FAST			=	0x01,	//发送太快
		SENDMESSAGERESULT_PUBLICCHAT_CLOSED	=	0x02,	//公聊被禁止
		SENDMESSAGERESULT_COLORTEXT_CLOSED	=	0x03,	//彩条被禁止
		SENDMESSAGERESULT_USER_SHUTUP		=	0x04,	//用户被禁言
		SENDMESSAGERESULT_NO_PERMISSION		=	0x05,	//没有权限
		SENDMESSAGERESULT_OTHER				=	0x10,
		SENDMESSAGERESULT_PUBLICCHAT_VIP	=	0x11,	//VIP以上可以公聊
		SENDMESSAGERESULT_PUBLICCHAT_REG	=	0x12,	//注册用户以上才可公聊
		SENDMESSAGERESULT_UNKNOWN			=	0xff,
	};

	struct MessageFont
	{
		uint8 font_size;
		uint32 font_color;
		uint8 font_style;//ENM_FontStyleType
		std::wstring font_type;
	};

	struct SendMessageEvent : public ExtraInfo
	{
		//EVENT_VALUE_CORE_SEND_MESSAGE

		uint32 roomid;
		ENM_SendMessageType enmType;
		uint32 dstuin;
		std::wstring message;
		MessageFont stFont;
		bool bIsAutoMsg;//是否是自动回复
		bool bRejectAutoMsg;//拒绝接收自动回复(应答本次消息)
	};

	struct SendMessageRspEvent : public ExtraInfo
	{
		//EVENT_VALUE_CORE_SEND_MESSAGE_RSP

		uint32 roomid;
		ENM_SendMessageResult enmResult;
		std::wstring strFailReason;
		uint32 unLeftTime ;
	};

	struct SendMessageNotifyEvent : public ExtraInfo
	{
		//EVENT_VALUE_CORE_SEND_MESSAGE_NOTIFY

		uint32 roomid;
		ENM_SendMessageType enmType;
		uint32 srcuin;
		uint32 dstuin;
		std::wstring message;
		MessageFont stFont;
		bool bIsAutoMsg;//是否是自动回复
		bool bRejectAutoMsg;//拒绝接收自动回复(应答本次消息)
	};	

	//////////////////////////////////////////////////////////////////////////

	enum ENM_GiftType
	{
		GIFTTYPE_NORMAL			=	0x00, //普通道具
		GIFTTYPE_STAMP			=	0x01, //盖章道具
		GIFTTYPE_SHECHI			=	0x02, //奢侈道具
		GIFTTYPE_CHANGEVOICE	=	0x03, //变声道具
	};

	enum ENM_SendGiftResult
	{
		SENDGIFTRESULT_OK					=	0x00,	//余额能送一部分也算成功
		SENDGIFTRESULT_NOT_ENOUGH_MONEY		=	0x01,	//余额不足
		SENDGIFTRESULT_IS_NOT_EXIST			=   0X02,	//道具不存在
		SENDGIFTRESULT_UPDATE_ASSET_FAILED	=	0X03,	//更新资产失败
		SENDGIFTRESULT_NO_PERMISSION		=	0X04,	//没有权限
		SENDGIFTRESULT_CAN_NOT_SEND			=	0x05,	//不满足使用条件
		SENDGIFTRESULT_UPDATEASSET_TIMEOUT	=   0x06 ,	//更新资产超时
		SENDGIFTRESULT_ALL_REBOT			=	0x07,	//都是机器人
		SENDGIFTRESULT_CAN_NOT_SEND2VISITOR =	0x08,	//不能对游客使用道具
		SENDGIFTRESULT_CAN_NOT_SEND2MANAGE  =	0x09,	//不能对超管和巡官使用道具
		SENDGIFTRESULT_OTHER				=	0x10, 	
		SENDGIFTRESULT_UNKNOWN				=	0xff,
	};

	enum ENM_GiftDstType
	{
		GIFTDSTTYPE_SINGLE	=	0x00,	//单个人+dstuin
		GIFTDSTTYPE_MANAGER	=	0x01,	//管理员
		GIFTDSTTYPE_ALL		=	0x02,	//所有人
	};

	struct SendGiftEvent : public ExtraInfo
	{
		//EVENT_VALUE_CORE_SEND_GIFT

		uint32			unRoomID;
		ENM_GiftType	enmType;
		uint16			unGiftID;
		uint32			unCount;//请求送出的数量
		ENM_GiftDstType	enmDstType;
		uint32			unDstUIN;//为0表示所有人
		std::wstring	strAttachMessage;//赠言
	};

	struct SendGiftLuckyInfo
	{
		uint16	unLuckyIndex;
		uint16	unLuckyTimes;
		uint32	unLuckyMoney;
	};

	struct SendGiftRspEvent : public ExtraInfo
	{
		//EVENT_VALUE_CORE_SEND_GIFT_RSP

		uint32				unRoomID;
		ENM_SendGiftResult	enmResult;
		std::wstring		strFailReason;
		uint32				unTime;
		ENM_GiftType		enmType;
		uint16				unGiftID;
		uint32				unSentCount;//实际送出的数量
		ENM_GiftDstType		enmDstType;
		uint32				unDstUIN;
		std::wstring		strAttachMessage;//赠言
		uint32				unLeftMoney;
		uint32				unRichUpdateMoney;
		std::vector<SendGiftLuckyInfo> vecLuckyInfo;
	};

	struct SendGiftNotifyEvent : public ExtraInfo
	{
		//EVENT_VALUE_CORE_SEND_GIFT_NOTIFY

		uint32			unRoomID;
		uint32			unTime;
		ENM_GiftType	enmType;
		uint16			unGiftID;
		uint32			unSentCount;//实际送出的数量
		uint32			unTodayGiftCount;//今天该礼物已经累计收到的数量
		uint32			unSrcUIN;
		uint32			unSrc179ID;
		std::wstring	strSrcName;
		ENM_GiftDstType	enmDstType;
		uint32			unDstUIN;
		uint32			unDst179ID;
		std::wstring	strDstName;
		std::wstring	strAttachMessage;//赠言
		bool			bAttachFlyEffect;//附带飞屏效果
		std::vector<SendGiftLuckyInfo> vecLuckyInfo;
	};

	//////////////////////////////////////////////////////////////////////////

	enum ENM_LoudSpeakerType
	{
		LOUDSPEAKERTYPE_SMALL	=	0x00,
		LOUDSPEAKERTYPE_BIG		=	0x01,
		LOUDSPEAKERTYPE_SUPER	=	0x02,
	};

	struct SendLoudSpeakerEvent : public ExtraInfo
	{
		//EVENT_VALUE_CORE_SEND_LOUDSPEAKER

		uint32				unRoomID;
		uint16				unGiftID;
		ENM_LoudSpeakerType enmType;
		std::wstring		strMessage;
	};

	struct SendLoudSpeakerRspEvent : public ExtraInfo
	{
		//EVENT_VALUE_CORE_SEND_LOUDSPEAKER_RSP

		uint32				unRoomID;
		ENM_SendGiftResult	enmResult;
		uint32				unLeftMoney;
		std::wstring		strMessage;
		ENM_LoudSpeakerType enmType;
		std::wstring		strFailReason;
		uint32				unRichUpdateMoney;
	};

	struct SendLoudSpeakerNotifyEvent : public ExtraInfo
	{
		//EVENT_VALUE_CORE_SEND_LOUDSPEAKER_NOTIFY

		enum SpeakerMessageType
		{
			SPEAKERMESSAGETYPE_LOUD			= 0x00,//喇叭消息
			SPEAKERMESSAGETYPE_LUCKY		= 0x01,//幸运物品中500，1000倍消息
			SPEAKERMESSAGETYPE_SYSTEM		= 0x02,//系统公告消息
			SPEAKERMESSAGETYPE_PLUTUSAWARD	= 0x03,//财神
			SPEAKERMESSAGETYPE_GAMEREWARD	= 0x04,//游戏通知消息
			SPEAKERMESSAGETYPE_LUXURY		= 0x05,//奢侈礼物消息
		};

		uint32				unRoomID;
		uint32				unSrcUIN;
		std::wstring		strName;
		uint32				un179id;
		uint8				unVipLevel;
		uint8				unRichLevel;
		std::vector<uint16>	vecGiftID;
		SpeakerMessageType	enmMessageType;
		ENM_LoudSpeakerType enmType;
		uint16				unGiftID;
		std::wstring		strMessage;
		uint8				unCache;		//幸运礼物是否是新的 0:新的 1:缓存的
	};

	//////////////////////////////////////////////////////////////////////////

	struct SendFlyWordsEvent : public ExtraInfo
	{
		//EVENT_VALUE_CORE_SEND_FLYWORDS

		uint32				unRoomID;
		uint16				unGiftID;
		std::wstring		strMessage;
	};

	struct SendFlyWordsRspEvent : public ExtraInfo
	{
		//EVENT_VALUE_CORE_SEND_FLYWORDS_RSP

		uint32				unRoomID;
		ENM_SendGiftResult	enmResult;
		uint32				unLeftMoney;
		std::wstring		strMessage;
		std::wstring		strFailReason;
		uint32				unRichUpdateMoney;
	};

	struct SendFlyWordsNotifyEvent : public ExtraInfo
	{
		//EVENT_VALUE_CORE_SEND_FLYWORDS_NOTIFY

		uint32				unRoomID;
		uint32				unSrcUIN;
		std::wstring		strName;
		uint32				un179id;
		uint16				unGiftID;
		std::wstring		strMessage;
	};

	//////////////////////////////////////////////////////////////////////////

	enum ENM_RequestShowType
	{
		REQUESTSHOWTYPE_ANY			=	0x00,	//排麦
		REQUESTSHOWTYPE_PUBLIC		=	0x01,	//排公麦
		REQUESTSHOWTYPE_PRIVATE		=	0x02,	//排私麦
	};

	enum ENM_RequestShowResult
	{
		REQUESTSHOWRESULT_NEED_SELECT			=	0x00,	//需要选择公麦还是私麦
		REQUESTSHOWRESULT_FULL					=	0x01,	//麦已满
		REQUESTSHOWRESULT_PUBLIC				=	0x02,	//排上公麦+index+time(sec)
		REQUESTSHOWRESULT_WAITTINGLIST			=	0x03,	//进入排麦队列+freeshow_opened
		REQUESTSHOWRESULT_PRIVATE				=	0x04,	//排上私麦
		REQUESTSHOWRESULT_PRIVATE_CLOSED		=	0x05,	//房间私麦已关闭
		REQUESTSHOWRESULT_FAIL					=	0x06,	//排麦失败
		REQUESTSHOWRESULT_ALREADY_ON_SHOW		=	0x07,
		REQUESTSHOWRESULT_ONMIC_IN_OTHERROOM 	=	0x08,	//在其他房间的麦上
		REQUESTSHOWRESULT_WAIT_BUT_ONMIC		=	0x09,	//从麦序上麦的时候，已在其他的麦上
		REQUESTSHOWRESULT_WAITTING_ONMIC		=   0x0a,   //在其他房间上麦，故上麦序
	};

	struct RequestShowRspEvent : public ExtraInfo
	{
		//EVENT_VALUE_CORE_REQUEST_SHOW_RSP

		uint32 unRoomID;
		ENM_RequestShowResult enmResult;
		uint8 unIndex;
		uint16 unTime;//0表示上麦时间不受限制,秒
		bool bFreeShowOpened;
	};

	struct RequestShowNotifyEvent : public ExtraInfo
	{
		//EVENT_VALUE_CORE_REQUEST_SHOW_NOTIFY

		uint32 unRoomID;
		uint32 unSrcUIN;
		ENM_RequestShowResult enmResult;
		uint8 unIndex;
		uint16 unTime;//0表示上麦时间不受限制,秒
	};	

	//////////////////////////////////////////////////////////////////////////

	enum ENM_OperateAVType
	{
		OPERATEAVTYPE_OPEN_VIDEO	=	0x00,
		OPERATEAVTYPE_OPEN_AUDIO	=	0x01,
		OPERATEAVTYPE_CLOSE_VIDEO	=	0x02,
		OPERATEAVTYPE_CLOSE_AUDIO	=	0x03,
	};

	enum ENM_OperateAVResult
	{
		OPERATEAVRESULT_OK			=	0x00,
		OPERATEAVRESULT_NOT_ON_SHOW	=	0x01,
		OPERATEAVRESULT_UNKNOWN		=	0xff,
	};

	struct OperateVideoAudioRspEvent : public ExtraInfo
	{
		//EVENT_VALUE_CORE_OPERATE_VIDEO_AUDIO_RSP

		uint32 unRoomID;
		uint32 unDstUIN;
		ENM_OperateAVType enmType;
		ENM_OperateAVResult enmResult;
	};

	//////////////////////////////////////////////////////////////////////////

	enum ENM_KickResult
	{
		KICKRESULT_OK				=	0x00,	//踢人，拉黑成功
		KICKRESULT_NO_PERMISSION	=	0x01,	//权限不足
		KICKRESULT_OTHER_IS_KING	=	0x02,	//对方是冠用户
		KICKRESULT_UNKNOWN			=	0xff,
	};

	struct KickUserEvent : public ExtraInfo
	{
		//EVENT_VALUE_CORE_KICK_USER

		uint32 unRoomID;
		bool bKickAllVisitor;//是否踢所有游客
		uint32 unDstUIN;//bKickAllVisitor为false时有效，踢单个人
		std::wstring strReason;
		uint16 unTime;//秒
	};

	struct KickUserRspEvent : public ExtraInfo
	{
		//EVENT_VALUE_CORE_KICK_USER_RSP

		uint32 unRoomID;
		ENM_KickResult enmKickResult;
		bool bKickAllVisitor;//是否踢所有游客
		uint32 unDstUIN;//bKickAllVisitor为false时有效，踢单个人		
	};

	struct KickUserNotifyEvent : public ExtraInfo
	{
		//EVENT_VALUE_CORE_KICK_USER_NOTIFY

		enum ENM_KickType
		{
			KICKTYPE_ONE					=	0x00,	//踢单个人
			KICKTYPE_VISITOR				=	0x01,	//踢游客
			KICKTYPE_FOR_MODIFY_CAPACITY	=	0x02,	//由于减容被踢
		};

		uint32 unRoomID;
		uint32 unSrcUIN;
		ENM_KickType enmType;
		uint32 unDstUIN;//KICKTYPE_ONE时有效
		std::wstring strReason;//KICKTYPE_ONE时有效
		uint16 unTime;//秒 KICKTYPE_ONE时有效
	};

	//////////////////////////////////////////////////////////////////////////

	struct LockIPEvent : public ExtraInfo
	{
		//EVENT_VALUE_CORE_LOCK_IP

		uint32 unRoomID;
		uint32 unDstUIN;
		std::wstring strReason;
		uint8 unTime;//分钟
	};

	struct LockIPNotifyEvent : public ExtraInfo
	{
		//EVENT_VALUE_CORE_LOCK_IP_NOTIFY

		uint32 unRoomID;
		uint32 unSrcUIN;
		uint32 unDstUIN;
		std::wstring strReason;
		uint8 unTime;//分钟
	};
	
	//////////////////////////////////////////////////////////////////////////

	struct PushOnShowNotifyEvent : public ExtraInfo
	{
		//EVENT_VALUE_CORE_PUSH_ON_SHOW_NOTIFY

		uint32 unRoomID;
		uint32 unSrcUIN;
		uint32 unDstUIN;
		uint8 unType;
		uint8 unIndex;
	};

	enum ENM_PushOnResult
	{
		PUSHONRESULT_OK				=	0x00,	//操作成功
		PUSHONRESULT_NO_PERMISSION	=	0x01,	//权限不足
		PUSHONRESULT_IS_ON_SHOW		=	0x02,	//已经在麦上
		PUSHONRESULT_MIC_IS_LOCKED	=	0x03,	//此麦被关
		PUSHONRESULT_NEED_ANSWER	=	0x04,	//需要对方应答
		PUSHONRESULT_UNKNOWN		=	0xff,
	};

	enum ENM_PushOnNotifyType
	{
		PUSHONNOTIFY_TYPE_NEEDANSWER	=	0x00,	//需要应答
		PUSHONNOTIFY_TYPE_CONNECT		=	0x01,	//完成连接
		PUSHONNOTIFY_TYPE_REJECT		=	0x02,	//拒绝
	};

	enum ENM_PullOffResult
	{
		PULLOFFRESULT_OK			=	0x00,	//操作成功
		PULLOFFRESULT_NO_PERMISSION	=	0x01,	//权限不足
		PULLOFFRESULT_IS_OFF_SHOW	=	0x02,	//不在麦上
		PULLOFFRESULT_UNKNOWN		=	0xff,
	};

	struct PushOnShowRspEvent : public ExtraInfo
	{
		//EVENT_VALUE_CORE_PUSH_ON_SHOW_RSP

		uint32	unRoomID;
		uint32	unDstUIN;
		uint8	unIndex;
		ENM_PushOnResult enmResult;
	};

	enum ENM_AnswerPushOnShowType
	{
		ANSWERPUSHONSHOW_ACCEPT	=	0x00,
		ANSWERPUSHONSHOW_REFUSE	=	0x01,
	};

	struct PushOnShowAnswerEvent : public ExtraInfo
	{
		//EVENT_VALUE_CORE_ANSWER_PUSH_ON_SHOW

		uint32	unRoomID;
		uint32	unDstUIN;
		ENM_AnswerPushOnShowType enmType;
		uint8	unIndex;
	};

	enum ENM_AnswerPushOnShowResult
	{
		ANSWERPUSHONSHOWRESULT_OK				=	0x00,	//操作成功
		ANSWERPUSHONSHOWRESULT_IS_ONMIC			=	0x01,	//已经在麦上
		ANSWERPUSHONSHOWRESULT_MIC_ISLOCKED		=	0x02,	//此麦被关
		ANSWERPUSHONSHOWRESULT_MIC_HASPLAYER	=	0x03,	//此麦上有人
		ANSWERPUSHONSHOWRESULT_UNKNOWN			=	0xff,	
	};

	struct PushOnShowAnswerRspEvent : public ExtraInfo
	{
		//EVENT_VALUE_CORE_ANSWER_PUSH_ON_SHOW_RSP

		uint32 unRoomID;
		uint32 unDstUIN;
		uint8  unIndex;
		ENM_AnswerPushOnShowType enmType;
		ENM_AnswerPushOnShowResult enmResult;
	};

	//////////////////////////////////////////////////////////////////////////

	enum ENM_ChangeTitleResult
	{
		CHANGETILERESULT_OK					=	0x00,
		CHANGETILERESULT_NO_PERMISSION		=	0x01,
		CHANGETILERESULT_IS_FULL			=	0x02,
		CHANGETILERESULT_IS_ADMIN_INOTHER	=	0x03,
		CHANGETILERESULT_UNKNOWN			=	0xff,
	};

	struct AddTitleRspEvent : public ExtraInfo
	{
		//EVENT_VALUE_CORE_ADD_TITLE_RSP

		uint32 unRoomID;
		uint32 unDstUIN;
		ENM_TitleLevel enmLevel;
		ENM_ChangeTitleResult enmResult;
	};

	struct AddTitleNotifyEvent : public ExtraInfo
	{
		//EVENT_VALUE_CORE_ADD_TITLE_NOTIFY

		uint32 unRoomID;
		uint32 unSrcUIN;
		uint32 unDstUIN;
		ENM_TitleLevel enmLevel;
	};

	struct RemoveTitleRspEvent : public ExtraInfo
	{
		//EVENT_VALUE_CORE_REMOVE_TITLE_RSP

		uint32 unRoomID;
		uint32 unDstUIN;
		ENM_ChangeTitleResult enmResult;
		ENM_TitleLevel enmLevel;
	};	

	struct RemoveTitleNotifyEvent : public ExtraInfo
	{
		//EVENT_VALUE_CORE_REMOVE_TITLE_NOTIFY

		uint32 unRoomID;
		uint32 unSrcUIN;
		uint32 unDstUIN;
		ENM_TitleLevel enmLevel;
	};

	//////////////////////////////////////////////////////////////////////////

	enum ENM_OpenPrivateResult
	{
		OPENPRIVATERESULT_OK				=	0x00,
		OPENPRIVATERESULT_NOT_OPEN			=	0x01,//对方私麦没有打开
		OPENPRIVATERESULT_NO_PERMISSION		=	0x02,//没有权限(非紫色vip以及以上)
		OPENPRIVATERESULT_IS_FULL			=	0x03,//达到私麦上限
		OPENPRIVATERESULT_ALREADY_OPENED	=	0x04,//已经打开过了
		OPENPRIVATERESULT_IS_SELF			=	0x05,//是自己
		OPENPRIVATERESULT_UNKNOWN			=	0xff,
	};

	//////////////////////////////////////////////////////////////////////////

	enum ENM_ConnectP2PResult
	{
		CONNECTP2PRESULT_OK					=	0x00,	//等待对方同意
		CONNECTP2PRESULT_SELF_ON_PUBLIC		=	0x01,	//自己已经在公麦中
		CONNECTP2PRESULT_SELF_ON_PRIVATE	=	0x02,	//自己已经在私麦中
		CONNECTP2PRESULT_SELF_ON_P2P		=	0x03,	//自己已经在1v1中
		CONNECTP2PRESULT_OTHER_ON_PUBLIC	=	0x04,	//对方已经在公麦中
		CONNECTP2PRESULT_OTHER_ON_PRIVATE	=	0x05,	//对方已经在私麦中
		CONNECTP2PRESULT_OTHER_ON_P2P		=	0x06,	//对方已经在1v1中
		CONNECTP2PRESULT_OTHER_NEED_VIP		=	0x07,	//对方只接受vip以上的(应该用不上)
		CONNECTP2PRESULT_OTHER_NEED_KING	=	0x08,	//对方只接受皇冠以上的
		CONNECTP2PRESULT_OTHER_CLOSE		=	0x09,	//对方关闭一对一
		CONNECTP2PRESULT_SELF_NOT_VIP		=	0x10,	//申请一对一的必须是vip以上

		CONNECTP2PRESULT_UNKNOWN			=	0xff,
	};	

	//////////////////////////////////////////////////////////////////////////

	enum ENM_ResponseP2PType
	{
		RESPONSEP2PTYPE_ACCEPT	=	0x00,
		RESPONSEP2PTYPE_REFUSE	=	0x01,
	};

	//////////////////////////////////////////////////////////////////////////

	struct ResponseP2PRspEvent : public ExtraInfo
	{
		//EVENT_VALUE_CORE_RESPONSE_P2P_RSP

		uint32 unRoomID;
		ENM_ResponseP2PType enmType;
		ENM_ConnectP2PResult enmResult;
		uint32 unDstUIN;
	};

	struct ResponseP2PNotifyEvent : public ExtraInfo
	{
		//EVENT_VALUE_CORE_RESPONSE_P2P_NOTIFY

		uint32 unRoomID;
		ENM_ResponseP2PType enmType;
		uint32 unSrcUIN;//发起答复者
		uint32 unDstUIN;//被答复者，为RESPONSEP2PTYPE_ACCEPT时有效
	};	

	struct ShutUpNotifyEvent : public ExtraInfo
	{
		//EVENT_VALUE_CORE_SHUT_UP_NOTIFY
		uint32 unRoomID;
		uint32 srcUIN;
		uint32 dstUIN;
		uint32 unLeftTime ;
	};
	//////////////////////////////////////////////////////////////////////////

	struct ReportClientInfoEvent : public ExtraInfo
	{
		//EVENT_VALUE_CORE_REPORT_CLIENTINFO

		uint32 unRoomID;

		bool bVideoInfo;//是否包含bHaveVideo信息
		bool bHaveVideo;

		bool bNetInfo;//是否包含bNetInfo信息
		uint8 unPacketLost;//eg: 123 -> 12.3%
		uint8 unTimeout;//eg:  23 -> 23ms

		bool bHardwareInfo;//是否包含硬件信息
		std::wstring strCPUType;
		uint16 unCPUUse;//eg: 1234 -> 12.34%
		uint16 unMemSize;//eg: 250 -> 2.5G
		uint16 unMemUse;//eg: 2345 -> 23.45%
	};

	struct ReportClientInfoNotifyEvent : public ExtraInfo
	{
		//EVENT_VALUE_CORE_REPORT_CLIENTINFO_NOTIFY

		uint32 unRoomID;
		uint32 unSrcUIN;

		bool bVideoInfo;//是否包含bHaveVideo信息
		bool bHaveVideo;

		bool bNetInfo;//是否包含bNetInfo信息
		uint8 unPacketLost;
		uint8 unTimeout;

		bool bHardwareInfo;//是否包含硬件信息
		std::wstring strCPUType;
		uint16 unCPUUse;//eg: 1234 -> 12.34%
		uint16 unMemSize;//eg: 250 -> 2.5G
		uint16 unMemUse;//eg: 2345 -> 23.45%
	};

	//////////////////////////////////////////////////////////////////////////

	struct QueryClientInfoEvent : public ExtraInfo
	{
		//EVENT_VALUE_CORE_QUERY_CLIENTINFO

		uint32 unRoomID;
		uint32 unDstUIN;
		bool bVideoInfo;//是否包含bHaveVideo信息
		bool bNetInfo;//是否包含bNetInfo信息
		bool bHardwareInfo;//是否包含硬件信息		
	};

	struct QueryClientInfoRspEvent : public ExtraInfo
	{
		//EVENT_VALUE_CORE_QUERY_CLIENTINFO_RSP

		uint32 unRoomID;
		
		bool bVideoInfo;//是否包含bHaveVideo信息
		bool bHaveVideo;

		bool bNetInfo;//是否包含bNetInfo信息
		uint8 unPacketLost;
		uint8 unTimeout;

		bool bHardwareInfo;//是否包含硬件信息
		std::wstring strCPUType;
		uint16 unCPUUse;//eg: 1234 -> 12.34%
		uint16 unMemSize;//eg: 250 -> 2.5G
		uint16 unMemUse;//eg: 2345 -> 23.45%
	};

	//////////////////////////////////////////////////////////////////////////

	enum ENM_RoomOptionType
	{
		ROOMOPTIONTYPE_STATUS					=	0x0001, //(0:打开房间)
		ROOMOPTIONTYPE_PUBLICCHAT				=	0x0002, //(0:公聊打开)
		ROOMOPTIONTYPE_COLORTEXT				=	0x0004, //(0:彩条打开)
		ROOMOPTIONTYPE_USER_INOUT				=	0x0008, //(0:用户进出信息关闭)
		ROOMOPTIONTYPE_FREESHOW_STATUS			=	0x0010, //(0:自由排麦关闭)
		ROOMOPTIONTYPE_FREESHOW_FOR_MANAGER		=	0x0020, //(0:不限制管理员麦时)
		ROOMOPTIONTYPE_FREESHOW_EDIT_TIME		=	0x0040, //(0:允许管理员修改麦时)
		ROOMOPTIONTYPE_PRIVATESHOW_ENABLE		=	0x0080, //(0:私麦disable)(是否有私麦)
		ROOMOPTIONTYPE_PRIVATESHOW_STATUS		=	0x0100, //(0:打开)(有私麦时是否打开)
		ROOMOPTIONTYPE_AUTO_WELCOME				=	0x0200, //(0:自动发欢迎词)
		ROOMOPTIONTYPE_SHOW_ROOMNAME			=	0x0400, //(0:昵称前显示房间名)
	};

	enum ENM_RoomInfoType
	{
		ROOMINFOTYPE_NAME				=	0x00, //string房间名字
		ROOMINFOTYPE_SIGNATURE			=	0x01, //string 房间主题
		ROOMINFOTYPE_PASSWORD			=	0x02, //string(空串即没有密码)
		ROOMINFOTYPE_FREESHOW_TIME		=	0x03, //uint8(自由排麦时上麦时间,min)
		ROOMINFOTYPE_WELCOME_WORDS		=	0x04, //string(欢迎词)
		ROOMINFOTYPE_NOTICE				=	0x05, //string(公告)
		ROOMINFOTYPE_OPTION				=	0x06, //uint32(RoomOptionType)
		ROOMINFOTYPE_MIC_FIRST_STATUS	=	0x07, //uint8 (一号麦的状态，0：open)
		ROOMINFOTYPE_MIC_SECOND_STATUS	=	0x08, //uint8 (二号麦的状态)
		ROOMINFOTYPE_MIC_THIRD_STATUS	=	0x09, //uint8 (三号麦的状态)
	};

	struct SetRoomInfoEvent : public ExtraInfo
	{
		//EVENT_VALUE_CORE_SET_ROOMINFO

		uint32 unRoomID;
		std::map<ENM_RoomInfoType, std::wstring>	map_key2string;
		std::map<ENM_RoomInfoType, uint8>			map_key2uin8;
		std::map<ENM_RoomInfoType, uint32>			map_key2uin32;
	};

	enum ENM_SetRoomInfoResult
	{
		SETROOMINFORESULT_OK			=	0x00,
		SETROOMINFORESULT_NO_PERMISSION	=	0x01,
		SETROOMINFORESULT_UNKNOWN		=	0xff,
	};

	struct SetRoomInfoNotifyEvent : public ExtraInfo
	{
		//EVENT_VALUE_CORE_SET_ROOMINFO_NOTIFY

		uint32 unRoomID;
		uint32 unSrcUIN;

		std::map<ENM_RoomInfoType, std::wstring>	map_key2string;
		std::map<ENM_RoomInfoType, uint8>			map_key2uin8;
		std::map<ENM_RoomInfoType, uint32>			map_key2uin32;
	};

	//////////////////////////////////////////////////////////////////////////

	struct GetRoomInfoRspEvent : public ExtraInfo
	{
		//EVENT_VALUE_CORE_GET_ROOMINFO_RSP

		uint32 unRoomID;
		std::map<ENM_RoomInfoType, std::wstring>	map_key2string;
		std::map<ENM_RoomInfoType, uint8>			map_key2uin8;
		std::map<ENM_RoomInfoType, uint32>			map_key2uin32;
	};

	//////////////////////////////////////////////////////////////////////////

	enum ENM_SetMicStatusResult
	{
		SETMICSTATUSRESULT_OK				=	0x00,
		SETMICSTATUSRESULT_NO_PERMISSION	=	0x01,
		SETMICSTATUSRESULT_UNKNOWN			=	0xff,
	};

	//////////////////////////////////////////////////////////////////////////

	struct NotifyUserInfoUpdateEvent : public ExtraInfo
	{
		//EVENT_VALUE_CORE_NOTIFY_ROOM_USER_INFO_UPDATE

		uint32 unRoomID;
		uint32 unSrcUIN;
		std::map<ENM_InfoType, std::wstring>	map_key2string;
		std::map<ENM_InfoType, uint8>			map_key2uin8;
		std::map<ENM_InfoType, uint16>			map_key2uin16;
		std::map<ENM_InfoType, uint32>			map_key2uin32;
	};

	//////////////////////////////////////////////////////////////////////////	

	struct NotifySystemMessageEvent : public ExtraInfo
	{
		//EVENT_VALUE_CORE_NOTIFY_SYSTEM_MESSAGE

		enum ENM_SystemMessageType
		{
			SYSTEMMESSAGETYPE_SYSTEM		=	0x00,//系统消息
			SYSTEMMESSAGETYPE_TEXT			=	0x01,//纯文本(单条)
			SYSTEMMESSAGETYPE_WITHOUT_TITLE	=	0x20,//客户端加上“尊敬的***”(单条)
		};

		struct MessageInfo
		{
			std::wstring strMessage;
			std::wstring strTipsText;
			std::wstring strTipsURL;
		};

		std::wstring strWindowTitle;//窗口title
		ENM_SystemMessageType enmType;
		std::vector<MessageInfo> strMessage;
	};

	//////////////////////////////////////////////////////////////////////////
	
	struct NotifyPrivateShowEvent : public ExtraInfo
	{
		//EVENT_VALUE_CORE_NOTIFY_PRIVATE_SHOW

		uint32 unRoomID;
		std::vector<uint32> vecUIN;
	};

	//////////////////////////////////////////////////////////////////////////

	struct NotifyOfficialNoticeEvent : public ExtraInfo
	{
		//EVENT_VALUE_CORE_NOTIFY_OFFICIAL_NOTICE

		uint32 unRoomID; //为0时，表示对所有房间
		std::wstring strMessage;
	};

	//////////////////////////////////////////////////////////////////////////

	struct NotifyPlutusAwardEvent : public ExtraInfo
	{
		//EVENT_VALUE_CORE_NOTIFY_PLUTUS_AWARD

		uint32 unRoomID;
		uint16 unGiftID;
		std::wstring strRoomName;
		std::wstring strName;
		uint32 un179ID;
		uint32 unSrcUIN;//请出财神的人
		std::vector<uint16> vecMoneys;
	};	

	//////////////////////////////////////////////////////////////////////////
	
	struct NotifyRoomLockedEvent : public ExtraInfo
	{
		//EVENT_VALUE_CORE_NOTIFY_ROOM_LOCKED

		uint32 unRoomID;
		std::wstring strReason;
		uint16 lockdays;//封停天数
		uint32 unlockdate;//解封日期
	};	

	//////////////////////////////////////////////////////////////////////////

	struct NotifyMediaServerResetEvent : public ExtraInfo
	{
		//EVENT_VALUE_CORE_NOTIFY_MEDIA_SERVER_RESET

		uint32 unRoomID;
		uint32 relayIP_dianxin;
		uint16 relayPort_dianxin;
		uint32 relayIP_wangtong;
		uint16 relayPort_wangtong;
	};

	//////////////////////////////////////////////////////////////////////////

	struct NotifyRichStarEvent : public ExtraInfo
	{
		//EVENT_VALUE_CORE_NOTIFY_RICH_STAR

		uint32				unRoomID;
		uint32				unSrcUIN;
		std::wstring		strName;
		uint32				un179id;
		uint32				unMoney;
	};	

	//////////////////////////////////////////////////////////////////////////	

	//SERVICE_VALUE_CORE_GET_GIFT_STAR_INFO
	
	struct GiftStarInfo
	{
		uint16 unGiftID;
		uint32 unCount;
	};

	struct GetGiftStarInfo
	{
		uint32 unUIN;
		std::vector<GiftStarInfo> vecInfo;
	};

	//////////////////////////////////////////////////////////////////////////	

	//SERVICE_VALUE_CORE_GET_MY_GOODS

	struct GetMyGoods
	{
		std::map<uint16, uint16> mapItem2Count;
	};

	//////////////////////////////////////////////////////////////////////////

	struct ReportCollectInfoEvent : public ExtraInfo
	{
		//EVENT_VALUE_CORE_REPORT_COLLECT_INFO

	#define COLLECT_INFO_UPLOST		L"UpPackLost"
	#define COLLECT_INFO_DOWN		L"DownPackLost"
	#define COLLECT_INFO_PROCESS	L"Process"

		struct Key2Value
		{
			std::wstring key;
			std::wstring value;
		};

		std::vector<Key2Value> vecCollecInfo;
	};

	//////////////////////////////////////////////////////////////////////////

	struct GetFansRspEvent : public ExtraInfo
	{
		//EVENT_VALUE_CORE_GET_FANS_RSP

		struct FansInfo
		{
			uint32 uin;
			std::wstring nickname;
			uint32 money;

			FansInfo():uin(0){}
		};

		uint32 result;
		uint32 uin;
		FansInfo day[5];
		FansInfo month[5];
		FansInfo all[5];
	};

	//////////////////////////////////////////////////////////////////////////

	struct GetSofaListRspEvent : public ExtraInfo
	{
		//EVENT_VALUE_CORE_GET_SOFA_LIST_RSP

		struct SofaInfo
		{
			uint32 uin;
			bool   bman;
			uint32 un179ID;
			std::wstring nickname;
			uint32 price;
			uint8 vipLevel;
			uint8 richLevel;

			SofaInfo():uin(0){}
		};

		uint32 roomID;
		uint32 result;
		SofaInfo sofa[5];
	};

	//////////////////////////////////////////////////////////////////////////

	struct BuySofaRspEvent : public ExtraInfo
	{
		//EVENT_VALUE_CORE_BUY_SOFA_RSP

		enum ENM_BuySofaResult
		{
			BUYSOFARESULT_OK				=	  0x00,	//操作成功
			BUYSOFARESULT_PRICE_TOO_LOW		=	  0x01,	//出价太低
			BUYSOFARESULT_NOT_ENOUGH_MONEY   =	  0x02,	//余额不足
			BUYSOFARESULT_UNKNOWN			=	  0xff,
		};

		uint32 roomid;
		uint8  sofaIndex;//1,2,3,4,5
		ENM_BuySofaResult enmResult;
		uint32 price;//单位沙发
		uint32 leftMoney;
		uint32 nextMagnateLevelSpend;
	};

	struct BuySofaNotifyEvent : public ExtraInfo
	{
		//EVENT_VALUE_CORE_BUY_SOFA_NOTIFY

		uint32 roomid;
		uint32 uin;
		uint8  sofaIndex;//1,2,3,4,5
		uint32 price;//单位沙发
	};

	//////////////////////////////////////////////////////////////////////////

	struct Song
	{
		std::wstring songName;
		uint8 songType;//1:hot
	};

	struct UpdateSongListEvent : public ExtraInfo
	{
		//EVENT_VALUE_UPDATE_SONGLIST

		uint32 roomid;
		std::vector<Song> vecSongList;
	};

	struct UpdateSongListRspEvent : public ExtraInfo
	{
		//EVENT_VALUE_UPDATE_SONGLIST_RSP

		enum ENM_UpdateSonglistResult
		{
			UPDATESONGRESULT_OK				= 0x00,   //成功
			UPDATESONGRESULT_FULL			= 0x01,   //歌单已满
			UPDATESONGRESULT_HOT_FULL		= 0x02,   //热推歌已满
			UPDATESONGRESULT_NO_PERMISSION	= 0x03,   //没有权限(不是主播..)
			UPDATESONGRESULT_UNKNOWN		= 0xff,   
		};

		uint32 roomid;
		ENM_UpdateSonglistResult enmResult;
		std::vector<Song> vecSongList;
	};

	//////////////////////////////////////////////////////////////////////////

	struct GetSongListRspEvent : public ExtraInfo
	{
		//EVENT_VALUE_GET_SONGLIST_RSP

		uint32 result;//0:成功
		uint32 roomid;
		uint32 songerUin;
		std::vector<Song> vecSongList;
	};	

	//////////////////////////////////////////////////////////////////////////

	struct OrderSongEvent : public ExtraInfo
	{
		//EVENT_VALUE_ORDER_SONG

		uint32 roomid;
		uint32 songerUin;
		std::wstring songName;
		std::wstring attachWords;
		uint16 count;

		enum ENM_OrderSongResult
		{
			ORDERSONGRESULT_OK					= 0x00,   //成功
			ORDERSONGRESULT_MONEY_NOT_ENOUGH	= 0x01,   //币不够
			ORDERSONGRESULT_NO_PERMISSION		= 0x02,   //对方不是主播
			ORDERSONGRESULT_TIMEOUT				= 0x03,   //超时
			ORDERSONGRESULT_UNKNOWN				= 0x04,   //未知错误
		};
	};

	struct SongOrder
	{
		uint32 orderId;
		uint32 orderTime;
		uint32 songerUin;
		std::wstring songerNickname;
		uint32 uin;
		std::wstring nickName;
		std::wstring songName;
		std::wstring attachWords;
		uint16 count;

		enum ENM_OrderStatus
		{
			ORDER_STATUS_TODO,//待主播处理
			ORDER_STATUS_ACCEPTED,//主播已同意，待评价
			ORDER_STATUS_REJECTED,//主播已拒绝
			ORDER_STATUS_MARK_GOOD,//点歌者好评
			ORDER_STATUS_MARK_BAD,//点歌者差评
			ORDER_STATUS_MARK_NONE,//点歌者弃评
		};

		ENM_OrderStatus status;

		SongOrder()
		{
			orderId = orderTime = songerUin = uin = 0;
			status = ORDER_STATUS_TODO;
		}
	};

	struct OrderSongNotifyEvent : public ExtraInfo
	{
		//EVENT_VALUE_ORDER_SONG_NOTIFY

		uint32 roomid;
		SongOrder order;
	};	

	//////////////////////////////////////////////////////////////////////////

	struct ProcessOrderNotifyEvent : public ExtraInfo
	{
		//EVENT_VALUE_PROCESS_ORDER_NOTIFY

		uint32 roomid;
		SongOrder order;
	};

	//////////////////////////////////////////////////////////////////////////
	
	struct GetOrderListRspEvent : public ExtraInfo
	{
		//EVENT_VALUE_GET_ORDERLIST_RSP

		uint32 roomid;
		uint32 result;//0:成功
		std::vector<SongOrder> orderList;
	};
	
	//////////////////////////////////////////////////////////////////////////

	struct MarkSongNotifyEvent : public ExtraInfo
	{
		//EVENT_VALUE_MARK_SONG_NOTIFY

		uint32 roomid;
		SongOrder order;
	};	

	//////////////////////////////////////////////////////////////////////////

	struct OpenBoxRspEvent : public ExtraInfo
	{
		//EVENT_VALUE_CORE_OPEN_BOX_RSP

		enum ENM_OpenBoxResult
		{
			OPENBOXRESULT_OK				= 0x00,   //成功
			OPENBOXRESULT_NO_FREE_CHANCE	= 0x01,   //没有免费资格
			OPENBOXRESULT_MONEY_NOT_ENOUGH	= 0x02,   //币不够
			OPENBOXRESULT_UNKNOWN			= 0x10,   //(未知错误)
		};

		struct RewardItem
		{
			uint16 itemid;
			uint16 itemcount;
		};

		uint32 roomid;
		ENM_OpenBoxResult result;
		std::vector<RewardItem> vecReward;
	};

	struct RewardBoxKeyNotifyEvent : public ExtraInfo
	{
		//EVENT_VALUE_CORE_REWARD_BOX_KEY_NOTIFY

		uint8 count;
		std::wstring reason;
		uint8 isFirstTime;
	};

	//////////////////////////////////////////////////////////////////////////

	struct GetOfflineActorRspEvent : public ExtraInfo
	{
		//EVENT_VALUE_CORE_GET_OFFLINE_ACTOR_RSP

		uint32 roomID;
		std::vector<room_user_info> offlineActor;
	};
}
