#include "stdafx.h"
#include "AudioChannel.h"
#include "AVEngine.h"
#include "AudioPacket.h"

AudioChannel::AudioChannel(UInt32 iRoomId,UInt32 iDesUin, UInt8 cLogicID)
:AVChannel(iRoomId, iDesUin, cLogicID,true)
{}

AudioChannel::~AudioChannel()
{}

void AudioChannel::StartSelfAV()
{

	//g_AVEngine.AddAudioLogicChannel(m_iRoomId,m_iFromUin, m_cLogicID); //此为发送语音注册，接收不应该干这些事
}

void AudioChannel::StopSelfAV()
{
	//g_AVEngine.DelAudioLogicChannel(m_iRoomId,m_iFromUin, m_cLogicID); //停止接收不应该干预发送的事
}

void AudioChannel::onReceive(const UInt8* pBuf, UInt32 iLen, unsigned int iSeq)
{
	if (m_bUserCancel) 
	{
		ErrTrace("AudioChannel::onReceive 已经取消操作....正在销毁...\n");
		return;
	}
	Packet tempPacket(-1);
	tempPacket.setContentPtr(const_cast<UInt8*>(pBuf), iLen);

	AudioPacket audioPacket;
	if (!audioPacket.parserPacket(&tempPacket))
	{
		Assert(0);
		return;
	}

	//进行媒体数据的解码
	switch (audioPacket.getAudioType())
	{
	case AUDIO_RTP_PACKET_TYPE:
		//		Trace("AudioChannel: 接收到RTP数据\n");
		g_AVEngine.RecvAudioData(m_iRoomId,m_iFromUin, audioPacket.getData(), audioPacket.getDataLen(),iSeq,true);
		break;
	case AUDIO_RTCP_PACKET_TYPE:
		//		Trace("AudioChannel: 接收到RTCP数据\n");
		g_AVEngine.RecvAudioData(m_iRoomId,m_iFromUin, audioPacket.getData(), audioPacket.getDataLen(),iSeq,false);
		break;
	default:
		Assert(0);
		break;
	}

	tempPacket.setContentPtr(NULL, 0);
}