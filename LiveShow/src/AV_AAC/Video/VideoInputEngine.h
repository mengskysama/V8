#pragma once
#define  H264

#include "VideoDeviceCapDx.h"
#include "DIBSection.h"
#include "RC.h"
#include "TimersManager.h"
#include "DIBRectCalc.h"

#ifdef  XVID
#include "xvidCodec.h"
#define  DEFAULT_QUALITY 5
#endif
#ifdef  H264
#include "h264Codec.h"
#define  DEFAULT_QUALITY 26
#endif

#include "Common.h"
#include "Mutex.h"

#include <map>
#include <vector>

class VideoInputEngine:public RCObject
{
public:
	VideoInputEngine(void);
	~VideoInputEngine(void);

	void                                   Init(UInt32 iWidth=320, UInt32 iHeight=240, UInt32 iQuality=DEFAULT_QUALITY, SInt32 iDeviceID=0,UInt32 iVideoMode = (UInt32)ROOM_VIDEO_CRF_MODE);
	void								   InitDecoder(UInt32 iWidth=640, UInt32 iHeight=480);
	void                                   Destroy();
	void								   SetType(enuVIDEO_TYPE enType);

	bool                                   StartCapture(UInt32 &iResult);
	bool                                   StopCapture(bool bForce=false);

	void                                   AddLocalHwnd(HWND hLocalWnd,UInt32 iRoomId,bool bFFTWav=TRUE);
	void                                   DelLocalHwnd(HWND hLocalWnd);
	void								   ChangeLocalHwnd(HWND hNewLocalWnd,HWND hOldLocalWnd);
	bool								   SetVideoDecoder(HWND hLocalWnd,bool bEnable);
	bool								   IsVideoDecoder(HWND hLocalWnd);
	bool								   PaintLocal(HWND hWnd);

	void                                   PauseLocalHwnd(HWND hLocalWnd);
	void                                   ContinueLocalHwnd(HWND hLocalWnd);
	void                                   CaptureAndEncodeVideo();
	void								   SetNetStatus(bool bEnable) { m_bNet = bEnable; }
	
	static bool							   IsVideoVirtualDevice(int iDevId);
	static void                            EnumVideoDevices(std::vector<std::wstring> &vDeviceName);
	static void							   EnumVideoDeviceInfo(int &iDeviceNum,Service_Video_Info struDeviceInfo[]);
	static bool                            ShowVideoAdjustDlg(HWND hParWnd, UInt32 iDeviceID);
	bool                                   IsPhotoableLocalHwnd(HWND hLocalWnd);
	bool								   IsOtherUsed(HWND hWnd);
	bool								   IsExistsUsed();
	Enum_Camera_Status					   GetCamerStatus();

	inline SInt32                          GetDeviceID()      {  return m_iDeviceID;}
	inline UInt32                          GetWidth()         {  return m_iWidth;   }
	inline UInt32                          GetHeight()        {  return m_iHeight;  }
	inline UInt32                          GetQuality()       {  return m_iQuality; }
	inline bool                            IsStart()          {  MutexLocker  locker(&m_Mutex); return m_bStart;} //timer 房间线程  m_VInputEngineMap  m_VideoInputEngineRoomMap
	inline bool							   IsInit()			  {  return m_bIsInit;	}

	static RCPtr<VideoInputEngine>		   Create(SInt32 iDeviceID=0);
	static void							   RemoveInputEngine(SInt32 iDeviceID);
	static RCPtr<VideoInputEngine>		   FindInputEngine(SInt32 iDeviceID);
	static void							   RemoveAllInputEngine();

	static void							   AppendRoom(SInt32 iDeviceID,SInt32 iRoomId);
	static void							   RemoveAppendRoom(SInt32 iRoomId);
	static void							   RemoveAppendDevice(SInt32 iDeviceId);
	static RCPtr<VideoInputEngine>		   FindAppendRoom(SInt32 iRoomId);
	static void							   RemoveAllAppendRoom();
	static bool							   IsDeviceUsed(SInt32 iDeviceId);
	static bool							   IsDeviceUsed(SInt32 iDeviceID,SInt32 iRoomId);

	static std::map<UInt32, RCPtr<VideoInputEngine> >   m_VInputEngineMap; //device 摄像头和VideoInputEngine 对应关系
	typedef std::pair<SInt32, RCPtr<VideoInputEngine> > vinput_pair;

	static std::map<UInt32,RCPtr<VideoInputEngine>> m_VideoInputEngineRoomMap; //房间和摄像头对应关系
	typedef std::pair<SInt32, RCPtr<VideoInputEngine> > vinputroom_pair;
	static	UInt32                         m_iCaptureTimerID;
	static bool							   m_bViewFFTME;//自己是否显示波形图
	static Mutex                           m_Mutex; //timer 房间线程  m_VInputEngineMap  m_VideoInputEngineRoomMap m_SelfWnds m_PauseSelfWnds
	Enum_Camera_Status					   m_enumCamerStatus;
private:
	bool								   m_bIsInit;
	bool								   m_bInitKey;
	bool								   m_bNet;//是否网络传输  防止多个实例同时编码 出错bug
	bool								   m_bShowRealVideo;
	SInt32                                 m_iDeviceID;
	bool                                   m_bStart;         //已经启动
	UInt32                                 m_iQuality;       //视频质量
	UInt32                                 m_iWidth;         //视频尺寸宽度
	UInt32                                 m_iHeight;        //视频尺寸高度
	SInt64                                 m_lTimePeriod;    //采样周期
	UInt32								   m_iDecoderWidth;  //解码视频宽度
	UInt32								   m_iDecoderHeight; //解码视频高度
	UInt32								   m_iVideoMode;
	CVideoDeviceCapDx                     *m_pVideoCapture;
	UInt8                                 *m_pVideoData;
	DIBRectCalc                            m_dibRectCalc;
#ifdef XVID
	xvidCodec                             *m_pVideoEncoder;
	xvidCodec							  *m_pVideoDecoder;
#endif
#ifdef H264		
	h264Codec							  *m_pVideoEncoder;
	h264Codec							  *m_pVideoDecoder;
#endif
	//Mutex                                 m_SelfMutex;
	std::vector<DIBSection*>              m_SelfWnds;       //自己本地的所有窗口句柄 本房间内
	std::vector<DIBSection*>              m_PauseSelfWnds;   //本地暂停的所有窗口句柄 本房间内
	enuVIDEO_TYPE						  m_enType;
};

#include "TimerHandler.h"

class CaptureTimer : public TimerHandler
{
public:

	virtual  void onPeriodExpired()
	{
		Trace("CaptureTimer::onPeriodExpired ……begin  %d\n",GetTickCount());
		MutexLocker m_Locker(&VideoInputEngine::m_Mutex);
		std::map<UInt32, RCPtr<VideoInputEngine> >::iterator iter;
		RCPtr<VideoInputEngine> pVInEngine(NULL);
		for (iter=VideoInputEngine::m_VInputEngineMap.begin();iter!=VideoInputEngine::m_VInputEngineMap.end();++iter)
		{
			iter->second->CaptureAndEncodeVideo();
		}

		if (VideoInputEngine::m_iCaptureTimerID!=0 && VideoInputEngine::m_VInputEngineMap.size()==0)
		{
			Trace("CaptureTimer::TimerHandler ……  removeTimerHandler %d\n",GetTickCount());
			Assert(0);
			TimersManager* pTimersManager   = TimersManager::getInstance();
			pTimersManager->removeTimerHandler(VideoInputEngine::m_iCaptureTimerID, false);
			VideoInputEngine::m_iCaptureTimerID = 0;
		}

		Trace("CaptureTimer::onPeriodExpired ……end  %d\n",GetTickCount());
	}
};



