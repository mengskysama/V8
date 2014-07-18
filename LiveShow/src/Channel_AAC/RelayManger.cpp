#include "StdAfx.h"
#include "RelayManger.h"
#include "TimeUtils.h"
#include "TimersManager.h"
#include "NetInterface.h"
#include "ThreadsManager.h"
#include "Buffers/BufferManager.h"
#include "ChannelManager.h"
#include "Setting.h"

RelayManger*       RelayManger::s_pInstance = NULL;
IModuleManager*	 RelayManger::s_pModuleManager = NULL;

SInt32           g_iP2PModuleRef        = 0;  //P2P模块的引用计数
SInt64           g_P2PLibSymbolTime     = 0;

void  RelayManger::Initialize(IModuleManager* pModuleManager)
{
	s_pModuleManager = pModuleManager;
}

void RelayManger::Initialize()
{
	if (0==g_iP2PModuleRef)
	{
		if (!s_pInstance)
		{
			s_pInstance = new RelayManger();
			g_P2PLibSymbolTime = TimeUtils::Milliseconds();
			Trace("P2PMoudle 启动时间 [%I64d] \n", g_P2PLibSymbolTime);
		}
	}

	++g_iP2PModuleRef;
}

RelayManger*  RelayManger::Instance()
{
	return s_pInstance;
}

void RelayManger::DestroyBufferInstance()
{
	ErrTrace("RelayManger::DestroyBufferInstance() begin\n");
	g_bufferManager.SetRelease(true);
	g_bufferManager.DestroyInstance();
	ErrTrace("RelayManger::DestroyBufferInstance() end\n");
}

void  RelayManger::DestroyInstance()
{
	--g_iP2PModuleRef;
	Assert(g_iP2PModuleRef>=0);
	if (0==g_iP2PModuleRef)
	{
		Trace("销毁P2PManager\n");
		SAFE_DELETE(s_pInstance);
		g_P2PLibSymbolTime = 0;
	}
	else
	{
		Assert(0);
	}
}

SInt64  RelayManger::GetLibSymbolTime()
{
	return g_P2PLibSymbolTime;
}

void  RelayManger::Config(UInt32 iMyUin, UInt32 iRelayIP, UInt16 sRelayPort, UInt32 iMaxConnTime)
{
	if (iMyUin!=g_Setting.getMyUin())
	{
		RCPtr<ChannelManager> pChannelMgr = ChannelManager::Instance();
		pChannelMgr->ClearChannels();
	}
	g_Setting.Config(iMyUin, iRelayIP, sRelayPort, iMaxConnTime); 
}


RelayManger::RelayManger()
{
	TimeUtils::Initialize();
	TimersManager::initialize();
	NetInterface::initialize();
	ThreadsManager::startThreads();
	ChannelManager::Initialize();
	g_bufferManager.Initialize();
}

RelayManger::~RelayManger()
{
	g_bufferManager.SetRelease(true);
	g_bufferManager.UnInitialize();
	ChannelManager::Instance()->Destroy();
	ThreadsManager::stopThreads();
	TimersManager::destroy();
	NetInterface::destroy();
	ChannelManager::DestroyInstance();
}

//注册一个逻辑通道 将逻辑通道注册在物理通道上
bool RelayManger::RegisterLogicChannel(SInt64 lSymbolTime, UInt32 iRoomId, UInt32 iFromUin,ILogicListener* pLogicListener, UInt8& cLogicID,bool bA)
{
	bool bRet=false;
	RCPtr<ChannelManager> pChannelMgr = ChannelManager::Instance();
	if (!pChannelMgr) return bRet;

	RCPtr<UdpChannel> pUdpChannel = pChannelMgr->FindUdpChannel(iRoomId);
	if (!pUdpChannel)
	{
		Trace("为[Room=%s]创建UDP通道",bA?"音频":"视频");
		pUdpChannel = pChannelMgr->CreateUdpChannel(iRoomId,s_pModuleManager);
	}
	
	
	if (pUdpChannel)
	{
		bRet = pUdpChannel->RegisterListener(pLogicListener, cLogicID, iRoomId, iFromUin, bA);
		if (bRet)
		{
			if ( CHANNEL_READY==pUdpChannel->GetStatus())
			{
				pUdpChannel->Start();
			}
		}
	}

	return bRet;
}

//注册UDP通道
bool RelayManger::RegisterUDPChannel(UInt32 iRoomId)
{
	RCPtr<ChannelManager> pChannelMgr = ChannelManager::Instance();
	if (!pChannelMgr) return false;
	RCPtr<UdpChannel> pUdpChannel = pChannelMgr->FindUdpChannel(iRoomId);
	if (!pUdpChannel)
	{
		Trace("为[Room=%d]创建UDP通道",iRoomId);
		pUdpChannel = pChannelMgr->CreateUdpChannel(iRoomId,s_pModuleManager);
	}
	if (pUdpChannel)
	{
		if ( CHANNEL_READY==pUdpChannel->GetStatus())
		{
			pUdpChannel->Start();
		}
	}
	return true;
}

//释放UDP通道
void RelayManger::UnregisterUDPChannel(UInt32 iRoomId,bool bFace)
{
	RCPtr<ChannelManager> pChannelMgr = ChannelManager::Instance();
	if (!pChannelMgr)
	{
		return;
	}

	RCPtr<UdpChannel> pUdpChannel = pChannelMgr->FindUdpChannel(iRoomId);
	if (pUdpChannel)
	{
		//当前已经没有不再使用通道
		if (!pUdpChannel->IsExistListener() || bFace) //bFace=true 强制退出  房间已经关闭的时候强制关闭UDP连接
		{
			Trace("RelayManger::UnregisterUDPChannel向对方发送Stop(REASON_NO_USE_FAIL)\n");
			pUdpChannel->Stop(REASON_NO_USE_FAIL);
			pChannelMgr->RemoveUdpChannel(iRoomId);
		}
	}
	else
	{
		Assert(0);
	}
}

//去除注册
void RelayManger::UnregisterLogicChannel(UInt32 iRoomId, UInt32 iFromUin, UInt8 cLogicID, bool bNotify)
{
	RCPtr<ChannelManager> pChannelMgr = ChannelManager::Instance();
	if (!pChannelMgr)
	{
		return;
	}

	RCPtr<UdpChannel> pUdpChannel = pChannelMgr->FindUdpChannel(iRoomId);
	if (pUdpChannel)
	{
		pUdpChannel->UnregisterListener(cLogicID, iRoomId, iFromUin, bNotify);

// 		//当前已经没有不再使用通道
// 		if (!pUdpChannel->IsExistListener())
// 		{
// 			Trace("RelayManger::UnregisterLogicChannel[clogicID = %d] 向对方发送Stop(REASON_NO_USE_FAIL)\n", cLogicID);
// 			pUdpChannel->Stop(REASON_NO_USE_FAIL);
// 			pChannelMgr->RemoveUdpChannel(iRoomId);
// 		}
	}
	else
	{
		Assert(0);
	}
}

 UInt16 RelayManger::QueryChannelStatus(UInt32 iRoomId)
 {
	 RCPtr<ChannelManager> pChannelMgr = ChannelManager::Instance();
	 if (!pChannelMgr)
	 {
		 return CHANNEL_NOEXIST;
	 }
 	UInt16 sStatus = CHANNEL_NOEXIST;
 	RCPtr<UdpChannel> pUdpChannel = pChannelMgr->FindUdpChannel(iRoomId);
 	if (pUdpChannel)
 	{
 		sStatus = pUdpChannel->GetStatus();
 	}
 
 	return sStatus;
 }

bool  RelayManger::IsUsingChannel(UInt32 iRoomId)
{
	bool bRet = false;
	RCPtr<ChannelManager> pChannelMgr = ChannelManager::Instance();
	if (!pChannelMgr) return false;

	RCPtr<UdpChannel> pUdpChannel = pChannelMgr->FindUdpChannel(iRoomId);
	if (pUdpChannel)
	{
		bRet = pUdpChannel->IsExistListener();
	}

	return bRet;
}

void  RelayManger::SendData (UInt32 iRoomId, UInt8* pData, UInt16 sDataLen,bool bA)
{
	RCPtr<ChannelManager> pChannelMgr = ChannelManager::Instance();
	if (!pChannelMgr)
	{
		return;
	}

	RCPtr<UdpChannel> pUdpChannel = pChannelMgr->FindUdpChannel(iRoomId);
	if (pUdpChannel)
	{
		pUdpChannel->SendData(pData, sDataLen,bA);
	}
	else
	{
		Assert(0);
	}
}

void  RelayManger::ProcessCmd(UInt32 iRoomId, UInt8* pCmd, UInt16 sCmdLen)
{
	RCPtr<ChannelManager> pChannelMgr = ChannelManager::Instance();
	if (!pChannelMgr)
	{
		return;
	}

	pChannelMgr->PutCmdPacket(iRoomId, pCmd, sCmdLen);
}

//修改中转服务器IP 端口
void RelayManger::ChangeRelayServer(UInt32 iRoomId,UInt32 iRelayIP, UInt16 sRelayPort)
{
	RCPtr<ChannelManager> pChannelMgr = ChannelManager::Instance();
	if (!pChannelMgr)
	{
		return;
	}

	RCPtr<UdpChannel> pUdpChannel = pChannelMgr->FindUdpChannel(iRoomId);
	if (pUdpChannel)
	{
		pUdpChannel->SetRelayIP(iRelayIP,sRelayPort);
	}
	else
	{
		Assert(0);
	}
}

void RelayManger::SetAVKey(UInt32 iRoomId,UInt32 iKey)
{
	g_Setting.setAVKey(iRoomId,iKey);
	CocTrace("设置AV iRoomId=%d, key=%d\n",iRoomId,iKey);
}

//获取上行音视频网络状态
void RelayManger::GetAVNetSendStatus(UInt32 iRoomId,UInt32& iAudioTotalPacket,UInt32& iLostAudioPacket, UInt32& iVideoTotalPacket, UInt32& iLostVideoPacket)
{
	RCPtr<ChannelManager> pChannelMgr = ChannelManager::Instance();
	if (!pChannelMgr)
	{
		return;
	}

	RCPtr<UdpChannel> pUdpChannel = pChannelMgr->FindUdpChannel(iRoomId);
	if (pUdpChannel)
	{
		iAudioTotalPacket = pUdpChannel->m_iSendAudioCount; 
		iLostAudioPacket  = pUdpChannel->m_iRequestAudioCount; 
		iVideoTotalPacket = pUdpChannel->m_iSendVideoCount;
		iLostVideoPacket  = pUdpChannel->m_iRequestVideoCount;
	}
	else
	{
		Assert(0);
	}
}


//获取上行丢包率
float RelayManger::GetLostRate()
{
	return g_Setting.m_iLostRate;
}