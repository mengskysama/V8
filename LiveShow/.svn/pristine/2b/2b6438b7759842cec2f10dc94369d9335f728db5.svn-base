// 计算机生成了由 Microsoft Visual C++ 创建的 IDispatch 包装类

// 注意: 不要修改此文件的内容。如果此类由
//  Microsoft Visual C++ 重新生成，您的修改将被改写。


#include "stdafx.h"
#include "flash.h"
#include "..\UserMessageDefine.h"

/////////////////////////////////////////////////////////////////////////////
// CFlash

IMPLEMENT_DYNCREATE(CFlash, CWnd)

/////////////////////////////////////////////////////////////////////////////
// CFlash 属性

/////////////////////////////////////////////////////////////////////////////
// CFlash 操作
BEGIN_MESSAGE_MAP(CFlash, CWnd)
	ON_WM_LBUTTONDOWN()
	ON_WM_KILLFOCUS()
END_MESSAGE_MAP()

void CFlash::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CWnd * pWnd = GetParent();
	pWnd->SendMessage(WM_CLICK_FLASH, 0, 0);

	CWnd::OnLButtonDown(nFlags, point);
}

void CFlash::OnKillFocus(CWnd* pNewWnd)
{
	CWnd::OnKillFocus(pNewWnd);

	// TODO: 在此处添加消息处理程序代码
	CWnd * pWnd = GetParent();
	pWnd->SendMessage(WM_FLASH_KILLFOCUS, 0, 0);
}
