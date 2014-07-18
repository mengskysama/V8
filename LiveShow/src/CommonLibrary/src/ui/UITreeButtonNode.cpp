#include "StdAfx.h"
#include "../../include/ui/UITreeButtonNode.h"
#include "../../include/ui/IUITreeContainer.h"

namespace common { namespace ui
{

#define TIMER_SHOW_TIPS_ID	1		//	定时器时间
#define TIMER_SHOW_DELAY	1000	//	必须停留1s之后才会显示TIPS

CUITreeButtonNode::CUITreeButtonNode(IUITreeContainer* pContainer)
:
IUITreeNode(pContainer),
m_iCursor(-1)
{
}

CUITreeButtonNode::~CUITreeButtonNode(void)
{
}

HCURSOR CUITreeButtonNode::GetCursor(POINT pt)
{
    IUITreeContainer* pContainer = GetContainer();
    HCURSOR hCursor = pContainer->GetCursor(m_iCursor);
    return hCursor;
}

void CUITreeButtonNode::OnLButtonDown(POINT pt, UINT uFlag)
{
    IUITreeContainer* pContainer = GetContainer();
    pContainer->SetCapture(this);
    SetPushed(true);

    pContainer->Redraw(this);
}

void CUITreeButtonNode::OnLButtonUp(POINT pt, UINT uFlag)
{
    IUITreeContainer* pContainer = GetContainer();
    IUITreeNode* pCapture = pContainer->GetCapture();
    pContainer->ReleaseCapture();
    SetPushed(false);

    pContainer->Redraw(this);

    CRect rect = GetRect();
    ParentToChild(rect);
    if (rect.PtInRect(pt) && (pCapture == this)) {
        TNN tnn = {rect, pt};
        GetParent()->OnChildNotify(this, TNN_CLICK, &tnn);
    }
}

void CUITreeButtonNode::OnMouseEnter(POINT pt, UINT uFlag)
{
    IUITreeNode::OnMouseEnter(pt, uFlag);

    IUITreeContainer* pContainer = GetContainer();
	if(pContainer && !m_strTip.IsEmpty())
		pContainer->ShowTip(this, m_strTip, _T(""));

    pContainer->Redraw(this);

    IUITreeNode* pParent = GetParent();
    if (pParent != NULL) {
        TNN tnn = {0, 0, 0, 0, pt};
        pParent->OnChildNotify(this, TNN_MOUSEENTER, &tnn);
    }
}

void CUITreeButtonNode::OnMouseLeave(POINT pt, UINT uFlag)
{
    IUITreeNode::OnMouseLeave(pt, uFlag);

	IUITreeContainer* pContainer = GetContainer();
	if(pContainer)
		pContainer->HideTip();

    pContainer->Redraw(this);

    IUITreeNode* pParent = GetParent();
    if (pParent != NULL) {
        TNN tnn = {0, 0, 0, 0, pt};
        pParent->OnChildNotify(this, TNN_MOUSELEAVE, &tnn);
    }
}

int CUITreeButtonNode::GetCursor()
{
    return m_iCursor;
}

void CUITreeButtonNode::SetCursor(int iCursor)
{
    m_iCursor = iCursor;
}

CString CUITreeButtonNode::GetTip()
{
    return m_strTip;
}

void CUITreeButtonNode::SetTip(LPCTSTR lpzTip)
{
    m_strTip = lpzTip;
}

} } //namespace common::ui
