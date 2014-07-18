// PreviewDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "..\Room.h"
#include "PreviewDlg.h"
#include "Skin_i.h"
#include "SkinUtil.h"
#include "..\..\CommonLibrary\include\utility\SystemHelper.h"
#include "..\..\CommonLibrary\include\utility\URLEncode.h"
#include "..\GlobalRoomDefine.h"

#define  SKIN_ON_CLICK_CLOSE         1
// CPreviewDlg 对话框

IMPLEMENT_DYNAMIC(CPreviewDlg, CDialog)

CPreviewDlg::CPreviewDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CPreviewDlg::IDD, pParent),
	m_PreviewLogic(0 , HTML_TYPE_PREVIEW )
{
	m_pSkinEngine = NULL;
	::GetSkinEngine(&m_pSkinEngine);
}

CPreviewDlg::~CPreviewDlg()
{
	if (m_pPreviewHtml)
	{
		delete m_pPreviewHtml;
		m_pPreviewHtml = NULL;
	}
}

void CPreviewDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CPreviewDlg, CDialog)
	ON_WM_CREATE()
	ON_SKIN_MESSAGE
END_MESSAGE_MAP()


// CPreviewDlg 消息处理程序

int CPreviewDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  在此添加您专用的创建代码
	//LoadSkin
	ASSERT(NULL != m_pSkinEngine);
	if (NULL != m_pSkinEngine)
	{
		HRESULT hr = m_pSkinEngine->LoadSkin(GetSafeHwnd(),L"PreviewDlg");
		ASSERT(hr== S_OK && __LINE__);
	}
    
	//标题字体首选微软雅黑
	std::wstring strFont;
	common::utility::systemhelper::GetDefaultFontName(strFont);
	CComVariant var = (CComVariant) strFont.c_str();
	m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(),L"textTitle",L"fontFamily",var);

	return 0;
}

LRESULT CPreviewDlg::OnSkinMessage(WPARAM wParam, LPARAM lParam)
{
	switch(wParam)
	{
	case SKIN_ON_CLICK_CLOSE:
		{
           //DestroyWindow();
			ShowWindow(SW_HIDE);
		}
		break;
	default:break;
	}

	return TRUE;
}

BOOL CPreviewDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	m_pPreviewHtml = new HtmlEditImpl ;
	m_pPreviewHtml->SetHtmlEditLogic( &m_PreviewLogic ) ;
	m_pPreviewHtml->Create( this , IDC_HTML_PREVIEW , CRect(0,0,0,0) , WS_CHILD|WS_VISIBLE|WS_HSCROLL ) ;
	CComVariant var = (LONG)m_pPreviewHtml->GetSafeHwnd() ;
	m_pSkinEngine->SetSubSkinProp(GetSafeHwnd() , L"itemHtmlPreview",L"hwnd",var);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

BOOL CPreviewDlg::PreTranslateMessage(MSG* pMsg)
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

void CPreviewDlg::SetContent( std::wstring strContent )
{
	if (strContent != L"")
	{
		std::wstring strModulePath = common::utility::systemhelper::Get179AppPath();
		common::CURLEncode urlEncode;
		strModulePath = urlEncode.URLEncodeLocalPath(strModulePath);
		std::wstring strNoticePngPath = strModulePath + IMG_NOTICE_PNG_PATH;

		CString strImg ;
		strImg.Format( L"<IMG src=\"%s\">" ,strNoticePngPath.c_str() ) ;

		m_pPreviewHtml->ClearMsg();
		m_pPreviewHtml->InsertText( strImg + L" " + strContent.c_str() );
	}
}

void CPreviewDlg::SetInitContent( std::wstring strContent )
{
	std::wstring strModulePath = common::utility::systemhelper::Get179AppPath();
	common::CURLEncode urlEncode;
	strModulePath = urlEncode.URLEncodeLocalPath(strModulePath);
	std::wstring strNoticePngPath = strModulePath + IMG_NOTICE_PNG_PATH;

	CString strImg ;
	strImg.Format( L"<IMG src=\"%s\">" ,strNoticePngPath.c_str() ) ;

	CString strTemp( strImg + L" " + strContent.c_str() );
	m_PreviewLogic.SetInitText(strTemp);
	//m_PreviewLogic->init(strNotice);
}