// SkinTextStyle.cpp : 实现文件
//

#include "stdafx.h"
#include "SkinTextStyle.h"

#include "RenderEngine.h"


// CSkinTextStyle

IMPLEMENT_DYNCREATE(CSkinTextStyle, CXmlCmdTarget)
CSkinTextStyle::CSkinTextStyle()
{
	EnableAutomation();
	
	// 为了使应用程序在 OLE 自动化对象处于活动状态时保持
	//	运行，构造函数调用 AfxOleLockApp。

	memset(&m_lfFont, 0, sizeof(LOGFONT));
	m_lfFont.lfWeight = FW_NORMAL;
	CString strFaceName = _T("tahoma");
	_macro_tcsncpy(m_lfFont.lfFaceName, LF_FACESIZE-1, strFaceName, LF_FACESIZE-1);

	m_bUnderline = FALSE;

	m_clrColor = RGB(0, 0, 0);//RGB(0x18, 0x27, 0x5E);

	m_clrBkColor = RGB(255, 255, 255);

	m_uBkMode = TRANSPARENT;

	m_iFontSize = 8;

	m_uFormat = DT_VCENTER | DT_LEFT | DT_END_ELLIPSIS | DT_SINGLELINE | DT_NOPREFIX;

	m_nBeginLineMargin = 0;	//第一行与上边间距
	m_nLineMargin = 0;		//多行的行间距
	m_nEndLineMargin = 0;	//最后一行与下边间距
	m_nUnderlineMargin = 1;

	m_bShadow = FALSE;
    m_bIgnoreCRLF = FALSE;

	m_bFontChanged = TRUE;

	AfxOleLockApp();
}

CSkinTextStyle::~CSkinTextStyle()
{
	// 为了在用 OLE 自动化创建所有对象后终止应用程序，
	// 	析构函数调用 AfxOleUnlockApp。

    if (m_fFont != NULL) {
        ::DeleteObject(m_fFont);
        m_fFont = NULL;
    }
	
	AfxOleUnlockApp();
}

CSkinTextStyle& CSkinTextStyle::operator = (const CSkinTextStyle& style)
{
	if (this != &style) {
		m_lfFont = style.m_lfFont;
		//m_fFont = style.m_fFont;
		m_clrColor = style.m_clrColor;
		m_clrBkColor = style.m_clrBkColor;
		m_iFontSize = style.m_iFontSize;
		m_uBkMode = style.m_uBkMode;
		m_uFormat = style.m_uFormat;
		m_nBeginLineMargin = style.m_nBeginLineMargin;
		m_nLineMargin = style.m_nLineMargin;
		m_nEndLineMargin = style.m_nEndLineMargin;
		m_nUnderlineMargin = style.m_nUnderlineMargin;
		m_bShadow = style.m_bShadow;
		m_bFontChanged = TRUE;
		m_bFormatChanged = TRUE;
	}
	return *this;
}

void CSkinTextStyle::OnFinalRelease()
{
	// 释放了对自动化对象的最后一个引用后，将调用
	// OnFinalRelease。基类将自动
	// 删除该对象。在调用该基类之前，请添加您的
	// 对象所需的附加清除代码。

	CCmdTarget::OnFinalRelease();
}


BEGIN_MESSAGE_MAP(CSkinTextStyle, CXmlCmdTarget)
END_MESSAGE_MAP()


BEGIN_SKIN_DISPATCH_MAP(CSkinTextStyle, CXmlCmdTarget)
	SKIN_DISP_PROPERTY(CSkinTextStyle, color, VT_I4)
	SKIN_DISP_PROPERTY(CSkinTextStyle, bkColor, VT_I4)
	SKIN_DISP_PROPERTY(CSkinTextStyle, bkMode, VT_BSTR)
	SKIN_DISP_PROPERTY(CSkinTextStyle, fontSize, VT_I4)
	SKIN_DISP_PROPERTY(CSkinTextStyle, fontWeight, VT_I4)
	SKIN_DISP_PROPERTY(CSkinTextStyle, italic, VT_BOOL)
	SKIN_DISP_PROPERTY(CSkinTextStyle, underline, VT_BOOL)
	SKIN_DISP_PROPERTY(CSkinTextStyle, strikeOut, VT_BOOL)
	SKIN_DISP_PROPERTY(CSkinTextStyle, fontFamily, VT_BSTR)
	SKIN_DISP_PROPERTY(CSkinTextStyle, wordBreak, VT_BOOL)
	SKIN_DISP_PROPERTY(CSkinTextStyle, endEllipsis, VT_BOOL)
	SKIN_DISP_PROPERTY(CSkinTextStyle, noPrefix, VT_BOOL)
	SKIN_DISP_PROPERTY(CSkinTextStyle, singleLine, VT_BOOL)
	SKIN_DISP_PROPERTY(CSkinTextStyle, horizAlign, VT_BSTR)
	SKIN_DISP_PROPERTY(CSkinTextStyle, vertiAlign, VT_BSTR)
	SKIN_DISP_PROPERTY(CSkinTextStyle, shadow, VT_BOOL)
	SKIN_DISP_PROPERTY(CSkinTextStyle, beginLineMargin, VT_I4)
	SKIN_DISP_PROPERTY(CSkinTextStyle, lineMargin, VT_I4)
	SKIN_DISP_PROPERTY(CSkinTextStyle, endLineMargin, VT_I4)
	SKIN_DISP_PROPERTY(CSkinTextStyle, underlineMargin, VT_I4)
    SKIN_DISP_PROPERTY(CSkinTextStyle, ignoreCRLF, VT_BOOL)
END_SKIN_DISPATCH_MAP(CSkinTextStyle)

// 注意: 我们添加 IID_ISkinTextStyle 支持
//以支持来自 VBA 的类型安全绑定。此 IID 必须同附加到 .IDL 文件中的
//调度接口的 GUID 匹配。

// {06A12AF4-0B00-4D87-9168-427211FD46F2}
static const IID IID_ISkinTextStyle =
{ 0x6A12AF4, 0xB00, 0x4D87, { 0x91, 0x68, 0x42, 0x72, 0x11, 0xFD, 0x46, 0xF2 } };

BEGIN_INTERFACE_MAP(CSkinTextStyle, CXmlCmdTarget)
	INTERFACE_PART(CSkinTextStyle, IID_ISkinTextStyle, Dispatch)
END_INTERFACE_MAP()

// {6F3649EA-8298-4325-AB89-2A1CE7A5E4AE}
IMPLEMENT_OLECREATE_FLAGS(CSkinTextStyle, "Skin.SkinTextStyle", afxRegApartmentThreading, 0x6f3649ea, 0x8298, 0x4325, 0xab, 0x89, 0x2a, 0x1c, 0xe7, 0xa5, 0xe4, 0xae)


// CSkinTextStyle 消息处理程序
BOOL CSkinTextStyle::IsChanged()
{
	return IsFontChanged() || IsFormatChanged();
}

BOOL CSkinTextStyle::IsFontChanged()
{
	return m_bFontChanged;
}

BOOL CSkinTextStyle::IsFormatChanged()
{
	return m_bFormatChanged;
}

UINT CSkinTextStyle::GetFormat()
{
	return m_uFormat;
}

CFont* CSkinTextStyle::GetFont()
{
    return CFont::FromHandle(m_fFont);
}

UINT CSkinTextStyle::GetBkMode()
{
	return m_uBkMode;
}

BOOL CSkinTextStyle::InternalUpdateFont(HDC hDC)
{
	if (hDC == NULL)
		return FALSE;

	m_lfFont.lfHeight = -::MulDiv(m_iFontSize, ::GetDeviceCaps(hDC, LOGPIXELSY), 72);
    HFONT hUpdateFont = ::CreateFontIndirect(&m_lfFont);
    if (hUpdateFont == NULL)
        return FALSE;

    if (m_fFont != NULL) {
        ::DeleteObject(m_fFont);
    }
    m_fFont = hUpdateFont;

	return TRUE;
}

BOOL CSkinTextStyle::InternalUpdateFormat(HDC hDC)
{
	return FALSE;
}

BOOL CSkinTextStyle::InternalGetTextSize(HDC hDC, LPCTSTR strText, LONG& width, LONG& height)
{
	if (hDC == NULL)
		return FALSE;

	if (m_bFontChanged && !InternalUpdateFont(hDC))
		return FALSE;

	//选择字体，计算文字的矩形
    HGDIOBJ hOldFont = ::SelectObject(hDC, (HGDIOBJ)m_fFont);

	//用GetTextExtentExPoint计算
	CSize newSize;
	int iFit;
	BOOL bRet = ::GetTextExtentExPoint(hDC, strText, 
		(int)_tcslen(strText), 100000, &iFit, NULL, &newSize);
	width = newSize.cx;
	height = newSize.cy;

	//恢复
    ::SelectObject(hDC, hOldFont);

	return bRet;
}

BOOL CSkinTextStyle::UpdateFont(HDC hDC)
{
	if (InternalUpdateFont(hDC)) {
		m_bFontChanged = FALSE;
		return TRUE;
	} else {
		return FALSE;
	}
}

BOOL CSkinTextStyle::UpdateFormat(HDC hDC)
{
	return FALSE;
}

BOOL CSkinTextStyle::GetTextSize(HDC hDC, LPCTSTR strText, LONG& width, LONG& height)
{
	if (hDC != NULL)
		return InternalGetTextSize(hDC, strText, width, height);

	HWND hDCWnd = ::GetDesktopWindow();
	if (hDCWnd == NULL)
		return FALSE;

    CString strText2 = strText;
    if (m_bIgnoreCRLF) {
        strText2.Replace(_T("\r"), _T(""));
        strText2.Replace(_T("\n"), _T(""));
    }
    hDC = ::GetWindowDC(hDCWnd);
	BOOL bRet = InternalGetTextSize(hDC, strText2, width, height);
    ::ReleaseDC(hDCWnd, hDC);

	return bRet;
}

BOOL CSkinTextStyle::GetTextSize(HDC hDC, LPCTSTR strText, CSize& size)
{
	return GetTextSize(hDC, strText, size.cx, size.cy);
}

int CSkinTextStyle::GlideByReturn(const CString& str)
{
	int j = 0;
	for (int i=0; i<str.GetLength()&&str.GetAt(i)==_T('\n'); i++)
		j = i + 1;
	return j;
}

BOOL CSkinTextStyle::DrawText(HDC hDC, LPCTSTR lpzText, LPRECT lpDrawRect, LPRECT lpRealDrawRect)
{
    return DrawTextEx(hDC, lpzText, lpDrawRect, m_clrColor, lpRealDrawRect);
}

BOOL CSkinTextStyle::DrawTextEx(HDC hDC, LPCTSTR lpzText, LPRECT lpDrawRect, LONG clrText, LPRECT lpRealDrawRect)
{
    ASSERT(hDC != NULL);
    if (hDC == NULL)
        return FALSE;

    if (lpDrawRect == NULL)
        return FALSE;

    //实际绘制的文本所占的外接矩形，一般比lpDrawRect小
    CRect realDrawRect(0, 0, 0, 0);

    //更新字体
    if (IsFontChanged())
        UpdateFont(hDC);

    //保存DC后，设置文本颜色、字体、背景模式
    //int iSaveDc = ::SaveDC(hDC);

    //设置文字颜色
    COLORREF oldTextColor = ::SetTextColor(hDC, clrText);

    //选择字体
    HGDIOBJ hOldFont = ::SelectObject(hDC, (HGDIOBJ)m_fFont);

    //背景颜色
    COLORREF oldBkColor = ::SetBkColor(hDC, m_clrBkColor);

    //背景模式
    int oldBkMode = ::SetBkMode(hDC, m_uBkMode);

    //内部使用的下划线间距
    const LONG nUnderlineMargin = m_nUnderlineMargin - 1;

    CString strText = lpzText;
    if (m_bIgnoreCRLF) {
        strText.Replace(_T("\r"), _T(""));
        strText.Replace(_T("\n"), _T(""));
    }

    const CRect drawRect = *lpDrawRect;
    BOOL bRet = FALSE;

    //检查是否有回车
    BOOL bHasReturn = (strText.Find(_T("\n")) != -1);

    if ( m_uFormat & DT_SINGLELINE ) {

        //计算实际尺寸
        int iFit = 0;
        CSize fitSize(0, 0);
        ::GetTextExtentExPoint(hDC, strText, 
            strText.GetLength(), drawRect.Width(), &iFit, NULL, &fitSize);
        realDrawRect = CRect(0, 0, min(fitSize.cx, drawRect.Width()), min(fitSize.cy, drawRect.Height()) );
        realDrawRect.OffsetRect(drawRect.TopLeft());
        if (m_uFormat & DT_CENTER) {
            realDrawRect.OffsetRect( (drawRect.Width() - realDrawRect.Width()) / 2, 0);
        } else if (m_uFormat & DT_RIGHT) {
            realDrawRect.OffsetRect( (drawRect.Width() - realDrawRect.Width()), 0);
        } else{
            //
        }
        if (m_uFormat & DT_VCENTER) {
            realDrawRect.OffsetRect(0, (drawRect.Height() - realDrawRect.Height()) / 2);
        } else if (m_uFormat & DT_BOTTOM) {
            realDrawRect.OffsetRect(0, (drawRect.Height() - realDrawRect.Height()));
        } else{
            //
        }

        bRet = ::DrawText(hDC, strText, -1, &realDrawRect, (m_uFormat & (~DT_CALCRECT)) );

        //绘制下划线
        if (m_bUnderline) {
            CPoint lp = CPoint(realDrawRect.left, realDrawRect.bottom);
            CPoint rp = CPoint(realDrawRect.right, realDrawRect.bottom);
            lp.y += nUnderlineMargin;
            rp.y += nUnderlineMargin;
            CRenderEngine::RenderLine(hDC, lp, rp, clrText);
        }

    } else {

        int iFit = 0;
        CSize fitSize(0, 0);
        CString tempText = strText;
        int posOfRet = tempText.Find(_T("\n"));
        int lenOfLine = (posOfRet != -1) ? (posOfRet) : (tempText.GetLength());
        ::GetTextExtentExPoint(hDC, tempText, 
            lenOfLine, drawRect.Width(), &iFit, NULL, &fitSize);
        if (tempText.GetLength() == iFit) {

            //计算实际尺寸
            realDrawRect = CRect(0, 0, fitSize.cx, fitSize.cy);
            realDrawRect.OffsetRect(drawRect.TopLeft());
            if (m_uFormat & DT_CENTER) {
                realDrawRect.OffsetRect( (drawRect.Width() - realDrawRect.Width()) / 2, 0);
            } else if (m_uFormat & DT_RIGHT) {
                realDrawRect.OffsetRect( (drawRect.Width() - realDrawRect.Width()), 0);
            } else {
                //
            }
            if (m_uFormat & DT_VCENTER) {
                realDrawRect.OffsetRect(0, (drawRect.Height() - realDrawRect.Height()) / 2);
            } else if (m_uFormat & DT_BOTTOM) {
                realDrawRect.OffsetRect(0, (drawRect.Height() - realDrawRect.Height()));
            } else{
                //
            }

            bRet = ::DrawText(hDC, strText, -1, &realDrawRect, (m_uFormat & (~DT_CALCRECT) | DT_SINGLELINE) );

            //绘制下划线
            if (m_bUnderline) {
                CPoint lp = CPoint(realDrawRect.left, realDrawRect.bottom);
                CPoint rp = CPoint(realDrawRect.right, realDrawRect.bottom);
                lp.y += nUnderlineMargin;
                rp.y += nUnderlineMargin;
                CRenderEngine::RenderLine(hDC, lp, rp, clrText);
            }

        } else {
            //计算文字所需高度，包括行间的Margin，不包括上下的Margin
            tempText = strText;

            int iFit = 0;
            CSize fitSize = CSize(0, 0);
            int textTotalHeight = 0;

            //取得一个初始的文字高度，以防止第一个字符为换行的情况
            ::GetTextExtentExPoint(hDC, _T("test"), 
                4, 1000, &iFit, NULL, &fitSize);
            int iReturn = GlideByReturn(tempText);
            if (iReturn > 0) {
                textTotalHeight += (fitSize.cy + m_nLineMargin) * iReturn;
                tempText = tempText.Right(tempText.GetLength() - iReturn);
            }
            int posOfRet = tempText.Find(_T("\n"));
            ASSERT(posOfRet != 0);
            int lenOfLine = (posOfRet != -1) ? (posOfRet) : (tempText.GetLength());
            ::GetTextExtentExPoint(hDC, tempText, 
                lenOfLine, drawRect.Width(), &iFit, NULL, &fitSize);
            while (iFit > 0) {

                textTotalHeight += (fitSize.cy + m_nLineMargin);

                tempText = tempText.Right(tempText.GetLength() - iFit);
                iReturn = GlideByReturn(tempText);
                if (iReturn > 0) {
                    textTotalHeight += (fitSize.cy + m_nLineMargin) * (iReturn - 1);
                    tempText = tempText.Right(tempText.GetLength() - iReturn);
                }

                posOfRet = tempText.Find(_T("\n"));
                ASSERT(posOfRet != 0);
                lenOfLine = (posOfRet != -1) ? (posOfRet) : (tempText.GetLength());
                ::GetTextExtentExPoint(hDC, tempText, 
                    lenOfLine, drawRect.Width(), &iFit, NULL, &fitSize);

            }

            int lineTop = 0;
            if (m_uFormat & DT_VCENTER) {
                int top = (drawRect.Height() - textTotalHeight) / 2;
                if (top < 0)
                    top = 0;
                lineTop = top + m_nBeginLineMargin;
            } else if (m_uFormat & DT_BOTTOM) {
                int top = drawRect.Height() - textTotalHeight;
                lineTop = top - m_nEndLineMargin;
            } else{
                lineTop = m_nBeginLineMargin;
            }

            CRect lineDrawRect(0, 0, 0, 0);
            lineDrawRect.top = drawRect.top;
            lineDrawRect.bottom = lineDrawRect.top;
            lineDrawRect.left = drawRect.left;
            lineDrawRect.right = drawRect.right;

            //计算实际尺寸
            realDrawRect = CRect(0, 0, 0, 0);

            tempText = strText;

            iFit = 0;
            fitSize = CSize(0, 0);

            //取得一个初始的文字高度，以防止第一个字符为换行的情况
            ::GetTextExtentExPoint(hDC, _T("test"), 
                4, 1000, &iFit, NULL, &fitSize);
            iReturn = GlideByReturn(tempText);
            if (iReturn > 0) {
                lineTop += (fitSize.cy + m_nLineMargin) * iReturn;
                tempText = tempText.Right(tempText.GetLength() - iReturn);
            }
            posOfRet = tempText.Find(_T("\n"));
            ASSERT(posOfRet != 0);
            lenOfLine = (posOfRet != -1) ? (posOfRet) : (tempText.GetLength());
            ::GetTextExtentExPoint(hDC, tempText, 
                lenOfLine, drawRect.Width(), &iFit, NULL, &fitSize);
            if (iFit < lenOfLine) {
                lenOfLine = iFit;
                ::GetTextExtentExPoint(hDC, tempText, 
                    lenOfLine, drawRect.Width(), &iFit, NULL, &fitSize);
            }
            int lineNum = 0;
            while (iFit > 0) {

                lineDrawRect.left = drawRect.left;
                lineDrawRect.right = lineDrawRect.left + fitSize.cx;
                if (lineNum != 0) {
                    if (m_uFormat & DT_CENTER) {
                        lineDrawRect.OffsetRect( (drawRect.Width() - realDrawRect.Width()) / 2, 0);
                    } else if (m_uFormat & DT_RIGHT) {
                        lineDrawRect.OffsetRect( (drawRect.Width() - realDrawRect.Width()), 0);
                    } else {
                        //
                    }
                }

                lineDrawRect.top = drawRect.top + lineTop;
                lineDrawRect.bottom = lineDrawRect.top + fitSize.cy;

                if (! (m_uFormat & DT_END_ELLIPSIS)) {
                    if (! ::DrawText(hDC, tempText, iFit, &lineDrawRect, (m_uFormat & (~DT_CALCRECT)) ))
                        bRet = FALSE;
                } else {
                    LONG bottomDistance = (drawRect.bottom - lineDrawRect.bottom);
                    if (bottomDistance >= (fitSize.cy + m_nLineMargin)) {
                        if (! ::DrawText(hDC, tempText, iFit, &lineDrawRect, (m_uFormat & (~DT_CALCRECT)) ))
                            bRet = FALSE;
                    } else if (bottomDistance >= 0) {
                        if (! ::DrawText(hDC, tempText, -1, &lineDrawRect, (m_uFormat & (~DT_CALCRECT)) ))
                            bRet = FALSE;
                    }
                }

                //绘制下划线
                if (m_bUnderline) {
                    CPoint lp = CPoint(lineDrawRect.left, lineDrawRect.bottom);
                    CPoint rp = CPoint(lineDrawRect.right, lineDrawRect.bottom);
                    lp.y += nUnderlineMargin;
                    rp.y += nUnderlineMargin;
                    CRenderEngine::RenderLine(hDC, lp, rp, clrText);
                }

                realDrawRect.UnionRect(&realDrawRect, &lineDrawRect);

                lineTop += (fitSize.cy + m_nLineMargin);

                tempText = tempText.Right(tempText.GetLength() - iFit);
                iReturn = GlideByReturn(tempText);
                if (iReturn > 0) {
                    lineTop += (fitSize.cy + m_nLineMargin) * (iReturn - 1);
                    tempText = tempText.Right(tempText.GetLength() - iReturn);
                }

                posOfRet = tempText.Find(_T("\n"));
                ASSERT(posOfRet != 0);
                lenOfLine = (posOfRet != -1) ? (posOfRet) : (tempText.GetLength());
                ::GetTextExtentExPoint(hDC, tempText, 
                    lenOfLine, drawRect.Width(), &iFit, NULL, &fitSize);
                if (iFit < lenOfLine) {
                    lenOfLine = iFit;
                    ::GetTextExtentExPoint(hDC, tempText, 
                        lenOfLine, drawRect.Width(), &iFit, NULL, &fitSize);
                }

                lineNum ++;

            }

        }

    }

    //恢复DC
    ::SetBkMode(hDC, oldBkMode);
    ::SetBkColor(hDC, oldBkColor);
    ::SelectObject(hDC, hOldFont);
    ::SetTextColor(hDC, oldTextColor);
    //::RestoreDC(hDC, iSaveDc);

    if(lpRealDrawRect != NULL)
        *lpRealDrawRect = realDrawRect;

    return bRet;
}

LONG CSkinTextStyle::HitTest(const CString& strText, LONG x, LONG y)
{
    HWND hDCWnd = ::GetDesktopWindow();
    if (hDCWnd == NULL)
        return -1;

    HDC hDC = ::GetWindowDC(hDCWnd);
    if (IsFontChanged())
        UpdateFont(hDC);

    HGDIOBJ hOldFont = ::SelectObject(hDC, (HGDIOBJ)m_fFont);
    INT iFit = 0;
    SIZE szFit;
    ::GetTextExtentExPoint(hDC, strText, strText.GetLength(), x, &iFit, NULL, &szFit);
    ::SelectObject(hDC, hOldFont);
    ::ReleaseDC(hDCWnd, hDC);

    return iFit;
}

LONG CSkinTextStyle::Getcolor(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: 在此添加调度处理程序代码
	return m_clrColor;
}

void CSkinTextStyle::Setcolor(LONG newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: 在此添加属性处理程序代码
	COLORREF color = newVal;
	if (m_clrColor == color)
		return;
	m_clrColor = color;
	m_bFontChanged = TRUE;
}

LONG CSkinTextStyle::GetbkColor(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: 在此添加调度处理程序代码
	return m_clrBkColor;
}

void CSkinTextStyle::SetbkColor(LONG newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: 在此添加属性处理程序代码
	COLORREF color = newVal;
	if (m_clrBkColor == color)
		return;
	m_clrBkColor = color;
	m_bFontChanged = TRUE;
}

BSTR CSkinTextStyle::GetbkMode(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CString strResult;

	// TODO: 在此添加调度处理程序代码
	switch (m_uBkMode)
	{
	case TRANSPARENT:
		strResult = _T("transparent");
		break;
	case OPAQUE:
		strResult = _T("opaque");
		break;
	default:
		strResult = _T("");
		break;
	}
	return strResult.AllocSysString();
}

void CSkinTextStyle::SetbkMode(LPCTSTR newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: 在此添加属性处理程序代码
	CString strMode = newVal;
	UINT mode = 0;

	if (strMode.CompareNoCase(_T("transparent")) == 0){
		mode = TRANSPARENT;
	}else if (strMode.CompareNoCase(_T("opaque")) == 0){
		mode = OPAQUE;
	}else if (strMode.CompareNoCase(_T("bkmode_last")) == 0){
		mode = OPAQUE;
	}
	if (m_uBkMode == mode)
		return;

	if (mode != 0)
		m_uBkMode = mode;

	m_bFontChanged = TRUE;
}

LONG CSkinTextStyle::GetfontSize(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: 在此添加调度处理程序代码
	return m_iFontSize;
}

void CSkinTextStyle::SetfontSize(LONG newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: 在此添加调度处理程序代码
	if (newVal < 0)
		return;
	if (m_iFontSize == newVal)
		return;

	m_iFontSize = newVal;
	m_bFontChanged = TRUE;
}

LONG CSkinTextStyle::GetfontWeight(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: 在此添加调度处理程序代码
	return m_lfFont.lfWeight;
}

void CSkinTextStyle::SetfontWeight(LONG newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: 在此添加调度处理程序代码
	if (m_lfFont.lfWeight == newVal)
		return;

	m_lfFont.lfWeight = newVal;
	m_bFontChanged = TRUE;
}

VARIANT_BOOL CSkinTextStyle::Getitalic(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: 在此添加调度处理程序代码
	return m_lfFont.lfItalic == TRUE ? (VARIANT_TRUE) : (VARIANT_FALSE);
}

void CSkinTextStyle::Setitalic(VARIANT_BOOL newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: 在此添加属性处理程序代码
	BYTE bNewVal = (newVal != 0);
	if (m_lfFont.lfItalic == bNewVal)
		return;

	m_lfFont.lfItalic = bNewVal;
	m_bFontChanged = TRUE;
}

VARIANT_BOOL CSkinTextStyle::Getunderline(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: 在此添加调度处理程序代码
	return m_bUnderline == TRUE ? (VARIANT_TRUE) : (VARIANT_FALSE);
	//return m_lfFont.lfUnderline == TRUE ? (VARIANT_TRUE) : (VARIANT_FALSE);
}

void CSkinTextStyle::Setunderline(VARIANT_BOOL newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: 在此添加属性处理程序代码
	BYTE bNewVal = (newVal != 0);
	if (m_bUnderline == bNewVal)
		return;

	m_bUnderline = bNewVal;
	m_bFontChanged = TRUE;
	//BYTE bNewVal = (newVal != 0);
	//if (m_lfFont.lfUnderline == bNewVal)
	//	return;

	//m_lfFont.lfUnderline = bNewVal;
	//m_bFontChanged = TRUE;
}

VARIANT_BOOL CSkinTextStyle::GetstrikeOut(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: 在此添加调度处理程序代码
	return m_lfFont.lfStrikeOut == TRUE ? (VARIANT_TRUE) : (VARIANT_FALSE);
}

void CSkinTextStyle::SetstrikeOut(VARIANT_BOOL newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: 在此添加属性处理程序代码
	BYTE bNewVal = (newVal != 0);
	if (m_lfFont.lfStrikeOut == bNewVal)
		return;

	m_lfFont.lfStrikeOut = bNewVal;
	m_bFontChanged = TRUE;
}

BSTR CSkinTextStyle::GetfontFamily(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: 在此添加调度处理程序代码
	CString strResult(m_lfFont.lfFaceName, LF_FACESIZE-1);
	return strResult.AllocSysString();
}

void CSkinTextStyle::SetfontFamily(LPCTSTR newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: 在此添加调度处理程序代码
	CString strOldFamily(m_lfFont.lfFaceName, LF_FACESIZE-1);
	CString strNewFamily = newVal;
	if (strOldFamily == strNewFamily)
		return;
	m_lfFont.lfFaceName[LF_FACESIZE-1] = 0;
	_macro_tcsncpy(m_lfFont.lfFaceName, LF_FACESIZE-1, strNewFamily, LF_FACESIZE-1);	
	m_bFontChanged = TRUE;
}

//阴影
VARIANT_BOOL CSkinTextStyle::Getshadow(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: 在此添加调度处理程序代码
	return m_bShadow == TRUE ? (VARIANT_TRUE) : (VARIANT_FALSE);
}

void CSkinTextStyle::Setshadow(VARIANT_BOOL newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: 在此添加属性处理程序代码
	BYTE bNewVal = (newVal != 0);
	if (m_bShadow == bNewVal)
		return;

	m_bShadow = bNewVal;
	m_bFontChanged = TRUE;
}

VARIANT_BOOL CSkinTextStyle::GetwordBreak(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: 在此添加调度处理程序代码
	return (DT_WORDBREAK & m_uFormat) ? (VARIANT_TRUE) : (VARIANT_FALSE);
}

void CSkinTextStyle::SetwordBreak(VARIANT_BOOL newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: 在此添加属性处理程序代码
	newVal = (newVal != 0);
	if (newVal){
		m_uFormat |= DT_WORDBREAK;
	}else{
		m_uFormat &= ~DT_WORDBREAK;
	}
	m_bFormatChanged = TRUE;
}

VARIANT_BOOL CSkinTextStyle::GetendEllipsis(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: 在此添加调度处理程序代码
	return (DT_END_ELLIPSIS & m_uFormat) ? (VARIANT_TRUE) : (VARIANT_FALSE);
}

void CSkinTextStyle::SetendEllipsis(VARIANT_BOOL newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: 在此添加属性处理程序代码
	newVal = (newVal != 0);
	if (newVal){
		m_uFormat |= DT_END_ELLIPSIS;
	}else{
		m_uFormat &= ~DT_END_ELLIPSIS;
	}
	m_bFormatChanged = TRUE;
}

VARIANT_BOOL CSkinTextStyle::GetnoPrefix(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: 在此添加调度处理程序代码
	return (DT_NOPREFIX & m_uFormat) ? (VARIANT_TRUE) : (VARIANT_FALSE);
}

void CSkinTextStyle::SetnoPrefix(VARIANT_BOOL newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: 在此添加属性处理程序代码
	newVal = (newVal != 0);
	if (newVal){
		m_uFormat |= DT_NOPREFIX;
	}else{
		m_uFormat &= ~DT_NOPREFIX;
	}
	m_bFormatChanged = TRUE;
}

VARIANT_BOOL CSkinTextStyle::GetsingleLine(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: 在此添加调度处理程序代码
	return (DT_SINGLELINE & m_uFormat) ? (VARIANT_TRUE) : (VARIANT_FALSE);
}

void CSkinTextStyle::SetsingleLine(VARIANT_BOOL newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: 在此添加属性处理程序代码
	newVal = (newVal != 0);
	if (newVal){
		m_uFormat |= DT_SINGLELINE;
	}else{
		m_uFormat &= ~DT_SINGLELINE;
	}
	m_bFormatChanged = TRUE;
}

BSTR CSkinTextStyle::GethorizAlign(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: 在此添加调度处理程序代码
	CString strResult = _T("left");
	if (DT_CENTER & m_uFormat){
		strResult = _T("center");
	}else if (DT_RIGHT & m_uFormat){
		strResult = _T("right");
	}
	return strResult.AllocSysString();
}

void CSkinTextStyle::SethorizAlign(LPCTSTR newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: 在此添加属性处理程序代码
	CString align = newVal;
	align.MakeLower();
	if (align.Compare(_T("left")) == 0){
		m_uFormat &= ~DT_CENTER;
		m_uFormat &= ~DT_RIGHT;
	}else if (align.Compare(_T("center")) == 0){
		m_uFormat |= DT_CENTER;
		m_uFormat &= ~DT_RIGHT;
	}else if (align.Compare(_T("right")) == 0){
		m_uFormat |= DT_RIGHT;
		m_uFormat &= ~DT_CENTER;
	}
	m_bFormatChanged = TRUE;
}

BSTR CSkinTextStyle::GetvertiAlign(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: 在此添加调度处理程序代码
	CString strResult = _T("top");
	if (DT_VCENTER & m_uFormat){
		strResult = _T("center");
	}else if (DT_BOTTOM & m_uFormat){
		strResult = _T("bottom");
	}
	return strResult.AllocSysString();
}

void CSkinTextStyle::SetvertiAlign(LPCTSTR newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: 在此添加属性处理程序代码
	CString align = newVal;
	align.MakeLower();
	if (align.Compare(_T("top")) == 0){
		m_uFormat &= ~DT_VCENTER;
		m_uFormat &= ~DT_BOTTOM;
	}else if (align.Compare(_T("center")) == 0){
		m_uFormat |= DT_VCENTER;
		m_uFormat &= ~DT_BOTTOM;
	}else if (align.Compare(_T("bottom")) == 0){
		m_uFormat |= DT_BOTTOM;
		m_uFormat &= ~DT_VCENTER;
	}
	m_bFormatChanged = TRUE;
}

//第一行与上边间距
LONG CSkinTextStyle::GetbeginLineMargin(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	return m_nBeginLineMargin;
}

void CSkinTextStyle::SetbeginLineMargin(LONG newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: 在此添加属性处理程序代码
	if (newVal == m_nBeginLineMargin)
		return;
	m_nBeginLineMargin = newVal;
	m_bFormatChanged = TRUE;
}

//多行的行间距
LONG CSkinTextStyle::GetlineMargin(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	return m_nLineMargin;
}

void CSkinTextStyle::SetlineMargin(LONG newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: 在此添加属性处理程序代码
	if (newVal == m_nLineMargin)
		return;
	m_nLineMargin = newVal;
	m_bFormatChanged = TRUE;
}

//最后一行与下边间距
LONG CSkinTextStyle::GetendLineMargin(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	return m_nEndLineMargin;
}

void CSkinTextStyle::SetendLineMargin(LONG newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: 在此添加属性处理程序代码
	if (newVal == m_nEndLineMargin)
		return;
	m_nEndLineMargin = newVal;
	m_bFormatChanged = TRUE;
}

//下划线间距
LONG CSkinTextStyle::GetunderlineMargin(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	return m_nUnderlineMargin;
}

void CSkinTextStyle::SetunderlineMargin(LONG newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: 在此添加属性处理程序代码
	if (newVal == m_nUnderlineMargin)
		return;
	m_nUnderlineMargin = newVal;
	m_bFormatChanged = TRUE;
}

//阴影
VARIANT_BOOL CSkinTextStyle::GetignoreCRLF(void)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());

    // TODO: 在此添加调度处理程序代码
    return m_bIgnoreCRLF == TRUE ? (VARIANT_TRUE) : (VARIANT_FALSE);
}

void CSkinTextStyle::SetignoreCRLF(VARIANT_BOOL newVal)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());

    // TODO: 在此添加属性处理程序代码
    BYTE bNewVal = (newVal != 0);
    if (m_bIgnoreCRLF == bNewVal)
        return;

    m_bIgnoreCRLF = bNewVal;
    m_bFontChanged = TRUE;
}
