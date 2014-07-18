#include "stdafx.h"
#include "GDIRender.h"
#include "MemDC.h"
#include "MyAssert.h"

GDIRender::GDIRender()
{
	m_iWidth = 0;
	m_iHeight = 0;
	m_hDC = 0;
	m_hBitmap = 0;
	m_hWnd = NULL;
	m_lpBuffer = NULL;
	m_iDepth = 3;//Ä¬ÈÏÍ¼ÏñÉî¶È
	m_bInit = FALSE;
	m_hloc = NULL;
}

GDIRender::~GDIRender()
{
	if (m_hloc)
	{
		GlobalUnlock(m_hloc);
		GlobalFree(m_hloc);  
	}

	if(m_hDC)
	{
		::DeleteDC(m_hDC);
		m_hDC = 0;
	}

	if(m_hBitmap)
	{
		::DeleteObject(m_hBitmap);
		m_hBitmap = 0;
	}
}

bool GDIRender::Create(HWND hWnd, long lWidth, long lHeight)
{
	m_hWnd = hWnd;
	m_iWidth = lWidth;
	m_iHeight = lHeight;

	m_hloc = GlobalAlloc(GMEM_ZEROINIT | GMEM_MOVEABLE,
		sizeof(BITMAPINFOHEADER) + (sizeof(RGBQUAD) * 256));
	if (!m_hloc)
	{
		ErrTrace("GDIRender::Create m_hloc GlobalAlloc Err!\n");
		return false;
	}
		
	m_BmpInfo = (LPBITMAPINFO) GlobalLock(m_hloc);
	if (!m_BmpInfo)
	{
		ErrTrace("GDIRender::Create m_BmpInfo GlobalAlloc Err!\n");
		return false;
	}
	
	int i;
	HANDLE hloc1 = NULL;
	RGBQUAD *argbq = NULL;
	
	hloc1 = LocalAlloc(LMEM_ZEROINIT | LMEM_MOVEABLE,(sizeof(RGBQUAD) * 256));
	if (!hloc1) 
	{
		ErrTrace("GDIRender::Create hloc1 LocalAlloc Err!\n");
		return false;
	}
	
	argbq = (RGBQUAD *) LocalLock(hloc1);
	if (!argbq) return false;
	for(i=0;i<256;i++) {
		argbq[i].rgbBlue=i;
		argbq[i].rgbGreen=i;
		argbq[i].rgbRed=i;
		argbq[i].rgbReserved=0;
	}

	m_BmpInfo->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	m_BmpInfo->bmiHeader.biPlanes = 1;
	m_BmpInfo->bmiHeader.biBitCount = m_iDepth*8;
	m_BmpInfo->bmiHeader.biCompression = BI_RGB;
	m_BmpInfo->bmiHeader.biWidth = m_iWidth;
	m_BmpInfo->bmiHeader.biHeight = m_iHeight;

	memcpy(m_BmpInfo->bmiColors, argbq, sizeof(RGBQUAD) * 256);

	LocalUnlock(hloc1);
	LocalFree(hloc1);
	
	try
	{
		HDC hDestDC = ::GetDC(m_hWnd);
		m_hDC = ::CreateCompatibleDC( hDestDC );
		m_hBitmap = CreateDIBSection(m_hDC,m_BmpInfo, DIB_RGB_COLORS,(LPVOID*)&m_lpBuffer, NULL, NULL);
		::ReleaseDC(m_hWnd,hDestDC);
	}
	catch (...)
	{
		ErrTrace("GDIRender::Create m_hDC CreateCompatibleDC Err!\n");
		return false;
	}
	
	m_bInit = TRUE;

	return true;
}

bool GDIRender::PutData(unsigned char* pData, long lWidth, long lHeight,int index)
{
	if (!m_bInit) return false;
	if (lWidth!=m_iWidth || lHeight!=m_iHeight) 
	{	
		m_iWidth = lWidth;
		m_iHeight = lHeight;
		
		m_BmpInfo->bmiHeader.biWidth = m_iWidth;
		m_BmpInfo->bmiHeader.biHeight = m_iHeight;

		if(m_hDC)
		{
			::DeleteDC(m_hDC);
			m_hDC = 0;
		}

		if(m_hBitmap)
		{
			::DeleteObject(m_hBitmap);
			m_hBitmap = 0;
		}

		try
		{
			HDC hDestDC = ::GetDC(m_hWnd);
			m_hDC = ::CreateCompatibleDC( hDestDC );
			m_hBitmap = CreateDIBSection(m_hDC,m_BmpInfo, DIB_RGB_COLORS,(LPVOID*)&m_lpBuffer, NULL, NULL);
			::ReleaseDC(m_hWnd,hDestDC);
		}
		catch (...)
		{
			ErrTrace("GDIRender::Create m_hDC CreateCompatibleDC Err!\n");
			return false;
		}
	}
	
	memcpy(m_lpBuffer,pData,m_iWidth*m_iHeight*m_iDepth);

	return true;
}

bool GDIRender::Paint(HWND hWnd,HDC hDC,int x,int y,int index,RECT*rc) 
{
	m_BmpInfo->bmiHeader.biBitCount = m_iDepth*8;
	CRect rcRect(rc);
	HBITMAP hOld = (HBITMAP)::SelectObject( m_hDC, m_hBitmap );
	int iOldMode = SetStretchBltMode(hDC, COLORONCOLOR); 
	::StretchBlt( hDC,rc->left,rc->top,rc->right-rc->left,rc->bottom-rc->top, m_hDC, 0, 0, m_BmpInfo->bmiHeader.biWidth, m_BmpInfo->bmiHeader.biHeight, SRCCOPY);
	SetStretchBltMode(hDC, iOldMode); 
	::SelectObject(m_hDC,hOld);

	return true;
}

bool GDIRender::BeginPaint()
{
	return true;
}

bool GDIRender::EndPaint()
{
	return true;
}