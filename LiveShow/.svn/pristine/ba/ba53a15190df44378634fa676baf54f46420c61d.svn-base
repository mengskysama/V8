#pragma once
#include "coredefine.h"

struct actor_info 
{
	uint32 unIndex;				//麦序
	uint32 unUin;				//主播ID
	std::wstring strName;		//主播昵称
	std::wstring strHeadPath;	//主播头像
};

struct mark_info 
{
	uint32 orderId;
	uint32 orderTime;
	uint32 songerUin;
	std::wstring songerNickname;
	std::wstring songName;
};

class COrderSongManager
{
public:
	COrderSongManager(void);
	~COrderSongManager(void);

	void ClearActorInfo();
	void AddActorInfo(uint32 unIndex, uint32 unUin, std::wstring strName, std::wstring strHeadPath);
	void OnGetSongList(uint32 unIndex, uint32 unSongerUin, std::vector<core::Song> & vecSongList);
	void OnGetOrderList(uint32 unSelfUin, bool bActor, std::vector<core::SongOrder> & vecOrderList);

	std::wstring GetHeadList();
	std::wstring GetSongListByUin(uint32 unSongerUin);
	std::wstring GetOrderList();
	std::wstring GetMarkList();

	uint32 GetUndealCount(uint32 unSongerID);

	//对主播麦序的转换，传入页面
	uint32 GetChangedIndex(uint32 unIndex);

	void GetSongListByUin(uint32 unSongerUin, std::vector<core::Song> & vecSongList);
	uint32 GetOrderListCount();
	uint32 GetMarkListCount();

private:
	std::map<uint32, std::vector<core::Song>> m_mapSongList;
	//std::map<uint32, std::string> m_mapSongList;
	std::map<uint32, actor_info> m_mapActorInfo;
	std::vector<core::SongOrder> m_vecOrderList;
	std::vector<mark_info> m_vecMarkList;
	//std::string m_strOrderSongList;
};
