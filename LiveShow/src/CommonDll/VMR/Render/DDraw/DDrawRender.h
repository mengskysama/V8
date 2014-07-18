#pragma once
#include "Common.h"
#include "IRender.h"
#include "IDDraw.h"
#include "RGB2YUV.h"


class DDrawRender:public IDDrawRender
{
public:
	DDrawRender();
	~DDrawRender();

	virtual bool Create(HWND hWnd, long lWidth, long lHeight);
	virtual bool PutData(unsigned char* pData, long lWidth, long lHeight,int index=0);
	virtual bool Paint(HWND hWnd,HDC hDC,int x,int y,int index=0,RECT* rc=NULL);
	virtual bool BeginPaint();
	virtual bool EndPaint();

	static ICommonRender *CreateRender(HWND hWnd, long lWidth, long lHeight)
	{
		HRESULT hr = S_OK ;
		DDrawRender *p = new DDrawRender () ;
		if (!p->Create(hWnd,lWidth,lHeight))
		{
			SAFE_DELETE(p);
			return NULL;
		}
		return p ;
	}

private:
	IDDraw* m_pIDDraw;
	BYTE* m_pBuffer;
	long m_lWidth;
	long m_lHeight;
	HWND m_hWnd;
	bool m_bInit;
	bool m_bPutData;
	CRGB2YUV m_rgb2yuv;
};