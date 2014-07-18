#include <stdafx.h>
#include <afxwin.h>
#include "LoginComboBox.h"
#include "179TipCtrl.h"
#include "GGHelper.h"
#include "../CommonLibrary/include/ui/UIHelper.h"
#include "../CommonLibrary/include/utility/StringHelper.h"
#include "../CommonLibrary/include/utility/systemhelper.h"

#ifndef MAKEDWORD
#define MAKEDWORD(w1,w2) (((w1)<<16)|(w2))
#endif

/**********************************************************
*														  *
* 功能说明：输入账号的编辑框控件，它具有自动补全功能	  *
*														  *
**********************************************************/
IMPLEMENT_DYNAMIC(CLoginAccountEdit, CRBLimitTextEdit)

CLoginAccountEdit::CLoginAccountEdit()
{

}

CLoginAccountEdit::~CLoginAccountEdit()
{

}

BEGIN_MESSAGE_MAP(CLoginAccountEdit, CRBLimitTextEdit)
	//ON_CONTROL_REFLECT( ON_EN_UPDATE, OnEditUpdate)
	ON_WM_MOUSEMOVE()
	ON_MESSAGE(WM_MOUSEHOVER, OnMouseHover)
	ON_MESSAGE(WM_MOUSELEAVE, OnMouseLeave)
END_MESSAGE_MAP()


#define ACCOUNT_TEXT_COLOR RGB(0, 90, 128)
#define DEFAULT_ACCOUNT_TEXT_COLOR RGB(169,169,169)

BOOL 
CLoginAccountEdit::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg && pMsg->message == WM_CHAR)
	{
		if( ( pMsg->wParam == VK_SPACE) || ( pMsg->wParam == VK_TAB))
			return TRUE;
		else
		{
			CString strAccount ;
			GetWindowText(strAccount);

			if( strAccount == DEFAULT_ACCOUNT_TEXT)
			{
				SetWindowText(_T(""));
			}
		}
	}
	else if (pMsg->message == WM_KEYDOWN)
	{
		WPARAM wChar = pMsg->wParam;
		if (wChar == VK_DOWN)
		{
			CWnd* pParent = GetParent();
			if( pParent)
			{
				pParent->SendMessage( WM_COMBOBOX_BUTTON_CLICK, 0,0);
				return TRUE;
			}
		}
	}

	return CEdit::PreTranslateMessage(pMsg);
}

void
CLoginAccountEdit::OnEditUpdate()
{

}

void CLoginAccountEdit::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	TRACKMOUSEEVENT tme;
	tme.cbSize = sizeof(tme);
	tme.hwndTrack = m_hWnd;
	tme.dwFlags = TME_LEAVE | TME_HOVER;
	tme.dwHoverTime = 1;
	_TrackMouseEvent(&tme);

	CWnd::OnMouseMove(nFlags, point);
}

HRESULT CLoginAccountEdit::OnMouseHover(WPARAM w, LPARAM l)
{
	CWnd* pWnd = GetParent();
	pWnd->PostMessage(ACCOUNT_EDIT_MOUSE_HOVER,NULL,NULL);

	return 0;
}

HRESULT CLoginAccountEdit::OnMouseLeave(WPARAM w, LPARAM l)
{
	CWnd* pWnd = GetParent();
	pWnd->PostMessage(ACCOUNT_EDIT_MOUSE_LEAVE,NULL,NULL);
	return 0;
}


/**********************************************************
*														  *
* 功能说明：账号登陆的ComboBox							  *
*														  *
**********************************************************/
IMPLEMENT_DYNAMIC(CLoginComboBox, CWnd)

CLoginComboBox::CLoginComboBox()
{
	m_bEditChangeRecursive = FALSE;
	m_bAutoComplete	= TRUE;

	m_pTipCtrl = NULL;
}

CLoginComboBox::~CLoginComboBox()
{

}


BEGIN_MESSAGE_MAP(CLoginComboBox, CWnd)
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_COMBOBOX_BUTTON, OnButtonClick)
	ON_EN_CHANGE( IDC_COMBOBOX_ACCOUNT, OnAccountEditChange)
	ON_EN_KILLFOCUS( IDC_COMBOBOX_ACCOUNT, OnAccountEditKillFocus)
	ON_EN_SETFOCUS( IDC_COMBOBOX_ACCOUNT, OnAccountEditSetFocus)


	ON_MESSAGE( WM_ITEM_CLICK, OnItemClick)
	ON_MESSAGE( WM_ITEM_DELETE, OnDeleteAccount)
	ON_MESSAGE( WM_COMBOBOX_BUTTON_CLICK, OnComboBoxClick)
	ON_MESSAGE( WM_LISTCTRL_SHOWHIDE,OnLoginListCtrlShowHide)
	ON_WM_SIZE()
	ON_WM_SETFOCUS()
	ON_WM_KILLFOCUS()

	ON_WM_MOUSEMOVE()
	ON_MESSAGE(WM_MOUSEHOVER, OnMouseHover)
	ON_MESSAGE(WM_MOUSELEAVE, OnMouseLeave)
	ON_MESSAGE(ACCOUNT_EDIT_MOUSE_HOVER,&CLoginComboBox::OnMouseHover)
	ON_MESSAGE(ACCOUNT_EDIT_MOUSE_LEAVE,&CLoginComboBox::OnMouseLeave)

END_MESSAGE_MAP()

BOOL	
CLoginComboBox::Create(const CRect& rc, CWnd* pParentWnd, UINT uId)
{
	BOOL bRet = __super::Create(AfxRegisterWndClass(CS_DBLCLKS, LoadCursor(NULL, IDC_IBEAM)), 
		_T(""), WS_CHILD | WS_VISIBLE | WS_TABSTOP, rc, pParentWnd, uId);
	ShowWindow( SW_SHOW);

	common::ui::CClientRect rcClient(this);
	rcClient.right -= 21;
	m_InnerEdit.Create(  WS_VISIBLE|ES_AUTOHSCROLL, rcClient, this, IDC_COMBOBOX_ACCOUNT);
	m_InnerEdit.ShowWindow( SW_SHOW);

	LOGFONT lf = {0};
	lf.lfHeight = 14;
	lf.lfWeight = 400;
	wcscpy(lf.lfFaceName ,L"tahoma");
	m_EditFont.CreateFontIndirect(&lf);
	m_InnerEdit.SetFont(&m_EditFont);

	common::ui::CClientRect rcButton(this);
	rcButton.left	=	rcButton.right - 21;
	rcButton.right	=	rcButton.left + 21;
	rcButton.bottom = rcButton.top+rcButton.Width();
	m_InnerButton.Create(L"", WS_CHILD, rcButton, this, IDC_COMBOBOX_BUTTON);
	m_InnerButton.ShowWindow( SW_SHOW);

	m_InnerList.Create( CRect(100,50,300,50), this, IDC_COMBOBOX_LIST);
	m_InnerList.SetAssoicatedWnd( this);


	SetTextColor(ACCOUNT_TEXT_COLOR);

	return bRet;
}

BOOL 
CLoginComboBox::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_KEYDOWN)
	{
		m_bAutoComplete = TRUE;

		int nVirtKey = (int) pMsg->wParam;
		if (nVirtKey == VK_DELETE || nVirtKey == VK_BACK || nVirtKey == VK_SPACE)
			m_bAutoComplete = FALSE;

		if(nVirtKey == VK_SPACE)
		{
			KeyDownSpace();
		}
	}

	if (pMsg->wParam == VK_CAPITAL)
	{
		CRect rect;
		GetWindowRect(rect);
		CPoint pt = CPoint(rect.left,rect.top);
		SHORT keystate = GetKeyState(VK_CAPITAL);
		//以下由DragonZ注释
		/*if((keystate & 0x0001) !=0 )
		{
		m_ctrlTip.SetHideDelay( RB_INFINITE);
		m_ctrlTip.Show( L"大写锁定已经打开", this, &pt);
		}
		else
		{
		m_ctrlTip.Hide();
		}*/

		return TRUE;
	}


	return CWnd::PreTranslateMessage(pMsg);
}


void	
CLoginComboBox::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	__super::OnCreate(lpCreateStruct);
}

void
CLoginComboBox::OnSetFocus(CWnd* pOldWnd)
{
	m_InnerEdit.SetFocus();

	CRect rect;
	GetWindowRect(rect);
	CPoint pt = CPoint(rect.left,rect.top);
	SHORT keystate = GetKeyState(VK_CAPITAL);
	//以下由DragonZ注释
	/*if((keystate & 0x0001)!=0)
	{
	m_ctrlTip.SetHideDelay( RB_INFINITE);
	m_ctrlTip.Show( L"大写锁定已经打开", this, &pt);
	}*/

}

void 
CLoginComboBox::OnPaint()
{
	CPaintDC dc(this);

	// 边框

	common::ui::CClientRect rcClient( this);


	CBrush backBrush(RGB(255,255,255));
	FillRect(dc.GetSafeHdc(), &rcClient, (HBRUSH)backBrush.GetSafeHandle());
}

/**
* @function		增加一个新的Item
* @param		nUin		Item对应的uin
* @param		strShowAccount	对应的账号
*/
BOOL	
CLoginComboBox::AddItem( UINT32	nUin,  CString strShowAccount, CString strNickName,  
						common::ui::CImageImpl* pImageWrapper)
{
	return m_InnerList.AddItem( nUin, strShowAccount, strNickName, pImageWrapper);
}

/**
* @function		删除一个给定用户的Item
* @param		nUin	被删除用户的uin
*/
void	
CLoginComboBox::DeleteItem( UINT32	nUin)
{
	m_InnerList.DeleteItem( nUin);
}

void
CLoginComboBox::DeleteItem( CString strShowAccount)
{
	m_InnerList.DeleteItem( strShowAccount);
}

void	
CLoginComboBox::DeleteAllItem()
{
	m_InnerList.DeleteAllItem();
}

/**
* @function		获取指定索引处的UIN
* @param		nIndex	Item的索引项
*/
uin		
CLoginComboBox::GetUinAtIndex( UINT nIndex)
{
	return m_InnerList.GetUinAtIndex( nIndex);
}

HRESULT	
CLoginComboBox::OnComboBoxClick(WPARAM w, LPARAM l)
{
	OnButtonClick();

	return 0;
}

void	
CLoginComboBox::OnButtonClick()
{
	if( m_InnerList.m_bHide == FALSE)
	{
		m_InnerList.Hide();
		m_InnerList.m_bHide	=	TRUE;
	}
	else
	{

		common::ui::CClientRect rcClient(this);
		ClientToScreen(&rcClient);
		CPoint pt(rcClient.left - 2, rcClient.bottom + 3);

		m_InnerList.Show(pt);
		CString strAccount;
		GetWindowText(strAccount);			
		m_InnerList.SetCurSel(Find(strAccount));
		m_InnerList.m_bHide	=	FALSE;

		SendMessage(WM_LISTCTRL_SHOWHIDE,0,0);	


	}
}

void
CLoginComboBox::OnAccountEditChange()
{
	if (!m_bAutoComplete) 
	{
		CWnd* pParent = GetParent();
		if( pParent)
		{
			pParent->SendMessage( WM_COMBOBOX_EDIT_CHANGE, 0, 0);
		}
		return;
	}

	CString strAccount;
	m_InnerEdit.GetWindowText(strAccount);
	CString strTrimAccount = strAccount;
	strTrimAccount.Trim();
	if( strTrimAccount != strAccount)
		m_InnerEdit.SetWindowText( strTrimAccount);

	int nIndex = m_InnerList.SelectAccount(0, strTrimAccount);
	if( nIndex != - 1)
	{
		CString strShowString = L"";

		if( common::utility::systemhelper::IsDigit( strAccount.GetBuffer(0)))
		{
			strShowString = m_InnerList.GetShowUinAtIndex( nIndex).c_str();
		}
		else
		{
			strShowString = m_InnerList.GetAccountAtIndex( nIndex).c_str();
		}

		if( strShowString != L"" && !m_bEditChangeRecursive)
		{
			m_bEditChangeRecursive = TRUE;
			m_InnerEdit.SetWindowText( strShowString);
			m_bEditChangeRecursive = FALSE;
			m_InnerEdit.SetSel( strAccount.GetLength() , -1);
		}
	}

	CWnd* pParent = GetParent();
	if( pParent)
	{
		pParent->SendMessage( WM_COMBOBOX_EDIT_CHANGE, 0, 0);
	}
}

void
CLoginComboBox::OnAccountEditKillFocus()
{
	CString strAccount;
	GetWindowText(strAccount);

	if( strAccount == _T(""))
	{		
		SetWindowText(DEFAULT_ACCOUNT_TEXT);
	}

	CWnd* pParent = GetParent();
	if( pParent)
	{
		pParent->SendMessage( WM_COMBOBOX_LOSE_FOCUS, 0, 0);
	}
}

void
CLoginComboBox::OnAccountEditSetFocus()
{
	CString strAccount;
	GetWindowText(strAccount);

	if( strAccount == DEFAULT_ACCOUNT_TEXT)
	{		
		SetWindowText(_T(""));
	}

	m_InnerEdit.PostMessage(EM_SETSEL, 0, -1);
	CWnd* pParent = GetParent();
	if( pParent)
	{
		pParent->SendMessage( WM_COMBOBOX_SET_FOCUS, 0, 0);
	}

}

HRESULT
CLoginComboBox::OnLoginListCtrlShowHide( WPARAM w, LPARAM l)
{
	CString strAccount;
	GetWindowText(strAccount);

	if( strAccount == _T("") && m_InnerList.m_bHide == TRUE)
	{	
		SetTextColor(DEFAULT_ACCOUNT_TEXT_COLOR);
		m_InnerEdit.SetWindowText(DEFAULT_ACCOUNT_TEXT);
	}
	else if(strAccount == DEFAULT_ACCOUNT_TEXT && m_InnerList.m_bHide == FALSE)
	{
		SetTextColor(ACCOUNT_TEXT_COLOR);
		m_InnerEdit.SetWindowText(_T(""));
	}
	return 0;
}

HRESULT	
CLoginComboBox::OnDeleteAccount( WPARAM w, LPARAM l)
{
	CWnd* pParent = GetParent();
	if( pParent)
	{
		pParent->SendMessage( WM_ITEM_DELETE, w, l);
	}

	return 0;
}

LRESULT
CLoginComboBox::OnItemClick(WPARAM w, LPARAM l)
{
	UINT nIndex = (UINT)w;

	wstring wstrShowAccount = m_InnerList.GetShowUinAtIndex( nIndex);
	wstring wstrAccount = m_InnerList.GetAccountAtIndex( nIndex);
	uint32 unUin = m_InnerList.GetUinAtIndex( nIndex);

	TRACE(L"%s\n", wstrShowAccount.c_str());
	TRACE(L"%s\n", wstrAccount.c_str());

	SetTextColor(ACCOUNT_TEXT_COLOR);
	if( wstrShowAccount != L"")
	{
		m_InnerEdit.SetWindowText( wstrShowAccount.c_str());
	}
	else if( wstrAccount != L"")
	{
		m_InnerEdit.SetWindowText( wstrAccount.c_str());
	}

	m_InnerEdit.Invalidate();
	CWnd* pParent = GetParent();
	if( pParent)
	{
		pParent->SendMessage( WM_COMBOBOX_SEL_CHANGE, unUin, 0);
	}

	m_InnerList.Hide();

	BOOL bRet = ::ReleaseCapture();
	return 0;
}

void
CLoginComboBox::OnSize(UINT nType, int cx, int cy)
{
	common::ui::CClientRect rcClient(this);
	rcClient.right -= 21;
	rcClient.top += 2;
	rcClient.left += 4;
	if( ::IsWindow(m_InnerEdit.GetSafeHwnd()))
		m_InnerEdit.MoveWindow( &rcClient);

	common::ui::CClientRect rcButton(this);
	rcButton.left	=	rcButton.right - 21;
	rcButton.right	=	rcButton.left	+ 21;
	rcButton.bottom = rcButton.top+rcButton.Width();
	if( ::IsWindow(m_InnerButton.GetSafeHwnd()))
		m_InnerButton.MoveWindow( &rcButton);

	if( ::IsWindow( m_InnerList.GetSafeHwnd()))
	{
		common::ui::CClientRect rcClient(this);

		CRect rcList;
		m_InnerList.GetClientRect(&rcList);
		common::ui::SetRectWidth(&rcList, rcClient.Width() + 4);
		m_InnerList.MoveWindow(&rcList);
	}
}

void
CLoginComboBox::Clear()
{
	m_InnerEdit.SetFocus();
	m_InnerEdit.SetSel(0,-1);
	m_InnerEdit.Clear();
}

int		
CLoginComboBox::GetCount() const
{
	return m_InnerList.GetCount();
}

int		
CLoginComboBox::GetCurSel()	const
{
	return m_InnerList.GetCurSel();
}

void	
CLoginComboBox::SetWindowText(LPCTSTR lpszString)
{
	if(lpszString == _T("") || lpszString == DEFAULT_ACCOUNT_TEXT)
	{
		CWnd* pFocus = GetFocus();		
		if(m_InnerEdit.GetSafeHwnd() == pFocus->GetSafeHwnd() ||  m_InnerButton.GetSafeHwnd() == pFocus->GetSafeHwnd() )  //当前焦点在登录编辑框或下拉按扭上时
		{
			SetTextColor(ACCOUNT_TEXT_COLOR);
			m_InnerEdit.SetWindowText(_T(""));
		}
		else
		{
			SetTextColor(DEFAULT_ACCOUNT_TEXT_COLOR);
			m_InnerEdit.SetWindowText(DEFAULT_ACCOUNT_TEXT);
		}

	}
	else
	{
		SetTextColor(ACCOUNT_TEXT_COLOR);
		m_InnerEdit.SetWindowText( lpszString);

	}


}

void	
CLoginComboBox::GetWindowText(CString& rString)
{
	m_InnerEdit.GetWindowText( rString);
}

void
CLoginComboBox::SetFocus( )
{
	m_InnerEdit.SetFocus();
}

DWORD	
CLoginComboBox::GetEditSel() const
{
	int nStart, nEnd;
	m_InnerEdit.GetSel( nStart, nEnd);

	return MAKEDWORD( nStart, nEnd);
}

void	
CLoginComboBox::SetEditSel( int nStartChar, int nEndChar)
{
	m_InnerEdit.SetSel( nStartChar, nEndChar);
}

int		
CLoginComboBox::GetLBUinText( int nIndex, CString&	rString) 
{
	uin nUin = m_InnerList.GetUinAtIndex( nIndex);
	if( nUin == 0)
		return CB_ERR;

	//rString = wstring_helper::from_int( nUin).c_str();
	rString = common::utility::stringhelper::IntToString(nUin).c_str();
	return rString.GetLength();
}

int		
CLoginComboBox::GetLBAccountText( int nIndex, CString&	rString) 
{
	rString = m_InnerList.GetAccountAtIndex( nIndex).c_str();
	return rString.GetLength();
}

BOOL	
CLoginComboBox::LimitText( int nMaxChars)
{
	m_InnerEdit.SetLimit( nMaxChars);
	return TRUE;
}

int		
CLoginComboBox::SetCurSel( int nSelect)
{
	if( nSelect == -1)
	{
		m_InnerEdit.SetFocus();
		m_InnerEdit.SetSel(0, -1);
		m_InnerEdit.Clear();

		return CB_ERR;
	}

	wstring wstrAccount = m_InnerList.GetAccountAtIndex( nSelect);
	if( wstrAccount == L"")
		return CB_ERR;

	m_InnerEdit.SetWindowText( wstrAccount.c_str());	
	m_InnerList.SetCurSel(nSelect);
	return nSelect;
}


int		
CLoginComboBox::Find( uin nUin)
{
	return m_InnerList.Find( nUin);
}

int
CLoginComboBox::Find( CString strShowAccount)
{
	return m_InnerList.Find( strShowAccount);
}

int		
CLoginComboBox::SelectAccount( int nStartAfter, LPCTSTR	lpszString)
{
	return m_InnerList.SelectAccount( nStartAfter, lpszString);
}

int		
CLoginComboBox::SelectUin( int nStartAfter, uin nUin)
{
	return m_InnerList.SelectUin( nStartAfter, nUin);
}

void	
CLoginComboBox::SetTextColor( COLORREF textColor)
{
	m_InnerEdit.SetTextColor( textColor);
}

void CLoginComboBox::OnKillFocus(CWnd* pNewWnd)
{
	CWnd::OnKillFocus(pNewWnd);

	// TODO: 在此处添加消息处理程序代码
	//以下由DragonZ注释
	/*m_ctrlTip.Hide();*/
}


void CLoginComboBox::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	TRACKMOUSEEVENT tme;
	tme.cbSize = sizeof(tme);
	tme.hwndTrack = m_hWnd;
	tme.dwFlags = TME_LEAVE | TME_HOVER;
	tme.dwHoverTime = 1;
	_TrackMouseEvent(&tme);

	CWnd::OnMouseMove(nFlags, point);
}

HRESULT CLoginComboBox::OnMouseHover(WPARAM w, LPARAM l)
{
	CWnd* pWnd = GetParent();
	pWnd->PostMessage(ACCOUNT_CTRL_MOUSE_HOVER,NULL,NULL);


	return 0;
}

HRESULT CLoginComboBox::OnMouseLeave(WPARAM w, LPARAM l)
{

	CWnd* pWnd = GetParent();
	pWnd->PostMessage(ACCOUNT_CTRL_MOUSE_LEAVE,NULL,NULL);


	return 0;
}

void CLoginComboBox::OnLButtonDown(UINT nFlags, CPoint point)
{		


}
void CLoginComboBox::KeyDownSpace()
{
	int nStart,nEnd;
	CString strAccount;
	m_InnerEdit.GetSel(nStart,nEnd);
	m_InnerEdit.GetWindowText(strAccount);
	if(nStart ==0 && nEnd == strAccount.GetLength() )
	{
		m_InnerEdit.SetWindowText(_T(""));
	}

}