#include "StdAfx.h"
#include "VideoManager.h"

VideoManager* VideoManager::s_pVideoManager = NULL;

#define GUAGUACAM	L"17GuaGua Cam"
#define MVBOXCAM	L"Virtual Cam"
#define V9158CAM	L"9158Capture"

VideoManager::VideoManager(void)
{
}

VideoManager::~VideoManager(void)
{
	ResetVideoList();
}

VideoManager * VideoManager::GetInstance()
{
	if (s_pVideoManager == NULL)
	{
		s_pVideoManager = new VideoManager();
	}

	return s_pVideoManager;
}

void VideoManager::Init( RoomAVLogic * pRoomAvLogic )
{
	m_pRoomAvLogic = pRoomAvLogic;
	VecVideoInfo vecVideoInfoList;
	LoadVideoDevice(vecVideoInfoList);
	Sort(vecVideoInfoList);
}

void VideoManager::LoadVideoDevice(VecVideoInfo & vecVideoInfoList)
{
	//Room_Get_Video_Device_List videolist;
	//memset(&videolist, 0x0, sizeof(Room_Get_Video_Device_List));
	Room_Get_Video_Device_Info_List videolist;
	memset(&videolist, 0x0, sizeof(Room_Get_Video_Device_Info_List));
	if (m_pRoomAvLogic != NULL)
	{
		//m_pRoomAvLogic->GetVideoDeviceList(videolist.iDeviceNum, videolist.wsDeviceName);
		m_pRoomAvLogic->GetVideoDeviceInfoList(videolist.iDeviceNum, videolist.stDeviceInfo);
	}

	for(int i=0;i<videolist.iDeviceNum;i++)
	{
		VInfo stInfo;
		//stInfo.strName = videolist.wsDeviceName[i];
		stInfo.nIndex = i;
		stInfo.strName = videolist.stDeviceInfo[i].wsDeviceName;
		stInfo.strGUID = videolist.stDeviceInfo[i].wsDeviceGUID;
		stInfo.nDeviceID = videolist.stDeviceInfo[i].iDeviceId;
		stInfo.bVirtual = m_pRoomAvLogic->IsVideoVirtualDevice(stInfo.nDeviceID);
		vecVideoInfoList.push_back(stInfo);

		CString strTemp;
		strTemp.Format(L"GetVideoDeviceInfoList before sort nIndex = %d, strName = %s, strGUID = %s, nDeviceID = %d, bVirtual = %d", stInfo.nIndex, stInfo.strName.c_str(), stInfo.strGUID.c_str(), stInfo.nDeviceID, stInfo.bVirtual);
		OutputDebugString(strTemp);
	}
}

void VideoManager::AddVirtualCam(std::wstring& strName, VecVideoInfo & vecVideoInfoList)
{
	for (VecVideoInfo::iterator it = vecVideoInfoList.begin(); it != vecVideoInfoList.end();)
	{
		VInfo& stInfo = *it;
		if(stInfo.strName.compare(strName) == 0)
		{
			m_vecVideoInfo.push_back(stInfo);
			it = vecVideoInfoList.erase(it);
		}
		else
		{
			it++;
		}
	}
}

void VideoManager::AddRealCam(VecVideoInfo & vecVideoInfoList)
{
	for (VecVideoInfo::iterator it = vecVideoInfoList.begin(); it != vecVideoInfoList.end();)
	{
		VInfo& stInfo = *it;
		if (!stInfo.bVirtual)
		{
			m_vecVideoInfo.push_back(stInfo);
			it = vecVideoInfoList.erase(it);
		}
		else
		{
			it++;
		}
	}
}

void VideoManager::Sort(VecVideoInfo & vecVideoInfoList)
{
	AddRealCam(vecVideoInfoList);

	AddVirtualCam(std::wstring(GUAGUACAM), vecVideoInfoList);
	AddVirtualCam(std::wstring(MVBOXCAM), vecVideoInfoList);
	AddVirtualCam(std::wstring(V9158CAM), vecVideoInfoList);

	for (VecVideoInfo::iterator it = vecVideoInfoList.begin(); it != vecVideoInfoList.end(); ++it)
	{
		m_vecVideoInfo.push_back(*it);
	}
	//µ÷Õûindex
	for (int i = 0; i < m_vecVideoInfo.size(); i++)
	{
		m_vecVideoInfo[i].nIndex = i;

		CString strTemp;
		strTemp.Format(L"GetVideoDeviceInfoList after sort nIndex = %d, strName = %s, strGUID = %s, nDeviceID = %d, bVirtual = %d", m_vecVideoInfo[i].nIndex, m_vecVideoInfo[i].strName.c_str(), m_vecVideoInfo[i].strGUID.c_str(), m_vecVideoInfo[i].nDeviceID, m_vecVideoInfo[i].bVirtual);
		OutputDebugString(strTemp);
	}
}

void VideoManager::Release()
{
	if (s_pVideoManager != NULL)
	{
		delete s_pVideoManager;
		s_pVideoManager = NULL;
	}
}

void VideoManager::ReloadVideoDevice()
{
	ASSERT(m_pRoomAvLogic != NULL);
	ResetVideoList();
	VecVideoInfo vecVideoInfoList;
	LoadVideoDevice(vecVideoInfoList);
	Sort(vecVideoInfoList);
}

VecVideoInfo & VideoManager::GetVideoList()
{
	return m_vecVideoInfo;
}

int VideoManager::GetFirstVirtualCam()
{
	for (VecVideoInfo::iterator it = m_vecVideoInfo.begin(); it != m_vecVideoInfo.end(); ++it)
	{
		VInfo& stInfo = *it;
		if (stInfo.bVirtual)
		{
			return stInfo.nIndex;
		}
	}

	return -1;
}

void VideoManager::ResetVideoList()
{
	m_vecVideoInfo.clear();
}

void VideoManager::DeleteInstance()
{
	if (s_pVideoManager)
	{
		delete s_pVideoManager;
		s_pVideoManager = NULL;
	}
}

bool VideoManager::GetVideoInfoByIndex( int nIndex, VInfo & stInfo )
{
	ASSERT(nIndex >= 0);
	if (nIndex >= m_vecVideoInfo.size())
	{
		return false;
	}

	stInfo =  m_vecVideoInfo[nIndex];
	return true;
}

bool VideoManager::GetVideoInfoByGUID( std::wstring strGUID, VInfo & stInfo )
{
	for (std::vector<VInfo>::iterator it = m_vecVideoInfo.begin(); it != m_vecVideoInfo.end(); ++it)
	{
		if ((*it).strGUID.compare(strGUID) == 0)
		{
			 stInfo = *it;
			 return true;
		}
	}

	return false;
}