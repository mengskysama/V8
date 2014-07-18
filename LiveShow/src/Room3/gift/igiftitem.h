#pragma  once
#include "coredefine.h"

namespace room
{
	enum ENM_GIFT_SENDTYPE
	{
		SENDTYPE_NONE			= 0x00,	//需要点击弹出新窗口操作
		SENDTYPE_OTHERSINGLE	= 0x01,	//对单个其他人使用
		SENDTYPE_OTHERALL		= 0x02,	//对房间其他多人使用（所有人或者所有管理员）
		SENDTYPE_SELF			= 0x03,	//只能对自己使用
	};

	struct gift_info
	{
		uint16 unID;
		uint16 unIndex;
		std::wstring strName;
		std::wstring strRichName;		//完整礼物名，奢侈礼物中用到
		std::wstring strDesc;
		std::wstring strUnitName;
		uint32 unPrice;
		uint32 unDstEarn;
		std::wstring strSmallImagePath;
		std::wstring strLargeImagePath;
		uint8 unGiftType;				//ENM_GiftType
		uint8 unSendType;				//ENM_GIFT_SENDTYPE
		bool bTempNotice;
		bool bGiftWords;
		uint8 unDstType;				//ENM_GiftDstType
		std::wstring strCatalog;
		std::wstring strType;			//配置中的类型字符串，一般不用
		std::wstring strSendBtn;		//Tip上的使用按钮文字
		std::wstring strPrescription;	//时效描述
		std::wstring strSendDesc;		//赠送描述
		std::wstring strGiftWords;
		CTime tmBeginTime;				//有效期开始时间
		CTime tmEndTime;				//有效期结束时间
		bool bInUse;					//是否上架

		gift_info() : strSendBtn(L"点此赠送"), strPrescription(L""), strSendDesc(L"赠送"), bGiftWords(false), strGiftWords(L""), tmBeginTime(1970,1,1,12,0,0),tmEndTime(2100,1,1,0,0,0),bInUse(true)
		{

		}
	};
}

struct IGiftItem
{
	virtual uint16 GetID()							= 0;
	virtual uint16 GetIndex()						= 0;
	virtual std::wstring GetName()					= 0;
	virtual std::wstring GetRichName()				= 0;
	virtual std::wstring GetDesc()					= 0;
	virtual std::wstring GetUnitName()				= 0;
	virtual uint32 GetPrice()						= 0;
	virtual uint32 GetDstEarn()						= 0;
	virtual std::wstring GetSmallImagePath()		= 0;
	virtual std::wstring GetLargeImagePath()		= 0;
	virtual uint8 GetGiftType()						= 0;
	virtual uint8 GetSendType()						= 0;
	virtual void SetGiftInfo(room::gift_info & giftinfo)	= 0;
	virtual bool IsNeedTempNotice()					= 0;
	virtual bool IsNeedGiftWords()					= 0;
	virtual std::wstring GetCatalogName()			= 0;
	virtual std::wstring GetStrType()				= 0;
	virtual uint8 GetDstType()						= 0;
	virtual std::wstring GetStrSendBtn()			= 0;
	virtual std::wstring GetStrPrescription()		= 0;
	virtual std::wstring GetStrSendDesc()			= 0;
	virtual std::wstring GetStrGiftWords()			= 0;
	virtual CTime GetBeginTime()					= 0;
	virtual CTime GetEndTime()						= 0;
	virtual bool IsInUse()							= 0;
};