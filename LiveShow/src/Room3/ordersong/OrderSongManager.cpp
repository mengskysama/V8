#include "StdAfx.h"
#include "OrderSongManager.h"
#include "..\..\CommonLibrary/include/utility/json/json.h"
#include "..\..\CommonLibrary\include\utility\StringHelper.h"

COrderSongManager::COrderSongManager( void )
{

}

COrderSongManager::~COrderSongManager( void )
{
	m_mapActorInfo.clear();
	m_mapSongList.clear();
	m_vecOrderList.clear();
	m_vecMarkList.clear();
}

void COrderSongManager::AddActorInfo( uint32 unIndex, uint32 unUin, std::wstring strName, std::wstring strHeadPath )
{
	actor_info info;
	info.unIndex = unIndex;
	info.unUin = unUin;
	info.strName = strName;
	info.strHeadPath = strHeadPath;
	m_mapActorInfo[unIndex] = info;
}

//转成Json串，保存在Map里
void COrderSongManager::OnGetSongList( uint32 unIndex, uint32 unSongerUin, std::vector<core::Song>  & vecSongList )
{
	m_mapSongList[unSongerUin] = vecSongList;
}

void COrderSongManager::OnGetOrderList( uint32 unSelfUin, bool bActor, std::vector<core::SongOrder> & vecOrderList )
{
	m_vecOrderList = vecOrderList;
	m_vecMarkList.clear();
	for (std::vector<core::SongOrder>::iterator it = m_vecOrderList.begin(); it != m_vecOrderList.end(); ++it)
	{
		core::SongOrder songorder = *it;
		//自己点的歌，而且主播同意但还没评价的歌加入评价列表
		if (songorder.uin == unSelfUin && songorder.status == core::SongOrder::ORDER_STATUS_ACCEPTED)
		{
			mark_info info;
			info.orderId = songorder.orderId;
			info.orderTime = songorder.orderTime;
			info.songerUin = songorder.songerUin;
			info.songerNickname = songorder.songerNickname;
			info.songName = songorder.songName;
			m_vecMarkList.push_back(info);
		}
	}
}

void COrderSongManager::GetSongListByUin( uint32 unSongerUin, std::vector<core::Song> & vecSongList )
{
	if (m_mapSongList.find(unSongerUin) != m_mapSongList.end())
	{
		vecSongList = m_mapSongList[unSongerUin];
	}
}

void COrderSongManager::ClearActorInfo()
{
	m_mapActorInfo.clear();
}

std::wstring COrderSongManager::GetHeadList()
{
	Json::Value root;
	for (std::map<uint32, actor_info>::iterator it = m_mapActorInfo.begin(); it != m_mapActorInfo.end(); ++it)
	{
		actor_info info = it->second;
		Json::Value jinfo;
		jinfo["order"] = info.unIndex - 1;
		jinfo["RoleID"] = info.unUin;
		jinfo["RoleName"] = common::utility::stringhelper::UnicodeToUTF8(info.strName);
		jinfo["thumb"] = common::utility::stringhelper::UnicodeToUTF8(info.strHeadPath);
		
		root.append(jinfo);
	}

	Json::FastWriter jwriter;
	std::string strHeadList = jwriter.write(root);
	return common::utility::stringhelper::UTF8ToUnicode(strHeadList);
}

std::wstring COrderSongManager::GetSongListByUin( uint32 unSongerUin )
{
#if 0
	//test
	std::vector<core::Song> vecSongList;
	for (int i = 0; i < 20; i++)
	{
		core::Song song;
		song.songName = L"testsong" + common::utility::stringhelper::IntToString(i);
		song.songType = 0;
		vecSongList.push_back(song);
	}
	m_mapSongList[unSongerUin] = vecSongList;
#endif

	if (m_mapSongList.find(unSongerUin) != m_mapSongList.end())
	{
		if (m_mapSongList[unSongerUin].size() > 0)
		{
			Json::Value root;
			for (std::vector<core::Song>::iterator it = m_mapSongList[unSongerUin].begin(); it != m_mapSongList[unSongerUin].end(); ++it)
			{
				core::Song song = *it;
				Json::Value jsong;
				jsong["song"] = common::utility::stringhelper::UnicodeToUTF8(song.songName);
				jsong["hot"] = song.songType;
				root.append(jsong);
			}

			Json::FastWriter jwriter;
			std::string strSongList = jwriter.write(root);
			return common::utility::stringhelper::UTF8ToUnicode(strSongList);
		}
	}

	return L"[]";
}

std::wstring COrderSongManager::GetOrderList()
{
#if 0
	//test
	for (int i = 0; i < 20; i++)
	{
		core::SongOrder ordersong;
		ordersong.orderId = i;
		ordersong.songerUin = (rand() % 2 == 0 ? 25781 : 10000);
		ordersong.songerNickname = L"da he xiang dong liu";
		ordersong.uin = 1001;
		ordersong.nickName = L"order user";
		ordersong.orderTime = 1120;
		ordersong.songName = L"test song";
		ordersong.attachWords = L"我haha测试1111我haha测试1111我haha测试1111我haha测试1111我haha测试1111我haha测试1111";
		ordersong.status = core::SongOrder::ORDER_STATUS_TODO;
		m_vecOrderList.push_back(ordersong);

	}
#endif

	if (m_vecOrderList.size() > 0)
	{
		Json::Value root;
		for (std::vector<core::SongOrder>::iterator it = m_vecOrderList.begin(); it != m_vecOrderList.end(); ++it)
		{
			core::SongOrder songorder = *it;
			Json::Value jsongorder;
			jsongorder["orderID"] = songorder.orderId;
			jsongorder["RoleID"] = songorder.songerUin;
			jsongorder["RoleName"] = common::utility::stringhelper::UnicodeToUTF8(songorder.songerNickname);
			jsongorder["UserID"] = songorder.uin;
			jsongorder["UserName"] = common::utility::stringhelper::UnicodeToUTF8(songorder.nickName);
			CTime tmOrder = songorder.orderTime;
			CString strTime = tmOrder.Format(L"%H:%M");
			jsongorder["time"] = common::utility::stringhelper::UnicodeToUTF8(strTime.GetBuffer(0));
			jsongorder["song"] = common::utility::stringhelper::UnicodeToUTF8(songorder.songName);
			jsongorder["wish"] = common::utility::stringhelper::UnicodeToUTF8(songorder.attachWords);
			//jsongorder["stat"] = songorder.status;
			CString strCount = L"";
			if (songorder.count >= 3)
			{
				strCount.Format(L"%d首", songorder.count);
			}
			jsongorder["priority"] = common::utility::stringhelper::UnicodeToUTF8(strCount.GetBuffer(0));
			switch (songorder.status)
			{
			case core::SongOrder::ORDER_STATUS_TODO:
				{
					jsongorder["stat"] = 0;
					jsongorder["evaluate"] = 4;
				}
				break;
			case core::SongOrder::ORDER_STATUS_ACCEPTED:
				{
					jsongorder["stat"] = 1;
					jsongorder["evaluate"] = 0;
				}
				break;
			case core::SongOrder::ORDER_STATUS_REJECTED:
				{
					jsongorder["stat"] = 2;
					jsongorder["evaluate"] = 4;
				}
				break;
			case core::SongOrder::ORDER_STATUS_MARK_GOOD:
				{
					jsongorder["stat"] = 1;
					jsongorder["evaluate"] = 3;
				}
				break;
			case core::SongOrder::ORDER_STATUS_MARK_BAD:
				{
					jsongorder["stat"] = 1;
					jsongorder["evaluate"] = 2;
				}
				break;
			case core::SongOrder::ORDER_STATUS_MARK_NONE:
				{
					jsongorder["stat"] = 1;
					jsongorder["evaluate"] = 1;
				}
				break;
			}
			root.append(jsongorder);
		}

		Json::FastWriter jwriter;
		std::string strOrderList = jwriter.write(root);
		return common::utility::stringhelper::UTF8ToUnicode(strOrderList);
	}

	return L"[]";
}

uint32 COrderSongManager::GetOrderListCount()
{
	return m_vecOrderList.size();
}

uint32 COrderSongManager::GetChangedIndex( uint32 unIndex )
{
	uint32 unCount = 0;
	for (std::map<uint32, actor_info>::iterator it = m_mapActorInfo.begin(); it != m_mapActorInfo.end(); ++it)
	{
		if (unIndex == it->first)
		{
			return unCount;
		}

		unCount++;
	}

	return unCount;
}

uint32 COrderSongManager::GetUndealCount( uint32 unSongerID )
{
	uint32 unCount = 0;
	for (std::vector<core::SongOrder>::iterator it = m_vecOrderList.begin(); it != m_vecOrderList.end(); ++it)
	{
		core::SongOrder songorder = *it;
		if (songorder.songerUin == unSongerID && songorder.status == core::SongOrder::ORDER_STATUS_TODO)
		{
			unCount++;
		}
	}

	return unCount;
}

std::wstring COrderSongManager::GetMarkList()
{
	if (m_vecMarkList.size() > 0)
	{
		Json::Value root;
		for (std::vector<mark_info>::iterator it = m_vecMarkList.begin(); it != m_vecMarkList.end(); ++it)
		{
			mark_info markinfo = *it;
			Json::Value jsmarkinfo;
			jsmarkinfo["orderID"] = markinfo.orderId;
			jsmarkinfo["RoleID"] = markinfo.songerUin;
			jsmarkinfo["RoleName"] = common::utility::stringhelper::UnicodeToUTF8(markinfo.songerNickname);
			CTime tmOrder = markinfo.orderTime;
			CString strTime = tmOrder.Format(L"%H:%M");
			jsmarkinfo["time"] = common::utility::stringhelper::UnicodeToUTF8(strTime.GetBuffer(0));
			jsmarkinfo["song"] = common::utility::stringhelper::UnicodeToUTF8(markinfo.songName);
			root.append(jsmarkinfo);
		}

		Json::FastWriter jwriter;
		std::string strMarkList = jwriter.write(root);
		return common::utility::stringhelper::UTF8ToUnicode(strMarkList);
	}

	return L"[]";
}

uint32 COrderSongManager::GetMarkListCount()
{
	return m_vecMarkList.size();
}