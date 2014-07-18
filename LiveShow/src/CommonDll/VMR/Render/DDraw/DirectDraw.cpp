#include "stdafx.h"
#include "DirectDraw.h"

CDirectDraw::CDirectDraw()
{
	memset(&this->ddsd,0,sizeof(this->ddsd));// 清空DirectDraw表面描述结构体
	this->lpDD=NULL; 						 // DirectDraw对象指针清空
	this->lpDDSPrimary=NULL;				 // DirectDraw主表面指针清空
	this->lpDDSOffscreen=NULL; 				 // DirectDraw离屏表面指针清空
	this->lpClipper=NULL; 					 // DirectDraw裁剪对象清空
	this->hModue=NULL;

	this->bitmap_width=0;					 // 待显示图象的宽度
	this->bitmap_height=0;					 // 待显示图象的高度
}

CDirectDraw::~CDirectDraw()
{
	this->Release( );
	if (this->hModue)
		FreeLibrary(this->hModue);
	this->hModue = NULL;
}

BOOL CDirectDraw::Init(HWND hwnd , int width , int height)
{
	if( !::IsWindow(hwnd) ) return FALSE;
	
	this->hModue = ::LoadLibraryA("ddraw.dll");
	if (this->hModue)
	{
		#pragma comment(lib,"ddraw.lib")
		#pragma comment(lib,"dxguid.lib")
	}else{
		return FALSE;
	}
	
	if (DirectDrawCreateEx(
		NULL,								
		(VOID**)&lpDD,								//接受初始化的DirectDraw对象的地址
		IID_IDirectDraw7,							//IID_IDirectDraw7，当前版本
		NULL) != DD_OK) 					
	{
		return FALSE;
	}

	if( FAILED ( this->lpDD->SetCooperativeLevel(   //设置DirectDraw控制级
		hwnd,										//与DirectDraw对象联系主窗口
		DDSCL_NORMAL | DDSCL_NOWINDOWCHANGES ) ) )  //控制级标志
	{
		return FALSE;
	}

	ZeroMemory(&ddsd,sizeof(ddsd));		
	ddsd.dwSize = sizeof(ddsd);						//DirectDraw表面描述结构体大小
	ddsd.dwFlags = DDSD_CAPS;						//设定DDSURFACEDESC2结构中的ddsCaps有效
	ddsd.ddsCaps.dwCaps = DDSCAPS_PRIMARYSURFACE;	//主表面

	if (lpDD->CreateSurface( &ddsd,					//被填充了表面信息的DDSURFACEDESC2结构的地址
		&lpDDSPrimary,								//接收主表面指针
		NULL) != DD_OK)	
	{
		return FALSE;
	}

	
	if( lpDD->CreateClipper( 0, 					//创裁减器
		&lpClipper,									//指向剪裁器对象的指针
		NULL ) != DD_OK )	
		return FALSE;
	
	if( lpClipper->SetHWnd( 0, hwnd ) != DD_OK )	//关联裁减器与显示窗
	{
		lpClipper->Release();
		return FALSE;
	}
	
	if( lpDDSPrimary->SetClipper( lpClipper ) != DD_OK ) //把裁减器加到主表面
	{
		lpClipper->Release();
		return FALSE;
	}

	lpClipper->Release();

	ZeroMemory(&ddsd, sizeof(ddsd));				// 创建YUV表面
	ddsd.dwSize = sizeof(ddsd);
	
	ddsd.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN;	// 离屏表面//DDSCAPS_OVERLAY | DDSCAPS_OFFSCREENPLAIN;
	ddsd.dwFlags = DDSD_CAPS | DDSD_HEIGHT | DDSD_WIDTH | DDSD_PIXELFORMAT;// 填充标志 宽、高与像素结构
	ddsd.dwWidth = width;							//离屏显示的宽
	ddsd.dwHeight = height;							//离屏显示的高
	ddsd.ddpfPixelFormat.dwFlags  = DDPF_FOURCC | DDPF_YUV ;		 
	ddsd.ddpfPixelFormat.dwFourCC = MAKEFOURCC('Y','V', '1', '2');	 
	ddsd.ddpfPixelFormat.dwYUVBitCount = 8;							 

	if (lpDD->CreateSurface(&ddsd, &lpDDSOffscreen, NULL) != DD_OK) // 创建YUV表面
	{
		lpDDSOffscreen = NULL;
		return FALSE;
	}

	this->bitmap_width = width;						//图像的宽
	this->bitmap_height = height;					//图像的高

	rctSour.left = 0;								// 待显示的图像窗口
	rctSour.top = 0;
	rctSour.right = ddsd.dwWidth;
	rctSour.bottom = ddsd.dwHeight;

	return TRUE;
}

BOOL CDirectDraw::ChgDataSize(int width,int height)
{
	if( this->lpDDSOffscreen ){		
		this->lpDDSOffscreen->Release(); this->lpDDSOffscreen=NULL;  
	}
	
	SwitchToThread();

	ZeroMemory(&ddsd, sizeof(ddsd));				// 创建YUV表面
	ddsd.dwSize = sizeof(ddsd);

	ddsd.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN;	// 离屏表面//DDSCAPS_OVERLAY | DDSCAPS_OFFSCREENPLAIN;
	ddsd.dwFlags = DDSD_CAPS | DDSD_HEIGHT | DDSD_WIDTH | DDSD_PIXELFORMAT;// 填充标志 宽、高与像素结构
	ddsd.dwWidth = width;							//离屏显示的宽
	ddsd.dwHeight = height;							//离屏显示的高
	ddsd.ddpfPixelFormat.dwFlags  = DDPF_FOURCC | DDPF_YUV ;		 
	ddsd.ddpfPixelFormat.dwFourCC = MAKEFOURCC('Y','V', '1', '2');	 
	ddsd.ddpfPixelFormat.dwYUVBitCount = 8;							 

	if (lpDD->CreateSurface(&ddsd, &lpDDSOffscreen, NULL) != DD_OK) // 创建YUV表面
	{
		lpDDSOffscreen = NULL;
		return FALSE;
	}

	this->bitmap_width = width;						//图像的宽
	this->bitmap_height = height;					//图像的高

	rctSour.left = 0;								// 待显示的图像窗口
	rctSour.top = 0;
	rctSour.right = ddsd.dwWidth;
	rctSour.bottom = ddsd.dwHeight;

	return true;
}

void CDirectDraw::Release( void )
{
	if( this->lpClipper ){			
		this->lpClipper->Release(); this->lpClipper=NULL;
	}
	if( this->lpDDSOffscreen ){		
		this->lpDDSOffscreen->Release(); this->lpDDSOffscreen=NULL;  
	}
	if( this->lpDDSPrimary ) {
		this->lpDDSPrimary->Release(); this->lpDDSPrimary=NULL;
	}
	if( this->lpDD ){		
		this->lpDD->Release(); this->lpDD=NULL;  
	}
}

BOOL CDirectDraw::Draw(HWND hwnd, void * buffer)
{
	HRESULT ddRval;

	if( buffer==NULL) 
		return FALSE;

	GetClientRect(hwnd,&rcDest);					// 获取目标客户区坐标
	ClientToScreen(hwnd, (LPPOINT)&rcDest.left); 
	ClientToScreen(hwnd, (LPPOINT)&rcDest.right);

	do {
		ddRval = lpDDSOffscreen->Lock(NULL, 
			&ddsd,	
			DDLOCK_WAIT | DDLOCK_WRITEONLY,
			NULL);	
	} while(ddRval == DDERR_WASSTILLDRAWING);

	if(ddRval != DD_OK)
		return FALSE;

	CopyToDDraw( (LPBYTE)ddsd.lpSurface , buffer); 	// 拷贝待显示图像到主表面内存
	lpDDSOffscreen->Unlock(NULL);					// 解锁离屏表面
	if (this->lpDDSPrimary->Blt( &rcDest , this->lpDDSOffscreen , rctSour, DDBLT_WAIT, NULL )!=DD_OK)// 将离屏表面的YUV源图像（rctSour）画到主表面的rcDest目标区
	{
		return FALSE;
	}
	return TRUE;
}

void CDirectDraw::CopyToDDraw( void * destination_buffer , void * source_buffer )
{
	if( ! destination_buffer || ! source_buffer ) 
		return;

	unsigned int i;
	BYTE* lpSurf = (BYTE *)destination_buffer;

	BYTE* lpY = (BYTE *)source_buffer;
	BYTE* lpV = (BYTE *)source_buffer + bitmap_width * bitmap_height;
	BYTE* lpU = (BYTE *)source_buffer + bitmap_width * bitmap_height * 5 / 4;	

	// fill Y data
	for(i=0; i<ddsd.dwHeight; i++)
	{
		memcpy(lpSurf, lpY, ddsd.dwWidth);
		lpY += bitmap_width;
		lpSurf += ddsd.lPitch;
	}

	// fill V data
	for(i=0; i<ddsd.dwHeight/2; i++)
	{
		memcpy(lpSurf, lpV, ddsd.dwWidth / 2);
		lpV += bitmap_width / 2;
		lpSurf += ddsd.lPitch / 2;
	}

	// fill U data
	for(i=0; i<ddsd.dwHeight/2; i++)
	{
		memcpy(lpSurf, lpU, ddsd.dwWidth / 2);
		lpU += bitmap_width / 2;
		lpSurf += ddsd.lPitch / 2;
	}

}