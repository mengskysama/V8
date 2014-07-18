#pragma once

#include "../Common.h"
#include "uitreebuttonnode.h"

namespace common { namespace ui
{

class COMMON_INTERFACE CUITreeTextButton :
    public CUITreeButtonNode
{
public:
    CUITreeTextButton(IUITreeContainer* pContainer);
    virtual ~CUITreeTextButton(void);

    virtual void Draw(CDC* pDC, const RECT& rcBound);
    virtual void SetRect(const RECT& rect);
    virtual void CalcSize(tagSIZE& size);

public:
    CString GetText();
    void SetText(LPCTSTR lpzText);
    int GetFont();
    void SetFont(int iFont);
    int GetNormalColor();
    void SetNormalColor(int iColor);
    int GetHighlightColor();
    void SetHighlightColor(int iColor);
    bool GetNormalUnderline();
    void SetNormalUnderline(bool bTrue);
    bool GetHighlightUnderline();
    void SetHighlightUnderline(bool bTrue);
    int GetUnderlineMargin();
    void SetUnderlineMargin(int iMargin);

protected:
    CString m_strText;
    int m_iFont;
    int m_iClrNormal;
    int m_iClrHighlight;
    bool m_bNormalUnderline;
    bool m_bHighlightUnderline;
    int m_iUnderlineMargin;
};

} } //namespace common::ui
