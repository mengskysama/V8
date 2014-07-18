#include "stdafx.h"
#include "UdpChannel.h"
#include "UdpSocketHandler.h"

#include "NetInterface.h"
#include "TimeUtils.h"
#include "MyAssert.h"
#include "Setting.h"
//#include "P2PManger.h"
#include "statisticdatadef.h"
#include "RelayAlivePacket.h"
#include "RelayPacket.h"
#include "TimersManager.h"
//#include "ChannelInterface.h"
//#include "ChannelModule.h"
#include <iostream>
#include <string.h>
#include <assert.h>
#include "Buffers/BufferManager.h"
#include "ChannelManager.h"


#define  DEFAULT_BEGIN_UDP_PORT                  10000
#define  MAX_WAIT_MSG_TIME                        3000
#define  MAX_WAIT_CMD_TIME                        3000
#define  SEND_CYCLE_TIME                          1000

#define  MAX_NO_ALIVE_TIME                       15000
#define  NETTEST_TIME							    10
#define  MAX_SEND_NETTEST_TIME					  5000
#define  MAX_GETIP_PORT_TIME					 15000
#define  MAX_RECV_NETTEST_ACK_TIME				MAX_SEND_NETTEST_TIME + 1000


UdpChannel::UdpChannel(UInt32 iMyUin, UInt32 iRoomId,IModuleManager *pModuleManager)
           : m_iSocketID(-1),
             m_iMyUin(iMyUin),
			 m_iRoomId(iRoomId),
			 m_iMyPrvIP(g_Setting.getMyIP()),
			 m_sMyPrvPort(DEFAULT_BEGIN_UDP_PORT),
			 m_iRelayIP(g_Setting.getRelayIP()),
			 m_sRelayPort(g_Setting.getRelayPort()),
			 m_iMaxConnTime(g_Setting.getMaxConnTime()),
			 m_lLastAliveSendTime(0),
			 m_lLastAliveRcvTime(0),
			 m_sRelaySeq(1),
			 m_cLogicSeed(0),
			 m_sStatus(CHANNEL_READY),
			 m_pModuleManager(pModuleManager),
			 m_lConnectBeginTime(0),//开始连接时间
			 m_bCreatingRelaySession(true),//正在创建中转连接Session
			 m_bLastValue(false), //是否设置连接成功时候的IP Port
			 m_iAliveIP(0), 
			 m_sAlivePort(0),
			 s_audio_sequence(0),
			 s_video_sequence(0),
			 m_bChangeServer(false),
			 m_keep_count(0),
			 m_iLastAudioReqSequence(0),
			 m_iRequestAudioCount(0),
			 m_iLastVideoReqSequence(0),
			 m_iRequestVideoCount(0),
			 m_iSendAudioCount(0),
			 m_iSendVideoCount(0),
			 m_dwLastRateTime(GetTickCount()),
			 m_iRequestAudioCountEx(0),
			 m_iRequestVideoCountEx(0),
			 m_iSendAudioCountEx(0),
			 m_iSendVideoCountEx(0)
{
	::srand((unsigned)time(NULL));
 	m_cLogicSeed = (UInt8) ::rand();
	if (m_cLogicSeed<10)  m_cLogicSeed += 10;//1-9为预留的逻辑ID
	Trace("UdpChannel: Constructor...time = %I64d \n", GetTickCount());
}

UdpChannel::~UdpChannel()
{
	if (m_iSocketID>=0)
	{
		NetInterface::unregisterSocket(m_iSocketID);
		m_iSocketID = -1;
	}
	Trace("UdpChannel: Destructor...time = %I64d \n", GetTickCount());
}

void UdpChannel::Start()
{
	Trace("UdpChannel: Start(),Cur status=%d \n",GetStatus());
	if (CHANNEL_READY==GetStatus())
	{
		StartListen(); //启动监听的端口
		SetStatus(CHANNEL_CONNECTING); //进入连接状态
		Trace("UdpChannel: Start(),status change into CHANNEL_CONNECTING time = %I64d \n", GetTickCount());
	}
}

void UdpChannel::StartListen()
{
	//随机选择Udp端口进行绑定
	::srand( (UInt32)::time( NULL ) );
	UdpSocketHandler* pUdpHandler = new UdpSocketHandler(m_iRoomId);

	unsigned int loop=0;
	do 
	{
		m_sMyPrvPort = DEFAULT_BEGIN_UDP_PORT;
		m_sMyPrvPort = m_sMyPrvPort + ::rand()%40000;

		m_iMyPrvIP = GetLocalIP("60.190.39.200");
		m_iSocketID = NetInterface::registerSocketHandler(m_iMyPrvIP, ::htons(m_sMyPrvPort), pUdpHandler);
	} 
	while( m_iSocketID<0 && ((loop++)<200) );

	Trace("UdpChannel: [MyUin= %d] 随机监听的端口 [ Port = %d ] \n", m_iMyUin, m_sMyPrvPort);
}

void UdpChannel::Stop(UInt8 cReason)
{
	Trace("UdpChannel::Stop [reason=%d]\n",cReason);
	if (SetStatus( CHANNEL_DESTROY ))
	{
	    Trace("UdpChannel:通知中转服务器destory,销毁连接 time = %I64d \n", GetTickCount());
	    SendToRelayServer(AV_CMD_DESTROY, ++m_sRelaySeq);

		//获取并清除所有的监听者
		{
			std::vector<RCPtr<WrapListener> > vAllListeners;
			std::map<UInt8, RCPtr<WrapListener> >::iterator iter;
			{
				MutexLocker locker(&m_ListenerMutex);//网络线程 房间线程  m_ListenersMap
				for (iter=m_ListenersMap.begin(); iter!=m_ListenersMap.end(); ++iter)
				{
					vAllListeners.push_back(iter->second);
				}
				m_ListenersMap.clear();
			
				//通知上层的监控者
				for (UInt32 i=0; i< vAllListeners.size(); i++)
				{
					vAllListeners[i]->onDisconnect(cReason);
				}
			}
		}
	}
}

bool UdpChannel::Maintain(SInt64 lCurTime)
{
	m_lCurTime = lCurTime;
	bool bRemove = false;

	switch(GetStatus()&CHANNEL_MASK) 
	{
	case CHANNEL_READY: 
		break;
	case CHANNEL_CONNECTING:
		{
			if (0==m_lConnectBeginTime) m_lConnectBeginTime = lCurTime;
			//if (lCurTime-m_lConnectBeginTime>2000)//进入房间2秒后连接中转服务器
			{
				Trace("主动方[MyUin=%d]，主动连接中转服务器  time = %I64d \n",m_iMyUin ,GetTickCount());
				SwitchToRelay(m_iRelayIP, m_sRelayPort);
			}
		}
		break;
	case CHANNEL_RELAYING:
// 		if (lCurTime-m_lConnectBeginTime<m_iMaxConnTime)  =========================haosl  调试  备用 日后长时间没有心跳包返回 提示上层处理
// 		{
			if (m_bCreatingRelaySession)
			{
				//尝试连接中转服务器
				if (lCurTime-m_lLastAliveSendTime>MAX_WAIT_MSG_TIME)
				{
					Trace("向中转服务器[%s:%d]发送RELAY_CREATE time = %I64d \n",NetInterface::IPUInt32ToString(m_iRelayIP).c_str(), m_sRelayPort, GetTickCount());
					if (lCurTime-m_lLastAliveSendTime>SEND_CYCLE_TIME*3)
					{
						for (int i=0; i<ALIVE_PACK_COUNT ; i++)
						{
							SendToRelayServer(AV_CMD_KEEPALIVE, ++m_sRelaySeq);
						}
					}
					m_lLastAliveSendTime = lCurTime;
				}
			}
			else
			{
				//收到Relay服务器的Relay_Success命令, 开始发送RelayKeepAlive
				if (lCurTime-m_lLastAliveSendTime>SEND_CYCLE_TIME)
				{
					Trace("尝试连接中转服务器[%s:%d]，发送AV_CMD_KEEPALIVE time = %I64d \n", NetInterface::IPUInt32ToString(m_iRelayIP).c_str(), m_sRelayPort, GetTickCount());
					for (UInt16 i=0; i<ALIVE_PACK_COUNT; i++)
					{
						SendToRelayServer(AV_CMD_KEEPALIVE, ++m_sRelaySeq);
					}
					m_lLastAliveSendTime = lCurTime;
					SetStatus(CHANNEL_SUCCESS);
				}
			}
//		}
// 		else
// 		{
// 			//最大连接时间内连接中转服务器失败
// 			Stop(REASON_RELAY_FAIL);
// 			bRemove = true;
// 			Trace("主动方[MyUin=%d]，主动连接中转服务器失败，REASON_RELAY_FAIL\n",m_iMyUin);
// 		}
		break;
	case CHANNEL_SUCCESS:
		if (lCurTime-m_lLastAliveSendTime>SEND_CYCLE_TIME*3) //&& lCurTime-m_lLastAliveRcvTime>SEND_CYCLE_TIME*3
		{
			Trace("--------------周期发送keep Alive,保持连接畅通！！！！ time = %I64d \n", GetTickCount());
			if (m_bChangeServer)
			{
				KeepAlive();
			}

			if (++m_keep_count%10 == 0 || m_bChangeServer)
			{
				Trace("--------------发送注册包！！！！ time = %I64d \n", GetTickCount());
				KeepAlive();
				m_bChangeServer = false;
			}
			else
			{
				Trace("--------------发送心跳维持包！！！！ time = %I64d \n", GetTickCount());
				KeepAliveEx();
			}
		}
		else
		{
			static unsigned int s_count = 0;
			s_count ++;
			//if (++s_count%50 == 0)
			//{
				Trace("+++++++++++++++++没有发送keep Alive,保持连接畅通！！！！ time = %I64d \n", GetTickCount());
			//}
		}

// 		if (lCurTime-m_lLastAliveRcvTime>MAX_NO_ALIVE_TIME) ==================================haosl调试   备用
// 		{
// 			Assert(0);
// 			Stop(REASON_ALIVE_TIMEOUT_FAIL);
// 			bRemove = true;
// 			Trace("RoomId[=%d]长时间没有收到Alive, 断开，REASON_RELAY_FAIL\n",m_iRoomId);
// 		}

		ResetRateCount();
		break;
	case CHANNEL_DESTROY:
		bRemove = true;
		break;
	default:
		Assert(0);
		break;
	}

	return bRemove;
}

//向中转服务器发包
void UdpChannel::SendToRelayServer(UInt16 cCmdType, UInt16 sSeq)
{
	Packet* pPacket = NULL;

	if (AV_CMD_ALIVE == cCmdType)
	{
		pPacket = new Packet(-1,0);
		UInt16 cmdType = htons(cCmdType);
		pPacket->setContent((UInt8*)&cmdType,2);
	}
	else
	{
		UInt32 iKey = g_Setting.getAVKey(m_iRoomId);
		AssertVS(iKey!=0,iKey,"iKey==0");
		pPacket = RelayAlivePacket::makePacket(-1, m_iMyUin, m_iRoomId,iKey);
		if (!pPacket)
		{
			Trace("UdpChannel::SendToRelayServer: 申请内存失败 time = %I64d\n",GetTickCount());
			return;
		}
		Trace("NetInterface::sendUDPPacket *****RoomId=%d,IP=%s,port=%d 注册包\n",m_iRoomId,NetInterface::IPUInt32ToString(m_iRelayIP).c_str(), m_sRelayPort);
	}

	pPacket->AddReference();
	NetInterface::sendUDPPacket(m_iSocketID, m_iRelayIP, ::htons(m_sRelayPort), pPacket);
	Trace("NetInterface::sendUDPPacket *****RoomId=%d,IP=%s,port=%d\n",m_iRoomId,NetInterface::IPUInt32ToString(m_iRelayIP).c_str(), m_sRelayPort);
}

//选择中转服务器IP和Port
void  UdpChannel::SwitchToRelay(UInt32 iRelayIP, UInt16 sRelayPort)
{
	if (GetStatus()!=CHANNEL_SUCCESS)
	{
		SetRelayIP(iRelayIP, sRelayPort);
		SetStatus(CHANNEL_RELAYING);
		m_lConnectBeginTime = TimeUtils::Milliseconds();
	}
}

//设置中转服务器IP和Port
void UdpChannel::SetRelayIP(UInt32 iRelayIP, UInt16 sRelayPort)
{
	m_iRelayIP   = iRelayIP;
	m_sRelayPort = sRelayPort;
	m_bChangeServer = true;
	
	Trace("UdpChannel::SetRelayIP *****RoomId=%d,IP=%s,port=%d\n",m_iRoomId,NetInterface::IPUInt32ToString(iRelayIP).c_str(), sRelayPort);
}

//维持心跳包
void UdpChannel::KeepAlive(UInt16 sNum/* =1 */, bool bAck)
{
	for (int i=0; i<sNum; ++i)
	{
		SendToRelayServer(AV_CMD_KEEPALIVE, ++m_sRelaySeq);
	}
	m_lLastAliveSendTime = TimeUtils::Milliseconds();
}

void UdpChannel::KeepAliveEx(UInt16 sNum/*=1*/)
{
	for (int i=0; i<sNum; ++i)
	{
		SendToRelayServer(AV_CMD_ALIVE, ++m_sRelaySeq);
	}
	m_lLastAliveSendTime = TimeUtils::Milliseconds();
}

void UdpChannel::SendData(UInt8* pData, UInt16 sDataLen,bool bA)
{
	if (CHANNEL_SUCCESS==GetStatus())
	{
		Packet* pDataPacket = NULL;
		//pDataPacket = RelayDataPacket::makePacket(-1, sDataLen, pData);
		static unsigned int s_data = 0;
		if (++s_data%500 == 0 )
		{
			ErrTrace("发送%s数据包。。。。time = %I64d \n",bA?"音频":"视频", GetTickCount());
		}
		
		unsigned int iSequence = bA?++s_audio_sequence:++s_video_sequence;

// 		static unsigned int icount = 0;
// 		if (bA && icount++ % 20==1) //+++++++++++++++++++++++++++++++++++++
// 			return;

		pDataPacket = RelayDataPacket::makePacket(-1,m_iMyUin,m_iRoomId,bA?AV_CMD_AUDIO:AV_CMD_VIDEO,iSequence,sDataLen,pData);
		if (!pDataPacket)
		{
			Trace("UdpChannel::SendData: 申请内存失败[len=%d] time = %I64d\n", sDataLen,GetTickCount());
			return;
		}

		pDataPacket->AddReference();
		g_bufferManager.AddInputPacket(m_iRoomId,pDataPacket,iSequence,bA);
		
		NetInterface::sendUDPPacket(m_iSocketID, m_iRelayIP, ::htons(m_sRelayPort), pDataPacket);
		//Trace("UdpChannel::SendData IP=%s ,port=%d, len=%d\n",NetInterface::IPUInt32ToString(m_iRelayIP).c_str(),ntohs(m_sRelayPort),sDataLen);

		bA?m_iSendAudioCount++:m_iSendVideoCount++;
		bA?m_iSendAudioCountEx++:m_iSendVideoCountEx++;
	}
	else
	{
		Assert(0);
		static unsigned int s_count = 0;
		if (s_count++%50 == 0){
			ErrTrace("UdpChannel not CHANNEL_SUCCESS status \n");
		}
	}
}

void UdpChannel::SendRepeatData(Packet* pRePacket,bool bA)
{
	if (CHANNEL_SUCCESS==GetStatus())
	{
		Packet* pDataPacket = NULL;
		static unsigned int s_data = 0;
		if (++s_data%10 == 0 )
		{
			Trace("SendRepeatData 送%s数据包。。。。time = %I64d \n",bA?"音频":"视频", GetTickCount());
		}
		pRePacket->AddReference();
		NetInterface::sendUDPPacket(m_iSocketID, m_iRelayIP, ::htons(m_sRelayPort), pRePacket);
	}
	else
	{
		Assert(0);
	}
}


//logic Listener,每一个应用一个Listener 注册一个逻辑通道
bool UdpChannel::RegisterListener(ILogicListener* pLogicListener, UInt8& cLogicID,UInt32 iRoomId,UInt32 iFromUin,bool bA)
{
	Trace("UdpChannel::RegisterListener..begin time = %I64d \n", GetTickCount());
	if (pLogicListener)
	{
		{
			MutexLocker locker(&m_ListenerMutex);//网络线程 房间线程  m_ListenersMap
			if (GetStatus()==CHANNEL_DESTROY)
			{
				Assert(0);
				return false;
			}

			if (0==cLogicID)
			{
				//新注册的监听者，需要产生新的LogicID
				int  iCount = 255;
				while (m_ListenersMap.find(m_cLogicSeed)!=m_ListenersMap.end())
				{
					--iCount;
					if( iCount<0)
					{
						Assert(0);
						return false;
					}
					m_cLogicSeed+=2;
					if (m_cLogicSeed<10) m_cLogicSeed += 10;
				}

				cLogicID              = m_cLogicSeed;
				m_cLogicSeed+=2;
				if (m_cLogicSeed<10) m_cLogicSeed += 10;
			}
			else
			{
				//使用对方产生的LogicID, 该ID应该是没有被用过的
				if(m_ListenersMap.find(cLogicID)!=m_ListenersMap.end())
				{
					Assert(0);
					return false;
				}
			}
			RCPtr<WrapListener> pWrapListener(new WrapListener(pLogicListener));
			m_ListenersMap.insert(listener_pair(cLogicID, pWrapListener));
			
			//映射处理
			if (bA) //语音
			{
				RCPtr<StructALogic> pLogic (new StructALogic());
				pLogic->cLogicId = cLogicID;
				pLogic->iFromUin=iFromUin;
				pLogic->iRoomId=iRoomId;
				m_ListenersRoomDestAMap.insert(listenerRoomDesA_pair(LogicIndex(iRoomId,iFromUin),pLogic));
			}
			else //视频
			{
				RCPtr<StructVLogic> pLogic (new StructVLogic());
				pLogic->cLogicId = cLogicID;
				pLogic->iFromUin=iFromUin;
				pLogic->iRoomId=iRoomId;
				m_ListenersRoomDestVMap.insert(listenerRoomDesV_pair(LogicIndex(iRoomId,iFromUin),pLogic));
			}

		}

		if (GetStatus()==CHANNEL_SUCCESS)
		{
			pLogicListener->onConnected(); //通知Listener
		}
		Trace("UdpChannel::RegisterListener success logicId=%d time = %I64d \n",cLogicID,GetTickCount());
		return true;
	}
	return false;
}

void  UdpChannel::UnregisterListener(UInt8 cLogicID, UInt32 iRoomId,UInt32 iFromUin, bool bCallBack)
{
	Trace("UdpChannel::UnregisterListener time = %I64d \n", GetTickCount());
	RCPtr<WrapListener> pWrapListener(NULL);
	{
		std::map<UInt8, RCPtr<WrapListener> >::iterator iter;
		MutexLocker locker(&m_ListenerMutex);//网络线程 房间线程  m_ListenersMap
		iter = m_ListenersMap.find(cLogicID);
		if (iter!=m_ListenersMap.end())
		{
			pWrapListener = iter->second;
			m_ListenersMap.erase(iter);
		}
	}
	
	//映射处理 取消注册
	RCPtr<StructALogic>  pALogic(NULL);
	{
		std::map<UInt64,RCPtr<StructALogic> >::iterator iter;
		iter = m_ListenersRoomDestAMap.find(LogicIndex(iRoomId,iFromUin));
		if (iter!=m_ListenersRoomDestAMap.end())
		{
			pALogic = iter->second;
			m_ListenersRoomDestAMap.erase(iter);
		}
	}

	RCPtr<StructVLogic>  pVLogic(NULL);
	{
		std::map<UInt64,RCPtr<StructVLogic> >::iterator iter;
		iter = m_ListenersRoomDestVMap.find(LogicIndex(iRoomId,iFromUin));
		if (iter!=m_ListenersRoomDestVMap.end())
		{
			pVLogic = iter->second;
			m_ListenersRoomDestVMap.erase(iter);
		}
	}

	if (pWrapListener)
	{
		pWrapListener->WaitUnshared();

		if (bCallBack)
		{
			Trace("UdpChannel::UnregisterListener[logicid = %d] onDisconnect time = %I64d\n", cLogicID,GetTickCount());
			pWrapListener->onDisconnect(REASON_CANCEL_FAIL);
		}
	}
}

RCPtr<WrapListener> UdpChannel::GetListener(UInt8 cLogicID,bool addRef)
{
	RCPtr<WrapListener> pWrapListener(NULL);
	{
		std::map<UInt8, RCPtr<WrapListener> >::iterator iter;
		MutexLocker locker(&m_ListenerMutex);//网络线程 房间线程  m_ListenersMap
		iter = m_ListenersMap.find(cLogicID);
		if (iter!=m_ListenersMap.end())
		{
			pWrapListener = iter->second;
			if (addRef) pWrapListener->AddReference();
		}
	}

	return pWrapListener;
}

RCPtr<StructVLogic> UdpChannel::GetListenerLogicV(UInt32 iRoomId,UInt32 iFromUin)
{
	RCPtr<StructVLogic> pLogic(NULL);
	{
		std::map<UInt64, RCPtr<StructVLogic> >::iterator iter;
		MutexLocker locker(&m_ListenerMutex);//网络线程 房间线程  m_ListenersMap
		iter = m_ListenersRoomDestVMap.find(LogicIndex(iRoomId,iFromUin));
		if (iter!=m_ListenersRoomDestVMap.end())
		{
			pLogic = iter->second;
		}
	}
	return pLogic;
}

RCPtr<StructALogic> UdpChannel::GetListenerLogicA(UInt32 iRoomId,UInt32 iFromUin)
{
	RCPtr<StructALogic> pLogic(NULL);
	{
		std::map<UInt64, RCPtr<StructALogic> >::iterator iter;
		MutexLocker locker(&m_ListenerMutex);//网络线程 房间线程  m_ListenersMap
		iter = m_ListenersRoomDestAMap.find(LogicIndex(iRoomId,iFromUin));
		if (iter!=m_ListenersRoomDestAMap.end())
		{
			pLogic = iter->second;
		}
	}
	return pLogic;
}

UInt64	UdpChannel::LogicIndex(UInt32 iRoomId,UInt32 iFromUin)
{
	UInt64 lIndex = iRoomId;
	return ((lIndex<<32)+iFromUin);
}

void  UdpChannel::GetAllListener(std::vector<RCPtr<WrapListener> >& vAllListeners)
{
	std::map<UInt8, RCPtr<WrapListener> >::iterator iter;
	MutexLocker locker(&m_ListenerMutex);//网络线程 房间线程  m_ListenersMap
	for (iter=m_ListenersMap.begin(); iter!=m_ListenersMap.end(); ++iter)
	{
		vAllListeners.push_back(iter->second);
	}
}

bool UdpChannel::IsExistListener()
{
	MutexLocker locker(&m_ListenerMutex);//网络线程 房间线程  m_ListenersMap
	return m_ListenersMap.empty() ? false : true;
}

bool UdpChannel::IsExistsListener(UInt32 iRoomId,UInt32 iFromUin,bool bA)
{
	std::map<UInt64,RCPtr<StructALogic> >::iterator iterA;
	std::map<UInt64,RCPtr<StructVLogic> >::iterator iterV;
	if (bA)
	{
		iterA = m_ListenersRoomDestAMap.find(LogicIndex(iRoomId,iFromUin));
		return iterA!=m_ListenersRoomDestAMap.end();
	}
	else
	{
		iterV = m_ListenersRoomDestVMap.find(LogicIndex(iRoomId,iFromUin));
		return iterV!=m_ListenersRoomDestVMap.end();
	}
}

bool UdpChannel::SetStatus(UInt16 sStatus)
{

	MutexLocker locker(&m_Mutex); // 防止同时掉用GetAllListener函数，可能会死锁                                

	if (sStatus==m_sStatus) return false;
	if (   (CHANNEL_SUCCESS!=m_sStatus)
		|| ( (CHANNEL_SUCCESS==m_sStatus)&&(sStatus>CHANNEL_SUCCESS) ) )
	{
		m_sStatus          = sStatus;
	}

	if (CHANNEL_DESTROY!=sStatus)//向监听者上报状态
	{
		std::vector<RCPtr<WrapListener> > vAllListeners;
		GetAllListener(vAllListeners);
		for (UInt32 i=0; i< vAllListeners.size(); i++)
		{
			//如果连接成功，调用onConnect
			if (CHANNEL_SUCCESS==sStatus)
			{
				vAllListeners[i]->onConnected();
			}
			else
			{
				vAllListeners[i]->onStatus(sStatus);
			}
		}
		vAllListeners.clear();
	}

	return true;
}

void UdpChannel::SetRelayIPAddr()
{
	m_iRelayIP		= g_Setting.getRelayIP();
	m_sRelayPort	= g_Setting.getRelayPort();
}

void UdpChannel::SetAliveIPAddr(UInt32 iAliveIP, UInt16 sAlivePort, bool bLastValue)
{
	if (!m_bLastValue)
	{
		m_iAliveIP    = iAliveIP;
		m_sAlivePort  = sAlivePort;
		m_bLastValue  = bLastValue;
		Trace("设置链接建立成功时候的[IP=%s, port=%d]\n",NetInterface::IPUInt32ToString(m_iAliveIP).c_str(),m_sAlivePort);
	}
}

void UdpChannel::FinishCreatingRelaySession()
{
	if (m_bCreatingRelaySession) 
	{
		m_bCreatingRelaySession = false;
		Trace("CreatingRelaySession Success!!!");
	}
		
}

void UdpChannel::PlayAudioPacket(UInt32 iRoomId,UInt32 iFromUin,Packet* pPacket,unsigned int iSeq)
{
	RCPtr<ChannelManager> pChannelMgr = ChannelManager::Instance();
	if (!pChannelMgr)
	{
		Assert(0);
		return;
	}
	RCPtr<UdpChannel> pUdpChannel = pChannelMgr->FindUdpChannel(iRoomId);
	if (pUdpChannel)
	{
		RCPtr<StructALogic> pLogic = pUdpChannel->GetListenerLogicA(iRoomId,iFromUin);
		if (pLogic)
		{
			RCPtr<WrapListener> pListener = pUdpChannel->GetListener(pLogic->cLogicId,true);
			if (pListener)
			{
				try
				{
					pListener->onReceive(pPacket->getPacketContent(),pPacket->getPacketLen(),iSeq);
					pListener->RemoveReference();
				}
				catch (...)
				{
					ErrTrace("UdpChannel::PlayAudioPacket Err\n");
				}
			}
			else
			{
				AssertV(0,iFromUin);
			}
		}
		else
		{
			AssertV(0,iRoomId);
		}
	}
	else
	{
		AssertV(0,iRoomId);
	}
}

void UdpChannel::PlayVideoPacket(UInt32 iRoomId,UInt32 iFromUin,Packet* pPacket)
{
	RCPtr<ChannelManager> pChannelMgr = ChannelManager::Instance();
	if (!pChannelMgr)
	{
		Assert(0);
		return;
	}
	RCPtr<UdpChannel> pUdpChannel = pChannelMgr->FindUdpChannel(iRoomId);
	if (pUdpChannel)
	{
		RCPtr<StructVLogic> pLogic = pUdpChannel->GetListenerLogicV(iRoomId,iFromUin);
		if (pLogic)
		{
			RCPtr<WrapListener> pListener = pUdpChannel->GetListener(pLogic->cLogicId,true);
			if (pListener)
			{
				try
				{
					pListener->onReceive(pPacket->getPacketContent(),pPacket->getPacketLen(),0);
					pListener->RemoveReference();
				}
				catch (...)
				{
					AssertVS(0,iFromUin,"UdpChannel::PlayVideoPacket Err\n");
				}
			}
			else
			{
				AssertV(0,iFromUin);
			}
		}
		else
		{
			AssertV(0,iRoomId);
		}
	}
	else
	{
		AssertV(0,iRoomId);
	}
}

void  UdpChannel::RequestCompenPacket(UInt32 iRoomId,UInt32 iFromUin,Packet* pPakcet)
{
	RCPtr<ChannelManager> pChannelMgr = ChannelManager::Instance();
	if (!pChannelMgr)
	{
		pPakcet->RemoveReference();
		AssertV(0,iRoomId);
		return;
	}
	RCPtr<UdpChannel> pUdpChannel = pChannelMgr->FindUdpChannel(iRoomId);
	if (pUdpChannel)
	{
		pUdpChannel->SendPacket(pPakcet);
	}
	else
	{
		pPakcet->RemoveReference();
		AssertV(0,iRoomId);
	}
}

void UdpChannel::SendPacket(Packet* pPakcet)
{
	NetInterface::sendUDPPacket(m_iSocketID, m_iRelayIP, ::htons(m_sRelayPort), pPakcet);
}

void UdpChannel::ResetRateCount()
{
	if (GetTickCount() - m_dwLastRateTime > 1000*60*5) //5分钟统计一次
	{
		if (m_iSendAudioCountEx>0 || m_iSendVideoCountEx>0)
		{
			g_Setting.m_iLostRate = ((float)(m_iRequestAudioCountEx+m_iRequestVideoCountEx)) /(m_iSendAudioCountEx+m_iSendVideoCountEx+m_iRequestAudioCountEx+m_iRequestVideoCountEx);
		}

		m_iRequestAudioCountEx = 0;
		m_iRequestVideoCountEx = 0;
		m_iSendAudioCountEx = 0;
		m_iSendVideoCountEx = 0;

		m_dwLastRateTime = GetTickCount();
	}
}