#pragma once

#include "ILogicListener.h"
#include "Common.h"
#include "51ktv.h"

enum
{
	OPTION_AUTO_RELAY  =  0,        //自动进入中转
	OPTION_FORCE_RELAY =  1,        //直接强制中转
	OPTION_NO_RELAY    =  2         //不进行中转
};

#ifdef CHANNEL_EXPORTS
#define CHANNEL_OUT_CLASS  _declspec(dllexport)
#else
#define CHANNEL_OUT_CLASS  _declspec(dllimport)
#endif

class CHANNEL_OUT_CLASS RelayManger
{
public:

 	static void        Initialize(IModuleManager* pModuleManager);
	static void        Initialize();
	static RelayManger*  Instance();
	static void        DestroyInstance();
	static void        DestroyBufferInstance();

	static __int64     GetLibSymbolTime();

	//配置全局参数
    static void       Config(UInt32 iMyUin, UInt32 iRelayIP, UInt16 sRelayPort, UInt32 iMaxConnTime);

	//与iRoomId之间建立一个逻辑通道, 并返回物理通道的当前状态
	bool			  RegisterLogicChannel(SInt64 lSymbolTime, UInt32 iRoomId, UInt32 iFromUin, ILogicListener* pLogicListener, UInt8& cLogicID,bool bA = false);

    //释放iDesUin之间的一个逻辑通道
	void              UnregisterLogicChannel (UInt32 iRoomId, UInt32 iFromUin, UInt8 cLogicID, bool bNotify=true);
	
	//注册UDP通道
	bool			  RegisterUDPChannel(UInt32 iRoomId);
	//释放UDP通道
	void              UnregisterUDPChannel(UInt32 iRoomId,bool bFace=true);

	//查询与iDesUin之间的物理通道状态
	//返回值：No_Exist、 Ready、Connecting、Success、Disconnect等
	unsigned short    QueryChannelStatus (UInt32 iRoomId);

	//判断与iRoomId之间的物理通道是否被利用
	bool              IsUsingChannel (UInt32 iRoomId);

	//在指定的Channel上发送数据
	void              SendData (UInt32 iRoomId, UInt8* pData, UInt16 sDataLen,bool bA=false);

	//处理通过im通道过来的命令数据
    void              ProcessCmd(UInt32 iRoomId, UInt8* pCmd, UInt16 sCmdLen);

	//修改中转服务器IP 端口
	void              ChangeRelayServer(UInt32 iRoomId,UInt32 iRelayIP, UInt16 sRelayPort);
	
	//设置AVkey
	void			  SetAVKey(UInt32 iRoomId,UInt32 iKey);
	
	//获取上行音视频网络状态
	void			  GetAVNetSendStatus(UInt32 iRoomId,UInt32& iAudioTotalPacket,UInt32& iLostAudioPacket, UInt32& iVideoTotalPacket, UInt32& iLostVideoPacket);

	//获取上行丢包率
	float             GetLostRate();
private:

	RelayManger();
	~RelayManger();

private:

	static RelayManger*            s_pInstance;
	static IModuleManager*       s_pModuleManager;

};
