#pragma once
#ifdef _DEBUG
//#include <vld.h>
#endif
#include <map>
#include <vector>
#include "UdpChannel.h"
#include "Mutex.h"
#include "CircleBuffer.h"

//全局类, 用于对全部ChannelUDP的管理

class ChannelManager : public RCObject
{
public:

 	static void                    Initialize();
 	static RCPtr<ChannelManager>   Instance();
 	static void                    DestroyInstance();

	ChannelManager();
	~ChannelManager();

 	RCPtr<UdpChannel>        CreateUdpChannel(UInt32 iRoomId,IModuleManager *pModuleManager);

 	RCPtr<UdpChannel>        FindUdpChannel(UInt32 iRoomId);
 	void                     RemoveUdpChannel(UInt32 iRoomId);
	void                     GetAllChannel(std::vector<RCPtr<UdpChannel> >& vAllChannels);
 	void                     ClearChannels();
	void                     Maintain();
 	void                     StartMaintainTimer();
	void					 StopMaintainTimer();
 	void                     Destroy();
 	void                     PutCmdPacket(UInt32 iRoomId, UInt8* pCmd, UInt32 iCmdLen);
 	void                     ProcessCmdPacket();

private:

	typedef std::pair<UInt64, RCPtr<UdpChannel> > channel_pair;

	Mutex                                  m_Mutex; //m_ChannelMap 主线程 网络线程
	std::map<UInt64, RCPtr<UdpChannel> >   m_ChannelMap;                 //标识是对方的Uin
	UInt32                                 m_iChannelMaintainTimeID;     //定时器ID
	CircleBuffer<Packet*>                  m_CmdPacketCircleBuffer;

	static RCPtr<ChannelManager>           s_pInstance;
};