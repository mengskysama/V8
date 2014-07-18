#include "stdafx.h"
#include "EnterRoomRequest.h"
#include "../Hall.h"
#include "..\CommonLibrary\include\ui\C179MsgBox.h"
#include "../SysSettingIni.h"
#include "utility/SystemHelper.h"
#include "HallDlg.h"
#include "webdefine.h"
#include "utility\timers.h"
#include <boost/bind.hpp>

#define ENTER_ROOM_TIMEOUT_TIMER_INTERVALTIME   30   //进房超时间隔时间 （单位：秒，即30秒）   
#define ENTER_ROOM_MAX_TIMER_INTERVALTIME  1  //申请进房返回已超过最大进房数时的重新进房申请间隔时间  （单位：秒，即1秒）   

#define ENTERROOM_RESP_NOT_EXIST _T("该房间不存在")
#define ENTERROOM_RESP_CLOSED _T("管理员已关闭该房间")
#define ENTERROOM_RESP_ROOM_FULL_VIP _T("房间人数已达上限，挤不进去？")
#define ENTERROOM_RESP_ROOM_FULL_VIP2 _T("去隔壁逛逛吧                                 ")
#define ENTERROOM_RESP_ROOM_FULL_NOVIP _T(" 房间人数已滿")
#define ENTERROOM_RESP_ROOM_FULL_NOVIP2 _T("升级VIP，人滿房间任意进")
#define ENTERROOM_RESP_ROOM_FULL_NOVIP_LINKSTEXT _T("立即升级")
#define ENTERROOM_RESP_BLACKLIST _T("您被管理员禁止进入此房间")
#define ENTERROOM_RESP_BLACKLIST2 _T("寄人篱下，处处受限")
#define ENTERROOM_RESP_BLACKLIST_LINKSTEXT _T("不如拥有一个自己的房间")
#define ENTERROOM_RESP_KICKED_GUEST	L"您已被踢出房间，%d分钟后才能重新进入"
#define ENTERROOM_RESP_KICKED_GUEST_2 L"想要畅玩白兔KTV？"
#define ENTERROOM_RESP_KICKED_GUEST_LINKSTEXT L"快去注册吧"

#define ENTERROOM_RESP_KICKED _T("您已被踢出房间，%d分钟后可才能重新进入")
#define ENTERROOM_RESP_KICKED2 _T("寄人篱下，处处受限")
#define ENTERROOM_RESP_KICKED_LINKSTEXT _T("不如拥有一个自己的房间")
#define ENTERROOM_RESP_IN_ROOM _T("您已在该房间内")
#define ENTERROOM_RESP_TIMEOUT _T("连接服务器超时        ")
#define ENTERROOM_RESP_ROOM_LOCKED _T("抱歉，该房间已被封停，剩余%d天解封")
#define ENTERROOM_RESP_IP_LOCKED_LINKSTEXT _T("不如拥有一个自己的房间")
#define ENTERROOM_RESP_SERVER_BUSY _T("服务器正忙，请稍后再试")

#define ENTERROOM_ONE_ROOM_TIP _T("您确认要关闭当前房间         ")
#define ENTERROOM_ROOM_TIP _T("前往该房间？                         ")
#define ENTERRROM_TWO_ROOM_TIP _T("您是否要关闭最先打开的%d个房间")
#define ENTERRROM_TWO_ROOM_TIP2 _T("前往该房间？                                  ")


CEnterRoomRequest::CEnterRoomRequest()
{
	m_Create_And_Show = NULL;
	m_pRoomPwdDlg = NULL;
	m_pEnteringDlg = NULL;
	m_pHallDlg = NULL;
	m_EnterRoomTimer = -1;
	m_MaxRoomTimer = -1;
	m_EnterRoomID = 0;
}

CEnterRoomRequest::~CEnterRoomRequest()
{
	ClearEnterRoomDataDlg();
}

void  CEnterRoomRequest::InitEnterRoom(CHallDlg *hallDlg)
{
	m_pHallDlg = hallDlg;
}

bool CEnterRoomRequest::HaveCamera()
{
	Room_Get_Video_Device_List audiolist;
	memset(&audiolist, 0x0, sizeof(Room_Get_Video_Device_List));
	//获取视频设备列表
	g_HallModule->m_pModuleManager->CallService(Room::SERVICE_ROOM_GETVIDEO_DEVICE_LIST, (param)&audiolist);
	return audiolist.iDeviceNum > 0;
}

//查询要申请的房间号findRoomID是否已打开过了
bool CEnterRoomRequest::FindRoomIDfromRoomList(uint32 findRoomID)
{
	Room_Get_Room_List roomList;
	g_HallModule->m_pModuleManager->CallService(Room::SERVICE_ROOM_GET_ROOM_LIST, (param)&roomList);

	vector<uint32>::iterator result = find(roomList.vecRoomList.begin(),roomList.vecRoomList.end(),findRoomID);
	if(result != roomList.vecRoomList.end()) //申请的房间号ReqRoomID在已打开房间列表中找到
	{
		
		return true;
	}
	else
	{
		
		return false;
	}
	

}

//申请进房
void CEnterRoomRequest::EnterRoomEventRequest(enterRoomInfo enRoom)
{
	if(m_Create_And_Show != NULL)
	{
		assert(0);
		ClearRoomData();
	}

	ClearMaxRoomTimer();

	bool findRoomID = FindRoomIDfromRoomList(enRoom.m_RoomID);  //查询要申请的房间号m_RoomID是否已打开过了
	if(findRoomID)  //若找到则不需要再发送进房申请，而是直接打开该房间
	{
		g_HallModule->m_pModuleManager->CallService(Room::SERVICE_ROOM_ACTIVE_A_ROOM, (param)enRoom.m_RoomID);
		return;
	}	


	m_Create_And_Show = new Room_Create_And_Show ;	

	CreateEnteringRoomDlg();	

	core::ClientInfo stClientInfo(HaveCamera());
	if(enRoom.m_hideLogin)
	{
		stClientInfo.setHideLogin();
	}
	core::EnterRoomEvent * enterRoomEvent = new core::EnterRoomEvent();	
	enterRoomEvent->roomid = enRoom.m_RoomID;
	enterRoomEvent->password = enRoom.m_password;
	enterRoomEvent->stClientInfo = stClientInfo;

	Event _event = MakeEvent<MODULE_ID_HALL>()(core::EVENT_VALUE_CORE_ENTER_ROOM, 
		MODULE_ID_CORE,0,0,0,enterRoomEvent);

	g_HallModule->m_pModuleManager->PushEvent(_event);		
}

//申请进房返回响应函数
void CEnterRoomRequest::OnEnterRoomEventRep(Event const& evt)
{
	ClearEnterRoomTimer();

	core::EnterRoomRspEvent *pInfo = (core::EnterRoomRspEvent*)(evt.m_pstExtraInfo) ;

	if(m_pRoomPwdDlg != NULL && pInfo->enmResult != core::EnterRoomRspEvent::ENTERROOM_ERROR_NEED_PASSWORD && pInfo->enmResult != core::EnterRoomRspEvent::ENTERROOM_ERROR_PASSWORD_WRONG)
	{
		DestroyRoomPwdDlg();  //房间密码输入正确，销毁输入密码对话框
	}

	if(m_pEnteringDlg != NULL && pInfo->enmResult != core::EnterRoomRspEvent::ENTERROOM_ERROR_OK)  //若未进房成功，则销毁进房中对话框
	{
		DestroyEnteringRoomDlg();  //进房失败，销毁进房中对话框	 
		ClearRoomData();
	}

	CString sUrl,sTemp;
	switch(pInfo->enmResult)
	{
	case core::EnterRoomRspEvent::ENTERROOM_ERROR_OK: //请求进房成功
		{
			if(m_Create_And_Show == NULL)
			{
				assert(0);
				m_Create_And_Show = new Room_Create_And_Show;
			}
			m_Create_And_Show->stEnterRoom = *pInfo;		
			GetRoomInfoEventRequest(pInfo->unRoomID);   //请求获取房间信息
		}
		break;	

	case core::EnterRoomRspEvent::ENTERROOM_ERROR_NOT_EXIST:  //房间不存在
		{
			C179MsgBox::Show(m_pHallDlg,NewMsgBox::MSGBOX_OK_ONE_STRING,ENTERROOM_RESP_NOT_EXIST);	
		}
		break;

	case core::EnterRoomRspEvent::ENTERROOM_ERROR_CLOSED: //房间已关闭
		{
			C179MsgBox::Show(m_pHallDlg,NewMsgBox::MSGBOX_OK_ONE_STRING,ENTERROOM_RESP_CLOSED);	
		}
		break;

	case core::EnterRoomRspEvent::ENTERROOM_ERROR_ROOM_FULL:  //房间已满  
		{			
			core::CurInfo m_MyCurInfo = GetCurInfo();

			if(m_MyCurInfo.enmVIPLevel < core::ENM_VipLevel::VIPLEVEL_VIP)
			{
				//注：房间人数已达额定人数(但未达上限，VIP以下用户不能进入)
				CSysSettingIni::GetWebInterfaceUrl(SYSSET_INI_KEYNAME_ENTERROOMFULL,sUrl);  //进房时房间已满，立即升级url
				sUrl = AppendUrlParamet(sUrl);
				C179MsgBox::Show(m_pHallDlg,NewMsgBox::MSGBOX_OK_TWO_STRING_WITH_LINK,ENTERROOM_RESP_ROOM_FULL_NOVIP,ENTERROOM_RESP_ROOM_FULL_NOVIP2, L"", ENTERROOM_RESP_ROOM_FULL_NOVIP_LINKSTEXT,sUrl);
			}
			else
			{
				//超过最大进房数， (房间人数达到房间最大容纳数。X人房上限人数为1.5X  ，所有人都不可进入)
				C179MsgBox::Show(m_pHallDlg,NewMsgBox::MSGBOX_OK_TWO_STRING,ENTERROOM_RESP_ROOM_FULL_VIP,ENTERROOM_RESP_ROOM_FULL_VIP2);
			}			
		}
		break;

	case core::EnterRoomRspEvent::ENTERROOM_ERROR_BLACKLIST:  //在黑名单中
		{
			CSysSettingIni::GetWebInterfaceUrl(SYSSET_INI_KEYNAME_KICKEDREQUESTROOM,sUrl);  //不如拥有一个自己的房间url
			C179MsgBox::Show(m_pHallDlg,NewMsgBox::MSGBOX_OK_TWO_STRING_WITH_LINK,ENTERROOM_RESP_BLACKLIST,ENTERROOM_RESP_BLACKLIST2,_T(""),ENTERROOM_RESP_BLACKLIST_LINKSTEXT,sUrl);
		}
		break;

	case core::EnterRoomRspEvent::ENTERROOM_ERROR_ROOM_LOCKED:   //房间被封，后接unLockDaysLeft
		{
			if ( pInfo->unLockDaysLeft >= 365*10 )
			{				
				sTemp.Format( L"该房间因\"%s\"被永久封停" , pInfo->strFailReason.c_str() ) ;
				C179MsgBox::Show(m_pHallDlg, NewMsgBox::MSGBOX_OK_TWO_STRING ,sTemp , L"给您带来的不便，敬请谅解！") ;
			}
			else
			{
				CString strTip1 , strTip2;
				strTip1.Format( L"该房间因\"%s\"已被封停，" , pInfo->strFailReason.c_str() ) ;
				strTip2.Format( L"将于%s解封" , GetUnLockRoomTime( pInfo->unLockDaysLeft ) ) ;
				C179MsgBox::Show(m_pHallDlg, NewMsgBox::MSGBOX_OK_THREE_STRING ,strTip1 , strTip2 , L"给您带来的不便，敬请谅解！") ;
			}

		}
		break;

	case core::EnterRoomRspEvent::ENTERROOM_ERROR_KICKED:   //被踢，后接kicked_left_time
		{
			core::CurInfo m_MyCurInfo = GetCurInfo();
			if ( m_MyCurInfo.enmVIPLevel == core::ENM_VipLevel::VIPLEVEL_NONE )
			{
				CSysSettingIni::GetWebInterfaceUrl(SYSSET_INT_KEYNAME_179REGISTER,sUrl);  //被踢后 不如拥有一个自已的房间url
				sTemp.Format( ENTERROOM_RESP_KICKED_GUEST , pInfo->unKickTimeLeft ) ;
				C179MsgBox::Show( m_pHallDlg,NewMsgBox::MSGBOX_GUEST_OPERATE ,sTemp , ENTERROOM_RESP_KICKED_GUEST_2 , L"" , ENTERROOM_RESP_KICKED_GUEST_LINKSTEXT , sUrl ) ;
			}
			else
			{
				CSysSettingIni::GetWebInterfaceUrl(SYSSET_INI_KEYNAME_KICKEDREQUESTROOM,sUrl);  //被踢后 不如拥有一个自已的房间url
				sTemp.Format(ENTERROOM_RESP_KICKED,pInfo->unKickTimeLeft);
				C179MsgBox::Show(m_pHallDlg,NewMsgBox::MSGBOX_OK_TWO_STRING_WITH_LINK,sTemp.GetBuffer(),ENTERROOM_RESP_KICKED2,_T(""),ENTERROOM_RESP_KICKED_LINKSTEXT,sUrl);
			}			
		}
		break;

	case core::EnterRoomRspEvent::ENTERROOM_ERROR_NEED_PASSWORD:   //房间被加锁，请输入密码
		{				
			CreateRoomPwdDlg(pInfo->unRoomID,false);			
		}
		break;

	case core::EnterRoomRspEvent::ENTERROOM_ERROR_PASSWORD_WRONG:  //房间密码输入错误，请重新输入密码
		{
			CreateRoomPwdDlg(pInfo->unRoomID,true);			
		}
		break;

	case core::EnterRoomRspEvent::ENTERROOM_ERROR_ALREADY_IN_ROOM:   //已经在房间里
		{
			C179MsgBox::Show(m_pHallDlg,NewMsgBox::MSGBOX_OK_ONE_STRING,ENTERROOM_RESP_IN_ROOM);
		}
		break;

	case core::EnterRoomRspEvent::ENTERROOM_ERROR_TIMEOUT:   //超时
		{
			C179MsgBox::Show(m_pHallDlg,NewMsgBox::MSGBOX_ICO_OK_ONE_STRING, NewMsgBox::ICONTYPE_ERROR,ENTERROOM_RESP_TIMEOUT);
		}
		break;

	case core::EnterRoomRspEvent::ENTERROOM_ERROR_IP_LOCKED:   //IP被锁
		{
			sTemp.Format( L"%d分钟后可重新进入" ,pInfo->unKickTimeLeft ) ;
			C179MsgBox::Show( m_pHallDlg , NewMsgBox::MSGBOX_OK_TWO_STRING , L"目前您已被封停IP" , sTemp.GetBuffer()  ) ;
		}
		break;

	case core::EnterRoomRspEvent::ENTERROOM_ERROR_OTHER:    //其它
	case core::EnterRoomRspEvent::ENTERROOM_ERROR_UNKNOWN:   //未知
		{
			C179MsgBox::Show(m_pHallDlg,NewMsgBox::MSGBOX_ICO_OK_ONE_STRING, NewMsgBox::ICONTYPE_ERROR,ENTERROOM_RESP_SERVER_BUSY);
		}
		break;
	case core::EnterRoomRspEvent::ENTERROOM_ERROR_MAX_ROOM_NUM://超过最大进房数  当前用户已打开房间至级别最多可打开数
		{			
			MaxRoomEnterRoom(evt);
		}
		break;
	case core::EnterRoomRspEvent::ENTERROOM_ERROR_OPNE_ROOM_TOOBIG:
		{
			C179MsgBox::Show(m_pHallDlg, NewMsgBox::MSGBOX_OK_ONE_STRING, L"抱歉，您打开的房间数量已达上限");
		}
		break;
	default:
		C179MsgBox::Show(m_pHallDlg,NewMsgBox::MSGBOX_ICO_OK_ONE_STRING, NewMsgBox::ICONTYPE_ERROR,ENTERROOM_RESP_SERVER_BUSY);
		break;
	}

}

//申请进房返回已超过最大进房数时的询问用户是否关闭以前的房间
void CEnterRoomRequest::MaxRoomEnterRoom(Event const& evt)
{
	Room_Get_Room_List roomList;
	g_HallModule->m_pModuleManager->CallService(Room::SERVICE_ROOM_GET_ROOM_LIST, (param)&roomList);

	if(roomList.vecRoomList.size() == 0)
	{
		C179MsgBox::Show(m_pHallDlg,NewMsgBox::MSGBOX_ICO_OK_ONE_STRING, NewMsgBox::ICONTYPE_ERROR,ENTERROOM_RESP_SERVER_BUSY);
		return ;  //当前已无房间， 但服务器返回超过最大进房数时，为错误信息
	}

	core::EnterRoomRspEvent *pInfo = (core::EnterRoomRspEvent*)(evt.m_pstExtraInfo) ;

	//否则需先关闭以前的首个房间，再发送进房申请
	core::CurInfo m_MyCurInfo = GetCurInfo();
	int msgbox; 
	int ifristNum;	
	if(roomList.vecRoomList.size() > 1)  //可打开多个房间
	{
		ifristNum = roomList.vecRoomList.size() - pInfo->unMaxRoomNum + 1;  ;//需关闭前ifristNum个房间后才可打开新的房间
		if(ifristNum > 0)
		{
			CString roomTip;
			roomTip.Format(ENTERRROM_TWO_ROOM_TIP,ifristNum);
			msgbox = C179MsgBox::Show(m_pHallDlg,NewMsgBox::MSGBOX_ICO_OK_CANCEL_TWO_STRING ,NewMsgBox::ICONTYPE_WARNING,roomTip,ENTERRROM_TWO_ROOM_TIP2);	
		}		
		
	}
	else //最多只能打开一个房间
	{
		ifristNum = 1;
		msgbox = C179MsgBox::Show(m_pHallDlg,NewMsgBox::MSGBOX_ICO_OK_CANCEL_TWO_STRING ,NewMsgBox::ICONTYPE_WARNING, ENTERROOM_ONE_ROOM_TIP,ENTERROOM_ROOM_TIP);
	}

	if(msgbox == RET_OK) //用户选择关闭以前的N个房间，打开新的房间
	{
		for(int i = 0; i<ifristNum; i++)
		{
			g_HallModule->m_pModuleManager->CallService(Room::SERVICE_ROOM_CLOSE_A_ROOM, (param)roomList.vecRoomList[i]);			
		}

		CreateMaxRoomTimer(ENTER_ROOM_MAX_TIMER_INTERVALTIME,pInfo->unRoomID);	
		m_pHallDlg->EnableWindow(FALSE);
		
	}
	else 
	{
		return; //用户选择取消，不再申请进房
	}


}

//请求获取房间信息
void CEnterRoomRequest::GetRoomInfoEventRequest(uint32 roomId) 
{
	Event _event = MakeEvent<MODULE_ID_HALL>()(core::EVENT_VALUE_CORE_GET_ROOMINFO, 
		MODULE_ID_CORE,roomId,0,0,NULL);

	IModuleManager * pModuleManager = g_HallModule->m_pModuleManager;
	if (pModuleManager != NULL)
	{
		pModuleManager->PushEvent(_event);
	}
	CreateEnterRoomTimer(ENTER_ROOM_TIMEOUT_TIMER_INTERVALTIME);
}

//创建输入密码对话框
void  CEnterRoomRequest::CreateRoomPwdDlg(uint32 roomId,bool showPwdError)
{
	if(m_pRoomPwdDlg == NULL)
	{
		SwitchResourceToModule(_T("Hall"));		
		m_pRoomPwdDlg = new CEnterRoomPwdDlg(this,m_pHallDlg);
		m_pRoomPwdDlg->Create(CEnterRoomPwdDlg::IDD);
		m_pRoomPwdDlg->SetEnterRoomID(roomId);
		m_pRoomPwdDlg->ShowWindow(SW_SHOW);
	}	
	else
	{
		m_pRoomPwdDlg->SetEnterRoomID(roomId);
		m_pRoomPwdDlg->ShowWindow(SW_SHOW);
	}

	m_pHallDlg->EnableWindow(FALSE);
	CComVariant curVerNum = showPwdError?L"true":L"false";  //是否显示密码输入错误的提示信息
	m_pRoomPwdDlg->m_pSkinEngine->SetSubSkinProp(m_pRoomPwdDlg->m_hWnd,L"textPwdError",L"visible",curVerNum);
}

//销毁输入密码对话框
void CEnterRoomRequest::DestroyRoomPwdDlg()
{
	if(m_pRoomPwdDlg != NULL)
	{
		m_pRoomPwdDlg->DestroyWindow();
		delete m_pRoomPwdDlg;
		m_pRoomPwdDlg = NULL;

		if(m_pHallDlg != NULL && m_pHallDlg->GetSafeHwnd())
		{
			m_pHallDlg->EnableWindow(TRUE);
			m_pHallDlg->ShowWindow(SW_SHOW);
			if(m_pHallDlg->IsIconic()) //窗口是否最小化
			{
				m_pHallDlg->ShowWindow(SW_RESTORE);
			}
			m_pHallDlg->SetForegroundWindow();				
		}		
	}
}

//创建进房中对话框
void CEnterRoomRequest::CreateEnteringRoomDlg()
{
	if(m_pEnteringDlg == NULL)
	{
		SwitchResourceToModule(_T("Hall"));		
		m_pEnteringDlg = new CEnteringRoomDlg(m_pHallDlg,this);
		if(m_pEnteringDlg != NULL)
		{
			m_pEnteringDlg->Create(CEnteringRoomDlg::IDD);
			m_pEnteringDlg->ShowWindow(SW_SHOW);
			m_pHallDlg->EnableWindow(FALSE);
		}		
	}	
	else
	{
		m_pEnteringDlg->ShowWindow(SW_SHOW);
		m_pHallDlg->EnableWindow(FALSE);
	}
	CreateEnterRoomTimer(ENTER_ROOM_TIMEOUT_TIMER_INTERVALTIME);	
}

//销毁进房中对话框
void CEnterRoomRequest::DestroyEnteringRoomDlg()
{
	if(m_pEnteringDlg != NULL)
	{
		m_pEnteringDlg->DestroyWindow();
		delete m_pEnteringDlg;
		m_pEnteringDlg = NULL;		

		if(m_pHallDlg != NULL && m_pHallDlg->GetSafeHwnd())
		{
			m_pHallDlg->EnableWindow(TRUE);
			m_pHallDlg->ShowWindow(SW_SHOW);
			if(m_pHallDlg->IsIconic()) //窗口是否最小化
			{
				m_pHallDlg->ShowWindow(SW_RESTORE);
			}
			m_pHallDlg->SetForegroundWindow();				
		}		
	}
	ClearEnterRoomTimer();	
}

//请求获取房间信息返回响应函数
void CEnterRoomRequest::OnGetRoomInfoEventRep(Event const& evt)
{
	core::GetRoomInfoRspEvent *pInfo = (core::GetRoomInfoRspEvent*)(evt.m_pstExtraInfo) ;	
	if(m_Create_And_Show == NULL || pInfo->unRoomID != m_Create_And_Show->stEnterRoom.unRoomID)  
	{
		assert(0);		
		ClearEnterRoomDataDlg();//清除数据与销毁进房对放框
		return;
	}

	ClearEnterRoomTimer();
	m_Create_And_Show->stRoomInfo = (*pInfo);

	DestroyEnteringRoomDlg();  //进房成功，销毁进房中对话框
	CreateAndShowRoom();  //创建并显示房间

	ClearRoomData();
	::PostMessage(m_pHallDlg->GetSafeHwnd(),WM_SYSCOMMAND, SC_MINIMIZE, 0);  //大厅最小化
}

//清除数据与销毁进房对放框
void CEnterRoomRequest::ClearEnterRoomDataDlg()
{
	ClearRoomData();
	DestroyRoomPwdDlg();
	DestroyEnteringRoomDlg();
}

void CEnterRoomRequest::ClearRoomData()
{
	if(m_Create_And_Show != NULL)
	{
		delete m_Create_And_Show;
		m_Create_And_Show = NULL;
	}
}


//创建并显示房间
void CEnterRoomRequest::CreateAndShowRoom()
{
	IModuleManager * pModuleManager = g_HallModule->m_pModuleManager;
	if (pModuleManager != NULL)
	{			
		pModuleManager->CallService( Room::SERVICE_ROOM_CREATE_AND_SHOW ,(param)m_Create_And_Show) ;		
	}
}

void CEnterRoomRequest::CreateEnterRoomTimer(uint32 interval)
{
	if(m_EnterRoomTimer != -1)
	{
		assert(0);
		ClearEnterRoomTimer();
	}

	m_EnterRoomTimer = common::utility::_Timers::instance().CreateTimer(interval, 
		common::utility::_Timers::TimerCallback(
		boost::bind(&CEnterRoomRequest::OnEnterRoomTimeOut,this,boost::arg<1>(),boost::arg<2>())), 
		common::utility::_Timers::ENM_TIMER_HALL_ENTERROOM);
}

void CEnterRoomRequest::ClearEnterRoomTimer()
{
	if(m_EnterRoomTimer != -1)
	{
		common::utility::_Timers::instance().DestroyTimer(m_EnterRoomTimer);
		m_EnterRoomTimer = -1;
	}
}


//申请进房定时器回调函数
void CEnterRoomRequest::OnEnterRoomTimeOut(uintptr,bool)
{
	ClearRoomData();

	if( m_pEnteringDlg !=NULL && m_pEnteringDlg->GetSafeHwnd() && m_pEnteringDlg->IsWindowVisible()) //申请进房超时
	{
		ClearEnterRoomTimer();
		DestroyEnteringRoomDlg(); //销毁进房对放框
		DestroyRoomPwdDlg();
		C179MsgBox::Show(m_pHallDlg,NewMsgBox::MSGBOX_ICO_OK_ONE_STRING, NewMsgBox::ICONTYPE_ERROR,_T("连接服务器超时        "));			
	}
}


//创建申请进房返回已超过最大进房数时的重新进房申请定时器
void CEnterRoomRequest::CreateMaxRoomTimer(uint32 interval,uint32 roomId)
{
	if(m_MaxRoomTimer != -1)
	{
		assert(0);
		ClearMaxRoomTimer();
	}

	m_EnterRoomID = roomId;
	m_MaxRoomTimer = common::utility::_Timers::instance().CreateTimer(interval, 
		common::utility::_Timers::TimerCallback(
		boost::bind(&CEnterRoomRequest::OnMaxRoomEnterRoomeOut,this,boost::arg<1>(),boost::arg<2>())), 
		common::utility::_Timers::ENM_TIMER_HALL_MAXROOM,1);
}

void CEnterRoomRequest::ClearMaxRoomTimer()
{
	if(m_MaxRoomTimer != -1)
	{
		common::utility::_Timers::instance().DestroyTimer(m_MaxRoomTimer);
		m_MaxRoomTimer = -1;
	}

	m_EnterRoomID = 0;
}


//申请进房返回已超过最大进房数时的重新进房申请定时器回调函数
void CEnterRoomRequest::OnMaxRoomEnterRoomeOut(uintptr,bool)
{
	enterRoomInfo roomid;
	roomid.m_RoomID = m_EnterRoomID;				
	m_pHallDlg->m_EnterRoomRequest.EnterRoomEventRequest(roomid);
}


CString CEnterRoomRequest::GetUnLockRoomTime( uint16 unLockDaysLeft ) 
{
	time_t nNow = time( NULL ) ;
	nNow += (unLockDaysLeft*24*3600) ;
	struct tm *pTime = localtime( &nNow ) ;
	CString strTemp ;
	strTemp.Format( L"%d年%d月%d日" , pTime->tm_year+1900 , pTime->tm_mon+1 , pTime->tm_mday ) ;
	return strTemp ;
}