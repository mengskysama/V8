#pragma once
#include "..\usermanager\useritem.h"

enum ENM_UserListIcon
{
	USERLIST_ICON_NONE						= 0,	//没有
	USERLIST_ICON_SHUTUP					= 1,	//禁言
	USERLIST_ICON_VIP_NONE					= 2,	//vip0
	USERLIST_ICON_VIP_RED					= 3,	//红vip
	USERLIST_ICON_VIP_RED_STATIC			= 4,	//红vip
	USERLIST_ICON_VIP_PURPLE				= 5,	//紫vip
	USERLIST_ICON_VIP_PURPLE_STATIC			= 6,	//紫vip
	USERLIST_ICON_VIP_REDKING				= 7,	//红冠
	USERLIST_ICON_VIP_REDKING_STATIC		= 8,	//红冠
	USERLIST_ICON_VIP_PURPLEKING			= 9,	//紫冠
	USERLIST_ICON_VIP_PURPLEKING_STATIC		= 10,	//紫冠
	USERLIST_ICON_VIP_SUPER					= 11,	//超冠
	USERLIST_ICON_VIP_SUPER_STATIC			= 12,	//超冠
	USERLIST_ICON_VIP_DIAMOND				= 13,	//钻冠
	USERLIST_ICON_VIP_DIAMOND_STATIC		= 14,	//钻冠
	USERLIST_ICON_VIP_BLACKDIAMOND			= 15,	//黑冠
	USERLIST_ICON_VIP_BLACKDIAMOND_STATIC	= 16,	//黑冠
	USERLIST_ICON_VIP_TOP					= 17,	//至冠
	USERLIST_ICON_VIP_TOP_STATIC			= 18,	//至冠
	USERLIST_ICON_PUB_MIC					= 19,	//公麦
	USERLIST_ICON_PRI_MIC					= 20,	//私麦
	USERLIST_ICON_P2P_MIC					= 21,	//一对一
	USERLIST_ICON_WAIT_MIC					= 22,	//排麦
	USERLIST_ICON_NORMAL_MIC				= 23,	//普通视频
	USERLIST_ICON_MALE_OWNER				= 24,	//男室主
	USERLIST_ICON_FEMALE_OWNER				= 25,	//女室主
	USERLIST_ICON_MALE_VICEOWNER			= 26,	//男副室主
	USERLIST_ICON_FEMALE_VICEOWNER			= 27,	//女副室主
	USERLIST_ICON_MALE_MANAGER				= 28,	//男管理员
	USERLIST_ICON_FEMALE_MANAGER			= 29,	//女管理员
	USERLIST_ICON_MALE_TEMPMANAGER			= 30,	//男临管
	USERLIST_ICON_FEMALE_TEMPMANAGER		= 31,	//女临管
	USERLIST_ICON_SINGER					= 32,	//人气歌手
	USERLIST_ICON_DANCER					= 33,	//热舞宝贝
	USERLIST_ICON_HOST						= 34,	//闪亮主播
	USERLIST_ICON_OPERATOR					= 35,	//线上运营
	USERLIST_ICON_AGENT						= 36,	//官方代理
	USERLIST_ICON_PATROLMAN					= 37,	//巡管
	USERLIST_ICON_ADCLEANER					= 38,	//广告处理
	USERLIST_ICON_PHOTO						= 39,	//查看照片
	USERLIST_ICON_USERLEVEL_MALE			= 40,	//用户男
	USERLIST_ICON_USERLEVEL_FEMALE			= 41,	//用户女
	USERLIST_ICON_USERLEVEL_ONE				= 42,	//用户等级
	USERLIST_ICON_USERLEVEL_TWO				= 43,	//用户等级
	USERLIST_ICON_USERLEVEL_THREE			= 44,	//用户等级
	USERLIST_ICON_USERLEVEL_FOUR			= 45,	//用户等级
	USERLIST_ICON_GUEST						= 46,	//游客
	USERLIST_ICON_GUEST_SHOW				= 47,	//展开
	USERLIST_ICON_GUEST_HIDE				= 48,	//收起
	USERLIST_ICON_WISH						= 49,	//祝福
	USERLIST_ICON_SMALLSPEAKER				= 50,	//小喇叭
	USERLIST_ICON_BIGSPEAKER				= 51,	//大喇叭
	USERLIST_ICON_SYSTEMMSG					= 52,	//系统消息
	USERLIST_ICON_FIREWORK					= 53,	//烟花
	USERLIST_ICON_SALUTE					= 54,	//礼炮
};

struct ListIconInfo 
{
	ENM_UserListIcon enmIconIndex;
	std::wstring strPath;
	std::wstring strTip;
};

typedef std::map<uint32, ListIconInfo*> MapIconList;

class ListIconManager
{
public:
	ListIconManager(void);
	~ListIconManager(void);

	static bool LoadListIcon();
	static void Release();
	static uint32 GetFirstColumnIconIndex(IUserItem * pUserItem);
	static uint32 GetSecondColumnIconIndex(IUserItem * pUserItem);
	static uint32 GetThirdColumnIconIndex(IUserItem * pUserItem);
	static uint32 GetFourthColumnIconIndex(IUserItem * pUserItem);
	static std::wstring GetVIPIcon(IUserItem * pUserItem);
	static std::wstring GetVIPIconStatic(IUserItem * pUserItem);
	static std::wstring GetFirstColumnIcon(IUserItem * pUserItem);
	static std::wstring GetSecondColumnIcon(IUserItem * pUserItem);
	static std::wstring GetThirdColumnIcon(IUserItem * pUserItem);
	static std::wstring GetFourthColumnIcon(IUserItem * pUserItem);
	static std::wstring GetIconPathByIndex(uint32 unIndex);
	static std::wstring GetIconPathByVipLevel(uint8 unVipLevel);
	static std::wstring GetIconTipByIndex(uint32 unIndex);
	static std::wstring GetIconTipByVipLevel(uint8 unVipLevel);
	static std::wstring GetIconTipByUser(IUserItem * pUserItem);
	static uint32 GetIconIndexByColumn(IUserItem * pUserItem, uint32 unColumn);
	static void GetAllIcon(std::map<int, std::wstring> & mapIconPath);
	static std::wstring GetOutputIcon( IUserItem * pUserItem ) ;
protected:
	static MapIconList g_mapIconList;
};
