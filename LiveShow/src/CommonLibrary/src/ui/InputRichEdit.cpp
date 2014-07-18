// InputRichEdit.cpp : 实现文件
//

#include "stdafx.h"
#include "../../include/ui/InputRichEdit.h"


// CInputRichEdit

IMPLEMENT_DYNAMIC(CInputRichEdit, CRichEditCtrl)

CInputRichEdit::CInputRichEdit()
{
	m_stFont.font_type = _T("宋体") ;
	m_stFont.font_size = 12;
	m_stFont.font_style = 0;
	m_stFont.font_color = RGB(0,0,0);
	m_strDefaultContent = L"";
}

CInputRichEdit::~CInputRichEdit()
{
}


BEGIN_MESSAGE_MAP(CInputRichEdit, CRichEditCtrl)

END_MESSAGE_MAP()

void CInputRichEdit::SetFontSize( UINT fontsize )
{
	CHARFORMAT cf;
	memset(&cf, 0, sizeof(CHARFORMAT));

	GetDefaultCharFormat(cf);
	cf.yHeight = fontsize * 15;
	m_stFont.font_size = fontsize;

	SetDefaultCharFormat(cf);
}

void CInputRichEdit::SetFontBold( bool bBold )
{
	CHARFORMAT cf;
	memset(&cf, 0, sizeof(CHARFORMAT));

	GetDefaultCharFormat(cf);
	if (bBold)
	{
		cf.dwEffects |= CFE_BOLD;
		m_stFont.font_style |= core::FONTSTYLETYPE_BOLD;
	}
	else
	{
		cf.dwEffects &= ~CFE_BOLD;
		m_stFont.font_style &= ~core::FONTSTYLETYPE_BOLD;
	}

	SetDefaultCharFormat(cf);
}

void CInputRichEdit::SetFontItalic( bool bItalic )
{
	CHARFORMAT cf;
	memset(&cf, 0, sizeof(CHARFORMAT));

	GetDefaultCharFormat(cf);
	if (bItalic)
	{
		cf.dwEffects |= CFE_ITALIC;
		m_stFont.font_style |= core::FONTSTYLETYPE_ITALICS;
	}
	else
	{
		cf.dwEffects &= ~CFE_ITALIC;
		m_stFont.font_style &= ~core::FONTSTYLETYPE_ITALICS;
	}

	SetDefaultCharFormat(cf);
}

void CInputRichEdit::SetFontUnderLine( bool bUnderLine )
{
	CHARFORMAT cf;
	memset(&cf, 0, sizeof(CHARFORMAT));

	GetDefaultCharFormat(cf);
	if (bUnderLine)
	{
		cf.dwEffects |= CFE_UNDERLINE;
		m_stFont.font_style |= core::FONTSTYLETYPE_UNDERLINE;
	}
	else
	{
		cf.dwEffects &= ~CFE_UNDERLINE;
		m_stFont.font_style &= ~core::FONTSTYLETYPE_UNDERLINE;
	}
	

	SetDefaultCharFormat(cf);
}

void CInputRichEdit::SetFontName( std::wstring strFontName )
{
	CHARFORMAT cf;
	memset(&cf, 0, sizeof(CHARFORMAT));

	GetDefaultCharFormat(cf);
	swprintf_s(cf.szFaceName, strFontName.c_str());
	m_stFont.font_type = strFontName;

	//CFontDialog dlg;
	//if (dlg.DoModal() == IDOK)
	//{
	//	dlg.GetCharFormat(cf);
	//}

	SetDefaultCharFormat(cf);
}

void CInputRichEdit::SetFontInfo(core::MessageFont& FontInfo ) 
{
	SetFontName( FontInfo.font_type ) ;
	SetFontSize( FontInfo.font_size ) ;
	SetFontColor( FontInfo.font_color ) ;

	if ( FontInfo.font_style&core::FONTSTYLETYPE_BOLD )
	{
		SetFontBold( true ) ;
	}
	else
	{
		SetFontBold( false ) ;
	}
	if ( FontInfo.font_style&core::FONTSTYLETYPE_UNDERLINE )
	{
		SetFontItalic( true ) ;
	}
	else
	{
		SetFontItalic( false ) ;
	}
	if ( FontInfo.font_style&core::FONTSTYLETYPE_ITALICS )
	{
		SetFontUnderLine( true ) ;
	}
	else
	{
		SetFontUnderLine( false ) ;
	}

}

core::MessageFont CInputRichEdit::GetFontInfo()
{
	return m_stFont;
}

void CInputRichEdit::UpdateFont(CHARFORMAT & cf)
{
	//SetSel(0, -1);
	//SetSelectionCharFormat(cf);
	//SetSel(-1, 0);
}

void CInputRichEdit::SetFontColor( COLORREF crColor )
{
	CHARFORMAT cf;
	memset(&cf, 0, sizeof(CHARFORMAT));

	GetDefaultCharFormat(cf);
	cf.dwEffects &= ~CFE_AUTOCOLOR;
	cf.crTextColor = crColor;
	m_stFont.font_color = crColor;

	SetDefaultCharFormat(cf);
}

void CInputRichEdit::SetDefaultFont(UINT fontsize/* = 12*/)
{
	CHARFORMAT cf;
	memset(&cf, 0, sizeof(CHARFORMAT));

	GetDefaultCharFormat(cf);
	cf.yHeight = fontsize * 15;
	cf.dwEffects &= ~CFE_BOLD;
	cf.dwEffects &= ~CFE_ITALIC;
	cf.dwEffects &= ~CFE_UNDERLINE;
	swprintf_s(cf.szFaceName, L"宋体");

	SetDefaultCharFormat(cf);

	m_stFont.font_size = fontsize;
}

void CInputRichEdit::ClearInput()
{
	SetWindowText(L"");
}

void CInputRichEdit::SetDefaultContent(CString strContent)
{

}
// CInputRichEdit 消息处理程序

