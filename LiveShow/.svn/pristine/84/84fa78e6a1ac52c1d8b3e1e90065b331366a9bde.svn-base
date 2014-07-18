#include "stdafx.h"
#include "DIBRectCalc.h"
#include "MyAssert.h"

DIBRectCalc::DIBRectCalc()
:m_nWidth(0),
 m_nHeight(0)
{

}

DIBRectCalc::~DIBRectCalc()
{

}

void DIBRectCalc::Initialize()
{
	m_nWidth = 0;
	m_nHeight = 0;
}

void DIBRectCalc::AddHWnd(HWND hWnd)
{
	CRect rc;
	if (!::GetWindowRect(hWnd,&rc))
	{
		Assert(0);
	}
	
	m_nWidth = rc.Width() > m_nWidth?rc.Width():m_nWidth;
	m_nHeight = rc.Height() > m_nHeight?rc.Height():m_nHeight;
}

CRect DIBRectCalc::GetRectInfo()
{
	CRect rc;
	rc.left = rc.top = 0;
	if (m_nWidth <= 224 && m_nHeight <= 168)
	{
		rc.right = 224;
		rc.bottom = 168;
	}
	else if (m_nWidth < 640 && m_nHeight < 480)
	{
		rc.right = 320;
		rc.bottom = 240;
	}
	else
	{
		rc.right = 640;
		rc.bottom = 480;
	}
	return rc;
}