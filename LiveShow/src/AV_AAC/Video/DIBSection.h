// DIBSection.h: interface for the DIBSection class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DIBSECTION_H__E71248C6_B9D8_4E56_BD33_C8145BF52940__INCLUDED_)
#define AFX_DIBSECTION_H__E71248C6_B9D8_4E56_BD33_C8145BF52940__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "IRender.h"
#include "VideoEffect.h"

class DIBSection  
{
public:

	DIBSection();
	virtual ~DIBSection();

	virtual unsigned char*  GetBuffer();
	virtual void         SetImage(unsigned char* p,int nWidth,int nHeight,int nDepth);
	virtual void         Create(HWND hWnd,int nWidth, int nHeight, int nDepth,bool bFFTWav=TRUE);	
	virtual void         Paint(HDC hDC,int x,int y);

	virtual inline bool  IsPhotoable() { 
		//if (GetTickCount()-m_dwLastFillDataTime>3000) return false;
		return m_bPhotoable; 
	}
	virtual inline HWND  GetHwnd() {  return mhWnd; }
	virtual inline bool  IsShowEncodeVideo(){ return mbShowEncodeVideo; }
	virtual inline bool  IsViewFFT(){return mbFFT;}
	virtual inline void  SetViewFFT(bool bEnable){mbFFT = bEnable;}
	virtual inline void  SetShowEncodeVide(bool bEnable){ this->mbShowEncodeVideo = bEnable;};
	virtual inline void  SetRoomId(unsigned int iRoomId){this->mnRoomId = iRoomId;};
	virtual inline unsigned int  GetRoomId(){return mnRoomId;};
protected:
	bool ModelImgToDIB(int nWidth,int nHeight,int nDepth, HWND hWnd,unsigned char** ppBuffer,HDC& hDC,
							  HBITMAP& hBitmap,int& w,int& h,int& nDstDepth);
	void Clear();
	void WhilteBalance(unsigned char* rgbImg, unsigned char* chImg, int rows, int cols);

protected:
	HBITMAP mhBitmap;
	HBITMAP mhBitmapDb;
	SIZE msizeBMP;
	int mnDepth;
	HDC mhDC;
	HDC mhDCDb;
	unsigned char *mpBuffer;
	unsigned char *ppBufferDb;
	bool   m_bPhotoable;
	bool   m_bNew;
	int mStretch;
	HWND   mhWnd;  //对应的窗口句柄
	bool mbShowEncodeVideo;
	bool mbFFT;
	unsigned int mnRoomId;
	DWORD m_dwLastFillDataTime;//上次填充数据时间

	unsigned char m_whiteBalanceArray[256];

	IRender* pRenderImplement;
	ICommonRender* pCommRender;
	IRender::En_Used_Order m_RenderType;
	bool m_bIsRenderInit;
	CVideoEffect m_VideoEffect;
};

#endif // !defined(AFX_DIBSECTION_H__E71248C6_B9D8_4E56_BD33_C8145BF52940__INCLUDED_)
