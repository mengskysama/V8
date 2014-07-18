#pragma once

#include "../Common.h"
#include "IUITreeNode.h"

namespace common { namespace ui
{

/**
@class IUIListTreeNode
@brief list型节点的实现
*/
class COMMON_INTERFACE IUIListTreeNode :
    public IUITreeNode
{
public:
    /**
    @fn IUIListTreeNode(IUITreeContainer* pContainer)
    @brief 构造函数
    @param [in] pContainer 节点容器
    */
    IUIListTreeNode(IUITreeContainer* pContainer);

    /**
    @fn ~IUIListTreeNode(void)
    @brief 析构函数
    */
    virtual ~IUIListTreeNode(void);

public:
    /**
    @fn LONG GetTitleHeight()
    @brief 获得标题高度
    @return 返回标题高度
    */
    LONG GetTitleHeight();

    /**
    @fn void SetTitleHeight(LONG height)
    @brief 设置标题高度
    */
    void SetTitleHeight(LONG height);

    /**
    @fn void SetRect(const RECT& rect)
    @brief 设置矩形
    @param [in] rect 
    */
    virtual void SetRect(const RECT& rect);

    /**
    @fn void OnChildNotify(IUITreeNode* pChild, UINT uCode, LPTNN lpTNN)
    @brief 子节点改变通知
    @param [in] pChild 子节点
    @param [in] uCode 通知码
    @param [in] lpTNN 通知数据结构指针
    */
    virtual void OnChildNotify(IUITreeNode* pChild, UINT uCode, LPTNN lpTNN);

    /**
    @fn bool AddListChild(IUITreeNode* pChild, int iPos, const tagSIZE& szChild, LONG xOffset = 0)
    @brief 增加子节点
    @param [in] pChild 要增加的子节点
    @param [in] iPos 位置
    @param [in] szChild 节点大小
    @param [in] xOffset 偏移
    @return 是否添加成功
    */
    virtual bool AddListChild(IUITreeNode* pChild, int iPos, const tagSIZE& szChild, LONG xOffset = 0);

    /**
    @fn bool UpdateListChild(IUITreeNode* pChild, const tagSIZE& szChlid, LONG xOffset = 0)
    @brief 更新子节点
    @param [in] pChild 要更新的子节点
    @param [in] szChild 节点大小
    @param [in] xOffset 偏移
    @return 是否更新成功
    */
    virtual bool UpdateListChild(IUITreeNode* pChild, const tagSIZE& szChlid, LONG xOffset = 0);

    /**
    @fn bool RemoveListChild(IUITreeNode* pChild)
    @brief 删除子节点
    @param [in] pChild 要更新的子节点
    @return 是否删除成功
    */
    virtual bool RemoveListChild(IUITreeNode* pChild);

private:
    LONG m_lTitleHeight;
};

} } //namespace common::ui
