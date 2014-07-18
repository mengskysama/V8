// ModelessMsgBox.cpp : 实现文件
//

#include "../../stdafx.h"
#include "../../include/ui/ModelessMsgBox.h"
#include "skin_i.h"
#include "SkinUtil.h"
#include "../../include/utility/SystemHelper.h"

#define SKIN_ON_CLICK_CLOSE                    1
#define SKIN_ON_CLICK_OK                       2
#define SKIN_ON_CLICK_CANCEL                   7
#define MSGBOX_STYLE_OK                        _T("3")
#define MSGBOX_STYLE_OK_CANCEL                 _T("4")
#define MSGBOX_STYLE_OK_WITH_LINK              _T("5")
#define MSGBOX_STYLE_OK_CANCEL_WITH_LINK       _T("6")
#define MSGBOX_STYLE_OK_CANCEL_WITH_2_LINK     _T("8")

// CModelessMsgBox 对话框

IMPLEMENT_DYNAMIC(CModelessMsgBox, CDialog)

CModelessMsgBox::CModelessMsgBox(CWnd* pParent /*=NULL*/)
	: CDialog(CModelessMsgBox::IDD, pParent)
{
	m_pSkinEngine = NULL;
	m_bDrawed = FALSE;
	m_strTitle = _T("提示");
	::GetSkinEngine(&m_pSkinEngine);
}

CModelessMsgBox::~CModelessMsgBox()
{
}

void CModelessMsgBox::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CModelessMsgBox, CDialog)
	ON_WM_CREATE()
	ON_SKIN_MESSAGE
	ON_WM_SIZE()
	ON_WM_DESTROY()
	ON_WM_CLOSE()
END_MESSAGE_MAP()

void CModelessMsgBox::OnClose()
{
	DestroyWindow();
}

void CModelessMsgBox::OnDestroy()
{
	GetParent()->EnableWindow(TRUE);
	CDialog::OnDestroy();
}

// CMsgBox 消息处理程序

int CModelessMsgBox::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  在此添加您专用的创建代码
	ASSERT(m_pSkinEngine != NULL);
	if(NULL != m_pSkinEngine)
	{
		HRESULT hr = m_pSkinEngine->LoadSkin(GetSafeHwnd(),L"MsgBox");
		ASSERT(hr==S_OK && __LINE__);
	}
    
	//标题字体首选微软雅黑
	std::wstring strFont;
	common::utility::systemhelper::GetDefaultFontName(strFont);
	CComVariant var = (CComVariant) strFont.c_str();
	m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(),L"textTitle",L"fontFamily",var);

	return 0;
}

LRESULT CModelessMsgBox::OnSkinMessage(WPARAM wParam,LPARAM lParam)
{
	switch(wParam)
	{
	   case SKIN_ON_CLICK_CLOSE:
	   case SKIN_ON_CLICK_CANCEL:
	   case SKIN_ON_CLICK_OK:
		   {
              OnClose();
		   }
		   break;

	   default:
		   break;
	}
	return TRUE;
}

void CModelessMsgBox::CreateAndShow(CWnd* pParent)
{	
	CDialog::Create(IDD, pParent);
	pParent->EnableWindow(FALSE);
	ShowWindow(SW_SHOW);
}

void CModelessMsgBox::Show(CWnd* pParent, LPCTSTR pzPrompt,MSGBOX_STYLE style,LPCTSTR pzTitle )
{
	m_strTitle = pzTitle;
	m_strPrompt = pzPrompt;
	m_Style = style;
	CreateAndShow(pParent);
}

void CModelessMsgBox::Show(CWnd* pParent, LPCTSTR psPrompt,LPCTSTR linkLabel,LPCTSTR linkAddress,MSGBOX_STYLE style,LPCTSTR pzTitle )
{
	m_strTitle = pzTitle;
    m_strPrompt = psPrompt;
	m_Style = style;
    m_strLinkLabel = linkLabel;
	m_strLinkAddress = linkAddress;
	CreateAndShow(pParent);
}

void CModelessMsgBox::Show(CWnd* pParent, LPCTSTR pzPrompt1,LPCTSTR linkLabel1,LPCTSTR linkAddress1,
		 LPCTSTR pzPrompt2,LPCTSTR linkLabel2,LPCTSTR linkAddress2,
		 MSGBOX_STYLE style,LPCTSTR pzTitle)
{
	m_strPrompt = pzPrompt1;
	m_strLinkLabel = linkLabel1;
	m_strLinkAddress = linkAddress1;
	m_strPrompt2 = pzPrompt2;
	m_strLinkLabel2 = linkLabel2;
	m_strLinkAddress2 = linkAddress2;
	m_Style = style;
	m_strTitle = pzTitle;
	CreateAndShow(pParent);
}

void CModelessMsgBox::OnSize(UINT nType, int cx, int cy)//因为是模态对话框，Domodal时才能获得句柄，
{                                               //所以在这里控制显示内容，并且控制次数
	CDialog::OnSize(nType, cx, cy);

	if(m_bDrawed)//不用每次都刷新
	{
		return;
	}
	
	CComVariant var = m_strPrompt.GetBuffer();
	CComVariant var2 = m_strTitle.GetBuffer();
	HWND hwnd = this->GetSafeHwnd();

	if (MsgBox::STYLE_OK == m_Style)
	{
		CComVariant varTemp = MSGBOX_STYLE_OK;
		m_pSkinEngine->SetSubSkinProp(hwnd,L"textMsgBoxStyle",L"text",varTemp);
		m_pSkinEngine->SetSubSkinProp(hwnd,L"textPrompt",L"text",var);
		m_pSkinEngine->SetSubSkinProp(hwnd,L"textTitle",L"text",var2);
		m_bDrawed = TRUE;
	}
	else if (MsgBox::STYLE_OK_CANCEL == m_Style)
	{
		CComVariant varTemp = MSGBOX_STYLE_OK_CANCEL;
		m_pSkinEngine->SetSubSkinProp(hwnd,L"textMsgBoxStyle",L"text",varTemp);
		m_pSkinEngine->SetSubSkinProp(hwnd,L"textPrompt",L"text",var);
		m_pSkinEngine->SetSubSkinProp(hwnd,L"textTitle",L"text",var2);
		m_bDrawed = TRUE;
	}
	else if (MsgBox::STYLE_OK_WITH_LINK == m_Style)
	{
		CComVariant varTemp = m_strLinkLabel.GetBuffer();
		
		m_pSkinEngine->SetSubSkinProp(hwnd,L"textPrompt",L"text",var);
		m_pSkinEngine->SetSubSkinProp(hwnd,L"textTitle",L"text",var2);
		m_pSkinEngine->SetSubSkinProp(hwnd,L"btnLink",L"label",varTemp);
		varTemp = m_strLinkAddress.GetBuffer();
		m_pSkinEngine->SetSubSkinProp(hwnd,L"btnLink",L"hyperLinkAddress",varTemp);
		varTemp = TRUE;
        m_pSkinEngine->SetSubSkinProp(hwnd,L"btnLink",L"visible",varTemp);
		m_bDrawed = TRUE;
		varTemp = MSGBOX_STYLE_OK_WITH_LINK;
		m_pSkinEngine->SetSubSkinProp(hwnd,L"textMsgBoxStyle",L"text",varTemp);
	}
	else if (MsgBox::STYLE_OK_CANCEL_WITH_LINK == m_Style)
	{
		CComVariant varTemp = MSGBOX_STYLE_OK_CANCEL_WITH_LINK;
		m_pSkinEngine->SetSubSkinProp(hwnd,L"textMsgBoxStyle",L"text",varTemp);

		m_pSkinEngine->SetSubSkinProp(hwnd,L"textPrompt",L"text",var);
		m_pSkinEngine->SetSubSkinProp(hwnd,L"textTitle",L"text",var2);

        varTemp = m_strLinkLabel.GetBuffer();
		m_pSkinEngine->SetSubSkinProp(hwnd,L"btnLink",L"label",varTemp);
		varTemp = m_strLinkAddress.GetBuffer();
		m_pSkinEngine->SetSubSkinProp(hwnd,L"btnLink",L"hyperLinkAddress",varTemp);
		varTemp = TRUE;
		m_pSkinEngine->SetSubSkinProp(hwnd,L"btnLink",L"visible",varTemp);
		m_bDrawed = TRUE;
	}
	else if (MsgBox::STYLE_OK_CANCEL_WITH_2_LINK == m_Style)
	{
		CComVariant varTemp = MSGBOX_STYLE_OK_CANCEL_WITH_2_LINK;
		m_pSkinEngine->SetSubSkinProp(hwnd,L"textMsgBoxStyle",L"text",varTemp);

		m_pSkinEngine->SetSubSkinProp(hwnd,L"textTitle",L"text",var2);

		m_pSkinEngine->SetSubSkinProp(hwnd,L"textPrompt1",L"text",var);
		varTemp = m_strPrompt2.GetBuffer();
		m_pSkinEngine->SetSubSkinProp(hwnd,L"textPrompt2",L"text",varTemp);

        varTemp = m_strLinkLabel.GetBuffer();
        m_pSkinEngine->SetSubSkinProp(hwnd,L"btnLinkMid",L"label",varTemp);
		varTemp = m_strLinkLabel2.GetBuffer();
		m_pSkinEngine->SetSubSkinProp(hwnd,L"btnLinkTail",L"label",varTemp);

        varTemp = m_strLinkAddress.GetBuffer();
        m_pSkinEngine->SetSubSkinProp(hwnd,L"btnLinkMid",L"hyperLinkAddress",varTemp);
		varTemp = m_strLinkAddress2.GetBuffer();
		m_pSkinEngine->SetSubSkinProp(hwnd,L"btnLinkTail",L"hyperLinkAddress",varTemp);
		m_bDrawed = TRUE;
	}
}