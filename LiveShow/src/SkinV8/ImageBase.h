#pragma once

struct tagANIMATEINFO;
struct tagFRAMEINFO;
class CImageRepository;
class CImageTransformParam;
class CImageBase
{
	friend class CImageRepository;
public:
	CImageBase(void);
	explicit CImageBase(const CImageBase& r);
	virtual ~CImageBase(void);
	CImageBase& operator = (const CImageBase& r);
public:
	static BOOL TransformColor(const CImageTransformParam* pParam, LONG color, LONG& transformColor);

	virtual BOOL LoadFromFile(LPCTSTR filePathName) = 0;
	virtual BOOL LoadFromData(const char* pzData, long size) = 0;
	virtual BOOL Transform(const CImageTransformParam* pParam) = 0;
	virtual BOOL Draw(HDC hDC, LPCRECT lpDstRect, LPRECT lpClipRect = NULL, const CImageTransformParam* pParam = NULL, 
        LONG clrTransparent = -1, BOOL bAlpha = FALSE, 
        BOOL bSideStretch = TRUE, LONG sideWidth = 0, LONG sideHeight = 0) = 0;
	virtual LONG Width() = 0;
	virtual LONG Height() = 0;
	virtual BOOL GetSize(LONG& lSize);

	//animation
	virtual LONG GetNumFrames();
	virtual BOOL GetAnimationInfo(tagANIMATEINFO& animateInfo);
	virtual BOOL GetFrameInfo(LONG iFrame, tagFRAMEINFO& frameInfo);
	virtual BOOL DrawFrame(HDC hDC, LONG iFrame, const RECT& dstRect);
};
