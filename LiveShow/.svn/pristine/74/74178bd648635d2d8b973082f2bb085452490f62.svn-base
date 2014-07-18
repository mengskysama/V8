#pragma once

#include "51ktv.h"
#include "..\personalsetting/VideoManager.h"

#define MAX_AUDIO_DEVICE_NUM             10
#define MAX_VIDEO_DEVICE_NUM             10
#define DEVICE_NAME_LEN					128


struct  Get_Video_Device_List		//获取视频设备列表
{
	int         iDeviceNum;              //设备数量
	wchar_t     wsDeviceName[MAX_VIDEO_DEVICE_NUM][DEVICE_NAME_LEN];
	bool        bVirtual[MAX_VIDEO_DEVICE_NUM]; //是否为虚拟视频
};

struct  Get_Video_DeviceInfo_List		//获取视频设备列表
{
	int         iDeviceNum;              //设备数量
	wchar_t     wsDeviceName[MAX_VIDEO_DEVICE_NUM][DEVICE_NAME_LEN];
	wchar_t		wsDeviceGUID[MAX_VIDEO_DEVICE_NUM][MAX_VIDEO_DEVICE_GUID_LEN];
	bool        bVirtual[MAX_VIDEO_DEVICE_NUM]; //是否为虚拟视频
};

struct Show_Video_Ajust_Dlg   //显示视频属性调整对话框
{
	HWND        hParentWnd;
	uint32      iDeviceID;
};

struct Set_Video_Device  //设置视频设备
{
	int iDeviceId ;
};

struct Start_Video_Device  //启动设置的视频设备
{
	int iDeviceId ;
	HWND hWnd ;
	uint32 iResult ;
};

struct Stop_Video_Device  //停止视频设备
{ 
	int iDeviceId ;
	HWND hWnd ;
};


//视频设置
class CVideoSetting
{
public:
	CVideoSetting(){};   // 标准构造函数
	virtual ~CVideoSetting(){};

public:
	static void GetVideoDeviceList(Get_Video_DeviceInfo_List &VideoList);  //获取视频列表
	static void TestStopVideoDevice(Stop_Video_Device &videoDevice); //停止视频设备
	static void TestStartVideoDevice(Start_Video_Device &videoDevice); //启动视频
	static void TestSetVideoDevice(Set_Video_Device &videoDevice);  //设置视频
	static void ShowVideoAjustDlg(Show_Video_Ajust_Dlg &videoAjust);    //显示视频属性对话框
	static BOOL IsCameraUsing( int iDeviceID ) ; 

};
