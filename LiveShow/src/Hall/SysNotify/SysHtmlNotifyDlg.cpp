// SysHtmlNotifyDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "../Hall.h"
#include "SysHtmlNotifyDlg.h"
#include "Skin_i.h"
#include "SkinUtil.h"
#include "../CommonLibrary/include/utility/SystemHelper.h"

#define SYSMESSAGE_HTML			L"resource\\html\\SysMes.html"

//#define  SKIN_ON_CLICK_BTN_CLOSE  1
// CSysHtmlNotifyDlg 对话框

IMPLEMENT_DYNAMIC(CSysHtmlNotifyDlg, CDialog)

CSysHtmlNotifyDlg::CSysHtmlNotifyDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSysHtmlNotifyDlg::IDD, pParent)
{
	m_pSkinEngine = NULL;
	::GetSkinEngine(&m_pSkinEngine);
	m_bMouseLeave= TRUE;
	m_CurSysMessage = NULL;
}

CSysHtmlNotifyDlg::~CSysHtmlNotifyDlg()
{
}

void CSysHtmlNotifyDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CSysHtmlNotifyDlg, CDialog)
	ON_WM_CREATE()
	ON_SKIN_MESSAGE	
	ON_WM_SYSCOMMAND()
	ON_MESSAGE(WM_MISSION_HYPERLINK_ONCLICK_MOUSEUP,&CSysHtmlNotifyDlg::OnClick)
END_MESSAGE_MAP()


// CSysHtmlNotifyDlg 消息处理程序


int CSysHtmlNotifyDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  在此添加您专用的创建代码
	ASSERT(m_pSkinEngine != NULL);
	if (NULL != m_pSkinEngine)
	{
		HRESULT hr = m_pSkinEngine->LoadSkin(GetSafeHwnd(),L"SysHtmlNotifyDlg");
		ASSERT(hr == S_OK && __LINE__);
	}

	return 0;
}

BOOL CSysHtmlNotifyDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	ModifyStyleEx( WS_EX_APPWINDOW , WS_EX_TOOLWINDOW) ;

	int cx = GetSystemMetrics(SM_CXSCREEN);  //屏幕宽度
	int cy = GetSystemMetrics(SM_CYSCREEN);  //屏幕高度
	// pSysNotifyDlg->SetWindowPos(NULL,cx - 250,cy - 170,250,170,SWP_SHOWWINDOW);
	RECT rect;
	::SystemParametersInfo(SPI_GETWORKAREA,sizeof(RECT),&rect,0); //工作区域大小
	RECT deskRect;
	::GetWindowRect(::GetDesktopWindow(),&deskRect); //桌面窗口大小
	int startHight = deskRect.bottom - rect.bottom;  //系统状态栏高度

	CRect clientRect(0,0,0,0);
	clientRect.left = cx - SYS_HTML_NOTIFY_WIDTH;
	clientRect.top = cy - startHight - SYS_HTML_NOTIFY_HIGHT;
	clientRect.right = clientRect.left + SYS_HTML_NOTIFY_WIDTH;
	clientRect.bottom = clientRect.top + SYS_HTML_NOTIFY_HIGHT;
	MoveWindow(&clientRect);

	CComVariant var;
	m_htmlSysNotify.CreateFromStatic(IDC_HTML_SYSNOTIFY, this);
	var = (LONG)m_htmlSysNotify.GetSafeHwnd();
	m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(),L"itemHtmlSysNotify",L"hwnd",var);
	InitTheHtml();

	return FALSE;//处理抢焦点问题 

	//return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


LRESULT CSysHtmlNotifyDlg::OnSkinMessage(WPARAM wParam, LPARAM lParam)
{
	switch(wParam)
	{
	/*case SKIN_ON_CLICK_BTN_CLOSE:
		{
			DestroyWindow();
		}
		break;*/

	default:break;
	}


	return S_OK;
}

void CSysHtmlNotifyDlg::InitTheHtml()
{
	
	std::wstring strUrl = common::utility::systemhelper::Get179AppPath() + SYSMESSAGE_HTML;
	m_htmlSysNotify.SetUrl(strUrl,true);
	m_htmlSysNotify.UpdateURL();

}

void CSysHtmlNotifyDlg::SetSystemMessage(SystemMessage* sysMes)
{
	m_CurSysMessage = sysMes;
}

//载取点击m_htmlSysNotify系统消息页面时的响应函数
LRESULT CSysHtmlNotifyDlg::OnClick(WPARAM wParam,LPARAM lParam)
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
		CString strName = m_htmlSysNotify.GetValue( pElement , _T("name") ) ;
		if(strName == _T("closemes"))
		{
			DestroyWindow();
		}	
	}

	return TRUE;
}
// 修改网页中消息元素的内容
void CSysHtmlNotifyDlg::PutElementHtmlMsg()
{
	if(m_CurSysMessage == NULL)
	{
		return;
	}

	vector<SystemMessage::MessageInfo> &MessInfo = m_CurSysMessage->strMessage;
	if(MessInfo.size() ==0 )
	{
		return;
	}
	
	switch(m_CurSysMessage->enmType)
	{
	case SystemMessage::SystemMessageType::SYSTEMMESSAGETYPE_TEXT:	
	case SystemMessage::SystemMessageType::SYSTEMMESSAGETYPE_WITHOUT_TITLE:
	case SystemMessage::SystemMessageType::SYSTEMMESSAGETYPE_MAGNATE:  //富豪等级升级消息
		{			
			m_htmlSysNotify.PutElementHtml(_T("title"),m_CurSysMessage->strWindowTitle.c_str()); 
			m_htmlSysNotify.PutElementHtml(_T("content"),MessInfo[0].strMessage.c_str()); 
		}
		break;
	default:
		break;
	}
	
}


LRESULT CSysHtmlNotifyDlg::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	switch(message)
	{
	case WM_MISSION_COMPLETED:
		{
			PutElementHtmlMsg();			
				
		}
		break;
	
	case WM_MISSION_NAVIGATECOMPLETE2:
		{
			
		}
		break;
	default:
		break;		
	}

	return CDialog::WindowProc(message, wParam, lParam);
}

BOOL CSysHtmlNotifyDlg::IfMouseLeave()
{
	if(this->GetSafeHwnd() && this->IsWindowVisible())
	{
		CPoint point2 ;
		GetCursorPos( &point2 ) ;
		CRect rect ;
		this->GetWindowRect(&rect) ;
		if ( rect.PtInRect(point2) )
		{
			m_bMouseLeave = FALSE;
		}else
		{
			m_bMouseLeave = TRUE;
		}		

		return m_bMouseLeave;
	}
	else
	{
		return TRUE;
	}
}
