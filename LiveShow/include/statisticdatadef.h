#pragma once

#include "51ktv.h"

enum
{
	EVENT_VALUE_P2P_REPORTTOSERVER	= 0x400B,		//像服务器报告状态
};

enum
{
	CONN_DIRECT=1,  //直连
	CONN_DIRECT_RELAY, //先直连后中转
	CONN_RELAY,		//直接中转
};

struct ChannelModule_InfoUpload_Event :  public ExtraInfo
{
	uint32       m_iMyUin;
	uint32       m_iDesUin;

	uint32       m_iMyPubIP;				//自己的公网地址
	uint16       m_sMyPubPort;				//自己的公网端口
	uint32       m_iDesPubIP;				//对方的公网地址
	uint16       m_sDesPubPort;				//对方的公网端口

	uint32       m_iRelayIpSelf;			//自己连接的中转服务器IP
	uint16       m_sRelayPortSelf;			//自己连接的中转服务器端口

	uint32		 m_iRelayIpOpp;				//对方连接的中转服务器IP
	uint16       m_sRelayPortOpp;			//对方连接的中转服务器端口

	uint8		 m_cSuccess;				//是否链接成功
	uint32		 m_lSuccessTime;			//连接成功所花时间
	uint8		 m_cSuccessType;			//连接成功类型 1：直连，2：先直连后中转，3：直接中转
	
	EventValue  eventValue;//事件值     //haosl ================================================================

	ChannelModule_InfoUpload_Event()
	{
		eventValue = EVENT_VALUE_P2P_REPORTTOSERVER;
	}
};



