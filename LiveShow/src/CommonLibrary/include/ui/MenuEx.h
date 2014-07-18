#pragma once

#include "../Common.h"
#include "./ClassDataMacro.h"
#include <xstring>
#include <vector>

struct IImage;

namespace common { namespace ui
{

#define NO_MODIFY_IMAGE (-2)

#define MII_ID          0x00000001
#define MII_TEXT        0x00000002
#define MII_IMAGE       0x00000004

typedef struct tagMENUITEMINFOEX
{
    UINT            uMask;
    UINT            uID;
    std::wstring    wstrText;
    int             iImage;
}MENUITEMINFOEX, *LPMENUITEMINFOEX;

typedef COLORREF (*COLOR_TRANSFORM_PROC)(COLORREF);

class COMMON_INTERFACE CMenuEx :
    public CMenu
{
    DECLARE_DYNAMIC(CMenuEx)

public:
    /**
    @fn CMenuEx(void)
    @brief 构造函数
    */
    CMenuEx(void);

    /**
    @fn ~CMenuEx(void)
    @brief 析构函数
    */
    ~CMenuEx(void);

    /**
    @fn BOOL LoadMenu(UINT nIDResource)
    @brief 从资源加载
    @param [in] nIDResource 资源ID
    */
    BOOL LoadMenu(UINT nIDResource);

    /**
    @fn BOOL LoadMenu(LPCTSTR lpszResourceName)
    @brief 从资源加载
    @param [in] lpszResourceName 资源名
    */
    BOOL LoadMenu(LPCTSTR lpszResourceName);

    /**
    @fn BOOL LoadMenu(HMENU hMenu)
    @brief 从菜单加载
    @param [in] hMenu 菜单句柄
    */
    BOOL LoadMenu(HMENU hMenu);

    /**
    @fn BOOL LoadMenu(const CMenu& menu)
    @brief 从菜单加载
    @param [in] menu 菜单
    */
    BOOL LoadMenu(const CMenu& menu);

    /**
    @fn BOOL DestroyMenu();
    @brief 销毁菜单
    @return 返回是否成功
    */
    BOOL DestroyMenu();

    /** 
    @addtogroup ImageList
    @brief 图像
    */
    //@{
    CImageList* GetImageList();
    void SetImageList(CImageList* pImageList);
    CImageList* GetImageListSelected();
    void SetImageListSelected(CImageList* pImageList);
    CImageList* GetImageListDisabled();
    void SetImageListDisabled(CImageList* pImageList);
    std::vector<IImage*>* GetImageVector();
    void SetImageVector(std::vector<IImage*>* pImageVector);
    std::vector<IImage*>* GetImageVectorSelected();
    void SetImageVectorSelected(std::vector<IImage*>* pImageVector);
    std::vector<IImage*>* GetImageVectorDisabled();
    void SetImageVectorDisabled(std::vector<IImage*>* pImageVector);
    //@}

    /**
    @brief 获取子目录
    @param [in] nPosition 子目录的位置，从0开始
    @return 返回子目录
    */
    CMenuEx* GetSubMenu(int nPosition);

    /** 
    @addtogroup Size
    @brief 尺寸相关设置
    */ 
    //@{
    LONG GetWidth();
    void SetWidth(LONG nWidth);
    LONG GetHeight();
    void SetHeight(LONG nHeight);
    LONG GetSeparatorHeight();
    void SetSeparatorHeight(LONG nHeight);
    LONG GetLeftSideWidth();
    void SetLeftSideWidth(LONG nWidth);
    //@}

    /** 
    @addtogroup Display
    @brief 显示相关设置
    */ 
    //@{
    COLORREF GetTextColor();
    void SetTextColor(COLORREF clrColor);
    COLORREF GetTextColorSelected();
    void SetTextColorSelected(COLORREF clrColor);
    COLORREF GetTextColorDisabled();
    void SetTextColorDisabled(COLORREF clrColor);
    COLORREF GetBkColor();
    void SetBkColor(COLORREF clrColor);
    COLORREF GetBkColorSelected();
    void SetBkColorSelected(COLORREF clrColor);
    COLORREF GetBorderColorSelected();
    void SetBorderColorSelected(COLORREF clrColor);
    COLORREF GetLeftSideColorLeft();
    void SetLeftSideColorLeft(COLORREF clrColor);
    COLORREF GetLeftSideColorRight();
    void SetLeftSideColorRight(COLORREF clrColor);
    COLORREF GetSeparatorColor();
    void SetSeparatorColor(COLORREF clrColor);
    COLORREF GetSeparatorColorSide();
    void SetSeparatorColorSide(COLORREF clrColor);
    COLORREF GetTransparentColor();
    void SetTransparentColor(COLORREF clrColor);
    BOOL GetNoPrefix();
    void SetNoPrefix(BOOL bTrue);
    //@}

    /**
    @brief 在尾部添加菜单项
    @param [in] nFlags 
    @param [in] nIDNewItem 
    @param [in] lpszNewItem 
    @param [in] iImage 菜单图标在ImageList中的位置
    @return 是否成功
    */
    BOOL AppendMenuEx(UINT nFlags, UINT nIDNewItem = 0, LPCTSTR lpszNewItem = NULL , int iImage = -1);

    /**
    @fn BOOL ModifyMenuEx(UINT nPosition, UINT nFlags, UINT nIDNewItem = 0, LPCTSTR lpszNewItem = NULL)
    @brief 修改菜单
    @param [in] nPosition 
    @param [in] nFlags 
    @param [in] nIDNewItem 
    @param [in] lpszNewItem 菜单项的文字，NULL表示不修改
    @param [in] iImage 菜单图标在ImageList中的位置，小于等于-2表示不修改
    @return 是否成功
    */
    BOOL ModifyMenuEx(UINT nPosition, UINT nFlags, UINT nIDNewItem = 0, LPCTSTR lpszNewItem = NULL, int iImage = NO_MODIFY_IMAGE);

    /**
    @fn BOOL RemoveMenuEx(UINT nPosition, UINT nFlags)
    @brief 删除一个菜单项，为CMenu的重载函数
    @param [in] nPosition 
    @param [in] nFlags 
    @return 是否成功
    */
    BOOL RemoveMenuEx(UINT nPosition, UINT nFlags);

    /**
    @fn static CMenuEx* NewPopupMenu(HMENU hPopup)
    @param [in] hOldPopup 
    @return 返回创建的菜单
    */
    static CMenuEx* NewPopupMenu(HMENU hPopup);

    /**
    @fn static void SetColorTransformProc(COLOR_TRANSFORM_PROC pfnColorTransform)
    @param [in] pfnColorTransform 变换函数指针，为空表示不转换
    */
    static void SetColorTransformProc(COLOR_TRANSFORM_PROC pfnColorTransform);

protected:
    /**
    */
    virtual void MeasureItem(LPMEASUREITEMSTRUCT lpMIS);

    /**
    */
    virtual void DrawItem(LPDRAWITEMSTRUCT lpDIS);

protected:
    DECLARE_CLASS_DATA(CMenuEx)
};

} } //namespace common::ui
