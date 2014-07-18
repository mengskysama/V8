#include "StdAfx.h"
#include "GiftComboBox.h"

CGiftComboBox::CGiftComboBox(void)
{
}

CGiftComboBox::~CGiftComboBox(void)
{
}
BEGIN_MESSAGE_MAP(CGiftComboBox, CComboBox)
	ON_CONTROL_REFLECT_EX(CBN_SELCHANGE, &CGiftComboBox::OnCbnSelchange)
	ON_CONTROL_REFLECT(CBN_DROPDOWN, &CGiftComboBox::OnCbnDropdown)
	ON_CONTROL_REFLECT(CBN_CLOSEUP, &CGiftComboBox::OnCbnCloseup)
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()

BOOL CGiftComboBox::OnCbnSelchange()
{
	// TODO: 在此添加控件通知处理程序代码
	int nSel = GetCurSel();
	CString sCount;
	GetLBText(nSel,sCount);
	sCount = sCount.Left(sCount.Find(L"("));
	nSel = AddString(sCount);
	SetCurSel(nSel);

	return TRUE;
}

void CGiftComboBox::OnCbnDropdown()
{
	// TODO: 在此添加控件通知处理程序代码
	ResetContent();
	InsertString(0,L"18(要发)");
	InsertString(1,L"38(三八)");
	InsertString(2,L"99(长久)");
	InsertString(3,L"520(我爱你)");
	InsertString(4,L"666(六六大顺)");
	InsertString(5,L"888(恭喜发财)");
	InsertString(6,L"1314(一生一世)");
	InsertString(7,L"3344(生生世世)");
	InsertString(8,L"9999(天长地久)");
}

void CGiftComboBox::InitGiftCombo()
{
	SetDroppedWidth(74);
	InsertString(0,L"1");
	SetCurSel(0);
	//((CEdit*)GetDlgItem(1001))->SetWindowText(L"1");
}

void CGiftComboBox::OnCbnCloseup()
{
	// TODO: 在此添加控件通知处理程序代码
	int nSel = GetCurSel();
	if (nSel == CB_ERR)
	{
		InsertString(0,L"1");
		SetCurSel(0);
	}
}

void CGiftComboBox::GetEditText( CString & strText )
{
	CEdit * pEdit = (CEdit*)(GetWindow(GW_CHILD));
	if (pEdit)
	{
		pEdit->GetWindowText(strText);
	}
}
HBRUSH CGiftComboBox::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	pDC->SetBkMode(TRANSPARENT);
	if (nCtlColor == CTLCOLOR_LISTBOX)
	{
		if (m_listbox.GetSafeHwnd() == NULL)
		{
			m_listbox.SubclassWindow(pWnd->GetSafeHwnd());
		}
	}
	if (nCtlColor == CTLCOLOR_EDIT)
	{
		if (m_edit.GetSafeHwnd() == NULL)
		{
			m_edit.SubclassWindow(pWnd->GetSafeHwnd());
		}
	}
	HBRUSH hbr = CComboBox::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  在此更改 DC 的任何属性
	//pDC->SetTextColor(RGB(0, 0, 255));
	//hbr = ::CreateSolidBrush(RGB(255, 0, 0));

	// TODO:  如果默认的不是所需画笔，则返回另一个画笔
	return hbr;
}

void CGiftComboBox::PreSubclassWindow()
{
	//CWnd * pWnd = GetWindow(GW_CHILD);
	//ModifyStyle(pWnd->GetSafeHwnd(), 0, ES_NUMBER, 0);

	CComboBox::PreSubclassWindow();
}
