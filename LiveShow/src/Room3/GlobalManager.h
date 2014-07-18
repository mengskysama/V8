#pragma once
#include <string>
#include <vector>
#include <map>
using namespace std;
#include "coredefine.h"
#include "..\CommonLibrary\include\ui\ImageEx.h"
class EmotionItem
{
public:
	EmotionItem() ;
	~EmotionItem() ;
	wstring GetBigEmotionPath() ;
	wstring GetCode() ;
public:
	int m_nType ;
	wstring m_strBigFile ;
	wstring m_strEmotionName ;
};

class EmotionManager
{
private:
	EmotionManager() ;
	~EmotionManager() ;
public:		
	bool LoadEmotionList() ;
	static EmotionManager* GetInstance() ;
	EmotionItem* SearchEmotionByCode( wstring strCode ) ;
	EmotionItem* SearchEmotionByBigFilePath( wstring strFileName ) ;
	void Release() ;
	map<int ,wstring> GetTypeList(){ return m_mTypeList; } ;
	vector<EmotionItem*> GetEmotionListByType( int nType ) ;
protected:
public:
	static EmotionManager* m_pEmotionManager ;
	vector<EmotionItem*> m_vEmotionList ;
	map<int ,wstring> m_mTypeList ;
};

#define EmotionMgr		EmotionManager::GetInstance()


class ShortCutMsgManager
{
private:
	ShortCutMsgManager() ;
	~ShortCutMsgManager() ;
public:
	static ShortCutMsgManager* GetInstance() ;
	bool LoadShortCutMsgList() ;
	void SaveShortCutMsgList() ;
	wstring GetShortCutMsgByIndex( int nIndex ) ;
	vector<wstring>& GetShortCutMsgList() ;	
	void UpdateShortCutMsgList(vector<wstring>& vShortCutList ) ;
	void Release() ;
private:
	static ShortCutMsgManager* m_pShortCutMsgManager ;
	vector<wstring > m_vShortCutList ;
};

#define ShortCutMsgMgr		ShortCutMsgManager::GetInstance()

class CaiZiItem
{
public:
	CaiZiItem() ;
	~CaiZiItem() ;
	int GetCaiZiIndex() ;
	wstring GetCaiZiHtml() ;
	wstring GetCaiZiName() ;
public:
	int m_nIndex ;
	wstring m_strName ;
	wstring m_strHtml ;
};

class CaiZiManager
{
private:
	CaiZiManager() ;
	~CaiZiManager() ;
public:
	static CaiZiManager* GetInstance() ;
	bool LoadCaiZiList() ;
	void Release() ;
	CaiZiItem* GetCaiZiItemByIndex( int nIndex ) ;
	vector<CaiZiItem*>& GetCaiZiList() ;
private:
	static CaiZiManager* m_pCaiZiManager ;
	vector<CaiZiItem* > m_vCaiZiList ;
};

#define CaiZiMgr		CaiZiManager::GetInstance()

class EnterManager
{
private:
	EnterManager() ;
	~EnterManager() ;
public:
	static EnterManager* GetInstance() ;
	bool LoadEnterList() ;
	void Release() ;
	CString GetEnterHtmlByVipLevel( int nVipLevel ) ;
private:
	static EnterManager* m_pEnterManager ;
	map< UINT32 , CString > m_mEnterList ;
};

#define EnterMgr       EnterManager::GetInstance() 

class WelcomeManager
{
private:
	WelcomeManager() ;
	~WelcomeManager() ;
public:
	static WelcomeManager* GetInstance() ;
	bool LoadWelcomeList() ;
	void Release() ;
	CString GetWelcomeRandom() ;
	CString GetFirstLoginUserWelcomeRandom();
	CString GetActorSpeakRandom();
	CString GetAgentSpeakRandom();
private:
	static WelcomeManager* m_pWelcomeManager ;
	vector<CString> m_vWelcomeList ;  
	vector<CString> m_vActorSpeak;
	vector<CString> m_vAgentSpeak;
};

#define WelcomeMgr       WelcomeManager::GetInstance() 


class VisitorManager
{
private:
	VisitorManager() ;
	~VisitorManager() ;
public:
	static VisitorManager* GetInstance() ;
	bool LoadVisitorList() ;
	void Release() ;
	CString GetVisitorRandom() ;
private:
	static VisitorManager* m_pVisitorManager ;
	vector<CString> m_vVisitorList ;	
};

#define VisitorMgr       VisitorManager::GetInstance()

struct list_effect_info 
{
	uint8 unIndex;
	std::wstring strImagePath;
	CTime tmInvalid;
	std::vector<uint32> vec179ID;
	list_effect_info() : unIndex(0), strImagePath(L""), tmInvalid(1970,1,1,12,0,0)
	{

	}

	~list_effect_info()
	{
		vec179ID.clear();
	}
};
class WealthyManager
{
private:
	WealthyManager() ;
	~WealthyManager() ;
public:
	static WealthyManager* GetInstance() ;
	bool LoadWealthyList() ;
	uint8 GetEffectIndex(uint32 un179ID);
	std::wstring GetWealthyImagePath(uint8 unIndex);
	void GetAllWealthyImg(std::map<uint8, std::wstring> & mapWealthyImgPath);
	void Release() ;	
private:
	static WealthyManager* m_pWealthyManager ;
	std::map<uint8, list_effect_info> m_mapListEffect;

};
#define WealthyMgr	WealthyManager::GetInstance()

class ImageManager
{
private:
	ImageManager();
	~ImageManager();

public:
	static ImageManager* getInstance()
	{
		if(s_pImageManager == NULL)
		{
			s_pImageManager = new ImageManager();
		}
		return s_pImageManager;
	}

	static void deleteInstance()
	{
		if(s_pImageManager != NULL)
		{
			delete s_pImageManager;
			s_pImageManager = NULL;
		}
	}

	void AddAllIcon(std::map<int, std::wstring>& mapIconPath);
	common::ui::ImageEx* GetIcon(int iconIndex);
	void AddStamp(int stampIndex, std::wstring strPath);
	common::ui::ImageEx* GetStamp(int stampIndex);
	void AddAllRichImg(std::map<uint8, std::wstring>& mapRichImgPath);
	common::ui::ImageEx* GetRichImg(uint8 unRichLevel);
	void AddAllWealthyImg(std::map<uint8, std::wstring>& mapWealthyImgPath);
	common::ui::ImageEx* GetWealthyImg(uint8 unWealthyIndex);

	void DrawAnimation();

private:

	static ImageManager* s_pImageManager;
	std::map<int, common::ui::ImageEx*> m_IconList;
	std::map<int, common::ui::ImageEx*> m_StampList;
	std::map<uint8, common::ui::ImageEx*> m_RichImgList;
	std::map<uint8, common::ui::ImageEx*> m_WealthyImgList;
};

struct event_enter_info 
{
	uint8 unIndex;
	CString strEnter;
	std::vector<uint32> vec179ID;
	event_enter_info() : unIndex(0), strEnter(L"")
	{

	}

	~event_enter_info()
	{
		vec179ID.clear();
	}
};

class EventEnterManager
{
private:
	EventEnterManager() ;
	~EventEnterManager() ;
public:
	static EventEnterManager* GetInstance() ;
	bool LoadEventEnterList() ;
	void Release() ;
	uint8 GetEventIndex(uint32 un179ID);
	CString GetEnterHtmlByIndex(uint8 unIndex);
private:
	static EventEnterManager* m_pEventEnterManager ;
	std::map<uint8, event_enter_info> m_mapEnterList;
};

#define EventEnterMgr       EventEnterManager::GetInstance() 