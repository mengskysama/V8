#include "stdafx.h"
#include "AV.h"
#include "DirectSound.h"
#include "Global.h"
#include "MyAssert.h"
#include "dxerr8.h"
//#include "utility/SystemHelper2.h"
//using namespace common::utility::systemhelper;
#include "AVModule.h"
//#include "dsutil.h"
#include "Shlwapi.h"

#undef FAILED
#define FAILED(Status) (static_cast<HRESULT>(Status)<0)

// #define MY_DEFERRED DS3D_DEFERRED
#define MY_DEFERRED DS3D_IMMEDIATE

#ifndef KSAUDIO_SPEAKER_5POINT1_SURROUND
#define KSAUDIO_SPEAKER_5POINT1_SURROUND (SPEAKER_FRONT_LEFT | SPEAKER_FRONT_RIGHT | \
	SPEAKER_FRONT_CENTER | SPEAKER_LOW_FREQUENCY | \
	SPEAKER_SIDE_LEFT  | SPEAKER_SIDE_RIGHT)
#endif

#ifndef KSAUDIO_SPEAKER_7POINT1_SURROUND
#define KSAUDIO_SPEAKER_7POINT1_SURROUND (SPEAKER_FRONT_LEFT | SPEAKER_FRONT_RIGHT | \
	SPEAKER_FRONT_CENTER | SPEAKER_LOW_FREQUENCY | \
	SPEAKER_BACK_LEFT | SPEAKER_BACK_RIGHT | \
	SPEAKER_SIDE_LEFT | SPEAKER_SIDE_RIGHT)
#endif

#define NBLOCKS 50
#define MAX(a,b)        ( (a) > (b) ? (a) : (b) )
#define MIN(a,b)        ( (a) < (b) ? (a) : (b) )

class DXAudioOutputRegistrar : public AudioOutputRegistrar {
public:
	DXAudioOutputRegistrar();
	virtual AudioOutput *create();
	virtual const std::vector<audioDevice> getDeviceChoices();
	virtual void setDeviceChoice(const QByteArray&, Settings &);

};

class DXAudioInputRegistrar : public AudioInputRegistrar {
public:
	DXAudioInputRegistrar();
	virtual AudioInput *create();
	virtual const std::vector<audioDevice> getDeviceChoices();
	virtual void setDeviceChoice(const QByteArray&, Settings &);
	virtual bool canEcho(const wstring &) const;

};

class DirectSoundInit : public DeferInit {
private:
	DXAudioInputRegistrar *airReg;
	DXAudioOutputRegistrar *aorReg;
public:
	DirectSoundInit():DeferInit(){
	}
	~DirectSoundInit(){ destroy();}

	void initialize();
	void destroy();
};

static DirectSoundInit dsinit;

void DirectSoundInit::initialize() {
	airReg = NULL;
	aorReg = NULL;

#ifdef USE_WASAPI
	OSVERSIONINFOEXW ovi;
	memset(&ovi, 0, sizeof(ovi));

	ovi.dwOSVersionInfoSize=sizeof(ovi);
	GetVersionEx(reinterpret_cast<OSVERSIONINFOW *>(&ovi));

	if ((ovi.dwMajorVersion > 6) || ((ovi.dwMajorVersion == 6) && (ovi.dwBuildNumber >= 6001))) {
		HMODULE hLib = LoadLibrary(L"AVRT.DLL");
		if (hLib != NULL) {
			FreeLibrary(hLib);
			Trace("DirectSound: Disabled as WASAPI is available");
			return;
		}
	}
#endif

	airReg = new DXAudioInputRegistrar();
	aorReg = new DXAudioOutputRegistrar();
}

void DirectSoundInit::destroy() {
	SAFE_DELETE(airReg)
	SAFE_DELETE(aorReg)
}


DXAudioOutputRegistrar::DXAudioOutputRegistrar() : AudioOutputRegistrar(wstring(L"DirectSound"),0) {
}

AudioOutput *DXAudioOutputRegistrar::create() {
	return new DXAudioOutput();
}

typedef std::pair<wstring, GUID> dsDevice;

static BOOL CALLBACK DSEnumProc(LPGUID lpGUID, LPCWSTR lpszDesc,
								LPCWSTR, void *ctx) {
									if (lpGUID) {
										std::list<dsDevice> *l =reinterpret_cast<std::list<dsDevice> *>(ctx);
										l->push_back(dsDevice(lpszDesc, *lpGUID));
									}

									return(true);
}

const std::vector<audioDevice> DXAudioOutputRegistrar::getDeviceChoices() {
	std::list<dsDevice> qlOutput;

	//qlOutput.push_back(dsDevice(L"Default DirectSound Voice Output", DSDEVID_DefaultVoicePlayback));
	DirectSoundEnumerateW(DSEnumProc, reinterpret_cast<void *>(&qlOutput));

	std::vector<audioDevice> qlReturn;

	const GUID *lpguid = NULL;

	if (! g_struct.s.qbaDXOutput.empty()) {
		lpguid = reinterpret_cast<LPGUID>(&(g_struct.s.qbaDXOutput[0]));
	} else {
		lpguid = &DSDEVID_DefaultVoicePlayback;
	}

	//////////////////////////////////////////////////////////////////////////

	std::list<dsDevice>::iterator lit;
	for (lit = qlOutput.begin(); lit != qlOutput.end(); lit++)
	{
		if(lit->second == *lpguid)
		{
			char *p = (char*)&(lit->second);
			QByteArray qba;
			for(int i=0; i<sizeof(GUID); i++)
			{
				qba.push_back(p[i]);
			}
			qlReturn.push_back(audioDevice(lit->first,qba));
		}		
	}

	for (lit = qlOutput.begin(); lit != qlOutput.end(); lit++)
	{
		if(lit->second != *lpguid)
		{
			char *p = (char*)&(lit->second);
			QByteArray qba;
			for(int i=0; i<sizeof(GUID); i++)
			{
				qba.push_back(p[i]);
			}
			qlReturn.push_back(audioDevice(lit->first,qba));
		}		
	}

	//////////////////////////////////////////////////////////////////////////

	return qlReturn;
}

void DXAudioOutputRegistrar::setDeviceChoice(const QByteArray &choice, Settings &s) {
	s.qbaDXOutput = choice;
}

DXAudioInputRegistrar::DXAudioInputRegistrar() : AudioInputRegistrar(wstring(L"DirectSound"),0) {
}

AudioInput *DXAudioInputRegistrar::create() {
	return new DXAudioInput();
}

const std::vector<audioDevice> DXAudioInputRegistrar::getDeviceChoices() {
	std::list<dsDevice> qlInput;

	//qlInput.push_back(dsDevice(L"Default DirectSound Voice Input", DSDEVID_DefaultVoiceCapture));
	DirectSoundCaptureEnumerateW(DSEnumProc, reinterpret_cast<void *>(&qlInput));

	std::vector<audioDevice> qlReturn;

	const GUID *lpguid = NULL;

	if (! g_struct.s.qbaDXInput.empty()) {
		lpguid = reinterpret_cast<LPGUID>(&(g_struct.s.qbaDXInput[0]));
	} else {
		lpguid = &DSDEVID_DefaultVoiceCapture;
	}

	std::list<dsDevice>::iterator lit;
	for (lit = qlInput.begin(); lit != qlInput.end(); lit++)
	{
		if(lit->second == *lpguid)
		{
			char *p = (char*)&(lit->second);
			QByteArray qba;
			for(int i=0; i<sizeof(GUID); i++)
			{
				qba.push_back(p[i]);
			}
			qlReturn.push_back(audioDevice(lit->first,qba));
		}
	}

	for (lit = qlInput.begin(); lit != qlInput.end(); lit++)
	{
		if(lit->second != *lpguid)
		{
			char *p = (char*)&(lit->second);
			QByteArray qba;
			for(int i=0; i<sizeof(GUID); i++)
			{
				qba.push_back(p[i]);
			}
			qlReturn.push_back(audioDevice(lit->first,qba));
		}		
	}

	return qlReturn;
}

void DXAudioInputRegistrar::setDeviceChoice(const QByteArray &choice, Settings &s) {
	s.qbaDXInput = choice;
}

bool DXAudioInputRegistrar::canEcho(const wstring &) const {
	return true;
}

DXAudioOutput::DXAudioOutput() {
	bRunning = true;
	pDSBOutput = NULL;
}

DXAudioOutput::~DXAudioOutput() {
	bRunning = false;
	if (pDSBOutput) {
		pDSBOutput->Stop();
		pDSBOutput->Release();
	}
	pDSBOutput = NULL;
}

enum OS_VERSION//请不要调整顺序
{
	OS_UNKNOWN =    0,
	//OS_WIN95   =    1,
	//OS_WIN98   =    2,
	//OS_WIN98SE =    3,
	//OS_WINME   =    4,
	//OS_WINNT    =   5,
	OS_WIN2000 =    6,
	OS_WINXP   =    7,
	OS_WIN2003   =  8,
	//OS_WINXP64   =  9,
	OS_VISTA    =   10,
	OS_WIN2008 =11,
	OS_WIN2008R2 = 12,
	OS_WIN7    =    13,
};


/*
* 获取系统版本参数
* @param [in] 一个OSVERSIONINFO结构的引用
* @return TRUE表示成功，FALSE表示失败
*/
BOOL GetOsVersion(OSVERSIONINFO & osInfo)
{
	osInfo.dwOSVersionInfoSize = sizeof(osInfo);
	return GetVersionEx(&osInfo);
}

/*
* 判断当前系统是否等于某个版本
* @param 无
* @return OS_VERSION枚举中的一个值
*/
OS_VERSION GetOsVersion()
{
	OSVERSIONINFO osVer;
	if(GetOsVersion(osVer)==TRUE)
	{
		DWORD majorVer=osVer.dwMajorVersion;
		DWORD minorVer=osVer.dwMinorVersion;

		if( majorVer == 5 && minorVer == 0)
		{
			return  OS_WIN2000;
		}
		else if( majorVer == 5 && minorVer == 1)
		{
			return OS_WINXP;
		}
		else if( majorVer == 5 && minorVer == 2)
		{
			return OS_WIN2003;
		}
		else if( majorVer == 6 && minorVer == 0)
		{
			return OS_VISTA;
		}
		else if(majorVer == 6 && minorVer == 1)
		{
			return OS_WIN7;
		}
		else
		{
			return OS_UNKNOWN;
		}
	}
	else
	{
		Assert(0);
		return OS_UNKNOWN;
	}
}

void DXAudioOutput::Entry() {
	HRESULT hr;
	DSBUFFERDESC        dsbdesc;
	WAVEFORMATEXTENSIBLE wfx;
	WAVEFORMATEXTENSIBLE wfxSet;
	int ns = 0;
	unsigned int chanmasks[32];
	LPDIRECTSOUND8             pDS = NULL;
	LPDIRECTSOUNDBUFFER       pDSBPrimary = NULL;

	LPDIRECTSOUNDNOTIFY8       pDSNotify = NULL;

	DWORD	dwBufferSize;
	DWORD	dwLastWritePos;
	DWORD	dwLastPlayPos;
	DWORD	dwTotalPlayPos;
	int iLastwriteblock;
	LPVOID aptr1, aptr2;
	unsigned char* pBufferTemp = NULL;
	DWORD nbytes1, nbytes2;

	int playblock;
	int nowriteblock;
	DWORD dwPlayPosition, dwWritePosition;

	unsigned int iByteSize;
	iChannels = 1;

	DWORD dwSpeakerConfig;
	iMixerFreq = SAMPLE_RATE;
	eSampleFormat = SampleShort;
	bool failed = false;

	DWORD dwMask = 0;
	bool bHead = false;
			
	ZeroMemory(&dsbdesc, sizeof(DSBUFFERDESC));
	dsbdesc.dwSize  = sizeof(DSBUFFERDESC);
	dsbdesc.dwFlags = DSBCAPS_PRIMARYBUFFER;
	dsbdesc.dwBufferBytes = 0;
	dsbdesc.lpwfxFormat = NULL;
// 	if (GetOsVersion() > OS_WIN2000)
// 	{
		if (! g_struct.s.qbaDXOutput.empty()) {
			ErrTrace("DXAudioOutput:Trace1 , len = %d\n",g_struct.s.qbaDXOutput.size());
			LPGUID lpguid = reinterpret_cast<LPGUID>((char*)&g_struct.s.qbaDXOutput[0]);
			if (FAILED(hr = DirectSoundCreate8(lpguid, &pDS, NULL))) {
				ErrTrace("DXAudioOutput: DirectSoundCreate8 error\n");
				failed = true;
			}
			ErrTrace("DXAudioOutput prefered output device create ok\n");
		}
// 	}
// 	else
// 	{
// 		ErrTrace("DXAudioOutput os2000 use default output device\n");
// 	}
	
	ErrTrace("DXAudioOutput:DirectSoundCreate8 1\n");

	if (! pDS && FAILED(hr = DirectSoundCreate8(&DSDEVID_DefaultVoicePlayback, &pDS, NULL))) {
		ErrTrace("DXAudioOutput: DirectSoundCreate");
		goto cleanup;
	} else if (FAILED(hr = pDS->SetCooperativeLevel(GetDesktopWindow(), DSSCL_PRIORITY))) {
		ErrTrace("DXAudioOutput: SetCooperativeLevel");
		goto cleanup;
	} else if (FAILED(hr = pDS->CreateSoundBuffer(&dsbdesc, &pDSBPrimary, NULL))) {
		ErrTrace("DXAudioOutput: CreateSoundBuffer (Primary) : 0x%08lx", hr);
		goto cleanup;
	}
	ErrTrace("DXAudioOutput:DirectSoundCreate8,SetCooperativeLevel,CreateSoundBuffer ok\n");

	pDS->GetSpeakerConfig(&dwSpeakerConfig);


	switch (DSSPEAKER_CONFIG(dwSpeakerConfig)) {
		case DSSPEAKER_HEADPHONE:
			dwMask = KSAUDIO_SPEAKER_STEREO;
			bHead = true;
			break;
		case DSSPEAKER_MONO:
			dwMask = KSAUDIO_SPEAKER_MONO;
			break;
		case DSSPEAKER_QUAD:
			dwMask = KSAUDIO_SPEAKER_QUAD;
			break;
		case DSSPEAKER_STEREO:
			dwMask = KSAUDIO_SPEAKER_STEREO;
			break;
		case DSSPEAKER_SURROUND:
			dwMask = KSAUDIO_SPEAKER_SURROUND;
			break;
		case DSSPEAKER_5POINT1:
			dwMask = KSAUDIO_SPEAKER_5POINT1;
			break;
		case DSSPEAKER_7POINT1:
			dwMask = KSAUDIO_SPEAKER_7POINT1;
			break;
		case DSSPEAKER_7POINT1_SURROUND:
			dwMask = KSAUDIO_SPEAKER_7POINT1_SURROUND;
			break;
		case DSSPEAKER_5POINT1_SURROUND:
			dwMask = KSAUDIO_SPEAKER_5POINT1_SURROUND;
			break;
		default:
			dwMask = 0;
			break;
	}

	if (! g_struct.s.doPositionalAudio())
		dwMask = KSAUDIO_SPEAKER_STEREO;

	for (int i=0;i<32;i++) {
		if (dwMask & (1 << i)) {
			chanmasks[ns++] = 1 << i;
		}
	}

	//plus
	//ns = 1;

	iMixerFreq = SAMPLE_RATE;
	iChannels = ns;
	eSampleFormat = SampleShort;

	iByteSize = iFrameSize * sizeof(short) * ns;
	try
	{
		pBufferTemp = new unsigned char[iByteSize];
	}
	catch (...)
	{
		Assert(0);
		ErrTrace("DXAudioOutput::Entry() Bad memory alloc~~~~!!!\n");
		return;
	}
	
	ZeroMemory(&wfxSet, sizeof(wfxSet));
	wfxSet.Format.wFormatTag = WAVE_FORMAT_PCM;

	ZeroMemory(&wfx, sizeof(wfx));
	wfx.Format.wFormatTag = WAVE_FORMAT_PCM;
	wfx.Format.nChannels = max(ns, 2);
	wfx.Format.nSamplesPerSec = SAMPLE_RATE;	
	wfx.Format.wBitsPerSample = 16;
	wfx.Format.nBlockAlign = wfx.Format.nChannels * wfx.Format.wBitsPerSample/8;
	wfx.Format.nAvgBytesPerSec = wfx.Format.nSamplesPerSec * wfx.Format.nBlockAlign;

	if (FAILED(hr = pDSBPrimary->SetFormat(reinterpret_cast<WAVEFORMATEX *>(&wfx)))) {
		ErrTrace("DXAudioOutput: SetFormat\n");
		goto cleanup;
	}
	if (FAILED(hr = pDSBPrimary->GetFormat(reinterpret_cast<WAVEFORMATEX *>(&wfxSet), sizeof(wfxSet), NULL))) {
		ErrTrace("DXAudioOutput: GetFormat\n");
		goto cleanup;
	}

	ErrTrace("DXAudioOutput: Primary buffer of %ld Hz, %d channels, %d bits\n",wfxSet.Format.nSamplesPerSec,wfxSet.Format.nChannels,wfxSet.Format.wBitsPerSample);

	ZeroMemory(&wfx, sizeof(wfx));
	wfx.Format.wFormatTag = WAVE_FORMAT_EXTENSIBLE;
	wfx.Format.nChannels = max(ns, 2);
	wfx.Format.nSamplesPerSec = SAMPLE_RATE;
	wfx.Format.wBitsPerSample = 16;
	wfx.Format.nBlockAlign = wfx.Format.nChannels * wfx.Format.wBitsPerSample/8;
	wfx.Format.nAvgBytesPerSec = wfx.Format.nSamplesPerSec * wfx.Format.nBlockAlign;	
	wfx.Format.cbSize = 32;
	wfx.Samples.wValidBitsPerSample = wfx.Format.wBitsPerSample;
	wfx.dwChannelMask = dwMask;
	wfx.SubFormat = KSDATAFORMAT_SUBTYPE_PCM;

	ZeroMemory(&dsbdesc, sizeof(DSBUFFERDESC));
	dsbdesc.dwSize          = sizeof(DSBUFFERDESC);
	dsbdesc.dwFlags         = DSBCAPS_GLOBALFOCUS|DSBCAPS_GETCURRENTPOSITION2;
	dsbdesc.dwFlags		   |= DSBCAPS_CTRLPOSITIONNOTIFY;
	dsbdesc.dwBufferBytes = wfx.Format.nChannels * iFrameSize * sizeof(short) * NBLOCKS;
	dsbdesc.lpwfxFormat     = reinterpret_cast<WAVEFORMATEX *>(&wfx);

	if (FAILED(hr = pDS->CreateSoundBuffer(&dsbdesc, (LPDIRECTSOUNDBUFFER*)&pDSBOutput, NULL))) {
		ErrTrace("DXAudioOutputUser: CreateSoundBuffer (Secondary): 0x%08lx Error\n", hr);
		goto cleanup;
	}

	if (FAILED(hr = pDSBOutput->QueryInterface(IID_IDirectSoundNotify, reinterpret_cast<void **>(&pDSNotify)))) {
		ErrTrace("DXAudioOutputUser: QueryInterface (Notify) Error\n");
		goto cleanup;
	}

	ErrTrace("DXAudioOutputUser: New %dHz output buffer of %ld bytes\n", SAMPLE_RATE, dsbdesc.dwBufferBytes);

	if (failed)
	{
		ErrTrace("Opening chosen DirectSound Output failed. Default device will be used.Error\n");
		AV_Audio_Fail_Event* pAudioFailEvent = new AV_Audio_Fail_Event(0,kAudio_Err_Output);
		if (!g_pAVModule->sendEvent(static_cast<void*>(pAudioFailEvent), EVENT_AV_AUDIO_FAIL))
		{
			Assert(0);
			delete pAudioFailEvent;
		}
	}

	ErrTrace("DXAudioOutput:directsound  initialized \n");

	initializeMixer(chanmasks, bHead);

	if (FAILED(hr = pDSBOutput->Lock(0, 0, &aptr1, &nbytes1, &aptr2, &nbytes2, DSBLOCK_ENTIREBUFFER))) {
		ErrTrace("DXAudioOutputUser: Initial Lock Error\n");
		goto cleanup;
	}

	dwBufferSize = nbytes1 + nbytes2;
	if (aptr1)
		ZeroMemory(aptr1, nbytes1);
	if (aptr2)
		ZeroMemory(aptr2, nbytes2);

	if (FAILED(hr = pDSBOutput->Unlock(aptr1, nbytes1, aptr2, nbytes2))) {
		ErrTrace("DXAudioOutputUser: Initial Unlock Error\n");
		goto cleanup;
	}

	if (FAILED(hr = pDSBOutput->Play(0, 0, DSBPLAY_LOOPING))) {
		ErrTrace("DXAudioOutputUser: Play Error\n");
		goto cleanup;
	}

	dwLastWritePos = 0;
	dwLastPlayPos = 0;
	dwTotalPlayPos = 0;

	iLastwriteblock = (NBLOCKS - 1 + g_struct.s.iOutputDelay) % NBLOCKS;
	DWORD dwStart = GetTickCount();

	try
	{
		SetRunningState(true);
		while (IsRunningState() && !IsStopRequested() && bRunning && ! FAILED(hr)) {
			try
			{
				if (FAILED(hr = pDSBOutput->GetCurrentPosition(&dwPlayPosition, &dwWritePosition))) {
					ErrTrace("DXAudioOutputUser: GetCurrentPosition Error");
					break;
				}
			}
			catch (...)
			{
				ErrTrace("DXAudioOutputUser: GetCurrentPosition Exception");
				break;
			}

			playblock = dwWritePosition / iByteSize;
			nowriteblock = (playblock + g_struct.s.iOutputDelay + 1) % NBLOCKS;

			for (int block=(iLastwriteblock + 1) % NBLOCKS;(!FAILED(hr)) && (block!=nowriteblock);block=(block + 1) % NBLOCKS) {
				iLastwriteblock = block;

				if (FAILED(hr = pDSBOutput->Lock(block * iByteSize, iByteSize, &aptr1, &nbytes1, &aptr2, &nbytes2, 0))) {
					ErrTrace("DXAudioOutput: Lock block %u (%d bytes) Error",block, iByteSize);
					break;
				}
				if (aptr2 || nbytes2) {
					ErrTrace("DXAudioOutput: Split buffer Error");
					break;
				}
				if (!aptr1 || ! nbytes1) {
					ErrTrace("DXAudioOutput: Zerolock Error");
					break;
				}
				//获取混音后的音频数据
				
				if (! mix(pBufferTemp, iFrameSize*iChannels))
				{
					ZeroMemory(aptr1, nbytes1);
					if (aptr2 && (iByteSize-nbytes1>0))
						ZeroMemory(aptr2,min(iByteSize-nbytes1,nbytes2));
					//OutputDebugStringA("mix   .....failed\n");
				}
				else
				{
					memcpy(aptr1,pBufferTemp,nbytes1);
					if (aptr2 && (iByteSize-nbytes1>0))
						memcpy(aptr2,pBufferTemp+nbytes1,min(nbytes2,iByteSize-nbytes1));
				}
				{
					Trace("音频播放间隔时间 %d \n",GetTickCount()-dwStart);
					dwStart = GetTickCount();
				}

				static unsigned char btForLevel=0;

				//计算音强，不需要每次都计算（浮点运算耗时）
				if(btForLevel++%10==0)
				{
					DWORD dwSize = iFrameSize*iChannels;
					double lLevel = 0;
					int register i = 0;
					float sample = 0;
					for (i=0; i<(long)dwSize / 2; i++)
					{
						sample = (float)abs(int(*((short*)aptr1+i)));
						lLevel += sample;
					}
					iLevel = short(lLevel * 255 * 20 / (i*0xffff)); //255为音强输出范围，20为增强20倍，不增强效果不明显
				}

				//Trace("输出语音音量 %d \n",iLevel);

				if (FAILED(hr = pDSBOutput->Unlock(aptr1, nbytes1, aptr2, nbytes2))) {
					ErrTrace("DXAudioOutput: Unlock %p(%lu) %p(%lu) Error\n",aptr1,nbytes1,aptr2,nbytes2);
					break;
				}

				if (FAILED(hr = pDSBOutput->GetCurrentPosition(&dwPlayPosition, &dwWritePosition))) {
					ErrTrace("DXAudioOutputUser: GetCurrentPosition Error1\n");
					break;
				}

				playblock = dwWritePosition / iByteSize;
				nowriteblock = (playblock + g_struct.s.iOutputDelay + 1) % NBLOCKS;

				// 			if (GetTickCount() - dwStart < 19)
				// 			{
				// 				Sleep(19-GetTickCount()+dwStart);
				// 			}

			}
			if (! FAILED(hr))
				Sleep(19);
		}
	}
	catch (...)
	{
		ErrTrace("DXAudioOutput while Err\n");
	}

	if (FAILED(hr)) {
		Assert(L"Lost DirectSound output device.Error");
	}
cleanup:

	if (pDSNotify)
	{
		pDSNotify->Release();
	    pDSNotify = NULL;
	}
	if (pDSBOutput) 
	{
		pDSBOutput->Stop();
		pDSBOutput->Release();
		pDSBOutput = NULL;
	}
	if (pDSBPrimary)
	{
		pDSBPrimary->Release();
	    pDSBPrimary = NULL;
	}
	if (pDS)
	{
		pDS->Release();
	    pDS = NULL;
	}
	SAFE_DELETEA(pBufferTemp);

	CancelStopRequest();
	SetRunningState(false);
	_endthreadex((UINT32)m_nThreadID);

	//return true;
}

void  DXAudioOutput::setVolume(int iVolume)
{
	if (pDSBOutput)
	{
		//LONG lVolume = (iVolume - 255)*5000/255;
		int lVolume = -5000L + (DSBVOLUME_MAX - (-5000L)) * iVolume/255;
		HRESULT hr = pDSBOutput->SetVolume(lVolume);
		if (FAILED(hr))
		{
			wstring strError = DXGetErrorString8(hr);
			ErrTrace("Set volume error %s. Error",DXGetErrorString8(hr));
		}
	}
}

int DXAudioOutput::getVolume()
{
	if (pDSBOutput)
	{
		LONG lVolume;
		HRESULT hr = pDSBOutput->GetVolume(&lVolume);
		if (FAILED(hr))
		{
			wstring strError = DXGetErrorString8(hr);
			ErrTrace("Get volume error %s.",DXGetErrorString8(hr));
			return -1;
		}

		int iVolume = (lVolume + 5000L)*255 / (DSBVOLUME_MAX + 5000L);
		return iVolume;
	}
	return -1;
}

bool DXAudioOutput::setPlayerMute(bool bMute)
{
	return m_volumectr.SetChannelMute(MainChannel,bMute);
}

bool DXAudioOutput::getPlayerMute(bool& bMute)
{
	return m_volumectr.GetChannelMute(MainChannel,bMute);
}

#define NBUFFBLOCKS 100

DXAudioInput::DXAudioInput() {
	bRunning = true;
	pDSCaptureBuffer = NULL;
}

DXAudioInput::~DXAudioInput() {
	bRunning = false;
	if (pDSCaptureBuffer)
		pDSCaptureBuffer->Release();
	pDSCaptureBuffer = NULL;
	ErrTrace("DXAudioInput::~DXAudioInput Error\n");
}

void DXAudioInput::Entry() {
	LPDIRECTSOUNDCAPTURE8      pDSCapture;

	pDSCapture = NULL;
	pDSCaptureBuffer = NULL;

	DWORD dwBufferSize;
	bool bOk = false;
	DWORD dwReadyBytes = 0;
	DWORD dwLastReadPos = 0;
	DWORD dwReadPosition;
	DWORD dwCapturePosition;

	LPVOID aptr1, aptr2;
	DWORD nbytes1, nbytes2;
	
	HRESULT       hr;
	WAVEFORMATEX  wfx;
	DSCBUFFERDESC dscbd;

	int iSampleSize = iFrameSize*2*sizeof(short);

	ZeroMemory(&wfx, sizeof(wfx));
	wfx.wFormatTag = WAVE_FORMAT_PCM;

	ZeroMemory(&dscbd, sizeof(dscbd));
	dscbd.dwSize = sizeof(dscbd);

	dscbd.dwBufferBytes = dwBufferSize = iFrameSize * sizeof(short) * NBUFFBLOCKS;
	dscbd.lpwfxFormat = &wfx;

	wfx.nChannels = 2;
	wfx.nSamplesPerSec = iSampleRate;
	wfx.wBitsPerSample = 16;
	wfx.nBlockAlign = (wfx.nChannels*wfx.wBitsPerSample)/8;
	wfx.nAvgBytesPerSec = wfx.nSamplesPerSec * wfx.nBlockAlign;

	// Create IDirectSoundCapture using the preferred capture device
	if (GetOsVersion() > OS_WIN2000)
	{
		if (! g_struct.s.qbaDXInput.empty()) {
			ErrTrace("DXAudioInput::Create IDirectSoundCapture using the preferred capture device\n");
			LPGUID lpguid = reinterpret_cast<LPGUID>(&(g_struct.s.qbaDXInput[0]));
			if (FAILED(hr = DirectSoundCaptureCreate8(lpguid, &pDSCapture, NULL)))
			{
				Assert(0);
				ErrTrace("DXAudioInput: fail create directsound capture use preferred capture device\n");
			}
		}
	}
	else
	{
		ErrTrace("DXAudioInput: OS2000 use default capture device\n");
	}
	
	try
	{
		ErrTrace("DXAudioInput: DirectSoundCaptureCreate8 use default device\n");
		if(! pDSCapture && FAILED(DirectSoundCaptureCreate8(&DSDEVID_DefaultVoiceCapture, &pDSCapture, NULL)))
		{
			Assert(0);
			ErrTrace("DXAudioInput: DirectSoundCaptureCreate use default device error\n");
		}
		else
		{
			ErrTrace("DXAudioInput: CreateCaptureBuffer\n");
			if(FAILED(pDSCapture->CreateCaptureBuffer(&dscbd, (LPDIRECTSOUNDCAPTUREBUFFER *)&pDSCaptureBuffer, NULL)))
			{
				Assert(0);
				ErrTrace("DXAudioInput: CreateCaptureBuffer error \n");
			}
			else
			{
				bOk = true;
			}
		}
	}
	catch (...)
	{
		Assert(0);
		ErrTrace("DirectSoundCaptureCreate8 OR CreateCaptureBuffer err!\n");
		goto cleanup;
	}


	ErrTrace("Trace2\n");
	if (!bOk)
	{
		Assert(L"Opening chosen DirectSound Input failed. Default device will be used.");
		AV_Audio_Fail_Event* pAudioFailEvent = new AV_Audio_Fail_Event(0,kAudio_Err_Input);
		if (!g_pAVModule->sendEvent(static_cast<void*>(pAudioFailEvent), EVENT_AV_AUDIO_FAIL))
		{
			Assert(0);
			delete pAudioFailEvent;
		}
	}

	if (pDSCaptureBuffer == NULL)
	{
		Assert(0);
		ErrTrace("DXAudioInput: Initialize error");
	}

	ErrTrace("DXAudioInput: Initialized");

	if (! bOk)
		goto cleanup;

	try
	{
		hr = pDSCaptureBuffer->Start(DSCBSTART_LOOPING);
	}
	catch(...)
	{
		ErrTrace("pDSCaptureBuffer->Start Err\n");
		Assert(0);
		goto cleanup;
	}
	
	if (FAILED(hr)) 
	{
		ErrTrace("DXAudioInput: Start failed");
		goto cleanup;
	} 
	else 
	{
		SetRunningState(true);
		while (IsRunningState() && bRunning && !IsStopRequested()) 
		{
			//保证至少采样一帧数据
			do {
				try
				{
					if ( FAILED(hr = pDSCaptureBuffer->GetCurrentPosition(&dwCapturePosition, &dwReadPosition))) 
					{
						ErrTrace("DXAudioInput: GetCurrentPosition");
						bRunning = false;
						break;
					}
				}
				catch (...)
				{
					Assert(0);
					ErrTrace("GetCurrentPosition error!\n");
					break;
				}
				if (dwReadPosition < dwLastReadPos)
					dwReadyBytes = (dwBufferSize - dwLastReadPos) + dwReadPosition;
				else
					dwReadyBytes = dwReadPosition - dwLastReadPos;

				if (static_cast<int>(dwReadyBytes) < sizeof(short)*iFrameSize*wfx.nChannels) {
					Sleep(19);
				}
			} while (static_cast<int>(dwReadyBytes) < sizeof(short)*iFrameSize*wfx.nChannels  && !IsStopRequested() && bRunning && IsRunningState());

			// Desynchonized?
			if (dwReadyBytes > (dwBufferSize / 2)) {
				ErrTrace("DXAudioInput: Lost synchronization");
				dwLastReadPos = dwReadPosition;
			} 
			else if (bRunning && !m_bStopRequested) {
				try
				{
					if (FAILED(hr = pDSCaptureBuffer->Lock(dwLastReadPos, sizeof(short)*iFrameSize*wfx.nChannels, &aptr1, &nbytes1, &aptr2, &nbytes2, 0))) {
						ErrTrace("DXAudioInput: Lock from %ld (%d bytes)",dwLastReadPos, sizeof(short)*iFrameSize*wfx.nChannels);
						bRunning = false;
						break;
					}
				}
				catch (...)
				{
					Assert(0);
					ErrTrace("pDSCaptureBuffer->Lock Error!\n");
					break;
				}

				if (aptr1 && nbytes1)
					CopyMemory(psMic, aptr1, min(nbytes1,iSampleSize));
				
				if (aptr2 && nbytes2 && nbytes1<iSampleSize)
					CopyMemory((byte*)psMic+nbytes1, aptr2, min(nbytes2,iSampleSize-nbytes1));

				if (g_struct.GetMicMute())
				{
					memset((void*)psMic,0,nbytes1+nbytes2);
				}
				
				const float mul = g_struct.s.fMicVolume * VOLUMEMIXGAIN;
				
				if(mul != 1.0f)
				{
					for (int j=0; j<iFrameSize*wfx.nChannels;j++)
					{
						psMic[j] = (short)(psMic[j] * mul);
					}
				}

// 				celt_int16 fout2[2560]={0};
// 				UINT dwDataWrote;
// 				if( FAILED(g_pWaveFile.Write( SAMPLE_RATE / SAMPLE_DIV*2*2, (BYTE*)psMic, 
// 					&dwDataWrote ) ))
// 				{
// 					int a=0;
// 					a++;
// 				}
// 				else
// 				{
// 					OutputDebugString(L"plushuwav g_pWaveFile.Write 3");				
// 				}


				if (g_struct.po && !g_struct.GetMicMute())
				{
					g_struct.po->Prepare((byte*)psMic,nbytes1+nbytes2);
				}

				static unsigned char btForLevel=0;

				//计算音强，不需要每次都计算（浮点运算耗时）
				if(btForLevel++%10==0)
				{
					DWORD dwSize = nbytes1 + nbytes2;
					double lLevel = 0;
					int register i = 0;
					float sample = 0;
					for (i=0; i<(long)dwSize / 2; i++)
					{
						sample = (float)(abs((int)psMic[i]));
					lLevel += abs(sample);
					}
					iLevel = short(lLevel * 255 * 20 / (i*0xffff));//255为输入音强范围，20为增强20倍，不增强效果不明显
				}
				
				try
				{
					if (FAILED(hr = pDSCaptureBuffer->Unlock(aptr1, nbytes1, aptr2, nbytes2))) {
						ErrTrace("DXAudioInput: Unlock\n");
						bRunning = false;
						break;
					}
				}
				catch (...)
				{
					ErrTrace("pDSCaptureBuffer->Unlock \n");
					Assert(0);
					break;
				}
				dwLastReadPos = (dwLastReadPos + sizeof(short)*iFrameSize*wfx.nChannels) % dwBufferSize;

				if (!g_struct.GetMicMute()) 
					encodeAudioFrame();
			}
		}
		try
		{
			pDSCaptureBuffer->Stop();
		}
		catch (...)
		{
			ErrTrace("pDSCaptureBuffer->Stop() Err\n");
			Assert(0);
		}
	}

	if (FAILED(hr)) {
		ErrTrace("Lost DirectSound input device.\n");
	}
cleanup:
	if (! bOk) {
		ErrTrace("Opening chosen DirectSound Input device failed. No microphone capture will be done.\n");
	}
	try
	{
		if (pDSCaptureBuffer)
		{
			pDSCaptureBuffer->Release();
			pDSCaptureBuffer = NULL;
		}
		if (pDSCapture)
		{
			pDSCapture->Release();
			pDSCapture = NULL;
		}



	}
	catch (...)
	{
		ErrTrace("pDSCaptureBuffer->Release() Error\n");
		Assert(0);
	}
	CancelStopRequest();
	ErrTrace("DXAudioInput:pDSCapture release\n");
	//SetRunningState(false);
	m_bThreadRunning = false;
	ErrTrace("DXAudioInput:pDSCapture release 1\n");
	_endthreadex((UINT32)m_nThreadID);
	ErrTrace("DXAudioInput:pDSCapture _endthreadex\n");
}


bool DXAudioInput::SetMicrPhoneVolume(DWORD dwVolume)
{
	BOOL bResult = FALSE;
	try
	{
		int mixerNum = (int)mixerGetNumDevs();
		for(int i = 0; i < mixerNum; i ++)
		{
			DWORD dwSource = 0;
			HMIXER m_hMixer = NULL;   
			MMRESULT mr = NULL;
			//打开Mixer设备   
			if(mixerOpen(&m_hMixer, i, 0, 0, MIXER_OBJECTF_MIXER) != MMSYSERR_NOERROR)
				continue; 

			MIXERLINE MixerLine;   
			//结构体的大小   
			MixerLine.cbStruct = sizeof(MixerLine);   
			//录制设备总线   
			MixerLine.dwComponentType   =   MIXERLINE_COMPONENTTYPE_DST_WAVEIN;   
			//得到录制总线中的连接数   
			if(mixerGetLineInfo((HMIXEROBJ)m_hMixer, &MixerLine,
				MIXER_OBJECTF_HMIXER | MIXER_GETLINEINFOF_COMPONENTTYPE) != MMSYSERR_NOERROR)
				continue; 

			//将连接数保存   
			DWORD dwConnections = MixerLine.cConnections;   
			//准备获取麦克风设备的ID   
			DWORD dwLineID = 0;   
			for(DWORD i = 0; i < dwConnections; i++ )   
			{   
				//枚举每一个设备，当Source的ID等于当前的迭代记数   
				MixerLine.dwSource = i;   
				//根据SourceID获得连接的信息   
				if(mixerGetLineInfo((HMIXEROBJ)m_hMixer, &MixerLine,   
					MIXER_OBJECTF_HMIXER | MIXER_GETLINEINFOF_SOURCE) != MMSYSERR_NOERROR) 
					//判断函数执行错误   
					break;  

				//如果当前设备类型是麦克风，则跳出循环。   
				if(MixerLine.dwComponentType == MIXERLINE_COMPONENTTYPE_SRC_MICROPHONE)   
				{   
					dwLineID = MixerLine.dwLineID;
					dwSource = MixerLine.dwSource;
					break;   
				}   
			}

			//如果没有找到，返回失败。   
			if(dwLineID == 0)   
				continue;   

			//get line id of destination line   
			MIXERLINE lineinfo_Dest;   
			::ZeroMemory(&lineinfo_Dest, sizeof(MIXERLINE));   
			lineinfo_Dest.cbStruct = sizeof(MIXERLINE);   
			lineinfo_Dest.dwComponentType = MIXERLINE_COMPONENTTYPE_DST_WAVEIN;   
			if(::mixerGetLineInfo(   
				(HMIXEROBJ)m_hMixer,   
				&lineinfo_Dest,   
				MIXER_OBJECTF_HMIXER   |     
				MIXER_GETLINEINFOF_COMPONENTTYPE) != MMSYSERR_NOERROR)
				continue;  

			//get id of specified connector   
			MIXERLINE lineinfo_Connector;   
			::ZeroMemory(&lineinfo_Connector, sizeof(MIXERLINE));  

			lineinfo_Connector.cbStruct = sizeof(MIXERLINE);   
			lineinfo_Connector.dwDestination = lineinfo_Dest.dwLineID;   
			lineinfo_Connector.dwSource = dwSource;   
			if(::mixerGetLineInfo(   
				(HMIXEROBJ)m_hMixer,   
				&lineinfo_Connector,     
				MIXER_GETLINEINFOF_SOURCE) != MMSYSERR_NOERROR)
				continue;  

			MIXERCONTROL mxc;   
			MIXERLINECONTROLS mxlc;   
			mxlc.cbStruct = sizeof(MIXERLINECONTROLS);   
			mxlc.dwLineID = lineinfo_Connector.dwLineID;   
			mxlc.dwControlType = MIXERCONTROL_CONTROLTYPE_VOLUME;   
			mxlc.cControls = 1;   
			mxlc.cbmxctrl = sizeof(MIXERCONTROL);   
			mxlc.pamxctrl = &mxc;   
			if(::mixerGetLineControls(   
				reinterpret_cast<HMIXEROBJ>(m_hMixer),   
				&mxlc,   
				MIXER_OBJECTF_HMIXER|MIXER_GETLINECONTROLSF_ONEBYTYPE)!= MMSYSERR_NOERROR)
				continue;  

			//get volume value range now   
			MIXERCONTROLDETAILS_UNSIGNED mxcdVolume;   
			MIXERCONTROLDETAILS mxcd;   
			mxcd.cbStruct = sizeof(MIXERCONTROLDETAILS);   
			mxcd.dwControlID = mxc.dwControlID;   
			mxcd.cChannels = 1;   
			mxcd.cMultipleItems = 0;   
			mxcd.cbDetails = sizeof(MIXERCONTROLDETAILS_UNSIGNED);   
			mxcd.paDetails = &mxcdVolume;   
			if(mixerGetControlDetails(   
				reinterpret_cast<HMIXEROBJ>(m_hMixer),   
				&mxcd,   
				MIXER_GETCONTROLDETAILSF_VALUE)!= MMSYSERR_NOERROR)
				continue;  

			//set volume   
			MIXERCONTROLDETAILS_UNSIGNED mxcdVolume_Set = { mxc.Bounds.dwMaximum * dwVolume / 255 };   
			MIXERCONTROLDETAILS mxcd_Set;   
			mxcd_Set.cbStruct = sizeof(MIXERCONTROLDETAILS);   
			mxcd_Set.dwControlID = mxc.dwControlID;   
			mxcd_Set.cChannels = 1;   
			mxcd_Set.cMultipleItems = 0;   
			mxcd_Set.cbDetails = sizeof(MIXERCONTROLDETAILS_UNSIGNED);   
			mxcd_Set.paDetails = &mxcdVolume_Set;   
			if(mixerSetControlDetails(reinterpret_cast<HMIXEROBJ>(m_hMixer),   
				&mxcd_Set,   
				MIXER_OBJECTF_HMIXER | MIXER_SETCONTROLDETAILSF_VALUE) != MMSYSERR_NOERROR)
				continue;   

			::mixerClose(m_hMixer);
			m_hMixer = NULL;
			bResult = TRUE;;
		}
	}
	catch(...){ return FALSE; }
	return bResult;
}

int DXAudioInput::GetMicrPhoneVolume()
{
	int iResult = 0;
	try
	{
		int mixerNum = (int)mixerGetNumDevs();
		if (mixerNum < 0)
		{
			return iResult;
		}

		DWORD dwSource = 0;
		HMIXER m_hMixer = NULL;   
		MMRESULT mr = NULL;
		//打开Mixer设备   
		if(mixerOpen(&m_hMixer, 0, 0, 0, MIXER_OBJECTF_MIXER) != MMSYSERR_NOERROR)
			return iResult; 

		MIXERLINE MixerLine;   
		//结构体的大小   
		MixerLine.cbStruct = sizeof(MixerLine);   
		//录制设备总线   
		MixerLine.dwComponentType   =   MIXERLINE_COMPONENTTYPE_DST_WAVEIN;   
		//得到录制总线中的连接数   
		if(mixerGetLineInfo((HMIXEROBJ)m_hMixer, &MixerLine,
			MIXER_OBJECTF_HMIXER | MIXER_GETLINEINFOF_COMPONENTTYPE) != MMSYSERR_NOERROR)
			return iResult; 

		//将连接数保存   
		DWORD dwConnections = MixerLine.cConnections;   
		//准备获取麦克风设备的ID   
		DWORD dwLineID = 0;   
		for(DWORD i = 0; i < dwConnections; i++ )   
		{   
			//枚举每一个设备，当Source的ID等于当前的迭代记数   
			MixerLine.dwSource = i;   
			//根据SourceID获得连接的信息   
			if(mixerGetLineInfo((HMIXEROBJ)m_hMixer, &MixerLine,   
				MIXER_OBJECTF_HMIXER | MIXER_GETLINEINFOF_SOURCE) != MMSYSERR_NOERROR) 
				//判断函数执行错误   
				break;  

			//如果当前设备类型是麦克风，则跳出循环。   
			if(MixerLine.dwComponentType == MIXERLINE_COMPONENTTYPE_SRC_MICROPHONE)   
			{   
				dwLineID = MixerLine.dwLineID;
				dwSource = MixerLine.dwSource;
				break;   
			}   
		}

		//如果没有找到，返回失败。   
		if(dwLineID == 0)   
			return iResult;   

		//get line id of destination line   
		MIXERLINE lineinfo_Dest;   
		::ZeroMemory(&lineinfo_Dest, sizeof(MIXERLINE));   
		lineinfo_Dest.cbStruct = sizeof(MIXERLINE);   
		lineinfo_Dest.dwComponentType = MIXERLINE_COMPONENTTYPE_DST_WAVEIN;   
		if(::mixerGetLineInfo(   
			(HMIXEROBJ)m_hMixer,   
			&lineinfo_Dest,   
			MIXER_OBJECTF_HMIXER   |     
			MIXER_GETLINEINFOF_COMPONENTTYPE) != MMSYSERR_NOERROR)
			return iResult;  

		//get id of specified connector   
		MIXERLINE lineinfo_Connector;   
		::ZeroMemory(&lineinfo_Connector, sizeof(MIXERLINE));  

		lineinfo_Connector.cbStruct = sizeof(MIXERLINE);   
		lineinfo_Connector.dwDestination = lineinfo_Dest.dwLineID;   
		lineinfo_Connector.dwSource = dwSource;   
		if(::mixerGetLineInfo(   
			(HMIXEROBJ)m_hMixer,   
			&lineinfo_Connector,     
			MIXER_GETLINEINFOF_SOURCE) != MMSYSERR_NOERROR)
			return iResult;  

		MIXERCONTROL mxc;   
		MIXERLINECONTROLS mxlc;   
		mxlc.cbStruct = sizeof(MIXERLINECONTROLS);   
		mxlc.dwLineID = lineinfo_Connector.dwLineID;   
		mxlc.dwControlType = MIXERCONTROL_CONTROLTYPE_VOLUME;   
		mxlc.cControls = 1;   
		mxlc.cbmxctrl = sizeof(MIXERCONTROL);   
		mxlc.pamxctrl = &mxc;   
		if(::mixerGetLineControls(   
			reinterpret_cast<HMIXEROBJ>(m_hMixer),   
			&mxlc,   
			MIXER_OBJECTF_HMIXER|MIXER_GETLINECONTROLSF_ONEBYTYPE)!= MMSYSERR_NOERROR)
			return iResult;  

		//get volume value range now   
		MIXERCONTROLDETAILS_UNSIGNED mxcdVolume;   
		MIXERCONTROLDETAILS mxcd;   
		mxcd.cbStruct = sizeof(MIXERCONTROLDETAILS);   
		mxcd.dwControlID = mxc.dwControlID;   
		mxcd.cChannels = 1;   
		mxcd.cMultipleItems = 0;   
		mxcd.cbDetails = sizeof(MIXERCONTROLDETAILS_UNSIGNED);   
		mxcd.paDetails = &mxcdVolume;   
		if(mixerGetControlDetails(   
			reinterpret_cast<HMIXEROBJ>(m_hMixer),   
			&mxcd,   
			MIXER_GETCONTROLDETAILSF_VALUE)!= MMSYSERR_NOERROR)
			return iResult;  
		else
			iResult =mxcdVolume.dwValue * 255 / mxc.Bounds.dwMaximum; 

		::mixerClose(m_hMixer);
		m_hMixer = NULL;
	}
	catch(...){ return FALSE; }
	return iResult;
}

bool DXAudioInput::SetMicInputMute(bool bMute)
{
	if (mixerGetNumDevs() < 0)
	{
		return false;
	}

	DWORD dwSource = 0;
	HMIXER m_hMixer = NULL;   
	MMRESULT mr = NULL;

	//打开Mixer设备   
	if(mixerOpen(&m_hMixer, 0, 0, 0, MIXER_OBJECTF_MIXER) != MMSYSERR_NOERROR)
		return false; 

	MIXERLINE MixerLine;   
	//结构体的大小   
	MixerLine.cbStruct = sizeof(MixerLine);   
	//录制设备总线   
	MixerLine.dwComponentType   =   MIXERLINE_COMPONENTTYPE_DST_WAVEIN;   
	//得到录制总线中的连接数   
	if(mixerGetLineInfo((HMIXEROBJ)m_hMixer, &MixerLine,
		MIXER_OBJECTF_HMIXER | MIXER_GETLINEINFOF_COMPONENTTYPE) != MMSYSERR_NOERROR)
		return false; 

	//将连接数保存   
	DWORD dwConnections = MixerLine.cConnections;   
	//准备获取麦克风设备的ID   
	DWORD dwLineID = 0;   
	for(DWORD i = 0; i < dwConnections; i++ )   
	{   
		//枚举每一个设备，当Source的ID等于当前的迭代记数   
		MixerLine.dwSource = i;   
		//根据SourceID获得连接的信息   
		if(mixerGetLineInfo((HMIXEROBJ)m_hMixer, &MixerLine,   
			MIXER_OBJECTF_HMIXER | MIXER_GETLINEINFOF_SOURCE) != MMSYSERR_NOERROR) 
			//判断函数执行错误   
			break;  

		//如果当前设备类型是麦克风，则跳出循环。   
		if(MixerLine.dwComponentType == MIXERLINE_COMPONENTTYPE_SRC_MICROPHONE)   
		{   
			dwLineID = MixerLine.dwLineID;
			dwSource = MixerLine.dwSource;
			break;   
		}   
	}

	//如果没有找到，返回失败。   
	if(dwLineID == 0)  return false;   

	// Find a mute control, if any, of the microphone line
	LPMIXERCONTROL pmxctrl = (LPMIXERCONTROL)malloc(sizeof MIXERCONTROL);
	MIXERLINECONTROLS mxlctrl = {sizeof mxlctrl, MixerLine.dwLineID,
		MIXERCONTROL_CONTROLTYPE_MUTE, 1, sizeof MIXERCONTROL, pmxctrl};
	if(!mixerGetLineControls((HMIXEROBJ) m_hMixer, &mxlctrl,
		MIXER_GETLINECONTROLSF_ONEBYTYPE)){
			// Found, so proceed
			DWORD cChannels = MixerLine.cChannels;
			if (MIXERCONTROL_CONTROLF_UNIFORM & pmxctrl->fdwControl)
				cChannels = 1;

			LPMIXERCONTROLDETAILS_BOOLEAN pbool =
				(LPMIXERCONTROLDETAILS_BOOLEAN) malloc(cChannels * sizeof
				MIXERCONTROLDETAILS_BOOLEAN);
			MIXERCONTROLDETAILS mxcd = {sizeof(mxcd), pmxctrl->dwControlID,
				cChannels, (HWND)0,
				sizeof MIXERCONTROLDETAILS_BOOLEAN, (LPVOID) pbool};
			mixerGetControlDetails((HMIXEROBJ)m_hMixer, &mxcd,
				MIXER_SETCONTROLDETAILSF_VALUE);
			// Unmute the microphone line (for both channels)
			pbool[0].fValue = pbool[cChannels - 1].fValue = bMute?0:1;
			mixerSetControlDetails((HMIXEROBJ)m_hMixer, &mxcd,
				MIXER_SETCONTROLDETAILSF_VALUE);

			free(pmxctrl);
			free(pbool);
	}
	else
		free(pmxctrl);

	mixerClose(m_hMixer);

	return true;
}

bool DXAudioInput::GetMicInputMute(bool& bMute)
{
	if (mixerGetNumDevs() < 0)
	{
		return false;
	}

	DWORD dwSource = 0;
	HMIXER m_hMixer = NULL;   
	MMRESULT mr = NULL;

	//打开Mixer设备   
	if(mixerOpen(&m_hMixer, 0, 0, 0, MIXER_OBJECTF_MIXER) != MMSYSERR_NOERROR)
		return false; 

	MIXERLINE MixerLine;   
	//结构体的大小   
	MixerLine.cbStruct = sizeof(MixerLine);   
	//录制设备总线   
	MixerLine.dwComponentType   =   MIXERLINE_COMPONENTTYPE_DST_WAVEIN;   
	//得到录制总线中的连接数   
	if(mixerGetLineInfo((HMIXEROBJ)m_hMixer, &MixerLine,
		MIXER_OBJECTF_HMIXER | MIXER_GETLINEINFOF_COMPONENTTYPE) != MMSYSERR_NOERROR)
		return false; 

	//将连接数保存   
	DWORD dwConnections = MixerLine.cConnections;   
	//准备获取麦克风设备的ID   
	DWORD dwLineID = 0;   
	for(DWORD i = 0; i < dwConnections; i++ )   
	{   
		//枚举每一个设备，当Source的ID等于当前的迭代记数   
		MixerLine.dwSource = i;   
		//根据SourceID获得连接的信息   
		if(mixerGetLineInfo((HMIXEROBJ)m_hMixer, &MixerLine,   
			MIXER_OBJECTF_HMIXER | MIXER_GETLINEINFOF_SOURCE) != MMSYSERR_NOERROR) 
			//判断函数执行错误   
			break;  

		//如果当前设备类型是麦克风，则跳出循环。   
		if(MixerLine.dwComponentType == MIXERLINE_COMPONENTTYPE_SRC_MICROPHONE)   
		{   
			dwLineID = MixerLine.dwLineID;
			dwSource = MixerLine.dwSource;
			break;   
		}   
	}

	//如果没有找到，返回失败。   
	if(dwLineID == 0)  return false;   

	// Find a mute control, if any, of the microphone line
	LPMIXERCONTROL pmxctrl = (LPMIXERCONTROL)malloc(sizeof MIXERCONTROL);
	MIXERLINECONTROLS mxlctrl = {sizeof mxlctrl, MixerLine.dwLineID,
		MIXERCONTROL_CONTROLTYPE_MUTE, 1, sizeof MIXERCONTROL, pmxctrl};
	if(!mixerGetLineControls((HMIXEROBJ) m_hMixer, &mxlctrl,
		MIXER_GETLINECONTROLSF_ONEBYTYPE)){
			// Found, so proceed
			DWORD cChannels = MixerLine.cChannels;
			if (MIXERCONTROL_CONTROLF_UNIFORM & pmxctrl->fdwControl)
				cChannels = 1;

			LPMIXERCONTROLDETAILS_BOOLEAN pbool =
				(LPMIXERCONTROLDETAILS_BOOLEAN) malloc(cChannels * sizeof
				MIXERCONTROLDETAILS_BOOLEAN);
			MIXERCONTROLDETAILS mxcd = {sizeof(mxcd), pmxctrl->dwControlID,
				cChannels, (HWND)0,
				sizeof MIXERCONTROLDETAILS_BOOLEAN, (LPVOID) pbool};
			mixerGetControlDetails((HMIXEROBJ)m_hMixer, &mxcd,
				MIXER_SETCONTROLDETAILSF_VALUE);

			bMute = pbool[0].fValue || pbool[cChannels - 1].fValue;

			free(pmxctrl);
			free(pbool);
	}
	else
		free(pmxctrl);

	mixerClose(m_hMixer);

	return true;
}
