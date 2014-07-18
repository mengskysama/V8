#include "StdAfx.h"
#include "CoreModule.h"
#include "protocolInclude.h"
#include "Roomdefine.h"
#include "Halldefine.h"
#include "log.h"
#include "Core.h"
#include "../ErrorReport/include/CrashRpt.h"
#include "BlacklistHelper.h"

core::CurInfo CoreModule::m_curInfo;

CoreModule::CoreModule(void)
:m_pModuleManager(NULL),
m_Connection(this),
m_lastRoomId(0)
{
}

CoreModule::~CoreModule(void)
{
}

void CoreModule::Load(IModuleManager* p)
{
	m_pModuleManager = p;
}

void CoreModule::Unload()
{

}

void CoreModule::ProcessEvent(Event const& _event)
{
	switch(_event.eventValue)
	{
	case core::EVENT_VALUE_CORE_LOGIN:
		ProcessLogin((param)_event.m_pstExtraInfo);
		break;
	case core::EVENT_VALUE_CORE_LOGOUT:
		ProcessLogout();
		break;
	case core::EVENT_VALUE_CORE_GET_SELFINFO:
		ProcessGetSelfInfo();
		break;
	case core::EVENT_VALUE_CORE_SET_SELFINFO:
		ProcessSetSelfInfo((param)_event.m_pstExtraInfo);
		break;
	case core::EVENT_VALUE_CORE_ENTER_ROOM:
		ProcessEnterRoom((param)_event.m_pstExtraInfo);
		break;
	case core::EVENT_VALUE_CORE_LEAVE_ROOM:
		ProcessLeaveRoom(_event.param0);
		break;
	case core::EVENT_VALUE_CORE_GET_USERLIST:		
		ProcessGetUserList(_event.param0);
		break;
	case core::EVENT_VALUE_CORE_GET_WAITTINGLIST:
		ProcessGetWaittingList(_event.param0);
		break;
	case core::EVENT_VALUE_CORE_SEND_MESSAGE:
		ProcessSendMessage((param)_event.m_pstExtraInfo);
		break;
	case core::EVENT_VALUE_CORE_SEND_GIFT:
		ProcessSendGift((param)_event.m_pstExtraInfo);
		break;
	case core::EVENT_VALUE_CORE_SEND_LOUDSPEAKER:
		ProcessSendLoudSpeaker((param)_event.m_pstExtraInfo);
		break;
	case core::EVENT_VALUE_CORE_REQUEST_SHOW:
		ProcessRequestShow(_event);
		break;
	case core::EVENT_VALUE_CORE_EXIT_SHOW:
		ProcessExitShow(_event);
		break;
	case core::EVENT_VALUE_CORE_OPERATE_VIDEO_AUDIO:
		ProcessOperateAV(_event);
		break;
	case core::EVENT_VALUE_CORE_KICK_USER:
		ProcessKickUser((param)_event.m_pstExtraInfo);
		break;
	case core::EVENT_VALUE_CORE_ADD_BLACK:
		ProcessAddBlack(_event);
		break;
	case core::EVENT_VALUE_CORE_LOCK_IP:
		ProcessLockIP((param)_event.m_pstExtraInfo);
		break;
	case core::EVENT_VALUE_CORE_SHUT_UP:
		ProcessShutUp(_event);
		break;
	case core::EVENT_VALUE_CORE_UNLOCK_SHUTUP:
		ProcessUnShutUp(_event);
		break;
	case core::EVENT_VALUE_CORE_PUSH_ON_SHOW:
		ProcessPushOnShow(_event);
		break;
	case core::EVENT_VALUE_CORE_PULL_OFF_SHOW:
		ProcessPullOffShow(_event);
		break;
	case core::EVENT_VALUE_CORE_ADD_TITLE:
		ProcessAddTitle(_event);
		break;
	case core::EVENT_VALUE_CORE_REMOVE_TITLE:
		ProcessRemoveTitle(_event);
		break;		
	case core::EVENT_VALUE_CORE_OPEN_PRIVATE:
		ProcessOpenPrivate(_event);
		break;
	case core::EVENT_VALUE_CORE_CLOSE_PRIVATE:
		ProcessClosePrivate(_event);
		break;
	case core::EVENT_VALUE_CORE_REQUEST_P2P:
		ProcessRequestP2P(_event);
		break;
	case core::EVENT_VALUE_CORE_RESPONSE_P2P:
		ProcessResponseP2P(_event);
		break;
	case core::EVENT_VALUE_CORE_REPORT_CLIENTINFO:
		ProcessReportClientInfo((param)_event.m_pstExtraInfo);
		break;
	case core::EVENT_VALUE_CORE_QUERY_CLIENTINFO:
		ProcessQueryClientInfo((param)_event.m_pstExtraInfo);
		break;
	case core::EVENT_VALUE_CORE_SET_ROOMINFO:
		ProcessSetRoomInfo((param)_event.m_pstExtraInfo);
		break;
	case core::EVENT_VALUE_CORE_GET_ROOMINFO:
		ProcessGetRoomInfo(_event);
		break;
	case core::EVENT_VALUE_CORE_MIC_SET_MIC_STATUS:
		ProcessSetMicStatus(_event);
		break;
	case core::EVENT_VALUE_CORE_SEND_FLYWORDS:
		ProcessSendFlyWords((param)_event.m_pstExtraInfo);
		break;
	case core::EVENT_VALUE_CORE_REPORT_COLLECT_INFO:
		ProcessReportCollectInfo((param)_event.m_pstExtraInfo);
		break;
	case core::EVENT_VALUE_CORE_ASYN_LOGOUT:
		ProcessLogout();
		break;
	case core::EVENT_VALUE_CORE_RELOGIN:
		Relogin();
		break;
	case core::EVENT_VALUE_CORE_ANSWER_PUSH_ON_SHOW:
		ProcessAnswerPushOnShow((param)_event.m_pstExtraInfo);
		break;
	case core::EVENT_VALUE_CORE_GET_TASK_REWARD:
		ProcessGetTaskReward(_event);
		break;
	case core::EVENT_VALUE_CORE_GET_FANS:
		ProcessGetFans(_event);
		break;
	case core::EVENT_VALUE_CORE_GET_SOFA_LIST:
		ProcessGetSofaList(_event);
		break;
	case core::EVENT_VALUE_CORE_BUY_SOFA:
		ProcessBuySofa(_event);
		break;
	case core::EVENT_VALUE_UPDATE_SONGLIST:
		ProcessUpdateSonglist(_event);
		break;
	case core::EVENT_VALUE_GET_SONGLIST:
		ProcessGetSonglist(_event);
		break;
	case core::EVENT_VALUE_ORDER_SONG:
		ProcessOrderSong(_event);
		break;
	case core::EVENT_VALUE_PROCESS_ORDER:
		ProcessSongOrder(_event);
		break;
	case core::EVENT_VALUE_GET_ORDERLIST:
		ProcessGetOrderlist(_event);
		break;
	case core::EVENT_VALUE_CORE_GET_OFFLINE_ACTOR_REQ:
		ProcessGetActorlist(_event);
		break;
	case core::EVENT_VALUE_MARK_SONG:
		ProcessMarkSong(_event);
		break;

	default:
		break;
	}
}

void CoreModule::ProcessMessage(Message const& msg)
{
	switch(msg.messageValue)
	{
	case MESSAGE_VALUE_SYS_CYCLE_TRIGGER:
		OnCycleTrigger();
		break;
	default:
		break;
	}
}

int32 CoreModule::Invoke(ServiceValue const serviceValue, param st)
{
	switch(serviceValue)
	{
	case core::SERVICE_VALUE_CORE_GET_CUR_INFO:
		return InvokeGetCurInfo(st);
		break;
	case core::SERVICE_VALUE_CORE_SWITCH_ISP:
		return InvokeSwitchISP(st);
		break;
	case core::SERVICE_VALUE_CORE_UPDATE_MONEY:
		return InvokeUpdateMoney(st);
		break;
	case core::SERVICE_VALUE_CORE_GET_GIFT_STAR_INFO:
		return InvokeGetGiftStarInfo(st);
		break;
	case core::SERVICE_VALUE_CORE_GET_MY_GOODS:
		return InvokeGetMyGoods(st);
		break;
	default:
		break;
	}

	return -1;
}

void CoreModule::OnLoginSuccess()
{
	m_localLoginTime = ::GetTickCount()/1000;
	m_serverLoginTime = CoreModule::m_curInfo.unSystemTime;

	SetLoginAccountInfo(CoreModule::m_curInfo.un179id);

	m_Connection.OnLoginSuccess();

	if(CoreModule::GetCurInfo().enmVIPLevel < core::VIPLEVEL_SILVER)
	{
		BlacklistHelper::CheckTick();
	}	

	if(m_lastRoomId > 0)
	{
		m_pModuleManager->PushEvent(
			MakeEvent<MODULE_ID_CORE>()(Hall::EVENT_VALUE_HALL_ENTER_ROOM_REQUEST, MODULE_ID_HALL, m_lastRoomId));

		m_lastRoomId = 0;
	}
}

void CoreModule::CacheLastRoom()
{
	Room_Get_Room_List roomList;
	m_pModuleManager->CallService(Room::SERVICE_ROOM_GET_ROOM_LIST, (param)&roomList);
	int size = roomList.vecRoomList.size();
	if(size > 0)
		m_lastRoomId = roomList.vecRoomList[size-1];
}

void CoreModule::OnConnectionClose()
{
	CacheLastRoom();

	AsynLogout();

	m_pModuleManager->PushMessage(
		MakeMessage<MODULE_ID_CORE>()(MESSAGE_VALUE_CORE_DISCONNECTED));

	m_pModuleManager->PushEvent(
		MakeEvent<MODULE_ID_CORE>()(EVENT_VALUE_CORE_DISCONNECTED_NOTIFY, 
		MODULE_ID_HALL, 0, 0, 0, NULL));
}

void CoreModule::AsynLogout()
{
	m_Connection.CleanCircleBuffer();

	m_pModuleManager->PushEvent(
		MakeEvent<MODULE_ID_CORE>()(EVENT_VALUE_CORE_ASYN_LOGOUT, 
		MODULE_ID_CORE, 0, 0, 0, NULL));
}

//////////////////////////////////////////////////////////////////////////

void CoreModule::Relogin()
{
	m_pModuleManager->PushMessage(
		MakeMessage<MODULE_ID_CORE>()(MESSAGE_VALUE_CORE_PRE_LOGIN));

	m_Connection.StartLogin(m_lastLoginRequest);
}

void CoreModule::ProcessLogin(param st)
{
	//memcpy(0,"hello",7);

	LOG_FILE_CACHE(L"ProcessLogin");

	ProcessLogout();

	m_pModuleManager->PushMessage(
		MakeMessage<MODULE_ID_CORE>()(MESSAGE_VALUE_CORE_PRE_LOGIN));

	m_lastLoginRequest = *(LoginEvent*)st;

	m_Connection.StartLogin(*(LoginEvent*)st);
}

void CoreModule::ProcessLogout()
{
	m_Connection.Logout();

	m_mapUIN2GiftStar.clear();
	m_map_MyGoods_Item2Count.clear();
}

void CoreModule::ProcessGetSelfInfo()
{
	m_Connection.SendPacket(&GetSelfInfoPacket());
}

void CoreModule::ProcessSetSelfInfo(param st)
{
	m_Connection.SendPacket(&SetSelfInfoPacket(*(core::SetSelfInfoEvent*)st));
}

void CoreModule::ProcessEnterRoom(param st)
{
	m_Connection.SendPacket(&EnterRoomPacket(*(core::EnterRoomEvent*)st));
}

void CoreModule::ProcessLeaveRoom(uint32 roomID)
{
	//memcpy(0,"hello",7);

	m_Connection.SendPacket(&LeaveRoomPacket(roomID));
}

void CoreModule::ProcessGetUserList(uint32 roomID)
{
	m_Connection.SendPacket(&GetUserListPacket(roomID));
}

void CoreModule::ProcessGetWaittingList(uint32 roomID)
{
	m_Connection.SendPacket(&GetWaittingListPacket(roomID));
}

void CoreModule::ProcessSendMessage(param st)
{
	m_Connection.SendPacket(&SendMessagePacket(*(core::SendMessageEvent*)st));
}

void CoreModule::ProcessSendGift(param st)
{
	m_Connection.SendPacket(&SendGiftPacket(*(core::SendGiftEvent*)st));
}

void CoreModule::ProcessSendLoudSpeaker(param st)
{
	m_Connection.SendPacket(&SendLoudSpeakerPacket(*(core::SendLoudSpeakerEvent*)st));
}

void CoreModule::ProcessRequestShow(Event const& _event)
{
	m_Connection.SendPacket(&RequestShowPacket(_event));
}

void CoreModule::ProcessExitShow(Event const& _event)
{
	m_Connection.SendPacket(&ExitShowPacket(_event));
}

void CoreModule::ProcessOperateAV(Event const& _event)
{
	m_Connection.SendPacket(&OperateAVPacket(_event));
}

void CoreModule::ProcessKickUser(param st)
{
	m_Connection.SendPacket(&KickUserPacket(*(core::KickUserEvent*)st));
}

void CoreModule::ProcessAddBlack(Event const& _event)
{
	m_Connection.SendPacket(&AddBlackPacket(_event));	
}

void CoreModule::ProcessLockIP(param st)
{
	m_Connection.SendPacket(&LockIPPacket(*(core::LockIPEvent*)st));
}

void CoreModule::ProcessShutUp(Event const& _event)
{
	m_Connection.SendPacket(&ShutUpPacket(_event));	
}

void CoreModule::ProcessUnShutUp(Event const& _event)
{
	m_Connection.SendPacket(&UnShutUpPacket(_event));	
}

void CoreModule::ProcessPushOnShow(Event const& _event)
{
	m_Connection.SendPacket(&PushOnShowPacket(_event));	
}

void CoreModule::ProcessPullOffShow(Event const& _event)
{
	m_Connection.SendPacket(&PullOffShowPacket(_event));	
}

void CoreModule::ProcessAddTitle(Event const& _event)
{
	m_Connection.SendPacket(&AddTitlePacket(_event));	
}

void CoreModule::ProcessRemoveTitle(Event const& _event)
{
	m_Connection.SendPacket(&RemoveTitlePacket(_event));
}

void CoreModule::ProcessOpenPrivate(Event const& _event)
{
	m_Connection.SendPacket(&OpenPrivatePacket(_event));
}

void CoreModule::ProcessClosePrivate(Event const& _event)
{
	m_Connection.SendPacket(&ClosePrivatePacket(_event));
}

void CoreModule::ProcessRequestP2P(Event const& _event)
{
	m_Connection.SendPacket(&RequestP2PPacket(_event));
}

void CoreModule::ProcessResponseP2P(Event const& _event)
{
	m_Connection.SendPacket(&ResponseP2PPacket(_event));
}

void CoreModule::ProcessReportClientInfo(param st)
{
	m_Connection.SendPacket(&ReportClientInfoPacket(*(core::ReportClientInfoEvent*)st));
}

void CoreModule::ProcessQueryClientInfo(param st)
{
	m_Connection.SendPacket(&QueryClientInfoPacket(*(core::QueryClientInfoEvent*)st));
}

void CoreModule::ProcessSetRoomInfo(param st)
{
	m_Connection.SendPacket(&SetRoomInfoPacket(*(core::SetRoomInfoEvent*)st));
}

void CoreModule::ProcessGetRoomInfo(Event const& _event)
{
	m_Connection.SendPacket(&GetRoomInfoPacket(_event));
}

void CoreModule::ProcessSetMicStatus(Event const& _event)
{
	m_Connection.SendPacket(&SetMicStatusPacket(_event));
}

void CoreModule::ProcessSendFlyWords(param st)
{
	m_Connection.SendPacket(&SendFlyWordsPacket(*(core::SendFlyWordsEvent*)st));
}

void CoreModule::ProcessReportCollectInfo(param st)
{
	m_Connection.SendPacket(&ReportCollectInfoPacket(*(core::ReportCollectInfoEvent*)st));
}

void CoreModule::ProcessAnswerPushOnShow(param st)
{
	m_Connection.SendPacket(&PushOnShowAnswerPacket(*(core::PushOnShowAnswerEvent*)st));
}

void CoreModule::ProcessGetTaskReward(Event const& _event)
{
	m_Connection.SendPacket(&GetTaskRewardPacket(_event));
}

void CoreModule::ProcessGetFans(Event const& _event)
{
	m_Connection.SendPacket(&GetFansPacket(_event));
}

void CoreModule::ProcessBuySofa(Event const& _event)
{
	m_Connection.SendPacket(&BuySofaPacket(_event));
}

void CoreModule::ProcessGetSofaList(Event const& _event)
{
	m_Connection.SendPacket(&GetSofaListPacket(_event));
}

void CoreModule::ProcessUpdateSonglist(Event const& _event)
{
	m_Connection.SendPacket(&UpdateSonglistPacket(*(core::UpdateSongListEvent*)_event.m_pstExtraInfo));
}

void CoreModule::ProcessGetSonglist(Event const& _event)
{
	m_Connection.SendPacket(&GetSonglistPacket(_event));
}

void CoreModule::ProcessOrderSong(Event const& _event)
{
	m_Connection.SendPacket(&OrderSongPacket(*(core::OrderSongEvent*)_event.m_pstExtraInfo));
}

void CoreModule::ProcessSongOrder(Event const& _event)
{
	m_Connection.SendPacket(&ProcessOrderPacket(_event));
}

void CoreModule::ProcessGetOrderlist(Event const& _event)
{
	m_Connection.SendPacket(&GetOrderlistPacket(_event));
}

void CoreModule::ProcessGetActorlist(Event const& _event)
{
	m_Connection.SendPacket(&GetOfflineActorPacket(_event));
}

void CoreModule::ProcessMarkSong(Event const& _event)
{
	m_Connection.SendPacket(&MarkSongPacket(_event));
}

//////////////////////////////////////////////////////////////////////////

int32 CoreModule::InvokeSwitchISP(param st)
{
	core::CurInfo& curInfo = CoreModule::m_curInfo;

	if((param)curInfo.bUseDianxin != st)
	{
		curInfo.bUseDianxin = st;

		m_pModuleManager->PushMessage(
			MakeMessage<MODULE_ID_CORE>()(MESSAGE_VALUE_CORE_ISP_CHANGE, st, 0, 0, 0));
	}

	return 0;
}

int32 CoreModule::InvokeGetCurInfo(param st)
{
	core::CurInfo* pCurInfo = (core::CurInfo*)st;
	if(pCurInfo != NULL)
	{
		*pCurInfo = CoreModule::m_curInfo;
		pCurInfo->unSystemTime = m_serverLoginTime+::GetTickCount()/1000-m_localLoginTime;
		return 0;
	}
	return -1;
}

int32 CoreModule::InvokeUpdateMoney(param st)
{
	CoreModule::m_curInfo.unLeftMoney += (int)st;
	return 0;
}

void CoreModule::OnGetGiftStar(std::map<uint32, std::vector<GiftStarInfo>>& mapGiftStar)
{
	//服务器已经排序好了
	m_mapUIN2GiftStar = mapGiftStar;	
}

void CoreModule::OnGetMyGoodsChange(bool bInit, std::map<uint16, std::pair<uint16, uint8>>& mapItem2Count)
{
	if(bInit)
	{
		m_map_MyGoods_Item2Count.clear();

		for(std::map<uint16, std::pair<uint16, uint8>>::iterator it = mapItem2Count.begin();
			it != mapItem2Count.end(); ++it)
		{
			m_map_MyGoods_Item2Count[it->first] = it->second.first;
		}
	}
	else
	{
		for(std::map<uint16, std::pair<uint16, uint8>>::iterator it = mapItem2Count.begin();
			it != mapItem2Count.end(); ++it)
		{
			int type = it->second.second;

			if(type == NotifyMyGoodsPacket::MY_GOODS_ADD || 
				type == NotifyMyGoodsPacket::MY_GOODS_DELETE )
			{
				std::map<uint16, uint16>::iterator it2 = m_map_MyGoods_Item2Count.find(it->first);
				
				if(it2 != m_map_MyGoods_Item2Count.end())
				{
					if(type == NotifyMyGoodsPacket::MY_GOODS_ADD)
					{
						it2->second += it->second.first;
					}
					else
					{
						it2->second -= it->second.first;

						if(it2->second <= 0)
							m_map_MyGoods_Item2Count.erase(it2);
					}
				}
				else if(type == NotifyMyGoodsPacket::MY_GOODS_ADD)
				{
					m_map_MyGoods_Item2Count[it->first] = it->second.first;
				}
				else
				{
					assert(0);
				}
			}
			else
			{
				assert(0);
			}
		}
	}	
}

int32 CoreModule::InvokeGetGiftStarInfo(param st)
{
	core::GetGiftStarInfo* pInfo = (core::GetGiftStarInfo*)st;
	std::map<uint32, std::vector<GiftStarInfo>>::iterator it = m_mapUIN2GiftStar.find(pInfo->unUIN);
	if(it != m_mapUIN2GiftStar.end())
	{
		pInfo->vecInfo = it->second;
	}
	return 0;
}

int32 CoreModule::InvokeGetMyGoods(param st)
{
	GetMyGoods* p = (GetMyGoods*)st;
	p->mapItem2Count = m_map_MyGoods_Item2Count;

	return 0;
}

//////////////////////////////////////////////////////////////////////////

void CoreModule::OnCycleTrigger()
{
	m_Connection.OnCycleTrigger();
}