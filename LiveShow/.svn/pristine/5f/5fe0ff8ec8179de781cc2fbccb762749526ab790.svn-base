#include "stdafx.h"
#include "../Hall.h"
#include "../HallWindow/HallDlg.h"
#include "GameDlg.h"
#include "skin_i.h"
#include "SkinUtil.h"
#include "../SysSettingIni.h"
#include "assert.h"
#include "../CommonLibrary/include/utility/SystemHelper.h"
#include "../CommonLibrary/include/utility/func4Stat.h"

#define SKIN_ON_CLICK_CLOSE   1
#define SKIN_ON_CLICK_MIN 2

// CGameDlg 对话框

IMPLEMENT_DYNAMIC(CGameDlg, CDialog)

CGameDlg::CGameDlg(HWND sendMesHWND, CWnd* pParent /*=NULL*/)
	: CDialog(CGameDlg::IDD, pParent)
{
    m_pSkinEngine = NULL;
	m_sendMesHWND = sendMesHWND;
	m_pParent = pParent;

    ::GetSkinEngine(&m_pSkinEngine);
}


CGameDlg::~CGameDlg()
{
}

void CGameDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CGameDlg, CDialog)
	ON_WM_CREATE()
	ON_SKIN_MESSAGE
	ON_WM_SYSCOMMAND()
	ON_MESSAGE(WM_MISSION_HYPERLINK_ONCLICK,&CGameDlg::OnClick)
	ON_MESSAGE(WM_MISSION_HYPERLINK_OPENURL,&CGameDlg::OnOpenURL)
END_MESSAGE_MAP()


// CGameDlg 消息处理程序

int CGameDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  在此添加您专用的创建代码
	ASSERT(NULL != m_pSkinEngine);
	if (NULL != m_pSkinEngine)
	{
		HRESULT hr = m_pSkinEngine->LoadSkin(GetSafeHwnd(),L"GameDlg");
		ASSERT(hr== S_OK && __LINE__);
	}

	if(m_pParent!= NULL)
	{
		CenterWindow(m_pParent);		
	}	

	return 0;
}

LRESULT CGameDlg::OnSkinMessage(WPARAM wParam,LPARAM lParam)
{
	switch(wParam)
	{
	case SKIN_ON_CLICK_CLOSE:
		{
			PostMessage(WM_CLOSE,NULL,NULL);
			g_HallModule->CloseHallGameDlg();
		}
		break;
	case SKIN_ON_CLICK_MIN:
		{
			PostMessage(WM_SYSCOMMAND, SC_MINIMIZE, 0);
			break;
		}
	default:break;
	}

	return TRUE;
}

LRESULT CGameDlg::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	switch(message)
	{
		case WM_MISSION_COMPLETED:
			{
				//RegisterElementSetFocus();
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
				//Register179Success();
			}
			break;
		default:
			break;		
	}

	return CDialog::WindowProc(message, wParam, lParam);

}
//载取点击m_htmlRegister注册页面中超链接时的响应函数
LRESULT CGameDlg::OnClick(WPARAM wParam,LPARAM lParam)
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
		CString strName = m_htmlGame.GetValue( pElement , _T("name") ) ;
		if(strName == _T("ok"))
		{			
			PostMessage(WM_CLOSE,0,0);					
			g_HallModule->CloseHallGameDlg();
		}
	}

	return TRUE;
}

LRESULT CGameDlg::OnOpenURL(WPARAM wParam,LPARAM lParam)
{
	LPCTSTR lpszURL = (LPCTSTR)wParam;
	std::wstring strURL = lpszURL;
	g_HallModule->Invoke(Hall::SERVICE_VALUE_HALL_OPEN_TOKEN_URL, (param)&strURL);
	return S_OK;
}

void CGameDlg::InitBrowser()
{
	CComVariant varImage1,varImage2;

	SetWindowSize(1024,738);
	
	m_htmlGame.SetUrl(wstring(m_gameURL.GetBuffer()), true);		
	m_htmlGame.UpdateURL();
}

//设置内嵌页面的焦点
void CGameDlg::RegisterElementSetFocus()
{	
	CString sElementID = _T("rolename");
	m_htmlGame.ElementSetFocus(sElementID);
}

BOOL CGameDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	ModifyStyleEx(WS_EX_TOOLWINDOW,WS_EX_APPWINDOW);
	SetWindowText(L"白兔小游戏");
	//标题字体首选微软雅黑
	std::wstring strFont;
	common::utility::systemhelper::GetDefaultFontName(strFont);
	CComVariant var = (CComVariant) strFont.c_str();
	m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(),L"textTitle",L"fontFamily",var);

	//创建浏览器
	m_htmlGame.CreateFromStatic(IDC_HTML_REGISTER,this);
	var = (LONG)m_htmlGame.GetSafeHwnd();
	m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(),L"itemHtmlGame",L"hwnd",var);	
	
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CGameDlg::SetURL(CString& strURL)
{
	m_gameURL = strURL;
	InitBrowser();
}

void CGameDlg::SetWindowSize(int wndWidth, int wndHeight)
{
	CComVariant var = wndWidth;
    m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(),L"Window",L"width",var);
	var = wndHeight;
	m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(),L"Window",L"height",var);

}

BOOL CGameDlg::PreTranslateMessage(MSG* pMsg)
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


void CGameDlg::OnSysCommand(UINT nID,LPARAM lParam)
{
	if(nID == SC_CLOSE && lParam == 0) //频弊 ALT + F4
	{
		return;
	}
	
	CWnd::OnSysCommand(nID,lParam);

}