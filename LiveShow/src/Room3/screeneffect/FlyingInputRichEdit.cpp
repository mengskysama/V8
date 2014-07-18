#include "stdafx.h"
#include "FlyingInputRichEdit.h"

CFlyingInputRichEdit::CFlyingInputRichEdit()
{

}

CFlyingInputRichEdit::~CFlyingInputRichEdit()
{

}

BEGIN_MESSAGE_MAP(CFlyingInputRichEdit ,CInputRichEdit)
	ON_WM_SETFOCUS()
	ON_WM_KILLFOCUS()
END_MESSAGE_MAP()

void CFlyingInputRichEdit::SetDefaultContent(CString strContent)
{
	m_strDefaultContent = strContent;
	SetFontColor(RGB(128, 128, 128));
	SetWindowText(strContent);
}


void CFlyingInputRichEdit::OnSetFocus(CWnd* pOldWnd)
{
	CInputRichEdit::OnSetFocus(pOldWnd);

	CString strText;
	GetWindowText(strText);

	if (strText == m_strDefaultContent)
	{
		SetWindowText(L"");
		SetFontColor(RGB(0, 0, 0));
	}
}

void CFlyingInputRichEdit::OnKillFocus(CWnd* pNewWnd)
{
	CInputRichEdit::OnKillFocus(pNewWnd);

	CString strText;
	GetWindowText(strText);

	if (m_strDefaultContent != L"" && strText == L"")
	{
		SetFontColor(RGB(128, 128, 128));
		SetWindowText(m_strDefaultContent);
	}
}