// TempAnnounceListCtrl.cpp : 实现文件
//

#include "stdafx.h"
#include "..\Room.h"
#include "TempAnnounceListCtrl.h"


// CTempAnnounceListCtrl

IMPLEMENT_DYNAMIC(CTempAnnounceListCtrl, CListCtrl)

CTempAnnounceListCtrl::CTempAnnounceListCtrl()
{

}

CTempAnnounceListCtrl::~CTempAnnounceListCtrl()
{
}


BEGIN_MESSAGE_MAP(CTempAnnounceListCtrl, CListCtrl)
END_MESSAGE_MAP()



// CTempAnnounceListCtrl 消息处理程序

BOOL CTempAnnounceListCtrl::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult) 
{
	switch( ((NMHDR*)lParam)->code )
	{
	case HDN_BEGINTRACKW:
	case HDN_BEGINTRACKA:
		*pResult = TRUE ;
		return TRUE ;
	}
	return CListCtrl::OnNotify( wParam ,lParam ,pResult ) ;
}
