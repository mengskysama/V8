#pragma once
#include "stdafx.h"
#include "51ktv.h"
#include "coredefine.h"
#include "AvDefine.h"

namespace Room
{
	enum E_RoomEventValue
	{
		EVENT_VALUE_ROOM_MODULE_BEGIN = EVENT_VALUE_ROOM_BEGIN,

		EVENT_VALUE_ROOM_SHOW_REGISTER,			//大厅弹出注册框

		EVENT_VALUE_ROOM_MODULE_END,
	};
	enum 
	{
		service_value_room_module_begin = SERVICE_VALUE_ROOM_BEGIN ,
		SERVICE_ROOM_CREATE_AND_SHOW,
		SERVICE_ROOM_GETAUDIO_DEVICE_LIST,		//获取音频输入输出设备列表
		SERVICE_ROOM_GETVIDEO_DEVICE_LIST,		//获取视频设备列表
		SERVICE_ROOM_SAVE_PHOTO_TOBMP,			//保存拍照为BMP到指定的路径 BMP格式
		SERVICE_ROOM_GET_NET_STATE,				//获取自己的网络状态
		SERVICE_ROOM_GET_ROOM_LIST,				//获取打开的房间列表
		SERVICE_ROOM_CLOSE_A_ROOM,				//关闭一个房间
		SERVICE_ROOM_ACTIVE_A_ROOM,				//激活显示出一个房间
		SERVICE_ROOM_CLOSED_REGISTER_DLG,		//关闭了注册对话框

		SERVICE_ROOM_SHOW_SYSSETTING,                //显示系统设置对话框  &SysSettingType
		SERVICE_ROOM_GET_RICHLEVEL_INFO,		//获得富豪等级信息，名字，图片
		SERVICE_VALUE_ROOM_MODULE_END ,

	};
};

enum SysSettingDlgType
{
	Show_User_Info_Dlg      =  0,  //基本资料	
	Show_Basic_Setting_Dlg      ,  //常规设置
	Show_Audio_Setting_Dlg      ,  //视频设置
	Show_PwdSafe_Setting_Dlg    ,  //密码安全
	Show_MgrRoom_Setting_Dlg    ,  //我的房间
	Show_BankAccount_Dlg		,  //银行账号
	Show_My_Account_Dlg         ,  //我的账户
	Show_Fans_Dlg				,  //粉丝排行
	Show_Sound_Setting_Dlg      ,  //音频设置
	Show_Message_Setting_Dlg    ,  //消息设置
};

struct SysSettingType
{
	SysSettingDlgType dlgType;
	HWND pParentWnd;
};

struct RoomShowRegister : public ExtraInfo
{		
	HWND m_parentHwnd;  
};

struct Room_Create_And_Show
{
	core::EnterRoomRspEvent stEnterRoom;
	core::GetRoomInfoRspEvent stRoomInfo;
};

#define MAX_AUDIO_DEVICE_NUM             10
#define MAX_VIDEO_DEVICE_NUM             10
#define DEVICE_NAME_LEN					128

struct Room_Get_Audio_Device_List		//获取音频输入输出设备列表
{
	int			iDeviceNum;
	wchar_t     wsDeviceName[MAX_AUDIO_DEVICE_NUM][DEVICE_NAME_LEN];
	bool		bInput;
};
struct  Room_Get_Video_Device_List		//获取视频设备列表
{
	int         iDeviceNum;              //设备数量
	wchar_t     wsDeviceName[MAX_VIDEO_DEVICE_NUM][DEVICE_NAME_LEN];
};

struct Room_Get_Video_Device_Info_List
{
	int			iDeviceNum;
	Service_Video_Info stDeviceInfo[MAX_VIDEO_DEVICE_NUM];
};

struct Room_Show_Video_Ajust_Dlg   //显示视频属性调整对话框
{
	HWND        hParentWnd;
	uint32      iDeviceID;
};

struct Room_Test_Set_Video_Device  //设置视频设备
{
	int iDeviceId ;
};

struct Room_Test_Stop_Video_Device
{
	int iDeviceId ;
	HWND hWnd ;
};

struct Room_Save_Photo_ToBmp		//保存拍照为BMP到指定的路径 BMP格式
{
	HWND hWnd ;
	std::wstring strPath ;
};

struct Room_Get_Net_State			//获取自己的网络状态
{
	float fRate ;
};

struct Room_Get_Room_List			//获取打开的房间列表
{
	std::vector<uint32> vecRoomList;
};

struct Room_Get_Rich_Level_Info
{
	uint8 unRichLevel;
	std::wstring strLevelName;
	std::wstring strImgPath;
};
