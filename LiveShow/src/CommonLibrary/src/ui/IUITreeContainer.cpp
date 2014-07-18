#include "StdAfx.h"
#include "../../include/ui/IUITreeContainer.h"

using namespace std;

namespace common { namespace ui
{

#pragma warning (push)
#pragma warning (disable:4355)

IUITreeContainer::IUITreeContainer(void)
:
IUIListTreeNode(this),
m_pCapture(NULL),
m_pSelected(NULL),
m_pMouseIn(NULL)
{
}

#pragma warning (pop)

IUITreeContainer::~IUITreeContainer(void)
{
    RemoveAll();
}

void IUITreeContainer::Draw(CDC* pDC, const RECT& rcBound)
{
    IUITreeNode::Draw(pDC, rcBound);
}

void IUITreeContainer::SetRect(const RECT& rect)
{
    for (int i=0; i<GetChildNum(); i++) {
        IUITreeNode* pNode = GetChild(i);
        CRect nodeRect = pNode->GetRect();
        nodeRect.left = rect.left;
        nodeRect.right = rect.right;
        pNode->SetRect(nodeRect);
    }
    IUITreeNode::SetRect(rect);
}

void IUITreeContainer::CalcSize(tagSIZE& size)
{
    IUITreeNode::CalcSize(size);
}

//消息响应
void IUITreeContainer::OnMouseMove(POINT pt, UINT uFlag)
{
    if (m_pCapture != NULL) {
        CPoint point = pt;
        m_pCapture->AncestorToChild(point);
        m_pCapture->OnMouseMove(point, uFlag);
        m_pMouseIn = m_pCapture;
        return;
    }
    IUITreeNode* pNode = GetDescendant(pt, TNS_VISIBLE, TNS_VISIBLE);
    if (pNode != m_pMouseIn) {
        if (m_pMouseIn != NULL) {
            CPoint point = pt;
            m_pMouseIn->AncestorToChild(point);
            m_pMouseIn->OnMouseLeave(point, uFlag);
        }
		if (pNode != NULL) {
			CPoint point = pt;
			pNode->AncestorToChild(point);
			pNode->OnMouseEnter(point, uFlag);
		}
    }
    m_pMouseIn = pNode;
    IUITreeNode::OnMouseMove(pt, uFlag);
}

void IUITreeContainer::OnLButtonDown(POINT pt, UINT uFlag)
{
    if (m_pCapture != NULL) {
        CPoint point = pt;
        m_pCapture->AncestorToChild(point);
        m_pCapture->OnLButtonDown(point, uFlag);
        return;
    }
    IUITreeNode::OnLButtonDown(pt, uFlag);
}

void IUITreeContainer::OnLButtonUp(POINT pt, UINT uFlag)
{
    if (m_pCapture != NULL) {
        CPoint point = pt;
        m_pCapture->AncestorToChild(point);
        m_pCapture->OnLButtonUp(point, uFlag);
        return;
    }
    IUITreeNode::OnLButtonUp(pt, uFlag);
}

void IUITreeContainer::OnLButtonDblClk(POINT pt, UINT uFlag)
{
    if (m_pCapture != NULL) {
        CPoint point = pt;
        m_pCapture->AncestorToChild(point);
        m_pCapture->OnLButtonDblClk(point, uFlag);
        return;
    }
    IUITreeNode::OnLButtonDblClk(pt, uFlag);
}

void IUITreeContainer::OnRButtonDown(POINT pt, UINT uFlag)
{
    if (m_pCapture != NULL) {
        CPoint point = pt;
        m_pCapture->AncestorToChild(point);
        m_pCapture->OnRButtonDown(point, uFlag);
        return;
    }
    IUITreeNode::OnRButtonDown(pt, uFlag);
}

void IUITreeContainer::OnRButtonUp(POINT pt, UINT uFlag)
{
    if (m_pCapture != NULL) {
        CPoint point = pt;
        m_pCapture->AncestorToChild(point);
        m_pCapture->OnRButtonUp(point, uFlag);
        return;
    }
    IUITreeNode::OnRButtonUp(pt, uFlag);
}

void IUITreeContainer::OnRButtonDblClk(POINT pt, UINT uFlag)
{
    if (m_pCapture != NULL) {
        CPoint point = pt;
        m_pCapture->AncestorToChild(point);
        m_pCapture->OnRButtonDblClk(point, uFlag);
        return;
    }
    IUITreeNode::OnRButtonDblClk(pt, uFlag);
}

void IUITreeContainer::OnMouseEnter(POINT pt, UINT uFlag)
{
    if (m_pCapture != NULL) {
        CPoint point = pt;
        m_pCapture->AncestorToChild(point);
        m_pCapture->OnMouseEnter(point, uFlag);
        return;
    }
    IUITreeNode::OnMouseEnter(pt, uFlag);
}

void IUITreeContainer::OnMouseHover(POINT pt, UINT uFlag)
{
    if (m_pCapture != NULL) {
        CPoint point = pt;
        m_pCapture->AncestorToChild(point);
        m_pCapture->OnMouseHover(point, uFlag);
        return;
    }
    IUITreeNode::OnMouseHover(pt, uFlag);
}

void IUITreeContainer::OnMouseLeave(POINT pt, UINT uFlag)
{
    if (m_pCapture != NULL) {
        CPoint point = pt;
        m_pCapture->AncestorToChild(point);
        m_pCapture->OnMouseLeave(point, uFlag);
        return;
    }
	if (m_pMouseIn != NULL) {
		CPoint point = pt;
		m_pMouseIn->AncestorToChild(point);
		m_pMouseIn->OnMouseLeave(point, uFlag);
	}
	m_pMouseIn = NULL;
    IUITreeNode::OnMouseLeave(pt, uFlag);
}

void IUITreeContainer::Add(IUITreeNode* pNode)
{

}

void IUITreeContainer::Remove(IUITreeNode* pNode)
{
    if (m_pCapture == pNode)
        m_pCapture = NULL;

    if (m_pSelected == pNode)
        m_pSelected = NULL;

    if (m_pMouseIn == pNode)
        m_pMouseIn = NULL;
}

//选择节点
bool IUITreeContainer::Select(IUITreeNode* pNode)
{
    //设置原来节点和所有子节点为Unckeck状态
    CUITreeNodeCheckVisitor uncheckVisitor(false);
    if (m_pSelected != NULL) {
        m_pSelected->VisitTree(&uncheckVisitor);
        Redraw(m_pSelected);
    }

    //设置新节点的所有子节点为check状态
    if (pNode != NULL) {
        CUITreeNodeCheckVisitor checkVisitor(true);
        pNode->VisitTree(&checkVisitor);
        Redraw(pNode);
    }

    m_pSelected = pNode;
    return true;
}

vector<IUITreeNode*> IUITreeContainer::GetSelected()
{
    vector<IUITreeNode*> vecResult;

    if (m_pSelected != NULL)
        vecResult.push_back(m_pSelected);

    return vecResult;
}

bool IUITreeContainer::Expand(IUITreeNode* pNode)
{
    return false;
}

bool IUITreeContainer::Redraw(IUITreeNode* pNode)
{
    HWND hWnd = GetHWND();
    if (! ::IsWindow(hWnd))
        return false;

    if (pNode == NULL)
        return ::InvalidateRect(hWnd, NULL, FALSE) ? (true) : (false);

    //计算节点相对根节点的坐标
    CRect nodeRect = pNode->GetRect();
    pNode->ParentToChild(nodeRect);
    pNode->ChildToAncestor(nodeRect);

    //获得客户区和根节点的偏移，计算节点相对客户区的坐标
    CPoint offset;
    GetOffset(offset);
    nodeRect.OffsetRect(-offset.x, -offset.y);

    //获得窗口客户区坐标
    CRect clientRect;
    ::GetClientRect(hWnd, &clientRect);

    //客户区与节点区域求交
    CRect interRect;
    interRect.IntersectRect(&clientRect, nodeRect);

    //刷新
    if (! interRect.IsRectEmpty())
        return ::InvalidateRect(hWnd, &interRect, FALSE) ? (true) : (false);

    return false;
}

bool IUITreeContainer::EnsureVisible(IUITreeNode* pNode)
{
    return false;
}

//显示Tips
bool IUITreeContainer::ShowTip(IUITreeNode* pNode, LPCTSTR lpzText, LPCTSTR lpzTitle)
{
    return false;
}

void IUITreeContainer::HideTip()
{
	return;
}

HWND IUITreeContainer::GetHWND()
{
    return NULL;
}

COLORREF IUITreeContainer::GetColor(int iIndex)
{
    return RGB(255, 255, 255);
}

HFONT IUITreeContainer::GetFont(int iIndex)
{
    return NULL;
}

HICON IUITreeContainer::GetIcon(int iIndex)
{
    return NULL;
}

HCURSOR IUITreeContainer::GetCursor(int iIndex)
{
    return NULL;
}

HPEN IUITreeContainer::GetPen(int iIndex)
{
    return NULL;
}

HBRUSH IUITreeContainer::GetBrush(int iIndex)
{
    return NULL;
}

HBITMAP IUITreeContainer::GetBitmap(int iIndex)
{
    return NULL;
}

//向Container发送消息
LRESULT IUITreeContainer::SendMessage(UINT uMsg, IUITreeNode* pSender, WPARAM wParam, LPARAM lParam)
{
    return S_OK;
}

IUITreeNode* IUITreeContainer::GetCapture()
{
    HWND hWnd = GetHWND();
    if (hWnd == NULL)
        return NULL;

    if (::GetCapture() == hWnd)
        return m_pCapture;

    return NULL;
}

IUITreeNode* IUITreeContainer::SetCapture(IUITreeNode* pNode)
{
    HWND hWnd = GetHWND();
    if (hWnd == NULL)
        return NULL;

    IUITreeNode* pOldCapture = m_pCapture;
    if (::GetCapture() == hWnd) {
        m_pCapture = pNode;
        return pOldCapture;
    } else {
        ::SetCapture(hWnd);
        m_pCapture = pNode;
        return NULL;
    }
}

bool IUITreeContainer::ReleaseCapture()
{
    HWND hWnd = GetHWND();
    if (hWnd == NULL)
        return false;

    if (::GetCapture() == hWnd) {
        bool bRet = ::ReleaseCapture() ? (true) : (false);
        CPoint pt;
        ::GetCursorPos(&pt);
        ::ScreenToClient(hWnd, &pt);
        m_pCapture = NULL;
        ::PostMessage( hWnd, WM_MOUSEMOVE, 0, (pt.x | (pt.y << 16)) );
        return bRet;
    } else {
        m_pCapture = NULL;
        return false;
    }
}

} } //namespace common::ui
