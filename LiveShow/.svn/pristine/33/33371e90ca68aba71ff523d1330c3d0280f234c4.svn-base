#include "StdAfx.h"
#include "RoomModule.h"
#include "Room.h"
#include "RoomBusiness.h"
#include "RoomTopManager.h"
#include "..\CommonLibrary\include\utility\SystemHelper.h"
#include "..\CommonLibrary\include\utility\StringHelper.h"
#include "..\CommonLibrary\include\utility\cpu.h"
#include "userlist\ListIconManager.h"
#include "gift\GiftManager.h"
#include "avlogic\RoomAVLogic.h"
#include "personalsetting\PersonalConfig.h"
#include "webdefine.h"
#include "HallDefine.h"
#include "..\CommonLibrary\include\ui\C179MsgBox.h"
#include "personalsetting/VideoManager.h"
#include "RoomParentDlg.h"
#include "micwindow\VideoDlg.h"
#include "usermanager\UserManager.h"
#include "roomsetting\RoomOption.h"
#include "..\CommonLibrary\include\ui\ImageEx.h"
#include "screeneffect\ScreenEffectManager.h"
#include "autoreply\AutoReplyConfig.h"
#include "richlevel\RichLevelManager.h"
#include "utility\timers.h"
#include <boost/bind.hpp>

#define  DLG_WIDTH 1020
#define  DLG_HEIGHT 740

#define ROOM_NETSTATE_TIMEOUT 10*60  //定时器间隔   获取音视频发包收包丢包率间隔时间（单位：秒  即10分钟）
#define ROOM_PERFORMANCE_TIMEOUT 10  //定时器间隔   检测cpu消耗（单位：秒  即10分钟）
#define CPU_PERFORMANCE_TEST_POINT 40.0 //进程cpu消耗超过40%时，转入"提速"模式
#define CPU_PERFORMANCE_TEST_POINT2 20.0 //进程cpu消耗超过20%时，禁用拉普拉斯锐化

RoomModule::RoomModule(void)
{
	m_mMessageFun[core::MESSAGE_VALUE_CORE_LOGIN_COMPLETE] = &RoomModule::ProcessLoginComplete ;
	//清空房间
	m_mMessageFun[core::MESSAGE_VALUE_CORE_DISCONNECTED] = &RoomModule::ProcessCleanRoom ;
	m_mMessageFun[core::MESSAGE_VALUE_CORE_BE_KICKED] = &RoomModule::ProcessCleanRoom ;
	m_mMessageFun[core::MESSAGE_VALUE_CORE_PRE_LOGIN] = &RoomModule::ProcessCleanRoom ;

	//大厅
	m_mMessageFun[Hall::MESSAGE_VALUE_HALL_OPENSOUND] = &RoomModule::ProcessHallOpenSound ;
	m_mMessageFun[Hall::MESSAGE_VALUE_HALL_HEAD_IMAGE_UPDATE] = &RoomModule::ProcessHallHeadImageUpdate ;
	m_mMessageFun[Hall::MESSAGE_VALUE_HALL_RES_UPDATE] = &RoomModule::ProcessHallResUpdate;

	//定时器
	m_mMessageFun[MESSAGE_VALUE_SYS_CYCLE_TRIGGER] = &RoomModule::OnCycleTrigger ;

	//个人
	m_mMessageFun[core::MESSAGE_VALUE_CORE_MY_INFO_UPDATE] = &RoomModule::ProcessMyInfoUpdate ;
	m_mMessageFun[core::MESSAGE_VALUE_CORE_MY_VIPLEVEL_UPDATE] = &RoomModule::ProcessMyVipLevelUpdate;
	m_mMessageFun[core::MESSAGE_VALUE_CORE_MY_RICHLEVEL_UPDATE] = &RoomModule::ProcessMyRichLevelUpdate;

	//通用的Web访问接口返回响应
	m_mEventFun[webinterface::event_value_web_get_http_resp] = &RoomModule::ProcessWebGetHttpResp ;

	//房间
	m_mEventFun[core::EVENT_VALUE_CORE_GET_USERLIST_RSP] = &RoomModule::ProcessGetUserListRsp ;
	m_mEventFun[core::EVENT_VALUE_CORE_GET_WAITTINGLIST_RSP] = &RoomModule::ProcessGetWaitingListRsp ;
	m_mEventFun[core::EVENT_VALUE_CORE_ENTER_ROOM_NOTIFY] = &RoomModule::ProcessEnterRoomNotify ;
	m_mEventFun[core::EVENT_VALUE_CORE_LEAVE_ROOM_NOTIFY] = &RoomModule::ProcessLeaveRoomNotify ;
	m_mEventFun[core::EVENT_VALUE_CORE_NOTIFY_ROOM_USER_INFO_UPDATE] = &RoomModule::ProcessNotifyUserInfoUpdate ;
	m_mEventFun[core::EVENT_VALUE_CORE_QUERY_CLIENTINFO_RSP] = &RoomModule::ProcessQueryClientInfoRsp ;
	m_mEventFun[core::EVENT_VALUE_CORE_SET_ROOMINFO_RSP] = &RoomModule::ProcessSetRoomInfoRsp ;
	m_mEventFun[core::EVENT_VALUE_CORE_SET_ROOMINFO_NOTIFY] = &RoomModule::ProcessSetRoomInfoNotify ;
	m_mEventFun[core::EVENT_VALUE_CORE_GET_ROOMINFO_RSP] = &RoomModule::ProcessGetRoomInfoRsp ;
	m_mEventFun[core::EVENT_VALUE_CORE_NOTIFY_ROOM_LOCKED] = &RoomModule::ProcessLockRoomNotify ;
	m_mEventFun[core::EVENT_VALUE_CORE_NOTIFY_MEDIA_SERVER_RESET] = &RoomModule::ProcessMediaServerResetNotify ;
	m_mEventFun[core::EVENT_VALUE_CORE_GET_SOFA_LIST_RSP] = &RoomModule::ProcessGetSofaListRsp;
	m_mEventFun[core::EVENT_VALUE_CORE_BUY_SOFA_RSP] = &RoomModule::ProcessBuySofaRsp;
	m_mEventFun[core::EVENT_VALUE_CORE_BUY_SOFA_NOTIFY] = &RoomModule::ProcessBuySofaNotify;
	m_mEventFun[core::EVENT_VALUE_CORE_NOTIFY_CLEAR_SOFA_LIST] = &RoomModule::ProcessNotifyClearSofaList;
	m_mEventFun[core::EVENT_VALUE_CORE_GET_OFFLINE_ACTOR_RSP] = &RoomModule::ProcessInitOffRoomAcotrRsp;

	//聊天
	m_mEventFun[core::EVENT_VALUE_CORE_SEND_MESSAGE_RSP] = &RoomModule::ProcessSendMessageRsp ;
	m_mEventFun[core::EVENT_VALUE_CORE_SEND_MESSAGE_NOTIFY] = &RoomModule::ProcessSendMessageNotify ;

	//喇叭
	m_mEventFun[core::EVENT_VALUE_CORE_SEND_LOUDSPEAKER_RSP] = &RoomModule::ProcessSendLoudSpeakerRsp ;
	m_mEventFun[core::EVENT_VALUE_CORE_SEND_LOUDSPEAKER_NOTIFY] =&RoomModule::ProcessSendLoudSpeakerNotify ;

	//麦
	m_mEventFun[core::EVENT_VALUE_CORE_REQUEST_SHOW_RSP] = &RoomModule::ProcessRequestShowRsp ;
	m_mEventFun[core::EVENT_VALUE_CORE_REQUEST_SHOW_NOTIFY] =&RoomModule::ProcessRequestShowNotify ;
	m_mEventFun[core::EVENT_VALUE_CORE_EXIT_SHOW_NOTIFY] = &RoomModule::ProcessExitShowNotify ;
	m_mEventFun[core::EVENT_VALUE_CORE_OPEN_PRIVATE_RSP] = &RoomModule::ProcessOpenPrivateRsp ;
	m_mEventFun[core::EVENT_VALUE_CORE_PUSH_ON_SHOW_RSP] = &RoomModule::ProcessPushOnShowRsp ;
	m_mEventFun[core::EVENT_VALUE_CORE_PUSH_ON_SHOW_NOTIFY] = &RoomModule::ProcessPushOnShowNotify ;
	m_mEventFun[core::EVENT_VALUE_CORE_PULL_OFF_SHOW_RSP] = &RoomModule::ProcessPullOffShowRsp ;
	m_mEventFun[core::EVENT_VALUE_CORE_PULL_OFF_SHOW_NOTIFY] = &RoomModule::ProcessPullOffShowNotify ;
	m_mEventFun[core::EVENT_VALUE_CORE_OPERATE_VIDEO_AUDIO_RSP] = &RoomModule::ProcessOpVideoAudioRsp ;
	m_mEventFun[core::EVENT_VALUE_CORE_OPERATE_VIDEO_AUDIO_NOTIFY] = &RoomModule::ProcessOpVideoAudioNotify ;
	m_mEventFun[core::EVENT_VALUE_CORE_REQUEST_P2P_RSP] = &RoomModule::ProcessRequestP2PRsp ;
	m_mEventFun[core::EVENT_VALUE_CORE_REQUEST_P2P_NOTIFY] = &RoomModule::ProcessRequestP2PNotify ;	
	m_mEventFun[core::EVENT_VALUE_CORE_RESPONSE_P2P_RSP] = &RoomModule::ProcessResponseP2PRsp ;	
	m_mEventFun[core::EVENT_VALUE_CORE_RESPONSE_P2P_NOTIFY] = &RoomModule::ProcessResponseP2PNotify;
	m_mEventFun[core::EVENT_VALUE_CORE_NOTIFY_PRIVATE_SHOW] = &RoomModule::ProcessNotifyPrivateShow ;
	m_mEventFun[core::EVENT_VALUE_CORE_MIC_SET_MIC_STATUS_RSP] = &RoomModule::ProcessSetMicStatusRsp ;
	m_mEventFun[core::EVENT_VALUE_CORE_MIC_SET_MIC_STATUS_NOTIFY] = &RoomModule::ProcessSetMicStatusNotify ;
	m_mEventFun[core::EVENT_VALUE_CORE_ANSWER_PUSH_ON_SHOW_RSP] = &RoomModule::ProcessAnswerPushOnShowRsp;

	//管理
	m_mEventFun[core::EVENT_VALUE_CORE_KICK_USER_RSP] = &RoomModule::ProcessKickUserRsp ;
	m_mEventFun[core::EVENT_VALUE_CORE_KICK_USER_NOTIFY] = &RoomModule::ProcessKickUserNotify ;
	m_mEventFun[core::EVENT_VALUE_CORE_ADD_BLACK_RSP] = &RoomModule::ProcessAddBlackRsp ;
	m_mEventFun[core::EVENT_VALUE_CORE_ADD_BLACK_NOTIFY] = &RoomModule::ProcessAddBlackNotify ;
	m_mEventFun[core::EVENT_VALUE_CORE_LOCK_IP_RSP] = &RoomModule::ProcessLockIpRsp ;
	m_mEventFun[core::EVENT_VALUE_CORE_LOCK_IP_NOTIFY] = &RoomModule::ProcessLockIpNotify ;
	m_mEventFun[core::EVENT_VALUE_CORE_SHUT_UP_RSP] = &RoomModule::ProcessShutUpRsp ;
	m_mEventFun[core::EVENT_VALUE_CORE_SHUT_UP_NOTIFY] =&RoomModule::ProcessShutUpNotify ;
	m_mEventFun[core::EVENT_VALUE_CORE_UNLOCK_SHUTUP_RSP] = &RoomModule::ProcessUnLockShutUpRsp ;
	m_mEventFun[core::EVENT_VALUE_CORE_UNLOCK_SHUTUP_NOTIFY] =&RoomModule::ProcessUnLockShutUpNotify ;

	m_mEventFun[core::EVENT_VALUE_CORE_ADD_TITLE_RSP] = &RoomModule::ProcessAddTitleRsp ; 
	m_mEventFun[core::EVENT_VALUE_CORE_REMOVE_TITLE_RSP] = &RoomModule::ProcessRemoveTitleRsp ;
	m_mEventFun[core::EVENT_VALUE_CORE_ADD_TITLE_NOTIFY] = &RoomModule::ProcessAddTitleNotify ;
	m_mEventFun[core::EVENT_VALUE_CORE_REMOVE_TITLE_NOTIFY] = &RoomModule::ProcessRemoveTitleNotify ;

	//礼物
	m_mEventFun[core::EVENT_VALUE_CORE_SEND_GIFT_RSP] = &RoomModule::ProcessSendGiftRsp ;
	m_mEventFun[core::EVENT_VALUE_CORE_SEND_GIFT_NOTIFY] = &RoomModule::ProcessSendGiftNotify ;
	m_mEventFun[core::EVENT_VALUE_CORE_NOTIFY_GIFT_OVER_TIME] = &RoomModule::ProcessNotifyGiftOverTime ;
	m_mEventFun[core::EVENT_VALUE_CORE_NOTIFY_PLUTUS_AWARD] = &RoomModule::ProcessNotifyPlutusAward ;
	m_mEventFun[core::EVENT_VALUE_CORE_NOTIFY_MONEY_UPDATE] = &RoomModule::ProcessNotifyMoneyUpdate ;
	m_mEventFun[core::EVENT_VALUE_CORE_NOTIFY_OFFICIAL_NOTICE] = &RoomModule::ProcessNotifyOfficeNotice ;
	m_mEventFun[core::EVENT_VALUE_CORE_SEND_FLYWORDS_RSP] = &RoomModule::ProcessSendFlyWordsRsp;
	m_mEventFun[core::EVENT_VALUE_CORE_SEND_FLYWORDS_NOTIFY] = &RoomModule::ProcessSendFlyWordsNotify;
	m_mEventFun[core::EVENT_VALUE_CORE_NOTIFY_RICH_STAR] = &RoomModule::ProcessNotifyRichStar ;

	//个人设置
	m_mEventFun[core::EVENT_VALUE_CORE_SET_SELFINFO_RSP] = &RoomModule::ProcessSetSelfInfoRsp ;
	m_mEventFun[core::EVENT_VALUE_CORE_GET_FANS_RSP] = &RoomModule::ProcessGetFansRsp ;

	//点歌系统
	m_mEventFun[core::EVENT_VALUE_UPDATE_SONGLIST_RSP] = &RoomModule::ProcessUpdateSongListRsp ;
	m_mEventFun[core::EVENT_VALUE_GET_SONGLIST_RSP] = &RoomModule::ProcessGetSongListRsp ;
	m_mEventFun[core::EVENT_VALUE_ORDER_SONG_RSP] = &RoomModule::ProcessOrderSongRsp ;
	m_mEventFun[core::EVENT_VALUE_ORDER_SONG_NOTIFY] = &RoomModule::ProcessOrderSongNotify ;
	m_mEventFun[core::EVENT_VALUE_PROCESS_ORDER_RSP] = &RoomModule::ProcessTakeOrderRsp ;
	m_mEventFun[core::EVENT_VALUE_PROCESS_ORDER_NOTIFY] = &RoomModule::ProcessTakeOrderNotify ;
	m_mEventFun[core::EVENT_VALUE_GET_ORDERLIST_RSP] = &RoomModule::ProcessGetOrderListRsp ;

	m_mEventFun[core::EVENT_VALUE_MARK_SONG_RSP] = &RoomModule::ProcessMarkSongRsp ;
	m_mEventFun[core::EVENT_VALUE_MARK_SONG_NOTIFY] = &RoomModule::ProcessMarkSongNotify ;

	//任务系统
	//DragonZ:V3.0去除房间右下角的任务
	//m_mEventFun[core::EVENT_VALUE_CORE_NOTIFY_TASK_INFO_UPDATE] = &RoomModule::ProcessTaskInfoNotify ;

	m_pRoomAvLogic = new RoomAVLogic( 0 , 0 ,*this ) ;
	m_pRoomAvLogic->SetVideoCfg( 0 ,240 ,320 ,26) ;
	m_pRoomAvLogic->SetAudioCfg( 0 ,0 ) ;
	m_pRoomSysSettingDlg = NULL;
	m_NetStateTimer = -1;	
	m_PerformanceTimer = -1;
	m_bSpeedUpMode = false;

	srand( (unsigned)time( NULL ) );
}

RoomModule::~RoomModule(void)
{
	if ( m_pRoomAvLogic != NULL )
	{
		m_pRoomAvLogic->ClearRoomAV() ;
		delete m_pRoomAvLogic ;
	}
	m_mapEnteredRoomID.clear();
}


core::CurInfo GetCurInfo()
{
	core::CurInfo user_info;
	memset(&user_info, 0x0, sizeof(core::CurInfo));
	IModuleManager * pModuleManager = g_RoomModule->m_pModuleManager;
	if (pModuleManager != NULL)
	{
		g_RoomModule->m_pModuleManager->CallService(core::SERVICE_VALUE_CORE_GET_CUR_INFO, (param)&user_info);
	}
	return user_info;
}

void RoomModule::Load(IModuleManager * pModuleManager)
{
	m_pModuleManager = pModuleManager;

	ListIconManager::LoadListIcon();
	RichLevelManager::LoadRichInfo();
	GiftManager::GetInstance()->LoadGiftList();
	//ScreenEffectManager::GetInstance()->Init();
	ScreenEffectManager::Init();
	EmotionMgr->LoadEmotionList() ;
	ShortCutMsgMgr->LoadShortCutMsgList() ;
	CaiZiMgr->LoadCaiZiList() ;
	EnterMgr->LoadEnterList() ;
	WelcomeMgr->LoadWelcomeList() ;
	VisitorMgr->LoadVisitorList() ;
	WealthyMgr->LoadWealthyList() ;
	EventEnterMgr->LoadEventEnterList() ;

	VideoManager::GetInstance()->Init(m_pRoomAvLogic);

	std::map<int, std::wstring> mapIconPath;
	ListIconManager::GetAllIcon(mapIconPath);
	ImageManager::getInstance()->AddAllIcon(mapIconPath);
	std::map<uint8, std::wstring> mapRichImgPath;
	RichLevelManager::GetAllRichImg(mapRichImgPath);
	ImageManager::getInstance()->AddAllRichImg(mapRichImgPath);
	std::map<uint8, std::wstring> mapWealthyImgPath;
	WealthyMgr->GetAllWealthyImg(mapWealthyImgPath);
	ImageManager::getInstance()->AddAllWealthyImg(mapWealthyImgPath);

	CreateNetStateTimer(ROOM_NETSTATE_TIMEOUT);

	m_bSpeedUpMode = common::utility::systemhelper::get_processor_number() == 1;

	//单核直接进入"加速"模式，不检测cpu占有率
	if(!m_bSpeedUpMode)
	{
		CreatePerformanceTimer(ROOM_PERFORMANCE_TIMEOUT);
	}
	else
	{
		m_pRoomAvLogic->SetEnableShapen(false);
	}
}

void RoomModule::Unload()
{
	Message msg;
	ProcessCleanRoom(msg);
	
	ListIconManager::Release();
	GiftManager::DeleteInstance();
	RichLevelManager::Release();
	//ScreenEffectManager::DeleteInstance();
	ScreenEffectManager::Release();
	EmotionMgr->Release() ;
	ShortCutMsgMgr->Release() ;
	CaiZiMgr->Release() ;
	EnterMgr->Release() ;
	AutoReplyMgr->Release();
	WealthyMgr->Release() ;
	VideoManager::DeleteInstance();
	CPersonalConfig::DeleteInstance();
	ImageManager::getInstance()->deleteInstance();
	VisitorMgr->Release() ;
	WelcomeMgr->Release() ;
	EventEnterMgr->Release() ;
	CVideoDlg::ReleaseImage();

	ClearNetStateTimer();
	ClearPerformanceTimer();
}

void RoomModule::ProcessEvent(Event const& evt)
{
	std::map<uint32,EventFun>::iterator iter ;
	iter = m_mEventFun.find( evt.eventValue ) ;
	if ( iter != m_mEventFun.end() )
	{
		EventFun fun = iter->second ;
		(this->*fun)( evt ) ;
	}
}

void RoomModule::ProcessMessage(Message const& msg)
{
	std::map<uint32,MessageFun>::iterator iter ;
	iter = m_mMessageFun.find( msg.messageValue ) ;
	if ( iter != m_mMessageFun.end() )
	{
		MessageFun fun = iter->second ;
		(this->*fun)( msg ) ;
	}

}

int32 RoomModule::Invoke(ServiceValue const serviceValue, param st)
{
	switch( serviceValue )
	{
	case Room::SERVICE_ROOM_CREATE_AND_SHOW:
		{
			return CreateAndShow(st) ;
		}
		break;
	case Room::SERVICE_ROOM_GETAUDIO_DEVICE_LIST:
		{
			return GetAudioDeviceList( st ) ;
		}
		break;
	case Room::SERVICE_ROOM_GETVIDEO_DEVICE_LIST:
		{
			return GetVideoDeviceList( st ) ;
		}
		break;	
	case Room::SERVICE_ROOM_SAVE_PHOTO_TOBMP:
		{
			return SavePhotoToBmp( st ) ;
		}
		break;
	case Room::SERVICE_ROOM_GET_NET_STATE:
		{
			return GetNetState( st ) ;
		}
		break;
	case Room::SERVICE_ROOM_GET_ROOM_LIST:
		{
			return GetRoomList( st ) ;
		}
		break;
	case Room::SERVICE_ROOM_CLOSE_A_ROOM:
		{
			return CloseARoom( st ) ;
		}
		break;
	case Room::SERVICE_ROOM_ACTIVE_A_ROOM:
		{
			return ActiveARoom( st ) ;
		}
		break;
	case Room::SERVICE_ROOM_CLOSED_REGISTER_DLG:
		{
			return ClosedRegisterDlg( st ) ;
		}
		break;
	case Room::SERVICE_ROOM_SHOW_SYSSETTING:
		{
			return ShowSysSetting(st);
		}
		break;
	case Room::SERVICE_ROOM_GET_RICHLEVEL_INFO:
		{
			return GetRichLevelInfo(st);
		}
		break;
	default:break;
	}
	return -1;
}

void RoomModule::OpenRoomDlg( Room_Create_And_Show * stCreateAndShow )
{
	uint32 unRoomID = stCreateAndShow->stEnterRoom.unRoomID;
	ASSERT(unRoomID != 0);
	CRoomParentDlg * pRoomDlg = FindRoomParentDlg(unRoomID);
	if (pRoomDlg != NULL)
	{
		pRoomDlg->ShowWindow(SW_RESTORE);
		pRoomDlg->SetActiveWindow();
		return;
	}

	SwitchResourceToModule(_T("Room"));
	core::CurInfo myCurInfo = GetCurInfo() ;
	uint32 uRelayIP, uRelayPort;
	if ( myCurInfo.bUseDianxin )
	{
		uRelayIP = stCreateAndShow->stEnterRoom.relayIP_dianxin;
		uRelayPort = stCreateAndShow->stEnterRoom.relayPort_dianxin;
	}
	else
	{
		uRelayIP = stCreateAndShow->stEnterRoom.relayIP_wangtong;
		uRelayPort = stCreateAndShow->stEnterRoom.relayPort_wangtong;
	}

	RoomOption * pRoomOption = new RoomOption(unRoomID);
	BaseRoomInfo stBaseInfo;
	stBaseInfo.strRoomName = stCreateAndShow->stEnterRoom.strRoomName;
	stBaseInfo.strRoomTheme = stCreateAndShow->stEnterRoom.strSignature;
	stBaseInfo.strOwnerName = stCreateAndShow->stEnterRoom.strOwnerName;
	stBaseInfo.unOwner179ID = stCreateAndShow->stEnterRoom.unOwner179ID;
	stBaseInfo.bOwnerMan = stCreateAndShow->stEnterRoom.bOwnerMan;
	pRoomOption->SetBaseRoomInfo(stBaseInfo);
	pRoomOption->LoadRoomOption(stCreateAndShow->stRoomInfo.map_key2string, stCreateAndShow->stRoomInfo.map_key2uin8, stCreateAndShow->stRoomInfo.map_key2uin32);

	int cx = GetSystemMetrics(SM_CXMAXIMIZED);
	int cy = GetSystemMetrics(SM_CYMAXIMIZED);
	int left = 0;
	int top = 0;
	if (cx > DLG_WIDTH)
	{
       left = (cx - DLG_WIDTH)/2;
	}
    if (cy > DLG_HEIGHT)
    {
		top = (cy - DLG_HEIGHT)/2;
    }

    CRoomParentDlg* pRoomParentDlg = new CRoomParentDlg(unRoomID, pRoomOption);
	if ( !pRoomParentDlg->InitAVLogic( unRoomID ,uRelayIP , uRelayPort  , stCreateAndShow->stEnterRoom.unAVKey ,*this ) )
	{
		delete pRoomParentDlg ;
		return ;
	}
	pRoomParentDlg->Create(CRoomParentDlg::IDD, CWnd::GetDesktopWindow());
	pRoomParentDlg->SetWindowPos(NULL,left,top,DLG_WIDTH,DLG_HEIGHT,SWP_SHOWWINDOW);	
	pRoomParentDlg->ShowWindow(SW_SHOWMAXIMIZED);
	pRoomParentDlg->CenterWindow();
	//pRoomParentDlg->ShowGuide();
	pRoomParentDlg->InitPublicMic( stCreateAndShow->stEnterRoom.vecPubMic ) ;
	pRoomParentDlg->SetCollectRoom(stCreateAndShow->stEnterRoom.bCollected);
	pRoomParentDlg->OnUpdateMoney( myCurInfo.unLeftMoney , TRUE ) ;

	UserManager * pUserManager = new UserManager(unRoomID, static_cast<IUserManagerSink*>(pRoomParentDlg));
	CSofaManager * pSofaManager = new CSofaManager(unRoomID, static_cast<ISofaManagerSink*>(pRoomParentDlg));
	RoomBusiness * pRoomBusiness = new RoomBusiness(unRoomID, pRoomOption, pUserManager, pSofaManager, static_cast<IRoomBusinessSink*>(pRoomParentDlg));

	pRoomParentDlg->SetRoomBusiness(pRoomBusiness);

	m_mapRoomParentDlg[unRoomID] = pRoomParentDlg;
	m_vRoomIDList.push_back( unRoomID ) ;
	if (m_mapEnteredRoomID.find(unRoomID) == m_mapEnteredRoomID.end())//没进过此房间
	{
		m_mapEnteredRoomID.insert(std::make_pair<uint32, bool>(unRoomID, false));
	}else
	{
		m_mapEnteredRoomID[unRoomID] = true;
	}

	if (m_pModuleManager != NULL)
	{
		m_pModuleManager->PushEvent( MakeEvent<MODULE_ID_ROOM>()(core::EVENT_VALUE_CORE_GET_USERLIST,MODULE_ID_CORE,unRoomID) );
	}

#if 0
	core::GetUserListRspEvent * pEvent = new core::GetUserListRspEvent();
	pEvent->bcomplete = true;
	pEvent->roomid = unRoomID;

	for (int i = 0; i < 100; i++)
	{
		core::room_user_info userinfo;
		userinfo.unUIN = 1000 + i;
		userinfo.strName = common::utility::stringhelper::IntToString(userinfo.unUIN);
		userinfo.unVipLevel = 0x80 >> (rand()%9);
		//userinfo.unTitleLevel = core::TITLELEVEL_NONE;
		userinfo.unTitleLevel = 0x10 >> (rand()%6);
		userinfo.unBusiness = 0x0000;
		//userinfo.unBusiness = 0x40 >> (rand()%7);
		//if (userinfo.unBusiness == core::BUSINESSTYPE_PATROLMAN)
		//{
		//	userinfo.unTitleLevel = core::TITLELEVEL_SUPER_MANAGER;
		//}
		if (userinfo.unTitleLevel == core::TITLELEVEL_SUPER_MANAGER)
		{
			userinfo.unBusiness = core::BUSINESSTYPE_PATROLMAN;
		}
		userinfo.unUsrLevel = rand()%100;
		userinfo.unStatus = 0;
		userinfo.un179id = 2000 + i;
		pEvent->vecUsrList.push_back(userinfo);
	}
	core::room_user_info use_info ;
	use_info.unUIN = 8888 ;
	use_info.unStatus = USERSTATUSTYPE_PRIVATE ;
	use_info.unStatus |= USERSTATUSTYPE_VIDEO ;
	use_info.unVipLevel = 0x01<<(rand()%8) ;
	use_info.unTitleLevel = 0x10>>(rand()%6) ;
	use_info.unUsrLevel = rand()%100 ;
	use_info.strName = common::utility::stringhelper::IntToString( 8888 ) ;
	pEvent->vecUsrList.push_back( use_info ) ;

	use_info.unUIN = 2727 ;
	use_info.unStatus = USERSTATUSTYPE_VIDEO ;
	use_info.unStatus |= USERSTATUSTYPE_AUDIO ;
	use_info.strName = common::utility::stringhelper::IntToString( 2727 ) ;
	pEvent->vecUsrList.push_back( use_info ) ;
	m_pModuleManager->PushEvent( MakeEvent<MODULE_ID_ROOM>()(core::EVENT_VALUE_CORE_GET_USERLIST_RSP, MODULE_ID_ROOM, 0, 0, 0, pEvent) );
#endif
}


CRoomParentDlg* RoomModule::FindRoomParentDlg(uint32 roomid ) 
{	
	if (m_mapRoomParentDlg.find(roomid) == m_mapRoomParentDlg.end())
	{
		return NULL;
	}

	return m_mapRoomParentDlg[roomid];
}

int32 RoomModule::CreateAndShow(param st ) 
{
	Room_Create_And_Show* prcas = (Room_Create_And_Show*)st ;
	if ( prcas != NULL )
	{
		OpenRoomDlg(prcas);		
		return 0;
	}

	return -1;
}

int32 RoomModule::GetAudioDeviceList(param st) 
{
	Room_Get_Audio_Device_List* pgvdl = (Room_Get_Audio_Device_List*)st ;
	if ( pgvdl != NULL )
	{
		m_pRoomAvLogic->GetAudioDeviceList( pgvdl->iDeviceNum , pgvdl->wsDeviceName , pgvdl->bInput ) ;
		return 0 ;
	}
	return -1 ;
}

int32 RoomModule::GetVideoDeviceList(param st ) 
{
	Room_Get_Video_Device_List *pgvdl = (Room_Get_Video_Device_List*)st;
	if (pgvdl != NULL)
	{
		VideoManager::GetInstance()->ReloadVideoDevice();
		VecVideoInfo vecVideoList = VideoManager::GetInstance()->GetVideoList();
		pgvdl->iDeviceNum = vecVideoList.size();
		for (int i = 0; i < vecVideoList.size(); ++i)
		{
			wcsncpy_s(&pgvdl->wsDeviceName[i][0], DEVICE_NAME_LEN, vecVideoList[i].strName.c_str(), DEVICE_NAME_LEN);
		}
		return 0;
	}

	//if ( pgvdl != NULL )
	//{
	//	m_pRoomAvLogic->GetVideoDeviceList( pgvdl->iDeviceNum , pgvdl->wsDeviceName ) ;
	//	return 0 ;
	//}
	return -1 ;
}

int32 RoomModule::SavePhotoToBmp(param st ) 
{
	Room_Save_Photo_ToBmp* psptb = ( Room_Save_Photo_ToBmp*)st ;
	if ( psptb != NULL )
	{
		m_pRoomAvLogic->SavePhotoToBmp( psptb->hWnd , CString(psptb->strPath.c_str())) ;
		return 0 ;
	}
	return -1 ;
}

int32 RoomModule::GetNetState(param st ) 
{
	Room_Get_Net_State* pgns = (Room_Get_Net_State*)st ;
	if ( pgns != NULL )
	{
		m_pRoomAvLogic->GetNetState( pgns->fRate ) ;
		return 0 ;
	}
	return -1 ;
}

int32 RoomModule::GetRoomList(param st )
{
	Room_Get_Room_List& roomList = *(Room_Get_Room_List*)st;
	roomList.vecRoomList = m_vRoomIDList ;
	return 0;
}


int32 RoomModule::CloseARoom(param st ) 
{
	CRoomParentDlg* pRoomParentDlg = FindRoomParentDlg( st ) ;
	if ( pRoomParentDlg != NULL )
	{
		pRoomParentDlg->SendMessage( WM_CLOSE , 0 , 0 ) ;
	}
	return 0 ;
}

int32 RoomModule::ActiveARoom(param st ) 
{
	CRoomParentDlg* pRoomParentDlg = FindRoomParentDlg( st ) ;
	if ( pRoomParentDlg != NULL )
	{
		pRoomParentDlg->SendMessage( WM_SYSCOMMAND , SC_MAXIMIZE , 0 ) ;
		pRoomParentDlg->SetActiveWindow() ;
	}
	return 0 ;
}

int32 RoomModule::ClosedRegisterDlg(param st ) 
{
	if ( !m_mapRoomParentDlg.empty() )
	{
		std::map<uint32, CRoomParentDlg*>::iterator it = m_mapRoomParentDlg.begin() ;
		CRoomParentDlg * pRoomParentDlg = it->second;
		if (pRoomParentDlg != NULL)
		{
			pRoomParentDlg->ClosedRegisterDlg() ;
		}
	}
	return 0 ;
}

int32 RoomModule::ShowSysSetting(param st )
{
	SysSettingType& dlgtype = *(SysSettingType*)st;
	CWnd *pParentWnd = CWnd::FromHandle(dlgtype.pParentWnd);
	if(m_pRoomSysSettingDlg == NULL)
	{
		//创建菜单设置
		m_pRoomAvLogic->InitMixerControl(NULL);
		
		SwitchResourceToModule(_T("Room"));
		m_pRoomSysSettingDlg = new CSysSettingDlg(pParentWnd);		
		m_pRoomSysSettingDlg->Create(CSysSettingDlg::IDD,pParentWnd);			
	}	

	m_pRoomSysSettingDlg->OpenDlg(dlgtype.dlgType);
	m_pRoomSysSettingDlg->ShowWindow(SW_SHOW);

	
	return 0;
}

void RoomModule::ProcessLoginComplete( Message const& msg ) 
{
	if(msg.param0 == core::LOGIN_ERROR_SUCCESS)
	{
		CPersonalConfig::GetInstance()->Init(GetCurInfo().unSelfSetting);

		std::wstring strDevice = CPersonalConfig::GetInstance()->GetUseVideoDevice();
		if (strDevice != L"")
		{
			Room_Get_Video_Device_Info_List videolist;
			memset(&videolist, 0x0, sizeof(Room_Get_Video_Device_Info_List));
			m_pRoomAvLogic->GetVideoDeviceInfoList(videolist.iDeviceNum, videolist.stDeviceInfo);

			for(int i=0;i<videolist.iDeviceNum;i++)
			{
				if (strDevice == videolist.stDeviceInfo[i].wsDeviceGUID)
				{
					CPersonalConfig::GetInstance()->SetUseVideoDeviceIndex(videolist.stDeviceInfo[i].iDeviceId);
				}
			}
		}
		//加载自动回复
		AutoReplyMgr->LoadAutoReplyList();

		if(m_pRoomSysSettingDlg != NULL && m_pRoomSysSettingDlg->IsWindowVisible())
		{		
			m_pRoomSysSettingDlg->ShowWindow(SW_HIDE);		
		}
	}	
}

//清空房间
void RoomModule::ProcessCleanRoom( Message const& msg )		
{
	while( !m_mapRoomParentDlg.empty() )
	{
		int size = m_mapRoomParentDlg.size();

		std::map<uint32, CRoomParentDlg*>::iterator it = m_mapRoomParentDlg.begin() ;
		CRoomParentDlg * pRoomParentDlg = it->second;
		if (pRoomParentDlg != NULL)
		{
			pRoomParentDlg->SendMessage( WM_CLOSE , 0 , 0 ) ;
		}

		if(m_mapRoomParentDlg.size() != size-1)
		{
			assert(0);
			break;
		}
	}
}

//通用的Web访问接口返回响应
void RoomModule::ProcessWebGetHttpResp(Event const&evt)
{
	webinterface::get_http_url_event_resp* phttpResp =(webinterface::get_http_url_event_resp* ) (evt.m_pstExtraInfo);
	if ( phttpResp != NULL )
	{
		for (MapRoomParentDlg::iterator it = m_mapRoomParentDlg.begin(); it != m_mapRoomParentDlg.end(); ++it)
		{
			CRoomParentDlg * pRoomParentDlg = it->second;
			if (pRoomParentDlg != NULL)
			{
				RoomBusiness * pRoomBusiness = pRoomParentDlg->GetRoomBusiness();
				ASSERT(pRoomBusiness != NULL);
				if ( pRoomBusiness->GetSearchSeqNo() == evt.param2 )
				{
					if ( evt.param0 == webinterface::WEB_RET_SUCCESS )
					{
						int nResult = 0 ;
						sscanf( phttpResp->strBuffer.c_str() , "{\"result\":%d}\"" ,&nResult ) ;
						if ( nResult > 0 )  
						{
							C179MsgBox::Show( pRoomParentDlg, NewMsgBox::MSGBOX_ICO_OK_ONE_STRING , NewMsgBox::ICONTYPE_BINGO , L"收藏成功             ");
							//DragonZ:
							pRoomParentDlg->SetCollectRoom(TRUE);
						}
						else if ( nResult == -1 )//收藏的房间已经存在自身的收藏夹列表中
						{
							C179MsgBox::Show( pRoomParentDlg, NewMsgBox::MSGBOX_OK_ONE_STRING ,L"您已经收藏过此房间" );
						}
						else if ( nResult == -2 )   //收藏房间数量已满
						{
							C179MsgBox::Show( pRoomParentDlg, NewMsgBox::MSGBOX_OK_ONE_STRING ,L"收藏的房间数量已达上限" );
						}
						break;
					}	
					else
					{
						//收藏失败
						C179MsgBox::Show( pRoomParentDlg, NewMsgBox::MSGBOX_OK_ONE_STRING , L"收藏房间失败,请稍后再试" );
						break;
					}
				}								
			}
		}
	}
}


void RoomModule::ProcessGetUserListRsp( Event const& evt ) 
{
	ASSERT(evt.m_pstExtraInfo != NULL);
	core::GetUserListRspEvent * pInfo = (core::GetUserListRspEvent*)evt.m_pstExtraInfo;

	CRoomParentDlg * pRoomParentDlg = FindRoomParentDlg(pInfo->roomid);
	if (pRoomParentDlg != NULL)
	{
		RoomBusiness * pRoomBusiness = pRoomParentDlg->GetRoomBusiness();
		ASSERT(pRoomBusiness != NULL);
		pRoomParentDlg->OnGetUserListBegin();
		pRoomBusiness->ProcessGetUserListRsp(evt);
		pRoomParentDlg->OnGetUserListComplete();
		m_RoomParentEvent.ProcessEvent( pRoomParentDlg , evt ) ;

		if (pInfo->bcomplete)
		{
			if (m_pModuleManager != NULL)
			{
				m_pModuleManager->PushEvent(MakeEvent<MODULE_ID_ROOM>()(core::EVENT_VALUE_CORE_GET_WAITTINGLIST,MODULE_ID_CORE,pInfo->roomid));
			}
		}
	}
}

void RoomModule::ProcessGetWaitingListRsp( Event const& evt ) 
{
	ASSERT(evt.m_pstExtraInfo != NULL);
	core::GetWaittinglistRspEvent * pInfo = (core::GetWaittinglistRspEvent*)evt.m_pstExtraInfo;

	CRoomParentDlg * pRoomDlg = FindRoomParentDlg(pInfo->roomid);
	if (pRoomDlg != NULL)
	{
		RoomBusiness * pRoomBusiness = pRoomDlg->GetRoomBusiness();
		ASSERT(pRoomBusiness != NULL);
		pRoomBusiness->ProcessGetWaitingListRsp(evt);
		m_RoomParentEvent.ProcessEvent( pRoomDlg , evt ) ;
		AddRecentBrowseRoom(pInfo->roomid);
	}

	if (m_pModuleManager != NULL)
	{
		m_pModuleManager->PushEvent(MakeEvent<MODULE_ID_ROOM>()(core::EVENT_VALUE_CORE_GET_SOFA_LIST, MODULE_ID_CORE, pInfo->roomid));
	}
}

void RoomModule::ProcessEnterRoomNotify(Event const& evt ) 
{
	ASSERT(evt.m_pstExtraInfo != NULL);
	core::EnterRoomNotifyEvent * pInfo = (core::EnterRoomNotifyEvent*)evt.m_pstExtraInfo;
	CRoomParentDlg * pRoomDlg = FindRoomParentDlg(pInfo->unRoomID);
	if (pRoomDlg != NULL)
	{
		RoomBusiness * pRoomBusiness = pRoomDlg->GetRoomBusiness();
		ASSERT(pRoomBusiness != NULL);
		pRoomBusiness->ProcessEnterRoomNotify(evt);
	}
}

void RoomModule::ProcessLeaveRoomNotify(Event const& evt ) 
{
	ASSERT(evt.param1 != 0);
	CRoomParentDlg * pRoomDlg = FindRoomParentDlg(evt.param1);
	if (pRoomDlg != NULL)
	{
		RoomBusiness * pRoomBusiness = pRoomDlg->GetRoomBusiness();
		ASSERT(pRoomBusiness != NULL);
		ASSERT(evt.param0 != 0);
		pRoomBusiness->ProcessLeaveRoomNotify(evt);
	}
}

void RoomModule::ProcessNotifyUserInfoUpdate(Event const& evt ) 
{
	ASSERT(evt.m_pstExtraInfo != NULL);
	core::NotifyUserInfoUpdateEvent * pInfo = (core::NotifyUserInfoUpdateEvent*)evt.m_pstExtraInfo;
	CRoomParentDlg * pRoomDlg = FindRoomParentDlg(pInfo->unRoomID);
	if (pRoomDlg != NULL)
	{
		RoomBusiness * pRoomBusiness = pRoomDlg->GetRoomBusiness();
		ASSERT(pRoomBusiness != NULL);
		pRoomBusiness->ProcessNotifyUserInfoUpdate(evt);

		CRoomTopManager *pRoomTopManager = pRoomDlg->GetRoomTopManager();
		if(pRoomTopManager != NULL)
		{
			pRoomTopManager->ProcessNotifyUserInfoUpdate(evt);
		}
	}
}

void RoomModule::ProcessQueryClientInfoRsp(Event const& evt ) 
{

}

void RoomModule::ProcessSetRoomInfoRsp(Event const& evt) 
{
	ASSERT(evt.param0 != NULL);
	CRoomParentDlg * pRoomParentDlg = FindRoomParentDlg(evt.param0);
	if (pRoomParentDlg != NULL)
	{
		RoomBusiness * pRoomBusiness = pRoomParentDlg->GetRoomBusiness();
		ASSERT( pRoomBusiness != NULL ) ;
		pRoomBusiness->ProcessSetRoomInfoRsp(evt);
		m_RoomParentEvent.ProcessEvent( pRoomParentDlg , evt ) ;
	}
}

void RoomModule::ProcessSetRoomInfoNotify(Event const& evt ) 
{
	ASSERT(evt.m_pstExtraInfo != NULL);
	core::SetRoomInfoNotifyEvent * pInfo = (core::SetRoomInfoNotifyEvent*)evt.m_pstExtraInfo;
	ASSERT(pInfo->unRoomID != 0);
	CRoomParentDlg * pRoomParentDlg = FindRoomParentDlg(pInfo->unRoomID);
	if (pRoomParentDlg != NULL)
	{
		RoomBusiness * pRoomBusiness = pRoomParentDlg->GetRoomBusiness();
		ASSERT( pRoomBusiness != NULL ) ;
		pRoomBusiness->ProcessSetRoomInfoNotify(evt);
		m_RoomParentEvent.ProcessEvent( pRoomParentDlg , evt ) ;
	}
}

void RoomModule::ProcessGetRoomInfoRsp(Event const& evt ) 
{
	ASSERT(evt.m_pstExtraInfo != NULL);
	core::GetRoomInfoRspEvent * pInfo = (core::GetRoomInfoRspEvent*)evt.m_pstExtraInfo;
	ASSERT(pInfo->unRoomID != 0);
	CRoomParentDlg * pRoomParentDlg = FindRoomParentDlg(pInfo->unRoomID);
	if (pRoomParentDlg != NULL)
	{
		RoomBusiness * pRoomBusiness = pRoomParentDlg->GetRoomBusiness();
		ASSERT( pRoomBusiness != NULL ) ;
		pRoomBusiness->ProcessGetRoomInfoRsp(evt);
	}
}

void RoomModule::ProcessLockRoomNotify(Event const& evt ) 
{
	ASSERT(evt.m_pstExtraInfo != NULL ) ;
	core::NotifyRoomLockedEvent* pInfo = ( core::NotifyRoomLockedEvent*)evt.m_pstExtraInfo ;
	CRoomParentDlg* pRoomParentDlg = FindRoomParentDlg( pInfo->unRoomID ) ;
	if ( pRoomParentDlg != NULL )
	{		
		RoomOption* pRoomOption = pRoomParentDlg->GetRoomBusiness()->GetRoomOption() ;
		ASSERT(pRoomOption != NULL);
		core::CurInfo myCurInfo = GetCurInfo() ;
		Hall::MsgBox179 msgbox179 ;

		CString strTip1 , strTip2 , strTip3 ;
		if ( pInfo->lockdays >= 365*10 )
		{
			strTip2 = L"永久封停" ;
		}
		else
		{
			strTip2.Format( L"被封停%d天" , pInfo->lockdays ) ;
		}

		if ( pRoomOption->GetOwner179ID() == myCurInfo.un179id )
		{
			//室主在线		
			strTip1.Format( L"您的房间%u因\"%s\"" ,pRoomParentDlg->GetRoomID() , pInfo->strReason.c_str() ) ;
			strTip3 = L"如有疑问，请联系官方客服" ;
		}
		else
		{				
			strTip1.Format( L"您所在的房间%u因\"%s\"" , pRoomParentDlg->GetRoomID() , pInfo->strReason.c_str() ) ;
			strTip3 = L"由此给您带来的不便，敬请谅解！" ;
		}


		msgbox179.m_sMessage1 = strTip1 ;
		msgbox179.m_sMessage2 = strTip2 ;
		msgbox179.m_sMessage3 = strTip3 ;
		msgbox179.m_sTitle = L"房间提示" ;
		msgbox179.m_boxStype = Hall::MsgBox179::MSGBOX_OK_THREE_STRING ;

		KickLeaveRoom( pInfo->unRoomID ) ;
		m_pModuleManager->CallService( Hall::SERVICE_MSGBOX_SHOW ,(param)&msgbox179 ) ;	
	}
}


//中转服务器IP地址变更
void RoomModule::ProcessMediaServerResetNotify(Event const& evt ) 
{
	ASSERT( evt.m_pstExtraInfo != NULL ) ;
	core::NotifyMediaServerResetEvent* pInfo = ( core::NotifyMediaServerResetEvent*)evt.m_pstExtraInfo ;
	CRoomParentDlg* pRoomParentDlg = FindRoomParentDlg( pInfo->unRoomID ) ;
	if ( pRoomParentDlg != NULL )
	{
		m_RoomParentEvent.ProcessEvent( pRoomParentDlg , evt ) ;
	}
}

void RoomModule::ProcessSendMessageRsp(Event const& evt ) 
{
	ASSERT(evt.m_pstExtraInfo != NULL ) ;
	core::SendMessageRspEvent *pInfo = (core::SendMessageRspEvent*)evt.m_pstExtraInfo ;
	CRoomParentDlg* pRoomParentDlg = FindRoomParentDlg( pInfo->roomid ) ;
	if ( pRoomParentDlg != NULL )
	{
		m_RoomParentEvent.ProcessEvent( pRoomParentDlg , evt ) ;
	}
}

void RoomModule::ProcessSendMessageNotify(Event const& evt ) 
{
	ASSERT(evt.m_pstExtraInfo != NULL ) ;
	core::SendMessageNotifyEvent *pInfo = (core::SendMessageNotifyEvent*)evt.m_pstExtraInfo ;
	CRoomParentDlg* pRoomParentDlg = FindRoomParentDlg( pInfo->roomid ) ;
	if ( pRoomParentDlg != NULL )
	{
		m_RoomParentEvent.ProcessEvent( pRoomParentDlg , evt ) ;
	}
}


//喇叭
void RoomModule::ProcessSendLoudSpeakerRsp(Event const& evt ) 
{
	ASSERT(evt.m_pstExtraInfo != NULL);
	core::SendLoudSpeakerRspEvent * pInfo = (core::SendLoudSpeakerRspEvent*)evt.m_pstExtraInfo;
	CRoomParentDlg* pRoomParentDlg = FindRoomParentDlg( pInfo->unRoomID ) ;
	if ( pRoomParentDlg != NULL )
	{
		m_RoomParentEvent.ProcessEvent( pRoomParentDlg , evt ) ;
	}

}

//喇叭
void RoomModule::ProcessSendLoudSpeakerNotify(Event const& evt ) 
{
	ASSERT(evt.m_pstExtraInfo != NULL);
	core::SendLoudSpeakerNotifyEvent * pInfo = (core::SendLoudSpeakerNotifyEvent*)evt.m_pstExtraInfo;
	if (pInfo->unRoomID != 0)
	{
		CRoomParentDlg* pRoomParentDlg = FindRoomParentDlg( pInfo->unRoomID ) ;
		if ( pRoomParentDlg != NULL )
		{
			m_RoomParentEvent.ProcessEvent( pRoomParentDlg , evt ) ;
		}
	}
	else
	{
		for ( MapRoomParentDlg::iterator it = m_mapRoomParentDlg.begin(); it != m_mapRoomParentDlg.end() ; ++it )
		{
			CRoomParentDlg* pRoomParentDlg = it->second ;
			if ( pRoomParentDlg != NULL )
			{
				m_RoomParentEvent.ProcessEvent( pRoomParentDlg , evt ) ;
			}
		}
	}
}

void RoomModule::ProcessRequestShowRsp(Event const& evt ) 
{
	ASSERT(evt.m_pstExtraInfo != NULL ) ;
	core::RequestShowRspEvent * pInfo = (core::RequestShowRspEvent*)evt.m_pstExtraInfo ;
	CRoomParentDlg* pRoomParentDlg = FindRoomParentDlg( pInfo->unRoomID ) ;
	if ( pRoomParentDlg != NULL )
	{
		RoomBusiness *pRoomBusiness = pRoomParentDlg->GetRoomBusiness() ;
		ASSERT( pRoomBusiness!= NULL ) ;
		pRoomBusiness->ProcessRequestShowRsp(evt);
		m_RoomParentEvent.ProcessEvent( pRoomParentDlg , evt ) ;
	}
}

void RoomModule::ProcessRequestShowNotify(Event const& evt ) 
{
	ASSERT(evt.m_pstExtraInfo != NULL ) ;
	core::RequestShowNotifyEvent* pInfo = (core::RequestShowNotifyEvent*)evt.m_pstExtraInfo ;
	CRoomParentDlg* pRoomParentDlg = FindRoomParentDlg( pInfo->unRoomID ) ;
	if ( pRoomParentDlg != NULL )
	{
		RoomBusiness *pRoomBusiness = pRoomParentDlg->GetRoomBusiness() ;
		ASSERT( pRoomBusiness!= NULL ) ;
		pRoomBusiness->ProcessRequestShowNotify(evt);
		m_RoomParentEvent.ProcessEvent( pRoomParentDlg , evt ) ;
	}
}

void RoomModule::ProcessExitShowNotify(Event const& evt) 
{
	CRoomParentDlg* pRoomParentDlg = FindRoomParentDlg( evt.param0 ) ;
	if ( pRoomParentDlg != NULL )
	{
		RoomBusiness *pRoomBusiness = pRoomParentDlg->GetRoomBusiness() ;
		ASSERT( pRoomBusiness!= NULL ) ;
		m_RoomParentEvent.ProcessEvent( pRoomParentDlg , evt ) ;
		pRoomBusiness->ProcessExitShowNotify(evt);		
	}
}

void RoomModule::ProcessOpenPrivateRsp(Event const& evt ) 
{
	CRoomParentDlg* pRoomParentDlg = FindRoomParentDlg( evt.param0 ) ;
	if ( pRoomParentDlg != NULL )
	{
		RoomBusiness *pRoomBusiness = pRoomParentDlg->GetRoomBusiness() ;
		ASSERT( pRoomBusiness!= NULL ) ;
		pRoomBusiness->ProcessOpenPrivateRsp(evt);
		m_RoomParentEvent.ProcessEvent( pRoomParentDlg , evt ) ;
	}
}
void RoomModule::ProcessPushOnShowRsp(Event const& evt ) 
{
	ASSERT( evt.m_pstExtraInfo != NULL ) ;
	core::PushOnShowRspEvent* pInfo = (core::PushOnShowRspEvent*)evt.m_pstExtraInfo ;
	CRoomParentDlg* pRoomParentDlg = FindRoomParentDlg( pInfo->unRoomID ) ;
	if ( pRoomParentDlg != NULL )
	{
		RoomBusiness *pRoomBusiness = pRoomParentDlg->GetRoomBusiness() ;
		ASSERT( pRoomBusiness!= NULL ) ;
		m_RoomParentEvent.ProcessEvent( pRoomParentDlg , evt ) ;
		pRoomBusiness->ProcessPushOnShowRsp(evt);		
	}
}

void RoomModule::ProcessPushOnShowNotify(Event const& evt ) 
{
	ASSERT(evt.m_pstExtraInfo != NULL);
	core::PushOnShowNotifyEvent* pInfo = (core::PushOnShowNotifyEvent*)evt.m_pstExtraInfo ;
	CRoomParentDlg* pRoomParentDlg = FindRoomParentDlg( pInfo->unRoomID ) ;
	if ( pRoomParentDlg != NULL )
	{
		RoomBusiness *pRoomBusiness = pRoomParentDlg->GetRoomBusiness() ;
		ASSERT( pRoomBusiness!= NULL ) ;		 
		pRoomBusiness->ProcessPushOnShowNotify(evt);
		m_RoomParentEvent.ProcessEvent( pRoomParentDlg , evt ) ;
	}
}

void RoomModule::ProcessPullOffShowRsp(Event const& evt ) 			//邀请下麦
{
	CRoomParentDlg* pRoomParentDlg = FindRoomParentDlg(  evt.param0 ) ;
	if ( pRoomParentDlg != NULL )
	{
		RoomBusiness* pRoomBusiness = pRoomParentDlg->GetRoomBusiness() ;
		ASSERT( pRoomBusiness != NULL ) ;
		m_RoomParentEvent.ProcessEvent( pRoomParentDlg , evt ) ;
		pRoomBusiness->ProcessPullOffShowRsp(evt);		
	}
}

void RoomModule::ProcessPullOffShowNotify(Event const& evt ) 
{
	CRoomParentDlg* pRoomParentDlg = FindRoomParentDlg(  evt.param0 ) ;
	if ( pRoomParentDlg != NULL )
	{
		RoomBusiness* pRoomBusiness = pRoomParentDlg->GetRoomBusiness() ;
		ASSERT( pRoomBusiness != NULL ) ;
		m_RoomParentEvent.ProcessEvent( pRoomParentDlg , evt ) ;
		pRoomBusiness->ProcessPullOffShowNotify(evt);		
	}
}

//打开、关闭音视频
void RoomModule::ProcessOpVideoAudioRsp( Event const& evt ) 
{
	ASSERT(evt.m_pstExtraInfo != NULL ) ;
	core::OperateVideoAudioRspEvent* pInfo = (core::OperateVideoAudioRspEvent*)evt.m_pstExtraInfo ;
	CRoomParentDlg* pRoomParentDlg = FindRoomParentDlg( pInfo->unRoomID ) ;
	if ( pRoomParentDlg != NULL )
	{
		RoomBusiness *pRoomBusiness = pRoomParentDlg->GetRoomBusiness() ;
		ASSERT( pRoomBusiness!= NULL ) ;
		pRoomBusiness->ProcessOpVideoAudioRsp(evt);
		m_RoomParentEvent.ProcessEvent( pRoomParentDlg , evt ) ;
	}
}

void RoomModule::ProcessOpVideoAudioNotify( Event const& evt ) 
{
	CRoomParentDlg* pRoomParentDlg = FindRoomParentDlg( evt.param0 ) ;
	if ( pRoomParentDlg != NULL )
	{
		RoomBusiness *pRoomBusiness = pRoomParentDlg->GetRoomBusiness() ;
		ASSERT( pRoomBusiness!= NULL ) ;
		pRoomBusiness->ProcessOpVideoAudioNotify(evt);
		m_RoomParentEvent.ProcessEvent( pRoomParentDlg , evt ) ;
	}
}

void RoomModule::ProcessRequestP2PRsp(Event const& evt ) 
{
	CRoomParentDlg* pRoomParentDlg = FindRoomParentDlg( evt.param0 ) ;
	if ( pRoomParentDlg != NULL )
	{
		RoomBusiness* pRoomBusiness = pRoomParentDlg->GetRoomBusiness() ;
		ASSERT( pRoomBusiness != NULL ) ;
		pRoomBusiness->ProcessRequestP2PRsp(evt);
		m_RoomParentEvent.ProcessEvent( pRoomParentDlg , evt ) ;
	}
}

void RoomModule::ProcessRequestP2PNotify(Event const& evt ) 
{
	CRoomParentDlg* pRoomParentDlg = FindRoomParentDlg( evt.param0 ) ;
	if ( pRoomParentDlg != NULL )
	{
		RoomBusiness* pRoomBusiness = pRoomParentDlg->GetRoomBusiness() ;
		ASSERT( pRoomBusiness != NULL ) ;
		pRoomBusiness->ProcessRequestP2PNotify(evt);
		m_RoomParentEvent.ProcessEvent( pRoomParentDlg , evt ) ;
	}
}

void RoomModule::ProcessResponseP2PRsp(Event const& evt ) 
{
	core::ResponseP2PRspEvent* pInfo = ( core::ResponseP2PRspEvent*)evt.m_pstExtraInfo ;
	CRoomParentDlg* pRoomParentDlg = FindRoomParentDlg( pInfo->unRoomID ) ;
	if ( pRoomParentDlg != NULL )
	{
		RoomBusiness* pRoomBusiness = pRoomParentDlg->GetRoomBusiness() ;
		ASSERT( pRoomBusiness != NULL ) ;
		pRoomBusiness->ProcessResponseP2PRsp(evt);
		m_RoomParentEvent.ProcessEvent( pRoomParentDlg , evt ) ;
	}
}

void RoomModule::ProcessResponseP2PNotify(Event const& evt ) 
{
	core::ResponseP2PNotifyEvent* pInfo = (core::ResponseP2PNotifyEvent*)evt.m_pstExtraInfo ;
	CRoomParentDlg* pRoomParentDlg = FindRoomParentDlg( pInfo->unRoomID ) ;
	if ( pRoomParentDlg != NULL )
	{
		RoomBusiness* pRoomBusiness = pRoomParentDlg->GetRoomBusiness() ;
		ASSERT( pRoomBusiness != NULL ) ;
		pRoomBusiness->ProcessResponseP2PNotify(evt);
		m_RoomParentEvent.ProcessEvent( pRoomParentDlg , evt ) ;
	}
}

void RoomModule::ProcessSendGiftRsp(Event const& evt ) 
{
	ASSERT(evt.m_pstExtraInfo != NULL);
	core::SendGiftRspEvent * pInfo = (core::SendGiftRspEvent*)evt.m_pstExtraInfo;
	CRoomParentDlg* pRoomParentDlg = FindRoomParentDlg( pInfo->unRoomID ) ;
	if ( pRoomParentDlg != NULL )
	{
		RoomBusiness* pRoomBusiness = pRoomParentDlg->GetRoomBusiness() ;
		ASSERT( pRoomBusiness != NULL ) ;
		//pRoomBusiness->OnSendGiftRsp( pInfo ) ;
		pRoomBusiness->ProcessSendGiftRsp(evt);
		m_RoomParentEvent.ProcessEvent( pRoomParentDlg , evt ) ;
	}	
}

void RoomModule::ProcessSendGiftNotify(Event const& evt )
{
	ASSERT(evt.m_pstExtraInfo != NULL ) ;
	core::SendGiftNotifyEvent* pInfo = (core::SendGiftNotifyEvent*)evt.m_pstExtraInfo ;
	CRoomParentDlg* pRoomParentDlg = FindRoomParentDlg( pInfo->unRoomID ) ;
	if ( pRoomParentDlg != NULL )
	{
		RoomBusiness* pRoomBusiness = pRoomParentDlg->GetRoomBusiness() ;
		ASSERT( pRoomBusiness != NULL ) ;
		//pRoomBusiness->OnSendGiftNotify( pInfo ) ;
		pRoomBusiness->ProcessSendGiftNotify(evt);
		m_RoomParentEvent.ProcessEvent( pRoomParentDlg , evt ) ;
	}
}

void RoomModule::ProcessNotifyPrivateShow(Event const& evt ) 
{
	ASSERT(evt.m_pstExtraInfo != NULL);
	core::NotifyPrivateShowEvent * pInfo = (core::NotifyPrivateShowEvent*)evt.m_pstExtraInfo;
	CRoomParentDlg* pRoomParentDlg = FindRoomParentDlg( pInfo->unRoomID ) ;
	if ( pRoomParentDlg != NULL )
	{
		m_RoomParentEvent.ProcessEvent( pRoomParentDlg , evt ) ;
	}
}

void RoomModule::ProcessSetMicStatusRsp( Event const& evt ) //限制使用此麦
{
	CRoomParentDlg* pRoomParentDlg = FindRoomParentDlg(evt.param0 ) ;
	if ( pRoomParentDlg != NULL )
	{
		RoomBusiness* pRoomBusiness = pRoomParentDlg->GetRoomBusiness() ;
		m_RoomParentEvent.ProcessEvent( pRoomParentDlg , evt ) ;
		pRoomBusiness->ProcessSetMicStatusRsp( evt ) ;		
	}
}

void RoomModule::ProcessSetMicStatusNotify( Event const& evt ) 
{
	CRoomParentDlg* pRoomParentDlg = FindRoomParentDlg(evt.param0 ) ;
	if ( pRoomParentDlg != NULL )
	{
		RoomBusiness* pRoomBusiness = pRoomParentDlg->GetRoomBusiness() ;
		m_RoomParentEvent.ProcessEvent( pRoomParentDlg , evt ) ;
		pRoomBusiness->ProcessSetMicStatusNotify( evt ) ;		
	}
}

void RoomModule::ProcessKickUserRsp(Event const& evt ) 
{
	ASSERT(evt.m_pstExtraInfo != NULL ) ;
	core::KickUserRspEvent * pInfo = (core::KickUserRspEvent*)evt.m_pstExtraInfo ;
	CRoomParentDlg* pRoomParentDlg = FindRoomParentDlg( pInfo->unRoomID ) ;
	if ( pRoomParentDlg != NULL )
	{
		RoomBusiness* pRoomBusiness = pRoomParentDlg->GetRoomBusiness() ;
		ASSERT( pRoomBusiness != NULL ) ;
		m_RoomParentEvent.ProcessEvent( pRoomParentDlg , evt ) ;
		pRoomBusiness->ProcessKickUserRsp(evt);		
	}
}

void RoomModule::ProcessKickUserNotify(Event const& evt ) 
{
	ASSERT(evt.m_pstExtraInfo != NULL);
	core::KickUserNotifyEvent * pInfo = (core::KickUserNotifyEvent*)evt.m_pstExtraInfo;
	CRoomParentDlg* pRoomParentDlg = FindRoomParentDlg( pInfo->unRoomID ) ;
	if (pRoomParentDlg != NULL)
	{
		RoomBusiness * pRoomBusiness = pRoomParentDlg->GetRoomBusiness();
		ASSERT(pRoomBusiness != NULL);
		m_RoomParentEvent.ProcessEvent( pRoomParentDlg , evt ) ;
		pRoomBusiness->ProcessKickUserNotify(evt);		
	}
}

void RoomModule::ProcessAddBlackRsp(Event const& evt ) 
{
	CRoomParentDlg* pRoomParentDlg = FindRoomParentDlg( evt.param0 ) ;
	if ( pRoomParentDlg != NULL )
	{
		RoomBusiness* pRoomBusiness = pRoomParentDlg->GetRoomBusiness() ;
		ASSERT( pRoomBusiness != NULL ) ;
		pRoomBusiness->ProcessAddBlackRsp(evt);
		m_RoomParentEvent.ProcessEvent( pRoomParentDlg , evt ) ;
	}
}

void RoomModule::ProcessAddBlackNotify(Event const& evt ) 
{
	ASSERT(evt.param0 != 0);
	CRoomParentDlg* pRoomParentDlg = FindRoomParentDlg( evt.param0 ) ;
	if ( pRoomParentDlg != NULL )
	{
		RoomBusiness * pRoomBusiness = pRoomParentDlg->GetRoomBusiness();
		ASSERT(pRoomBusiness != NULL);
		ASSERT(evt.param2 != 0);
		pRoomBusiness->ProcessAddBlackNotify(evt);
		m_RoomParentEvent.ProcessEvent( pRoomParentDlg , evt ) ;
	}

}

void RoomModule::ProcessLockIpRsp(Event const& evt ) 
{
	CRoomParentDlg* pRoomParentDlg = FindRoomParentDlg( evt.param0 ) ;
	if ( pRoomParentDlg != NULL )
	{
		RoomBusiness* pRoomBusiness = pRoomParentDlg->GetRoomBusiness() ;
		ASSERT( pRoomBusiness != NULL ) ;
		pRoomBusiness->ProcessLockIpRsp(evt);
		m_RoomParentEvent.ProcessEvent( pRoomParentDlg , evt ) ;
	}

}

void RoomModule::ProcessLockIpNotify(Event const& evt ) 
{
	ASSERT(evt.m_pstExtraInfo != NULL);
	core::LockIPNotifyEvent * pInfo = (core::LockIPNotifyEvent*)evt.m_pstExtraInfo;
	CRoomParentDlg* pRoomParentDlg = FindRoomParentDlg( pInfo->unRoomID ) ;
	if ( pRoomParentDlg != NULL )
	{
		RoomBusiness * pRoomBusiness = pRoomParentDlg->GetRoomBusiness();
		ASSERT(pRoomBusiness != NULL);
		ASSERT(pInfo->unDstUIN != 0);
		pRoomBusiness->ProcessLockIpNotify(evt);
		m_RoomParentEvent.ProcessEvent( pRoomParentDlg , evt ) ;
	}
}

void RoomModule::ProcessShutUpRsp(Event const& evt ) 
{
	CRoomParentDlg* pRoomParentDlg = FindRoomParentDlg( evt.param0 ) ;
	if ( pRoomParentDlg != NULL )
	{
		RoomBusiness* pRoomBusiness = pRoomParentDlg->GetRoomBusiness() ;
		ASSERT( pRoomBusiness != NULL ) ;
		pRoomBusiness->ProcessShutUpRsp(evt);
		m_RoomParentEvent.ProcessEvent( pRoomParentDlg , evt ) ;
	}
}

void RoomModule::ProcessShutUpNotify(Event const& evt ) 
{
	//禁言
	ASSERT(evt.m_pstExtraInfo != NULL);
	core::ShutUpNotifyEvent* pInfo = (core::ShutUpNotifyEvent*)evt.m_pstExtraInfo ;
	CRoomParentDlg* pRoomParentDlg = FindRoomParentDlg( pInfo->unRoomID ) ;
	if ( pRoomParentDlg != NULL )
	{
		RoomBusiness * pRoomBusiness = pRoomParentDlg->GetRoomBusiness();
		ASSERT(pRoomBusiness != NULL);
		pRoomBusiness->ProcessShutUpNotify(evt);
		m_RoomParentEvent.ProcessEvent( pRoomParentDlg , evt ) ;
	}
}

void RoomModule::ProcessUnLockShutUpRsp(Event const& evt )
{
	CRoomParentDlg* pRoomParentDlg = FindRoomParentDlg( evt.param0 ) ;
	if ( pRoomParentDlg != NULL )
	{
		RoomBusiness* pRoomBusiness = pRoomParentDlg->GetRoomBusiness() ;
		ASSERT( pRoomBusiness != NULL ) ;
		pRoomBusiness->ProcessUnLockShutUpRsp(evt);
		m_RoomParentEvent.ProcessEvent( pRoomParentDlg , evt ) ;
	}
}

void RoomModule::ProcessUnLockShutUpNotify(Event const&evt ) 
{
	//解除禁言
	ASSERT(evt.param0 != NULL);
	CRoomParentDlg* pRoomParentDlg = FindRoomParentDlg( evt.param0 ) ;
	if ( pRoomParentDlg != NULL )
	{
		RoomBusiness * pRoomBusiness = pRoomParentDlg->GetRoomBusiness();
		ASSERT(pRoomBusiness != NULL);
		ASSERT(evt.param2 != 0);
		pRoomBusiness->ProcessUnLockShutUpNotify(evt);
		m_RoomParentEvent.ProcessEvent( pRoomParentDlg , evt ) ;
	}
}

//添加管理员
void RoomModule::ProcessAddTitleRsp(Event const& evt ) 
{
	ASSERT( evt.m_pstExtraInfo != NULL ) ;
	core::AddTitleRspEvent* pInfo = (core::AddTitleRspEvent*)evt.m_pstExtraInfo ;
	CRoomParentDlg* pRoomParentDlg = FindRoomParentDlg( pInfo->unRoomID ) ;
	if ( pRoomParentDlg != NULL )
	{
		RoomBusiness* pRoomBusiness = pRoomParentDlg->GetRoomBusiness() ;
		ASSERT( pRoomBusiness != NULL ) ;
		pRoomBusiness->ProcessAddTitleRsp(evt);
		m_RoomParentEvent.ProcessEvent( pRoomParentDlg , evt ) ;
	}
}

//取消管理员
void RoomModule::ProcessRemoveTitleRsp( Event const& evt ) 
{
	ASSERT( evt.m_pstExtraInfo != NULL ) ;
	core::RemoveTitleRspEvent* pInfo = (core::RemoveTitleRspEvent*)evt.m_pstExtraInfo ;
	CRoomParentDlg* pRoomParentDlg = FindRoomParentDlg( pInfo->unRoomID ) ;
	if ( pRoomParentDlg != NULL )
	{
		RoomBusiness* pRoomBusiness = pRoomParentDlg->GetRoomBusiness() ;
		ASSERT( pRoomBusiness != NULL ) ;
		pRoomBusiness->ProcessRemoveTitleRsp(evt);
		m_RoomParentEvent.ProcessEvent( pRoomParentDlg , evt ) ;
	}
}

void RoomModule::ProcessAddTitleNotify( Event const& evt ) 
{
	ASSERT( evt.m_pstExtraInfo != NULL ) ;
	core::AddTitleNotifyEvent* pInfo = (core::AddTitleNotifyEvent*)evt.m_pstExtraInfo ;
	CRoomParentDlg* pRoomParentDlg = FindRoomParentDlg( pInfo->unRoomID ) ;
	if ( pRoomParentDlg != NULL )
	{
		RoomBusiness* pRoomBusiness = pRoomParentDlg->GetRoomBusiness() ;
		ASSERT( pRoomBusiness != NULL ) ;
		pRoomBusiness->ProcessAddTitleNotify(evt);
		m_RoomParentEvent.ProcessEvent( pRoomParentDlg , evt ) ;
	}
}

void RoomModule::ProcessRemoveTitleNotify( Event const& evt ) 
{
	ASSERT( evt.m_pstExtraInfo != NULL ) ;
	core::RemoveTitleNotifyEvent* pInfo = (core::RemoveTitleNotifyEvent*)evt.m_pstExtraInfo ;
	CRoomParentDlg* pRoomParentDlg = FindRoomParentDlg( pInfo->unRoomID ) ;
	if ( pRoomParentDlg != NULL )
	{
		RoomBusiness* pRoomBusiness = pRoomParentDlg->GetRoomBusiness() ;
		ASSERT( pRoomBusiness != NULL ) ;
		pRoomBusiness->ProcessRemoveTitleNotify(evt);
		m_RoomParentEvent.ProcessEvent( pRoomParentDlg , evt ) ;
	}
}

//礼物到期
void RoomModule::ProcessNotifyGiftOverTime(Event const& evt ) 
{
	CRoomParentDlg* pRoomParentDlg = FindRoomParentDlg( evt.param0 ) ;
	if ( pRoomParentDlg != NULL )
	{
		RoomBusiness* pRoomBusiness = pRoomParentDlg->GetRoomBusiness() ;
		ASSERT( pRoomBusiness != NULL ) ;
		pRoomBusiness->ProcessNotifyGiftOverTime(evt);
		m_RoomParentEvent.ProcessEvent( pRoomParentDlg , evt ) ;
	}
}

//财神到
void RoomModule::ProcessNotifyPlutusAward(Event const& evt ) 
{
	ASSERT( evt.m_pstExtraInfo != NULL ) ;
	core::CurInfo myCurInfo = GetCurInfo() ;
	if ( myCurInfo.enmVIPLevel == VIPLEVEL_NONE )
	{
		return ;
	}
	core::NotifyPlutusAwardEvent* pInfo = (core::NotifyPlutusAwardEvent*)evt.m_pstExtraInfo ;
	CRoomParentDlg* pRoomParentDlg = FindRoomParentDlg( pInfo->unRoomID ) ;
	if ( pRoomParentDlg != NULL )
	{
		RoomBusiness * pRoomBusiness = pRoomParentDlg->GetRoomBusiness();
		ASSERT(pRoomBusiness != NULL);
		pRoomBusiness->ProcessNotifyPlutusAward(evt);
		m_RoomParentEvent.ProcessEvent( pRoomParentDlg , evt ) ;
	}
}

//钱币更新
void RoomModule::ProcessNotifyMoneyUpdate(Event const& evt ) 
{
	UpdateAllRoomMoney( evt.param0 ) ;
}

//官方公告
void RoomModule::ProcessNotifyOfficeNotice(Event const& evt ) 
{
	ASSERT( evt.m_pstExtraInfo != NULL ) ;
	core::NotifyOfficialNoticeEvent* pInfo = (core::NotifyOfficialNoticeEvent*)evt.m_pstExtraInfo ;
	if ( pInfo->unRoomID == 0 )
	{
		for ( MapRoomParentDlg::iterator it = m_mapRoomParentDlg.begin() ; it != m_mapRoomParentDlg.end() ; ++it )
		{
			CRoomParentDlg* pRoomParentDlg = it->second ;
			if ( pRoomParentDlg != NULL )
			{
				m_RoomParentEvent.ProcessEvent( pRoomParentDlg , evt ) ;
			}
		}
	}
	else
	{
		CRoomParentDlg* pRoomParentDlg = FindRoomParentDlg( pInfo->unRoomID ) ;
		if ( pRoomParentDlg != NULL )
		{
			m_RoomParentEvent.ProcessEvent( pRoomParentDlg , evt ) ;
		}
	}

}


void RoomModule::OnRoomDlgClose( uint32 roomid )
{
	std::vector<uint32>::iterator  iter ;
	for ( iter = m_vRoomIDList.begin() ; iter != m_vRoomIDList.end() ; ++iter )
	{
		if ( *iter == roomid )
		{
			m_vRoomIDList.erase( iter ) ;
			break;
		}
	}	
	if ( m_mapRoomParentDlg.find( roomid ) != m_mapRoomParentDlg.end() )
	{		
		m_mapRoomParentDlg.erase(roomid) ;
	}

	if (m_mapRoomParentDlg.size() == 0)
	{
		m_pModuleManager->PushEvent(MakeEvent<MODULE_ID_ROOM>()(Hall::EVENT_VALUE_HALL_ACTIVATE_SHOW, MODULE_ID_HALL, 0, 0, 0, NULL));
	}
}

void RoomModule::KickLeaveRoom( uint32 roomid ) 
{
	CloseARoom( roomid ) ;
}

//财神降临时，财神送钱时金币的变更或自己中奖时钱币的变更
void RoomModule::UpdatePlutusAwardOrLuckMoney( uint32 unMoney ) 
{
	core::CurInfo myCurInfo = GetCurInfo() ; 
	if ( myCurInfo.enmVIPLevel != VIPLEVEL_NONE )
	{
		m_pModuleManager->CallService( core::SERVICE_VALUE_CORE_UPDATE_MONEY ,(param)unMoney ) ;
		for (MapRoomParentDlg::iterator it = m_mapRoomParentDlg.begin(); it != m_mapRoomParentDlg.end() ; ++it )
		{
			CRoomParentDlg* pRoomParentDlg = it->second ;
			if ( pRoomParentDlg != NULL )
			{
				pRoomParentDlg->OnUpdateMoney( unMoney + myCurInfo.unLeftMoney ) ;
			}
		}
				
	}
	CString strTemp ;
	strTemp.Format( L"钱UpdatePlutusAwardOrLuckMoney unMoney=%u , myCurInfo.unLeftMoney=%ld\r\n" , unMoney , myCurInfo.unLeftMoney ) ;
	OutputDebugString( strTemp ) ;
}

void RoomModule::UpdateAllRoomMoney( uint32 unMoney )
{
	core::CurInfo myCurInfo = GetCurInfo() ;
	uint32 nLeftMoney = 0 ;
	MapRoomParentDlg::iterator it ;
	for ( it = m_mapRoomParentDlg.begin(); it != m_mapRoomParentDlg.end() ; ++it )
	{
		CRoomParentDlg* pRoomParentDlg = it->second ;
		if ( pRoomParentDlg != NULL )
		{
			nLeftMoney += pRoomParentDlg->GetPlutusAwardLeftMoney( ) ;
			nLeftMoney += pRoomParentDlg->GetLuckLeftMoney() ;
		}
	}
	int lChangeMoney = unMoney - myCurInfo.unLeftMoney - nLeftMoney ;
	m_pModuleManager->CallService( core::SERVICE_VALUE_CORE_UPDATE_MONEY ,(param)lChangeMoney ) ;
	for ( it = m_mapRoomParentDlg.begin(); it != m_mapRoomParentDlg.end() ; ++it )
	{
		CRoomParentDlg* pRoomParentDlg = it->second ;
		if ( pRoomParentDlg != NULL )
		{
			pRoomParentDlg->OnUpdateMoney( unMoney - nLeftMoney ) ;
		}
	}
	CString strTemp ;
	strTemp.Format( L"UpdateAllRoomMoney=%u ,nLeftMoney=%d,OnUpdateMoney=%ld\r\n" , unMoney ,nLeftMoney ,unMoney - nLeftMoney ) ;
	OutputDebugString(strTemp) ;
}

//离开房间时，修正因财神引起的金币变更
void RoomModule::ReviseAllRoomMoney( uint32 lMoney ) 
{
	for (MapRoomParentDlg::iterator it = m_mapRoomParentDlg.begin(); it != m_mapRoomParentDlg.end() ; ++it )
	{
		CRoomParentDlg* pRoomParentDlg = it->second ;
		if ( pRoomParentDlg != NULL )
		{
			pRoomParentDlg->OnUpdateMoney( lMoney ) ;
		}
	}
}

void RoomModule::UpdateAllRoomMic( uint32 unMicVolume ) 
{
	for (MapRoomParentDlg::iterator it = m_mapRoomParentDlg.begin(); it != m_mapRoomParentDlg.end() ; ++it )
	{
		CRoomParentDlg* pRoomParentDlg = it->second ;
		if ( pRoomParentDlg != NULL )
		{
			pRoomParentDlg->SetRoomMicVolume( unMicVolume ) ;
		}
	}
}

void RoomModule::UpdateAllRoomStatus(BOOL bOnLine)
{
	for (MapRoomParentDlg::iterator it = m_mapRoomParentDlg.begin(); it != m_mapRoomParentDlg.end() ; ++it )
	{
		CRoomParentDlg* pRoomParentDlg = it->second;
		if ( pRoomParentDlg != NULL )
		{
			pRoomParentDlg->SetMyStatus(bOnLine);
		}
	}
}

//关闭话筒
void RoomModule::CloseMic() 
{
	for (MapRoomParentDlg::iterator it = m_mapRoomParentDlg.begin(); it != m_mapRoomParentDlg.end(); ++it)
	{
		CRoomParentDlg * pRoomParentDlg = it->second;
		if (pRoomParentDlg != NULL)
		{
			pRoomParentDlg->CloseMic() ;
		}
	}
}

//开启话筒
void RoomModule::OpenMic() 
{
	for (MapRoomParentDlg::iterator it = m_mapRoomParentDlg.begin(); it != m_mapRoomParentDlg.end(); ++it)
	{
		CRoomParentDlg * pRoomParentDlg = it->second;
		if (pRoomParentDlg != NULL)
		{
			pRoomParentDlg->OpenMic() ;
		}
	}
}

void RoomModule::SetAllRoomVideoCfg( int iDevice ) 
{
	for (MapRoomParentDlg::iterator it = m_mapRoomParentDlg.begin(); it != m_mapRoomParentDlg.end(); ++it)
	{
		CRoomParentDlg * pRoomParentDlg = it->second;
		if (pRoomParentDlg != NULL)
		{
			pRoomParentDlg->SetAllVideoCfg( iDevice ) ;
		}
	}
}

//印章变更
void RoomModule::UpdateAllRoomUserStampChange( uint32 unDstUIN, uint16 unStampID ) 
{
	for (MapRoomParentDlg::iterator it = m_mapRoomParentDlg.begin(); it != m_mapRoomParentDlg.end(); ++it)
	{
		CRoomParentDlg * pRoomParentDlg = it->second;
		if (pRoomParentDlg != NULL)
		{
			RoomBusiness* pRoomBusiness = pRoomParentDlg->GetRoomBusiness() ;
			if ( pRoomBusiness != NULL )
			{
				pRoomBusiness->UserStampChange( unDstUIN , unStampID ) ;
			}
		}
	}
}

void RoomModule::ProcessHallOpenSound( Message const& msg )
{
	m_pRoomAvLogic->SetAllSoundMute( !(bool)msg.param0);
}

void RoomModule::ProcessHallHeadImageUpdate(Message const& msg ) 
{
	for (MapRoomParentDlg::iterator it = m_mapRoomParentDlg.begin(); it != m_mapRoomParentDlg.end(); ++it)
	{
		CRoomParentDlg * pRoomParentDlg = it->second;
		if (pRoomParentDlg != NULL)
		{
			pRoomParentDlg->OnHallHeadImageUpdate(msg.param0);
		}
	}

	if(m_pRoomSysSettingDlg != NULL && m_pRoomSysSettingDlg->IsWindowVisible())
	{		
		m_pRoomSysSettingDlg->OnGetHeadImage(msg.param0);		
	}
	
}

void RoomModule::ProcessSetSelfInfoRsp( Event const& evt )
{
	core::ENM_SetSelfInfoResult enmResult = (core::ENM_SetSelfInfoResult)evt.param0;
	if (SETSELFINFO_OK == enmResult)
	{
		CPersonalConfig::GetInstance()->OnSetSelfInfoOK();
	}
}

//粉丝排行
void RoomModule::ProcessGetFansRsp(Event const&evt)
{
	for (MapRoomParentDlg::iterator it = m_mapRoomParentDlg.begin(); it != m_mapRoomParentDlg.end(); ++it)
	{
		CRoomParentDlg * pRoomParentDlg = it->second;
		if (pRoomParentDlg != NULL)
		{
			pRoomParentDlg->OnGetFansRsp(evt);
		}
	}

	if(m_pRoomSysSettingDlg != NULL && m_pRoomSysSettingDlg->IsWindowVisible())
	{
		m_pRoomSysSettingDlg->OnGetFansRsp(evt);
	}
	
}

//任务系统更新
//DragonZ:V3.0去除房间右下角的任务
//void RoomModule::ProcessTaskInfoNotify(Event const& evt)
//{
//	for (MapRoomParentDlg::iterator it = m_mapRoomParentDlg.begin(); it != m_mapRoomParentDlg.end(); ++it)
//	{
//		CRoomParentDlg * pRoomParentDlg = it->second;
//		if (pRoomParentDlg != NULL)
//		{
//			pRoomParentDlg->UpdateTaskInfo();			
//		}
//	}
//}

void RoomModule::OnCycleTrigger( Message const& msg )
{
	for (MapRoomParentDlg::iterator it = m_mapRoomParentDlg.begin(); it != m_mapRoomParentDlg.end(); ++it)
	{
		CRoomParentDlg * pRoomParentDlg = it->second;
		if (pRoomParentDlg != NULL)
		{
			pRoomParentDlg->OnCycleTrigger();
		}
	}

	//ScreenEffectManager::GetInstance()->OnCycleTrigger();

#if 0
	static int nCount = 0;
	static std::vector<int> vecUin;
	nCount++;
	if (nCount % 10 != 0)
	{
		return;
	}

	if (rand() % 2 == 0)
	{
		core::EnterRoomNotifyEvent* pEvent = new core::EnterRoomNotifyEvent();
		pEvent->unRoomID = 1111;
		pEvent->stUserInfo.strName = L"test";
		pEvent->stUserInfo.un179id = nCount;
		pEvent->stUserInfo.unUIN = nCount;
		pEvent->stUserInfo.unVipLevel = 0x80>>(rand()%9) ;
		//pEvent->stUserInfo.unVipLevel = 0x00;
		//pEvent->stUserInfo.unTitleLevel = 0x10>>(rand()%6) ;
		pEvent->stUserInfo.unTitleLevel = 0x00;
		vecUin.push_back(nCount);

		m_pModuleManager->PushEvent( MakeEvent<MODULE_ID_ROOM>()(core::EVENT_VALUE_CORE_ENTER_ROOM_NOTIFY, MODULE_ID_ROOM, 0, 0, 0, pEvent) );
	}
	else
	{
		if (vecUin.size() > 0)
		{
			for(std::vector<int>::iterator it = vecUin.begin(); it != vecUin.end();)
			{
				int unUIN = *it;
				m_pModuleManager->PushEvent(MakeEvent<MODULE_ID_ROOM>()(EVENT_VALUE_CORE_LEAVE_ROOM_NOTIFY, MODULE_ID_ROOM, unUIN, 1111, 0, NULL));
				vecUin.erase(it);
				break;
			}
		}
	}

#endif
}

void RoomModule::ProcessSendFlyWordsRsp( Event const& evt )
{
	ASSERT(evt.m_pstExtraInfo != NULL ) ;
	core::SendFlyWordsRspEvent* pInfo = (core::SendFlyWordsRspEvent*)evt.m_pstExtraInfo ;
	CRoomParentDlg* pRoomParentDlg = FindRoomParentDlg( pInfo->unRoomID ) ;
	if ( pRoomParentDlg != NULL )
	{
		RoomBusiness* pRoomBusiness = pRoomParentDlg->GetRoomBusiness() ;
		ASSERT( pRoomBusiness != NULL ) ;
		//pRoomBusiness->ProcessSendFlyWordsRsp(evt);
		m_RoomParentEvent.ProcessEvent( pRoomParentDlg , evt ) ;
	}
}

void RoomModule::ProcessSendFlyWordsNotify( Event const& evt )
{
	ASSERT(evt.m_pstExtraInfo != NULL ) ;
	core::SendFlyWordsNotifyEvent* pInfo = (core::SendFlyWordsNotifyEvent*)evt.m_pstExtraInfo ;
	CRoomParentDlg* pRoomParentDlg = FindRoomParentDlg( pInfo->unRoomID ) ;
	if ( pRoomParentDlg != NULL )
	{
		RoomBusiness* pRoomBusiness = pRoomParentDlg->GetRoomBusiness() ;
		ASSERT( pRoomBusiness != NULL ) ;
		//pRoomBusiness->ProcessSendFlyWordsNofity(evt);
		m_RoomParentEvent.ProcessEvent( pRoomParentDlg , evt ) ;
	}
}

//财富宝座
void RoomModule::ProcessNotifyRichStar(Event const& evt ) 
{
	ASSERT(evt.m_pstExtraInfo != NULL ) ;
	core::NotifyRichStarEvent* pInfo = (core::NotifyRichStarEvent*)evt.m_pstExtraInfo ;
	CRoomParentDlg* pRoomParentDlg = FindRoomParentDlg( pInfo->unRoomID ) ;
	if ( pRoomParentDlg != NULL )
	{
		RoomBusiness* pRoomBusiness = pRoomParentDlg->GetRoomBusiness() ;
		ASSERT( pRoomBusiness != NULL ) ;
		pRoomBusiness->ProcessNotifyRichStar(evt) ;
		m_RoomParentEvent.ProcessEvent( pRoomParentDlg , evt ) ;
	}
}
//V2.21版本后去掉音频模式
//void RoomModule::UpdateAllRoomAudioMode()
//{
//	for (MapRoomParentDlg::iterator it = m_mapRoomParentDlg.begin(); it != m_mapRoomParentDlg.end() ; ++it )
//	{
//		CRoomParentDlg* pRoomParentDlg = it->second ;
//		if ( pRoomParentDlg != NULL )
//		{
//			pRoomParentDlg->UpdateAudioMode() ;
//		}
//	}
//}


void RoomModule::AddRecentBrowseRoom( uint32 roomId )
{
	CString strUrl,strTemp;
	Hall::WebInterfaceUrl weburl;
	weburl.m_WebWithPara = true;
	weburl.m_WebInterfaceKeyName = SYSSET_INI_KEYNAME_BROWSEROOM;
	g_RoomModule->m_pModuleManager->CallService(Hall::SERVICE_VALUE_HALL_GET_WEBINTERFACEURL, (param)&weburl);
	strUrl = weburl.m_WebInterfaceUrl.c_str() ;
	strTemp.Format( L"&roomid=%d" ,roomId ) ;
	strUrl += strTemp ;
	webinterface::get_http_url_event_req getHttpEvent;
	getHttpEvent.srcMId	=	MODULE_ID_ROOM;
	getHttpEvent.desMId	=	MODULE_ID_WEBSERVICE;
	getHttpEvent.wstrUrl = strUrl;

	g_RoomModule->m_pModuleManager->CallService( getHttpEvent.eventValue , (param)&getHttpEvent ) ;
}

void RoomModule::ProcessMyInfoUpdate( Message const& msg )
{
	for (MapRoomParentDlg::iterator it = m_mapRoomParentDlg.begin(); it != m_mapRoomParentDlg.end() ; ++it )
	{
		CRoomParentDlg* pRoomParentDlg = it->second ;
		if ( pRoomParentDlg != NULL )
		{
			RoomBusiness* pRoomBusiness = pRoomParentDlg->GetRoomBusiness() ;
			pRoomBusiness->ProcessMyInfoUpdate();
			pRoomParentDlg->AddBottomUserInfo();
			
		}
	}
}

void RoomModule::CloseAllRoom()
{
	Message msg;
	ProcessCleanRoom(msg);
}

void RoomModule::CreateNetStateTimer(uint32 interval)
{
	if(m_NetStateTimer == -1)
	{
		m_NetStateTimer = common::utility::_Timers::instance().CreateTimer(interval, 
			common::utility::_Timers::TimerCallback(
			boost::bind(&RoomModule::OnNetStateTimeOut,this,boost::arg<1>(),boost::arg<2>())), 
			common::utility::_Timers::ENM_TIMER_ROOM_NETSTATE);
	}
}

void RoomModule::ClearNetStateTimer()
{
	if(m_NetStateTimer != -1)
	{
		common::utility::_Timers::instance().DestroyTimer(m_NetStateTimer);
		m_NetStateTimer = -1;
	}
}

void RoomModule::OnNetStateTimeOut(uintptr,bool)
{
	MapRoomParentDlg::iterator it ;
	for ( it = m_mapRoomParentDlg.begin(); it != m_mapRoomParentDlg.end() ; ++it )
	{
		CRoomParentDlg* pRoomParentDlg = it->second ;
		if ( pRoomParentDlg != NULL )
		{
			RoomAVLogic* pRoomAVLogic = pRoomParentDlg->GetRoomAvLogic();
			if(pRoomAVLogic != NULL)
			{
				uint32 iRelayIP;
				Service_Av_Net_Status_All avNetSataus;
				pRoomAVLogic->GetNetState(iRelayIP,avNetSataus);				
				pRoomParentDlg->GetStatReportInstance().OnGetNetState(iRelayIP,avNetSataus);  //发送音视频丢包率数据至服务器
			}
		}
	}
}

void RoomModule::CreatePerformanceTimer(uint32 interval)
{
	if(m_PerformanceTimer == -1)
	{
		m_PerformanceTimer = common::utility::_Timers::instance().CreateTimer(interval, 
			common::utility::_Timers::TimerCallback(
			boost::bind(&RoomModule::OnPerformanceTimerOut,this,boost::arg<1>(),boost::arg<2>())), 
			common::utility::_Timers::ENM_TIMER_ROOM_CPU_PERFORMANCE);
	}
}

void RoomModule::ClearPerformanceTimer()
{
	if(m_PerformanceTimer != -1)
	{
		common::utility::_Timers::instance().DestroyTimer(m_PerformanceTimer);
		m_PerformanceTimer = -1;
	}
}

void RoomModule::OnPerformanceTimerOut(uintptr,bool)
{
	float cpu = common::utility::systemhelper::get_cpu_usage();
	m_bSpeedUpMode = cpu > CPU_PERFORMANCE_TEST_POINT;

	if(cpu > CPU_PERFORMANCE_TEST_POINT2)
	{
		m_pRoomAvLogic->SetEnableShapen(false);
	}
}

void RoomModule::ProcessMyVipLevelUpdate( Message const& msg )
{
	for (MapRoomParentDlg::iterator it = m_mapRoomParentDlg.begin(); it != m_mapRoomParentDlg.end() ; ++it )
	{
		CRoomParentDlg* pRoomParentDlg = it->second ;
		if ( pRoomParentDlg != NULL )
		{
			RoomBusiness* pRoomBusiness = pRoomParentDlg->GetRoomBusiness() ;
			pRoomBusiness->ProcessMyVipLevelUpdate();
			pRoomParentDlg->AddBottomUserInfo();

			CRoomTopManager *pRoomTopManager = pRoomParentDlg->GetRoomTopManager();
			if(pRoomTopManager != NULL)
			{
				pRoomTopManager->ProcessMyVipLevelUpdate();
			}
		}
	}
}

void RoomModule::ProcessMyRichLevelUpdate( Message const& msg )
{
	for (MapRoomParentDlg::iterator it = m_mapRoomParentDlg.begin(); it != m_mapRoomParentDlg.end() ; ++it )
	{
		CRoomParentDlg* pRoomParentDlg = it->second ;
		if ( pRoomParentDlg != NULL )
		{
			RoomBusiness* pRoomBusiness = pRoomParentDlg->GetRoomBusiness() ;
			pRoomBusiness->ProcessMyRichLevelUpdate();
			pRoomParentDlg->AddBottomUserInfo();

			CRoomTopManager *pRoomTopManager = pRoomParentDlg->GetRoomTopManager();
			if(pRoomTopManager != NULL)
			{
				pRoomTopManager->ProcessMyRichLevelUpdate();
			}
		}
	}
}

void RoomModule::ProcessAnswerPushOnShowRsp( Event const& evt )
{
	ASSERT(evt.m_pstExtraInfo != NULL);
	core::PushOnShowAnswerRspEvent* pInfo = (core::PushOnShowAnswerRspEvent*)evt.m_pstExtraInfo ;
	CRoomParentDlg* pRoomParentDlg = FindRoomParentDlg( pInfo->unRoomID ) ;
	if ( pRoomParentDlg != NULL )
	{
		RoomBusiness *pRoomBusiness = pRoomParentDlg->GetRoomBusiness() ;
		ASSERT( pRoomBusiness!= NULL ) ;		 
		pRoomBusiness->ProcessPushOnShowNotify(evt);
		m_RoomParentEvent.ProcessEvent( pRoomParentDlg , evt ) ;
	}
}

int32 RoomModule::GetRichLevelInfo( param st )
{
	Room_Get_Rich_Level_Info& richLevelInfo = *(Room_Get_Rich_Level_Info*)st;
	if (richLevelInfo.unRichLevel >= 0)
	{
		richLevelInfo.strLevelName = RichLevelManager::GetRichLevelNameByLevel(richLevelInfo.unRichLevel);
		richLevelInfo.strImgPath = RichLevelManager::GetRichLevelImgPathByLevel(richLevelInfo.unRichLevel);
	}
	return 0;
}

void RoomModule::ProcessGetSofaListRsp( Event const& evt )
{
	ASSERT(evt.m_pstExtraInfo != NULL);
	core::GetSofaListRspEvent * pInfo = (core::GetSofaListRspEvent*)evt.m_pstExtraInfo;
	CRoomParentDlg* pRoomParentDlg = FindRoomParentDlg(pInfo->roomID);
	if (pRoomParentDlg != NULL)
	{
		RoomBusiness* pRoomBusiness = pRoomParentDlg->GetRoomBusiness();
		ASSERT(pRoomBusiness != NULL);
		pRoomBusiness->ProcessGetSofaListRsp(evt);
		m_RoomParentEvent.ProcessEvent(pRoomParentDlg, evt);
	}
}

void RoomModule::ProcessBuySofaRsp( Event const& evt )
{
	ASSERT(evt.m_pstExtraInfo != NULL);
	core::BuySofaRspEvent * pInfo = (core::BuySofaRspEvent*)evt.m_pstExtraInfo;
	CRoomParentDlg* pRoomParentDlg = FindRoomParentDlg(pInfo->roomid);
	if (pRoomParentDlg != NULL)
	{
		RoomBusiness* pRoomBusiness = pRoomParentDlg->GetRoomBusiness();
		ASSERT(pRoomBusiness != NULL);
		pRoomBusiness->ProcessBuySofaRsp(evt);
		m_RoomParentEvent.ProcessEvent(pRoomParentDlg, evt);
	}
}

void RoomModule::ProcessBuySofaNotify( Event const& evt )
{
	ASSERT(evt.m_pstExtraInfo != NULL);
	core::BuySofaNotifyEvent * pInfo = (core::BuySofaNotifyEvent*)evt.m_pstExtraInfo;
	CRoomParentDlg* pRoomParentDlg = FindRoomParentDlg(pInfo->roomid);
	if (pRoomParentDlg != NULL)
	{
		RoomBusiness* pRoomBusiness = pRoomParentDlg->GetRoomBusiness();
		ASSERT(pRoomBusiness != NULL);
		pRoomBusiness->ProcessBuySofaNotify(evt);
		m_RoomParentEvent.ProcessEvent(pRoomParentDlg, evt);
	}
}

void RoomModule::ProcessHallResUpdate( Message const& msg )
{
	GiftManager::GetInstance()->ReloadGiftList();

	for (MapRoomParentDlg::iterator it = m_mapRoomParentDlg.begin(); it != m_mapRoomParentDlg.end() ; ++it )
	{
		CRoomParentDlg* pRoomParentDlg = it->second ;
		if ( pRoomParentDlg != NULL )
		{
			pRoomParentDlg->ReloadGiftData();
		}
	}
}

void RoomModule::ProcessNotifyClearSofaList( Event const& evt )
{
	ASSERT(evt.param0 != 0);
	CRoomParentDlg * pRoomParentDlg = FindRoomParentDlg(evt.param0);
	if (pRoomParentDlg != NULL)
	{
		RoomBusiness * pRoomBusiness = pRoomParentDlg->GetRoomBusiness();
		ASSERT(pRoomBusiness != NULL);
		pRoomBusiness->ProcessNotifyClearSofaList(evt);
		m_RoomParentEvent.ProcessEvent(pRoomParentDlg, evt);
	}
}

void RoomModule::ProcessUpdateSongListRsp( Event const& evt )
{
	ASSERT(evt.m_pstExtraInfo != NULL);
	core::UpdateSongListRspEvent * pInfo = (core::UpdateSongListRspEvent*)evt.m_pstExtraInfo;
	CRoomParentDlg* pRoomParentDlg = FindRoomParentDlg(pInfo->roomid);
	if (pRoomParentDlg != NULL)
	{
		RoomBusiness* pRoomBusiness = pRoomParentDlg->GetRoomBusiness();
		ASSERT(pRoomBusiness != NULL);
		pRoomBusiness->ProcessUpdateSongListRsp(evt);
		m_RoomParentEvent.ProcessEvent(pRoomParentDlg, evt);
	}
}

void RoomModule::ProcessGetSongListRsp( Event const& evt )
{
	ASSERT(evt.m_pstExtraInfo != NULL);
	core::GetSongListRspEvent * pInfo = (core::GetSongListRspEvent*)evt.m_pstExtraInfo;
	CRoomParentDlg * pRoomParentDlg = FindRoomParentDlg(pInfo->roomid);
	if (pRoomParentDlg != NULL)
	{
		RoomBusiness* pRoomBusiness = pRoomParentDlg->GetRoomBusiness();
		ASSERT(pRoomBusiness != NULL);
		pRoomBusiness->ProcessGetSongListRsp(evt);
		m_RoomParentEvent.ProcessEvent(pRoomParentDlg, evt);
	}
}

void RoomModule::ProcessOrderSongRsp( Event const& evt )
{
	ASSERT(evt.param0 != 0);
	CRoomParentDlg * pRoomParentDlg = FindRoomParentDlg(evt.param0);
	if (pRoomParentDlg != NULL)
	{
		RoomBusiness* pRoomBusiness = pRoomParentDlg->GetRoomBusiness();
		ASSERT(pRoomBusiness != NULL);
		pRoomBusiness->ProcessOrderSongRsp(evt);
		m_RoomParentEvent.ProcessEvent(pRoomParentDlg, evt);
	}
}

void RoomModule::ProcessOrderSongNotify( Event const& evt )
{
	ASSERT(evt.m_pstExtraInfo != NULL);
	core::OrderSongNotifyEvent * pInfo = (core::OrderSongNotifyEvent*)evt.m_pstExtraInfo;
	CRoomParentDlg * pRoomParentDlg = FindRoomParentDlg(pInfo->roomid);
	if (pRoomParentDlg != NULL)
	{
		RoomBusiness* pRoomBusiness = pRoomParentDlg->GetRoomBusiness();
		ASSERT(pRoomBusiness != NULL);
		pRoomBusiness->ProcessOrderSongNotify(evt);
		m_RoomParentEvent.ProcessEvent(pRoomParentDlg, evt);
	}
}

void RoomModule::ProcessTakeOrderRsp( Event const& evt )
{
	ASSERT(evt.param0 != 0);
	CRoomParentDlg * pRoomParentDlg = FindRoomParentDlg(evt.param0);
	if (pRoomParentDlg != NULL)
	{
		RoomBusiness* pRoomBusiness = pRoomParentDlg->GetRoomBusiness();
		ASSERT(pRoomBusiness != NULL);
		pRoomBusiness->ProcessTakeOrderRsp(evt);
		m_RoomParentEvent.ProcessEvent(pRoomParentDlg, evt);
	}
}

void RoomModule::ProcessTakeOrderNotify( Event const& evt )
{
	ASSERT(evt.m_pstExtraInfo != NULL);
	core::ProcessOrderNotifyEvent * pInfo = (core::ProcessOrderNotifyEvent*)evt.m_pstExtraInfo;
	CRoomParentDlg * pRoomParentDlg = FindRoomParentDlg(pInfo->roomid);
	if (pRoomParentDlg != NULL)
	{
		RoomBusiness* pRoomBusiness = pRoomParentDlg->GetRoomBusiness();
		ASSERT(pRoomBusiness != NULL);
		pRoomBusiness->ProcessTakeOrderNotify(evt);
		m_RoomParentEvent.ProcessEvent(pRoomParentDlg, evt);
	}
}

void RoomModule::ProcessGetOrderListRsp( Event const& evt )
{
	ASSERT(evt.m_pstExtraInfo != NULL);
	core::GetOrderListRspEvent * pInfo = (core::GetOrderListRspEvent*)evt.m_pstExtraInfo;
	CRoomParentDlg * pRoomParentDlg = FindRoomParentDlg(pInfo->roomid);
	if (pRoomParentDlg != NULL)
	{
		RoomBusiness* pRoomBusiness = pRoomParentDlg->GetRoomBusiness();
		ASSERT(pRoomBusiness != NULL);
		pRoomBusiness->ProcessGetOrderListRsp(evt);
		m_RoomParentEvent.ProcessEvent(pRoomParentDlg, evt);
	}
}

void RoomModule::ProcessMarkSongRsp( Event const& evt )
{
	ASSERT(evt.param0 != 0);
	CRoomParentDlg * pRoomParentDlg = FindRoomParentDlg(evt.param0);
	if (pRoomParentDlg != NULL)
	{
		RoomBusiness* pRoomBusiness = pRoomParentDlg->GetRoomBusiness();
		ASSERT(pRoomBusiness != NULL);
		pRoomBusiness->ProcessMarkSongRsp(evt);
		m_RoomParentEvent.ProcessEvent(pRoomParentDlg, evt);
	}
}

void RoomModule::ProcessMarkSongNotify( Event const& evt )
{
	ASSERT(evt.m_pstExtraInfo != NULL);
	core::MarkSongNotifyEvent * pInfo = (core::MarkSongNotifyEvent*)evt.m_pstExtraInfo;
	CRoomParentDlg * pRoomParentDlg = FindRoomParentDlg(pInfo->roomid);
	if (pRoomParentDlg != NULL)
	{
		RoomBusiness* pRoomBusiness = pRoomParentDlg->GetRoomBusiness();
		ASSERT(pRoomBusiness != NULL);
		pRoomBusiness->ProcessMarkSongNotify(evt);
		m_RoomParentEvent.ProcessEvent(pRoomParentDlg, evt);
	}
}

//离线主播列表返回响应函数
void RoomModule::ProcessInitOffRoomAcotrRsp(Event const& evt)
{
	ASSERT(evt.m_pstExtraInfo != NULL);
	core::GetOfflineActorRspEvent *pInfo = (core::GetOfflineActorRspEvent*)evt.m_pstExtraInfo;
	CRoomParentDlg * pRoomParentDlg = FindRoomParentDlg(pInfo->roomID);
	if (pRoomParentDlg != NULL)
	{
		CRoomTopManager * pRoomTopManager = pRoomParentDlg->GetRoomTopManager();
		ASSERT(pRoomTopManager != NULL);
		pRoomTopManager->ProcessInitOffRoomActorRsp(evt);
	}
}

bool RoomModule::IsEnteredTheRoom( uint32 unRoomID )
{
	return m_mapEnteredRoomID[unRoomID];
}