// ShowFansDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "..\Room.h"
#include "skin_i.h"
#include "SkinUtil.h"
#include "SysSettingDlg.h"
#include "ShowFansDlg.h"
#include "utility/SystemHelper.h"

#define ANCHOR_MYFANS_HTML			L"resource\\html\\fans\\myFans.html"

// CShowFansDlg 对话框

IMPLEMENT_DYNAMIC(CShowFansDlg, CDialog)

CShowFansDlg::CShowFansDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CShowFansDlg::IDD, pParent)
{
	m_pSkinEngine = NULL;
	::GetSkinEngine(&m_pSkinEngine);

}

CShowFansDlg::~CShowFansDlg()
{
}

void CShowFansDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CShowFansDlg, CDialog)
	ON_WM_CREATE()
	ON_SKIN_MESSAGE
	ON_WM_SHOWWINDOW()
END_MESSAGE_MAP()


LRESULT CShowFansDlg::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	switch(message)
	{
	case WM_NOTIFY_ROOM_SYSSET_CLICKOK:  //“确定”按扭的消息		
	case WM_NOTIFY_ROOM_SYSSET_CLICKAPPLI:  // "应用"按扭的消息
		{

		}
		break;	
	case WM_MISSION_COMPLETED:  	//网页真正加载完毕
		{
			
			m_fansValue.OnGetFans(GetCurInfo().unUIN);
		}
		break;		
	default:
		break;
	}

	return CDialog::WindowProc(message, wParam, lParam);
}

BOOL CShowFansDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	//屏蔽ESC按键和ENTER键
	if ( pMsg->message == WM_KEYDOWN)
	{
		switch(pMsg->wParam)
		{
		case VK_ESCAPE:
		case VK_RETURN:

			return TRUE ;
		default: break;
		}	
	}

	return CDialog::PreTranslateMessage(pMsg);
}


void CShowFansDlg::OnShowWindow(BOOL bShow,UINT nStatus)
{
	CDialog::OnShowWindow(bShow, nStatus);

	if(bShow)
	{
		Init();
	}

}
BOOL CShowFansDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	m_htmlFans.CreateFromStatic(IDC_ATROOM_HTML_SHOW_FANS,this);
	CComVariant var = (LONG)m_htmlFans.GetSafeHwnd();
	m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(),L"itemHtmlFans",L"hwnd",var);

	m_fansValue.Init(&m_htmlFans);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CShowFansDlg::Init()
{		
	std::wstring strUrl = common::utility::systemhelper::Get179AppPath() + ANCHOR_MYFANS_HTML;
	m_htmlFans.SetUrl(strUrl);
	m_htmlFans.UpdateURL();
}

void CShowFansDlg::SetApplyEffect(BOOL bEffect)
{
	CComVariant var = bEffect;
	m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(), L"imgApplyEffect", L"visible", var);
}
// CShowFansDlg 消息处理程序



int CShowFansDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  在此添加您专用的创建代码
	ASSERT(NULL != m_pSkinEngine);
	if (NULL != m_pSkinEngine)
	{
		HRESULT hr = m_pSkinEngine->LoadSkin(GetSafeHwnd(),L"ShowFansDlg_AtRoom");
		ASSERT(hr== S_OK && __LINE__);
	}

	return 0;
}

LRESULT CShowFansDlg::OnSkinMessage(WPARAM wParam,LPARAM lParam)
{

	return TRUE;
}

//获取粉丝数据返回响应函数	
void CShowFansDlg::OnGetFansRsp(Event const&evt)
{
	m_fansValue.OnGetFansRsp(evt,_T("您当前还没有粉丝"));
}