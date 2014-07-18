#pragma once

#ifdef CDIB_SUPPORT

#include "imagebase.h"

class CDib;
class CImageRepository;
class CImageBaseImplCDib :
	public CImageBase
{
	friend class CImageRepository;
public:
	CImageBaseImplCDib(void);
	virtual ~CImageBaseImplCDib(void);
protected:
    explicit CImageBaseImplCDib(const CImageBaseImplCDib& r);
	CImageBaseImplCDib& operator = (const CImageBaseImplCDib& r);
public:
	static BOOL TransformColor(const CImageTransformParam* pParam, LONG color, LONG& transformColor);

	virtual BOOL LoadFromFile(LPCTSTR filePathName);
	virtual BOOL LoadFromData(const char* pzData, long size);
	virtual BOOL Transform(const CImageTransformParam* pParam);
	virtual BOOL Draw(HDC hDC, LPCRECT lpDstRect, LPRECT lpClipRect = NULL, const CImageTransformParam* pParam = NULL, 
        LONG clrTransparent = -1, BOOL bAlpha = FALSE, 
        BOOL bSideStretch = TRUE, LONG sideWidth = 0, LONG sideHeight = 0);
	virtual LONG Width();
	virtual LONG Height();
protected:
	CDib* m_pDib;
};

#endif //CDIB_SUPPORT
