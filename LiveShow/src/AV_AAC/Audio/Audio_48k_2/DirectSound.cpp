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
	DXAudioInputRegistrar *airReg;
	DXAudioOutputRegistrar *aorReg;
public:
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
	if (airReg)
		delete airReg;
	if (aorReg)
		delete aorReg;
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
		assert(0);
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
	if (GetOsVersion() > OS_WIN2000)
	{
		if (! g_struct.s.qbaDXOutput.empty()) {
			Trace("DXAudioOutput:Trace1 , len = %d\n",g_struct.s.qbaDXOutput.size());
			LPGUID lpguid = reinterpret_cast<LPGUID>((char*)&g_struct.s.qbaDXOutput[0]);
			if (FAILED(hr = DirectSoundCreate8(lpguid, &pDS, NULL))) {
				Trace("DXAudioOutput: DirectSoundCreate8 error\n");
				failed = true;
			}
			Trace("DXAudioOutput prefered output device create ok\n");
		}
	}
	else
	{
		Trace("DXAudioOutput os2000 use default output device\n");
	}
	
	Trace("DXAudioOutput:DirectSoundCreate8 1\n");

	if (! pDS && FAILED(hr = DirectSoundCreate8(&DSDEVID_DefaultVoicePlayback, &pDS, NULL))) {
		Trace("DXAudioOutput: DirectSoundCreate");
		goto cleanup;
	} else if (FAILED(hr = pDS->SetCooperativeLevel(GetDesktopWindow(), DSSCL_PRIORITY))) {
		Trace("DXAudioOutput: SetCooperativeLevel");
		goto cleanup;
	} else if (FAILED(hr = pDS->CreateSoundBuffer(&dsbdesc, &pDSBPrimary, NULL))) {
		Trace("DXAudioOutput: CreateSoundBuffer (Primary) : 0x%08lx", hr);
		goto cleanup;
	}
	Trace("DXAudioOutput:DirectSoundCreate8,SetCooperativeLevel,CreateSoundBuffer ok\n");

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
		Trace("DXAudioOutput: SetFormat");
		goto cleanup;
	}
	if (FAILED(hr = pDSBPrimary->GetFormat(reinterpret_cast<WAVEFORMATEX *>(&wfxSet), sizeof(wfxSet), NULL))) {
		Trace("DXAudioOutput: GetFormat");
		goto cleanup;
	}

	Trace("DXAudioOutput: Primary buffer of %ld Hz, %d channels, %d bits",wfxSet.Format.nSamplesPerSec,wfxSet.Format.nChannels,wfxSet.Format.wBitsPerSample);

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
	dsbdesc.dwFlags	 |= DSBCAPS_CTRLPOSITIONNOTIFY;
	dsbdesc.dwBufferBytes = wfx.Format.nChannels * iFrameSize * sizeof(short) * NBLOCKS;
	dsbdesc.lpwfxFormat     = reinterpret_cast<WAVEFORMATEX *>(&wfx);

	if (FAILED(hr = pDS->CreateSoundBuffer(&dsbdesc, (LPDIRECTSOUNDBUFFER*)&pDSBOutput, NULL))) {
		Trace("DXAudioOutputUser: CreateSoundBuffer (Secondary): 0x%08lx", hr);
		goto cleanup;
	}

	if (FAILED(hr = pDSBOutput->QueryInterface(IID_IDirectSoundNotify, reinterpret_cast<void **>(&pDSNotify)))) {
		Trace("DXAudioOutputUser: QueryInterface (Notify)");
		goto cleanup;
	}

	Trace("DXAudioOutputUser: New %dHz output buffer of %ld bytes", SAMPLE_RATE, dsbdesc.dwBufferBytes);

	if (failed)
	{
		Trace("Opening chosen DirectSound Output failed. Default device will be used.\n");
		AV_Audio_Fail_Event* pAudioFailEvent = new AV_Audio_Fail_Event(0,kAudio_Err_Output);
		if (!g_pAVModule->sendEvent(static_cast<void*>(pAudioFailEvent), EVENT_AV_AUDIO_FAIL))
		{
			Assert(0);
			delete pAudioFailEvent;
		}
	}

	Trace("DXAudioOutput:directsound  initialized\n");

	initializeMixer(chanmasks, bHead);

	if (FAILED(hr = pDSBOutput->Lock(0, 0, &aptr1, &nbytes1, &aptr2, &nbytes2, DSBLOCK_ENTIREBUFFER))) {
		Trace("DXAudioOutputUser: Initial Lock");
		goto cleanup;
	}

	dwBufferSize = nbytes1 + nbytes2;
	if (aptr1)
		ZeroMemory(aptr1, nbytes1);
	if (aptr2)
		ZeroMemory(aptr2, nbytes2);

	if (FAILED(hr = pDSBOutput->Unlock(aptr1, nbytes1, aptr2, nbytes2))) {
		Trace("DXAudioOutputUser: Initial Unlock");
		goto cleanup;
	}

	if (FAILED(hr = pDSBOutput->Play(0, 0, DSBPLAY_LOOPING))) {
		Trace("DXAudioOutputUser: Play");
		goto cleanup;
	}

	dwLastWritePos = 0;
	dwLastPlayPos = 0;
	dwTotalPlayPos = 0;

	iLastwriteblock = (NBLOCKS - 1 + g_struct.s.iOutputDelay) % NBLOCKS;


	while (!IsStopRequested() && bRunning && ! FAILED(hr)) {
		if (FAILED(hr = pDSBOutput->GetCurrentPosition(&dwPlayPosition, &dwWritePosition))) {
			Trace("DXAudioOutputUser: GetCurrentPosition");
			break;
		}
		SetRunningState(true);

		playblock = dwWritePosition / iByteSize;
		nowriteblock = (playblock + g_struct.s.iOutputDelay + 1) % NBLOCKS;

		for (int block=(iLastwriteblock + 1) % NBLOCKS;(!FAILED(hr)) && (block!=nowriteblock);block=(block + 1) % NBLOCKS) {
			iLastwriteblock = block;

			if (FAILED(hr = pDSBOutput->Lock(block * iByteSize, iByteSize, &aptr1, &nbytes1, &aptr2, &nbytes2, 0))) {
				Trace("DXAudioOutput: Lock block %u (%d bytes)",block, iByteSize);
				break;
			}
			if (aptr2 || nbytes2) {
				Trace("DXAudioOutput: Split buffer");
				break;
			}
			if (!aptr1 || ! nbytes1) {
				Trace("DXAudioOutput: Zerolock");
				break;
			}
			//获取混音后的音频数据
			if (! mix(aptr1, iFrameSize*iChannels))
			{
				ZeroMemory(aptr1, iByteSize);
			}

			UInt64 lLevel = 0;
			int i = 0;
			for (i=0; i<iFrameSize*2*iChannels/10; i++)
			{
				lLevel += *((unsigned char*)aptr1 + i);
			}
			iLevel = lLevel * 18/ (i * 255);
			//iLevel = (*(char*)aptr1 + *((char*)aptr1 + 10) + *((char*)aptr1 + 20) + *((char*)aptr1 + 30))/(9*3);

			if (FAILED(hr = pDSBOutput->Unlock(aptr1, nbytes1, aptr2, nbytes2))) {
				Trace("DXAudioOutput: Unlock %p(%lu) %p(%lu)",aptr1,nbytes1,aptr2,nbytes2);
				break;
			}

			if (FAILED(hr = pDSBOutput->GetCurrentPosition(&dwPlayPosition, &dwWritePosition))) {
				Trace("DXAudioOutputUser: GetCurrentPosition");
				break;
			}

			playblock = dwWritePosition / iByteSize;
			nowriteblock = (playblock + g_struct.s.iOutputDelay + 1) % NBLOCKS;
		}
		if (! FAILED(hr))
			Sleep(19);
	}

	if (FAILED(hr)) {
		Assert(L"Lost DirectSound output device.");
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
			Trace("Set volume error %s.",DXGetErrorString8(hr));
		}
	}
}

#define NBUFFBLOCKS 100

DXAudioInput::DXAudioInput() {
	bRunning = true;
	eMicFormat = eEchoFormat = SampleShort;
	pDSCaptureBuffer = NULL;
}

DXAudioInput::~DXAudioInput() {
	bRunning = false;
	if (pDSCaptureBuffer)
		pDSCaptureBuffer->Release();
	pDSCaptureBuffer = NULL;
}

void DXAudioInput::Entry() {
	
	LPDIRECTSOUNDCAPTURE8      pDSCapture;
	LPDIRECTSOUNDNOTIFY8       pDSNotify;

	pDSCapture = NULL;
	pDSCaptureBuffer = NULL;
	pDSNotify = NULL;

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

	pDSCapture = NULL;
	pDSCaptureBuffer = NULL;
	pDSNotify = NULL;

	bool failed = false;
	float safety = 2.0f;
	bool didsleep = false;
	bool firstsleep = false;

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
			Trace("DXAudioInput::Create IDirectSoundCapture using the preferred capture device\n");
			LPGUID lpguid = reinterpret_cast<LPGUID>(&(g_struct.s.qbaDXInput[0]));
			if (FAILED(hr = DirectSoundCaptureCreate8(lpguid, &pDSCapture, NULL))) {
				Trace("DXAudioInput: fail create directsound capture use preferred capture device\n");
				failed = true;
			}
			bOk = true;
		}
	}
	else
	{
		Trace("DXAudioInput: OS2000 use default capture device\n");
	}
	
	Trace("Trace1\n");


	Trace("DXAudioInput: DirectSoundCaptureCreate8 use default device\n");
	if(! pDSCapture && FAILED(DirectSoundCaptureCreate8(&DSDEVID_DefaultVoiceCapture, &pDSCapture, NULL)))
	{
		Trace("DXAudioInput: DirectSoundCaptureCreate use default device error\n");
	}
	else
	{
		Trace("DXAudioInput: CreateCaptureBuffer\n");
		if(FAILED(pDSCapture->CreateCaptureBuffer(&dscbd, (LPDIRECTSOUNDCAPTUREBUFFER *)&pDSCaptureBuffer, NULL)))
		{
			Trace("DXAudioInput: CreateCaptureBuffer error \n");
		}
		else
		{
			Trace("DXAudioInput: QueryInterface(Notify)\n");
			if (FAILED(hr = pDSCaptureBuffer->QueryInterface(IID_IDirectSoundNotify, reinterpret_cast<void **>(&pDSNotify))))
				Trace("DXAudioInput: QueryInterface (Notify) error\n");
			else
			{
				bOk = true;
			}
		}
	}

	Trace("Trace2\n");
	if (failed)
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
		Trace("DXAudioInput: Initialize error");
	}

	Trace("DXAudioInput: Initialized");

	if (! bOk)
		goto cleanup;

	if (FAILED(hr = pDSCaptureBuffer->Start(DSCBSTART_LOOPING))) {
		Trace("DXAudioInput: Start failed");
	} else {
		while (bRunning && !IsStopRequested()) {
			SetRunningState(true);
			firstsleep = true;
			didsleep = false;

			//保证至少采样一帧数据
			do {
				if ( FAILED(hr = pDSCaptureBuffer->GetCurrentPosition(&dwCapturePosition, &dwReadPosition))) {
					Trace("DXAudioInput: GetCurrentPosition");
					bRunning = false;
					break;
				}
				if (dwReadPosition < dwLastReadPos)
					dwReadyBytes = (dwBufferSize - dwLastReadPos) + dwReadPosition;
				else
					dwReadyBytes = dwReadPosition - dwLastReadPos;

				if (static_cast<int>(dwReadyBytes) < sizeof(short)*iFrameSize*wfx.nChannels) {
					double msecleft = 20.0 - (dwReadyBytes * 20.0) / (sizeof(short)*iFrameSize*wfx.nChannels);

					if (didsleep)
						safety *= 1.1f;
					else if (firstsleep)
						safety *= 0.998f;

					int msec = static_cast<int>(msecleft + (firstsleep ? safety : 0.0));

					Sleep(msec);

					didsleep = true;
					firstsleep = false;
				}
			} while (static_cast<int>(dwReadyBytes) < sizeof(short)*iFrameSize*wfx.nChannels);

			// Desynchonized?
			if (dwReadyBytes > (dwBufferSize / 2)) {
				Trace("DXAudioInput: Lost synchronization");
				dwLastReadPos = dwReadPosition;
			} 
			else if (bRunning && !m_bStopRequested) {
				if (FAILED(hr = pDSCaptureBuffer->Lock(dwLastReadPos, sizeof(short)*iFrameSize*wfx.nChannels, &aptr1, &nbytes1, &aptr2, &nbytes2, 0))) {
					Trace("DXAudioInput: Lock from %ld (%d bytes)",dwLastReadPos, sizeof(short)*iFrameSize*wfx.nChannels);
					bRunning = false;
					break;
				}

				if (aptr1 && nbytes1)
					CopyMemory(psMic, aptr1, nbytes1);

				UInt64 lLevel = 0;
				int i = 0;
				for (i=0; i<iFrameSize*2/10; i++)
				{
					lLevel += *((unsigned char*)aptr1 + i);
				}
				iLevel = lLevel * 18 / (i * 255);

				if (aptr2 && nbytes2)
					CopyMemory(psMic+nbytes1, aptr2, nbytes2);

				if (FAILED(hr = pDSCaptureBuffer->Unlock(aptr1, nbytes1, aptr2, nbytes2))) {
					Trace("DXAudioInput: Unlock");
					bRunning = false;
					break;
				}

				dwLastReadPos = (dwLastReadPos + sizeof(short)*iFrameSize*wfx.nChannels) % dwBufferSize;			

				encodeAudioFrame();
			}
		}
		pDSCaptureBuffer->Stop();
	}

	if (FAILED(hr)) {
		Trace("Lost DirectSound input device.\n");
	}
cleanup:
	if (! bOk) {
		Trace("Opening chosen DirectSound Input device failed. No microphone capture will be done.\n");
	}
	if (pDSNotify)
	{
		pDSNotify->Release();
	    pDSNotify = NULL;
	}
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
	
	CancelStopRequest();
	Trace("DXAudioInput:pDSCapture release\n");
	SetRunningState(false);
	_endthreadex((UINT32)m_nThreadID);
}
