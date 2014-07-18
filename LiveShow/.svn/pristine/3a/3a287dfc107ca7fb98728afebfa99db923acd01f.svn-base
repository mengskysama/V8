#include "../../StdAfx.h"
#include "../../include/ui/IUITreeContainer.h"
#include "../../include/ui/IUITreeNode.h"
#include "../../thirdlibrary/CxImage/include/ximage.h"

#include <algorithm>
#include <functional>

using namespace std;

namespace common { namespace ui
{

IUITreeNode::IUITreeNode(IUITreeContainer* pContainer)
:
m_pContainer(pContainer),
m_pParent(NULL),
m_rcRect(0, 0, 0, 0),
m_uStatus(TNS_VISIBLE | TNS_EXPANDED),
m_iData(0)
{
}

IUITreeNode::~IUITreeNode(void)
{
    RemoveAll();
}

void IUITreeNode::Destroy()
{
    if (m_pParent != NULL) {
        m_pParent->RemoveChild(this);
        return;
    }
    delete this;
}

//绘制函数
void IUITreeNode::Draw(CDC* pDC, const RECT& rcBound)
{
    vector<IUITreeNode*>::iterator iter = m_vecChildNodes.begin(),
        last = m_vecChildNodes.end();
    for (; iter!=last; iter++) {

        IUITreeNode* pNode = (*iter);
        if (! pNode->IsVisible())
            continue;

        CRect rect = pNode->GetRect();

        //失效区域和节点区域求交
        CRect interRect;
        interRect.IntersectRect(&rcBound, &rect);
        if (interRect.IsRectEmpty())
            continue;

        //偏移DC
        COffsetDC offsetDC(*pDC, rect.TopLeft());
        pNode->ParentToChild(interRect);
        pNode->Draw(pDC, interRect);
    }
}

void IUITreeNode::ParentToChild(POINT& pt)
{
    pt.x -= m_rcRect.left;
    pt.y -= m_rcRect.top;
}

void IUITreeNode::ParentToChild(RECT& rect)
{
    ::OffsetRect(&rect, -m_rcRect.left, -m_rcRect.top);
}

void IUITreeNode::ChildToParent(POINT& pt)
{
    pt.x += m_rcRect.left;
    pt.y += m_rcRect.top;
}

void IUITreeNode::ChildToParent(RECT& rect)
{
    ::OffsetRect(&rect, m_rcRect.left, m_rcRect.top);
}

void IUITreeNode::AncestorToChild(POINT& pt)
{
    IUITreeNode* pParent = this;
    while (pParent != NULL) {
        pParent->ParentToChild(pt);
        pParent = pParent->GetParent();
    }
}

void IUITreeNode::AncestorToChild(RECT& rect)
{
    IUITreeNode* pParent = this;
    while (pParent != NULL) {
        pParent->ParentToChild(rect);
        pParent = pParent->GetParent();
    }
}

void IUITreeNode::ChildToAncestor(POINT& pt)
{
    IUITreeNode* pParent = this;
    while (pParent != NULL) {
        pParent->ChildToParent(pt);
        pParent = pParent->GetParent();
    }
}

void IUITreeNode::ChildToAncestor(RECT& rect)
{
    IUITreeNode* pParent = this;
    while (pParent != NULL) {
        pParent->ChildToParent(rect);
        pParent = pParent->GetParent();
    }
}

//获得节点位置
RECT IUITreeNode::GetRect()
{
    return m_rcRect;
}

//设置节点大小
void IUITreeNode::SetRect(const RECT& rect)
{
    m_rcRect = rect;
}

void IUITreeNode::OnChildNotify(IUITreeNode* pChild, UINT uCode, LPTNN lpTNN)
{
    int pos = GetChildPos(pChild);
    ASSERT(pos >= 0);
    if (pos < 0)
        return;

    return;
}

//计算节点所需的大小
void IUITreeNode::CalcSize(tagSIZE& size)
{
    vector<IUITreeNode*>::iterator iter = m_vecChildNodes.begin(),
        last = m_vecChildNodes.end();
    CSize totalSize(0, 0);
    for (; iter!=last; iter++) {
        CSize sz;
        (*iter)->CalcSize(sz);
        if (sz.cx > totalSize.cx)
            totalSize.cx = sz.cx;
        totalSize.cy += sz.cy;
    }
    size = totalSize;
}

//添加子节点
void IUITreeNode::AddChild(IUITreeNode* pChild, int pos)
{
    ASSERT(pChild->GetContainer() == m_pContainer && pChild->GetParent() == NULL);
    if (pChild->GetContainer() != m_pContainer || pChild->GetParent() != NULL)
        return;

    ASSERT(pos >= -1 && pos < (int)m_vecChildNodes.size());
    //if (pos < -1 || pos >= (int)m_vecChildNodes.size()) {
    //    return;
    //}

    vector<IUITreeNode*>::iterator finded = ::find(m_vecChildNodes.begin(), m_vecChildNodes.end(), pChild);

    ASSERT(finded == m_vecChildNodes.end());
    if (finded != m_vecChildNodes.end())
        return;

    pChild->m_pParent = this;

    IUITreeContainer* pContainer = GetContainer();
    if (pos >= 0) {
        if (pContainer != NULL) {
            pContainer->Add(pChild);
        }
        m_vecChildNodes.insert(m_vecChildNodes.begin() + pos, pChild);
        return;
    }
    if (pContainer != NULL) {
        pContainer->Add(pChild);
    }
    m_vecChildNodes.insert(m_vecChildNodes.end(), pChild);
}

//删除子节点
void IUITreeNode::RemoveChild(IUITreeNode* pChild)
{
    vector<IUITreeNode*>::iterator finded = ::find(m_vecChildNodes.begin(), m_vecChildNodes.end(), pChild);

    ASSERT(finded != m_vecChildNodes.end());
    if (finded == m_vecChildNodes.end())
        return;

    IUITreeContainer* pContainer = GetContainer();
    if (pContainer != NULL) {
        pContainer->Remove(*finded);
    }
    delete (*finded);
    m_vecChildNodes.erase(finded);
}

//删除所有子节点
void IUITreeNode::RemoveAll()
{
    vector<IUITreeNode*>::iterator iter = m_vecChildNodes.begin(),
        last = m_vecChildNodes.end();
    IUITreeContainer* pContainer = GetContainer();
    for (; iter!=last; iter++) {
        if (pContainer != NULL) {
            pContainer->Remove(*iter);
        }
        delete (*iter);
    }
    m_vecChildNodes.clear();
}

//是否有子节点
bool IUITreeNode::HasChild()
{
    return (m_vecChildNodes.size() > 0);
}

//获得所在容器
IUITreeContainer* IUITreeNode::GetContainer()
{
    return m_pContainer;
}

//获得父节点
IUITreeNode* IUITreeNode::GetParent()
{
    return m_pParent;
}

//获得子节点数目
int IUITreeNode::GetChildNum()
{
    return (int)m_vecChildNodes.size();
}

//获得位置iIndex的子节点
IUITreeNode* IUITreeNode::GetChild(int iIndex)
{
    ASSERT(iIndex >= 0 && iIndex < (int)m_vecChildNodes.size());
    if (iIndex < 0 || iIndex >= (int)m_vecChildNodes.size())
        return NULL;

    return m_vecChildNodes[iIndex];
}

//获得点pt所在的子节点
IUITreeNode* IUITreeNode::GetChild(POINT pt, UINT uFilterMask, UINT uFilterValue, LPINT pPos)
{
    vector<IUITreeNode*>::iterator iter = m_vecChildNodes.begin(),
        last = m_vecChildNodes.end();
    for (int count=0; iter!=last; iter++, count++) {
        IUITreeNode* pChildNode = (*iter);
        CRect rect = pChildNode->GetRect();
        if (rect.PtInRect(pt) && pChildNode->IsFiltered(uFilterMask, uFilterValue)) {
            (pPos != NULL) ? ((*pPos) = count) : (-1);
            return pChildNode;
        }
    }
    (pPos != NULL) ? (*pPos = -1) : (-1);
    return NULL;
}

//获得点pt所在的子孙节点
IUITreeNode* IUITreeNode::GetDescendant(POINT pt, UINT uFilterMask, UINT uFilterValue)
{
    vector<IUITreeNode*>::iterator iter = m_vecChildNodes.begin(),
        last = m_vecChildNodes.end();
    for (; iter!=last; iter++) {
        IUITreeNode* pChildNode = (*iter);
        CRect rect = pChildNode->GetRect();
        if (rect.PtInRect(pt) && pChildNode->IsFiltered(uFilterMask, uFilterValue)) {
            pChildNode->ParentToChild(pt);
            IUITreeNode* pGrandChildNode = pChildNode->GetDescendant(pt);
            return (pGrandChildNode != NULL && pGrandChildNode->IsFiltered(uFilterMask, uFilterValue))
                ? (pGrandChildNode) : pChildNode;
        }
    }
    return NULL;
}

int IUITreeNode::GetChildPos(IUITreeNode* pChild)
{
    vector<IUITreeNode*>::iterator finded = ::find(m_vecChildNodes.begin(), m_vecChildNodes.end(), pChild);
    if (finded == m_vecChildNodes.end())
        return -1;

    ASSERT( (finded - m_vecChildNodes.begin()) < INT_MAX );
    return (int)(finded - m_vecChildNodes.begin());
}

//子节点判断
bool IUITreeNode::IsChildOf(IUITreeNode* pParent)
{
    return (GetParent() == pParent);
}

//子孙节点判断
bool IUITreeNode::IsDescendantOf(IUITreeNode* pAncestor)
{
    IUITreeNode* pNext = GetParent();
    while (pNext != NULL) {
        if (pNext == pAncestor)
            return true;
        pNext = pNext->GetParent();
    }
    return false;
}

HCURSOR IUITreeNode::GetCursor(POINT pt)
{
    return NULL;
}

bool IUITreeNode::IsFiltered(UINT uMask, UINT uValue)
{
    return (m_uStatus & uMask) == (uValue & uMask);
}

//子节点访问
void IUITreeNode::VisitChildren(IUITreeNodeVisitor* pVisitor)
{
    vector<IUITreeNode*>::iterator iter = m_vecChildNodes.begin(),
        last = m_vecChildNodes.end();
    for (; iter!=last; iter++) {
        IUITreeNode* pChildNode = (*iter);
        pVisitor->Visit(pChildNode);
    }
}

//节点树访问
void IUITreeNode::VisitTree(IUITreeNodeVisitor* pVisitor)
{
    pVisitor->Visit(this);
    vector<IUITreeNode*>::iterator iter = m_vecChildNodes.begin(),
        last = m_vecChildNodes.end();
    for (; iter!=last; iter++) {
        IUITreeNode* pChildNode = (*iter);
        pChildNode->VisitTree(pVisitor);
    }
}

INT_PTR IUITreeNode::GetData()
{
    return m_iData;
}

void IUITreeNode::SetData(INT_PTR iData)
{
    m_iData = iData;
}

struct IUITreeNodeComparaFunctional
    : std::binary_function<IUITreeNode*, IUITreeNode*, bool>
{
public:
    IUITreeNodeComparaFunctional(IUITreeNodeComparator* pComp)
        :pComparator(pComp)
    {};

public:
    bool operator()(IUITreeNode* pLeft, IUITreeNode* pRight)
    {
        return pComparator->Compare(pLeft, pRight);
    };

private:
    IUITreeNodeComparator* pComparator;
};

void IUITreeNode::Sort(IUITreeNodeComparator* pComparator)
{
    vector<IUITreeNode*>::size_type vecSize = m_vecChildNodes.size();
    vector<IUITreeNode*>::size_type i = 0;

    //记录排序前子节点的坐标信息
    vector<CRect> vecRects;
    vecRects.reserve(vecSize);
    for (i=0; i<vecSize; i++) {
        IUITreeNode* pNode = m_vecChildNodes[i];
        vecRects.push_back(pNode->GetRect());
    }

    //排序
    IUITreeNodeComparaFunctional comparator(pComparator);
    std::sort(m_vecChildNodes.begin(), m_vecChildNodes.end(), comparator);

    //设置排序后的坐标信息，此处实现是，每个位置上节点的区域保持不变
    for (i=0; i<vecSize; i++) {
        IUITreeNode* pNode = m_vecChildNodes[i];
        pNode->SetRect(vecRects[i]);
    }
}

//消息响应
void IUITreeNode::OnMouseMove(POINT pt, UINT uFlag)
{
    IUITreeNode* pChild = GetChild(pt, TNS_VISIBLE, TNS_VISIBLE);
    if (pChild != NULL) {
        pChild->ParentToChild(pt);
        pChild->OnMouseMove(pt, uFlag);
    }
}

void IUITreeNode::OnLButtonDown(POINT pt, UINT uFlag)
{
    IUITreeNode* pChild = GetChild(pt, TNS_VISIBLE, TNS_VISIBLE);
    if (pChild != NULL) {
        pChild->ParentToChild(pt);
        pChild->OnLButtonDown(pt, uFlag);
    }
}

void IUITreeNode::OnLButtonUp(POINT pt, UINT uFlag)
{
    IUITreeNode* pChild = GetChild(pt, TNS_VISIBLE, TNS_VISIBLE);
    if (pChild != NULL) {
        pChild->ParentToChild(pt);
        pChild->OnLButtonUp(pt, uFlag);
    }
}

void IUITreeNode::OnLButtonDblClk(POINT pt, UINT uFlag)
{
    IUITreeNode* pChild = GetChild(pt, TNS_VISIBLE, TNS_VISIBLE);
    if (pChild != NULL) {
        pChild->ParentToChild(pt);
        pChild->OnLButtonDblClk(pt, uFlag);
    }
}

void IUITreeNode::OnRButtonDown(POINT pt, UINT uFlag)
{
    IUITreeNode* pChild = GetChild(pt, TNS_VISIBLE, TNS_VISIBLE);
    if (pChild != NULL) {
        pChild->ParentToChild(pt);
        pChild->OnRButtonDown(pt, uFlag);
    }
}

void IUITreeNode::OnRButtonUp(POINT pt, UINT uFlag)
{
    IUITreeNode* pChild = GetChild(pt, TNS_VISIBLE, TNS_VISIBLE);
    if (pChild != NULL) {
        pChild->ParentToChild(pt);
        pChild->OnRButtonUp(pt, uFlag);
    }
}

void IUITreeNode::OnRButtonDblClk(POINT pt, UINT uFlag)
{
    IUITreeNode* pChild = GetChild(pt, TNS_VISIBLE, TNS_VISIBLE);
    if (pChild != NULL) {
        pChild->ParentToChild(pt);
        pChild->OnRButtonDblClk(pt, uFlag);
    }
}

void IUITreeNode::OnMouseEnter(POINT pt, UINT uFlag)
{
    IUITreeNode* pChild = GetChild(pt, TNS_VISIBLE, TNS_VISIBLE);
    if (pChild != NULL) {
        pChild->ParentToChild(pt);
        pChild->OnMouseEnter(pt, uFlag);
    } else {
        SetHot(true);
    }
}

void IUITreeNode::OnMouseHover(POINT pt, UINT uFlag)
{
    IUITreeNode* pChild = GetChild(pt, TNS_VISIBLE, TNS_VISIBLE);
    if (pChild != NULL) {
        pChild->ParentToChild(pt);
        pChild->OnMouseHover(pt, uFlag);
    }
}

void IUITreeNode::OnMouseLeave(POINT pt, UINT uFlag)
{
    IUITreeNode* pChild = GetChild(pt, TNS_VISIBLE, TNS_VISIBLE);
    if (pChild != NULL) {
        pChild->ParentToChild(pt);
        pChild->OnMouseLeave(pt, uFlag);
    } else {
        SetHot(false);
    }
}

} } //namespace common::ui
