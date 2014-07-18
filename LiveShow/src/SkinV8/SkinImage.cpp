// SkinImage.cpp : implementation file
//

#include "stdafx.h"
#include "SkinEngine.h"
#include "ImageBase.h"
#include "RenderEngine.h"
#include "ImageType.h"
#include "ImageBaseImplIImage.h"

#include "SkinItemDialog.h"
#include "SkinImage.h"

#define ANIMATION_TIMER 1234

#ifdef V8_JS_ENGINE_SUPPORT
using namespace v8;
#endif //V8_JS_ENGINE_SUPPORT

// CSkinImage

IMPLEMENT_DYNCREATE(CSkinImage, CSkinBase)

CSkinImage::CSkinImage()
{
	EnableAutomation();

	m_pImage = NULL;
	m_strImage = _T("");

	m_pAbsoluteImage = NULL;
	m_strAbsoluteImage = _T("");

    m_pExternalImage = NULL;

	m_bConstrainedRatio = FALSE;
	m_bSmoothDisplay = FALSE;
    m_bUseOriginSize = TRUE;
	m_bNeedClip = FALSE;
	m_rcClipRc.SetRectEmpty();

	//默认为背景
	m_bBackground = TRUE;

    m_clrBackgroundColor = -1;

	//animation
	m_bAnimating = FALSE;
	m_lCurFrame = 0;
	m_hMemDC = NULL;
	m_hBitmap = NULL;
	m_hOldBitmap = NULL;
	m_hPrevMemDC = NULL;
	m_hPrevBitmap = NULL;
	m_hPrevOldBitmap = NULL;
	m_szDC = CSize(0, 0);
	m_bRestoreDC = FALSE;
}

CSkinImage::~CSkinImage()
{
	if (m_pAbsoluteImage != NULL) {
		delete m_pAbsoluteImage;
		m_pAbsoluteImage = NULL;
	}

    if (m_pExternalImage != NULL)
    {
        delete m_pExternalImage;
        m_pExternalImage = NULL;
    }

	//停止动画
	AnimationStop();
}

void CSkinImage::OnFinalRelease()
{
	// When the last reference for an automation object is released
	// OnFinalRelease is called.  The base class will automatically
	// deletes the object.  Add additional cleanup required for your
	// object before calling the base class.

	CCmdTarget::OnFinalRelease();
}


BEGIN_MESSAGE_MAP(CSkinImage, CSkinBase)
END_MESSAGE_MAP()


BEGIN_SKIN_DISPATCH_MAP(CSkinImage, CSkinBase)
	SKINBASE_DISP_PROPERTIES(CSkinImage)
	SKIN_DISP_PROPERTY_ALIAS(CSkinImage, transColor, transparentColor, VT_I4)

	SKIN_DISP_PROPERTY(CSkinImage, image, VT_BSTR)
	SKIN_DISP_PROPERTY(CSkinImage, absoluteImage, VT_BSTR)
    SKIN_DISP_PROPERTY(CSkinImage, externalImage, VT_I4)
	SKIN_DISP_PROPERTY(CSkinImage, constrainedRatio, VT_BOOL)
	SKIN_DISP_PROPERTY(CSkinImage, smoothDisplay, VT_BOOL)
    SKIN_DISP_PROPERTY(CSkinImage, useOriginSize, VT_BOOL)
	SKIN_DISP_PROPERTY(CSkinImage, originWidth, VT_I4)
	SKIN_DISP_PROPERTY(CSkinImage, originHeight, VT_I4)
    SKIN_DISP_PROPERTY(CSkinImage, backgroundColor, VT_I4)

	//DragonZ:支持画指定区域图片
	SKIN_DISP_PROPERTY(CSkinImage, needClip, VT_BOOL)
	SKIN_DISP_PROPERTY(CSkinImage, clipLeft, VT_I4)
	SKIN_DISP_PROPERTY(CSkinImage, clipTop, VT_I4)
	SKIN_DISP_PROPERTY(CSkinImage, clipWidth, VT_I4)
	SKIN_DISP_PROPERTY(CSkinImage, clipHeight, VT_I4)

	SKIN_DISP_FUNCTION(CSkinImage, Play, VT_BOOL, VTS_NONE)
	SKIN_DISP_FUNCTION(CSkinImage, Pause, VT_BOOL, VTS_NONE)
	SKIN_DISP_FUNCTION(CSkinImage, Step, VT_BOOL, VTS_NONE)
	SKIN_DISP_FUNCTION(CSkinImage, Stop, VT_BOOL, VTS_NONE)
	SKIN_DISP_FUNCTION(CSkinImage, IsPlayable, VT_BOOL, VTS_NONE)
	SKIN_DISP_FUNCTION(CSkinImage, IsPlaying, VT_BOOL, VTS_NONE)
	SKIN_DISP_FUNCTION(CSkinImage, IsPaused, VT_BOOL, VTS_NONE)

END_SKIN_DISPATCH_MAP(CSkinImage)

// Note: we add support for IID_ISkinImage to support typesafe binding
//  from VBA.  This IID must match the GUID that is attached to the 
//  dispinterface in the .IDL file.

// {2C500D0F-D872-406D-AA5B-D02208364C58}
static const IID IID_ISkinImage =
{ 0x2C500D0F, 0xD872, 0x406D, { 0xAA, 0x5B, 0xD0, 0x22, 0x8, 0x36, 0x4C, 0x58 } };

BEGIN_INTERFACE_MAP(CSkinImage, CSkinBase)
	INTERFACE_PART(CSkinImage, IID_ISkinImage, Dispatch)
END_INTERFACE_MAP()


// CSkinImage message handlers
BOOL CSkinImage::OnMouseEnter()
{
	return CSkinBase::OnMouseEnter();
}

BOOL CSkinImage::OnMouseLeave()
{
	return CSkinBase::OnMouseLeave();
}

BOOL CSkinImage::OnLButtonDown(LONG x, LONG y, UINT flag)
{
    if (GetScriptHandleEvent())
        return CSkinBase::OnLButtonDown(x, y, flag);

	if (IsPointInSkinRgn(x, y)) {
		SetCaptureSkin();
	} else {
		ReleaseCaptureSkin();
	}

	return CSkinBase::OnLButtonDown(x, y, flag);
}

BOOL CSkinImage::OnLButtonUp(LONG x, LONG y, UINT flag)
{
    if (GetScriptHandleEvent())
        return CSkinBase::OnLButtonUp(x, y, flag);

	CSkinBase* pSkin = NULL;
	BOOL bGet = GetCaptureSkin(&pSkin);
	ReleaseCaptureSkin();
	if (IsPointInSkinRgn(x, y)) {
		if (bGet && this == pSkin) {
			OnClick();
		}
	} else {
		OnMouseLeave();
	}

	return CSkinBase::OnLButtonUp(x, y, flag);
}

BOOL CSkinImage::OnTimer(UINT nIDEvent)
{
	if (nIDEvent == ANIMATION_TIMER) {

		//销毁动画定时器，并刷新元素
		AnimationStopTimer();
        if (GetVisible())
		    InvalidateRect(NULL, FALSE);

		//如果不在动画状态则返回
		if (!m_bAnimating)
			return TRUE;

		//判断图片是否存在
		CImageBase* pImage = GetDrawImage();
		if (pImage == NULL)
			return TRUE;

		//获得总帧数
		LONG lFrameNum = pImage->GetNumFrames();
		if (lFrameNum < 2)
			return TRUE;

		//绘制缓存DC
		//AnimationDrawDC();

        //检查上一帧定时是否到时
        LONG lPrevFrame = AnimationGetPrevFrame(m_lCurFrame);
        FRAMEINFO prevFrameInfo = {0};
        BOOL bSuc = pImage->GetFrameInfo(lPrevFrame, prevFrameInfo);
        ASSERT(bSuc);
        if (!bSuc)
            return TRUE;
        DWORD dwCurTickCount = ::GetTickCount();
        DWORD dwDiff = dwCurTickCount - m_dwAnimationTickCount;
        if (dwDiff >= (DWORD)prevFrameInfo.delayTime) {
		    //获得下一帧到本帧的延迟时间
            FRAMEINFO frameInfo = {0};
            BOOL bSuc = pImage->GetFrameInfo(m_lCurFrame, frameInfo);
            ASSERT(bSuc);
            if (!bSuc)
                return TRUE;
            m_lCurFrame = AnimationGetNextFrame(m_lCurFrame);
            m_dwAnimationTickCount = dwCurTickCount;

		    //设定下一帧定时器
            AnimationStartTimer(frameInfo.delayTime);
        } else {
            AnimationStartTimer(100);
        }

		return TRUE;
	}

	return CSkinBase::OnTimer(nIDEvent);
}

CImageBase* CSkinImage::GetDrawImage()
{
    if (m_pExternalImage != NULL)
        return m_pExternalImage;

    if (m_pAbsoluteImage != NULL)
        return m_pAbsoluteImage;

    if (m_pImage != NULL)
        return m_pImage;

    return NULL;
}

void CSkinImage::Draw(CDC* pDC, LPRECT lpInvalidRect, CRgn* pInvalidRgn)
{
	CRect drawRect = GetSkinRect();
    //绘制背景
    if (m_clrBackgroundColor != -1) {
        LONG bkColor = m_clrBackgroundColor;
        if (GetColorTransform())
            GetEngine()->TransformColor(bkColor, bkColor);
        CRenderEngine::FillRect(*pDC, drawRect, bkColor);
    }

	if (m_bConstrainedRatio){
		long drawWidth = drawRect.Width();
		long drawHeight = drawRect.Height();
		long originWidth = GetoriginWidth();
		long originHeight = GetoriginHeight();
		if (originWidth > drawWidth || originHeight > drawHeight) {
			if ( (drawHeight * originWidth) > (drawWidth * originHeight) ){
				drawHeight = drawWidth * originHeight / originWidth;
				drawRect.top = drawRect.top + (drawRect.Height() - drawHeight) / 2;
				drawRect.bottom = drawRect.top + drawHeight;
			} else {
				drawWidth = drawHeight * originWidth / originHeight;
				drawRect.left = drawRect.left + (drawRect.Width() - drawWidth) / 2;
				drawRect.right = drawRect.left + drawWidth;
			}
		} else {
			drawRect.DeflateRect((drawWidth - originWidth) / 2, (drawHeight - originHeight) / 2);
		}
	}

	//绘制动画或图片
	CImageBase* pImage = GetDrawImage();
	LONG lFrameNum = (pImage != NULL) ? (pImage->GetNumFrames()) : (0);
	if (m_bAnimating && pImage != NULL && lFrameNum > 1) { //Animation
		if (m_bRestoreDC) {
			AnimationRestoreDC(*pDC, drawRect);
			AnimationDrawDC();
			m_bRestoreDC = FALSE;
		}
		AnimationDraw(*pDC, drawRect);
	} else if (pImage != NULL) { //Still
		if (m_bSmoothDisplay) {
			CImageTransformParam param;
			param.enTransformType = TRANSFORM_TYPE_SMOOTH;
			param.width = drawRect.Width();
			param.height = drawRect.Height();
			//DragonZ:支持画指定区域图片
			if (m_bNeedClip)
			{
				pImage->Draw(*pDC, &drawRect, &m_rcClipRc, &param, GetTransparentColor(), m_bAlpha);
			}else
			{
				pImage->Draw(*pDC, &drawRect, NULL, &param, GetTransparentColor(), m_bAlpha);
			}
		} else if (! GetColorTransform()) {
			if (m_bNeedClip)
			{
				pImage->Draw(*pDC, &drawRect, &m_rcClipRc, NULL, GetTransparentColor(), m_bAlpha);
			} 
			else
			{
				pImage->Draw(*pDC, &drawRect, NULL, NULL, GetTransparentColor(), m_bAlpha);
			}
		} else {
			if (m_bNeedClip)
			{
				pImage->Draw(*pDC, &drawRect, &m_rcClipRc, GetImageParam(), GetTransformedTransparentColor(), m_bAlpha);
			} 
			else
			{
				pImage->Draw(*pDC, &drawRect, NULL, GetImageParam(), GetTransformedTransparentColor(), m_bAlpha);
			}
		}
	}

	//绘制区域边框
	if (m_nBorderLineWidth > 0 && m_clrBorderLineColor != -1){
		CBrush brush;
		LONG lineColor = m_clrBorderLineColor;
		if (GetColorTransform()) {
			GetEngine()->TransformColor(lineColor, lineColor);
		}
		brush.CreateSolidBrush(lineColor);
		CRgn rgn;
		rgn.Attach(GetSkinRgn());
		pDC->FrameRgn(&rgn, &brush, m_nBorderLineWidth, m_nBorderLineWidth);
	}
}

BSTR CSkinImage::Getimage()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: 在此添加调度处理程序代码
	return m_strImage.AllocSysString();
}

void CSkinImage::Setimage(LPCTSTR newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: 在此添加属性处理程序代码
    SetImageHelper(m_pImage, m_strImage, newVal, m_rcSkinRect.IsRectNull() && m_bUseOriginSize);
}

BSTR CSkinImage::GetabsoluteImage()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: 在此添加调度处理程序代码
	return m_strAbsoluteImage.AllocSysString();
}

void CSkinImage::SetabsoluteImage(LPCTSTR newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: 在此添加属性处理程序代码
	CString filePathName = newVal;
    if (filePathName.IsEmpty()) {
        if (m_pAbsoluteImage != NULL) {
            delete m_pAbsoluteImage;
            m_pAbsoluteImage = NULL;
        }
        InvalidateRect();
        return;
    }

	CImageBase* pImage = new CImageType;
	if (! pImage->LoadFromFile(filePathName) ){
		delete pImage;
		pImage = NULL;
		return;
	}

	if (m_pAbsoluteImage != NULL) {
		delete m_pAbsoluteImage;
		m_pAbsoluteImage = NULL;
	}
	m_pAbsoluteImage = pImage;
	m_strAbsoluteImage = newVal;
	if (m_rcSkinRect.IsRectNull() && m_bUseOriginSize){
		Setwidth(m_pAbsoluteImage->Width());
		Setheight(m_pAbsoluteImage->Height());
	}

	InvalidateRect();
}

LONG CSkinImage::GetexternalImage()
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());

    return 0;
}

void CSkinImage::SetexternalImage(LONG newVal)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());

    if (m_pExternalImage != NULL)
    {
        delete m_pExternalImage;
        m_pExternalImage = NULL;
    }

#pragma warning (push)
#pragma warning(disable : 4312)
    BYTE* ptr = (BYTE*)newVal;
#pragma warning (pop)

    IUnknown* pUnknown = reinterpret_cast<IUnknown*>(ptr);
    CComQIPtr<IImage> pImage = pUnknown;
    if (pImage != NULL)
    {
        m_pExternalImage = new CImageBaseImplIImage(pImage);
    }

    if (m_rcSkinRect.IsRectNull() && m_bUseOriginSize && m_pExternalImage) {
        Setwidth(m_pExternalImage->Width());
        Setheight(m_pExternalImage->Height());
    }

    InvalidateRect();
}

VARIANT_BOOL CSkinImage::GetconstrainedRatio()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: 在此添加调度处理程序代码
	return m_bConstrainedRatio ? (VARIANT_TRUE) : (VARIANT_FALSE);
}

void CSkinImage::SetconstrainedRatio(VARIANT_BOOL newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: 在此添加调度处理程序代码
	newVal = (newVal != VARIANT_FALSE);
	if (newVal == m_bConstrainedRatio)
		return;
	m_bConstrainedRatio = newVal;
	InvalidateRect(NULL);
}

VARIANT_BOOL CSkinImage::GetsmoothDisplay()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: 在此添加调度处理程序代码
	return m_bSmoothDisplay ? (VARIANT_TRUE) : (VARIANT_FALSE);
}

void CSkinImage::SetsmoothDisplay(VARIANT_BOOL newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: 在此添加调度处理程序代码
	newVal = (newVal != VARIANT_FALSE);
	if (newVal == m_bSmoothDisplay)
		return;
	m_bSmoothDisplay = newVal;
	InvalidateRect(NULL);
}

VARIANT_BOOL CSkinImage::GetuseOriginSize()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: 在此添加调度处理程序代码
	return m_bUseOriginSize ? (VARIANT_TRUE) : (VARIANT_FALSE);
}

void CSkinImage::SetuseOriginSize(VARIANT_BOOL newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: 在此添加调度处理程序代码
	newVal = (newVal != VARIANT_FALSE);
	if (newVal == m_bUseOriginSize)
		return;
	m_bUseOriginSize = newVal;
	InvalidateRect(NULL);
}

VARIANT_BOOL CSkinImage::GetneedClip()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: 在此添加调度处理程序代码
	return m_bNeedClip ? (VARIANT_TRUE) : (VARIANT_FALSE);
}

void CSkinImage::SetneedClip(VARIANT_BOOL newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: 在此添加调度处理程序代码
	newVal = (newVal != VARIANT_FALSE);
	if (newVal == m_bNeedClip)
		return;
	m_bNeedClip = newVal;
	InvalidateRect(NULL);
}

LONG CSkinImage::GetclipLeft()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	
	if ((!m_bNeedClip) || m_rcClipRc.IsRectEmpty())
	    return 0;

	return m_rcClipRc.left;
}

void CSkinImage::SetclipLeft(LONG newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	
	if (!m_bNeedClip)
		return ;

	if (newVal < 0 || (newVal == m_rcClipRc.left))
		return;

	m_rcClipRc.left = newVal;
}

LONG CSkinImage::GetclipTop()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	if ((!m_bNeedClip) || m_rcClipRc.IsRectEmpty())
		return 0;

	return m_rcClipRc.top;
}

void CSkinImage::SetclipTop(LONG newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	if (!m_bNeedClip)
		return ;

	if (newVal < 0 || (newVal == m_rcClipRc.top))
		return;

	m_rcClipRc.top = newVal;
}

LONG CSkinImage::GetclipWidth()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	if ((!m_bNeedClip) || m_rcClipRc.IsRectEmpty())
		return 0;
	
	return m_rcClipRc.Width();
}

void CSkinImage::SetclipWidth(LONG newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	if (!m_bNeedClip)
		return ;

	if (newVal < 0 || (newVal == m_rcClipRc.Width()))
		return;

	m_rcClipRc.right = m_rcClipRc.left + newVal;
}

LONG CSkinImage::GetclipHeight()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	if ((!m_bNeedClip) || m_rcClipRc.IsRectEmpty())
		return 0;

	return m_rcClipRc.Height();
}

void CSkinImage::SetclipHeight(LONG newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	if (!m_bNeedClip)
		return ;

	if (newVal < 0 || (newVal == m_rcClipRc.Height()))
		return;

	m_rcClipRc.bottom = m_rcClipRc.top + newVal;
}

LONG CSkinImage::GetoriginWidth()
{
    CImageBase* pImage = GetDrawImage();
    if (pImage != NULL)
        return pImage->Width();

    return 0;
}

void CSkinImage::SetoriginWidth(LONG newVal)
{
	//Do Nothing
}

LONG CSkinImage::GetoriginHeight()
{
    CImageBase* pImage = GetDrawImage();
    if (pImage != NULL)
        return pImage->Height();

    return 0;
}

void CSkinImage::SetoriginHeight(LONG newVal)
{
	//Do Nothing
}

LONG CSkinImage::GetbackgroundColor()
{
    return m_clrBackgroundColor;
}

void CSkinImage::SetbackgroundColor(LONG newVal)
{
    if (m_clrBackgroundColor == newVal)
        return;

    m_clrBackgroundColor = newVal;
    InvalidateRect();
}

//animation timer
void CSkinImage::AnimationStartTimer(UINT nElapse)
{
	m_uAnimationTimer = SetTimer(ANIMATION_TIMER, nElapse);
}

void CSkinImage::AnimationStopTimer()
{
	KillTimer(ANIMATION_TIMER);
	m_uAnimationTimer = NULL;
}

void CSkinImage::AnimationPrepareDC()
{
	if (m_hMemDC)
		AnimationDestroyDC();

	//DC大小
	m_szDC = CSize(GetWidth(), GetHeight());

	//获得DC
	HDC hDC = ::GetDC(NULL);

	//
	m_hMemDC = ::CreateCompatibleDC(hDC);
	m_hBitmap = ::CreateCompatibleBitmap(hDC, m_szDC.cx, m_szDC.cy);
	m_hOldBitmap = (HBITMAP)::SelectObject(m_hMemDC, m_hBitmap);
	//
	m_hPrevMemDC = ::CreateCompatibleDC(hDC);
	m_hPrevBitmap = ::CreateCompatibleBitmap(hDC, m_szDC.cx, m_szDC.cy);
	m_hPrevOldBitmap = (HBITMAP)::SelectObject(m_hPrevMemDC, m_hPrevBitmap);

	//初始化为背景色
	CImageBase* pImage = GetDrawImage();
	COLORREF color = RGB(255, 255, 255);
	ANIMATEINFO animateInfo;
	if (pImage != NULL && pImage->GetAnimationInfo(animateInfo) && animateInfo.bkColorIndex > 0) {
		color = animateInfo.bkColor;
	}
	CRenderEngine::FillRect(m_hMemDC, CRect(0, 0, m_szDC.cx, m_szDC.cy), color);

	//释放DC
	::ReleaseDC(NULL, hDC);
}

void CSkinImage::AnimationDestroyDC()
{
	if (m_hMemDC == NULL)
		return;

	::SelectObject(m_hMemDC, m_hOldBitmap);
	::DeleteDC(m_hMemDC);
	::DeleteObject(m_hBitmap);
	m_hMemDC = NULL;
	m_hBitmap = NULL;
	m_hOldBitmap = NULL;

	::SelectObject(m_hPrevMemDC, m_hPrevOldBitmap);
	::DeleteDC(m_hPrevMemDC);
	::DeleteObject(m_hPrevBitmap);
	m_hPrevMemDC = NULL;
	m_hPrevBitmap = NULL;
	m_hPrevOldBitmap = NULL;

	m_szDC = CSize(0, 0);
}

void CSkinImage::AnimationRestoreDC(HDC hDC, const RECT& srcRect)
{
	ASSERT(m_hMemDC != NULL);
	if (m_hMemDC == NULL)
		return;

	int iOldMode = ::SetStretchBltMode(m_hMemDC, COLORONCOLOR);
	BOOL bSuc = ::StretchBlt(m_hMemDC, 0, 0, m_szDC.cx, m_szDC.cy, 
		hDC, srcRect.left, srcRect.top, (srcRect.right - srcRect.left), (srcRect.bottom - srcRect.top), 
		SRCCOPY);
	::SetStretchBltMode(m_hMemDC, iOldMode);
}

void CSkinImage::AnimationDrawDC()
{
	ASSERT(m_hMemDC != NULL);
	if (m_hMemDC == NULL)
		return;

	CImageBase* pImage = GetDrawImage();
	if (pImage == NULL)
		return;

	FRAMEINFO frameInfo;
	if (! pImage->GetFrameInfo(m_lCurFrame, frameInfo))
		frameInfo.disposalMethod = 0;

	m_bRestoreDC = FALSE;
	if (frameInfo.disposalMethod == 2) {
		m_bRestoreDC = TRUE;
	} else if (frameInfo.disposalMethod == 3) {
		::BitBlt(m_hMemDC, 0, 0, m_szDC.cx, m_szDC.cy, m_hPrevMemDC, 0, 0, SRCCOPY);
	}
	::BitBlt(m_hPrevMemDC, 0, 0, m_szDC.cx, m_szDC.cy, m_hMemDC, 0, 0, SRCCOPY);
	pImage->DrawFrame(m_hMemDC, m_lCurFrame, CRect(0, 0, m_szDC.cx, m_szDC.cy));
}

BOOL CSkinImage::AnimationDraw(HDC hDC, const RECT& dstRect)
{
	//ASSERT(m_hMemDC != NULL);
	//if (m_hMemDC == NULL)
	//	return FALSE;

	//int iOldMode = ::SetStretchBltMode(hDC, COLORONCOLOR);
	//BOOL bSuc = ::StretchBlt(hDC, dstRect.left, dstRect.top,
	//	(dstRect.right - dstRect.left), (dstRect.bottom - dstRect.top),
	//	m_hMemDC, 0, 0, m_szDC.cx, m_szDC.cy, 
	//	SRCCOPY);
	//::SetStretchBltMode(hDC, iOldMode);

    CImageBase* pImage = GetDrawImage();
    if (pImage == NULL)
        return FALSE;

    BOOL bSuc = pImage->DrawFrame(hDC, m_lCurFrame, dstRect);

	return bSuc;
}

LONG CSkinImage::AnimationGetPrevFrame(LONG iCurFrame)
{
    CImageBase* pImage = GetDrawImage();
    if (pImage == NULL)
        return -1;

    LONG lFrameNum = pImage->GetNumFrames();
    ASSERT(iCurFrame >= 0 && iCurFrame < lFrameNum);
    if (iCurFrame < 0 || iCurFrame >= lFrameNum)
        return 0;

    LONG iFrame = iCurFrame;
    iFrame --;
    if (iFrame < 0)
        iFrame = lFrameNum - 1;

    return iFrame;
}

LONG CSkinImage::AnimationGetNextFrame(LONG iCurFrame)
{
	CImageBase* pImage = GetDrawImage();
	if (pImage == NULL)
		return -1;

	LONG lFrameNum = pImage->GetNumFrames();
	ASSERT(iCurFrame >= 0 && iCurFrame < lFrameNum);
	if (iCurFrame < 0 || iCurFrame >= lFrameNum)
		return 0;

	LONG iFrame = iCurFrame;
	iFrame ++;
	if (iFrame >= lFrameNum)
		iFrame = 0;

	return iFrame;
}

//animation
BOOL CSkinImage::AnimationPlay()
{
	if (!AnimationIsPlayable())
		return FALSE;

	//如果是暂停则只启动定时器
	if (m_bAnimating && m_uAnimationTimer == NULL) {
		AnimationStartTimer(10);
		return TRUE;
	}

	//准备缓存DC
	AnimationPrepareDC();

	//
	m_bAnimating = TRUE;
	m_bRestoreDC = FALSE;
	//启动动画定时器
	AnimationStartTimer(10);

	return TRUE;
}

BOOL CSkinImage::AnimationPause()
{
	AnimationStopTimer();

	if (! m_bAnimating)
		return FALSE;

	if (m_uAnimationTimer == NULL)
		return FALSE;

	return TRUE;
}

BOOL CSkinImage::AnimationStep()
{
	if (! m_bAnimating)
		return FALSE;

	CImageBase* pImage = GetDrawImage();
	if (pImage == NULL)
		return FALSE;

	AnimationDrawDC();

	m_lCurFrame = AnimationGetNextFrame(m_lCurFrame);

	InvalidateRect(NULL, FALSE);
	return TRUE;
}

BOOL CSkinImage::AnimationStop()
{
	//停止动画定时器
	AnimationStopTimer();
	m_bAnimating = FALSE;
	m_lCurFrame = 0;

	//销毁动画DC缓存
	AnimationDestroyDC();

	//刷新区域
	InvalidateRect(NULL, TRUE);
	return TRUE;
}

BOOL CSkinImage::AnimationIsPlayable()
{
	CImageBase* pImage = GetDrawImage();
	if (pImage == NULL)
		return FALSE;

	return (pImage->GetNumFrames() > 0);
}

BOOL CSkinImage::AnimationIsPlaying()
{
	BOOL bPlaying = m_bAnimating;
	return bPlaying ? (TRUE) : (FALSE);
}

BOOL CSkinImage::AnimationIsPaused()
{
	BOOL bPaused = m_bAnimating && (m_uAnimationTimer == NULL);
	return bPaused ? (TRUE) : (FALSE);
}

VARIANT_BOOL CSkinImage::Play()
{
	return AnimationPlay() ? (VARIANT_TRUE) : (VARIANT_FALSE);
}

VARIANT_BOOL CSkinImage::Pause()
{
	return AnimationPause() ? (VARIANT_TRUE) : (VARIANT_FALSE);
}

VARIANT_BOOL CSkinImage::Step()
{
	return AnimationStep() ? (VARIANT_TRUE) : (VARIANT_FALSE);
}

VARIANT_BOOL CSkinImage::Stop()
{
	return AnimationStop() ? (VARIANT_TRUE) : (VARIANT_FALSE);
}

VARIANT_BOOL CSkinImage::IsPlayable()
{
	return AnimationIsPlayable() ? (VARIANT_TRUE) : (VARIANT_FALSE);
}

VARIANT_BOOL CSkinImage::IsPlaying()
{
	return AnimationIsPlaying() ? (VARIANT_TRUE) : (VARIANT_FALSE);
}

VARIANT_BOOL CSkinImage::IsPaused()
{
	return AnimationIsPaused() ? (VARIANT_TRUE) : (VARIANT_FALSE);
}

#ifdef V8_JS_ENGINE_SUPPORT

Handle<Value> CSkinImage::V8Play(const Arguments& args)
{
	HandleScope handleScope;

	BOOL bSuc = AnimationPlay();

	Handle<Value> ret = Boolean::New(bSuc ? (true) : (false));

	return handleScope.Close(ret);
}

Handle<Value> CSkinImage::V8Pause(const Arguments& args)
{
	HandleScope handleScope;

	BOOL bSuc = AnimationPause();

	Handle<Value> ret = Boolean::New(bSuc ? (true) : (false));

	return handleScope.Close(ret);
}

Handle<Value> CSkinImage::V8Step(const Arguments& args)
{
	HandleScope handleScope;

	BOOL bSuc = AnimationStep();

	Handle<Value> ret = Boolean::New(bSuc ? (true) : (false));

	return handleScope.Close(ret);
}

Handle<Value> CSkinImage::V8Stop(const Arguments& args)
{
	HandleScope handleScope;

	BOOL bSuc = AnimationStop();

	Handle<Value> ret = Boolean::New(bSuc ? (true) : (false));

	return handleScope.Close(ret);
}

Handle<Value> CSkinImage::V8IsPlayable(const Arguments& args)
{
	HandleScope handleScope;

	BOOL bSuc = AnimationIsPlayable();

	Handle<Value> ret = Boolean::New(bSuc ? (true) : (false));

	return handleScope.Close(ret);
}

Handle<Value> CSkinImage::V8IsPlaying(const Arguments& args)
{
	HandleScope handleScope;

	BOOL bSuc = AnimationIsPlaying();

	Handle<Value> ret = Boolean::New(bSuc ? (true) : (false));

	return handleScope.Close(ret);
}

Handle<Value> CSkinImage::V8IsPaused(const Arguments& args)
{
	HandleScope handleScope;

	BOOL bSuc = AnimationIsPaused();

	Handle<Value> ret = Boolean::New(bSuc ? (true) : (false));

	return handleScope.Close(ret);
}

Handle<Value> CSkinImage::V8InternalCallMethod(const CString& funcName, const Arguments& args)
{
	if (funcName.CompareNoCase(_T("Play")) == 0) {
		return V8Play(args);
	} else if (funcName.CompareNoCase(_T("Pause")) == 0) {
		return V8Pause(args);
	} else if (funcName.CompareNoCase(_T("Step")) == 0) {
		return V8Step(args);
	} else if (funcName.CompareNoCase(_T("Stop")) == 0) {
		return V8Stop(args);
	} else if (funcName.CompareNoCase(_T("IsPlayable")) == 0) {
		return V8IsPlayable(args);
	} else if (funcName.CompareNoCase(_T("IsPlaying")) == 0) {
		return V8IsPlaying(args);
	} else if (funcName.CompareNoCase(_T("IsPaused")) == 0) {
		return V8IsPaused(args);
	} 

	return CSkinBase::V8InternalCallMethod(funcName, args);
}

#endif //V8_JS_ENGINE_SUPPORT
