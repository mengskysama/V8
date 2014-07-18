#include "StdAfx.h"
#include "SkinLayoutItem.h"
#include "SkinBase.h"

CSkinLayoutItem::CSkinLayoutItem(CSkinBase* pSkin)
:
m_pSkin(pSkin)
{
}

CSkinLayoutItem::~CSkinLayoutItem(void)
{
}

tagSIZE CSkinLayoutItem::SizeHint()
{
    ASSERT(m_pSkin != NULL);
    return m_pSkin->GetSkinRect().Size();
}

tagSIZE CSkinLayoutItem::MinSize()
{
    return CSize(0, 0);
}

tagSIZE CSkinLayoutItem::MaxSize()
{
    return CSize(-1, -1);
}

RECT CSkinLayoutItem::GetRect()
{
    ASSERT(m_pSkin != NULL);
    CRect rcOrigin = m_pSkin->GetSkinRect();
    CSize szOffset = GetRootOffset();
    rcOrigin.OffsetRect(-szOffset.cx, -szOffset.cy);
    return rcOrigin;
}

void CSkinLayoutItem::SetRect(const RECT& rcRect)
{
    ASSERT(m_pSkin != NULL);
    CRect skinRect = rcRect;
    CSize szOffset = GetRootOffset();
    skinRect.OffsetRect(szOffset.cx, szOffset.cy);
    m_pSkin->MoveWindow(&skinRect, TRUE);
}
