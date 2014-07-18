#include "StdAfx.h"
#include "OrderHtmlView.h"
#include "coredefine.h"
#include "OrderSongDlg.h"

IMPLEMENT_DYNCREATE(COrderHtmlView, CMLHtmlView)

BEGIN_DISPATCH_MAP(COrderHtmlView, CMLHtmlView)
	DISP_FUNCTION(COrderHtmlView, "vod_saveSongList", Invoke_SaveSongList, VT_BOOL, VTS_I4 VTS_BSTR)
	DISP_FUNCTION(COrderHtmlView, "vod_addSong", Invoke_AddSong, VT_BOOL, VTS_I4 VTS_BSTR)
	DISP_FUNCTION(COrderHtmlView, "vod_dropSongList", Invoke_ClearSongList, VT_BOOL, VTS_I4)
	DISP_FUNCTION(COrderHtmlView, "vod_orderSong", Invoke_OrderSong, VT_BOOL, VTS_I4 VTS_BSTR)
	DISP_FUNCTION(COrderHtmlView, "vod_orderPriority", Invoke_OrderPriority, VT_BOOL, VTS_I4 VTS_BSTR)
	DISP_FUNCTION(COrderHtmlView, "vod_songListChange", Invoke_SongListChange, VT_BOOL, VTS_I4)
	DISP_FUNCTION(COrderHtmlView, "vod_decideSong", Invoke_DecideSong, VT_BOOL, VTS_I4 VTS_I4)
	DISP_FUNCTION(COrderHtmlView, "vod_decideEvaluate", Invoke_DecideEvaluate, VT_BOOL, VTS_I4 VTS_I4)
	DISP_FUNCTION(COrderHtmlView, "vod_openOrderSong", Invoke_OpenOrderSong, VT_BOOL, VTS_NONE)
END_DISPATCH_MAP()

COrderHtmlView::COrderHtmlView(void)
{
}

COrderHtmlView::~COrderHtmlView(void)
{
}

BOOL COrderHtmlView::Invoke_SaveSongList( int nSongerID, LPCTSTR lpszParam )
{
	return ((COrderSongDlg*)m_pParent)->OnSaveSongList(nSongerID, lpszParam);
}

BOOL COrderHtmlView::Invoke_AddSong( int nSongerID, LPCTSTR lpszSong )
{
	return ((COrderSongDlg*)m_pParent)->OnAddSong(nSongerID, lpszSong);
}

BOOL COrderHtmlView::Invoke_ClearSongList( int nSongerID )
{
	return ((COrderSongDlg*)m_pParent)->OnClearSongList(nSongerID);
}

BOOL COrderHtmlView::Invoke_OrderSong( int nSongerID, LPCTSTR lpszSong )
{
	return ((COrderSongDlg*)m_pParent)->OnOrderSong(nSongerID, lpszSong);
}

BOOL COrderHtmlView::Invoke_SongListChange( int nChange )
{
	return ((COrderSongDlg*)m_pParent)->OnSongListChange(nChange);
}

BOOL COrderHtmlView::Invoke_DecideSong( int nOrderID, int nStat )
{
	return ((COrderSongDlg*)m_pParent)->OnDecideSong(nOrderID, nStat);
}

void COrderHtmlView::DocumentComplete( LPDISPATCH pDisp, VARIANT* URL )
{
	if(m_lpDisp && (m_lpDisp == pDisp))//判断网页是否真正加载完毕,用于多Frame网页
	{
		SetScriptDocument();

		GetParent()->PostMessage(WM_MISSION_COMPLETED, NULL, (LPARAM)m_enmType);  //completed
		m_lpDisp=NULL;//Reset glpDisp		

	}
}

void COrderHtmlView::SetType( ENM_ListType enmType )
{
	m_enmType = enmType;
}

BOOL COrderHtmlView::Invoke_OrderPriority( int nSongerID, LPCTSTR lpszSong )
{
	return ((COrderSongDlg*)m_pParent)->OnOrderPriority(nSongerID, lpszSong);
}

BOOL COrderHtmlView::Invoke_DecideEvaluate( int nOrderID, int nStat )
{
	return ((COrderSongDlg*)m_pParent)->OnDecideEvaluate(nOrderID, nStat);
}

BOOL COrderHtmlView::Invoke_OpenOrderSong()
{
	return ((COrderSongDlg*)m_pParent)->OnOpenOrderSong();
}