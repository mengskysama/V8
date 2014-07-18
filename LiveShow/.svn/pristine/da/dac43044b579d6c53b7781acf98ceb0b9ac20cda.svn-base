#pragma once

/*
#ifdef CHANNEL_EXPORTS
#define CHANNEL_OUT_CLASS  _declspec(dllexport)
#else
#define CHANNEL_OUT_CLASS  _declspec(dllimport)
#endif
*/

//channel状态
enum
{
	CHANNEL_NOEXIST      = 0x0000,       //通道不存在
	CHANNEL_READY        = 0x0100,       //准备就绪
	CHANNEL_CONNECTING   = 0x0200,       //正在尝试直连

	CHANNEL_RELAYING     = 0x0300,       //正在尝试中转
	CHANNEL_SUCCESS      = 0x0400,       //建立成功
	CHANNEL_DESTROY      = 0x0500,       //释放通道

	CHANNEL_MASK         = 0xFF00
};

enum
{
	REASON_NO_USE_FAIL        = 0x00,      //不再使用该通道
	REASON_ALIVE_TIMEOUT_FAIL = 0x01,      //Alive超时
	REASON_CANCEL_FAIL        = 0x02,      //对方取消
	REASON_RELAY_FAIL         = 0x03,      //中转失败
	REASON_DIRECT_FAIL        = 0x04,      //直连失败
	REASON_EXCHANGEIP_FAIL    = 0x05,      //透传IP信息失败
	REASON_STATUS_FAIL        = 0x06       //状态不一致
};


/************************************************************************/
/* 语音视频Channel接口基类                                              */
/************************************************************************/



class /*CHANNEL_OUT_CLASS*/ ILogicListener
{
public:

	virtual void  onConnected()                                             =0;
	virtual void  onReceive(const unsigned char *pBuf, unsigned int iLen, unsigned int iSeq)   =0;
	virtual void  onDisconnect(unsigned char cReason)                       =0;
 	virtual void  onStatus(unsigned short sStatus)                          =0;
};
