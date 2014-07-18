#include "stdafx.h"
#include "RBLabel.h"
#include "string_helper.h"

CTextLabel::CTextLabel()
{
	m_pDC = NULL;
	m_clrText = RGB(0,0,0);
	m_pFont = NULL;

	m_bUnderline = FALSE;
	m_nLineSpace = 0;
}

CTextLabel::~CTextLabel()
{
}

UINT 
CTextLabel::GetCharWidthInfo( wstring strText )
{
	if ( strText == L"")
		return 0;

	if( m_pDC == NULL)
		return 0;

	UINT nWidth = 0;

	if( m_pFont)
	{
		CFont* pOldFont = m_pDC->SelectObject( m_pFont);
		CSize szRet;
		if (GetTextExtentPoint32(m_pDC->GetSafeHdc(), strText.c_str(), (int)strText.length(), &szRet))
			nWidth = szRet.cx;
		else
		{
			nWidth = 0;
			DWORD dwError = GetLastError();
			dwError = 0;
		}

		m_pDC->SelectObject(pOldFont);
		return nWidth;
	}

	return 0;
}

UINT	
CTextLabel::GetCharHeightInfo( wstring strText)
{
	if ( strText == L"")
		return 0;

	if( m_pDC == NULL)
		return 0;

	UINT nHeight = 0;

	if( m_pFont)
	{
		CFont* pOldFont = m_pDC->SelectObject( m_pFont);
		CSize szRet;
		if (GetTextExtentPoint32(m_pDC->GetSafeHdc(), strText.c_str(), (int)strText.length(), &szRet))
			nHeight = szRet.cy + m_nLineSpace;
		else
			nHeight = 0;

		m_pDC->SelectObject(pOldFont);
		return nHeight;
	}

	return 0;
}

wstring		
CTextLabel::GetFittableStringFromFront( wstring wstrText, UINT nWidth, int nBeginIndex, int& nEndIndex)
{
	int i = 0;
	for( ; i <= (int)wstrText.length() - nBeginIndex; i++)
	{
		wstring strTemp = wstrText.substr( nBeginIndex, i);
		UINT nTempWidth = GetCharWidthInfo( strTemp);
		if( nTempWidth > nWidth)
		{
			i--;
			break;
		}
	}

	wstring wstrFittableText = wstrText.substr( nBeginIndex, i);
	nEndIndex = nBeginIndex + i - 1 ;

	return wstrFittableText;
}

wstring 
CTextLabel::GetSplitterString( wstring wstrText)
{
	int nEndIndex = 0;
	wstring wstrResult = L"";

	wstring wstrFittableText = wstrText;
	while( wstrFittableText != L"")
	{
		wstring wstrText = GetFittableStringFromFront( wstrFittableText, m_rcText.Width(), 0, nEndIndex);
		wstrResult += wstrText;

		if( nEndIndex == wstrFittableText.length())
			break;

		wstrResult += wstring(L"\n");
		wstrFittableText = wstrFittableText.substr( nEndIndex+1, wstrFittableText.length() - nEndIndex);
	}

	return wstrResult;
}

void
CTextLabel::Draw( )
{
	if( m_pDC == NULL)
		return;

	ASSERT( m_pFont != NULL);
	if( m_pFont == NULL)
		return;

	int nLineNum = 0;			//	当前输出的行号
	int nEndIndex = -1;
	wstring wstrFittableText = m_strText;
	while( wstrFittableText != L"")
	{
		int nRectTop = m_rcText.top + GetCharHeightInfo(m_strText)* nLineNum + 1 ;
		int nRectButtom = nRectTop + GetCharHeightInfo( m_strText);

		// 如果当前输出的文字底部已经超出了矩形的最底部，那么
		// 如果文字超出了宽度，右边的将显示为...
		bool bExit = false;
		nLineNum++;

		int nBeginIndex = nEndIndex + 1;

		if( nRectButtom + GetCharHeightInfo(m_strText) + 1 > m_rcText.bottom)
		{
			wstrFittableText = GetFittableStringFromFront( m_strText, m_rcText.Width(), nBeginIndex, nEndIndex);
			if( nEndIndex == m_strText.length())
			{
				bExit = true;
			}
			else
			{
				wstrFittableText = GetFittableStringFromFront( m_strText, m_rcText.Width() - 20, nBeginIndex, nEndIndex);
				wstrFittableText += L"...";
				bExit = true;
			}
		}
		else
		{
			wstrFittableText = GetFittableStringFromFront( m_strText, m_rcText.Width(), nBeginIndex, nEndIndex);
			if( nEndIndex == m_strText.length())
				bExit = true;
		}

		CFont* pOldFont = m_pDC->SelectObject(m_pFont);
		COLORREF clr = m_pDC->SetTextColor( m_clrText);
		m_pDC->TextOut( m_rcText.left, nRectTop, wstring_helper::trim(wstrFittableText).c_str());
		m_pDC->SelectObject( pOldFont);

		if(bExit == true)
			break;
	}
}

void	
CTextLabel::DrawLine( BOOL bDraw )
{
	if( m_pDC == NULL)
		return;

	ASSERT( m_pFont != NULL);
	if( m_pFont == NULL)
		return;

	int nLineNum = 0;			//	当前输出的行号
	int nEndIndex = -1;
	wstring wstrFittableText = m_strText;
	while( wstrFittableText != L"")
	{
		int nRectTop = m_rcText.top + GetCharHeightInfo(m_strText)* nLineNum + 1 ;
		int nRectButtom = nRectTop + GetCharHeightInfo( m_strText);

		// 如果当前输出的文字底部已经超出了矩形的最底部，那么
		// 如果文字超出了宽度，右边的将显示为...
		bool bExit = false;
		nLineNum++;

		int nBeginIndex = nEndIndex + 1;

		if( nRectButtom + GetCharHeightInfo(m_strText) + 1 > m_rcText.bottom)
		{
			wstrFittableText = GetFittableStringFromFront( m_strText, m_rcText.Width(), nBeginIndex, nEndIndex);
			if( nEndIndex == m_strText.length())
			{
				bExit = true;
			}
			else
			{
				wstrFittableText = GetFittableStringFromFront( m_strText, m_rcText.Width() - 20, nBeginIndex, nEndIndex);
				wstrFittableText += L"...";
				bExit = true;
			}
		}
		else
		{
			wstrFittableText = GetFittableStringFromFront( m_strText, m_rcText.Width(), nBeginIndex, nEndIndex);
			if( nEndIndex == m_strText.length())
				bExit = true;
		}

		CFont* pOldFont = m_pDC->SelectObject(m_pFont);

		if( bDraw == TRUE )
		{
			CPen linePen( PS_SOLID, 1, m_clrText);
			CPen* pOldPen = m_pDC->SelectObject(&linePen);

			m_pDC->MoveTo( m_rcText.left, nRectButtom );

			CPoint pt;
			pt.x = m_rcText.left + GetCharWidthInfo( wstrFittableText);
			pt.y = nRectButtom;
			m_pDC->LineTo( pt.x, pt.y);

			m_pDC->SelectObject( pOldPen);
			m_pDC->SelectObject( pOldFont);
		}
		else
		{
			CPen linePen( PS_SOLID, 1, RGB(255,255,255));
			CPen* pOldPen = m_pDC->SelectObject(&linePen);

			m_pDC->MoveTo( m_rcText.left, nRectButtom );

			CPoint pt;
			pt.x = m_rcText.left + GetCharWidthInfo( wstrFittableText);
			pt.y = nRectButtom;
			m_pDC->LineTo( pt.x, pt.y);

			m_pDC->SelectObject( pOldPen);
			m_pDC->SelectObject( pOldFont);
		}

		if(bExit == true)
			break;
	}
}