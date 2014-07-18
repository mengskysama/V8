#include "stdafx.h"
#include "GDIPlusRender.h"
#include "MemDC.h"
#include <gdiplus.h>
#include "MyAssert.h"
using namespace Gdiplus;


static ColorMatrix colorMatrix;
static bool bSet = false;

GDIPlusRender::GDIPlusRender()
{
	m_bInit = false;
	m_hModule = NULL;

	if (!bSet)
	{
		REAL f = 1.07f;//对比度系数
		REAL b = 0.00f;//亮度系数
		ColorMatrix colorMatrix_tmp = {
			f, 0, 0, 0, 0,
			0, f, 0, 0, 0,
			0, 0, f, 0, 0,
			0, 0, 0, 1, 0,
			0.5f*(1-f)+b, 0.5f*(1-f)+b, 0.5f*(1-f)+b, 0, 1 };

		colorMatrix = colorMatrix_tmp;

		bSet = true;
	}
	
}

GDIPlusRender::~GDIPlusRender()
{
	Clear();
	if (m_hModule)
		FreeLibrary(m_hModule);
}

bool GDIPlusRender::Create(HWND hWnd, long lWidth, long lHeight)
{
	if( !::IsWindow(hWnd) ) return FALSE;
	
	m_hModule = ::LoadLibraryA("GdiPlus.dll");
	if (!m_hModule)//GDI+ DLL加载失败，环境可能不支持
	{
		ErrTrace("GDIPlusRender::Create 加载库失败~!\n");
		return false;
	}

	#pragma comment(lib,"gdiplus.lib")
	
	Clear();

	m_hWnd = hWnd;

	return CreateDIB(lWidth,lHeight);
}

bool GDIPlusRender::PutData(unsigned char* pData, long lWidth, long lHeight,int index)
{
	if (m_sizeBMP.cx != lWidth || m_sizeBMP.cy != lHeight || !m_pBuffer)
	{
		Clear();
		bool bret = ModelImgToDIB(lWidth,lHeight,m_nDepth,m_hWnd,&m_pBuffer,m_hDC,m_hBitmap,(int&)m_sizeBMP.cx,(int&)m_sizeBMP.cy,m_nDepth);
		if (!bret)
		{
			ErrTrace("GDIPlusRender::PutData ModelImgToDIB Err~!\n");
			return bret;
		}
			
	}

	memcpy(m_pBuffer,pData,lWidth*lHeight*m_nDepth);

	return true;
}

bool GDIPlusRender::Paint(HWND hWnd,HDC hDC,int x,int y,int index,RECT*rc) 
{
	if (!rc) return false;

	CDC *pDC=CDC::FromHandle(hDC);
	CRect rcRect(rc);
	CMemDC memDC(pDC,&rcRect,FALSE);
	Rect rct(rc->left,rc->top,rc->right,rc->bottom);
	Bitmap bitmap(m_hBitmap,NULL);
	Graphics graphics(memDC->GetSafeHdc());
	graphics.SetSmoothingMode(SmoothingModeHighQuality);
	//graphics.SetInterpolationMode(InterpolationModeHighQualityBicubic);

	//ImageAttributes imageAttr;
	//imageAttr.SetColorMatrix(&colorMatrix);

	ImageAttributes imageAttr;
	imageAttr.SetWrapMode(WrapModeTileFlipXY);//GDI+在位伸图片时会自动加上渐变效果。但此处不需要，所以得加上此属性

	graphics.DrawImage(&bitmap,rct, 0, 0, bitmap.GetWidth(), bitmap.GetHeight(), UnitPixel, &imageAttr);

	return true;
}

bool GDIPlusRender::BeginPaint()
{
	return true;
}

bool GDIPlusRender::EndPaint()
{
	return true;
}

bool GDIPlusRender::ModelImgToDIB(int nWidth,int nHeight,int nDepth,HWND hWnd,unsigned char** ppBuffer,HDC& hDC,
				   HBITMAP& hBitmap,int& w,int& h,int& nDstDepth)
{
	w = nWidth;
	h = nHeight;
	nDstDepth = nDepth;
	HDC hDestDC = ::GetDC(hWnd);
	hDC = ::CreateCompatibleDC(hDestDC);

	const int BITMAPINFO_QUAD_SIZE = sizeof(BITMAPINFO) + 2*sizeof(RGBQUAD);
	BYTE* abBitmapInfo[BITMAPINFO_QUAD_SIZE];
	BITMAPINFO* pBMI = (BITMAPINFO*) abBitmapInfo;
	memset(pBMI,0,BITMAPINFO_QUAD_SIZE);

	pBMI->bmiHeader.biSize		=	sizeof(BITMAPINFOHEADER);
	pBMI->bmiHeader.biWidth		=	nWidth;
	pBMI->bmiHeader.biHeight	=   nHeight;
	pBMI->bmiHeader.biPlanes	=   1;
	pBMI->bmiHeader.biBitCount  =	nDepth*8;

	hBitmap = ::CreateDIBSection(hDC,pBMI,DIB_RGB_COLORS,(void**)ppBuffer,NULL,NULL);

	::ReleaseDC(hWnd,hDestDC);

	return true;
}

bool GDIPlusRender::CreateDIB(int nWidth,int nHeight,int nDepth)
{
	Clear();
	return ModelImgToDIB(nWidth,nHeight,nDepth,m_hWnd,&m_pBuffer,m_hDC,m_hBitmap,(int&)m_sizeBMP.cx,(int&)m_sizeBMP.cy,m_nDepth);
}

void GDIPlusRender::Clear()
{
	if (m_hBitmap)
		::DeleteObject(m_hBitmap);
	m_hBitmap = 0;
	if (m_hDC)
		::DeleteDC(m_hDC);
	m_hDC = 0;
	m_pBuffer = 0;
}
