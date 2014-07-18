#ifndef _GLOBAL_H
#define _GLOBAL_H

#include "Settings.h"
#include "audiotransport.h"
#include "fft/AnalyserProc.h"

// Global helper class to spread variables around across threads.

extern bool bIsVistaSP1;
extern bool bIsWin7;

class AudioInput;
class AudioOutput;
class CELTCodec;

typedef int(*pfPrepareWavData)(UInt64 luin,byte* pData,unsigned int iLen);
typedef bool(*pfGetRoomAudioStatus)(unsigned int iRoomId);

struct Global {
public:
	static Global *g_global_struct;
	Settings s;
	boost::shared_ptr<AudioInput> ai;
	boost::shared_ptr<AudioOutput> ao;
	FFTWav::CAnalyserProcess* po;
	int iTarget; 
	int iPrevTarget;
	bool bPushToMute;
	UInt64 uiSession;
	bool bAttenuateOthers;
	AudioTransport *trans;
	bool bEnableSharpen; //是否开启图像显示锐化
public:
	Global(AudioTransport *ptrans);
	~Global();

	void Init(HWND hWnd);
	void SetMicMute(bool bMute){bMuteMic=bMute;}
	bool GetMicMute(){return bMuteMic;}
	inline float GetRoomSpeakerVolume(UInt32 iRoomId)
	{
		if (roomSpeakerMap.find(iRoomId)!=roomSpeakerMap.end())
		{
			return roomSpeakerMap.find(iRoomId)->second;
		}
		else
		{
			return 1.0f;
		}
	}
	void SetRoomSpeakerVolume(UInt32 iRoomId,float fVolume)
	{
		roomSpeakerMap[iRoomId] = fVolume;
	}
	bool IsRoomAudioPause(UInt32 iRoomId)
	{
		if (FunIsRoomAudioPause)
		{
			return FunIsRoomAudioPause(iRoomId);
		}
		return false;
	}
	inline void SetPrepareWavDataFun(pfPrepareWavData pFun)
	{
		if (!FunPrepareWavData)
		{
			FunPrepareWavData = pFun;
		}

	}
	inline void SetGetRoomStatusFun(pfGetRoomAudioStatus pFun)
	{
		if (!FunIsRoomAudioPause)
		{
			FunIsRoomAudioPause = pFun;
		}
	}
	inline int PrepareWavDatas(UInt64 luin,byte* pData,unsigned int iLen)
	{
		if (FunPrepareWavData)
		{
			return FunPrepareWavData(luin,pData,iLen);
		}
		return -1;
	}
private:
	bool bMuteMic;
	std::map<UInt32,float> roomSpeakerMap; //房间音量大小控制
	pfPrepareWavData FunPrepareWavData;
	pfGetRoomAudioStatus FunIsRoomAudioPause;
};

void osInit();

class DeferInit {
protected:
	static std::multimap<int, DeferInit *> *qmDeferers;
	void add(int priority);
public:
	DeferInit(int priority) {
		add(priority);
	};
	DeferInit() {
		add(0);
	};
	virtual ~DeferInit();
	virtual void initialize() {};
	virtual void destroy(){};
	static void run_initializers();
	static void run_destroyers();
};


#define g_struct (*Global::g_global_struct)


#define BAD_MEM_WRITE(a,b,c)  if (IsBadWritePtr((a),(b))) \
	{  Assert(0); ErrTrace("%s: 内存不可写 time = %I64d \n",(c),GetTickCount());  return;  }

#define BAD_MEM_READ(a,b,c)  if (IsBadReadPtr((a),(b))) \
	{  Assert(0); ErrTrace("%s: 内存不可读 time = %I64d \n",(c),GetTickCount());  return;  }

#define BAD_MEM_WRITE_RETURN(a,b,c,d) if (IsBadWritePtr((a),(b))) \
{  Assert(0); ErrTrace("%s: 内存不可写 time = %I64d \n",(c),GetTickCount());  return d;  }

#define BAD_MEM_READ_RETURN(a,b,c,d)  if (IsBadReadPtr((a),(b))) \
{  Assert(0); ErrTrace("%s: 内存不可读 time = %I64d \n",(c),GetTickCount());  return d;  }

#endif
