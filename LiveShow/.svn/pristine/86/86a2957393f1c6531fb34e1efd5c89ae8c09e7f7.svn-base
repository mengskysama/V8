#pragma  once
#include "UserItem.h"

struct IUserManagerSink
{
	virtual void OnGetUserList(std::map<uint32, IUserItem*> & mapAllUser)		= 0;
	virtual void OnGetAdminList(std::map<uint32, IUserItem*> & mapAdminUser)		= 0;
	virtual void OnGetWaitingList(std::map<uint32, IUserItem*> & mapWaitingUser) = 0;
	virtual void OnAddUser(IUserItem * pUserItem)								= 0;
	virtual void OnUpdateUser(IUserItem * pUserItem)							= 0;
	virtual void OnDeleteUser(IUserItem * pUserItem)							= 0;
	virtual void OnAddAdminUser(IUserItem * pUserItem)							= 0;
	virtual void OnDeleteAdminUser(IUserItem * pUserItem)						= 0;
	virtual void OnAddWaitingUser(IUserItem * pUserItem)						= 0;
	virtual void OnDeleteWaitingUser(IUserItem * pUserItem)						= 0;
	virtual void OnUserListCountChange()										= 0;
};