#pragma once
#include "../stdafx.h"

#include "../../../../include/Skin_i.h"
#include "../../include/ui/ImageWrapper.h"
#include "../../include/ui/Animator.h"
#include "../../include/ui/ImageImpl.h"
#include "../../include/ui/UIHelper.h"

namespace common { namespace ui
{

class CAnimator;

class CAnimationWnd
{
public:
    CAnimationWnd()
        :
    m_hWnd(NULL),
    m_pfnOldProc(NULL),
    m_pAnimator(NULL)
    {
        Create();
    };

    ~CAnimationWnd()
    {
        Destory();
    };

public:
    void SetAnimator(CAnimator* pAnimator)
    {
        m_pAnimator = pAnimator;
    };

#define ANIMATION_TIMER_ID 1

    void SetAnimation(UINT uElapse)
    {
        m_TimerID = (UINT)::SetTimer(m_hWnd, ANIMATION_TIMER_ID, uElapse, NULL);
        ASSERT(m_TimerID == ANIMATION_TIMER_ID);
    };

    void StopAnimation()
    {
        ::KillTimer(m_hWnd, ANIMATION_TIMER_ID);
        m_TimerID = 0;
    };

    BOOL IsAnimating()
    {
        return (m_TimerID == ANIMATION_TIMER_ID);
    };

protected:
    void OnTimer(UINT uEventID)
    {
        ASSERT(uEventID == ANIMATION_TIMER_ID);
        if (uEventID != ANIMATION_TIMER_ID)
            return;

        if (! IsAnimating())
            return;

        if (m_pAnimator != NULL)
            m_pAnimator->Animate();
    };

    static LRESULT CALLBACK AnimationWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
    {
#pragma warning(push)
#pragma warning (disable:4312)
        CAnimationWnd* pAnimatorWnd = (CAnimationWnd*)::GetWindowLong(hWnd, GWL_USERDATA);
#pragma warning(pop)
        ASSERT(pAnimatorWnd != NULL);

        if (uMsg == WM_TIMER) {
            pAnimatorWnd->OnTimer((UINT)wParam);
        }

        WNDPROC oldProc = pAnimatorWnd->m_pfnOldProc;
        return (*oldProc)(hWnd, uMsg, wParam, lParam);
    };

    void Create()
    {
        m_hWnd = ::CreateWindow(_T("STATIC"), NULL, WS_POPUP,
            0, 0, 0, 0, 
            HWND_MESSAGE,	//Message-only
            NULL, NULL, NULL);
        ASSERT(::IsWindow(m_hWnd));
        if (::IsWindow(m_hWnd)) {
#pragma warning(push)
#pragma warning (disable:4311)
#pragma warning (disable:4312)
            m_pfnOldProc = (WNDPROC)::SetWindowLong(m_hWnd, GWL_WNDPROC, (LONG)AnimationWndProc);
            ::SetWindowLong(m_hWnd, GWL_USERDATA, (LONG)(this));
#pragma warning(pop)
        };
    };

    void Destory()
    {
        ::DestroyWindow(m_hWnd);
        m_hWnd = NULL;
        m_pAnimator = NULL;
    };

protected:
    HWND m_hWnd;
    WNDPROC m_pfnOldProc;
    UINT m_TimerID;
    CAnimator* m_pAnimator;
};

#define CLASS_DATA_MEMBER_LIST(V) \
    V(CImageWrapper, image) \
    V(CAnimationWnd, animationWnd) \
    V(HWND, hWnd) \
    V(RECT, rcDraw) \
    V(COLORREF, clrBackgroundColor) \
    V(UINT, curFrame) \
    V(DWORD, lastTick) \
    V(DWORD, tickEllapse) \
    V(IAnimatorCallback*, pAnimatorCallback)

DECLARE_CLASS_DATA_STRUCT(CAnimator, CLASS_DATA_MEMBER_LIST)

CAnimator::CAnimator()
{
    NEW_CLASS_DATA(CAnimator)

    USE_CLASS_DATA(CLASS_DATA_MEMBER_LIST);

    m_animationWnd.SetAnimator(this);
    m_hWnd = NULL;
    m_rcDraw = CRect(0, 0, 0, 0);
    m_clrBackgroundColor = RGB(255, 255, 255);
    m_curFrame = 0;
    m_lastTick = 0;
    m_tickEllapse = 0;
    m_pAnimatorCallback = NULL;
}

CAnimator::~CAnimator()
{
    DELETE_CLASS_DATA()
}

CImageWrapper CAnimator::GetImage()
{
    USE_CLASS_DATA(CLASS_DATA_MEMBER_LIST);

    return m_image;
}

void CAnimator::SetImage(CImageWrapper image)
{
    USE_CLASS_DATA(CLASS_DATA_MEMBER_LIST);

    BOOL bAnimating = IsAnimating();
    if (bAnimating)
        Stop();

    m_image = image;
    if (bAnimating)
        Play();
}

BOOL CAnimator::Load(LPCTSTR lpzFilePath)
{
    USE_CLASS_DATA(CLASS_DATA_MEMBER_LIST);

    CImageImpl* pImage = new CImageImpl(NULL);
    CComPtr<IImage> spImage;
    spImage.Attach(pImage);
    if (! pImage->Load(lpzFilePath))
        return FALSE;

    SetImage(CImageWrapper(spImage));
    return TRUE;
}

void CAnimator::SetPaintInfo(HWND hWnd, const RECT& rcDraw)
{
    USE_CLASS_DATA(CLASS_DATA_MEMBER_LIST);

    m_hWnd = hWnd;
    m_rcDraw = rcDraw;
}

COLORREF CAnimator::GetBackgroundColor()
{
    USE_CLASS_DATA(CLASS_DATA_MEMBER_LIST);

    return m_clrBackgroundColor;
}

void CAnimator::SetBackgroundColor(COLORREF clrColor)
{
    USE_CLASS_DATA(CLASS_DATA_MEMBER_LIST);

    m_clrBackgroundColor = clrColor;
}

void CAnimator::Play()
{
    USE_CLASS_DATA(CLASS_DATA_MEMBER_LIST);

    if (! m_image.IsValid())
        return;

    if (m_image.GetNumFrames() <= 1)
        return;

    if (IsAnimating())
        return;

    m_curFrame = 0;

    FRAMEINFO info;
    BOOL bSuc = m_image.GetFrameInfo(m_curFrame, info);
    ASSERT(bSuc);

    //m_lastTick = ::GetTickCount();
    //m_tickEllapse = info.delayTime;
    //m_animationWnd.SetAnimation(m_tickEllapse);
    m_lastTick = 0;
    Animate();
}

void CAnimator::Stop()
{
    USE_CLASS_DATA(CLASS_DATA_MEMBER_LIST);

    m_animationWnd.StopAnimation();
    m_curFrame = 0;
    m_lastTick = 0;
    m_tickEllapse = 0;
}

BOOL CAnimator::IsAnimating()
{
    USE_CLASS_DATA(CLASS_DATA_MEMBER_LIST);

    return m_animationWnd.IsAnimating();
}

BOOL CAnimator::Draw(HDC hDC, const RECT& rcDraw)
{
    USE_CLASS_DATA(CLASS_DATA_MEMBER_LIST);

    if (! m_image.IsValid())
        return FALSE;

    //绘制背景
    if (m_clrBackgroundColor != -1) {
        CDC::FromHandle(hDC)->FillSolidRect(&rcDraw, m_clrBackgroundColor);
    }

    //绘制图片
    UINT uFrameNum = m_image.GetNumFrames();
    ASSERT(m_curFrame >= 0 && m_curFrame < uFrameNum);

    if (m_pAnimatorCallback)
        m_pAnimatorCallback->PrePaint(hDC, m_rcDraw);

    BOOL bRet = m_image.RenderFrame(hDC, m_curFrame, rcDraw);

    if (m_pAnimatorCallback)
        m_pAnimatorCallback->PostPaint(hDC, m_rcDraw);

    return bRet;
}

void CAnimator::SetAnimatorCallback(IAnimatorCallback* pCallback)
{
    USE_CLASS_DATA(CLASS_DATA_MEMBER_LIST);

    m_pAnimatorCallback = pCallback;
}

void CAnimator::Animate()
{
    USE_CLASS_DATA(CLASS_DATA_MEMBER_LIST);

    DWORD tickCount = ::GetTickCount();
    DWORD diffCount = tickCount - m_lastTick;
    if (diffCount >= m_tickEllapse) {

        m_animationWnd.StopAnimation();

        //绘制当前帧
        HWND hWnd = m_hWnd;
        ASSERT(::IsWindow(hWnd));
        if (::IsWindow(hWnd) && ::IsWindowVisible(hWnd)) {
            HDC hDC = ::GetDC(hWnd);
            BOOL bSuc = FALSE;
            ASSERT(hDC != NULL);
            {
                CMemoryDC memDC(hDC, m_rcDraw);
                bSuc = Draw(memDC, m_rcDraw);
            }
            ASSERT(bSuc);
            ::ReleaseDC(hWnd, hDC);
        }

        //到下一帧
        m_lastTick = tickCount;
        m_curFrame ++;
        UINT uFrameNum = m_image.GetNumFrames();
        m_curFrame %= uFrameNum;
        FRAMEINFO info;
        BOOL bSuc = m_image.GetFrameInfo(m_curFrame, info);
        ASSERT(bSuc);

        m_tickEllapse = info.delayTime;
        m_animationWnd.SetAnimation(info.delayTime);
    }
}


IMPLEMENT_DYNAMIC(CAnimatorWnd, CWnd)

CAnimatorWnd::CAnimatorWnd()
{
}

CAnimatorWnd::~CAnimatorWnd()
{
}

BOOL CAnimatorWnd::Create(CWnd* pParent, const RECT& rcRect, UINT uID)
{
    return CWnd::Create(AfxRegisterWndClass(0), NULL, WS_VISIBLE | WS_CHILD, rcRect, pParent, uID);
}

BEGIN_MESSAGE_MAP(CAnimatorWnd, CWnd)
    ON_WM_CREATE()
    ON_WM_DESTROY()
    ON_WM_SIZE()
    ON_WM_ERASEBKGND()
    ON_WM_PAINT()
END_MESSAGE_MAP()

int CAnimatorWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
    if (CWnd::OnCreate(lpCreateStruct) == -1)
        return -1;

    CRect rcClient(0, 0, 0, 0);
    GetClientRect(&rcClient);
    CAnimator::SetPaintInfo(m_hWnd, rcClient);

    return 0;
}

void CAnimatorWnd::OnDestroy()
{
    CWnd::OnDestroy();
}

void CAnimatorWnd::OnSize(UINT nType, int cx, int cy)
{
    CRect rcClient(0, 0, 0, 0);
    GetClientRect(&rcClient);
    CAnimator::SetPaintInfo(m_hWnd, rcClient);
    Invalidate();

    CWnd::OnSize(nType, cx, cy);
}

BOOL CAnimatorWnd::OnEraseBkgnd(CDC* pDC)
{
    (pDC);
    return TRUE;
}

void CAnimatorWnd::OnPaint()
{
    CPaintDC dc(this);

    CRect rcClient(0, 0, 0, 0);
    GetClientRect(&rcClient);

    CAnimator::Draw(dc.GetSafeHdc(), rcClient);
}

} } //namespace common::ui
