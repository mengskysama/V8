#pragma once
#include "Common.h"
#include "IRender.h"

class GDIRender:public IGDIRender
{
public:
	GDIRender();
	~GDIRender();

	virtual bool Create(HWND hWnd, long lWidth, long lHeight);
	virtual bool PutData(unsigned char* pData, long lWidth, long lHeight,int index=0);
	virtual bool Paint(HWND hWnd,HDC hDC,int x,int y,int index=0,RECT* rc=NULL);
	virtual bool BeginPaint();
	virtual bool EndPaint();

	static ICommonRender *CreateRender(HWND hWnd, long lWidth, long lHeight)
	{
		HRESULT hr = S_OK ;
		GDIRender *p = new GDIRender () ;
		if (!p->Create(hWnd,lWidth,lHeight))
		{
			SAFE_DELETE(p);
			return NULL;
		}
		return p ;
	}
protected:
	HANDLE m_hloc;
	LPBITMAPINFO m_BmpInfo;
	int m_iWidth,m_iHeight,m_iDepth;
	HWND m_hWnd;
	HDC  m_hDC;
	HBITMAP m_hBitmap;
	LPBYTE m_lpBuffer;
	bool m_bInit;
};
