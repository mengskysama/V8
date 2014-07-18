// SkinItemAnimate.cpp : 实现文件
//

#include "stdafx.h"
#include "SkinItemAnimate.h"


// CSkinItemAnimate

IMPLEMENT_DYNCREATE(CSkinItemAnimate, CXmlCmdTarget)
CSkinItemAnimate::CSkinItemAnimate()
{
	EnableAutomation();
	
	// 为了使应用程序在 OLE 自动化对象处于活动状态时保持
	//	运行，构造函数调用 AfxOleLockApp。
	
	AfxOleLockApp();
}

CSkinItemAnimate::~CSkinItemAnimate()
{
	// 为了在用 OLE 自动化创建所有对象后终止应用程序，
	// 	析构函数调用 AfxOleUnlockApp。
	
	AfxOleUnlockApp();
}


void CSkinItemAnimate::OnFinalRelease()
{
	// 释放了对自动化对象的最后一个引用后，将调用
	// OnFinalRelease。基类将自动
	// 删除该对象。在调用该基类之前，请添加您的
	// 对象所需的附加清除代码。

	CXmlCmdTarget::OnFinalRelease();
}


BEGIN_MESSAGE_MAP(CSkinItemAnimate, CXmlCmdTarget)
END_MESSAGE_MAP()


BEGIN_SKIN_DISPATCH_MAP(CSkinItemAnimate, CXmlCmdTarget)
END_SKIN_DISPATCH_MAP(CSkinItemAnimate)

// 注意: 我们添加 IID_ISkinItemAnimate 支持
//以支持来自 VBA 的类型安全绑定。此 IID 必须同附加到 .IDL 文件中的
//调度接口的 GUID 匹配。

// {AC2B91D0-1017-4810-8878-A960F473460F}
static const IID IID_ISkinItemAnimate =
{ 0xAC2B91D0, 0x1017, 0x4810, { 0x88, 0x78, 0xA9, 0x60, 0xF4, 0x73, 0x46, 0xF } };

BEGIN_INTERFACE_MAP(CSkinItemAnimate, CXmlCmdTarget)
	INTERFACE_PART(CSkinItemAnimate, IID_ISkinItemAnimate, Dispatch)
END_INTERFACE_MAP()

// {6ABDF86F-1A1F-4EBC-95BF-15BF4192BAF0}
IMPLEMENT_OLECREATE_FLAGS(CSkinItemAnimate, "Skin.SkinItemAnimate", afxRegApartmentThreading, 0x6abdf86f, 0x1a1f, 0x4ebc, 0x95, 0xbf, 0x15, 0xbf, 0x41, 0x92, 0xba, 0xf0)


// CSkinItemAnimate 消息处理程序
