#include "stdafx.h"
#include "AVChannel.h"
#include "RelayManger.h"
#include "AVModule.h"
#include "AV.h"

AVChannel::AVChannel(UInt32 iRoomId,UInt32 iFromUin, UInt8 cLogicID,bool bA)
          :m_iRoomId(iRoomId),
		   m_iFromUin(iFromUin),
		   m_cLogicID(cLogicID),
		   m_bNeedDestory(false),
		   m_bUserCancel(false),
		   m_bA(bA)
{

}

AVChannel::~AVChannel()
{
}

bool  AVChannel::Start(UInt64 lSymbolTime)
{

	RelayManger* pRelayMgr = RelayManger::Instance();
	bool bRet = pRelayMgr->RegisterLogicChannel(lSymbolTime, m_iRoomId, m_iFromUin, this, m_cLogicID, m_bA);
	if (bRet)
	{
		StartSelfAV();
	}

	return bRet;
}

void  AVChannel::Stop(bool bRemoveListener)
{
	if (!m_bNeedDestory)
	{
		if (bRemoveListener)
		{
			RelayManger* pRelayMgr = RelayManger::Instance();
			pRelayMgr->UnregisterLogicChannel(m_iRoomId, m_iFromUin, m_cLogicID);
		}

		m_bNeedDestory = true;
	}
}

void  AVChannel::UserCanncel()
{
	StopSelfAV();
	m_bUserCancel = true;
	ErrTrace("AVChannel::UserCanncel() 停止接收网络数据包..m_bUserCancel = true...\n");
}

bool  AVChannel::Maintain()
{
	if (m_bUserCancel)
	{
		Stop();
	}

	if (m_bNeedDestory)
	{
		return false;
	}

	onTimer();

	return true;
}

void  AVChannel::onConnected()
{
//连接建立成功的一些提示信息      ======haosl1
#ifndef _AV_REMOVE_NOTIFY
	AV_Connect_Start_Event* pStartEvent = new AV_Connect_Start_Event(m_iRoomId,m_iFromUin, m_cLogicID);
	if (!g_pAVModule->sendEvent(static_cast<void*>(pStartEvent), EVENT_AV_CONN_START))
	{
		Assert(0);
		delete pStartEvent;
	}
#endif
}

void  AVChannel::onDisconnect(UInt8 cReason)
{
#ifdef _DEBUG
    OutputDebugStr(L"AVChannel onDisconnect 与中转服务器连接断开\r\n");
#endif
	StopSelfAV();

	Stop(false);

	//连接断开时候的一些提示信息 ===haosl1
#ifndef _AV_REMOVE_NOTIFY
	AV_Connect_Stop_Event* pStopEvent = 
		new AV_Connect_Stop_Event(m_iRoomId, m_iFromUin, (REASON_CANCEL_FAIL==cReason) ? kReason_User_Cancel : kReason_Net_Error);
	if(!g_pAVModule->sendEvent(static_cast<void*>(pStopEvent), EVENT_AV_CONN_STOP))
	{
		Assert(0);
		delete pStopEvent;
	}
#endif
}

void  AVChannel::onStatus(UInt16 sStatus)
{
	//调试 UDP连接中转费服务器状态改变的一些信息
}