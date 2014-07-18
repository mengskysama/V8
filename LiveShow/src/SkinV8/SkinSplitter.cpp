// SkinSplitter.cpp : 实现文件
//

#include "stdafx.h"
#include "SkinBase.h"
#include "SkinSplitter.h"


// CSkinSplitter

IMPLEMENT_DYNCREATE(CSkinSplitter, CSkinBase)
CSkinSplitter::CSkinSplitter()
{
	EnableAutomation();
	
	// 为了使应用程序在 OLE 自动化对象处于活动状态时保持
	//	运行，构造函数调用 AfxOleLockApp。
	
	AfxOleLockApp();
}

CSkinSplitter::~CSkinSplitter()
{
	// 为了在用 OLE 自动化创建所有对象后终止应用程序，
	// 	析构函数调用 AfxOleUnlockApp。
	
	AfxOleUnlockApp();
}


void CSkinSplitter::OnFinalRelease()
{
	// 释放了对自动化对象的最后一个引用后，将调用
	// OnFinalRelease。基类将自动
	// 删除该对象。在调用该基类之前，请添加您的
	// 对象所需的附加清除代码。

	CSkinBase::OnFinalRelease();
}


BEGIN_MESSAGE_MAP(CSkinSplitter, CSkinBase)
END_MESSAGE_MAP()


BEGIN_SKIN_DISPATCH_MAP(CSkinSplitter, CSkinBase)
	SKINBASE_DISP_PROPERTIES(CSkinSplitter)
END_SKIN_DISPATCH_MAP(CSkinSplitter)

// 注意: 我们添加 IID_ISkinSplitter 支持
//以支持来自 VBA 的类型安全绑定。此 IID 必须同附加到 .IDL 文件中的
//调度接口的 GUID 匹配。

// {5480A98C-0A88-4696-A102-4964DB31D777}
static const IID IID_ISkinSplitter =
{ 0x5480A98C, 0xA88, 0x4696, { 0xA1, 0x2, 0x49, 0x64, 0xDB, 0x31, 0xD7, 0x77 } };

BEGIN_INTERFACE_MAP(CSkinSplitter, CSkinBase)
	INTERFACE_PART(CSkinSplitter, IID_ISkinSplitter, Dispatch)
END_INTERFACE_MAP()

// {8BDFDABC-DA04-429C-8309-213A29D9A2F8}
IMPLEMENT_OLECREATE_FLAGS(CSkinSplitter, "Skin.SkinSplitter", afxRegApartmentThreading, 0x8bdfdabc, 0xda04, 0x429c, 0x83, 0x9, 0x21, 0x3a, 0x29, 0xd9, 0xa2, 0xf8)


// CSkinSplitter 消息处理程序
