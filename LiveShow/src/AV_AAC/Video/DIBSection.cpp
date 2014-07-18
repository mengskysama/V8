// DIBSection.cpp: implementation of the DIBSection class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DIBSection.h"
#include "MyAssert.h"
#include "Global.h"


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

DIBSection::DIBSection()
:mhDC(0),mhBitmap(0),mpBuffer(0),m_dwLastFillDataTime(GetTickCount()),m_bPhotoable(false),mbShowEncodeVideo(false),mbFFT(false),mnRoomId(0),m_bNew(false),m_bIsRenderInit(false)
{
	for(unsigned short i=0;i<=255;i++)
	{
		unsigned int ave = (unsigned int)(1.05f*i);
		if(ave > 255)
			m_whiteBalanceArray[i] = 255;
		else
			m_whiteBalanceArray[i] = ave;
	}
	pCommRender = NULL;
	pRenderImplement = IRender::GetInstance();
}

DIBSection::~DIBSection()
{
	Clear();
	pRenderImplement = NULL;
	SAFE_DELETE(pCommRender);
}
bool DIBSection::ModelImgToDIB(int nWidth,int nHeight,int nDepth, HWND hWnd,unsigned char** ppBuffer,HDC& hDC,
							  HBITMAP& hBitmap,int& w,int& h,int& nDstDepth)
{	
	w = nWidth;
	h = nHeight;
	nDstDepth = nDepth;
	HDC hDestDC = ::GetDC(hWnd);
    hDC = ::CreateCompatibleDC( hDestDC );
	
	const int BITMAPINFO_QUAD_SIZE = sizeof(BITMAPINFO) + 2 * sizeof(RGBQUAD);
	BYTE* abBitmapInfo[BITMAPINFO_QUAD_SIZE] ;
    BITMAPINFO* pBMI = (BITMAPINFO*) abBitmapInfo;
    memset(pBMI, 0, BITMAPINFO_QUAD_SIZE );
	
    pBMI->bmiHeader.biSize 			= sizeof(BITMAPINFOHEADER);
    pBMI->bmiHeader.biWidth 		= nWidth;
	pBMI->bmiHeader.biHeight 		= nHeight;
	
    pBMI->bmiHeader.biPlanes 		= 1;
    pBMI->bmiHeader.biBitCount 		= nDepth*8;	
	
	hBitmap= ::CreateDIBSection(hDC, pBMI, DIB_RGB_COLORS,(void **)ppBuffer, NULL, NULL);
	
	::ReleaseDC(hWnd,hDestDC);	
	
	return true;
}
void DIBSection::Clear()
{
	if(mhBitmap)
		::DeleteObject(mhBitmap);
	mhBitmap = 0;
	if(mhDC)
		::DeleteDC(mhDC);
	mhDC = 0;
	mpBuffer = 0;
}

//  黑白平衡 + 亮度增强
void DIBSection::WhilteBalance(unsigned char* rgbImg, unsigned char* chImg, int rows, int cols)
{
	int count = rows*cols*3;
	for(int i=0;i<count;i++)
	{
		chImg[i]  = m_whiteBalanceArray[rgbImg[i]];
	}
//测试用
// 	for (register int i = 0; i < rows; i++){
// 		for (register int j = 0; j < cols/2; j++){
// 			const unsigned int m = i*cols*3+3*j;
// 			chImg[m]  = m_whiteBalanceArray[rgbImg[m]];
// 			chImg[m+1]  = m_whiteBalanceArray[rgbImg[m+1]];
// 			chImg[m+2]  = m_whiteBalanceArray[rgbImg[m+2]];
// 			if (chImg[m] + chImg[m+1] + chImg[m+2] < 150 )
// 			{
// 				chImg[m]  = chImg[m]<7?0:chImg[m]-2;
// 				chImg[m+1]  = chImg[m+1]<9?0:chImg[m+1]-9;
// 				chImg[m+2]  = chImg[m+2];
// 				continue;
// 			}
// 			else if (chImg[m]>240 && chImg[m+1]>240 && chImg[m+2]>240)
// 			{
// 				continue;
// 			}
// 			else if (chImg[m]>210 && chImg[m+1]>210 && chImg[m+2]>210)
// 			{
// 				chImg[m] = min(chImg[m]+7,255);
// 				chImg[m+1] = min(chImg[m+1]+1,255);
// 				chImg[m+2] = min(chImg[m+2]+9,255);
// 				continue;
// 			}
// 			chImg[m]  = min(chImg[m]+7,255);
// 			chImg[m+1]  = chImg[m+1]>0?chImg[m+1]-1:0;
// 			chImg[m+2]  = min(chImg[m+2]+9,255);
//  		}
// 	}
}

#define WIDTHBYTES(bit) (((bit)+31)/32 * 4)
#define BOUND(x, mn, mx)	((x) < (mn) ? (mn) : ((x) > (mx) ? (mx) : (x)))

// IncreaseContrast 
void IncreaseContrast(BYTE *pByte, const int Low, const int High, const float Grad)
{ 
	if (*pByte <= Low) 
		*pByte = 0; 
	else if ((Low < *pByte) && (*pByte <  High))
		*pByte = (BYTE)( (*pByte - Low) / Grad); 
	else // pElem->rgbGreen >= High 
		*pByte = 255; 
}

// DecreaseContrast 
void DecreaseContrast(BYTE *pByte, const int Level, const float Grad)
{ 
	Assert(pByte); 
	Assert(Grad != 0.0); 
	*pByte = (BYTE) ( ((int) (*pByte / Grad)) - Level); 
}

//亮度调节
void ChangeBrightness(int nDelta, BYTE *r, BYTE *g, BYTE *b)
{
	int R = *r + nDelta;
	int G = *g + nDelta;
	int B = *b + nDelta;

	*r = (BYTE)BOUND(R, 0, 255);
	*g = (BYTE)BOUND(G, 0, 255);
	*b = (BYTE)BOUND(B, 0, 255);
}

//对比度调节
void ChangeContrast(int nDelta, BYTE *r, BYTE *g, BYTE *b)
{
	if (nDelta >= 0)
	{
		int Low  = 0 + nDelta; 
		int High = 255 - nDelta; 
		float Grad= ((float)(High - Low)) / 255; 
		IncreaseContrast(r, Low, High, Grad); 
		IncreaseContrast(g, Low, High, Grad); 
		IncreaseContrast(b, Low, High, Grad); 
	}
	else
	{
		float Grad = 255 / (255 + (float)nDelta + (float)nDelta); 
		DecreaseContrast(r, nDelta, Grad); 
		DecreaseContrast(g, nDelta, Grad); 
		DecreaseContrast(b, nDelta, Grad); 
	}
}

void AdjustDIBBrightness(unsigned char* rgbImg, unsigned char* chImg, int rows, int cols,const int v)
{
	int nDelta = WIDTHBYTES(24*cols) - cols*24/8;
	BYTE r, g, b;
	for (int ny=0; ny<rows; ny++)
	{
		for (int nx=0; nx<cols/2; nx++){
			const unsigned int m = ny*cols*3+3*nx;
			b = rgbImg[m];
			g = rgbImg[m+1];
			r = rgbImg[m+2];
			ChangeBrightness(v, &r, &g, &b);
			//ChangeContrast(-v+10, &r, &g, &b);
			rgbImg[m] = b;
			rgbImg[m+1] = g;
			rgbImg[m+2] = r;
		}
	}
}

void DIBSection::Paint(HDC hDC,int x,int y)
{
	if(!hDC||!mhDC||!mhBitmap||!::IsWindow(mhWnd))
		return;

	RECT rect;
	if (!::GetClientRect(mhWnd, &rect))
	{
		rect.left  = 0;
		rect.right = 320;
		rect.top   = 0;
		rect.bottom = 240;
	}

	int rcWidth = rect.right - rect.left;
	int rcHeight = rect.bottom-rect.top;

	if (m_bNew)
	{
		//WhilteBalance(mpBuffer,mpBuffer,msizeBMP.cy,msizeBMP.cx);
		//AdjustDIBBrightness(mpBuffer,mpBuffer,msizeBMP.cy,msizeBMP.cx,10);
	}

	if (pRenderImplement && pCommRender)
	{
		pCommRender->BeginPaint();
		if (!pCommRender->Paint(mhWnd,hDC,x,y,0,&rect))
		{
			ErrTrace("DIBSection::Paint 绘制图形失败...\n");
		}
		pCommRender->EndPaint();
	}
	else
	{
		HBITMAP hOld = (HBITMAP)::SelectObject( mhDC, mhBitmap );
		int iOldMode = SetStretchBltMode(hDC, COLORONCOLOR); 
		::StretchBlt( hDC, x, y, rcWidth, rcHeight, mhDC, 0, 0, msizeBMP.cx, msizeBMP.cy, SRCCOPY);

		 SetStretchBltMode(hDC, iOldMode); 
		::SelectObject(mhDC,hOld);

	}
	m_bNew = false;
}
void DIBSection::Create(HWND hWnd,int nWidth, int nHeight, int nDepth,bool bFFTWav)
{
	Clear();
	ModelImgToDIB(nWidth,nHeight,nDepth,hWnd,&mpBuffer,mhDC,mhBitmap,(int&)msizeBMP.cx,(int&)msizeBMP.cy,mnDepth);
	mhWnd = hWnd;
	mbFFT = bFFTWav;
	if (!m_bIsRenderInit)
	{
		SAFE_DELETE(pCommRender)
		pCommRender = pRenderImplement->CreateRender(hWnd,nWidth,nHeight,m_RenderType);
		m_bIsRenderInit = true;
	}
}

void DIBSection::SetImage(unsigned char *p, int nWidth, int nHeight, int nDepth)
{
	if (GetTickCount()-m_dwLastFillDataTime<200 && m_bNew){
		return;
	}
	m_dwLastFillDataTime = GetTickCount();
	if(mnDepth != nDepth || msizeBMP.cx != nWidth || msizeBMP.cy != nHeight|| !mpBuffer)
	{
		Clear();
		ModelImgToDIB(nWidth,nHeight,nDepth,mhWnd,&mpBuffer,mhDC,mhBitmap,(int&)msizeBMP.cx,(int&)msizeBMP.cy,mnDepth);
	}
	memcpy(mpBuffer,p,nWidth*nHeight*nDepth);
	m_bPhotoable = true;
	m_bNew = true;

	if (pRenderImplement && pCommRender)
	{
		//WhilteBalance(p,p,msizeBMP.cy,msizeBMP.cx);
		int db = (msizeBMP.cx>=320)?18:11;
		if (g_struct.bEnableSharpen)
		m_VideoEffect.SharpLaplacianCl(p,p,msizeBMP.cx,msizeBMP.cy,db);
		pCommRender->PutData(p,nWidth,nHeight);
	}
	else
	{
		switch(m_RenderType)
		{
		case IRender::ER_DDR_RENDER:
			AssertVS(pCommRender,0,"IRender::ER_DDR_RENDER");
			break;
		case IRender::ER_GDP_RENDER:
			AssertVS(pCommRender,0,"IRender::ER_GDP_RENDER");
			break;
		case IRender::ER_GDI_RENDER:
			AssertVS(pCommRender,0,"IRender::ER_GDI_RENDER");
			break;
		default:
			ErrTrace("DIBSection::SetImage IRender::无效类型\n");
		}

		if (!pCommRender)
		{
			ErrTrace("DIBSection::SetImage pCommRender==NULL\n");
			if (!m_bIsRenderInit) 
				ErrTrace("DIBSection::SetImage pCommRender==NULL m_bIsRenderInit=FALSE\n");
		}
		else
		{
			ErrTrace("DIBSection::SetImage pRenderImplement==NULL \n");
		}
	}
}

unsigned char* DIBSection::GetBuffer()
{
	return mpBuffer;
}