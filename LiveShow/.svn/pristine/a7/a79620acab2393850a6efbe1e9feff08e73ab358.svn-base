// QQLoginDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "../Hall.h"
#include "QQLoginDlg.h"
#include "Skin_i.h"
#include "SkinUtil.h"
#include "../SysSettingIni.h"
#include "webdefine.h"
#include "../CommonLibrary/include/utility/json/json.h"
#include "../CommonLibrary/include/utility/StringHelper.h"
#include "../CommonLibrary/include/utility/SystemHelper.h"
#include "LoginDlg.h"

#define  TIMER_GRADUALLY_SHOW    1  //淡出
#define  TIMER_GRADUALLY_HIDE    2  //淡入

#define  GRADUALLY_SPEED         70

#define WM_QQLOGIN_GET_PRELOADQQ WM_USER + 230

#define QQLOGIN_RESULTS_179ACCOUNT "vdcID"
#define QQLOGIN_RESULTS_179PASSWORD "password"
#define QQLOGIN_RESULTS_179ERROR "error"

#define QQLOGIN_TIMER_IDEVENT 3     //定时器 QQ登录页面大小
#define QQLOGIN_TIMER_INTERVALTIME  200  //定时器间隔 QQ登录页面获取页面大小间隔时间（单位：豪秒）

#define QQLOGIN_OPENERROR			L"resource\\html\\qq\\qq_err_mes.html"
#define QQLOGIN_LOAD 			L"resource\\html\\qq\\qq_loading.html"

// CQQLoginDlg 对话框

IMPLEMENT_DYNAMIC(CQQLoginDlg, CDialog)

CQQLoginDlg::CQQLoginDlg(CWnd* pParent /*=NULL*/)
: CDialog(CQQLoginDlg::IDD, pParent)
{
	m_pSkinEngine = NULL;
	::GetSkinEngine(&m_pSkinEngine);

	m_alpha = 255;//(区间0-255， 0为全透明)
	m_hInst = NULL;
	SetLayeredWindowAttributes = NULL;
	m_QQLoginURL = NULL;
	m_bLayered = FALSE;

	m_nSeqNo= 0;
	m_IE6 = false;
	m_HtmlHeight = 0;
	m_HtmlWidth = 0;
	m_defineHeight = QQ_LOGIN_DLG_HEIGHT;
	m_defineWidth = QQ_LOGIN_DLG_WIDTH;

}

CQQLoginDlg::~CQQLoginDlg()
{
	m_htmlQQLogin.DestroyWindow();
	if (m_hInst != NULL)
	{
		FreeLibrary(m_hInst);
		m_hInst = NULL;
		SetLayeredWindowAttributes = NULL;
	}

	if(m_QQLoginURL != NULL)
	{
		delete m_QQLoginURL;
		m_QQLoginURL = NULL;
	}	
}

void CQQLoginDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CQQLoginDlg, CDialog)
	ON_WM_CREATE()
	ON_SKIN_MESSAGE
	ON_WM_TIMER()
	ON_WM_SHOWWINDOW()	
	ON_WM_SYSCOMMAND()
END_MESSAGE_MAP()


// CQQLoginDlg 消息处理程序

int CQQLoginDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  在此添加您专用的创建代码
	ASSERT(m_pSkinEngine != NULL);
	if (NULL != m_pSkinEngine)
	{
		HRESULT hr = m_pSkinEngine->LoadSkin(GetSafeHwnd(),L"QQLoginDlg");
		ASSERT(hr == S_OK && __LINE__);
	}

	return 0;
}

LRESULT CQQLoginDlg::OnSkinMessage(WPARAM wParam,LPARAM lParam)
{
	switch(wParam)
	{

	default:break;
	}

	return TRUE;
}

void CQQLoginDlg::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialog::OnShowWindow(bShow, nStatus);

	if(bShow)
	{		
		SetTimer(QQLOGIN_TIMER_IDEVENT,QQLOGIN_TIMER_INTERVALTIME,NULL);
	}
	else
	{
		KillTimer(QQLOGIN_TIMER_IDEVENT);
	}
}

LRESULT CQQLoginDlg::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	switch(message)
	{

	case WM_QQLOGIN_GET_PRELOADQQ:
		{
			OnQQloginGetProLoad(); //预加载QQ登录页面响应函数
		}
		break;
	case WM_MISSION_HTML_ERROR:
		{
			QQloginLoadError(); //QQ登录页面加载失败
		}
		break;
	case WM_MISSION_HYPERLINK_ONCLICK:
		{
			OnQQloginClick(lParam); //QQ登录有点击操作
		}
		break;		
	case WM_MISSION_NAVIGATECOMPLETE2:
		{
			OnQQLoginUserCannel();
			OnQQloginGet179AccountPwd(); //获取OpenID对应的179号和MD5加密密码
		}
		break;

	default:
		break;
	}

	return CDialog::WindowProc(message, wParam, lParam);
}


//WM_QQLOGIN_GET_PRELOADQQ 消息的响应函数，预加载QQ登录页面
void CQQLoginDlg::OnQQloginGetProLoad()
{
	CStringA qqBuffer;
	qqBuffer.Format("%s",m_QQBuffer); 
	if(qqBuffer.IsEmpty())
	{
		QQloginLoadError();
	}
	else
	{
		QQloginLoad(); 

	}

}

//QQ登录授权页面中用户取消授权后返回的页面
void CQQLoginDlg::OnQQLoginUserCannel() 
{
	CString qqCannel = _T("http://www.179.com");
	CString strURL = m_htmlQQLogin.GetLocationURL();
	if(strURL.Right(1) == _T("/"))
	{
		strURL = strURL.Left(strURL.GetLength()-1);
	}
	if(strURL == qqCannel)
	{
		QQloginUse179login(); //转至179登录界面
	}

}

void CQQLoginDlg::OnQQloginGet179AccountPwd()
{
	CString strURL = m_htmlQQLogin.GetLocationURL();
	CString codePre;
	CSysSettingIni::GetWebInterfaceUrl(SYSSET_INI_KEYNAME_QQLOGIN_179ACCOUNT,codePre);  	

	int index = strURL.Find(codePre);	
	if(index >= 0 )
	{
		CString QQ179Account,QQ179Pwd,QQError;
		m_htmlQQLogin.GetElementValue(_T("QQ_vdcID"),QQ179Account);
		m_htmlQQLogin.GetElementValue(_T("QQ_password"),QQ179Pwd);
		m_htmlQQLogin.GetElementValue(_T("QQ_error"),QQError);
		if(QQError == _T("0"))
		{
			if(!QQ179Account.IsEmpty() && !QQ179Pwd.IsEmpty())  //获取QQ对应的179号和MD5加密密码成功
			{
				QQLogin179Account qqAcount;
				qqAcount.m_Account = QQ179Account;
				qqAcount.m_Password = QQ179Pwd;
				::SendMessage(GetParent()->GetSafeHwnd(),UM_QQLOGIN_AUTOLOGIN,(WPARAM)&qqAcount,0);  //发送QQ对应179账号自动登录消息

			}
			else //获取QQ对应的179号或MD5加密密码失败
			{
				QQloginError();  //QQ登录失败
			}
		}
	}

}

//WM_MISSION_HYPERLINK_ONCLICK 消息的响应函数， QQ登录有点击操作
void CQQLoginDlg::OnQQloginClick(LPARAM lParam)
{
	IHTMLEventObj *pEvtObj = (IHTMLEventObj *)lParam;

	if(pEvtObj == NULL)
	{
		assert(false);
		return ;
	}

	CComPtr<IHTMLElement> pElement;
	pEvtObj->get_srcElement(&pElement);

	if(pElement == NULL)
	{
		assert(false);
		return ;
	}	

	CComBSTR tag;
	pElement->get_tagName(&tag);
	if(tag == _T("A") || tag == _T("a"))
	{		

		CString strName = m_htmlQQLogin.GetValue( pElement , _T("name") ) ;
		if(strName =="againload")  //再试一次
		{
			QQloginAgainLoad();
			return ;
		}		
		if(strName == _T("use179login"))  //使用179账号登录
		{	
			QQloginUse179login();
			return ;
		}	
		if(strName == _T("register179web"))
		{
			QQloginRegister179Web(); //注册179账号， 使用浏览器打开官方179账号注册页面
			return ;
		}		

	}

	return ;
}

BOOL CQQLoginDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	CComVariant var;

	m_htmlQQLogin.CreateFromStatic(IDC_HTML_QQ_LOGIN,this);
	var = (LONG)m_htmlQQLogin.GetSafeHwnd();
	m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(),L"itemHtmlQQLogin",L"hwnd",var);	

	//淡入淡出用
	LONG dwstyle = GetWindowLong(GetSafeHwnd(),GWL_EXSTYLE) ;
	dwstyle |= WS_EX_LAYERED;
	SetWindowLong( GetSafeHwnd() , GWL_EXSTYLE , dwstyle) ;
	m_bLayered = TRUE;

	m_hInst = LoadLibrary(_T("User32.DLL") ) ;
	SetLayeredWindowAttributes = (FSetLayeredWindowAttributes)GetProcAddress(m_hInst ,("SetLayeredWindowAttributes") ) ;	

	m_IE6 = IEversion6();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CQQLoginDlg::Show()//淡出效果
{
	if (!m_bLayered)
	{
		LONG dwstyle = GetWindowLong(GetSafeHwnd(),GWL_EXSTYLE) ;
		dwstyle |= WS_EX_LAYERED;
		SetWindowLong( GetSafeHwnd() , GWL_EXSTYLE , dwstyle) ;
		m_bLayered = TRUE;
	}
	m_alpha = 0;
	SetTimer(TIMER_GRADUALLY_SHOW, 50, NULL);
}

void CQQLoginDlg::Hide()//淡入效果
{	
	if (!m_bLayered)
	{
		LONG dwstyle = GetWindowLong(GetSafeHwnd(),GWL_EXSTYLE) ;
		dwstyle |= WS_EX_LAYERED;
		SetWindowLong( GetSafeHwnd() , GWL_EXSTYLE , dwstyle) ;
		m_bLayered = TRUE;
	}
	m_alpha = 255;
	SetTimer(TIMER_GRADUALLY_HIDE, 50, NULL);
}
void CQQLoginDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	switch(nIDEvent)
	{
	case TIMER_GRADUALLY_SHOW:
		{
			m_alpha += GRADUALLY_SPEED;
			if (m_alpha > 255)
			{
				m_alpha = 255;
			}

			if ( SetLayeredWindowAttributes)
			{
				SetLayeredWindowAttributes( m_hWnd , RGB(0,0,0) ,m_alpha ,LWA_COLORKEY|LWA_ALPHA) ;
				::RedrawWindow( m_hWnd , NULL ,NULL , RDW_ERASE|RDW_INVALIDATE|RDW_FRAME|RDW_ALLCHILDREN) ;
				ShowWindow(SW_SHOWNOACTIVATE);
			}

			if (255 == m_alpha)
			{
				KillTimer(TIMER_GRADUALLY_SHOW);
				LONG dwstyle = GetWindowLong(GetSafeHwnd(),GWL_EXSTYLE) ;
				if (m_bLayered)
				{
					dwstyle &= ~WS_EX_LAYERED;
					SetWindowLong( GetSafeHwnd() , GWL_EXSTYLE , dwstyle) ;
					m_bLayered = FALSE;
				}
				
			}

		}
		break;
	case TIMER_GRADUALLY_HIDE:
		{
			m_alpha -= GRADUALLY_SPEED;
			if (m_alpha < 0)
			{
				m_alpha = 0;
			}

			if ( SetLayeredWindowAttributes)
			{
				SetLayeredWindowAttributes( m_hWnd , RGB(0,0,0) ,m_alpha ,LWA_COLORKEY|LWA_ALPHA) ;
				::RedrawWindow( m_hWnd , NULL ,NULL , RDW_ERASE|RDW_INVALIDATE|RDW_FRAME|RDW_ALLCHILDREN) ;
				ShowWindow(SW_SHOWNOACTIVATE);
			}

			if (0 == m_alpha)
			{
				KillTimer(TIMER_GRADUALLY_HIDE);
				ShowWindow(SW_HIDE);
			}
		}
		break;
	case QQLOGIN_TIMER_IDEVENT:
		{			
			QQSetHTMLSize();

		}
		break;

	default:break;
	}

	CDialog::OnTimer(nIDEvent);
}

void CQQLoginDlg::OnSysCommand(UINT nID,LPARAM lParam)
{
	if(nID == SC_CLOSE && lParam == 0) //频弊 ALT + F4
	{
		return;
	}

	CWnd::OnSysCommand(nID,lParam);

}

BOOL CQQLoginDlg::PreTranslateMessage(MSG* pMsg)
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




void CQQLoginDlg::QQloginOpenURL(UINT unMessage,CString strURL)
{
	if(m_QQLoginURL != NULL)
	{
		delete m_QQLoginURL;
		m_QQLoginURL = NULL;
	}	
	memset(m_QQBuffer,0,sizeof(m_QQBuffer));
	//使用sizeof(m_QQBuffer)-1 是因为如果m_QQBuffer大小不够时，最后一位可存放'\0'，否则(即若使用sizeof(m_QQBuffer))，最后存放'\0'时将会下标超出
	m_QQLoginURL = new common::utility::CAsynOpenURL(this->m_hWnd,unMessage,NULL, m_QQBuffer, sizeof(m_QQBuffer)-1);   

	m_QQLoginURL->OpenURL(strURL);	

}

//获取NewVersion.ini配置文件中QQ登录框的默认大小
void CQQLoginDlg::GetDefineSize()
{
	
	CString strModulePath	= common::utility::systemhelper::Get179AppPath().c_str();	
	CString NewVerFile,AppTip;
	NewVerFile.Format(_T("%s%s\\%s"),strModulePath,VERDOWN_LOCAL_DIR,VERSION_FILE_NEW);//NewVersion.ini最新版本配置文件路径	

	CString strWidth,strHeight;	
	if(m_IE6)  
	{				
		GetPrivateProfileString(_T("QQLoginWindow"),_T("ie6Width"),_T("0"),strWidth.GetBuffer(INI_SIZE_16),INI_SIZE_16,NewVerFile);
		strWidth.ReleaseBuffer();
		GetPrivateProfileString(_T("QQLoginWindow"),_T("ie6Height"),_T("0"),strHeight.GetBuffer(INI_SIZE_16),INI_SIZE_16,NewVerFile);
		strHeight.ReleaseBuffer();  

	}
	else
	{
		GetPrivateProfileString(_T("QQLoginWindow"),_T("Width"),_T("0"),strWidth.GetBuffer(INI_SIZE_16),INI_SIZE_16,NewVerFile);
		strWidth.ReleaseBuffer(); 
		GetPrivateProfileString(_T("QQLoginWindow"),_T("Height"),_T("0"),strHeight.GetBuffer(INI_SIZE_16),INI_SIZE_16,NewVerFile);
		strHeight.ReleaseBuffer(); 

	}

	m_defineWidth = _wtoi(strWidth);
	m_defineHeight = _wtoi(strHeight);

	if(m_defineWidth == 0  )
	{
		if(m_IE6) //若当前为ie6版本浏览器，则内嵌页面宽度设置为516，否则为默认宽度(540)
		{
			m_defineWidth = 516;
		}
		else
		{
			m_defineWidth = QQ_LOGIN_DLG_WIDTH;
		}
		
	}
	if(m_defineHeight == 0)
	{
		m_defineHeight = QQ_LOGIN_DLG_HEIGHT;
	}

	m_HtmlWidth = m_defineWidth;
	m_HtmlHeight = m_defineHeight;	

	CComVariant var;
	var = m_HtmlWidth; 		
	m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(),L"Window",L"width",var);
	var = m_HtmlHeight; 		
	m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(),L"Window",L"height",var);

	
	::SendMessage(GetParent()->GetSafeHwnd(),UM_QQLOGIN_MOVEWINDOW,0,0);  
	
}
void CQQLoginDlg::QQSetHTMLSize()
{
	int qqWidth,qqHeight;
	qqWidth = 0;
	qqHeight = 0;
	m_htmlQQLogin.GetScrollSize(qqWidth,qqHeight);	
	CComVariant var = qqWidth; 		
	//m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(),L"Window",L"width",var);
	if(qqHeight < m_defineHeight)
	{
		qqHeight = m_defineHeight;
	} 	

	if(m_HtmlHeight != qqHeight)
	{
		var = qqHeight;
		m_HtmlHeight = qqHeight ;
		m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(),L"Window",L"height",var);
		::SendMessage(GetParent()->GetSafeHwnd(),UM_QQLOGIN_MOVEWINDOW,0,0);  
	}
	
}

//QQ登录页面预加载
void CQQLoginDlg::QQloginPreLoad()
{
	GetDefineSize();

	if(m_IE6) //当前IE版本是否为IE6
	{
		QQloginLoad();  //IE6版本时，直接加载QQ登录页面
		return;

	}
	
	std::wstring strUrl = common::utility::systemhelper::Get179AppPath() + QQLOGIN_LOAD;	
	m_htmlQQLogin.SetUrl(strUrl,true);
	m_htmlQQLogin.UpdateURL();

	CString sQQUrl;
	CSysSettingIni::GetWebInterfaceUrl(SYSSET_INI_KEYNAME_QQLOGIN,sQQUrl);  
	QQloginOpenURL(WM_QQLOGIN_GET_PRELOADQQ,sQQUrl);
}

//QQ登录页面加载
void CQQLoginDlg::QQloginLoad()
{
	CString sQQUrl;
	CSysSettingIni::GetWebInterfaceUrl(SYSSET_INI_KEYNAME_QQLOGIN,sQQUrl);
	m_htmlQQLogin.SetUrl(std::wstring(sQQUrl),true);
	m_htmlQQLogin.UpdateURL();

}

//QQ登录页面加载失败
void CQQLoginDlg::QQloginLoadError()
{	
	std::wstring strUrl = common::utility::systemhelper::Get179AppPath() + QQLOGIN_OPENERROR;
	m_htmlQQLogin.SetUrl(strUrl,true);
	m_htmlQQLogin.UpdateURL();
}

//QQ登录失败
void CQQLoginDlg::QQloginError()
{
	CString sQQError;
	CSysSettingIni::GetWebInterfaceUrl(SYSSET_INI_KEYNAME_QQLOGIN_ERROR,sQQError);  
	m_htmlQQLogin.SetUrl(std::wstring(sQQError),true);
	m_htmlQQLogin.UpdateURL();

}

//再试一次,重新打开QQ登录页面
void CQQLoginDlg::QQloginAgainLoad()
{
	QQloginPreLoad();
}


//使用179账号登录，转至179登录
void CQQLoginDlg::QQloginUse179login()
{
	::SendMessage(GetParent()->GetSafeHwnd(),UM_QQLOGIN_SET,0,0);  

}


//注册179账号， 使用浏览器打开官方179账号注册页面
void CQQLoginDlg::QQloginRegister179Web()
{
	CString strUrl;
	CSysSettingIni::GetWebInterfaceUrl(SYSSET_INI_KEYNAME_179REGISTER_OFFICAL,strUrl);  
	common::utility::systemhelper::OpenUrl(wstring(strUrl));    //使用浏览器打开官方179账号注册页面

}

//当前IE版本是否为IE6
bool CQQLoginDlg::IEversion6()
{
	bool isIE6 = false;
	CString strIEVer = common::utility::systemhelper::GetIEVersion().c_str();
	if(strIEVer.Left(2) == _T("6."))  //若注册表中SOFTWARE\\Microsoft\\Interent Explorer 下的Version键值前2字符等于6. 则为IE6
	{
		isIE6 = true;
	}

	return isIE6;
}


void CQQLoginDlg::GetQQHtmlSize(int &qqWidth,int &qqHeight)
{
	qqWidth = m_HtmlWidth;
	qqHeight = m_HtmlHeight;
}