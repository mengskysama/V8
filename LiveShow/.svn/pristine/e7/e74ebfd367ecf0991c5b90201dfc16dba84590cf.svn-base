#pragma once

#include "../Common.h"
#include "IUIListTreeNode.h"

namespace common { namespace ui
{

class COMMON_INTERFACE CUITreeNodeCheckVisitor :
    public IUITreeNodeVisitor
{
public:
    CUITreeNodeCheckVisitor(bool bCheck = true) {
        m_bCheck = (bCheck != false);
    };
protected:
    virtual void Visit(IUITreeNode* pNode) {
        pNode->SetChecked(m_bCheck);
    };
protected:
    bool m_bCheck;
};

class COMMON_INTERFACE IUITreeContainer :
    public IUIListTreeNode
{
    friend class IUITreeNode;

public:
    /**
    @fn IUITreeContainer(void)
    @brief 构造函数
    */
    IUITreeContainer(void);

    /**
    @fn ~IUITreeContainer(void)
    @brief 析构函数
    */
    virtual ~IUITreeContainer(void);

    //////////////////////////////////////////////////////////////////////////
    //IUITreeNode
    //////////////////////////////////////////////////////////////////////////
    /** 
    @addtogroup IUITreeNode
    @brief IUITreeNode
    */ 
    //@{
    virtual void Draw(CDC* pDC, const RECT& rcBound);
    virtual void SetRect(const RECT& rect);
    virtual void CalcSize(tagSIZE& size);
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
    /**
    @fn void Add(IUITreeNode* pNode)
    @brief 添加新节点
    */
    virtual void Add(IUITreeNode* pNode);

    /**
    @fn void Remove(IUITreeNode* pNode)
    @brief 移除节点
    */
    virtual void Remove(IUITreeNode* pNode);

public:
    /**
    @fn bool Select(IUITreeNode* pNode)
    @brief 选择节点
    @param [in] pNode 要选择的节点
    @return 是否选择成功
    */
    virtual bool Select(IUITreeNode* pNode);

    /**
    @fn std::vector<IUITreeNode*> GetSelected()
    @brief 获得选择的节点
    */
    virtual std::vector<IUITreeNode*> GetSelected();

    /**
    @fn bool Expand(IUITreeNode* pNode)
    @brief 展开节点
    @param [in] pNode 要展开的节点
    @return 是否展开成功
    */
    virtual bool Expand(IUITreeNode* pNode);

    /**
    @fn bool Redraw(IUITreeNode* pNode = NULL)
    @brief 重绘节点
    @param [in] pNode 要重绘的节点，NULL全部重绘
    @return 是否重绘成功
    */
    virtual bool Redraw(IUITreeNode* pNode = NULL);

    /**
    @fn bool EnsureVisible(IUITreeNode* pNode)
    @brief 使节点可见
    @param [in] pNode 要可见的节点
    @return 是否成功
    */
    virtual bool EnsureVisible(IUITreeNode* pNode);

    /**
    @fn bool ShowTip(IUITreeNode* pNode, LPCTSTR lpzText, LPCTSTR lpzTitle)
    @brief 显式tip
    @param [in] pNode 要显示tip的节点
    @param [in] lpzText 要显示的tip内容
    @param [in] lpzTitle 要显示tip的标题
    @return 是否成功
    */
    virtual bool ShowTip(IUITreeNode* pNode, LPCTSTR lpzText, LPCTSTR lpzTitle);

    /**
    @fn void HideTip()
    @brief 隐藏tip
    */
	virtual void HideTip();

    /**
    @fn HWND GetHWND()
    @brief 获得窗口句柄
    @return 返回窗口句柄
    */
    virtual HWND GetHWND() = 0;

    /**
    @fn void GetOffset(POINT& offset)
    @brief 获得窗口客户区相对根节点的偏移
    @param [out] offset 偏移
    */
    virtual void GetOffset(POINT& offset) = 0;

    /** 
    @addtogroup GDI资源
    @brief GDI资源
    */ 
    //@{
    virtual COLORREF GetColor(int iIndex = 0);
    virtual HFONT GetFont(int iIndex = 0);
    virtual HICON GetIcon(int iIndex = 0);
    virtual HCURSOR GetCursor(int iIndex = 0);
    virtual HPEN GetPen(int iIndex = 0);
    virtual HBRUSH GetBrush(int iIndex = 0);
    virtual HBITMAP GetBitmap(int iIndex = 0);
    //@}

    /**
    @fn LRESULT SendMessage(UINT uMsg, IUITreeNode* pSender, WPARAM wParam, LPARAM lParam)
    @brief 消息发送，用于事件通知
    @param [in] uMsg 消息值
    @param [in] pSender 消息发送者
    @param [in] wParam 参数1
    @param [in] lParam 参数2
    */
    virtual LRESULT SendMessage(UINT uMsg, IUITreeNode* pSender, WPARAM wParam = 0, LPARAM lParam = 0);

    /**
    @fn IUITreeNode* GetCapture()
    @brief 获得鼠标捕获
    @return 获得捕获鼠标的节点
    */
    virtual IUITreeNode* GetCapture();

    /**
    @fn IUITreeNode* SetCapture(IUITreeNode* pNode)
    @brief 设置鼠标捕获
    @param [in] pNode 要捕获的节点
    @return 旧的捕获节点
    */
    virtual IUITreeNode* SetCapture(IUITreeNode* pNode);

    /**
    @fn bool ReleaseCapture()
    @brief 释放鼠标捕获
    @return 是否释放
    */
    virtual bool ReleaseCapture();

protected:
    IUITreeNode* m_pCapture;
    IUITreeNode* m_pSelected;
    IUITreeNode* m_pMouseIn;
};

} } //namespace common::ui
