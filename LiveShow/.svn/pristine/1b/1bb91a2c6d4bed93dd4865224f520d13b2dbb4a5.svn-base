// SysNotifyDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "../Hall.h"
#include "SysNotifyDlg.h"
#include "Skin_i.h"
#include "SkinUtil.h"
#include "../CommonLibrary/include/utility/SystemHelper.h"

#define  SKIN_ON_CLICK_BTN_CLOSE    1
#define  SKIN_ON_CLICK_BTN_JOIN     2
#define  SKIN_ON_CLICK_BTN_PREPAGE  3
#define  SKIN_ON_CLICK_BTN_NEXTPAGE 4
// CSysNotifyDlg 对话框

IMPLEMENT_DYNAMIC(CSysNotifyDlg, CDialog)

CSysNotifyDlg::CSysNotifyDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSysNotifyDlg::IDD, pParent)
{
	m_pSkinEngine = NULL;
	::GetSkinEngine(&m_pSkinEngine);

	m_curPage = 0;  
	m_totalPage = 0;

	m_bMouseLeave = TRUE;
}

CSysNotifyDlg::~CSysNotifyDlg()
{
	ClearSysMes();
}

void CSysNotifyDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CSysNotifyDlg, CDialog)
	ON_WM_CREATE()
	ON_SKIN_MESSAGE
	ON_WM_MOUSEMOVE()
	ON_MESSAGE(WM_MOUSELEAVE,OnMouseLeave)
    //下面两个是针对非客户区的
	ON_WM_NCMOUSEMOVE()
	ON_MESSAGE(WM_NCMOUSELEAVE,OnNcMouseLeave)
END_MESSAGE_MAP()


// CSysNotifyDlg 消息处理程序

int CSysNotifyDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  在此添加您专用的创建代码
	ASSERT(m_pSkinEngine != NULL);
	if (NULL != m_pSkinEngine)
	{
		HRESULT hr = m_pSkinEngine->LoadSkin(GetSafeHwnd(),L"SysNotifyDlg");
		ASSERT(hr == S_OK && __LINE__);
	}
	
	//标题字体首选微软雅黑
	/*std::wstring strFont;
	common::utility::systemhelper::GetDefaultFontName(strFont);
	CComVariant var = (CComVariant) strFont.c_str();
	m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(),L"textTitle",L"fontFamily",var);*/

	return 0;
}

LRESULT CSysNotifyDlg::OnSkinMessage(WPARAM wParam, LPARAM lParam)
{
	switch(wParam)
	{
	case SKIN_ON_CLICK_BTN_CLOSE:
		{
           DestroyWindow();
		}
		break;
	case SKIN_ON_CLICK_BTN_JOIN:
		{
			SYSMSGMAP::iterator iter =  m_mapSysMsg.find(m_curPage);
			if (iter != m_mapSysMsg.end())
			{
				stSysMsg* pSysMsg = iter->second;
				common::utility::systemhelper::OpenUrl(std::wstring(pSysMsg->strHyperLingAddr));
			}
		}
		break;
	case SKIN_ON_CLICK_BTN_PREPAGE:
		{
            m_curPage--;
			ShowSysMsg();
		}
		break;
	case SKIN_ON_CLICK_BTN_NEXTPAGE:
		{
            m_curPage++;
			ShowSysMsg();
		}
		break;

	default:break;
	}


	return TRUE;
}

void CSysNotifyDlg::SetSysMsg(SYSMSGMAP mapSysMsg ,LPCTSTR pTitle)
{
	CString strTitle = pTitle;
	CComVariant var = strTitle;
	m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(),L"textTheTitle",L"text",var);

	m_mapSysMsg = mapSysMsg;
	if (m_mapSysMsg.size() > 0)
	{
		m_curPage = 0;//0为第一条消息,即为当前页,显示则为curPage + 1
		m_totalPage = m_mapSysMsg.size();//总页数
		ShowSysMsg();
	}
    
}

//清除所有消息
void CSysNotifyDlg::ClearSysMes()
{
	SYSMSGMAP::iterator iter;
	for(iter = m_mapSysMsg.begin(); iter != m_mapSysMsg.end(); )
	{
		stSysMsg *sysMsg = iter->second;
		delete sysMsg;
		iter = m_mapSysMsg.erase(iter);
	}
}

BOOL CSysNotifyDlg::IfMouseLeave()
{
	if(this->GetSafeHwnd() && this->IsWindowVisible())
	{
		return m_bMouseLeave;
	}
	else
	{
		return TRUE;
	}
}
void CSysNotifyDlg::ShowSysMsg()
{
	SYSMSGMAP::iterator iter =  m_mapSysMsg.find(m_curPage);
	if (iter != m_mapSysMsg.end())
	{
		stSysMsg* pSysMsg = iter->second;

		CComVariant var = pSysMsg->strContent;
		m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(),L"textNotify",L"text",var);
		var = pSysMsg->strHyperLinkLabel;
		m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(),L"btnJoin",L"label",var);
	//	var = pSysMsg->strHyperLingAddr;
	//	m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(),L"btnJoin",L"hyperLinkAddress",var);

		
		if (m_totalPage == 1)//只有一条消息
		{
			var = FALSE;
			m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(),L"btnPrePage",L"visible",var);
			m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(),L"btnNextPage",L"visible",var);
			CString textPage;
			textPage.Format(L"%d/%d",m_curPage + 1,m_totalPage);
			var = textPage;
			m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(),L"textPage",L"text",var);
		}else if(m_curPage == 0 && m_totalPage > 1)//不只一条消息，显示第一条时
		{
			var = FALSE;
			m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(),L"btnPrePage",L"visible",var);
			var = TRUE;
			m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(),L"btnNextPage",L"visible",var);
			CString textPage;
			textPage.Format(L"%d/%d",m_curPage + 1,m_totalPage);
			var = textPage;
			m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(),L"textPage",L"text",var);
		}else if (m_curPage == (m_totalPage - 1) && m_totalPage > 1)//不只一条消息，显示最后一条时
		{
			var = TRUE;
			m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(),L"btnPrePage",L"visible",var);
			var = FALSE;
			m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(),L"btnNextPage",L"visible",var);
			CString textPage;
			textPage.Format(L"%d/%d",m_curPage + 1,m_totalPage);
			var = textPage;
			m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(),L"textPage",L"text",var);
		}else 
		{
			var = TRUE;
			m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(),L"btnPrePage",L"visible",var);
			var = TRUE;
			m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(),L"btnNextPage",L"visible",var);
			CString textPage;
			textPage.Format(L"%d/%d",m_curPage + 1,m_totalPage);
			var = textPage;
			m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(),L"textPage",L"text",var);
		}

	}

}

void CSysNotifyDlg::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
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

	TRACKMOUSEEVENT tme ;
	tme.cbSize = sizeof( tme ) ;
	tme.dwFlags = TME_LEAVE | TME_HOVER;
	tme.dwHoverTime = 10 ;
	tme.hwndTrack = m_hWnd ;
	::TrackMouseEvent(&tme) ;
  
	CDialog::OnMouseMove(nFlags, point);
}

HRESULT CSysNotifyDlg::OnMouseLeave( WPARAM wParam , LPARAM lParam )
{
	CPoint point ;
	GetCursorPos( &point ) ;
	CRect rect ;
	this->GetWindowRect(&rect) ;
	if ( rect.PtInRect(point) )
	{
		m_bMouseLeave = FALSE;
	}else
	{
		m_bMouseLeave = TRUE;
	}
	
	return S_OK;
}

HRESULT CSysNotifyDlg::OnNcMouseLeave( WPARAM wParam , LPARAM lParam )
{
	CPoint point ;
	GetCursorPos( &point ) ;
	CRect rect ;
	this->GetWindowRect(&rect) ;
	if ( rect.PtInRect(point) )
	{
		m_bMouseLeave = FALSE;
	}else
	{
		m_bMouseLeave = TRUE;
	}
	return TRUE;
}

void CSysNotifyDlg::OnNcMouseMove(UINT nHitTest, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
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

	TRACKMOUSEEVENT tme ;
	tme.cbSize = sizeof( tme ) ;
	tme.dwFlags = TME_LEAVE | TME_HOVER | TME_NONCLIENT;
	tme.dwHoverTime = 10 ;
	tme.hwndTrack = m_hWnd ;
	::TrackMouseEvent(&tme) ;

	CDialog::OnNcMouseMove(nHitTest, point);
}

BOOL CSysNotifyDlg::OnInitDialog()
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

	m_rect.left = cx - SYSNOTIFY_WIDTH;
	m_rect.top = cy - startHight - SYSNOTIFY_HIGHT;
	m_rect.right = m_rect.left + SYSNOTIFY_WIDTH;
	m_rect.bottom = m_rect.top + SYSNOTIFY_HIGHT;

	//this->SetWindowPos(NULL,m_rect.left,m_rect.top,m_rect.right,m_rect.bottom,SWP_SHOWWINDOW);
	MoveWindow(&m_rect);

	return FALSE;//处理抢焦点问题 
	//return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}
