#include "stdafx.h"
#include "../Hall.h"
#include "PicShowCtrl.h"
#include "..\src\CommonLibrary\include\ui\UIHelper.h"
#include "..\src\CommonLibrary\thirdlibrary\CxImage\include\ximage.h"
#include "..\CommonLibrary\include\utility\StringHelper.h"

CPicShowCtrl::CPicShowCtrl(void)
{
	m_hBmp = NULL;
	m_iStartx = 0;
	m_iStarty = 0;
}

CPicShowCtrl::~CPicShowCtrl(void)
{
	if (m_hBmp != NULL) 
	{
		::DeleteObject(m_hBmp);
		m_hBmp = NULL;
	}
}

void CPicShowCtrl::SetBitmap(HBITMAP hBmp)
{
	if (m_hBmp != NULL) 
	{
		::DeleteObject(m_hBmp);
		m_hBmp = NULL;
	}

	m_hBmp = hBmp;
}

bool CPicShowCtrl::Create(CWnd* pwndParent, CRect rc, UINT uId)
{
	return CWnd::Create(AfxRegisterWndClass(0), NULL, WS_VISIBLE | WS_CHILD, rc, pwndParent, uId);
}

BEGIN_MESSAGE_MAP(CPicShowCtrl, CWnd)
	ON_WM_PAINT()
END_MESSAGE_MAP()

void CPicShowCtrl::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: 在此处添加消息处理程序代码
	common::ui::CClientRect rcThis(this);
	if (m_hBmp == NULL)
	{
		//还没有选择图片,则绘制白色背景
		HBRUSH bgBrush = ::CreateSolidBrush(RGB(255,255,255));
		CRect rect;
		GetWindowRect(&rcThis);
		ScreenToClient(&rcThis);
		FillRect(dc,&rcThis,bgBrush);
		::DeleteObject(bgBrush);
	}
	else
	{
		int width=180;
		int height=180;

		CxImage img;
		img.CreateFromHBITMAP(m_hBmp);

		CBitmap bitmapmem;          
		CBitmap *pOldBit;
		CDC m_pMemDC;
		m_pMemDC.CreateCompatibleDC(&dc); 
		bitmapmem.CreateCompatibleBitmap(&dc, width, height);
		pOldBit=m_pMemDC.SelectObject(&bitmapmem);

		CRect rect(0,0,width,height);
		HBRUSH bgBrush = ::CreateSolidBrush(RGB(255,255,255));
		FillRect(m_pMemDC,&rect,bgBrush);
		DeleteObject(bgBrush);

		img.Draw(m_pMemDC,m_iStartx,m_iStarty,img.GetWidth(),img.GetHeight(),&rect);
		dc.SetStretchBltMode(HALFTONE);
		dc.StretchBlt(rcThis.left,rcThis.top,rcThis.Width(),rcThis.Height(),&m_pMemDC,0,0,width,height,SRCCOPY);
		m_pMemDC.SelectObject(pOldBit);
		m_pMemDC.DeleteDC();
		bitmapmem.DeleteObject();
	}
}


bool CPicShowCtrl::SaveFile(CString strPath)
{
	int width=180;
	int height=180;

	CPaintDC dc(this);
	CxImage img;
	img.CreateFromHBITMAP(m_hBmp);

	CBitmap bitmapmem;          
	CBitmap *pOldBit;
	CDC m_pMemDC;
	m_pMemDC.CreateCompatibleDC(&dc); 
	bitmapmem.CreateCompatibleBitmap(&dc, width, height);
	pOldBit=m_pMemDC.SelectObject(&bitmapmem);

	CRect rect(0,0,width,height);
	HBRUSH bgBrush = ::CreateSolidBrush(RGB(255,255,255));
	FillRect(m_pMemDC,&rect,bgBrush);
	DeleteObject(bgBrush);
	img.Draw(m_pMemDC,m_iStartx,m_iStarty,img.GetWidth(),img.GetHeight(),&rect);

	CBitmap* pBmp=m_pMemDC.SelectObject(pOldBit);

	CxImage xImagebmp;
	xImagebmp.CreateFromHBITMAP((HBITMAP)bitmapmem.m_hObject);
	xImagebmp.Resample(100,100,0);  
	bitmapmem.DeleteObject();
	m_pMemDC.DeleteDC();
	if(xImagebmp.Save(common::utility::stringhelper::UnicodeToAscii(strPath.GetBuffer()).c_str(), CXIMAGE_FORMAT_JPG))
	{
		return true;
	}
	return false;
}

void CPicShowCtrl::SetStartx(int iStartx)
{
	m_iStartx = iStartx;
}

void CPicShowCtrl::SetStarty(int iStarty)
{
	m_iStarty = iStarty;
}
