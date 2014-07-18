#pragma once
#include "../Common.h"
#include "coredefine.h"

// CInputRichEdit

class COMMON_INTERFACE CInputRichEdit : public CRichEditCtrl
{
	DECLARE_DYNAMIC(CInputRichEdit)

public:
	CInputRichEdit();
	virtual ~CInputRichEdit();

	void SetDefaultFont(UINT fontsize = 12);
	void SetDefaultContent(CString strContent);

	void SetFontSize(UINT fontsize);
	void SetFontBold(bool bBold);
	void SetFontItalic(bool bItalic);
	void SetFontUnderLine(bool bUnderLine);
	void SetFontName(std::wstring strFontName);
	void SetFontColor(COLORREF crColor);
	void ClearInput();

	void SetFontInfo(core::MessageFont& FontInfo ) ;
	core::MessageFont GetFontInfo();		//得到字体信息(大小、颜色、字体名)

protected:
	void UpdateFont(CHARFORMAT & cf);

protected:
	core::MessageFont m_stFont;
	CString m_strDefaultContent;

protected:
	DECLARE_MESSAGE_MAP()
public:
};


