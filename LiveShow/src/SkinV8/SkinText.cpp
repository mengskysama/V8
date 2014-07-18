// SkinText.cpp : implementation file
//

#include "stdafx.h"
#include "SkinText.h"
#include "SkinItemDialog.h"
#include "RenderEngine.h"
#include ".\skintext.h"
#include "SkinHelper.h"


// CSkinText

IMPLEMENT_DYNCREATE(CSkinText, CSkinBase)

CSkinText::CSkinText()
{
	EnableAutomation();

	m_enMouseState = MOUSE_NORMAL;

    m_clrBkColorNormal = -1;

	m_rcTextArea = CRect(0, 0, 0, 0);

	//文字排布
	m_iMaxWidth = -1;		//无限制
	m_iMaxHeight = -1;		//无限制
	m_bAdjustEnable = false;

    m_bNormalUnderline = FALSE;

	//滚动
	m_uScrollFlag = 0x00000000;
	m_nScrollOffset = 0;
	m_nScrollSpeed = 2;
	m_tScrollTimer = NULL;

	m_bBackground = TRUE;

    m_bColorTransform = FALSE;

    m_nSelStart = -1;
    m_nSelEnd = -1;
}

CSkinText::~CSkinText()
{

}

void CSkinText::OnFinalRelease()
{
	// When the last reference for an automation object is released
	// OnFinalRelease is called.  The base class will automatically
	// deletes the object.  Add additional cleanup required for your
	// object before calling the base class.

	CCmdTarget::OnFinalRelease();
}

BEGIN_MESSAGE_MAP(CSkinText, CSkinBase)
END_MESSAGE_MAP()

BEGIN_SKIN_DISPATCH_MAP(CSkinText, CSkinBase)
	SKINBASE_DISP_PROPERTIES(CSkinText)

    SKIN_DISP_PROPERTY(CSkinText, bkColorNormal, VT_I4)

	SKIN_DISP_PROPERTY(CSkinText, bkMode, VT_BSTR)
    SKIN_DISP_PROPERTY(CSkinText, bkColor, VT_I4)
	SKIN_DISP_PROPERTY(CSkinText, fontSize, VT_I4)
	SKIN_DISP_PROPERTY(CSkinText, fontWeight, VT_I4)
	SKIN_DISP_PROPERTY(CSkinText, italic, VT_BOOL)
	SKIN_DISP_PROPERTY(CSkinText, underline, VT_BOOL)
    SKIN_DISP_PROPERTY(CSkinText, normalUnderline, VT_BOOL)
	SKIN_DISP_PROPERTY(CSkinText, strikeOut, VT_BOOL)
	SKIN_DISP_PROPERTY(CSkinText, fontFamily, VT_BSTR)
	SKIN_DISP_PROPERTY(CSkinText, normalColor, VT_I4)
	SKIN_DISP_PROPERTY(CSkinText, hoverColor, VT_I4)
	SKIN_DISP_PROPERTY(CSkinText, downColor, VT_I4)
	SKIN_DISP_PROPERTY(CSkinText, adjustEnable, VT_BOOL)
	SKIN_DISP_PROPERTY(CSkinText, wordBreak, VT_BOOL)
	SKIN_DISP_PROPERTY(CSkinText, singleLine, VT_BOOL)
	SKIN_DISP_PROPERTY(CSkinText, endEllipsis, VT_BOOL)
	SKIN_DISP_PROPERTY(CSkinText, noPrefix, VT_BOOL)
	SKIN_DISP_PROPERTY(CSkinText, maxWidth, VT_I4)
	SKIN_DISP_PROPERTY(CSkinText, maxHeight, VT_I4)
	SKIN_DISP_PROPERTY(CSkinText, horizAlign, VT_BSTR)
	SKIN_DISP_PROPERTY(CSkinText, vertiAlign, VT_BSTR)
	SKIN_DISP_PROPERTY(CSkinText, scrollable, VT_BOOL)
	SKIN_DISP_PROPERTY(CSkinText, scrollDirection, VT_BSTR)
	SKIN_DISP_PROPERTY(CSkinText, scrollSpeed, VT_I4)
	SKIN_DISP_PROPERTY(CSkinText, textWidth, VT_I4)
	SKIN_DISP_PROPERTY(CSkinText, textHeight, VT_I4)
	SKIN_DISP_PROPERTY(CSkinText, lineMargin, VT_I4)
	SKIN_DISP_PROPERTY(CSkinText, underlineMargin, VT_I4)
    SKIN_DISP_PROPERTY(CSkinText, ignoreCRLF, VT_BOOL)

    SKIN_DISP_PROPERTY(CSkinText, selStart, VT_I4)
    SKIN_DISP_PROPERTY(CSkinText, selEnd, VT_I4)
    SKIN_DISP_FUNCTION(CSkinText, GetSelText, VT_BSTR, VTS_NONE)
    SKIN_DISP_FUNCTION(CSkinText, Hittest, VT_I4, VTS_I4 VTS_I4)

	SKIN_DISP_PROPERTY_ALIAS(CSkinText, value, text, VT_BSTR)
	SKIN_DISP_PROPERTY_ALIAS(CSkinText, color, normalColor, VT_I4)
	SKIN_DISP_PROPERTY_ALIAS(CSkinText, autoSpace, adjustEnable, VT_BOOL)

END_SKIN_DISPATCH_MAP(CSkinText)

// Note: we add support for IID_ISkinText to support typesafe binding
//  from VBA.  This IID must match the GUID that is attached to the 
//  dispinterface in the .IDL file.

// {29FD8AF3-2631-42DA-8316-70C37B9A9988}
static const IID IID_ISkinText =
{ 0x29FD8AF3, 0x2631, 0x42DA, { 0x83, 0x16, 0x70, 0xC3, 0x7B, 0x9A, 0x99, 0x88 } };

BEGIN_INTERFACE_MAP(CSkinText, CSkinBase)
	INTERFACE_PART(CSkinText, IID_ISkinText, Dispatch)
END_INTERFACE_MAP()


// CSkinText message handlers
BOOL CSkinText::OnMouseEnter()
{
    if(	GetDisabled() ) {
        return FALSE;
    }

    m_enMouseState = MOUSE_HOVER;
	InvalidateRect();

	return CSkinBase::OnMouseEnter();
}

BOOL CSkinText::OnMouseLeave()
{
    if(	GetDisabled() ) {
        return FALSE;
    }

    m_enMouseState = MOUSE_NORMAL;
	InvalidateRect();

	return CSkinBase::OnMouseLeave();
}

BOOL CSkinText::OnLButtonDown(LONG x, LONG y, UINT flag)
{
    if(	GetDisabled() ) {
        return FALSE;
    }

	if (IsPointInSkinRgn(x, y)) {
		SetCaptureSkin();
	} else {
		ReleaseCaptureSkin();
	}

	m_enMouseState = MOUSE_DOWN;
	InvalidateRect();

	return CSkinBase::OnLButtonDown(x, y, flag);
}

BOOL CSkinText::OnLButtonUp(LONG x, LONG y, UINT flag)
{
    if(	GetDisabled() ) {
        return FALSE;
    }

    CSkinBase* pCapture = NULL;
	if (GetCaptureSkin(&pCapture)) {
		if (pCapture == this 
			&& IsPointInSkinRgn(x, y) 
			&& m_rcTextArea.PtInRect(CPoint(x, y)) )
		{
			OnClick(0);
		}
	}
	ReleaseCaptureSkin();

	m_enMouseState = MOUSE_HOVER;
	InvalidateRect();

	return CSkinBase::OnLButtonUp(x, y, flag);
}

BOOL CSkinText::OnClick(LONG iTab)
{
	m_enMouseState = MOUSE_DOWN;

	InvalidateRect();

	return CSkinBase::OnClick(0);
}

BOOL CSkinText::OnTimer(UINT nIDEvent)
{
	switch (nIDEvent)
	{
	case SKIN_SCROLL_TIMER:
		{
			if ( (m_uScrollFlag & SKIN_SCROLL_ENABLE) != 0 && m_enMouseState != MOUSE_HOVER){
				m_nScrollOffset += m_nScrollSpeed;
				InvalidateRect(NULL, FALSE);
			}else{
				KillTimer(SKIN_SCROLL_TIMER);
				m_tScrollTimer = NULL;
			}
		}
		break;
	default:
		break;
	}
	return CSkinBase::OnTimer(nIDEvent);
}

HCURSOR CSkinText::GetSkinCursor(LPPOINT lpPoint)
{
	if (lpPoint == NULL)
		return NULL;

    if (GetDisabled())
        return NULL;

	if (m_rcTextArea.PtInRect(*lpPoint)) {
        if (m_hCursor)
		    return m_hCursor;
        if (m_hSysCursor)
            return m_hSysCursor;
        return NULL;
	} else {
		return NULL;
	}
}

BOOL CSkinText::UpdateFont(HDC hDC)
{
	if (hDC != NULL)
		return m_textStyleNormal.UpdateFont(hDC);

	HWND hWnd = GetSkinHwnd();
	if (! ::IsWindow(hWnd))
		return FALSE;

	CWnd* pWnd = CWnd::FromHandle(hWnd);
	CWindowDC wndDC(pWnd);
	if (wndDC.GetSafeHdc() == NULL)
		return FALSE;

	return m_textStyleNormal.UpdateFont(wndDC.GetSafeHdc());
}

BOOL CSkinText::GetTextSize(CSize& size, HDC hDC)
{
	if (hDC != NULL)
		return m_textStyleNormal.GetTextSize(hDC, m_strText, size);

	HWND hWnd = GetSkinHwnd();
	if (::IsWindow(hWnd)) {
		CWindowDC wndDC(CWnd::FromHandle(hWnd));
		return m_textStyleNormal.GetTextSize(wndDC.GetSafeHdc(), m_strText, size);
	}

	return m_textStyleNormal.GetTextSize(NULL, m_strText, size);
}

BOOL CSkinText::AdjustRect(HDC hDC)
{
	CSize textSize;
	if (! GetTextSize(textSize, hDC))
		return FALSE;

	//调整矩形
	if (m_rcSkinRect.Width() < textSize.cx){
		if (m_iMaxWidth < 0){
			Setwidth(textSize.cx);
		}else{
			Setwidth(min(m_iMaxWidth, textSize.cx));
		}
	}
	if (m_rcSkinRect.Height() < textSize.cy){
		if (m_iMaxHeight < 0){
			Setheight(textSize.cy);
		}else{
			Setheight(min(m_iMaxHeight, textSize.cy));
		}
	}

	return TRUE;
}

void CSkinText::Draw(CDC* pDC, LPRECT lpInvalidRect, CRgn* pInvalidRgn)
{
	//创建字体
	if (m_textStyleNormal.IsFontChanged()) {
		UpdateFont(*pDC);
	}

	//自动调整
	if (m_bAdjustEnable){
		AdjustRect(*pDC);
	}

    //绘制背景色
	CRect drawRect = GetSkinRect();
    if (m_clrBkColorNormal != -1) {
        CRenderEngine::FillRect(*pDC, drawRect, m_clrBkColorNormal);
    }

	//选择颜色
	LONG color = m_textStyleNormal.Getcolor();
    VARIANT_BOOL newUnderline = GetnormalUnderline();
	switch (m_enMouseState)
	{
	case MOUSE_NORMAL:
		color = m_textStyleNormal.Getcolor();
        newUnderline = GetnormalUnderline();
		break;
	case MOUSE_HOVER:
		if (m_textStyleHover.Getcolor() != -1)
			color = m_textStyleHover.Getcolor();
        newUnderline = m_textStyleNormal.Getunderline();
		break;
	case MOUSE_DOWN:
		if (m_textStyleDown.Getcolor() != -1)
			color = m_textStyleDown.Getcolor();
        newUnderline = m_textStyleNormal.Getunderline();
		break;
	default:
		break;
	}
    if (GetColorTransform())
        GetEngine()->TransformColor(color, color);

	CRect oldDrawRect = GetSkinRect();
	CSize textSize;
	GetTextSize(textSize, *pDC);

	if ( (m_uScrollFlag & SKIN_SCROLL_ENABLE) != 0 ){
		int oldScrollOffset = m_nScrollOffset - m_nScrollSpeed;
		if ( (m_uScrollFlag & SKIN_SCROLL_DIRECTION) == 0 ){
			//计算旧的位置
			oldScrollOffset = oldScrollOffset % (drawRect.Width() + textSize.cx);
			if (m_nScrollSpeed >= 0)
				oldDrawRect.OffsetRect(oldScrollOffset - textSize.cx, 0);
			else
				oldDrawRect.OffsetRect(oldScrollOffset + drawRect.Width(), 0);

			//新的位置
			m_nScrollOffset = m_nScrollOffset % (drawRect.Width() + textSize.cx);
			if (m_nScrollSpeed >= 0)
				drawRect.OffsetRect(m_nScrollOffset - textSize.cx, 0);
			else
				drawRect.OffsetRect(m_nScrollOffset + drawRect.Width(), 0);
		}else{
			oldScrollOffset = oldScrollOffset % (drawRect.Height() + textSize.cy);
			//计算旧的位置
			if (m_nScrollSpeed >= 0)
				oldDrawRect.OffsetRect(0, oldScrollOffset);
			else
				oldDrawRect.OffsetRect(0, oldScrollOffset + drawRect.Height());

			//新的位置
			m_nScrollOffset = m_nScrollOffset % (drawRect.Height() + textSize.cy);
			if (m_nScrollSpeed >= 0)
				drawRect.OffsetRect(0, m_nScrollOffset);
			else
				drawRect.OffsetRect(0, m_nScrollOffset + drawRect.Height());
		}
		drawRect.right = drawRect.left + textSize.cx;
	}

    CString strSel = InternalGetSelText();
    if (strSel.IsEmpty()) {
        VARIANT_BOOL oldUnderline = m_textStyleNormal.Getunderline();
        LONG oldColor = m_textStyleNormal.Getcolor();
        m_textStyleNormal.Setunderline(newUnderline);
        m_textStyleNormal.Setcolor(color);
        m_textStyleNormal.DrawText(*pDC, m_strText, &drawRect, &m_rcTextArea);
        m_textStyleNormal.Setcolor(oldColor);
        m_textStyleNormal.Setunderline(oldUnderline);
    } else {
        LONG nStart = min(m_nSelStart, m_nSelEnd);
        nStart = max(nStart, 0);
        LONG nEnd = max(m_nSelStart, m_nSelEnd);
        nEnd = min(nEnd, m_strText.GetLength());

        VARIANT_BOOL oldUnderline = m_textStyleNormal.Getunderline();
        LONG oldColor = m_textStyleNormal.Getcolor();
        m_textStyleNormal.Setunderline(newUnderline);
        m_textStyleNormal.Setcolor(color);

        //绘制选中部分左侧文字
        CString strLeft = m_strText.Left(nStart);
        CRect rcLeft = drawRect;
        CRect rcLeftArea = drawRect;
        m_textStyleNormal.DrawText(*pDC, strLeft, &rcLeft, &rcLeftArea);

        //绘制选中文字
        strSel;
        CRect rcSel = drawRect;
        rcSel.left = rcLeftArea.right;
        CRect rcSelArea = rcSel;
        m_textStyleNormal.DrawTextEx(*pDC, strSel, &rcSel, RGB(255, 255, 255), &rcSelArea);

        //绘制选中文字右侧文字
        CString strRight = m_strText.Right(m_strText.GetLength() - nEnd);
        CRect rcRight = drawRect;
        rcRight.left = rcSelArea.right;
        CRect rcRightArea = drawRect;
        m_textStyleNormal.DrawText(*pDC, strRight, &rcRight, &rcRightArea);

        m_rcTextArea = drawRect;
        m_rcTextArea.right = rcRightArea.right;

        m_textStyleNormal.Setcolor(oldColor);
        m_textStyleNormal.Setunderline(oldUnderline);
    }
}

//////////////////////////////////////////////////////////////////////////
//设置文字内容
//////////////////////////////////////////////////////////////////////////
BSTR CSkinText::GetText(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: 在此添加调度处理程序代码
	return m_strText.AllocSysString();
}

void CSkinText::SetText(LPCTSTR newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: 在此添加属性处理程序代码
	CSkinBase::SetText(newVal);
	if (m_bAdjustEnable)
		AdjustRect(NULL);
}

//////////////////////////////////////////////////////////////////////////
//设置字体的样式、大小
//////////////////////////////////////////////////////////////////////////
LONG CSkinText::GetbkColorNormal(void)
{
    return m_clrBkColorNormal;
}

void CSkinText::SetbkColorNormal(LONG newVal)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());
    if (m_clrBkColorNormal == newVal)
        return;

    m_clrBkColorNormal = newVal;
    InvalidateRect();
}

LONG CSkinText::GetbkColor(void)
{
	return m_textStyleNormal.GetbkColor();
}

void CSkinText::SetbkColor(LONG newVal)
{
	m_textStyleNormal.SetbkColor(newVal);
	InvalidateRect();
}

BSTR CSkinText::GetbkMode(void)
{
	return m_textStyleNormal.GetbkMode();
}

void CSkinText::SetbkMode(LPCTSTR newVal)
{
	m_textStyleNormal.SetbkMode(newVal);
	InvalidateRect();
}

LONG CSkinText::GetfontSize(void)
{
	return m_textStyleNormal.GetfontSize();
}

void CSkinText::SetfontSize(LONG newVal)
{
	m_textStyleNormal.SetfontSize(newVal);
	InvalidateRect();
}

LONG CSkinText::GetfontWeight(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: 在此添加调度处理程序代码
	return m_textStyleNormal.GetfontWeight();
}

void CSkinText::SetfontWeight(LONG newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: 在此添加调度处理程序代码
	m_textStyleNormal.SetfontWeight(newVal);
	InvalidateRect();
}

VARIANT_BOOL CSkinText::Getitalic(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: 在此添加调度处理程序代码
	return m_textStyleNormal.Getitalic();
}

void CSkinText::Setitalic(VARIANT_BOOL newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: 在此添加属性处理程序代码
	m_textStyleNormal.Setitalic(newVal);
	InvalidateRect();
}

VARIANT_BOOL CSkinText::Getunderline(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: 在此添加调度处理程序代码
	return m_textStyleNormal.Getunderline();
}

void CSkinText::Setunderline(VARIANT_BOOL newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: 在此添加属性处理程序代码
	m_textStyleNormal.Setunderline(newVal);
	InvalidateRect();
}

VARIANT_BOOL CSkinText::GetnormalUnderline()
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());

    // TODO: 在此添加属性处理程序代码
    return m_bNormalUnderline ? (VARIANT_TRUE) : (VARIANT_FALSE);
}

void CSkinText::SetnormalUnderline(VARIANT_BOOL newVal)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());

    // TODO: 在此添加属性处理程序代码
    newVal = (newVal != FALSE);

    if (m_bNormalUnderline == newVal)
        return;

    m_bNormalUnderline = newVal;
    InvalidateRect();
}

VARIANT_BOOL CSkinText::GetstrikeOut(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: 在此添加调度处理程序代码
	return m_textStyleNormal.GetstrikeOut();
}

void CSkinText::SetstrikeOut(VARIANT_BOOL newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: 在此添加属性处理程序代码
	m_textStyleNormal.SetstrikeOut(newVal);
	InvalidateRect();
}

BSTR CSkinText::GetfontFamily(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: 在此添加调度处理程序代码
	return m_textStyleNormal.GetfontFamily();
}

void CSkinText::SetfontFamily(LPCTSTR newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: 在此添加调度处理程序代码
	m_textStyleNormal.SetfontFamily(newVal);
	InvalidateRect();
}

//////////////////////////////////////////////////////////////////////////
//设置颜色
//////////////////////////////////////////////////////////////////////////
LONG CSkinText::GetnormalColor(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: 在此添加调度处理程序代码
	return m_textStyleNormal.Getcolor();
}

void CSkinText::SetnormalColor(LONG newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: 在此添加属性处理程序代码
	m_textStyleNormal.Setcolor(newVal);
	InvalidateRect();
}

LONG CSkinText::GethoverColor(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: 在此添加调度处理程序代码
	return m_textStyleHover.Getcolor();
}

void CSkinText::SethoverColor(LONG newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: 在此添加属性处理程序代码
	m_textStyleHover.Setcolor(newVal);
	InvalidateRect();
}

LONG CSkinText::GetdownColor(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: 在此添加调度处理程序代码
	return m_textStyleDown.Getcolor();
}

void CSkinText::SetdownColor(LONG newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: 在此添加属性处理程序代码
	m_textStyleDown.Setcolor(newVal);
	InvalidateRect();
}

//////////////////////////////////////////////////////////////////////////
//设置文字排布格式
//////////////////////////////////////////////////////////////////////////
VARIANT_BOOL CSkinText::GetadjustEnable(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: 在此添加调度处理程序代码
	return m_bAdjustEnable ? (VARIANT_TRUE) : (VARIANT_FALSE);
}

void CSkinText::SetadjustEnable(VARIANT_BOOL newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: 在此添加属性处理程序代码
	m_bAdjustEnable = (newVal != 0);
	if (m_bAdjustEnable){
		AdjustRect(NULL);
	}
	InvalidateRect();
}

VARIANT_BOOL CSkinText::GetwordBreak(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: 在此添加调度处理程序代码
	return m_textStyleNormal.GetwordBreak();
}

void CSkinText::SetwordBreak(VARIANT_BOOL newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: 在此添加属性处理程序代码
	m_textStyleNormal.SetwordBreak(newVal);
	InvalidateRect();
}

VARIANT_BOOL CSkinText::GetsingleLine(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: 在此添加调度处理程序代码
	return m_textStyleNormal.GetsingleLine();
}

void CSkinText::SetsingleLine(VARIANT_BOOL newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: 在此添加属性处理程序代码
	m_textStyleNormal.SetsingleLine(newVal);
	InvalidateRect();
}

VARIANT_BOOL CSkinText::GetendEllipsis(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: 在此添加调度处理程序代码
	return m_textStyleNormal.GetendEllipsis();
}

void CSkinText::SetendEllipsis(VARIANT_BOOL newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: 在此添加属性处理程序代码
	m_textStyleNormal.SetendEllipsis(newVal);
	InvalidateRect();
}

VARIANT_BOOL CSkinText::GetnoPrefix(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: 在此添加调度处理程序代码
	return m_textStyleNormal.GetnoPrefix();
}

void CSkinText::SetnoPrefix(VARIANT_BOOL newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: 在此添加属性处理程序代码
	m_textStyleNormal.SetnoPrefix(newVal);
	InvalidateRect();
}

LONG CSkinText::GetmaxWidth(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: 在此添加调度处理程序代码
	return m_iMaxWidth;
}

void CSkinText::SetmaxWidth(LONG newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: 在此添加属性处理程序代码
	const int width = Getwidth();
	if (newVal < width){
		newVal = width;
	}
	m_iMaxWidth = newVal;
}

LONG CSkinText::GetmaxHeight(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: 在此添加调度处理程序代码
	return m_iMaxHeight;
}

void CSkinText::SetmaxHeight(LONG newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: 在此添加属性处理程序代码
	const int height = Getheight();
	if (newVal < height){
		newVal = height;
	}
	m_iMaxHeight = newVal;
}

BSTR CSkinText::GethorizAlign(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: 在此添加调度处理程序代码
	return m_textStyleNormal.GethorizAlign();
}

void CSkinText::SethorizAlign(LPCTSTR newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: 在此添加属性处理程序代码
	m_textStyleNormal.SethorizAlign(newVal);
	InvalidateRect();
}

BSTR CSkinText::GetvertiAlign(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: 在此添加调度处理程序代码
	return m_textStyleNormal.GetvertiAlign();
}

void CSkinText::SetvertiAlign(LPCTSTR newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: 在此添加属性处理程序代码
	m_textStyleNormal.SetvertiAlign(newVal);
	InvalidateRect();
}

//////////////////////////////////////////////////////////////////////////
//滚动
//////////////////////////////////////////////////////////////////////////
VARIANT_BOOL CSkinText::Getscrollable(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: 在此添加属性处理程序代码
	return ( (m_uScrollFlag & SKIN_SCROLL_ENABLE) != 0 ) ? (VARIANT_TRUE) : (VARIANT_FALSE);
}

void CSkinText::Setscrollable(VARIANT_BOOL newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: 在此添加属性处理程序代码
	newVal = (newVal != FALSE);
	if ( newVal == ((m_uScrollFlag & SKIN_SCROLL_ENABLE) != 0) )
		return;

	if (newVal){
		m_uScrollFlag |= SKIN_SCROLL_ENABLE;
		if (m_tScrollTimer == NULL)
			m_tScrollTimer = SetTimer(SKIN_SCROLL_TIMER, SKIN_SCROLL_ECLAPSE);
	}else{
		m_uScrollFlag &= (~SKIN_SCROLL_ENABLE);
		KillTimer(SKIN_SCROLL_TIMER);
		m_tScrollTimer = NULL;
		m_nScrollOffset = 0;
	}
	InvalidateRect();
}

BSTR CSkinText::GetscrollDirection(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: 在此添加属性处理程序代码
	CString strResult = _T("horizontal");
	if ( (m_uScrollFlag & SKIN_SCROLL_DIRECTION) != 0 ){
		strResult = _T("vertical");
	}
	return strResult.AllocSysString();
}

void CSkinText::SetscrollDirection(LPCTSTR newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: 在此添加属性处理程序代码
	CString strDirec = newVal;
	bool bChange = false;
	if ( strDirec.CompareNoCase(_T("horizontal")) == 0 ){
		if ( (m_uScrollFlag & SKIN_SCROLL_DIRECTION) != 0 ){
			m_uScrollFlag &= (~SKIN_SCROLL_DIRECTION);
			bChange = true;
		}
	}else if ( strDirec.CompareNoCase(_T("vertical")) == 0 ){
		if ( (m_uScrollFlag & SKIN_SCROLL_DIRECTION) == 0 ){
			m_uScrollFlag |= SKIN_SCROLL_DIRECTION;
			bChange = true;
		}
	}
	if (bChange)
		InvalidateRect();
}

LONG CSkinText::GetscrollSpeed(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: 在此添加属性处理程序代码
	return m_nScrollSpeed;
}

void CSkinText::SetscrollSpeed(LONG newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: 在此添加属性处理程序代码
	if (newVal == m_nScrollSpeed)
		return;
	m_nScrollSpeed = newVal;
	InvalidateRect();
}

LONG CSkinText::GettextWidth(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	
	CSize textSize(Getwidth(), Getheight());
	GetTextSize(textSize);
	return textSize.cx;
}

void CSkinText::SettextWidth(LONG newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	
	//
	ASSERT(0 && _T("CSkinText::SettextWidth 方法不可用"));
}

LONG CSkinText::GettextHeight(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CSize textSize(Getwidth(), Getheight());
	GetTextSize(textSize);
	return textSize.cy;
}

void CSkinText::SettextHeight(LONG newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	//
	ASSERT(0 && _T("CSkinText::SettextHeight 方法不可用"));
}

//行间距
LONG CSkinText::GetlineMargin(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	return m_textStyleNormal.GetlineMargin();
}

void CSkinText::SetlineMargin(LONG newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: 在此添加属性处理程序代码
	m_textStyleNormal.SetlineMargin(newVal);
	InvalidateRect();
}

//下划线间距
LONG CSkinText::GetunderlineMargin(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	return m_textStyleNormal.GetunderlineMargin();
}

void CSkinText::SetunderlineMargin(LONG newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: 在此添加属性处理程序代码
	m_textStyleNormal.SetunderlineMargin(newVal);
	InvalidateRect();
}

//下划线间距
VARIANT_BOOL CSkinText::GetignoreCRLF(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	return m_textStyleNormal.GetignoreCRLF();
}

void CSkinText::SetignoreCRLF(VARIANT_BOOL newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: 在此添加属性处理程序代码
	m_textStyleNormal.SetignoreCRLF(newVal);
	InvalidateRect();
}

//下划线间距
LONG CSkinText::GetselStart(void)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());
    return m_nSelStart;
}

void CSkinText::SetselStart(LONG newVal)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());
    if (m_nSelStart == newVal)
        return;

    m_nSelStart = newVal;
    InvalidateRect();
}

//下划线间距
LONG CSkinText::GetselEnd(void)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());
    return m_nSelEnd;
}

void CSkinText::SetselEnd(LONG newVal)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());
    if (m_nSelEnd == newVal)
        return;

    m_nSelEnd = newVal;
    InvalidateRect();
}

CString CSkinText::InternalGetSelText()
{
    LONG nStart = min(m_nSelStart, m_nSelEnd);
    nStart = max(nStart, 0);
    LONG nEnd = max(m_nSelStart, m_nSelEnd);
    nEnd = min(nEnd, m_strText.GetLength());

    if (nStart == nEnd)
        return _T("");

    CString strSel = m_strText;
    strSel = strSel.Left(nEnd);
    strSel = strSel.Right(nEnd - nStart);

    return strSel;
}

LONG CSkinText::InternalHittest(LONG x, LONG y)
{
    x -= GetSkinRect().left;
    return m_textStyleNormal.HitTest(m_strText, x, y);
}

BSTR CSkinText::GetSelText()
{
    return InternalGetSelText().AllocSysString();
}

LONG CSkinText::Hittest(LONG x, LONG y)
{
    return InternalHittest(x, y);
}

#ifdef V8_JS_ENGINE_SUPPORT

v8::Handle<v8::Value> CSkinText::V8GetSelText(const v8::Arguments& args)
{
    v8::HandleScope handleScope;

    v8::Handle<v8::Value> result = CStringToString(InternalGetSelText());

    return handleScope.Close(result);
}

v8::Handle<v8::Value> CSkinText::V8Hittest(const v8::Arguments& args)
{
    v8::HandleScope handleScope;

    LONG x = v8::Handle<v8::Integer>::Cast(args[0])->Int32Value();
    LONG y = v8::Handle<v8::Integer>::Cast(args[1])->Int32Value();
    v8::Handle<v8::Value> result = v8::Integer::New(InternalHittest(x, y));

    return handleScope.Close(result);
}

v8::Handle<v8::Value> CSkinText::V8InternalCallMethod(const CString& funcName, const v8::Arguments& args)
{
    if (funcName.CompareNoCase(_T("GetSelText")) == 0) {
        return V8GetSelText(args);
    } else if (funcName.CompareNoCase(_T("Hittest")) == 0) {
        return V8Hittest(args);
    } 

    return __super::V8InternalCallMethod(funcName, args);
}

#endif //V8_JS_ENGINE_SUPPORT
