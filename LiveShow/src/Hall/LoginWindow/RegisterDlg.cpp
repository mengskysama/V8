// RegisterDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "../Hall.h"
#include "../HallWindow/HallDlg.h"
#include "RegisterDlg.h"
#include "skin_i.h"
#include "SkinUtil.h"
#include "../SysSettingIni.h"
#include "assert.h"
#include "../CommonLibrary/include/utility/SystemHelper.h"
#include "../CommonLibrary/include/utility/func4Stat.h"

#define SKIN_ON_CLICK_CLOSE   1
#define SKIN_NOTIFY_DLG_TYPE  2



// CRegisterDlg 对话框

IMPLEMENT_DYNAMIC(CRegisterDlg, CDialog)

CRegisterDlg::CRegisterDlg(HWND sendMesHWND,eDlgType dlgType/* = Dlg_Type_For_Register*/,CWnd* pParent /*=NULL*/)
	: CDialog(CRegisterDlg::IDD, pParent)
{
    m_pSkinEngine = NULL;
	m_dlgType = dlgType;
	m_sendMesHWND = sendMesHWND;
	m_pParent = pParent;
	m_bRoomRegister = false;

    ::GetSkinEngine(&m_pSkinEngine);
}


CRegisterDlg::~CRegisterDlg()
{
}

void CRegisterDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CRegisterDlg, CDialog)
	ON_WM_CREATE()
	ON_SKIN_MESSAGE
	ON_WM_SYSCOMMAND()
	ON_MESSAGE(WM_MISSION_HYPERLINK_ONCLICK,&CRegisterDlg::OnClick)
	ON_MESSAGE(WM_MISSION_HYPERLINK_OPENURL,&CRegisterDlg::OnOpenURL)
END_MESSAGE_MAP()


// CRegisterDlg 消息处理程序

int CRegisterDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  在此添加您专用的创建代码
	ASSERT(NULL != m_pSkinEngine);
	if (NULL != m_pSkinEngine)
	{
		HRESULT hr = m_pSkinEngine->LoadSkin(GetSafeHwnd(),L"RegisterDlg");
		ASSERT(hr== S_OK && __LINE__);
	}

	if(m_pParent!= NULL)
	{
		CenterWindow(m_pParent);		
	}	

	return 0;
}

LRESULT CRegisterDlg::OnSkinMessage(WPARAM wParam,LPARAM lParam)
{
	switch(wParam)
	{
	case SKIN_ON_CLICK_CLOSE:
		{
			PostMessage(WM_CLOSE,NULL,NULL);
			g_HallModule->CloseHallRegisterDlg();
		}
		break;
	default:break;
	}

	return TRUE;
}

LRESULT CRegisterDlg::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	switch(message)
	{
		case WM_MISSION_COMPLETED:
			{
				RegisterElementSetFocus();
			}
			break;

		case WM_MISSION_HYPERLINK_LOGIN:
			{
				PostMessage(WM_CLOSE,0,0);				
				g_HallModule->ShowLoginDialog(true);
			}
			break;
		case WM_MISSION_NAVIGATECOMPLETE2:
			{
				Register179Success();
			}
			break;
		default:
			break;		
	}

	return CDialog::WindowProc(message, wParam, lParam);

}
//载取点击m_htmlRegister注册页面中超链接时的响应函数
LRESULT CRegisterDlg::OnClick(WPARAM wParam,LPARAM lParam)
{
	IHTMLEventObj *pEvtObj = (IHTMLEventObj *)lParam;

	if(pEvtObj == NULL)
	{
		assert(false);
		return FALSE;
	}

	CComPtr<IHTMLElement> pElement;
	pEvtObj->get_srcElement(&pElement);

	if(pElement == NULL)
	{
		assert(false);
		return FALSE;
	}	

	CComBSTR tag;
	pElement->get_tagName(&tag);
	if(tag == _T("A") || tag == _T("a"))
	{		
		if(m_dlgType == Dlg_Type_For_Register)  //<a Name="login" account="dxgss415@126.com" password="12345678" href="#" class="btn_101_29">马上去大厅</a>
		{
			CString strName = m_htmlRegister.GetValue( pElement , _T("name") ) ;
			if(strName == _T("login"))  //用户在注册成功之后点击了注册页面中的"开启您的缘分之旅"图片按扭
			{
				CString strAccount = m_htmlRegister.GetValue( pElement , _T("account") ) ;  //获取图片按扭中的账号
				CString strPassWord = m_htmlRegister.GetValue( pElement , _T("password") ) ; //获取图片按扭中的密码(未加密的原始密码)				
				if ( !strAccount.IsEmpty() && !strPassWord.IsEmpty() )
				{					
					Register179Account r179Accout;
					r179Accout.m_Account = strAccount;
					r179Accout.m_Password = strPassWord;					
					
					g_HallModule->ShowLoginDialog(true);
					::SendMessage(m_sendMesHWND,UM_179REGISTER_AUTOLOGIN,(WPARAM)&r179Accout,0);
					PostMessage(WM_CLOSE,0,0);

					g_HallModule->CloseHallRegisterDlg();
				}
			}
			else if(strName == _T("QQLogin")) //用户在注册框中点击了注册页面中的"QQ登录"图片按扭
			{
				g_HallModule->ShowLoginDialog(true);
				::SendMessage(m_sendMesHWND,UM_179REGISTER_QQLOGIN,0,0);				
				PostMessage(WM_CLOSE,0,0);

				g_HallModule->CloseHallRegisterDlg();
			}
		}
		if(m_dlgType == Dlg_Type_For_CompleteInfo) //QQ用户首次登录欢迎对话框中点击了"开启您的缘分之旅"图片按扭
		{
			CString strName = m_htmlRegister.GetValue( pElement , _T("name") ) ;
			if(strName == _T("ok"))
			{			
				PostMessage(WM_CLOSE,0,0);					
				g_HallModule->CloseHallRegisterDlg();				
				
			}
		}		
	}

	return TRUE;
}

LRESULT CRegisterDlg::OnOpenURL(WPARAM wParam,LPARAM lParam)
{
	LPCTSTR lpszURL = (LPCTSTR)wParam;
	std::wstring strURL = lpszURL;
	g_HallModule->Invoke(Hall::SERVICE_VALUE_HALL_OPEN_TOKEN_URL, (param)&strURL);
	return S_OK;
}

void CRegisterDlg::InitRegisterBrowser()
{
	CComVariant varImage1,varImage2;

	if(m_dlgType == Dlg_Type_For_Register)
	{
		/*varImage1 = L"true";
		varImage2 = L"false";
		m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(),L"imgRegisterEffect1",L"visible",varImage1);	
		m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(),L"imgRegisterEffect2",L"visible",varImage2);*/	

		CString s179RegisterUrl,channelNum,s179Register;
		CSysSettingIni::GetWebInterfaceUrl(SYSSET_INT_KEYNAME_179REGISTER,s179RegisterUrl);  //获取web接口中的179注册url地址
		GetRegisterChannelNum(channelNum);  //获取注册渠道号
		s179Register.Format(_T("%s?from=%s"),s179RegisterUrl,channelNum);

		std::wstring mac, verify;
		get_macverify(_wtoi(channelNum.GetBuffer()), mac, verify);
		s179Register.AppendFormat(_T("&mac=%s&verify=%s"), mac.c_str(), verify.c_str());

		m_htmlRegister.SetUrl(wstring(s179Register), true);
		m_htmlRegister.UpdateURL();	    
	}

	if(m_dlgType == Dlg_Type_For_CompleteInfo)
	{
		/*varImage1 = L"false";
		varImage2 = L"true";
		m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(),L"imgRegisterEffect1",L"visible",varImage1);	
		m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(),L"imgRegisterEffect2",L"visible",varImage2);*/
		
		SetWindowSize(491,398);
		CString sQQFristLoginUrl,channelNum,sQQFristLogin;
		CSysSettingIni::GetWebInterfaceUrl(SYSSET_INT_KEYNAME_QQFRISTLOGININFOOK,sQQFristLoginUrl);  //获取QQ用户首次登录白兔KTV时的欢迎页面
		GetRegisterChannelNum(channelNum);  //获取注册渠道号
		sQQFristLoginUrl = AppendUrlParamet(sQQFristLoginUrl);
		sQQFristLogin.Format(_T("%s&from=%s"),sQQFristLoginUrl,channelNum);
		m_htmlRegister.SetUrl(wstring(sQQFristLogin), true);		
		m_htmlRegister.UpdateURL();
		OutputDebugString(sQQFristLogin);
	}
	

}

//179注册成功页面时，调整注册框大小
void CRegisterDlg::Register179Success()
{
	CString curUrl = m_htmlRegister.GetLocationURL();
	CString sOKUrl;
	
	CSysSettingIni::GetWebInterfaceUrl(SYSSET_INI_KEYNAME_179REGISTEROK,sOKUrl);  //获取web接口中的179注册成功url地址

	if(curUrl == sOKUrl && m_dlgType == Dlg_Type_For_Register)
	{

		/*CComVariant varImage1,varImage2;
		varImage1 = L"false";
		varImage2 = L"true";
		m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(),L"imgRegisterEffect1",L"visible",varImage1);	
		m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(),L"imgRegisterEffect2",L"visible",varImage2);*/

		SetWindowSize(491,398);
		/*CComVariant var = FALSE;
		m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(), L"imgRegisterEffect1", L"visible", var);
		var = TRUE;
		m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(), L"imgRegisterEffect2", L"visible", var);*/
	}	

	if(m_pParent!= NULL && curUrl == sOKUrl)
	{
		CenterWindow(m_pParent);		
	}	
}

//设置内嵌页面的焦点
void CRegisterDlg::RegisterElementSetFocus()
{
	
	CString sElementID;
	if(m_dlgType == Dlg_Type_For_Register)  //注册页面
	{		
		sElementID = _T("email");
	}
	if(m_dlgType == Dlg_Type_For_CompleteInfo) //QQ用户首次登录白兔KTV时的完善资料
	{		
		sElementID = _T("rolename");
	}	
	m_htmlRegister.ElementSetFocus(sElementID);		
	
}
BOOL CRegisterDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	//标题字体首选微软雅黑
	std::wstring strFont;
	common::utility::systemhelper::GetDefaultFontName(strFont);
	CComVariant var = (CComVariant) strFont.c_str();
	m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(),L"textTitle",L"fontFamily",var);

	//创建浏览器
	m_htmlRegister.CreateFromStatic(IDC_HTML_REGISTER,this);
	var = (LONG)m_htmlRegister.GetSafeHwnd();
	m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(),L"itemHtmlRegister",L"hwnd",var);
	InitRegisterBrowser();

	if (m_dlgType == Dlg_Type_For_Register)//注册框
	{
        NOTIFY_SKIN_HELPER(SKIN_NOTIFY_DLG_TYPE,(LONG)0);//传0进去

	}else if (m_dlgType == Dlg_Type_For_CompleteInfo)//完善信息资料框 
	{
        NOTIFY_SKIN_HELPER(SKIN_NOTIFY_DLG_TYPE,(LONG)1);//传1进去
	}	
	
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CRegisterDlg::SetWindowSize(int wndWidth, int wndHeight)
{
	CComVariant var = wndWidth;
    m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(),L"Window",L"width",var);
	var = wndHeight;
	m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(),L"Window",L"height",var);

}

//获取对话框类型 
int CRegisterDlg::GetDlgType()
{
	return m_dlgType;
}

BOOL CRegisterDlg::PreTranslateMessage(MSG* pMsg)
{

	if(pMsg && pMsg->message == WM_KEYDOWN)
	{
		if(pMsg->wParam == VK_ESCAPE || pMsg->wParam == VK_RETURN )  //频弊 ESC
		{
			return TRUE;
		}
		
	}

	return CDialog::PreTranslateMessage(pMsg);
}


void CRegisterDlg::OnSysCommand(UINT nID,LPARAM lParam)
{
	if(nID == SC_CLOSE && lParam == 0) //频弊 ALT + F4
	{
		return;
	}
	
	CWnd::OnSysCommand(nID,lParam);

}

//设置是否为房间注册框
void CRegisterDlg::SetRoomRegister(bool isRoom)
{
	m_bRoomRegister = isRoom;
}
	
//房间注册框是否正在显示
bool CRegisterDlg::RoomRegisterDlgIsShow()
{
	if(IsWindowVisible() && m_dlgType == Dlg_Type_For_Register && m_bRoomRegister)
	{
		return true;
	}
	else
	{
		return false;
	}
}