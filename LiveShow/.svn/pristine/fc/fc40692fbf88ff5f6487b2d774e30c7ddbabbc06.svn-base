#pragma once
#include "coredefine.h"
#include "..\usermanager\useritem.h"

enum ENM_OnSofaType
{
	ONSOFATYPE_EMPTY	= 1,		//上空沙发
	ONSOFATYPE_KICK		= 2,		//踢人上沙发
	ONSOFATYPE_SELF		= 3,		//自己		
	ONSOFATYPE_UPDATE	= 4,		//更新用户信息
};

struct sofa_info
{
	uint8 unIndex;
	uint32 unPrice;
	uint32 unUIN;
	uint32 un179ID;
	std::wstring strUserName;
	bool bMan;
	uint8 unVipLevel;
	uint8 unRichLevel;
	std::wstring strHeadPath;
	ENM_OnSofaType enmType;

	uint32 unLastUIN;
	uint32 unLast179ID;
	std::wstring strLastUserName;
	uint8 unLastVipLevel;
	uint8 unLastRichLevel;

	sofa_info() : unIndex(0), unPrice(0), unUIN(0), un179ID(0), strUserName(L""), bMan(true), unVipLevel(0), unRichLevel(0), strHeadPath(L""), enmType(ONSOFATYPE_EMPTY), unLastUIN(0), unLast179ID(0), strLastUserName(L""), unLastVipLevel(0), unLastRichLevel(0)
	{

	}
};

class CSofaItem
{
public:
	CSofaItem(uint8 unIndex);
	~CSofaItem(void);

	void SetSofaInfo(uint8 unIndex, core::GetSofaListRspEvent::SofaInfo sofainfo);
	//void UpdateSofaInfo(core::GetSofaListRspEvent::SofaInfo sofainfo);
	void UpdateSofaInfo(uint32 unPrice, IUserItem * pUserItem);
	void UpdateSofaInfo(IUserItem * pUserItem);
	void UpdateHead();
	void UpdateVipLevel(uint8 unVipLevel);
	void UpdateRichLevel(uint8 unRichLevel);
	CString GetStringParam(bool bJustUpdate = false);			//传递给flash的字符串参数
	ENM_OnSofaType GetOnSofaType();
	uint32 GetUserUIN();
	uint32 GetLastUserUIN();
	uint8 GetIndex();
	uint32 GetPrice();
	uint8 GetLastVipLevel();
	uint8 GetLastRichLevel();
	uint32 GetLast179ID();
	std::wstring GetLastUserName();

protected:
	sofa_info m_SofaInfo;
};
