// SkinItemScrollBar.cpp : 实现文件
//

#include "stdafx.h"
#include "SkinItemScrollBar.h"


// CSkinItemScrollBar

IMPLEMENT_DYNCREATE(CSkinItemScrollBar, CXmlCmdTarget)
CSkinItemScrollBar::CSkinItemScrollBar()
{
	EnableAutomation();
	
	// 为了使应用程序在 OLE 自动化对象处于活动状态时保持
	//	运行，构造函数调用 AfxOleLockApp。
	
	AfxOleLockApp();
}

CSkinItemScrollBar::~CSkinItemScrollBar()
{
	// 为了在用 OLE 自动化创建所有对象后终止应用程序，
	// 	析构函数调用 AfxOleUnlockApp。
	
	AfxOleUnlockApp();
}


void CSkinItemScrollBar::OnFinalRelease()
{
	// 释放了对自动化对象的最后一个引用后，将调用
	// OnFinalRelease。基类将自动
	// 删除该对象。在调用该基类之前，请添加您的
	// 对象所需的附加清除代码。

	CXmlCmdTarget::OnFinalRelease();
}


BEGIN_MESSAGE_MAP(CSkinItemScrollBar, CXmlCmdTarget)
END_MESSAGE_MAP()


BEGIN_SKIN_DISPATCH_MAP(CSkinItemScrollBar, CXmlCmdTarget)
END_SKIN_DISPATCH_MAP(CSkinItemScrollBar)

// 注意: 我们添加 IID_ISkinItemScrollBar 支持
//以支持来自 VBA 的类型安全绑定。此 IID 必须同附加到 .IDL 文件中的
//调度接口的 GUID 匹配。

// {818F4473-9AB9-4017-BC1B-D04394DEACF4}
static const IID IID_ISkinItemScrollBar =
{ 0x818F4473, 0x9AB9, 0x4017, { 0xBC, 0x1B, 0xD0, 0x43, 0x94, 0xDE, 0xAC, 0xF4 } };

BEGIN_INTERFACE_MAP(CSkinItemScrollBar, CXmlCmdTarget)
	INTERFACE_PART(CSkinItemScrollBar, IID_ISkinItemScrollBar, Dispatch)
END_INTERFACE_MAP()

// {1D510678-90A1-4F0F-A440-9327389A462F}
IMPLEMENT_OLECREATE_FLAGS(CSkinItemScrollBar, "Skin.SkinItemScrollBar", afxRegApartmentThreading, 0x1d510678, 0x90a1, 0x4f0f, 0xa4, 0x40, 0x93, 0x27, 0x38, 0x9a, 0x46, 0x2f)


// CSkinItemScrollBar 消息处理程序
