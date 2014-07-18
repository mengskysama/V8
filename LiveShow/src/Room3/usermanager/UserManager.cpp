#include "StdAfx.h"
#include "UserManager.h"
#include "..\userlist/ListIconManager.h"
#include "..\..\CommonLibrary\include\utility\StringHelper.h"

UserManager::UserManager(uint32 roomid, IUserManagerSink * pRoomBusinessSink)
: m_roomid(roomid),
  m_pRoomBusinessSink(pRoomBusinessSink),
  m_pSelfUserItem(NULL),
  m_nHideUserCount(0)
{
}

UserManager::~UserManager(void)
{
	for (MapUserItem::iterator iter = m_mapAllUserItem.begin(); iter != m_mapAllUserItem.end(); ++iter)
	{
		if (iter->second != NULL)
		{
			delete iter->second;
		}
	}
	m_mapAllUserItem.clear();
	m_mapWaitingUserItem.clear();
	m_mapAdminUserItem.clear();
	m_mapGuestUserItem.clear();
	m_mapAgentUserItem.clear();
}

void UserManager::GetWaitingList( std::vector<uint32> & vecWaitinglist )
{
	for (std::vector<uint32>::iterator it = vecWaitinglist.begin(); it != vecWaitinglist.end(); ++it)
	{
		MapUserItem::iterator iter = m_mapAllUserItem.find(*it);
		if (iter != m_mapAllUserItem.end())
		{
			IUserItem * pUserItem = iter->second;
			m_mapWaitingUserItem[*it] = pUserItem;
			pUserItem->SetWaiting(true);
			m_pRoomBusinessSink->OnAddWaitingUser(pUserItem);
		}
	}

	m_pRoomBusinessSink->OnUserListCountChange();
	//m_pRoomBusinessSink->OnGetWaitingList(m_mapWaitingUserItem);
}

IUserItem * UserManager::AddUserItem( core::room_user_info & UserInfo )
{
	IUserItem * pUserItem = NULL;
	MapUserItem::iterator iter = m_mapAllUserItem.find(UserInfo.unUIN);
	if (iter != m_mapAllUserItem.end())
	{
		pUserItem = m_mapAllUserItem[UserInfo.unUIN];
	}
	else
	{
		pUserItem = new IUserItem;
	}

	pUserItem->SetUserInfo(UserInfo);
	UpdateUserIcon(pUserItem);

	if (pUserItem->IsHideLogin() && pUserItem->GetUserUIN() != GetCurInfo().unUIN)
	{
		m_nHideUserCount++;
	}

	m_mapAllUserItem[UserInfo.unUIN] = pUserItem;

	if (m_pSelfUserItem == NULL)
	{
		if (GetCurInfo().unUIN == UserInfo.unUIN)
		{
			m_pSelfUserItem = pUserItem;
		}
	}

	//Notify UI
	m_pRoomBusinessSink->OnAddUser(pUserItem);

	if (UserInfo.unTitleLevel > core::TITLELEVEL_NONE)
	{
		m_mapAdminUserItem[UserInfo.unUIN] = pUserItem;

		//Notify UI
		//m_pRoomBusinessSink->OnAddAdminUser(pUserItem);
	}

	if (UserInfo.unVipLevel == core::VIPLEVEL_NONE)
	{
		m_mapGuestUserItem[UserInfo.unUIN] = pUserItem;
	}

	if ((UserInfo.unBusiness & core::BUSINESSTYPE_AGENT) != 0)
	{
		m_mapAgentUserItem[UserInfo.unUIN] = pUserItem;
	}

	m_pRoomBusinessSink->OnUserListCountChange();

	return pUserItem;
}

bool UserManager::DeleteUserItem( uint32 UIN )
{
	bool bAdmin = false;
	bool bWaiting = false;
	bool bGuest = false;
	MapUserItem::iterator iter = m_mapAllUserItem.find(UIN);
	if (iter != m_mapAllUserItem.end())
	{
		IUserItem * pUserItem = iter->second;

		if (pUserItem->IsHideLogin())
		{
			m_nHideUserCount--;
			ASSERT(m_nHideUserCount >= 0);
		}

		//Notify UI
		m_pRoomBusinessSink->OnDeleteUser(pUserItem);
		//if (pUserItem->IsWaiting())
		//{
		//	bWaiting = true;
		//}
		//if (pUserItem->GetUserTitleLevel() > core::TITLELEVEL_NONE)
		//{
		//	bAdmin = true;
		//}
		//if (pUserItem->GetUserVipLevel() == core::VIPLEVEL_NONE)
		//{
		//	bGuest = true;
		//}
		delete iter->second;
		m_mapAllUserItem.erase(iter);
	}

	if (m_mapWaitingUserItem.find(UIN) != m_mapWaitingUserItem.end())
	{
		m_mapWaitingUserItem.erase(UIN);
	}

	if (m_mapAdminUserItem.find(UIN) != m_mapAdminUserItem.end())
	{
		m_mapAdminUserItem.erase(UIN);
	}

	if (m_mapGuestUserItem.find(UIN) != m_mapGuestUserItem.end())
	{
		m_mapGuestUserItem.erase(UIN);
	}

	if (m_mapAgentUserItem.find(UIN) != m_mapAgentUserItem.end())
	{
		m_mapAgentUserItem.erase(UIN);
	}

	m_pRoomBusinessSink->OnUserListCountChange();

	return true;
}

bool UserManager::DeleteUserItem( IUserItem * pUserItem )
{
	return true;
}

void UserManager::DeleteUserItem( std::vector<uint32> & vecDelList )
{
	for (std::vector<uint32>::iterator it = vecDelList.begin(); it != vecDelList.end(); ++it)
	{
		DeleteUserItem(*it);
	}
}

bool UserManager::UpdateUserStatus( uint32 UIN, uint16 unStatus, bool bAdd )
{
	IUserItem * pUserItem = SearchUserByUIN(UIN);
	if (pUserItem != NULL)
	{
		if (bAdd)
		{
			pUserItem->AddUserStatus(unStatus);
		}
		else
		{
			pUserItem->RemoveUserStatus(unStatus);
		}
		UpdateUserIcon(pUserItem);

		//Notify UI
		m_pRoomBusinessSink->OnUpdateUser(pUserItem);
		return true;
	}

	return false;
}

bool UserManager::UpdateUserStamp( uint32 UIN, uint16 unStampID, bool bAdd )
{
	IUserItem * pUserItem = SearchUserByUIN(UIN);
	if (pUserItem != NULL)
	{
		if (bAdd)
		{
			pUserItem->AddGiftItemID(unStampID);
		}
		else
		{
			pUserItem->DelGiftItemID(unStampID);
		}
		//pUserItem->SetUserStamp(unStampID);
		UpdateUserIcon(pUserItem);

		//Notify UI
		m_pRoomBusinessSink->OnUpdateUser(pUserItem);
		return true;
	}

	return false;
}

bool UserManager::UpdateUserTitle( uint32 UIN, uint16 unTitle )
{
	if (m_mapAllUserItem.find(UIN) == m_mapAllUserItem.end())
	{
		return false;
	}

	IUserItem * pUserItem = NULL;
	pUserItem = m_mapAllUserItem[UIN];
	if (pUserItem != NULL)
	{
		//从管理列表里删除
		if (pUserItem->GetUserTitleLevel() > core::TITLELEVEL_NONE && unTitle < core::TITLELEVEL_TEMP_MANAGER)
		{
			m_mapAdminUserItem.erase(UIN);
			m_pRoomBusinessSink->OnDeleteAdminUser(pUserItem);
		}
		pUserItem->SetUserTitle(unTitle);
		UpdateUserIcon(pUserItem);

		//添加到管理列表
		if (pUserItem->GetUserTitleLevel() > core::TITLELEVEL_NONE)
		{
			m_mapAdminUserItem[UIN] = pUserItem;
			m_pRoomBusinessSink->OnAddAdminUser(pUserItem);
		}

		//Notify UI
		m_pRoomBusinessSink->OnUpdateUser(pUserItem);
		
		m_pRoomBusinessSink->OnUserListCountChange();
		return true;
	}

	return false;
}

uint32 UserManager::GetAllUserCount()
{
	return m_mapAllUserItem.size() - m_nHideUserCount;
}

uint32 UserManager::GetWaitingCount()
{
	return m_mapWaitingUserItem.size();
}

uint32 UserManager::GetAdminCount()
{
	return m_mapAdminUserItem.size();
}


uint32 UserManager::GetGuestCount()
{
	return m_mapGuestUserItem.size();
}

bool UserManager::UpdateWaitingList( uint32 UIN, bool bAdd )
{
	if (bAdd)
	{
		MapUserItem::iterator iter = m_mapAllUserItem.find(UIN);
		if (iter != m_mapAllUserItem.end())
		{
			IUserItem * pUserItem = iter->second;
			pUserItem->SetWaiting(true);
			m_mapWaitingUserItem[UIN] = iter->second;

			//Notify UI
			m_pRoomBusinessSink->OnAddWaitingUser(iter->second);
			m_pRoomBusinessSink->OnUserListCountChange();
			return true;
		}
		else
		{
			return false;
		}
	}
	else
	{
		MapUserItem::iterator iter = m_mapWaitingUserItem.find(UIN);
		if (iter != m_mapWaitingUserItem.end())
		{
			IUserItem * pUserItem = iter->second;
			pUserItem->SetWaiting(false);
			//Notify UI
			m_pRoomBusinessSink->OnDeleteWaitingUser(iter->second);
			m_mapWaitingUserItem.erase(iter);
			m_pRoomBusinessSink->OnUserListCountChange();
			return true;
		}
		else
		{
			return false;
		}
	}

	return false;
}

IUserItem * UserManager::SearchUserByUIN( uint32 unUIN )
{
	MapUserItem::iterator iter = m_mapAllUserItem.find(unUIN);
	if (iter != m_mapAllUserItem.end())
	{
		return iter->second;
	}
	else
	{
		return NULL;
	}

	return NULL;
}

IUserItem * UserManager::SearchUserBy179ID( uint32 un179ID )
{
	for (MapUserItem::iterator iter = m_mapAllUserItem.begin(); iter != m_mapAllUserItem.end(); ++iter)
	{
		IUserItem * pUserItem = iter->second;
		if (pUserItem->GetUser179ID() == un179ID)
		{
			return pUserItem;
		}
	}

	return NULL;
}

IUserItem * UserManager::SearchUserByName( std::wstring strName )
{
	for (MapUserItem::iterator iter = m_mapAllUserItem.begin(); iter != m_mapAllUserItem.end(); ++iter)
	{
		IUserItem * pUserItem = iter->second;
		if (pUserItem->GetUserName() == strName)
		{
			return pUserItem;
		}
	}

	return NULL;
}

void UserManager::GetGuestList( std::vector<IUserItem*> & vecGuestList )
{
	for (MapUserItem::iterator it = m_mapGuestUserItem.begin(); it != m_mapGuestUserItem.end(); ++it)
	{
		IUserItem * pUserItem = it->second;
		vecGuestList.push_back(pUserItem);
	}
}

IUserItem * UserManager::GetSelfUserItem()
{
	return m_pSelfUserItem;
}

void UserManager::UpdateUserIcon( IUserItem * pUserItem )
{
	for (int i = 0; i < 5; i++)
	{
		pUserItem->SetListIconIndex(i,ListIconManager::GetIconIndexByColumn(pUserItem,i));
	}
}

void UserManager::SmartSearch( ENM_UserListType enmListType, std::wstring strKey, std::vector<IUserItem*> & vecUserItem )
{
	MapUserItem::iterator it;
	switch (enmListType)
	{
	case USERLISTTYPE_ALL:
		{
			for (it = m_mapAllUserItem.begin(); it != m_mapAllUserItem.end(); ++it)
			{
				if (common::utility::stringhelper::IsNumeric(strKey))
				{
					IUserItem * pUserItem = it->second;
					CString strUIN = common::utility::stringhelper::IntToString(pUserItem->GetUser179ID()).c_str();
					if (strUIN.Find(strKey.c_str()) != -1 && (!pUserItem->IsHideLogin() || pUserItem->GetUserUIN() == GetCurInfo().unUIN))
					{
						vecUserItem.push_back((IUserItem*)it->second);
						continue;
					}
				}
				IUserItem * pUserItem = it->second;
				CString strName = pUserItem->GetUserName().c_str();
				if (strName.Find(strKey.c_str()) != -1 && (!pUserItem->IsHideLogin() || pUserItem->GetUserUIN() == GetCurInfo().unUIN))
				{
					vecUserItem.push_back((IUserItem*)pUserItem);
				}
			}
		}
		break;
	case USERLISTTYPE_MANAGER:
		{
			for (it = m_mapAdminUserItem.begin(); it != m_mapAdminUserItem.end(); ++it)
			{
				if (common::utility::stringhelper::IsNumeric(strKey))
				{
					IUserItem * pUserItem = it->second;
					CString strUIN = common::utility::stringhelper::IntToString(pUserItem->GetUser179ID()).c_str();
					if (strUIN.Find(strKey.c_str()) != -1 && (!pUserItem->IsHideLogin() || pUserItem->GetUserUIN() == GetCurInfo().unUIN))
					{
						vecUserItem.push_back((IUserItem*)it->second);
						continue;
					}
				}
				IUserItem * pUserItem = it->second;
				CString strName = pUserItem->GetUserName().c_str();
				if (strName.Find(strKey.c_str()) != -1 && (!pUserItem->IsHideLogin() || pUserItem->GetUserUIN() == GetCurInfo().unUIN))
				{
					vecUserItem.push_back((IUserItem*)pUserItem);
				}
			}
		}
		break;
	case USERLISTTYPE_MIC:
		{
			for (it = m_mapWaitingUserItem.begin(); it != m_mapWaitingUserItem.end(); ++it)
			{
				if (common::utility::stringhelper::IsNumeric(strKey))
				{
					IUserItem * pUserItem = it->second;
					CString strUIN = common::utility::stringhelper::IntToString(pUserItem->GetUser179ID()).c_str();
					if (strUIN.Find(strKey.c_str()) != -1 && (!pUserItem->IsHideLogin() || pUserItem->GetUserUIN() == GetCurInfo().unUIN))
					{
						vecUserItem.push_back((IUserItem*)it->second);
						continue;
					}
				}
				IUserItem * pUserItem = it->second;
				CString strName = pUserItem->GetUserName().c_str();
				if (strName.Find(strKey.c_str()) != -1 && (!pUserItem->IsHideLogin() || pUserItem->GetUserUIN() == GetCurInfo().unUIN))
				{
					vecUserItem.push_back((IUserItem*)pUserItem);
				}
			}
		}
		break;
	case USERLISTTYPE_AGENT:
		{
			for (it = m_mapAgentUserItem.begin(); it != m_mapAgentUserItem.end(); ++it)
			{
				if (common::utility::stringhelper::IsNumeric(strKey))
				{
					IUserItem * pUserItem = it->second;
					CString strUIN = common::utility::stringhelper::IntToString(pUserItem->GetUser179ID()).c_str();
					if (strUIN.Find(strKey.c_str()) != -1 && (!pUserItem->IsHideLogin() || pUserItem->GetUserUIN() == GetCurInfo().unUIN))
					{
						vecUserItem.push_back((IUserItem*)it->second);
						continue;
					}
				}
				IUserItem * pUserItem = it->second;
				CString strName = pUserItem->GetUserName().c_str();
				if (strName.Find(strKey.c_str()) != -1 && (!pUserItem->IsHideLogin() || pUserItem->GetUserUIN() == GetCurInfo().unUIN))
				{
					vecUserItem.push_back((IUserItem*)pUserItem);
				}
			}
		}
		break;
	default:
		break;
	}
}

void UserManager::GetManagerList( std::vector<IUserItem*> & vecManagerList )
{
	for (MapUserItem::iterator it = m_mapAdminUserItem.begin(); it != m_mapAdminUserItem.end(); ++it)
	{
		IUserItem * pUserItem = it->second;
		vecManagerList.push_back(pUserItem);
	}
}

void UserManager::GetAllExceptGuest( std::vector<IUserItem*> & vecAllList )
{
	for (MapUserItem::iterator it = m_mapAllUserItem.begin(); it != m_mapAllUserItem.end(); ++it)
	{
		IUserItem * pUserItem = it->second;
		if (pUserItem->GetUserVipLevel() > core::VIPLEVEL_NONE)
		{
			vecAllList.push_back(pUserItem);
		}
	}
}

void UserManager::GetAllActorExceptSelf(std::vector<IUserItem*>& vecActorList) 
{
	core::CurInfo myCurInfo = GetCurInfo() ;
	for (MapUserItem::iterator it = m_mapAllUserItem.begin() ; it != m_mapAllUserItem.end() ; ++it )
	{
		IUserItem* pUserItem = it->second ;
		if ( pUserItem->IsActor() && myCurInfo.unUIN != pUserItem->GetUserUIN() )
		{
			vecActorList.push_back( pUserItem ) ;
		}
	}
}

bool UserManager::UpdateUserGiftItem( uint32 UIN, uint16 unGiftID, bool bAdd /*= true*/ )
{
	IUserItem * pUserItem = SearchUserByUIN(UIN);
	if (pUserItem != NULL)
	{
		if (bAdd)
		{
			pUserItem->AddGiftItemID(unGiftID);
		}
		else
		{
			pUserItem->DelGiftItemID(unGiftID);
		}
		//pUserItem->SetUserStamp(unStampID);
		UpdateUserIcon(pUserItem);

		//Notify UI
		m_pRoomBusinessSink->OnUpdateUser(pUserItem);
		return true;
	}

	return false;
}

bool UserManager::DeleteAllGuest()
{
	for (MapUserItem::iterator it = m_mapGuestUserItem.begin(); it != m_mapGuestUserItem.end();/* ++it*/)
	{
		MapUserItem::iterator iter = m_mapAllUserItem.find(it->first);
		if (iter != m_mapAllUserItem.end())
		{
			IUserItem * pUserItem = iter->second;

			m_pRoomBusinessSink->OnDeleteUser(pUserItem);

			delete iter->second;
			m_mapAllUserItem.erase(iter);

			it = m_mapGuestUserItem.erase(it);
		}
		else
		{
			++it;
		}	
	}

	m_pRoomBusinessSink->OnUserListCountChange();

	return true;
}

bool UserManager::UpdateUserChangeVoiceItem( uint32 UIN, uint16 unChangeVoiceID, bool bAdd /*= true*/ )
{
	IUserItem * pUserItem = SearchUserByUIN(UIN);
	if (pUserItem != NULL)
	{
		if (bAdd)
		{
			pUserItem->SetUserChangeVoice(unChangeVoiceID);
		}
		else
		{
			pUserItem->SetUserChangeVoice(unChangeVoiceID);
		}

		//UpdateUserIcon(pUserItem);

		//Notify UI
		//m_pRoomBusinessSink->OnUpdateUser(pUserItem);
		return true;
	}

	return false;
}

int UserManager::GetHideUserCount()
{
	return m_nHideUserCount;
}

bool UserManager::UpdateUserVipLevel( uint32 UIN, uint8 unVipLevel )
{
	IUserItem * pUserItem = SearchUserByUIN(UIN);
	if (pUserItem != NULL)
	{
		pUserItem->SetUserVipLevel(unVipLevel);
		UpdateUserIcon(pUserItem);

		//Notify UI
		m_pRoomBusinessSink->OnUpdateUser(pUserItem);
		return true;
	}
	return false;
}

bool UserManager::UpdateUserRichLevel( uint32 UIN, uint8 unRichLevel )
{
	IUserItem * pUserItem = SearchUserByUIN(UIN);
	if (pUserItem != NULL)
	{
		pUserItem->SetUserRichLevel(unRichLevel);
		UpdateUserIcon(pUserItem);

		//Notify UI
		m_pRoomBusinessSink->OnUpdateUser(pUserItem);
		return true;
	}
	return false;
}

void UserManager::GetAllActors( std::vector<IUserItem*>& vecActorList )
{
	for (MapUserItem::iterator it = m_mapAllUserItem.begin() ; it != m_mapAllUserItem.end() ; ++it )
	{
		IUserItem* pUserItem = it->second ;
		if (pUserItem->IsActor() && pUserItem->GetUserTitleLevel()> core::TITLELEVEL_TEMP_MANAGER)  //为艺人且为管理员
		{
			vecActorList.push_back(pUserItem);
		}
	}

}

void UserManager::GetAllAgent( std::vector<IUserItem*> & vecAgentList )
{
	for (MapUserItem::iterator it = m_mapAllUserItem.begin() ; it != m_mapAllUserItem.end() ; ++it )
	{
		IUserItem* pUserItem = it->second ;
		if (pUserItem->IsAgent())
		{
			vecAgentList.push_back(pUserItem);
		}
	}
}