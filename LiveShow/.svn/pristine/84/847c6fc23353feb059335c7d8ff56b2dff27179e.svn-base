#pragma once
#include "..\..\CommonLibrary/include/ui/MLHtmlView.h"

enum ENM_ListType
{
	LISTTYPE_SONG,			//当前是主播歌曲列表
	LISTTYPE_ORDER,			//当前是已点歌曲列表
	LISTTYPE_MARK,			//当前是评价歌曲列表
};

class COrderHtmlView : public CMLHtmlView
{
	DECLARE_DYNCREATE(COrderHtmlView)
	DECLARE_DISPATCH_MAP()

public:
	COrderHtmlView(void);
	virtual ~COrderHtmlView(void);

	void SetType(ENM_ListType enmType);

protected:
	BOOL Invoke_SaveSongList(int nSongerID, LPCTSTR lpszParam);		//保存歌单
	BOOL Invoke_AddSong(int nSongerID, LPCTSTR lpszSong);			//添加歌曲
	BOOL Invoke_ClearSongList(int nSongerID);						//清空歌单
	BOOL Invoke_OrderSong(int nSongerID, LPCTSTR lpszSong);			//用户点歌
	BOOL Invoke_OrderPriority(int nSongerID, LPCTSTR lpszSong);		//用户插播
	BOOL Invoke_SongListChange(int nChange);						//歌单是否变更
	BOOL Invoke_DecideSong(int nOrderID, int nStat);				//主播处理点歌
	BOOL Invoke_DecideEvaluate(int nOrderID, int nStat);			//评价歌曲
	BOOL Invoke_OpenOrderSong();									//打开点歌界面

protected:
	virtual void DocumentComplete(LPDISPATCH pDisp, VARIANT* URL);

private:
	ENM_ListType m_enmType;
};
