#include "StdAfx.h"
#include "ImageTransformParam.h"
#include ".\imagebase.h"

CImageBase::CImageBase(void)
{
	
}

CImageBase::CImageBase(const CImageBase& r)
{
	
}

CImageBase::~CImageBase(void)
{

}

CImageBase& CImageBase::operator = (const CImageBase& r)
{
	if (this != &r){
		//do nothing
	}
	return *this;
}

BOOL CImageBase::LoadFromFile(LPCTSTR filePathName)
{
	return FALSE;
}

BOOL CImageBase::LoadFromData(const char* pzData, long size)
{
	return FALSE;
}

BOOL CImageBase::Transform(const CImageTransformParam* pParam)
{
	return FALSE;
}

BOOL CImageBase::TransformColor(const CImageTransformParam* pParam, LONG color, LONG& transformColor)
{
	return FALSE;
}

BOOL CImageBase::Draw(HDC hDC, LPCRECT lpDstRect, LPRECT lpClipRect, const CImageTransformParam* pParam, 
                      LONG clrTransparent, BOOL bAlpha, 
                      BOOL bSideStretch, LONG sideWidth, LONG sideHeight)
{
	return FALSE;
}

LONG CImageBase::Width()
{
	return -1;
}

LONG CImageBase::Height()
{
	return -1;
}

BOOL CImageBase::GetSize(LONG& lSize)
{
	lSize = 0;
	return FALSE;
}

LONG CImageBase::GetNumFrames() 
{
	return 0;
};

BOOL CImageBase::GetAnimationInfo(tagANIMATEINFO& animateInfo)
{
	return FALSE;
}

BOOL CImageBase::GetFrameInfo(LONG iFrame, tagFRAMEINFO& frameInfo)
{
	return FALSE;
};

BOOL CImageBase::DrawFrame(HDC hDC, LONG iFrame, const RECT& dstRect) 
{
	return  FALSE;
};
