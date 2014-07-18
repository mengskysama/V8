// OtherUserInfoDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "..\Room.h"
#include "OtherUserInfoDlg.h"
#include "skin_i.h"
#include "SkinUtil.h"
#include "..\..\CommonLibrary\include\utility\SystemHelper.h"
#include "..\..\CommonLibrary\include\utility\StringHelper.h"
#include "HallDefine.h"
#include "SysSettingIniKey.h"

#define OTHER_FANS_HTML			L"resource\\html\\fans\\taFans.html"

#define SKIN_ON_CLICK_CLOSE                    1
#define SKIN_ON_CLICK_USERINFO                 2
#define SKIN_ON_CLICK_ITS_ROOM                 3
#define SKIN_ON_CLICK_ITS_FANS				   4
#define SKIN_ON_CLICK_BANKACCOUNT			   5
#define SKIN_NOTIFY_BANKACCOUNT_DLG_SHOW	   6
#define SKIN_NOTIFY_ITSFANS_DLG_SHOW		   7


// COtherUserInfoDlg 对话框

IMPLEMENT_DYNAMIC(COtherUserInfoDlg, CDialog)

COtherUserInfoDlg::COtherUserInfoDlg(CWnd* pParent /*=NULL*/)
	: CDialog(COtherUserInfoDlg::IDD, pParent)
{
	m_pSkinEngine = NULL;
	::GetSkinEngine(&m_pSkinEngine);	
}

COtherUserInfoDlg::~COtherUserInfoDlg()
{

}

void COtherUserInfoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(COtherUserInfoDlg, CDialog)
	ON_WM_CREATE()
	ON_SKIN_MESSAGE
	ON_WM_SHOWWINDOW()
	ON_MESSAGE(WM_MISSION_HYPERLINK_ENTERROOM,	&COtherUserInfoDlg::OnEnterRoom)	
END_MESSAGE_MAP()


// COtherUserInfoDlg 消息处理程序

int COtherUserInfoDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  在此添加您专用的创建代码
	ASSERT(NULL != m_pSkinEngine);
	if (NULL != m_pSkinEngine)
	{
		HRESULT hr = m_pSkinEngine->LoadSkin(GetSafeHwnd(),L"OtherUserInfoDlg");
		ASSERT(hr== S_OK && __LINE__);
	}

	//标题字体首选微软雅黑
	std::wstring strFont;
	common::utility::systemhelper::GetDefaultFontName(strFont);
	CComVariant var = (CComVariant) strFont.c_str();
	m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(),L"textTitle",L"fontFamily",var);

	return 0;
}

BOOL COtherUserInfoDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	CComVariant var;
	m_htmlOtherUserInfo.CreateFromStatic(IDC_HTML_OTHER_USER_INFO,this);
	var = (LONG)m_htmlOtherUserInfo.GetSafeHwnd();
	m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(),L"itemHtmlOtherUserInfo",L"hwnd",var);

//	ModifyStyleEx(0,WS_EX_TOOLWINDOW);

	m_fansValue.Init(&m_htmlOtherUserInfo);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


LRESULT COtherUserInfoDlg::OnSkinMessage(WPARAM wParam,LPARAM lParam)
{
	switch(wParam)
	{
	case SKIN_ON_CLICK_CLOSE:
		{
			OnClickClose();
		}
		break;
	case SKIN_ON_CLICK_USERINFO:
		{
			OnClickUserInfo();
		}
		break;
	case SKIN_ON_CLICK_ITS_ROOM:
		{
			OnClickItsRoom();
		}
		break;
	case SKIN_ON_CLICK_ITS_FANS:
		{
			OnClickItsFans();
		}
		break;
	case SKIN_ON_CLICK_BANKACCOUNT:
		{
			OnClickBankAccount();
		}
		break;


	default:break;
	}

	return TRUE;
}

LRESULT COtherUserInfoDlg::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	switch(message)
	{
	
	case WM_MISSION_COMPLETED:  	//网页真正加载完毕
		{
			if(IsCurFansDlg())  //当前是否为TA的粉丝
			{
				m_fansValue.OnGetFans(m_userInfo.m_UserUin); //获取 TA的粉丝 排行数据
			}
			
		}
		break;		
	default:
		break;
	}

	return CDialog::WindowProc(message, wParam, lParam);
}

void COtherUserInfoDlg::InitUserInfoBrowser()
{
	Hall::WebInterfaceUrl weburl;
	weburl.m_WebWithPara = false;
	weburl.m_WebInterfaceKeyName = SYSSET_INI_KEYNAME_USERINFO;
	g_RoomModule->m_pModuleManager->CallService(Hall::SERVICE_VALUE_HALL_GET_WEBINTERFACEURL, (param)&weburl);

	uint32 gender = m_userInfo.m_Gender?1:0 ;
	std::wstring userInfoUrl;		
	std::wstring roleEncode = common::utility::stringhelper::UrlUTF8EncodeW(m_userInfo.m_UserRoleName); //UTF-8 的url编写

	userInfoUrl = common::utility::stringhelper::Format(_T("%s?userid=%d&account=%d&gender=%d&rolename=%s&viplevel=%d&MagnateLevel=%d"),
		weburl.m_WebInterfaceUrl.c_str(),m_userInfo.m_UserUin,m_userInfo.m_UserAccount,gender,roleEncode.c_str(),m_userInfo.m_VipLevel,m_userInfo.m_RichLevel);

	m_htmlOtherUserInfo.SetUrl(userInfoUrl);
	m_htmlOtherUserInfo.UpdateURL();

}

//用户管理的房间页面
void COtherUserInfoDlg::InitUserItsRoom()	
{
	Hall::WebInterfaceUrl weburl;
	weburl.m_WebWithPara = false;
	weburl.m_WebInterfaceKeyName = SYSSET_INI_KEYNAME_USERITROOM;
	g_RoomModule->m_pModuleManager->CallService(Hall::SERVICE_VALUE_HALL_GET_WEBINTERFACEURL, (param)&weburl);

	std::wstring userRoomUrl;		
	userRoomUrl = common::utility::stringhelper::Format(_T("%s?userid=%d"),weburl.m_WebInterfaceUrl.c_str(),m_userInfo.m_UserUin);

	m_htmlOtherUserInfo.SetUrl(userRoomUrl);
	m_htmlOtherUserInfo.UpdateURL();

}


//TA的粉丝
void COtherUserInfoDlg::InitUserItsFans()	
{
	std::wstring strUrl = common::utility::systemhelper::Get179AppPath() + OTHER_FANS_HTML;	
	m_htmlOtherUserInfo.SetUrl(strUrl);
	m_htmlOtherUserInfo.UpdateURL();

}

//用户银行账号页面
void COtherUserInfoDlg::InitUserBankAccount()
{
	Hall::WebInterfaceUrl weburl;
	weburl.m_WebWithPara = false;
	weburl.m_WebInterfaceKeyName = SYSSET_INI_KEYNAME_USERBANKACCOUNT;
	g_RoomModule->m_pModuleManager->CallService(Hall::SERVICE_VALUE_HALL_GET_WEBINTERFACEURL, (param)&weburl);

	std::wstring userBankUrl;		
	userBankUrl = common::utility::stringhelper::Format(_T("%s?userid=%d"),weburl.m_WebInterfaceUrl.c_str(),m_userInfo.m_UserUin);

	m_htmlOtherUserInfo.SetUrl(userBankUrl);
	m_htmlOtherUserInfo.UpdateURL();
}

void COtherUserInfoDlg::OnClickClose()
{
	ShowWindow( SW_HIDE ) ;
}

void COtherUserInfoDlg::OpenOtherDlg(OtherUserInfoDlgType dlgType)
{
	CComVariant varTRUE = TRUE;
	CComVariant varFALSE = FALSE;		

	m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(),L"imgInfoBk",L"visible",varFALSE);  //其他人的 "基本资料"默认为未被选中
	m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(),L"imgItsRoomBk",L"visible",varFALSE);  //其他人的 "IA的房间"默认为未被选中
	m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(),L"imgItsFansBk",L"visible",varFALSE);  //其他人的 "IA的粉丝"默认为未被选中
	m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(),L"imgBankAccountBk",L"visible",varFALSE);  //其他人的 "银行账号"默认为未被选中

	switch(dlgType)
	{
	case OtherUserInfoDlgType::Show_Other_UserInfo:
		{	
			InitUserInfoBrowser();
			m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(),L"imgInfoBk",L"visible",varTRUE);  //其他人的 "基本资料"被选中
		}
		break;
	case OtherUserInfoDlgType::Show_Other_ItsRoom:
		{		   
			InitUserItsRoom();
			m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(),L"imgItsRoomBk",L"visible",varTRUE);  //其他人的 "IA的房间"被选中
		}
		break;
	case OtherUserInfoDlgType::Show_Other_ItsFans:
		{		   
			InitUserItsFans();
			m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(),L"imgItsFansBk",L"visible",varTRUE);  //其他人的 "IA的粉丝"被选中
		}
		break;
	case OtherUserInfoDlgType::Show_Other_ItsBankAccount:
		{		   
			InitUserBankAccount();
			m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(),L"imgBankAccountBk",L"visible",varTRUE);  //其他人的 "银行账号"被选中
		}
		break;
	default:break;
	}

}

void COtherUserInfoDlg::OnClickUserInfo()
{
	OpenOtherDlg(Show_Other_UserInfo);
}

void COtherUserInfoDlg::OnClickItsRoom()
{
	OpenOtherDlg(Show_Other_ItsRoom);
}
void COtherUserInfoDlg::OnClickItsFans()
{
	OpenOtherDlg(Show_Other_ItsFans);
}
void COtherUserInfoDlg::OnClickBankAccount()
{
	OpenOtherDlg(Show_Other_ItsBankAccount);
}



void COtherUserInfoDlg::SetUserInfo(OtherUserInfo &userInfo)
{
	m_userInfo = userInfo;
	OpenOtherDlg(Show_Other_UserInfo);
	HeadImageUpdate(m_userInfo.m_UserUin);
	CComVariant varTRUE = TRUE;	
	CComVariant varFALSE = FALSE;	
	if(m_userInfo.m_IsActor) 
	{
		//该用户为艺人，显示TA的粉丝	
		 NOTIFY_SKIN_HELPER(SKIN_NOTIFY_ITSFANS_DLG_SHOW,(LONG)1);//传1进去  显示 TA的粉丝

	}
	else
	{
		//该用户不为艺人，不显示TA的粉丝
		NOTIFY_SKIN_HELPER(SKIN_NOTIFY_ITSFANS_DLG_SHOW,(LONG)0);//传0进去  不显示 TA的粉丝

	}

	if(m_userInfo.m_IsAgent)
	{
		//该用户为销售，显示银行账号
		NOTIFY_SKIN_HELPER(SKIN_NOTIFY_BANKACCOUNT_DLG_SHOW,(LONG)1);//传1进去  显示 银行账号
	}
	else
	{
		//该用户不为销售，不显示银行账号
		NOTIFY_SKIN_HELPER(SKIN_NOTIFY_BANKACCOUNT_DLG_SHOW,(LONG)0);//传0进去  不显示 银行账号
	}

}

void COtherUserInfoDlg::OnShowWindow(BOOL bShow,UINT nStatus)
{
	CDialog::OnShowWindow(bShow, nStatus);

	if(bShow)
	{		
		HeadImageUpdate(m_userInfo.m_UserUin);
		CenterWindow(GetParent());		
	}

}

void COtherUserInfoDlg::OnGetHeadImage(uint32 unUIN)
{	
	Hall::HeadImage headImage ;
	headImage.m_bMan = m_userInfo.m_Gender ;
	headImage.m_unUIN = m_userInfo.m_UserUin ;	
	headImage.m_isGuest = false;
	
	g_RoomModule->m_pModuleManager->CallService(Hall::SERVICE_VALUE_HALL_GET_HEAD_IMAGE_PATH, (param)&headImage);
	CComVariant var = CString(headImage.m_HeadImagePath.c_str()) ;
	m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(), L"UserHead", L"absoluteImage", var );

}

void COtherUserInfoDlg::HeadImageUpdate(uint32 unUIN)
{
	Hall::HeadImage headImage ;
	headImage.m_bMan = m_userInfo.m_Gender ;
	headImage.m_unUIN = m_userInfo.m_UserUin ;	
	headImage.m_isGuest = false;

	g_RoomModule->m_pModuleManager->CallService(Hall::SERVICE_VALUE_HALL_UPDATE_HEAD_IMAGE_PATH, (param)&headImage);
	CComVariant var = CString(headImage.m_HeadImagePath.c_str()) ;
	m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(), L"UserHead", L"absoluteImage", var );

}

//内嵌页面中接口发送的进房请求
LRESULT COtherUserInfoDlg::OnEnterRoom(WPARAM wParam,LPARAM lParam)
{
	g_RoomModule->m_pModuleManager->PushEvent(MakeEvent<MODULE_ID_ROOM>()(Hall::EVENT_VALUE_HALL_ENTER_ROOM_REQUEST, MODULE_ID_HALL, wParam));  //请申进房
	return S_OK;
}

//当前显示是否为TA的粉丝
bool COtherUserInfoDlg::IsCurFansDlg()
{
	CComVariant var;
	m_pSkinEngine->GetSubSkinProp(GetSafeHwnd(), L"imgItsFansBk", L"visible", &var);
	if (var.boolVal)
	{
		return true;
	}
	else
	{
		return false;
	}
}

//获取粉丝数据返回响应函数	
void COtherUserInfoDlg::OnGetFansRsp(Event const&evt)
{
	if(IsCurFansDlg())
	{
		m_fansValue.OnGetFansRsp(evt,_T("TA当前没有粉丝哦，快快送礼成为TA的粉丝吧"));
	}	
}
