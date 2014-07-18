#include "StdAfx.h"
#include "HallModule.h"
#include "HallWindow/HallDlg.h"
#include "LoginWindow/LoginDlg.h"
#include "HallDefine.h"
#include "Hall.h"
#include "SysSettingIni.h"
#include "..\CommonLibrary\include\utility\SystemHelper.h"
#include "../CommonLibrary/include/utility/StringHelper.h"
#include "..\CommonLibrary\include\ui\MenuEx.h"
#include "..\CommonLibrary\include\ui\UIHelper.h"
#include "coredefine.h"
#include "webdefine.h"
#include "LoginWindow/VerifyDlg.h"
#include "LoginWindow/LoginVerifyHander.h"
#include "SysNotify/SysNotifyDlg.h"
#include "RoomDefine.h"
#include "..\CommonLibrary\include\ui\C179MsgBox.h"
#include "LoginWindow/RegisterDlg.h"
#include "HallWindow/SearchDlg.h"
#include "UserInfo.h"
#include "../CommonLibrary/include/utility/ConfigManager.h"
#include "GlobalManager.h"

#define COMMAND_TOKEN_51AUTOLOGIN L"caihong" //51彩红登录179客户端
#define COMMAND_TOKEN_51_ENTER_ROOM L"room" //51彩红登录179客户端进房命令
#define COMMAND_TOKEN_PREFIX L"x179://" //进房命令
#define COMMAND_TOKEN_ENTER_ROOM L"go" //进房命令

HallModule::HallModule(void)
:m_pModuleManager(NULL)
{
	m_pHallDlg = NULL;
	m_pLoginDlg=NULL;	
	m_pHeadSettingDlg = NULL;
	m_trayMgrCur = 0;
	m_bCommandLineProcessed = false;
}

HallModule::~HallModule(void)
{
	//程序退出时删除托盘
	if (NULL != m_trayMgr.GetWindowHandle())
	{
		m_trayMgr.DeleteEventHandler(this);
		m_trayMgr.Delete(true);	
	}	
}


core::CurInfo GetCurInfo()
{
	core::CurInfo user_info;
	IModuleManager * pModuleManager = g_HallModule->m_pModuleManager;
	if (pModuleManager != NULL)
	{
		pModuleManager->CallService(core::SERVICE_VALUE_CORE_GET_CUR_INFO, (param)&user_info);
	}

	return user_info;
}


void HallModule::Load(IModuleManager* p)
{
	m_pModuleManager = p;
	DeferPicPathMgr->LoadDeferPicPathList() ;
}

void HallModule::Unload()
{
	DeferPicPathMgr->Release() ;

	CloseNonModalDlg((CDialog*&)m_pLoginDlg);
	CloseNonModalDlg((CDialog*&)m_pHallDlg);
	CloseNonModalDlg((CDialog*&)m_pHeadSettingDlg);

	for(std::map<CString, common::ui::CImageImpl*>::iterator it = m_mapPath2Image.begin();
		it != m_mapPath2Image.end(); ++it)
	{
		it->second->Destroy();
	}
	m_mapPath2Image.clear();
}

void HallModule::ProcessEvent(Event const& evt)
{
	switch(evt.eventValue)
	{
	case Hall::EVENT_VALUE_HALL_ENTER_ROOM_REQUEST:  //申请进房
		{							
			enterRoomInfo roomid;
			roomid.m_RoomID = evt.param0;				
			m_pHallDlg->m_EnterRoomRequest.EnterRoomEventRequest(roomid);
		}
		break;
	case core::EVENT_VALUE_CORE_ENTER_ROOM_RSP: //申请进房返回消息
		{				
			m_pHallDlg->m_EnterRoomRequest.OnEnterRoomEventRep(evt);
		}
		break;
	case core::EVENT_VALUE_CORE_GET_ROOMINFO_RSP: //请求获取房间信息返回消息
		{
			m_pHallDlg->m_EnterRoomRequest.OnGetRoomInfoEventRep(evt);
		}
		break;
	case Hall::EVENT_VALUE_HALL_CREATEANDSHOW:  //创建并显示大厅
		{
			ProcessHallCreateAndShow(evt);
		}
		break;
	case Hall::EVENT_VALUE_HALL_HEAD_UPLOAD:  //头像上传
		{
			PrcoessHallHeadUpload(evt);
		}
		break;
	case Hall::EVENT_VALUE_HALL_ACTIVATE_SHOW: //大厅激活显示
		{
			PrcoessHallActivateShow(evt);
		}
		break;
	case Hall::EVENT_VALUE_HALL_InLineURL_SHOW: //大厅右侧打开内嵌链接  (大厅右侧内嵌页面)
		{
			PrcoessInLineURLShow(evt);
		}
		break;
	case core::EVENT_VALUE_CORE_BE_KICKED_NOTIFY:  //账号重登陆，老用户大厅被踢
		{
			ProcessKicked(evt);
		}
		break;
	case core::EVENT_VALUE_CORE_LOGIN_RSP:  //登录返回响应
		{
			ProcessLoginResp(evt);			
		}
		break;			
	case core::EVENT_VALUE_CORE_NOTIFY_SYSTEM_MESSAGE:   //大厅通知
		{
			m_SysMessagePopup.OnSystemMessageEvent(evt);  //系统弹窗响应函数	
		}
		break;
	case webinterface::event_value_web_get_login_identify_code_resp:  //获取登陆验证码响应
		{
			ProcessWebGetLoginIdentifyResp(evt);
		}
		break;
	case webinterface::event_value_web_download_file_resp:  //web下载完毕时的返回响应
		{
			ProcessWebDownFileResp(evt);
		}
		break;
	case webinterface::event_value_web_download_head_image_resp:  //web下载头像完毕时的返回响应
		{
			ProcessWebHeadImageResp(evt);
		}
		break;
	case webinterface::event_value_web_get_http_resp:     //通用的Web访问接口返回响应
		{
			ProcessWebGetHttpResp(evt);
		}
		break;	
	case webinterface::event_value_web_upload_head_image_resp: //上传用户头像响应
		{
			ProcessWebHeadUpLoadResp(evt);
		}
		break;		
	case core::EVENT_VALUE_CORE_NOTIFY_PASSWORD_SAFE:
		{
			ProcessPassWordSafe( evt ) ;
		}
		break;
	case core::EVENT_VALUE_CORE_NOTIFY_MY_INFO_UPDATE:  //我的信息变更(昵称，性别)
		{
			PrcoessMyInfoUpdate(evt);
		}
		break;	
	case core::EVENT_VALUE_CORE_DISCONNECTED_NOTIFY:  //断线了
		{
			ProcessDisconnected(evt);
		}
		break;
	case Room::EVENT_VALUE_ROOM_SHOW_REGISTER:
		{
			ProcessRegisterResp(evt);
		}
		break;
	case core::EVENT_VALUE_CORE_NOTIFY_RICH_UPDATE_MONEY_CHANGE:  //富豪等级升级所需要的钱更新
		{
			ProcessRichUpdateMoneyResp(evt);
		}
		break;
	default:
		break;
	}
}

void HallModule::PrcoessHallActivateShow(Event const &evt)
{
	if( m_pHallDlg != NULL)
	{
		if(m_pHallDlg->IsIconic()) //窗口是否最小化
		{
			m_pHallDlg->ShowWindow(SW_RESTORE);
		}
		m_pHallDlg->SetForegroundWindow();
	}
	
}

void HallModule::PrcoessInLineURLShow(Event const &evt)
{
	ASSERT(evt.m_pstExtraInfo != NULL);
	Hall::InLineUrl *pInfo = (Hall::InLineUrl*)(evt.m_pstExtraInfo) ;
	
	m_pHallDlg->SetAllRoomBrowser(pInfo->m_InLineUrl);  //设置并打开大厅右侧内嵌指定页面
	PrcoessHallActivateShow(evt);  //大厅激活显示

}


void HallModule::ProcessMessage(Message const& msg)
{
	switch(msg.messageValue)
	{
	case core::MESSAGE_VALUE_CORE_LOGIN_COMPLETE:
		{
			
		}
		break;
	case core::MESSAGE_VALUE_CORE_PRE_LOGIN: //切换用户
		{
			m_pHallDlg->CloseAllDialog();
			m_pHallDlg->HeadInit();//头部信息初始化
			CloseNonModalDlg((CDialog*&)m_pHeadSettingDlg);
			m_SysMessagePopup.ClearSysMessage();
		}
		break;
	case Hall::MESSAGE_VALUE_HALL_HEAD_IMAGE_UPDATE:  //有最新头像下载完毕
		{			
			m_pHallDlg->OnHeadImageUpdate(msg.param0);

		}
		break;
	case core::MESSAGE_VALUE_CORE_MY_VIPLEVEL_UPDATE:  //vip等级变更
		{
			MessageMyVipLevelUpdate(msg);
		}
		break;
	case core::MESSAGE_VALUE_CORE_MY_RICHLEVEL_UPDATE:  //富豪等级变更
		{
			MessageMyRichLevelUpdate(msg);
		}
		break;
	
	default:
		break;
	}
	
}

int32 HallModule::Invoke(ServiceValue const serviceValue, param st)
{
	switch(serviceValue)
	{	
	case Hall::SERVICE_VALUE_HALL_GET_WEBINTERFACEURL:
		return InvokeGetServiceWebInterfaceUrl(st);
		break;
	case Hall::SERVICE_VALUE_HALL_GET_TOKEN_URL:
		return InvokeGetTokenURL(st);
		break;
	case Hall::SERVICE_VALUE_HALL_OPEN_TOKEN_URL:
		return InvokeOpenTokenURL(st);
		break;
	case Hall::SERVICE_MSGBOX_SHOW:
		return InvokeMsgBoxShow(st);
		break;	
	case Hall::SERVICE_VALUE_HALL_GET_HEAD_IMAGE_PATH:
		return InvokeGetHeadImagePath(st); //SERVICE 获取指定用户unUIN的头像路径
		break;
	case Hall::SERVICE_VALUE_HALL_UPDATE_HEAD_IMAGE_PATH:
		return InvokeUpDateHeadImagePath(st); //SERVICE 获取指定用户unUIN的头像路径并下载最新头像
		break;
	default:
		break;
	}

	return -1;
}

void HallModule::OnTrayEvent(WPARAM w, LPARAM l)
{
	uint32 uMsgId = l;

	switch(uMsgId){
		case WM_RBUTTONUP:
			{
				CreateHallTrayMenu();
			}
			break;
		case WM_LBUTTONDBLCLK:  //双击托盘图标
			{
				//以下代码请谨慎修改 plushu 2012-8-3

				if (m_pHallDlg != NULL && m_pLoginDlg != NULL)
				{
					bool bLoginDlgShow = m_pLoginDlg->IsWindowVisible();

					if(m_pHallDlg->IsIconic()) //窗口是否最小化
					{
						m_pHallDlg->ShowWindow(SW_RESTORE);
					}
					m_pHallDlg->SetForegroundWindow();

					if(bLoginDlgShow)
					{
						if(m_pLoginDlg->IsIconic()) //窗口是否最小化
						{
							m_pLoginDlg->ShowWindow(SW_RESTORE);
						}
						m_pLoginDlg->SetForegroundWindow();
					}
					else
					{
						m_pLoginDlg->ShowWindow(SW_HIDE);
					}
				}
			}

			break;
		default:
			break;
	}
}

void HallModule::OnFlashStart(DWORD dwParam)
{

}

void HallModule::OnFlashEnd(DWORD dwParam)
{

}

void HallModule::ShowLoginDialog(bool bShow)
{
	if(m_pHallDlg != NULL)
	{
		m_pHallDlg->EnableWindow(bShow ? FALSE:TRUE);
	}

	if(m_pLoginDlg != NULL)
	{
		m_pLoginDlg->ShowWindow(bShow ? SW_SHOW:SW_HIDE);
		if(bShow && m_pLoginDlg->IsIconic()) //窗口是否最小化
		{			
			m_pLoginDlg->ShowWindow(SW_RESTORE);
			m_pLoginDlg->CenterWindow(m_pHallDlg);
		}
		
	}			
}


CWnd* HallModule::GetLoginDlgWnd()
{
	return m_pLoginDlg;
}

void HallModule::CloseHallRegisterDlg()
{
	if(m_pHallDlg != NULL)
	{
		m_pHallDlg->CloseRegisterDlg();		
	}
}

void HallModule::CloseHallGameDlg()
{
	if(m_pHallDlg != NULL)
	{
		m_pHallDlg->CloseHallGameDlg();		
	}
}

//隐藏大厅，登录框，退出大厅托盘图标
void HallModule::HideLoginHallDialog()
{
	if(m_pHallDlg != NULL)
	{
		m_pHallDlg->ModifyStyleEx(WS_EX_APPWINDOW,WS_EX_TOOLWINDOW);  //不在系统状态栏上显示运行时的标题
		m_pHallDlg->ShowWindow(SW_HIDE);		
	}

	if(m_pLoginDlg != NULL)
	{
		m_pLoginDlg->ShowWindow(SW_HIDE);
	}		

	if (NULL != m_trayMgr.GetWindowHandle())
	{
		m_trayMgr.DeleteEventHandler(this);
		m_trayMgr.Delete(true);	
	}

	
	
}

void HallModule::ShowLoginHallDialog()
{
	if(m_pHallDlg != NULL)
	{
		m_pHallDlg->ShowWindow(SW_SHOW);
		m_pHallDlg->EventUpdateTicker();
		m_pHallDlg->InitAllRoomBrowser();
	}

	ShowLoginDialog(TRUE);

}

 //获取本地头像图片路径，若未找到指定unUIN的对应头像图片则使用默认男女图片， 当unUIN为0时，使用默认游客图片
CString HallModule::GetLoaclHeadImage(uint32 unUIN,bool bMan)
{
	CString headPath;
	if(unUIN > 0 )
	{
		headPath =  m_headDownload.GetLocalHeadImage(unUIN,bMan);
	}
	else
	{
		headPath =  m_headDownload.GetGuestHeadImage();
	}
	return headPath;
}

int32 HallModule::InvokeGetServiceWebInterfaceUrl(param st)
{
	Hall::WebInterfaceUrl *pWebUrl = (Hall::WebInterfaceUrl *)st;
	if(pWebUrl != NULL)
	{
		CString sUrl,sTemp;
		CSysSettingIni::GetWebInterfaceUrl(pWebUrl->m_WebInterfaceKeyName.c_str(),sTemp);  //获取WebInterfaceUrl节下的键值url
		if(pWebUrl->m_WebWithPara)  //在web接口之后带参, (url之后加参数uin,tokentype,token)  
		{
			sUrl = AppendUrlParamet(sTemp);
		}
		else  //web接口之后不带参
		{
			sUrl = sTemp;
		}

		pWebUrl->m_WebInterfaceUrl = sUrl.GetBuffer();
		return 0;
	}
	return -1;
}

int32 HallModule::InvokeGetTokenURL(param st)
{
	std::wstring& strURL = *(std::wstring*)st;
	CString strURL2 = AppendUrlParamet(strURL.c_str());
	strURL = strURL2.GetBuffer();
	return 0;
}

int32 HallModule::InvokeOpenTokenURL(param st)
{
	InvokeGetTokenURL(st);
	std::wstring& strURL = *(std::wstring*)st;
	common::utility::systemhelper::OpenUrl(strURL);
	return 0;
}

int32 HallModule::InvokeMsgBoxShow(param st)
{
	m_pHallDlg->ShowWindow(SW_SHOW);
	if(m_pHallDlg->IsIconic()) //窗口是否最小化
	{
		m_pHallDlg->ShowWindow(SW_RESTORE);
	}
	m_pHallDlg->SetForegroundWindow();	
	m_pHallDlg->SendMessage(WM_MODULE_MSGBOX_SHOW,(WPARAM)st,0);

	return 0;

}

//SERVICE 获取指定用户unUIN的头像路径
int32 HallModule::InvokeGetHeadImagePath(param st)
{
	Hall::HeadImage *phead = (Hall::HeadImage *)st;
	if(!phead->m_isGuest)
	{
		phead->m_HeadImagePath = m_headDownload.GetLocalHeadImage(phead->m_unUIN,phead->m_bMan); //获取本地头像图片路径
	}
	else
	{
		phead->m_HeadImagePath = m_headDownload.GetGuestHeadImage();  //获取本地默认游客图片
	}
	return 0;
}

//SERVICE 获取指定用户unUIN的头像路径并下载最新头像
int32 HallModule::InvokeUpDateHeadImagePath(param st)
{
	Hall::HeadImage *phead = (Hall::HeadImage *)st;
	if(!phead->m_isGuest)
	{
		phead->m_HeadImagePath = m_headDownload.GetLocalHeadImage(phead->m_unUIN,phead->m_bMan); //获取本地头像图片路径，若未找到指定unUIN的对应头像图片则使用默认图片
		m_headDownload.EventSendNewHeadDown(phead->m_unUIN);  //发送最新头像下载请求
	}
	else
	{
		phead->m_HeadImagePath = m_headDownload.GetGuestHeadImage();  //获取本地默认游客图片
	}
	
	return 0;
}


void HallModule::InitTrayMgr()
{
	HICON icon;
	CString strModulePath	= common::utility::systemhelper::Get179AppPath().c_str();	
	CString updateTray;
	updateTray.Format(_T("%sresource\\listicon\\TrayLogining.ico"),strModulePath);  //灰色大厅托盘图标
	icon = (HICON)LoadImage( AfxGetResourceHandle(), updateTray, 
		IMAGE_ICON,	16, 16, LR_LOADFROMFILE );
	m_trayMgrPath.push_back(icon);

	updateTray.Format(_T("%sresource\\listicon\\TrayLogined.ico"),strModulePath);  //亮色大厅托盘图标
	icon = (HICON)LoadImage( AfxGetResourceHandle(), updateTray,  
		IMAGE_ICON,	16, 16, LR_LOADFROMFILE );
	m_trayMgrPath.push_back(icon);

	m_trayMgrCur = 0;
	m_trayMgr.Init(No_Care_HWND, eTNMode_Event, eTFSMode_Click);
	m_trayMgr.AddEventHandler(this);	
	
}

//登录过程中大厅托盘动态切换显示
void HallModule::LoginingTrayMgr()
{
	m_trayMgr.Add(m_trayMgrPath[m_trayMgrCur], _T("白兔KTV"));	

	m_trayMgrCur ++ ;
	if(m_trayMgrCur >= m_trayMgrPath.size())
	{
		m_trayMgrCur = 0;
	}

}

//启动程序后的自动登录
void HallModule::FirstAutoLogin()
{
	m_pLoginDlg->OnFirstLogin();
}

//加载大厅图标
//isLogined 为TRUE时 加载亮色大厅托盘图标, 为FALSE时 加载灰色大厅托盘图标
void HallModule::LoadTrayMgr(BOOL isLogined)
{
	if(m_trayMgrPath.size()>=2)
	{
		if(isLogined) //加载亮色大厅托盘图标
		{
			m_trayMgr.Add(m_trayMgrPath[1], _T("白兔KTV"));	
		}
		else  //加载灰色大厅托盘图标
		{
			m_trayMgr.Add(m_trayMgrPath[0], _T("白兔KTV"));	
		}
	}
}

void HallModule::CreateHallTrayMenu()
{
	core::CurInfo m_MyCurInfo = GetCurInfo();
	UINT setflags,abortflags,userflas;

	if(m_pHallDlg == NULL)
	{
		assert(0);
		return;
	}
	
	//大厅不能操作时
	if(!m_pHallDlg->IsWindowEnabled())
	{
		setflags = MF_GRAYED ;  //使菜单项无效并变灰，使其不能被选
		abortflags = MF_GRAYED;
		userflas = MF_GRAYED;	
	}
	else  //大厅可以操作时
	{
		if(m_MyCurInfo.enmAccountType == core::ACCOUNTTYPE_VISITOR)  //游客
		{
			setflags = MF_GRAYED ;  
		}
		else  //非游客
		{
			setflags = MF_ENABLED;  //使菜单项有效，并从变灰状态中恢复
		}

		abortflags = MF_ENABLED;
		userflas = MF_ENABLED;
	}

	common::ui::CMenuEx * pm = new common::ui::CMenuEx();
	pm->CreatePopupMenu();
	if(m_pHallDlg->IsWindowVisible())
		pm->AppendMenuEx(MF_ENABLED,ID_TRAYMENU_HIDEWND,L"隐藏窗口(H)");
	else
		pm->AppendMenuEx(MF_ENABLED,ID_TRAYMENU_HIDEWND,L"显示窗口(S)");
	pm->AppendMenuEx(setflags,ID_TRAYMENU_USERSETTING,L"个人设置");
	pm->AppendMenuEx(setflags,ID_TRAYMENU_VEDIOSETTING,L"视频设置");
	pm->AppendMenuEx(MF_SEPARATOR);
	pm->AppendMenuEx(MF_ENABLED,ID_TRAYMENU_HELP,L"帮助");
	pm->AppendMenuEx(abortflags,ID_TRAYMENU_ABOUT,L"关于此软件");
	pm->AppendMenuEx(MF_SEPARATOR);
	pm->AppendMenuEx(userflas,ID_TRAYMENU_CHANGEUSER,L"更改用户");
	pm->AppendMenuEx(MF_ENABLED,ID_TRAYMENU_EXIT,L"安全退出");

	common::ui::CCursorPos pt;
	SetForegroundWindow(m_pHallDlg->GetSafeHwnd());
	pm->TrackPopupMenu(0, pt.x, pt.y, m_pHallDlg);
	pm->DestroyMenu();

	delete pm;
	pm = NULL;

}


//在url之后加参数uin,tokentype,token
CString AppendUrlParamet(CString url)
{
	if(url.IsEmpty())
	{
		return _T("");
	}

	CString reUrl,tokenType;
	uint32 unUIN;
	core::CurInfo m_MyCurInfo = GetCurInfo();

	if(m_MyCurInfo.enmAccountType == core::ACCOUNTTYPE_QQ)
	{
		tokenType = _T("QQ");		
	}	
	else
	{
		tokenType = _T("179");		
	}
	unUIN = m_MyCurInfo.unUIN;

	if(url.Right(1) == _T("?"))
	{
		url = url.Left(url.GetLength()-1);
	}
	if(url.Find(_T("?")) != -1)
	{
		reUrl.Format(_T("%s&uin=%d&tokentype=%s&token=%s"),url,unUIN,tokenType,m_MyCurInfo.strToken.c_str());
	}
	else
	{
		reUrl.Format(_T("%s?uin=%d&tokentype=%s&token=%s"),url,unUIN,tokenType,m_MyCurInfo.strToken.c_str());
	}
	

	return reUrl;
}

CComVariant HallModule::LoadImageWrapper(CString imagePath,bool isUpload)
{
	common::ui::CImageImpl* pImage = NULL;

	std::map<CString, common::ui::CImageImpl*>::iterator it = m_mapPath2Image.find(imagePath);

	if(it != m_mapPath2Image.end())
	{
		pImage = it->second;	
		if(isUpload)
		{
			if(!pImage->Load(imagePath))
			{
				assert(0);
				return 0;
			}
		}

	}
	else
	{
		pImage = new common::ui::CImageImpl;
		if(!pImage->Load(imagePath))
		{
			assert(0);
			return 0;
		}
		m_mapPath2Image[imagePath] = pImage;
	}

	LONG lValue = (LONG)((IUnknown*)pImage);
	CComVariant varImage = lValue;

	return varImage;
}

//客户端发送验证码请求返回响应函数
void HallModule::ProcessWebGetLoginIdentifyResp( Event const &evt )
{
	m_pLoginDlg->OnVerfiyCodeResp(evt);
}

void HallModule::ExcuteWebPluginCommand(CString strCommand)
{
	strCommand.Replace(L"\"", L"");
	if(strCommand.GetAt(strCommand.GetLength()-1) == '/')
	{
		strCommand = strCommand.Left(strCommand.GetLength()-1);
	}
	
	if(strCommand.Find(COMMAND_TOKEN_PREFIX) != 0)
	{
		return;	
	}

	strCommand = strCommand.Right(strCommand.GetLength()-wcslen(COMMAND_TOKEN_PREFIX));
	
	if(strCommand.Find(COMMAND_TOKEN_ENTER_ROOM) != 0)
	{
		assert(0);
		return;
	}

	strCommand = strCommand.Right(strCommand.GetLength()-wcslen(COMMAND_TOKEN_ENTER_ROOM));

	if(common::utility::stringhelper::IsNumeric(std::wstring(strCommand.GetBuffer())))
	{
		enterRoomInfo roomid;
		roomid.m_RoomID = _wtoi(strCommand.GetBuffer());
		m_pHallDlg->m_EnterRoomRequest.EnterRoomEventRequest(roomid);
	}
}

void HallModule::ExcuteCommandCaihong()
{
	if(__argc < 6)
	{
		return;
	}

	CString strCaiHong = __targv[1]; //51彩红自动登录白兔KTV时参数为 caihong 	
	if(strCaiHong != COMMAND_TOKEN_51AUTOLOGIN)
	{
		return;  //不是从51彩红登录白兔KTV的
	}

	//命令行    "caihong  801000507 701a2e6fdc0e4322b554cd5d9067ce64 room 100000"
	CString strAccount = __targv[2]; //51彩红自动登录白兔KTV时参数为 对应的179号
	CString strPassword = __targv[3]; //51彩红自动登录白兔KTV时参数为 对应的MD5密码
	CString strRoom = __targv[4]; //51彩红自动登录白兔KTV时参数为 进房申请
	CString strRoomID = __targv[5]; //51彩红自动登录白兔KTV时参数为 进房申请ID

	CString str;
	str.Format(_T("命令:  %s %s %s %s %s"),strCaiHong,strAccount,strPassword,strRoom,strRoomID);
	OutputDebugString(str);

	if(strRoom != COMMAND_TOKEN_51_ENTER_ROOM)
	{
		return;
	}

	//是否会有欢迎对话框显示
	core::CurInfo m_MyCurInfo = GetCurInfo();
	if(!(m_MyCurInfo.enmAccountType != core::ACCOUNTTYPE_VISITOR && m_MyCurInfo.strName == _T("") && m_MyCurInfo.bFirstLogin))
	{																							
		if(common::utility::stringhelper::IsNumeric(wstring(strRoomID)))
		{
			enterRoomInfo roomid;
			roomid.m_RoomID = _wtoi(strRoomID.GetBuffer());
			m_pHallDlg->m_EnterRoomRequest.EnterRoomEventRequest(roomid);
		}
		OutputDebugString(_T("ExcuteCommandCaihong  命令执行 进房"));
	}
}

void HallModule::ProcessCommandLine()
{	
	std::wstring strCommand = L"";

	if(!m_bCommandLineProcessed)
	{
		strCommand = common::utility::systemhelper::GetCommandLineCommand();
		m_bCommandLineProcessed = true;
	}

	if(!strCommand.empty())
	{
		ExcuteWebPluginCommand(strCommand.c_str());
		ExcuteCommandCaihong();
	}
	else if(!m_cacheWebPluginCommand.IsEmpty())
	{
		ExcuteWebPluginCommand(m_cacheWebPluginCommand);
		m_cacheWebPluginCommand = "";
	}
}

void HallModule::OnWebPluginCommand(CString strCommand)
{
	//登陆框还在，未登录成功，缓存命令
	if(m_pLoginDlg->IsWindowVisible())
	{
		m_cacheWebPluginCommand = strCommand;
	}
	else
	{
		ExcuteWebPluginCommand(strCommand);
	}
}

bool HallModule::InitShareMemInfo(HWND hwndHall, HWND hwndLogin)
{
	HANDLE hMutex = ::OpenMutex(SYNCHRONIZE, FALSE, _T("Global\\51KTVMainApp"));
	if(hMutex == NULL)
	{
		DWORD d = ::GetLastError();
		assert(0);
		return false;
	}

	if(WAIT_OBJECT_0 == ::WaitForSingleObject(hMutex, INFINITE))
	{
		HANDLE hMapFile = ::OpenFileMapping(FILE_MAP_READ|FILE_MAP_WRITE, FALSE, _T("Local\\51KTVMainFileMap"));
		if(hMapFile == NULL)
		{
			ReleaseMutex(hMutex);
			CloseHandle(hMutex);
			return true;
		}

		ShareMemInfo* pMemInfo = (ShareMemInfo*)MapViewOfFile(hMapFile, FILE_MAP_WRITE|FILE_MAP_READ, 0, 0, sizeof(ShareMemInfo));
		if(pMemInfo == NULL)
		{
			CloseHandle(hMapFile);
			ReleaseMutex(hMutex);
			CloseHandle(hMutex);
			return false;
		}

		pMemInfo->hwndHall = hwndHall;
		pMemInfo->hwndLogin = hwndLogin;

		UnmapViewOfFile(pMemInfo);
		CloseHandle(hMapFile);
		ReleaseMutex(hMutex);
		CloseHandle(hMutex);
		return true;
	}

	CloseHandle(hMutex);

	return false;
}

void HallModule::ProcessHallCreateAndShow(Event const &evt)
{
	SwitchResourceToModule(_T("Hall"));
	m_pHallDlg = new CHallDlg(m_pModuleManager);
	if (NULL != m_pHallDlg)
	{
		if(!m_pHallDlg->Create(CHallDlg::IDD,NULL))
		{
			AfxMessageBox(L"对不起，UI加载失败，请尝试重新安装客户端。");
			TerminateProcess(OpenProcess(PROCESS_TERMINATE, FALSE, GetCurrentProcessId()), 0);
			return;
		}

		if (m_pLoginDlg==NULL)
		{
			m_pLoginDlg=new CLoginDlg(m_pModuleManager,m_pHallDlg);
			m_pLoginDlg->Create(CLoginDlg::IDD, m_pHallDlg);
		}
		m_pHallDlg->EnableWindow(FALSE);		
		m_pLoginDlg->ShowWindow(SW_HIDE);	
		m_pHallDlg->ShowWindow(SW_HIDE);

		//检查失败(多开?)
		if(!InitShareMemInfo(m_pHallDlg->GetSafeHwnd(), m_pLoginDlg->GetSafeHwnd()))
		{
			TerminateProcess(OpenProcess(PROCESS_TERMINATE, FALSE, GetCurrentProcessId()), 0);
			return;
		}

		m_AutoUpdate.ShowCheckUpdateDlg(m_pHallDlg); //显示检测更新对话框，并开始检测最新版本
		InitTrayMgr();
		LoadTrayMgr(FALSE);  //加载灰色大厅托盘图标		

	}
}


void HallModule::ProcessLoginSuccess()
{
	m_pLoginDlg->OnLoginSuccess();
	m_pHallDlg->OnLoginSuccess() ;

	core::CurInfo m_MyCurInfo = GetCurInfo();

	if(m_MyCurInfo.enmAccountType != core::ACCOUNTTYPE_VISITOR) //非游客
	{
		if(m_MyCurInfo.strName == _T("") && m_MyCurInfo.bFirstLogin) //冠用户资料不完善，则弹出首次登录欢迎对话框 
		{																							
			PostMessage(m_pHallDlg->GetSafeHwnd(),UM_QQFRISTLOGIN_INFO,0,0);  //后台开号首次登录欢迎对话框
		}
		if(m_MyCurInfo.enmAccountType == core::ACCOUNTTYPE_QQ && m_MyCurInfo.bFirstLogin) 
		{																							 
			PostMessage(m_pHallDlg->GetSafeHwnd(),UM_QQFRISTLOGIN_INFO,0,0);  //QQ首次登录欢迎对话框
		}
		m_headDownload.EventSendNewHeadDown(m_MyCurInfo.unUIN);//登录成功后请求下载最新头像	
	}	

	ProcessCommandLine();


}

void HallModule::ProcessLoginResp(Event const &evt)
{
	ASSERT(evt.m_pstExtraInfo != NULL);
	core::LoginRspEvent *pEvent=static_cast<core::LoginRspEvent*>(evt.m_pstExtraInfo);

	m_pLoginDlg->StopLoginingHallTray(); //停止登录过程中大厅托盘动态切换显示，
										 //注：将此函数放在这而不是放在m_pLoginDlg->OnLoginResp中调用是防止有登录失败提示时托盘定时在亮色大厅托盘图标  

	LoadTrayMgr(FALSE);  //加载灰色大厅托盘图标 (当登录失败且有提示框弹出时，此处有用，用于防止有登录失败提示时托盘定时在亮色大厅托盘图标)
	if(pEvent->enmLoginResult == core::LOGIN_ERROR_SUCCESS)
	{
		ProcessLoginSuccess();
		LoadTrayMgr(TRUE);  //加载亮色大厅托盘图标
	}
	else
	{
		//交给m_pLoginDlg处理
  	   m_pLoginDlg->OnLoginResp(evt); //请求登录返回响应函数
	   m_pHallDlg->OnLoginFailed() ;
	   LoadTrayMgr(FALSE);  //加载灰色大厅托盘图标	

	}
	
	
}

void HallModule::ProcessKicked(Event const &evt)
{
	ASSERT(evt.m_pstExtraInfo != NULL);
	core::BeKickedNotifyEvent *pInfo = (core::BeKickedNotifyEvent*)(evt.m_pstExtraInfo) ;	

	m_pLoginDlg->StopLoginingHallTray(); //停止登录过程中大厅托盘动态切换显示
	LoadTrayMgr(FALSE);  //加载灰色大厅托盘图标	
	
	CloseNonModalDlg((CDialog*&)m_pHeadSettingDlg);	
	m_SysMessagePopup.ClearSysMessage();
	m_pLoginDlg->CloseReconnectMsgBox();

	m_pHallDlg->CloseAllDialog();
	m_pLoginDlg->UIEnable(TRUE);
	ShowLoginDialog(true);

	switch (pInfo->result)
	{					

	case core::BeKickedNotifyEvent::KICK_RE_LOGIN ://账号重登陆
		{
			CString passUrl;
			core::CurInfo m_MyCurInfo = GetCurInfo();
			if(m_MyCurInfo.enmAccountType == core::ACCOUNTTYPE_QQ)
			{
				CSysSettingIni::GetWebInterfaceUrl(SYSSET_INT_KEYNAME_QQMODIFYPASSWORD,passUrl); //修改密码url
			}
			else
			{
				CSysSettingIni::GetWebInterfaceUrl(SYSSET_INI_KEYNAME_179MODIFYPASSWORD,passUrl); //修改密码url
			}
			
			C179MsgBox::Show(m_pLoginDlg->Get179MsgBoxParent(),NewMsgBox::MSGBOX_OK_TWO_STRING_WITH_LINK,_T("您的账号异地登录，将被迫下线。"),_T("如非本人操作请点击这里               "),_T(""),_T("修改密码"),passUrl);				

		}
		break;

	case core::BeKickedNotifyEvent::KICK_ACCOUNT_LOCK : //账号被锁定
		{
		
			if ( pInfo->lockDays >= 365*10 )
			{
				CString strKick;
				strKick.Format(_T("您因\"%s\"被永久封停账号"),pInfo->strReason.c_str() );
				C179MsgBox::Show(m_pLoginDlg->Get179MsgBoxParent(),NewMsgBox::MSGBOX_OK_TWO_STRING ,strKick,L"如有疑问，请联系官方客服" );	
			}			
			else
			{
				CString strKick;
				strKick.Format(_T("您因\"%s\"被封停账号%d天"),pInfo->strReason.c_str() , pInfo->lockDays );
				C179MsgBox::Show(m_pLoginDlg->Get179MsgBoxParent(),NewMsgBox::MSGBOX_OK_TWO_STRING ,strKick,L"如有疑问，请联系官方客服" );	
			}
		}
		break;

	case core::BeKickedNotifyEvent::KICK_IP_LOCK : //ip被锁定
		{			
			if ( pInfo->lockDays >= 365*10 )
			{
				CString strKick;
				strKick.Format(_T("您因\"%s\"被永久封停IP"),pInfo->strReason.c_str() );
				C179MsgBox::Show(m_pLoginDlg->Get179MsgBoxParent(),NewMsgBox::MSGBOX_OK_TWO_STRING ,strKick,L"如有疑问，请联系官方客服" );	
			}			
			else
			{
				CString strKick;
				strKick.Format(_T("您因\"%s\"被封停IP%d天"),pInfo->strReason.c_str() , pInfo->lockDays );
				C179MsgBox::Show(m_pLoginDlg->Get179MsgBoxParent(),NewMsgBox::MSGBOX_OK_TWO_STRING ,strKick,L"如有疑问，请联系官方客服" );	
			}
		}
		break;
	case core::BeKickedNotifyEvent::KICK_MAC_LOCK: //mac被锁定
		{		
			if ( pInfo->lockDays >= 365*10 )
			{
				CString strKick;
				strKick.Format(_T("您因\"%s\"被永久封停mac地址"),pInfo->strReason.c_str() );
				C179MsgBox::Show(m_pLoginDlg->Get179MsgBoxParent(),NewMsgBox::MSGBOX_OK_TWO_STRING ,strKick,L"如有疑问，请联系官方客服" );	
			}			
			else
			{
				CString strKick;
				strKick.Format(_T("您因\"%s\"被封停mac地址%d天"),pInfo->strReason.c_str() , pInfo->lockDays );
				C179MsgBox::Show(m_pLoginDlg->Get179MsgBoxParent(),NewMsgBox::MSGBOX_OK_TWO_STRING ,strKick,L"如有疑问，请联系官方客服" );	
			}
		}
		break;
	case core::BeKickedNotifyEvent::KICK_OTHER: //其它		
	default:
		{		
			
			//C179MsgBox::Show(NewMsgBox::MSGBOX_OK_ONE_STRING ,_T("您已被踢"));
		}
		break;
	}


}

void HallModule::ProcessWebDownFileResp(Event const &evt)
{
	m_pHallDlg->OnUpdateHallTreeResp(evt);  //大厅目录树文件下载完毕
	m_AutoUpdate.OnEventRespWebDown(evt); //最新版本配置文件、最新安装包\更新包下载完毕		
}

void HallModule::ProcessWebHeadImageResp(Event const &evt)
{
	m_headDownload.OnEventRespNewHeadDown(evt); //最新头像下载请求返回响应函数，若最新头像下载成功则自动发送消息 MESSAGE_VALUE_HALL_NEW_HEAD_IMAGE
}

void HallModule::ProcessWebGetHttpResp(Event const &evt)
{
	CSearchDlg *pSearchDlg = m_pHallDlg->GetSearchDlg();
	if(pSearchDlg != NULL)
	{
		pSearchDlg->OnEventRespSearch(evt); //房间搜索请求返回响应函数
	}
	m_pHallDlg->OnUpdateTickerResp(evt);

}


//头像上传
void HallModule::PrcoessHallHeadUpload(Event const &evt)
{
	//弹出头像上传对话框
	ASSERT(evt.m_pstExtraInfo != NULL);
	Hall::HeadSetting *pInfo = (Hall::HeadSetting*)(evt.m_pstExtraInfo) ;

	CloseNonModalDlg((CDialog*&)m_pHeadSettingDlg);
	if(m_pHeadSettingDlg == NULL)
	{
		SwitchResourceToModule(_T("Hall"));
		CWnd *pParentWnd = CWnd::FromHandle(pInfo->m_parentHwnd);
		m_pHeadSettingDlg = new CHeadSettingDlg(pParentWnd);		
		m_pHeadSettingDlg->Create(CHeadSettingDlg::IDD, pParentWnd);		
	}

	m_pHeadSettingDlg->ShowWindow(SW_SHOW);	

}
void HallModule::ProcessWebHeadUpLoadResp(Event const &evt)
{	
	if(m_pHeadSettingDlg != NULL)
	{
		m_pHeadSettingDlg->OnEventRespHeadUpload(evt);//用户上传自定义头像返回响应函数
			
		CloseNonModalDlg((CDialog*&)m_pHeadSettingDlg);
	}


}

//设置了(安全中心)密保
void HallModule::ProcessPassWordSafe(Event const &evt ) 
{
	m_pHallDlg->OnPassWordSafe() ;
}

void HallModule::PrcoessMyInfoUpdate(Event const &evt)
{	
	::SendMessage(m_pHallDlg->GetSafeHwnd(),UM_USERINFO_UPDATE,0,0);		
	m_pLoginDlg->SaveUserAccount();
}

void HallModule::MessageMyVipLevelUpdate(Message const& msg)
{
	::SendMessage(m_pHallDlg->GetSafeHwnd(),UM_USERINFO_UPDATE,0,0);	
}

void HallModule::MessageMyRichLevelUpdate(Message const& msg)
{
	::SendMessage(m_pHallDlg->GetSafeHwnd(),UM_USERINFO_UPDATE,0,0);
	m_SysMessagePopup.OnSystemMessageOther(SystemMessage::SystemMessageType::SYSTEMMESSAGETYPE_MAGNATE);  //富豪等级变更需系统消息弹框

}

void HallModule::ProcessDisconnected(Event const &evt)
{
	m_pHallDlg->CloseAllDialog();
	ShowLoginDialog(true);
	LoadTrayMgr(FALSE); //加载灰色大厅托盘图标
	CloseNonModalDlg((CDialog*&)m_pHeadSettingDlg);
	m_pLoginDlg->ReLogin();
	m_pHallDlg->EnableWindow(FALSE);
}

void HallModule::ProcessRegisterResp(Event const &evt)
{	
	ASSERT(evt.m_pstExtraInfo != NULL);
	RoomShowRegister *pInfo = (RoomShowRegister*)(evt.m_pstExtraInfo) ;

	CWnd *pParentWnd = CWnd::FromHandle(pInfo->m_parentHwnd);
	m_pHallDlg->CreateAndShowRegisterDlg(true,pParentWnd);  //创建并显示注册对话框
	m_pHallDlg->EnableWindow(TRUE);
	m_pHallDlg->SetRoomRegister(); //设置为房间注册对话框

}

void HallModule::ProcessRichUpdateMoneyResp(Event const &evt)
{
	m_pHallDlg->SetRickLevelTip();  //设置富豪等级TIP提示语
}

//关闭非模态对放框
void CloseNonModalDlg(CDialog*& dlg)
{
	if(dlg != NULL)
	{
		dlg->DestroyWindow();
		delete dlg;
		dlg = NULL;
	}
}
