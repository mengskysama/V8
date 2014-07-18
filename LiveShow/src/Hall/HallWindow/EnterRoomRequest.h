#pragma once

#include "51ktv.h"
#include "coredefine.h"
#include "roomdefine.h"
#include "HallDefine.h"
#include "EnterRoomPwdDlg.h"
#include "EnteringRoomDlg.h"

struct enterRoomInfo
{
	uint32 m_RoomID;
	bool m_hideLogin;
	std::wstring m_password;

	enterRoomInfo():m_hideLogin(false),m_password(L"")
	{
	}
};

class CHallDlg;
class CEnterRoomRequest
{
public:
	CEnterRoomRequest();
	~CEnterRoomRequest();

public:
	void InitEnterRoom(CHallDlg *hallDlg);
	void EnterRoomEventRequest(enterRoomInfo enRoom);  //申请进房
	
	void OnEnterRoomEventRep(Event const& evt);  //申请进房返回响应函数	
	void OnGetRoomInfoEventRep(Event const& evt);  //请求获取房间信息返回响应函数
	void ClearEnterRoomDataDlg();  //清除数据与销毁进房对放框

private:
	bool HaveCamera();//是否有视频设备
	bool FindRoomIDfromRoomList(uint32 findRoomID); //查询要申请的findRoomID房间号是否已打开过了
	void ClearRoomData();  //清除m_Create_And_Show
	void CreateAndShowRoom();  //创建并显示房间
	void GetRoomInfoEventRequest(uint32 roomId);  //请求获取房间信息
	void AddRecentBrowseRoom(uint32 roomId);  //添加最近浏览房间
	void CreateRoomPwdDlg(uint32 roomId,bool showPwdError);  //创建输入密码对话框
	void DestroyRoomPwdDlg();  //销毁输入密码对话框
	void CreateEnteringRoomDlg(); //创建进房中对话框
	void DestroyEnteringRoomDlg(); //销毁进房中对话框

	void CreateEnterRoomTimer(uint32 interval);  //创建申请进房定时器
	void ClearEnterRoomTimer();  //清除申请进房定时器
	
	void OnEnterRoomTimeOut(uintptr,bool);  //申请进房定时器回调函数
	CString GetUnLockRoomTime( uint16 unLockDaysLeft ) ;

	void MaxRoomEnterRoom(Event const& evt); //申请进房返回已超过最大进房数时的询问用户是否关闭以前的房间
	void CreateMaxRoomTimer(uint32 interval,uint32 roomId); //创建申请进房返回已超过最大进房数时的重新进房申请定时器
	void ClearMaxRoomTimer();  //清除重新进房申请定时器
	void OnMaxRoomEnterRoomeOut(uintptr,bool);  //申请进房返回已超过最大进房数时的重新进房申请定时器回调函数
private:
	Room_Create_And_Show *m_Create_And_Show ;
	CEnterRoomPwdDlg *m_pRoomPwdDlg;  //输入密码对话框
	CEnteringRoomDlg *m_pEnteringDlg;  //进房中对话框
	CHallDlg *m_pHallDlg;
	int32 m_EnterRoomTimer;
	int32 m_MaxRoomTimer;
	uint32 m_EnterRoomID;  //申请进房返回已超过最大进房数时重新进房ID
};