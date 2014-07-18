#pragma once
#include "..\..\CommonLibrary\include\ui\InputRichEdit.h"
class CFlyingInputRichEdit : public CInputRichEdit
{
public:
	CFlyingInputRichEdit() ;
	virtual ~CFlyingInputRichEdit() ;

public:
	void SetDefaultContent(CString strContent);
protected:
	DECLARE_MESSAGE_MAP()

protected:
	CString m_strDefaultContent;
public:
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
};
