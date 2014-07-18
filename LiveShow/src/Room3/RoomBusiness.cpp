#include "StdAfx.h"
#include "RoomBusiness.h"
#include "usermanager\UserManager.h"
#include "Room.h"
#include "loudspeaker\SpeakerMsg.h"
#include "personalsetting\PersonalConfig.h"
#include "webdefine.h"
#include "gift\GiftManager.h"
#include "roomsetting\RoomOption.h"
#include "screeneffect\ScreenEffectManager.h"
#include <Mmsystem.h>
#include "..\CommonLibrary\include\utility\SystemHelper.h"
#include "..\CommonLibrary/include\utility\URLEncode.h"
#include "..\CommonLibrary\include\utility\StringHelper.h"
#include "..\CommonLibrary\include\ui\C179MsgBox.h"


RoomBusiness::RoomBusiness(uint32 roomid, RoomOption * pRoomOption, UserManager * pUserManager, CSofaManager * pSofaManager, IRoomBusinessSink * pRoomBusinessSink)
: m_roomid(roomid),
m_pRoomOption(pRoomOption),
m_pUserManager(pUserManager),
m_pSofaManager(pSofaManager),
m_pRoomBusinessSink(pRoomBusinessSink)
{
	m_nSeqNo = 0 ;
	m_pOrderSongManager = new COrderSongManager();
	CreatePrivateMicDlgPos() ;	
}

RoomBusiness::~RoomBusiness(void)
{
	if (m_pUserManager)
	{
		delete m_pUserManager;
		m_pUserManager = NULL;
	}
	if (m_pRoomOption)
	{
		delete m_pRoomOption;
		m_pRoomOption = NULL;
	}
	if (m_pSofaManager)
	{
		delete m_pSofaManager;
		m_pSofaManager = NULL;
	}
	if (m_pOrderSongManager)
	{
		delete m_pOrderSongManager;
		m_pOrderSongManager = NULL;
	}
}

//更改麦克风音量
void RoomBusiness::UpdateAllRoomMic( uint32 unMicVolume )
{
	g_RoomModule->UpdateAllRoomMic( unMicVolume ) ;
}

RoomOption* RoomBusiness::GetRoomOption() 
{ 
	return m_pRoomOption;	
}

void RoomBusiness::OnGetUserList( std::vector<core::room_user_info> & userlist )
{
	ASSERT(m_pUserManager != NULL);
	std::vector<core::room_user_info>::iterator it ;
	std::vector<IUserItem*> vecLevelTop, vecLevelBlackDiamond, vecLevelDiamond, vecLuckyStar, vecBlink, vecGiftStar, vecCrest;
	std::map<uint8, std::vector<IUserItem*>> mapEventEnter;
	for ( it = userlist.begin(); it != userlist.end(); ++it)
	{
		IUserItem * pUserItem = m_pUserManager->AddUserItem(*it);
		if (pUserItem != NULL)
		{
			if (!pUserItem->IsBlinkEnter() && pUserItem->GetUserVipLevel() == core::VIPLEVEL_TOP)
			{
				vecLevelTop.push_back(pUserItem);
			}
			if (!pUserItem->IsBlinkEnter() && pUserItem->GetUserVipLevel() == core::VIPLEVEL_BLACKDIAMOND)
			{
				vecLevelBlackDiamond.push_back(pUserItem);
			}
			if (!pUserItem->IsBlinkEnter() && pUserItem->GetUserVipLevel() == core::VIPLEVEL_DIAMOND)
			{
				vecLevelDiamond.push_back(pUserItem);
			}
			if (pUserItem->IsLuckyStar())
			{
				vecLuckyStar.push_back(pUserItem);
			}
			if (pUserItem->IsBlinkEnter())
			{
				vecBlink.push_back(pUserItem);
			}
			vector<core::GiftStarInfo> vGiftStarInfoList = pUserItem->GetGiftStarInfo();
			if (vGiftStarInfoList.size() != 0)
			{
				vecGiftStar.push_back(pUserItem);
			}
			if (!pUserItem->IsBlinkEnter() && pUserItem->GetUserVipLevel() > core::VIPLEVEL_HONOR && pUserItem->GetUserVipLevel() < core::VIPLEVEL_DIAMOND)
			{
				vecCrest.push_back(pUserItem);
			}
			uint8 unEventEnterIndex = EventEnterMgr->GetEventIndex(pUserItem->GetUser179ID());
			if (unEventEnterIndex != 0)
			{
				mapEventEnter[unEventEnterIndex].push_back(pUserItem);
			}
		}
	}
	std::vector<IUserItem*>::iterator iter;
	for (iter = vecLevelTop.begin(); iter != vecLevelTop.end(); ++iter)
	{
		m_pRoomBusinessSink->OnEnterRoomGetUserList_VIPLevelTop(*iter);		//至尊皇冠
	}
	for (iter = vecLevelBlackDiamond.begin(); iter != vecLevelBlackDiamond.end(); ++iter)
	{
		m_pRoomBusinessSink->OnEnterRoomGetUserList_VIPLevelBlackDiamond(*iter);
	}
	for (iter = vecLevelDiamond.begin(); iter != vecLevelDiamond.end(); ++iter)
	{
		m_pRoomBusinessSink->OnEnterRoomGetUserList_VIPLevelDiamond(*iter);	//钻石皇冠
	}
	for (iter = vecLuckyStar.begin(); iter != vecLuckyStar.end(); ++iter)
	{
		m_pRoomBusinessSink->OnEnterRoomGetUserList_LuckyStar(*iter);			//超级幸运星
	}
	for (iter = vecBlink.begin(); iter != vecBlink.end(); ++iter)
	{
		m_pRoomBusinessSink->OnEnterRoomGetUserList_Blink(*iter);				//闪亮登场
	}
	for (iter = vecGiftStar.begin(); iter != vecGiftStar.end(); ++iter)
	{
		m_pRoomBusinessSink->OnEnterRoomGetUserList_GiftStar(*iter);			//礼物之星
	}
	for (iter = vecCrest.begin(); iter != vecCrest.end(); ++iter)
	{
		m_pRoomBusinessSink->OnEnterRoomGetUserList_Crest(*iter);				//冠用户
	}
	for (std::map<uint8, std::vector<IUserItem*>>::iterator ite = mapEventEnter.begin(); ite != mapEventEnter.end(); ++ite)
	{
		std::vector<IUserItem*> vecEventEnter = ite->second;
		for (iter = vecEventEnter.begin(); iter != vecEventEnter.end(); ++iter)
		{
			m_pRoomBusinessSink->OnEnterRoomGetUserList_Event(*iter, ite->first);
		}
	}
}

void RoomBusiness::OnGetWaitingList( std::vector<uint32> & vecWaitinglist )
{
	ASSERT(m_pUserManager != NULL);
	m_pUserManager->GetWaitingList(vecWaitinglist);
}

void RoomBusiness::OnEnterRoomNotify( core::room_user_info & userinfo )
{
	ASSERT(m_pUserManager != NULL);
	IUserItem * pUserItem = m_pUserManager->AddUserItem(userinfo);
	m_pRoomBusinessSink->OnEnterRoom(pUserItem);
}

void RoomBusiness::OnLeaveRoomNotify( uint32 UIN )
{
	ASSERT(m_pUserManager != NULL);
	//RoomOption* pRoomOption = m_pRoomDlg->GetRoomOption() ;//RoomOption放到RoomBusiness里面
	//if ( pRoomOption != NULL )
	//{
	//	if ( pRoomOption->IsShowUserInOut() )
	//	{
	//		m_pHtmlOutPutManager->OnLeaveRoom( UIN ) ;
	//	}
	//}	
	//m_pRoomDlg->OnLeaveRoom( UIN ) ;	//优化到usermanager回调
	m_pUserManager->DeleteUserItem(UIN);
}

void RoomBusiness::OnUserStatusChange( uint32 UIN, uint16 unStatus, bool bAdd)
{
	ASSERT(m_pUserManager != NULL);
	m_pUserManager->UpdateUserStatus(UIN, unStatus, bAdd);
}

uint16 RoomBusiness::GetUserStatusByIndex( uint8 unIndex ) 
{
	uint16 unStatus = 0 ;
	switch( unIndex )
	{
	case 1:
		{
			unStatus = USERSTATUSTYPE_PUBLIC_1 ;
		}
		break;
	case 2:
		{
			unStatus = USERSTATUSTYPE_PUBLIC_2 ;
		}
		break ;
	case 3:
		{
			unStatus = USERSTATUSTYPE_PUBLIC_3 ;
		}
		break ;
	default: break ;
	}
	return unStatus ;
}

void RoomBusiness::OnUserTitleChange( uint32 unSrcUIN , uint32 unDstUIN, uint16 unTitle )
{
	ASSERT(m_pUserManager != NULL);
	//	m_pHtmlOutPutManager->OnUserTitleChange( unDstUIN , unSrcUIN , unTitle ) ;
	m_pUserManager->UpdateUserTitle(unDstUIN, unTitle);
}

void RoomBusiness::OnUserStampChange( uint32 unDstUIN, uint16 unStampID)
{
	ASSERT(m_pUserManager != NULL);

	g_RoomModule->UpdateAllRoomUserStampChange( unDstUIN , unStampID ) ;
}

void RoomBusiness::OnUpdateWaitingList( uint32 UIN, bool bAdd )
{
	ASSERT(m_pUserManager != NULL);
	m_pUserManager->UpdateWaitingList(UIN, bAdd);
}

void RoomBusiness::OnOpVideoAudioOK( uint32 nOpAVType ,uint32 unUIN ) 
{
	switch( nOpAVType )
	{
		//更改用户状态
	case OPERATEAVTYPE_OPEN_VIDEO:
		{
			OnUserStatusChange( unUIN , USERSTATUSTYPE_VIDEO , true ) ;
		}
		break;
	case OPERATEAVTYPE_OPEN_AUDIO:
		{
			OnUserStatusChange( unUIN , USERSTATUSTYPE_AUDIO , true ) ;
		}
		break;
	case OPERATEAVTYPE_CLOSE_VIDEO:
		{
			OnUserStatusChange( unUIN , USERSTATUSTYPE_VIDEO , false ) ;
		}
		break;
	case OPERATEAVTYPE_CLOSE_AUDIO:
		{
			OnUserStatusChange( unUIN , USERSTATUSTYPE_AUDIO , false ) ;
		}
		break;
	default:break;
	}
}

void RoomBusiness::OnSpeakerNotify( uint32 unSrcUIN, core::ENM_LoudSpeakerType enmType, std::wstring strMessage )
{
	std::wstring strTemp = strMessage;
	core::MessageFont stFont;
	SpeakerInfo stInfo;
	if(CSpeakerMsg::DecodeMessage(strTemp, stFont, stInfo))
	{
	}

	if (enmType >= core::LOUDSPEAKERTYPE_BIG && !CPersonalConfig::GetInstance()->GetIsBlockSpeakerSound())
	{
		std::wstring strPath = common::utility::systemhelper::Get179AppPath();
		strPath += L"resource\\sound\\msg.wav";
		::PlaySound(strPath.c_str(), NULL, SND_FILENAME | SND_ASYNC);
	}
}

void RoomBusiness::OnSendGiftRsp( SendGiftRspEvent * pInfo ) 
{
	switch( pInfo->enmResult )
	{
	case core::SENDGIFTRESULT_ALL_REBOT:
		pInfo->unSentCount = 1 ;
	case core::SENDGIFTRESULT_OK:
		{
			core::CurInfo myCurInfo = GetCurInfo() ;
			if ( myCurInfo.unUIN == pInfo->unDstUIN )
			{
				return ;
			}			
			if ( pInfo->enmType == core::GIFTTYPE_STAMP )
			{
				//盖章
				OnUserStampChange( pInfo->unDstUIN, pInfo->unGiftID);
			}			
		}
		break;
	default: break;
	}
}

void RoomBusiness::OnSendGiftNotify( SendGiftNotifyEvent *pInfo ) 
{
	core::CurInfo myCurInfo = GetCurInfo() ;
	if ( pInfo->enmType == core::GIFTTYPE_STAMP )
	{
		//盖章
		OnUserStampChange( pInfo->unDstUIN, pInfo->unGiftID);
	}
	else if ( pInfo->enmType == core::GIFTTYPE_CHANGEVOICE)
	{
		OnUserChangeVoice(pInfo->unDstUIN, pInfo->unGiftID);
	}
}

void RoomBusiness::OnNotifyGiftOverTime( uint32 unDstUIN , uint32 unGiftID ) 
{
	IGiftItem *pGiftItem = GiftManager::GetInstance()->SearchStampByID( unGiftID ) ;
	if ( pGiftItem != NULL )
	{
		//m_pUserManager->UpdateUserStamp( dstuin , giftid, false ) ;
		m_pUserManager->UpdateUserGiftItem(unDstUIN, unGiftID, false);
	}
}

//收藏房间
void RoomBusiness::CollectRoom() 
{
	CString strUrl,strTemp;

	Hall::WebInterfaceUrl weburl;
	weburl.m_WebWithPara = true;
	weburl.m_WebInterfaceKeyName = SYSSET_INI_KEYNAME_COLLECTROOM;
	g_RoomModule->m_pModuleManager->CallService(Hall::SERVICE_VALUE_HALL_GET_WEBINTERFACEURL, (param)&weburl);
	strUrl = weburl.m_WebInterfaceUrl.c_str() ;
	strTemp.Format( L"&roomid=%d" ,m_roomid ) ;
	strUrl += strTemp ;
	webinterface::get_http_url_event_req getHttpEvent;
	getHttpEvent.srcMId	=	MODULE_ID_ROOM;
	getHttpEvent.desMId	=	MODULE_ID_WEBSERVICE;
	getHttpEvent.wstrUrl = strUrl;

	m_nSeqNo = g_RoomModule->m_pModuleManager->CallService( getHttpEvent.eventValue , (param)&getHttpEvent ) ;
}

uint32 RoomBusiness::GetSearchSeqNo() 
{
	return m_nSeqNo ;
}

//印章变更
void RoomBusiness::UserStampChange( uint32 unDstUIN, uint16 unStampID ) 
{	
	m_pUserManager->UpdateUserGiftItem(unDstUIN, unStampID);	
}

CString RoomBusiness::GetRegisteUrl()
{
	return URL_REGISTER ;
	//	Hall::WebInterfaceUrl weburl;
	//	weburl.m_WebWithPara = true;
	//	weburl.m_WebInterfaceKeyName = SYSSET_INT_KEYNAME_REGISTER;
	//	g_RoomModule->m_pModuleManager->CallService(Hall::SERVICE_VALUE_HALL_GET_WEBINTERFACEURL, (param)&weburl);
	//	return weburl.m_WebInterfaceUrl ;
}

CString RoomBusiness::GetUpdateVipUrl() 
{
	Hall::WebInterfaceUrl weburl;
	weburl.m_WebWithPara = true;
	weburl.m_WebInterfaceKeyName = SYSSET_INI_KEYNAME_UPDATEVIP;
	g_RoomModule->m_pModuleManager->CallService(Hall::SERVICE_VALUE_HALL_GET_WEBINTERFACEURL, (param)&weburl);
	return weburl.m_WebInterfaceUrl.c_str() ;
}

CString RoomBusiness::GetRequestRoomUrl()
{
	Hall::WebInterfaceUrl weburl;
	weburl.m_WebWithPara = true;
	weburl.m_WebInterfaceKeyName = SYSSET_INI_KEYNAME_REQUESTROOM;
	g_RoomModule->m_pModuleManager->CallService(Hall::SERVICE_VALUE_HALL_GET_WEBINTERFACEURL, (param)&weburl);
	return weburl.m_WebInterfaceUrl.c_str();
}

CString RoomBusiness::GetFeedBackRoomUrl() 
{
	Hall::WebInterfaceUrl weburl;
	weburl.m_WebWithPara = true;
	weburl.m_WebInterfaceKeyName = SYSSET_INI_KEYNAME_FEEDBACK;
	g_RoomModule->m_pModuleManager->CallService(Hall::SERVICE_VALUE_HALL_GET_WEBINTERFACEURL, (param)&weburl);
	return weburl.m_WebInterfaceUrl.c_str();
}

CString RoomBusiness::GetPayUrl() 
{
	Hall::WebInterfaceUrl weburl;
	weburl.m_WebWithPara = true;
	weburl.m_WebInterfaceKeyName = SYSSET_INI_KEYNAME_PAY;
	g_RoomModule->m_pModuleManager->CallService(Hall::SERVICE_VALUE_HALL_GET_WEBINTERFACEURL, (param)&weburl);
	return weburl.m_WebInterfaceUrl.c_str() ;
}

CString RoomBusiness::GetLevelUpUrl()
{
	Hall::WebInterfaceUrl weburl;
	weburl.m_WebWithPara = true;
	weburl.m_WebInterfaceKeyName = SYSSET_INI_KEYNAME_QUICKLEVELUP;
	g_RoomModule->m_pModuleManager->CallService(Hall::SERVICE_VALUE_HALL_GET_WEBINTERFACEURL, (param)&weburl);
	return weburl.m_WebInterfaceUrl.c_str() ;
}

CString RoomBusiness::GetCopyRoomUrl()
{
	Hall::WebInterfaceUrl weburl;
	weburl.m_WebWithPara = false;
	weburl.m_WebInterfaceKeyName = SYSSET_INI_KEYNAME_COPYROOMURL;
	g_RoomModule->m_pModuleManager->CallService(Hall::SERVICE_VALUE_HALL_GET_WEBINTERFACEURL, (param)&weburl);
	return weburl.m_WebInterfaceUrl.c_str() ;
}


CString RoomBusiness::GetTeachAnnounceUrl()
{
	Hall::WebInterfaceUrl weburl;
	weburl.m_WebWithPara = false;
	weburl.m_WebInterfaceKeyName = SYSSET_INI_KEYNAME_TEACHANNOUNCE;
	g_RoomModule->m_pModuleManager->CallService(Hall::SERVICE_VALUE_HALL_GET_WEBINTERFACEURL, (param)&weburl);
	return weburl.m_WebInterfaceUrl.c_str() ;
}

BOOL RoomBusiness::IsBlockChat( uint32 unUIN ) 
{
	BOOL bResult = FALSE ;
	std::wstring strModulePath = common::utility::systemhelper::Get179AppPath();
	common::CURLEncode urlEncode;
	strModulePath = urlEncode.URLEncodeLocalPath(strModulePath);
	std::wstring strForbidChatPath = strModulePath + BLOCK_CHAT_INI_PATH;
	CString strTemp ;
	strTemp.Format( L"%u" , unUIN ) ;
	CString strKeyValue ;
	GetPrivateProfileString( L"BlockChat" ,strTemp ,L"" ,strKeyValue.GetBuffer(MAX_PATH) ,MAX_PATH , strForbidChatPath.c_str() ) ;
	if ( strKeyValue == L"1" )
	{
		bResult = TRUE ;
	}
	strKeyValue.ReleaseBuffer() ;
	return bResult ;
}

void RoomBusiness::UnBlockChat(uint32 unUIN ) 
{
	std::wstring strModulePath = common::utility::systemhelper::Get179AppPath();
	common::CURLEncode urlEncode;
	strModulePath = urlEncode.URLEncodeLocalPath(strModulePath);
	std::wstring strForbidChatPath = strModulePath + BLOCK_CHAT_INI_PATH;
	CString strTemp ;
	strTemp.Format( L"%u" , unUIN ) ;
	WritePrivateProfileString( L"BlockChat" , strTemp , NULL , strForbidChatPath.c_str() ) ;
}

void RoomBusiness::BlockChat( uint32 unUIN ) 
{
	std::wstring strModulePath = common::utility::systemhelper::Get179AppPath();
	common::CURLEncode urlEncode;
	strModulePath = urlEncode.URLEncodeLocalPath(strModulePath);
	std::wstring strForbidChatPath = strModulePath + BLOCK_CHAT_INI_PATH;
	CString strTemp ;
	strTemp.Format( L"%u" , unUIN ) ;
	WritePrivateProfileString( L"BlockChat" , strTemp , L"1" , strForbidChatPath.c_str() ) ;
}

void RoomBusiness::CreatePrivateMicDlgPos()
{
	int cxx = GetSystemMetrics(SM_CXSCREEN) - ROOM_PRIVATE_MIC_DLG_WIDTH ;
	int cyy = GetSystemMetrics(SM_CYSCREEN) - ROOM_PRIVATE_MIC_DLG_HEIGTH ;
	for ( int y = 410 ; y <= cyy ;)
	{
		for ( int x= 260 ; x<= cxx ;)
		{
			PrivateMicDlgPos MicDlgPos ;
			MicDlgPos.nPosX = x ;
			MicDlgPos.nPosY = y ;
			MicDlgPos.bUsed = FALSE ;
			m_vPrivateMicDlgPos.push_back( MicDlgPos ) ;
			x+= ROOM_PRIVATE_MIC_DLG_WIDTH ;
		}
		y+= ROOM_PRIVATE_MIC_DLG_HEIGTH ;
	}
}

void RoomBusiness::GetPrivateMicDlgPos( int& nPosX , int& nPosY , int& nIndex) 
{
	nIndex = -1 ;
	std::vector<PrivateMicDlgPos>::iterator iter ;
	for ( iter = m_vPrivateMicDlgPos.begin() ; iter != m_vPrivateMicDlgPos.end() ; ++iter )
	{
		++nIndex ;
		if ( !iter->bUsed )
		{
			nPosX = iter->nPosX ;
			nPosY = iter->nPosY ;
			iter->bUsed = TRUE ;
			return ;
		}
	}
	nIndex = -1 ;
	nPosX = rand()% (GetSystemMetrics(SM_CXSCREEN) -ROOM_PRIVATE_MIC_DLG_WIDTH) ;
	nPosY = rand()% (GetSystemMetrics(SM_CYSCREEN)-ROOM_PRIVATE_MIC_DLG_HEIGTH);
}

void RoomBusiness::ErasePrivateMicDlgPos( int nIndex ) 
{
	if ( nIndex < m_vPrivateMicDlgPos.size() && nIndex != -1 )
	{
		m_vPrivateMicDlgPos[nIndex].bUsed = FALSE ;
	}
}


void RoomBusiness::ProcessGetUserListRsp( Event const& evt )
{
	core::GetUserListRspEvent * pInfo = (core::GetUserListRspEvent*)evt.m_pstExtraInfo;

	ASSERT( m_pRoomOption != NULL ) ;
	//OnSignature( m_pRoomOption->GetRoomSignature() ) ;
	//OutPutNoticeMsg( m_pRoomOption->GetRoomNotice() ) ;
	OnGetUserList(pInfo->vecUsrList);
	//OnWelecomeWords( m_pRoomOption ) ;
}

void RoomBusiness::ProcessGetWaitingListRsp( Event const& evt )
{
	core::GetWaittinglistRspEvent * pInfo = (core::GetWaittinglistRspEvent*)evt.m_pstExtraInfo;
	OnGetWaitingList(pInfo->vecWaittinglist);
}

void RoomBusiness::ProcessEnterRoomNotify( Event const& evt )
{
	core::EnterRoomNotifyEvent * pInfo = (core::EnterRoomNotifyEvent*)evt.m_pstExtraInfo;
	OnEnterRoomNotify(pInfo->stUserInfo);
}

void RoomBusiness::ProcessLeaveRoomNotify( Event const& evt )
{
	OnLeaveRoomNotify(evt.param0);
}

void RoomBusiness::ProcessNotifyUserInfoUpdate( Event const& evt )
{
	core::NotifyUserInfoUpdateEvent * pInfo = (core::NotifyUserInfoUpdateEvent*)evt.m_pstExtraInfo;
	//目前只用在更新用户vip等级
	if (pInfo->map_key2uin8.size() != 0)
	{
		if (pInfo->map_key2uin8.find(core::INFOTYPE_VIP_LEVEL) != pInfo->map_key2uin8.end())
		{
			m_pUserManager->UpdateUserVipLevel(pInfo->unSrcUIN, pInfo->map_key2uin8[core::INFOTYPE_VIP_LEVEL]);
			m_pSofaManager->UpdateSofaVipLevel(pInfo->unSrcUIN, pInfo->map_key2uin8[core::INFOTYPE_VIP_LEVEL]);
		}

		if (pInfo->map_key2uin8.find(core::INFOTYPE_RICH_LEVEL) != pInfo->map_key2uin8.end())
		{
			m_pUserManager->UpdateUserRichLevel(pInfo->unSrcUIN, pInfo->map_key2uin8[core::INFOTYPE_RICH_LEVEL]);
			m_pSofaManager->UpdateSofaRichLevel(pInfo->unSrcUIN, pInfo->map_key2uin8[core::INFOTYPE_RICH_LEVEL]);
		}
	}
}

void RoomBusiness::ProcessQueryClientInfoRsp( Event const& evt )
{

}

void RoomBusiness::ProcessSetRoomInfoRsp( Event const& evt )
{
	if (evt.param1 == core::SETROOMINFORESULT_OK)
	{
		m_pRoomOption->OnSetInfoOK();
	}
}

void RoomBusiness::ProcessSetRoomInfoNotify( Event const& evt )
{
	core::SetRoomInfoNotifyEvent * pInfo = (core::SetRoomInfoNotifyEvent*)evt.m_pstExtraInfo;
	m_pRoomOption->LoadRoomOption(pInfo->map_key2string, pInfo->map_key2uin8, pInfo->map_key2uin32);

}

void RoomBusiness::ProcessGetRoomInfoRsp( Event const& evt )
{
	core::GetRoomInfoRspEvent * pInfo = (core::GetRoomInfoRspEvent*)evt.m_pstExtraInfo;
	m_pRoomOption->LoadRoomOption(pInfo->map_key2string, pInfo->map_key2uin8, pInfo->map_key2uin32);
}

void RoomBusiness::ProcessLockRoomNotify( Event const& evt )
{

}

void RoomBusiness::ProcessSendMessageRsp( Event const& evt )
{

}

void RoomBusiness::ProcessSendMessageNotify( Event const& evt )
{

}

void RoomBusiness::ProcessRequestShowRsp( Event const& evt )
{
	core::RequestShowRspEvent * pInfo = (core::RequestShowRspEvent*)evt.m_pstExtraInfo ;
	switch(pInfo->enmResult)
	{
	case REQUESTSHOWRESULT_NEED_SELECT:  //需要选择公麦还是私麦
		{
			//m_pHtmlOutPutManager->OnRequestShow_NeedSelect() ;
			//m_pRoomDlg->OnRequestShow_NeedSelect() ;
		}
		break;
	case REQUESTSHOWRESULT_FULL:		//麦已满
	case REQUESTSHOWRESULT_PRIVATE_CLOSED: //房间私麦已关闭
	case REQUESTSHOWRESULT_FAIL:		//排麦失败
		{
			//m_pHtmlOutPutManager->OnRequestShow_Failed( nResult ) ;
			//m_pRoomDlg->OnRequestShow_Failed() ;
		}
		break;
	case REQUESTSHOWRESULT_PUBLIC:  //排上公麦
		{
			if ( m_pUserManager->GetSelfUserItem() != NULL )
			{
				OnUserStatusChange( m_pUserManager->GetSelfUserItem()->GetUserUIN() , GetUserStatusByIndex(pInfo->unIndex) , true ) ;
				OnUpdateWaitingList( m_pUserManager->GetSelfUserItem()->GetUserUIN() , false ) ;
			}
		}
		break;
	case REQUESTSHOWRESULT_WAITTINGLIST: //进入到排麦队列
		{
			if (m_pUserManager->GetSelfUserItem() != NULL)
			{			
				OnUpdateWaitingList( m_pUserManager->GetSelfUserItem()->GetUserUIN() ,true ) ;
			}
		}
		break;
	case REQUESTSHOWRESULT_PRIVATE:  //排上私麦 ,更改用户列表中的状态
		{
			if ( m_pUserManager->GetSelfUserItem() != NULL )
			{
				OnUserStatusChange( m_pUserManager->GetSelfUserItem()->GetUserUIN() ,USERSTATUSTYPE_PRIVATE , true ) ;
			}			
			//m_pHtmlOutPutManager->OnRequestShow_Private() ;
			//m_pRoomDlg->OnRequestShow_Private() ;
		}
		break;
	case REQUESTSHOWRESULT_WAIT_BUT_ONMIC://从麦序上麦的时候，已在其他的麦上
		{
			if ( m_pUserManager->GetSelfUserItem() != NULL )
			{
				OnUpdateWaitingList( m_pUserManager->GetSelfUserItem()->GetUserUIN() , false ) ;
			}			
		}
		break;
	case REQUESTSHOWRESULT_WAITTING_ONMIC://在其他房间上麦，故上麦序
		{
			if ( m_pUserManager->GetSelfUserItem() != NULL )
			{
				OnUpdateWaitingList(  m_pUserManager->GetSelfUserItem()->GetUserUIN() , true ) ;
			}			
		}
		break;
	default: break;
	}	
}

void RoomBusiness::ProcessRequestShowNotify( Event const& evt )
{
	core::RequestShowNotifyEvent* pInfo = (core::RequestShowNotifyEvent*)evt.m_pstExtraInfo ;
	switch(pInfo->enmResult)
	{
	case REQUESTSHOWRESULT_PUBLIC:  //排上公麦
		{
			OnUserStatusChange( pInfo->unSrcUIN, GetUserStatusByIndex(pInfo->unIndex), true ) ;
			//m_pHtmlOutPutManager->OnRequestShow_PublicNotify( unSrcUIN ) ;
			//m_pRoomDlg->OnRequestShow_PublicNotify( unSrcUIN , unIndex , unTime ) ;
			OnUpdateWaitingList( pInfo->unSrcUIN, false ) ;
		}
		break;
	case REQUESTSHOWRESULT_WAITTINGLIST: //进入到排麦队列
		{
			OnUpdateWaitingList( pInfo->unSrcUIN, true ) ;
		}
		break;
	case REQUESTSHOWRESULT_PRIVATE:  //排上私麦 ,更改用户列表中的状态
		{
			OnUserStatusChange( pInfo->unSrcUIN, USERSTATUSTYPE_PRIVATE, true ) ;
			//m_pHtmlOutPutManager->OnRequestShow_PrivateNotify( unSrcUIN ) ;
		}
		break;
	case REQUESTSHOWRESULT_WAIT_BUT_ONMIC:
		{
			OnUpdateWaitingList( pInfo->unSrcUIN, false ) ;
		}
		break;
	default: break;
	}
}

void RoomBusiness::ProcessExitShowNotify( Event const& evt )
{
	if ( evt.param2 == EXIT_SHOW_OTHER )
	{
		OnUpdateWaitingList( evt.param1 ,false ) ;
		OnUserStatusChange( evt.param1 ,USERSTATUSTYPE_PUBLIC_1|USERSTATUSTYPE_PUBLIC_2|
			USERSTATUSTYPE_PUBLIC_3| USERSTATUSTYPE_PRIVATE , false ) ;
	}
	else
	{
		OnUserStatusChange( evt.param1 , USERSTATUSTYPE_P2P , false ) ;
	}
	OnUserStatusChange( evt.param1 , USERSTATUSTYPE_VIDEO , true ) ;
	OnUserStatusChange( evt.param1 , USERSTATUSTYPE_AUDIO , false ) ;
}

void RoomBusiness::ProcessOpenPrivateRsp( Event const& evt )
{

}

void RoomBusiness::ProcessPushOnShowRsp( Event const& evt )
{
	core::PushOnShowRspEvent* pInfo = (core::PushOnShowRspEvent*)evt.m_pstExtraInfo ;
	switch(pInfo->enmResult)
	{
	case PUSHONRESULT_OK:
		{
			//操作成功
			OnUserStatusChange( pInfo->unDstUIN, GetUserStatusByIndex(pInfo->unIndex), true ) ;
			//m_pHtmlOutPutManager->OnPushOnShowRsp_Success( unDstUIN ) ;
			//m_pRoomDlg->OnPushOnShow_Success( unDstUIN , unIndex ) ;   
			OnUpdateWaitingList( pInfo->unDstUIN , false ) ;
		}
		break;
	case PUSHONRESULT_NO_PERMISSION:
		{
			//权限不足
			//m_pHtmlOutPutManager->OnOperateNotPermission() ;
		}
		break;
	case PUSHONRESULT_IS_ON_SHOW:
		{
			//已经在麦上
			//m_pHtmlOutPutManager->OnPushOnShowRsp_Failed() ;
		}
		break;
	case PUSHONRESULT_NEED_ANSWER:
		{

		}
		break;
	}
}

void RoomBusiness::ProcessPushOnShowNotify( Event const& evt )
{
	core::PushOnShowNotifyEvent* pInfo = (core::PushOnShowNotifyEvent*)evt.m_pstExtraInfo ;
	switch (pInfo->unType)
	{
	case PUSHONNOTIFY_TYPE_NEEDANSWER:
		{

		}
		break;
	case PUSHONNOTIFY_TYPE_CONNECT:
		{
			OnUserStatusChange( pInfo->unDstUIN, GetUserStatusByIndex(pInfo->unIndex), true ) ;
			OnUpdateWaitingList( pInfo->unDstUIN , false ) ;
		}
		break;
	case PUSHONNOTIFY_TYPE_REJECT:
		{

		}
		break;
	}

}

void RoomBusiness::ProcessPullOffShowRsp( Event const& evt )
{
	switch( evt.param1 )
	{
	case core::PULLOFFRESULT_OK:
		{
			//操作成功
			//m_pHtmlOutPutManager->OnPullOffShowRsp_Success( unDstUIN )  ;
			//m_pRoomDlg->OnPullOffShowRsp_Success( unDstUIN ) ;
			OnUserStatusChange( evt.param2, USERSTATUSTYPE_PUBLIC_1 | USERSTATUSTYPE_PUBLIC_2 | USERSTATUSTYPE_PUBLIC_3, false ) ;
			OnUserStatusChange( evt.param2, USERSTATUSTYPE_VIDEO, true ) ;
			OnUserStatusChange( evt.param2, USERSTATUSTYPE_AUDIO, false ) ;
		}
		break ;
	case core::PULLOFFRESULT_NO_PERMISSION:
		{
			//权限不足
			//m_pHtmlOutPutManager->OnOperateNotPermission() ;
		}
		break;
	case core::PULLOFFRESULT_IS_OFF_SHOW:
		{
			//不在麦上
			//m_pHtmlOutPutManager->OnPullOffShowRsp_Failed() ;
		}
		break;
	}
}

void RoomBusiness::ProcessPullOffShowNotify( Event const& evt )
{
	//m_pHtmlOutPutManager->OnPuffOffShowNotify( unSrcUIN , unDstUIN ) ;
	//m_pRoomDlg->OnExitShowNotify( unDstUIN ) ;
	OnUserStatusChange( evt.param2, USERSTATUSTYPE_PUBLIC_1 | USERSTATUSTYPE_PUBLIC_2 | USERSTATUSTYPE_PUBLIC_3, false ) ;
	OnUserStatusChange( evt.param2, USERSTATUSTYPE_VIDEO, true ) ;
	OnUserStatusChange( evt.param2, USERSTATUSTYPE_AUDIO, false ) ;
}

void RoomBusiness::ProcessOpVideoAudioRsp( Event const& evt )
{
	core::OperateVideoAudioRspEvent* pInfo = (core::OperateVideoAudioRspEvent*)evt.m_pstExtraInfo ;
	switch(pInfo->enmResult)
	{
	case OPERATEAVRESULT_OK:
		{
			//m_pRoomDlg->OnOpVideoAudioRsp( nType , unDstUIN ) ;
			OnOpVideoAudioOK( pInfo->enmType, pInfo->unDstUIN ) ;
		}
		break;
	case OPERATEAVRESULT_NOT_ON_SHOW:
		{

		}
		break;
	default:break;
	}
}

void RoomBusiness::ProcessOpVideoAudioNotify( Event const& evt )
{
	OnOpVideoAudioOK( evt.param1, evt.param2 ) ;
	//m_pRoomDlg->OnOpVideoAudioNotify( nOpAVType , unSrcUIN ) ;
}

void RoomBusiness::ProcessRequestP2PRsp( Event const& evt )
{

}

void RoomBusiness::ProcessRequestP2PNotify( Event const& evt )
{

}

void RoomBusiness::ProcessResponseP2PRsp( Event const& evt )
{
	core::ResponseP2PRspEvent* pInfo = ( core::ResponseP2PRspEvent*)evt.m_pstExtraInfo ;
	if ( pInfo->enmResult == core::CONNECTP2PRESULT_OK )
	{
		if ( pInfo->enmType == core::RESPONSEP2PTYPE_ACCEPT )
		{
			core::CurInfo myCurInfo = GetCurInfo() ; 
			OnUserStatusChange( myCurInfo.unUIN , USERSTATUSTYPE_P2P , true ) ; 
			OnUserStatusChange( pInfo->unDstUIN , USERSTATUSTYPE_P2P , true ) ;
		}
	}
}

void RoomBusiness::ProcessResponseP2PNotify( Event const& evt )
{
	core::ResponseP2PNotifyEvent* pInfo = (core::ResponseP2PNotifyEvent*)evt.m_pstExtraInfo ;
	if ( pInfo->enmType == core::RESPONSEP2PTYPE_ACCEPT )
	{
		OnUserStatusChange( pInfo->unSrcUIN , USERSTATUSTYPE_P2P , true ) ; 
		OnUserStatusChange( pInfo->unDstUIN , USERSTATUSTYPE_P2P , true ) ;
	}
}

void RoomBusiness::ProcessNotifyPrivateShow( Event const& evt )
{

}


void RoomBusiness::ProcessSetMicStatusRsp( Event const& evt ) //限制使用此麦
{

	uint16 nIndex = evt.param1&0xFFFF;
	uint16 nStatus = evt.param1>>16 ;
	m_pRoomOption->SetPublicMicStatus( nIndex , nStatus ) ;
}

void RoomBusiness::ProcessSetMicStatusNotify( Event const& evt ) 
{
	uint16 nIndex = evt.param1&0xFFFF;
	uint16 nStatus = evt.param1>>16 ;
	m_pRoomOption->SetPublicMicStatus( nIndex , nStatus ) ;
}

void RoomBusiness::ProcessKickUserRsp( Event const& evt )
{
	core::KickUserRspEvent * pInfo = (core::KickUserRspEvent*)evt.m_pstExtraInfo ;
	switch( pInfo->enmKickResult )
	{
	case core::KICKRESULT_OK:
		{
			//m_pHtmlOutPutManager->OnKickUserRsp_Success(unDstUIN , bKickAllVisitor ) ;
			if ( pInfo->bKickAllVisitor )
			{
				std::vector<IUserItem*>::iterator iter ;
				std::vector<IUserItem*> vecGuestList ;
				m_pUserManager->GetGuestList( vecGuestList ) ;
				for ( iter = vecGuestList.begin() ; iter != vecGuestList.end() ; ++iter )
				{
					IUserItem* pUserItem = *iter ;
					//m_pHtmlOutPutManager->OnKickOutRsp( pUserItem->GetUserUIN() ) ;
					//m_pRoomDlg->OnLeaveRoom( pUserItem->GetUserUIN() ) ;//优化到UserManager里回调
					m_pRoomBusinessSink->OnKickUserRsp(pUserItem->GetUserUIN());
				}
				m_pUserManager->DeleteAllGuest();
			}
			else
			{
				//m_pRoomDlg->OnLeaveRoom( unDstUIN ) ;//优化到UserManager里回调
				m_pUserManager->DeleteUserItem( pInfo->unDstUIN ) ; 
			}		
		}
		break;
	case core::KICKRESULT_NO_PERMISSION:
		{
			//m_pHtmlOutPutManager->OnOperateNotPermission() ;
		}
		break;
	case core::KICKRESULT_OTHER_IS_KING:
		{
			//m_pHtmlOutPutManager->OnKickUserNotPermission( unDstUIN ) ;
		}
		break;
	default:break;
	}
}

void RoomBusiness::ProcessKickUserNotify( Event const& evt )
{
	core::KickUserNotifyEvent * pInfo = (core::KickUserNotifyEvent*)evt.m_pstExtraInfo;
	CString strTip1, strTip2, strTip3;
	core::CurInfo myCurInfo = GetCurInfo() ;
	Hall::MsgBox179 msgbox179;
	switch( pInfo->enmType )
	{
	case core::KickUserNotifyEvent::KICKTYPE_ONE://踢单个人
		{
			if ( myCurInfo.unUIN == pInfo->unDstUIN )
			{				
				if ( myCurInfo.enmVIPLevel == core::VIPLEVEL_NONE )
				{
					msgbox179.m_sMessage1 = L"被请出房间了？";
					msgbox179.m_sMessage2 = L"想要畅玩白兔KTV？";
					msgbox179.m_sMessage3 = L"";
					msgbox179.m_linkLocal = GetRegisteUrl() ;
					msgbox179.m_linkText = L"快去注册吧" ;
					msgbox179.m_boxStype = Hall::MsgBox179::MSGBOX_GUEST_OPERATE ;
					KickLeaveRoom(pInfo->unRoomID, msgbox179);
					return;
				}
				else
				{
					IUserItem* pSrcUserItem = m_pUserManager->SearchUserByUIN( pInfo->unSrcUIN ) ;
					if ( pSrcUserItem == NULL )
					{
						return;
					}										
					strTip1.Format(L"您因\"%s\"被%s踢出房间", pInfo->strReason.c_str() , pSrcUserItem->GetUserName().c_str() );
					int unTime = pInfo->unTime/60 ;
					if ( unTime == 0 )
					{
						unTime = 1 ;
					}
					strTip2.Format(L"%d分钟后可重新进入，寄人篱下，处处受限", unTime );
					msgbox179.m_sMessage1 = strTip1 ;
					msgbox179.m_sMessage2 = strTip2 ;
					msgbox179.m_sMessage3 = L"";
					msgbox179.m_linkLocal = GetRequestRoomUrl() ;
					msgbox179.m_linkText =  L"不如拥有一个自己的房间";
					msgbox179.m_sTitle = L"提 示" ;
					msgbox179.m_boxStype = Hall::MsgBox179::MSGBOX_OK_TWO_STRING_WITH_LINK ;
					KickLeaveRoom(pInfo->unRoomID, msgbox179);
					return;
				}
			}
			else
			{
				//m_pHtmlOutPutManager->OnKickOutNotify( unSrcUIN  , unDstUIN ) ;
				m_pRoomBusinessSink->OnKickUserNotify(pInfo->unSrcUIN, pInfo->unDstUIN);
				//m_pRoomDlg->OnLeaveRoom( unDstUIN ) ;//优化到usermanager回调
				m_pUserManager->DeleteUserItem(pInfo->unDstUIN);
			}
		}
		break;
	case core::KickUserNotifyEvent::KICKTYPE_VISITOR: //踢游客
		{
			if ( myCurInfo.enmVIPLevel == core::VIPLEVEL_NONE )
			{
				msgbox179.m_sMessage1 = L"被请出房间了？";
				msgbox179.m_sMessage2 = L"想要畅玩白兔KTV？";
				msgbox179.m_sMessage3 = L"";
				msgbox179.m_linkLocal = GetRegisteUrl() ;
				msgbox179.m_linkText = L"快去注册吧" ;
				msgbox179.m_boxStype = Hall::MsgBox179::MSGBOX_GUEST_OPERATE ;
				KickLeaveRoom(pInfo->unRoomID, msgbox179);
				return;
			}
			else
			{
				std::vector<IUserItem*>::iterator iter ;
				std::vector<IUserItem*> vecGuestList ;
				m_pUserManager->GetGuestList( vecGuestList ) ;
				for ( iter = vecGuestList.begin() ; iter != vecGuestList.end() ; ++iter )
				{
					IUserItem* pUserItem = *iter ;
					//m_pHtmlOutPutManager->OnKickOutNotify( unSrcUIN  , pUserItem->GetUserUIN() ) ;
					m_pRoomBusinessSink->OnKickUserNotify(pInfo->unSrcUIN, pInfo->unDstUIN);
					//m_pRoomDlg->OnLeaveRoom( pUserItem->GetUserUIN() ) ;//优化到usermanager回调
				}
				m_pUserManager->DeleteAllGuest();
			}
		}
		break;
	case core::KickUserNotifyEvent::KICKTYPE_FOR_MODIFY_CAPACITY://由于减容被踢
		{
			if (myCurInfo.unUIN == pInfo->unSrcUIN)
			{
				//strTip1.Format(L"尊敬的%s：", myCurInfo.strName.c_str());
				//strTip2.Format(L"您所在的房间:%s(%d)人数已超过上限，", m_pRoomOption->GetRoomName().c_str() ,GetRoomID());
				//strTip3 = L"请换至其他房间，由此给您带来的不便敬请谅解";
				msgbox179.m_sMessage1 = L"抱歉，您所在的房间人数已达到上限，" ;
				msgbox179.m_sMessage2 = L"还是去隔壁逛逛吧" ;
				msgbox179.m_sMessage3 = L"" ;
				msgbox179.m_sTitle = L"提 示" ;
				msgbox179.m_boxStype = Hall::MsgBox179::MSGBOX_OK_THREE_STRING ;
				KickLeaveRoom(pInfo->unRoomID, msgbox179);
				return;
			}
			else
			{
				m_pUserManager->DeleteUserItem(pInfo->unSrcUIN);
			}
		}
		break;
	default:break;
	}

	return;
}

void RoomBusiness::ProcessAddBlackRsp( Event const& evt )
{
	switch( evt.param1 )
	{
	case core::KICKRESULT_OK:
		{
			//m_pHtmlOutPutManager->OnAddBlackRsp_Success( dstuin ) ;
			//m_pRoomDlg->OnLeaveRoom( dstuin ) ;//优化到UserManager里回调
			m_pRoomBusinessSink->OnAddBlackRsp(evt.param2);
			m_pUserManager->DeleteUserItem(evt.param2);
		}
		break;
	case core::KICKRESULT_NO_PERMISSION:
		{
			//m_pHtmlOutPutManager->OnOperateNotPermission() ;
		}
		break;
	case core::KICKRESULT_OTHER_IS_KING:
		{
			//m_pHtmlOutPutManager->OnKickUserNotPermission( dstuin ) ;
		}
		break;
	default:break;
	}
}

void RoomBusiness::ProcessAddBlackNotify( Event const& evt )
{
	core::CurInfo myCurInfo = GetCurInfo() ;
	IUserItem* pSrcUserItem = m_pUserManager->SearchUserByUIN( evt.param1 ) ;
	if ( pSrcUserItem == NULL )
	{
		return;
	}
	if ( myCurInfo.unUIN == evt.param2 )
	{
		Hall::MsgBox179 msgbox179;
		msgbox179.m_sMessage1 = L"您被管理员禁止进入此房间";
		msgbox179.m_sMessage2 = L"寄人篱下,处处受限" ;
		msgbox179.m_sMessage3 = L"";
		msgbox179.m_linkLocal = GetRequestRoomUrl() ;
		msgbox179.m_linkText =  L"不如拥有一个自己的房间";
		msgbox179.m_sTitle = L"提 示" ;
		msgbox179.m_boxStype = Hall::MsgBox179::MSGBOX_OK_TWO_STRING_WITH_LINK ;
		KickLeaveRoom(evt.param0, msgbox179);
		return;
	}
	else
	{
		//m_pHtmlOutPutManager->OnAddBlackNotify( unSrcUIN , unDstUIN ) ;
		//m_pRoomDlg->OnLeaveRoom( unDstUIN ) ;//优化到usermanager回调
		m_pRoomBusinessSink->OnAddBlackNotify(evt.param1, evt.param2);
		m_pUserManager->DeleteUserItem( evt.param2 );
		return;
	}
}

void RoomBusiness::ProcessLockIpRsp( Event const& evt )
{
	switch( evt.param1 )
	{
	case core::KICKRESULT_OK:
		{
			//m_pHtmlOutPutManager->OnLockIpRsp_Success( dstuin ) ;
			//m_pRoomDlg->OnLeaveRoom( dstuin ) ;//优化到UserManager里回调
			m_pRoomBusinessSink->OnLockIpRsp(evt.param2);
			m_pUserManager->DeleteUserItem(evt.param2);
		}
		break;
	case core::KICKRESULT_NO_PERMISSION:
		{
			//m_pHtmlOutPutManager->OnOperateNotPermission() ;
		}
		break;
	case core::KICKRESULT_OTHER_IS_KING:
		{

		}
		break;
	default:break;
	}
}

void RoomBusiness::ProcessLockIpNotify( Event const& evt )
{
	core::LockIPNotifyEvent * pInfo = (core::LockIPNotifyEvent*)evt.m_pstExtraInfo;
	core::CurInfo myCurInfo = GetCurInfo() ;
	IUserItem* pSrcUserItem = m_pUserManager->SearchUserByUIN( pInfo->unSrcUIN ) ;
	if ( pSrcUserItem == NULL )
	{
		return;
	}
	IUserItem* pSelfUserItem = m_pUserManager->SearchUserByUIN( myCurInfo.unUIN ) ;
	if ( pSelfUserItem == NULL )
	{
		return ;
	}

	if ( myCurInfo.unUIN == pInfo->unDstUIN )
	{
		RoomOption *pRoomOption = GetRoomOption() ;
		if ( pRoomOption == NULL )
		{
			return ;
		}
		CString strTip1, strTip2 ;
		Hall::MsgBox179 msgbox179;	
		if ( pRoomOption->GetOwner179ID() == myCurInfo.un179id )
		{
			strTip1.Format(L"您因\"%s\"被超管封停IP", pInfo->strReason.c_str());
			strTip2.Format(L"%d分钟后可重新进入！", pInfo->unTime );
			msgbox179.m_sMessage1 = strTip1 ;
			msgbox179.m_sMessage2 = strTip2 ;
			msgbox179.m_sMessage3 = L"";
			msgbox179.m_sTitle = L"提 示" ;
			msgbox179.m_boxStype = Hall::MsgBox179::MSGBOX_OK_TWO_STRING ;
		}
		else
		{
			strTip1.Format(L"您因\"%s\"被管理员封停IP", pInfo->strReason.c_str());
			strTip2.Format(L"%d分钟后可重新进入！寄人篱下，处处受限", pInfo->unTime );
			msgbox179.m_sMessage1 = strTip1 ;
			msgbox179.m_sMessage2 = strTip2 ;
			msgbox179.m_sMessage3 = L"";
			msgbox179.m_sTitle = L"提 示" ;
			msgbox179.m_linkLocal = GetRequestRoomUrl() ;
			msgbox179.m_linkText =  L"不如拥有一个自己的房间";
			msgbox179.m_boxStype = Hall::MsgBox179::MSGBOX_OK_TWO_STRING_WITH_LINK ;
		}
		KickLeaveRoom(pInfo->unRoomID, msgbox179);
		return;
	}
	else
	{
		//m_pHtmlOutPutManager->OnLockIpNotify( unSrcUIN , unDstUIN ) ;
		//m_pRoomDlg->OnLeaveRoom( unDstUIN ) ;//优化到usermanager回调
		m_pRoomBusinessSink->OnLockIpNotify(pInfo->unSrcUIN, pInfo->unDstUIN);
		m_pUserManager->DeleteUserItem( pInfo->unDstUIN );
		return;
	}
}

void RoomBusiness::ProcessShutUpRsp( Event const& evt )
{
	switch( evt.param1 )
	{
	case core::KICKRESULT_OK:
		{
			//m_pHtmlOutPutManager->OnShutUpRsp_Success( dstuin ) ;
			OnUserStatusChange(evt.param2, core::USERSTATUSTYPE_FORBIDDEN, true);
		}
		break;
	case core::KICKRESULT_NO_PERMISSION:
		{
			//m_pHtmlOutPutManager->OnOperateNotPermission() ;
		}
		break;
	default:break;
	}
}

void RoomBusiness::ProcessShutUpNotify( Event const& evt )
{
	core::ShutUpNotifyEvent* pInfo = (core::ShutUpNotifyEvent*)evt.m_pstExtraInfo ;
	OnUserStatusChange(pInfo->dstUIN, core::USERSTATUSTYPE_FORBIDDEN, true);
}

void RoomBusiness::ProcessUnLockShutUpRsp( Event const& evt )
{
	switch( evt.param1 )
	{
	case core::KICKRESULT_OK:
		{
			//m_pHtmlOutPutManager->OnUnLockShutUpRsp_Success( dstuin ) ;
			OnUserStatusChange(evt.param2, core::USERSTATUSTYPE_FORBIDDEN, false);
		}
		break;
	case core::KICKRESULT_NO_PERMISSION:
		{
			//m_pHtmlOutPutManager->OnOperateNotPermission() ;
		}
		break;
	default:break;
	}
}

void RoomBusiness::ProcessUnLockShutUpNotify( Event const& evt )
{
	//m_pHtmlOutPutManager->OnUnLockShutUpNotify( dstuin ) ;
	OnUserStatusChange( evt.param2 , core::USERSTATUSTYPE_FORBIDDEN, false);
}

void RoomBusiness::ProcessAddTitleRsp( Event const& evt )
{
	core::AddTitleRspEvent* pInfo = (core::AddTitleRspEvent*)evt.m_pstExtraInfo ;
	switch( pInfo->enmResult )
	{
	case  core::CHANGETILERESULT_OK:
		{
			//m_pHtmlOutPutManager->OnAddTitleRsp_Success( nTitleLevel , unDstUIN ) ;
			m_pUserManager->UpdateUserTitle( pInfo->unDstUIN, pInfo->enmLevel );
		}
		break;
	case core::CHANGETILERESULT_NO_PERMISSION:
		{
			//m_pHtmlOutPutManager->OnOperateNotPermission( ) ;
		}
		break;
	case core::CHANGETILERESULT_IS_FULL:  //管理员数量已满
		{ 
			//m_pHtmlOutPutManager->OnAddTitleRsp_IsFull( nTitleLevel , unDstUIN ) ;
		}
		break;
	case core::CHANGETILERESULT_IS_ADMIN_INOTHER: //已经是其他房间的管理员
		{
			//m_pHtmlOutPutManager->OnAddTitleRsp_IsAdmin_InOther( nTitleLevel , unDstUIN ) ;
		}
		break;
	default: break;
	}
}

void RoomBusiness::ProcessRemoveTitleRsp( Event const& evt )
{
	core::RemoveTitleRspEvent* pInfo = (core::RemoveTitleRspEvent*)evt.m_pstExtraInfo ;
	switch( pInfo->enmResult )
	{
	case core::CHANGETILERESULT_OK:
		{
			//m_pHtmlOutPutManager->OnRemoveTitleRsp_Success( nTitleLevel , unDstUIN ) ;
			m_pUserManager->UpdateUserTitle( pInfo->unDstUIN, core::TITLELEVEL_NONE );
		}
		break;
	case core::CHANGETILERESULT_NO_PERMISSION:
		{
			//m_pHtmlOutPutManager->OnOperateNotPermission( ) ;
		}
		break;
	default:break;
	}
}

void RoomBusiness::ProcessAddTitleNotify( Event const& evt )
{
	core::AddTitleNotifyEvent* pInfo = (core::AddTitleNotifyEvent*)evt.m_pstExtraInfo ;
	//m_pHtmlOutPutManager->OnAddTitleNotify( unSrcUIN , unDstUIN , enmLevel , m_roomid ) ;
	m_pUserManager->UpdateUserTitle( pInfo->unDstUIN, pInfo->enmLevel ) ;
}

void RoomBusiness::ProcessRemoveTitleNotify( Event const& evt )
{
	core::RemoveTitleNotifyEvent* pInfo = (core::RemoveTitleNotifyEvent*)evt.m_pstExtraInfo ;
	//m_pHtmlOutPutManager->OnRemoveTitleNotify( unSrcUIN , unDstUIN , enmLevel , m_roomid ) ;
	m_pUserManager->UpdateUserTitle( pInfo->unDstUIN, core::TITLELEVEL_NONE ) ;
}

void RoomBusiness::ProcessSendGiftRsp( Event const& evt )
{
	core::SendGiftRspEvent* pInfo = (core::SendGiftRspEvent*)evt.m_pstExtraInfo;
	OnSendGiftRsp(pInfo);
	//if (pInfo->enmResult == core::SENDGIFTRESULT_OK || pInfo->enmResult == core::SENDGIFTRESULT_ALL_REBOT)
	//{
	//IUserItem * pDstUserItem = m_pUserManager->SearchUserByUIN(pInfo->unDstUIN);
	//std::wstring strSrcUserName = L"";
	//std::wstring strDstUserName = L"";
	//if (m_pUserManager->GetSelfUserItem() != NULL)
	//{
	//	strSrcUserName = m_pUserManager->GetSelfUserItem()->GetUserName();
	//}
	//if (pDstUserItem != NULL)
	//{
	//	strDstUserName = pDstUserItem->GetUserName();
	//}


	//ScreenEffectManager::GetInstance()->ProcessSendGift(pInfo->unSentCount, pInfo->unGiftID, strSrcUserName, strDstUserName);
	//}
}

void RoomBusiness::ProcessSendGiftNotify( Event const& evt )
{
	core::SendGiftNotifyEvent* pInfo = (core::SendGiftNotifyEvent*)evt.m_pstExtraInfo;
	OnSendGiftNotify(pInfo);
	//IUserItem * pSrcUserItem = m_pUserManager->SearchUserByUIN(pInfo->unSrcUIN);
	//IUserItem * pDstUserItem = m_pUserManager->SearchUserByUIN(pInfo->unDstUIN);
	//std::wstring strSrcUserName = L"";
	//std::wstring strDstUserName = L"";
	//if (pSrcUserItem != NULL)
	//{
	//	strSrcUserName = pSrcUserItem->GetUserName();
	//}
	//if (pDstUserItem != NULL)
	//{
	//	strDstUserName = pDstUserItem->GetUserName();
	//}

	//ScreenEffectManager::GetInstance()->ProcessSendGift(pInfo->unSentCount, pInfo->unGiftID, strSrcUserName, strDstUserName);
}

void RoomBusiness::ProcessNotifyGiftOverTime( Event const& evt )
{
	m_pUserManager->UpdateUserGiftItem(evt.param1, evt.param2, false);
}

void RoomBusiness::ProcessNotifyPlutusAward( Event const& evt )
{
	//ScreenEffectManager::GetInstance()->ProcessSendPlutus();
}

void RoomBusiness::ProcessNotifyMoneyUpdate( Event const& evt )
{

}

void RoomBusiness::ProcessNotifyOfficeNotice( Event const& evt )
{

}

//财富宝座
void RoomBusiness::ProcessNotifyRichStar(Event const& evt ) 
{

}

void RoomBusiness::KickLeaveRoom( uint32 unRoomID, Hall::MsgBox179 msgbox179 )
{
	g_RoomModule->KickLeaveRoom(unRoomID);
	g_RoomModule->m_pModuleManager->CallService(Hall::SERVICE_MSGBOX_SHOW, (param)&msgbox179);
}

uint32 RoomBusiness::GetCaiZiIntervalTime( core::ENM_VipLevel nVipLevel ) 
{
	uint32 nIntervalTime = -1 ;
	switch( nVipLevel )
	{
	case core::VIPLEVEL_NONE:
		break;
	case core::VIPLEVEL_REGISTER:   //已注册
		nIntervalTime = 30 ;
		break;
	case core::VIPLEVEL_VIP:		//vip
	case core::VIPLEVEL_HONOR:
		nIntervalTime = 10 ;
		break;
	case core::VIPLEVEL_SILVER:		//银色皇冠
	case core::VIPLEVEL_GOLDEN:
	case core::VIPLEVEL_CRYSTAL:
	case core::VIPLEVEL_DIAMOND:	//钻石皇冠
		nIntervalTime = 5 ;
		break;
	case core::VIPLEVEL_BLACKDIAMOND://黑钻皇冠
	case core::VIPLEVEL_TOP:		//至尊皇冠
		nIntervalTime = 0 ;
	}
	return nIntervalTime ;
}

void RoomBusiness::OnUserChangeVoice( uint32 unDstUIN, uint16 unChangeVoiceID )
{
	m_pUserManager->UpdateUserChangeVoiceItem(unDstUIN, unChangeVoiceID);
}

void RoomBusiness::ProcessSendFlyWordsRsp( Event const& evt )
{
	core::SendFlyWordsRspEvent * pInfo = (core::SendFlyWordsRspEvent*)evt.m_pstExtraInfo;
	if (pInfo->enmResult == core::SENDGIFTRESULT_OK)
	{
		//ScreenEffectManager::GetInstance()->ProcessFlyingText(m_pUserManager->GetSelfUserItem()->GetUserName(), pInfo->strMessage);
	}
}

void RoomBusiness::ProcessSendFlyWordsNofity( Event const& evt )
{
	core::SendFlyWordsNotifyEvent * pInfo = (core::SendFlyWordsNotifyEvent*)evt.m_pstExtraInfo;

	//ScreenEffectManager::GetInstance()->ProcessFlyingText(pInfo->strName, pInfo->strMessage);
}

void RoomBusiness::ProcessMyInfoUpdate()
{
	m_pUserManager->UpdateUserVipLevel(GetCurInfo().unUIN, GetCurInfo().enmVIPLevel);
}

void RoomBusiness::ProcessMyVipLevelUpdate()
{
	m_pUserManager->UpdateUserVipLevel(GetCurInfo().unUIN, GetCurInfo().enmVIPLevel);
	m_pSofaManager->UpdateSofaVipLevel(GetCurInfo().unUIN, GetCurInfo().enmVIPLevel);
}

void RoomBusiness::ProcessMyRichLevelUpdate()
{
	m_pUserManager->UpdateUserRichLevel(GetCurInfo().unUIN, GetCurInfo().unRichLevel);
	m_pSofaManager->UpdateSofaRichLevel(GetCurInfo().unUIN, GetCurInfo().unRichLevel);
}

void RoomBusiness::ProcessAnswerPushOnShowRsp( Event const& evt )
{

}

void RoomBusiness::ProcessGetSofaListRsp( Event const& evt )
{
	core::GetSofaListRspEvent * pInfo = (core::GetSofaListRspEvent*)evt.m_pstExtraInfo;
	if (m_pSofaManager != NULL)
	{
		m_pSofaManager->InitList(pInfo->sofa);
	}
}

void RoomBusiness::ProcessBuySofaRsp( Event const& evt )
{
	//test
	//std::vector<core::Song> vecSongList;
	//for (int i = 0; i < 10; i++)
	//{
	//	core::Song song;
	//	song.songName = L"test";
	//	song.songType = 1;
	//	vecSongList.push_back(song);
	//}

	//m_pOrderSongManager->OnGetSongList(1, 1111, vecSongList);
}

void RoomBusiness::ProcessBuySofaNotify( Event const& evt )
{
	core::BuySofaNotifyEvent * pInfo = (core::BuySofaNotifyEvent*)evt.m_pstExtraInfo;
	IUserItem * pUserItem = m_pUserManager->SearchUserByUIN(pInfo->uin);
	if (pUserItem != NULL)
	{
		if (m_pSofaManager != NULL)
		{
			m_pSofaManager->UpdateSofa(pInfo->sofaIndex, pInfo->price, pUserItem);
		}
	}
}

void RoomBusiness::ProcessNotifyClearSofaList( Event const& evt )
{
	if (m_pSofaManager != NULL)
	{
		m_pSofaManager->ClearSofaList();
	}
}

void RoomBusiness::ProcessUpdateSongListRsp( Event const& evt )
{
	core::UpdateSongListRspEvent * pInfo = (core::UpdateSongListRspEvent*)evt.m_pstExtraInfo;
	if (pInfo->enmResult == core::UpdateSongListRspEvent::UPDATESONGRESULT_OK)
	{
		IUserItem * pSelfItem = m_pUserManager->GetSelfUserItem();
		if (m_pOrderSongManager != NULL && pSelfItem != NULL)
		{
			m_pOrderSongManager->OnGetSongList(pSelfItem->GetPublicMicIndex(), pSelfItem->GetUserUIN(), pInfo->vecSongList);
		}
	}
}

void RoomBusiness::ProcessGetSongListRsp( Event const& evt )
{
	core::GetSongListRspEvent * pInfo = (core::GetSongListRspEvent*)evt.m_pstExtraInfo;
	IUserItem * pUserItem = m_pUserManager->SearchUserByUIN(pInfo->songerUin);
	if (m_pOrderSongManager != NULL && pUserItem != NULL)
	{
		m_pOrderSongManager->OnGetSongList(pUserItem->GetPublicMicIndex(), pUserItem->GetUserUIN(), pInfo->vecSongList);
	}
}

void RoomBusiness::ProcessOrderSongRsp( Event const& evt )
{

}

void RoomBusiness::ProcessOrderSongNotify( Event const& evt )
{
	core::OrderSongNotifyEvent * pInfo = (core::OrderSongNotifyEvent*)evt.m_pstExtraInfo;
}

void RoomBusiness::ProcessTakeOrderRsp( Event const& evt )
{

}

void RoomBusiness::ProcessTakeOrderNotify( Event const& evt )
{
}

void RoomBusiness::ProcessGetOrderListRsp( Event const& evt )
{
	core::GetOrderListRspEvent * pInfo = (core::GetOrderListRspEvent*)evt.m_pstExtraInfo;
	IUserItem * pSelfItem = m_pUserManager->GetSelfUserItem();
	if (m_pOrderSongManager != NULL && pSelfItem != NULL)
	{
		m_pOrderSongManager->OnGetOrderList(pSelfItem->GetUserUIN(), pSelfItem->IsActor(), pInfo->orderList);
	}

}

void RoomBusiness::ProcessMarkSongRsp( Event const& evt )
{

}

void RoomBusiness::ProcessMarkSongNotify( Event const& evt )
{

}

CString RoomBusiness::GetRoomPayUrl()
{
	Hall::WebInterfaceUrl weburl;
	weburl.m_WebWithPara = true;
	weburl.m_WebInterfaceKeyName = SYSSET_INI_KEYNAME_ROOMPAY;
	g_RoomModule->m_pModuleManager->CallService(Hall::SERVICE_VALUE_HALL_GET_WEBINTERFACEURL, (param)&weburl);
	return weburl.m_WebInterfaceUrl.c_str() ;
}

CString RoomBusiness::GetServiceUrl()
{
	Hall::WebInterfaceUrl weburl;
	weburl.m_WebWithPara = false;
	weburl.m_WebInterfaceKeyName = SYSSET_INI_KEYNAME_SERVICEURL;
	g_RoomModule->m_pModuleManager->CallService(Hall::SERVICE_VALUE_HALL_GET_WEBINTERFACEURL, (param)&weburl);
	return weburl.m_WebInterfaceUrl.c_str() ;
}