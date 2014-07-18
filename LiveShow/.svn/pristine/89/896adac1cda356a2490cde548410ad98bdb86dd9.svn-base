#pragma once
#include "XTipListBox.h"
#include "NumEdit.h"

class CGiftComboBox : public CComboBox
{
public:
	CGiftComboBox(void);
	~CGiftComboBox(void);
	DECLARE_MESSAGE_MAP()
	afx_msg BOOL OnCbnSelchange();
	afx_msg void OnCbnDropdown();

public:
	void InitGiftCombo();
	void GetEditText(CString & strText);
	afx_msg void OnCbnCloseup();

protected:
	CXTipListBox m_listbox;
	CNumEdit m_edit;
public:
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
protected:
	virtual void PreSubclassWindow();
};
