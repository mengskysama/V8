#include "stdafx.h"
#include "AudioVideoSetting.h"


void CVideoSetting::GetVideoDeviceList(Get_Video_DeviceInfo_List &VideoList)
{	

	VideoManager::GetInstance()->ReloadVideoDevice();
	VecVideoInfo vecVideoList = VideoManager::GetInstance()->GetVideoList();
	VideoList.iDeviceNum = vecVideoList.size();
	for (int i = 0; i < vecVideoList.size(); ++i)
	{
		wcsncpy_s(&VideoList.wsDeviceName[i][0], DEVICE_NAME_LEN, vecVideoList[i].strName.c_str(), DEVICE_NAME_LEN);
		wcsncpy_s(&VideoList.wsDeviceGUID[i][0], MAX_VIDEO_DEVICE_GUID_LEN, vecVideoList[i].strGUID.c_str(), MAX_VIDEO_DEVICE_GUID_LEN);
		VideoList.bVirtual[i] = vecVideoList[i].bVirtual;

	}
	
}

void CVideoSetting::TestStopVideoDevice(Stop_Video_Device &videoDevice)
{
	RoomAVLogic* pRoomAvLogic= VideoManager::GetInstance()->GetRoomAvLogic();
	if ( pRoomAvLogic != NULL )
	{
		pRoomAvLogic->TestStopVideoDevice( videoDevice.iDeviceId ,videoDevice.hWnd ) ;
	}
	

}

void CVideoSetting::TestStartVideoDevice(Start_Video_Device &videoDevice)
{	
	RoomAVLogic* pRoomAvLogic= VideoManager::GetInstance()->GetRoomAvLogic();
	if ( pRoomAvLogic != NULL )
	{
		pRoomAvLogic->TestStartVideoDevice( videoDevice.iDeviceId , videoDevice.hWnd , videoDevice.iResult ) ;
	}

}

void CVideoSetting::TestSetVideoDevice(Set_Video_Device &videoDevice)
{
	RoomAVLogic* pRoomAvLogic= VideoManager::GetInstance()->GetRoomAvLogic();
	if ( pRoomAvLogic != NULL )
	{
		pRoomAvLogic->TestSetVideoDevice(videoDevice.iDeviceId);
	}

}

//显示视频属性对话框
void CVideoSetting::ShowVideoAjustDlg(Show_Video_Ajust_Dlg &videoAjust)
{	
	RoomAVLogic* pRoomAvLogic= VideoManager::GetInstance()->GetRoomAvLogic();
	if ( pRoomAvLogic != NULL )
	{
		pRoomAvLogic->ShowVideoAjustDlg(videoAjust.hParentWnd,videoAjust.iDeviceID);
	}
		
}

BOOL CVideoSetting::IsCameraUsing( int iDeviceID )
{
	RoomAVLogic* pRoomAvLogic= VideoManager::GetInstance()->GetRoomAvLogic();
	if ( pRoomAvLogic != NULL )
	{
		if ( pRoomAvLogic->GetCameraStatus( iDeviceID ) == kVideo_IsUsing )
		{
			return TRUE ;
		}		
	}
	return FALSE ;
}