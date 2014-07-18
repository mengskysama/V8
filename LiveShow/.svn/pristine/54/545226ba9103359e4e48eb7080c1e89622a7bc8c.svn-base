#include "stdafx.h"
#include "VideoDeviceCapDx.h"
#include <uuids.h>
#include <streams.h>
#include <dvdmedia.h>
#include "Global.h"

//#pragma comment(lib, "amstrmid.lib")
//#pragma comment(lib, "quartz.lib")
//#pragma comment(lib, "strmiids.lib")
#pragma comment(lib, "Strmbase.lib")
//#pragma comment(lib, "cscc.lib")

#ifdef DEBUG
//#pragma comment(lib, "libxvidcored.lib")
//#pragma comment(lib, "libx264d.lib")
//#pragma comment(lib, "h264decoderd.lib")
#else
//#pragma comment(lib, "libxvidcore.lib")
//#pragma comment(lib, "libx264.lib")
//#pragma comment(lib, "h264decoder.lib")
#endif

std::map<unsigned int,std::wstring> g_mapVideoInputIdGuid;  //id<----->guid
std::map<std::wstring,std::wstring> g_mapVideoInputGuidName;//guid<----->friendname
std::map<unsigned int,unsigned int> g_mapVideoInputIdStatus;//id<----->status
unsigned int g_maxVideoInputId = 0;//当前最大的DeviceId
bool g_bIsDeviceInit = false;
Mutex g_inputVideoMutex;

class CCapGrabberCB : public ISampleGrabberCB 
{
public:
	// these will get set by the main thread below. We need to
	// know this in order to write out the bmp
	CVideoDeviceCapDx* m_pOwner;

	CCapGrabberCB( )
	{
		m_pOwner = NULL;
	}   

	// fake out any COM ref counting
	//
	STDMETHODIMP_(ULONG) AddRef() { return 2; }
	STDMETHODIMP_(ULONG) Release() { return 1; }

	// fake out any COM QI'ing
	//
	STDMETHODIMP QueryInterface(REFIID riid, void ** ppv)
	{
		if( riid == IID_ISampleGrabberCB || riid == IID_IUnknown ) 
		{
			*ppv = (void *) static_cast<ISampleGrabberCB*> ( this );
			return NOERROR;
		}    
		return E_NOINTERFACE;
	}

	// we don't implement this interface for this example
	//
	STDMETHODIMP SampleCB( double SampleTime, IMediaSample * pSample )
	{
		return 0;
	}

	// The sample grabber is calling us back on its deliver thread.
	// This is NOT the main app thread!
	//
	//           !!!!! WARNING WARNING WARNING !!!!!
	//
	// On Windows 9x systems, you are not allowed to call most of the 
	// Windows API functions in this callback.  Why not?  Because the
	// video renderer might hold the global Win16 lock so that the video
	// surface can be locked while you copy its data.  This is not an
	// issue on Windows 2000, but is a limitation on Win95,98,98SE, and ME.
	// Calling a 16-bit legacy function could lock the system, because 
	// it would wait forever for the Win16 lock, which would be forever
	// held by the video renderer.
	//
	// As a workaround, copy the bitmap data during the callback,
	// post a message to our app, and write the data later.
	//
	STDMETHODIMP BufferCB( double dblSampleTime, BYTE * pBuffer, long lBufferSize )
	{
		// this flag will get set to true in order to take a picture
		if (!pBuffer)
			return E_POINTER;

		//if(m_pOwner->IsCaptureTimeOut())
		{
			//OutputDebugStringA("采集视频数据\n");
			m_pOwner->PutData((char*)pBuffer,lBufferSize);
		}
		return 0;
	}
};

//CCapGrabberCB GrabberCB;
std::map<int,CCapGrabberCB*> CVideoDeviceCapDx::m_grabberMap;
static Mutex s_gmutex;  //m_grabberMap 主线程

CVideoDeviceCapDx::CVideoDeviceCapDx()
{
	m_nDeviceID=0;
	m_hDeviceCapWnd=0;
	m_pVSC=0;	
	m_bInit = false;
	m_bReset = false;
	m_dwGraphTime = GetTickCount();
	m_dwLastReset = m_dwGraphTime;
	mpBuffer = NULL;
	m_enumCamerStatus=kVideo_Success;
}

CVideoDeviceCapDx::~CVideoDeviceCapDx()
{
	Destroy();
}

bool CVideoDeviceCapDx::Init(int iwidth, int iheight, UInt32 &iResult, int iDeviceID)
{
	m_nDeviceID = iDeviceID;
	m_nHeight = iheight;
	m_nWidth = iwidth;
	mpBuffer = new unsigned char[m_nHeight*m_nWidth*3];
	mbGotData = false;
	m_hDeviceCapWnd=::CreateWindow(_T("STATIC"),_T("Video Capture Window"),WS_POPUP,0,0,10,10,NULL,NULL,NULL,NULL);
    HRESULT hr=InitGraph(iResult);
	m_enumCamerStatus = kVideo_Success;
    if( FAILED( hr ) ){
		switch(iResult)
		{
		case kResult_Success:
			m_enumCamerStatus = kVideo_Success;
			break;
		case kResult_NoCamer:
			m_enumCamerStatus = kVideo_NoCamer;
			break;
		case kResult_NoExist:
			m_enumCamerStatus = kVideo_NoExist;
			break;
		case kResult_IsUsing:
			m_enumCamerStatus = kVideo_IsUsing;
			break;
		case kResult_Failed:
			m_enumCamerStatus = kVideo_Failed;
			break;
		default:
			break;
		}
		Assert(0);
		Destroy();
		return false;
	}
	m_bInit=true;

	return m_bInit;
}

void CVideoDeviceCapDx::Destroy()
{
	Trace("CVideoDeviceCapDx: 开始释放视频捕捉的资源 \n");

	m_bInit=false;

	DestroyGraph();

	if(m_hDeviceCapWnd)
		::DestroyWindow(m_hDeviceCapWnd);
	m_hDeviceCapWnd = NULL;

	Trace("CVideoDeviceCapDx: 完成释放视频捕捉的资源 \n");
	
	MutexLocker locker(&s_gmutex);//m_grabberMap 主线程

	if (m_grabberMap.find(m_nDeviceID)!=m_grabberMap.end())
	{
		CCapGrabberCB* pGrabberCB = m_grabberMap.find(m_nDeviceID)->second;
		m_grabberMap.erase(m_grabberMap.find(m_nDeviceID));
		delete pGrabberCB;
	}
	
	if(mpBuffer)
		delete []mpBuffer;
	mpBuffer = 0;

// 	if (!m_bReset)
// 		CoUninitialize();
}

bool CVideoDeviceCapDx::ResetGraph()
{
	m_pVSC=0;
	m_bReset = true;
	UInt32 iResult;
	Destroy();
	Sleep(300);
	Init(m_nWidth,m_nHeight,iResult,m_nDeviceID);
	m_bReset = false;
	return true;
}


void PrintGUID(const GUID* pGuid)
{
// 	char buf[1024];
// 	sprintf(buf,"VIDEO_TYPE_GUID=0x%08x-0x%04x-0x%04x-0x%02x-0x%02x-0x%02x-0x%02x-0x%02x-0x%02x-0x%02x-0x%02x\n",pGuid->Data1,pGuid->Data2,pGuid->Data3,pGuid->Data4[0],pGuid->Data4[1],pGuid->Data4[2],pGuid->Data4[3],pGuid->Data4[4],pGuid->Data4[5],pGuid->Data4[6],pGuid->Data4[7]);
// 	OutputDebugStringA(buf);
}

void PrintSizeInfo(char* pType,int width,int height,WORD biBitCount)
{
// 	char buf[1024];
// 	sprintf(buf,"VIDEO_TYPE=%s  width=%d  height=%d  biBitCount=%d\n",pType,width,height,biBitCount);
// 	OutputDebugStringA(buf);
}

HRESULT CVideoDeviceCapDx::InitGraph(UInt32 &iResult)
{
	iResult = kResult_Failed;

    HRESULT hr=E_FAIL;
	if(m_pGraph)
	{
		return hr;
	}

	CoInitialize(NULL);

    //create a filter graph
    hr = m_pGraph.CoCreateInstance(CLSID_FilterGraph);
    if(!m_pGraph )
    {
        return E_FAIL;
    }

    // get capture device 
    CComPtr< IBaseFilter > pCap;
	unsigned int iCamCount = 0;
    GetCapDevice(m_nDeviceID, &pCap,iCamCount);
    if(!pCap)
    {
		iResult = kResult_NoExist;
		if (iCamCount ==0)
		{
			iResult = kResult_NoCamer;
		}
		Assert(0);
		return E_FAIL;
    }

// 	CComPtr <IAMVideoProcAmp> pProcAmp;
// 	hr = pCap->QueryInterface(IID_IAMVideoProcAmp, (void**)&pProcAmp);
// 	if(SUCCEEDED(hr))
// 	{
// 		long Min, Max, Step, Default, Flags, Val;
// 
// 		long g_Brightness = 100; //亮度
// 		long g_Contrast = 75; //亮度
// 		long g_Saturation = 75;//饱和度
// 
// 		if( g_Brightness > 0 ){
// 			hr = pProcAmp->GetRange(VideoProcAmp_Brightness, &Min, &Max, &Step,
// 				&Default, &Flags);
// 			if(SUCCEEDED(hr))
// 			{
// 				hr = pProcAmp->Get(VideoProcAmp_Brightness, &Val, &Flags);
// 				Val = Min+(Max-Min)*g_Brightness/100;
// 				pProcAmp->Set(VideoProcAmp_Brightness, Val, Flags);
// 			}
// 		}
// 
// 		if( g_Contrast > 0 ){
// 			hr = pProcAmp->GetRange(VideoProcAmp_Contrast, &Min, &Max, &Step,
// 				&Default, &Flags);
// 			if(SUCCEEDED(hr))
// 			{
// 				hr = pProcAmp->Get(VideoProcAmp_Contrast, &Val, &Flags);
// 				Val = Min+(Max-Min)*g_Contrast/100;
// 				pProcAmp->Set(VideoProcAmp_Contrast, Val, Flags);
// 			}
// 		}
// 
// 		if( g_Saturation > 0 ){
// 			hr = pProcAmp->GetRange(VideoProcAmp_Saturation, &Min, &Max, &Step,
// 				&Default, &Flags);
// 			if(SUCCEEDED(hr))
// 			{
// 				hr = pProcAmp->Get(VideoProcAmp_Saturation, &Val, &Flags);
// 				Val = Min+(Max-Min)*g_Saturation/100;
// 				pProcAmp->Set(VideoProcAmp_Saturation, Val, Flags);
// 			}
// 		}
// 	}


    // add the capture filter to the graph
    hr = m_pGraph->AddFilter(pCap, L"Cap");
    if( FAILED( hr ) )
    {
		Assert(0);
        return E_FAIL;
    }

    // create a sample grabber
    hr = m_pGrabber.CoCreateInstance( CLSID_SampleGrabber );
    if( !m_pGrabber )
    {
		AssertV(0, ::GetLastError());
        return hr;
    }
    CComQIPtr< IBaseFilter, &IID_IBaseFilter > pGrabBase( m_pGrabber );

    // force it to connect to video, 24 bit
    CMediaType VideoType;
    VideoType.SetType( &MEDIATYPE_Video );
    VideoType.SetSubtype( &MEDIASUBTYPE_RGB24 );
    hr = m_pGrabber->SetMediaType( &VideoType );
    if( FAILED( hr ) )
    {
		Assert(0);
        return hr;
    }

    // add the grabber to the graph
    hr = m_pGraph->AddFilter( pGrabBase, L"Grabber" );
    if( FAILED( hr ) )
    {
		Assert(0);
        return hr;
    }
    // build the graph
    hr = m_pCGB2.CoCreateInstance (CLSID_CaptureGraphBuilder2, NULL, CLSCTX_INPROC);
    if (FAILED( hr ))
    {
		Assert(0);
        return hr;
    }
    hr = m_pCGB2->SetFiltergraph( m_pGraph );
    if (FAILED( hr ))
    {
		Assert(0);
        return hr;
    }

    hr = m_pCGB2->FindInterface(&PIN_CATEGORY_CAPTURE,
                                &MEDIATYPE_Interleaved,
                                pCap, IID_IAMStreamConfig, (void **)&m_pVSC);
    if(hr != NOERROR)
    {
        hr = m_pCGB2->FindInterface(&PIN_CATEGORY_CAPTURE,
                                 &MEDIATYPE_Video, pCap,
                                 IID_IAMStreamConfig, (void **)&m_pVSC);
        if(hr != NOERROR)
        {
			Assert(0);
            // this means we can't set frame rate (non-DV only)
            //Error(TEXT("Cannot find VCapture:IAMStreamConfig"));
			return hr;
        }
    }

	int nCount;
	int nSize;
	hr = m_pVSC->GetNumberOfCapabilities(&nCount, &nSize);
	if (FAILED(hr))
		return hr;

	BOOL bFound320x240 = FALSE;
	for (int j = 0; j < nCount; j++)
	{
		CMediaType mt;
		AM_MEDIA_TYPE *pmt = NULL;
		VIDEO_STREAM_CONFIG_CAPS vcaps;
		BYTE *pSCC = (BYTE *)(&vcaps);
		hr = m_pVSC->GetStreamCaps(j, &pmt, pSCC);
		if (FAILED(hr))
			return hr;
		mt = *pmt;
		DeleteMediaType(pmt);

		PrintGUID(mt.FormatType());

		WORD nBitCount = 0;
		long nWidth = 0;
		long nHeight = 0;
		if (*mt.FormatType() == FORMAT_DvInfo)
		{
// 			if (*mt->pbFormat!=NULL)
// 			{
// 				VIDEOINFOHEADER *pInfo = (VIDEOINFOHEADER *)mt.pbFormat;
// 				nBitCount = pInfo->bmiHeader.biBitCount;
// 				nWidth = pInfo->bmiHeader.biWidth;
// 				nHeight = pInfo->bmiHeader.biHeight;
// 			}
			//OutputDebugStringA("FORMAT_DvInfo\n");
		}
		else if (*mt.FormatType() == FORMAT_MPEGVideo)
		{
			MPEG1VIDEOINFO *pInfo = (MPEG1VIDEOINFO *)mt.Format();
			nBitCount = pInfo->hdr.bmiHeader.biBitCount;
			nWidth = pInfo->hdr.bmiHeader.biWidth;
			nHeight = pInfo->hdr.bmiHeader.biHeight;
			PrintSizeInfo("FORMAT_MPEGVideo",nWidth,nHeight,nBitCount);
		}
		else if (*mt.FormatType() == FORMAT_MPEG2Video)
		{
			MPEG2VIDEOINFO *pInfo = (MPEG2VIDEOINFO *)mt.Format();
			nBitCount = pInfo->hdr.bmiHeader.biBitCount;
			nWidth = pInfo->hdr.bmiHeader.biWidth;
			nHeight = pInfo->hdr.bmiHeader.biHeight;
			PrintSizeInfo("FORMAT_MPEG2Video",nWidth,nHeight,nBitCount);
		}
		else if (*mt.FormatType() == FORMAT_VideoInfo)
		{
			VIDEOINFOHEADER *pInfo = (VIDEOINFOHEADER *)mt.Format();
			nBitCount = pInfo->bmiHeader.biBitCount;
			nWidth = pInfo->bmiHeader.biWidth;
			nHeight = pInfo->bmiHeader.biHeight;
			PrintSizeInfo("FORMAT_VideoInfo",nWidth,nHeight,nBitCount);
		}
		else if (*mt.FormatType() == FORMAT_VideoInfo2)
		{
			VIDEOINFOHEADER2 *pInfo = (VIDEOINFOHEADER2 *)mt.Format();
			nBitCount = pInfo->bmiHeader.biBitCount;
			nWidth = pInfo->bmiHeader.biWidth;
			nHeight = pInfo->bmiHeader.biHeight;
			PrintSizeInfo("FORMAT_VideoInfo2",nWidth,nHeight,nBitCount);
		}
		if ((nWidth == 320) && (nHeight == 240) && (nBitCount == 24))
		{
			bFound320x240 = TRUE;
			hr = m_pVSC->SetFormat(&mt);
			if (FAILED(hr))
				return hr;
			break;
		}
	}
	if (!bFound320x240)
	{
		//OutputDebugStringA("VIDEO_TYPE_GUID -- 没有找到合适的类型 - 1\n");
		for (int j = 0; j < nCount; j++)
		{
			CMediaType mt;
			AM_MEDIA_TYPE *pmt = NULL;
			VIDEO_STREAM_CONFIG_CAPS vcaps;
			BYTE *pSCC = (BYTE *)(&vcaps);
			hr = m_pVSC->GetStreamCaps(j, &pmt, pSCC);
			if (FAILED(hr))
				return hr;
			mt = *pmt;
			PrintGUID(mt.FormatType());
			if(pmt->formattype == FORMAT_VideoInfo)
			{
				VIDEOINFOHEADER *pvi = (VIDEOINFOHEADER *)pmt->pbFormat;
				if (pvi->bmiHeader.biWidth==320 && pvi->bmiHeader.biHeight==240 && pvi->bmiHeader.biBitCount==24)
				{
					DeleteMediaType(pmt);
					bFound320x240 = TRUE;
					hr = m_pVSC->SetFormat(&mt);
					if (FAILED(hr))
						return hr;
					break;
				}
			}
			DeleteMediaType(pmt);
		}
	}
	if (!bFound320x240)
	{
		//OutputDebugStringA("VIDEO_TYPE_GUID -- 没有找到合适的类型 - 2\n");
		AM_MEDIA_TYPE *pmt;
		hr = m_pVSC->GetFormat(&pmt);

		// DV capture does not use a VIDEOINFOHEADER
		if(hr == NOERROR)
		{
			if(pmt->formattype == FORMAT_VideoInfo)
			{
				VIDEOINFOHEADER *pvi = (VIDEOINFOHEADER *)pmt->pbFormat;
				pvi->AvgTimePerFrame = (LONGLONG)(10000000 / MAX_VIDEO_FRAME_RATE);
				pvi->bmiHeader.biSize=sizeof(BITMAPINFOHEADER);
				pvi->bmiHeader.biWidth=m_nWidth;
				pvi->bmiHeader.biHeight=m_nHeight;
				pvi->bmiHeader.biPlanes=1;
				pvi->bmiHeader.biBitCount=24;
				pvi->bmiHeader.biSizeImage=m_nWidth*m_nHeight*3;
				pmt->lSampleSize = pvi->bmiHeader.biSizeImage;
				hr = m_pVSC->SetFormat(pmt);
				if(hr != NOERROR)
				{
					Assert(0);
					return hr;
				}
			}
			DeleteMediaType(pmt);
		}
	}

// 	if (!bFound320x240)
// 		return E_FAIL;
/*
    AM_MEDIA_TYPE *pmt;
	hr = m_pVSC->GetFormat(&pmt);
	
	// DV capture does not use a VIDEOINFOHEADER
	if(hr == NOERROR)
	{
		if(pmt->formattype == FORMAT_VideoInfo)
		{
			VIDEOINFOHEADER *pvi = (VIDEOINFOHEADER *)pmt->pbFormat;
			pvi->AvgTimePerFrame = (LONGLONG)(10000000 / MAX_VIDEO_FRAME_RATE);
			pvi->bmiHeader.biSize=sizeof(BITMAPINFOHEADER);
			pvi->bmiHeader.biWidth=m_nWidth;
			pvi->bmiHeader.biHeight=m_nHeight;
			pvi->bmiHeader.biPlanes=1;
			pvi->bmiHeader.biBitCount=24;
			pvi->bmiHeader.biSizeImage=m_nWidth*m_nHeight*3;
			pmt->lSampleSize = pvi->bmiHeader.biSizeImage;
			hr = m_pVSC->SetFormat(pmt);
			if(hr != NOERROR)
			{
				Assert(0);
				return hr;
			}
		}
		DeleteMediaType(pmt);
	}
*/

    // If there is a VP pin present on the video device, then put the 
    // renderer on CLSID_NullRenderer
    CComPtr<IPin> pVPPin;
    hr = m_pCGB2->FindPin(
                        pCap, 
                        PINDIR_OUTPUT, 
                        &PIN_CATEGORY_VIDEOPORT, 
                        NULL, 
                        FALSE, 
                        0, 
                        &pVPPin);
    // If there is a VP pin, put the renderer on NULL Renderer
    CComPtr<IBaseFilter> pRenderer;
    if (S_OK == hr)
    {   
        hr = pRenderer.CoCreateInstance(CLSID_NullRenderer);    
        if (S_OK != hr)
        {
            return S_OK;
        }
        hr = m_pGraph->AddFilter(pRenderer, L"NULL renderer");
        if (FAILED (hr))
        {
			Assert(0);
            return hr;
        }
    }
    hr = m_pCGB2->RenderStream(
                            &PIN_CATEGORY_PREVIEW,
                            &MEDIATYPE_Interleaved, 
                            pCap, 
                            pGrabBase, 
                            pRenderer);
    if (FAILED (hr))
    {
        // try to render preview pin
        hr = m_pCGB2->RenderStream( 
                                &PIN_CATEGORY_PREVIEW, 
                                &MEDIATYPE_Video,
                                pCap, 
                                pGrabBase, 
                                pRenderer);
        // try to render capture pin
        if( FAILED( hr ) )
        {
            hr = m_pCGB2->RenderStream( 
                                    &PIN_CATEGORY_CAPTURE, 
                                    &MEDIATYPE_Video,
                                    pCap, 
                                    pGrabBase, 
                                    pRenderer);
        }
    }
    if( FAILED( hr ) )
    {
		//此时音视频设备正在使用
		AssertV(0, ::GetLastError());
		iResult = kResult_IsUsing;
        return hr;
    }
	// ask for the connection media type so we know how big
    // it is, so we can write out bitmaps
    //
    AM_MEDIA_TYPE mt;
    hr = m_pGrabber->GetConnectedMediaType( &mt );
    if ( FAILED( hr) )
    {
		Assert(0);
        return hr;
    }
    
    VIDEOINFOHEADER * vih = (VIDEOINFOHEADER*) mt.pbFormat;
	vih->bmiHeader.biSize=sizeof(BITMAPINFOHEADER);
	vih->bmiHeader.biWidth=m_nWidth;
	vih->bmiHeader.biHeight=m_nHeight;
	vih->bmiHeader.biPlanes=1;
	vih->bmiHeader.biBitCount=24;
	vih->bmiHeader.biSizeImage=m_nWidth*m_nHeight*3;
	m_pGrabber->SetMediaType(&mt);

	//m_nDeviceID
	MutexLocker locker(&s_gmutex);//m_grabberMap 主线程

	CCapGrabberCB* pGrabber = NULL;
	if (m_grabberMap.find(m_nDeviceID)==m_grabberMap.end())
	{
		m_grabberMap.insert(m_grabberPair(m_nDeviceID,new CCapGrabberCB()));
	}
	pGrabber = m_grabberMap.find(m_nDeviceID)->second;
    pGrabber->m_pOwner = this;

    FreeMediaType(mt);
    // don't buffer the samples as they pass through
    //
    hr = m_pGrabber->SetBufferSamples( FALSE );
    // only grab one at a time, stop stream after
    // grabbing one sample
    //
    hr = m_pGrabber->SetOneShot( FALSE );
    // set the callback, so we can grab the one sample
    //
    hr = m_pGrabber->SetCallback(&*pGrabber, 1);
    // find the video window and stuff it in our window

    CComQIPtr< IVideoWindow, &IID_IVideoWindow > pWindow = m_pGraph;
    if( !pWindow )
    {
		Assert(0);
        return E_FAIL;
    }
    // set up the preview window to be in our dialog
    // instead of floating popup
    //
    RECT rc;
    ::GetWindowRect( m_hDeviceCapWnd, &rc );
    hr = pWindow->put_Owner( (OAHWND) m_hDeviceCapWnd );
    hr = pWindow->put_Left( 0 );
    hr = pWindow->put_Top( 0 );
    hr = pWindow->put_Width( 0 );
    hr = pWindow->put_Height( 0 );
    hr = pWindow->put_WindowStyle( WS_CHILD | WS_CLIPSIBLINGS );
    hr = pWindow->put_Visible( OAFALSE );
    
    // run the graph
	m_pControl = m_pGraph;
    hr = m_pControl->Run( );
    if( FAILED( hr ) )
    {
		iResult = kResult_IsUsing;
		Assert(0);
        return hr;
    }

	iResult = kResult_Success;

    return hr;
}

void CVideoDeviceCapDx::DestroyGraph( )
{
	try
	{
		// Destroy capture graph
		if( m_pGraph )
		{
			// have to wait for the graphs to stop first
			//
			if( m_pControl ) 
			{
				m_pControl->Pause( );
				Sleep(200);
				m_pControl->StopWhenReady( );
			}

			// make the window go away before we release graph
			// or we'll leak memory/resources
			// 
			CComQIPtr< IVideoWindow, &IID_IVideoWindow > pWindow = m_pGraph;
			if( pWindow )
			{
				pWindow->put_Visible( OAFALSE );
				pWindow->put_Owner( NULL );
			}

			if(m_pVSC) m_pVSC->Release();
			if(m_pGraph) m_pGraph.Release( );
			if(m_pGrabber) m_pGrabber.Release( );
		}
	}
	catch(...)
	{
		AssertV(0, ::GetLastError());
		::SetLastError(0);
	}

	CComPtr<IGraphBuilder> m_pGraphEx;
	m_pGraph = m_pGraphEx;
	CComPtr< ISampleGrabber> m_pGrabberEx;
	m_pGrabber = m_pGrabberEx;
	CComQIPtr< IMediaControl, &IID_IMediaControl>  m_pControlEx;
	m_pControl = m_pControlEx;
	CComPtr<ICaptureGraphBuilder2>  m_pCGB2Ex; 
	m_pCGB2 = m_pCGB2Ex;
	m_pVSC = 0;
}
//query and get data
bool CVideoDeviceCapDx::GetData(unsigned char *pv,int len)
{
	bool bret = false;
	//MutexLocker locker(&m_Mutex);

	if(mbGotData && m_nWidth*m_nHeight*3 == len && !m_bReset)
	{
		memcpy(pv,mpBuffer,len);
		bret = true;
		mbGotData = false;
	}

	if (m_bReset)
	{
		Trace("CVideoDeviceCapDx::GetData 正在重置....\n");
		return false;
	}
	
	if (GetTickCount() - m_dwGraphTime > 4000 && GetTickCount() - m_dwLastReset > 2*60*1000) //3s没有采集到数据 重置,重置摄像头间隔时间不得小于2分钟
	{
		m_dwLastReset = GetTickCount();

		if(!ResetGraph())
		{
			Assert(0);
			Trace("摄像头重置失败.....\n");
		}		
	}

	static unsigned int s = 0;
	if (s++ % 30 == 0)
	{
		Trace("获取数据CVideoDeviceCapDx::GetData........%d,mbGotData=%d\n",GetTickCount(),mbGotData?1:0);
	}

	return bret;
}

int CVideoDeviceCapDx::PutData(char *pv,int len)
{
	//MutexLocker locker(&m_Mutex);
	m_dwGraphTime = GetTickCount();
	BAD_MEM_READ_RETURN(pv,len,"CVideoDeviceCapDx::PutData",-1)

	if(m_nWidth*m_nHeight*3 == len && mpBuffer)
	{
		memcpy(mpBuffer,pv,len);
		mbGotData = true;
	}

	static unsigned int s = 0;
	if (s++ % 30 == 0)
	{
		Trace("采集到数据CVideoDeviceCapDx::PutData........%d\n",GetTickCount());
	}

	return 1;
}

// std::map<unsigned int,std::wstring> g_mapVideoInputIdGuid;  //id<----->guid
// std::map<std::wstring,std::wstring> g_mapVideoInputGuidName;//guid<----->friendname
// std::map<unsigned int,unsigned int> g_mapVideoInputIdStatus;//id<----->status
// unsigned int g_maxVideoInputId = 0;//当前最大的DeviceId

wstring CVideoDeviceCapDx::GetCapDeviceGuid(UINT no)
{

	wstring strGUID = L"";
	unsigned int uiStatus;
	std::map<unsigned int,unsigned int>::iterator iter = g_mapVideoInputIdStatus.find(no);
	if (iter!=g_mapVideoInputIdStatus.end() && iter->second==1){
		std::map<unsigned int,std::wstring>::iterator iterg = g_mapVideoInputIdGuid.find(no);
		if (iterg!=g_mapVideoInputIdGuid.end()){
			strGUID = iterg->second;
		}
	}
	return strGUID;
}

HRESULT CVideoDeviceCapDx::GetCapDevice(UINT no, IBaseFilter ** ppCap,UINT& num)
{
	MutexLocker l(&g_inputVideoMutex);
	std::wstring strGuid = CVideoDeviceCapDx::GetCapDeviceGuid(no);
	HRESULT hr=E_FAIL;

	ASSERT(ppCap);
	if (!ppCap)
		return hr;

	*ppCap = NULL;

	// create an enumerator
	CComPtr< ICreateDevEnum > pCreateDevEnum;
	hr = pCreateDevEnum.CoCreateInstance( CLSID_SystemDeviceEnum );

	ASSERT(pCreateDevEnum);
	if( hr!=S_OK || !pCreateDevEnum )
		return hr;

	// enumerate video capture devices
	CComPtr< IEnumMoniker > pEm;
	hr = pCreateDevEnum->CreateClassEnumerator( CLSID_VideoInputDeviceCategory, &pEm, 0 );

	if( hr!=S_OK || !pEm )
		return hr;

	pEm->Reset( );

	UINT number = num = 0;
	while( number++<g_maxVideoInputId )
	{
		ULONG ulFetched = 0;
		CComPtr< IMoniker > pM;

		hr = pEm->Next( 1, &pM, &ulFetched );
		if( hr != S_OK )
			break;

		// get the property bag interface from the moniker
		CComPtr< IPropertyBag > pBag;
		hr = pM->BindToStorage( 0, 0, IID_IPropertyBag, (void**) &pBag );
		if( hr != S_OK )
			continue;

		CComVariant var;
		var.vt = VT_BSTR;
		hr = pBag->Read( L"FriendlyName", &var, NULL );
		if( hr != S_OK )
			continue;

		CComVariant varGuid;
		varGuid.vt = VT_BSTR;
		hr = pBag->Read( L"CLSID", &varGuid, NULL );
		if( hr != S_OK )
			continue;

		hr=pBag->Read( L"DevicePath", &varGuid, NULL );
		wstring strg = varGuid.bstrVal;
		num ++;
		if(!strGuid.empty() && strg.compare(strGuid)!=0){
			continue;
		}

		// ask for the actual filter
		hr = pM->BindToObject( 0, 0, IID_IBaseFilter, (void**) ppCap );
		if( *ppCap )
			break;
	}

	return hr;
}


