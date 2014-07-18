// SkinItemTree.cpp : 实现文件
//

#include "stdafx.h"
#include "SkinItemTree.h"


// CSkinItemTree

IMPLEMENT_DYNCREATE(CSkinItemTree, CSkinItem)
CSkinItemTree::CSkinItemTree()
{
	EnableAutomation();

	// 为了使应用程序在 OLE 自动化对象处于活动状态时保持
	//	运行，构造函数调用 AfxOleLockApp。
	
	AfxOleLockApp();
}

CSkinItemTree::~CSkinItemTree()
{
	// 为了在用 OLE 自动化创建所有对象后终止应用程序，
	// 	析构函数调用 AfxOleUnlockApp。
	
	AfxOleUnlockApp();
}


void CSkinItemTree::OnFinalRelease()
{
	// 释放了对自动化对象的最后一个引用后，将调用
	// OnFinalRelease。基类将自动
	// 删除该对象。在调用该基类之前，请添加您的
	// 对象所需的附加清除代码。

	CCmdTarget::OnFinalRelease();
}


BEGIN_MESSAGE_MAP(CSkinItemTree, CSkinItem)
END_MESSAGE_MAP()


BEGIN_SKIN_DISPATCH_MAP(CSkinItemTree, CSkinItem)
    SKINBASE_DISP_PROPERTIES(CSkinItemTree)
    SKINITEM_DISP_PROPERTIES(CSkinItemTree)

END_SKIN_DISPATCH_MAP(CSkinItemTree)

// 注意: 我们添加 IID_ISkinItemTree 支持
//以支持来自 VBA 的类型安全绑定。此 IID 必须同附加到 .IDL 文件中的
//调度接口的 GUID 匹配。

// {73B3B8AA-D093-49C5-A98B-E07AA6D83A74}
static const IID IID_ISkinItemTree =
{ 0x73B3B8AA, 0xD093, 0x49C5, { 0xA9, 0x8B, 0xE0, 0x7A, 0xA6, 0xD8, 0x3A, 0x74 } };

BEGIN_INTERFACE_MAP(CSkinItemTree, CSkinItem)
	INTERFACE_PART(CSkinItemTree, IID_ISkinItemTree, Dispatch)
END_INTERFACE_MAP()

// {D9A6C415-86E1-424E-A8CF-1DB9C52F0402}
IMPLEMENT_OLECREATE_FLAGS(CSkinItemTree, "Skin.SkinItemTree", afxRegApartmentThreading, 0xd9a6c415, 0x86e1, 0x424e, 0xa8, 0xcf, 0x1d, 0xb9, 0xc5, 0x2f, 0x4, 0x2)


// CSkinItemTree 消息处理程序
BOOL CSkinItemTree::OnCustomDraw(NMHDR *pNMHDR, LRESULT *pResult)
{
    ///获取Tree View 的CustomDraw结构
    LPNMTVCUSTOMDRAW pNMTVCD = reinterpret_cast<LPNMTVCUSTOMDRAW>(pNMHDR);
    ///获取通用的CustomDraw结构
    LPNMCUSTOMDRAW pNMCD = &pNMTVCD->nmcd;
    // TODO: 在此添加控件通知处理程序代码
    *pResult = 0;
    // First thing - check the draw stage. If it's the control's prepaint
    // stage, then tell Windows we want messages for every item.
    if ( CDDS_PREPAINT == pNMCD->dwDrawStage )
    {
        *pResult = CDRF_NOTIFYITEMDRAW;
        return TRUE;
    }
    else if ( CDDS_ITEMPREPAINT == pNMCD->dwDrawStage )
    {
        if (   (CDIS_SELECTED & pNMCD->uItemState) 
            //||(CDIS_FOCUS & pNMCD->uItemState)
            )
        {
            // This is the prepaint stage for an item. Here's where we set the
            // item's text color. Our return value will tell Windows to draw the
            // item itself, but it will use the new color we set here.
            // We'll cycle the colors through red, green, and light blue.
            COLORREF crTextBk = GetbkColorNormal();
            COLORREF crText = GettextColorNormal();

            // Store the color back in the NMLVCUSTOMDRAW struct.
            pNMTVCD->clrTextBk = crTextBk;
            pNMTVCD->clrText = crText;
        }
        else
        {
            pNMTVCD->clrTextBk = GetbkColorNormal();

            pNMTVCD->clrText = GettextColorNormal();
        }

        // Tell Windows to paint the control itself.
        *pResult = CDRF_DODEFAULT;
    }

    return TRUE;
}
