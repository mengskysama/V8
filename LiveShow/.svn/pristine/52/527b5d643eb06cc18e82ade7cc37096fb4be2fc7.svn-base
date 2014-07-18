// SkinItemGroup.cpp : 实现文件
//

#include "stdafx.h"
#include "SkinItem.h"
#include "SkinItemGroup.h"


// CSkinItemGroup

IMPLEMENT_DYNCREATE(CSkinItemGroup, CSkinItem)
CSkinItemGroup::CSkinItemGroup()
{
	EnableAutomation();
	
	// 为了使应用程序在 OLE 自动化对象处于活动状态时保持
	//	运行，构造函数调用 AfxOleLockApp。
	
	AfxOleLockApp();
}

CSkinItemGroup::~CSkinItemGroup()
{
	// 为了在用 OLE 自动化创建所有对象后终止应用程序，
	// 	析构函数调用 AfxOleUnlockApp。
	
	AfxOleUnlockApp();
}


void CSkinItemGroup::OnFinalRelease()
{
	// 释放了对自动化对象的最后一个引用后，将调用
	// OnFinalRelease。基类将自动
	// 删除该对象。在调用该基类之前，请添加您的
	// 对象所需的附加清除代码。

	CSkinItem::OnFinalRelease();
}


BEGIN_MESSAGE_MAP(CSkinItemGroup, CSkinItem)
END_MESSAGE_MAP()


BEGIN_SKIN_DISPATCH_MAP(CSkinItemGroup, CSkinItem)
END_SKIN_DISPATCH_MAP(CSkinItemGroup)

// 注意: 我们添加 IID_ISkinItemGroup 支持
//以支持来自 VBA 的类型安全绑定。此 IID 必须同附加到 .IDL 文件中的
//调度接口的 GUID 匹配。

// {540DEB4D-8322-4C53-983D-4942AA7B237A}
static const IID IID_ISkinItemGroup =
{ 0x540DEB4D, 0x8322, 0x4C53, { 0x98, 0x3D, 0x49, 0x42, 0xAA, 0x7B, 0x23, 0x7A } };

BEGIN_INTERFACE_MAP(CSkinItemGroup, CSkinItem)
	INTERFACE_PART(CSkinItemGroup, IID_ISkinItemGroup, Dispatch)
END_INTERFACE_MAP()

// {623BD64E-9E96-4DB7-A6DD-126390DD47EF}
IMPLEMENT_OLECREATE_FLAGS(CSkinItemGroup, "Skin.SkinItemGroup", afxRegApartmentThreading, 0x623bd64e, 0x9e96, 0x4db7, 0xa6, 0xdd, 0x12, 0x63, 0x90, 0xdd, 0x47, 0xef)


// CSkinItemGroup 消息处理程序
