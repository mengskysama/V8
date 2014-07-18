#pragma once

#include <ddraw.h>
#include "IDDraw.h"
#include <atltypes.h>

class CDirectDraw:public IDDraw
{
public:
	CDirectDraw();
	virtual ~CDirectDraw();

	BOOL Init(HWND hwnd,int width,int height);
	BOOL ChgDataSize(int width,int height);
	void Release(void);
	BOOL Draw(HWND hwnd,void *buffer);

protected:
	void CopyToDDraw(void* destination_buffer,void* source_buffer);

private:
	DDSURFACEDESC2          ddsd;			//DirectDraw表面描述结构体
	LPDIRECTDRAW7           lpDD;			//DirectDraw对象指针
	LPDIRECTDRAWSURFACE7    lpDDSPrimary;	//DirectDraw主表面指针
	LPDIRECTDRAWSURFACE7    lpDDSOffscreen;	//DirectDraw离屏表面指针
	LPDIRECTDRAWCLIPPER     lpClipper;		//DirectDraw裁剪对象

	int                    bitmap_width;
	int                    bitmap_height;
	HMODULE				   hModue;

	CRect rctSour;
	CRect rcDest;
};


