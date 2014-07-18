// SearchDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SearchDlg.h"
#include "SearchEdit.h"
#include "..\Room.h"
#include "skin_i.h"
#include "SkinUtil.h"
#include "..\UserMessageDefine.h"

// CSearchDlg dialog
#define DEFAULT_TEXT_COLOR RGB(133, 133, 133)
#define TEXT_COLOR RGB(0, 0, 0)
#define DEFAULT_TEXT  L"输入用户昵称/白兔KTV号码"
#define SKIN_ON_CLICK_SEARCH 1



IMPLEMENT_DYNAMIC(CSearchDlg, CDialog)

CSearchDlg::CSearchDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSearchDlg::IDD, pParent)
	, m_strEdit(DEFAULT_TEXT)
{
	m_pSkinEngine = NULL;
	::GetSkinEngine(&m_pSkinEngine);
}


CSearchDlg::~CSearchDlg()
{

}

void CSearchDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_SEARCH, m_strEdit);
	DDX_Control(pDX, IDC_EDIT_SEARCH, m_editSearch);
}


BEGIN_MESSAGE_MAP(CSearchDlg, CDialog)
	ON_WM_SETFOCUS()
	ON_WM_CTLCOLOR()
	ON_WM_SIZE()
	ON_WM_CREATE()
	ON_SKIN_MESSAGE
	ON_EN_CHANGE(IDC_EDIT_SEARCH, &CSearchDlg::OnEnChangeEditSearch)
END_MESSAGE_MAP()



BOOL CSearchDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	
	return TRUE;  // return TRUE unless you set the focus to a control
}

void CSearchDlg::OnOK()
{
    OnSearchClick();
}

void CSearchDlg::OnCancel()
{

}

HBRUSH CSearchDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
    if (pWnd->GetSafeHwnd() == m_editSearch.GetSafeHwnd()) {
	    if (pDC && nCtlColor == CTLCOLOR_EDIT)
	    {
		    CString strText;
		    m_editSearch.GetWindowText(strText);
		    if (strText == DEFAULT_TEXT)
		    {
			    pDC->SetTextColor(DEFAULT_TEXT_COLOR);
		    }
		    else
		    {
			    pDC->SetTextColor(TEXT_COLOR);
		    }
	    }
        m_EditBrush.DeleteObject();
        m_EditBrush.CreateSolidBrush(RGB(255, 255, 255));
        return m_EditBrush;
    }

	return CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
}

void CSearchDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

}

int CSearchDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  在此添加您专用的创建代码
	//Load Skin
	ASSERT(m_pSkinEngine != NULL);
	if (m_pSkinEngine != NULL){
		HRESULT result = m_pSkinEngine->LoadSkin(GetSafeHwnd(), L"search_bar_room");
		ASSERT(result == S_OK && __LINE__);
	}

	return 0;
}

LRESULT CSearchDlg::OnSkinMessage(WPARAM wParam, LPARAM lParam)
{
	switch(wParam)
	{
	case SKIN_ON_CLICK_SEARCH:
		OnSearchClick();
		break;
	default:
		return FALSE;
	}
	return TRUE;
}

void CSearchDlg::OnSearchClick()
{
	m_editSearch.SendBySel();
}

void CSearchDlg::PostNcDestroy()
{
	//if (m_pList)
	//{
	//	delete m_pList;
	//	m_pList = NULL;
	//}
	CDialog::PostNcDestroy();
}

void CSearchDlg::OnMove(int x, int y)
{

}

void CSearchDlg::OnLogOff()
{
	m_editSearch.m_bFakeLoseFocus = TRUE;
	m_editSearch.SetWindowText(DEFAULT_TEXT);
	//m_editSearch.SetWindowText(L"");
}

void CSearchDlg::OnEnChangeEditSearch()
{
	// TODO:  如果该控件是 RICHEDIT 控件，则它将不会
	// 发送该通知，除非重写 CDialog::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
	CWnd * pParent = GetParent();
	if (pParent)
	{
		UpdateData(TRUE);
		pParent->PostMessage(WM_SEARCHEDIT_CHANGE, 0, (LPARAM)(LPCTSTR)m_strEdit);
	}
}
