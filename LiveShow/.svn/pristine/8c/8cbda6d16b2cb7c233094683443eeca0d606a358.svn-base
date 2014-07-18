#include "StdAfx.h"
#include "MixerOther.h"
#include <xstring>

const IID IID_IDeviceTopology = __uuidof(IDeviceTopology);
const IID IID_IPart = __uuidof(IPart);
const IID IID_IConnector = __uuidof(IConnector);
const IID IID_IAudioInputSelector = __uuidof(IAudioInputSelector);

CMixerOther g_mixerOther;

CMixerOther::CMixerOther(void)
:m_pEndptVolume(NULL),
 m_pEnumerator(NULL),
 m_pDevice(NULL),
 m_pVolumeLevel(NULL),
 m_Guid(GUID_NULL)

{
	m_OsType = GetOsType();
}

CMixerOther::~CMixerOther(void)
{

}

//获取最大音量
DWORD CMixerOther::GetMaxVolume()
{
	return MAX_VOLUME;
}

//获取麦克风音量
BOOL CMixerOther::GetMixerMicVolume(UInt32& iVolume)
{
	if(m_pEndptVolume)
		UnInitEndPoint();

	InitEndPoint(AudioCapture); //录音设备

	float fVolume;

	if(m_pEndptVolume)
	{
		m_pEndptVolume->GetMasterVolumeLevelScalar(&fVolume);
		iVolume= GetMaxVolume() * fVolume + 0.5;
	}

	return TRUE;
}

BOOL CMixerOther::UnInitEndPoint()
{
	SAFE_RELEASE(m_pEnumerator);
	SAFE_RELEASE(m_pDevice);
	SAFE_RELEASE(m_pEndptVolume);
	CoUninitialize();
	return TRUE;
}

BOOL CMixerOther::InitEndPoint(EnAudioType enType)
{
	if(m_pEndptVolume != NULL) return FALSE;
	
	//g_EPVolEvents.SetCallback(fCallback); //set callback

	HRESULT hr = S_OK;
	CoInitialize(NULL);
	hr = CoCreateGuid(&m_Guid);
	if(FAILED(hr)) return FALSE;

	// Get enumerator for audio endpoint devices.
	hr = CoCreateInstance(__uuidof(MMDeviceEnumerator),
		NULL, CLSCTX_INPROC_SERVER,
		__uuidof(IMMDeviceEnumerator),
		(void**)&m_pEnumerator);
	if(FAILED(hr)) return FALSE;

	// Get default audio-rendering or audio-capture device.
	if(enType == AudioRender)
		hr = m_pEnumerator->GetDefaultAudioEndpoint(eRender, eConsole, &m_pDevice);
	else
		hr = m_pEnumerator->GetDefaultAudioEndpoint(eCapture, eConsole, &m_pDevice);

	if(FAILED(hr)) return FALSE;

	hr = m_pDevice->Activate(__uuidof(IAudioEndpointVolume),
		CLSCTX_ALL, NULL, (void**)&m_pEndptVolume);

	if(FAILED(hr)) return FALSE;

	/*if(g_pEndptVolCtrl)
	hr = g_pEndptVolCtrl->RegisterControlChangeNotify((IAudioEndpointVolumeCallback*)&g_EPVolEvents);
	if(FAILED(hr)) return FALSE;*/
	return TRUE;
}


//设置麦克风音量
BOOL CMixerOther::SetMixerMicVolume(UInt32 iVolume)
{
	if(m_pEndptVolume)
		UnInitEndPoint();

	InitEndPoint(AudioCapture);

	if(iVolume < 0) iVolume = 0;
	if(iVolume > MAX_VOLUME) iVolume = MAX_VOLUME;

	if(m_pEndptVolume)
	{
		HRESULT hr = m_pEndptVolume->SetMasterVolumeLevelScalar((float)iVolume / MAX_VOLUME, &m_Guid);
		return SUCCEEDED(hr) ? TRUE : FALSE;
	}

	return FALSE;
}

//获取扬声器音量
BOOL CMixerOther::GetMixerSpeakerVolume(UInt32& iVolume)
{
	if(m_pEndptVolume)
		UnInitEndPoint();

	InitEndPoint(AudioRender); //录音设备

	float fVolume;

	if(m_pEndptVolume)
	{
		m_pEndptVolume->GetMasterVolumeLevelScalar(&fVolume);
		iVolume= GetMaxVolume() * fVolume + 0.5;
	}

	return TRUE;
}

//设置扬声器音量
BOOL CMixerOther::SetMixerSpeakerVolume(UInt32 iVolume)
{
	if(m_pEndptVolume)
		UnInitEndPoint();

	InitEndPoint(AudioRender);

	if(iVolume < 0) iVolume = 0;
	if(iVolume > MAX_VOLUME) iVolume = MAX_VOLUME;

	if(m_pEndptVolume)
	{
		HRESULT hr = m_pEndptVolume->SetMasterVolumeLevelScalar((float)iVolume / MAX_VOLUME, &m_Guid);
		return SUCCEEDED(hr) ? TRUE : FALSE;
	}

	return FALSE;
}

//获取操作系统类型
EnOsType CMixerOther::GetOsType()
{
	OSVERSIONINFO osInfo;
	memset(&osInfo, 0, sizeof(OSVERSIONINFO));
	osInfo.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
	EnOsType eType = OsWindowsUnKnown;
	//获取版本类型
	if(::GetVersionEx(&osInfo))
	{
		//Trace("CMixerManager::GetOsType  dwMajorVersion= %d, dwMinorVersion = %d, dwBuildNumber = %d \n", osInfo.dwMajorVersion, osInfo.dwMinorVersion, osInfo.dwBuildNumber);

		if(osInfo.dwMajorVersion <= 5)//XP or early
		{
			eType = OsWindowsXP;
		}
		else
		{
			if(osInfo.dwMinorVersion == 0)
			{
				eType = OsWindowsVista;
			}
			else
			{
				eType = OsWindows7;
			}
		}

	}
	return eType;
}

//获取麦克风是否静音
BOOL CMixerOther::GetMixerMicMute(BOOL& bMute)
{
	if(m_pEndptVolume)
		UnInitEndPoint();

	InitEndPoint(AudioCapture);

	if(m_pEndptVolume)
		m_pEndptVolume->SetMute(bMute, &m_Guid);

	return TRUE;
}

//设置麦克风静音
BOOL CMixerOther::SetMixerMicMute(BOOL bMute)
{
	if(m_pEndptVolume)
		UnInitEndPoint();

	InitEndPoint(AudioCapture);

	if(m_pEndptVolume)
		m_pEndptVolume->GetMute(&bMute);
	return TRUE;
}

//获取扬声器是否静音
BOOL CMixerOther::GetMixerSpeakerMute(BOOL& bMute)
{
	if(m_pEndptVolume)
		UnInitEndPoint();

	InitEndPoint(AudioRender);

	if(m_pEndptVolume)
		m_pEndptVolume->SetMute(bMute, &m_Guid);

	return TRUE;
}
//设置扬声器是否静音
BOOL CMixerOther::SetMixerSpeakerMute(BOOL bMute)
{
	if(m_pEndptVolume)
		UnInitEndPoint();

	InitEndPoint(AudioRender);

	if(m_pEndptVolume)
		m_pEndptVolume->GetMute(&bMute);
	return TRUE;
}

//获取麦克风加强 控制器
HRESULT CMixerOther::GetMicrophoneBoostLevel(IMMDevice *pEndptDev, IAudioVolumeLevel** ppVolumeLevel)
{
	HRESULT hr = S_OK;  
	DataFlow flow;  
	IDeviceTopology *pDeviceTopology = NULL;  
	IConnector *pConnFrom = NULL;  
	IConnector *pConnTo = NULL;  
	IPart *pPartPrev = NULL;  
	IPart *pPartNext = NULL;  
	*ppVolumeLevel = NULL;  
	std::wstring microphoneBoostName = L"麦克风加强";//if your system language is English,the name is "microphone boost" 
	std::wstring microphoneBoostNameEn = L"microphone boost";
	if (pEndptDev == NULL)  
	{  
		EXIT_ON_ERROR(hr = E_POINTER)  
	}  
	// Get the endpoint device's IDeviceTopology interface.   
	hr = pEndptDev->Activate(  
		IID_IDeviceTopology, CLSCTX_ALL, NULL,  
		(void**)&pDeviceTopology);  
	EXIT_ON_ERROR(hr)  
		// The device topology for an endpoint device always   
		// contains just one connector (connector number 0).   
		hr = pDeviceTopology->GetConnector(0, &pConnFrom);  
	SAFE_RELEASE(pDeviceTopology)  
		EXIT_ON_ERROR(hr)  
		// Make sure that this is a capture device.   
		hr = pConnFrom->GetDataFlow(&flow);  
	EXIT_ON_ERROR(hr)  
		if (flow != Out)  
		{  
			// Error -- this is a rendering device.   
			EXIT_ON_ERROR(hr = AUDCLNT_E_WRONG_ENDPOINT_TYPE)  
		}  
		// Outer loop: Each iteration traverses the data path   
		// through a device topology starting at the input   
		// connector and ending at the output connector.   
		while (TRUE)  
		{  
			BOOL bConnected;  
			hr = pConnFrom->IsConnected(&bConnected);  
			EXIT_ON_ERROR(hr)  
				// Does this connector connect to another device?   
				if (bConnected == FALSE)  
				{  
					// This is the end of the data path that   
					// stretches from the endpoint device to the   
					// system bus or external bus. Verify that   
					// the connection type is Software_IO.   
					ConnectorType  connType;  
					hr = pConnFrom->GetType(&connType);  
					EXIT_ON_ERROR(hr)  
						if (connType == Software_IO)  
						{  
							break;  // finished   
						}  
						EXIT_ON_ERROR(hr = E_FAIL)  
				}  
				// Get the connector in the next device topology,   
				// which lies on the other side of the connection.   
				hr = pConnFrom->GetConnectedTo(&pConnTo);  
				EXIT_ON_ERROR(hr)  
					SAFE_RELEASE(pConnFrom)  
					// Get the connector's IPart interface.   
					hr = pConnTo->QueryInterface(  
					IID_IPart, (void**)&pPartPrev);  
				EXIT_ON_ERROR(hr)  
					SAFE_RELEASE(pConnTo)  
					// Inner loop: Each iteration traverses one link in a   
					// device topology and looks for input multiplexers.   
					while (TRUE)  
					{  
						PartType parttype;  
						IPartsList *pParts;  
						// Follow downstream link to next part.   
						hr = pPartPrev->EnumPartsOutgoing(&pParts);  
						EXIT_ON_ERROR(hr)  
							hr = pParts->GetPart(0, &pPartNext);  
						pParts->Release();  
						EXIT_ON_ERROR(hr)  
							hr = pPartNext->GetPartType(&parttype);  
						EXIT_ON_ERROR(hr)  

							LPWSTR pName;  
						if (SUCCEEDED(pPartNext->GetName(&pName)))  
						{  
							// Failure of the following call means only that   
							// the part is not a boost (micrphone boost).   
							if (microphoneBoostName.compare(pName) == 0 || microphoneBoostNameEn.compare(pName)==0) 
							{  
								//get IAudioVolumeLevel to control volume    
								hr = pPartNext->Activate(CLSCTX_ALL, __uuidof(IAudioVolumeLevel), (void**)ppVolumeLevel);  
								goto Exit;  
							}  
							CoTaskMemFree(pName);  
						}  
						GUID subType;  
						pPartNext->GetSubType(&subType);  
						if (parttype == Connector)  
						{  
							// We've reached the output connector that   
							// lies at the end of this device topology.   
							hr = pPartNext->QueryInterface(  
								IID_IConnector,  
								(void**)&pConnFrom);  
							EXIT_ON_ERROR(hr)  
								SAFE_RELEASE(pPartPrev)  
								SAFE_RELEASE(pPartNext)  
								break;  
						}  
						SAFE_RELEASE(pPartPrev)  
							pPartPrev = pPartNext;  
						pPartNext = NULL;  
					}  
		}  
Exit:  
		SAFE_RELEASE(pConnFrom)  
			SAFE_RELEASE(pConnTo)  
			SAFE_RELEASE(pPartPrev)  
			SAFE_RELEASE(pPartNext)  
			return hr;  
}

BOOL CMixerOther::SetCaptureDeviceSelect(IMMDevice *pEndptDev)
{
	HRESULT hr = S_OK;
	DataFlow flow;
	IDeviceTopology *pDeviceTopology = NULL;
	IConnector *pConnFrom = NULL;
	IConnector *pConnTo = NULL;
	IPart *pPartPrev = NULL;
	IPart *pPartNext = NULL;
	IAudioInputSelector *pSelector = NULL;
	IAudioLoudness *pLoudness = NULL;

	// Get the endpoint device's IDeviceTopology interface.
	hr = pEndptDev->Activate(
		IID_IDeviceTopology, CLSCTX_ALL, NULL,
		(void**)&pDeviceTopology);
	EXIT_ON_ERROR(hr)

		// The device topology for an endpoint device always
		// contains just one connector (connector number 0).
		hr = pDeviceTopology->GetConnector(0, &pConnFrom);
	SAFE_RELEASE(pDeviceTopology)
		EXIT_ON_ERROR(hr)

		// Make sure that this is a capture device.
		hr = pConnFrom->GetDataFlow(&flow);
	EXIT_ON_ERROR(hr)

		if (flow != Out)
		{
			// Error -- this is a rendering device. 
			EXIT_ON_ERROR(hr = E_FAIL) //AUDCLNT_E_WRONG_ENDPOINT_TYPE;
		}

		// Outer loop: Each iteration traverses the data path
		// through a device topology starting at the input
		// connector and ending at the output connector.
		while (TRUE)
		{
			BOOL bConnected;
			hr = pConnFrom->IsConnected(&bConnected);
			EXIT_ON_ERROR(hr)

				// Does this connector connect to another device

				if (!bConnected)
				{

					// This is the end of the data path that
					// stretches from the endpoint device to the
					// system bus or external bus. Verify that
					// the connection type is Software_IO.
					ConnectorType  connType;
					hr = pConnFrom->GetType(&connType);
					EXIT_ON_ERROR(hr)

						if (connType == Software_IO) 
							break;  // finished 

					EXIT_ON_ERROR(hr = E_FAIL)
				} 

				// Get the connector in the next device topology,
				// which lies on the other side of the connection.
				hr = pConnFrom->GetConnectedTo(&pConnTo);
				EXIT_ON_ERROR(hr)
					SAFE_RELEASE(pConnFrom)

					// Get the connector's IPart interface.
					hr = pConnTo->QueryInterface(
					IID_IPart, (void**)&pPartPrev);
				EXIT_ON_ERROR(hr)
					SAFE_RELEASE(pConnTo)

					// Inner loop: Each iteration traverses one link in a
					// device topology and looks for input multiplexers.

					while (TRUE)
					{
						PartType parttype;
						UINT localId;
						IPartsList *pParts;

						// Follow downstream link to next part.
						hr = pPartPrev->EnumPartsOutgoing(&pParts);
						EXIT_ON_ERROR(hr)

							hr = pParts->GetPart(0, &pPartNext);
						pParts->Release();
						EXIT_ON_ERROR(hr)

							hr = pPartNext->GetPartType(&parttype);
						EXIT_ON_ERROR(hr)

							if (parttype == Connector)
							{
								// We've reached the output connector that
								// lies at the end of this device topology.
								hr = pPartNext->QueryInterface(
									IID_IConnector,
									(void**)&pConnFrom);
								EXIT_ON_ERROR(hr)

									SAFE_RELEASE(pPartPrev)
									SAFE_RELEASE(pPartNext)
									break;
							}


							LPWSTR pWcsName = NULL;
							hr = pPartNext->GetName(&pWcsName);
							EXIT_ON_ERROR(hr) 
							
							WCHAR str[256] = {0};
							swprintf(str,L"%s",pWcsName);
							OutputDebugString(str); 


							CoTaskMemFree(pWcsName);
							pWcsName = NULL; 

							UINT cCount = 0;
							pPartNext->GetControlInterfaceCount(&cCount); 

							for(UINT index=0; index<cCount; index++)
							{
								IControlInterface* pCtrlItem = NULL;
								hr = pPartNext->GetControlInterface(index, &pCtrlItem);
								if(FAILED(hr))
									break;

								hr = pCtrlItem->GetName(&pWcsName);

								if(str == L"麦克风加强" || str == L"microphone boost")
								{
									hr = pPartNext->Activate(CLSCTX_ALL,
										__uuidof(IAudioLoudness),
										(void**)&pLoudness);

									SAFE_RELEASE(pCtrlItem)

										if(FAILED(hr))
											break;

									CoTaskMemFree(pWcsName);
									pWcsName = NULL; 

									goto Exit;
								}


							}

							//  HRESULT GetControlInterface(
							//	[in]   UINT nIndex,
							//	[out]  IControlInterface **ppFunction
							//	);

							// Failure of the following call means only that
							// the part is not a MUX (input selector).
							hr = pPartNext->Activate(
								CLSCTX_ALL,
								IID_IAudioInputSelector,
								(void**)&pSelector);

							if (hr == S_OK)
							{
								// We found a MUX (input selector), so select
								// the input from our endpoint device.
								hr = pPartPrev->GetLocalId(&localId);
								EXIT_ON_ERROR(hr)

									hr = pSelector->SetSelection(localId, NULL);
								EXIT_ON_ERROR(hr)

									SAFE_RELEASE(pSelector)
							}

							SAFE_RELEASE(pPartPrev)
								pPartPrev = pPartNext;
							pPartNext = NULL;
					}
		}

Exit:
		SAFE_RELEASE(pConnFrom)
			SAFE_RELEASE(pConnTo)
			SAFE_RELEASE(pPartPrev)
			SAFE_RELEASE(pPartNext)
			SAFE_RELEASE(pSelector)
			return SUCCEEDED(hr);
}

BOOL CMixerOther::GetMicLevelRange()
{
	if(m_pVolumeLevel)
	{
		m_pVolumeLevel->GetLevelRange(0,&m_fMin,&m_fMax,&m_fStep);
	}
	return TRUE;
}

BOOL CMixerOther::SetMicLevel(float fValue)
{
	if(m_pVolumeLevel)
	{
		m_pVolumeLevel->SetLevel(0,fValue,NULL);
	}
	return TRUE;
}

BOOL CMixerOther::GetMicLevel(float& fValue)
{
	if(m_pVolumeLevel)
	{
		m_pVolumeLevel->GetLevel(0,&fValue);
	}
	return TRUE;
}

//设置麦克风加强值  0~255
BOOL CMixerOther::SetMixerMicBoostVolume(UInt32 iVolume)
{
	if(m_pEndptVolume)
		UnInitEndPoint();

	InitEndPoint(AudioCapture);

	if (m_pDevice)
		GetMicrophoneBoostLevel(m_pDevice, &m_pVolumeLevel);
	GetMicLevelRange();

	if (m_pVolumeLevel)
		return SetMicLevel((float)iVolume * (m_fMax - m_fMin) / MAX_VOLUME + m_fMin);

	return FALSE;
}

//获取麦克风加强值 0~255
BOOL CMixerOther::GetMixerMicBoostVolume(UInt32& iVolume)
{
	if(m_pEndptVolume)
		UnInitEndPoint();

	InitEndPoint(AudioCapture);

	if (m_pDevice) 
		GetMicrophoneBoostLevel(m_pDevice, &m_pVolumeLevel);
	GetMicLevelRange();
	
	float fVolume = 0;
	BOOL bRet = FALSE;
	if (m_pVolumeLevel)
		 bRet = GetMicLevel(fVolume);
	if (m_fMax - m_fMin > 0)
		iVolume = (fVolume-m_fMin) * MAX_VOLUME/ (m_fMax - m_fMin);

	return bRet;
}

//设置麦克风加强 TRUE FALSE  设置加强增益为 0 OR MAX
BOOL CMixerOther::SetMixerMicBoostEnable(BOOL bEnable)
{
	if(m_pEndptVolume)
		UnInitEndPoint();

	InitEndPoint(AudioCapture);

	if (m_pDevice)
		GetMicrophoneBoostLevel(m_pDevice, &m_pVolumeLevel);
	GetMicLevelRange();

	if (m_pVolumeLevel)
		return SetMicLevel(bEnable?m_fMax:m_fMin);

	return FALSE;
}

//获取麦克风是否加强 增强Db == 0  没有加强 >0  加强
BOOL CMixerOther::GetMixerMicBoostEnable(BOOL& bEnable)
{
	if(m_pEndptVolume)
		UnInitEndPoint();

	InitEndPoint(AudioCapture);

	if (m_pDevice) 
		GetMicrophoneBoostLevel(m_pDevice, &m_pVolumeLevel);
	GetMicLevelRange();

	float fVolume = 0;
	BOOL bRet = FALSE;
	if (m_pVolumeLevel)
		bRet = GetMicLevel(fVolume);
	bEnable=(fVolume-m_fMin)>0?TRUE:FALSE;

	return bRet;
}
