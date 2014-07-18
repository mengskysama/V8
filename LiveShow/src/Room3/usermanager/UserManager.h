#pragma once
//#include "iusermanager.h"
#include "iusermanagersink.h"
#include "UserItem.h"
#include "..\Room.h"

enum ENM_UserListType
{
	USERLISTTYPE_ALL		= 1,
	USERLISTTYPE_MANAGER	= 2,
	USERLISTTYPE_MIC		= 3,
	USERLISTTYPE_AGENT		= 4,
};

typedef std::map<uint32, IUserItem*> MapUserItem;

class UserManager/* :	public IUserManager*/
{
public:
	UserManager(uint32 roomid, IUserManagerSink * pRoomBusinessSink);
	~UserManager(void);

	//添加麦序列表
	virtual void GetWaitingList(std::vector<uint32> & vecWaitinglist);
	//添加用户
	virtual IUserItem * AddUserItem(core::room_user_info & UserInfo);
	//删除用户
	virtual bool DeleteUserItem(uint32 UIN);
	//删除用户
	virtual bool DeleteUserItem(IUserItem * pUserItem);
	//批量删除用户
	virtual void DeleteUserItem(std::vector<uint32> & vecDelList);
	//删除所有游客
	virtual bool DeleteAllGuest();
	//获得所有用户数量
	virtual uint32 GetAllUserCount();
	//获得麦序列表用户数量
	virtual uint32 GetWaitingCount();
	//获得管理列表用户数量
	virtual uint32 GetAdminCount();
	//获得游客数量
	virtual uint32 GetGuestCount();
	//更新用户状态
	virtual bool UpdateUserStatus(uint32 UIN, uint16 unStatus, bool bAdd);
	//更新用户管理头衔
	virtual bool UpdateUserTitle(uint32 UIN, uint16 unTitle);
	//更新用户vip等级
	virtual bool UpdateUserVipLevel(uint32 UIN, uint8 unVipLevel);
	//更新用户富豪等级
	virtual bool UpdateUserRichLevel(uint32 UIN, uint8 unRichLevel);
	//更新用户印章
	virtual bool UpdateUserStamp(uint32 UIN, uint16 unStampID, bool bAdd = true);
	//更新用户道具信息
	virtual bool UpdateUserGiftItem(uint32 UIN, uint16 unGiftID, bool bAdd = true);
	//更新用户变声卡道具
	virtual bool UpdateUserChangeVoiceItem(uint32 UIN, uint16 unChangeVoiceID, bool bAdd = true);
	//更新麦序列表
	virtual bool UpdateWaitingList(uint32 UIN, bool bAdd);
	//搜索用户
	virtual IUserItem * SearchUserByUIN(uint32 unUIN);
	//搜索用户
	virtual IUserItem * SearchUserBy179ID(uint32 un179ID);
	//搜索用户
	virtual IUserItem * SearchUserByName(std::wstring strName);
	//获得自己用户信息
	virtual IUserItem * GetSelfUserItem();
	//智能搜索，用于用户列表的搜索功能
	virtual void SmartSearch(ENM_UserListType enmListType, std::wstring strKey, std::vector<IUserItem*> & vecUserItem);
	//获得游客列表
	virtual void GetGuestList(std::vector<IUserItem*> & vecGuestList);
	//获得管理列表
	virtual void GetManagerList(std::vector<IUserItem*> & vecManagerList);
	//获得除了游客外其他所有人
	virtual void GetAllExceptGuest(std::vector<IUserItem*> & vecAllList);
	//获得所有艺人
	virtual void GetAllActorExceptSelf(std::vector<IUserItem*>& vecActorList) ;
	//获得所有艺人包括自己
	virtual void GetAllActors(std::vector<IUserItem*>& vecActorList);
	//获得所有销售
	virtual void GetAllAgent(std::vector<IUserItem*> & vecAgentList);

	virtual int GetHideUserCount();

private:
	void UpdateUserIcon(IUserItem * pUserItem);

protected:
	uint32 m_roomid;
	std::vector<core::room_user_info> m_vecRawUserList;
	MapUserItem m_mapAllUserItem;
	MapUserItem m_mapAdminUserItem;
	MapUserItem m_mapWaitingUserItem;
	MapUserItem m_mapGuestUserItem;
	MapUserItem m_mapAgentUserItem;
	IUserManagerSink * m_pRoomBusinessSink;
	IUserItem * m_pSelfUserItem;
	int m_nHideUserCount;
};
