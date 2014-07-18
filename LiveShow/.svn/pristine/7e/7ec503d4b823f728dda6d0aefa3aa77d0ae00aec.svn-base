#include "VideoRateControl.h"
#include "AvDefine.h"
#ifndef Video_Device_CapDx_h
#define Video_Device_CapDx_h
#include "RC.h"
#ifndef SELF_CONTROL_VIDEO
#define MAX_VIDEO_FRAME_RATE  12
#else
#define MAX_VIDEO_FRAME_RATE  30
#endif


#include <atlbase.h>
#include <qedit.h>
#include <control.h>
#include <map>
#include "Mutex.h"

enum
{
	kResult_Success  = 0,
	kResult_NoCamer  = 1,
	kResult_NoExist  = 2,
	kResult_IsUsing  = 3,
	kResult_Failed   = 4
};

class CCapGrabberCB;
class CVideoDeviceCapDx 
{
public:
	CVideoDeviceCapDx();
	virtual ~CVideoDeviceCapDx();

	bool             Init(int iwidth, int iheight, UInt32 &iResult, int iDeviceID);
	void             Destroy();
	bool             GetData(unsigned char *pv,int len);
	static HRESULT   GetCapDevice(UINT no, IBaseFilter** ppCap,UINT& num);
	static std::wstring GetCapDeviceGuid(UINT no);
	Enum_Camera_Status	   m_enumCamerStatus;
protected:
	int              PutData(char *pv,int len);

	HWND                                              m_hDeviceCapWnd;//捕捉窗口句柄
	
	CComPtr<IGraphBuilder>                            m_pGraph;      //filter管理变量  

    CComPtr< ISampleGrabber>                          m_pGrabber;    //采样变量

	CComQIPtr< IMediaControl, &IID_IMediaControl>     m_pControl;    //控制器

	CComPtr<ICaptureGraphBuilder2>                    m_pCGB2; 
	
	IAMStreamConfig *                                 m_pVSC;         //控制采集的格式 

	int                    m_nDeviceID;
	bool                   m_bInit;
	int                    m_nHeight;
	int                    m_nWidth;
	unsigned char*         mpBuffer;
	bool                   mbGotData;
	volatile DWORD         m_dwGraphTime;   //上次采集数据时间
	volatile DWORD         m_dwLastReset;   //上次重置时间
	volatile bool          m_bReset;

	Mutex                  m_Mutex;
protected:

	friend class CCapGrabberCB;
	static std::map<int,CCapGrabberCB*> m_grabberMap;
	typedef std::pair<int, CCapGrabberCB* > m_grabberPair;

	HRESULT           InitGraph(UInt32 &iResult);//初始化采集各个变量
	void              DestroyGraph();//销毁采集各个变量
	bool			  ResetGraph();
};

extern std::map<unsigned int,std::wstring> g_mapVideoInputIdGuid;  //id<----->guid
extern std::map<std::wstring,std::wstring> g_mapVideoInputGuidName;//guid<----->friendname
extern std::map<unsigned int,unsigned int> g_mapVideoInputIdStatus;//id<----->status
extern unsigned int g_maxVideoInputId;//当前最大的DeviceId
extern Mutex g_inputVideoMutex;
extern bool g_bIsDeviceInit;

#endif // Video_Device_CapDx_h
