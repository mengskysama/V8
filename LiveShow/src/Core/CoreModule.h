#pragma once

#include "51ktv.h"
#include "imodule.h"
#include "Connection.h"
#include "coredefine.h"

using namespace core;

class CoreModule : public IModule
{
public:

	CoreModule(void);
	~CoreModule(void);

	virtual void Load(IModuleManager *);
	virtual void Unload();
	virtual void ProcessEvent(Event const&);
	virtual void ProcessMessage(Message const&);
	virtual int32 Invoke(ServiceValue const serviceValue, param);

	void OnLoginSuccess();
	void OnConnectionClose();

	void OnGetGiftStar(std::map<uint32, std::vector<GiftStarInfo>>& mapGiftStar);
	void OnGetMyGoodsChange(bool bInit, std::map<uint16, std::pair<uint16, uint8>>& mapItem2Count);

	IModuleManager*& GetModuleManager()
	{
		return m_pModuleManager;
	}

	static core::CurInfo& GetCurInfo()
	{
		return m_curInfo;
	}

	//(在m_circleBuffer.Read()的调用栈中)异步Logout
	void AsynLogout();

private:

	void OnCycleTrigger();
	void ProcessLogin(param st);
	void ProcessLogout();
	void ProcessGetSelfInfo();	
	void ProcessSetSelfInfo(param st);
	void ProcessEnterRoom(param st);
	void ProcessLeaveRoom(uint32 roomID);
	void ProcessGetUserList(uint32 roomID);
	void ProcessGetWaittingList(uint32 roomID);
	void ProcessSendMessage(param st);
	void ProcessSendGift(param st);
	void ProcessSendLoudSpeaker(param st);
	void ProcessRequestShow(Event const& _event);
	void ProcessExitShow(Event const& _event);
	void ProcessOperateAV(Event const& _event);
	void ProcessKickUser(param st);
	void ProcessAddBlack(Event const& _event);
	void ProcessLockIP(param st);
	void ProcessShutUp(Event const& _event);
	void ProcessUnShutUp(Event const& _event);
	void ProcessPushOnShow(Event const& _event);
	void ProcessPullOffShow(Event const& _event);
	void ProcessAddTitle(Event const& _event);
	void ProcessRemoveTitle(Event const& _event);
	void ProcessOpenPrivate(Event const& _event);
	void ProcessClosePrivate(Event const& _event);
	void ProcessRequestP2P(Event const& _event);
	void ProcessResponseP2P(Event const& _event);
	void ProcessReportClientInfo(param st);
	void ProcessQueryClientInfo(param st);
	void ProcessSetRoomInfo(param st);
	void ProcessGetRoomInfo(Event const& _event);
	void ProcessSetMicStatus(Event const& _event);
	void ProcessSendFlyWords(param st);
	void ProcessReportCollectInfo(param st);
	void ProcessAnswerPushOnShow(param st);
	void ProcessGetTaskReward(Event const& _event);
	void ProcessGetFans(Event const& _event);
	void ProcessBuySofa(Event const& _event);
	void ProcessGetSofaList(Event const& _event);
	void ProcessUpdateSonglist(Event const& _event);
	void ProcessGetSonglist(Event const& _event);
	void ProcessOrderSong(Event const& _event);
	void ProcessSongOrder(Event const& _event);
	void ProcessGetOrderlist(Event const& _event);
	void ProcessGetActorlist(Event const& _event);
	void ProcessMarkSong(Event const& _event);
	
	int32 InvokeSwitchISP(param st);
	int32 InvokeGetCurInfo(param st);
	int32 InvokeUpdateMoney(param st);
	int32 InvokeGetGiftStarInfo(param st);
	int32 InvokeGetMyGoods(param st);
	
	void CacheLastRoom();

	//自动重连
	void Relogin();

private:

	IModuleManager* m_pModuleManager;
	Connection m_Connection;

	uint32 m_serverLoginTime;//登陆时服务器时间
	uint32 m_localLoginTime;//登陆时本机时间(开机启动时间)
	static core::CurInfo m_curInfo;

	LoginEvent m_lastLoginRequest;//上一次登录请求
	uint32 m_lastRoomId;//最后一次进房id(用作自动重连)

	std::map<uint32, std::vector<GiftStarInfo>> m_mapUIN2GiftStar;
	std::map<uint16, uint16> m_map_MyGoods_Item2Count;
};
