#pragma once

#include "../Common.h"
#include "uitreebuttonnode.h"

namespace common { namespace ui
{

class COMMON_INTERFACE CUITreeIconButton :
    public CUITreeButtonNode
{
public:
    CUITreeIconButton(IUITreeContainer* pContainer);
    virtual ~CUITreeIconButton(void);

    virtual void Draw(CDC* pDC, const RECT& rcBound);
    virtual void SetRect(const RECT& rect);
    virtual void CalcSize(tagSIZE& size);

public:
    // Ù–‘…Ë÷√
    int GetIcon();
    void SetIcon(int iIcon);
    int GetHoverIcon();
    void SetHoverIcon(int iIcon);
    int GetClickIcon();
    void SetClickIcon(int iIcon);
	void SetBitmap(int iBmp);
	int GetBitmap(int iBmp);
	void SetCheckedBitmap(int iBmp);
	int GetCheckedBitmap(int iBmp);
    int GetCheckedIcon();
    void SetCheckedIcon(int iIcon);
	bool GetCheckable();
	void SetCheckable(bool bTrue);

protected:
    int m_iIcon;
    int m_iHoverIcon;
    int m_iClickIcon;
    int m_iCheckedIcon;
	int m_iBitmap;
	int m_iCheckedBitmap;
	bool m_bCheckable;
};

} } //namespace common::ui
