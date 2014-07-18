#pragma once
#ifndef H264
#define  H264
#endif

#include "VideoDeviceCapDx.h"
#include "DIBSection.h"
#include "RC.h"
#include "fft/AnalyserProc.h"
#include "DIBRectCalc.h"

using namespace FFTWav;

#ifdef  XVID
#include "xvidCodec.h"
#define  DEFAULT_QUALITY 5
#endif
#ifdef  H264
#include "h264Codec.h"
#define  DEFAULT_QUALITY 26
#endif

#define VIDEO_LOST_PACKET_TIME_INTERVAL  30*1000

#include "Common.h"
#include "Mutex.h"

#include <map>
#include <vector>

typedef struct stPartner
{
	DIBSection* pDibSection;
	bool bPause;
};

class VideoOutputEngine:public RCObject
{
public:
	VideoOutputEngine(void);
	~VideoOutputEngine(void);

	void                                   Destroy();

	void                                   AddPartnerHwnd(UInt32 iDesUin, HWND hPartnerWnd, UInt32 iWidth, UInt32 iHeight, SInt32 iQuality,UInt32 iVideoMode,enuVIDEO_TYPE enType);
	bool								   ChangePartnerHwnd(UInt32 iDesUin, HWND hPartnerWnd, UInt32 iWidth, UInt32 iHeight, HWND hOldWnd);
	void                                   DelPartnerHwnd(UInt32 iDesUin, HWND hPartnerWnd);
	void								   ClearPartnerHwnd(UInt32 iDesUin);
	void								   ClearPartnerHwndRoom();
	float								   GetLostPacketRate();
	UInt32								   GetTotalPackets();

	void                                   PausePartnerHwnd(UInt32 iDesUin, HWND hPartnerWnd);
	void                                   ContinuePartnerHwnd(UInt32 iDesUin, HWND hPartnerWnd);
	bool								   PaintRemote(UInt32 iFromUin,HWND hWnd);

	void                                   DecodeVideoData(UInt32 iDesUin, UInt8* pData, UInt32 iDataLen, UInt8 cKey, UInt32 iSeqNum, UInt32 iKeySeqNum);
	bool                                   IsPhotoablePartnerHwnd(UInt32 iDesUin, HWND hPartnerWnd);

	bool								   GetShowFFTWav(UInt32 iFromUin);
	void								   SetShowFFTWav(UInt32 iFromUin,bool bEnable);
	void								   PrepareWav(UInt32 iFromUin,byte* pData,UInt32 iLen);

	static RCPtr<VideoOutputEngine>		   Create(SInt32 iRoomId);
	static void							   RemoveOutputEngine(SInt32 iRoomId);
	static RCPtr<VideoOutputEngine>		   FindOutputEngine(SInt32 iRoomId);
	static void							   RemoveAllOutputEngine();

	static Mutex						   m_Mutex_Static;//网络线程 房间线程 m_VOutputEngineMap 
	static std::map<UInt32, RCPtr<VideoOutputEngine> >   m_VOutputEngineMap;
	typedef std::pair<SInt32, RCPtr<VideoOutputEngine> > voutput_pair;
	static bool									 m_bViewFFTALL;//全局是否显示波形图
private:
	std::map<UInt32, std::vector<stPartner> >     m_PartnerWnds;    //对方的所有窗口句柄
	std::map<UInt32,BOOL>						  m_PartnerFFT;     //对方窗口是否显示波形
	std::map<UInt32,RCPtr<CAnalyserProcess>>	  m_PartnerFFTcoder; //傅里叶变换实际编码

	std::map<UInt32, h264Codec*>				  m_PartnerDecoder;	//对应的解码器
	
	Mutex								  m_Mutex; //房间线程 网络线程 m_PartnerWnds m_PartnerDecoder m_PartnerFFT m_PartnerFFTcoder
	DIBRectCalc                           m_dibRectCalc;
private:
	UInt64								   m_iLastActiveTime;
	unsigned int						   m_unTotalPacket;
	unsigned int						   m_unLostPacket;
	float								   fLostRate;
};

