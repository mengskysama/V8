// IconIntroductionDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "..\Room.h"
#include "IconIntroductionDlg.h"
#include "Skin_i.h"
#include "SkinUtil.h"
#include "..\RoomBottomLeftDlg.h"
#include "..\..\CommonLibrary\include\utility\SystemHelper.h"

// CIconIntroductionDlg 对话框

IMPLEMENT_DYNAMIC(CIconIntroductionDlg, CDialog)

CIconIntroductionDlg::CIconIntroductionDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CIconIntroductionDlg::IDD, pParent)
{
	m_pSkinEngine = NULL;
	::GetSkinEngine(&m_pSkinEngine);
	m_pParentDlg = (CRoomBottomLeftDlg *)pParent;
}

CIconIntroductionDlg::~CIconIntroductionDlg()
{
}

void CIconIntroductionDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CIconIntroductionDlg, CDialog)
	ON_WM_CREATE()
	ON_SKIN_MESSAGE
END_MESSAGE_MAP()


// CIconIntroductionDlg 消息处理程序

int CIconIntroductionDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  在此添加您专用的创建代码
	//LoadSkin
	ASSERT(NULL != m_pSkinEngine);
	if (NULL != m_pSkinEngine)
	{
		HRESULT hr = m_pSkinEngine->LoadSkin(GetSafeHwnd(),L"IconIntroductionDlg");
		ASSERT(hr== S_OK && __LINE__);
	}

	//标题字体首选微软雅黑
	std::wstring strFont;
	common::utility::systemhelper::GetDefaultFontName(strFont);
	CComVariant var = (CComVariant) strFont.c_str();
	m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(),L"textTitle",L"fontFamily",var);

	return 0;
}


LRESULT CIconIntroductionDlg::OnSkinMessage(WPARAM wParam, LPARAM lParam)
{
	switch(wParam)
	{
	case SKIN_ON_CLICK_CLOSE:
		{
		   if (m_pParentDlg && m_pParentDlg->GetSafeHwnd())
		   {
			   //m_pParentDlg->OnClickIconIntroClose();
			   this->ShowWindow(SW_HIDE);
		   }   
		}
		break;

	default:break;
	}

	return TRUE;
}

BOOL CIconIntroductionDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	m_htmlIconIntroduce.CreateFromStatic(IDC_HTML_ICON_INTRODUCTION,this);
	CComVariant var = (LONG)m_htmlIconIntroduce.GetSafeHwnd();
	var = (LONG)m_htmlIconIntroduce.GetSafeHwnd();
	m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(),L"itemHtmlIconIntroduce",L"hwnd",var);

	InitBrowser();
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CIconIntroductionDlg::InitBrowser()
{
	std::wstring strUrl = common::utility::systemhelper::Get179AppPath() + ICON_DIRECTION_PATH;
	m_htmlIconIntroduce.SetUrl(strUrl);
	m_htmlIconIntroduce.UpdateURL();
	
}
BOOL CIconIntroductionDlg::PreTranslateMessage(MSG* pMsg)
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
