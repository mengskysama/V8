// SkinImageStyle.cpp : 实现文件
//

#include "stdafx.h"
#include "SkinImageStyle.h"


// CSkinImageStyle

IMPLEMENT_DYNCREATE(CSkinImageStyle, CXmlCmdTarget)
CSkinImageStyle::CSkinImageStyle()
{
	EnableAutomation();
	
	// 为了使应用程序在 OLE 自动化对象处于活动状态时保持
	//	运行，构造函数调用 AfxOleLockApp。
	
	AfxOleLockApp();
}

CSkinImageStyle::~CSkinImageStyle()
{
	// 为了在用 OLE 自动化创建所有对象后终止应用程序，
	// 	析构函数调用 AfxOleUnlockApp。
	
	AfxOleUnlockApp();
}


void CSkinImageStyle::OnFinalRelease()
{
	// 释放了对自动化对象的最后一个引用后，将调用
	// OnFinalRelease。基类将自动
	// 删除该对象。在调用该基类之前，请添加您的
	// 对象所需的附加清除代码。

	CXmlCmdTarget::OnFinalRelease();
}


BEGIN_MESSAGE_MAP(CSkinImageStyle, CXmlCmdTarget)
END_MESSAGE_MAP()


BEGIN_SKIN_DISPATCH_MAP(CSkinImageStyle, CXmlCmdTarget)
END_SKIN_DISPATCH_MAP(CSkinImageStyle)

// 注意: 我们添加 IID_ISkinImageStyle 支持
//以支持来自 VBA 的类型安全绑定。此 IID 必须同附加到 .IDL 文件中的
//调度接口的 GUID 匹配。

// {D8CFDFC4-4628-4DD7-B9C8-A8C7223BEBB7}
static const IID IID_ISkinImageStyle =
{ 0xD8CFDFC4, 0x4628, 0x4DD7, { 0xB9, 0xC8, 0xA8, 0xC7, 0x22, 0x3B, 0xEB, 0xB7 } };

BEGIN_INTERFACE_MAP(CSkinImageStyle, CXmlCmdTarget)
	INTERFACE_PART(CSkinImageStyle, IID_ISkinImageStyle, Dispatch)
END_INTERFACE_MAP()

// {CF9EED20-C3E0-4E26-B0C2-B5DE9F89F641}
IMPLEMENT_OLECREATE_FLAGS(CSkinImageStyle, "Skin.SkinImageStyle", afxRegApartmentThreading, 0xcf9eed20, 0xc3e0, 0x4e26, 0xb0, 0xc2, 0xb5, 0xde, 0x9f, 0x89, 0xf6, 0x41)


// CSkinImageStyle 消息处理程序
