/**********************************************************
* 版权所有 (C)2009, 51.com
*
* 文件名称：CommonWndOleDropTarget.cpp
* 内容摘要：拖拽COleDropTarget扩展类，用于实现在拖拽目的方实现拖拽功能
* 其它说明：
* 当前版本：
* 作    者：温辉敏
* 完成日期：2009-10-29
*
* 修改记录1：
*    修改日期：
*    版 本 号：
*    修 改 人：
*    修改内容：
**********************************************************/
// CommonWndOleDropTarget.cpp : 实现文件
//

#include "stdafx.h"
#include "../../include/ui/CommonWndOleDropTarget.h"

namespace common { namespace ui
{

//////////////////////////////////////////////////////////////////////////
/// CCommonWndOleDropTarget的实现代码

IMPLEMENT_DYNAMIC(CCommonWndOleDropTarget, COleDropTarget)

CCommonWndOleDropTarget::CCommonWndOleDropTarget()
{
}

CCommonWndOleDropTarget::~CCommonWndOleDropTarget()
{
}

BEGIN_MESSAGE_MAP(CCommonWndOleDropTarget, COleDropTarget)
END_MESSAGE_MAP()

/// CCommonWndOleDropTarget 消息处理程序

DROPEFFECT CCommonWndOleDropTarget::OnDragEnter(
	CWnd* pWnd, 
	COleDataObject* pDataObject, 
	DWORD dwKeyState, 
	CPoint point 
	)
{
	/**下面使用了MFC的IsKindOf或RTTI的dynamic_cast来进行类型识别
	1.由于IsKindOf(使用IMPLEMENT_DYNAMIC)不支持多重继承，有多重继承时IsKindOf将失败此时就采用RTTI来进行识别了。
	*/
	DROPEFFECT deResult = DROPEFFECT_NONE;
	if (NULL != pWnd 
		&& NULL != m_hWnd 
		&& (pWnd->IsKindOf(RUNTIME_CLASS(ICommonWndOleDropTarget)) || dynamic_cast<ICommonWndOleDropTarget *>(pWnd))
		)
	{
#ifdef _CPPRTTI 
		ICommonWndOleDropTarget *ptrCommonWndDropTarget = dynamic_cast<ICommonWndOleDropTarget *>(pWnd);
#else
		ICommonWndOleDropTarget *ptrCommonWndDropTarget = (ICommonWndOleDropTarget *)pWnd;
#endif
		if (NULL != ptrCommonWndDropTarget)
		{
			deResult = ptrCommonWndDropTarget->OnDragEnter(pWnd, pDataObject, dwKeyState, point);
		}
	}
	else
	{
		deResult = __super::OnDragEnter(pWnd, pDataObject, dwKeyState, point);
	}

	return deResult;
}

DROPEFFECT CCommonWndOleDropTarget::OnDragOver(
	CWnd* pWnd, 
	COleDataObject* pDataObject, 
	DWORD dwKeyState, 
	CPoint point 
	)
{
	DROPEFFECT deResult = DROPEFFECT_NONE;
	if (NULL != pWnd 
		&& NULL != m_hWnd 
		&& (pWnd->IsKindOf(RUNTIME_CLASS(ICommonWndOleDropTarget)) || dynamic_cast<ICommonWndOleDropTarget *>(pWnd))
		)
	{
#ifdef _CPPRTTI 
		ICommonWndOleDropTarget *ptrCommonWndDropTarget = dynamic_cast<ICommonWndOleDropTarget *>(pWnd);
#else
		ICommonWndOleDropTarget *ptrCommonWndDropTarget = (ICommonWndOleDropTarget *)pWnd;
#endif
		if (NULL != ptrCommonWndDropTarget)
		{
			deResult = ptrCommonWndDropTarget->OnDragOver(pWnd, pDataObject, dwKeyState, point);
		}
	}
	else
	{
		deResult = __super::OnDragOver(pWnd, pDataObject, dwKeyState, point);
	}

	return deResult;
}

void CCommonWndOleDropTarget::OnDragLeave(CWnd* pWnd)
{
	//DROPEFFECT deResult = DROPEFFECT_NONE;
	if (NULL != pWnd 
		&& NULL != m_hWnd 
		&& (pWnd->IsKindOf(RUNTIME_CLASS(ICommonWndOleDropTarget)) || dynamic_cast<ICommonWndOleDropTarget *>(pWnd))
		)
	{
#ifdef _CPPRTTI 
		ICommonWndOleDropTarget *ptrCommonWndDropTarget = dynamic_cast<ICommonWndOleDropTarget *>(pWnd);
#else
		ICommonWndOleDropTarget *ptrCommonWndDropTarget = (ICommonWndOleDropTarget *)pWnd;
#endif
		if (NULL != ptrCommonWndDropTarget)
		{
			ptrCommonWndDropTarget->OnDragLeave(pWnd);
		}
	}
	else
	{
		__super::OnDragLeave(pWnd);
	}
}

BOOL CCommonWndOleDropTarget::OnDrop(
	CWnd* pWnd,
	COleDataObject* pDataObject,
	DROPEFFECT dropEffect, 
	CPoint point 
	)
{
	//DROPEFFECT deResult = DROPEFFECT_NONE;
	BOOL bResult = FALSE;
	if (NULL != pWnd 
		&& NULL != m_hWnd 
		&& (pWnd->IsKindOf(RUNTIME_CLASS(ICommonWndOleDropTarget)) || dynamic_cast<ICommonWndOleDropTarget *>(pWnd))
		)
	{
#ifdef _CPPRTTI 
		ICommonWndOleDropTarget *ptrCommonWndDropTarget = dynamic_cast<ICommonWndOleDropTarget *>(pWnd);
#else
		ICommonWndOleDropTarget *ptrCommonWndDropTarget = (ICommonWndOleDropTarget *)pWnd;
#endif
		if (NULL != ptrCommonWndDropTarget)
		{
			bResult = ptrCommonWndDropTarget->OnDrop(pWnd, pDataObject, dropEffect, point);	
		}
	}
	else
	{
		bResult = __super::OnDrop(pWnd, pDataObject, dropEffect, point);
	}

	return bResult;
}

//////////////////////////////////////////////////////////////////////////
///ICommonWndOleDropTarget的实现代码
IMPLEMENT_DYNAMIC(ICommonWndOleDropTarget, CObject)

} } //namespace common::ui
