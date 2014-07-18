// FlyingTextInputDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "..\Room.h"
#include "SkinUtil.h"
#include "FlyingTextInputDlg.h"
#include "..\SendEventManager.h"
#include "..\gift/GiftManager.h"
#include "..\RoomParentDlg.h"
#include "..\..\CommonLibrary\include\utility\StringHelper.h"
#include "..\..\CommonLibrary\include\utility\SystemHelper.h"
#include "FlyingInputRichEdit.h"

#define SKIN_ON_CLICK_CLOSE 1 
#define SKIN_ON_CLICK_SEND  2
#define SKIN_ON_CLICK_BTN_EFFECT1 3
#define SKIN_ON_CLICK_BTN_EFFECT2 4
#define SKIN_ON_CLICK_BTN_EFFECT3 5
#define SKIN_ON_CLICK_BTN_EFFECT4 6
#define SKIN_ON_CLICK_BTN_RANDOM 7
#define SKIN_NOTIFY_EFFECT_CHANGE 8

#define DEFAULT_TIP_TEXT L"10000币/条，炫目飞屏，给您无尽惊喜的聊天特效"


// CFlyingTextInputDlg 对话框

IMPLEMENT_DYNAMIC(CFlyingTextInputDlg, CDialog)

CFlyingTextInputDlg::CFlyingTextInputDlg(CRoomParentDlg * pParentDlg, CWnd* pParent /*=NULL*/)
	: CDialog(CFlyingTextInputDlg::IDD, pParent),
	m_pParentDlg(pParentDlg)
{
	m_pSkinEngine = NULL;
	m_nType = 0;
	::GetSkinEngine(&m_pSkinEngine);
}

CFlyingTextInputDlg::~CFlyingTextInputDlg()
{
}

void CFlyingTextInputDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CFlyingTextInputDlg, CDialog)
	ON_WM_CREATE()
	ON_EN_CHANGE(IDC_FLYING_TEXT_INPUT, OnInputChange)
	ON_SKIN_MESSAGE
	ON_WM_SHOWWINDOW()
END_MESSAGE_MAP()


// CFlyingTextInputDlg 消息处理程序

int CFlyingTextInputDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  在此添加您专用的创建代码
	ASSERT(NULL != m_pSkinEngine);
	if (NULL != m_pSkinEngine)
	{
		HRESULT hr = m_pSkinEngine->LoadSkin(GetSafeHwnd(),L"FlyingTextInputDlg");
		ASSERT(hr== S_OK && __LINE__);
	}

	return 0;
}

BOOL CFlyingTextInputDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	m_InputCtrl.Create( WS_VISIBLE | WS_CHILD | ES_MULTILINE | ES_WANTRETURN | WS_VSCROLL | ES_AUTOVSCROLL , CRect(0,0,0,0), this, IDC_FLYING_TEXT_INPUT);
	m_InputCtrl.SetDefaultFont();
	m_InputCtrl.SetDefaultContent(DEFAULT_TIP_TEXT);
	m_InputCtrl.SendMessage(EM_SETLANGOPTIONS, 0, 0);
	CComVariant var = (long)m_InputCtrl.GetSafeHwnd();
	m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(), L"itemFlyingTextInput", L"hwnd", var);

	m_InputCtrl.SetEventMask(ENM_CHANGE);
	m_InputCtrl.LimitText(50);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CFlyingTextInputDlg::OnInputChange()
{
	CString strText;
	m_InputCtrl.GetWindowText(strText);

	if (strText == DEFAULT_TIP_TEXT)
	{
		m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(), L"op_send", L"disabled", (CComVariant)TRUE);
		return;
	}
	//strText.Trim();

	int nLen = common::utility::stringhelper::CalcStringAsciiLength(strText.GetBuffer(0));
	if (nLen >= 0 && nLen <= 50)
	{
		CString strPrompt;
		strPrompt.Format(L"您还可以输入%d个字符", 50 - nLen);
		CComVariant var = strPrompt;
		m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(), L"textPrompt", L"text", var);
		m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(), L"op_send", L"disabled", (CComVariant)FALSE);
	}
	else
	{
		CComVariant var = L"字数超出范围了";
		m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(), L"textPrompt", L"text", var);
		m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(), L"op_send", L"disabled", (CComVariant)TRUE);
	}

	CString strTemp = strText;
	strTemp.Trim();
	if (common::utility::stringhelper::CalcStringAsciiLength(strTemp.GetBuffer(0)) == 0)
	{
		m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(), L"op_send", L"disabled", (CComVariant)TRUE);
	}
}

LRESULT CFlyingTextInputDlg::OnSkinMessage( WPARAM wParam,LPARAM lParam )
{
	switch(wParam)
	{
	case SKIN_ON_CLICK_CLOSE:
		{
			//DestroyWindow();
			ShowWindow(SW_HIDE);
		}
		break;
	case SKIN_ON_CLICK_SEND:
		{
			OnClickSend();
		}
		break;
	case SKIN_ON_CLICK_BTN_EFFECT1:
		{
			if (1 == (int)lParam)
			{
				m_nType = 1;
			}
			else
			{
				m_nType = 0;
			}
		}
		break;
	case SKIN_ON_CLICK_BTN_EFFECT2:
		{
			if (1 == (int)lParam)
			{
				m_nType = 2;
			}
			else
			{
				m_nType = 0;
			}
		}
		break;
	case SKIN_ON_CLICK_BTN_EFFECT3:
		{
			if (1 == (int)lParam)
			{
				m_nType = 3;
			}
			else
			{
				m_nType = 0;
			}
		}
		break;
	case SKIN_ON_CLICK_BTN_EFFECT4:
		{
			if (1 == (int)lParam)
			{
				m_nType = 4;
			}
			else
			{
				m_nType = 0;
			}
		}
		break;
	case SKIN_ON_CLICK_BTN_RANDOM:
		{
			m_nType = 0;
		}
		break;
	default:break;
	}

	m_pParentDlg->SetFlyTextEffect(m_nType);

	return TRUE;
}

void CFlyingTextInputDlg::OnClickSend()
{
	CString strText;
	m_InputCtrl.GetWindowText(strText);
	strText.Replace(L"\r", L"");
	strText.Replace(L"\n", L"");
	int nType;
	if (m_nType == 0)
	{
		nType = rand() % 4 + 1;
	}
	else
	{
		nType = m_nType;
	}

	std::wstring strContent = strText.GetBuffer(0);
	common::utility::systemhelper::FilterWords(strContent);
	ScreenEffectManager::EncodeFlyingText(strContent, nType);

	IGiftItem * pGiftItem = GiftManager::GetInstance()->GetFlyingWordsItem();
	if (pGiftItem != NULL)
	{
		CSendEventManager::SendFlyingWords(m_pParentDlg->GetRoomID(), pGiftItem->GetID(), strContent);
	}

	ShowWindow(SW_HIDE);
}

int CFlyingTextInputDlg::GetType()
{
	return m_nType;
}

void CFlyingTextInputDlg::SetType( int nType )
{
	m_nType = nType;
	NOTIFY_SKIN_HELPER(SKIN_NOTIFY_EFFECT_CHANGE, (LONG)nType);
}
void CFlyingTextInputDlg::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialog::OnShowWindow(bShow, nStatus);

	if (bShow)
	{
		SetFocus();
	}
	else
	{
		m_InputCtrl.SetWindowText(L"");
		m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(), L"op_send", L"disabled", (CComVariant)TRUE);
		m_InputCtrl.SetDefaultContent(DEFAULT_TIP_TEXT);
	}
}
