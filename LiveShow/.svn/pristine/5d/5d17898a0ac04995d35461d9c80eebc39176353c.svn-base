// PasswordCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "../../include/ui/RBPasswordCtrl.h"
#include <Imm.h>
#include <string>
#include "../CommonLibrary/include/utility/Base64Coding.h"
#include "utility/stringhelper.h"
#include "utility/Md5.h"
#include "utility/SystemHelper.h"
#include "Log.h"

EXTERN_LOG()

using namespace std;
#pragma comment(lib, "imm32.lib")

// 禁用IME
enum _IME_PREVENT
{
	IME_PREVENT_TIMER_ID = 0,
	IME_PREVENT_TIMER_INTERVAL = 50,
};

IMPLEMENT_DYNAMIC(CRBPasswordCtrl, CWnd)
CRBPasswordCtrl::CRBPasswordCtrl()
{
	m_nCaretPos			= 0;
	m_bCaretCreated		= FALSE;
	m_nBeginPos			= 0;
	m_strPasswordText	= L"";
	m_strShowText		= L"";
	m_bFirstClick		= TRUE;
	m_nLimitText		= 0;
	m_bRememberPassword	= FALSE;	//	默认不是记住密码的
	m_strPasswordMD5Text= L"";

	m_bFirstFocus		= TRUE;
	m_bShowDefaultText = TRUE;
}

CRBPasswordCtrl::~CRBPasswordCtrl()
{
	if (m_font.GetSafeHandle())
	{
		DeleteObject(m_font.GetSafeHandle());
	}
	if (m_defaultTextFont.GetSafeHandle())
	{
		DeleteObject(m_defaultTextFont.GetSafeHandle());
	}
}


BEGIN_MESSAGE_MAP(CRBPasswordCtrl, CWnd)
	ON_WM_SETFOCUS()
	ON_WM_ERASEBKGND()
	ON_WM_LBUTTONDOWN()
	ON_WM_PAINT()
	ON_WM_KILLFOCUS()
	ON_WM_TIMER()
	ON_WM_DESTROY()
	ON_WM_MOUSEMOVE()
	ON_MESSAGE(WM_MOUSEHOVER, OnMouseHover)
	ON_MESSAGE(WM_MOUSELEAVE, OnMouseLeave)
END_MESSAGE_MAP()


BOOL 
CRBPasswordCtrl::Create(const CRect& rc, CWnd* pParentWnd, UINT uId, DWORD dwStyle/* = WS_CHILD | WS_VISIBLE | WS_BORDER | WS_TABSTOP*/)
{
	BOOL bRet = __super::Create(AfxRegisterWndClass(CS_DBLCLKS, LoadCursor(NULL, IDC_IBEAM)), 
		_T(""), dwStyle, rc, pParentWnd, uId);

	m_rectCtrl = rc;
	m_bCaretCreated = ::CreateCaret(GetSafeHwnd(), NULL, 1, 14);

	SetTimer(IME_PREVENT_TIMER_ID, IME_PREVENT_TIMER_INTERVAL, NULL);
	return bRet;
}

inline BOOL IsKeyDown(int nVirtKey)
{
	return GetKeyState(nVirtKey) < 0;
}

void CRBPasswordCtrl::OnDestroy()
{
	CWnd::OnDestroy();

	KillTimer(IME_PREVENT_TIMER_ID);
}

wstring 
CRBPasswordCtrl::GetPlainPassword()
{	wstring strPassword = m_strPasswordText;
	if( m_strPasswordText != L"")
		strPassword = common::CBase64Coding::Decode( m_strPasswordText);

	return strPassword;
}

wstring 
CRBPasswordCtrl::GetPassword()
{
	// 如果是记住密码，则直接返回传入的MD5值
	if( m_bRememberPassword)
		return m_strPasswordMD5Text;

	if( GetPlainPassword() == L"")
		return L"";

	std::string strPlainPassword = common::utility::stringhelper::UnicodeToAscii(GetPlainPassword());
	std::string strMd5 = common::utility::md5::GetStringMd5(strPlainPassword);
	return common::utility::stringhelper::AsciiToUnicode(strMd5);
}

// 设置的密码都是MD5
void 
CRBPasswordCtrl::SetPassword( wstring sPassword)
{
	if( sPassword == L"")
	{
		m_bRememberPassword = FALSE;
		m_strPasswordMD5Text = sPassword;

		m_strPasswordText = L"";
		m_strShowText = L"";
		m_nCaretPos = wcslen(L"");

		if (GetFocus() == this)
		{
			m_bShowDefaultText = FALSE;
		}else
		{
			m_bShowDefaultText = TRUE;
		}
	}
	else
	{
		m_bRememberPassword = TRUE;
		m_strPasswordMD5Text = sPassword;

		m_strPasswordText = common::CBase64Coding::Encode(L"●●●●●●●●●●");
		m_strShowText = L"●●●●●●●●●●";
		m_nCaretPos = wcslen(L"●●●●●●●●●●");

		m_bShowDefaultText = FALSE;
	}
	

	if (IsWindow(GetSafeHwnd()))
	{
		Invalidate();
		OnProcessEnd();
	}
}

void	
CRBPasswordCtrl::ClearPassword()
{
	m_strPasswordText = common::CBase64Coding::Encode(L"");
	m_strPasswordMD5Text = L"";
	m_strShowText = L"";
	m_nCaretPos = 0;

	if (IsWindow(GetSafeHwnd()))
	{
		if (GetFocus() != this)
		{
			m_bShowDefaultText = TRUE;
		}
		Invalidate();
		OnProcessEnd();
	}
}

void	
CRBPasswordCtrl::LimitText(LONG uNum)
{
	m_nLimitText = uNum;
}

UINT 
CRBPasswordCtrl::GetCharWidthInfo( wstring strText )
{
	if ( strText == L"")
		return 0;

	CDC* pDC = GetDC();
	if( pDC == NULL)
		return FALSE;

	UINT nWidth = 0;

	CFont* pOldFont = pDC->SelectObject(&m_font);
	CSize szRet;
	if (GetTextExtentPoint32(pDC->GetSafeHdc(), strText.c_str(), (int)strText.length(), &szRet))
		nWidth = szRet.cx;
	else
		nWidth = 0;

	pDC->SelectObject(pOldFont);
	ReleaseDC(pDC);

	return nWidth;
}

BOOL
CRBPasswordCtrl::CaretAtTextEnd()
{
	wstring strPassword = m_strPasswordText;
	if( m_strPasswordText != L"")
		strPassword = common::CBase64Coding::Decode( m_strPasswordText);

	if( m_nBeginPos + m_nCaretPos < strPassword.length())
		return FALSE;

	return TRUE;
}

BOOL
CRBPasswordCtrl::CaretAtTextFront()
{
	if( m_nBeginPos > 0)
		return FALSE;

	if( m_nCaretPos > 0)
		return FALSE;

	return TRUE;
}

BOOL
CRBPasswordCtrl::CaretAtCtrlEnd()
{
	wstring strTemp1 = GetPlainPassword().substr( m_nBeginPos, m_nCaretPos);
	wstring strTemp2 = GetPlainPassword().substr( m_nBeginPos, m_nCaretPos + 1);

	CPoint ptCaret1( m_rectCtrl.left, m_rectCtrl.top - 1);
	ptCaret1.x = m_rectCtrl.left + GetCharWidthInfo( ConvertToStarChar(strTemp1));

	CPoint ptCaret2( m_rectCtrl.left, m_rectCtrl.top - 1);
	ptCaret2.x = m_rectCtrl.left + GetCharWidthInfo( ConvertToStarChar(strTemp2));

	if( (ptCaret1.x <m_rectCtrl.right - 1) && (ptCaret2.x >= m_rectCtrl.right - 1) )
		return TRUE;

	return FALSE;
}

BOOL
CRBPasswordCtrl::CaretAtCtrlFront()
{
	if( m_nCaretPos == 0)
		return TRUE;

	return FALSE;
}

void 
CRBPasswordCtrl::OnSetFocus(CWnd* pOldWnd)
{
	CWnd::OnSetFocus(pOldWnd);

	//DragonZ:
	m_bShowDefaultText = FALSE;
	Invalidate();

	if( m_bFirstFocus == TRUE)
	{
		m_bFirstFocus = FALSE;
		return;
	}

	CRect rect;
	GetWindowRect(rect);
	CPoint pt = CPoint(rect.left,rect.top);
	SHORT keystate = GetKeyState(VK_CAPITAL);
	if((keystate & 0x0001)!=0)
	{
		//m_ctrlTip.SetHideDelay( RB_INFINITE);
		//m_ctrlTip.Show( L"大写锁定已经打开", this, &pt);
	}

	UpdateCaretPos();

	CWnd* pWnd = GetParent();
	pWnd->PostMessage(PASSWORD_CTRL_SETFOCUS,NULL,NULL);
	
}

void 
CRBPasswordCtrl::UpdateCaretPos()
{
	if (!m_bCaretCreated)
	{
		m_bCaretCreated = ::CreateCaret(GetSafeHwnd(), NULL, 1, 14);
		if( m_bCaretCreated == FALSE)
			return;
	}

	wstring strTemp = m_strShowText.substr( 0, m_nCaretPos);

	CPoint ptCaret( m_rectCtrl.left, m_rectCtrl.top - 1);
	ptCaret.x = m_rectCtrl.left + GetCharWidthInfo( ConvertToStarChar(strTemp));
	if( ptCaret.x >= m_rectCtrl.right - 1)
		ptCaret.x = m_rectCtrl.right - 1;

	SetCaretPos(ptCaret);
	ShowCaret();
}

int 
CRBPasswordCtrl::Point2CharIndex(CPoint pt)
{	
	wstring strPassword = m_strShowText;

	int i = 0;
	for( ; i <= (int)m_strShowText.length() ; i++)
	{
		wstring strTemp = m_strShowText.substr( 0, i);
		int nWidth = GetCharWidthInfo( ConvertToStarChar(strTemp));
		if( nWidth > pt.x)
		{
			i--;
			break;
		}
	}

	if( i > (int)m_strShowText.length())
		i = (int)m_strShowText.length();

	if( i < 0)  i = 0;

	return i;
}

wstring		
CRBPasswordCtrl::ConvertToStarChar( wstring strSrc)
{
	wstring strDest = L"";

	for( int i = 0; i < (int)strSrc.length(); i++)
		strDest += L"●";

	return strDest;
}

wstring		
CRBPasswordCtrl::GetFittableStringFromEnd( UINT nEndIndex, UINT& nBeginIndex)
{
	wstring strPassword = GetPlainPassword();

	int i = nEndIndex ;
	for( ; i  >=  0; i--)
	{
		wstring strTemp = strPassword.substr( i, nEndIndex - i);
		UINT nWidth = GetCharWidthInfo( ConvertToStarChar(strTemp));
		if( nWidth >= GetTextWidth() )
		{
			i++;
			break;
		}
	}

	if( i < 0 ) i = 0;
	m_strShowText = strPassword.substr( i, nEndIndex - i);
	nBeginIndex = i;

	return m_strShowText;
}

wstring		
CRBPasswordCtrl::GetFittableStringFromFront( UINT nBeginIndex, UINT& nEndIndex)
{
	wstring strPassword = GetPlainPassword();

	int i = 0;
	for( ; i <= (int)strPassword.length(); i++)
	{
		wstring strTemp = strPassword.substr( nBeginIndex, i);
		UINT nWidth = GetCharWidthInfo( ConvertToStarChar(strTemp));
		if( nWidth >= GetTextWidth() )
		{
			i--;
			break;
		}
	}

	m_strShowText = strPassword.substr( nBeginIndex, i);
	nEndIndex = nBeginIndex + i - 1 ;

	return m_strShowText;
}

BOOL 
CRBPasswordCtrl::OnEraseBkgnd(CDC* pDC)
{
	CRect rcThis;
	GetClientRect(&rcThis);

	pDC->FillSolidRect(&rcThis, RGB(255,255,255));
	return TRUE;
}

void 
CRBPasswordCtrl::OnLButtonDown(UINT nFlags, CPoint point)
{
	CWnd::OnLButtonDown(nFlags, point);

	if (this != GetFocus())
	{
		SetFocus();
		m_bFirstClick = TRUE;
	}
	else
	{
		m_bFirstClick = FALSE;
		m_nCaretPos = Point2CharIndex(point);
	}


	UpdateCaretPos();
}

void
CRBPasswordCtrl::OnPaint()
{
	if (!m_font.GetSafeHandle())
	{
		m_font.CreateFont(10, 0, 0, 0, 400, 0, 0, 0, DEFAULT_CHARSET, 0, 0, 0, 0, L"宋体");
	}
	if (!m_defaultTextFont.GetSafeHandle())
	{
		m_defaultTextFont.CreateFont(14, 0, 0, 0, 400, 0, 0, 0, DEFAULT_CHARSET, 0, 0, 0, 0, L"tahoma");
	}
	CRect rcThis;
	GetClientRect(&rcThis);
	m_rectCtrl.CopyRect(&rcThis);
	if (rcThis.Height() > 14)
	{
		m_rectCtrl.DeflateRect(2, (rcThis.Height() - 14) / 2);
	}

	CPaintDC dc(this);
	CFont* pOldFont;
	//dc.SetTextColor(RGB(117,160,203));
	//dc.SetTextColor(RGB(204,204,204));
	if (!m_bShowDefaultText)
	{
		dc.SetTextColor(RGB(0,90,128));
		pOldFont = dc.SelectObject(&m_font);
		dc.DrawText( ConvertToStarChar(m_strShowText).c_str(), &m_rectCtrl, DT_LEFT | DT_VCENTER | DT_SINGLELINE | DT_NOPREFIX);
	}else
	{
		dc.SetTextColor(RGB(169,169,169));
		pOldFont = dc.SelectObject(&m_defaultTextFont);
		dc.DrawText( PWD_CTRL_DEFAULT_TEXT, &m_rectCtrl, DT_LEFT | DT_VCENTER | DT_SINGLELINE | DT_NOPREFIX);
	}
	
	dc.SelectObject(pOldFont);
}

void 
CRBPasswordCtrl::OnKillFocus(CWnd* pNewWnd)
{
	CWnd::OnKillFocus(pNewWnd);

	if (m_bCaretCreated)
	{
		HideCaret();
		::DestroyCaret();
		m_bCaretCreated = FALSE;
	}
	std::wstring strTemp = GetPlainPassword();
	if (strTemp == L"")
	{
		m_bShowDefaultText = TRUE;
		Invalidate();
	}
	//m_ctrlTip.Hide();
}

void 
CRBPasswordCtrl::OnTimer(UINT nIDEvent)
{
	// 采用偏方来禁用IME，如果用ImmDisableIME，整个线程的所有窗口的IME都被禁用，
	// 且没有对应的方法(如ImmEnableIME)来启用IME
	if (nIDEvent == IME_PREVENT_TIMER_ID)
	{
		if (GetFocus() && GetFocus()->GetSafeHwnd() == GetSafeHwnd())
		{
			HIMC hImc = ImmGetContext(GetSafeHwnd()); 
			if(ImmGetOpenStatus(hImc))
			{
				ImmSetOpenStatus(hImc, FALSE);
			}
			ImmReleaseContext(GetSafeHwnd(), hImc); 
		}
	}

	CWnd::OnTimer(nIDEvent);
}


LRESULT 
CRBPasswordCtrl::DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	if (message == WM_IME_SETCONTEXT)
	{
		wParam = FALSE;
	}

	return CWnd::DefWindowProc(message, wParam, lParam);
}

void
CRBPasswordCtrl::OnProcessLeftKey( )
{
	// 如果已经在最左侧，则不需要做任何操作
	if( CaretAtTextFront())
		return;

	if( m_nCaretPos > 0)
	{
		m_nCaretPos--;
		return;
	}

	// 将编辑框中的内容右移一个字符
	m_nBeginPos--;
	
	UINT nEndIndex = 0;
	m_strShowText = GetFittableStringFromFront( m_nBeginPos,nEndIndex);
}

void
CRBPasswordCtrl::OnProcessRightKey()
{
	if( CaretAtTextEnd())
		return;

	if( m_nCaretPos >= 0 && m_nCaretPos < (int)m_strShowText.length() )
	{
		m_nCaretPos++;
		return;
	}

	UINT nEndIndex = m_nBeginPos + m_nCaretPos + 1;

	UINT nBeginIndex = 0;
	m_strShowText = GetFittableStringFromEnd( nEndIndex, nBeginIndex);
	m_nBeginPos = nBeginIndex;
}

void
CRBPasswordCtrl::OnProcessHome()
{
	m_nBeginPos = 0;
	m_nCaretPos = 0;

	UINT nEndIndex = 0;
	m_strShowText = GetFittableStringFromFront( 0, nEndIndex);
}

void
CRBPasswordCtrl::OnProcessEnd()
{
	UINT nBeginIndex  = 0;
	m_strShowText = GetFittableStringFromEnd( (UINT)GetPlainPassword().length(), nBeginIndex);

	m_nBeginPos = nBeginIndex;
	m_nCaretPos = (int)m_strShowText.length();
}

void		
CRBPasswordCtrl::OnProcessDelete()
{
	m_bRememberPassword = FALSE;

	UINT nDeletedIndex = m_nBeginPos + m_nCaretPos;
	if( nDeletedIndex >= GetPlainPassword().length())
		return;

	m_strPasswordText = GetPlainPassword();
	m_strPasswordText.erase( m_strPasswordText.begin() + nDeletedIndex);
	m_strPasswordText = common::CBase64Coding::Encode( m_strPasswordText);

	UINT nEndIndex = 0;
	m_strShowText = GetFittableStringFromFront( m_nBeginPos, nEndIndex);
}

void		
CRBPasswordCtrl::OnProcessBack()
{
	m_bRememberPassword = FALSE;

	int nDeletedIndex = m_nBeginPos + m_nCaretPos - 1;
	if( nDeletedIndex < 0)
		return;

	m_strPasswordText = GetPlainPassword();
	m_strPasswordText.erase( m_strPasswordText.begin() + nDeletedIndex);
	m_strPasswordText = common::CBase64Coding::Encode( m_strPasswordText);

	if( m_nBeginPos > 0)
	{
		// 光标在最左侧，此时如果删除一个字符，后面的字符将依次跟上
		if( m_nCaretPos != 0)
		{
			m_nBeginPos--;		
		}
		else
		{
			m_nBeginPos -= 2;
			if( m_nBeginPos < 0)
				m_nBeginPos = 0;

			m_nCaretPos++;
		}

		UINT nEndIndex = 0;
		m_strShowText = GetFittableStringFromFront( m_nBeginPos, nEndIndex);

		return;
	}

	if( m_nBeginPos == 0)
	{
		m_nCaretPos--;
	}

	UINT nEndIndex = 0;
	m_strShowText = GetFittableStringFromFront( m_nBeginPos, nEndIndex);
}

void
CRBPasswordCtrl::OnProcessChar( UINT nChar )
{
	m_bRememberPassword = FALSE;
	m_strPasswordText = GetPlainPassword();
	m_strPasswordText.insert( m_strPasswordText.begin() + m_nBeginPos + m_nCaretPos, 1, nChar);
	m_strPasswordText = common::CBase64Coding::Encode( m_strPasswordText);

	//DragonZ:
	std::wstring strTemp = L"";
	strTemp = GetPlainPassword();
	if (strTemp != L"")
	{
		CWnd* pWnd = GetParent();
		pWnd->PostMessage(WM_PWD_CTRL_TIP,FALSE,NULL);
	}

	// 在编辑框的末尾追加字符，则文本中显示的字符需要左移一个字符
	if( CaretAtCtrlEnd())
	{
		UINT nEndIndex = m_nBeginPos + m_nCaretPos + 1;
		UINT nBeginIndex = 0;
		m_strShowText = GetFittableStringFromEnd( nEndIndex, nBeginIndex);

		m_nBeginPos = nBeginIndex;
		return;
	}

	m_nCaretPos++;

	UINT nEndIndex = 0;
	m_strShowText = GetFittableStringFromFront( m_nBeginPos, nEndIndex);
	
	return;
}

BOOL 
CRBPasswordCtrl::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_CHAR && !IsKeyDown(VK_CONTROL) )
	{
		if (  m_nLimitText < 0 || (int)GetPlainPassword().length() < m_nLimitText) 
		{
			OnProcessChar( (UINT)pMsg->wParam);
		}

		Invalidate();
		UpdateCaretPos();

		return TRUE;
	}


	if (pMsg->message == WM_KEYDOWN)
	{
		switch (pMsg->wParam)
		{			
		case VK_HOME:
			{
				OnProcessHome();
				Invalidate();
				UpdateCaretPos();

				return TRUE;
			}

		case VK_END:
			{
				OnProcessEnd();
				Invalidate();
				UpdateCaretPos();

				return TRUE;
			}

		case VK_LEFT:
			{		
				OnProcessLeftKey();
				Invalidate();
				UpdateCaretPos();

				return TRUE;
			}

		case VK_RIGHT:
			{
				OnProcessRightKey();
				Invalidate();
				UpdateCaretPos();

				return TRUE;
			}

		case VK_DELETE:
			{
				OnProcessDelete();
				Invalidate();
				UpdateCaretPos();

				return TRUE;
			}

		case VK_BACK:
			{
				OnProcessBack();
				Invalidate();
				UpdateCaretPos();

				return TRUE;
			}

		case VK_CAPITAL:
			{
				CRect rect;
				GetWindowRect(rect);
				CPoint pt = CPoint(rect.left,rect.top);
				SHORT keystate = GetKeyState(VK_CAPITAL);
				if((keystate & 0x0001) !=0 )
				{
					//m_ctrlTip.SetHideDelay( RB_INFINITE);
					//m_ctrlTip.Show( L"大写锁定已经打开", this, &pt);
				}
				else
				{
					//m_ctrlTip.Hide();
				}

				return TRUE;
			}

		}
	} 

	return CWnd::PreTranslateMessage(pMsg);
}

void CRBPasswordCtrl::OnMouseMove(UINT nFlags, CPoint point)
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

HRESULT CRBPasswordCtrl::OnMouseHover(WPARAM w, LPARAM l)
{
	CWnd* pWnd = GetParent();
	pWnd->PostMessage(PASSWORD_CTRL_MOUSE_HOVER,NULL,NULL);

	return 0;
}

HRESULT CRBPasswordCtrl::OnMouseLeave(WPARAM w, LPARAM l)
{

	CWnd* pWnd = GetParent();
	pWnd->PostMessage(PASSWORD_CTRL_MOUSE_LEAVE,NULL,NULL);

	return 0;
}
