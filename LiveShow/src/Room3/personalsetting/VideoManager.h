#pragma once
#include "..\avlogic\RoomAVLogic.h"

struct VInfo
{
	int nIndex;
	std::wstring strName;
	std::wstring strGUID;
	unsigned int nDeviceID;
	bool bVirtual;
};

typedef std::vector<VInfo> VecVideoInfo;

class VideoManager
{
public:
	VideoManager(void);
	~VideoManager(void);

	static VideoManager * GetInstance();
	static void DeleteInstance();

	void Init(RoomAVLogic * pRoomAvLogic);
	void Release();
	void ReloadVideoDevice();
	VecVideoInfo & GetVideoList();
	int GetFirstVirtualCam();
	RoomAVLogic* GetRoomAvLogic(){ return m_pRoomAvLogic ;} ;
	bool GetVideoInfoByIndex(int nIndex, VInfo & stInfo);
	bool GetVideoInfoByGUID(std::wstring strGUID, VInfo & stInfo);

protected:
	void AddRealCam(VecVideoInfo & vecVideoInfoList);
	void AddVirtualCam(std::wstring& strName, VecVideoInfo & vecVideoInfoList);
	void LoadVideoDevice(VecVideoInfo & vecVideoInfoList);
	void Sort(VecVideoInfo & vecVideoInfoList);
	void ResetVideoList();

protected:
	static VideoManager * s_pVideoManager;
	RoomAVLogic * m_pRoomAvLogic;
	VecVideoInfo m_vecVideoInfo;
};
