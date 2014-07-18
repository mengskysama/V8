#include "StdAfx.h"
#include "RC.h"
#include "UdpSocketHandler.h"
#include "ChannelManager.h"
#include "TimeUtils.h"
#include "NetInterface.h"
#include "Buffers\BufferManager.h"
#include "RelayAlivePacket.h"
#include "RelayPacket.h"
#include <assert.h>

//处理通道上所有的包
void  UdpSocketHandler::onPacketRcved(Packet* pPacket)
{
	Trace("UdpSocketHandler::onPacketRcved begin ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
	RCPtr<ChannelManager> pChannelMgr = ChannelManager::Instance();
	if (!pChannelMgr)
	{
		Assert(0);
		return;
	}

	UInt32 iRemoteIP = 0;
	UInt16 sRemotePort = 0;
	pPacket->getRemoteAddr(iRemoteIP, sRemotePort);

	RCPtr<UdpChannel> pUdpChannel = pChannelMgr->FindUdpChannel(m_iRoomId);
	if (pUdpChannel)
	{
		Trace("中转服务器IP=%d  收到数据包IP=%d Port=%d\n",pUdpChannel->GetRelayIP(),iRemoteIP,ntohs(sRemotePort));
		if (pUdpChannel->GetRelayIP()==iRemoteIP) //pUdpChannel->GetRelayPort()==ntohs(sRemotePort)&&
		{
			processAllRelay(pPacket); //中转过来的数据包
		}
		else
		{
			Trace("收到非中转服务器数据包 ip=%s, port=%d\n",NetInterface::IPUInt32ToString(iRemoteIP).c_str(),ntohs(sRemotePort));
			Assert(0); //不是本中转服务器数据 哪儿来的？ 
		}
	}
}

//处理从中转服务器过来的包
void  UdpSocketHandler::processAllRelay(Packet* pPacket)
{
	RCPtr<ChannelManager> pChannelMgr = ChannelManager::Instance();
	if (!pChannelMgr)
	{
		Assert(0);
		return;
	}
	
	//Trace("收到中转服务器过来的数据包 cmd = %d fromUin = %d, roomid = %d \n", dataPacket.getCmdType() ,dataPacket.getFromUin(), dataPacket.getRoomId());
	RCPtr<UdpChannel> pUdpChannel = pChannelMgr->FindUdpChannel(m_iRoomId);
	if (pPacket->getPacketLen() == 2)
	{
		//收到心跳维持包
		if ( pPacket->readUInt16()== AV_CMD_ALIVE)
		{
			Trace("UdpChannel: 从中转服务器接收到 维持 心跳包 AV_CMD_ALIVE time = %I64d \n",GetTickCount());
			if(pUdpChannel)
				pUdpChannel->SetLastAliveRcvTime(TimeUtils::Milliseconds());
		}
		return;
	}


	RelayDataPacket dataPacket;
	if (!dataPacket.parserPacketToSelf(pPacket))
	{
		Assert(0);
		return;
	}
	
	Trace("收到中转服务器过来的数据包 cmd = %d fromUin = %d, roomid = %d \n", dataPacket.getCmdType() ,dataPacket.getFromUin(), dataPacket.getRoomId());
	//RCPtr<UdpChannel> pUdpChannel = pChannelMgr->FindUdpChannel(m_iRoomId);
	if (pUdpChannel)
	{
		Packet * pData = NULL;
		static unsigned int s_count = 0;
		static unsigned int s_audio = 0;
		static unsigned int s_video = 0;
		switch(dataPacket.getCmdType())
		{
		case AV_CMD_KEEPALIVE:
			s_count++;
			//if (++s_count%2 == 0)
			{
				Trace("UdpChannel: 从中转服务器接收到 注册 心跳包 AV_CMD_KEEPALIVE time = %I64d \n",GetTickCount());
			}
			
			pUdpChannel->FinishCreatingRelaySession();
			pUdpChannel->SetLastAliveRcvTime(TimeUtils::Milliseconds());

			if (CHANNEL_RELAYING==pUdpChannel->GetStatus())
			{
				UInt32 iRemoteIP;
				UInt16 sRemotePort;
				pPacket->getRemoteAddr(iRemoteIP, sRemotePort);
				pUdpChannel->SetAliveIPAddr(iRemoteIP, ::ntohs(sRemotePort), true);
				ErrTrace("收到中转服务器 AV_CMD_KEEPALIVE 心跳包\n");

//收到心跳包不回复
// 				for (int i=0; i<ALIVE_PACK_COUNT; ++i)
// 				{
// 					pUdpChannel->SendToRelayServer(AV_CMD_KEEPALIVE, 0);
// 				}
// 				pUdpChannel->SetStatus(CHANNEL_SUCCESS);//本处不发 由UDP Channel发送
			}

			
			break;
		case AV_CMD_AUDIO:
			{
				if (++s_audio%25 == 0)
				{
					Trace("UdpChannel: 从中转服务器接收到 语音数据包 AV_CMD_AUDIO time = %I64d ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n",GetTickCount());
				}

				//如果收到数据包，也认为通道建立成功
				if (pUdpChannel->GetStatus()==CHANNEL_RELAYING) 
				{    
					ErrTrace("收到中转服务器音频包 AV_CMD_KEEPALIVE CHANNEL_SUCCESS\n");
					UInt32 iRemoteIP;
					UInt16 sRemotePort;
					pPacket->getRemoteAddr(iRemoteIP, sRemotePort);
					pUdpChannel->SetAliveIPAddr(iRemoteIP, ::ntohs(sRemotePort), true);

					for (int i=0; i<ALIVE_PACK_COUNT; ++i)
					{
						pUdpChannel->SendToRelayServer(AV_CMD_KEEPALIVE, 0);
					}
					pUdpChannel->SetStatus(CHANNEL_SUCCESS);
				}

				pUdpChannel->SetLastAliveRcvTime(TimeUtils::Milliseconds());

				Trace("UdpChannel: 从中转服务器接收到AV_CMD_AUDIO [CmdType = %d RoomId = %d]\n", dataPacket.getCmdType(),dataPacket.getRoomId());
// 				RCPtr<StructALogic> pLogic = pUdpChannel->GetListenerLogicA(dataPacket.getRoomId(),dataPacket.getFromUin());
// 				if (pLogic)
// 				{
// 					RCPtr<WrapListener> pListener = pUdpChannel->GetListener(pLogic->cLogicId);
// 					if (pListener)
// 					{
						Packet* pDataPacket = RelayDataPacket::makePacket(-1,dataPacket.getDataLen(),dataPacket.getData());
						if (!pDataPacket)
						{
							Assert(0);
							return;
						}
						
						if (::IsBadWritePtr(pDataPacket->getPacketContent(),pDataPacket->getPacketLen()) != 0)
						{
							Assert(0);
						}

						g_bufferManager.AddOutputPacket(dataPacket.getRoomId(),dataPacket.getFromUin(),pDataPacket,dataPacket.getSequence(),TRUE);
						//pListener->onReceive(dataPacket.getData(), dataPacket.getDataLen());
// 					}
// 					else
// 					{
// 						AssertV(0, dataPacket.getRoomId());
// 					}
// 				}
// 				else
// 				{
// 					AssertV(0, dataPacket.getRoomId());
// 				}
			}
			break;

		case AV_CMD_VIDEO:
			{
				if (++s_video%10 == 0)
				{
					Trace("UdpChannel: 从中转服务器接收到 视频数据包 AV_CMD_VIDEO time = %I64d ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n",GetTickCount());
				}
				

				//如果收到数据包，也认为通道建立成功
				if (pUdpChannel->GetStatus()==CHANNEL_RELAYING) 
				{    
					ErrTrace("收到中转服务器视频包 AV_CMD_KEEPALIVE CHANNEL_SUCCESS\n");
					UInt32 iRemoteIP;
					UInt16 sRemotePort;
					pPacket->getRemoteAddr(iRemoteIP, sRemotePort);
					pUdpChannel->SetAliveIPAddr(iRemoteIP, ::ntohs(sRemotePort), true);

					for (int i=0; i<ALIVE_PACK_COUNT; ++i)
					{
						pUdpChannel->SendToRelayServer(AV_CMD_KEEPALIVE, 0);
					}
					pUdpChannel->SetStatus(CHANNEL_SUCCESS);
				}

				pUdpChannel->SetLastAliveRcvTime(TimeUtils::Milliseconds());
				
				
				Trace("UdpChannel: 从中转服务器接收到AV_CMD_VIDEO [CmdType = %d RoomId = %d FromUin=%d DateLen=%d]\n", dataPacket.getCmdType(),dataPacket.getRoomId(),dataPacket.getFromUin(),dataPacket.getDataLen());
// 				RCPtr<StructVLogic> pLogic = pUdpChannel->GetListenerLogicV(dataPacket.getRoomId(),dataPacket.getFromUin());
// 				if (pLogic)
// 				{
// 					RCPtr<WrapListener> pListener = pUdpChannel->GetListener(pLogic->cLogicId);
// 					if (pListener)
// 					{
						Packet* pDataPacket = RelayDataPacket::makePacket(-1,dataPacket.getDataLen(),dataPacket.getData());
						if (!pDataPacket)
						{
							Assert(0);
							return;
						}

						if (::IsBadWritePtr(pDataPacket->getPacketContent(),pDataPacket->getPacketLen()) != 0)
						{
							Assert(0);
						}

						g_bufferManager.AddOutputPacket(dataPacket.getRoomId(),dataPacket.getFromUin(),pDataPacket,dataPacket.getSequence(),FALSE);
						//pListener->onReceive(dataPacket.getData(), dataPacket.getDataLen());
// 					}
// 					else
// 					{
// 						AssertV(0, dataPacket.getRoomId());
// 					}
// 				}
// 				else
// 				{
// 					AssertV(0, dataPacket.getRoomId());
// 				}
			}
			break;
		case AV_CMD_GET_AUDIO: //获取音频补偿包
			if (pUdpChannel->GetMyUin() == dataPacket.getFromUin()) //是自己
			{
				LostTrace("[RoomId:%12ld] [MyUin:%12ld] [Audio-SequenceId:%12ld] [time:%12ld] [-----Get Request-----]  %s\n",m_iRoomId,dataPacket.getFromUin(),dataPacket.getSequence(),GetTickCount(),"请求补偿数据包");
				if (g_bufferManager.GetInputPacket(dataPacket.getRoomId(),pData,dataPacket.getSequence(),TRUE) && pData)
				{
					LostTrace("[RoomId:%12ld] [MyUin:%12ld] [Audio-SequenceId:%12ld] [time:%12ld] [-----Get Request Success-----]  %s\n",m_iRoomId,dataPacket.getFromUin(),dataPacket.getSequence(),GetTickCount(),"请求补偿数据包成功");
					pUdpChannel->SendRepeatData(pData,TRUE);
					
					if (pUdpChannel->m_iLastAudioReqSequence != dataPacket.getSequence())
					{
						pUdpChannel->m_iRequestAudioCount++;
						pUdpChannel->m_iRequestAudioCountEx++;
						pUdpChannel->m_iLastAudioReqSequence = dataPacket.getSequence();
					}
				}
			}
			else
			{
				//收到音频补偿包
				Trace("收到有问题的音频请求补偿包。。。。。。。。\n");
				Assert(0);
			}
			break;
		case AV_CMD_GET_VIDEO://获取视频补偿包
			if (pUdpChannel->GetMyUin() == dataPacket.getFromUin()) //是自己
			{
				LostTrace("[RoomId:%12ld] [MyUin:%12ld] [Video-SequenceId:%12ld] [time:%12ld] [-----Get Request-----]  %s\n",m_iRoomId,dataPacket.getFromUin(),dataPacket.getSequence(),GetTickCount(),"请求补偿数据包");
				if (g_bufferManager.GetInputPacket(dataPacket.getRoomId(),pData,dataPacket.getSequence(),FALSE) && pData)
				{
					LostTrace("[RoomId:%12ld] [MyUin:%12ld] [Video-SequenceId:%12ld] [time:%12ld] [-----Get Request Success-----]  %s\n",m_iRoomId,dataPacket.getFromUin(),dataPacket.getSequence(),GetTickCount(),"请求补偿数据包成功");
					pUdpChannel->SendRepeatData(pData,TRUE);

					if (pUdpChannel->m_iLastVideoReqSequence != dataPacket.getSequence())
					{
						pUdpChannel->m_iRequestVideoCount++;
						pUdpChannel->m_iRequestVideoCountEx++;
						pUdpChannel->m_iLastVideoReqSequence = dataPacket.getSequence();
					}
				}
			}
			else
			{
				//补偿包
				Trace("收到有问题的视频请求补偿包。。。。。。。。\n");
				Assert(0);
			}

			break;
		default:
			ErrTrace("从中转服务器接收到不可解析的服务器命令[%d] time = %I64d \n",dataPacket.getCmdType() ,GetTickCount());
			Assert(0);
		    break;
		}
	}
	
}