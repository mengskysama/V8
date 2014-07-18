#include "stdafx.h"
#include "RBExtEdit.h"
#include <string>
#include "uihelper.h"
#include "GGHelper.h"

using namespace std;

//=================================================================
// CShowHintEdit
// 当Edit第一次获取焦点的时候，自动定位到文字的末尾


IMPLEMENT_DYNAMIC(CAutoEndEdit, CLimitTextEdit)

CAutoEndEdit::CAutoEndEdit()
{
	m_bFirstClick = true;
}

CAutoEndEdit::~CAutoEndEdit()
{
}

BEGIN_MESSAGE_MAP(CAutoEndEdit, CLimitTextEdit)
	ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()

void	
CAutoEndEdit::OnLButtonDown(UINT nFlags, CPoint point)
{
	CEdit::OnLButtonDown( nFlags, point);

	if( m_bFirstClick == true)
	{
		m_bFirstClick = false;

		int nLen = GetWindowTextLength();
		SetFocus();
		SetSel( nLen, nLen);
	}
}

//=================================================================
// CRBLimitText
// 

IMPLEMENT_DYNAMIC(CLimitTextEdit, CColorEdit)

CLimitTextEdit::CLimitTextEdit()
{
	m_nLimitInBytes = 0;
}

CLimitTextEdit::~CLimitTextEdit()
{

}

BEGIN_MESSAGE_MAP(CLimitTextEdit, CColorEdit)
	ON_WM_CHAR()
	ON_MESSAGE(WM_PASTE, OnPaste)
END_MESSAGE_MAP()

void	
CLimitTextEdit::SetLimit( UINT nLimitNum)
{
	m_nLimitInBytes = nLimitNum;
}

int 
CLimitTextEdit::GetLengthInAnsi()
{
	CString sText;
	GetWindowText(sText);

	int nRet = HelperGetStringLengthInAnsi((wchar_t*)(LPCTSTR)sText);

	return nRet;
}

int 
CLimitTextEdit::GetSelLengthInAnsi()
{
	int nSelStart = 0;
	int nSelEnd = 0;
	GetSel(nSelStart, nSelEnd);

	CString sText;
	GetWindowText(sText);

	CString sSel = sText.Mid(nSelStart, nSelEnd - nSelStart);

	int nRet = HelperGetStringLengthInAnsi((wchar_t*)(LPCTSTR)sSel);

	return nRet;
}

void 
CLimitTextEdit::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	int nBytesInc = HelperIsAnsiChar(nChar) ? 1 : 2;
	if (!HelperIsKeyDown(VK_CONTROL) && nChar != VK_BACK && (GetLengthInAnsi() + 
		nBytesInc - GetSelLengthInAnsi() > m_nLimitInBytes))
	{
		MessageBeep(0);
		NotifyInputExceed(m_nLimitInBytes, GetLengthInAnsi() + nBytesInc - GetSelLengthInAnsi());

		return;
	}

	CEdit::OnChar(nChar, nRepCnt, nFlags);
}

int 
CLimitTextEdit::ReplaceSel(const wchar_t* szTextNew)
{
	int nLenTextNewInAnsi = HelperGetStringLengthInAnsi(szTextNew);
	int nLenInAnsi = GetLengthInAnsi();
	int nLenSelInAnsi = GetSelLengthInAnsi();

	int nLenCanPaste = m_nLimitInBytes - (nLenInAnsi - nLenSelInAnsi);
	int nLenReallyPaste = nLenCanPaste;
	if (nLenTextNewInAnsi <= nLenCanPaste)
	{
		nLenReallyPaste = nLenTextNewInAnsi;
	}
	else	// too much text in clipboard, will truncate, make a beep :)
	{
		MessageBeep(0);

		NotifyInputExceed(m_nLimitInBytes, nLenInAnsi + nLenTextNewInAnsi - nLenSelInAnsi);
	}

	if (nLenReallyPaste <= 0)
	{
		return 0;
	}

	wchar_t* szReallyPaste = new wchar_t[nLenReallyPaste + 1];
	ZeroMemory(szReallyPaste, sizeof(wchar_t) * (nLenReallyPaste + 1));
	HelperGetSubstrInAnsi(szTextNew, szReallyPaste, nLenReallyPaste);
	ASSERT(wcslen(szReallyPaste));

	CEdit::ReplaceSel(szReallyPaste, true);	// core

	int nRet = wcslen(szReallyPaste);

	delete[] szReallyPaste;
	szReallyPaste = NULL;

	return nRet;
}

LRESULT 
CLimitTextEdit::OnPaste(WPARAM w, LPARAM l)
{
	int nLenClipboardText = HelperGetClipboardTextLength();
	if (nLenClipboardText <= 0)
	{
		return 0;
	}

	wchar_t* szClipboardText = new wchar_t[nLenClipboardText + 1];
	ZeroMemory(szClipboardText, sizeof(wchar_t) * (nLenClipboardText + 1));
	HelperGetClipboardText(szClipboardText, nLenClipboardText + 1);
	{
		ReplaceSel(szClipboardText);
	}
	delete[] szClipboardText;
	szClipboardText = NULL;

	return 0;
}

void 
CLimitTextEdit::NotifyInputExceed(int nLimitNow, int nInputLenYouWant)
{
	NMHDR nmh;
	nmh.hwndFrom = GetSafeHwnd();
	nmh.idFrom = GetDlgCtrlID();
	nmh.code = RBEditNotify_InputExceed;

	GetParent()->SendMessage(WM_NOTIFY, nmh.idFrom, (LPARAM)(LPNMHDR)&nmh);
}


//=================================================================
//                      CShowHintEdit


IMPLEMENT_DYNAMIC(CShowHintEdit, CLimitTextEdit)

CShowHintEdit::CShowHintEdit()
{
}

CShowHintEdit::~CShowHintEdit()
{
}

BEGIN_MESSAGE_MAP(CShowHintEdit, CLimitTextEdit)
	ON_WM_SETFOCUS()
	ON_WM_KILLFOCUS()
END_MESSAGE_MAP()

void	
CShowHintEdit::OnSetFocus( CWnd* pOldWnd )
{
	CWnd::OnSetFocus( pOldWnd);

	CString strShowText;
	GetWindowText(strShowText);

	if( strShowText == SHOW_TEXT)
		SetWindowText(L"");

	SetTextColor( RGB(0,0,0));
}

void 
CShowHintEdit::OnKillFocus(CWnd* pNewWnd)
{
	CWnd::OnKillFocus(pNewWnd);

	CString strShowText;
	GetWindowText(strShowText);

	if( strShowText == L"")
	{
		SetWindowText(SHOW_TEXT);
		SetTextColor( RGB(153,153,153));
	}
	else
	{
		SetTextColor( RGB(0,0,0));
	}
}