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
	}

	wstring qsName;
	unsigned int uiSession;
	int iId;
	float fPowerMin, fPowerMax;
	float fAverageAvailable;
	bool bPause;
public:
	static std::map<UInt64, ClientUser *> c_qmUsers;
	static Mutex c_lUsers;

	static ClientUser *get(UInt64);
	static ClientUser *add(UInt64);

	static void remove(UInt64);
	static void remove(ClientUser *);
	static void clearRoom(UInt32);
	static void clearRoom();

	static bool isRoomUp(UInt32 iRoomId);
	static bool isPause(UInt64);
	static bool setPause(UInt64,bool);

};
