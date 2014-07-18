#pragma  once
#include "audio_pch.h"
#include "Mutex.h"

//对端用户管理
class ClientUser 
{
public:
	ClientUser(){
		fPowerMin = fPowerMax = 0.0f;
		fAverageAvailable = 0.0f;
		bPause = false;
		uiLastActiveSeq = 0;
		uiTotalPacket = 0;
		uiLostPacket = 0;
	}

	wstring qsName;
	UInt64 uiSession;
	int iId;
	float fPowerMin, fPowerMax;
	float fAverageAvailable;
	bool bPause;
	unsigned int uiLastActiveSeq;
	unsigned int uiTotalPacket;//接收到总的数据包数目
	unsigned int uiLostPacket;//丢失的总的数据包数目
public:
	static std::map<UInt64, ClientUser *> c_qmUsers;
	static Mutex c_lUsers;//c_qmUsers 主线程 网络线程

	static ClientUser *get(UInt64);
	static ClientUser *add(UInt64);

	static void remove(UInt64);
	static void remove(ClientUser *);
	static void clearRoom(UInt32);
	static void clearRoom();

	static bool isRoomUp(UInt32 iRoomId);
	static bool isPause(UInt64);
	static bool setPause(UInt64,bool);
	static bool getAudioPackStatus(UInt32 iRoomId,UInt32& iTotalPacket,UInt32& iLostPacket);
};
