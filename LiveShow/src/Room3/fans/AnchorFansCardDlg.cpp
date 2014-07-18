// AchorFansCardDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "..\Room.h"
#include "SkinUtil.h"
#include "AnchorFansCardDlg.h"
#include "..\RoomParentDlg.h"
#include "..\include\utility\SystemHelper.h"
#include "..\..\CommonLibrary\include\utility\URLEncode.h"

#define ANCHOR_FANS_CARD_HTML			L"resource\\html\\fans\\anchorFans.html"
#define FANS_NUMBER                 _T("number")
#define FANS_NAME					_T("name")
#define FANS_VALUE					_T("value")

// CAchorFansCardDlg 对话框

IMPLEMENT_DYNAMIC(CAnchorFansCardDlg, CDialog)

CAnchorFansCardDlg::CAnchorFansCardDlg( CRoomParentDlg* pParentDlg ,CWnd* pParent /*=NULL*/)
: CDialog(CAnchorFansCardDlg::IDD, pParent),
m_pParentDlg( pParentDlg )
{
	m_pSkinEngine = NULL;
	::GetSkinEngine(&m_pSkinEngine);
	m_unAnchorUin = 0;

	m_PansHtmlPath = common::utility::systemhelper::Get179AppPath() + ANCHOR_FANS_CARD_HTML;	

}

CAnchorFansCardDlg::~CAnchorFansCardDlg()
{
}

void CAnchorFansCardDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CAnchorFansCardDlg, CDialog)
	ON_WM_CREATE()
	ON_SKIN_MESSAGE
END_MESSAGE_MAP()


// CUserInfoCardDlg 消息处理程序


int CAnchorFansCardDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  在此添加您专用的创建代码
	//LoadSkin
	ASSERT(NULL != m_pSkinEngine);
	if (NULL != m_pSkinEngine)
	{
		HRESULT hr = m_pSkinEngine->LoadSkin(GetSafeHwnd(),L"AnchorFansCardDlg");
		ASSERT(hr== S_OK && __LINE__);
	}

	return 0;
}

BOOL CAnchorFansCardDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	ModifyStyleEx(0,WS_EX_TOOLWINDOW);

	CComVariant var;
	m_htmlFansCard.CreateFromStatic(IDC_HTML_ANCHORFANS_CARD,this);
	var = (LONG)m_htmlFansCard.GetSafeHwnd();
	m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(),L"itemHtmlFansCard",L"hwnd",var);

	m_fansValue.Init(&m_htmlFansCard);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CAnchorFansCardDlg::Show( IUserItem * pUserItem )
{
	ASSERT(pUserItem != NULL);
	if(pUserItem == NULL)
		return;

	m_htmlFansCard.SetUrl(m_PansHtmlPath,false,false,true);
	m_htmlFansCard.UpdateURL();

	m_unAnchorUin =  pUserItem->GetUserUIN();

	ShowWindow(SW_SHOW);
}


//获取粉丝数据返回响应函数	
void CAnchorFansCardDlg::OnGetFansRsp(Event const&evt)
{
	m_fansValue.OnGetFansRsp(evt,_T("送<strong>TA</strong>礼物，抢做<strong>TA</strong>的粉丝吧"));
}

void CAnchorFansCardDlg::Hide()
{
	ShowWindow(SW_HIDE);
}


LRESULT CAnchorFansCardDlg::OnSkinMessage( WPARAM wParam,LPARAM lParam )
{
	switch(wParam)
	{
	
	default:break;
	}


	return TRUE;
}

LRESULT CAnchorFansCardDlg::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	switch(message)
	{
	case WM_MISSION_COMPLETED:  	//网页真正加载完毕
		{
			m_fansValue.OnGetFans(m_unAnchorUin);
		}
		break;	
		
	case WM_MISSION_HYPERLINK_MOUSEOUT:  //页面的鼠标移动事件  OnMouseOut
		{
			CPoint *point2 = (CPoint*)lParam;			
			CRect rect ;
			this->GetWindowRect(&rect) ;			
			if (!rect.PtInRect(*point2) )
			{				
				Hide();
			}
		}
		break;

		
	default:
		break;
	}

	return CDialog::WindowProc(message, wParam, lParam);
}