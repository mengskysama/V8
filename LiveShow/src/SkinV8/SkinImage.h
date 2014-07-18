
#pragma once

#include "SkinBase.h"

// CSkinImage command target
class CSkinItemDialog;
class CImageBase;
class CSkinImage : public CSkinBase
{
	friend class CSkinEngine;
	DECLARE_DYNCREATE(CSkinImage)

protected:
	CSkinImage();
	virtual ~CSkinImage();

	virtual void OnFinalRelease();

protected:
	DECLARE_MESSAGE_MAP()
	DECLARE_SKIN_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()


protected:
	enum 
	{
		SKINBASE_DISPIDS(CSkinImage),

		SKIN_DISPID(CSkinImage, smoothDisplay),
		SKIN_DISPID(CSkinImage, constrainedRatio),
        SKIN_DISPID(CSkinImage, useOriginSize),
        SKIN_DISPID(CSkinImage, externalImage),
		SKIN_DISPID(CSkinImage, absoluteImage),
		SKIN_DISPID(CSkinImage, image),
		SKIN_DISPID(CSkinImage, originWidth),
		SKIN_DISPID(CSkinImage, originHeight),
        SKIN_DISPID(CSkinImage, backgroundColor),

		//DragonZ:支持画指定区域图片
		SKIN_DISPID(CSkinImage, needClip),
		SKIN_DISPID(CSkinImage, clipLeft),
		SKIN_DISPID(CSkinImage, clipTop),
		SKIN_DISPID(CSkinImage, clipWidth),
		SKIN_DISPID(CSkinImage, clipHeight),

		SKIN_DISPID(CSkinImage, Play),
		SKIN_DISPID(CSkinImage, Pause),
		SKIN_DISPID(CSkinImage, Step),
		SKIN_DISPID(CSkinImage, Stop),
		SKIN_DISPID(CSkinImage, IsPlayable),
		SKIN_DISPID(CSkinImage, IsPlaying),
		SKIN_DISPID(CSkinImage, IsPaused),

		
	};

public:
	//属性
	BSTR Getimage();
	void Setimage(LPCTSTR newVal);
	BSTR GetabsoluteImage();
	void SetabsoluteImage(LPCTSTR newVal);
    LONG GetexternalImage();
    void SetexternalImage(LONG newVal);
	VARIANT_BOOL GetconstrainedRatio();
	void SetconstrainedRatio(VARIANT_BOOL newVal);
	VARIANT_BOOL GetsmoothDisplay();
	void SetsmoothDisplay(VARIANT_BOOL newVal);
    VARIANT_BOOL GetuseOriginSize();
    void SetuseOriginSize(VARIANT_BOOL newVal);

	//DragonZ:支持画指定区域图片
	VARIANT_BOOL GetneedClip();
	void SetneedClip(VARIANT_BOOL newVal);
	LONG GetclipLeft();
	void SetclipLeft(LONG newVal);
	LONG GetclipTop();
	void SetclipTop(LONG newVal);
	LONG GetclipWidth();
	void SetclipWidth(LONG newVal);
	LONG GetclipHeight();
	void SetclipHeight(LONG newVal);
	//////////////////////////////

	LONG GetoriginWidth();
	void SetoriginWidth(LONG newVal);
	LONG GetoriginHeight();
	void SetoriginHeight(LONG newVal);

    LONG GetbackgroundColor();
    void SetbackgroundColor(LONG newVal);

	//animation
	VARIANT_BOOL Play();
	VARIANT_BOOL Pause();
	VARIANT_BOOL Step();
	VARIANT_BOOL Stop();
	VARIANT_BOOL IsPlayable();
	VARIANT_BOOL IsPlaying();
	VARIANT_BOOL IsPaused();

public:
	//animation
	void AnimationStartTimer(UINT nElapse);
	void AnimationStopTimer();
	void AnimationPrepareDC();
	void AnimationDestroyDC();
	void AnimationRestoreDC(HDC hDC, const RECT& srcRect);
	void AnimationDrawDC();
	BOOL AnimationDraw(HDC hDC, const RECT& dstRect);
    LONG AnimationGetPrevFrame(LONG iCurFrame);
	LONG AnimationGetNextFrame(LONG iCurFrame);
	BOOL AnimationPlay();
	BOOL AnimationPause();
	BOOL AnimationStep();
	BOOL AnimationStop();
	BOOL AnimationIsPlayable();
	BOOL AnimationIsPlaying();
	BOOL AnimationIsPaused();

#ifdef V8_JS_ENGINE_SUPPORT

	v8::Handle<v8::Value> V8Play(const v8::Arguments& args);
	v8::Handle<v8::Value> V8Pause(const v8::Arguments& args);
	v8::Handle<v8::Value> V8Stop(const v8::Arguments& args);
	v8::Handle<v8::Value> V8Step(const v8::Arguments& args);
	v8::Handle<v8::Value> V8IsPlayable(const v8::Arguments& args);
	v8::Handle<v8::Value> V8IsPlaying(const v8::Arguments& args);
	v8::Handle<v8::Value> V8IsPaused(const v8::Arguments& args);

protected:
	virtual v8::Handle<v8::Value> V8InternalCallMethod(const CString& funcName, const v8::Arguments& args);

#endif //V8_JS_ENGINE_SUPPORT

protected:
	CImageBase* GetDrawImage();

public:
	virtual BOOL OnMouseEnter();
	virtual BOOL OnMouseLeave();
	virtual BOOL OnLButtonDown(LONG x, LONG y, UINT flag);
	virtual BOOL OnLButtonUp(LONG x, LONG y, UINT flag);

	virtual BOOL OnTimer(UINT nIDEvent);

public:
	void Draw(CDC* pDC, LPRECT lpInvalidRect = NULL, CRgn* pInvalidRgn = NULL);

protected:
	CImageBase*	m_pImage;
	CString		m_strImage;
	CImageBase* m_pAbsoluteImage;
	CString		m_strAbsoluteImage;
    CImageBase* m_pExternalImage;

	BOOL		m_bConstrainedRatio;
	BOOL		m_bSmoothDisplay;
    BOOL        m_bUseOriginSize;
	//DragonZ:支持画指定区域图片
	BOOL        m_bNeedClip;
	CRect       m_rcClipRc;

    LONG        m_clrBackgroundColor;

	//animation
	BOOL		m_bAnimating;
	UINT_PTR    m_uAnimationTimer;
    DWORD       m_dwAnimationTickCount;
	LONG		m_lCurFrame;
	HDC			m_hMemDC;
	HBITMAP		m_hBitmap;
	HBITMAP		m_hOldBitmap;
	HDC			m_hPrevMemDC;
	HBITMAP		m_hPrevBitmap;
	HBITMAP		m_hPrevOldBitmap;
	CSize		m_szDC;
	BOOL		m_bRestoreDC;
};
