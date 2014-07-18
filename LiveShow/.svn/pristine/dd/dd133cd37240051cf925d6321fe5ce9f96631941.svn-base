#pragma once
#include <string>
#include <map>
#include "51ktvtype.h"
#include "..\usermanager\UserItem.h"

enum ENM_Marker_Type
{
	MARKER_TYPE_OWNER			= 1,		//室主
	MARKER_TYPE_ACTOR			= 2,		//主播
	MARKER_TYPE_ORDER			= 3,		//点歌者
	MARKER_TYPE_HAVE_TICKET		= 4,		//有票大众评委
	MARKER_TYPE_NO_TICKET		= 5,		//无票大众评委
};

enum ENM_Mark_Step
{
	MARK_STEP_NO_MARK			= 0,		//空闲阶段
	MARK_STEP_GET_TICKET		= 1,		//抢票阶段
	MARK_STEP_GET_RESULT		= 2,		//抢票结果
	MARK_STEP_ENJOY_TIME		= 3,		//欣赏阶段
	MARK_STEP_MARK_TIME			= 4,		//投票阶段
	MARK_STEP_MRAK_RESULT		= 5,		//投票结果
	MARK_STEP_FINAL_RESULT		= 6,		//最终结果
};

struct marker_info
{
	ENM_Marker_Type enmType;
	ENM_Mark_Step enmStep;
	std::wstring str1;
	std::wstring str2;
	std::wstring str3;

	marker_info() : enmType(MARKER_TYPE_NO_TICKET), enmStep(MARK_STEP_NO_MARK), str1(L""), str2(L""), str3(L"")
	{

	}
};

class CSongMarkManager
{
public:
	CSongMarkManager(void);
	~CSongMarkManager(void);

public:
	static bool LoadMarkerConfig();
	static void Release();
	
	static void GetMarkerInfo(ENM_Marker_Type enmType, ENM_Mark_Step enmStep, marker_info & stMarkerInfo);
	static uint8 GetUnitTicket();

	void SetCurMarkInfo(uint32 unOrderID, uint32 unSongerID, std::wstring strSongerName, uint32 unOrdererID, std::wstring strSong);
	ENM_Marker_Type CheckMarkerType(IUserItem * pUserItem, bool bHaveTicket);			//判断投票用户类型

	void SetLeftTicket(uint32 unLeftTicket) { m_unLeftTicket = unLeftTicket; }
	uint32 GetLeftTicket() { return m_unLeftTicket; }
	void SetLastGetTicket(bool bLast) { m_bLastGetTicket = bLast; }
	bool IsLastGetTicket() { return m_bLastGetTicket; }
	void SetGetTicketFail(bool bFail) { m_bGetTicketFail = bFail; }
	bool IsGetTicketFail() { return m_bGetTicketFail; }

	void Reset();
private:
	static std::map<ENM_Marker_Type, std::map<ENM_Mark_Step, marker_info>> m_mapMarkerInfo;

	//uint32 m_unRoomOwnerID;				//当前室主ID
	static uint8 m_unUnitTicket;		//点歌者超级票基数
	uint32 m_unSongOrderID;				//当前订单号
	uint32 m_unCurSongerID;				//当前歌手ID
	uint32 m_unCurOrdererID;			//当前点歌者ID
	std::wstring m_strSongerName;		//当前歌手名
	std::wstring m_strCurSong;			//当前歌曲
	uint32 m_unLeftTicket;				//当前剩余票数
	bool m_bLastGetTicket;				//是否最后一个取得票的
	bool m_bGetTicketFail;				//是否是抢票失败的
	ENM_Marker_Type m_curMarkerType;	//用户当前的投票类型
};
