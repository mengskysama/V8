// GiftWordsDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "..\Room.h"
#include "GiftWordsDlg.h"
#include "skin_i.h"
#include "SkinUtil.h"
#include "..\..\CommonLibrary\include\utility\SystemHelper.h"
#include "..\..\CommonLibrary\include\utility\StringHelper.h"
#include "..\RoomParentDlg.h"
#include "..\SendEventManager.h"
#include "igiftitem.h"

#define  SKIN_ON_CLICK_BTN_SENDWORDS          1

#define  THE_MAX_WIDTH_OF_GIFTIMAGE_REGION    226
#define  THE_MAX_HEIGHT_OF_GIFTIMAGE_REGION   90
// CGiftWordsDlg 对话框

IMPLEMENT_DYNAMIC(CGiftWordsDlg, CDialog)

CGiftWordsDlg::CGiftWordsDlg( CRoomParentDlg* pParentDlg ,CWnd* pParent /*=NULL*/)
	: CDialog(CGiftWordsDlg::IDD, pParent),
	m_pParentDlg(pParentDlg)
{
	m_pSkinEngine = NULL;
	::GetSkinEngine(&m_pSkinEngine);
	m_pGiftItem = NULL ; 
	m_unUIN = m_unCount = 0 ;
}

CGiftWordsDlg::~CGiftWordsDlg()
{
}

void CGiftWordsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CGiftWordsDlg, CDialog)
	ON_WM_CREATE()
	ON_EN_CHANGE(IDC_ROOM_GIFTWORDS_INPUT, OnInputChange)
	ON_SKIN_MESSAGE
END_MESSAGE_MAP()


// CGiftWordsDlg 消息处理程序

int CGiftWordsDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;
	//Load Skin
	ASSERT(m_pSkinEngine != NULL);
	if (m_pSkinEngine != NULL){
		HRESULT result = m_pSkinEngine->LoadSkin(GetSafeHwnd(), L"GiftWordsDlg");
		ASSERT(result == S_OK && __LINE__);
	}

	return 0;
}

BOOL CGiftWordsDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_InputCtrl.Create( WS_VISIBLE | WS_CHILD /*| ES_MULTILINE | ES_WANTRETURN | WS_VSCROLL | ES_AUTOVSCROLL */, CRect(0,0,0,0), this, IDC_ROOM_GIFTWORDS_INPUT);
	m_InputCtrl.SetDefaultFont();
	m_InputCtrl.SetFontColor(RGB(95, 95, 95));
	m_InputCtrl.SetDefaultContent(L"默认赠言");
	//m_InputCtrl.SendMessage(EM_SETLANGOPTIONS, 0, 0);
	CComVariant var = (long)m_InputCtrl.GetSafeHwnd();
	m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(), L"itemEditGiftWords", L"hwnd", var);

	m_InputCtrl.SetEventMask(ENM_CHANGE);
	m_InputCtrl.LimitText(30);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

LRESULT CGiftWordsDlg::OnSkinMessage(WPARAM wParam, LPARAM lParam)
{
	switch(wParam)
	{
	case SKIN_ON_CLICK_BTN_SENDWORDS:
		{
			OnClickBtnSendWords();
		}
		break;
	default:break;
	}

	return S_OK;
}

BOOL CGiftWordsDlg::PreTranslateMessage(MSG* pMsg)
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

void CGiftWordsDlg::OnClickBtnSendWords()
{
	if ( m_pGiftItem != NULL )
	{
		CString strText;
		m_InputCtrl.GetWindowText(strText);
		strText.Replace(L"\r", L"");
		strText.Replace(L"\n", L"");
		CSendEventManager::SendNormalGiftEvent(m_pParentDlg->GetRoomID(), m_unUIN, m_pGiftItem->GetID(),
			m_unCount, m_pGiftItem->GetDstType(),strText.GetBuffer(), TRUE);
	}	
	ShowWindow( SW_HIDE ) ;
}

void CGiftWordsDlg::UpdateGiftWordsInfo( IGiftItem * pGiftItem , uint32 unI79ID , uint32 unUIN , std::wstring strUserName  , uint16 unCount )
{
	//////////////////  DragonZ: test  ////////////////////
	//礼物图片
	std::wstring strImagePath = pGiftItem->GetLargeImagePath();
	m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(), L"imgGiftItem", L"absoluteImage", (CComVariant)strImagePath.c_str() );
	CComVariant var1, var2;
	m_pSkinEngine->GetSubSkinProp(GetSafeHwnd(), L"imgGiftItem", L"originWidth", &var1);
	m_pSkinEngine->GetSubSkinProp(GetSafeHwnd(), L"imgGiftItem", L"originHeight", &var2);
	int nPosX = (THE_MAX_WIDTH_OF_GIFTIMAGE_REGION - var1.intVal)/2;
	int nPosY = 30 + (THE_MAX_HEIGHT_OF_GIFTIMAGE_REGION - var2.intVal)/2;

	m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(), L"imgGiftItem", L"left", (CComVariant)nPosX);
	m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(), L"imgGiftItem", L"top", (CComVariant)nPosY);

	m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(), L"imgGiftItem", L"width", var1);
	m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(), L"imgGiftItem", L"height", var2);
	//礼物信息
	var1 = strUserName.c_str() ;
	CString strTemp ;
	strTemp.Format( L"%d个" , unCount ) ;
	var2 = strTemp ;
	strTemp.Format( L"%u" , unI79ID ) ;
	CComVariant var = strTemp;
	m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(), L"textReceiveName", L"text", var1);
	m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(), L"textGiftCount", L"text", var2);
	m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(), L"textReceiveID", L"text", var);

	m_pGiftItem = pGiftItem ;
	m_unCount = unCount ; 
	m_unUIN = unUIN ;
	m_InputCtrl.SetWindowText( pGiftItem->GetStrGiftWords().c_str() ) ;
}


void CGiftWordsDlg::OnInputChange() 
{
	CString strText;
	m_InputCtrl.GetWindowText(strText);

	int nLen = common::utility::stringhelper::CalcStringAsciiLength(strText.GetBuffer(0));
	if (nLen >= 0 && nLen <= 30)
	{
		CString strPrompt;
		strPrompt.Format(L"您还可以输入%d个字符", 30 - nLen);
		CComVariant var = strPrompt;
		m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(), L"textToInput", L"text", var);
		m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(), L"btnSendWords", L"disabled", (CComVariant)FALSE);
	}
	else
	{
		CComVariant var = L"字数超出范围了";
		m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(), L"textToInput", L"text", var);
		m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(), L"btnSendWords", L"disabled", (CComVariant)TRUE);
	}

	CString strTemp = strText;
	strTemp.Trim();
}