#pragma once
#include "..\gift\GiftManager.h"
#include "PngDlg.h"
#include "..\flash\flashWnd.h"
//#include "GlobalRoomDefine.h"

class CCombineGiftDlg;
class CFlyingTextDlg;
class CRoomParentDlg;
class CFlyLuxuryWnd;
struct combinegift_info
{
	int nGiftType;						//数量类型，520等
	int nWidth;							//宽度
	int nHeight;						//高度
	std::wstring strBackImage;			//背景图片
	std::wstring strCardImage;			//卡片图片
	std::wstring strUnitImage;			//单元图片，一般不用
	std::wstring strFrontImage;			//前景图片
	int nInterval;						//间隔时间，单位毫秒
	int nFrames;						//帧数
	int nFreezeTime;					//停留时间
	std::vector<CPoint> vecPos;			//坐标点
	CPoint backPos;						//背景位置
	CPoint frontPos;					//前景位置
	CPoint cardPos;						//卡片位置
	CPoint srcNamePos;					//赠送者名字位置
	CPoint dstNamePos;					//接受者名字位置
	CPoint unitPos;						//礼物单位位置
	CPoint giftNamePos;					//礼物名称位置

	combinegift_info() : nWidth(0), nHeight(0)
	{

	}

	~combinegift_info()
	{
		vecPos.clear();
	}
};

struct flash_info 
{
	int nType;
	int nWidth;
	int nHeight;
	int nInterval;
	int nPlayTime;
	std::wstring strFlashPath;
};

enum ENM_Effect_Type
{
	EFFECT_TYPE_COMBINEGIFT		= 1,		//组合礼物
	EFFECT_TYPE_FIREWORK		= 2,		//烟花
	EFFECT_TYPE_SALUTE			= 3,		//礼炮
	EFFECT_TYPE_PLUTUS			= 4,		//财神
	EFFECT_TYPE_FLYINGTEXT		= 5,		//飞屏
	EFFECT_TYPE_500_IN_ROOM		= 6,		//500倍自己房间
	EFFECT_TYPE_500_OTHER_ROOM	= 7,		//500倍其他房间
	EFFECT_TYPE_1000_IN_ROOM	= 8,		//1000倍自己房间
	EFFECT_TYPE_1000_OTHER_ROOM = 9,		//1000倍其他房间
	EFFECT_TYPE_5000_IN_ROOM	= 10,		//5000倍自己房间
	EFFECT_TYPE_5000_OTHER_ROOM = 11,		//5000倍其他房间
	EFFECT_TYPE_LUCKY_STAR		= 12,		//超级幸运星
	EFFECT_TYPE_WEALTH_THRONE	= 13,		//财富宝座
	EFFECT_TYPE_VIP_TOP			= 14,		//至尊皇冠
	EFFECT_TYPE_ADD_MONEY		= 15,		//加币效果
	EFFECT_TYPE_GUIDE			= 16,		//新手引导
	EFFECT_TYPE_CHRISTMAS		= 17,		//圣诞
	EFFECT_TYPE_SOFA			= 18,		//沙发
	EFFECT_TYPE_SOFACHAT		= 19,		//沙发聊天
	EFFECT_TYPE_ORDERSONG       = 20,       //主播接受点歌
};

struct Effect_Info
{
	ENM_Effect_Type enmType;
	uint16 unGiftID;
	int nGiftCount;
	std::wstring strSrcName;
	uint32 unSrc179ID;
	std::wstring strDstName;
	uint32 unDst179ID;
	std::wstring strFlyingText;
	int nFlyingTextType;

	Effect_Info() : unGiftID(0), nGiftCount(0), strSrcName(L""), strDstName(L""), strFlyingText(L""), nFlyingTextType(0)
	{
	}
};

struct OrderSong_Info : public Effect_Info
{
	//基类的strFlyingText用做歌曲名
	std::wstring strSongWish;//点歌祝福语

	OrderSong_Info() : strSongWish(L"")
	{
	}
};

struct Luxury_Info
{
	std::wstring strSrcName;
	std::wstring strDstName;
	uint32 unSrc179ID;
	uint32 unDst179ID;
	uint16 unGiftID;
	uint16 unCount;
	std::wstring strTime;

	Luxury_Info() : strSrcName(L""), strDstName(L""), unSrc179ID(0), unDst179ID(0), unGiftID(0), unCount(0), strTime(L"")
	{
	}
};

struct flying_text 
{
	std::wstring strFontFamily;			//字体
	int nFontSize;						//字大小
	DWORD dwBeginColor;					//渐变开始颜色
	DWORD dwEndColor;					//渐变结束颜色
	DWORD dwStrokeColor;				//描边颜色
	DWORD dwShadowColor;				//阴影颜色
	bool bBold;							//是否粗体
	bool bItalic;						//是否斜体
	bool bUnderline;					//是否下划线
	bool bStroke;						//是否描边
	bool bShadow;						//是否阴影
	int nGradientMode;					//渐变方式

	flying_text() : strFontFamily(L"宋体"), nFontSize(20), dwBeginColor(0), dwEndColor(0), dwStrokeColor(0), bBold(true), bItalic(true), bUnderline(true), bStroke(false), nGradientMode(1), dwShadowColor(0), bShadow(false)
	{

	}
};

struct flying_text_info 
{
	int nFlyingType;
	flying_text stNickName;
	flying_text stSay;
	flying_text stContent;
	std::vector<std::wstring> vecStrImagePath;

	~flying_text_info()
	{
		vecStrImagePath.clear();
	}
};

enum ENM_FlyingText_Type
{
	FLYING_TEXT_TYPE_1 = 1,
	FLYING_TEXT_TYPE_2 = 2,
	FLYING_TEXT_TYPE_3 = 3,
	FLYING_TEXT_TYPE_4 = 4,
	FLYING_TEXT_TYPE_5 = 5,
};

struct flying_window_info
{
	int nPosX;
	int nPosY;
	bool bInUse;
	int nWidth;
	int nHeight;
	bool bFlyText;
	CFlyingTextDlg * pFlyingTextDlg;
	CFlyLuxuryWnd * pFlyLuxuryWnd;

	flying_window_info() : nPosX(0), nPosY(0), bInUse(false), nWidth(0), nHeight(0), bFlyText(true), pFlyingTextDlg(NULL), pFlyLuxuryWnd(NULL)
	{

	}
};

class ScreenEffectManager
{
public:
	ScreenEffectManager(CRoomParentDlg * pRoomParentDlg);
	~ScreenEffectManager(void);

	//static ScreenEffectManager * GetInstance();
	//static void DeleteInstance();

	static void Init();
	static void Release();
	static bool LoadCombineGiftConfig();
	static bool LoadFlyingTextConfig();
	static bool LoadFlashRes();
	void ProcessSendGift(int nCount, uint16 unGiftID, std::wstring strSrcName, uint32 unSrc179ID, std::wstring strDstName, uint32 unDst179ID);				//处理礼物赠送消息
	void ProcessSendPlutus();																							//处理财神消息
	void ProcessFlyingText(std::wstring strSrcName, std::wstring strText, int nType);									//处理飞屏消息
	void ProcessLucky(uint32 unSrc179ID, uint16 unLuckyTimes, std::wstring strUserName, std::wstring strRoomName, bool bInRoom);			//处理中奖消息
	void ProcessLuckyStar(std::wstring strUserName, uint32 unSrc179ID);												//处理幸运星
	void ProcessVipTop(std::wstring strUserName, uint32 unSrc179ID);												//处理至尊皇冠
	void ProcessFlyLuxury(CString strSrcName , CString strDstName , uint32 unSrc179ID , 
		uint32 unDst179ID ,  uint16 unGiftID ,uint16 unCount);															//处理飞屏奢侈礼物
	void ProcessOrderSong(std::wstring strSrcName, std::wstring strDstName, uint32 unSrc179ID, uint32 unDst179ID, std::wstring strSongName, std::wstring strWish);

	static flying_text_info * GetFlyingInfo(ENM_FlyingText_Type enmType);
	static flash_info * GetFlashInfo(ENM_Effect_Type enmType);
	static bool EncodeFlyingText(std::wstring & strText, int nType);
	static bool DecodeFlyingText(std::wstring & strText, int & nType);

	void OnCycleTrigger();
	void OnMoving(LPRECT pRect);
	void OnSize(UINT nType, int cx, int cy);

	void StopAllFlash();

protected:
	void ProcessCombineGift(int nCount, uint16 unGiftID, std::wstring strSrcName, std::wstring strDstName);				//处理组合礼物
	void ProcessSendFireWork();																							//处理烟花消息
	void ProcessSendSalute();																							//处理礼炮消息	

	void ShowCombineGift(int nType, IGiftItem * pGiftItem, std::wstring strSrcName, std::wstring strDstName);			//组合礼物
	void ShowFirework();																								//烟花
	void ShowSalute();																									//礼炮
	void ShowPlutus();																									//财神
	void ShowFlyingText(std::wstring strNickName, std::wstring strText, ENM_FlyingText_Type enmType, int nIndex);		//飞屏
	void ShowLuckyEffect(ENM_Effect_Type enmType, std::wstring strUserName, std::wstring strRoomName);
	void ShowEnterEffect(ENM_Effect_Type enmType, std::wstring strUserName, std::wstring str179ID);
	void ShowFlyLuxury(CString strSrcName , CString strDstName , uint32 unSrc179ID , 
		uint32 unDst179ID ,  uint16 unGiftID ,uint16 unCount ,int nIndex);
	void ShowChristmas(int nType, IGiftItem * pGiftItem, std::wstring strSrcName, uint32 unSrc179ID, std::wstring strDstName, uint32 unDst179ID);
	void ShowOrderSong(std::wstring strSrcName, std::wstring strDstName, uint32 unSrc179ID, uint32 unDst179ID, std::wstring strSongName, std::wstring strWish);

protected:
	static void ParseSinglePos(CString strPos, CPoint & point);															//解析单个坐标字符串
	static void ParsePos(CString strPos, std::vector<CPoint>& vecPos);													//解析多个坐标字符串
	static DWORD ParseStringColor(std::wstring strColor);																//解析颜色字符串

	void CreateFlyingTextWindowInfo();
	int FindFreeFlyingTextWindowIndex();

private:
	static std::map<int, combinegift_info*> s_mapGiftConfig;
	static std::map<int, flying_text_info*> s_mapFlyingConfig;
	static std::map<int, flash_info*> s_mapFlashConfig;

	CCombineGiftDlg * m_pCurCombineGiftDlg;
	CFlashWnd * m_pFireworkWnd;
	CFlashWnd * m_pSaluteWnd;
	CFlashWnd * m_pPlutusWnd;
	CFlashWnd * m_pLuckyWnd;
	CFlashWnd * m_pEnterEffectWnd;
	CFlashWnd * m_pChristmasWnd;
	CFlyLuxuryWnd * m_pFlyLuxuryWnd;
	CFlashWnd * m_pOrderSongWnd;

	bool m_bCombinGift;
	bool m_bFirework;
	bool m_bSalute;
	bool m_bPlutus;
	bool m_bFlyingText;
	bool m_bLuckyEffect;
	bool m_bEnterEffect;
	bool m_bChristmas;
	bool m_bOrderSong;

	std::list<Effect_Info*> m_listCombineGift;
	std::list<Effect_Info*> m_listFirework;
	std::list<Effect_Info*> m_listSalute;
	std::list<Effect_Info*> m_listPlutus;
	std::list<Effect_Info*> m_listFlyingText;
	std::list<Effect_Info*> m_listLucky;
	std::list<Effect_Info*> m_listEnterEffect;
	std::list<Luxury_Info*> m_listFlyLuxury;
	std::list<Effect_Info*> m_listChristmas;
	std::list<OrderSong_Info*> m_listOrderSong;

	flying_window_info m_FlyTextWindowInfo[6];
	ENM_Effect_Type m_enmCurLuckyEffect;
	ENM_Effect_Type m_enmCurEnterEffect;

	int m_nFlyLuxuryCount;
	//bool m_bFlyingTextWindowFull;

	CRoomParentDlg * m_pRoomParentDlg;

protected:
	//static ScreenEffectManager * s_pScreenEffectManager;
};
