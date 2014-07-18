#pragma once

#include "../Common.h"

#include <vector>

namespace common { namespace ui
{

class IUITreeNode;
class COMMON_INTERFACE IUITreeNodeVisitor
{
public:
    IUITreeNodeVisitor() {};
    virtual ~IUITreeNodeVisitor() {};
public:
    virtual void Visit(IUITreeNode* pNode) = 0;
};

class COMMON_INTERFACE IUITreeNodeComparator
{
public:
    virtual bool Compare(IUITreeNode* pLeft, IUITreeNode* pRight) = 0;
};

typedef bool (IUITreeNode::*pfnIUITreeNodeProperty)();

class COMMON_INTERFACE CUITreeNodePropertyVisitor :
    public IUITreeNodeVisitor
{
public:
    CUITreeNodePropertyVisitor(pfnIUITreeNodeProperty pfn, bool bCheck = true) {
        m_pfn = pfn;
        m_bCheck = bCheck;
        m_bResult = false;
    };
    ~CUITreeNodePropertyVisitor() {};

    virtual void Visit(IUITreeNode* pNode) {
        bool bCheck = (pNode->*m_pfn)();
        if (bCheck && m_bCheck)
            m_bResult = true;
        if (!bCheck && !m_bCheck)
            m_bResult = true;
    };

    bool GetResult() {
        return m_bResult;
    };

    operator bool() {
        return GetResult();
    };

protected:
    pfnIUITreeNodeProperty m_pfn;
    bool m_bCheck;
    bool m_bResult;
};

typedef struct tagTREENODENOTIFY
{
    RECT rc;
    POINT pt;
}TNN, *LPTNN;

//Notify Code
//位置改变
#define TNN_POSCHANGED      0
//鼠标消息
#define TNN_MOUSEMOVE       10
#define TNN_LBUTTONDOWN     11
#define TNN_LBUTTONUP       12
#define TNN_LBUTTONDBLCLK   13
#define TNN_MBUTTONDOWN     14
#define TNN_MBUTTONUP       15
#define TNN_MBUTTONDBLCLK   16
#define TNN_RBUTTONDOWN     17
#define TNN_RBUTTONUP       18
#define TNN_RBUTTONDBLCLK   19
#define TNN_MOUSEENTER      20
#define TNN_MOUSEHOVER      21
#define TNN_MOUSELEAVE      22
//点击
#define TNN_CLICK           30

class IUITreeContainer;
/**
@class IUITreeNode
@brief IUITreeNode 类定义
*/
class COMMON_INTERFACE IUITreeNode
{
public:
    /**
    @fn IUITreeNode(IUITreeContainer* pContainer)
    @brief 构造函数
    @param [in] pContainer 节点所属容器指针
    */
    IUITreeNode(IUITreeContainer* pContainer);

protected:
    /**
    @fn ~IUITreeNode(void)
    @brief 析构函数，设为protected防止栈构造
    */
    virtual ~IUITreeNode(void);

public:
    /**
    @fn void Destroy()
    @brief 显式析构函数
    */
    void Destroy();

public:
    //属性状态
    enum
    {
        TNS_VISIBLE  = 0x00000001,  //可见
        TNS_DISABLED = 0x00000002,  //不可用
        TNS_SELECTED = 0x00000004,  //被选中
        TNS_EXPANDED = 0x00000008,  //被展开
        TNS_CHECKED  = 0x00000010,  //check状态
        TNS_HOT      = 0x00000020,  //hot状态
        TNS_PUSHED   = 0x00000040,  //pushed状态
    };

//定义属性列表宏，减少重复代码编写
#define PROPERTY_LIST(V) \
    V(Visible,  TNS_VISIBLE) \
    V(Disabled, TNS_DISABLED) \
    V(Selected, TNS_SELECTED) \
    V(Expanded, TNS_EXPANDED) \
    V(Checked,  TNS_CHECKED) \
    V(Hot,      TNS_HOT) \
    V(Pushed,   TNS_PUSHED) \


public:
    //绘制函数，注意CDC的坐标已经偏移至本节点坐标系
    /**
    @fn void Draw(CDC* pDC, const RECT& rcBound)
    @brief 节点绘制函数
    @param [in] pDC DC指针
    @param [in] rcBound 无效区域
    */
    virtual void Draw(CDC* pDC, const RECT& rcBound);

    /** 
    @addtogroup 坐标转换
    @brief 坐标转换函数
    */ 
    //@{
    void ParentToChild(POINT& pt);
    void ParentToChild(RECT& rect);
    void ChildToParent(POINT& pt);
    void ChildToParent(RECT& rect);
    void AncestorToChild(POINT& pt);
    void AncestorToChild(RECT& rect);
    void ChildToAncestor(POINT& pt);
    void ChildToAncestor(RECT& rect);
    //@}

    //获得节点大小，为相对父节点坐标
    /**
    @fn RECT GetRect()
    @brief 获得节点的矩形
    @return 节点相对父节点的区域
    */
    RECT GetRect();

    //设置节点大小，为相对父节点坐标
    /**
    @fn void SetRect(const RECT& rect)
    @brief 设置节点区域
    @param [in] rect 区域
    */
    virtual void SetRect(const RECT& rect);

    /**
    @fn void CalcSize(tagSIZE& size)
    @brief 计算节点所需的大小，用于父节点安排子节点的位置，注意此大小并非最终的节点大小，父节点会根据自身的限制，对子节点大小做调整
    @param [out] size 计算的节点所需大小
    */
    virtual void CalcSize(tagSIZE& size);

    /**
    @fn void OnChildNotify(IUITreeNode* pChild, UINT uCode, LPTNN lpTNN)
    @brief 子节点通知
    @param [in] pChild 子节点指针
    @param [in] uCode 通知码
    @param [in] lpTNN 通知的数据结构
    */
    virtual void OnChildNotify(IUITreeNode* pChild, UINT uCode, LPTNN lpTNN);

    /**
    @fn AddChild(IUITreeNode* pChild, int pos = -1)
    @brief 添加子节点，添加到位置pos，-1为末尾
    @param [in] pChild 要添加的子节点
    @param [in] pos 添加的位置
    */
    void AddChild(IUITreeNode* pChild, int pos = -1);

    /**
    @fn void RemoveChild(IUITreeNode* pChild)
    @brief 删除子节点
    @param pChild 要删除的节点
    */
    void RemoveChild(IUITreeNode* pChild);

    /**
    @fn void RemoveAll()
    @brief 删除所有的子节点
    */
    void RemoveAll();

    /**
    @fn bool HasChild()
    @brief 是否有子节点
    @return 返回是否有子节点
    */
    bool HasChild();

    /**
    @fn IUITreeContainer* GetContainer()
    @brief 获得节点容器
    @return 返回节点容器
    */
    IUITreeContainer* GetContainer();

    /**
    @fn IUITreeNode* GetParent()
    @brief 获得父节点
    @return 返回父节点
    */
    IUITreeNode* GetParent();

    /**
    @fn int GetChildNum()
    @brief 获得子节点的数目
    @return 返回子节点数目
    */
    int GetChildNum();

    /**
    @fn IUITreeNode* GetChild(int iIndex)
    @brief 获得位置为iIndex的子节点
    @param [in] iIndex
    @return 返回子节点
    */
    IUITreeNode* GetChild(int iIndex);

    /**
    @fn IUITreeNode* GetChild(POINT pt, UINT uFilterMask = 0, UINT uFilterValue = 0, LPINT pPos = NULL)
    @brief 获得点pt所在的子节点，且满足特定的属性
    @param [in] pt 位置点
    @param [in] uFilterMask 需要满足属性的掩码
    @param [in] uFilterValue 需要满足属性的值
    @param [out] pPos 子节点的位置
    @return 返回子节点
    */
    IUITreeNode* GetChild(POINT pt, UINT uFilterMask = 0, UINT uFilterValue = 0, LPINT pPos = NULL);

    //获得点pt所在的子孙节点
    /**
    @fn IUITreeNode* GetDescendant(POINT pt, UINT uFilterMask = 0, UINT uFilterValue = 0)
    @brief 获得点pt所在的子孙节点，且满足特定的属性
    @param [in] pt 位置点
    @param [in] uFilterMask 需要满足属性的掩码
    @param [in] uFilterValue 需要满足属性的值
    @return 返回子节点
    */
    IUITreeNode* GetDescendant(POINT pt, UINT uFilterMask = 0, UINT uFilterValue = 0);

    /**
    @fn int GetChildPos(IUITreeNode* pChild)
    @brief 获得子节点位置
    @param [in] pChild 子节点的指针
    @return 返回子节点的位置
    */
    int GetChildPos(IUITreeNode* pChild);

    /**
    @fn bool IsChildOf(IUITreeNode* pParent)
    @brief 子节点关系判断
    @param [in] pParent 要判断的父节点
    @return 是否是pParent的子节点
    */
    bool IsChildOf(IUITreeNode* pParent);

    /**
    @fn bool IsDescendantOf(IUITreeNode* pAncestor)
    @brief 子孙节点判断
    @param [in] pAncestor 要判断的祖先节点
    @return 是否是pAncestor的子孙节点
    */
    bool IsDescendantOf(IUITreeNode* pAncestor);

    /**
    @fn HCURSOR GetCursor(POINT pt)
    @brief 获得鼠标光标
    @param [in] pt 鼠标位置
    @return 返回鼠标光标句柄
    */
    virtual HCURSOR GetCursor(POINT pt);

    //属性状态
#define IMPLEMENT_PROPERTY(name, mask) \
    inline bool Is##name() {return (m_uStatus & mask) != 0;}; \
    inline void Set##name(bool bTrue) { \
        bTrue ? (m_uStatus |= mask) : (m_uStatus &= ~(mask)); \
    };
PROPERTY_LIST(IMPLEMENT_PROPERTY)
#undef IMPLEMENT_PROPERTY
    bool IsFiltered(UINT uMask, UINT uValue);

#define PROPERTY_VISITOR(name, mask) \
class C##Property##name##Visitor : \
    public CUITreeNodePropertyVisitor \
{ \
public: \
    C##Property##name##Visitor(bool bCheck = true) \
    : CUITreeNodePropertyVisitor(&Is##name, bCheck) \
    { \
         \
    } \
};
PROPERTY_LIST(PROPERTY_VISITOR)
#undef PROPERTY_VISITOR

    /**
    @fn void VisitChildren(IUITreeNodeVisitor* pVisitor)
    @brief 子节点访问
    @param [in] pVisitor 访问者
    */
    void VisitChildren(IUITreeNodeVisitor* pVisitor);

    /**
    @fn void VisitTree(IUITreeNodeVisitor* pVisitor)
    @brief 节点树访问，包括本节点
    @param [in] pVisitor 访问者
    */
    void VisitTree(IUITreeNodeVisitor* pVisitor);

#define CHILDREN_PROPERTY(name, mask) \
    bool IsChildren##name(bool bTrue) { \
        C##Property##name##Visitor  visitor(bTrue); \
        VisitChildren(&visitor); \
        return visitor; \
    };
PROPERTY_LIST(CHILDREN_PROPERTY)
#undef CHILDREN_PROPERTY

#define TREE_PROPERTY(name, mask) \
    bool IsTree##name(bool bTrue) { \
    C##Property##name##Visitor  visitor(bTrue); \
    VisitTree(&visitor); \
    return visitor; \
};
PROPERTY_LIST(TREE_PROPERTY)
#undef TREE_PROPERTY

    /**
    @fn INT_PTR GetData()
    @brief 获得节点数据
    */
    INT_PTR GetData();

    /**
    @fn void SetData(INT_PTR iData)
    @brief 设置节点数据
    */
    void SetData(INT_PTR iData);

    /**
    @fn void Sort(IUITreeNodeComparator* pComparator)
    @brief 排序
    @param [in] pComparator 排序算子
    */
    virtual void Sort(IUITreeNodeComparator* pComparator);

    /** 
    @addtogroup 消息响应
    @brief  消息响应
    */ 
    //@{
    virtual void OnMouseMove(POINT pt, UINT uFlag);
    virtual void OnLButtonDown(POINT pt, UINT uFlag);
    virtual void OnLButtonUp(POINT pt, UINT uFlag);
    virtual void OnLButtonDblClk(POINT pt, UINT uFlag);
    virtual void OnRButtonDown(POINT pt, UINT uFlag);
    virtual void OnRButtonUp(POINT pt, UINT uFlag);
    virtual void OnRButtonDblClk(POINT pt, UINT uFlag);
    virtual void OnMouseEnter(POINT pt, UINT uFlag);
    virtual void OnMouseHover(POINT pt, UINT uFlag);
    virtual void OnMouseLeave(POINT pt, UINT uFlag);
    //@}

protected:
    //节点的容器
    IUITreeContainer* const m_pContainer;
    //父节点
    IUITreeNode* m_pParent;
    //子节点
    std::vector<IUITreeNode*> m_vecChildNodes;
    //节点的大小
    CRect m_rcRect;
    //属性状态
    UINT m_uStatus;
    //节点数据
    INT_PTR m_iData;
};

template<typename T>
class CSelectGdiHelper
{
public:
    CSelectGdiHelper(HDC hDC, T hT, BOOL bDelete = FALSE)
        :
    m_hDC(NULL),
    m_hNew(NULL),
    m_hOld(NULL),
    m_bDelete(FALSE)
    {
        m_hDC = hDC;
        m_hNew = hT;
        m_bDelete = bDelete;
        if (m_hNew)
            m_hOld = (T)::SelectObject(m_hDC, (HGDIOBJ)m_hNew);
    };
    ~CSelectGdiHelper() {
        Dispose();
    };
    void Dispose() {
        if (m_hDC == NULL)
            return;
        if (m_hOld)
            ::SelectObject(m_hDC, (HGDIOBJ)m_hOld);
        if (m_bDelete)
            ::DeleteObject((HGDIOBJ)m_hNew);
        m_hDC = NULL;
        m_hNew = NULL;
        m_hOld = NULL;
        m_bDelete = FALSE;
    };
protected:
    HDC m_hDC;
    T m_hNew;
    T m_hOld;
    BOOL m_bDelete;
};

class CSelectGdiBkModeHelper
{
public:
    CSelectGdiBkModeHelper(HDC hDC, UINT uMode)
        :
    m_hDC(NULL),
    m_uNew(0),
    m_uOld(0)
    {
        m_hDC = hDC;
        m_uOld = ::SetBkMode(m_hDC, uMode);
        m_uNew = uMode;
    };
    ~CSelectGdiBkModeHelper() {
        Dispose();
    };
    void Dispose() {
        if (m_hDC == NULL)
            return;
        ::SetBkMode(m_hDC, m_uOld);
        m_hDC = NULL;
        m_uOld = 0;
        m_uNew = 0;
    };
protected:
    HDC m_hDC;
    UINT m_uNew;
    UINT m_uOld;
};

class CSelectGdiTextColorHelper
{
public:
    CSelectGdiTextColorHelper(HDC hDC, LONG clrColor)
        :
    m_hDC(NULL),
    m_clrNew(0),
    m_clrOld(0)
    {
        m_hDC = hDC;
        m_clrOld = ::SetTextColor(m_hDC, clrColor);
        m_clrNew = clrColor;
    };
    ~CSelectGdiTextColorHelper() {
        Dispose();
    };
    void Dispose() {
        if (m_hDC == NULL)
            return;
        ::SetTextColor(m_hDC, m_clrOld);
        m_hDC = NULL;
        m_clrNew = 0;
        m_clrOld = 0;
    };
protected:
    HDC m_hDC;
    LONG m_clrNew;
    LONG m_clrOld;
};

class COffsetDC
{
public:
    COffsetDC(HDC hDC, const POINT& offset) {
        m_hDC = hDC;
        m_ptOffset = offset;
        ::OffsetViewportOrgEx(m_hDC, m_ptOffset.x, m_ptOffset.y, NULL);
    };
    ~COffsetDC() {
        Dispose();
    };
    void Dispose() {
        if (m_hDC == NULL)
            return;
        ::OffsetViewportOrgEx(m_hDC, -m_ptOffset.x, -m_ptOffset.y, NULL);
        m_hDC = NULL;
        m_ptOffset.x = 0;
        m_ptOffset.y = 0;
    };
protected:
    HDC m_hDC;
    POINT m_ptOffset;
};

} } //namespace common::ui
