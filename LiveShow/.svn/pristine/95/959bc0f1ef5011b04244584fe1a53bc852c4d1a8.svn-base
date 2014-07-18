
#include "..\..\include\ui\VDCComboBox.h"


CVDCComboBox::CVDCComboBox(void)
{
	m_bReadOnly = FALSE;
	m_hBrush = CreateSolidBrush(RGB(255, 255, 255));
}

CVDCComboBox::~CVDCComboBox(void)
{
	if (m_hBrush != NULL)
	{
		DeleteObject(m_hBrush);
		m_hBrush = NULL;
	}
}
BEGIN_MESSAGE_MAP(CVDCComboBox, CComboBox)
	ON_WM_CTLCOLOR()
	ON_WM_SETFOCUS()
END_MESSAGE_MAP()

HBRUSH CVDCComboBox::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CComboBox::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  在此更改 DC 的任何属性
	//设置只读属性，只执行一次
	if (!m_bReadOnly)
	{
		CEdit* pEdit = (CEdit*)this->GetWindow(GW_CHILD);
		if ((pEdit != NULL) && (pEdit->GetSafeHwnd() != NULL))
		{
			pEdit->SetReadOnly(TRUE);
			m_bReadOnly = TRUE;
		}
	}

	//设置背景色
	if (m_hBrush)
	{
		pDC->SetBkColor(RGB(255, 255, 255));
		return m_hBrush;
	}
	
	// TODO:  如果默认的不是所需画笔，则返回另一个画笔
	return hbr;
}

void CVDCComboBox::OnSetFocus(CWnd* pOldWnd)
{
	CComboBox::OnSetFocus(pOldWnd);

	// TODO: 在此处添加消息处理程序代码
	//第一次点下拉时，会有光标出现，在此隐藏
	CEdit* pEdit = (CEdit*)this->GetWindow(GW_CHILD);
	if ( (pEdit != NULL) && (pEdit->GetSafeHwnd() != NULL) )
	{
		pEdit->HideCaret();
	}
}

BOOL CVDCComboBox::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	HWND hWnd = ::GetFocus();
	CEdit* pEdit = (CEdit*)this->GetWindow(GW_CHILD);
	if ( (pEdit != NULL) && (pEdit->GetSafeHwnd() != NULL) )
	{
		if (pEdit->GetSafeHwnd() == hWnd)
		{
			pEdit->HideCaret();
		}
	}
	
	return CComboBox::PreTranslateMessage(pMsg);
}
