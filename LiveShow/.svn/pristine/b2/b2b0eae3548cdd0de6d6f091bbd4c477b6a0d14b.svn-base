// SkinItemSlider.cpp : 实现文件
//

#include "stdafx.h"
#include "SkinItem.h"
#include "ImageBase.h"
#include "SkinEngine.h"
#include "SkinItemSlider.h"


// CSkinItemSlider

IMPLEMENT_DYNCREATE(CSkinItemSlider, CSkinItem)
CSkinItemSlider::CSkinItemSlider()
{
	EnableAutomation();
	
	// 为了使应用程序在 OLE 自动化对象处于活动状态时保持
	//	运行，构造函数调用 AfxOleLockApp。

	m_bTransparent = TRUE;
	
	m_pChannelImage = NULL;
	m_pThumbNormalImage = NULL;
	m_pThumbClickImage = NULL;
    m_pFullImage = NULL;
	m_lBarHeight = 5;
	m_lFullBegin = 0;

    m_bInThumbOrDrag = FALSE;
	m_bVeritySlider = FALSE;

	AfxOleLockApp();
}

CSkinItemSlider::~CSkinItemSlider()
{
	// 为了在用 OLE 自动化创建所有对象后终止应用程序，
	// 	析构函数调用 AfxOleUnlockApp。
	
	AfxOleUnlockApp();
}

void CSkinItemSlider::OnFinalRelease()
{
	// 释放了对自动化对象的最后一个引用后，将调用
	// OnFinalRelease。基类将自动
	// 删除该对象。在调用该基类之前，请添加您的
	// 对象所需的附加清除代码。

	CSkinItem::OnFinalRelease();
}


BEGIN_MESSAGE_MAP(CSkinItemSlider, CSkinItem)
END_MESSAGE_MAP()


BEGIN_SKIN_DISPATCH_MAP(CSkinItemSlider, CSkinItem)
	SKINBASE_DISP_PROPERTIES(CSkinItemSlider)
	SKINITEM_DISP_PROPERTIES(CSkinItemSlider)

	SKIN_DISP_PROPERTY(CSkinItemSlider, channelImage, VT_BSTR)
	SKIN_DISP_PROPERTY(CSkinItemSlider, thumbNormalImage, VT_BSTR)
	SKIN_DISP_PROPERTY(CSkinItemSlider, thumbClickImage, VT_BSTR)
	SKIN_DISP_PROPERTY(CSkinItemSlider, fullImage, VT_BSTR)//DragonZ
	SKIN_DISP_PROPERTY(CSkinItemSlider, barHeight, VT_I4)//DragonZ
	SKIN_DISP_PROPERTY(CSkinItemSlider, fullBegin, VT_I4)//DragonZ
	SKIN_DISP_PROPERTY(CSkinItemSlider, veritySlider, VT_BOOL)//DragonZ

	SKIN_DISP_PROPERTY_ALIAS(CSkinItemSlider, thumbImage, thumbNormalImage, VT_BSTR)
	SKIN_DISP_PROPERTY_ALIAS(CSkinItemSlider, thumbImageClick, thumbClickImage, VT_BSTR)

END_SKIN_DISPATCH_MAP(CSkinItemSlider)

// 注意: 我们添加 IID_ISkinItemSlider 支持
//以支持来自 VBA 的类型安全绑定。此 IID 必须同附加到 .IDL 文件中的
//调度接口的 GUID 匹配。

// {EE3ADE8C-C3DB-43A1-BE49-A76EBCDCBE3D}
static const IID IID_ISkinItemSlider =
{ 0xEE3ADE8C, 0xC3DB, 0x43A1, { 0xBE, 0x49, 0xA7, 0x6E, 0xBC, 0xDC, 0xBE, 0x3D } };

BEGIN_INTERFACE_MAP(CSkinItemSlider, CSkinItem)
	INTERFACE_PART(CSkinItemSlider, IID_ISkinItemSlider, Dispatch)
END_INTERFACE_MAP()

// {E7F24884-272E-4AB0-9E96-777FC927BCBE}
IMPLEMENT_OLECREATE(CSkinItemSlider, "Skin.SkinItemSlider", 0xe7f24884, 0x272e, 0x4ab0, 0x9e, 0x96, 0x77, 0x7f, 0xc9, 0x27, 0xbc, 0xbe)


// CSkinItemSlider 消息处理程序
BOOL CSkinItemSlider::GetPos(LONG& pos)
{
	ASSERT(::IsWindow(m_hWnd));
	if (!::IsWindow(m_hWnd))
		return FALSE;

	pos = (UINT)::SendMessage(m_hWnd, TBM_GETPOS, 0, 0);
	return TRUE;
}

BOOL CSkinItemSlider::SetPos(LONG pos)
{
	ASSERT(::IsWindow(m_hWnd));
	if (!::IsWindow(m_hWnd))
		return FALSE;

	return (BOOL)::SendMessage(m_hWnd, TBM_SETPOS, 0, (LPARAM)pos);
}

BOOL CSkinItemSlider::GetMin(LONG& minValue)
{
	ASSERT(::IsWindow(m_hWnd));
	if (!::IsWindow(m_hWnd))
		return FALSE;

	minValue = (UINT)::SendMessage(m_hWnd, TBM_GETRANGEMIN, 0, 0);
	return TRUE;
}

BOOL CSkinItemSlider::SetMin(LONG minValue)
{
	ASSERT(::IsWindow(m_hWnd));
	if (!::IsWindow(m_hWnd))
		return FALSE;

	::SendMessage(m_hWnd, TBM_SETRANGEMIN, TRUE, (LPARAM)minValue);
	return TRUE;
}

BOOL CSkinItemSlider::GetMax(LONG& maxValue)
{
	ASSERT(::IsWindow(m_hWnd));
	if (!::IsWindow(m_hWnd))
		return FALSE;

	maxValue = (UINT)::SendMessage(m_hWnd, TBM_GETRANGEMAX, 0, 0);
	return TRUE;
}

BOOL CSkinItemSlider::SetMax(LONG maxValue)
{
	ASSERT(::IsWindow(m_hWnd));
	if (!::IsWindow(m_hWnd))
		return FALSE;

	::SendMessage(m_hWnd, TBM_SETRANGEMAX, TRUE, (LPARAM)maxValue);
	return TRUE;
}

BOOL CSkinItemSlider::GetThumbRect(RECT& thumbRect)
{
	ASSERT(::IsWindow(m_hWnd));
	if (!::IsWindow(m_hWnd))
		return FALSE;

	::SendMessage(m_hWnd, TBM_GETTHUMBRECT, 0, (LPARAM)(&thumbRect));
	return TRUE;
}

BOOL CSkinItemSlider::GetThumbImgRect(RECT& thumbRect)
{
	CImageBase * pImgThumb = m_pThumbNormalImage;
	RECT tempRect;
	GetThumbRect(tempRect);
	int nThumbWidth = tempRect.right - tempRect.left;
	int nThumbHeight = tempRect.bottom - tempRect.top;

	int nTemp = (pImgThumb->Width() - nThumbWidth)/2;
	thumbRect.left = tempRect.left - nTemp - 2;//奇怪，怎么会偏差了两个象素。。。难道是图片的问题
	if (thumbRect.left < 0)
	{
		thumbRect.left = 0;
	}
	nTemp = (pImgThumb->Height() - nThumbHeight)/2;
	thumbRect.top = tempRect.top - nTemp;
	if (thumbRect.top < 0)
	{
		thumbRect.top = 0;
	}

	thumbRect.right = thumbRect.left + pImgThumb->Width();
	thumbRect.bottom = thumbRect.top + pImgThumb->Height();

	return TRUE;
}

BOOL CSkinItemSlider::GetChannelRect(RECT& channelRect)
{
	ASSERT(::IsWindow(m_hWnd));
	if (!::IsWindow(m_hWnd))
		return FALSE;
	RECT rcTemp;
	::SendMessage(m_hWnd, TBM_GETCHANNELRECT, 0, (LPARAM)(&rcTemp));
	if (!m_bVeritySlider)
	{
		channelRect = rcTemp;
	}else
	{
		RECT rcSlider;
		GetClientRect(m_hWnd, &rcSlider);
		channelRect.left = rcSlider.left + (rcSlider.right - rcSlider.left - (rcTemp.bottom - rcTemp.top) )/2;
		channelRect.right = channelRect.left + rcTemp.bottom - rcTemp.top;
		channelRect.top = rcSlider.top + (rcSlider.bottom - rcSlider.top - (rcTemp.right - rcTemp.left) )/2;;
		channelRect.bottom = channelRect.top + rcTemp.right - rcTemp.left;
	}
	return TRUE;
}
//DragonZ
BOOL CSkinItemSlider::GetFullRect(RECT& fullRect)
{
    RECT channelRc,thumbRc,fullRc;
	GetChannelRect(channelRc);
	GetThumbImgRect(thumbRc);
    fullRc = channelRc;
	if (!m_bVeritySlider)
	{
		fullRc.right = thumbRc.left;
	}else
	{
		fullRc.top = thumbRc.bottom; // 从下往上填充
		fullRc.bottom -= 2;
	}
	fullRect = fullRc;
	return TRUE;
}
//////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////
BSTR CSkinItemSlider::GetchannelImage()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: 在此添加调度处理程序代码
	return m_strChannelImage.AllocSysString();
}

void CSkinItemSlider::SetchannelImage(LPCTSTR newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: 在此添加属性处理程序代码
    SetImageHelper(m_pChannelImage, m_strChannelImage, newVal);
}

BSTR CSkinItemSlider::GetthumbNormalImage()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: 在此添加调度处理程序代码
	return m_strThumbNormalImage.AllocSysString();
}

void CSkinItemSlider::SetthumbNormalImage(LPCTSTR newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: 在此添加属性处理程序代码
    SetImageHelper(m_pThumbNormalImage, m_strThumbNormalImage, newVal);
}

BSTR CSkinItemSlider::GetthumbClickImage()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: 在此添加调度处理程序代码
	return m_strThumbClickImage.AllocSysString();
}

void CSkinItemSlider::SetthumbClickImage(LPCTSTR newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: 在此添加属性处理程序代码
    SetImageHelper(m_pThumbClickImage, m_strThumbClickImage, newVal);
}

//以下几个DragonZ添加
BSTR CSkinItemSlider::GetfullImage()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: 在此添加调度处理程序代码
	return m_strFullImage.AllocSysString();
}

void CSkinItemSlider::SetfullImage(LPCTSTR newVal)
{ 
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: 在此添加属性处理程序代码
	SetImageHelper(m_pFullImage, m_strFullImage, newVal);
}

LONG CSkinItemSlider::GetbarHeight()
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());

	return m_lBarHeight;
}

void CSkinItemSlider::SetbarHeight(LONG newVal)
{
	m_lBarHeight = newVal;
}

LONG CSkinItemSlider::GetfullBegin()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	return m_lFullBegin;
}

void CSkinItemSlider::SetfullBegin(LONG newVal)
{
	m_lFullBegin = newVal;
}

void CSkinItemSlider::SetveritySlider( VARIANT_BOOL newVal )
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	if (newVal != m_bVeritySlider)
	{
		m_bVeritySlider = newVal;
	}
}

VARIANT_BOOL CSkinItemSlider::GetveritySlider()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	return m_bVeritySlider;
}

//////////////////////////////////////////////////////////////////////////
//绘制
//////////////////////////////////////////////////////////////////////////
void CSkinItemSlider::DrawBackground(CDC* pDC, LPRECT lpInvalidRect, CRgn* pInvalidRgn, BOOL bChildSkin)
{
	CSkinBase::DrawBackground(pDC, lpInvalidRect, pInvalidRgn);
	if (m_pChannelImage != NULL) {
		CRect channelRect = GetSkinAbsRect();
		GetChannelRect(channelRect);
		LONG lBarHeight = GetbarHeight();
		if (lBarHeight > 0)
		{
			if (!m_bVeritySlider)
			{
				channelRect.top = channelRect.top - (lBarHeight - channelRect.Height())/2;
				channelRect.bottom = channelRect.top + lBarHeight;
		    }else
			{
                channelRect.left = channelRect.left - (lBarHeight - channelRect.Width())/2;
				channelRect.right = channelRect.left + lBarHeight;
			}
		}
		m_pChannelImage->Draw(*pDC, &channelRect, NULL, GetImageParam(), GetTransformedTransparentColor());
	}
	
}


void CSkinItemSlider::DrawForeground(CDC* pDC, LPRECT lpInvalidRect, CRgn* pInvalidRgn)
{
	CImageBase* pImage = m_pThumbNormalImage;
	if (pImage == NULL)
		pImage = m_pThumbClickImage;
	if (pImage != NULL) {
		CRect thumbRect = GetSkinAbsRect();
		//GetThumbRect(thumbRect);
		GetThumbImgRect(thumbRect);
		pImage->Draw(*pDC, &thumbRect, NULL, GetImageParam(), GetTransformedTransparentColor(), true);

	}
	/////////////DragonZ/////////////////
	CImageBase* pFullImage = m_pFullImage;
	if (pFullImage == NULL)
		pFullImage = m_pFullImage;
	if (pFullImage != NULL) {
		CRect fullRect = GetSkinAbsRect();
		GetFullRect(fullRect);
		LONG lBarHeight = GetbarHeight();

		if (lBarHeight > 0)
		{
			if (!m_bVeritySlider)
			{
				fullRect.top = fullRect.top - (lBarHeight - fullRect.Height())/2;
				fullRect.bottom = fullRect.top + lBarHeight;
			}else
			{
				fullRect.left = fullRect.left + (fullRect.Width() - lBarHeight)/2;
				fullRect.right = fullRect.left + lBarHeight;
			}
		}

		LONG lFullBegin = GetfullBegin();
        if (lFullBegin > 0)
        {
			if (!m_bVeritySlider)
			{
				fullRect.left = fullRect.left + lFullBegin;
			}else
			{
				fullRect.bottom = fullRect.bottom - lFullBegin; // 从下往上填充
			}
        }

		pFullImage->Draw(*pDC, &fullRect, NULL, GetImageParam(), GetTransformedTransparentColor());
	}
	/////////////////////////////////////
}

void CSkinItemSlider::DrawFrame(CDC* pDC, LPRECT lpInvalidRect, CRgn* pInvalidRgn)
{
}

void CSkinItemSlider::Draw(CDC* pDC, LPRECT lpInvalidRect, CRgn* pInvalidRgn)
{
	DrawBackground(pDC, lpInvalidRect, pInvalidRgn);
	DrawForeground(pDC, lpInvalidRect, pInvalidRgn);
	DrawFrame(pDC, lpInvalidRect, pInvalidRgn);
}

//更新皮肤属性，在皮肤新附着窗口时调用
BOOL CSkinItemSlider::OnPaint(UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* lpResult)
{
	*lpResult = 0;

	ASSERT(::IsWindow(m_hWnd));
	if (! ::IsWindow(m_hWnd) || ::IsIconic(m_hWnd))
		return FALSE;

	//准备CWnd
	CWnd* pWnd = CWnd::FromHandle(m_hWnd);
	if (pWnd == NULL)
		return TRUE;

	//准备DC
	CPaintDC dc(pWnd);

	//获得相关矩形
	CRect rcInvalid = dc.m_ps.rcPaint;

	//双缓存
	CDC memDC;
	memDC.CreateCompatibleDC(&dc);
	CBitmap memBitmap;
	memBitmap.CreateCompatibleBitmap(&dc, GetWidth(), GetHeight());
	CBitmap* pOldBitmap = memDC.SelectObject(&memBitmap);

	//在缓存上绘制
	Draw(&memDC, &rcInvalid, NULL);

	//复制缓存DC到显示DC
	dc.BitBlt(0, 0, GetWidth(), GetHeight(), &memDC, 0, 0, SRCCOPY);

	//恢复memDC
	memDC.SelectObject(pOldBitmap);
	memDC.DeleteDC();

	memBitmap.DeleteObject();

	return !GetselfOnPaint();
}

BOOL CSkinItemSlider::OnMouseMove(UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* lpResult)
{
    LONG x = GETXPARAM(lParam);
    LONG y = GETYPARAM(lParam);
    UINT flag = (UINT)wParam;
    CRect rect(0, 0, 0, 0);
    GetThumbRect(rect);
    if (rect.PtInRect(CPoint(x, y))
        || ::GetCapture() == m_hWnd) {
        m_bInThumbOrDrag = TRUE;
    } else {
        m_bInThumbOrDrag = FALSE;
    }

    return CSkinItem::OnMouseMove(uMsg, wParam, lParam, lpResult);
}

BOOL CSkinItemSlider::OnCaptureChanged(UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* lpResult)
{
    m_bInThumbOrDrag = FALSE;

    return CSkinItem::OnCaptureChanged(uMsg, wParam, lParam, lpResult);
}

BOOL CSkinItemSlider::OnSetCursor(UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* lpResult)
{
    HCURSOR hCur = GetSkinCursor(NULL);
    if (m_bInThumbOrDrag && hCur != NULL) {
        ::SetCursor(hCur);
        return TRUE;
    }
    return CSkinItem::OnSetCursor(uMsg, wParam, lParam, lpResult);
}

