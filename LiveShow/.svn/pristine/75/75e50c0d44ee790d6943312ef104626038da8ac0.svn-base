#include "stdafx.h"
#include "DDrawRender.h"
#include "DirectDraw.h"

DDrawRender::DDrawRender()
{
	m_pIDDraw = NULL;
	m_pBuffer = NULL;
	m_lWidth = 0;
	m_lHeight = 0;
	m_hWnd  = NULL;
	m_bInit = FALSE;
	m_bPutData = FALSE;
}

DDrawRender::~DDrawRender()
{
	if (m_pIDDraw) 
		m_pIDDraw->Release();
	SAFE_DELETE(m_pBuffer);
	SAFE_DELETE(m_pIDDraw);
}

bool DDrawRender::Create(HWND hWnd, long lWidth, long lHeight)
{
	if (!m_pIDDraw)
	{
		m_pIDDraw = new CDirectDraw();
	}

	//Assert(m_pIDDraw);
	if (!m_pIDDraw)
	{
		return false;
	}

	m_lWidth = lWidth;
	m_lHeight = lHeight;
	m_hWnd  = hWnd;

	try
	{
		m_pBuffer = new BYTE[m_lWidth*m_lHeight*3/2];
	}
	catch (...)
	{
		return false;
	}

	m_bInit = m_pIDDraw->Init(m_hWnd,m_lWidth,m_lHeight);
	return m_bInit;
}

bool DDrawRender::PutData(unsigned char* pData, long lWidth, long lHeight,int index)
{
	if (!m_bInit || !m_pIDDraw) 
		return false;
	
	if (m_bPutData) 
		return true;
	
	//图像大小改变自动修改缓冲区大小
	if (m_lWidth!=lWidth || m_lHeight!=lHeight)
	{
		bool bret = m_pIDDraw->ChgDataSize(lWidth,lHeight);
		if (bret)
		{
			m_lWidth = lWidth;
			m_lHeight = lHeight;
		}
		else
		{
			return bret;
		}
	}

	m_rgb2yuv.Rgb2Yuv((unsigned int)m_lWidth,(unsigned int)m_lHeight,pData,(unsigned char*)m_pBuffer);
	//memcpy(m_pBuffer,pData,m_lWidth*m_lHeight*3/2);

	m_bPutData = TRUE;

	return true;
}

bool DDrawRender::Paint(HWND hWnd,HDC hDC,int x,int y,int index,RECT*rc) 
{
	if (!m_bInit || !m_pIDDraw) return false;
	
	m_pIDDraw->Draw(hWnd,m_pBuffer);

	m_bPutData = FALSE;

	return true;
}

bool DDrawRender::BeginPaint()
{
	if (!m_bInit || !m_pIDDraw) return false;

	return true;
}

bool DDrawRender::EndPaint()
{
	if (!m_bInit || !m_pIDDraw) return false;

	return true;
}