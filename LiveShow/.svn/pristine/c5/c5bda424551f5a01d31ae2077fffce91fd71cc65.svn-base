#pragma once
#include "igiftitem.h"

class GiftItem : public IGiftItem
{
public:
	GiftItem(void);
	virtual ~GiftItem(void);

	virtual uint16 GetID() { return m_GiftInfo.unID; }

	virtual uint16 GetIndex() { return m_GiftInfo.unIndex; }

	virtual std::wstring GetName() { return m_GiftInfo.strName; }

	virtual std::wstring GetRichName() { return m_GiftInfo.strRichName; }

	virtual std::wstring GetDesc() { return m_GiftInfo.strDesc; }

	virtual std::wstring GetUnitName() { return m_GiftInfo.strUnitName; }

	virtual uint32 GetPrice() { return m_GiftInfo.unPrice; }

	virtual uint32 GetDstEarn() { return m_GiftInfo.unDstEarn; }

	virtual std::wstring GetSmallImagePath() { return m_GiftInfo.strSmallImagePath; }

	virtual std::wstring GetLargeImagePath() { return m_GiftInfo.strLargeImagePath; }

	virtual uint8 GetGiftType() { return m_GiftInfo.unGiftType; }

	virtual uint8 GetSendType() { return m_GiftInfo.unSendType; }

	virtual void SetGiftInfo(room::gift_info & giftinfo);

	virtual bool IsNeedTempNotice() { return m_GiftInfo.bTempNotice; }		//是否有发到临时公告的链接

	virtual bool IsNeedGiftWords() { return m_GiftInfo.bGiftWords; }

	virtual std::wstring GetCatalogName() { return m_GiftInfo.strCatalog; }

	virtual std::wstring GetStrType() { return m_GiftInfo.strType; }

	virtual uint8 GetDstType() { return m_GiftInfo.unDstType;}

	virtual std::wstring GetStrSendBtn() { return m_GiftInfo.strSendBtn; }

	virtual std::wstring GetStrPrescription() { return m_GiftInfo.strPrescription; }

	virtual std::wstring GetStrSendDesc() { return m_GiftInfo.strSendDesc; }

	virtual std::wstring GetStrGiftWords() { return m_GiftInfo.strGiftWords; }

	virtual CTime GetBeginTime() { return m_GiftInfo.tmBeginTime; }

	virtual CTime GetEndTime() { return m_GiftInfo.tmEndTime; }

	virtual bool IsInUse() { return m_GiftInfo.bInUse; }

public:
	room::gift_info m_GiftInfo;
};
