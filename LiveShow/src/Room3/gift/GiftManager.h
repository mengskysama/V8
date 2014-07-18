#pragma once
#include "GiftItem.h"

typedef std::vector<IGiftItem*> VecGiftItem;
typedef std::map<uint16, GiftItem*> MapGiftItem;

#define SPECIAL_GIFTNAME_FIREWORK	L"—Ãª®"
#define SPECIAL_GIFTNAME_SALUTE		L"¿Ò≈⁄"
#define SPECIAL_GIFTNAME_BLINK		L"…¡¡¡µ«≥°"

class GiftManager
{
public:
	GiftManager(void);
	~GiftManager(void);

	static GiftManager * GetInstance();
	static void DeleteInstance();

	bool LoadGiftList();
	void Release();
	bool ReloadGiftList();
	IGiftItem * SearchGiftByID(uint16 unGiftID);
	IGiftItem * SearchStampByID(uint16 unGiftID);
	IGiftItem * SearchBlinkEnterByID(uint16 unGiftID);
	std::map<int ,std::wstring>& GetStampTypeList();
	std::vector<std::wstring>& GetGiftCatalogList();
	bool IsStamp(uint16 unGiftID);
	bool IsFirework(uint16 unGiftID);
	bool IsSalute(uint16 unGiftID);
	void GetGiftListByCatalog(std::wstring strCatalog, VecGiftItem & vecGiftList);
	void GetStampListByCatalog(std::wstring strCatalog, VecGiftItem & vecStampList);
	void GetChangeVoiceList(VecGiftItem & vecChangeVoiceList);
	IGiftItem * GetGiftItemByName(std::wstring strName);
	IGiftItem * GetWishSongItem();
	IGiftItem * GetFlyingWordsItem();
	VecGiftItem& GetPropsList();
	VecGiftItem& GetSpeakerList();
	bool IsChangeVoice(uint16 unGiftID);

protected:
	MapGiftItem m_mapAllGiftItem;
	MapGiftItem m_mapStampItem;
	MapGiftItem m_mapChangeVoiceItem;
	MapGiftItem m_mapBlinkEnterItem;
	VecGiftItem m_vecSpeakerItem;
	std::map<int, std::wstring> m_mapStampTypeList;
	std::vector<std::wstring> m_vecGiftCataList;
	//IGiftItem * m_pBlinkEnterItem;
	IGiftItem * m_pWishSongItem;
	IGiftItem * m_pFlyingWordsItem;
	VecGiftItem m_vecPropsList;

protected:
	static GiftManager * s_pGiftManager;
};
