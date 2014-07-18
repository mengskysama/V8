// RBEdit.cpp : 实现文件
//

#include "stdafx.h"
#include "RBEdit.h"

#include "TipCtrl.h"
#include "GGHelper.h"

#include "../CommonLibrary/include/ui/UIHelper.h"
#include "../CommonLibrary/include/utility/StringHelper.h"

using namespace common;
using namespace common::utility;


#define UWM_CHECKTEXT_MSG _T("UWM_CHECKTEXT_MSG-{D53C7065_FD4C_138A_BC72_01A0253651C0}")

//Resize Message
const UINT CRBEdit::UWM_CHECKTEXT = ::RegisterWindowMessage(UWM_CHECKTEXT_MSG);

// CRBEdit

IMPLEMENT_DYNAMIC(CRBEdit, CEdit)

CRBEdit::CRBEdit()
{
	m_nLimitInBytes = 0xffff;
	m_crBorder = RGB(0, 0, 255);
	m_crBack = RGB(0xff, 0xff, 0xff);
	m_bBorderDrawFlag = false;
	m_crText = RGB(0, 0, 0);
	m_crTextGray = RGB(128, 128, 128);
	m_bReplaceNewlineWithBlankFlag = false;
	m_pctrlTip = NULL;
	m_autoscrollvisible = FALSE;
}

CRBEdit::~CRBEdit()
{
	if (m_brBack.GetSafeHandle())
	{
		m_brBack.DeleteObject();
	}
	if (m_brBackGray.GetSafeHandle())
	{
		m_brBackGray.DeleteObject();
	}
}


BEGIN_MESSAGE_MAP(CRBEdit, CEdit)
	ON_WM_CHAR()
    ON_WM_SIZE()
	ON_MESSAGE(WM_PASTE, OnPaste)
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	ON_WM_CTLCOLOR_REFLECT()
	ON_REGISTERED_MESSAGE(UWM_CHECKTEXT, OnCheckText)
END_MESSAGE_MAP()



// CRBEdit 消息处理程序


int CRBEdit::GetLengthInAnsi()
{
	CString sText;
	GetWindowText(sText);

	int nRet = stringhelper::CalcStringAsciiLength((wchar_t*)(LPCTSTR)sText);

	return nRet;
}

int CRBEdit::GetSelLengthInAnsi()
{
	int nSelStart = 0;
	int nSelEnd = 0;
	GetSel(nSelStart, nSelEnd);

	CString sText;
	GetWindowText(sText);

	CString sSel = sText.Mid(nSelStart, nSelEnd - nSelStart);

	int nRet = stringhelper::CalcStringAsciiLength((wchar_t*)(LPCTSTR)sSel);

	return nRet;
}

void CRBEdit::ShowCharsetForbiddenTip()
{
	ui::CClientRect rcThis(this, GetParent());
	CPoint ptTip;
	ptTip.x = rcThis.right;
	ptTip.y = rcThis.bottom + 2;

	if (!m_pctrlTip)
	{
		m_pctrlTip = new CTipCtrl;
	}
	GetParent()->ClientToScreen(&ptTip);
	CString sTipForbidden = m_sCharsetFobiddenTip + L"\n" + m_sCharsetFobidden;
	m_pctrlTip->Show(sTipForbidden, GetParent(), &ptTip);
}

void CRBEdit::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	PostMessage(UWM_CHECKTEXT);
	if (IsInCharsetForbidden(nChar))
	{
		ShowCharsetForbiddenTip();
		MessageBeep(0);
		return;
	}

	int nBytesInc = stringhelper::IsCharAscii(nChar) ? 1 : 2;
	if (!::HelperIsKeyDown(VK_CONTROL) && nChar != VK_BACK 
		&& (GetLengthInAnsi() + nBytesInc - GetSelLengthInAnsi() > m_nLimitInBytes))
	{
		MessageBeep(0);
		NotifyInputExceed(m_nLimitInBytes, GetLengthInAnsi() + nBytesInc - GetSelLengthInAnsi());

		return;
	}

	CEdit::OnChar(nChar, nRepCnt, nFlags);
}

void CRBEdit::OnSize(UINT nType, int cx, int cy)
{
    PostMessage(UWM_CHECKTEXT);
    __super::OnSize(nType, cx, cy);
}

int CRBEdit::ReplaceSel(const wchar_t* szTextNew)
{
	int nLenTextNewInAnsi = stringhelper::CalcStringAsciiLength(szTextNew);
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

	if (m_bReplaceNewlineWithBlankFlag || !(GetStyle() & ES_MULTILINE))	// fix bug1356
	{
		CString s(szReallyPaste);
		s.Replace(L"\r\n", L"\n");
		s.Replace(L"\r", L"\n");
		s.Replace(L"\n", L" ");

		wcscpy(szReallyPaste, s);
	}

	CEdit::ReplaceSel(szReallyPaste, true);	// core

	int nRet = wcslen(szReallyPaste);

	delete[] szReallyPaste;
	szReallyPaste = NULL;

	return nRet;
}

LRESULT CRBEdit::OnPaste(WPARAM w, LPARAM l)
{
	int nLenClipboardText = HelperGetClipboardTextLength();
	if (nLenClipboardText <= 0)
	{
		return 0;
	}

	wchar_t* szClipboardText = new wchar_t[nLenClipboardText + 1];
	ZeroMemory(szClipboardText, sizeof(wchar_t) * (nLenClipboardText + 1));
	HelperGetClipboardText(szClipboardText, nLenClipboardText+1);
	{
		CString sPasteReal = szClipboardText;

		int idx = FindCharForbidden(szClipboardText);
		if (idx != -1)
		{
			sPasteReal = sPasteReal.Left(idx);
		}

		ReplaceSel(sPasteReal);

		if (idx != -1)
		{
			ShowCharsetForbiddenTip();
		}
	}
	delete[] szClipboardText;
	szClipboardText = NULL;

	return 0;
}

void CRBEdit::OnPaint()
{
	//CPaintDC dc(this); // device context for painting
	// TODO: 在此处添加消息处理程序代码
	// 不为绘图消息调用 CEdit::OnPaint()
	CEdit::OnPaint();

	if (m_bBorderDrawFlag)
	{
		ui::CWindowRect rcThis(this);
		CWindowDC dc(this);
		ScreenToClient(rcThis);
		dc.Draw3dRect(rcThis, m_crBorder, m_crBorder);
	}
}

void CRBEdit::NotifyInputExceed(int nLimitNow, int nInputLenYouWant)
{
	NMHDR nmh;
	nmh.hwndFrom = GetSafeHwnd();
	nmh.idFrom = GetDlgCtrlID();
	nmh.code = RBEditNotify_InputExceed;

	GetParent()->SendMessage(WM_NOTIFY, nmh.idFrom, (LPARAM)(LPNMHDR)&nmh);
}

BOOL CRBEdit::OnEraseBkgnd(CDC* pDC)
{
	return CEdit::OnEraseBkgnd(pDC);
}

HBRUSH CRBEdit::CtlColor(CDC* pDC, UINT nCtlColor)
{
	if (!m_brBack.GetSafeHandle())
	{
		CreateBackBrush();
	}

	if (!m_brBackGray.GetSafeHandle())
	{
		CreateBackBrush();
	}

	if (IsWindowEnabled() || (GetStyle()&ES_READONLY) == TRUE)
	{
		pDC->SetTextColor(m_crText);
		pDC->SetBkColor(m_crBack);
		return m_brBack;
	}
	else
	{
		pDC->SetTextColor(m_crTextGray);
		pDC->SetBkColor(m_crBackGray);
		return m_brBackGray;
	}
}

void CRBEdit::CreateBackBrush()
{
	if (m_brBack.GetSafeHandle())
	{
		m_brBack.DeleteObject();
	}

	m_brBack.CreateSolidBrush(m_crBack);
	
	if (m_brBackGray.GetSafeHandle())
	{
		m_brBackGray.DeleteObject();
	}

	m_brBackGray.CreateSolidBrush(m_crBackGray);
}

bool CRBEdit::IsInCharsetForbidden(wchar_t wc)
{
	return m_sCharsetFobidden.Find(wc) != -1;
}

int CRBEdit::FindCharForbidden(CString s)
{
	for (int i = 0; i < s.GetLength(); i++)
	{
		if (IsInCharsetForbidden(s[i]))
		{
			return i;
		}
	}

	return -1;
}
LRESULT CRBEdit::OnCheckText(WPARAM wParam, LPARAM lParam)
{
	CString oStr;
	GetWindowText(oStr);
	CheckScrolling(oStr);
	return 0;
}
void CRBEdit::CheckScrolling(LPCTSTR lpszString)
{
	if (m_autoscrollvisible == FALSE)
	{
		return;
	}
	CRect oRect;
	GetClientRect(&oRect);
	CClientDC oDC(this);
	int iHeight=0;
	BOOL bHoriz = FALSE;
	CFont* pEdtFont = GetFont();
	if(pEdtFont != NULL)
	{
		//Determine Text Width and Height
		SIZE oSize;
		CFont* pOldFont = oDC.SelectObject(pEdtFont);
		//Determine the line Height
		oSize = oDC.GetTextExtent(CString(_T(" ")));
		iHeight = oSize.cy;


		//Text Width
		int iWidth=0, i =0;
		CString oStr;
		//Parse the string, the lines in a multiline Edit are separated by "\r\n"
		/*		while(TRUE == ::AfxExtractSubString(oStr, lpszString, i, _T('\n')))
		{
		if(FALSE == bHoriz)
		{
		int iLen = oStr.GetLength()-1;
		if(iLen >=0)
		{
		//Eliminate last '\r'
		if(_T('\r') == oStr.GetAt(iLen))
		oStr = oStr.Left(iLen);
		oSize = oDC.GetTextExtent(oStr);
		if(iWidth < oSize.cx)
		iWidth = oSize.cx;
		if(iWidth >= oRect.Width())
		bHoriz = TRUE;
		}
		}
		i++;
		}*/
		oDC.SelectObject(pOldFont);
		//Text Height
		iHeight *=GetLineCount();//(i+1);
	}
	//	ShowHorizScrollBar(bHoriz);
	ShowVertScrollBar(iHeight >= oRect.Height());
}
void CRBEdit::SetWindowText(LPCTSTR lpszString)
{

	__super::SetWindowText(lpszString);	
	CheckScrolling(lpszString);
}