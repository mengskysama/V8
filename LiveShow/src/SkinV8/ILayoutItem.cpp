#include "StdAfx.h"
#include "ILayoutItem.h"
#include "ILayout.h"

ILayoutItem::ILayoutItem(void)
:
m_pLayout(NULL)
{
};

ILayoutItem::~ILayoutItem(void)
{
}

ILayout* ILayoutItem::GetLayout()
{
    return m_pLayout;
};

void ILayoutItem::SetLayout(ILayout* pLayout)
{
    m_pLayout = pLayout;
};

tagSIZE ILayoutItem::GetRootOffset()
{
    CSize szOffset(0, 0);
    ILayout* pLayout = GetLayout();
    while (pLayout != NULL) {
        CRect rc = pLayout->GetRect();
        szOffset.cx += rc.left;
        szOffset.cy += rc.top;
    }

    return szOffset;
}
