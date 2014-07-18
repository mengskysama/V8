// SongWishDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "..\Room.h"
#include "SongWishDlg.h"
#include "SkinUtil.h"
#include "..\..\CommonLibrary\include\utility\StringHelper.h"
#include "..\..\CommonLibrary\include\utility\SystemHelper.h"
#include "..\screeneffect\FlyingInputRichEdit.h"
#include "..\SendEventManager.h"

#define SKIN_ON_CLICK_CLOSE        1
#define SKIN_ON_CLICK_OK           2

#define DEFAULT_SONGWISH_TEXT      L""

// CSongWishDlg 对话框

IMPLEMENT_DYNAMIC(CSongWishDlg, CDialog)

CSongWishDlg::CSongWishDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSongWishDlg::IDD, pParent)
{
	m_pSkinEngine = NULL;
	::GetSkinEngine(&m_pSkinEngine);
	
	m_unRoomID = 0;
	m_nSongerID = 0;
	m_strSongName = L"";
	m_nCount = 1;
}

CSongWishDlg::~CSongWishDlg()
{
}

void CSongWishDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CSongWishDlg, CDialog)
	ON_WM_CREATE()
	ON_SKIN_MESSAGE
END_MESSAGE_MAP()


// CSongWishDlg 消息处理程序

int CSongWishDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  在此添加您专用的创建代码
	ASSERT(NULL != m_pSkinEngine);
	if (NULL != m_pSkinEngine)
	{
		HRESULT hr = m_pSkinEngine->LoadSkin(GetSafeHwnd(),L"SongWishDlg");
		ASSERT(hr== S_OK && __LINE__);
	}

	return 0;
}

BOOL CSongWishDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	m_InputCtrl.Create( WS_VISIBLE | WS_CHILD | ES_MULTILINE | ES_WANTRETURN | WS_VSCROLL | ES_AUTOVSCROLL , CRect(0,0,0,0), this, IDC_RICHEDIT_WISH_INPUT);
	m_InputCtrl.SetDefaultFont();
	m_InputCtrl.SetDefaultContent(DEFAULT_SONGWISH_TEXT);
	//m_InputCtrl.SendMessage(EM_SETLANGOPTIONS, 0, 0);
	CComVariant var = (long)m_InputCtrl.GetSafeHwnd();
	m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(), L"itemSongWishInput", L"hwnd", var);

	//m_InputCtrl.SetEventMask(ENM_CHANGE);
	m_InputCtrl.LimitText(50);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

BOOL CSongWishDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	if ( pMsg->message == WM_KEYDOWN )
	{
		switch(pMsg->wParam)
		{
		case VK_ESCAPE:
		case VK_RETURN:
			return TRUE;

		default: 
			break;
		}
	}	

	return CDialog::PreTranslateMessage(pMsg);
}

LRESULT CSongWishDlg::OnSkinMessage( WPARAM wParam,LPARAM lParam )
{
	switch(wParam)
	{
	case SKIN_ON_CLICK_OK:
		{
			OnClickOK();
		}
		break;
	case SKIN_ON_CLICK_CLOSE:
		{
			m_InputCtrl.SetWindowText(L"");
			PostMessage(WM_CLOSE, 0, 0);
		}
		break;
	default:break;
	}

	return TRUE;
}

void CSongWishDlg::SetSongInfo(uint32 unRoomID, int nSongerID, CString strName, int nCount)
{
	if (m_pSkinEngine == NULL || strName == L"")
	{
		return;
	}
	CComVariant var = strName.GetBuffer(0);
	m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(), L"textSong", L"text", var);

	m_unRoomID = unRoomID;
	m_strSongName = strName;
	m_nSongerID = nSongerID;
	m_nCount = nCount;
}

CString CSongWishDlg::GetSongWish()
{
	CString strText;
	m_InputCtrl.GetWindowText(strText);
	strText.Replace(L"\r", L"");
	strText.Replace(L"\n", L"");

	return strText;
}

void CSongWishDlg::OnClickOK()
{
	CString strText;
	m_InputCtrl.GetWindowText(strText);
	strText.Replace(L"\r", L"");
	strText.Replace(L"\n", L"");
	strText.Replace(L"<", L"&lt;");
	strText.Replace(L">", L"&gt;");

	if (m_unRoomID != 0 && m_nSongerID != 0)
	{
		CSendEventManager::SendOrderSongEvent(m_unRoomID, m_nSongerID, m_strSongName.GetBuffer(0), strText.GetBuffer(0), m_nCount);
	}

	m_InputCtrl.SetWindowText(L"");
	PostMessage(WM_CLOSE, 0, 0);
}