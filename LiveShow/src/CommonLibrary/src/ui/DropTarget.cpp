// GGListDropTarget.cpp : 实现文件
//

#include "../../StdAfx.h"
#include "../../include/ui/DropTarget.h"

// CDropTarget
namespace common { namespace ui
{

IMPLEMENT_DYNAMIC(CDropTarget, COleDropTarget)


CDropTarget::CDropTarget()
{
}

CDropTarget::~CDropTarget()
{
}


BEGIN_MESSAGE_MAP(CDropTarget, COleDropTarget)
END_MESSAGE_MAP()



// CDropTarget 消息处理程序

DROPEFFECT CDropTarget::OnDragEnter(CWnd* pWnd, COleDataObject* pDataObject, DWORD 
					   dwKeyState, CPoint point )
{
	return DROPEFFECT_MOVE;
}

DROPEFFECT CDropTarget::OnDragOver(CWnd* pWnd, COleDataObject* pDataObject, DWORD 
					  dwKeyState, CPoint point )
{
	DragOverInfo doi;
	doi.dwKeyState = dwKeyState;
	doi.pDataObject = pDataObject;
	doi.point = point;
	doi.pWnd = pWnd;

	return pWnd->SendMessage(WM_OnDragOver, 0, (LPARAM)&doi);
}

void CDropTarget::OnDragLeave(CWnd* pWnd)
{
	pWnd->SendMessage(WM_OnDragLeave, 0, 0);
}

BOOL CDropTarget::OnDrop(CWnd* pWnd, COleDataObject* pDataObject, DROPEFFECT 
			dropEffect, CPoint point )
{
	DragEndInfo dei;
	dei.dropEffect = dropEffect;
	dei.pDataObject = pDataObject;
	dei.point = point;
	dei.pWnd = pWnd;

	return pWnd->SendMessage(WM_OnDragEnd, 0, (LPARAM)&dei);
}

} } //namespace common::ui
