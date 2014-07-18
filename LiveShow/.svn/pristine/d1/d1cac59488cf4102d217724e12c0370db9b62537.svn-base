#pragma once

#include <vector>
#include <map>
#include "CommonPacket.h"

#include "WrapListener.h"
#include "TimerHandler.h"
#include "51ktv.h"


struct StructVLogic:public RCObject
{
	UInt32 iRoomId;
	UInt32 iFromUin;
	UInt8  cLogicId;
	HWND hWnd;
};

struct StructALogic:public RCObject
{
	UInt32 iRoomId;
	UInt32 iFromUin;
	UInt8  cLogicId;
};

class UdpChannel : public RCObject
{
public:

	UdpChannel(UInt32 iMyUin, UInt32 iRoomId,IModuleManager *pModuleManager);

	~UdpChannel();

	void                            Start();
	void                            Stop(UInt8 cReason);

	bool                            Maintain(SInt64 lCurTime);
	void                            KeepAlive(UInt16 sNum=1, bool bAck=false);
	void							KeepAliveEx(UInt16 sNum=1);
	void                            SendData(UInt8* pData, UInt16 sDataLen,bool bA=false);
	void							SendRepeatData(Packet* pRePacket,bool bA=false);
	void                            ProcessCmdPacket(Packet* pCmdPacket){}; //占不处理TCP过来的命令消息，留着以后补充

	//监听者相关
	bool                            RegisterListener(ILogicListener* pLogicListener, UInt8& cLogicID,UInt32 iRoomId,UInt32 iFromUin,bool bA = false);
	void                            UnregisterListener(UInt8 cLogicID, UInt32 iRoomId,UInt32 iFromUin, bool bCallBack=false);
	RCPtr<WrapListener>             GetListener(UInt8 cLogicID,bool addRef=false);
	RCPtr<StructVLogic>				GetListenerLogicV(UInt32 iRoomId,UInt32 iFromUin);
	RCPtr<StructALogic>				GetListenerLogicA(UInt32 iRoomId,UInt32 iFromUin);
	void                            GetAllListener(std::vector<RCPtr<WrapListener> >& vAllListeners);
	bool                            IsExistListener();

	bool                            SetStatus(UInt16 sStatus);
	void                            SetRelayIPAddr();

	void                            SetAliveIPAddr(UInt32 iAliveIP, UInt16 sAlivePort, bool bLastValue=false);
	void                            SwitchToRelay(UInt32 iRelayIP, UInt16 sRelayPort);
	void                            SendToRelayServer(UInt16 cCmdType, UInt16 sSeq=0);
	void                            FinishCreatingRelaySession();
	inline UInt32                   GetRelayIP()             { return m_iRelayIP; }
	inline UInt16                   GetRelayPort()           { return m_sRelayPort; }

	inline UInt32                   GetRoomId()                      {  return m_iRoomId; }
	inline UInt16                   GetStatus()                      {  return m_sStatus; }
	inline UInt32                   GetMyUin()						 {  return m_iMyUin;  }

	inline  void                    SetLastAliveRcvTime(SInt64 lTime){ m_lLastAliveRcvTime = lTime; }
	void                            SetRelayIP(UInt32 iRelayIP, UInt16 sRelayPort);

	void							UploadInfoToServer();
	UInt64							LogicIndex(UInt32 iRoomId,UInt32 iFromUin);

	bool							IsExistsListener(UInt32 iRoomId,UInt32 iFromUin,bool bA=false);
	static void						PlayAudioPacket(UInt32 iRoomId,UInt32 iFromUin,Packet* pPacket,unsigned int iSeq);
	static void						PlayVideoPacket(UInt32 iRoomId,UInt32 iFromUin,Packet* pPacket);
	static void                     RequestCompenPacket(UInt32 iRoomId,UInt32 iFromUin,Packet* pPakcet);
	void							SendPacket(Packet* pPakcet);
	void							ResetRateCount();

	unsigned int				    m_iLastAudioReqSequence; //上次请求的音频SeqId.
	unsigned int				    m_iRequestAudioCount;//请求音频补偿次数
	unsigned int				    m_iLastVideoReqSequence;//上一次视频序列号
	unsigned int				    m_iRequestVideoCount;//请求视频补偿次数
	unsigned int					m_iSendAudioCount;//发送音频包总数
	unsigned int				    m_iSendVideoCount;//发送视频包总数

	unsigned int					m_iRequestAudioCountEx;
	unsigned int					m_iRequestVideoCountEx;
	unsigned int					m_iSendAudioCountEx;
	unsigned int					m_iSendVideoCountEx;

	DWORD                           m_dwLastRateTime;
private:
	void                            StartListen();

private:
	SInt32                                 m_iSocketID;      //注册成功的socketID标识
	UInt32								   m_iRoomId;		 //房间ID，该房间的socketChannel

	UInt32                                 m_iMyUin;         //自身的Uin
	UInt32                                 m_iMyPrvIP;       //自身的内网IP
	UInt16                                 m_sMyPrvPort;     //自身的内网Port

	bool                                   m_bLastValue;		 //设置链接建立成功时候的IP PORT
	SInt64                                 m_lLastAliveSendTime; //上次Alive的发送时间
	SInt64                                 m_lLastAliveRcvTime;  //最后一次接收到Alive的时间

	SInt64                                 m_lConnectBeginTime; //开始连接时间

	UInt16                                 m_sRelaySeq;			//中转心跳计数
	UInt32                                 m_iRelayIP;			//中转服务器IP地址
	UInt16                                 m_sRelayPort;		//中转服务器端口
	UInt32								   m_iAliveIP;			//最后连接成功时候的IP
	UInt16								   m_sAlivePort;		//最后连接成功时候的Port


	Mutex                                  m_Mutex;          
	UInt16                                 m_sStatus;               //Channel的当前状态
	bool                                   m_bCreatingRelaySession; //是否已经创建中转Session
	bool								   m_bChangeServer;

	Mutex                                  m_ListenerMutex;   //网络线程 房间线程  m_ListenersMap
	std::map<UInt8, RCPtr<WrapListener> >  m_ListenersMap;    //所有的上层监听者
	UInt8                                  m_cLogicSeed;      //产生LogicID的种子
	SInt64								   m_lCurTime;		  //当前时间
	UInt32								   m_iMaxConnTime;    //最大连接时间
 
	unsigned int						   s_audio_sequence;
	unsigned int						   s_video_sequence;
	unsigned int						   m_keep_count;

	IModuleManager *m_pModuleManager;
	typedef std::pair<UInt8, RCPtr<WrapListener> > listener_pair;

	std::map<UInt64,RCPtr<StructVLogic> >  m_ListenersRoomDestVMap; //VideoChannel LogicId 和 RoomId FromUin映射
	typedef std::pair<UInt64,RCPtr<StructVLogic> > listenerRoomDesV_pair;

	std::map<UInt64,RCPtr<StructALogic> >  m_ListenersRoomDestAMap; //AudioChannel LogicId 和 RoomId FromUin映射
	typedef std::pair<UInt64,RCPtr<StructALogic> > listenerRoomDesA_pair;
};

